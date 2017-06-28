/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#include "ddr/ir/Symbol_IR.hpp"

#include <assert.h>
#include <algorithm>
#include <map>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include <vector>

#include "ddr/config.hpp"
#include "ddr/ir/ClassType.hpp"
#include "ddr/ir/ClassUDT.hpp"
#include "ddr/ir/EnumUDT.hpp"
#include "ddr/ir/EnumUDT.hpp"
#include "ddr/ir/NamespaceUDT.hpp"
#include "ddr/ir/NamespaceUDT.hpp"
#include "ddr/ir/TypedefUDT.hpp"
#include "ddr/ir/TypedefUDT.hpp"
#include "ddr/ir/UnionUDT.hpp"

#if defined(OMR_HAVE_TR1)
using std::tr1::unordered_map;
#else
using std::unordered_map;
#endif

using std::map;
using std::stack;

Symbol_IR::~Symbol_IR()
{
	for (size_t i = 0; i < _types.size(); i++) {
		if (NULL == _types[i]) {
			ERRMSG("Null member, cannot free");
		} else {
			delete(_types[i]);
		}
	}
	_types.clear();
}

DDR_RC
Symbol_IR::applyOverrideList(OMRPortLibrary *portLibrary, const char *overrideFiles)
{
	DDR_RC rc = DDR_RC_OK;
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	/* Read list of type override files from specified file. */
	intptr_t fd = omrfile_open(overrideFiles, EsOpenRead, 0);
	if (0 > fd) {
		ERRMSG("Failure attempting to open %s\nExiting...\n", overrideFiles);
		rc = DDR_RC_ERROR;
	} else {
		char *buff = NULL;
		int64_t offset = omrfile_seek(fd, 0, EsSeekEnd);
		if (-1 != offset) {
			buff = (char *)malloc(offset + 1);
			memset(buff, 0, offset + 1);
			omrfile_seek(fd, 0, EsSeekSet);

			/* Read each line as a file name, then open that file to get type overrides. */
			if (0 < omrfile_read(fd, buff, offset)) {
				char *line = buff;
				char *nextLine = strchr(line, '\n');
				nextLine[0] = '\0';
				while ((NULL != nextLine) && (DDR_RC_OK == rc)) {
					nextLine[0] = '\0';
					rc = applyOverrides(portLibrary, line);
					line = nextLine + 1;
					nextLine = strchr(line, '\n');
				}
			}
			free(buff);
		}
	}
	return rc;
}

DDR_RC
Symbol_IR::applyOverrides(OMRPortLibrary *portLibrary, const char *overrideFile)
{
	DDR_RC rc = DDR_RC_OK;
	OMRPORT_ACCESS_FROM_OMRPORT(portLibrary);

	/* Read list of type overrides from specified file. */
	vector<FieldOverride> overrideList;
	intptr_t fd = omrfile_open(overrideFile, EsOpenRead, 0);
	if (0 > fd) {
		ERRMSG("Failure attempting to open %s\nExiting...\n", overrideFile);
		rc = DDR_RC_ERROR;
	} else {
		char *buff = NULL;
		int64_t offset = omrfile_seek(fd, 0, EsSeekEnd);
		if (-1 != offset) {
			buff = (char *)malloc(offset + 1);
			memset(buff, 0, offset + 1);
			omrfile_seek(fd, 0, EsSeekSet);

			/* Read each line, expecting format of "[typeoverride/fieldoverride].[StructureName].[fieldName]=[overrideType]" */
			if (0 < omrfile_read(fd, buff, offset)) {
				char *nextLine = strtok(buff, "\n");
				while (NULL != nextLine) {
					string line = nextLine;

					/* Remove comment portion of the line, if present. */
					size_t commentPosition = line.find("#");
					if (string::npos != commentPosition) {
						line = line.substr(0, commentPosition);
					}
					line.erase(line.find_last_not_of(" \t") + 1);

					/* Ignore the prefix "ddrblob.", if present. */
					string prefix = "ddrblob.";
					if (string::npos != line.find(prefix)) {
						line = line.substr(prefix.length(), line.length());
					}

					/* Check if the override is a typeoverride or a fieldoverride. */
					string typeoverride = "typeoverride.";
					string fieldoverride = "fieldoverride.";
					bool isTypeOverride = false;
					if (string::npos != line.find(typeoverride)) {
						isTypeOverride = true;
						line = line.substr(typeoverride.length(), line.length());
					} else if (string::npos != line.find(fieldoverride)) {
						line = line.substr(fieldoverride.length(), line.length());
					}

					/* Remove any pointer or array specifiers in the line and use those the field has. */
					line.erase(remove(line.begin(), line.end(), '*'), line.end());
					line.erase(remove(line.begin(), line.end(), '['), line.end());
					line.erase(remove(line.begin(), line.end(), ']'), line.end());

					/* Correctly formatted lines must have exactly 1 dot and 1 equals. */
					size_t dotCount = count(line.begin(), line.end(), '.');
					size_t equalCount = count(line.begin(), line.end(), '=');
					if ((1 == dotCount) && (1 == equalCount)) {
						/* Get the structure name, field name, and override name. Add to list to process. */
						size_t dotPosition = line.find(".");
						size_t equalsPosition = line.find("=", dotPosition);
						FieldOverride to = {
							line.substr(0, dotPosition),
							line.substr(dotPosition + 1, equalsPosition - dotPosition - 1),
							line.substr(equalsPosition + 1, line.length() - equalsPosition - 1),
							isTypeOverride
						};
						overrideList.push_back(to);
						if (DDR_RC_OK != rc) {
							break;
						}
					}
					nextLine = strtok(NULL, "\n");
				}
			}
			free(buff);
		}
	}

	/* Create a map of type name to vector of types to check all types
	 * by name for type overrides.
	 */
	map<string, vector<Type *> > typeNames;
	for (vector<Type *>::iterator it = _types.begin(); it != _types.end(); it += 1) {
		Type *type = *it;
		typeNames[type->_name].push_back(type);
	}

	/* Apply the type overrides. */
	for (vector<FieldOverride>::iterator it = overrideList.begin(); it != overrideList.end(); it += 1) {
		FieldOverride type = *it;
		/* Check if the structure to override exists. */
		if (typeNames.find(type.structName) != typeNames.end()) {
			Type *replacementType = NULL;
			if (type.isTypeOverride) {
				/* If the type for the override exists in the IR, use it. Otherwise, create it. */
				if (typeNames.find(type.overrideName) == typeNames.end()) {
					replacementType = new Type(0);
					replacementType->_name = type.overrideName;
					typeNames[replacementType->_name].push_back(replacementType);
				} else {
					replacementType = typeNames[type.overrideName].front();
				}
			}

			/* Iterate over the types with a matching name for the override. */
			vector<Type *> *typesWithName = &typeNames[type.structName];
			for (vector<Type *>::iterator it2 = typesWithName->begin(); it2 != typesWithName->end(); it2 += 1) {
				(*it2)->renameFieldsAndMacros(type, replacementType);
			}
		}
	}

	return rc;
}

/* Compute the field offsets for all types in the IR from the sizes of the fields. */
void
Symbol_IR::computeOffsets()
{
	/* For each Type in the ir, compute the field offsets from the size of each field. */
	for (size_t i = 0; i < _types.size(); i++) {
		_types[i]->computeFieldOffsets();
	}
}

void
Symbol_IR::removeDuplicates()
{
	for (vector<Type *>::iterator it = _types.begin(); it != _types.end(); ++it) {
		(*it)->checkDuplicate(this);
	}
}

class MergeVisitor : public TypeVisitor
{
private:
	Symbol_IR *_ir;
	Type *_other;
	vector<Type *> *_merged;

public:
	MergeVisitor(Symbol_IR *ir, Type *other, vector<Type *> *merged) : _ir(ir), _other(other), _merged(merged) {}

	DDR_RC visitType(Type *type) const;
	DDR_RC visitType(NamespaceUDT *type) const;
	DDR_RC visitType(EnumUDT *type) const;
	DDR_RC visitType(TypedefUDT *type) const;
	DDR_RC visitType(ClassUDT *type) const;
	DDR_RC visitType(UnionUDT *type) const;
};

DDR_RC
MergeVisitor::visitType(Type *type) const
{
	/* No merging necessary for base types. */
	return DDR_RC_OK;
}

DDR_RC
MergeVisitor::visitType(NamespaceUDT *type) const
{
	/* Merge by adding the fields/subtypes of '_other' into 'type'. */
	NamespaceUDT *other = (NamespaceUDT *)_other;
	bool currentlyHasAnonymousSubtypes = false;
	for (vector<UDT *>::iterator it = type->getSubUDTS()->begin(); it != type->getSubUDTS()->end(); ++it) {
		if ((*it)->_name.empty()) {
			currentlyHasAnonymousSubtypes = true;
			break;
		}
	}
	_ir->mergeTypes(type->getSubUDTS(), other->getSubUDTS(), !currentlyHasAnonymousSubtypes, type, _merged);
	return DDR_RC_OK;
}

DDR_RC
MergeVisitor::visitType(EnumUDT *type) const
{
	/* Merge by adding the fields/subtypes of '_other' into 'type'. */
	EnumUDT *other = (EnumUDT *)_other;
	_ir->mergeEnums(&type->_enumMembers, &other->_enumMembers);
	return DDR_RC_OK;
}

DDR_RC
MergeVisitor::visitType(TypedefUDT *type) const
{
	/* No merging necessary for typedefs. */
	return DDR_RC_OK;
}

DDR_RC
MergeVisitor::visitType(ClassUDT *type) const
{
	/* Merge by adding the fields/subtypes of '_other' into 'type'. */
	ClassUDT *other = (ClassUDT *)_other;
	bool currentlyHasAnonymousSubtypes = false;
	for (vector<UDT *>::iterator it = type->getSubUDTS()->begin(); it != type->getSubUDTS()->end(); ++it) {
		if ((*it)->_name.empty()) {
			currentlyHasAnonymousSubtypes = true;
			break;
		}
	}
	_ir->mergeTypes(type->getSubUDTS(), other->getSubUDTS(), !currentlyHasAnonymousSubtypes, type, _merged);
	_ir->mergeFields(&type->_fieldMembers, &other->_fieldMembers, type, _merged);
	_ir->mergeEnums(&type->_enumMembers, &other->_enumMembers);
	return DDR_RC_OK;
}

DDR_RC
MergeVisitor::visitType(UnionUDT *type) const
{
	/* Merge by adding the fields/subtypes of '_other' into 'type'. */
	UnionUDT *other = (UnionUDT *)_other;
	bool currentlyHasAnonymousSubtypes = false;
	for (vector<UDT *>::iterator it = type->getSubUDTS()->begin(); it != type->getSubUDTS()->end(); ++it) {
		if ((*it)->_name.empty()) {
			currentlyHasAnonymousSubtypes = true;
			break;
		}
	}
	_ir->mergeTypes(type->getSubUDTS(), other->getSubUDTS(), !currentlyHasAnonymousSubtypes, type, _merged);
	_ir->mergeFields(&type->_fieldMembers, &other->_fieldMembers, type, _merged);
	_ir->mergeEnums(&type->_enumMembers, &other->_enumMembers);
	return DDR_RC_OK;
}

template<typename T> void
Symbol_IR::mergeTypes(vector<T *> *source, vector<T *> *other, bool mergeAnonymous,
	NamespaceUDT *outerNamespace, vector<Type *> *merged)
{
	// of the same class of the merged type. Broken for typedefs with same name as class.. why is that so anyway?
	/* Create a map of all types in the source list. */
	/* Types are matched by full name. */
	unordered_map<string, Type *> typeMapName;
	for (typename vector<T *>::iterator it = source->begin(); it != source->end(); ++it) {
		Type *t = (Type *)(*it);
		string nameKey = t->getFullName();
		if (NULL == t->getBaseType()) {
			nameKey = t->getSymbolKindName() + nameKey;
		}
		if (!t->_name.empty()) {
			typeMapName[nameKey] = t;
		}
	}
	for (typename vector<T *>::iterator it = other->begin(); it != other->end();) {
		Type *t = (Type *)(*it);
		bool moved = false;
		string nameKey = t->getFullName();
		if (NULL == t->getBaseType()) {
			nameKey = t->getSymbolKindName() + nameKey;
		}

		/* Types in the other list not in the source list are added. */
		/* Types in both lists are merged. */
		if (!t->_name.empty()) {
			if (typeMapName.end() == typeMapName.find(nameKey)) {
				moved = true;
				typeMapName[nameKey] = t;
			} else {
				Type *original = typeMapName.find(nameKey)->second;
				original->acceptVisitor(MergeVisitor(this, t, merged));
			}
		} else if (mergeAnonymous) {
			/* Anonymous subtypes within one scan must be all or nothing.
			 * If a type has anonymous subtypes already, do not add/merge more.
			 */
			moved = true;
		}
		if (moved) {
			source->push_back((T *)t);
			merged->push_back(t);
			it = other->erase(it);
			if (NULL != outerNamespace) {
				((UDT *)t)->_outerNamespace = outerNamespace;
			}
		} else {
			++ it;
		}
	}
}

void
Symbol_IR::mergeFields(vector<Field *> *source, vector<Field *> *other, Type *type, vector<Type *> *merged)
{
	/* Create a map of all fields in the source list. */
	set<string> fieldNames;
	for (vector<Field *>::iterator it = source->begin(); it != source->end(); ++it) {
		fieldNames.insert((*it)->_name);
	}
	for (vector<Field *>::iterator it = other->begin(); it != other->end();) {
		/* Fields in the other list not in the source list are added. */
		if (fieldNames.end() == fieldNames.find((*it)->_name)) {
			source->push_back(*it);
			merged->push_back(type);
			fieldNames.insert((*it)->_name);
			it = other->erase(it);
		} else {
			++ it;
		}
	}
}

void
Symbol_IR::mergeEnums(vector<EnumMember *> *source, vector<EnumMember *> *other)
{
	/* Create a map of all fields in the source list. */
	set<string> enumNames;
	for (vector<EnumMember *>::iterator it = source->begin(); it != source->end(); ++it) {
		enumNames.insert((*it)->_name);
	}
	for (vector<EnumMember *>::iterator it = other->begin(); it != other->end();) {
		/* Fields in the other list not in the source list are added. */
		if (enumNames.end() == enumNames.find((*it)->_name)) {
			source->push_back(*it);
			enumNames.insert((*it)->_name);
			it = other->erase(it);
		} else {
			++ it;
		}
	}
}

class TypeReplaceVisitor : public TypeVisitor
{
private:
	unordered_map<string, Type *> *_typeMap;
	set<Type *> *_typeSet;

public:
	TypeReplaceVisitor(unordered_map<string, Type *> *typeMap, set<Type *> *typeSet)
		: _typeMap(typeMap), _typeSet(typeSet) {}

	DDR_RC visitType(Type *type) const;
	DDR_RC visitType(NamespaceUDT *type) const;
	DDR_RC visitType(EnumUDT *type) const;
	DDR_RC visitType(TypedefUDT *type) const;
	DDR_RC visitType(ClassUDT *type) const;
	DDR_RC visitType(UnionUDT *type) const;
};

DDR_RC
TypeReplaceVisitor::visitType(Type *type) const
{
	return DDR_RC_OK;
}

DDR_RC
TypeReplaceVisitor::visitType(NamespaceUDT *type) const
{
	DDR_RC rc = DDR_RC_OK;
	for (vector<UDT *>::iterator it = type->_subUDTs.begin(); it != type->_subUDTs.end(); ++it) {
		rc = (*it)->acceptVisitor(TypeReplaceVisitor(_typeMap, _typeSet));
	}
	return rc;
}

DDR_RC
TypeReplaceVisitor::visitType(EnumUDT *type) const
{
	return DDR_RC_OK;
}

DDR_RC
TypeReplaceVisitor::visitType(TypedefUDT *type) const
{
	DDR_RC rc = DDR_RC_OK;
	if ((NULL != type->_aliasedType) && (!type->_aliasedType->_name.empty())) {
		if (_typeSet->end() == _typeSet->find(type->_aliasedType)) {
			string nameKey = type->_aliasedType->getFullName();
			if (NULL == type->_aliasedType->getBaseType()) {
				nameKey = type->_aliasedType->getSymbolKindName() + nameKey;
			}

			if (_typeMap->end() == _typeMap->find(nameKey)) {
				ERRMSG("Error replacing aliased type. Type not found, but was expected to be in IR after merge: Typedef '%s' of type '%s'.",
					type->getFullName().c_str(), type->_aliasedType->getFullName().c_str());
				rc = DDR_RC_ERROR;
			} else {
				type->_aliasedType = _typeMap->find(nameKey)->second;
			}
		}
	}
	return rc;
}

DDR_RC
TypeReplaceVisitor::visitType(ClassUDT *type) const
{
	DDR_RC rc = DDR_RC_OK;
	for (vector<UDT *>::iterator it = type->_subUDTs.begin(); it != type->_subUDTs.end(); ++it) {
		(*it)->acceptVisitor(TypeReplaceVisitor(_typeMap, _typeSet));
	}
	for (vector<Field *>::iterator it = type->_fieldMembers.begin(); it != type->_fieldMembers.end(); ++it) {
		if ((NULL != (*it)->_fieldType) && (!(*it)->_fieldType->_name.empty())) {
			if (_typeSet->end() == _typeSet->find((*it)->_fieldType)) {
				string nameKey = (*it)->_fieldType->getFullName();
				if (NULL == (*it)->_fieldType->getBaseType()) {
					nameKey = (*it)->_fieldType->getSymbolKindName() + nameKey;
				}

				if (_typeMap->end() == _typeMap->find(nameKey)) {
					ERRMSG("Error replacing field type. Type not found, but was expected to be in IR after merge: Type '%s', field '%s', type '%s'.",
						type->getFullName().c_str(), (*it)->_name.c_str(), (*it)->_fieldType->getFullName().c_str());
					rc = DDR_RC_ERROR;
				} else {
					(*it)->_fieldType = _typeMap->find(nameKey)->second;
				}
			}
		}
	}

	/* Replace super class too. */
	if ((NULL != type->_superClass) && (!type->_superClass->_name.empty())) {
		if (_typeSet->end() == _typeSet->find(type->_superClass)) {
			string nameKey = type->_superClass->getFullName();
			if (NULL == type->_superClass->getBaseType()) {
				nameKey = type->_superClass->getSymbolKindName() + nameKey;
			}

			if (_typeMap->end() == _typeMap->find(nameKey)) {
				ERRMSG("Error replacing superclass type. Type not found, but was expected to be in IR after merge: Type '%s', superclass '%s'.",
					type->getFullName().c_str(), type->_superClass->_name.c_str());
				rc = DDR_RC_ERROR;
			} else {
				type->_superClass = (ClassUDT *)_typeMap->find(nameKey)->second;
			}
		}
	}
	return rc;
}

DDR_RC
TypeReplaceVisitor::visitType(UnionUDT *type) const
{
	DDR_RC rc = DDR_RC_OK;
	for (vector<UDT *>::iterator it = type->_subUDTs.begin(); it != type->_subUDTs.end(); ++it) {
		(*it)->acceptVisitor(TypeReplaceVisitor(_typeMap, _typeSet));
	}
	for (vector<Field *>::iterator it = type->_fieldMembers.begin(); it != type->_fieldMembers.end(); ++it) {
		if ((NULL != (*it)->_fieldType) && (!(*it)->_fieldType->_name.empty())) {
			if (_typeSet->end() == _typeSet->find((*it)->_fieldType)) {
				string nameKey = (*it)->_fieldType->getFullName();
				if (NULL == (*it)->_fieldType->getBaseType()) {
					nameKey = (*it)->_fieldType->getSymbolKindName() + nameKey;
				}

				if (_typeMap->end() == _typeMap->find(nameKey)) {
					ERRMSG("Error replacing field type. Type not found, but was expected to be in IR after merge: Type '%s', field '%s', type '%s'.",
						type->getFullName().c_str(), (*it)->_name.c_str(), (*it)->_fieldType->getFullName().c_str());
					rc = DDR_RC_ERROR;
				} else {
					(*it)->_fieldType = _typeMap->find(nameKey)->second;
				}
			}
		}
	}
	return rc;
}

DDR_RC
Symbol_IR::mergeIR(Symbol_IR *other)
{
	DDR_RC rc = DDR_RC_OK;
	vector<Type *> mergedTypes;
	mergeTypes(&_types, &other->_types, true, NULL, &mergedTypes);

	/* Create map of this source IR of type full name to type. Use it to replace all
	 * pointers in types acquired from other IRs. The problem of encountering
	 * types to replace that are not yet in this IR is solved by doing all of the type
	 * replacement after all types are merged.
	 */
	unordered_map<string, Type *> typeMap;
	set<Type *> typeSet;
	for (vector<Type *>::iterator it = _types.begin(); it != _types.end(); ++it) {
		Type *t = *it;
		stack<vector<UDT *>::iterator> itStack;
		if (NULL != t->getSubUDTS()) {
			itStack.push(t->getSubUDTS()->begin());
		}

		while (NULL != t) {
			string nameKey = t->getFullName();
			if (NULL == t->getBaseType()) {
				nameKey = t->getSymbolKindName() + nameKey;
			}

			typeMap[nameKey] = t;
			typeSet.insert(t);
			if (NULL == t->getSubUDTS() || t->getSubUDTS()->empty()) {
				t = t->getNamespace();
			} else if (t->getSubUDTS()->end() == itStack.top()) {
				t = t->getNamespace();
				itStack.pop();
			} else {
				t = *itStack.top();
				itStack.top() ++;
				if (NULL != t->getSubUDTS() && !t->getSubUDTS()->empty()) {
					itStack.push(t->getSubUDTS()->begin());
				}
			}
		};
	}

	for (vector<Type *>::iterator it = mergedTypes.begin(); it != mergedTypes.end(); ++it) {
		rc = (*it)->acceptVisitor(TypeReplaceVisitor(&typeMap, &typeSet));
		if (DDR_RC_OK != rc) {
			break;
		}
	}
	return rc;
}

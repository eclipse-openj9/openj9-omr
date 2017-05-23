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
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "ddr/config.hpp"
#include "ddr/ir/ClassType.hpp"
#include "ddr/ir/ClassUDT.hpp"
#include "ddr/ir/EnumUDT.hpp"
#include "ddr/ir/EnumUDT.hpp"
#include "ddr/ir/Field.hpp"
#include "ddr/ir/NamespaceUDT.hpp"
#include "ddr/ir/NamespaceUDT.hpp"
#include "ddr/ir/TypedefUDT.hpp"
#include "ddr/ir/TypedefUDT.hpp"
#include "ddr/ir/UnionUDT.hpp"

using std::map;

string
Symbol_IR::getUDTname(Type *type)
{
	UDT *udt = dynamic_cast<UDT *>(type);
	assert(NULL != udt);

	string name;
	if (NULL != udt->_outerUDT) {
		name = getUDTname(udt->_outerUDT) + udt->_name;
	} else {
		name = udt->_name;
	}

	return name;
}

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

typedef struct {
	string structName;
	string fieldName;
	string overrideName;
	bool isTypeOverride;
} FieldOverride;


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
					replacementType = new Type(BASE, 0);
					replacementType->_name = type.overrideName;
					typeNames[replacementType->_name].push_back(replacementType);
				} else {
					replacementType = typeNames[type.overrideName].front();
				}
			}

			/* Iterate over the types with a matching name for the override. */
			vector<Type *> *typesWithName = &typeNames[type.structName];
			for (vector<Type *>::iterator it2 = typesWithName->begin(); it2 != typesWithName->end(); it2 += 1) {
				ClassType *ct = dynamic_cast<ClassType *>(*it2);
				/* Iterate the fields of structures with matching names. */
				if (NULL != ct) {
					for (vector<Field *>::iterator it3 = ct->_fieldMembers.begin(); it3 != ct->_fieldMembers.end(); it3 += 1) {
						/* Once a matching structure and field name are found, apply the override. */
						if ((*it3)->_name == type.fieldName) {
							if (type.isTypeOverride) {
								if (0 == replacementType->_sizeOf) {
									replacementType->_sizeOf = (*it3)->_fieldType->_sizeOf;
								}
								(*it3)->_fieldType = replacementType;
							} else {
								(*it3)->_name = type.overrideName;
							}
						}
					}
				}
				NamespaceUDT *ns = dynamic_cast<NamespaceUDT *>(*it2);
				/* Iterate the macros as well for field name overrides. */
				if (!type.isTypeOverride) {
					if (NULL != ns) {
						for (vector<Macro>::iterator it3 = ns->_macros.begin(); it3 != ns->_macros.end(); it3 += 1) {
							if (it3->_name == type.fieldName) {
								it3->_name = type.overrideName;
							}
						}
					}
				}
			}
		}
	}

	return rc;
}

/* Compute the field offsets for all types in the IR from the sizes of the fields. */
DDR_RC
Symbol_IR::computeOffsets()
{
	DDR_RC rc = DDR_RC_OK;
	/* For each Type in the ir, compute the field offsets from the size of each field. */
	for (size_t i = 0; i < _types.size(); i++) {
		rc = computeFieldOffsets(_types[i]);
		if (DDR_RC_OK != rc) {
			break;
		}
	}
	return rc;
}

DDR_RC
Symbol_IR::computeFieldOffsets(Type *type)
{
	DDR_RC rc = DDR_RC_OK;
	NamespaceUDT *ns = dynamic_cast<NamespaceUDT *>(type);
	if (NULL != ns) {
		for (size_t i = 0; i < ns->_subUDTs.size(); i += 1) {
			rc = computeFieldOffsets(ns->_subUDTs[i]);
			if (DDR_RC_OK != rc) {
				break;
			}
		}
		/* For classes, structs, and unions, find the field offsets. */
		ClassType *ct = dynamic_cast<ClassType *>(ns);
		if ((NULL != ct) && (DDR_RC_OK == rc)) {
			size_t offset = 0;
			for (size_t i = 0; i < ct->_fieldMembers.size(); i += 1) {
				Field *field = (ct->_fieldMembers[i]);

				if (!field->_isStatic) {
					/* Use the field size to compute offsets. */
					field->_offset = offset;
					offset += field->_sizeOf;
				}
			}
			/* If class has no total size, set it now. */
			if (0 == ct->_sizeOf) {
				ct->_sizeOf = offset;
			}
		}
	}
	return rc;
}

DDR_RC
Symbol_IR::removeDuplicates()
{
	DDR_RC rc = DDR_RC_OK;
	for (vector<Type *>::iterator it = _types.begin(); it != _types.end(); ++it) {
		rc = (*it)->checkDuplicate(this);
		if (DDR_RC_OK != rc) {
			break;
		}
	}
	return rc;
}

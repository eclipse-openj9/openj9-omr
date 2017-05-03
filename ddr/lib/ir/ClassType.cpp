/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016
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

#include "ddr/ir/ClassType.hpp"

#include "ddr/config.hpp"

ClassType::ClassType(SymbolType symbolType, size_t size, unsigned int lineNumber)
	: NamespaceUDT(lineNumber)
{
	_symbolType = symbolType;
	_sizeOf = size;
}

ClassType::~ClassType()
{
	/*enum members may be added to a classType in the case of anonymous enums */
	for (size_t i = 0; i < _enumMembers.size(); i++) {
		if (NULL == _enumMembers[i]) {
			ERRMSG("Null member, cannot free");
		} else {
			delete(_enumMembers[i]);
		}
	}
	_enumMembers.clear();

	for (size_t i = 0; i < _fieldMembers.size(); i++) {
		if (NULL == _fieldMembers[i]) {
			ERRMSG("Null member, cannot free");
		} else {
			delete(_fieldMembers[i]);
		}
	}
	_fieldMembers.clear();
}

bool
ClassType::isAnonymousType()
{
	return _name.empty();
}

bool
ClassType::equal(Type const& type, set<Type const*> *checked) const
{
	bool ret = false;
	if (checked->find(this) != checked->end()) {
		ret = true;
	} else {
		checked->insert(this);
		ClassType const *classType = dynamic_cast<ClassType const *>(&type);
		if (NULL != classType) {
			bool fieldsEqual = _fieldMembers.size() == classType->_fieldMembers.size();
			if (fieldsEqual) {
				for (size_t i = 0; i < _fieldMembers.size(); i += 1) {
					Field *field = _fieldMembers[i];
					Field *fieldCompare = classType->_fieldMembers[i];
					if ((field->_fieldType != fieldCompare->_fieldType && !(*field->_fieldType == *fieldCompare->_fieldType))
						|| (field->_offset != fieldCompare->_offset)
						|| !(field->_modifiers == fieldCompare->_modifiers)
						|| (field->_sizeOf != fieldCompare->_sizeOf)
						|| (field->_bitField != fieldCompare->_bitField)
					) {
						fieldsEqual = false;
						break;
					}
				}
			}

			bool enumMembersEqual = _enumMembers.size() == classType->_enumMembers.size() && fieldsEqual;
			if (enumMembersEqual) {
				for (size_t i = 0; i < _enumMembers.size(); i += 1) {
					if ((_enumMembers[i]->_name != classType->_enumMembers[i]->_name)
						|| (_enumMembers[i]->_value != classType->_enumMembers[i]->_value)
					) {
						enumMembersEqual = false;
						break;
					}
				}
			}
			ret = (NamespaceUDT::equal(type, checked))
				&& (fieldsEqual)
				&& (enumMembersEqual);
		}
	}
	return ret;
}

void
ClassType::replaceType(Type *typeToReplace, Type *replaceWith)
{
	NamespaceUDT::replaceType(typeToReplace, replaceWith);
	for (size_t i = 0; i < _fieldMembers.size(); i += 1) {
		if (_fieldMembers[i]->_fieldType == typeToReplace) {
			_fieldMembers[i]->_fieldType = replaceWith;
		}
	}
}

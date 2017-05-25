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

ClassType::ClassType(SymbolKind symbolKind, size_t size, unsigned int lineNumber)
	: NamespaceUDT(lineNumber)
{
	_symbolKind = symbolKind;
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

void
ClassType::computeFieldOffsets()
{
	NamespaceUDT::computeFieldOffsets();
	
	/* For classes, structs, and unions, find the field offsets. */
	size_t offset = 0;
	for (size_t i = 0; i < _fieldMembers.size(); i += 1) {
		Field *field = (_fieldMembers[i]);

		if (!field->_isStatic) {
			/* Use the field size to compute offsets. */
			field->_offset = offset;
			offset += field->_sizeOf;
		}
	}
	/* If class has no total size, set it now. */
	if (0 == _sizeOf) {
		_sizeOf = offset;
	}
}

void
ClassType::renameFieldsAndMacros(FieldOverride fieldOverride, Type *replacementType)
{
	/* Iterate the fields of structures with matching names. */
	for (vector<Field *>::iterator it = _fieldMembers.begin(); it != _fieldMembers.end(); it += 1) {
		/* Once a matching structure and field name are found, apply the override. */
		if ((*it)->_name == fieldOverride.fieldName) {
			if (fieldOverride.isTypeOverride) {
				if (0 == replacementType->_sizeOf) {
					replacementType->_sizeOf = (*it)->_fieldType->_sizeOf;
				}
				(*it)->_fieldType = replacementType;
			} else {
				(*it)->_name = fieldOverride.overrideName;
			}
		}
	}
}


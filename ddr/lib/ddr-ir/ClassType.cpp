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

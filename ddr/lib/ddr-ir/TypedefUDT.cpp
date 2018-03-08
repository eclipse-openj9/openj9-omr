/*******************************************************************************
 * Copyright (c) 2016, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "ddr/ir/TypedefUDT.hpp"

TypedefUDT::TypedefUDT(unsigned int lineNumber)
	: UDT(0, lineNumber), _aliasedType(NULL)
{
}

TypedefUDT::~TypedefUDT()
{
}

DDR_RC
TypedefUDT::acceptVisitor(const TypeVisitor &visitor)
{
	return visitor.visitTypedef(this);
}

const string &
TypedefUDT::getSymbolKindName() const
{
	static const string typedefKind("");

	return typedefKind;
}

size_t
TypedefUDT::getPointerCount()
{
	size_t count = _modifiers._pointerCount;
	if (NULL != _aliasedType) {
		count += _aliasedType->getPointerCount();
	}
	return count;
}

size_t
TypedefUDT::getArrayDimensions()
{
	size_t count = _modifiers.getArrayDimensions();
	if (NULL != _aliasedType) {
		count += _aliasedType->getArrayDimensions();
	}
	return count;
}

Type *
TypedefUDT::getBaseType()
{
	return _aliasedType;
}

bool
TypedefUDT::operator==(const Type & rhs) const
{
	return rhs.compareToTypedef(*this);
}

bool
TypedefUDT::compareToTypedef(const TypedefUDT &other) const
{
	return compareToType(other)
		&& *_aliasedType == *other._aliasedType
		&& _modifiers == other._modifiers;
}

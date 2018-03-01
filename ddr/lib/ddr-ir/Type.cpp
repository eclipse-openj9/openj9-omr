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

#include "ddr/ir/Type.hpp"

Type::Type(size_t size)
	: _blacklisted(false)
	, _name()
	, _sizeOf(size)
{
}

Type::~Type()
{
}

bool
Type::isAnonymousType() const
{
	return _name.empty();
}

string
Type::getFullName()
{
	return _name;
}

const string &
Type::getSymbolKindName() const
{
	static const string typeKind("");

	return typeKind;
}

DDR_RC
Type::acceptVisitor(const TypeVisitor &visitor)
{
	return visitor.visitType(this);
}

bool
Type::insertUnique(Symbol_IR *ir)
{
	/* No-op: since Types aren't printed, there's no need to check if they're duplicates either. */
	return false;
}

NamespaceUDT *
Type::getNamespace()
{
	return NULL;
}

size_t
Type::getPointerCount()
{
	return 0;
}

size_t
Type::getArrayDimensions()
{
	return 0;
}

void
Type::addMacro(Macro *macro)
{
	/* No-op: macros cannot be associated with base types. */
}

vector<UDT *> *
Type::getSubUDTS()
{
	return NULL;
}

void
Type::renameFieldsAndMacros(const FieldOverride &fieldOverride, Type *replacementType)
{
	/* No-op: base types have no fields. */
}

Type *
Type::getBaseType()
{
	return NULL;
}

bool
Type::operator==(const Type & rhs) const
{
	return rhs.compareToType(*this);
}

bool
Type::compareToType(const Type &other) const
{
	return (_name == other._name)
		&& ((0 == _sizeOf) || (0 == other._sizeOf) || (_sizeOf == other._sizeOf));
}

bool
Type::compareToUDT(const UDT &) const
{
	return false;
}

bool
Type::compareToNamespace(const NamespaceUDT &) const
{
	return false;
}

bool
Type::compareToEnum(const EnumUDT &) const
{
	return false;
}

bool
Type::compareToTypedef(const TypedefUDT &) const
{
	return false;
}

bool
Type::compareToClasstype(const ClassType &) const
{
	return false;
}

bool
Type::compareToUnion(const UnionUDT &) const
{
	return false;
}

bool
Type::compareToClass(const ClassUDT &) const
{
	return false;
}

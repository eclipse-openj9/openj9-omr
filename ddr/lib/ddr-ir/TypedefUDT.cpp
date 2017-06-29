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

#include "ddr/ir/TypedefUDT.hpp"

TypedefUDT::TypedefUDT(unsigned int lineNumber)
	: UDT(0, lineNumber), _aliasedType(NULL)
{
}

TypedefUDT::~TypedefUDT()
{
}

DDR_RC
TypedefUDT::acceptVisitor(TypeVisitor const &visitor)
{
	return visitor.visitType(this);
}

void
TypedefUDT::checkDuplicate(Symbol_IR *ir)
{
	/* No-op: since TypedefUDTs aren't printed, there's no need to check if they're duplicates either */
}

string
TypedefUDT::getSymbolKindName()
{
	if (NULL == _aliasedType) return "";
	return _aliasedType->getSymbolKindName();
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
TypedefUDT::operator==(Type const & rhs) const
{
	return rhs.compareToTypedef(*this);
}

bool
TypedefUDT::compareToTypedef(TypedefUDT const &other) const
{
	return compareToType(other)
		&& *_aliasedType == *other._aliasedType
		&& _modifiers == other._modifiers;
}

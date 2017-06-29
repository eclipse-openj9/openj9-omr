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

#include "ddr/ir/UnionUDT.hpp"

UnionUDT::UnionUDT(size_t size, unsigned int lineNumber)
	: ClassType(size, lineNumber)
{
}

UnionUDT::~UnionUDT() {};

DDR_RC
UnionUDT::acceptVisitor(TypeVisitor const &visitor)
{
	return visitor.visitType(this);
}

string
UnionUDT::getSymbolKindName()
{
	return "union";
}

bool
UnionUDT::operator==(Type const & rhs) const
{
	return rhs.compareToUnion(*this);
}

bool
UnionUDT::compareToUnion(UnionUDT const &other) const
{
	return compareToClasstype(other);
}

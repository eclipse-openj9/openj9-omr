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

#include "ddr/ir/Type.hpp"

Type::Type(SymbolKind symbolKind, size_t size)
	: _symbolKind(symbolKind), _sizeOf(size), _isDuplicate(false)
{
}

Type::~Type() {}

SymbolKind
Type::getSymbolKind()
{
	return _symbolKind;
}

bool
Type::isAnonymousType()
{
	return false;
}

string
Type::getFullName()
{
	return _name;
}

string
Type::getSymbolKindName()
{
	return "";
}

DDR_RC
Type::acceptVisitor(TypeVisitor const &visitor)
{
	return visitor.visitType(this);
}

void
Type::checkDuplicate(Symbol_IR *ir)
{
	/* No-op: since Types aren't printed, there's no need to check if they're duplicates either */
}

NamespaceUDT *
Type::getNamespace()
{
	return NULL;
}

int
Type::getPointerCount()
{
	return 0;
}

void
Type::computeFieldOffsets()
{
	/* No-op: base types have no fields. */
}

void
Type::addMacro(Macro *macro)
{
	/* No-op: macros cannot be associated with base types. */
}

std::vector<UDT *> *
Type::getSubUDTS()
{
	return NULL;
}

void
Type::renameFieldsAndMacros(FieldOverride fieldOverride, Type *replacementType)
{
	/* No-op: base types have no fields. */
}
	
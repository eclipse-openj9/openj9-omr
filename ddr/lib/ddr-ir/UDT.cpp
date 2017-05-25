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

#include "ddr/config.hpp"
#include "ddr/ir/UDT.hpp"
#include "ddr/ir/NamespaceUDT.hpp"

UDT::UDT(SymbolKind symbolKind, size_t size, unsigned int lineNumber)
	: Type(symbolKind, size), _outerNamespace(NULL), _lineNumber(lineNumber)
{
}

UDT::~UDT()
{
}

string
UDT::getFullName()
{
	return ((NULL == _outerNamespace) ? "" : _outerNamespace->getFullName() + "::") + _name;
}


void
UDT::checkDuplicate(Symbol_IR *ir)
{
	if (!this->isAnonymousType()) {
		string fullName = getFullName();
		if (ir->_fullTypeNames.end() != ir->_fullTypeNames.find(fullName)) {
			this->_isDuplicate = true;
		} else {
			ir->_fullTypeNames.insert(fullName);
		}
	}
}

NamespaceUDT *
UDT::getNamespace()
{
	return _outerNamespace;
}
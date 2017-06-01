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

#ifndef TYPEDEFUDT_HPP
#define TYPEDEFUDT_HPP

#include <string>

#include "Modifiers.hpp"
#include "ddr/ir/UDT.hpp"

class TypedefUDT : public UDT
{
public:
	Type *_aliasedType;
	Modifiers _modifiers;

	TypedefUDT(unsigned int lineNumber = 0);
	~TypedefUDT();

	virtual DDR_RC acceptVisitor(TypeVisitor const &visitor);
	virtual void checkDuplicate(Symbol_IR *ir);
	virtual string getSymbolKindName();
	virtual size_t getPointerCount();
	virtual size_t getArrayDimensions();
	virtual Type *getBaseType();
};

#endif /* TYPEDEFUDT_HPP */

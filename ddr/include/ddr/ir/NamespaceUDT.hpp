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

#ifndef NAMESPACEUDT_HPP
#define NAMESPACEUDT_HPP

#include "ddr/ir/Macro.hpp"
#include "ddr/ir/UDT.hpp"

class EnumMember;

class NamespaceUDT : public UDT
{
public:
	std::vector<UDT *> _subUDTs;
	std::vector<Macro> _macros;
	vector<EnumMember *> _enumMembers; /* used for anonymous enums */

	explicit NamespaceUDT(unsigned int lineNumber = 0);
	~NamespaceUDT();

	virtual DDR_RC acceptVisitor(const TypeVisitor &visitor);
	virtual bool insertUnique(Symbol_IR *ir);
	virtual const string &getSymbolKindName() const;
	virtual void addMacro(Macro *macro);
	virtual std::vector<UDT *> * getSubUDTS();
	virtual void renameFieldsAndMacros(const FieldOverride &fieldOverride, Type *replacementType);

	bool operator==(const Type & rhs) const;
	virtual bool compareToNamespace(const NamespaceUDT &) const;
};

#endif /* NAMESPACEUDT_HPP */

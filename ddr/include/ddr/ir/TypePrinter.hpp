/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

#include "ddr/ir/TypeVisitor.hpp"

#include "ddr/ir/Macro.hpp"

class EnumMember;
class Field;

#include <vector>

class TypePrinter : public TypeVisitor
{
private:
	const int32_t _flags;
	const int32_t _indent;

	void printIndent() const;
	void printFields(const std::vector<Field *> &fields) const;
	void printLiterals(const std::vector<EnumMember *> &literals) const;
	void printMacros(const std::vector<Macro> &macros) const;

	explicit TypePrinter(const TypePrinter *outer)
		: TypeVisitor()
		, _flags(outer->_flags)
		, _indent(outer->_indent + 1)
	{
	}

public:
	enum { FIELDS = 1, LITERALS = 2, MACROS = 4 };

	explicit TypePrinter(int32_t flags)
		: TypeVisitor()
		, _flags(flags)
		, _indent(0)
	{
	}

	template<typename TypeVector>
	void
	printUDTs(const TypeVector &types) const
	{
		for (typename TypeVector::const_iterator it = types.begin(); it != types.end(); ++it) {
			(*it)->acceptVisitor(*this);
		}
	}

	virtual DDR_RC visitType(Type *type) const;
	virtual DDR_RC visitClass(ClassUDT *type) const;
	virtual DDR_RC visitEnum(EnumUDT *type) const;
	virtual DDR_RC visitNamespace(NamespaceUDT *type) const;
	virtual DDR_RC visitTypedef(TypedefUDT *type) const;
	virtual DDR_RC visitUnion(UnionUDT *type) const;
};

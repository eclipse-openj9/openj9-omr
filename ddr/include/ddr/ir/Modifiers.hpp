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

#ifndef MODIFIERS_HPP
#define MODIFIERS_HPP

#include "ddr/std/string.hpp"
#include <vector>

using std::string;
using std::vector;

class Modifiers
{
public:
	vector<size_t> _arrayLengths;
	uint8_t _modifierFlags;
	size_t _pointerCount;
	size_t _referenceCount;

	static const uint8_t MODIFIER_FLAGS = 31;
	static const uint8_t NO_MOD = 0;
	static const uint8_t CONST_TYPE = 1;
	static const uint8_t VOLATILE_TYPE = 2;
	static const uint8_t UNALIGNED_TYPE = 4;
	static const uint8_t RESTRICT_TYPE = 8;
	static const uint8_t SHARED_TYPE = 16;

	Modifiers();
	~Modifiers();

	string getPointerType() const;
	string getModifierNames() const;
	void addArrayDimension(size_t length);
	bool isArray() const;
	size_t getArrayLength(size_t index) const;
	size_t getArrayDimensions() const;
	size_t getSize(size_t typeSize) const;
	bool operator==(const Modifiers &type) const;
};

#endif /* MODIFIERS_HPP */

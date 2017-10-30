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

#include <stdlib.h>

#include "ddr/config.hpp"
#include "ddr/ir/Modifiers.hpp"
#include "ddr/std/sstream.hpp"

using std::string;
using std::stringstream;
using std::vector;

Modifiers::Modifiers()
	: _arrayLengths()
	, _modifierFlags(NO_MOD)
	, _pointerCount(0)
	, _referenceCount(0)
{
}

Modifiers::~Modifiers()
{
}

static const char * const MODIFIER_NAMES[] = { "const", "volatile", "unaligned", "restrict", "shared" };

string
Modifiers::getModifierNames() const
{
	stringstream modifiers;
	for (uint8_t i = 1, index = 0; i < MODIFIER_FLAGS; i <<= 1, ++index) {
		if (0 != (_modifierFlags & i)) {
			modifiers << MODIFIER_NAMES[index] << " ";
		}
	}
	return modifiers.str();
}

string
Modifiers::getPointerType() const
{
	stringstream s;

	for (size_t i = 0; i < _pointerCount; ++i) {
		s << "*";
	}

	for (size_t i = 0; i < _referenceCount; ++i) {
		s << "&";
	}

	for (size_t i = getArrayDimensions(); i != 0; i -= 1) {
		s << "[]";
	}

	return s.str();
}

void
Modifiers::addArrayDimension(size_t length)
{
	_arrayLengths.push_back(length);
}

bool
Modifiers::isArray() const
{
	return _arrayLengths.size() > 0;
}

size_t
Modifiers::getArrayLength(size_t i) const
{
	if (i >= _arrayLengths.size()) {
		return 0;
	}
	return _arrayLengths[i];
}

size_t
Modifiers::getArrayDimensions() const
{
	return _arrayLengths.size();
}

size_t
Modifiers::getSize(size_t typeSize) const
{
	size_t sizeOf = typeSize;
	if (_pointerCount > 0) {
		sizeOf = sizeof(void *);
	}
	for (vector<size_t>::const_iterator it = _arrayLengths.begin(); it != _arrayLengths.end(); ++it) {
		sizeOf *= *it;
	}
	return sizeOf;
}

bool
Modifiers::operator==(const Modifiers &type) const
{
	bool arrayLengthsEqual = true;
	for (size_t i = _arrayLengths.size(); i != 0; i -= 1) {
		if (_arrayLengths[i] != type.getArrayLength(i)) {
			arrayLengthsEqual = false;
			break;
		}
	}

	return arrayLengthsEqual
		&& (_modifierFlags == type._modifierFlags)
		&& (_referenceCount == type._referenceCount)
		&& (_pointerCount == type._pointerCount);
}

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

#include <stdlib.h>

#include "ddr/config.hpp"
#include "ddr/ir/Modifiers.hpp"

using std::string;
using std::vector;

Modifiers::Modifiers()
	: _modifierFlags(NO_MOD),
	  _offset(0),
	  _pointerCount(0),
	  _referenceCount(0)
{
}

Modifiers::~Modifiers() {}

const string Modifiers::MODIFIER_NAMES[] = {"const", "volatile", "unaligned", "restrict", "shared"};

string
Modifiers::getModifierNames()
{
	string modifierString = "";
	for (int i = 1, index = 0; i < MODIFIER_FLAGS; i *= 2, index ++) {
		if (0 != (_modifierFlags & i)) {
			modifierString += MODIFIER_NAMES[index] + " ";
		}
	}
	return modifierString;
}

string
Modifiers::getPointerType()
{
	string s = string("");

	for (size_t i = 0; i < _pointerCount; i++) {
		s += "*";
	}

	for (size_t i = 0; i < _referenceCount; i++) {
		s += "&";
	}

	if (isArray()) {
		for (size_t i = 0; i < getArrayDimensions(); i++) {
			s += "[]";
		}
	}

	return s;
}

void
Modifiers::addArrayDimension(int length)
{
	_arrayLengths.push_back(length);
}

bool
Modifiers::isArray()
{
	return _arrayLengths.size() > 0;
}

size_t
Modifiers::getArrayLength(unsigned int i)
{
	if (i >= _arrayLengths.size()) {
		return 0;
	}
	return _arrayLengths[i];
}

size_t
Modifiers::getArrayDimensions()
{
	return _arrayLengths.size();
}

size_t
Modifiers::getSize(size_t typeSize)
{
	size_t sizeOf = 0;
	if (_pointerCount > 0) {
		sizeOf = sizeof(void *);
	} else {
		sizeOf = typeSize;
	}
	if (isArray()) {
		size_t arrayElements = 0;
		for (vector<size_t>::iterator it = _arrayLengths.begin(); it != _arrayLengths.end(); it += 1) {
			arrayElements += *it;
		}
		sizeOf *= arrayElements;
	}
	return sizeOf;
}

bool
Modifiers::operator==(Modifiers const& type) const
{
	bool arrayLengthsEqual = true;
	for (size_t i = 0; i < _arrayLengths.size(); i += 1) {
		if (_arrayLengths[i] != type._arrayLengths[i]) {
			arrayLengthsEqual = false;
			break;
		}
	}

	return (_modifierFlags == type._modifierFlags)
		&& (_referenceCount == type._referenceCount)
		&& (_pointerCount == type._pointerCount)
		&& arrayLengthsEqual;
}

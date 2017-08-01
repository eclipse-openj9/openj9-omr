/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2017
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

#ifndef FIELD_HPP
#define FIELD_HPP

#include "ddr/config.hpp"

#include <string>

#include "ddr/ir/Members.hpp"
#include "ddr/ir/Modifiers.hpp"
#include "ddr/ir/Type.hpp"

using std::string;

class Field : public Members
{
public:
	Type *_fieldType;
	size_t _sizeOf;
	size_t _offset;
	Modifiers _modifiers;
	size_t _bitField;
	bool _isStatic;

	Field();

	string getTypeName();
};

#endif /* FIELD_HPP */

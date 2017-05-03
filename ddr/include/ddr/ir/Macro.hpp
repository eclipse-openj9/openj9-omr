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

#ifndef MACRO_HPP
#define MACRO_HPP

#include <string>

#include "ddr/config.hpp"

class Macro
{
private:
	std::string _value;

public:
	std::string _name;

	Macro(std::string name, std::string value) : _value(value), _name(name)
	{
	}

	std::string
	getValue() const
	{
		return _value;
	}

	DDR_RC getNumeric(long long *ret);
};

#endif /* MACRO_HPP */

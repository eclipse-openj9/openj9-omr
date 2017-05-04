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

#include "ddr/macros/MacroInfo.hpp"

MacroInfo::MacroInfo(string typeName)
	: _typeName(typeName)
{
}

string
MacroInfo::getTypeName()
{
	return _typeName;
}

void
MacroInfo::addMacro(pair<string, string> p)
{
	_macros.insert(p);
}

size_t
MacroInfo::getNumMacros()
{
	return _macros.size();
}

set<pair<string, string> >::iterator
MacroInfo::getMacroStart()
{
	return _macros.begin();
}

set<pair<string, string> >::iterator
MacroInfo::getMacroEnd()
{
	return _macros.end();
}

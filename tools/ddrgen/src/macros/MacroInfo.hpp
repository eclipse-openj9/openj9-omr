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

#ifndef MACROINFO_HPP
#define MACROINFO_HPP

#include <string>
#include <set>
#include <vector>

using std::set;
using std::string;
using std::pair;
using std::vector;
using std::ifstream;

class MacroInfo {
private:
	/* This could be a real type name, or a fake type name representing a custom rule
	 * associated with a macro, or a name based off of a file
	 */
	string _typeName;

	/* A list of all macros associated with typeName */
	set<pair<string, string> > _macros;

public:
	MacroInfo(string typeName);

	string getTypeName();
	void addMacro(pair<string, string> p);
	size_t getNumMacros();
	set<pair<string, string> >::iterator getMacroStart();
	set<pair<string, string> >::iterator getMacroEnd();
};

#endif /* MACROINFO_HPP */

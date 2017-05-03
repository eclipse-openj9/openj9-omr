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

#ifndef MACROTOOL_HPP
#define MACROTOOL_HPP

#include <string>

#include "ddr/config.hpp"
#include "ddr/macros/MacroInfo.hpp"
#include "ddr/ir/Symbol_IR.hpp"

using std::string;
using std::vector;

class MacroTool
{
private:
	vector<MacroInfo> macroList;

	string getTypeName(string s);
	pair<string, string> getMacroInfo(string s);
	string getFileName(string s);

public:
	DDR_RC getMacros(string fname);
	DDR_RC addMacrosToIR(Symbol_IR *ir);
};

#endif /* MACROTOOL_HPP */

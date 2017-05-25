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

#ifndef SYMBOL_IR_HPP
#define SYMBOL_IR_HPP

#include <vector>
#include <set>

#include "omrport.h"

#include "ddr/config.hpp"
#include "ddr/ir/Type.hpp"

using std::vector;
using std::set;

class Symbol_IR {
public:
	vector<Type *> _types;
	/* Keep a set of names of structures already printed, to avoid printing duplicates in the superset. 
	 * Currently, only use this approach for AIX, where removeDuplicates() runs too slowly. Using this
	 * method on other platforms has not been tested yet. There may still be issues related to differences
	 * in the DWARF/intermediate representation structures between platforms which may be revealed by this
	 * approach.
	 */
	set<string> _fullTypeNames;

	~Symbol_IR();

	DDR_RC applyOverrideList(OMRPortLibrary *portLibrary, const char *overrideFiles);
	DDR_RC computeOffsets();
	void removeDuplicates();
private:
	DDR_RC applyOverrides(OMRPortLibrary *portLibrary, const char *overrideFile);
	DDR_RC computeFieldOffsets(Type *type);
};

#endif /* SYMBOL_IR_HPP */

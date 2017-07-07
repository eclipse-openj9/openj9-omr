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
#include <unordered_map>

#include "omrport.h"

#include "ddr/config.hpp"
#include "ddr/ir/EnumMember.hpp"
#include "ddr/ir/Field.hpp"
#include "ddr/ir/Type.hpp"

using std::vector;
using std::set;
using std::unordered_map;

struct FieldOverride {
	string structName;
	string fieldName;
	string overrideName;
	bool isTypeOverride;
};

class Field;
class NamespaceUDT;

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
	set<Type *> _typeSet;
	unordered_map<string, set<Type *> > _typeMap;

	Symbol_IR() {}
	~Symbol_IR();

	DDR_RC applyOverrideList(OMRPortLibrary *portLibrary, const char *overrideFiles);
	void computeOffsets();
	void removeDuplicates();
	DDR_RC mergeIR(Symbol_IR *other);

private:
	DDR_RC applyOverrides(OMRPortLibrary *portLibrary, const char *overrideFile);
	template<typename T> void mergeTypes(vector<T *> *source, vector<T *> *other,
		NamespaceUDT *outerNamespace, vector<Type *> *merged);
	void mergeFields(vector<Field *> *source, vector<Field *> *other, Type *type, vector<Type *> *merged);
	void mergeEnums(vector<EnumMember *> *source, vector<EnumMember *> *other);
	void addTypeToMap(Type *type);
	Type *findTypeInMap(Type *typeToFind);
	DDR_RC replaceTypeUsingMap(Type **type, Type *outer);

	friend class MergeVisitor;
	friend class TypeReplaceVisitor;
};

#endif /* SYMBOL_IR_HPP */

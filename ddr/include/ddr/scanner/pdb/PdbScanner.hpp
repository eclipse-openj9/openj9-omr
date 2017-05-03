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

#ifndef PDBSCANNER_HPP
#define PDBSCANNER_HPP

#include <map>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined(WIN32) || defined(WIN64)
/* windows.h defined uintptr_t.  Ignore its definition */
#define UDATA UDATA_win_
#include "dia2.h"
#undef UDATA	/* this is safe because our UDATA is a typedef, not a macro */
#endif

#include "ddr/ir/ClassUDT.hpp"
#include "ddr/ir/EnumUDT.hpp"
#include "ddr/scanner/Scanner.hpp"
#include "ddr/ir/Symbol_IR.hpp"

using std::hash;
using std::ifstream;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

typedef struct PostponedType
{
	Type **type;
	string name;
	size_t size;
	string typeIdentifier;
} PostponedType;


class PdbScanner: public Scanner
{
public:
	DDR_RC startScan(OMRPortLibrary *portLibrary, Symbol_IR *const ir, vector<string> *debugFiles);

private:
	Symbol_IR *_ir;
	unordered_map<string, Type *> _typeMap;
	vector<PostponedType> _postponedFields;
	set<string> _blacklist;
	set<string> _blacklistWildcard;

	void addType(Type *type, bool addToIR);
	DDR_RC addFieldMember(IDiaSymbol *symbol, ClassUDT *const udt);
	DDR_RC setBaseType(IDiaSymbol *typeSymbol, Type **type);
	DDR_RC setBaseTypeFloat(ULONGLONG ulLen, Type **type);
	DDR_RC setBaseTypeInt(ULONGLONG ulLen, Type **type);
	DDR_RC setBaseTypeUInt(ULONGLONG ulLen, Type **type);
	DDR_RC setPointerType(IDiaSymbol *symbol, Modifiers *modifiers);
	DDR_RC setTypeUDT(IDiaSymbol *typeSymbol, Type **type, NamespaceUDT *outerUDT);
	DDR_RC setType(IDiaSymbol *symbol, Type **type, Modifiers *modifiers, NamespaceUDT *outerUDT);
	DDR_RC setTypeModifier(IDiaSymbol *symbol, Modifiers *modifiers);

	DDR_RC addSymbol(IDiaSymbol *symbol, NamespaceUDT *outerUDT);
	DDR_RC setMemberOffset(IDiaSymbol *symbol, Field *newField);

	DDR_RC addChildrenSymbols(IDiaSymbol *symbol, enum SymTagEnum symTag, NamespaceUDT *outerUDT);
	DDR_RC addEnumMembers(IDiaSymbol *diaSymbol, EnumUDT *const e);
	DDR_RC createClassUDT(IDiaSymbol *symbol, NamespaceUDT *outerUDT);
	DDR_RC createEnumUDT(IDiaSymbol *symbol, NamespaceUDT *outerUDT);
	DDR_RC createTypedef(IDiaSymbol *symbol, NamespaceUDT *outerUDT);
	DDR_RC loadDataFromPdb(const wchar_t *filename, IDiaDataSource **diaDataSource, IDiaSession **diaSession, IDiaSymbol **diaSymbol);
	DDR_RC setSuperClassName(IDiaSymbol *symbol, ClassUDT *newUDT);
	void getNamespaceFromName(string *name, NamespaceUDT **outerUDT);

	bool blacklistedSymbol(string name);
	DDR_RC getBlacklist();
	DDR_RC getName(IDiaSymbol *symbol, string *name);
	DDR_RC getSize(IDiaSymbol *symbol, ULONGLONG *size);
	Type *getType(string s);
	string getUDTname(UDT *u);
	void initBaseTypeList();
	DDR_RC updatePostponedFieldNames();
	void renameAnonymousTypes();
	void renameAnonymousType(Type *type, ULONGLONG *unnamedTypeCount);

	DDR_RC dispatchScanChildInfo(Type *type, void *data);
};

#endif /* PDBSCANNER_HPP */

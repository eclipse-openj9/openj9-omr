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

#ifndef DWARFSCANNER_HPP
#define DWARFSCANNER_HPP

#include <map>
#include <unordered_map>

#include <dwarf.h>
#include <libdwarf.h>

#include "ClassUDT.hpp"
#include "config.hpp"
#include "EnumUDT.hpp"
#include "../Scanner.hpp"
#include "Symbol_IR.hpp"
#include "TypedefUDT.hpp"

struct TypeKey {
	char *fileName;
	std::string typeName;
	int lineNumber;

	bool operator==(const TypeKey& other) const;
};

struct KeyHasher {
	std::size_t operator()(const TypeKey& key) const;
};

class DwarfScanner: public Scanner
{
public:
	DwarfScanner();
	~DwarfScanner();

	DDR_RC startScan(OMRPortLibrary *portLibrary, Symbol_IR *const ir, vector<string> *debugFiles);

	DDR_RC dispatchScanChildInfo(Type *newType, void *data);
	DDR_RC dispatchScanChildInfo(NamespaceUDT *newClass, void *data);
	DDR_RC dispatchScanChildInfo(EnumUDT *newEnum, void *data);
	DDR_RC dispatchScanChildInfo(TypedefUDT *newTypedef, void *data);
	DDR_RC dispatchScanChildInfo(ClassUDT *newTypedef, void *data);
	DDR_RC dispatchScanChildInfo(UnionUDT *newTypedef, void *data);

private:
	Dwarf_Signed _fileNameCount;
	char **_fileNamesTable;
	std::unordered_map<TypeKey, Type *, KeyHasher> _typeMap;
	std::unordered_map<std::string, Type *> _typeStubMap;
	Symbol_IR *_ir;
	Dwarf_Debug _debug;

	DDR_RC scanFile(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *filepath);
	DDR_RC traverse_cu_in_debug_section(Symbol_IR *const ir);
	DDR_RC addType(Dwarf_Die die, Dwarf_Half tag, bool ignoreFilter, bool isSubUDT, Type **type);
	DDR_RC getType(Dwarf_Die die, Dwarf_Half tag, Type **const newUDT, bool isSubUDT, int *typeNum);
	DDR_RC createType(Dwarf_Die die, Dwarf_Half tag, std::string dieName, unsigned int lineNumber, Type **const newUDT);
	DDR_RC addEnumMember(Dwarf_Die die, EnumUDT *const udt);
	DDR_RC addClassField(Dwarf_Die die, ClassType *const newClass, std::string fieldName);
	DDR_RC getSuperUDT(Dwarf_Die die, ClassUDT *const udt);
	DDR_RC getTypeInfo(Dwarf_Die die, Dwarf_Die *dieout, std::string *typeName, Modifiers *modifiers, size_t *typeSize, size_t *bitField);
	DDR_RC getTypeSize(Dwarf_Die die, size_t *typeSize);
	DDR_RC getTypeTag(Dwarf_Die die, Dwarf_Die *typedie, Dwarf_Half *tag);
	DDR_RC getBlacklist(Dwarf_Die die);
	DDR_RC blackListedDie(Dwarf_Die die, bool *dieBlackListed);
	DDR_RC getName(Dwarf_Die die, std::string *name);
	DDR_RC createTypeKey(Dwarf_Die die, std::string typeName, TypeKey *key, unsigned int *lineNumber, std::string *fileName);
	DDR_RC getNextSibling(Dwarf_Die *die);
	DDR_RC getBitField(Dwarf_Die die, size_t *bitField);
};

#endif /* DWARFSCANNER_HPP */

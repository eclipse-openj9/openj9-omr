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

#include "PdbScanner.hpp"

#include <assert.h>
#include <comdef.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <sstream>

#include "config.hpp"
#include "EnumMember.hpp"
#include "ClassUDT.hpp"
#include "EnumUDT.hpp"
#include "Field.hpp"
#include "Symbol_IR.hpp"
#include "Type.hpp"
#include "TypedefUDT.hpp"

void
PdbScanner::addType(Type *type, bool addToIR = true)
{
}

DDR_RC
PdbScanner::getBlacklist()
{
	return DDR_RC_OK;
}

void
PdbScanner::initBaseTypeList()
{
}

DDR_RC
PdbScanner::startScan(OMRPortLibrary *portLibrary, Symbol_IR *const ir, vector<string> *debugFiles)
{
	return DDR_RC_OK;
}

void
PdbScanner::renameAnonymousTypes()
{
}

void
PdbScanner::renameAnonymousType(Type *type, ULONGLONG *unnamedTypeCount)
{
}

DDR_RC
PdbScanner::loadDataFromPdb(const wchar_t *filename, IDiaDataSource **dataSource, IDiaSession **session, IDiaSymbol **symbol)
{
	return DDR_RC_OK;
}

bool
PdbScanner::blacklistedSymbol(string name)
{
	return false;
}

DDR_RC
PdbScanner::updatePostponedFieldNames()
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::addChildrenSymbols(IDiaSymbol *symbol, enum SymTagEnum symTag, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::createTypedef(IDiaSymbol *symbol, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::addEnumMembers(IDiaSymbol *symbol, EnumUDT *const enumUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::createEnumUDT(IDiaSymbol *symbol, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setMemberOffset(IDiaSymbol *symbol, Field *newField)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setTypeModifier(IDiaSymbol *symbol, Modifiers *modifiers)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setTypeUDT(IDiaSymbol *typeSymbol, Type **type, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setPointerType(IDiaSymbol *symbol, Modifiers *modifiers)
{
	return DDR_RC_OK;
}

Type *
PdbScanner::getType(string s)
{
	return NULL;
}

DDR_RC
PdbScanner::setBaseTypeInt(ULONGLONG ulLen, Type **type)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setBaseTypeFloat(ULONGLONG ulLen, Type **type)
{
	return DDR_RC_OK;
}


DDR_RC
PdbScanner::setBaseTypeUInt(ULONGLONG ulLen, Type **type)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setBaseType(IDiaSymbol *typeSymbol, Type **type)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setType(IDiaSymbol *symbol, Type **type, Modifiers *modifiers, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::getName(IDiaSymbol *symbol, string *name)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::getSize(IDiaSymbol *symbol, ULONGLONG *size)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::addFieldMember(IDiaSymbol *symbol, ClassUDT *const udt)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::setSuperClassName(IDiaSymbol *symbol, ClassUDT *newUDT)
{
	return DDR_RC_OK;
}

DDR_RC
PdbScanner::createClassUDT(IDiaSymbol *symbol, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

void
PdbScanner::getNamespaceFromName(string *name, NamespaceUDT **outerUDT)
{
}

DDR_RC
PdbScanner::addSymbol(IDiaSymbol *symbol, NamespaceUDT *outerUDT)
{
	return DDR_RC_OK;
}

string
PdbScanner::getUDTname(UDT *u)
{
	return "";
}

DDR_RC
PdbScanner::dispatchScanChildInfo(Type *type, void *data)
{
	return DDR_RC_OK;
}

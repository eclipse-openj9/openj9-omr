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

#ifndef GENBLOB_HPP
#define GENBLOB_HPP

#include "ddr/config.hpp"

#include <string>
#include <vector>

#include "omrport.h"
#include "ddr/error.hpp"

using std::string;

class Symbol_IR;
class Type;
class EnumUDT;
class NamespaceUDT;
class TypedefUDT;
class ClassUDT;
class UnionUDT;

DDR_RC genBlob(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile, const char *blobFile);

class BlobGenerator
{
public:
	virtual DDR_RC genBinaryBlob(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *blobFile) = 0;

	virtual DDR_RC dispatchEnumerateType(Type *type, bool addFieldsOnly) = 0;
	virtual DDR_RC dispatchEnumerateType(NamespaceUDT *type, bool addFieldsOnly) {return dispatchEnumerateType((Type *)type, addFieldsOnly);}
	virtual DDR_RC dispatchEnumerateType(EnumUDT *type, bool addFieldsOnly) {return dispatchEnumerateType((NamespaceUDT *)type, addFieldsOnly);}
	virtual DDR_RC dispatchEnumerateType(TypedefUDT *type, bool addFieldsOnly) {return dispatchEnumerateType((NamespaceUDT *)type, addFieldsOnly);}
	virtual DDR_RC dispatchEnumerateType(ClassUDT *type, bool addFieldsOnly) {return dispatchEnumerateType((NamespaceUDT *)type, addFieldsOnly);}
	virtual DDR_RC dispatchEnumerateType(UnionUDT *type, bool addFieldsOnly) {return dispatchEnumerateType((NamespaceUDT *)type, addFieldsOnly);}

	virtual DDR_RC dispatchBuildBlob(Type *type, bool addFieldsOnly, string prefix) = 0;
	virtual DDR_RC dispatchBuildBlob(NamespaceUDT *type, bool addFieldsOnly, string prefix) {return dispatchBuildBlob((Type *)type, addFieldsOnly, prefix);}
	virtual DDR_RC dispatchBuildBlob(EnumUDT *type, bool addFieldsOnly, string prefix) {return dispatchBuildBlob((NamespaceUDT *)type, addFieldsOnly, prefix);}
	virtual DDR_RC dispatchBuildBlob(TypedefUDT *type, bool addFieldsOnly, string prefix) {return dispatchBuildBlob((NamespaceUDT *)type, addFieldsOnly, prefix);}
	virtual DDR_RC dispatchBuildBlob(ClassUDT *type, bool addFieldsOnly, string prefix) {return dispatchBuildBlob((NamespaceUDT *)type, addFieldsOnly, prefix);}
	virtual DDR_RC dispatchBuildBlob(UnionUDT *type, bool addFieldsOnly, string prefix) {return dispatchBuildBlob((NamespaceUDT *)type, addFieldsOnly, prefix);}
};

class SupersetGenerator
{
public:
	virtual DDR_RC printSuperset(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile) = 0;
};

#endif /* GENBLOB_HPP */

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
};

class SupersetGenerator
{
public:
	virtual DDR_RC printSuperset(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile) = 0;
};

#endif /* GENBLOB_HPP */

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

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <vector>

#include "omrport.h"

class Symbol_IR;
class Type;
class EnumUDT;
class NamespaceUDT;
class TypedefUDT;
class ClassUDT;
class UnionUDT;

using std::string;
using std::vector;

class Scanner
{
public:
	virtual DDR_RC startScan(OMRPortLibrary *portLibrary, Symbol_IR *const ir, vector<string> *debugFiles) = 0;

	virtual DDR_RC dispatchScanChildInfo(Type *type, void *data) = 0;
	virtual DDR_RC dispatchScanChildInfo(EnumUDT *type, void *data) {return DDR_RC_OK;}
	virtual DDR_RC dispatchScanChildInfo(NamespaceUDT *type, void *data) {return DDR_RC_OK;}
	virtual DDR_RC dispatchScanChildInfo(TypedefUDT *type, void *data) {return DDR_RC_OK;}
	virtual DDR_RC dispatchScanChildInfo(ClassUDT *type, void *data) {return DDR_RC_OK;}
	virtual DDR_RC dispatchScanChildInfo(UnionUDT *type, void *data) {return DDR_RC_OK;}
};

#endif /* SCANNER_HPP */

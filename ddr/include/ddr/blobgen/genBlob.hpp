/*******************************************************************************
 * Copyright (c) 2015, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef GENBLOB_HPP
#define GENBLOB_HPP

#include "ddr/std/string.hpp"
#include "ddr/error.hpp"
#include "omrport.h"

using std::string;

class ClassUDT;
class EnumUDT;
class NamespaceUDT;
class Symbol_IR;
class Type;
class TypedefUDT;
class UnionUDT;

DDR_RC genBlob(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile, const char *blobFile, bool printEmptyTypes);

class BlobGenerator
{
protected:
	 bool _printEmptyTypes;

public:
	virtual DDR_RC genBinaryBlob(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *blobFile) = 0;
};

class SupersetGenerator
{
protected:
	bool _printEmptyTypes;

public:
	virtual DDR_RC printSuperset(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile) = 0;
};

#endif /* GENBLOB_HPP */

/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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

#include <stdio.h>

#ifdef MS_WINDOWS
#undef BYTE
#include "windows.h"
#define PATH_MAX MAXPATHLEN
#else
#include <dlfcn.h>
#endif

#include <errno.h>

#include "TestDriver.hpp"
#include "il/Node.hpp"
#include "il/Symbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/TreeTop.hpp"
#include "compile/Method.hpp"
#include "ilgen/IlGeneratorMethodDetails.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "gtest/gtest.h"

namespace Test
{
void
TestDriver::RunTest()
   {
   allocateTestData();
   compileTestMethods();
   invokeTests();
   deallocateTestData();
   }

int32_t
TestDriver::compileMethodBuilder(TR::MethodBuilder *m, uint8_t ** entry)
   {
   TR::ResolvedMethod resolvedMethod(m);
   TR::IlGeneratorMethodDetails details(&resolvedMethod);

   int32_t rc=0;
   *entry = (uint8_t *) compileMethod(details, warm, rc);
   return rc;
   }

} //namespace Test

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

#include <limits.h>
#include <stdio.h>
#include "compile/Method.hpp"
#include "tests/SimplifierFoldAndTest.hpp"
#include "tests/SimplifierFoldAndIlInjector.hpp"
#include "gtest/gtest.h"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

namespace TestCompiler
{
SimplifierFoldAndTest::TestCompiledMethodType SimplifierFoldAndTest::testCompiledMethod = 0;

void
SimplifierFoldAndTest::allocateTestData()
   {
   }

void
SimplifierFoldAndTest::deallocateTestData()
   {
   }

void
SimplifierFoldAndTest::compileTestMethods()
   {
   int32_t rc = 0;

   TR::TypeDictionary types;

   TR::IlType* Int32 = types.PrimitiveType(TR::Int32);
   TR::IlType* Int64 = types.PrimitiveType(TR::Int64);

   int32_t numberOfArguments = 1;

   TR::IlType** argTypes = new TR::IlType*[numberOfArguments]
   {
      Int32
   };

   SimplifierFoldAndIlInjector ilInjector(&types, this);

   TR::ResolvedMethod compilee(__FILE__, LINETOSTR(__LINE__), "simplifierFoldAnd", numberOfArguments, argTypes, Int64, 0, &ilInjector);

   TR::IlGeneratorMethodDetails details(&compilee);

   testCompiledMethod = reinterpret_cast<SimplifierFoldAndTest::TestCompiledMethodType>(compileMethod(details, warm, rc));
   }

void
SimplifierFoldAndTest::invokeTests()
   {
   ASSERT_EQ(0ll, testCompiledMethod(0));
   ASSERT_EQ(0ll, testCompiledMethod(1));
   ASSERT_EQ(0ll, testCompiledMethod(-1));
   ASSERT_EQ(0ll, testCompiledMethod(-9));
   ASSERT_EQ(0ll, testCompiledMethod(2147483647));
   }
}

TEST(JITSimplifierTest, SimplifierFoldAndTest)
   {
   ::TestCompiler::SimplifierFoldAndTest().RunTest();
   }

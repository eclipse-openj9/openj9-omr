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
#include "OpCodesTest.hpp"
#include "tests/Qux2IlInjector.hpp"
#include "tests/Qux2Test.hpp"
#include "gtest/gtest.h"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"

namespace TestCompiler
{
class Qux2IlInjector;
testMethodType  * Qux2Test::_qux2 = 0;

void
Qux2Test::compileTestMethods()
   {
   int32_t rc = 0;

   TR::TypeDictionary types;
   Qux2IlInjector quxIlInjector(&types, this);
   int32_t numberOfArguments = 1;
   TR::IlType ** argTypes = new TR::IlType*[numberOfArguments];
   TR::IlType *Int32 = types.PrimitiveType(TR::Int32);
   argTypes[0]= Int32;
   TR::ResolvedMethod qux2Compilee(__FILE__, LINETOSTR(__LINE__), "qux2", numberOfArguments, argTypes, Int32, 0, &quxIlInjector);
   TR::IlGeneratorMethodDetails qux2Details(&qux2Compilee);
   _qux2 = (testMethodType *) (compileMethod(qux2Details, warm, rc));
   }

int32_t
Qux2Test::qux2(int32_t num)
   {
   int i = num;
   i = i * 2;
   return i;
   }

void
Qux2Test::invokeTests()
   {
   OMR_CT_EXPECT_EQ(_qux2, qux2(5), _qux2(5));
   OMR_CT_EXPECT_EQ(_qux2, 10, _qux2(5));
   OMR_CT_EXPECT_EQ(_qux2, qux2(INT_MAX/2), _qux2(INT_MAX/2));
//   OMR_CT_EXPECT_EQ(_qux2, qux2(INT_MIN), _qux2(INT_MIN));
   }

} // namespace TestCompiler

TEST(JITQuxTest, QuxTest2)
   {
   ::TestCompiler::Qux2Test qux2Test;
   qux2Test.RunTest();
   }

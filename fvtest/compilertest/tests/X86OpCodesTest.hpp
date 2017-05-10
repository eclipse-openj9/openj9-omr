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

#include "OpCodesTest.hpp"

namespace TestCompiler
{
class X86OpCodesTest : public OpCodesTest
   {
   public:
   virtual void compileIntegerArithmeticTestMethods();
   virtual void compileFloatArithmeticTestMethods();
   virtual void compileMemoryOperationTestMethods();
   virtual void compileUnaryTestMethods();
   virtual void compileShiftOrRolTestMethods();
   virtual void compileBitwiseTestMethods();
   virtual void compileDirectCallTestMethods();
   virtual void compileCompareTestMethods();
   virtual void compileTernaryTestMethods();
   virtual void compileAddressTestMethods();

   virtual void invokeIntegerArithmeticTests();
   virtual void invokeFloatArithmeticTests();
   virtual void invokeMemoryOperationTests();
   virtual void invokeUnaryTests();
   virtual void invokeShiftOrRolTests();
   virtual void invokeBitwiseTests();
   virtual void invokeDirectCallTests();
   virtual void invokeCompareTests();
   virtual void invokeTernaryTests();
   virtual void invokeAddressTests();
   virtual void UnsupportedOpCodesTests();

   virtual void compileDisabledIntegerArithmeticTestMethods();
   virtual void invokeDisabledIntegerArithmeticTests();
   virtual void compileDisabledConvertOpCodesTest();
   virtual void invokeDisabledConvertOpCodesTest();
   virtual void compileDisabledMemoryOpCodesTest();
   virtual void invokeDisabledMemoryOpCodesTest();

   virtual void invokeNoHelperUnaryTests();
   };

} // namespace TestCompiler

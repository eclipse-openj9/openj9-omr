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
class PPCOpCodesTest : public OpCodesTest
   {
   public:
   virtual void compileUnaryTestMethods();
   virtual void compileMemoryOperationTestMethods();
   virtual void compileTernaryTestMethods();
   virtual void compileCompareTestMethods();
   virtual void compileShiftOrRolTestMethods();
   virtual void compileBitwiseTestMethods();
   virtual void compileAddressTestMethods();

   virtual void invokeUnaryTests();
   virtual void invokeMemoryOperationTests();
   virtual void invokeTernaryTests();
   virtual void invokeCompareTests();
   virtual void invokeShiftOrRolTests();
   virtual void invokeBitwiseTests();
   virtual void invokeAddressTests();
   virtual void UnsupportedOpCodesTests();

   virtual void compileDisabledConvertTestMethods();
   virtual void compileDisabledCompareTestMethods();
   virtual void compileDisabledIntegerArithmeticTestMethods();
   virtual void compileDisabledFloatArithmeticTestMethods();
   virtual void compileDisabledMemoryOperationTestMethods();
   virtual void compileDisabledUnaryTestMethods();
   virtual void compileDisabledShiftOrRolTestMethods();
   virtual void compileDisabledBitwiseTestMethods();
   virtual void compileDisabledTernaryTestMethods();
   virtual void compileDisabledDirectCallTestMethods();

   virtual void invokeDisabledConvertTests();
   virtual void invokeDisabledCompareTests();
   virtual void invokeDisabledIntegerArithmeticTests();
   virtual void invokeDisabledFloatArithmeticTests();
   virtual void invokeDisabledMemoryOperationTests();
   virtual void invokeDisabledUnaryTests();
   virtual void invokeDisabledShiftOrRolTests();
   virtual void invokeDisabledBitwiseTests();
   virtual void invokeDisabledTernaryTest();
   virtual void invokeDisabledDirectCallTest();
   };

} // namespace TestCompiler

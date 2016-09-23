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
class S390OpCodesTest : public OpCodesTest
   {
   public:
   virtual void compileIntegerArithmeticTestMethods();
   virtual void compileFloatArithmeticTestMethods();
   virtual void compileMemoryOperationTestMethods();
   virtual void compileUnaryTestMethods();
   virtual void compileShiftOrRolTestMethods();
   virtual void compileBitwiseTestMethods();
   virtual void compileTernaryTestMethods();
   virtual void compileCompareTestMethods();
   virtual void compileDirectCallTestMethods();
   virtual void compileAddressTestMethods();

   virtual void invokeIntegerArithmeticTests();
   virtual void invokeFloatArithmeticTests();
   virtual void invokeMemoryOperationTests();
   virtual void invokeUnaryTests();
   virtual void invokeShiftOrRolTests();
   virtual void invokeBitwiseTests();
   virtual void invokeTernaryTests();
   virtual void invokeCompareTests();
   virtual void invokeDirectCallTests();
   virtual void invokeAddressTests();

   virtual void compileDisabledCompareOpCodesTestMethods();
   virtual void compileDisabledIntegerArithmeticTestMethods();
   virtual void compileDisabledUnaryTestMethods();
   virtual void compileDisabledShiftOrRolTestMethods();
   virtual void compileDisabledTernaryTestMethods();
   virtual void compileDisabledMemoryOperationTestMethods();
   virtual void compileDisabledBitwiseTestMethods();
   virtual void compileDisabledDirectCallTestMethods();
   virtual void compileDisabledS390ConvertToAddressOpCodesTests();

   virtual void invokeDisabledCompareOpCodesTests();
   virtual void invokeDisabledIntegerArithmeticTests();
   virtual void invokeDisabledUnaryTests();
   virtual void invokeDisabledShiftOrRolTests();
   virtual void invokeDisabledTernaryTests();
   virtual void invokeDisabledMemoryOperationTests();
   virtual void invokeDisabledBitwiseTests();
   virtual void invokeDisabledDirectCallTests();
   virtual void invokeDisabledS390ConvertToAddressOpCodesTests();
   };

} // namespace TestCompiler

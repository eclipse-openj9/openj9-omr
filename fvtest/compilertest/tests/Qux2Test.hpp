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

#include "TestDriver.hpp"

namespace Test
{
class Qux2IlInjector;
typedef int32_t (testMethodType)(int32_t);

class Qux2Test : public TestDriver
   {
   protected:
   virtual void allocateTestData();
   virtual void compileTestMethods();
   virtual void invokeTests();
   virtual void deallocateTestData();

   private:

   static testMethodType *_qux2;

   // straight C(++) implementations of foo and bar for initial test validation purposes

   static int32_t qux2(int32_t);
   };

} // namespace Test

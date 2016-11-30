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

namespace TestCompiler
{

typedef int32_t (FooMethodType)(int32_t);
typedef int32_t (BarMethodType)(int32_t);

class FooBarTest : public TestDriver
   {
   public:
   static int32_t          dataArraySize() { return _dataArraySize; }
   static int32_t        * dataArray()     { return _dataArray; }
   static TR::ResolvedMethod * barMethod() { return _barCompilee; }

   protected:
   virtual void compileTestMethods();
   virtual void invokeTests();

   private:
   const static int32_t _dataArraySize = 100;
   static int32_t _dataArray[100];

   static FooMethodType *_foo;
   static BarMethodType *_bar;

   static TR::ResolvedMethod *_barCompilee;

   // straight C(++) implementations of foo and bar for initial test validation purposes
   static int32_t foo(int32_t);
   static int32_t bar(int32_t);
   };

} // namespace TestCompiler

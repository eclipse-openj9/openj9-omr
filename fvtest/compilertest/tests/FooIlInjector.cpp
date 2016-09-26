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

#include "compile/Compilation.hpp"
#include "env/FrontEnd.hpp"
#include "compile/Method.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "tests/FooIlInjector.hpp"
#include "tests/FooBarTest.hpp"

namespace TestCompiler
{

bool
FooIlInjector::injectIL()
   {
   FooBarTest *test = static_cast<FooBarTest *>(_test);
   createBlocks(4);

   // 4 blocks requested start at 2 (0 is entry, 1 is exit)
   // by default, generate to block 2

   // Block 2: blocks(0)
   // int32_t newIndex = _bar(index);
   // if (newIndex < 0) goto Block5;
   TR::SymbolReference *newIndexSymRef = newTemp(Int32);
   TR::ResolvedMethod *barMethod = test->barMethod();
   storeToTemp(newIndexSymRef, callFunction(barMethod, Int32, 1, indexParameter()));
   ifjump(TR::ificmplt, loadTemp(newIndexSymRef), iconst(0), 3);

   // Block 3: blocks(1)
   // if (newIndex >= 100) goto Block5;
   ifjump(TR::ificmpge, loadTemp(newIndexSymRef), iconst(test->dataArraySize()), 3);

   // Block 4: blocks(2)
   // return dataArray[newIndex];
   returnValue(arrayLoad(staticAddress(test->dataArray()), loadTemp(newIndexSymRef), Int32));

   // Block 5: blocks(3)
   // return -1;
   generateToBlock(3);
   returnValue(iconst(-1));

   return true;
   }

} // namespace TestCompiler

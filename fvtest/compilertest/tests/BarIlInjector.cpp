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
#include "tests/BarIlInjector.hpp"
#include "tests/FooBarTest.hpp"

namespace TestCompiler
{

bool
BarIlInjector::injectIL()
   {
   FooBarTest *test = static_cast<FooBarTest *>(_test);
   createBlocks(4);


   // 4 blocks requested start at 2 (0 is entry, 1 is exit)
   // by default, generate to block 2

   // Block2: blocks(0)
   // if (index < 0) goto Block5;
   ifjump(TR::ificmplt, indexParameter(), iconst(0), 3);

   // Block3: blocks(1)
   // if (index >= 100) goto Block5;
   ifjump(TR::ificmpge, indexParameter(), iconst(test->dataArraySize()), 3);

   // Block4: blocks(2)
   // return dataArray[index];
   returnValue(arrayLoad(staticAddress(test->dataArray()), indexParameter(), Int32));

   // Block5: blocks(3)
   // return -1;
   generateToBlock(3);
   returnValue(iconst(-1));

   return true;
   }

} // namespace TestCompiler

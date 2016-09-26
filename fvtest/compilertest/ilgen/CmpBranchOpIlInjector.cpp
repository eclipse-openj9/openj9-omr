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
#include "ilgen/CmpBranchOpIlInjector.hpp"

namespace TestCompiler
{
bool
CmpBranchOpIlInjector::injectIL()
   {
   if (!isOpCodeSupported())
      return false;
   createBlocks(3);

   // 3 blocks requested start at 2 (0 is entry, 1 is exit)
   // by default, generate to block 2

   // Block2: blocks(0)
   // if () goto Block4;
   ifjump(_opCode, parm(1), parm(2), 2);

   // Block3: blocks(1)
   // return 0;
   returnValue(iconst(0));

   // Block4: blocks(2)
   // return 1;
   generateToBlock(2);
   returnValue(iconst(1));

   return true;
   }

} // namespace TestCompiler

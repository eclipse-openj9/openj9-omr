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

#include "codegen/CodeGenerator.hpp"
#include "codegen/TRSystemLinkage.hpp"
#include "compile/Compilation.hpp"
#include "env/CompilerEnv.hpp"

namespace Test
{
namespace Z
{

CodeGenerator::CodeGenerator() :
   Test::CodeGenerator()
   {
   self()->getS390Linkage()->initS390RealRegisterLinkage();

   self()->setAccessStaticsIndirectly(true);
   }


TR::Linkage *
CodeGenerator::createLinkage(TR_LinkageConventions lc)
   {
   // *this    swipeable for debugging purposes
   TR::Linkage * linkage;
   TR::Compilation *comp = self()->comp();
   switch (lc)
      {
      case TR_Helper:
         linkage = new (self()->trHeapMemory()) TR_S390zLinuxSystemLinkage(self());
         break;

      case TR_Private:
        // no private linkage, fall through to system

      case TR_System:
         if (TR::Compiler->target.isLinux())
            linkage = new (self()->trHeapMemory()) TR_S390zLinuxSystemLinkage(self());
         else
            linkage = new (self()->trHeapMemory()) TR_S390zOSSystemLinkage(self());
         break;

      default :
         TR_ASSERT(0, "\nTestarossa error: Illegal linkage convention %d\n", lc);
      }
   self()->setLinkage(lc, linkage);
   return linkage;
   }

} // namespace Z
} // namespace Test

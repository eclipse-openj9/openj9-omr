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

#include "codegen/Analyser.hpp"

#include "compile/Compilation.hpp"  // for Compilation
#include "compile/SymbolReferenceTable.hpp"    // for SymbolReferenceTable, etc
#include "il/ILOps.hpp"             // for ILOpCode
#include "il/Node.hpp"              // for Node
#include "il/Node_inlines.hpp"      // for Node::getReferenceCount
#include "il/Symbol.hpp"            // for Symbol

namespace TR { class SymbolReference; }

void TR_Analyser::setInputs(TR::Node     *firstChild,
                            TR::Register *firstRegister,
                            TR::Node     *secondChild,
                            TR::Register *secondRegister,
                            bool         nonClobberingDestination,
                            bool         dontClobberAnything,
                            TR::Compilation *comp,
                            bool         lockedIntoRegister1,
                            bool         lockedIntoRegister2)
   {
   _inputs = 0;

   if (firstRegister)
      {
      setReg1();
      }

   if (secondRegister)
      {
      setReg2();
      }

   // The VFT pointer can be low-tagged, so generally we can't operate on it
   // directly in memory.
   //
   TR::SymbolReference *vftPointerSymRef = TR::comp()->getSymRefTab()->element(TR::SymbolReferenceTable::vftSymbol);

   if (  firstChild->getOpCode().isMemoryReference()
      && firstChild->getSymbolReference() != vftPointerSymRef
      && firstChild->getReferenceCount() == 1
      && !firstChild->getSymbol()->isRegisterSymbol()
      && !lockedIntoRegister1)
      {
      setMem1();
      }

   if (  secondChild->getOpCode().isMemoryReference()
      && secondChild->getSymbolReference() != vftPointerSymRef
      && secondChild->getReferenceCount() == 1
      && !secondChild->getSymbol()->isRegisterSymbol()
      && !lockedIntoRegister2)
      {
      setMem2();
      }

   if (!dontClobberAnything)
      {

      if (!nonClobberingDestination)
         {
         if (firstChild == secondChild && firstChild->getReferenceCount() == 2)
            {
            setClob1();
            setClob2();
            }

         if (firstChild->getReferenceCount() == 1)
            {
            setClob1();
            }

         if (secondChild->getReferenceCount() == 1)
            {
            setClob2();
            }
         }
      else
         {
         // set both operands to be clobberable because know that instruction doesn't
         // actually clobber destination operand so want to make either one a possible
         // no-cost destination operand.
         setClob1();
         setClob2();
         }
      }
   }

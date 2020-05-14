/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#pragma csect(CODE,"OMRZPeephole#C")
#pragma csect(STATIC,"OMRZPeephole#S")
#pragma csect(TEST,"OMRZPeephole#T")

#include "codegen/Peephole.hpp"

#include "codegen/CodeGenerator.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/S390GenerateInstructions.hpp"
#include "codegen/S390Instruction.hpp"

namespace TR { class Node; }

OMR::Z::Peephole::Peephole(TR::Compilation* comp) :
   OMR::Peephole(comp)
   {}

bool
OMR::Z::Peephole::performOnInstruction(TR::Instruction* cursor)
   {
   bool performed = false;

   if (cg()->afterRA())
      {
      switch(cursor->getOpCodeValue())
         {
         case TR::InstOpCode::NILF:
            {
            performed |= attemptToRemoveDuplicateNILF(cursor);
            break;
            }
         case TR::InstOpCode::NILH:
            {
            performed |= attemptToRemoveDuplicateNILH(cursor);
            break;
            }
         default:
            break;
         }
      }
   else
      {
      switch(cursor->getOpCodeValue())
         {
         case TR::InstOpCode::L:
            {
            performed |= attemptLoadStoreReduction(cursor, TR::InstOpCode::ST, 4);
            break;
            }
         case TR::InstOpCode::LFH:
            {
            performed |= attemptLoadStoreReduction(cursor, TR::InstOpCode::STFH, 4);
            break;
            }
         case TR::InstOpCode::LG:
            {
            performed |= attemptLoadStoreReduction(cursor, TR::InstOpCode::STG, 8);
            break;
            }
         default:
            break;
         }
      }

   return performed;
   }

bool
OMR::Z::Peephole::attemptLoadStoreReduction(TR::Instruction* cursor, TR::InstOpCode::Mnemonic storeOpCode, uint16_t size)
   {
   if (cursor->getNext()->getOpCodeValue() == storeOpCode)
      {
      TR::S390RXInstruction* loadInst = static_cast<TR::S390RXInstruction*>(cursor);
      TR::S390RXInstruction* storeInst = static_cast<TR::S390RXInstruction*>(cursor->getNext());

      // Cannot change to MVC if symbol references are unresolved, MVC doesn't follow same format as load and store so patching won't work
      if (loadInst->getMemoryReference()->getSymbolReference() && loadInst->getMemoryReference()->getSymbolReference()->isUnresolved() ||
          storeInst->getMemoryReference()->getSymbolReference() && storeInst->getMemoryReference()->getSymbolReference()->isUnresolved())
         {
         return false;
         }

      if (!loadInst->getMemoryReference()->getBaseRegister() && !loadInst->getMemoryReference()->getIndexRegister() ||
          !storeInst->getMemoryReference()->getBaseRegister() && !storeInst->getMemoryReference()->getIndexRegister())
         {
         return false;
         }

      TR::Register* reg = loadInst->getRegisterOperand(1);

      if (reg != storeInst->getRegisterOperand(1))
         {
         return false;
         }

      // Register must only be used in the load-store sequence, possibly used in the memory reference
      // of the load however, not in the store since the the register would have to be loaded first.
      ncount_t uses = 2;
      uses += reg == loadInst->getMemoryReference()->getBaseRegister();

      // Bail out if register could have future uses, or if either instruction has dependencies.
      // Since 2 instructions are replaced with one before RA, we can't handle things like merging
      // dependencies.
      if (reg->getTotalUseCount() != uses || loadInst->getDependencyConditions() || storeInst->getDependencyConditions())
         {
         return false;
         }

      if (performTransformation(comp(), "O^O S390 PEEPHOLE: Transforming load-store sequence at %p to MVC.", storeInst))
         {
         TR::DebugCounter::incStaticDebugCounter(comp(), "z/peephole/load-store");

         loadInst->getMemoryReference()->resetMemRefUsedBefore();
         storeInst->getMemoryReference()->resetMemRefUsedBefore();

         reg->decTotalUseCount(2);

         if (loadInst->getMemoryReference()->getBaseRegister())
            {
            loadInst->getMemoryReference()->getBaseRegister()->decTotalUseCount();
            }

         if (storeInst->getMemoryReference()->getBaseRegister())
            {
            storeInst->getMemoryReference()->getBaseRegister()->decTotalUseCount();
            }

         if (loadInst->getMemoryReference()->getIndexRegister())
            {
            loadInst->getMemoryReference()->getIndexRegister()->decTotalUseCount();
            }

         if (storeInst->getMemoryReference()->getIndexRegister())
            {
            storeInst->getMemoryReference()->getIndexRegister()->decTotalUseCount();
            }

         TR::Instruction * newInst = generateSS1Instruction(cg(), TR::InstOpCode::MVC, comp()->getStartTree()->getNode(), size - 1, storeInst->getMemoryReference(), loadInst->getMemoryReference(), cursor->getPrev());

         if (loadInst->getGCMap())
            {
            newInst->setGCMap(loadInst->getGCMap());
            }

         if (loadInst->needsGCMap())
            {
            newInst->setNeedsGCMap(loadInst->getGCRegisterMask());
            }

         if (storeInst->getGCMap())
            {
            newInst->setGCMap(storeInst->getGCMap());
            }

         if (storeInst->needsGCMap())
            {
            newInst->setNeedsGCMap(storeInst->getGCRegisterMask());
            }

         cg()->deleteInst(storeInst);
         cg()->replaceInst(loadInst, newInst);

         return true;
         }
      }
   return false;
   }

bool
OMR::Z::Peephole::attemptToRemoveDuplicateNILF(TR::Instruction* cursor)
   {
   if (cursor->getNext()->getKind() == TR::Instruction::IsRIL)
      {
      TR::S390RILInstruction* currInst = static_cast<TR::S390RILInstruction*>(cursor);
      TR::S390RILInstruction* nextInst = static_cast<TR::S390RILInstruction*>(cursor->getNext());

      bool instructionsMatch =
         (currInst->getTargetPtr() == nextInst->getTargetPtr()) &&
         (currInst->getTargetSnippet() == nextInst->getTargetSnippet()) &&
         (currInst->getTargetSymbol() == nextInst->getTargetSymbol()) &&
         (currInst->getTargetLabel() == nextInst->getTargetLabel()) &&
         (currInst->getMask() == nextInst->getMask()) &&
         (currInst->getSymbolReference() == nextInst->getSymbolReference());

      if (instructionsMatch)
         {
         if (currInst->matchesTargetRegister(nextInst->getRegisterOperand(1)) &&
            nextInst->matchesTargetRegister(currInst->getRegisterOperand(1)))
            {
            if (currInst->getSourceImmediate() == nextInst->getSourceImmediate())
               {
               if (performTransformation(comp(), "O^O S390 PEEPHOLE: deleting duplicate NILF from pair %p %p*\n", currInst, nextInst))
                  {
                  cg()->deleteInst(nextInst);

                  return true;
                  }
               }
            // To perform
            //
            // NILF     @05,X'000000C0'
            // NILF     @05,X'000000FF'
            //
            // ->
            //
            // NILF     @05,X'000000C0'
            //
            // test if
            // ((C0 & FF) == C0) && ((FF & C0) != FF)
            //
            // want to remove the second unnecessary instruction
            //
            else if (((currInst->getSourceImmediate() & nextInst->getSourceImmediate()) == currInst->getSourceImmediate()) &&
               ((nextInst->getSourceImmediate() & currInst->getSourceImmediate()) != nextInst->getSourceImmediate()))
               {
               if (performTransformation(comp(), "O^O S390 PEEPHOLE: deleting unnecessary NILF from pair %p %p*\n", currInst, nextInst))
                  {
                  cg()->deleteInst(nextInst);

                  return true;
                  }
               }
            }
         }
      }

   return false;
   }

bool
OMR::Z::Peephole::attemptToRemoveDuplicateNILH(TR::Instruction* cursor)
   {
   if (cursor->getNext()->getKind() == TR::Instruction::IsRI)
      {
      TR::S390RIInstruction* currInst = static_cast<TR::S390RIInstruction*>(cursor);
      TR::S390RIInstruction* nextInst = static_cast<TR::S390RIInstruction*>(cursor->getNext());

      if (currInst->isImm() == nextInst->isImm())
         {
         if (currInst->isImm())
            {
            if (currInst->getSourceImmediate() == nextInst->getSourceImmediate())
               {
               if (currInst->matchesTargetRegister(nextInst->getRegisterOperand(1)) &&
                   nextInst->matchesTargetRegister(currInst->getRegisterOperand(1)))
                  {
                  if (performTransformation(comp(), "O^O S390 PEEPHOLE: deleting duplicate NILH from pair %p %p*\n", currInst, nextInst))
                     {
                     cg()->deleteInst(nextInst);

                     return true;
                     }
                  }
               }
            }
         }
      }

   return false;
   }

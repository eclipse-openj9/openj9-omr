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
         case TR::InstOpCode::SLLG:
         case TR::InstOpCode::SLAG:
         case TR::InstOpCode::SLLK:
         case TR::InstOpCode::SRLK:
         case TR::InstOpCode::SLAK:
         case TR::InstOpCode::SRAK:
            {
            performed |= attemptToReduce64BitShiftTo32BitShift(cursor);
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
OMR::Z::Peephole::attemptToReduce64BitShiftTo32BitShift(TR::Instruction* cursor)
   {
   TR::S390RSInstruction* shiftInst = static_cast<TR::S390RSInstruction*>(cursor);

   // The shift is supposed to be an integer shift when reducing 64bit shifts.
   // Note the NOT in front of second boolean expr. pair
   TR::InstOpCode::Mnemonic oldOpCode = shiftInst->getOpCodeValue();
   if ((oldOpCode == TR::InstOpCode::SLLG || oldOpCode == TR::InstOpCode::SLAG)
         && shiftInst->getNode()->getOpCodeValue() != TR::ishl)
      {
      return false;
      }

   TR::Register* sourceReg = (shiftInst->getSecondRegister())?(shiftInst->getSecondRegister()->getRealRegister()):NULL;
   TR::Register* targetReg = (shiftInst->getRegisterOperand(1))?(shiftInst->getRegisterOperand(1)->getRealRegister()):NULL;

   // Source and target registers must be the same
   if (sourceReg != targetReg)
      {
      return false;
      }

   if (performTransformation(comp(), "O^O S390 PEEPHOLE: Reverting int shift at %p from SLLG/SLAG/S[LR][LA]K to SLL/SLA/SRL/SRA.\n", shiftInst))
      {
      TR::InstOpCode::Mnemonic newOpCode = TR::InstOpCode::BAD;
      switch (oldOpCode)
         {
         case TR::InstOpCode::SLLG:
         case TR::InstOpCode::SLLK:
            newOpCode = TR::InstOpCode::SLL; break;
         case TR::InstOpCode::SLAG:
         case TR::InstOpCode::SLAK:
            newOpCode = TR::InstOpCode::SLA; break;
         case TR::InstOpCode::SRLK:
            newOpCode = TR::InstOpCode::SRL; break;
         case TR::InstOpCode::SRAK:
            newOpCode = TR::InstOpCode::SRA; break;
         default:
            TR_ASSERT_FATAL(false, "Unexpected OpCode for revertTo32BitShift\n");
            break;
         }

      TR::S390RSInstruction* newInstr = NULL;

      if (shiftInst->getSourceImmediate())
         {
         newInstr = new (cg()->trHeapMemory()) TR::S390RSInstruction(newOpCode, shiftInst->getNode(), shiftInst->getRegisterOperand(1), shiftInst->getSourceImmediate(), shiftInst->getPrev(), cg());
         }
      else if (shiftInst->getMemoryReference())
         {
         TR::MemoryReference* memRef = shiftInst->getMemoryReference();
         memRef->resetMemRefUsedBefore();
         newInstr = new (cg()->trHeapMemory()) TR::S390RSInstruction(newOpCode, shiftInst->getNode(), shiftInst->getRegisterOperand(1), memRef, shiftInst->getPrev(), cg());
         }
      else
         {
         TR_ASSERT_FATAL(false, "Unexpected RSY format\n");
         }

      cg()->replaceInst(shiftInst, newInstr);

      return true;
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

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

static bool
isBarrierToPeepHoleLookback(TR::Instruction* cursor)
   {
   if (cursor == NULL)
      return true;
   
   if (cursor->isLabel())
      return true;

   if (cursor->isCall())
      return true;

   if (cursor->isBranchOp())
      return true;

   if (cursor->getOpCodeValue() == TR::InstOpCode::DCB)
      return true;

   return false;
   }

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
         case TR::InstOpCode::CGIT:
            {
            performed |= attemptToRemoveRedundantCompareAndTrap(cursor);
            break;
            }
         case TR::InstOpCode::LGR:
         case TR::InstOpCode::LTGR:
            {
            performed |= attemptToReduceAGI(cursor);
            break;
            }
         case TR::InstOpCode::L:
            {
            performed |= attemptToReduceLToICM(cursor);
            break;
            }
         case TR::InstOpCode::LLC:
            {
            performed |= attemptToReduceLLCToLLGC(cursor);
            break;
            }
         case TR::InstOpCode::LR:
         case TR::InstOpCode::LTR:
            {
            performed |= attemptToReduceAGI(cursor);
            break;
            }
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
OMR::Z::Peephole::attemptToReduceAGI(TR::Instruction* cursor)
   {
   bool performed=false;
   int32_t windowSize=0;
   const int32_t MaxWindowSize=8; // Look down 8 instructions (4 cycles)
   const int32_t MaxLAWindowSize=6; // Look up 6 instructions (3 cycles)

   TR::Register *lgrTargetReg = cursor->getRegisterOperand(1);
   TR::Register *lgrSourceReg = cursor->getRegisterOperand(2);

   TR::RealRegister *gpr0 = cg()->machine()->getRealRegister(TR::RealRegister::GPR0);

   // no renaming possible if both target and source are the same
   // this can happend with LTR and LTGR
   // or if source reg is gpr0
   if  (toRealRegister(lgrSourceReg)==gpr0 || (lgrTargetReg==lgrSourceReg) ||
        toRealRegister(lgrTargetReg)==cg()->getStackPointerRealRegister(NULL))
      return performed;

   TR::Instruction* current = cursor->getNext();

   // if:
   //  1) target register is invalidated, or
   //  2) current instruction is a branch or call
   // then cannot continue.
   // if:
   //  1) source register is invalidated, or
   //  2) current instruction is a label
   // then cannot rename registers, but might be able to replace
   // _cursor instruction with LA

   bool reachedLabel = false;
   bool reachedBranch = false;
   bool sourceRegInvalid = false;
   bool attemptLA = false;

   while ((current != NULL) &&
         !current->matchesTargetRegister(lgrTargetReg) &&
         !isBarrierToPeepHoleLookback(current) &&
         windowSize<MaxWindowSize)
      {
      // do not look across Transactional Regions, the Register save mask is optimistic and does not allow renaming
      if (current->getOpCodeValue() == TR::InstOpCode::TBEGIN ||
          current->getOpCodeValue() == TR::InstOpCode::TBEGINC ||
          current->getOpCodeValue() == TR::InstOpCode::TEND ||
          current->getOpCodeValue() == TR::InstOpCode::TABORT)
         {
         return false;
         }

      // if we reach a label or the source reg becomes invalidated
      // then we cannot rename regs in mem refs, but we could still try using LA
      reachedLabel = reachedLabel || current->isLabel();
      reachedBranch = reachedBranch || (current->isBranchOp() && !(current->isExceptBranchOp()));

      // does instruction load or store? otherwise just ignore and move to next instruction
      if (current->isLoad() || current->isStore())
         {
         TR::MemoryReference *mr = current->getMemoryReference();
         while (mr)
            {
            if (mr && mr->getBaseRegister() == lgrTargetReg)
               {
               if (!reachedLabel && !reachedBranch && !sourceRegInvalid)
                  {
                  if (performTransformation(comp(), "O^O S390 PEEPHOLE: AGI register renaming on [%p] from source load [%p].\n", current, cursor))
                     {
                     mr->setBaseRegister(lgrSourceReg, cg());

                     performed = true;
                     }
                  }
               else
                  {
                  // We couldn't do register renaming, but perhapse can use an LA instruction
                  attemptLA = true;
                  }
               }

            if (mr && mr->getIndexRegister() == lgrTargetReg)
               {
               if (!reachedLabel && !reachedBranch && !sourceRegInvalid)
                  {
                  if (performTransformation(comp(), "O^O S390 PEEPHOLE: AGI register renaming on [%p] from source load [%p].\n", current, cursor))
                     {
                     mr->setIndexRegister(lgrSourceReg);

                     performed = true;
                     }
                  }
               else
                  {
                  // We couldn't do register renaming, but perhapse can use an LA instruction
                  attemptLA = true;
                  }
               }

            if (mr == current->getMemoryReference())
               mr = current->getMemoryReference2();
            else
               mr = NULL;
            }
         }

         // If the current instruction invalidates the source register, subsequent instructions' memref cannot be renamed
         sourceRegInvalid = sourceRegInvalid || current->matchesTargetRegister(lgrSourceReg);

         current = current->getNext();
         windowSize++;
      }

   // We can replace the cursor with an LA instruction if:
   //   1. The mnemonic is LGR
   //   2. The target is 64-bit (because LA sets the upppermost bit to 0 on 31-bit)
   attemptLA &= cursor->getOpCodeValue() == TR::InstOpCode::LGR && comp()->target().is64Bit();

   if (attemptLA)
      {
      // We check to see that we are eliminating the AGI, not just propagating it up in the code. We check for labels,
      // source register invalidation, and routine calls (basically the opposite of the above while loop condition).
      current = cursor->getPrev();
      windowSize = 0;
      while ((current != NULL) &&
         !isBarrierToPeepHoleLookback(current) &&
         !current->matchesTargetRegister(lgrSourceReg) &&
         windowSize < MaxLAWindowSize)
         {
         current = current->getPrev();
         windowSize++;
         }

      // if we reached the end of the loop and didn't find a conflict, switch the instruction to LA
      if (windowSize == MaxLAWindowSize)
         {
         if (performTransformation(comp(), "O^O S390 PEEPHOLE: AGI LA reduction on [%p] from source load [%p].\n", current, cursor))
            {
            auto laInst = generateRXInstruction(cg(), TR::InstOpCode::LA, comp()->getStartTree()->getNode(), lgrTargetReg, 
               generateS390MemoryReference(lgrSourceReg, 0, cg()), cursor->getPrev());

            cg()->replaceInst(cursor, laInst);

            performed = true;
            }
         }
      }

   return performed;
   }

bool
OMR::Z::Peephole::attemptToReduceLToICM(TR::Instruction* cursor)
   {
   // Look for L/LTR instruction pair and reduce to LTR
   TR::Instruction* prev = cursor->getPrev();
   TR::Instruction* next = cursor->getNext();
   if (next->getOpCodeValue() != TR::InstOpCode::LTR && next->getOpCodeValue() != TR::InstOpCode::CHI)
      return false;

   bool performed = false;
   bool isICMOpportunity = false;

   TR::S390RXInstruction* load= (TR::S390RXInstruction*) cursor;

   TR::MemoryReference* mem = load->getMemoryReference();

   if (mem == NULL) return false;
   // We cannot reduce L to ICM if the L uses both index and base registers.
   if (mem->getBaseRegister() != NULL && mem->getIndexRegister() != NULL) return false;

   // L and LTR/CHI instructions must work on the same register.
   if (load->getRegisterOperand(1) != next->getRegisterOperand(1)) return false;

   if (next->getOpCodeValue() == TR::InstOpCode::LTR)
      {
      // We must check for sourceRegister on the LTR instruction, in case there was a dead load
      //        L      GPRX, MEM       <-- this is a dead load, for whatever reason (e.g. GRA)
      //        LTR    GPRX, GPRY
      // it is wrong to transform the above sequence into
      //        ICM    GPRX, MEM
      if (next->getRegisterOperand(1) != ((TR::S390RRInstruction*)next)->getRegisterOperand(2)) return false;
      isICMOpportunity = true;
      }
   else
      {
      // CHI Opportunty:
      //      L  GPRX,..
      //      CHI  GPRX, '0'
      // reduce to:
      //      ICM GPRX,...
      TR::S390RIInstruction* chi = (TR::S390RIInstruction*) next;

      // CHI must be comparing against '0' (i.e. NULLCHK) or else our
      // condition codes will be wrong.
      if (chi->getSourceImmediate() != 0) return false;

      isICMOpportunity = true;
      }

   if (isICMOpportunity && performTransformation(comp(), "\nO^O S390 PEEPHOLE: Reducing L [%p] being reduced to ICM.\n", cursor))
      {
      // Prevent reuse of memory reference
      TR::MemoryReference* memcp = generateS390MemoryReference(*load->getMemoryReference(), 0, cg());

      if ((memcp->getBaseRegister() == NULL) &&
          (memcp->getIndexRegister() != NULL))
         {
         memcp->setBaseRegister(memcp->getIndexRegister(), cg());
         memcp->setIndexRegister(0);
         }

      // Do the reduction - create the icm instruction
      TR::S390RSInstruction* icm = new (cg()->trHeapMemory()) TR::S390RSInstruction(TR::InstOpCode::ICM, load->getNode(), load->getRegisterOperand(1), 0xF, memcp, prev, cg());

      // Check if the load has an implicit NULLCHK.  If so, we need to ensure a GCmap is copied.
      if (load->throwsImplicitNullPointerException())
         {
         icm->setNeedsGCMap(0x0000FFFF);
         icm->setThrowsImplicitNullPointerException();
         icm->setGCMap(load->getGCMap());

         TR_Debug * debugObj = cg()->getDebug();
         if (debugObj)
            debugObj->addInstructionComment(icm, "Throws Implicit Null Pointer Exception");
         }

      cg()->replaceInst(load, icm);
      cg()->deleteInst(next);

      memcp->stopUsingMemRefRegister(cg());
      performed = true;
      }

   return performed;
   }

bool
OMR::Z::Peephole::attemptToReduceLLCToLLGC(TR::Instruction* cursor)
   {
   TR::Instruction *current = cursor->getNext();
   auto mnemonic = current->getOpCodeValue();

   if (mnemonic == TR::InstOpCode::LGFR || mnemonic == TR::InstOpCode::LLGTR)
      {
      TR::Register *llcTgtReg = ((TR::S390RRInstruction *) cursor)->getRegisterOperand(1);

      TR::Register *curSrcReg = ((TR::S390RRInstruction *) current)->getRegisterOperand(2);
      TR::Register *curTgtReg = ((TR::S390RRInstruction *) current)->getRegisterOperand(1);

      if (llcTgtReg == curSrcReg && llcTgtReg == curTgtReg)
         {
         if (performTransformation(comp(), "O^O S390 PEEPHOLE: Reducing LLC/%s [%p] to LLGC.\n", TR::InstOpCode::metadata[mnemonic].name, current))
            {
            // Remove the LGFR/LLGTR
            cg()->deleteInst(current);

            // Replace the LLC with LLGC
            TR::MemoryReference* memRef = ((TR::S390RXInstruction *) cursor)->getMemoryReference();
            memRef->resetMemRefUsedBefore();
            auto llgcInst = generateRXInstruction(cg(), TR::InstOpCode::LLGC, comp()->getStartTree()->getNode(), llcTgtReg, memRef, cursor->getPrev());
            cg()->replaceInst(cursor, llgcInst);
            
            return true;
            }
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

bool
OMR::Z::Peephole::attemptToRemoveRedundantCompareAndTrap(TR::Instruction* cursor)
   {
   if (comp()->target().isZOS())
      {
      // CLT cannot do the job in zOS because in zOS it is legal to read low memory address (like 0x000000, literally NULL),
      // and CLT will read the low memory address legally (in this case NULL) to compare it with the other operand.
      // The result will not make sense since we should trap for NULLCHK first.
      return false;
      }

   int32_t windowSize=0;
   const int32_t maxWindowSize=8;
   static char *disableRemoveMergedNullCHK = feGetEnv("TR_DisableRemoveMergedNullCHK");

   if (disableRemoveMergedNullCHK != NULL) return false;

   TR::Instruction * cgitInst = cursor;
   TR::Instruction * current = cgitInst->getNext();

   cgitInst->setUseDefRegisters(false);
   TR::Register * cgitSource = cgitInst->getSourceRegister(0);

   while ((current != NULL) &&
         !isBarrierToPeepHoleLookback(current) &&
         windowSize<maxWindowSize)
      {
      // do not look across Transactional Regions, the Register save mask is optimistic and does not allow renaming
      if (current->getOpCodeValue() == TR::InstOpCode::TBEGIN ||
          current->getOpCodeValue() == TR::InstOpCode::TBEGINC ||
          current->getOpCodeValue() == TR::InstOpCode::TEND ||
          current->getOpCodeValue() == TR::InstOpCode::TABORT)
         {
         return false;
         }

      if (current->getOpCodeValue() == TR::InstOpCode::CLT)
          {
          TR::Instruction * cltInst = current;
          cltInst->setUseDefRegisters(false);
          TR::Register * cltSource = cltInst->getSourceRegister(0);
          TR::Register * cltSource2 = cltInst->getSourceRegister(1);
          if (!cgitSource || !cltSource || !cltSource2)
             return false;
          if (toRealRegister(cltSource2)->getRegisterNumber() == toRealRegister(cgitSource)->getRegisterNumber())
             {
             if (performTransformation(comp(), "\nO^O S390 PEEPHOLE: removeMergedNullCHK on 0x%p.\n", cgitInst))
                {
                cg()->deleteInst(cgitInst);

                return true;
                }
             }
          }
       else if (current->usesRegister(cgitSource))
          {
          return false;
          }

      current = current->getNext() == NULL ? NULL : current->getNext();
      windowSize++;
      }

   return false;
   }

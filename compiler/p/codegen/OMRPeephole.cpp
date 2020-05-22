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

#include "codegen/Peephole.hpp"

#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateOMRInstructions.hpp"
#include "codegen/Instruction.hpp"
#include "p/codegen/PPCInstruction.hpp"

static bool
isWAWOrmrPeepholeCandidateInstr(TR::Instruction *instr)
   {
   if (instr != NULL && !instr->willBePatched() &&
      (!instr->isLabel() ||
       (instr->getNode()->getOpCodeValue() == TR::BBStart &&
        instr->getNode()->getBlock()->isExtensionOfPreviousBlock())) &&
      //FIXME: properly -- defs/refs of instructions with Mem are not quite right.
       (instr->getMemoryReference()==NULL ||
        instr->getMemoryReference()->getUnresolvedSnippet()==NULL))
      {
      return true;
      }
   return false;
   }

OMR::Power::Peephole::Peephole(TR::Compilation* comp) :
   OMR::Peephole(comp)
   {}

bool
OMR::Power::Peephole::performOnInstruction(TR::Instruction* cursor)
   {
   bool performed = false;

   // Cache the cursor for use in the peephole functions
   self()->cursor = cursor;

   switch(cursor->getOpCodeValue())
      {
      case TR::InstOpCode::stw:
#if defined(TR_HOST_64BIT)
      case TR::InstOpCode::std:
#endif
         {
         performed |= tryToRemoveRedundantLoadAfterStore();
         break;
         }
      default:
         {
         performed |= tryToRemoveRedundantWriteAfterWrite();
         break;
         }
      }

   return performed;
   }

bool
OMR::Power::Peephole::tryToRemoveRedundantLoadAfterStore()
   {
   static bool disableLHSPeephole = feGetEnv("TR_DisableLHSPeephole") != NULL;
   if (disableLHSPeephole)
      return false;

   TR::Instruction *storeInstruction = cursor;
   TR::Instruction   *loadInstruction = storeInstruction->getNext();
   TR::InstOpCode&      storeOp = storeInstruction->getOpCode();
   TR::InstOpCode&      loadOp = loadInstruction->getOpCode();
   TR::MemoryReference *storeMemRef = ((TR::PPCMemSrc1Instruction *)storeInstruction)->getMemoryReference();
   TR::MemoryReference *loadMemRef = ((TR::PPCTrg1MemInstruction *)loadInstruction)->getMemoryReference();

   // Next instruction has to be a load,
   // the store and load have to agree on size,
   // they have to both be GPR ops or FPR ops,
   // neither can be the update form,
   // and they both have to be resolved.
   // TODO: Relax the first constraint a bit and handle cases were unrelated instructions appear between the store and the load
   if (!loadOp.isLoad() ||
       storeMemRef->getLength() != loadMemRef->getLength() ||
       !(storeOp.gprOp() == loadOp.gprOp() ||
         storeOp.fprOp() == loadOp.fprOp()) ||
       storeOp.isUpdate() ||
       loadOp.isUpdate() ||
       storeMemRef->getUnresolvedSnippet() ||
       loadMemRef->getUnresolvedSnippet())
      {
      return false;
      }

   // TODO: Don't bother with indexed loads/stores for now, fix them later
   if (storeMemRef->getIndexRegister() || loadMemRef->getIndexRegister())
      {
      return false;
      }

   // The store and load mem refs have to agree on whether or not they need delayed offsets
   if (storeMemRef->hasDelayedOffset() != loadMemRef->hasDelayedOffset())
      {
      // Memory references that use delayed offsets will not have the correct offset
      // at this point, so we can't compare their offsets to the offsets used by regular references,
      // however we can still compare two mem refs that both use delayed offsets.
      return false;
      }

   // The store and load have to use the same base and offset
   if (storeMemRef->getBaseRegister() != loadMemRef->getBaseRegister() ||
       storeMemRef->getOffset(*comp()) != loadMemRef->getOffset(*comp()))
      {
      return false;
      }

   TR::Register *srcReg = ((TR::PPCMemSrc1Instruction *)storeInstruction)->getSourceRegister();
   TR::Register *trgReg = ((TR::PPCTrg1MemInstruction *)loadInstruction)->getTargetRegister();

   if (srcReg == trgReg)
      {
      // Found the pattern:
      //   stw/std rX, ...
      //   lwz/ld rX, ...
      // will remove ld
      // and replace lwz with rlwinm, rX, rX, 0, 0xffffffff
      if (loadInstruction->getOpCodeValue() == TR::InstOpCode::lwz)
         {
         if (performTransformation(comp(), "O^O PPC PEEPHOLE: Replace lwz " POINTER_PRINTF_FORMAT " with rlwinm after store " POINTER_PRINTF_FORMAT ".\n", loadInstruction, storeInstruction))
            {
            generateTrg1Src1Imm2Instruction(cg(), TR::InstOpCode::rlwinm, loadInstruction->getNode(), trgReg, srcReg, 0, 0xffffffff, storeInstruction);
            loadInstruction->remove();
            return true;
            }
         }
      else if (performTransformation(comp(), "O^O PPC PEEPHOLE: Remove redundant load " POINTER_PRINTF_FORMAT " after store " POINTER_PRINTF_FORMAT ".\n", loadInstruction, storeInstruction))
         {
         loadInstruction->remove();
         return true;
         }

      return false;
      }

   // Found the pattern:
   //   stw/std rX, ...
   //   lwz/ld rY, ...
   // and will remove lwz, which will result in:
   //   stw rX, ...
   //   rlwinm rY, rX, 0, 0xffffffff
   // or will remove ld, which will result in:
   //   std rX, ...
   //   mr rY, rX
   // and then the mr peephole should run on the resulting mr
   if (loadInstruction->getOpCodeValue() == TR::InstOpCode::lwz)
      {
      if (performTransformation(comp(), "O^O PPC PEEPHOLE: Replace redundant load " POINTER_PRINTF_FORMAT " after store " POINTER_PRINTF_FORMAT " with rlwinm.\n", loadInstruction, storeInstruction))
         {
         generateTrg1Src1Imm2Instruction(cg(), TR::InstOpCode::rlwinm, loadInstruction->getNode(), trgReg, srcReg, 0, 0xffffffff, storeInstruction);
         loadInstruction->remove();
         return true;
         }
      }
   else if (performTransformation(comp(), "O^O PPC PEEPHOLE: Replace redundant load " POINTER_PRINTF_FORMAT " after store " POINTER_PRINTF_FORMAT " with mr.\n", loadInstruction, storeInstruction))
      {
      generateTrg1Src1Instruction(cg(), TR::InstOpCode::mr, loadInstruction->getNode(), trgReg, srcReg, storeInstruction);
      loadInstruction->remove();
      return true;
      }

   return false;
   }

bool
OMR::Power::Peephole::tryToRemoveRedundantWriteAfterWrite()
   {
   static bool disableWAWPeephole = feGetEnv("TR_DisableWAWPeephole") != NULL;
   if (disableWAWPeephole)
      return false;

   int32_t window = 0;
   int32_t maxWindowSize = comp()->isOptServer() ? 20 : 10;

   // Get and check if there is a target register
   TR::Register *currTargetReg = cursor->getTargetRegister(0);

   if (currTargetReg && currTargetReg->getKind() != TR_CCR && isWAWOrmrPeepholeCandidateInstr(cursor) &&
       !cursor->isBranchOp() && !cursor->isCall() &&
       !cursor->getOpCode().isRecordForm() &&
       !cursor->getOpCode().setsCarryFlag())
      {
      TR::Instruction *current = cursor->getNext();

      while (isWAWOrmrPeepholeCandidateInstr(current) &&
             !current->isBranchOp() &&
             !current->isCall() &&
             window < maxWindowSize)
         {
         if (current->getOpCode().usesTarget())
            return false;

         switch(current->getKind())
            {
            case OMR::Instruction::IsSrc1:
            case OMR::Instruction::IsTrg1Src1:
            case OMR::Instruction::IsTrg1Src1Imm:
            case OMR::Instruction::IsTrg1Src1Imm2:
               if (currTargetReg == current->getSourceRegister(0))
                  return false;
               break;
            case OMR::Instruction::IsSrc2:
            case OMR::Instruction::IsTrg1Src2:
            case OMR::Instruction::IsTrg1Src2Imm:
            case OMR::Instruction::IsMem:
            case OMR::Instruction::IsTrg1Mem:
               if (currTargetReg == current->getSourceRegister(0) ||
                   currTargetReg == current->getSourceRegister(1))
                  return false;
               break;
            case OMR::Instruction::IsTrg1Src3:
            case OMR::Instruction::IsMemSrc1:
               if (currTargetReg == current->getSourceRegister(0) ||
                   currTargetReg == current->getSourceRegister(1) ||
                   currTargetReg == current->getSourceRegister(2))
                  return false;
               break;
            default:
                  return false;
            }

         if (current->getTargetRegister(0) == currTargetReg)
            {
            TR::Instruction *q[4];
            // In case the instruction is part of the requestor
            bool isConstData = cg()->checkAndFetchRequestor(cursor, q);
            if (isConstData)
               {
               if (performTransformation(comp(), "O^O PPC PEEPHOLE: Remove dead instrcution group from WAW %p -> %p.\n", cursor, current))
                  {
                  for (int32_t i = 0; i < 4; i++)
                     {
                     if (q[i])  q[i]->remove();
                     }

                  return true;
                  }
               }
            else
               {
               if (performTransformation(comp(), "O^O PPC PEEPHOLE: Remove dead instrcution from WAW %p -> %p.\n", cursor, current))
                  {
                  cursor->remove();
                  return true;
                  }
               }

            return false;
            }

         current = current->getNext();
         window++;
         }
      }

   return false;
   }
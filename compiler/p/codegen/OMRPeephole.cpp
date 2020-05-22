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
#include "codegen/Instruction.hpp"

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
      default:
         {
         performed |= tryToRemoveRedundantWriteAfterWrite();
         break;
         }
      }

   return performed;
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
/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

#include <stddef.h> // for NULL, etc
#include <stdint.h> // for uint16_t, int32_t, etc

#include "codegen/CodeGenerator.hpp"
#include "codegen/Machine.hpp"
#include "codegen/Machine_inlines.hpp"
#include "codegen/RealRegister.hpp"
#include "infra/Assert.hpp"

OMR::ARM64::Machine::Machine(TR::CodeGenerator *cg) :
   OMR::Machine(cg, NUM_ARM64_GPR, NUM_ARM64_FPR)
   {
   _registerFile = (TR::RealRegister **)cg->trMemory()->allocateMemory(sizeof(TR::RealRegister *)*TR::RealRegister::NumRegisters, heapAlloc);
   self()->initialiseRegisterFile();
   }

TR::RealRegister *OMR::ARM64::Machine::findBestFreeRegister(TR_RegisterKinds rk,
                                                            bool considerUnlatched)
   {
   int32_t first;
   int32_t last;

   switch(rk)
      {
      case TR_GPR:
         first = TR::RealRegister::FirstGPR;
         last  = TR::RealRegister::LastAssignableGPR;
         break;
      case TR_FPR:
         first = TR::RealRegister::FirstFPR;
         last  = TR::RealRegister::LastFPR;
         break;
      default:
         TR_ASSERT(false, "Unsupported RegisterKind.");
   }

   uint32_t bestWeightSoFar = 0xffffffff;
   TR::RealRegister *freeRegister = NULL;
   for (int32_t i = first; i <= last; i++)
      {
      if ((_registerFile[i]->getState() == TR::RealRegister::Free ||
           (considerUnlatched &&
            _registerFile[i]->getState() == TR::RealRegister::Unlatched)) &&
          _registerFile[i]->getWeight() < bestWeightSoFar)
         {
         freeRegister    = _registerFile[i];
         bestWeightSoFar = freeRegister->getWeight();
         }
      }
   if (freeRegister != NULL && freeRegister->getState() == TR::RealRegister::Unlatched)
      {
      freeRegister->setAssignedRegister(NULL);
      freeRegister->setState(TR::RealRegister::Free);
      }
   return freeRegister;
   }

TR::RealRegister *OMR::ARM64::Machine::freeBestRegister(TR::Instruction *currentInstruction,
                                                        TR::Register *virtualRegister,
                                                        TR::RealRegister *forced)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

TR::RealRegister *OMR::ARM64::Machine::reverseSpillState(TR::Instruction *currentInstruction,
                                                     TR::Register *spilledRegister,
                                                     TR::RealRegister *targetRegister)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

TR::RealRegister *OMR::ARM64::Machine::assignOneRegister(TR::Instruction *currentInstruction,
                                                         TR::Register *virtualRegister)
   {
   TR_RegisterKinds rk = virtualRegister->getKind();
   TR::RealRegister *assignedRegister;

   self()->cg()->clearRegisterAssignmentFlags();
   self()->cg()->setRegisterAssignmentFlag(TR_NormalAssignment);

   if (virtualRegister->getTotalUseCount() != virtualRegister->getFutureUseCount())
      {
      self()->cg()->setRegisterAssignmentFlag(TR_RegisterReloaded);
      assignedRegister = self()->reverseSpillState(currentInstruction, virtualRegister, NULL);
      }
   else
      {
      assignedRegister = self()->findBestFreeRegister(rk, virtualRegister);
      if (assignedRegister == NULL)
         {
         self()->cg()->setRegisterAssignmentFlag(TR_RegisterSpilled);
         assignedRegister = self()->freeBestRegister(currentInstruction, virtualRegister, NULL);
         }
      }

   virtualRegister->setAssignedRegister(assignedRegister);
   assignedRegister->setAssignedRegister(virtualRegister);
   assignedRegister->setState(TR::RealRegister::Assigned);
   self()->cg()->traceRegAssigned(virtualRegister, assignedRegister);
   return assignedRegister;
   }

void OMR::ARM64::Machine::coerceRegisterAssignment(TR::Instruction *currentInstruction,
                                                   TR::Register *virtualRegister,
                                                   TR::RealRegister::RegNum registerNumber)
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void OMR::ARM64::Machine::initialiseRegisterFile()
   {
   _registerFile[TR::RealRegister::NoReg] = NULL;
   _registerFile[TR::RealRegister::SpilledReg] = NULL;

   _registerFile[TR::RealRegister::x0] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x0,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x1] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x1,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x2] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x2,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x3] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x3,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x4] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x4,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x5] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x5,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x6] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x6,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x7] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x7,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x8] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x8,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x9] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x9,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x10] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x10,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x11] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x11,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x12] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x12,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x13] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x13,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x14] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x14,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x15] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x15,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x16] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x16,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x17] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x17,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x18] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x18,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x19] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x19,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x20] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x20,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x21] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x21,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x22] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x22,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x23] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x23,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x24] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x24,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x25] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x25,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x26] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x26,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x27] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x27,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x28] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x28,
                                                 self()->cg());

   _registerFile[TR::RealRegister::x29] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::x29,
                                                 self()->cg());

   /* x30 is used as LR on ARM64 */
   _registerFile[TR::RealRegister::lr] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_GPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::lr,
                                                 self()->cg());

   /* x31 is unavailable as GPR on ARM64 */

   _registerFile[TR::RealRegister::v0] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v0,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v1] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v1,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v2] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v2,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v3] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v3,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v4] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v4,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v5] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v5,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v6] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v6,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v7] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v7,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v8] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v8,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v9] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v9,
                                                 self()->cg());
 
   _registerFile[TR::RealRegister::v10] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v10,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v11] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v11,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v12] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v12,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v13] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v13,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v14] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v14,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v15] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v15,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v16] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v16,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v17] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v17,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v18] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v18,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v19] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v19,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v20] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v20,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v21] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v21,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v22] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v22,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v23] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v23,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v24] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v24,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v25] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v25,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v26] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v26,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v27] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v27,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v28] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v28,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v29] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v29,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v30] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v30,
                                                 self()->cg());

   _registerFile[TR::RealRegister::v31] = new (self()->cg()->trHeapMemory()) TR::RealRegister(TR_FPR,
                                                 0,
                                                 TR::RealRegister::Free,
                                                 TR::RealRegister::v31,
                                                 self()->cg());
   }

void
OMR::ARM64::Machine::takeRegisterStateSnapShot()
   {
   int32_t i;
   for (i = TR::RealRegister::FirstGPR; i < TR::RealRegister::NumRegisters - 1; i++)
      {
      _registerStatesSnapShot[i] = _registerFile[i]->getState();
      _assignedRegisterSnapShot[i] = _registerFile[i]->getAssignedRegister();
      _registerFlagsSnapShot[i] = _registerFile[i]->getFlags();
      }
   }

void
OMR::ARM64::Machine::restoreRegisterStateFromSnapShot()
   {
   int32_t i;
   for (i = TR::RealRegister::FirstGPR; i < TR::RealRegister::NumRegisters - 1; i++) // Skipping SpilledReg
      {
      _registerFile[i]->setFlags(_registerFlagsSnapShot[i]);
      _registerFile[i]->setState(_registerStatesSnapShot[i]);
      if (_registerFile[i]->getState() == TR::RealRegister::Free)
         {
         if (_registerFile[i]->getAssignedRegister() != NULL)
            {
            // clear the Virt -> Real reg assignment if we restored the Real reg state to FREE
            _registerFile[i]->getAssignedRegister()->setAssignedRegister(NULL);
            }
         }
      _registerFile[i]->setAssignedRegister(_assignedRegisterSnapShot[i]);
      // make sure to double link virt - real reg if assigned
      if (_registerFile[i]->getState() == TR::RealRegister::Assigned)
         {
         _registerFile[i]->getAssignedRegister()->setAssignedRegister(_registerFile[i]);
         }
      // Don't restore registers that died after the snapshot was taken since they are guaranteed to not be used in the outlined path
      if (_registerFile[i]->getState() == TR::RealRegister::Assigned &&
          _registerFile[i]->getAssignedRegister()->getFutureUseCount() == 0)
         {
         _registerFile[i]->setState(TR::RealRegister::Free);
         _registerFile[i]->getAssignedRegister()->setAssignedRegister(NULL);
         _registerFile[i]->setAssignedRegister(NULL);
         }
      }
   }

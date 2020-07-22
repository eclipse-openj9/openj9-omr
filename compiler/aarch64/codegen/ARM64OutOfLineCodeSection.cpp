/*******************************************************************************
 * Copyright (c) 2018, 2020 IBM Corp. and others
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

#include "codegen/ARM64OutOfLineCodeSection.hpp"
#include "codegen/BackingStore.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Machine.hpp"
#include "codegen/Register.hpp"

TR_ARM64OutOfLineCodeSection::TR_ARM64OutOfLineCodeSection(TR::Node *callNode,
                            TR::ILOpCodes callOp,
                            TR::Register *targetReg,
                            TR::LabelSymbol *entryLabel,
                            TR::LabelSymbol *restartLabel,
                            TR::CodeGenerator *cg) :
                            TR_OutOfLineCodeSection(callNode, callOp, targetReg, entryLabel, restartLabel, cg)
   {
   generateARM64OutOfLineCodeSectionDispatch();
   }

void TR_ARM64OutOfLineCodeSection::assignRegisters(TR_RegisterKinds kindsToBeAssigned)
   {
   TR::Compilation *comp = _cg->comp();
   if (hasBeenRegisterAssigned())
      return;
   // nested internal control flow assert:
   _cg->setInternalControlFlowSafeNestingDepth(_cg->internalControlFlowNestingDepth());

   // Create a dependency list on the first instruction in this stream that captures all current real register associations.
   // This is necessary to get the register assigner back into its original state before the helper stream was processed.
   //
   _cg->incOutOfLineColdPathNestedDepth();

   // this is to prevent the OOL entry label from resetting all register's startOfrange during RA
   _cg->toggleIsInOOLSection();
   TR::RegisterDependencyConditions *liveRealRegDeps = _cg->machine()->createDepCondForLiveGPRs(_cg->getSpilledRegisterList());

   if (liveRealRegDeps)
      {
      _firstInstruction->setDependencyConditions(liveRealRegDeps);
      liveRealRegDeps->bookKeepingRegisterUses(_firstInstruction, _cg);
      }
   _cg->toggleIsInOOLSection(); // toggle it back because there is another toggle inside swapInstructionListsWithCompilation()


   // Register assign the helper dispatch instructions.
   swapInstructionListsWithCompilation();
   _cg->doRegisterAssignment(kindsToBeAssigned);

   TR::list<TR::Register*> *firstTimeLiveOOLRegisterList = _cg->getFirstTimeLiveOOLRegisterList();
   TR::list<TR::Register*> *spilledRegisterList = _cg->getSpilledRegisterList();

   for (auto li = firstTimeLiveOOLRegisterList->begin(); li != firstTimeLiveOOLRegisterList->end(); ++li)
      {
      if ((*li)->getBackingStorage())
         {
         (*li)->getBackingStorage()->setMaxSpillDepth(1);
         traceMsg(comp,"Adding virtReg:%s from _firstTimeLiveOOLRegisterList to _spilledRegisterList \n", _cg->getDebug()->getName((*li)));
         spilledRegisterList->push_front((*li));
         }
      }
   _cg->getFirstTimeLiveOOLRegisterList()->clear();

   swapInstructionListsWithCompilation();

   _cg->decOutOfLineColdPathNestedDepth();

   // Returning to mainline, reset this counter
   _cg->setInternalControlFlowSafeNestingDepth(0);

   // Link in the helper stream into the mainline code.
   // We will end up with the OOL items attached at the bottom of the instruction stream
   TR::Instruction *appendInstruction = _cg->getAppendInstruction();
   appendInstruction->setNext(_firstInstruction);
   _firstInstruction->setPrev(appendInstruction);
   _cg->setAppendInstruction(_appendInstruction);

   setHasBeenRegisterAssigned(true);
   }

void TR_ARM64OutOfLineCodeSection::generateARM64OutOfLineCodeSectionDispatch()
   {
   // Switch to cold helper instruction stream.
   //
   swapInstructionListsWithCompilation();

   TR::Instruction *entryLabelInstruction = generateLabelInstruction(_cg, TR::InstOpCode::label, _callNode, _entryLabel);

   _cg->incOutOfLineColdPathNestedDepth();
   TR_Debug *debugObj = _cg->getDebug();
   if (debugObj)
     {
     debugObj->addInstructionComment(entryLabelInstruction, "Denotes start of OOL sequence");
     }

   TR::Register *resultReg = TR::TreeEvaluator::performCall(_callNode, _callNode->getOpCode().isCallIndirect(), _cg);

   if (_targetReg)
      {
      TR_ASSERT(resultReg, "resultReg must not be a NULL");
      generateMovInstruction(_cg, _callNode, _targetReg, resultReg);
      }
   _cg->decReferenceCount(_callNode);

   TR::Instruction *returnBranchInstruction = generateLabelInstruction(_cg, TR::InstOpCode::b, _callNode, _restartLabel);

   if (debugObj)
     {
     debugObj->addInstructionComment(returnBranchInstruction, "Denotes end of OOL: return to mainline");
     }

   _cg->decOutOfLineColdPathNestedDepth();

   // Switch from cold helper instruction stream.
   swapInstructionListsWithCompilation();
   }

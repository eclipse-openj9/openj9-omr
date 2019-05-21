/*******************************************************************************
 * Copyright (c) 2018, 2019 IBM Corp. and others
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

#include <stdlib.h>

#include "codegen/ARM64Instruction.hpp"
#include "codegen/ARM64SystemLinkage.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/CodeGenerator_inlines.hpp"
#include "codegen/GCStackMap.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterIterator.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"

OMR::ARM64::CodeGenerator::CodeGenerator() :
      OMR::CodeGenerator(),
      _constantData(NULL),
      _outOfLineCodeSectionList(getTypedAllocator<TR_ARM64OutOfLineCodeSection*>(self()->comp()->allocator()))
   {
   // Initialize Linkage for Code Generator
   self()->initializeLinkage();

   _unlatchedRegisterList =
      (TR::RealRegister**)self()->trMemory()->allocateHeapMemory(sizeof(TR::RealRegister*)*(TR::RealRegister::NumRegisters + 1));

   _unlatchedRegisterList[0] = 0; // mark that list is empty

   _linkageProperties = &self()->getLinkage()->getProperties();

   self()->setStackPointerRegister(self()->machine()->getRealRegister(_linkageProperties->getStackPointerRegister()));
   self()->setMethodMetaDataRegister(self()->machine()->getRealRegister(_linkageProperties->getMethodMetaDataRegister()));

   // Tactical GRA settings
   //
   self()->setGlobalRegisterTable(TR::Machine::getGlobalRegisterTable());
   _numGPR = _linkageProperties->getNumAllocatableIntegerRegisters();
   _numFPR = _linkageProperties->getNumAllocatableFloatRegisters();
   self()->setLastGlobalGPR(TR::Machine::getLastGlobalGPRRegisterNumber());
   self()->setLastGlobalFPR(TR::Machine::getLastGlobalFPRRegisterNumber());

   self()->getLinkage()->initARM64RealRegisterLinkage();

   _numberBytesReadInaccessible = 0;
   _numberBytesWriteInaccessible = 0;

   if (TR::Compiler->vm.hasResumableTrapHandler(self()->comp()))
      self()->setHasResumableTrapHandler();

   if (!self()->comp()->getOption(TR_DisableRegisterPressureSimulation))
      {
      for (int32_t i = 0; i < TR_numSpillKinds; i++)
         _globalRegisterBitVectors[i].init(self()->getNumberOfGlobalRegisters(), self()->trMemory());

      for (TR_GlobalRegisterNumber grn=0; grn < self()->getNumberOfGlobalRegisters(); grn++)
         {
         TR::RealRegister::RegNum reg = (TR::RealRegister::RegNum)self()->getGlobalRegister(grn);
         if (self()->getFirstGlobalGPR() <= grn && grn <= self()->getLastGlobalGPR())
            _globalRegisterBitVectors[ TR_gprSpill ].set(grn);
         else if (self()->getFirstGlobalFPR() <= grn && grn <= self()->getLastGlobalFPR())
            _globalRegisterBitVectors[ TR_fprSpill ].set(grn);

         if (!self()->getProperties().getPreserved(reg))
            _globalRegisterBitVectors[ TR_volatileSpill ].set(grn);
         if (self()->getProperties().getIntegerArgument(reg) || self()->getProperties().getFloatArgument(reg))
            _globalRegisterBitVectors[ TR_linkageSpill  ].set(grn);
         }
      }

   // Calculate inverse of getGlobalRegister function
   //
   TR_GlobalRegisterNumber grn;
   int i;

   TR_GlobalRegisterNumber globalRegNumbers[TR::RealRegister::NumRegisters];
   for (i = 0; i < self()->getNumberOfGlobalGPRs(); i++)
     {
     grn = self()->getFirstGlobalGPR() + i;
     globalRegNumbers[self()->getGlobalRegister(grn)] = grn;
     }
   for (i = 0; i < self()->getNumberOfGlobalFPRs(); i++)
     {
     grn = self()->getFirstGlobalFPR() + i;
     globalRegNumbers[self()->getGlobalRegister(grn)] = grn;
     }

   // Initialize linkage reg arrays
   TR::ARM64LinkageProperties linkageProperties = self()->getProperties();
   for (i = 0; i < linkageProperties.getNumIntArgRegs(); i++)
     _gprLinkageGlobalRegisterNumbers[i] = globalRegNumbers[linkageProperties.getIntegerArgumentRegister(i)];
   for (i = 0; i < linkageProperties.getNumFloatArgRegs(); i++)
     _fprLinkageGlobalRegisterNumbers[i] = globalRegNumbers[linkageProperties.getFloatArgumentRegister(i)];

   if (self()->comp()->getOptions()->getRegisterAssignmentTraceOption(TR_TraceRARegisterStates))
      {
      self()->setGPRegisterIterator(new (self()->trHeapMemory()) TR::RegisterIterator(self()->machine(), TR::RealRegister::FirstGPR, TR::RealRegister::LastGPR));
      self()->setFPRegisterIterator(new (self()->trHeapMemory()) TR::RegisterIterator(self()->machine(), TR::RealRegister::FirstFPR, TR::RealRegister::LastFPR));
      }
   }

void
OMR::ARM64::CodeGenerator::beginInstructionSelection()
   {
   TR::Compilation *comp = self()->comp();
   TR::Node *startNode = comp->getStartTree()->getNode();
   if (comp->getMethodSymbol()->getLinkageConvention() == TR_Private)
      {
      TR_UNIMPLEMENTED();

      _returnTypeInfoInstruction = new (self()->trHeapMemory()) TR::ARM64ImmInstruction(TR::InstOpCode::dd, startNode, 0, self());
      }
   else
      {
      _returnTypeInfoInstruction = NULL;
      }

   new (self()->trHeapMemory()) TR::ARM64AdminInstruction(TR::InstOpCode::proc, startNode, NULL, _returnTypeInfoInstruction, self());
   }

void
OMR::ARM64::CodeGenerator::endInstructionSelection()
   {
   if (_returnTypeInfoInstruction != NULL)
      {
      _returnTypeInfoInstruction->setSourceImmediate(static_cast<uint32_t>(self()->comp()->getReturnInfo()));
      }
   }

void
OMR::ARM64::CodeGenerator::doRegisterAssignment(TR_RegisterKinds kindsToAssign)
   {
   // Registers are assigned in backward direction

   TR::Compilation *comp = self()->comp();

   TR::Instruction *instructionCursor = self()->getAppendInstruction();

   if (!comp->getOption(TR_DisableOOL))
      {
      TR::list<TR::Register*> *spilledRegisterList = new (self()->trHeapMemory()) TR::list<TR::Register*>(getTypedAllocator<TR::Register*>(comp->allocator()));
      self()->setSpilledRegisterList(spilledRegisterList);
      }

   if (self()->getDebug())
      self()->getDebug()->startTracingRegisterAssignment();

   while (instructionCursor)
      {
      TR::Instruction *prevInstruction = instructionCursor->getPrev();

      self()->tracePreRAInstruction(instructionCursor);

      self()->setCurrentBlockIndex(instructionCursor->getBlockIndex());

      instructionCursor->assignRegisters(TR_GPR);

      // Maintain Internal Control Flow Depth
      // Track internal control flow on labels
      if (instructionCursor->isLabel())
         {
         TR::ARM64LabelInstruction *li = (TR::ARM64LabelInstruction *)instructionCursor;

         if (li->getLabelSymbol() != NULL)
            {
            if (li->getLabelSymbol()->isStartInternalControlFlow())
               {
               self()->decInternalControlFlowNestingDepth();
               }
            if (li->getLabelSymbol()->isEndInternalControlFlow())
               {
               self()->incInternalControlFlowNestingDepth();
               }
            }
         }

      self()->freeUnlatchedRegisters();
      self()->buildGCMapsForInstructionAndSnippet(instructionCursor);

      self()->tracePostRAInstruction(instructionCursor);

      instructionCursor = prevInstruction;
      }

   if (self()->getDebug())
      {
      self()->getDebug()->stopTracingRegisterAssignment();
      }
   }

void
OMR::ARM64::CodeGenerator::doBinaryEncoding()
   {
   TR::Compilation *comp = self()->comp();
   int32_t estimate = 0;
   TR::Instruction *cursorInstruction = self()->getFirstInstruction();

   self()->getLinkage()->createPrologue(cursorInstruction);

   bool skipOneReturn = false;
   while (cursorInstruction)
      {
      if (cursorInstruction->getOpCodeValue() == TR::InstOpCode::retn)
         {
         if (skipOneReturn == false)
            {
            TR::Instruction *temp = cursorInstruction->getPrev();
            self()->getLinkage()->createEpilogue(temp);
            cursorInstruction = temp->getNext();
            skipOneReturn = true;
            }
         else
            {
            skipOneReturn = false;
            }
         }
      estimate = cursorInstruction->estimateBinaryLength(estimate);
      cursorInstruction = cursorInstruction->getNext();
      }

   estimate = self()->setEstimatedLocationsForSnippetLabels(estimate);

   self()->setEstimatedCodeLength(estimate);

   cursorInstruction = self()->getFirstInstruction();
   uint8_t *coldCode = NULL;
   uint8_t *temp = self()->allocateCodeMemory(self()->getEstimatedCodeLength(), 0, &coldCode);

   self()->setBinaryBufferStart(temp);
   self()->setBinaryBufferCursor(temp);
   self()->alignBinaryBufferCursor();

   while (cursorInstruction)
      {
      self()->setBinaryBufferCursor(cursorInstruction->generateBinaryEncoding());
      cursorInstruction = cursorInstruction->getNext();
      }
   }

TR::Linkage *OMR::ARM64::CodeGenerator::createLinkage(TR_LinkageConventions lc)
   {
   TR::Linkage *linkage;
   switch (lc)
      {
      case TR_System:
         linkage = new (self()->trHeapMemory()) TR::ARM64SystemLinkage(self());
         break;
      default:
         TR_ASSERT(false, "using system linkage for unrecognized convention %d\n", lc);
         linkage = new (self()->trHeapMemory()) TR::ARM64SystemLinkage(self());
      }
   self()->setLinkage(lc, linkage);
   return linkage;
   }

void OMR::ARM64::CodeGenerator::emitDataSnippets()
   {
   TR_UNIMPLEMENTED();
   /*
    * Commented out until TR::ConstantDataSnippet is implemented
   self()->setBinaryBufferCursor(_constantData->emitSnippetBody());
    */
   }

bool OMR::ARM64::CodeGenerator::hasDataSnippets()
   {
   return (_constantData == NULL) ? false : true;
   }

int32_t OMR::ARM64::CodeGenerator::setEstimatedLocationsForDataSnippetLabels(int32_t estimatedSnippetStart)
   {
   TR_UNIMPLEMENTED();
   return 0;
   /*
    * Commented out until TR::ConstantDataSnippet is implemented
   return estimatedSnippetStart + _constantData->getLength();
    */
   }


#ifdef DEBUG
void OMR::ARM64::CodeGenerator::dumpDataSnippets(TR::FILE *outFile)
   {
   if (outFile == NULL)
      return;

   TR_UNIMPLEMENTED();
   /*
    * Commented out until TR::ConstantDataSnippet is implemented
   _constantData->print(outFile);
    */
   }
#endif


TR::Instruction *OMR::ARM64::CodeGenerator::generateSwitchToInterpreterPrePrologue(TR::Instruction *cursor, TR::Node *node)
   {
   TR_UNIMPLEMENTED();

   return NULL;
   }


// different from evaluate in that it returns a clobberable register
TR::Register *OMR::ARM64::CodeGenerator::gprClobberEvaluate(TR::Node *node)
   {
   if (node->getReferenceCount() > 1)
      {
      TR::Register *targetReg = self()->allocateRegister();
      generateMovInstruction(self(), node, targetReg, self()->evaluate(node));
      return targetReg;
      }
   else
      {
      return self()->evaluate(node);
      }
   }


void OMR::ARM64::CodeGenerator::buildRegisterMapForInstruction(TR_GCStackMap *map)
   {
   TR_UNIMPLEMENTED();
   }


TR_GlobalRegisterNumber OMR::ARM64::CodeGenerator::pickRegister(TR_RegisterCandidate *regCan,
                                                          TR::Block **barr,
                                                          TR_BitVector &availRegs,
                                                          TR_GlobalRegisterNumber &highRegisterNumber,
                                                          TR_LinkHead<TR_RegisterCandidate> *candidates)
   {
   return OMR::CodeGenerator::pickRegister(regCan, barr, availRegs, highRegisterNumber, candidates);
   }

bool OMR::ARM64::CodeGenerator::allowGlobalRegisterAcrossBranch(TR_RegisterCandidate *regCan, TR::Node *branchNode)
   {
   // If return false, processLiveOnEntryBlocks has to dis-qualify any candidates which are referenced
   // within any CASE of a SWITCH statement.
   return true;
   }

int32_t OMR::ARM64::CodeGenerator::getMaximumNumberOfGPRsAllowedAcrossEdge(TR::Node *node)
   {
   TR_UNIMPLEMENTED();
   return 0;
   }

int32_t OMR::ARM64::CodeGenerator::getMaximumNumberOfFPRsAllowedAcrossEdge(TR::Node *node)
   {
   TR_UNIMPLEMENTED();
   return 0;
   }

int32_t OMR::ARM64::CodeGenerator::getMaximumNumbersOfAssignableGPRs()
   {
   return TR::RealRegister::LastGPR - TR::RealRegister::FirstGPR + 1;
   }

int32_t OMR::ARM64::CodeGenerator::getMaximumNumbersOfAssignableFPRs()
   {
   return TR::RealRegister::LastFPR - TR::RealRegister::FirstFPR + 1;
   }

bool OMR::ARM64::CodeGenerator::isGlobalRegisterAvailable(TR_GlobalRegisterNumber i, TR::DataType dt)
   {
   return self()->machine()->getRealRegister((TR::RealRegister::RegNum)self()->getGlobalRegister(i))->getState() == TR::RealRegister::Free;
   }

TR_GlobalRegisterNumber OMR::ARM64::CodeGenerator::getLinkageGlobalRegisterNumber(int8_t linkageRegisterIndex, TR::DataType type)
   {
   TR_UNIMPLEMENTED();
   return 0;
   }

void OMR::ARM64::CodeGenerator::apply24BitLabelRelativeRelocation(int32_t *cursor, TR::LabelSymbol *label)
   {
   // for "b.cond" instruction
   TR_ASSERT(label->getCodeLocation(), "Attempt to relocate to a NULL label address!");

   intptrj_t distance = (uintptrj_t)label->getCodeLocation() - (uintptrj_t)cursor;
   *cursor |= ((distance >> 2) & 0x7ffff) << 5; // imm19
   }

void OMR::ARM64::CodeGenerator::apply32BitLabelRelativeRelocation(int32_t *cursor, TR::LabelSymbol *label)
   {
   // for unconditional "b" instruction
   TR_ASSERT(label->getCodeLocation(), "Attempt to relocate to a NULL label address!");

   intptrj_t distance = (uintptrj_t)label->getCodeLocation() - (uintptrj_t)cursor;
   *cursor |= ((distance >> 2) & 0x3ffffff); // imm26
   }

int64_t OMR::ARM64::CodeGenerator::getLargestNegConstThatMustBeMaterialized()
   {
   TR_ASSERT(0, "Not Implemented on AArch64");
   return 0;
   }

int64_t OMR::ARM64::CodeGenerator::getSmallestPosConstThatMustBeMaterialized()
   {
   TR_ASSERT(0, "Not Implemented on AArch64");
   return 0;
   }

bool
OMR::ARM64::CodeGenerator::directCallRequiresTrampoline(intptrj_t targetAddress, intptrj_t sourceAddress)
   {
   return
      !TR::Compiler->target.cpu.isTargetWithinUnconditionalBranchImmediateRange(targetAddress, sourceAddress) ||
      self()->comp()->getOption(TR_StressTrampolines);
   }

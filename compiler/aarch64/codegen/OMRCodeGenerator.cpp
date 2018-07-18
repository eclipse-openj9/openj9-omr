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

#include "codegen/CodeGenerator.hpp"
#include "codegen/CodeGenerator_inlines.hpp"
#include "codegen/GCStackMap.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/TreeEvaluator.hpp"

OMR::ARM64::CodeGenerator::CodeGenerator() :
      OMR::CodeGenerator(),
      _constantData(NULL),
      _outOfLineCodeSectionList(getTypedAllocator<TR_ARM64OutOfLineCodeSection*>(self()->comp()->allocator()))
   {
   /*
    * To be filled
    */
   }

void
OMR::ARM64::CodeGenerator::beginInstructionSelection()
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void
OMR::ARM64::CodeGenerator::endInstructionSelection()
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void
OMR::ARM64::CodeGenerator::doRegisterAssignment(TR_RegisterKinds kindsToAssign)
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void
OMR::ARM64::CodeGenerator::doBinaryEncoding()
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

TR::Linkage *OMR::ARM64::CodeGenerator::createLinkage(TR_LinkageConventions lc)
   {
   TR::Linkage *linkage = NULL;
   TR_ASSERT(false, "Not implemented yet.");
   /*
    * Commented out until TR::ARM64SystemLinkage is implemented
   switch (lc)
      {
      case TR_System:
         linkage = new (self()->trHeapMemory()) TR::ARM64SystemLinkage(self());
         break;
      default:
         TR_ASSERT(false, "using system linkage for unrecognized convention %d\n", lc);
         linkage = new (self()->trHeapMemory()) TR::ARM64SystemLinkage(self());
      }
   */
   self()->setLinkage(lc, linkage);
   return linkage;
   }

void OMR::ARM64::CodeGenerator::emitDataSnippets()
   {
   TR_ASSERT(false, "Not implemented yet.");
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
   TR_ASSERT(false, "Not implemented yet.");
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

   TR_ASSERT(false, "Not implemented yet.");
   /*
    * Commented out until TR::ConstantDataSnippet is implemented
   _constantData->print(outFile);
    */
   }
#endif


TR::Instruction *OMR::ARM64::CodeGenerator::generateSwitchToInterpreterPrePrologue(TR::Instruction *cursor, TR::Node *node)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }


// different from evaluate in that it returns a clobberable register
TR::Register *OMR::ARM64::CodeGenerator::gprClobberEvaluate(TR::Node *node)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }


void OMR::ARM64::CodeGenerator::buildRegisterMapForInstruction(TR_GCStackMap *map)
   {
   TR_ASSERT(false, "Not implemented yet.");
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
   TR_ASSERT(false, "Not implemented yet.");

   return 0;
   }

int32_t OMR::ARM64::CodeGenerator::getMaximumNumberOfFPRsAllowedAcrossEdge(TR::Node *node)
   {
   TR_ASSERT(false, "Not implemented yet.");

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
   return self()->machine()->getARM64RealRegister((TR::RealRegister::RegNum)self()->getGlobalRegister(i))->getState() == TR::RealRegister::Free;
   }

TR_GlobalRegisterNumber OMR::ARM64::CodeGenerator::getLinkageGlobalRegisterNumber(int8_t linkageRegisterIndex, TR::DataType type)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return 0;
   }

int64_t getLargestNegConstThatMustBeMaterialized() 
   { 
   TR_ASSERT(0, "Not Implemented on AArch64"); 
   return 0; 
   }

int64_t getSmallestPosConstThatMustBeMaterialized() 
   { 
   TR_ASSERT(0, "Not Implemented on AArch64"); 
   return 0; 
   }

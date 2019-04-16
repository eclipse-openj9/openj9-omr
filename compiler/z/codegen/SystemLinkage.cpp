/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
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

#include <algorithm>
#include <stddef.h>
#include <stdint.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/ConstantDataSnippet.hpp"
#include "codegen/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/Machine.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterPair.hpp"
#include "codegen/Snippet.hpp"
#include "codegen/SystemLinkage.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "compile/Compilation.hpp"
#include "compile/ResolvedMethod.hpp"
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/CompilerEnv.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/Symbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/TreeTop.hpp"
#include "il/TreeTop_inlines.hpp"
#include "il/symbol/AutomaticSymbol.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/symbol/ParameterSymbol.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "infra/Array.hpp"
#include "infra/Assert.hpp"
#include "infra/List.hpp"
#include "ras/Debug.hpp"
#include "ras/Delimiter.hpp"
#include "z/codegen/S390Evaluator.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"
#include "z/codegen/S390Instruction.hpp"
#include "z/codegen/SystemLinkage.hpp"

TR::SystemLinkage::SystemLinkage(TR::CodeGenerator* cg, TR_S390LinkageConventions elc, TR_LinkageConventions lc)
   : 
      TR::Linkage(cg, elc,lc),
      _GPRSaveMask(0), 
      _FPRSaveMask(0)
   {
   }

TR::SystemLinkage *
TR::SystemLinkage::self()
   {
   return static_cast<TR::SystemLinkage *>(this);
   }

void
TR::SystemLinkage::initS390RealRegisterLinkage()
   {
   int32_t icount = 0, ret_count=0;

   TR::RealRegister * spReal  = getStackPointerRealRegister();
   spReal->setState(TR::RealRegister::Locked);
   spReal->setAssignedRegister(spReal);
   spReal->setHasBeenAssignedInMethod(true);

   // set register weight
   for (icount = TR::RealRegister::FirstGPR; icount >= TR::RealRegister::LastAssignableGPR; icount++)
      {
      int32_t weight;
      if (getIntegerReturn((TR::RealRegister::RegNum) icount))
         {
         weight = ++ret_count;
         }
      else if (getPreserved((TR::RealRegister::RegNum) icount))
         {
         weight = 0xf000 + icount;
         }
      else
         {
         weight = icount;
         }
      cg()->machine()->getRealRegister((TR::RealRegister::RegNum) icount)->setWeight(weight);
      }
   }


// Non-Java use
void
TR::SystemLinkage::initParamOffset(TR::ResolvedMethodSymbol * method, int32_t stackIndex, List<TR::ParameterSymbol> *parameterList)
{
   ListIterator<TR::ParameterSymbol> parameterIterator((parameterList != NULL)? parameterList : &method->getParameterList());
   parameterIterator.reset();
   TR::ParameterSymbol * parmCursor = parameterIterator.getFirst();
   int8_t gprSize = cg()->machine()->getGPRSize();

   int32_t paramNum = -1;
   setIncomingParmAreaBeginOffset(stackIndex);

   while (parmCursor != NULL)
      {
      paramNum++;

      int8_t lri = parmCursor->getLinkageRegisterIndex();
      int32_t parmSize = parmCursor->getSize();
      if (isXPLinkLinkageType())
         {
         parmCursor->setParameterOffset(stackIndex);
         // parameters aligned on GPR sized boundaries - set for next
         int32_t alignedSize = parmCursor->getSize();
         alignedSize = ((alignedSize + gprSize - 1)/gprSize)*gprSize;
         stackIndex += alignedSize;
         }
      else if (lri == -1)
         {
         parmCursor->setParameterOffset(stackIndex);
         stackIndex += std::max<int32_t>(parmCursor->getSize(), cg()->machine()->getGPRSize());
         }
      else
         { // linkage parm registers reserved in caller save area
         TR::RealRegister::RegNum argRegNum;
         if (parmCursor->getType().isFloatingPoint())
             argRegNum = getFloatArgumentRegister(lri);
         else
             argRegNum = getIntegerArgumentRegister(lri);
         parmCursor->setParameterOffset(getStackFrameSize() + getRegisterSaveOffset(REGNUM(argRegNum)));
         }

      if (isSmallIntParmsAlignedRight() && parmCursor->getType().isIntegral() && (gprSize > parmCursor->getSize())) // WCode motivated
         {
         // linkage related
         parmCursor->setParameterOffset(parmCursor->getParameterOffset() + gprSize - parmCursor->getSize());
         }

      parmCursor = parameterIterator.getNext();
      }
   setIncomingParmAreaEndOffset(stackIndex);
}

/**
 * General utility
 * Perform a save or a restore operation of preserved FPRs from automatic
 * The operation depends on supplied opcode
 */
TR::Instruction *
TR::SystemLinkage::getputFPRs(TR::InstOpCode::Mnemonic opcode, TR::Instruction * cursor, TR::Node *node, TR::RealRegister *spReg)
   {
   int16_t FPRSaveMask;
   int32_t offset, firstSaved, lastSaved, beginSaveOffset;
   TR::MemoryReference *rsa;
   int32_t i,j, fprSize;

   if (spReg == NULL)
      {
      spReg = getNormalStackPointerRealRegister();
      }

   FPRSaveMask = getFPRSaveMask();
   beginSaveOffset = getFPRSaveAreaEndOffset(); // end offset is really begin offset -- see mapStack()

   firstSaved = TR::Linkage::getFirstMaskedBit(FPRSaveMask);
   lastSaved = TR::Linkage::getLastMaskedBit(FPRSaveMask);

   fprSize = cg()->machine()->getFPRSize();
   j = 0;
   for (i = firstSaved; i <= lastSaved; ++i)
      {
      if (FPRSaveMask & (1 << (i)))
         {
         offset = beginSaveOffset + (j*fprSize);
         rsa = generateS390MemoryReference(spReg, offset, cg());
         cursor = generateRXInstruction(cg(), opcode,
               node, getRealRegister(REGNUM(i+TR::RealRegister::FirstFPR)), rsa, cursor);
         j++;
         }
      }

   return cursor;
   }

/**
 * General utility
 * Returns
 *  *** if checkTempNeeded parm is non-NULL ***
 *
 *   checkTempNeeded parm is set to whether temporary register is needed
 *   to produce the instruction sequence.
 *   This is used by the caller to determine if it should allocate a temporary
 *   register to provided on a subsequent call to this routine. The caller
 *   could always provide a temporary register - but that will have adverse
 *   affects on performance if the register is not needed.
 *
 *  *** if checkTempNeeded parm is NULL ***
 *   instruction  sequence
 */
TR::Instruction *
TR::SystemLinkage::addImmediateToRealRegister(TR::RealRegister *targetReg, int32_t value, TR::RealRegister *tempReg, TR::Node *node, TR::Instruction *cursor, bool *checkTempNeeded)
   {
   bool smallPositiveValue = (value<MAXDISP && value>=0);
   bool largeValue = (value<MIN_IMMEDIATE_VAL || value>MAX_IMMEDIATE_VAL);

   if (checkTempNeeded)
      *checkTempNeeded = false; // assume no reg needed - change this below

   if (smallPositiveValue)
      {
      if (!checkTempNeeded)
         cursor = generateRXInstruction(cg(), TR::InstOpCode::LA, node, targetReg, generateS390MemoryReference(targetReg,value,cg()),cursor);
      }
   else if (largeValue)
      {
      // TODO: could reduce number of cases dependent on temporary register
      //  For example, could generate sequence of AHI's for suitably small large value
      if (checkTempNeeded)
         *checkTempNeeded = true;
      else if (!tempReg)
         TR_ASSERT( 0,"temporary register needed for add to register");
      else
         {
         cursor = generateS390ImmToRegister(cg(), node, tempReg, (intptr_t)(value), cursor);
         cursor = generateRRInstruction(cg(), TR::InstOpCode::getAddRegOpCode(), node, targetReg, tempReg, cursor);
         }
      }
   else
      {
      if (!checkTempNeeded)
         cursor = generateRXInstruction(cg(), TR::InstOpCode::LAY, node, targetReg, generateS390MemoryReference(targetReg,value,cg()), cursor);
      }

   return cursor;
   }

/**
 * General utility
 * Reverse the bit ordering of the save mask
 */
uint16_t
TR::SystemLinkage::flipBitsRegisterSaveMask(uint16_t mask)
   {
   TR_ASSERT(NUM_S390_GPR == NUM_S390_FPR, "need special flip bits for FPR");
   uint16_t i, outputMask;

   outputMask = 0;
   for (i = 0; i < NUM_S390_GPR; i++)
       {
       if (mask & (1 << i))
          {
          outputMask |= 1 << (NUM_S390_GPR-1-i);
          }
       }
   return outputMask;
   }

/**
 * Front-end customization of callNativeFunction
 */
void
TR::SystemLinkage::generateInstructionsForCall(TR::Node * callNode, TR::RegisterDependencyConditions * deps, intptrj_t targetAddress,
      TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel,
      TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint)
   {
   TR_ASSERT(0,"Different system types should have their own implementation.");
   }

/**
 * Call System routine
 * @return return value will be return value from system routine copied to private linkage return reg
 */
TR::Register *
TR::SystemLinkage::callNativeFunction(TR::Node * callNode, TR::RegisterDependencyConditions * deps, intptrj_t targetAddress,
      TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel,
      TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint)
   {
   return 0;
   }

void
TR::SystemLinkage::mapStack(TR::ResolvedMethodSymbol * method)
  	{
		{
     	mapStack(method,0);
  		}
  	}

/**
 * This function will intitialize all the offsets from frame pointer
 * i.e. stackPointer value before buying stack for method being jitted
 * The offsets will be fixed up (at the end /in createPrologue) when the stackFrameSize is known
 */
void
TR::SystemLinkage::mapStack(TR::ResolvedMethodSymbol * method, uint32_t stackIndex)
   {
   int32_t i;

// ====== COMPLEXITY ALERT:  (TODO: fix this nonsense: map stack positively)
// This method is called twice - once with a 0 value of stackIndex and secondly with the stack size.
// This adds complexities:
//     -  alignment: ensuring things are aligned properly
//     -  item layout is done in a reverse order : those things mapped first here are at higher addresses
//        end offsets are really begin offsets and vice versa.
// ====== COMPLEXITY ALERT

   setStackSizeCheckNeeded(true);


   // process incomming parameters
   if (!isParmBlockRegister() && !isZLinuxLinkageType()) // ! OS (Type 1 linkage) && ! zLinux Linkage
       {
       initParamOffset(method,getOutgoingParmAreaBeginOffset()+ stackIndex);
       }

   // Now map the locals
   //
   setLocalsAreaBeginOffset(stackIndex);
   ListIterator<TR::AutomaticSymbol> automaticIterator(&method->getAutomaticList());
   TR::AutomaticSymbol * localCursor = automaticIterator.getFirst();
   automaticIterator.reset();
   localCursor = automaticIterator.getFirst();

   while (localCursor != NULL)
      {
      if (!TR::Linkage::needsAlignment(localCursor->getDataType(), cg()))
         {
         mapSingleAutomatic(localCursor, stackIndex);
         }
      localCursor = automaticIterator.getNext();
      }

   automaticIterator.reset();
   localCursor = automaticIterator.getFirst();

   // align double - but there is more to do to align the stack in general as double.
   while (localCursor != NULL)
      {
      if (TR::Linkage::needsAlignment(localCursor->getDataType(), cg()))
         {
         mapSingleAutomatic(localCursor, stackIndex);
         }
      localCursor = automaticIterator.getNext();
      }


   // Force the stack size to be increased by...
   stackIndex -= (comp()->getOptions()->get390StackBufferSize()/4)*4;
   stackIndex -= (stackIndex & 0x4) ? 4 : 0;

   if (true)
      {
      //Save used Preserved FPRs
      setFPRSaveAreaBeginOffset(stackIndex);
      int16_t FPRSaveMask = 0;
      for (i = TR::RealRegister::FirstFPR; i <= TR::RealRegister::LastFPR; ++i)
         {
         if (getPreserved(REGNUM(i)) && ((getRealRegister(REGNUM(i)))->getHasBeenAssignedInMethod()))
            {
            FPRSaveMask |= 1 << (i - TR::RealRegister::FirstFPR);
            stackIndex -= cg()->machine()->getFPRSize();
            }
         }

      setFPRSaveMask(FPRSaveMask);

      if (FPRSaveMask != 0 && isOSLinkageType())
         {
         #define DELTA_ALIGN(x, align) ((x & (align-1)) ? (align -((x)&(align-1))) : 0)
         stackIndex -= DELTA_ALIGN(stackIndex, 16);
         }
      setFPRSaveAreaEndOffset(stackIndex);
      }

   // Map slot for long displacement
   if (TR::Compiler->target.isLinux())
      {
      // Linux on Z has a special reserved slot in the linkage convention
      setOffsetToLongDispSlot(TR::Compiler->target.is64Bit() ? 8 : 4);
      }
   else
      {
      setOffsetToLongDispSlot(stackIndex -= 16);
      }

   if (comp()->getOption(TR_TraceCG))
      traceMsg(comp(), "\n\nOffsetToLongDispSlot = %d\n", getOffsetToLongDispSlot());

   if (isParmBlockRegister()) // OS Type 1 linkage
       {
       // map areas for storing registers in lower stack addresses
       initParamOffset(method,stackIndex);
       stackIndex -= (getIncomingParmAreaBeginOffset()-getIncomingParmAreaEndOffset());
       }
   else if (isZLinuxLinkageType())
       //initParamOffset(method, stackIndex);
       initParamOffset(method,getOutgoingParmAreaBeginOffset()+ stackIndex);

   method->setLocalMappingCursor(stackIndex);

   stackIndex -= (stackIndex & 0x4) ? 4 : 0;
   setLocalsAreaEndOffset(stackIndex);
   }

void TR::SystemLinkage::mapSingleAutomatic(TR::AutomaticSymbol * p, uint32_t & stackIndex)
   {
   size_t align = 1;
   size_t size = p->getSize();

   if ((size & (size - 1)) == 0  && size <= 8) // if size is power of 2 and small
      {
      align = size;
      }
   else if (size > 8)
      {
      align = 8;
      }

   align = (align == 0) ? 1 : align;

   // we map in a backwards fashion
   int32_t roundup = align - 1;
   stackIndex = (stackIndex-size) & (~roundup);
   p->setOffset(stackIndex);
   }

bool TR::SystemLinkage::hasToBeOnStack(TR::ParameterSymbol * parm)
   {
   return parm->getAllocatedIndex() >=  0 &&  parm->isParmHasToBeOnStack();
   }

TR::Instruction*
TR::SystemLinkage::saveGPRsInPrologue(TR::Instruction * cursor)
   {
   int16_t GPRSaveMask = 0;
   bool hasSavedSP = false;
   int32_t offset = 0;
   TR::RealRegister * spReg = getNormalStackPointerRealRegister();
   TR::Node * firstNode = comp()->getStartTree()->getNode();

   TR::RealRegister::RegNum lastReg, firstReg;
   int32_t i;
   bool findStartReg = true;

   for( i = lastReg = firstReg = TR::RealRegister::FirstGPR; i <= TR::RealRegister::LastGPR; ++i )
      {
      if (comp()->getOption(TR_TraceCG))
         traceMsg(comp(), "Considering Register %d:\n",i- TR::RealRegister::FirstGPR);
      if(getPreserved(REGNUM(i)))
         {
         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "\tIt is Preserved\n");

         if (findStartReg)
            {
            //traceMsg(comp(), "\tSetting firstReg to %d\n",(i- TR::RealRegister::FirstGPR));
            firstReg = static_cast<TR::RealRegister::RegNum>(i);
            }

         if ((getRealRegister(REGNUM(i)))->getHasBeenAssignedInMethod())
            {
            if (comp()->getOption(TR_TraceCG))
               traceMsg(comp(), "\t It is Assigned. Putting in to GPRSaveMask\n");
            GPRSaveMask |= 1 << (i - TR::RealRegister::FirstGPR);
            findStartReg = false;
            }

         if (!findStartReg && ( !(getRealRegister(REGNUM(i)))->getHasBeenAssignedInMethod() || (i == TR::RealRegister::LastGPR && firstReg != TR::RealRegister::LastGPR)) )
            {
            // LastGPR is the stack pointer on zLinux which always be saved and restored
            if (i == TR::RealRegister::LastGPR)
               {
               lastReg = static_cast<TR::RealRegister::RegNum>(i);
               hasSavedSP = true;
               }
            else
               lastReg = static_cast<TR::RealRegister::RegNum>(i-1);

            if (comp()->getOption(TR_TraceCG))
               traceMsg(comp(), "\tGenerating preserve stores from %d  to %d \n",(lastReg - TR::RealRegister::FirstGPR),firstReg - TR::RealRegister::FirstGPR);

            offset =  getRegisterSaveOffset(REGNUM(firstReg));
            //traceMsg(comp(),"\tstackFrameSize = %d offset = %d\n",getStackFrameSize(),offset);

            TR::MemoryReference *retAddrMemRef = generateS390MemoryReference(spReg, offset, cg());

            if (lastReg - firstReg == 0)
               cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), firstNode, getRealRegister(REGNUM(firstReg)), retAddrMemRef, cursor);
            else
               cursor = generateRSInstruction(cg(),  TR::InstOpCode::getStoreMultipleOpCode(), firstNode, getRealRegister(REGNUM(firstReg)),  getRealRegister(REGNUM(lastReg)), retAddrMemRef, cursor);


            findStartReg = true;
            }
         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "\n");
         }
      }


   // LastGPR is the stack pointer on zLinux which always be saved and restored
   if (!hasSavedSP)
      {
       lastReg = static_cast<TR::RealRegister::RegNum>(TR::RealRegister::LastGPR);
       offset =  getRegisterSaveOffset(REGNUM(lastReg));
       TR::MemoryReference *retAddrMemRef = generateS390MemoryReference(spReg, offset, cg());
       cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), firstNode, getRealRegister(REGNUM(lastReg)), retAddrMemRef, cursor);
      }

   setGPRSaveMask(GPRSaveMask);
   return cursor;
   }

////////////////////////////////////////////////////////////////////////////////
// TR::390SystemLinkage::createPrologue() - create prolog for System linkage
////////////////////////////////////////////////////////////////////////////////
void TR::SystemLinkage::createPrologue(TR::Instruction * cursor)
   {
   TR::Delimiter delimiter (comp(), comp()->getOption(TR_TraceCG), "Prologue");
   TR::RealRegister * spReg = getNormalStackPointerRealRegister();
   TR::RealRegister * lpReg = getLitPoolRealRegister();
   TR::Node * firstNode = comp()->getStartTree()->getNode();
   int32_t i=0;
   int32_t firstLocalOffset = getOffsetToFirstLocal();
   TR::ResolvedMethodSymbol * bodySymbol = comp()->getJittedMethodSymbol();
   int32_t argSize = getOutgoingParameterBlockSize();
   TR::Snippet * firstSnippet = NULL;

   int16_t FPRSaveMask = 0;

   int32_t localSize; // Auto + Spill size

   localSize =  -1 * (int32_t) (bodySymbol->getLocalMappingCursor());

   setOutgoingParmAreaEndOffset(getOutgoingParmAreaBeginOffset() + argSize);

   setStackFrameSize(((getOffsetToFirstParm() +  argSize + localSize + 7)>>3)<<3);
   int32_t stackFrameSize = getStackFrameSize();

   if (comp()->getOption(TR_TraceCG))
      traceMsg(comp(), "initial stackFrameSize: stack size %d = offset to first parm %d + arg size %d + localSize %d, rounded up to 8\n", stackFrameSize, getOffsetToFirstParm(), argSize, localSize);

   TR_ASSERT( ((int32_t) stackFrameSize % 8 == 0), "misaligned stack detected");

   setVarArgRegSaveAreaOffset(getStackFrameSize());

   //  We assume frame size is less than 32k
   //TR_ASSERT(stackFrameSize<=MAX_IMMEDIATE_VAL,
   //      "TR::SystemLinkage::createPrologue -- Frame size (0x%x) greater than 0x7FFF\n",stackFrameSize);

   //Now that stackFrame size is known, map stack
   mapStack(bodySymbol, stackFrameSize);

   if (comp()->getOption(TR_TraceCG))
      traceMsg(comp(), "final stackFrameSize: stack size %d\n", stackFrameSize);

   setFirstPrologueInstruction(cursor);
   
   cursor = saveGPRsInPrologue(cursor);

   int16_t GPRSaveMask = getGPRSaveMask();

#if defined(JITTEST)
   // Literal Pool for Test compiler
   firstSnippet = cg()->getFirstSnippet();
   if ( cg()->isLiteralPoolOnDemandOn() != true && firstSnippet != NULL )
      cursor = new (trHeapMemory()) TR::S390RILInstruction(TR::InstOpCode::LARL, firstNode, lpReg, firstSnippet, cursor, cg());
#endif

   // Check for large stack
   bool largeStack = (stackFrameSize < MIN_IMMEDIATE_VAL || stackFrameSize > MAX_IMMEDIATE_VAL);

   TR::RealRegister * tempReg = getRealRegister(TR::RealRegister::GPR0);
   TR::RealRegister * backChainReg = getRealRegister(TR::RealRegister::GPR1);


   if (!largeStack)
      {
      // Adjust stack pointer with LA (reduce AGI delay)
      cursor = generateRXInstruction(cg(), TR::InstOpCode::LAY, firstNode, spReg, generateS390MemoryReference(spReg,(stackFrameSize) * -1, cg()),cursor);
      }
   else
      {
      // adjust stack frame pointer
      if (largeStack)
         {
         cursor = generateS390ImmToRegister(cg(), firstNode, tempReg, (intptr_t)(stackFrameSize * -1), cursor);
         cursor = generateRRInstruction(cg(), TR::InstOpCode::getAddRegOpCode(), firstNode, spReg, tempReg, cursor);
         }
      else
         {
         cursor = generateRIInstruction(cg(), TR::InstOpCode::getAddHalfWordImmOpCode(), firstNode, spReg, (stackFrameSize) * -1, cursor);
         }
      }

   //Save preserved FPRs
   FPRSaveMask = getFPRSaveMask();
   int32_t firstSaved = TR::Linkage::getFirstMaskedBit(FPRSaveMask);
   int32_t lastSaved = TR::Linkage::getLastMaskedBit(FPRSaveMask);
   int32_t offset = 0;
   TR::MemoryReference * retAddrMemRef ;

   if (TR::Compiler->target.isLinux())
      offset = getFPRSaveAreaEndOffset();
   else
      offset = getFPRSaveAreaBeginOffset() + 16*cg()->machine()->getGPRSize();

   for (i = firstSaved; i <= lastSaved; ++i)
      {
      if (FPRSaveMask & (1 << (i)))
         {
         retAddrMemRef = generateS390MemoryReference(spReg, offset, cg());
         offset += cg()->machine()->getFPRSize();
         cursor = generateRXInstruction(cg(), TR::InstOpCode::STD,
               firstNode, getRealRegister(REGNUM(i+TR::RealRegister::FirstFPR)), retAddrMemRef, cursor);
         }
      }

   cursor = (TR::Instruction *) saveArguments(cursor, false);

   setLastPrologueInstruction(cursor);
   }

TR::Instruction*
TR::SystemLinkage::restoreGPRsInEpilogue(TR::Instruction *cursor)
   {
   int16_t GPRSaveMask = 0;
   bool    hasRestoreSP = false;
   int32_t offset = 0;
   int32_t stackFrameSize = getStackFrameSize();
   TR::RealRegister * spReg = getNormalStackPointerRealRegister();
   TR::Node * nextNode = cursor->getNext()->getNode();

   TR::RealRegister::RegNum lastReg, firstReg;
   int32_t i;
   bool findStartReg = true;

   int32_t blockNumber = cursor->getNext()->getBlockIndex();

   for( i = lastReg = firstReg = TR::RealRegister::FirstGPR; i <= TR::RealRegister::LastGPR; ++i )
      {
      if (comp()->getOption(TR_TraceCG))
         traceMsg(comp(), "Considering Register %d:\n",i- TR::RealRegister::FirstGPR);
      if(getPreserved(REGNUM(i)))
         {
         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "\tIt is Preserved\n");

         if (findStartReg)
            firstReg = static_cast<TR::RealRegister::RegNum>(i);

         if ((getRealRegister(REGNUM(i)))->getHasBeenAssignedInMethod())
            {
            if (comp()->getOption(TR_TraceCG))
               traceMsg(comp(), "\tand Assigned. ");
            findStartReg = false;
            }

         if (!findStartReg && ( !(getRealRegister(REGNUM(i)))->getHasBeenAssignedInMethod() || (i == TR::RealRegister::LastGPR && firstReg != TR::RealRegister::LastGPR)) )
            {
            // LastGPR is the stack pointer on zLinux which always be saved and restored
            if (i == TR::RealRegister::LastGPR)
               {
               hasRestoreSP = true;
               lastReg = static_cast<TR::RealRegister::RegNum>(i);
               }
            else
               lastReg = static_cast<TR::RealRegister::RegNum>(i-1);

            offset =  getRegisterSaveOffset(REGNUM(firstReg));

            //traceMsg(comp(),"\tstackFrameSize = %d offset = %d\n",stackFrameSize,offset);

            TR::MemoryReference *retAddrMemRef = generateS390MemoryReference(spReg, offset + stackFrameSize, cg());

            if (lastReg - firstReg == 0)
               cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), nextNode, getRealRegister(REGNUM(firstReg )), retAddrMemRef, cursor);
            else
               cursor = generateRSInstruction(cg(),  TR::InstOpCode::getLoadMultipleOpCode(), nextNode, getRealRegister(REGNUM(firstReg)),  getRealRegister(REGNUM(lastReg)), retAddrMemRef, cursor);


            findStartReg = true;
            }

         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "\n");
         }
      }

    // LastGPR is the stack pointer on zLinux which always be saved and restored
    if (!hasRestoreSP)
       {
       lastReg = static_cast<TR::RealRegister::RegNum>(TR::RealRegister::LastGPR);
       offset =  getRegisterSaveOffset(REGNUM(lastReg));
       TR::MemoryReference *retAddrMemRef = generateS390MemoryReference(spReg, offset + stackFrameSize, cg());
       cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), nextNode, getRealRegister(REGNUM(lastReg )), retAddrMemRef, cursor);
       }
   return cursor;
   }

/**
 * Create epilog for System linkage
 */
void TR::SystemLinkage::createEpilogue(TR::Instruction * cursor)
   {
   TR::Delimiter delimiter (comp(), comp()->getOption(TR_TraceCG), "Epilogue");

   TR::RealRegister * spReg = getNormalStackPointerRealRegister();
   TR::Node * currentNode = cursor->getNode();
   TR::Node * nextNode = cursor->getNext()->getNode();
   TR::ResolvedMethodSymbol * bodySymbol = comp()->getJittedMethodSymbol();
   int32_t i, offset = 0, firstSaved, lastSaved ;
   int16_t GPRSaveMask = getGPRSaveMask();
   int16_t FPRSaveMask = getFPRSaveMask();
   int32_t stackFrameSize = getStackFrameSize();

   TR::MemoryReference * retAddrMemRef ;

   TR::LabelSymbol * epilogBeginLabel = generateLabelSymbol(cg());

   cursor = generateS390LabelInstruction(cg(), TR::InstOpCode::LABEL, nextNode, epilogBeginLabel, cursor);

   //Restore preserved FPRs
   firstSaved = TR::Linkage::getFirstMaskedBit(FPRSaveMask);
   lastSaved = TR::Linkage::getLastMaskedBit(FPRSaveMask);
   if (TR::Compiler->target.isLinux())
      offset = getFPRSaveAreaEndOffset();
   else
      offset = getFPRSaveAreaBeginOffset() + 16*cg()->machine()->getGPRSize();

   for (i = firstSaved; i <= lastSaved; ++i)
     {
     if (FPRSaveMask & (1 << (i)))
        {
        retAddrMemRef = generateS390MemoryReference(spReg, offset, cg());

        cursor = generateRXInstruction(cg(), TR::InstOpCode::LD,
           nextNode, getRealRegister(REGNUM(i+TR::RealRegister::FirstFPR)), retAddrMemRef, cursor);

        offset +=  cg()->machine()->getFPRSize();
        }
     }

   // Check for large stack
   bool largeStack = (stackFrameSize < MIN_IMMEDIATE_VAL || stackFrameSize > MAX_IMMEDIATE_VAL);

   TR::RealRegister * tempReg = getRealRegister(TR::RealRegister::GPR0);

   if (comp()->getOption(TR_TraceCG))
      traceMsg(comp(), "GPRSaveMask: Register context %x\n", GPRSaveMask&0xffff);

   cursor = restoreGPRsInEpilogue(cursor);

   cursor = generateS390RegInstruction(cg(), TR::InstOpCode::BCR, nextNode,
          getRealRegister(REGNUM(TR::RealRegister::GPR14)), cursor);
   ((TR::S390RegInstruction *)cursor)->setBranchCondition(TR::InstOpCode::COND_BCR);
   }

/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
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

// See also S390Linkage.cpp which contains more S390 Linkage
// implementations (primarily Private Linkage and base class).

#include <algorithm>
#include <stddef.h>
#include <stdint.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/ConstantDataSnippet.hpp"
#include "codegen/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
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
#include "z/codegen/SystemLinkagezOS.hpp"
#include "z/codegen/snippet/XPLINKCallDescriptorSnippet.hpp"

#ifdef J9_PROJECT_SPECIFIC
#include "env/VMJ9.h"
#endif

#define  GPREGINDEX(i)   (i-TR::RealRegister::FirstGPR)

TR::S390zOSSystemLinkage::S390zOSSystemLinkage(TR::CodeGenerator * codeGen)
   : 
      TR::SystemLinkage(codeGen, TR_SystemXPLink),
      _stackPointerUpdate(NULL),
      _ppa1(NULL),
      _ppa2(NULL)
   {
   setProperties(FirstParmAtFixedOffset);

   setProperty(SplitLongParm);
   setProperty(SkipGPRsForFloatParms);

   if (TR::Compiler->target.is64Bit())
      {
      setProperty(NeedsWidening);
      }

   setRegisterFlag(TR::RealRegister::GPR4, Preserved);
   setRegisterFlag(TR::RealRegister::GPR8, Preserved);
   setRegisterFlag(TR::RealRegister::GPR9, Preserved);
   setRegisterFlag(TR::RealRegister::GPR10, Preserved);
   setRegisterFlag(TR::RealRegister::GPR11, Preserved);
   setRegisterFlag(TR::RealRegister::GPR12, Preserved);
   setRegisterFlag(TR::RealRegister::GPR13, Preserved);
   setRegisterFlag(TR::RealRegister::GPR14, Preserved);
   setRegisterFlag(TR::RealRegister::GPR15, Preserved);

   setRegisterFlag(TR::RealRegister::FPR8, Preserved);
   setRegisterFlag(TR::RealRegister::FPR9, Preserved);
   setRegisterFlag(TR::RealRegister::FPR10, Preserved);
   setRegisterFlag(TR::RealRegister::FPR11, Preserved);
   setRegisterFlag(TR::RealRegister::FPR12, Preserved);
   setRegisterFlag(TR::RealRegister::FPR13, Preserved);
   setRegisterFlag(TR::RealRegister::FPR14, Preserved);
   setRegisterFlag(TR::RealRegister::FPR15, Preserved);
   
   if (codeGen->getSupportsVectorRegisters())
      {
      setRegisterFlag(TR::RealRegister::VRF16, Preserved);
      setRegisterFlag(TR::RealRegister::VRF17, Preserved);
      setRegisterFlag(TR::RealRegister::VRF18, Preserved);
      setRegisterFlag(TR::RealRegister::VRF19, Preserved);
      setRegisterFlag(TR::RealRegister::VRF20, Preserved);
      setRegisterFlag(TR::RealRegister::VRF21, Preserved);
      setRegisterFlag(TR::RealRegister::VRF22, Preserved);
      setRegisterFlag(TR::RealRegister::VRF23, Preserved);
      }

   setIntegerReturnRegister (TR::RealRegister::GPR3 );
   setLongLowReturnRegister (TR::RealRegister::GPR3 );
   setLongHighReturnRegister(TR::RealRegister::GPR2 );
   setLongReturnRegister    (TR::RealRegister::GPR3 );
   setFloatReturnRegister   (TR::RealRegister::FPR0 );
   setDoubleReturnRegister  (TR::RealRegister::FPR0 );
   setLongDoubleReturnRegister0  (TR::RealRegister::FPR0 );
   setLongDoubleReturnRegister2  (TR::RealRegister::FPR2 );
   setLongDoubleReturnRegister4  (TR::RealRegister::FPR4 );
   setLongDoubleReturnRegister6  (TR::RealRegister::FPR6 );
   setStackPointerRegister  (TR::RealRegister::GPR4 );
   setNormalStackPointerRegister  (TR::RealRegister::GPR4 );
   setAlternateStackPointerRegister  (TR::RealRegister::GPR9 );
   setEntryPointRegister    (TR::RealRegister::GPR6);
   setLitPoolRegister       (TR::RealRegister::GPR8 );
   setReturnAddressRegister (TR::RealRegister::GPR7 );

   setIntegerArgumentRegister(0, TR::RealRegister::GPR1);
   setIntegerArgumentRegister(1, TR::RealRegister::GPR2);
   setIntegerArgumentRegister(2, TR::RealRegister::GPR3);
   setNumIntegerArgumentRegisters(3);

   setFloatArgumentRegister(0, TR::RealRegister::FPR0);
   setFloatArgumentRegister(1, TR::RealRegister::FPR2);
   setFloatArgumentRegister(2, TR::RealRegister::FPR4);
   setFloatArgumentRegister(3, TR::RealRegister::FPR6);
   setNumFloatArgumentRegisters(4);

   if (codeGen->getSupportsVectorRegisters())
      {
      int32_t index = 0;
      
      setVectorArgumentRegister(index++, TR::RealRegister::VRF24);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF25);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF26);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF27);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF28);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF29);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF30);
      setVectorArgumentRegister(index++, TR::RealRegister::VRF31);
      setNumVectorArgumentRegisters(index);

      setVectorReturnRegister(TR::RealRegister::VRF24);
      }

   setOffsetToFirstParm   ((TR::Compiler->target.is64Bit()) ? 2176 : 2112);
   setOffsetToRegSaveArea   (2048);
   setNumberOfDependencyGPRegisters(32);
   setLargestOutgoingArgumentAreaSize(0);
   setOffsetToLongDispSlot(0);
   }

/**
 * WCode use
 */
int32_t
TR::S390zOSSystemLinkage::getOutgoingParameterBlockSize()
   {
   //
   // Calculate size of outgoing argument area
   //
   int32_t argAreaSize = getLargestOutgoingArgumentAreaSize();
   if (argAreaSize)
      {
      // xplink spec has minimal size
      // we assume yes to be safe
      int32_t minimalArgAreaSize = ((TR::Compiler->target.is64Bit()) ? 32 : 16);
      argAreaSize = (argAreaSize < minimalArgAreaSize) ? minimalArgAreaSize : argAreaSize;
      }
   return argAreaSize;
   }

/**
 * TR::S390zOSSystemLinkage::callNativeFunction - call System routine
 * return value will be return value from system routine copied to private linkage return reg
 */
TR::Register *
TR::S390zOSSystemLinkage::callNativeFunction(TR::Node * callNode, TR::RegisterDependencyConditions * deps, intptrj_t targetAddress,
      TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel,
      TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint)
   {

   /*****************************/
   /***Front-end customization***/
   /*****************************/
   generateInstructionsForCall(callNode, deps, targetAddress, methodAddressReg,
         javaLitOffsetReg, returnFromJNICallLabel, jniCallDataSnippet, isJNIGCPoint);

   TR::CodeGenerator * codeGen = cg();

   TR::Register * retReg = NULL;
   TR::Register * returnRegister = NULL;
   // set dependency on return register
   TR::Register * lowReg = NULL, * highReg = NULL;

   TR::Register * Real_highReg = NULL, * Real_lowReg = NULL, * Img_highReg = NULL, * Img_lowReg = NULL, * Real = NULL, * Imaginary = NULL;
   switch (callNode->getOpCodeValue())
      {
      case TR::acall:
      case TR::acalli:
         retReg = deps->searchPostConditionRegister(getIntegerReturnRegister());
         returnRegister = deps->searchPostConditionRegister(getIntegerReturnRegister());
         if(TR::Compiler->target.is64Bit() && returnRegister && !returnRegister->is64BitReg())
            {
            returnRegister->setIs64BitReg(true);    //in 64bit target, force return 64bit address register,
            }                                           //until it's known that dispatch functions can return non-64bit addresses.
         break;
      case TR::icall:
      case TR::iucall:
      case TR::icalli:
      case TR::iucalli:
         retReg = deps->searchPostConditionRegister(getIntegerReturnRegister());
         returnRegister = deps->searchPostConditionRegister(getIntegerReturnRegister());
         break;
      case TR::lcalli:
      case TR::lucalli:
      case TR::lcall:
      case TR::lucall:
         {
         if (TR::Compiler->target.is64Bit())
            {
            retReg = deps->searchPostConditionRegister(getIntegerReturnRegister());
            returnRegister = deps->searchPostConditionRegister(getIntegerReturnRegister());
            }
         else
            {
            TR::Instruction *cursor = NULL;
            lowReg = deps->searchPostConditionRegister(getLongLowReturnRegister());
            highReg = deps->searchPostConditionRegister(getLongHighReturnRegister());

            generateRSInstruction(codeGen, TR::InstOpCode::SLLG, callNode, highReg, highReg, 32);
            cursor =
               generateRRInstruction(codeGen, TR::InstOpCode::LR, callNode, highReg, lowReg);

            codeGen->stopUsingRegister(lowReg);
            retReg = highReg;
            returnRegister = retReg;
            }
         }
         break;
      case TR::fcalli:
      case TR::dcalli:
      case TR::fcall:
      case TR::dcall:
#if defined(SUPPORT_DFP) && defined(J9_PROJECT_SPECIFIC)
      case TR::dfcalli:
      case TR::ddcalli:
      case TR::dfcall:
      case TR::ddcall:
#endif
         retReg = deps->searchPostConditionRegister(getFloatReturnRegister());
         returnRegister = retReg;
         break;
#if defined(SUPPORT_DFP) && defined(J9_PROJECT_SPECIFIC)
      case TR::decall:
      case TR::decalli:
         highReg = deps->searchPostConditionRegister(getLongDoubleReturnRegister0());
         lowReg = deps->searchPostConditionRegister(getLongDoubleReturnRegister2());
         retReg = codeGen->allocateFPRegisterPair(lowReg, highReg);
         returnRegister = retReg;
         break;
#endif
      case TR::calli:
      case TR::call:
         retReg = NULL;
         returnRegister = retReg;
         break;
      default:
         retReg = NULL;
         returnRegister = retReg;
         TR_ASSERT(0, "Unknown direct call Opcode %d.", callNode->getOpCodeValue());
      }


   if (returnRegister != retReg)
      {
      generateRRInstruction(codeGen, TR::InstOpCode::getLoadRegOpCode(), callNode, returnRegister, retReg);
      }

   return returnRegister;
   }

int32_t
TR::S390zOSSystemLinkage::getRegisterSaveOffset(TR::RealRegister::RegNum srcReg)
   {
   int32_t offset;
   if ((srcReg >= TR::RealRegister::GPR4) && (srcReg <= TR::RealRegister::GPR15))
      {
      int32_t offset = 2048 + (srcReg - TR::RealRegister::GPR4) * cg()->machine()->getGPRSize();
      return offset;
      }
   else
      {
      TR_ASSERT(false, "ERROR: TR::S390zOSSystemLinkage::getRegisterSaveOffset called for volatile reg: %d\n",srcReg);
      return -1;
      }
   }

#define  GPREGINDEX(i)   (i-TR::RealRegister::FirstGPR)

TR::Instruction * TR::S390zOSSystemLinkage::buyFrame(TR::Instruction * cursor, TR::Node * node)
   {
   enum TR_XPLinkFrameType frameType;
   int16_t GPRSaveMask;
   TR::LabelSymbol *stmLabel;

   int8_t gprSize = cg()->machine()->getGPRSize();
   int32_t stackFrameSize, offsetToRegSaveArea, offsetToFirstSavedReg, gpr3ParmOffset;
   int32_t  firstSaved, lastSaved, firstPossibleSaved;

   // As defined by XPLINK specification
   int32_t intermediateThreshold = TR::Compiler->target.is64Bit() ? 1024 * 1024 : 4096;

   TR::RealRegister *spReg = getNormalStackPointerRealRegister(); // normal sp reg used in prol/epil
   stackFrameSize = getStackFrameSize();
   offsetToRegSaveArea = getOffsetToRegSaveArea();

   // This delta is slightly pessimistic and could be a "tad" more. But, taking
   // into account the "tad" complicates this code more than its worth.
   // I.E. small set of cases could be forced to being intermediate vs. small
   // frame types because of this "tad".
   int32_t delta = XPLINK_STACK_FRAME_BIAS - stackFrameSize;

   if (stackFrameSize > intermediateThreshold)
      // guard page option mandates explicit checking
      frameType = TR_XPLinkStackCheckFrame;
   else if (delta >=0)
      // delta > 0 implies stack offset for STM will be positive and <2K
      frameType = TR_XPLinkSmallFrame;
   else if (stackFrameSize < intermediateThreshold)
      frameType = TR_XPLinkIntermediateFrame;
   else
      frameType = TR_XPLinkStackCheckFrame;

   TR::MemoryReference *rsa, *bosRef, *extenderRef, *gpr3ParmRef;
   int32_t rsaOffset, bosOffset, extenderOffset;

   //
   // Determine minimal Preserved GPRs. Notes:
   //   1) an option can force the save of the stack pointer register
   //   2) XPLink noleaf:  routines mandates the save of the entry point register (r6) & return register (r7)
   //               leaf:  routines cannot modify r6, r7
   //
   GPRSaveMask = 0;
   TR::RealRegister::RegNum lastUsedReg = TR::RealRegister::NoReg;
   for (int32_t i = TR::RealRegister::FirstGPR; i <= TR::RealRegister::LastGPR; ++i)
     {
     if (getPreserved(REGNUM(i)))
        {
        if (getRealRegister(REGNUM(i))->getHasBeenAssignedInMethod())
           {
           GPRSaveMask |= 1 << GPREGINDEX(i);
           }
        }
     }

   GPRSaveMask  |= 1 << GPREGINDEX(getReturnAddressRegister());
   GPRSaveMask  |= 1 << GPREGINDEX(getEntryPointRegister());

    // Determine if we need a temp register in either buying or releasing
    // the stack frame. If so, we will save the back chain so epilogue code
    // can just restore it from the frame and not require a temp register.
    // We still need a temp register for prologue code though.
    bool needAddTempReg, saveTempReg;
    addImmediateToRealRegister(NULL, stackFrameSize, NULL, NULL, NULL, &needAddTempReg);

   // For 64 bit stack checking code - need a temp reg for LAA access
   if (TR::Compiler->target.is64Bit() && (frameType == TR_XPLinkStackCheckFrame))
      needAddTempReg = true;

   if (needAddTempReg)
      GPRSaveMask  |= 1 << GPREGINDEX(getNormalStackPointerRegister());
   else
      GPRSaveMask  &= ~(1 << GPREGINDEX(getNormalStackPointerRegister()));


   // CAA is either locked or restored on RET - so remove it from mask
   // But not in 64 bit code
   if(!TR::Compiler->target.is64Bit())
      {
      GPRSaveMask  &= ~(1 << GPREGINDEX(getCAAPointerRegister()));
      }
   
   if ((frameType == TR_XPLinkStackCheckFrame)
      || ((frameType == TR_XPLinkIntermediateFrame) && needAddTempReg))
      { // GPR4 is saved but indirectly via GPR0 in stack check or medium case - so don't save GPR4 via STM
      firstSaved = getFirstMaskedBit(GPRSaveMask&~(1 << GPREGINDEX(getNormalStackPointerRegister())));
      }
   else
      firstSaved = getFirstMaskedBit(GPRSaveMask);

   setGPRSaveMask(GPRSaveMask);

   if (comp()->getOption(TR_TraceCG))
      {
      traceMsg(comp(), "GPRSaveMask: Register context %x\n", GPRSaveMask&0xffff);
      }

   firstPossibleSaved = GPREGINDEX(TR::RealRegister::GPR4); // GPR4 is first reg in save area
   lastSaved = getLastMaskedBit(GPRSaveMask);
   offsetToFirstSavedReg = (firstSaved-firstPossibleSaved)*gprSize; // relative to start of save area

   // variables for stack extension code - xplink spec mandates these particular registers
   TR::RealRegister * gpr0Real = getRealRegister(TR::RealRegister::GPR0);
   TR::RealRegister * gpr3Real = getRealRegister(TR::RealRegister::GPR3);
   TR::RealRegister * caaReal  = getRealRegister(getCAAPointerRegister());  // 31 bit oly
   gpr3ParmOffset = getOffsetToFirstParm() + 2 * gprSize;
   
   _firstSaved = REGNUM(firstSaved + TR::RealRegister::FirstGPR);
   _lastSaved  = REGNUM(lastSaved  + TR::RealRegister::FirstGPR);

   switch (frameType)
      {
      case TR_XPLinkSmallFrame:
          // STM rx,ry,offset(R4)
          rsaOffset =  (offsetToRegSaveArea - stackFrameSize) + offsetToFirstSavedReg;
          rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
          cursor = generateRSInstruction(cg(),  TR::InstOpCode::getStoreMultipleOpCode(),
                  node, getRealRegister(REGNUM(firstSaved + TR::RealRegister::FirstGPR)),
                  getRealRegister(REGNUM(lastSaved + TR::RealRegister::FirstGPR)), rsa, cursor);

          cursor = generateS390LabelInstruction(cg(), InstOpCode::LABEL, node, generateLabelSymbol(cg()), cursor);
          _stackPointerUpdate = cursor;

          // AHI R4,-stackFrameSize
          cursor = addImmediateToRealRegister(spReg, (stackFrameSize) * -1, NULL, node, cursor);
          break;

      case TR_XPLinkIntermediateFrame:
          if (needAddTempReg)
             {
             // (needAddTempReg) LR R0,R4
             // (saveTempReg|needAddTempReg) ST R3,D(R4)   <-- save temp register
             // R4<- R4 - stackSize
             // LR R3,R0
             // (saveTempReg) L  R3,D(R3)   <-- load temp register
             saveTempReg = true;  // PERFORMANCE NOTE: TODO: set to false if GPR3 is not used as a parameter

             if (saveTempReg || needAddTempReg)
                {
                cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr0Real, spReg, cursor); // LR R0,R4
                }
             if (saveTempReg)
                {
                gpr3ParmRef = generateS390MemoryReference(spReg, gpr3ParmOffset, cg()); // used for temporary register
                cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr3Real, gpr3ParmRef, cursor); // ST R3,parmRef(R4)
                }

             cursor = generateS390LabelInstruction(cg(), InstOpCode::LABEL, node, generateLabelSymbol(cg()), cursor);
             _stackPointerUpdate = cursor;

             cursor = addImmediateToRealRegister(spReg, (stackFrameSize) * -1, gpr3Real, node, cursor);  // R4 <- R4 - stack size

             if (saveTempReg)
                {
                cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr3Real, gpr0Real, cursor);
                gpr3ParmRef = generateS390MemoryReference(gpr3Real, gpr3ParmOffset, cg());
                cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), node, gpr3Real, gpr3ParmRef, cursor);
                }
             }
          else
             { // no temp reg needed
             // (needAddTempReg)    LR R0,R4
             // AHI R4,-stackFrameSize
             // STM rx,ry,offset(R4)
             // (needAddTempReg)    ST R0,D(R4)

             if (needAddTempReg) // LR R0,R4
                cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node, gpr0Real, spReg, cursor);
             
             cursor = generateS390LabelInstruction(cg(), InstOpCode::LABEL, node, generateLabelSymbol(cg()), cursor);
             _stackPointerUpdate = cursor;

             cursor = addImmediateToRealRegister(spReg, (stackFrameSize) * -1, NULL, node, cursor);
             }
          rsaOffset =  offsetToRegSaveArea + offsetToFirstSavedReg;
          rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
          cursor = generateRSInstruction(cg(),  TR::InstOpCode::getStoreMultipleOpCode(),
                  node, getRealRegister(REGNUM(firstSaved + TR::RealRegister::FirstGPR)),
                  getRealRegister(REGNUM(lastSaved + TR::RealRegister::FirstGPR)), rsa, cursor);
          if (needAddTempReg)
             { // ST R0,2048(R4)
             rsaOffset = offsetToRegSaveArea + 0; // GPR4 saved at start of RSA
             rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr0Real, rsa, cursor);
             }
          break;

      case TR_XPLinkStackCheckFrame:
          // TODO: move the code to exend stack to "cold" path
          // Code:
          //   important note: saveTempReg -> needAddTempReg
          //
          //     (saveTempReg)      ST R3,parmOffsetR3(GPR4)
          //     (needAddTempReg)    LR R0,R4
          //     AHI R4, -stackFrameSize  [this varies based on stackFrameSize]
          //     [31 bit] C  R4,BOS(R12)           R12=CAA
          //     [64 bit] LLGT  R3,1268            R3=control block - low mem
          //     [64 bit] C  R4,BOS(R3)            R3=ditto
          //     BL stmLabel
          //     /* extension logic */
          //     (!needAddTempReg)    LR R0,R3   /* could be smarter here */
          //     L  R3,extender(R12)
          //     BASR R3,R3
          //     NOP x                    [xplink noop]
          //     (!needAddTempReg)   LR R3,R0   /* could be smarter here */
          //
          //   stmLabel:
          //     STM rx,ry,offset(R4)
          //     (needAddTempReg)    ST R0,2048(,R4)
          //     (saveTempReg)      LR R3,R0   /* could be smarter here */
          //     (saveTempReg)      L  R3,parmOffsetR3(GPR3)
          //
          stmLabel = generateLabelSymbol(cg());
          saveTempReg = needAddTempReg;  // TODO: and with check that R3 is used as parm

          //------------------------------
          // Stack check - prologue part 1
          //------------------------------

          if (saveTempReg)
             {
             gpr3ParmRef = generateS390MemoryReference(spReg, gpr3ParmOffset, cg()); // used for temporary register
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr3Real, gpr3ParmRef, cursor); // ST R3,parmRef(R4)
             }
          if (needAddTempReg) // LR R0,R4
              cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr0Real, spReg, cursor);
          cursor = addImmediateToRealRegister(spReg, (stackFrameSize) * -1, gpr3Real, node, cursor); // AHI ...

          if (TR::Compiler->target.is64Bit())
             {
             TR::MemoryReference *laaRef;
             bosOffset = 64; // 64=offset in LAA to BOS
             int32_t laaLowMemAddress = 1208; // LAA address
             laaRef = generateS390MemoryReference(gpr0Real, laaLowMemAddress, cg());
             cursor = generateRXInstruction(cg(), InstOpCode::LLGT, node, gpr3Real, laaRef, cursor);
             bosRef = generateS390MemoryReference(gpr3Real, bosOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getCmpOpCode(), node, spReg, bosRef, cursor); // C R4,bos(tempreg)
             }
          else
             {
             bosOffset = 868; // 868=offset in CAA to BOS
             bosRef = generateS390MemoryReference(caaReal, bosOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getCmpOpCode(), node, spReg, bosRef, cursor); // C R4,bos(R12)
             }
          cursor = generateS390BranchInstruction(cg(), InstOpCode::BRC, InstOpCode::COND_BNM, node, stmLabel, cursor); // BNM stmLabel

          //------------------------------
          // Stack check - extension routine logic
          //------------------------------

          // This logic should be moved out of line eventually
          if (!needAddTempReg)
             {
             cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr0Real, gpr3Real, cursor); // LR R0,R3
             }

          if (TR::Compiler->target.is64Bit())
             {
             extenderOffset = 72;  // offset of extender routine in LAA
             extenderRef = generateS390MemoryReference(gpr3Real, extenderOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), node, gpr3Real, extenderRef, cursor); //  L R3,extender(R12)
             }
          else
             {
             extenderOffset = 872;  // offset of extender routine in CAA
             extenderRef = generateS390MemoryReference(caaReal, extenderOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), node, gpr3Real, extenderRef, cursor); //  L R3,extender(R12)
             }
          cursor = generateRRInstruction(cg(), InstOpCode::BASR, node, gpr3Real, gpr3Real, cursor);
          cursor = genCallNOPAndDescriptor(cursor, node, NULL, TR_XPLinkCallType_BASR33);

          if (!needAddTempReg)
             { // LR R3,R0
             cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr3Real, gpr0Real, cursor);
             }

          //------------------------------
          // Stack check - prologue part 2
          //------------------------------

          // STM rx,ry,disp(R4)
          cursor = generateS390LabelInstruction(cg(), InstOpCode::LABEL, node, stmLabel, cursor);

          rsaOffset =  offsetToRegSaveArea + offsetToFirstSavedReg;
          rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
          cursor = generateRSInstruction(cg(),  TR::InstOpCode::getStoreMultipleOpCode(),
                  node, getRealRegister(REGNUM(firstSaved + TR::RealRegister::FirstGPR)),
                  getRealRegister(REGNUM(lastSaved + TR::RealRegister::FirstGPR)), rsa, cursor);
          if (needAddTempReg)
             { // ST R0,2048(R4)
             rsaOffset = offsetToRegSaveArea + 0; // GPR4 saved at start of RSA
             rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr0Real, rsa, cursor);
             }
          if (saveTempReg)
             {
             cursor = generateRRInstruction(cg(), TR::InstOpCode::getLoadRegOpCode(), node,  gpr3Real, gpr0Real, cursor);
             gpr3ParmRef = generateS390MemoryReference(gpr3Real, gpr3ParmOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), node, gpr3Real, gpr3ParmRef, cursor);
             }

          break;

      default:
          TR_ASSERT( 0,"invalid xplink frame type");
          break;
      }

   return cursor;
   }

/////////////////////////////////////////////////////////////////////////////////
// TR::S390zOSSystemLinkage::generateInstructionsForCall - Front-end
//  customization of callNativeFunction
////////////////////////////////////////////////////////////////////////////////
void
TR::S390zOSSystemLinkage::generateInstructionsForCall(TR::Node * callNode, TR::RegisterDependencyConditions * deps, intptrj_t targetAddress,
      TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel,
      TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint)
   {
   TR::CodeGenerator * codeGen = cg();

    // WCode specific
    //
    // There are 4 cases for outgoing branch sequences
    //   case 1)            pure OS linkage call using entry point
    //                      BASR R14,R15
    //   case 2) (indirect) Call vi function pointer
    //                      LM R5,R6,disp(regfp)
    //                      BASR R7,R6
    //          where:
    //             a) regfp is a register containing pointer to function descriptor
    //                (most likely placed in R6)
    //             b) disp is offset into function descriptor - 16/0 (31/64 bit respectively)
    //   case 3) (direct) Call to (static or global) function defined in the compilation unit:
    //                      BRASL R7,func
    //
    //   case 4) (direct) Call to external function referenced function
    //                      LM R5,R6,disp(regenv)
    //                      BASR R7,R6
    //         where:
    //             a) disp is an offset in the environment (aka ADA) containing the
    //                function descriptor body (i.e. not pointer to function descriptor)
    TR_XPLinkCallTypes callType;

    // Find GPR6 (xplink) or GPR15 (os linkage) in post conditions
    TR::Register * systemEntryPointRegister = deps->searchPostConditionRegister(getEntryPointRegister());
    // Find GPR7 (xplik) or GPR14 (os linkage) in post conditions
    TR::Register * systemReturnAddressRegister = deps->searchPostConditionRegister(getReturnAddressRegister());

    TR::RegisterDependencyConditions * preDeps = new (trHeapMemory()) TR::RegisterDependencyConditions(deps->getPreConditions(), NULL, deps->getAddCursorForPre(), 0, cg());

    if (callNode->getOpCode().isIndirect())
       {
       generateRRInstruction(cg(), InstOpCode::BASR, callNode, systemReturnAddressRegister, systemEntryPointRegister, preDeps);
       callType = TR_XPLinkCallType_BASR;
       }
    else
       {

       TR::SymbolReference *callSymRef = callNode->getSymbolReference();
       TR::Symbol *callSymbol = callSymRef->getSymbol();

       TR::Instruction * callInstr = new (cg()->trHeapMemory()) TR::S390RILInstruction(TR::InstOpCode::BRASL, callNode, systemReturnAddressRegister, callSymbol, callSymRef, cg());
       callInstr->setDependencyConditions(preDeps);
       callType = TR_XPLinkCallType_BRASL7;
       }

    generateS390LabelInstruction(codeGen, InstOpCode::LABEL, callNode, returnFromJNICallLabel);

    genCallNOPAndDescriptor(NULL, callNode, callNode, callType);

    // Append post-dependencies after NOP
    TR::LabelSymbol * afterNOP = generateLabelSymbol(cg());
    TR::RegisterDependencyConditions * postDeps =
          new (trHeapMemory()) TR::RegisterDependencyConditions(NULL,
                deps->getPostConditions(), 0, deps->getAddCursorForPost(), cg());
    generateS390LabelInstruction(codeGen, InstOpCode::LABEL, callNode, afterNOP, postDeps);
}
TR::LabelSymbol*
TR::S390zOSSystemLinkage::getEntryPointMarkerLabel() const
   {
   return _entryPointMarkerLabel;
   }

TR::Instruction*
TR::S390zOSSystemLinkage::getStackPointerUpdate() const
   {
   return _stackPointerUpdate;
   }

TR::PPA1Snippet*
TR::S390zOSSystemLinkage::getPPA1Snippet() const
   {
   return _ppa1;
   }

TR::PPA2Snippet*
TR::S390zOSSystemLinkage::getPPA2Snippet() const
   {
   return _ppa2;
   }

// Create prologue for XPLink
void TR::S390zOSSystemLinkage::createPrologue(TR::Instruction * cursor)
   {
   TR_ASSERT( cursor!=NULL,"cursor is NULL in createPrologue");
   
   TR::ResolvedMethodSymbol *bodySymbol = comp()->getJittedMethodSymbol();

   // Logic for main most of which applies to secondary entry points
   // I.E. all except for parameter offset information.

   int32_t localSize, argSize;
   int32_t stackFrameSize, alignedStackFrameSize;

   // Set for initParamOffset() - relative offset from start of frame
   setOutgoingParmAreaBeginOffset(getOffsetToFirstParm() - XPLINK_STACK_FRAME_BIAS);

   //
   // Calculate size of locals - determined in prior call to mapStack()
   // We make the size a multiple of the strictest alignment symbol
   // So that backwards mapping of auto symbols will follow the alignment
   //
   localSize =  -1 * (int32_t) (bodySymbol->getLocalMappingCursor());  // Auto+Spill size
   int32_t strictestAutoAlign = 8;
   localSize = (localSize + strictestAutoAlign - 1) & ~(strictestAutoAlign-1);

   //
   // Calculate size of outgoing argument area
   //
   argSize = getOutgoingParameterBlockSize();

   //
   // Calculate stack frame size
   //
   int32_t outGoingAreaOffsetUnbiased = (TR::Compiler->target.is64Bit()) ? 128 : 64;
   stackFrameSize = outGoingAreaOffsetUnbiased + argSize + localSize;
   int32_t minFrameAlign = (TR::Compiler->target.is64Bit()) ? 32 : 16;
   TR_ASSERT( minFrameAlign >= strictestAutoAlign,"automatic symbol has alignment that cannot be satisfied");
   alignedStackFrameSize = (stackFrameSize + minFrameAlign - 1) & ~(minFrameAlign-1);
   stackFrameSize = alignedStackFrameSize;

   setStackFrameSize(alignedStackFrameSize);

   //
   // Map stack - which is done in backwards fashion
   //
   int32_t stackMappingIndex = XPLINK_STACK_FRAME_BIAS + stackFrameSize;
   mapStack(bodySymbol, stackMappingIndex);

   TR::Node *node = cursor->getNode();

   TR::CodeGenerator* cg = self()->cg();

   _ppa1 = new (self()->trHeapMemory()) TR::PPA1Snippet(cg, this);
   _ppa2 = new (self()->trHeapMemory()) TR::PPA2Snippet(cg, this);

   cg->addSnippet(_ppa1);
   cg->addSnippet(_ppa2);

   stackFrameSize = getStackFrameSize();

   TR_ASSERT_FATAL((stackFrameSize & 31) == 0, "XPLINK stack frame size (%d) has to be aligned to 32-bytes.", stackFrameSize);
   
   cursor = cursor->getPrev();

   // "C.E.E.1"
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00C300C5, cursor);
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00C500F1, cursor);
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00000000, cursor);

   cg->addRelocation(new (cg->trHeapMemory()) InstructionLabelRelative32BitRelocation(cursor, -8, _ppa1->getSnippetLabel(), 1));
   
   // DSA size is the frame size aligned to 32-bytes which means it's least significant 5 bits are zero and are used to
   // represent the flags which are always 0 for OMR as we do not support leaf frames or direct calls to alloca()
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, stackFrameSize, cursor);

   cursor = cursor->getNext();

   setFirstPrologueInstruction(cursor);

   //
   // Generate code to acquire the stack frame
   //
   cursor = buyFrame(cursor, node);

   //
   // Save FPRs and ARs in local automatic
   // This is done after mapStack - as it calculates the register mask and
   // save area offset.
   //
   cursor = getputFPRs(InstOpCode::STD, cursor, node);

   //
   // Save arguments as required
   //
   cursor = (TR::Instruction *) saveArguments(cursor, false);
   
   // Last prologue instruction
   setLastPrologueInstruction(cursor);
   }

void
TR::S390zOSSystemLinkage::createEpilogue(TR::Instruction * cursor)
   {
   int16_t GPRSaveMask;
   int8_t gprSize = cg()->machine()->getGPRSize();
   TR::Node * currentNode = cursor->getNode();
   int32_t offset;

   int32_t stackFrameSize, offsetToFirstSavedReg;
   int32_t  firstSaved, lastSaved, firstPossibleSaved;
   TR::MemoryReference *rsa;

   int32_t blockNumber = -1;

   TR::RealRegister *spReg = getNormalStackPointerRealRegister(); // normal sp reg used in prol/epil
   stackFrameSize = getStackFrameSize();

   //
   // Restore FPRs and ARs from local automatic
   //
   cursor = getputFPRs(InstOpCode::LD, cursor, currentNode);

   GPRSaveMask = getGPRSaveMask(); // restore mask is subset of save mask
   GPRSaveMask  &= ~(1 << GPREGINDEX(getEntryPointRegister())); // entry point register not restored

   firstSaved = getFirstMaskedBit(GPRSaveMask);
   firstPossibleSaved = 4; // GPR4 is first reg in save area
   lastSaved = getLastMaskedBit(GPRSaveMask);

   if (firstSaved >= 0)
      {
      offsetToFirstSavedReg = (firstSaved-firstPossibleSaved)*gprSize; // relative to start of save area
      offset = XPLINK_STACK_FRAME_BIAS + offsetToFirstSavedReg;
      rsa = generateS390MemoryReference(spReg, offset, cg());

      if (firstSaved == lastSaved)
         {
            cursor = generateRXInstruction(cg(), TR::InstOpCode::getLoadOpCode(), currentNode,
                                           getRealRegister(REGNUM(firstSaved + TR::RealRegister::FirstGPR)), rsa, cursor);
         }
      else if (firstSaved < lastSaved)
         {
         int8_t numNeededToRestore = -1;

            cursor = restorePreservedRegs(REGNUM(firstSaved + TR::RealRegister::FirstGPR),
                                         REGNUM(lastSaved + TR::RealRegister::FirstGPR),
                                         blockNumber, cursor, currentNode, spReg, rsa, getNormalStackPointerRegister());
         }
      }

   if (!(GPRSaveMask &(1 << GPREGINDEX(getNormalStackPointerRegister()))))
      { // GPR4 not restored by previous and needs restoring
      cursor = addImmediateToRealRegister(spReg, stackFrameSize,  NULL, currentNode, cursor);
      TR_ASSERT( cursor != NULL, "xplink retore code - should not need temp register");
      }

   // 31 bit - B 4(,7),  64 bit B 2(,7)
   TR::RealRegister *dummyUncondMask = getRealRegister(REGNUM(TR::RealRegister::GPR15));
   TR::RealRegister * realReturnRegister = getReturnAddressRealRegister();
   offset = (TR::Compiler->target.is64Bit()) ? 2 : 4;
   cursor = generateRXInstruction(cg(), InstOpCode::BC, currentNode, dummyUncondMask, generateS390MemoryReference(realReturnRegister, offset, cg()), cursor);
   }

/**
 * General XPLink utility
 */
TR::Instruction *
TR::S390zOSSystemLinkage::genCallNOPAndDescriptor(TR::Instruction* cursor, TR::Node* node, TR::Node* callNode, TR_XPLinkCallTypes callType)
   {
   if (TR::Compiler->target.is32Bit())
      {
      uint32_t callDescriptorValue = TR::XPLINKCallDescriptorSnippet::generateCallDescriptorValue(this, callNode);

      TR::Snippet* callDescriptor = new (self()->trHeapMemory()) TR::XPLINKCallDescriptorSnippet(cg(), this, callDescriptorValue);
      cg()->addSnippet(callDescriptor);

      uint32_t nopDescriptor = 0x47000000 | (static_cast<uint32_t>(callType) << 16);
      cursor = generateDataConstantInstruction(cg(), TR::InstOpCode::DC, node, nopDescriptor, cursor);

      cg()->addRelocation(new (cg()->trHeapMemory()) InstructionLabelRelative16BitRelocation(cursor, 2, callDescriptor->getSnippetLabel(), 8));
      }
   else
      {
      // TODO: Once we support generic TR::InstOpCode::DC of any size we need to modify this line to use it, similarly
      // to what we do above for the 31-bit case.
      uint16_t nopDescriptor = 0x1800 | static_cast<uint16_t>(callType);
      cursor = new (cg()->trHeapMemory()) TR::S390Imm2Instruction(TR::InstOpCode::DC2, node, nopDescriptor, cursor, cg());
      }

   return cursor;
   }

TR::Instruction*
TR::S390zOSSystemLinkage::getputFPRs(TR::InstOpCode::Mnemonic opcode, TR::Instruction * cursor, TR::Node *node, TR::RealRegister *spReg)
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

TR::Instruction *
TR::S390zOSSystemLinkage::addImmediateToRealRegister(TR::RealRegister *targetReg, int32_t value, TR::RealRegister *tempReg, TR::Node *node, TR::Instruction *cursor, bool *checkTempNeeded)
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

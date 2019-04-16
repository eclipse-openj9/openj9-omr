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
   // linkage properties
   setProperties(FirstParmAtFixedOffset);
   setProperty(SplitLongParm);
   setProperty(SkipGPRsForFloatParms);
   if (TR::Compiler->target.is64Bit())
      {
      setProperty(NeedsWidening);
      }
   else
      {
      setProperty(FloatParmDescriptors);
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

   static const bool enableVectorLinkage = codeGen->getSupportsVectorRegisters();

   if (enableVectorLinkage)
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
   if (enableVectorLinkage) setVectorReturnRegister  (TR::RealRegister::VRF24);
   setStackPointerRegister  (TR::RealRegister::GPR4 );
   setNormalStackPointerRegister  (TR::RealRegister::GPR4 );
   setAlternateStackPointerRegister  (TR::RealRegister::GPR9 );
   setEntryPointRegister    (TR::RealRegister::GPR6);
   setLitPoolRegister       (TR::RealRegister::GPR8 );
   setReturnAddressRegister (TR::RealRegister::GPR7 );

   setEnvironmentPointerRegister (TR::RealRegister::GPR5  );
   setCAAPointerRegister   (TR::RealRegister::GPR12  );
   setDebugHooksRegister(TR::Compiler->target.is64Bit() ? TR::RealRegister::GPR10 : TR::RealRegister::GPR12);

   setIntegerArgumentRegister(0, TR::RealRegister::GPR1);
   setIntegerArgumentRegister(1, TR::RealRegister::GPR2);
   setIntegerArgumentRegister(2, TR::RealRegister::GPR3);
   setNumIntegerArgumentRegisters(3);

   setFloatArgumentRegister(0, TR::RealRegister::FPR0);
   setFloatArgumentRegister(1, TR::RealRegister::FPR2);
   setFloatArgumentRegister(2, TR::RealRegister::FPR4);
   setFloatArgumentRegister(3, TR::RealRegister::FPR6);
   setNumFloatArgumentRegisters(4);

   if (enableVectorLinkage)
      {
      int vecIndex = 0;
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF25);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF26);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF27);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF28);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF29);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF30);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF31);
      setVectorArgumentRegister(vecIndex++, TR::RealRegister::VRF24);
      setNumVectorArgumentRegisters(vecIndex);
      }

   setOffsetToFirstParm   ((TR::Compiler->target.is64Bit()) ? 2176 : 2112);
   setOffsetToRegSaveArea   (2048);
   setNumberOfDependencyGPRegisters(32);
   setFrameType(TR_XPLinkUnknownFrame);
   setLargestOutgoingArgumentAreaSize(0);
   setOffsetToLongDispSlot(0);

   setPrologueInfoCalculated(false);
   setGuardPageSize   ((TR::Compiler->target.is64Bit()) ? 1024*1024 : 4096);
   setAlternateStackPointerAdjust(0);
   setSaveBackChain(false); // default: no forced stack pointer save by prologue
   }


/**
 * Non-Java use.
 * Calculate XPLink call descriptor for given method body (excl. entry offset).
 * This value is present in PPA1 block and also used for determining register
 * linkage for incoming floating point parameters.
 */
uint32_t
TR::S390zOSSystemLinkage::calculateInterfaceMappingFlags(TR::ResolvedMethodSymbol *method)
   {
   uint32_t callDescValue;

   if (TR::Compiler->target.is64Bit())
      {
      return 0;
      }

   // Linkage field  (0 means XPLink)
   // Bits 0-2 inclusive
   //
   callDescValue = 0x00000000;

   //
   // Return value adjust field
   // Bits 3-7 inclusive
   //

   TR::DataType dataType = TR::NoType;
   int32_t aggregateLength = 0;
      TR_ASSERT( 0, "dont know how to get datatype of non-WCode method");
   uint32_t rva = calculateReturnValueAdjustFlag(dataType, aggregateLength);
   callDescValue |= rva << 24;

   //
   // Float parameter description fields
   // Bits 8-31 inclusive
   //

   uint32_t parmDescriptorFields = 0;

   ListIterator<TR::ParameterSymbol> parameterIterator(&method->getParameterList());
   parameterIterator.reset();
   TR::ParameterSymbol * parmCursor = parameterIterator.getFirst();
   TR::Symbol *funcSymbol = method;

   int32_t parmCount = 1;

   int32_t floatParmNum = 0;
   uint32_t parmAreaOffset = 0;
   uint32_t lastFloatParmAreaOffset= 0;

   bool done = false;
   while ((parmCursor != NULL) && !done)
      {
      TR::Symbol *parmSymbol = parmCursor;
      TR::DataType dataType = parmSymbol->getDataType();
      int32_t argSize = parmSymbol->getSize();

      done = updateFloatParmDescriptorFlags(&parmDescriptorFields, funcSymbol, parmCount, argSize, dataType, &floatParmNum, &lastFloatParmAreaOffset, &parmAreaOffset);

      parmCursor = parameterIterator.getNext();
      parmCount++;
      }

   callDescValue |= parmDescriptorFields;
   return callDescValue;
   }

/**
 * XPLink utility
 * Calculate "Return value adjust" component of XPLink call descriptor
 */
uint32_t
TR::S390zOSSystemLinkage::calculateReturnValueAdjustFlag(TR::DataType dataType, int32_t aggregateLength)
   {
   // 5 bit values for "return value adjust" field of XPLink descriptor
   #define XPLINK_RVA_RETURN_VOID_OR_UNUSED    0x00
   #define XPLINK_RVA_RETURN_INT32_OR_LESS     0x01
   #define XPLINK_RVA_RETURN_INT64             0x02
   #define XPLINK_RVA_RETURN_FAR_POINTER       0x04
   #define XPLINK_RVA_RETURN_FLOAT4            0x08
   #define XPLINK_RVA_RETURN_FLOAT8            0x09
   #define XPLINK_RVA_RETURN_FLOAT16           0x0A
   #define XPLINK_RVA_RETURN_COMPLEX4          0x0C
   #define XPLINK_RVA_RETURN_COMPLEX8          0x0D
   #define XPLINK_RVA_RETURN_COMPLEX16         0x0E
   #define XPLINK_RVA_RETURN_AGGREGATE         0x10 // lower bits have meaning

   uint32_t rva = 0;

   switch (dataType)
      {
      case TR::NoType:
           rva = XPLINK_RVA_RETURN_VOID_OR_UNUSED;
           break;
      case TR::Int8:
      case TR::Int16:
      case TR::Int32:
      case TR::Address:
           rva = XPLINK_RVA_RETURN_INT32_OR_LESS;
           break;
      case TR::Int64:
           rva = XPLINK_RVA_RETURN_INT64;
           break;
      case TR::Float:
#ifdef J9_PROJECT_SPECIFIC
      case TR::DecimalFloat:
#endif
           rva = XPLINK_RVA_RETURN_FLOAT4;
           break;
      case TR::Double:
#ifdef J9_PROJECT_SPECIFIC
      case TR::DecimalDouble:
#endif
           rva = XPLINK_RVA_RETURN_FLOAT8;
           break;
#ifdef J9_PROJECT_SPECIFIC
      case TR::DecimalLongDouble:
           rva = XPLINK_RVA_RETURN_FLOAT16;
           break;
      case TR::PackedDecimal:
      case TR::ZonedDecimal:
      case TR::ZonedDecimalSignLeadingEmbedded:
      case TR::ZonedDecimalSignLeadingSeparate:
      case TR::ZonedDecimalSignTrailingSeparate:
      case TR::UnicodeDecimal:
      case TR::UnicodeDecimalSignLeading:
      case TR::UnicodeDecimalSignTrailing:
#endif
      case TR::Aggregate:
           TR_ASSERT( aggregateLength != 0, "aggregate length is zero");
           rva = XPLINK_RVA_RETURN_AGGREGATE;
           if (isAggregateReturnedInIntRegisters(aggregateLength))
              {
              rva = rva + aggregateLength;
              }
           break;
      default:
           TR_ASSERT( 0, "unknown datatype for parm descriptor calculation");
           break;
      }

   return rva;
   }

/**
 * XPLink utility
 * Utility return to help float parameter fields of XPLink call descriptor
 * This routine is called once for each "parameter" unless the method returns
 * false - meaning float parameter fields of the call descriptor are now fully
 * determined.
 */
bool
TR::S390zOSSystemLinkage::updateFloatParmDescriptorFlags(uint32_t *parmDescriptorFields, TR::Symbol *funcSymbol, int32_t parmCount, int32_t argSize, TR::DataType dataType, int32_t *floatParmNum, uint32_t *lastFloatParmAreaOffset, uint32_t *parmAreaOffset)
   {
   uint32_t gprSize = cg()->machine()->getGPRSize();

   // Note: complex type is attempted to be handled although other code needs
   // to change in 390 codegen to support complex
   //
   // PERFORMANCE TODO: it is desirable to use the defined "parameter count" of
   // the function symbol to help determine if we have an unprototyped argument
   // of a call (site) to a vararg function.  Currently we overcompensate for
   // outgoing float parms to vararg functions and always shadow in FPR and
   // and stack/gprs as with an unprotoyped call - see pushArg(). Precise
   // information can help remove such compensation. Changes to fix this would
   // involve: this function, pushArg() and buildArgs().

   int32_t numFPRsNeeded = 0;
   switch (dataType)
      {
      case TR::Float:
      case TR::Double:
#ifdef J9_PROJECT_SPECIFIC
      case TR::DecimalFloat:
      case TR::DecimalDouble:
#endif
         numFPRsNeeded = 1;
         break;
#ifdef J9_PROJECT_SPECIFIC
      case TR::DecimalLongDouble:
         break;
#endif
      }

   if (numFPRsNeeded != 0)
      {
      uint32_t unitSize = argSize / numFPRsNeeded;
      uint32_t wordsToPreviousParm = ((*parmAreaOffset) - (*lastFloatParmAreaOffset))/gprSize;
      if (wordsToPreviousParm > 0xF)
         { // to big for descriptor. Will pass in stack
         return true; // done
         }
      uint32_t val = wordsToPreviousParm + ((unitSize == 4) ? 0x10 : 0x20);

      (*parmDescriptorFields) |= shiftFloatParmDescriptorFlag(val, (*floatParmNum));

      (*floatParmNum)++;
      if ((*floatParmNum) >= getNumFloatArgumentRegisters())
         {
         return true; // done
         }

      if (numFPRsNeeded == 2)
         {
         (*parmDescriptorFields) |= shiftFloatParmDescriptorFlag(0x33, (*floatParmNum));  // 0x33 == bits 110001
         (*floatParmNum)++;
         if ((*floatParmNum) >= getNumFloatArgumentRegisters())
            {
            return true; // done
            }
         }
      }
   (*parmAreaOffset) += argSize < gprSize ? gprSize : argSize;
   if (numFPRsNeeded != 0)
      (*lastFloatParmAreaOffset) = (*parmAreaOffset);

   return false;
   }

TR::Register *
TR::S390zOSSystemLinkage::loadEnvironmentIntoRegister(TR::Node * callNode, TR::RegisterDependencyConditions * deps, bool loadIntoEnvironmentRegister)
   {
   // Important note: this should only be called in pre-prologue phase for routines we know are
   // not leaf routines.

   TR::Register *envReg;
   if (loadIntoEnvironmentRegister)
      {
      TR::Register * systemEnvironmentRegister  = deps->searchPostConditionRegister(getEnvironmentPointerRegister()); // gpr 5
      envReg = systemEnvironmentRegister;
      }
   else
      {
      envReg = cg()->allocateRegister();
      }


   return envReg;
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

bool
TR::S390zOSSystemLinkage::isAggregateReturnedInIntRegistersAndMemory(int32_t aggregateLenth)
   {
   return isFastLinkLinkageType() &&
      (aggregateLenth  > (getNumIntegerArgumentRegisters()* cg()->machine()->getGPRSize()));
   }

bool
TR::S390zOSSystemLinkage::isAggregateReturnedInIntRegisters(int32_t aggregateLenth)
   {
   return aggregateLenth  <= (getNumIntegerArgumentRegisters()* cg()->machine()->getGPRSize());
   }


/**
 * General XPLink utility
 *
 * Calculate XPLink call descriptor for given call site (excl. entry offset).
 * "callNode"  can be null which means that this is a special call.
 * The retunred value is used for call descriptor emission and is also used to
 * determine floating point register likage for outgoing parameters.
 */
uint32_t
TR::S390zOSSystemLinkage::calculateCallDescriptorFlags(TR::Node *callNode)
   {
   #define XPLINK_FLOAT_PARM_UNPROTYPED_CALL 0x08
   uint32_t callDescValue;

   if (TR::Compiler->target.is64Bit())
      {
      return 0;
      }

   // Linkage field  (0 means XPLink)
   // Bits 0-2 inclusive
   //
   bool linkageIsXPLink;
   linkageIsXPLink = true;
   callDescValue = 0x00000000;   // xplink (0) value here


   //
   // Return value adjust field
   // Bits 3-7 inclusive
   //

   TR::DataType dataType;
   TR::ILOpCodes opcode;
   int32_t aggregateLength = 0;

   if (callNode == NULL)  // specialized calls no real descriptor needed
      dataType = TR::NoType;
   else
      {
      dataType = callNode->getDataType();
#ifdef J9_PROJECT_SPECIFIC
      if (callNode->getType().isBCD())
         aggregateLength = callNode->getSize();
#endif
      }

   uint32_t rva;
   if (linkageIsXPLink)
      {
      rva = calculateReturnValueAdjustFlag(dataType, aggregateLength);
      }
   else
      {
      // TOBEY generates 0 for non-XPLink cases.
      // We are consistent with TOBEY although not sure that is 100% correct
      rva = 0;
      }
   callDescValue |= rva << 24;

   if (callNode == NULL)
      {
      return callDescValue; // specialized calls - so no float requirements
      }

   //
   // Float parameter description fields
   // Bits 8-31 inclusive
   //

   bool isPrototyped = true; // All Java calls are prototyped.
   uint32_t parmAreaOffset = 0;

#ifdef J9_PROJECT_SPECIFIC
   TR::MethodSymbol* callSymbol = callNode->getSymbol()->castToMethodSymbol();
   if (callSymbol->isJNI() && callNode->isPreparedForDirectJNI())
      {
      TR::ResolvedMethodSymbol * cs = callSymbol->castToResolvedMethodSymbol();
      TR_ResolvedMethod * resolvedMethod = cs->getResolvedMethod();
      // JNI Calls include a JNIEnv* pointer that is not included in list of children nodes.
      // For FastJNI, certain calls do not require us to pass the JNIEnv.
      if (!cg()->fej9()->jniDoNotPassThread(resolvedMethod))
         parmAreaOffset += sizeof(uintptrj_t);

      // For FastJNI, certain calls do not have to pass in receiver object.
      if (cg()->fej9()->jniDoNotPassReceiver(resolvedMethod))
         parmAreaOffset -= sizeof(uintptrj_t);
      }
#endif

   uint32_t parmDescriptorFields = 0;

   if (!isPrototyped)
      {
      parmDescriptorFields = shiftFloatParmDescriptorFlag(XPLINK_FLOAT_PARM_UNPROTYPED_CALL, 0);  // FPR0 field has special value for unprototped call
      callDescValue |= parmDescriptorFields;
      return callDescValue;
      }

   // WCode only logic follows for float parameter description fields

   TR::Symbol *funcSymbol = callNode->getSymbolReference()->getSymbol();

   uint32_t firstArgumentChild = callNode->getFirstArgumentIndex();
   int32_t to = callNode->getNumChildren() - 1;
   int32_t parmCount = 1;

   int32_t floatParmNum = 0;

   uint32_t lastFloatParmAreaOffset= 0;

   bool done = false;
   for (int32_t i = firstArgumentChild; (i <= to) && !done; i++, parmCount++)
      {
      TR::Node *child = callNode->getChild(i);
      TR::DataType dataType = child->getDataType();
      TR::SymbolReference *parmSymRef = child->getOpCode().hasSymbolReference() ? child->getSymbolReference() : NULL;
      int32_t argSize = 0;

      if (parmSymRef==NULL)
         argSize = child->getSize();
      else
         argSize = parmSymRef->getSymbol()->getSize();

      done = updateFloatParmDescriptorFlags(&parmDescriptorFields, funcSymbol, parmCount, argSize, dataType, &floatParmNum, &lastFloatParmAreaOffset, &parmAreaOffset);
      }

   callDescValue |= parmDescriptorFields;
   return callDescValue;
   }

#define  GPREGINDEX(i)   (i-TR::RealRegister::FirstGPR)
   
// Calculate prologue info for XPLink
// This is only done once for multiple entry point routines
void TR::S390zOSSystemLinkage::calculatePrologueInfo(TR::Instruction * cursor)
   {
   TR::ResolvedMethodSymbol *bodySymbol = comp()->getJittedMethodSymbol();

   // Logic for main most of which applies to secondary entry points
   // I.E. all except for parameter offset information.

   int32_t localSize, argSize;
   int32_t stackFrameSize, alignedStackFrameSize;
   int32_t stackFrameBias = getStackFrameBias();

   // Set for initParamOffset() - relative offset from start of frame
   setOutgoingParmAreaBeginOffset(getOffsetToFirstParm()-stackFrameBias);

   {
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
   int32_t stackMappingIndex = stackFrameBias + stackFrameSize;
   mapStack(bodySymbol, stackMappingIndex);
   }

   setPrologueInfoCalculated(true);
   }

TR::Instruction * TR::S390zOSSystemLinkage::buyFrame(TR::Instruction * cursor, TR::Node * node)
   {
   enum TR_XPLinkFrameType frameType;
   int16_t GPRSaveMask;
   TR::LabelSymbol *stmLabel;

   int8_t gprSize = cg()->machine()->getGPRSize();
   int32_t stackFrameSize, offsetToRegSaveArea, offsetToFirstSavedReg, stackFrameBias, gpr3ParmOffset;
   int32_t  firstSaved, lastSaved, firstPossibleSaved;
   bool saveBackChain;

   // TODO: enlarge this intermediate threshold for 64 bit (up to 1M). Needs changes in code below
   int32_t intermediateThreshold = getGuardPageSize();

   TR::RealRegister *spReg = getNormalStackPointerRealRegister(); // normal sp reg used in prol/epil
   stackFrameSize = getStackFrameSize();
   stackFrameBias = getStackFrameBias();
   offsetToRegSaveArea = getOffsetToRegSaveArea();

   // This delta is slightly pessimistic and could be a "tad" more. But, taking
   // into account the "tad" complicates this code more than its worth.
   // I.E. small set of cases could be forced to being intermediate vs. small
   // frame types because of this "tad".
   int32_t delta = stackFrameBias - stackFrameSize;

   if (stackFrameSize > getGuardPageSize())
      // guard page option mandates explicit checking
      frameType = TR_XPLinkStackCheckFrame;
   else if (delta >=0)
      // delta > 0 implies stack offset for STM will be positive and <2K
      frameType = TR_XPLinkSmallFrame;
   else if (stackFrameSize < intermediateThreshold)
      frameType = TR_XPLinkIntermediateFrame;
   else
      frameType = TR_XPLinkStackCheckFrame;
   setFrameType(frameType);

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
      setSaveBackChain(true);

   saveBackChain = getSaveBackChain();
   if (saveBackChain)
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
      || ((frameType == TR_XPLinkIntermediateFrame) && saveBackChain))
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
             // (saveBackChain) LR R0,R4
             // (saveTempReg|saveBackChain) ST R3,D(R4)   <-- save temp register
             // R4<- R4 - stackSize
             // LR R3,R0
             // (saveTempReg) L  R3,D(R3)   <-- load temp register
             saveTempReg = true;  // PERFORMANCE NOTE: TODO: set to false if GPR3 is not used as a parameter

             if (saveTempReg || saveBackChain)
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
             // (saveBackChain)    LR R0,R4
             // AHI R4,-stackFrameSize
             // STM rx,ry,offset(R4)
             // (saveBackChain)    ST R0,D(R4)

             if (saveBackChain) // LR R0,R4
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
          if (saveBackChain)
             { // ST R0,2048(R4)
             rsaOffset = offsetToRegSaveArea + 0; // GPR4 saved at start of RSA
             rsa = generateS390MemoryReference(spReg, rsaOffset, cg());
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr0Real, rsa, cursor);
             }
          break;

      case TR_XPLinkStackCheckFrame:
          // TODO: move the code to exend stack to "cold" path
          // Code:
          //   important note: saveTempReg -> saveBackChain
          //
          //     (saveTempReg)      ST R3,parmOffsetR3(GPR4)
          //     (saveBackChain)    LR R0,R4
          //     AHI R4, -stackFrameSize  [this varies based on stackFrameSize]
          //     [31 bit] C  R4,BOS(R12)           R12=CAA
          //     [64 bit] LLGT  R3,1268            R3=control block - low mem
          //     [64 bit] C  R4,BOS(R3)            R3=ditto
          //     BL stmLabel
          //     /* extension logic */
          //     (!saveBackChain)    LR R0,R3   /* could be smarter here */
          //     L  R3,extender(R12)
          //     BASR R3,R3
          //     NOP x                    [xplink noop]
          //     (!saveBackChain)   LR R3,R0   /* could be smarter here */
          //
          //   stmLabel:
          //     STM rx,ry,offset(R4)
          //     (saveBackChain)    ST R0,2048(,R4)
          //     (saveTempReg)      LR R3,R0   /* could be smarter here */
          //     (saveTempReg)      L  R3,parmOffsetR3(GPR3)
          //
          stmLabel = generateLabelSymbol(cg());
          saveTempReg = saveBackChain;  // TODO: and with check that R3 is used as parm

          //------------------------------
          // Stack check - prologue part 1
          //------------------------------

          if (saveTempReg)
             {
             gpr3ParmRef = generateS390MemoryReference(spReg, gpr3ParmOffset, cg()); // used for temporary register
             cursor = generateRXInstruction(cg(), TR::InstOpCode::getStoreOpCode(), node, gpr3Real, gpr3ParmRef, cursor); // ST R3,parmRef(R4)
             }
          if (saveBackChain) // LR R0,R4
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
          if (!saveBackChain)
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

          if (!saveBackChain)
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
          if (saveBackChain)
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

void
TR::S390zOSSystemLinkage::createEntryPointMarker(TR::Instruction* cursor, TR::Node* node)
   {
   }

TR_XPLinkCallTypes
TR::S390zOSSystemLinkage::genWCodeCallBranchCode(TR::Node *callNode, TR::RegisterDependencyConditions * deps)
   {
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
      return callType;
      }

   TR::SymbolReference *callSymRef = callNode->getSymbolReference();
   TR::Symbol *callSymbol = callSymRef->getSymbol();

   TR::Instruction * callInstr = new (cg()->trHeapMemory()) TR::S390RILInstruction(TR::InstOpCode::BRASL, callNode, systemReturnAddressRegister, callSymbol, callSymRef, cg());
   callInstr->setDependencyConditions(preDeps);
   callType = TR_XPLinkCallType_BRASL7;

   return callType;
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

    TR_XPLinkCallTypes callType;  // for XPLink calls

    // only pre-dependencies from dep will be attached to the call
    callType = genWCodeCallBranchCode(callNode, deps);

    generateS390LabelInstruction(codeGen, InstOpCode::LABEL, callNode, returnFromJNICallLabel);

    genCallNOPAndDescriptor(NULL, callNode, callNode, callType);

    // Append post-dependencies after NOP
    TR::LabelSymbol * afterNOP = generateLabelSymbol(cg());
    TR::RegisterDependencyConditions * postDeps =
          new (trHeapMemory()) TR::RegisterDependencyConditions(NULL,
                deps->getPostConditions(), 0, deps->getAddCursorForPost(), cg());
    generateS390LabelInstruction(codeGen, InstOpCode::LABEL, callNode, afterNOP, postDeps);
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
   calculatePrologueInfo(cursor);

   TR::ResolvedMethodSymbol *bodySymbol = comp()->getJittedMethodSymbol();

   TR::Node *node = cursor->getNode();

   TR::CodeGenerator* cg = self()->cg();

   _ppa1 = new (self()->trHeapMemory()) TR::PPA1Snippet(cg, this);
   _ppa2 = new (self()->trHeapMemory()) TR::PPA2Snippet(cg, this);

   cg->addSnippet(_ppa1);
   cg->addSnippet(_ppa2);

   uint32_t stackFrameSize = getStackFrameSize();

   TR_ASSERT_FATAL((stackFrameSize & 31) == 0, "XPLINK stack frame size (%d) has to be aligned to 32-bytes.", stackFrameSize);
   
   cursor = cursor->getPrev();

   // "C.E.E.1"
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00C300C5, cursor);
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00C500F1, cursor);
   cursor = generateDataConstantInstruction(cg, TR::InstOpCode::DC, node, 0x00000000, cursor);

   cg->addRelocation(new (cg->trHeapMemory()) EntryPointMarkerOffsetToPPA1Relocation(cursor, _ppa1->getSnippetLabel()));
   
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
   enum TR_XPLinkFrameType frameType = getFrameType();
   TR::Node * currentNode = cursor->getNode();
   int32_t offset;

   int32_t stackFrameSize, offsetToFirstSavedReg, stackFrameBias;
   int32_t  firstSaved, lastSaved, firstPossibleSaved;
   TR::MemoryReference *rsa;

   int32_t blockNumber = -1;

   TR::RealRegister *spReg = getNormalStackPointerRealRegister(); // normal sp reg used in prol/epil
   stackFrameSize = getStackFrameSize();
   stackFrameBias = getStackFrameBias();

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
      offset = stackFrameBias + offsetToFirstSavedReg;
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
TR::S390zOSSystemLinkage::genCallNOPAndDescriptor(TR::Instruction * cursor,
                                                    TR::Node *node,
                                                    TR::Node *callNode,
                                                    TR_XPLinkCallTypes callType)
   {
   TR::CodeGenerator * codeGen = cg();
   // In 64 bit XPLINK, the caller returns at RetAddr+2, so add 2 byte nop
   // In 31 bit XPLINK, the caller returns at RetAddr+4, so add 4 byte nop with last two bytes
   // as signed offset in doublewords at or preceding NOP
   uint32_t padSize = TR::Compiler->target.is64Bit() ? 2 : 4;

   cursor = codeGen->insertPad(node, cursor, padSize, false);
   TR::S390NOPInstruction *nop = static_cast<TR::S390NOPInstruction *>(cursor);

   if (TR::Compiler->target.is32Bit())
      {
      uint32_t callDescValues = calculateCallDescriptorFlags(callNode);  // lower 32 bits

      TR::S390ConstantDataSnippet * callDescSnippet = cg()->findOrCreate8ByteConstant(node, (int64_t)callDescValues, false);
      nop->setTargetSnippet(callDescSnippet);

      // Create a pseudo instruction that will generate
      //      BRAS 4
      //      DC <Call Descriptor>
      // if the snippet is > 15k bytes away.

      TR::S390PseudoInstruction *pseudoCallDesc = static_cast<TR::S390PseudoInstruction *>(generateS390PseudoInstruction(codeGen, TR::InstOpCode::XPCALLDESC, node, NULL, cursor));
      // Assign this pseudoCallDesc to the NOP Instruction.
      static_cast<TR::S390NOPInstruction *>(cursor)->setCallDescInstr(pseudoCallDesc);
      cursor = pseudoCallDesc;
      }

   nop->setCallType(callType);
   return cursor;
   }

TR::S390zOSSystemLinkage::EntryPointMarkerOffsetToPPA1Relocation::EntryPointMarkerOffsetToPPA1Relocation(TR::Instruction* cursor, TR::LabelSymbol* ppa2)
   :
      TR::LabelRelocation(NULL, ppa2),
      _cursor(cursor)
   {
   }

void
TR::S390zOSSystemLinkage::EntryPointMarkerOffsetToPPA1Relocation::apply(TR::CodeGenerator* cg)
   {
   uint8_t* cursor = _cursor->getBinaryEncoding();

   // The 0x08 is the static distance from the field which we need to relocate to the start of the Entry Point Marker
   *reinterpret_cast<int32_t*>(cursor) = static_cast<int32_t>(getLabel()->getCodeLocation() - (cursor - 0x08));
   }
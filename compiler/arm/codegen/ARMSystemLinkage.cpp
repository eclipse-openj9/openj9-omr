/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#include "env/CompilerEnv.hpp"
#include "il/Symbol.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "il/symbol/RegisterMappedSymbol.hpp"
#include "il/symbol/StaticSymbol.hpp"
#include "codegen/ARMInstruction.hpp"
#include "codegen/Machine.hpp"
#include "codegen/ARMSystemLinkage.hpp"
#include "codegen/GCStackAtlas.hpp"
#include "codegen/GCStackMap.hpp"
#include "codegen/GenerateInstructions.hpp"
#ifdef J9_PROJECT_SPECIFIC
#include "codegen/CallSnippet.hpp"
#endif
#include "codegen/StackCheckFailureSnippet.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"

TR::ARMLinkageProperties TR::ARMSystemLinkage::properties =
    {                           // TR_System
    IntegersInRegisters|        // linkage properties
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
    FloatsInRegisters  |
#endif
    RightToLeft,
       {                        // register flags
       0,                       // NoReg
       IntegerReturn|           // gr0
       IntegerArgument,
       IntegerReturn|           // gr1
       IntegerArgument,
       IntegerArgument,         // gr2
       IntegerArgument,         // gr3
       Preserved,               // gr4
       Preserved,               // gr5
       Preserved,               // gr6
       Preserved,               // gr7
       Preserved,               // gr8
       Preserved,               // gr9
       Preserved,               // gr10
       Preserved,               // gr11
       Preserved,               // gr12
       Preserved,               // gr13
       Preserved,               // gr14
       Preserved,               // gr15
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
       FloatArgument|           // fp0
       FloatReturn,
       FloatArgument,           // fp1
       FloatArgument,           // fp2
       FloatArgument,           // fp3
       FloatArgument,           // fp4
       FloatArgument,           // fp5
       FloatArgument,           // fp6
       FloatArgument,           // fp7
       Preserved,               // fp8
       Preserved,               // fp9
       Preserved,               // fp10
       Preserved,               // fp11
       Preserved,               // fp12
       Preserved,               // fp13
       Preserved,               // fp14
       Preserved,               // fp15
#else
       0,                        // fp0
       0,                        // fp1
       0,                        // fp2
       0,                        // fp3
       0,                        // fp4
       0,                        // fp5
       0,                        // fp6
       0,                        // fp7
#endif
       },
       {                        // preserved registers
       TR::RealRegister::gr4,
       TR::RealRegister::gr5,
       TR::RealRegister::gr6,
       TR::RealRegister::gr7,
       TR::RealRegister::gr8,
       TR::RealRegister::gr9,
       TR::RealRegister::gr10,
       TR::RealRegister::gr11,
       TR::RealRegister::gr12,
       TR::RealRegister::gr13,
       TR::RealRegister::gr14,
       TR::RealRegister::gr15,
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
       TR::RealRegister::fp8,
       TR::RealRegister::fp9,
       TR::RealRegister::fp10,
       TR::RealRegister::fp11,
       TR::RealRegister::fp12,
       TR::RealRegister::fp13,
       TR::RealRegister::fp14,
       TR::RealRegister::fp15,
#endif
       },
       {                        // argument registers
       TR::RealRegister::gr0,
       TR::RealRegister::gr1,
       TR::RealRegister::gr2,
       TR::RealRegister::gr3,
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
       TR::RealRegister::fp0,
       TR::RealRegister::fp1,
       TR::RealRegister::fp2,
       TR::RealRegister::fp3,
       TR::RealRegister::fp4,
       TR::RealRegister::fp5,
       TR::RealRegister::fp6,
       TR::RealRegister::fp7,
#endif
       },
       {                        // return registers
       TR::RealRegister::gr0,
       TR::RealRegister::gr1,
#if defined(__VFP_FP__) && !defined(__SOFTFP__)
       TR::RealRegister::fp0,
#endif
       },
       MAX_ARM_GLOBAL_GPRS,     // numAllocatableIntegerRegisters
       MAX_ARM_GLOBAL_FPRS,     // numAllocatableFloatRegisters
       0x0000fff0,              // preserved register map
       TR::RealRegister::gr11, // frame register
       TR::RealRegister::gr8, // method meta data register
       TR::RealRegister::gr13, // stack pointer register
       TR::RealRegister::NoReg, // vtable index register
       TR::RealRegister::NoReg,  // j9method argument register
       15,                      // numberOfDependencyGPRegisters
       0,                       // offsetToFirstStackParm (relative to old sp)
       -4,                      // offsetToFirstLocal (not counting out-args)
       4,                       // numIntegerArgumentRegisters
       0,                       // firstIntegerArgumentRegister (index into ArgumentRegisters)
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
       8,                       // numFloatArgumentRegisters
       4,                       // firstFloatArgumentRegister (index into ArgumentRegisters)
       0,                       // firstIntegerReturnRegister
       2                        // firstFloatReturnRegister
#else
       0,                       // numFloatArgumentRegisters
       0,                       // firstFloatArgumentRegister (index into ArgumentRegisters)
       0,                       // firstIntegerReturnRegister
       0                        // firstFloatReturnRegister
#endif
    };

void TR::ARMSystemLinkage::initARMRealRegisterLinkage()
   {
#if 0
   // Each real register's weight is set to match this linkage convention
   TR_ARMMachine *machine = cg()->getARMMachine();
   int icount;

   icount = TR::RealRegister::gr1;
   machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setState(TR::RealRegister::Locked);
   machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setAssignedRegister(machine->getARMRealRegister((TR::RealRegister::RegNum)icount));

   icount = TR::RealRegister::gr2;
   machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setState(TR::RealRegister::Locked);
   machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setAssignedRegister(machine->getARMRealRegister((TR::RealRegister::RegNum)icount));

   for (icount=TR::RealRegister::gr3; icount<=TR::RealRegister::gr12; icount++)
      machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setWeight(icount);

   for (icount=TR::RealRegister::LastGPR; icount>=TR::RealRegister::gr13; icount--)
      machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setWeight(0xf000-icount);

   for (icount=TR::RealRegister::FirstFPR;        icount<=TR::RealRegister::fp3; icount++)
      machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setWeight(icount);

   for (icount=TR::RealRegister::LastFPR; icount>=TR::RealRegister::fp4; icount--)
      machine->getARMRealRegister((TR::RealRegister::RegNum)icount)->setWeight(0xf000-icount);
#else
   TR_ASSERT(0, "unimplemented");
#endif
   }

uint32_t TR::ARMSystemLinkage::getRightToLeft()
   {
   return getProperties().getRightToLeft();
   }

void TR::ARMSystemLinkage::mapStack(TR::ResolvedMethodSymbol *method)
   {
#if 0
   ListIterator<TR::AutomaticSymbol>  automaticIterator(&method->getAutomaticList());
   TR::AutomaticSymbol               *localCursor       = automaticIterator.getFirst();
   const TR::ARMLinkageProperties&    linkage           = getProperties();
   TR_ARMCodeGenerator              *codeGen           = cg();
   TR_ARMMachine                    *machine           = codeGen->getARMMachine();
   uint32_t                          stackIndex;
   int32_t                           lowGCOffset;
   TR::GCStackAtlas                  *atlas             = codeGen->getStackAtlas();

   // map all garbage collected references together so can concisely represent
   // stack maps. They must be mapped so that the GC map index in each local
   // symbol is honoured.

   stackIndex = linkage.getOffsetToFirstLocal() +
                codeGen->getLargestOutgoingArgSize();
   lowGCOffset = stackIndex;
   int32_t firstLocalGCIndex = atlas->getNumberOfParmSlotsMapped();

   // Map local references to get the right amount of stack reserved
   //
   for (localCursor = automaticIterator.getFirst(); localCursor; localCursor = automaticIterator.getNext())
      if (localCursor->getGCMapIndex() >= 0)
         mapSingleAutomatic(localCursor,stackIndex);

   // Map local references again to set the stack position correct according to
   // the GC map index.
   //
   automaticIterator.reset();
   for (localCursor = automaticIterator.getFirst(); localCursor; localCursor = automaticIterator.getNext())
      if (localCursor->getGCMapIndex() >= 0)
         localCursor->setOffset(stackIndex +4*(localCursor->getGCMapIndex()-firstLocalGCIndex));

   method->setObjectTempSlots((lowGCOffset-stackIndex) >> 2);
   atlas->setLocalBaseOffset(lowGCOffset);
   lowGCOffset = stackIndex;

   // Now map the rest of the locals
   //
   automaticIterator.reset();
   localCursor = automaticIterator.getFirst();

   while (localCursor != NULL)
      {
      if (localCursor->getGCMapIndex() < 0)
         mapSingleAutomatic(localCursor, stackIndex);
      localCursor = automaticIterator.getNext();
      }
   method->setScalarTempSlots((stackIndex-lowGCOffset) >> 2);

   TR::RealRegister::RegNum regIndex = TR::RealRegister::gr13;
   if (!method->isEHAware())
      {
      while (regIndex<=TR::RealRegister::LastGPR && !machine->getARMRealRegister(regIndex)->getHasBeenAssignedInMethod())
         regIndex = (TR::RealRegister::RegNum)((uint32_t)regIndex+1);
      }
   stackIndex += (TR::RealRegister::LastGPR-regIndex+1)*4;
   // Make the stack frame doubleword aligned.
   stackIndex = ((stackIndex+7)>>3)<<3;

   regIndex = TR::RealRegister::fp4;  // TODO - random fix
   if (!method->isEHAware())
      {
      while (regIndex<=TR::RealRegister::LastFPR && !machine->getARMRealRegister(regIndex)->getHasBeenAssignedInMethod())
         regIndex = (TR::RealRegister::RegNum)((uint32_t)regIndex+1);
      }
   stackIndex += (TR::RealRegister::LastFPR-regIndex+1)*8;
   method->setLocalMappingCursor(stackIndex);

   ListIterator<TR::ParameterSymbol> parameterIterator(&method->getParameterList());
   TR::ParameterSymbol              *parmCursor = parameterIterator.getFirst();
   int32_t                          offsetToFirstParm = linkage.getOffsetToFirstParm();
   if (linkage.getRightToLeft())
      {
      while (parmCursor != NULL)
         {
         parmCursor->setParameterOffset(parmCursor->getParameterOffset() + offsetToFirstParm + stackIndex);
         parmCursor = parameterIterator.getNext();
         }
      }
   else
      {
      uint32_t sizeOfParameterArea = method->getNumParameterSlots() << 2;
      while (parmCursor != NULL)
         {
         parmCursor->setParameterOffset(sizeOfParameterArea -
                                        parmCursor->getParameterOffset() -
                                        parmCursor->getSize() + stackIndex +
                                        offsetToFirstParm);
         parmCursor = parameterIterator.getNext();
         }
      }

   atlas->setParmBaseOffset(atlas->getParmBaseOffset() + offsetToFirstParm + stackIndex);
#else
   TR_ASSERT(0, "unimplemented");
#endif
   }

void TR::ARMSystemLinkage::mapSingleAutomatic(TR::AutomaticSymbol *p, uint32_t &stackIndex)
   {
#if 0
   p->setOffset(stackIndex);
   if (p->getSize() <= 4)
      {
      stackIndex += 4;
      }
   else
      {
      stackIndex += 8;
      }
#else
   TR_ASSERT(0, "unimplemented");
#endif
   }

TR::ARMLinkageProperties& TR::ARMSystemLinkage::getProperties()
   {
   return properties;
   }

void TR::ARMSystemLinkage::createPrologue(TR::Instruction *cursor)
   {
   TR_ASSERT(0, "unimplemented");
   }

void TR::ARMSystemLinkage::createEpilogue(TR::Instruction *cursor)
   {
   TR_ASSERT(0, "unimplemented");
   }

TR::MemoryReference *TR::ARMSystemLinkage::getOutgoingArgumentMemRef(int32_t               totalSize,
                                                                      int32_t               offset,
                                                                      TR::Register          *argReg,
                                                                      TR_ARMOpCodes         opCode,
                                                                      TR::ARMMemoryArgument &memArg)
   {
   int32_t                spOffset = offset - (getProperties().getNumIntArgRegs() * TR::Compiler->om.sizeofReferenceAddress());
   TR::RealRegister    *sp       = cg()->machine()->getARMRealRegister(properties.getStackPointerRegister());
   TR::MemoryReference *result   = new (trHeapMemory()) TR::MemoryReference(sp, spOffset, cg());

   memArg.argRegister = argReg;
   memArg.argMemory   = result;
   memArg.opCode      = opCode;

   return result;
   }

int32_t TR::ARMSystemLinkage::buildArgs(TR::Node                            *callNode,
                                       TR::RegisterDependencyConditions *dependencies,
                                       TR::Register*                       &vftReg,
                                       bool                                isJNI)
   {
   return buildARMLinkageArgs(callNode, dependencies, vftReg, TR_System, isJNI);
   }

TR::Register *TR::ARMSystemLinkage::buildDirectDispatch(TR::Node *callNode)
   {
   return buildARMLinkageDirectDispatch(callNode, true);
   }

TR::Register *TR::ARMSystemLinkage::buildIndirectDispatch(TR::Node *callNode)
   {
   TR_ASSERT(0, "unimplemented");
   return NULL;
   }

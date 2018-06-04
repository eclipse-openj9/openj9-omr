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

#include <stddef.h>
#include <stdint.h>
#include "codegen/Linkage.hpp"

void OMR::ARM64::Linkage::mapStack(TR::ResolvedMethodSymbol *method)
   {
   /* do nothing */
   }

void OMR::ARM64::Linkage::mapSingleAutomatic(TR::AutomaticSymbol *p, uint32_t &stackIndex)
   {
   /* do nothing */
   }

void OMR::ARM64::Linkage::initARM64RealRegisterLinkage()
   {
   /* do nothing */
   }

void OMR::ARM64::Linkage::setParameterLinkageRegisterIndex(TR::ResolvedMethodSymbol *method)
   {
   /* do nothing */
   }

bool OMR::ARM64::Linkage::hasToBeOnStack(TR::ParameterSymbol *parm)
   {
   return(false);
   }

TR::Instruction *OMR::ARM64::Linkage::saveArguments(TR::Instruction *cursor)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return cursor;
   }

TR::Instruction *OMR::ARM64::Linkage::loadUpArguments(TR::Instruction *cursor)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return cursor;
   }

TR::Instruction *OMR::ARM64::Linkage::flushArguments(TR::Instruction *cursor)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return cursor;
   }

TR::Register *OMR::ARM64::Linkage::pushIntegerWordArg(TR::Node *child)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

TR::Register *OMR::ARM64::Linkage::pushAddressArg(TR::Node *child)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

TR::Register *OMR::ARM64::Linkage::pushLongArg(TR::Node *child)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

TR::Register *OMR::ARM64::Linkage::pushFloatArg(TR::Node *child)
   {
   TR::Register *pushRegister = self()->cg()->evaluate(child);
   self()->cg()->decReferenceCount(child);
   return pushRegister;
   }

TR::Register *OMR::ARM64::Linkage::pushDoubleArg(TR::Node *child)
   {
   TR::Register *pushRegister = self()->cg()->evaluate(child);
   self()->cg()->decReferenceCount(child);
   return pushRegister;
   }

int32_t
OMR::ARM64::Linkage::numArgumentRegisters(TR_RegisterKinds kind)
   {
   switch (kind)
      {
      case TR_GPR:
         return self()->getProperties().getNumIntArgRegs();
      case TR_FPR:
         return self()->getProperties().getNumFloatArgRegs();
      default:
         return 0;
      }
   }

TR_HeapMemory
OMR::ARM64::Linkage::trHeapMemory()
   {
   return self()->trMemory();
   }

TR_StackMemory
OMR::ARM64::Linkage::trStackMemory()
   {
   return self()->trMemory();
   }

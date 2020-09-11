/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
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
#include "codegen/MemoryReference.hpp"
#include "il/MethodSymbol.hpp"
#include "il/SymbolReference.hpp"
#include "infra/Assert.hpp"
#include "objectfmt/GlobalFunctionCallData.hpp"
#include "objectfmt/JitCodeObjectFormat.hpp"
#include "runtime/Runtime.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"
#include "runtime/CodeCacheManager.hpp"


TR::Instruction *
OMR::Z::JitCodeObjectFormat::emitGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *callSymRef = data.globalMethodSymRef;
   TR::CodeGenerator *cg = data.cg;
   if (callSymRef == NULL)
      {
      TR_ASSERT_FATAL(data.runtimeHelperIndex > 0, "GlobalFunctionCallData does not contain symbol reference for call or valid TR_RuntimeHelper");
      callSymRef = cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }

   uintptr_t targetAddress = data.targetAddress != NULL ? data.targetAddress : reinterpret_cast<uintptr_t>(callSymRef->getMethodAddress());

   TR::Node *callNode = data.callNode;
   TR_ASSERT_FATAL(targetAddress != NULL, "Unable to make a call for n%dn %s as targetAddress for the Call is not found.", callNode->getGlobalIndex(), callNode->getOpCode().getName());

   TR::Instruction *callInstr = NULL;

   // Now we are going to make call. For that, ReturnAddress register should be set.
   TR_ASSERT_FATAL(data.returnAddressReg != NULL, "returnAddressReg should be set to make a call for n%dn %s", callNode->getGlobalIndex(), callNode->getOpCode().getName());

   if (cg->canUseRelativeLongInstructions(targetAddress))
      {
      callInstr = generateRILInstruction(cg, TR::InstOpCode::BRASL, callNode, data.returnAddressReg, callSymRef, (void *)targetAddress, data.prevInstr);
      }
   else
      {
		TR_ASSERT_FATAL(false, "targetAddress %p for callNode n%dn is not in relative addressing range", targetAddress, callNode->getGlobalIndex());
      }
   data.out_callInstr = callInstr;
   if (data.regDeps != NULL)
      {
      callInstr->setDependencyConditions(data.regDeps);
      }
   return callInstr;
   }


uint8_t *
OMR::Z::JitCodeObjectFormat::encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *callSymRef = data.globalMethodSymRef;
   TR::CodeGenerator *cg = data.cg;
   TR::Node *callNode = data.callNode;

   if (callSymRef == NULL)
      {
      TR_ASSERT_FATAL(data.runtimeHelperIndex > 0, "GlobalFunctionCallData does not contain symbol reference for call or valid TR_RuntimeHelper");
      callSymRef = cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }
   
   uintptr_t targetAddress = data.targetAddress ? data.targetAddress : reinterpret_cast<uintptr_t>(callSymRef->getMethodAddress());

   uint8_t *cursor = data.bufferAddress;

   TR_ASSERT_FATAL(targetAddress != NULL, "Unable to make a call for n%dn %s as targetAddress for the Call is not found.", callNode->getGlobalIndex(), callNode->getOpCode().getName());
   if (data.snippet->getKind() != TR::Snippet::IsUnresolvedCall || data.snippet->getKind() != TR::Snippet::IsUnresolvedData)
      {
      // TODO: Following part of the code should be in common codegen function so that every part of the JIT can use it.
#if defined(TR_TARGET_64BIT)
#if defined(J9ZOS390)
      if (cg->comp()->getOption(TR_EnableRMODE64))
#endif
         {
         // TODO: There is implicit cast from uintptr_t to intptr_t here. As they represent the address, should the directCallRequiresTrampoline needs uintptr_t? 
         if (cg->directCallRequiresTrampoline(targetAddress, reinterpret_cast<intptr_t>(cursor)))
            {
            targetAddress = static_cast<uintptr_t>(TR::CodeCacheManager::instance()->findHelperTrampoline(callSymRef->getReferenceNumber(), (void *)cursor));
            if (data.snippet != NULL)
               data.snippet->setUsedTrampoline(true);
            }
         }
#endif
      // ENCODING BRASL R14,address
      *reinterpret_cast<int16_t *>(cursor) = 0xC0E5;
      cursor += sizeof(int16_t);
      // Address Calculation in half words
      *reinterpret_cast<int32_t *>(cursor) = (int32_t)((targetAddress - (uintptr_t)(data.bufferAddress)) / 2);
      cursor += sizeof(int32_t);
      }
   else
      {
      *reinterpret_cast<int16_t *>(cursor) = 0xC0E0; // ENCODING LARL
      cursor += sizeof(int16_t);
      intptr_t dataConstantAddress = reinterpret_cast<intptr_t>(cursor + globalFunctionCallBinaryLength() + data.snippet->getPadBytes() - 2); // Getting Address of constant data area
      *reinterpret_cast<int32_t *>(cursor) = static_cast<int32_t>((dataConstantAddress - (intptr_t)(cursor) + 2) / 2);
      cursor += sizeof(int32_t);
      uint32_t rEP = static_cast<uint32_t>(cg->getEntryPointRegister()) - 1;
      if (cg->comp()->target().is64Bit())
         {
        *reinterpret_cast<int32_t *>(cursor) = 0xe300e000 + (rEP << 20);           // LG  rEP, 0(r14)
         cursor += sizeof(int32_t);
         *reinterpret_cast<int16_t *>(cursor) = 0x0004;
         cursor += sizeof(int16_t);
         }
      else
         {
        *reinterpret_cast<int32_t *>(cursor) = 0xe300e000 + (rEP << 20);           // LGF  rEP, 0(r14)
         cursor += sizeof(int32_t);
         *reinterpret_cast<int16_t *>(cursor) = 0x0014;
         cursor += sizeof(int16_t);
         }
		// BCR   rEP
      *reinterpret_cast<int16_t *>(cursor) = 0x07F0 + rEP;
      cursor += sizeof(int16_t);
      }
   data.bufferAddress = cursor;
   return data.bufferAddress;
   }

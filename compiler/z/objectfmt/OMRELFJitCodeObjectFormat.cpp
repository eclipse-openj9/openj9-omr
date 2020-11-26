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

#include "codegen/CCData.hpp"
#include "codegen/CCData_inlines.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/MemoryReference.hpp"
#include "il/MethodSymbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/StaticSymbol.hpp"
#include "infra/Assert.hpp"
#include "objectfmt/GlobalFunctionCallData.hpp"
#include "objectfmt/JitCodeObjectFormat.hpp"
#include "objectfmt/ELFJitCodeObjectFormat.hpp"
#include "runtime/CodeCache.hpp"
#include "runtime/CodeCacheManager.hpp"
#include "runtime/Runtime.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"


TR::Instruction *
OMR::Z::ELFJitCodeObjectFormat::emitGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *callSymRef = data.globalMethodSymRef;
   TR::CodeGenerator *cg = data.cg;
   

   if (callSymRef == NULL)
      {
      TR_ASSERT_FATAL(data.runtimeHelperIndex > 0, "GlobalFunctionCallData does not contain symbol reference for call or valid TR_RuntimeHelper");
      callSymRef = cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }
   
   uintptr_t targetAddress = data.targetAddress ? data.targetAddress : reinterpret_cast<uintptr_t>(callSymRef->getMethodAddress());

   TR::Node *callNode = data.callNode;
   TR_ASSERT_FATAL(targetAddress != NULL, "Unable to make a call for n%dn %s as targetAddress for the Call is not found.", callNode->getGlobalIndex(), callNode->getOpCode().getName());

   TR::CCData *codeCacheData = cg->getCodeCache()->manager()->getCodeCacheData();
   TR::CCData::index_t index;
   
   if (!(codeCacheData->put(NULL, sizeof(ccGlobalFunctionData), alignof(ccGlobalFunctionData), NULL, index)))
      {
      cg->comp()->failCompilation<TR::CompilationException>("Could not allocate global function data");
      }

   ccGlobalFunctionData *ccGlobalFunctionDataAddress = codeCacheData->get<ccGlobalFunctionData>(index);

	TR_ASSERT_FATAL(cg->canUseRelativeLongInstructions(reinterpret_cast<int64_t>(ccGlobalFunctionDataAddress)), "ccGlobalFunctionDataAddress is outside relative immediate range", targetAddress);
   ccGlobalFunctionDataAddress->address = targetAddress;

   TR::StaticSymbol *globalFunctionDataSymbol =
      TR::StaticSymbol::createWithAddress(cg->trHeapMemory(), TR::Address, reinterpret_cast<void *>(ccGlobalFunctionDataAddress));
   globalFunctionDataSymbol->setNotDataAddress();

   TR_ASSERT_FATAL(data.returnAddressReg != NULL, "returnAddressReg should be set to make a call for n%dn %s", callNode->getGlobalIndex(), callNode->getOpCode().getName());

   if (data.entryPointReg == NULL)
      data.entryPointReg = data.returnAddressReg;
   
   data.out_loadInstr = generateRILInstruction(cg, TR::InstOpCode::LGRL, callNode, data.entryPointReg, reinterpret_cast<void *>(ccGlobalFunctionDataAddress), data.prevInstr);   

   data.out_callInstr = generateRRInstruction(cg, TR::InstOpCode::BASR, callNode, data.returnAddressReg, data.entryPointReg, data.out_loadInstr);

   if (data.regDeps != NULL)
      {
      data.out_callInstr->setDependencyConditions(data.regDeps);
      }

   return data.out_callInstr;
   }


uint8_t *
OMR::Z::ELFJitCodeObjectFormat::encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *callSymRef = data.globalMethodSymRef;
   TR::CodeGenerator *cg = data.cg;

   if (callSymRef == NULL)
      {
      TR_ASSERT_FATAL(data.runtimeHelperIndex > 0, "GlobalFunctionCallData does not contain symbol reference for call or valid TR_RuntimeHelper");
      callSymRef = cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }
   
   uintptr_t targetAddress = data.targetAddress != NULL ? data.targetAddress : reinterpret_cast<uintptr_t>(callSymRef->getMethodAddress());

   uint8_t *cursor = data.bufferAddress;

   TR::Node *callNode = data.callNode;
   TR_ASSERT_FATAL(targetAddress != NULL, "Unable to make a call for n%dn %s as targetAddress for the Call is not found.", callNode->getGlobalIndex(), callNode->getOpCode().getName());

   ccGlobalFunctionData *ccGlobalFunctionDataAddress = 
      reinterpret_cast<ccGlobalFunctionData *>(cg->allocateCodeMemory(sizeof(ccGlobalFunctionData), false));

   if (!ccGlobalFunctionDataAddress)
      {
      cg->comp()->failCompilation<TR::CompilationException>("Could not allocate global function call data");
      }
   
	TR_ASSERT_FATAL(cg->canUseRelativeLongInstructions(reinterpret_cast<int64_t>(ccGlobalFunctionDataAddress)), "ccGlobalFunctionDataAddress is outside relative immediate range", targetAddress);
   ccGlobalFunctionDataAddress->address = targetAddress;

   // LGRL R14,<displacementForMemoryReferencePointingToCCGlobalFunctionCallData>
   // BASR R14,R14

   *reinterpret_cast<int16_t *>(cursor) = 0xC4E8;
   cursor += sizeof(int16_t);
   *reinterpret_cast<int32_t *>(cursor) = static_cast<int32_t>((reinterpret_cast<intptr_t>(ccGlobalFunctionDataAddress) - (intptr_t)cursor + 2) / 2);
   cursor += sizeof(int32_t);
   *reinterpret_cast<int16_t *>(cursor) = 0x0DEE;
   cursor += sizeof(int16_t);
   data.bufferAddress = cursor;
   return data.bufferAddress;
   }

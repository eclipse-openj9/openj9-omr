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
#include "codegen/X86Instruction.hpp"
#include "compile/Compilation.hpp"
#include "il/MethodSymbol.hpp"
#include "il/SymbolReference.hpp"
#include "infra/Assert.hpp"
#include "objectfmt/GlobalFunctionCallData.hpp"
#include "objectfmt/ELFJitCodeObjectFormat.hpp"
#include "runtime/Runtime.hpp"


TR::Instruction *
OMR::X86::AMD64::ELFJitCodeObjectFormat::emitGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *globalMethodSymRef;

   if (data.runtimeHelperIndex > 0)
      {
      globalMethodSymRef = data.cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }
   else
      {
      globalMethodSymRef = data.globalMethodSymRef;
      }

   /**
    * If a targetAddress has been provided then use that to override whatever would be
    * derived from the symbol reference.
    */
   TR_ASSERT_FATAL(!(data.runtimeHelperIndex && data.targetAddress), "a runtime helper and target address cannot both be provided");

   uintptr_t targetAddress = data.targetAddress ? data.targetAddress : (uintptr_t)globalMethodSymRef->getMethodAddress();

   TR_ASSERT_FATAL(targetAddress, "global function address is unknown");

   data.cg->resetIsLeafMethod();

   TR::Compilation *comp = data.cg->comp();

   ccGlobalFunctionData *ccGlobalFunctionDataAddress =
      reinterpret_cast<ccGlobalFunctionData *>(data.cg->allocateCodeMemory(sizeof(ccGlobalFunctionData), false));

   if (!ccGlobalFunctionDataAddress)
      {
      comp->failCompilation<TR::CompilationException>("Could not allocate global function data");
      }

   ccGlobalFunctionDataAddress->address = targetAddress;

   TR::StaticSymbol *globalFunctionDataSymbol =
      TR::StaticSymbol::createWithAddress(comp->trHeapMemory(), TR::Address, reinterpret_cast<void *>(ccGlobalFunctionDataAddress));
   globalFunctionDataSymbol->setNotDataAddress();
   TR::SymbolReference *globalFunctionDataSymRef = new (comp->trHeapMemory()) TR::SymbolReference(comp->getSymRefTab(), globalFunctionDataSymbol, 0);

   TR::X86CallMemInstruction *callInstr;
   TR::MemoryReference *callMR = new (comp->trHeapMemory()) TR::MemoryReference(globalFunctionDataSymRef, data.cg, true);

   if (data.prevInstr)
      {
      callInstr = generateCallMemInstruction(data.prevInstr, CALLMem, callMR, data.regDeps, data.cg);
      }
   else
      {
      callInstr = generateCallMemInstruction(CALLMem, data.callNode, callMR, data.regDeps, data.cg);
      }

   if (data.adjustsFramePointerBy != 0)
      {
      callInstr->setAdjustsFramePointerBy(data.adjustsFramePointerBy);
      }

/*
   if (data.reloKind != TR_NoRelocation) // FIXME
      {
      loadInstr->setReloKind(data.reloKind);
      }
*/

   data.out_callInstr = callInstr;

   return callInstr;
   }


uint8_t *
OMR::X86::AMD64::ELFJitCodeObjectFormat::encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   TR::SymbolReference *globalMethodSymRef;

   if (data.runtimeHelperIndex > 0)
      {
      globalMethodSymRef = data.cg->symRefTab()->findOrCreateRuntimeHelper(data.runtimeHelperIndex, false, false, false);
      }
   else
      {
      globalMethodSymRef = data.globalMethodSymRef;
      }

   /**
    * If a targetAddress has been provided then use that to override whatever would be
    * derived from the symbol reference.
    */
   TR_ASSERT_FATAL(!(data.runtimeHelperIndex && data.targetAddress), "a runtime helper and target address cannot both be provided");

   uintptr_t targetAddress = data.targetAddress ? data.targetAddress : (uintptr_t)globalMethodSymRef->getMethodAddress();

   TR_ASSERT_FATAL(targetAddress, "global function address is unknown");

   data.cg->resetIsLeafMethod();

   TR::Compilation *comp = data.cg->comp();

   ccGlobalFunctionData *ccGlobalFunctionDataAddress =
      reinterpret_cast<ccGlobalFunctionData *>(data.cg->allocateCodeMemory(sizeof(ccGlobalFunctionData), false));

   if (!ccGlobalFunctionDataAddress)
      {
      comp->failCompilation<TR::CompilationException>("Could not allocate global function data");
      }

   ccGlobalFunctionDataAddress->address = targetAddress;

   *data.bufferAddress++ = 0xff;  // CALL | JMP Mem
   uint8_t modRM = data.isCall ? 0x15 : 0x25;  // RIP addressing mode
   *data.bufferAddress++ = modRM;

   TR_ASSERT_FATAL(comp->target().cpu.isTargetWithinRIPRange(
                      reinterpret_cast<intptr_t>(ccGlobalFunctionDataAddress),
                      reinterpret_cast<intptr_t>(data.bufferAddress+4)),
                      "ccGlobalFunctionData must be reachable directly");

   int32_t disp32 = (int32_t)(reinterpret_cast<intptr_t>(ccGlobalFunctionDataAddress) - reinterpret_cast<intptr_t>(data.bufferAddress+4));
   *(int32_t *)data.bufferAddress = disp32;

   data.out_encodedMethodAddressLocation = reinterpret_cast<uint8_t *>(ccGlobalFunctionDataAddress);
   data.bufferAddress += 4;

   return data.bufferAddress;
   }

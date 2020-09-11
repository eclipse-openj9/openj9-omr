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
#include "objectfmt/JitCodeObjectFormat.hpp"
#include "runtime/Runtime.hpp"


TR::Instruction *
OMR::X86::AMD64::JitCodeObjectFormat::emitGlobalFunctionCall(TR::GlobalFunctionCallData &data)
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

   TR::Instruction *callInstr;

   if (data.runtimeHelperIndex || globalMethodSymRef->getSymbol()->castToMethodSymbol()->isHelper())
      {
      TR::X86ImmSymInstruction *callImmSym;
      TR_X86OpCodes op = data.isCall ? CALLImm4 : JMP4;

      if (data.prevInstr)
         {
         callImmSym = generateImmSymInstruction(data.prevInstr, op, targetAddress, globalMethodSymRef, data.regDeps, data.cg);
         }
      else
         {
         callImmSym = generateImmSymInstruction(op, data.callNode, targetAddress, globalMethodSymRef, data.regDeps, data.cg);
         }

      if (data.adjustsFramePointerBy != 0)
         {
         callImmSym->setAdjustsFramePointerBy(data.adjustsFramePointerBy);
         }

      callInstr = callImmSym;
      }
   else
      {
      TR_ASSERT_FATAL(data.scratchReg, "scratch register is not available");

      TR_ASSERT_FATAL((data.adjustsFramePointerBy == 0), "frame pointer adjustment not supported for CALLReg instructions");

      TR::Instruction *prevInstr;

      /**
       * Ideally, only the RegImm64Sym instruction form would be used.  However, there are
       * subtleties in the handling of relocations between these different forms of instructions
       * that need to be sorted out first..
       */
      if (data.useSymInstruction)
         {
         TR::AMD64RegImm64SymInstruction *loadInstr;

         if (data.prevInstr)
            {
            loadInstr = generateRegImm64SymInstruction(
               data.prevInstr,
               MOV8RegImm64,
               data.scratchReg,
               targetAddress,
               data.globalMethodSymRef,
               data.cg);
            }
         else
            {
            loadInstr = generateRegImm64SymInstruction(
               MOV8RegImm64,
               data.callNode,
               data.scratchReg,
               targetAddress,
               data.globalMethodSymRef,
               data.cg);
            }

         if (data.reloKind != TR_NoRelocation)
            {
            loadInstr->setReloKind(data.reloKind);
            }

         data.out_loadInstr = loadInstr;
         }
      else
         {
         TR::AMD64RegImm64Instruction *loadInstr;

         if (data.prevInstr)
            {
            loadInstr = generateRegImm64Instruction(
               data.prevInstr,
               MOV8RegImm64,
               data.scratchReg,
               targetAddress,
               data.cg,
               data.reloKind);
            }
         else
            {
            loadInstr = generateRegImm64Instruction(
               MOV8RegImm64,
               data.callNode,
               data.scratchReg,
               targetAddress,
               data.cg,
               data.reloKind);
            }

         data.out_loadInstr = loadInstr;
         }

      TR_X86OpCodes op = data.isCall ? CALLReg : JMPReg;
      if (data.prevInstr)
         {
         callInstr = generateRegInstruction(data.out_loadInstr, op, data.scratchReg, data.regDeps, data.cg);
         }
      else
         {
         callInstr = generateRegInstruction(op, data.callNode, data.scratchReg, data.regDeps, data.cg);
         }
      }

   data.out_callInstr = callInstr;

   return callInstr;
   }


uint8_t *
OMR::X86::AMD64::JitCodeObjectFormat::encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data)
   {
   uint8_t op = data.isCall ? 0xe8 : 0xe9;   // CALL rel32 | JMP rel32
   *data.bufferAddress++ = op;

   int32_t disp32 = data.cg->branchDisplacementToHelperOrTrampoline(data.bufferAddress+4, data.globalMethodSymRef);
   *(int32_t *)data.bufferAddress = disp32;

   data.out_encodedMethodAddressLocation = data.bufferAddress;

   data.bufferAddress += 4;

   return data.bufferAddress;
   }

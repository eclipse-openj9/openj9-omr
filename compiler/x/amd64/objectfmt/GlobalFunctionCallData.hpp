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

#ifndef TR_GLOBALFUNCTIONCALLDATA_INCL
#define TR_GLOBALFUNCTIONCALLDATA_INCL

#include "objectfmt/OMRGlobalFunctionCallData.hpp"
#include "infra/Annotations.hpp"
#include "runtime/Runtime.hpp"

namespace TR { class Instruction; }
namespace TR { class SymbolReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class CodeGenerator; }


namespace TR
{

class OMR_EXTENSIBLE GlobalFunctionCallData : public OMR::GlobalFunctionCallDataConnector
   {
public:

   /**
    * APIs for global non-helper calls
    */
   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         TR::Register *parm_scratchReg,
         uintptr_t parm_targetAddress,
         TR::RegisterDependencyConditions *parm_regDeps,
         int32_t parm_reloKind,
         bool parm_useSymInstruction,
         int32_t parm_adjustsFramePointerBy,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(parm_globalMethodSymRef, parm_callNode, parm_scratchReg, parm_targetAddress,
         parm_regDeps, (TR_RuntimeHelper)0, 0, parm_reloKind, parm_adjustsFramePointerBy, parm_useSymInstruction, 0, parm_isCall, parm_cg) {}

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Instruction *parm_prevInstr,
         TR::Register *parm_scratchReg,
         uintptr_t parm_targetAddress,
         TR::RegisterDependencyConditions *parm_regDeps,
         int32_t parm_reloKind,
         bool parm_useSymInstruction,
         int32_t parm_adjustsFramePointerBy,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(parm_globalMethodSymRef, 0, parm_scratchReg, parm_targetAddress,
         parm_regDeps, (TR_RuntimeHelper)0, parm_prevInstr, parm_reloKind, parm_adjustsFramePointerBy, parm_useSymInstruction, 0, parm_isCall, parm_cg) {}

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         TR::Register *parm_scratchReg,
         uintptr_t parm_targetAddress,
         TR::RegisterDependencyConditions *parm_regDeps,
         int32_t parm_reloKind,
         bool parm_useSymInstruction,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(parm_globalMethodSymRef, parm_callNode, parm_scratchReg, parm_targetAddress,
         parm_regDeps, (TR_RuntimeHelper)0, 0, parm_reloKind, 0, parm_useSymInstruction, 0, parm_isCall, parm_cg) {}

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Instruction *parm_prevInstr,
         TR::Register *parm_scratchReg,
         uintptr_t parm_targetAddress,
         TR::RegisterDependencyConditions *parm_regDeps,
         int32_t parm_reloKind,
         bool parm_useSymInstruction,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(parm_globalMethodSymRef, 0, parm_scratchReg, parm_targetAddress,
         parm_regDeps, (TR_RuntimeHelper)0, parm_prevInstr, parm_reloKind, 0, parm_useSymInstruction, 0, parm_isCall, parm_cg) {}

   /**
    * APIs for global helper calls
    */
   GlobalFunctionCallData(
         TR_RuntimeHelper parm_runtimeHelperIndex,
         TR::Node *parm_callNode,
         TR::RegisterDependencyConditions *parm_regDeps,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(0, parm_callNode, 0, 0,
         parm_regDeps, parm_runtimeHelperIndex, 0, TR_NoRelocation, 0, false, 0, parm_isCall, parm_cg) {}

   GlobalFunctionCallData(
         TR_RuntimeHelper parm_runtimeHelperIndex,
         TR::Instruction *parm_prevInstr,
         TR::RegisterDependencyConditions *parm_regDeps,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(0, 0, 0, 0,
         parm_regDeps, parm_runtimeHelperIndex, parm_prevInstr, TR_NoRelocation, 0, false, 0, parm_isCall, parm_cg) {}

   /**
    * APIs for encoded calls from snippets
    */
   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         uint8_t *parm_bufferAddress,
         TR::CodeGenerator *parm_cg,
         bool parm_isCall = true) :
      OMR::GlobalFunctionCallDataConnector(parm_globalMethodSymRef, parm_callNode, 0, 0, 0,
         (TR_RuntimeHelper)0, 0, TR_NoRelocation, 0, false, parm_bufferAddress, parm_isCall, parm_cg) {}

   };

}

#endif

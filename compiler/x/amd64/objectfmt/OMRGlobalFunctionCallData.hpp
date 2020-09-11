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

#ifndef OMR_AMD64_GLOBALFUNCTIONCALLDATA_INCL
#define OMR_AMD64_GLOBALFUNCTIONCALLDATA_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
#define OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
namespace OMR { namespace X86 { namespace AMD64 { class GlobalFunctionCallData; } } }
namespace OMR { typedef OMR::X86::AMD64::GlobalFunctionCallData GlobalFunctionCallDataConnector; }
#else
#error OMR::X86::AMD64::GlobalFunctionCallData expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/x/objectfmt/OMRGlobalFunctionCallData.hpp"
#include "runtime/Runtime.hpp"

namespace TR { class Instruction; }
namespace TR { class SymbolReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class CodeGenerator; }

namespace OMR
{

namespace X86
{

namespace AMD64
{

class OMR_EXTENSIBLE GlobalFunctionCallData : public OMR::X86::GlobalFunctionCallData
   {
public:

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         TR::Register *parm_scratchReg,
         uintptr_t parm_targetAddress,
         TR::RegisterDependencyConditions *parm_regDeps,
         TR_RuntimeHelper parm_runtimeHelperIndex,
         TR::Instruction *parm_prevInstr,
         int32_t parm_reloKind,
         int32_t parm_adjustsFramePointerBy,
         bool parm_useSymInstruction,
         uint8_t *parm_bufferAddress,
         bool parm_isCall,
         TR::CodeGenerator *parm_cg) :
      OMR::X86::GlobalFunctionCallData(parm_globalMethodSymRef, parm_callNode, parm_bufferAddress, parm_cg),
         targetAddress(parm_targetAddress),
         scratchReg(parm_scratchReg),
         regDeps(parm_regDeps),
         prevInstr(parm_prevInstr),
         reloKind(parm_reloKind),
         useSymInstruction(parm_useSymInstruction),
         runtimeHelperIndex(parm_runtimeHelperIndex),
         adjustsFramePointerBy(parm_adjustsFramePointerBy),
         isCall(parm_isCall),
         out_loadInstr(0),
         out_callInstr(0) {}

   /**
    * If a non-zero targetAddress is provided, use that as the address of the function to call
    */
   uintptr_t targetAddress;

   TR::Register *scratchReg;

   TR::RegisterDependencyConditions *regDeps;

   TR::Instruction *prevInstr;

   /**
    * If an intermediate load instruction was used to materialize the function to
    * call then it may be set in this field.
    */
   TR::Instruction *out_loadInstr;

   /**
    * The call instruction that was generated to dispatch to the global function.
    */
   TR::Instruction *out_callInstr;

   /**
    * Should a CALL instruction be used?  If false then a JMP instruction will
    * be emitted instead.
    */
   bool isCall;

   bool useSymInstruction;

   int32_t reloKind;

   int32_t adjustsFramePointerBy;

   TR_RuntimeHelper runtimeHelperIndex;

   };

}

}

}

#endif

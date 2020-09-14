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

#ifndef OMR_Z_GLOBALFUNCTIONCALLDATA_INCL
#define OMR_Z_GLOBALFUNCTIONCALLDATA_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
#define OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
namespace OMR { namespace Z { class GlobalFunctionCallData; } }
namespace OMR { typedef OMR::Z::GlobalFunctionCallData GlobalFunctionCallDataConnector; }
#endif

#include "compiler/objectfmt/OMRGlobalFunctionCallData.hpp"

namespace TR { class SymbolReference; }
namespace TR { class Node; }
namespace TR { class CodeGenerator; }

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE GlobalFunctionCallData : public OMR::GlobalFunctionCallData
   {
public:

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         uint8_t *parm_bufferAddress,
         intptr_t parm_targetAddress,
         TR::Register *parm_returnAddressReg,
         TR::Register *parm_entryPointReg,
         TR::RegisterDependencyConditions *parm_regDeps,
         TR::Instruction *parm_prevInstr,
         TR_RuntimeHelper parm_runtimeHelperIndex,
         TR::Snippet *parm_snippet,
         TR::CodeGenerator *parm_cg) :
      OMR::GlobalFunctionCallData(parm_globalMethodSymRef, parm_callNode, parm_bufferAddress, parm_cg),
         targetAddress(parm_targetAddress),
         returnAddressReg(parm_returnAddressReg),
         entryPointReg(parm_entryPointReg),
         regDeps(parm_regDeps),
         prevInstr(parm_prevInstr),
         runtimeHelperIndex(parm_runtimeHelperIndex),
         snippet(parm_snippet),
         out_loadInstr(NULL),
         out_callInstr(NULL) {}

   /**
    * If a non-zero targetAddress is provided, use that as the address of the function to call
    */
   uintptr_t targetAddress;

   TR::Register *returnAddressReg;

   TR::Register *entryPointReg;

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

   TR_RuntimeHelper runtimeHelperIndex;
   
   TR::Snippet *snippet;
   };

}

}

#endif

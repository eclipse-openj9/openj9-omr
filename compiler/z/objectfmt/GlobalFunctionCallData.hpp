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
   // Used for Helper Call
   GlobalFunctionCallData(
         TR::SymbolReference *parm_methodSymRef,
         TR::Node *parm_callNode,
         TR::Register *parm_returnAddressReg,
         TR::CodeGenerator *parm_cg,
         TR::Register *parm_entryPointReg = NULL,
         TR::RegisterDependencyConditions *parm_regDeps = NULL) :
      OMR::GlobalFunctionCallDataConnector(parm_methodSymRef, parm_callNode, NULL, 0, parm_returnAddressReg, parm_entryPointReg, parm_regDeps, NULL, static_cast<TR_RuntimeHelper>(0), NULL, parm_cg) {}

   // Used For Snippet
   GlobalFunctionCallData(
         TR::SymbolReference *parm_methodSymRef,
         TR::Node *parm_callNode,
         uint8_t *parm_bufferAddress,
         TR::CodeGenerator *parm_cg,
         TR::Snippet *parm_snippet) :
      OMR::GlobalFunctionCallDataConnector(parm_methodSymRef, parm_callNode, parm_bufferAddress, 0, NULL, NULL, NULL, NULL, static_cast<TR_RuntimeHelper>(0), parm_snippet, parm_cg) {}

   };

}

#endif

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

#ifndef OMR_GLOBALFUNCTIONCALLDATA_INCL
#define OMR_GLOBALFUNCTIONCALLDATA_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
#define OMR_GLOBALFUNCTIONCALLDATA_CONNECTOR
namespace OMR { class GlobalFunctionCallData; }
namespace OMR { typedef OMR::GlobalFunctionCallData GlobalFunctionCallDataConnector; }
#endif

#include <stddef.h>
#include <stdint.h>
#include "infra/Annotations.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class SymbolReference; }

namespace OMR
{

class OMR_EXTENSIBLE GlobalFunctionCallData
   {

public:

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         TR::CodeGenerator *parm_cg) :
      globalMethodSymRef(parm_globalMethodSymRef),
      callNode(parm_callNode),
      bufferAddress(0),
      out_encodedMethodAddressLocation(0),
      cg(parm_cg) {}

   GlobalFunctionCallData(
         TR::SymbolReference *parm_globalMethodSymRef,
         TR::Node *parm_callNode,
         uint8_t *parm_bufferAddress,
         TR::CodeGenerator *parm_cg) :
      globalMethodSymRef(parm_globalMethodSymRef),
      callNode(parm_callNode),
      bufferAddress(parm_bufferAddress),
      out_encodedMethodAddressLocation(0),
      cg(parm_cg) {}

   TR::SymbolReference *globalMethodSymRef;

   TR::Node *callNode;

   TR::CodeGenerator *cg;

   /**
    * The address of a buffer where the global function call is to be emitted,
    * or 0 if the buffer address is unknown or not required.
    */
   uint8_t *bufferAddress;

   /**
    * If the address of the global funtion is known and it is encoded somewhere
    * as part of the global function dispatch sequence then this field records
    * the address where that function address is written.
    */
   uint8_t *out_encodedMethodAddressLocation;

   };

}

#endif

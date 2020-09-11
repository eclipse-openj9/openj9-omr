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

#ifndef OMR_OBJECTFORMAT_INCL
#define OMR_OBJECTFORMAT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_OBJECTFORMAT_CONNECTOR
#define OMR_OBJECTFORMAT_CONNECTOR
namespace OMR { class ObjectFormat; }
namespace OMR { typedef OMR::ObjectFormat ObjectFormatConnector; }
#endif

#include <stddef.h>
#include <stdint.h>
#include "env/TRMemory.hpp"
#include "infra/Annotations.hpp"

namespace TR { class GlobalFunctionCallData; }
namespace TR { class Instruction; }

namespace OMR
{

class OMR_EXTENSIBLE ObjectFormat
   {

public:

   TR_ALLOC(TR_Memory::ObjectFormat)

   /**
    * @brief Emit the code as a sequence of TR::Instructions to call a "global" function
    *        (e.g., in a separate shared library) for this object format.
    *
    * @param[in] data : a populated TR::GlobalFunctionCallData structure with valid parameters
    *               for this call site.  Note that this function may alter the contents of
    *               this structure.
    * @return : the final TR::Instruction produced in the instruction stream to emit a global
    *           function call for this object format.
    */
   virtual TR::Instruction *emitGlobalFunctionCall(TR::GlobalFunctionCallData &data) = 0;

   /**
    * @brief Emit the binary code to call a "global" function (e.g., in a separate shared library)
    *        for this object format.
    *
    * @param[in] data : a populated TR::GlobalFunctionCallData structure with valid parameters
    *               for this call site.  Note that this function may alter the contents of
    *               this structure.
    *
    * @return : the buffer address following the necessary instruction(s) to emit a global
    *           fuction call for this object format.
    */
   virtual uint8_t *encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data) = 0;

   /**
    * @brief Return the length in bytes of the binary encoding of a global function call for this
    *        object format.  If the exact length cannot be determined, this should return a
    *        conservatively correct estimate that is at least as large as the actual length.
    *
    * @return : the length in bytes of the encoding of a global function call
    */
   virtual int32_t globalFunctionCallBinaryLength() = 0;

   };

}

#endif

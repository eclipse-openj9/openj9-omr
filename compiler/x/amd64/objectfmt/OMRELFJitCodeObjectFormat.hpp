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

#ifndef OMR_AMD64_ELFJITCODEOBJECTFORMAT_INCL
#define OMR_AMD64_ELFJITCODEOBJECTFORMAT_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_ELFJITCODEOBJECTFORMAT_CONNECTOR
#define OMR_ELFJITCODEOBJECTFORMAT_CONNECTOR
namespace OMR { namespace X86 { namespace AMD64 { class ELFJitCodeObjectFormat; } } }
namespace OMR { typedef OMR::X86::AMD64::ELFJitCodeObjectFormat ELFJitCodeObjectFormatConnector; }
#else
#error OMR::X86::AMD64::ELFJitCodeObjectFormat expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/x/objectfmt/OMRELFJitCodeObjectFormat.hpp"

namespace TR { class Instruction; }
namespace TR { class GlobalFunctionCallData; }


namespace OMR
{

namespace X86
{

namespace AMD64
{

class OMR_EXTENSIBLE ELFJitCodeObjectFormat : public OMR::X86::ELFJitCodeObjectFormat
   {
public:

   virtual TR::Instruction *emitGlobalFunctionCall(TR::GlobalFunctionCallData &data);

   uint8_t *encodeGlobalFunctionCall(TR::GlobalFunctionCallData &data);

   virtual int32_t globalFunctionCallBinaryLength()
      {
      return 6;  // CALL [RIP+disp32]
      }

  struct ccGlobalFunctionData
      {
      uintptr_t address;
      };

   };
}

}

}

#endif

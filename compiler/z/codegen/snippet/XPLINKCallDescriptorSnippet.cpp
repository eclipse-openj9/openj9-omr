/*******************************************************************************
 * Copyright (c) 2019, 2020 IBM Corp. and others
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
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/Relocation.hpp"
#include "codegen/SystemLinkagezOS.hpp"
#include "codegen/snippet/XPLINKCallDescriptorSnippet.hpp"
#include "env/CompilerEnv.hpp"
#include "il/DataTypes.hpp"
#include "il/Node.hpp"
#include "OMR/Bytes.hpp"

TR::XPLINKCallDescriptorSnippet::XPLINKCallDescriptorSnippet(TR::CodeGenerator* cg, TR::S390zOSSystemLinkage* linkage, uint32_t callDescriptorValue)
   :
   TR::S390ConstantDataSnippet(cg, NULL, NULL, SIZE),
   _linkage(linkage),
   _callDescriptorValue(callDescriptorValue)
   {
   *(reinterpret_cast<uint32_t*>(_value) + 0) = 0;
   *(reinterpret_cast<uint32_t*>(_value) + 1) = callDescriptorValue;
   }

uint8_t*
TR::XPLINKCallDescriptorSnippet::emitSnippetBody()
   {
   uint8_t* cursor = cg()->getBinaryBufferCursor();

   // TODO: We should not have to do this here. This should be done by the caller.
   getSnippetLabel()->setCodeLocation(cursor);

   TR_ASSERT_FATAL((reinterpret_cast<uintptr_t>(cursor) % 8) == 0, "XPLINKCallDescriptorSnippet is not aligned on a doubleword bounary");

   // Signed offset, in bytes, to Entry Point Marker (if it exists)
   if (_linkage->getEntryPointMarkerLabel() != NULL)
      {
      *reinterpret_cast<int32_t*>(cursor) = _linkage->getEntryPointMarkerLabel()->getInstruction()->getBinaryEncoding() - cursor;
      *reinterpret_cast<int32_t*>(_value) = *reinterpret_cast<int32_t*>(cursor);
      }
   else
      {
      *reinterpret_cast<int32_t*>(cursor) = 0x00000000;
      }

   cursor += sizeof(int32_t);

   // Linkage, Return Value Adjust, and Parameter Adjust
   *reinterpret_cast<int32_t*>(cursor) = _callDescriptorValue;
   cursor += sizeof(int32_t);

   return cursor;
   }

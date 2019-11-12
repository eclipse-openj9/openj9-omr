/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
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

#ifndef OMR_Z_SNIPPET_XPLINKCALLDESCRIPTORSNIPPET_INCL
#define OMR_Z_SNIPPET_XPLINKCALLDESCRIPTORSNIPPET_INCL

#include <stdint.h>
#include "codegen/ConstantDataSnippet.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class S390zOSSystemLinkage; }

namespace TR {

/** \brief
 *
 *  Represents the XPLINK Call Descriptor snippet which is created only on 31-bit targets when:
 *
 *  1. The call site is so far removed from the Entry Point Marker of the function that its offset cannot be contained
 *     in the space available in the call NOP descriptor following the call site.
 *
 *  2. The call contains a return value or parameters that are passed in registers or in ways incompatible with non-
 *     XPLINK code.
 *
 *  \details
 *
 *  The Call Descriptor snippet has the following format:
 *
 *  \verbatim
 *                                         0x01                               0x02                               0x03
 *  0x00 +----------------------------------+----------------------------------+----------------------------------+----------------------------------+
 *       | Signed offset, in bytes, to Entry Point Marker                                                                                            |
 *  0x04 +----------------------------------+----------------------------------+----------------------------------+----------------------------------+
 *       | Linkage and Return Value Adjust  | Parameter Adjust                                                                                       |
 *       +----------------------------------+----------------------------------+----------------------------------+----------------------------------+
 *  \endverbatim
 */
class XPLINKCallDescriptorSnippet : public TR::S390ConstantDataSnippet
   {
   public:

   /** \brief
    *     Size (in bytes) of the call descriptor data structure.
    */
   static const size_t SIZE = 8;

   static uint32_t generateCallDescriptorValue(TR::S390zOSSystemLinkage* linkage, TR::Node* callNode);

   public:

   XPLINKCallDescriptorSnippet(TR::CodeGenerator* cg, TR::S390zOSSystemLinkage* linkage, uint32_t callDescriptorValue);

   virtual uint8_t* emitSnippetBody();

   private:

   TR::S390zOSSystemLinkage* _linkage;
   uint32_t _callDescriptorValue;
   };
}

#endif

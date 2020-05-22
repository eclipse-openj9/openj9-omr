/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#ifndef OMR_Power_PEEPHOLE_INCL
#define OMR_Power_PEEPHOLE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_PEEPHOLE_CONNECTOR
#define OMR_PEEPHOLE_CONNECTOR
namespace OMR { namespace Power { class Peephole; } }
namespace OMR { typedef OMR::Power::Peephole PeepholeConnector; }
#else
#error OMR::Power::Peephole expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/codegen/OMRPeephole.hpp"

namespace TR { class Compilation; }
namespace TR { class Instruction; }

namespace OMR
{

namespace Power
{

class OMR_EXTENSIBLE Peephole : public OMR::Peephole
   {
   public:

   Peephole(TR::Compilation* comp);

   virtual bool performOnInstruction(TR::Instruction* cursor);

   private:

   /** \brief
    *     Tries to remove redundant loads after stores which have the same source and target. For example:
    *
    *     <code>
    *     std r10,8(r12)
    *     ld r10,8(r12)
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     std r10,8(r12)
    *     </code>
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool tryToRemoveRedundantLoadAfterStore();

   /** \brief
    *     Tries to remove redundant synchronization instructions.
    *
    *  \param window
    *     The size of the peephole window to look through before giving up.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool tryToRemoveRedundantSync(int32_t window);

   /** \brief
    *     Tries to remove redundant consecutive instruction which write to the same target register.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool tryToRemoveRedundantWriteAfterWrite();

   private:

   /// The instruction cursor currently being processed by the peephole optimization
   TR::Instruction* cursor;
   };

}

}

#endif

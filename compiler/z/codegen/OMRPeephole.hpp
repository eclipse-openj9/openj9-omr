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

#ifndef OMR_Z_PEEPHOLE_INCL
#define OMR_Z_PEEPHOLE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_PEEPHOLE_CONNECTOR
#define OMR_PEEPHOLE_CONNECTOR
namespace OMR { namespace Z { class Peephole; } }
namespace OMR { typedef OMR::Z::Peephole PeepholeConnector; }
#else
#error OMR::Z::Peephole expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/codegen/OMRPeephole.hpp"

#include <stdint.h>
#include "codegen/InstOpCode.hpp"
#include "env/jittypes.h"
#include "infra/Flags.hpp"

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE Peephole : public OMR::Peephole
   {
   public:

   Peephole(TR::Compilation* comp);

   virtual bool performOnInstruction(TR::Instruction* cursor);
   };

}

}

#endif

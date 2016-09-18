/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#include "control/Recompilation.hpp"

#include <limits.h>                          // for INT_MAX
#include <stdint.h>                          // for uint16_t, uint32_t, etc
#include <stdlib.h>                          // for NULL, strtoul
#include <string.h>                          // for memset
#include "codegen/CodeGenerator.hpp"         // for CodeGenerator
#include "codegen/FrontEnd.hpp"              // for TR_FrontEnd, feGetEnv, etc
#include "compile/Compilation.hpp"           // for Compilation
#include "compile/CompilationTypes.hpp"      // for TR_Hotness
#include "compile/ResolvedMethod.hpp"        // for TR_ResolvedMethod
#include "compile/SymbolReferenceTable.hpp"  // for SymbolReferenceTable
#include "control/OptimizationPlan.hpp"      // for TR_OptimizationPlan, etc
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"       // for TR::Options, etc
#include "env/PersistentInfo.hpp"            // for PersistentInfo
#include "env/TRMemory.hpp"                  // for TR_Link::operator new, etc
#include "env/jittypes.h"                    // for uintptrj_t
#include "il/DataTypes.hpp"                  // for etc
#include "infra/Assert.hpp"                  // for TR_ASSERT
#include "infra/Link.hpp"                    // for TR_LinkHead
#include "infra/Timer.hpp"                   // for TR_SingleTimer

class TR_OpaqueMethodBlock;
namespace TR { class Instruction; }
namespace TR { class SymbolReference; }


OMR::Recompilation::Recompilation(TR::Compilation * comp) :
   _compilation(comp)
   {
   }


void
OMR::Recompilation::shutdown()
   {
   }


TR::Recompilation *
OMR::Recompilation::self()
   {
   return static_cast<TR::Recompilation *>(this);
   }

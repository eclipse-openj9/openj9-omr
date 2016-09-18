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
 *******************************************************************************/

#ifndef OMR_Z_CODEGEN_PHASE
#define OMR_Z_CODEGEN_PHASE

/*
 * The following #define and typedef must appear before any #includes in this file
 */

#ifndef OMR_CODEGEN_PHASE_CONNECTOR
#define OMR_CODEGEN_PHASE_CONNECTOR
namespace OMR { namespace Z { class CodeGenPhase; } }
namespace OMR { typedef OMR::Z::CodeGenPhase CodeGenPhaseConnector; }
#else
#error OMR::Z::CodeGenPhase expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRCodeGenPhase.hpp"

namespace OMR
{

namespace Z
{

class CodeGenPhase : public OMR::CodeGenPhase
   {
   protected:

   CodeGenPhase(TR::CodeGenerator *cg): OMR::CodeGenPhase(cg) {}

   public:
   static void performMarkLoadAsZeroOrSignExtensionPhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);
   static void performSetBranchOnCountFlagPhase(TR::CodeGenerator * cg, TR::CodeGenPhase *);

   // override base class implementation because new phases are being added
   static int getNumPhases();
   const char * getName();
   static const char* getName(PhaseValue phase);

   };
}

}

#endif

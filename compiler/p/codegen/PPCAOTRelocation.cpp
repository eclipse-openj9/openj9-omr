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

#include "p/codegen/PPCAOTRelocation.hpp"

#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for int32_t
#include "codegen/CodeGenerator.hpp"        // for CodeGenerator
#include "codegen/Instruction.hpp"          // for Instruction
#include "codegen/Relocation.hpp"
#include "compile/Compilation.hpp"          // for Compilation
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/CompilerEnv.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"                   // for intptrj_t
#include "il/symbol/LabelSymbol.hpp"        // for LabelSymbol
#include "runtime/Runtime.hpp"              // for LO_VALUE

void TR_PPCPairedRelocation::mapRelocation(TR::CodeGenerator *cg)
   {
   if (cg->comp()->getOption(TR_AOT))
      {
      cg->addAOTRelocation(
         new (cg->trHeapMemory()) TR_32BitExternalOrderedPairRelocation(
            getSourceInstruction()->getBinaryEncoding(),
            getSource2Instruction()->getBinaryEncoding(),
            getRelocationTarget(),
            getKind(), cg),
         __FILE__,
         __LINE__,
         getNode());
      }
   }


void TR_PPCPairedLabelAbsoluteRelocation::apply(TR::CodeGenerator *cg)
   {
   intptrj_t p = (intptrj_t)getLabel()->getCodeLocation();

   if (TR::Compiler->target.is32Bit())
      {
      _instr1->updateImmediateField(cg->hiValue(p) & 0x0000ffff);
      _instr2->updateImmediateField(LO_VALUE(p) & 0x0000ffff);
      }
   else
      {
      if (_instr4->getMemoryReference() != NULL)
	 {
         // We should add an updateImmediateField method to Mem instruction classes instead
         int32_t *cursor = (int32_t *)_instr4->getBinaryEncoding();
         *cursor |= LO_VALUE(p) & 0x0000ffff;
	 }
      else
         _instr4->updateImmediateField(LO_VALUE(p) & 0x0000ffff);
      p = cg->hiValue(p);
      _instr1->updateImmediateField((p>>32) & 0x0000ffff);
      _instr2->updateImmediateField((p>>16) & 0x0000ffff);
      _instr3->updateImmediateField(p & 0x0000ffff);
      }
   }

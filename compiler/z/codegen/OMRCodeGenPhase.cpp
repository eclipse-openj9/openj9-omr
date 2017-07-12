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

//On zOS XLC linker can't handle files with same name at link time
//This workaround with pragma is needed. What this does is essentially
//give a different name to the codesection (csect) for this file. So it
//doesn't conflict with another file with same name.

#pragma csect(CODE,"OMRZCGPhase#C")
#pragma csect(STATIC,"OMRZCGPhase#S")
#pragma csect(TEST,"OMRZCGPhase#T")


#include "codegen/CodeGenPhase.hpp"

#include "infra/Assert.hpp"                           // for TR_ASSERT
#include "codegen/CodeGenerator.hpp"                  // for CodeGenerator, etc
#include "compile/Compilation.hpp"                    // for Compilation, etc
#include "optimizer/LoadExtensions.hpp"
#include "optimizer/OptimizationManager.hpp"
#include "il/TreeTop.hpp"

void
OMR::Z::CodeGenPhase::performMarkLoadAsZeroOrSignExtensionPhase(TR::CodeGenerator * cg, TR::CodeGenPhase * phase)
   {
   if (TR::Compiler->target.cpu.isZ() && cg->getOptimizationPhaseIsComplete())
      {
      TR::Compilation* comp = cg->comp();
      TR::OptimizationManager *manager = comp->getOptimizer()->getOptimization(OMR::loadExtensions);
      TR_ASSERT(manager, "Load extensions optimization should be initialized.");
      TR_LoadExtensions *loadExtensions = (TR_LoadExtensions *) manager->factory()(manager);
      cg->setSignExtensionFlags( loadExtensions->getFlags() );
      loadExtensions->perform();
      delete loadExtensions;
      }
   }

void
OMR::Z::CodeGenPhase::performSetBranchOnCountFlagPhase(TR::CodeGenerator * cg, TR::CodeGenPhase *)
   {
   if (!cg->comp()->getOption(TR_DisableBranchOnCount))
      {
      TR::TreeTop * tt;
      vcount_t visitCount = cg->comp()->incVisitCount();

      for (tt = cg->comp()->getStartTree(); tt; tt = tt->getNextTreeTop())
         {
         cg->setBranchOnCountFlag(tt->getNode(), visitCount);
         }
      }
   }

int
OMR::Z::CodeGenPhase::getNumPhases()
   {
   return static_cast<int>(TR::CodeGenPhase::LastOMRZPhase);
   }

const char *
OMR::Z::CodeGenPhase::getName()
   {
   return TR::CodeGenPhase::getName(_currentPhase);
   }

const char *
OMR::Z::CodeGenPhase::getName(PhaseValue phase)
   {
   switch (phase)
      {
      case markLoadAsZeroOrSignExtension:
         return "markLoadAsZeroOrSignExtension";
      case SetBranchOnCountFlagPhase:
         return "SetBranchOnCountFlagPhase";
      default:
         // call parent class for common phases
         return OMR::CodeGenPhase::getName(phase);
      }
   }

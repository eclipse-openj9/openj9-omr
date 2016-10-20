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

#include "control/CompilationController.hpp"

#include <stdint.h>                           // for int32_t
#include <stdio.h>                            // for fprintf, NULL, stderr
#include <stdlib.h>                           // for atoi
#include "codegen/FrontEnd.hpp"               // for feGetEnv
#include "compile/Compilation.hpp"            // for Compilation
#include "compile/CompilationTypes.hpp"       // for TR_Hotness
#include "control/OptimizationPlan.hpp"
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"        // for TR::Options
#include "control/Recompilation.hpp"          // for TR_Recompilation
#include "env/TRMemory.hpp"                   // for PERSISTENT_NEW, etc
#include "infra/Monitor.hpp"                  // for Monitor
#include "infra/ThreadLocal.h"                // for tlsAlloc, tlsFree

namespace TR { class CompilationInfo; }

int32_t                 TR::CompilationController::_verbose = 0;
TR::CompilationStrategy *TR::CompilationController::_compilationStrategy = NULL;
TR::CompilationInfo *    TR::CompilationController::_compInfo = 0;
bool                    TR::CompilationController::_useController = false;


bool TR::CompilationController::init(TR::CompilationInfo *compInfo)
   {
   TR::Options *options = TR::Options::getCmdLineOptions();
   char *strategyName = options->getCompilationStrategyName();

      {
      _compInfo = compInfo;
      _compilationStrategy = new (PERSISTENT_NEW) TR::DefaultCompilationStrategy();

         {
         TR_OptimizationPlan::_optimizationPlanMonitor = TR::Monitor::create("OptimizationPlanMonitor");
         _useController = (TR_OptimizationPlan::_optimizationPlanMonitor != 0);
         if (_useController)
            {
            static char *verboseController = feGetEnv("TR_VerboseController");
            if (verboseController)
               setVerbose(atoi(verboseController));
            if (verbose() >= LEVEL1)
               fprintf(stderr, "Using %s comp strategy\n", strategyName);
            }
         }
      }

   tlsAlloc(OMR::compilation);

   return _useController;
   }

void TR::CompilationController::shutdown()
   {
   tlsFree(OMR::compilation);
   if (!_useController)
      return;

   // would like to free all entries in the pool of compilation plans
   int32_t remainingPlans = TR_OptimizationPlan::freeEntirePool();

   // print some stats
   if (verbose() >= LEVEL1)
      {
      fprintf(stderr, "Remaining optimizations plans in the system: %d\n", remainingPlans);
      }
   _compilationStrategy->shutdown();
   }

TR_OptimizationPlan *
TR::DefaultCompilationStrategy::processEvent(TR_MethodEvent *event, bool *newPlanCreated)
   {
   TR_OptimizationPlan *plan = NULL;
   return plan;
   }

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

#ifndef JITBUILDER_OPTIMIZER_INCL
#define JITBUILDER_OPTIMIZER_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef JITBUILDER_OPTIMIZER_CONNECTOR
#define JITBUILDER_OPTIMIZER_CONNECTOR
namespace JitBuilder { class Optimizer; }
namespace JitBuilder { typedef JitBuilder::Optimizer OptimizerConnector; }
#endif

#include "optimizer/OMROptimizer.hpp"

#include <stddef.h>                    // for NULL
#include <stdint.h>                    // for uint16_t

namespace TR { class Compilation; }
namespace TR { class Optimizer; }
namespace TR { class ResolvedMethodSymbol; }
struct OptimizationStrategy;

namespace JitBuilder
{

class Optimizer : public OMR::OptimizerConnector
   {
   public:

   Optimizer(TR::Compilation *comp, TR::ResolvedMethodSymbol *methodSymbol, bool isIlGen,
         const OptimizationStrategy *strategy = NULL, uint16_t VNType = 0);

   static const OptimizationStrategy *optimizationStrategy( TR::Compilation *c);

   private:
   TR::Optimizer *self();
   };

}

#endif

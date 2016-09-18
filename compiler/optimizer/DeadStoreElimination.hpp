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

#ifndef DEADSTOR_INCL
#define DEADSTOR_INCL

#include "optimizer/OptimizationManager.hpp"
#include "optimizer/IsolatedStoreElimination.hpp"

namespace TR { class Optimization; }

// Dead Store Elimination
//
// Uses the information computed by ReachingDefinitions (stored in
// UseDefInfo) to perform the transformation. If a store (def) node has no
// uses then remove the node from the IL trees. Usually useful
// after doing copy propagation.
//
// This class is derived from Isolated Store Elimination, which uses the use/def
// information if it is available and uses a tree walk if it isn't. When entered
// through this class there will always be use/def information unless it can't be
// built for size reasons.
//


class TR_DeadStoreElimination : public TR_IsolatedStoreElimination
   {
   public:

   // Performs dead store elimination using the
   // use/def values of relevant nodes.
   //
   TR_DeadStoreElimination(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_DeadStoreElimination(manager);
      }
   };

#endif

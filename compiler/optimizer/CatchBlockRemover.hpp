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

#ifndef CATCHBLOCKREMOVER_INCL
#define CATCHBLOCKREMOVER_INCL

#include <stdint.h>                           // for int32_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager


/*
 * Class TR_CatchBlockRemover
 * ==========================
 *
 * Catch block removal is a simple optimization that aims to do two things
 *
 * 1) Eliminate catch blocks that have no incoming exception edges, i.e. the 
 * optimizer has proven that no exception thrown from any of its exception 
 * predecessors (usually blocks generated for the try region) can reach a 
 * particular catch block.
 * 
 * 2) Eliminate exception edges that cannot be taken, e.g. if a particular 
 * block had all its exception points removed by the optimizer then it can 
 * no longer throw anything; so all the exception edges going out from the 
 * block can be deleted. It also eliminates an exception edge if the possible 
 * exceptions are still not ones that can be caught by a particular catch block.
 * 
 * These transformations simplify the flow of control in general and make it 
 * simpler for later analyses to work.
 */

class TR_CatchBlockRemover : public TR::Optimization
   {
   public:
   TR_CatchBlockRemover(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_CatchBlockRemover(manager);
      }

   virtual int32_t perform();

   private :

   };

#endif

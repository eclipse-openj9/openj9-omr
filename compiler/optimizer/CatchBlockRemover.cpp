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

#include "optimizer/CatchBlockRemover.hpp"

#include <stddef.h>                              // for NULL
#include <stdint.h>                              // for int32_t, uint32_t
#include "env/StackMemoryRegion.hpp"
#include "compile/Compilation.hpp"               // for Compilation
#include "env/TRMemory.hpp"                      // for TR_Memory
#include "il/Block.hpp"                          // for Block, toBlock, etc
#include "il/ILOpCodes.hpp"                      // for ILOpCodes::monent, etc
#include "il/Node.hpp"                           // for Node, vcount_t
#include "il/TreeTop.hpp"                        // for TreeTop
#include "il/TreeTop_inlines.hpp"                // for TreeTop::getNode, etc
#include "infra/Cfg.hpp"                         // for CFG
#include "infra/List.hpp"                        // for List, ListIterator, etc
#include "infra/TRCfgEdge.hpp"                   // for CFGEdge
#include "infra/TRCfgNode.hpp"                   // for CFGNode
#include "optimizer/Optimization.hpp"            // for Optimization
#include "optimizer/Optimizations.hpp"
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/Optimizer.hpp"               // for Optimizer


/**
 * Class CatchBlockRemover
 * =======================
 * Catch block remover is a simple optimization that aims to do two things:
 *
 * 1) Eliminate catch blocks that have no incoming exception edges, i.e. the
 * optimizer has proven that no exception thrown from any of it's exception
 * predecessors (usually blocks generated for the try region) can reach a
 * particular catch block;
 * 2) Eliminate exception edges that cannot be taken, e.g. if a particular
 * block had all it's exception points removed by the optimizer then it can
 * no longer throw anything; so all the exception edges going out from the
 * block can be deleted. It also eliminates an exception edge if the possible
 * exceptions are still not ones that can be caught by a particular catch block.
 *
 * These transformations simplify the flow of control in general and make it
 * simpler for later analyses to work.
 */


TR_CatchBlockRemover::TR_CatchBlockRemover(TR::OptimizationManager *manager)
   : TR::Optimization(manager)
   {}

int32_t TR_CatchBlockRemover::perform()
   {
   TR::CFG *cfg = comp()->getFlowGraph();
   if (cfg == NULL)
      {
      if (trace())
         traceMsg(comp(), "Can't do Catch Block Removal, no CFG\n");
      return 0;
      }

   if (trace())
      traceMsg(comp(), "Starting Catch Block Removal\n");

   bool thereMayBeRemovableCatchBlocks = false;

   {
   TR::StackMemoryRegion stackMemoryRegion(*trMemory());

   TR::Block *block;
   ListIterator<TR::CFGEdge> edgeIterator;

   // Go through all blocks that have exception successors and see if any of them
   // are not reached. Mark each of these edges with a visit count so they can
   // be identified later.
   //
   vcount_t visitCount = comp()->incOrResetVisitCount();

   TR::CFGNode *cfgNode;
   for (cfgNode = cfg->getFirstNode(); cfgNode; cfgNode = cfgNode->getNext())
      {
      if (cfgNode->getExceptionSuccessors().empty())
         continue;

      block = toBlock(cfgNode);
      uint32_t reachedExceptions = 0;
      TR::TreeTop *treeTop;
      for (treeTop = block->getEntry(); treeTop != block->getExit(); treeTop = treeTop->getNextTreeTop())
         {
         reachedExceptions |= treeTop->getNode()->exceptionsRaised();

         if (treeTop->getNode()->getOpCodeValue() == TR::monexitfence) // for live monitor metadata
            reachedExceptions |= TR::Block::CanCatchMonitorExit;
         }

      if (reachedExceptions & TR::Block::CanCatchUserThrows)
         continue;

      for (auto edge = block->getExceptionSuccessors().begin(); edge != block->getExceptionSuccessors().end();)
         {
         TR::CFGEdge * current = *(edge++);
         TR::Block *catchBlock = toBlock(current->getTo());
         if (catchBlock->isOSRCodeBlock() || catchBlock->isOSRCatchBlock()) continue;
         if (!reachedExceptions &&
             performTransformation(comp(), "%sRemove redundant exception edge from block_%d at [%p] to catch block_%d at [%p]\n", optDetailString(), block->getNumber(), block, catchBlock->getNumber(), catchBlock))
            {
            cfg->removeEdge(block, catchBlock);
            thereMayBeRemovableCatchBlocks = true;
            }
         else
            {
            if (!catchBlock->canCatchExceptions(reachedExceptions))
               {
               current->setVisitCount(visitCount);
               thereMayBeRemovableCatchBlocks = true;
               }
            }
         }
      }

   bool edgesRemoved = false;

   // Now look to see if there are any catch blocks for which all exception
   // predecessors have the visit count set. If so, the block is unreachable and
   // can be removed.
   // If only some of the exception predecessors are marked, these edges are
   // left in place to identify the try/catch structure properly.
   //
   while (thereMayBeRemovableCatchBlocks)
      {
      thereMayBeRemovableCatchBlocks = false;
      for (cfgNode = cfg->getFirstNode(); cfgNode; cfgNode = cfgNode->getNext())
         {
         if (cfgNode->getExceptionPredecessors().empty())
            continue;
         auto edgeIt = cfgNode->getExceptionPredecessors().begin();
         for (; edgeIt != cfgNode->getExceptionPredecessors().end(); ++edgeIt)
            {
            if ((*edgeIt)->getVisitCount() != visitCount)
               break;
            }

         if (edgeIt == cfgNode->getExceptionPredecessors().end() && performTransformation(comp(), "%sRemove redundant catch block_%d at [%p]\n", optDetailString(), cfgNode->getNumber(), cfgNode))
            {
            while (!cfgNode->getExceptionPredecessors().empty())
               {
               cfg->removeEdge(cfgNode->getExceptionPredecessors().front());
               }
            edgesRemoved = true;
            thereMayBeRemovableCatchBlocks = true;
            }
         }
      }


   // Any transformations invalidate use/def and value number information
   //
   if (edgesRemoved)
      {
      optimizer()->setUseDefInfo(NULL);
      optimizer()->setValueNumberInfo(NULL);
      requestOpt(OMR::treeSimplification, true);
      }

   } // scope of the stack memory region

   if (trace())
      traceMsg(comp(), "\nEnding Catch Block Removal\n");

   return 1; // actual cost
   }

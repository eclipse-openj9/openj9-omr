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

#include "optimizer/AsyncCheckInsertion.hpp"

#include <limits.h>                            // for INT_MAX
#include <stdlib.h>                            // for atoi
#include <string.h>                            // for strncmp, memset, NULL

#include "compile/Compilation.hpp"             // for Compilation
#include "compile/ResolvedMethod.hpp"          // for TR_ResolvedMethod
#include "compile/SymbolReferenceTable.hpp"    // for SymbolReferenceTable
#include "env/StackMemoryRegion.hpp"
#include "env/jittypes.h"                      // for TR_ByteCodeInfo, etc
#include "il/Block.hpp"                        // for Block, toBlock
#include "il/DataTypes.hpp"                    // for getMaxSigned, etc
#include "il/ILOps.hpp"                        // for ILOpCode, TR::ILOpCode
#include "il/Node.hpp"                         // for Node
#include "il/Node_inlines.hpp"                 // for Node::getFirstChild, etc
#include "il/Symbol.hpp"                       // for Symbol
#include "il/SymbolReference.hpp"              // for SymbolReference
#include "il/TreeTop.hpp"                      // for TreeTop
#include "il/TreeTop_inlines.hpp"              // for TreeTop::getNode, etc
#include "infra/Assert.hpp"                    // for TR_ASSERT
#include "infra/TRCfgEdge.hpp"                 // for CFGEdge
#include "infra/TRCfgNode.hpp"                 // for CFGNode
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/Optimizer.hpp"             // for Optimizer


#define NUMBER_OF_NODES_IN_LARGE_METHOD 2000



// Add an async check into a block - MUST be at block entry
//
void TR_AsyncCheckInsertion::insertAsyncCheck(TR::Block *block, TR::Compilation *comp)
   {
   TR::TreeTop *lastTree = block->getLastRealTreeTop();
   TR::TreeTop *asyncTree =
      TR::TreeTop::create(comp,
         TR::Node::createWithSymRef(lastTree->getNode(), TR::asynccheck, 0,
            comp->getSymRefTab()->findOrCreateAsyncCheckSymbolRef(comp->getMethodSymbol())));


   if (lastTree->getNode()->getOpCode().isReturn())
      {
      TR::TreeTop *prevTree = lastTree->getPrevTreeTop();
      prevTree->join(asyncTree);
      asyncTree->join(lastTree);
      }
   else
      {
      TR::TreeTop *nextTree = block->getEntry()->getNextTreeTop();
      block->getEntry()->join(asyncTree);
      asyncTree->join(nextTree);
      }
   }

int32_t TR_AsyncCheckInsertion::insertReturnAsyncChecks(TR::Compilation *comp)
   {
   int numAsyncChecksInserted = 0;
   for (TR::TreeTop *treeTop = comp->getStartTree();
        treeTop;
        /* nothing */ )
      {
      TR::Block *block = treeTop->getNode()->getBlock();
      if (block->getLastRealTreeTop()->getNode()->getOpCode().isReturn())
         {
         insertAsyncCheck(block, comp);
         numAsyncChecksInserted++;
         }

      treeTop = block->getExit()->getNextRealTreeTop();
      }
   return numAsyncChecksInserted;
   }

TR_AsyncCheckInsertion::TR_AsyncCheckInsertion(TR::OptimizationManager *manager) : TR::Optimization(manager)
   {
   }

bool TR_AsyncCheckInsertion::shouldPerform()
   {
   // Don't run when profiling
   //
   if (comp()->isProfilingCompilation() || comp()->generateArraylets())
      return false;

   if (comp()->getOption(TR_EnableOSR))  // cannot move async checks around arbitrarily if OSR is possible
      return false;

   return true;
   }

int32_t TR_AsyncCheckInsertion::perform()
   {
   TR::StackMemoryRegion stackMemoryRegion(*trMemory());

   // If this is a large acyclic method - add a yield point at each return from this method
   // so that sampling will realize that we are actually in this method.
   //
   if (!comp()->mayHaveLoops())
      {
      static const char *p;
      static uint32_t numNodesInLargeMethod     = (p = feGetEnv("TR_LargeMethodNodes"))     ? atoi(p) : NUMBER_OF_NODES_IN_LARGE_METHOD;

      int32_t numAsyncChecksInserted = 0;
      if ((uint32_t) comp()->getNodeCount() > numNodesInLargeMethod)
         numAsyncChecksInserted = insertReturnAsyncChecks(comp());
      if (trace())
         traceMsg(comp(), "Inserted %d async checks\n", numAsyncChecksInserted);
      return 1;
      }
   return 0;
   }

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

#include "VirtualGuardHeadMerger.hpp"

#include <stddef.h>                            // for NULL
#include "codegen/FrontEnd.hpp"                // for feGetEnv
#include "compile/Compilation.hpp"             // for Compilation
#include "env/jittypes.h"                      // for TR_ByteCodeInfo, etc
#include "il/Block.hpp"                        // for Block
#include "il/ILOpCodes.hpp"                    // for ILOpCodes::Goto, etc
#include "il/ILOps.hpp"                        // for ILOpCode
#include "il/Node.hpp"                         // for Node
#include "il/Node_inlines.hpp"
#include "il/TreeTop.hpp"                      // for TreeTop
#include "il/TreeTop_inlines.hpp"              // for TreeTop::getNode, etc
#include "il/symbol/ResolvedMethodSymbol.hpp"  // for ResolvedMethodSymbol
#include "infra/Cfg.hpp"                       // for CFG
#include "infra/List.hpp"                      // for List, ListIterator, etc
#include "infra/TRCfgEdge.hpp"                 // for CFGEdge
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/Optimizer.hpp"             // for Optimizer
#include "optimizer/TransformUtil.hpp"         // for TransformUtil
#include "ras/DebugCounter.hpp"

#define OPT_DETAILS "O^O VG HEAD MERGE: "

static bool isMergeableGuard(TR::Node *node)
   {
   static char *mergeOnlyHCRGuards = feGetEnv("TR_MergeOnlyHCRGuards");
   return mergeOnlyHCRGuards ? node->isHCRGuard() : node->isNopableInlineGuard();
   }

static bool safeToMoveGuard(TR::Block *destination, TR::TreeTop *guardCandidate,
   TR::TreeTop *branchDest)
   {
   static char *disablePrivArgMovement = feGetEnv("TR_DisableRuntimeGuardPrivArgMovement");
   TR::TreeTop *start = destination ? destination->getExit() : TR::comp()->getStartTree();
   bool stopTheWorld = guardCandidate->getNode()->isHCRGuard();
   if (stopTheWorld)
      {
      for (TR::TreeTop *tt = start; tt && tt != guardCandidate; tt = tt->getNextTreeTop())
         {
         if (tt->getNode()->canGCandReturn())
            return false;
         }
      }
   else
      {
      for (TR::TreeTop *tt = start; tt && tt != guardCandidate; tt = tt->getNextTreeTop())
         {
          if (tt->getNode()->getOpCodeValue() != TR::BBStart
             && tt->getNode()->getOpCodeValue() != TR::BBEnd
             && !tt->getNode()->chkIsPrivatizedInlinerArg()
             && !tt->getNode()->isNopableInlineGuard())
                return false;

         if (tt->getNode()->chkIsPrivatizedInlinerArg() && disablePrivArgMovement)
            return false;

         if (tt->getNode()->isNopableInlineGuard()
             && tt->getNode()->getBranchDestination() != branchDest)
            return false;
         }
      }
   return true;
   }

static void moveBlockAfterDest(TR::CFG *cfg, TR::Block *toMove, TR::Block *dest)
   {
   TR::Compilation *comp = TR::comp();
   // Step1 splice out toMove
   TR::Block *toMovePrev = toMove->getPrevBlock();
   TR::Block *toMoveSucc = toMove->getNextBlock();

   toMovePrev->getExit()->join(toMoveSucc->getEntry());

   // Step 2 splice toMove in after dest
   TR::Block *destNext = dest->getNextBlock();
   dest->getExit()->join(toMove->getEntry());
   toMove->getExit()->join(destNext->getEntry());

   cfg->addEdge(toMove, destNext);
   cfg->addEdge(dest, toMove);
   cfg->removeEdge(dest, destNext);

   cfg->addEdge(toMovePrev, toMoveSucc);
   cfg->removeEdge(toMovePrev, toMove);
   cfg->removeEdge(toMove, toMoveSucc);
   }

// This opt tries to reduce merge backs from cold code that are the result of inliner
// gnerated nopable virtual guards
// It looks for one basic pattern
//
// guard1 -> cold1
// BBEND
// BBSTART
// guard2 -> cold2
// if guard1 is the guard for a method which calls the method guard2 protects or cold1 is
// a predecessor of cold2 (a situation commonly greated by virtual guard tail splitter) we
// can transform the guards as follows when guard1 and guard2 a
// guard1 -> cold1
// BBEND
// BBSTART
// guard2 -> cold1
// This is safe because there are no trees between the guards and calling the caller will
// result in the call to the callee if we need to patch guard2. cold2 and its mergebacks
// can then be eliminated
//
// In addition this opt will try to move guard2 up from the end of a block to the
// start of the block. We can do this if guard2 is an HCR guard and there is no GC point
// between BBSTART and guard2 since HCR is a stop-the-world event.
//
// Finally, there is a simple tail splitting step run before the analysis of a guard if we
// detect that the taken side of the guard merges back in the next block - this happens
// for some empty methods and is common for Object.<init> at the top of constructors.
int32_t TR_VirtualGuardHeadMerger::perform() {
   static char *disableVGHeadMergerTailSplitting = feGetEnv("TR_DisableVGHeadMergerTailSplitting");
   TR::CFG *cfg = comp()->getFlowGraph();
   for (TR::Block *block = optimizer()->getMethodSymbol()->getFirstTreeTop()->getNode()->getBlock();
        block; block = block->getNextBlock())
      {
      TR::Node *guard1 = block->getLastRealTreeTop()->getNode();

      if (isMergeableGuard(guard1))
         {
         if (trace())
            traceMsg(comp(), "Found mergeable guard in block_%d\n", block->getNumber());
         TR::Block *cold1 = guard1->getBranchDestination()->getEnclosingBlock();

         // check for an immediate merge back from the cold block and
         // tail split one block if we can - we only handle splitting a block
         // ending in a fallthrough, a branch or a goto for now for simplicity
         if (!disableVGHeadMergerTailSplitting &&
             (cold1->getSuccessors().size() == 1) &&
             cold1->hasSuccessor(block->getNextBlock()) &&
             cold1->getLastRealTreeTop()->getNode()->getOpCode().isGoto())
            {
            // TODO handle moving code earlier in the block down below the guard
            // tail split
            if ((block->getNextBlock()->getSuccessors().size() == 1) ||
                ((block->getNextBlock()->getSuccessors().size() == 2) &&
                 block->getNextBlock()->getLastRealTreeTop()->getNode()->getOpCode().isBranch()) &&
                performTransformation(comp(), "%sCloning block_%d and placing clone after block_%d to reduce HCR guard nops\n", OPT_DETAILS, block->getNextBlock()->getNumber(), cold1->getNumber()))
               tailSplitBlock(block, cold1);
            }

         // guard motion is fairly complex but what we want to achieve around guard1 is a sequence
         // of relocated privarg blocks, followed by a sequence of runtime patchable guards going to
         // guard1's cold block, followed by a sequence of stop-the-world guards going to guard1's
         // cold block
         //
         // The following code is to setup the various insert points based on the following diagrams
         // of basic blocks:
         //
         // start:               setup:                          end result after moving runtime guard'
         //                       |       |                        +-------+ <-- privargIns
         //                       |       | <-- privargIns             |
         //                       +-------+ <-- runtimeIns         +-------+
         //   |       |               |                            | Guard'|
         //   |       |               V                            +-------+ <-- runtimeIns
         //   +-------+           +-------+                            |
         //   | Guard |           | Guard |                            V
         //   +-------+           +-------+ <-- HCRIns             +-------+
         //       |        ===>       |                    ===>    | Guard |
         //       V                   V                            +-------+ <-- HCRIns
         //   +-------+           +-------+                            |
         //   |       |           |       |                            V
         //   |       |           |       |                        +-------+
         //
         // Note we always split the block - this may create an empty block but preserves the incoming
         // control flow we leave the rest to block extension to fix later

         block = block->split(block->getLastRealTreeTop(), cfg, true, false);
         TR::Block *privargIns = block->getPrevBlock();
         TR::Block *runtimeIns = block->getPrevBlock();
         TR::Block *HCRIns = block;

         // scan for candidate guards to merge with guard1 identified above
         for (TR::Block *nextBlock = block->getNextBlock(); nextBlock; nextBlock = nextBlock->getNextBlock())
            {
            if (!(nextBlock->getPredecessors().size() == 1) ||
                !nextBlock->hasPredecessor(block))
               {
               break;
               }

            TR::TreeTop *guard2Tree = NULL;
            if (isMergeableGuard(nextBlock->getFirstRealTreeTop()->getNode()))
               {
               guard2Tree = nextBlock->getFirstRealTreeTop();
               }
            else if (isMergeableGuard(nextBlock->getLastRealTreeTop()->getNode()))
               {
               guard2Tree = nextBlock->getLastRealTreeTop();
               }
            else
               break;

            TR::Node *guard2 = guard2Tree->getNode();
            TR::Block *guard2Block = nextBlock;

            TR::Block *insertPoint = guard2->isHCRGuard() ? HCRIns : runtimeIns;
            if (!safeToMoveGuard(insertPoint, guard2Tree, guard1->getBranchDestination()))
               break;

            // now we figure out if we can redirect guard2 to guard1's cold block
            // ie can we do the head merge
            TR::Block *cold2 = guard2->getBranchDestination()->getEnclosingBlock();
            if (guard1->getInlinedSiteIndex() == guard2->getInlinedSiteIndex())
               {
               if (trace())
                  traceMsg(comp(), "  Guard1 [%p] is guarding the same call as Guard2 [%p] - proceeding with guard merging\n", guard1, guard2);
               }
            else if (guard2->getInlinedSiteIndex() > -1 &&
                guard1->getInlinedSiteIndex() == TR::comp()->getInlinedCallSite(guard2->getInlinedSiteIndex())._byteCodeInfo.getCallerIndex())
               {
               if (trace())
                  traceMsg(comp(), "  Guard1 [%p] is the caller of Guard2 [%p] - proceeding with guard merging\n", guard1, guard2);
               }
            else if ((cold1->getSuccessors().size() == 1) &&
                     cold1->hasSuccessor(cold2))
               {
               if (trace())
                  traceMsg(comp(), "  Guard1 cold destination block_%d has guard2 cold destination block_%d as its only successor - proceeding with guard merging\n", cold1->getNumber(), cold2->getNumber());
               }
            else
               {
               if (trace())
                  traceMsg(comp(), "  Cold1 block_%d and cold2 block_%d of guard2 [%p] in unknown relationship - abandon the merge attempt\n", cold1->getNumber(), cold2->getNumber(), guard2);
               break;
               }

            if (!performTransformation(comp(), "%sRedirecting guard [%p] in block_%d to parent guard cold block_%d\n", OPT_DETAILS, guard2, guard2Block->getNumber(), cold1->getNumber()))
                  continue;

            if (guard2->getBranchDestination() != guard1->getBranchDestination())
               guard2Block->changeBranchDestination(guard1->getBranchDestination(), cfg);

            if (guard2Tree != guard2Block->getFirstRealTreeTop())
               {
               cfg->setStructure(NULL);
               guard2Block = guard2Block->split(guard2Tree, cfg, true, false);
               if (trace())
                  traceMsg(comp(), "  Created new block_%d to hold guard [%p] from block_%d\n", guard2Block->getNumber(), guard2, guard2Block->getNumber());

               // the block created above guard2 contains only privarg treetops if
               // guard2 is a runtime-patchable guard. We need to move these args up
               // to the insert point and update the insertion pointers as needed

               //TODO We can safely leave code ahead of an HCR guard in place if no runtime guards are going to be moved or if
               //     there is no data dependence between the code moved from ahead of the runtime guards and the code ahead
               //     of the HCR guard. This opt will increase the chances of a patch point not requiring nops (perf opportunity).
			   TR::Block *privargBlock = guard2Block->getPrevBlock();

			   if (trace())
				  traceMsg(comp(), "  Moving privarg block_%d after block_%d\n", privargBlock->getNumber(), privargIns->getNumber());

			   moveBlockAfterDest(cfg, privargBlock, privargIns);

			   if (HCRIns == privargIns)
				  HCRIns = privargBlock;
			   if (runtimeIns == privargIns)
				  runtimeIns = privargBlock;
			   privargIns = privargBlock;

			   // refresh the insertPoint since it could be stale after the above updates
			   insertPoint = runtimeIns;
               }

            if (insertPoint != guard2Block->getPrevBlock())
               {
               TR::DebugCounter::incStaticDebugCounter(comp(), TR::DebugCounter::debugCounterName(comp(), "headMerger/%s_%s/(%s)", guard1->isHCRGuard() ? "hcr" : "runtime", guard2->isHCRGuard() ? "hcr" : "runtime", comp()->signature()));
               cfg->setStructure(NULL);

               block = nextBlock = guard2Block->getPrevBlock();
               if (trace())
                  traceMsg(comp(), "  Moving guard2 block block_%d after block_%d\n", guard2Block->getNumber(), insertPoint->getNumber());

               moveBlockAfterDest(cfg, guard2Block, insertPoint);

               if (HCRIns == insertPoint)
                  HCRIns = guard2Block;
               if (runtimeIns == insertPoint)
                  runtimeIns = guard2Block;
               }
            else
               {
               block = guard2Block;
               }
            guard1 = guard2;
            }
         }
      }
   return 1;
}

// This function splits a single succeesor block following an guard and is used to
// do the following transform
//    block - cold1         block - cold1
//      \     /        =>     |       |
//     nextBlock           nextBlock nextBlock' (called tailSplitBlock below)
//         |                  \      /
//        ...                   ...
void TR_VirtualGuardHeadMerger::tailSplitBlock(TR::Block * block, TR::Block * cold1)
   {
   TR::CFG *cfg = comp()->getFlowGraph();
   cfg->setStructure(NULL);
   TR_BlockCloner cloner(cfg);
   TR::Block *tailSplitBlock = cloner.cloneBlocks(block->getNextBlock(), block->getNextBlock());
   tailSplitBlock->setFrequency(cold1->getFrequency());
   if (cold1->isCold())
      tailSplitBlock->setIsCold();

   // physically put the block after cold1 since we want cold1 to fall through
   tailSplitBlock->getExit()->join(cold1->getExit()->getNextTreeTop());
   cold1->getExit()->join(tailSplitBlock->getEntry());

   // remove cold1's goto
   TR::TransformUtil::removeTree(comp(), cold1->getExit()->getPrevRealTreeTop());

   // copy the exception edges
   for (auto e = block->getNextBlock()->getExceptionSuccessors().begin(); e != block->getNextBlock()->getExceptionSuccessors().end(); ++e)
      cfg->addExceptionEdge(tailSplitBlock, (*e)->getTo());

   cfg->addEdge(cold1, tailSplitBlock);
   // lastly fix up the exit of tailSplitBlock
   TR::Node *tailSplitEnd = tailSplitBlock->getExit()->getPrevRealTreeTop()->getNode();
   if (tailSplitEnd->getOpCode().isGoto())
      {
      tailSplitEnd->setBranchDestination(block->getNextBlock()->getLastRealTreeTop()->getNode()->getBranchDestination());
      cfg->addEdge(tailSplitBlock, block->getNextBlock()->getSuccessors().front()->getTo());
      }
   else if (tailSplitEnd->getOpCode().isBranch())
      {
      TR::Block *gotoBlock = TR::Block::createEmptyBlock(tailSplitEnd, comp(), cold1->getFrequency());
      if (cold1->isCold())
          gotoBlock->setIsCold(true);
      gotoBlock->getExit()->join(tailSplitBlock->getExit()->getNextTreeTop());
      tailSplitBlock->getExit()->join(gotoBlock->getEntry());
      cfg->addNode(gotoBlock);

      gotoBlock->append(TR::TreeTop::create(comp(), TR::Node::create(tailSplitEnd, TR::Goto, 0, block->getNextBlock()->getExit()->getNextTreeTop())));
      cfg->addEdge(tailSplitBlock, gotoBlock);
      cfg->addEdge(tailSplitBlock, tailSplitBlock->getLastRealTreeTop()->getNode()->getBranchDestination()->getEnclosingBlock());
      cfg->addEdge(gotoBlock, block->getNextBlock()->getNextBlock());
      }
   else if (
            !tailSplitEnd->getOpCode().isReturn() &&
            !tailSplitEnd->getOpCode().isJumpWithMultipleTargets() &&
             tailSplitEnd->getOpCodeValue() != TR::athrow &&
            !(tailSplitEnd->getNumChildren() >= 1 && tailSplitEnd->getFirstChild()->getOpCodeValue() == TR::athrow)
           )
      {
      tailSplitBlock->append(TR::TreeTop::create(comp(), TR::Node::create(tailSplitEnd, TR::Goto, 0, block->getNextBlock()->getExit()->getNextTreeTop())));
      cfg->addEdge(tailSplitBlock, block->getNextBlock()->getNextBlock());
      }
   else
      {
      for (auto e = block->getNextBlock()->getSuccessors().begin(); e != block->getNextBlock()->getSuccessors().end(); ++e)
         cfg->addEdge(tailSplitBlock, (*e)->getTo());
      }
   cfg->removeEdge(cold1, block->getNextBlock());

   optimizer()->setUseDefInfo(NULL);
   optimizer()->setValueNumberInfo(NULL);
   }

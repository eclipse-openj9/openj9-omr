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

#include "optimizer/Simplifier.hpp"

#include "optimizer/OMRSimplifierHelpers.hpp"
#include "optimizer/OMRSimplifierHandlers.hpp"
#include "optimizer/SimplifierTable.hpp"

#include <limits.h>                            // for SCHAR_MAX, SHRT_MAX, etc
#include <math.h>                              // for fabs
#include <stddef.h>                            // for size_t
#include <stdint.h>                            // for int32_t, int64_t, etc
#include <stdio.h>                             // for sprintf
#include <stdlib.h>                            // for abs
#include <string.h>                            // for NULL, strlen, strcmp, etc
#include "codegen/CodeGenerator.hpp"           // for CodeGenerator
#include "codegen/FrontEnd.hpp"                // for TR_FrontEnd
#include "codegen/Linkage.hpp"                 // for Linkage
#include "codegen/RecognizedMethods.hpp"
#include "codegen/StorageInfo.hpp"
#include "codegen/TreeEvaluator.hpp"           // for TreeEvaluator
#include "compile/Compilation.hpp"             // for Compilation, comp
#include "compile/ResolvedMethod.hpp"          // for TR_ResolvedMethod
#include "compile/SymbolReferenceTable.hpp"    // for SymbolReferenceTable
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"         // for TR::Options, etc
#include "cs2/sparsrbit.h"
#include "env/IO.hpp"                          // for POINTER_PRINTF_FORMAT
#include "env/ObjectModel.hpp"                 // for ObjectModel
#include "env/TRMemory.hpp"                    // for TR_Memory, etc
#include "il/AliasSetInterface.hpp"
#include "il/Block.hpp"                        // for Block
#include "il/DataTypes.hpp"                    // for DataTypes, etc
#include "il/ILOpCodes.hpp"                    // for ILOpCodes::lconst, etc
#include "il/ILOps.hpp"                        // for ILOpCode, TR::ILOpCode
#include "il/Node.hpp"                         // for Node, etc
#include "il/Node_inlines.hpp"                 // for Node::getFirstChild, etc
#include "il/Symbol.hpp"                       // for Symbol
#include "il/SymbolReference.hpp"              // for SymbolReference
#include "il/TreeTop.hpp"                      // for TreeTop
#include "il/TreeTop_inlines.hpp"              // for TreeTop::getNode, etc
#include "il/symbol/LabelSymbol.hpp"           // for LabelSymbol
#include "il/symbol/MethodSymbol.hpp"          // for MethodSymbol
#include "il/symbol/ResolvedMethodSymbol.hpp"  // for ResolvedMethodSymbol
#include "il/symbol/StaticSymbol.hpp"          // for StaticSymbol
#include "infra/Assert.hpp"                    // for TR_ASSERT
#include "infra/Bit.hpp"                       // for trailingZeroes, etc
#include "infra/BitVector.hpp"                 // for TR_BitVector, etc
#include "infra/Cfg.hpp"                       // for CFG
#include "infra/Link.hpp"                      // for TR_LinkHead1
#include "infra/List.hpp"                      // for List, ListElement, etc
#include "infra/TRCfgEdge.hpp"                 // for CFGEdge
#include "infra/TRCfgNode.hpp"                 // for CFGNode
#include "compiler/il/ILOpCodes.hpp"                // for ILOpCodes
#include "optimizer/Optimization.hpp"          // for Optimization
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/OptimizationManager.hpp"   // for OptimizationManager
#include "optimizer/Optimizations.hpp"
#include "optimizer/Optimizer.hpp"             // for Optimizer
#include "optimizer/Structure.hpp"             // for TR_BlockStructure, etc
#include "ras/Debug.hpp"                       // for TR_DebugBase, etc

extern const SimplifierPtr simplifierOpts[];

static_assert(TR::NumIlOps ==
              (sizeof(simplifierOpts) / sizeof(simplifierOpts[0])),
              "simplifierOpts is not the correct size");


/*
 * Local helper functions
 */

static void resetBlockVisitFlags(TR::Compilation *comp)
   {
   for (TR::Block *block = comp->getStartBlock(); block != NULL; block = block->getNextBlock())
      {
      block->setHasBeenVisited(false);
      }
   }

static TR::TreeTop *findNextLegalTreeTop(TR::Compilation *comp, TR::Block *block)
   {
   vcount_t startVisitCount = comp->getStartTree()->getNode()->getVisitCount();
   TR::TreeTop * tt = NULL;
   for (tt = comp->getStartTree(); tt; tt = tt->getNextTreeTop())
      {
      if (tt->getNode()->getVisitCount() < startVisitCount)
         break;
      if (tt->getNode()->getOpCodeValue() == TR::BBStart)
         tt = tt->getNode()->getBlock()->getExit();
      }
   return tt;
   }

static void countNodes(CS2::ABitVector< TR::Allocator > & mark,
                       TR::Node * n,
                       size_t & numNodes)
   {
   if (mark.ValueAt(n->getGlobalIndex()))
      {
      return;
      }

   mark[n->getGlobalIndex()] = true;
   numNodes += 1;

   for (size_t i = 0; i < n->getNumChildren(); i++)
      {
      countNodes(mark, n->getChild(i), numNodes);
      }
   }

static void computeInvarianceOfAllStructures(TR::Compilation *comp, TR_Structure * s)
   {
   TR_RegionStructure *region = s->asRegion();

   if (region)
      {
      TR_StructureSubGraphNode *node;
      TR_RegionStructure::Cursor si(*region);
      for (node = si.getCurrent(); node != NULL; node = si.getNext())
         computeInvarianceOfAllStructures(comp, node->getStructure());

      region->resetInvariance();
      if (region->isNaturalLoop() /*|| region->containsInternalCycles() */)
         {
         region->computeInvariantExpressions();
         }
      }
   }


/*
 * Simplifier class functions
 */

// Simplify all blocks
//
OMR::Simplifier::Simplifier(TR::OptimizationManager *manager)
   : TR::Optimization(manager),
     _hashTable(manager->trMemory(), stackAlloc),
     _ccHashTab(manager->trMemory(), stackAlloc)
   {
   _invalidateUseDefInfo      = false;
   _alteredBlock = false;
   _blockRemoved = false;

   _useDefInfo = optimizer()->getUseDefInfo();
   _valueNumberInfo = optimizer()->getValueNumberInfo();

   _reassociate = comp()->getOption(TR_EnableReassociation);

   _containingStructure = NULL;
   }

void
OMR::Simplifier::prePerformOnBlocks()
   {
   _invalidateUseDefInfo      = false;
   _alteredBlock = false;
   _blockRemoved = false;

   _useDefInfo = optimizer()->getUseDefInfo();
   _valueNumberInfo = optimizer()->getValueNumberInfo();
   _containingStructure = NULL;


   if (_reassociate)
      {
      _hashTable.reset();
      _hashTable.init(1000, true);

      TR_ASSERT(comp()->getFlowGraph()->getStructure(), "assertion failure");
      computeInvarianceOfAllStructures(comp(), comp()->getFlowGraph()->getStructure());
      }
      _ccHashTab.reset();
      _ccHashTab.init(64, true);

   if (trace())
      {
      comp()->dumpMethodTrees("Trees before simplification");
      }
   }

void
OMR::Simplifier::postPerformOnBlocks()
   {
   if (trace())
      comp()->dumpMethodTrees("Trees after simplification");

#ifdef DEBUG
   resetBlockVisitFlags(comp());
#endif

   // Invalidate usedef and value number information if necessary
   //
   if (_useDefInfo && _invalidateUseDefInfo)
      optimizer()->setUseDefInfo(NULL);
   if (_valueNumberInfo && _invalidateValueNumberInfo)
      optimizer()->setValueNumberInfo(NULL);
   }

int32_t
OMR::Simplifier::perform()
   {

   vcount_t visitCount = comp()->incOrResetVisitCount();
   TR::TreeTop * tt;
   for (tt = comp()->getStartTree(); tt; tt = tt->getNextTreeTop())
      tt->getNode()->initializeFutureUseCounts(visitCount);

   comp()->incVisitCount();
   for (tt = comp()->getStartTree(); tt; tt = tt->getNextTreeTop())
      cleanupFlags(tt->getNode());

   visitCount = comp()->incVisitCount();
   tt = comp()->getStartTree();
   while (tt)
      tt = simplifyExtendedBlock(tt);

   comp()->getFlowGraph()->removeUnreachableBlocks();

   if (manager()->numPassesCompleted() == 0)
      manager()->incNumPassesCompleted();

   return 1;
   }


int32_t
OMR::Simplifier::performOnBlock(TR::Block * block)
   {
   if (block->getEntry())
      {
      TR::TreeTop *extendedExitTree = block->getEntry()->getExtendedBlockExitTreeTop();
      vcount_t visitCount = comp()->incOrResetVisitCount();
      for (TR::TreeTop * tt = block->getEntry(); tt; tt = tt->getNextTreeTop())
         {
         tt->getNode()->initializeFutureUseCounts(visitCount);
         if (tt == extendedExitTree)
            break;
         }

      comp()->incVisitCount();
      simplifyExtendedBlock(block->getEntry());
      }
   return 0;
   }

// Pre-order traversal, remove the flags on the way in.
// On the way out set the flags to true as appropriate:
//     nodeRequiresConditionCodes on the first child of computeCC nodes.
//     adjunct                    on the third child of dual high nodes.
//
void
OMR::Simplifier::cleanupFlags(TR::Node *node)
   {
   if (node->getVisitCount() == comp()->getVisitCount())
      return;
   node->setVisitCount(comp()->getVisitCount());

   if (node->nodeRequiresConditionCodes())
      node->setNodeRequiresConditionCodes(false);

   if (node->isAdjunct())
      node->setIsAdjunct(false);

   for (int32_t i = node->getNumChildren()-1; i >= 0; --i)
      cleanupFlags(node->getChild(i));

   if (node->getOpCodeValue() == TR::computeCC)
      node->getFirstChild()->setNodeRequiresConditionCodes(true);

   if (node->isDualHigh())
      node->getChild(2)->setIsAdjunct(true);
   }

void
OMR::Simplifier::setCC(TR::Node *node, OMR::TR_ConditionCodeNumber cc)
   {
   TR_ASSERT(node->nodeRequiresConditionCodes(), "assertion failure");
   TR_ASSERT(cc <= OMR::ConditionCodeLast, "illegal condition code setting");

   TR_HashId index = 0;
//   FIXME: is this assume actually needed? It seems harmless if the node is already present
//   TR_ASSERT(!_ccHashTab.locate(node->getGlobalIndex(), index),
//   "node already present in cc hashtable");
   _ccHashTab.add(node->getGlobalIndex(), index, (void*)cc);
   }

OMR::TR_ConditionCodeNumber
OMR::Simplifier::getCC(TR::Node *node)
   {
   TR_HashId index;
   if (!_ccHashTab.locate(node->getGlobalIndex(), index))
      return OMR::ConditionCodeInvalid;
   return (OMR::TR_ConditionCodeNumber)(uintptr_t) _ccHashTab.getData(index);
   }

TR::TreeTop *
OMR::Simplifier::simplifyExtendedBlock(TR::TreeTop * treeTop)
   {
   TR::Block * block = 0;

   _containingStructure = NULL;
   _blockRemoved = false;

   for (; treeTop; treeTop = treeTop->getNextTreeTop())
      {
      TR::Node * node = treeTop->getNode();
      TR_ASSERT(node->getOpCodeValue() == TR::BBStart, "Simplification, expected BBStart treetop");

      TR::Block * b = node->getBlock();
      if (block && !b->isExtensionOfPreviousBlock())
         break;

#ifdef DEBUG
      if (block != b)
         b->setHasBeenVisited();
#endif

      if (!block && _reassociate &&
          comp()->getFlowGraph()->getStructure() != NULL         // [99391] getStructureOf() only valid if structure isn't invalidated
         )
         { // b is first block in the extended block
         TR_BlockStructure *blockStructure = b->getStructureOf();

         if(blockStructure)
            {
            TR_Structure *parent = blockStructure->getParent();
            while (parent)
               {
               TR_RegionStructure *region = parent->asRegion();
               if (region->isNaturalLoop() /* || region->containsInternalCycles() */)
                  {
                  _containingStructure = region;
                  break;
                  }
               parent = parent->getParent();
               }
            }
         }

      block = b;

      if (trace())
         traceMsg(comp(), "simplifying block_%d\n", block->getNumber());

      _performLowerTreeSimplifier=NULL;
      _performLowerTreeNode=NULL;
      simplify(block);

      if(_performLowerTreeSimplifier)
         {
         _performLowerTreeNode = postWalkLowerTreeSimplifier(_performLowerTreeSimplifier, _performLowerTreeNode, block, (TR::Simplifier *) this);
         _performLowerTreeSimplifier->setNode(_performLowerTreeNode);
         }

      // If the block itself was removed from the CFG during simplification, find
      // the next 'legitimate' block to be simplified
      //
      //if (comp()->getFlowGraph()->getRemovedNodes().find(block))
        if(block->nodeIsRemoved())
         {
         TR::TreeTop * tt = findNextLegalTreeTop(comp(), block);
         // in certain cases the removed block might be the last one we haven't
         // visited and therefore we won't be able to find a treetop to continue
         // in such cases we exit the loop
         //
         treeTop = tt ? tt->getPrevTreeTop() : 0;
         if (!treeTop)
            break;
         }
      else
         {
         treeTop = block->getExit();
         }
      }

   // now remove any unreachable blocks
   //
   if (_blockRemoved)
      {
      // if the next block to be processed has been removed,
      // find the next valid block to process
      //
      if (treeTop)
         {
         TR::Block *b = treeTop->getNode()->getBlock();
         //if (comp()->getFlowGraph()->getRemovedNodes().find(b))
           if(b->nodeIsRemoved())
            treeTop = findNextLegalTreeTop(comp(), b);
         }
      }

   return treeTop;
   }

void
OMR::Simplifier::simplify(TR::Block * block)
   {
   _alteredBlock = false;

   TR::TreeTop * tt, * next;

   //vcount_t visitCount = comp()->incVisitCount();
   //for (tt = block->getEntry(); tt; tt = tt->getNextTreeTop())
    //  tt->getNode()->initializeFutureUseCounts(visitCount);

   for (tt = block->getEntry(); tt; tt = next)
      {
      next = simplify(tt, block);

      // NOTE: simplification can change the exit for a block, so don't move
      // this getExit call out of the loop.
      //
      if (tt == block->getExit())
         break;
      }

   if (_alteredBlock)
      {
      _invalidateValueNumberInfo = true;
      requestOpt(OMR::localCSE, true, block);
      }
   }

// Simplify a complete expression tree.
// Returns the next treetop to be processed.
//
TR::TreeTop *
OMR::Simplifier::simplify(TR::TreeTop * treeTop, TR::Block * block)
   {
   TR::Node * node = treeTop->getNode();
   if (node->getVisitCount() == comp()->getVisitCount())
      return treeTop->getNextTreeTop();

   // Note that this call to simplify may cause the treetops before or after
   // this treetop to be removed, so we can't hold the previous or next
   // treetop locally across this call.
   //
   _curTree = treeTop;
   node = simplify(node, block);
   treeTop->setNode(node);

   // Grab the next treetop AFTER simplification of the current treetop, since
   // the next may be affected by simplification.
   //
   TR::TreeTop * next = _curTree->getNextTreeTop();

   // If the node is null, this treetop can be removed
   //
   if (node == NULL &&
       (!block->getPredecessors().empty() ||
        !block->getExceptionPredecessors().empty()))
      comp()->removeTree(treeTop);

   return next;
   }

// Simplify a sub-tree.
// Returns the replaced root of the sub-tree, which may be null if the sub-tree
// has been removed.
//
TR::Node *
OMR::Simplifier::simplify(TR::Node * node, TR::Block * block)
   {
   // Set the visit count for this node to prevent recursion into it
   //
   vcount_t visitCount = comp()->getVisitCount();
   node->setVisitCount(visitCount);

   if (node->nodeRequiresConditionCodes())
      {
      // On Java, nodes that require condition codes must not be simplified.
      dftSimplifier(node, block, (TR::Simplifier *) this);
      return node;
      }

   // Simplify this node.
   // Note that the processing routine for the node is responsible for
   // simplifying its children.
   //
   TR::Node * newNode = simplifierOpts[node->getOpCodeValue()](node, block, (TR::Simplifier *) this);
   if ((node != newNode) ||
       (newNode &&
        ((newNode->getOpCodeValue() != node->getOpCodeValue()) ||
         (newNode->getNumChildren() != node->getNumChildren()))))
        requestOpt(OMR::localCSE, true, block);

   return newNode;
   }

TR::Node *
OMR::Simplifier::unaryCancelOutWithChild(TR::Node * node, TR::Node * firstChild, TR::TreeTop *anchorTree, TR::ILOpCodes opcode, bool anchorChildren)
   {
#ifdef J9_PROJECT_SPECIFIC
   if (node->getOpCode().isConversionWithFraction() &&
       firstChild->getOpCode().isConversionWithFraction() &&
       (node->getDecimalFraction() != firstChild->getDecimalFraction()))
      {
      // illegal to fold a pattern like:
      // pd2f  frac=5
      //    f2pd frac=0
      //       f
      // to just 'f' because the extra digits that should be introduced by the frac=5 in the parent will be lost
      if (trace())
         traceMsg(comp(),"disallow unaryCancel of node %p and firstChild %p due to mismatch of decimal fractions (%d != %d)\n",
                 node,firstChild,node->getDecimalFraction(),firstChild->getDecimalFraction());
      return 0;
      }
   if (firstChild->getOpCodeValue() == opcode &&
       node->getType().isBCD() && firstChild->getType().isBCD() && firstChild->getFirstChild()->getType().isBCD() &&
       node->hasIntermediateTruncation())
      {
      // illegal to fold if there is an intermediate (firstChild) truncation:
      // zd2pd p=4         0034
      //    pd2zd p=2        34
      //       pdx p=4     1234
      // if folding is performed to remove zd2pd/pd2zd then the result will be 1234 instead of 0034
      if (trace())
         traceMsg(comp(),"disallow unaryCancel of node %p and firstChild %p due to intermediate truncation of node\n",node,firstChild);
      return 0;
      }
   else if (firstChild->getOpCodeValue() == opcode && node->getType().isBCD() && !firstChild->getType().isBCD())
      {
      // illegal to fold an intermediate truncation:
      // dd2zd p=20 srcP=13
      //   zd2dd (no p specifed, but by data type, must be <= 16 and by srcP must be <= 13)
      //     zdX p=20
      // Folding gives an incorrect result if either srcP or the implied p of the zd2dd is less than p on the dd2zd
      int32_t nodeP = node->getDecimalPrecision();
      int32_t childP = TR::DataType::getMaxPackedDecimalPrecision();
      int32_t grandChildP = firstChild->getFirstChild()->getDecimalPrecision();

      if (node->hasSourcePrecision())
         childP = node->getSourcePrecision();
      else if (TR::DataType::canGetMaxPrecisionFromType(firstChild->getDataType()))
         childP = TR::DataType::getMaxPrecisionFromType(firstChild->getDataType());

      if (childP < nodeP && childP < grandChildP)
         {
         if (trace())
            traceMsg(comp(),"disallow unaryCancel of node %p and firstChild %p due to intermediate truncation of node\n",node,firstChild);
         return 0;
         }
      }
   else if (firstChild->getOpCodeValue() == opcode && !node->getType().isBCD() && !firstChild->getType().isBCD())
      {
      // illegal to fold an intermediate truncation:
      // dd2l
      //   l2dd
      //     lX
      // Folding could give an incorrect result because the max precision of a dd is 16 and the max precision of an l is 19
      if (TR::DataType::canGetMaxPrecisionFromType(node->getDataType()) && TR::DataType::canGetMaxPrecisionFromType(firstChild->getDataType()) &&
          TR::DataType::getMaxPrecisionFromType(node->getDataType()) > TR::DataType::getMaxPrecisionFromType(firstChild->getDataType()))
         {
         if (trace())
            traceMsg(comp(),"disallow unaryCancel of node %p and firstChild %p due to intermediate truncation of node\n",node,firstChild);
         return 0;
         }
      }
#endif

   int32_t bytesLeftAfterTruncation = -1;
   if (firstChild->getOpCodeValue() == opcode &&
       (node->getType().isAggregate() || firstChild->getType().isAggregate()) &&
       (node->getSize() > firstChild->getSize() || node->getSize() != firstChild->getFirstChild()->getSize()))
      {
      // ensure a truncation side-effect of a conversion is not lost
      // o2a size=3
      //   a2o size=3 // conversion truncates in addition to type cast so cannot be removed
      //     loadaddr size=4
      // This restriction could be loosened to only disallow intermediate truncations (see BCD case above) but then would require a node
      // op that would just correct for size (e.g. addrSizeMod size=3 to replace the o2a/a2o pair)
      //
      // Do allow cases when all three sizes are the same and when the middle node widens but the top and bottom node have the same size, e.g.
      //
      // i2o size=3
      //   o2i size=4
      //     oload size=3
      //
      // Also allow the special case where the grandchild is not really truncated as the 'truncated' bytes are known to be zero
      // (i.e. there really isn't an intermediate truncation of 4->3 even though it appears that way from looking at the sizes alone)
      // o2i
      //   i2o size=3
      //     iushr
      //       x
      //       iconst 8
      bool disallow = true;
      TR::Node *grandChild = firstChild->getFirstChild();
      size_t nodeSize = node->getSize();
      if (node->getType().isIntegral() &&
          nodeSize == grandChild->getSize() &&
          nodeSize > firstChild->getSize())
         {
         size_t truncatedBits = (nodeSize - firstChild->getSize()) * 8;
         if (grandChild->getOpCode().isRightShift() && grandChild->getOpCode().isShiftLogical() &&
             grandChild->getSecondChild()->getOpCode().isLoadConst() &&
             (grandChild->getSecondChild()->get64bitIntegralValue() == truncatedBits))
            {
            disallow = false;
            if (trace())
               traceMsg(comp(),"do allow unaryCancel of node %s (%p) and firstChild %s (%p) as grandChild %s (%p) zeros the %d truncated bytes\n",
                       node->getOpCode().getName(),node,firstChild->getOpCode().getName(),firstChild,
                       grandChild->getOpCode().getName(),grandChild,truncatedBits/8);
            }
         }

      if (disallow)
         {
         if (trace())
            traceMsg(comp(),"disallow unaryCancel of node %s (%p) and firstChild %s (%p) due to unequal sizes (nodeSize %d, firstChildSize %d, firstChild->childSize %d)\n",
                    node->getOpCode().getName(),node,firstChild->getOpCode().getName(),firstChild,
                    node->getSize(),firstChild->getSize(),firstChild->getFirstChild()->getSize());
         return 0;
         }
      }

   if (firstChild->getOpCodeValue() == opcode &&
       performTransformation(comp(), "%sRemoving node [" POINTER_PRINTF_FORMAT "] %s and its child [" POINTER_PRINTF_FORMAT "] %s\n",
             optDetailString(), node, node->getOpCode().getName(), firstChild, firstChild->getOpCode().getName()))
      {
      TR::Node * grandChild = firstChild->getFirstChild();
      grandChild->incReferenceCount();
      bool anchorChildrenNeeded = anchorChildren &&
         (node->getNumChildren() > 1 ||
          firstChild->getNumChildren() > 1 ||
          node->getOpCode().hasSymbolReference() ||
          firstChild->getOpCode().hasSymbolReference());
      prepareToStopUsingNode(node, anchorTree, anchorChildrenNeeded);
      node->recursivelyDecReferenceCount();
#ifdef J9_PROJECT_SPECIFIC
      TR_RawBCDSignCode alwaysGeneratedSign = comp()->cg()->alwaysGeneratedSign(node);
      if (node->getType().isBCD() &&
          grandChild->getType().isBCD() &&
          (node->getDecimalPrecision() != grandChild->getDecimalPrecision() || alwaysGeneratedSign != raw_bcd_sign_unknown))
         {
         // must maintain the top level node's precision when replacing with the grandchild
         // (otherwise if the parent of the node is call it will pass a too small or too big value)
         TR::Node *origOrigGrandChild = grandChild;
         if (node->getDecimalPrecision() != grandChild->getDecimalPrecision())
            {
            TR::Node *origGrandChild = grandChild;
            grandChild = TR::Node::create(TR::ILOpCode::modifyPrecisionOpCode(grandChild->getDataType()), 1, origGrandChild);
            origGrandChild->decReferenceCount(); // inc'd an extra time when creating modPrecision node above
            grandChild->incReferenceCount();
            grandChild->setDecimalPrecision(node->getDecimalPrecision());
            dumpOptDetails(comp(), "%sCreate %s [" POINTER_PRINTF_FORMAT "] to reconcile precision mismatch between node %s [" POINTER_PRINTF_FORMAT "] grandChild %s [" POINTER_PRINTF_FORMAT "] (%d != %d)\n",
                             optDetailString(),
                             grandChild->getOpCode().getName(),
                             grandChild,
                             node->getOpCode().getName(),
                             node,
                             origOrigGrandChild->getOpCode().getName(),
                             origOrigGrandChild,
                             node->getDecimalPrecision(),
                             origOrigGrandChild->getDecimalPrecision());
            }
         // if the top level was always setting a particular sign code (e.g. ud2pd) then must maintain this side-effect here when cancelling
         if (alwaysGeneratedSign != raw_bcd_sign_unknown)
            {
            TR::Node *origGrandChild = grandChild;
            TR::ILOpCodes setSignOp = TR::ILOpCode::setSignOpCode(grandChild->getDataType());
            TR_ASSERT(setSignOp != TR::BadILOp,"could not find setSignOp for type %d on %s (%p)\n",
                    grandChild->getDataType(),grandChild->getOpCode().getName(),grandChild);
            grandChild = TR::Node::create(setSignOp, 2,
                                         origGrandChild,
                                         TR::Node::iconst(origGrandChild, TR::DataType::getValue(alwaysGeneratedSign)));
            origGrandChild->decReferenceCount(); // inc'd an extra time when creating setSign node above
            grandChild->incReferenceCount();
            grandChild->setDecimalPrecision(origGrandChild->getDecimalPrecision());
            dumpOptDetails(comp(), "%sCreate %s [" POINTER_PRINTF_FORMAT "] to preserve setsign side-effect between node %s [" POINTER_PRINTF_FORMAT "] grandChild %s [" POINTER_PRINTF_FORMAT "] (sign=0x%x)\n",
                             optDetailString(),
                             grandChild->getOpCode().getName(),
                             grandChild,
                             node->getOpCode().getName(),
                             node,
                             origOrigGrandChild->getOpCode().getName(),
                             origOrigGrandChild,
                             TR::DataType::getValue(alwaysGeneratedSign));
            }
         }
      else if (node->getType().isDFP() && firstChild->getType().isBCD())
         {
         // zd2dd
         //   dd2zd p=12 srcP=13
         //     ddX (p possibly unknown but <= 16)
         // Folding gives an incorrect result if the truncation on the dd2zd isn't preserved
         int32_t nodeP = TR::DataType::getMaxPackedDecimalPrecision();
         int32_t childP = firstChild->getDecimalPrecision();
         int32_t grandChildP = TR::DataType::getMaxPackedDecimalPrecision();

         if (TR::DataType::canGetMaxPrecisionFromType(node->getDataType()))
            {
            nodeP = TR::DataType::getMaxPrecisionFromType(node->getDataType());
            grandChildP = nodeP;
            }
         if (firstChild->hasSourcePrecision())
            grandChildP = firstChild->getSourcePrecision();

         if (childP < nodeP && childP < grandChildP)
            {
            TR::Node *origOrigGrandChild = grandChild;
            TR::Node *origGrandChild = grandChild;
            grandChild = TR::Node::create(TR::ILOpCode::modifyPrecisionOpCode(grandChild->getDataType()), 1, origGrandChild);
            origGrandChild->decReferenceCount(); // inc'd an extra time when creating modPrecision node above
            grandChild->incReferenceCount();
            grandChild->setDFPPrecision(childP);
            dumpOptDetails(comp(), "%sCreate %s [" POINTER_PRINTF_FORMAT "] to reconcile precision mismatch between node %s [" POINTER_PRINTF_FORMAT "] grandChild %s [" POINTER_PRINTF_FORMAT "] (%d != %d)\n",
                             optDetailString(),
                             grandChild->getOpCode().getName(),
                             grandChild,
                             node->getOpCode().getName(),
                             node,
                             origOrigGrandChild->getOpCode().getName(),
                             origOrigGrandChild,
                             nodeP,
                             childP);
            }
         }
      else
#endif
         if (bytesLeftAfterTruncation > 0)
         {
         TR_ASSERT(bytesLeftAfterTruncation < 8,"bytesLeftAfterTruncation %d should be < 8 for node %p\n",bytesLeftAfterTruncation,node);
         TR_ASSERT(grandChild->getType().isInt64(),"node %s (%p) should an Int64 type\n",grandChild->getOpCode().getName(),grandChild);
         uint64_t bitsLeftAfterTruncation = (uint64_t)bytesLeftAfterTruncation*8;
         uint64_t constantForAnd = (1ull << bitsLeftAfterTruncation)-1;
         grandChild = TR::Node::create(TR::land, 2, grandChild, TR::Node::lconst(grandChild, constantForAnd));
         grandChild->getFirstChild()->decReferenceCount(); // inc'd an extra time when creating modPrecision node above
         grandChild->incReferenceCount();
         dumpOptDetails(comp(), "%sCreate %s [" POINTER_PRINTF_FORMAT "] 0x%llx to account for %d truncated bytes between node %s [" POINTER_PRINTF_FORMAT "] grandChild %s [" POINTER_PRINTF_FORMAT "]\n",
                          optDetailString(),
                          grandChild->getOpCode().getName(),
                          grandChild,
                          (uint64_t)constantForAnd,
                          bytesLeftAfterTruncation,
                          node->getOpCode().getName(),
                          node,
                          grandChild->getFirstChild()->getOpCode().getName(),
                          grandChild->getFirstChild());
         }

      node->setVisitCount(0);
      return grandChild;
      }
   return 0;
   }

//---------------------------------------------------------------------
// Common routine to change a conditional branch into an unconditional one.
// Change the node to be the unconditional branch or NULL if no branch taken.
// Return true if blocks were removed as a result of the change
//
bool
OMR::Simplifier::conditionalToUnconditional(TR::Node *&node, TR::Block * block, int takeBranch)
   {
   if (!performTransformation(comp(), "%s change conditional to unconditional n%in\n",
         optDetailString(), node->getNodePoolIndex()))
      {
      return false;
      }

   TR::CFGEdge* removedEdge = changeConditionalToUnconditional(node, block, takeBranch, _curTree, optDetailString());
   bool blocksWereRemoved = removedEdge ? removedEdge->getTo()->nodeIsRemoved() : false;


   if (takeBranch)
      {
      TR_ASSERT(node->getOpCodeValue() == TR::Goto, "expecting the node to have been converted to a goto");
      node = simplify(node, block);
      }

   if (blocksWereRemoved)
      {
      _invalidateUseDefInfo = true;
      _alteredBlock = true;
      _blockRemoved = true;
      }
   return blocksWereRemoved;
   }

void
OMR::Simplifier::prepareToReplaceNode(TR::Node * node)
   {
   OMR::Optimization::prepareToReplaceNode(node);
   _alteredBlock = true;
   }

void
OMR::Simplifier::anchorOrderDependentNodesInSubtree(TR::Node *node, TR::Node *replacement, TR::TreeTop* anchorTree)
{
   if (node == replacement)
      return;

   if (nodeIsOrderDependent(node, 0, false))
      {
      if (trace())
         traceMsg(comp(), "anchor detached node %p\n", node);

      generateAnchor(node, anchorTree);
      }
   else
      anchorChildren(node, anchorTree, 0, node->getReferenceCount() > 1, replacement);
}

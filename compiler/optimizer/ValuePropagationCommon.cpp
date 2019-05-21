/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

// ***************************************************************************
//
// TODO - float and double constants
//
// ***************************************************************************

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/FrontEnd.hpp"
#include "codegen/RecognizedMethods.hpp"
#include "compile/Compilation.hpp"
#include "compile/Method.hpp"
#include "compile/ResolvedMethod.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "control/Recompilation.hpp"
#include "cs2/hashtab.h"
#include "env/CompilerEnv.hpp"
#include "env/ObjectModel.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "optimizer/TransformUtil.hpp"
#include "il/Block.hpp"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/Symbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/TreeTop.hpp"
#include "il/TreeTop_inlines.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/symbol/ParameterSymbol.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "il/symbol/StaticSymbol.hpp"
#include "infra/Array.hpp"
#include "infra/Assert.hpp"
#include "infra/BitVector.hpp"
#include "infra/Cfg.hpp"
#include "infra/Link.hpp"
#include "infra/List.hpp"
#include "infra/Stack.hpp"
#include "infra/Statistics.hpp"
#include "infra/CfgEdge.hpp"
#include "infra/Timer.hpp"
#include "optimizer/Inliner.hpp"
#include "optimizer/Optimization.hpp"
#include "optimizer/Optimization_inlines.hpp"
#include "optimizer/Optimizations.hpp"
#include "optimizer/Optimizer.hpp"
#include "optimizer/Structure.hpp"
#include "optimizer/TransformUtil.hpp"
#include "optimizer/UseDefInfo.hpp"
#include "optimizer/ValueNumberInfo.hpp"
#include "optimizer/VPConstraint.hpp"
#include "optimizer/OMRValuePropagation.hpp"
#include "optimizer/LocalValuePropagation.hpp"
#include "ras/Debug.hpp"
#include "ras/DebugCounter.hpp"
#include "env/IO.hpp"

#ifdef J9_PROJECT_SPECIFIC
#include "env/VMJ9.h"
#include "optimizer/IdiomRecognitionUtils.hpp"
#endif

namespace TR { class CFGNode; }
namespace TR { class OptimizationManager; }

#define OPT_DETAILS "O^O VALUE PROPAGATION: "
#define NEED_WRITE_BARRIER 1
#define NEED_ARRAYSTORE_CHECK 2
#define NEED_ARRAYSTORE_CHECK_AND_WRITE_BARRIER 3
#define FORWARD_ARRAYCOPY  4
#define NEED_RUNTIME_TEST_FOR_SRC  8
#define NEED_RUNTIME_TEST_FOR_DST  16

#define MAX_RELATION_DEPTH 30

extern void collectArraylengthNodes(TR::Node *node, vcount_t visitCount, List<TR::Node> *arraylengthNodes);

// ***************************************************************************
//
// Methods of Value Propagation optimization
//
// ***************************************************************************

OMR::ValuePropagation::ValuePropagation(TR::OptimizationManager *manager)
   : TR::Optimization(manager),
     _parmValues(NULL),
     _currentParent(NULL),
     _arraylengthNodes(trMemory()),
     _javaLangClassGetComponentTypeCalls(trMemory()),
     _unknownTypeArrayCopyTrees(trMemory()),
     _scalarizedArrayCopies(trMemory()),
     _cachedStringBufferVcalls(trMemory()),
     _cachedStringPeepHolesVcalls(trMemory()),
     _predictedThrows(trMemory()),
     _prexClasses(trMemory()),
     _prexMethods(trMemory()),
     _prexClassesThatShouldNotBeNewlyExtended(trMemory()),
     _resetClassesThatShouldNotBeNewlyExtended(trMemory()),
     _referenceArrayCopyTrees(trMemory()),
     _needRunTimeCheckArrayCopy(trMemory()),
     _needMultiLeafArrayCopy(trMemory()),
     _arrayCopySpineCheck(trMemory()),
     _multiLeafCallsToInline(trMemory()),
     _converterCalls(trMemory()),
     _objectCloneCalls(trMemory()),
     _arrayCloneCalls(trMemory()),
     _objectCloneTypes(trMemory()),
     _arrayCloneTypes(trMemory()),
     _parmInfo(NULL),
     _parmTypeValid(NULL),
     _constNodeInfo(comp()->allocator())
   {
   // DANGER !!!
   // Virtual methods on ValuePropagation should only be called when the most derived class of VP's
   // constructor is completed for correct behavior.
   // setVP does not call any virtual method on VP, so it's ok to be here.
   _vcHandler.setVP(this);
   }




void OMR::ValuePropagation::initialize()
   {
   _resetClassesThatShouldNotBeNewlyExtended.deleteAll();
   _enableVersionBlocks       = false;
   _disableVersionBlockForThisBlock = true;

   _nullObjectConstraint        = new (trStackMemory()) TR::VPNullObject();
   _nonNullObjectConstraint     = new (trStackMemory()) TR::VPNonNullObject();
   _preexistentObjectConstraint = new (trStackMemory()) TR::VPPreexistentObject();
   _constantZeroConstraint      = new (trStackMemory()) TR::VPIntConst(0);
   _unreachablePathConstraint   = new (trStackMemory()) TR::VPUnreachablePath();


   _invalidateUseDefInfo      = false;
   _invalidateValueNumberInfo = false;
   _enableSimplifier          = false;
   _checksRemoved             = false;
   _usePreexistence           = false;
   _chTableWasValid           = true;
   _chTableValidityChecked    = false;
   _changedThis               = TR_maybe;

   _constraintsHashTable = (ConstraintsHashTableEntry**)trMemory()->allocateStackMemory(VP_HASH_TABLE_SIZE*sizeof(ConstraintsHashTableEntry*));
   memset(_constraintsHashTable, 0, VP_HASH_TABLE_SIZE*sizeof(ConstraintsHashTableEntry*));

   // determine if there are any invariant parms
   //
   int32_t numParms = comp()->getMethodSymbol()->getParameterList().getSize();
   _parmInfo = (int32_t *) trMemory()->allocateStackMemory(numParms * sizeof(int32_t));
   _parmTypeValid = (bool *) trMemory()->allocateStackMemory(numParms * sizeof(bool));
   // mark all parms as invariant (0) to begin with
   //
   memset(_parmInfo, 0, numParms * sizeof(int32_t));
   // mark info to be true
   for (int32_t i = 0; i < numParms; i++)
      _parmTypeValid[i] = true;

   for (TR::TreeTop *tt = comp()->getStartTree();
         tt;
         tt = tt->getNextTreeTop())
      {
      TR::Node *node = tt->getNode();
      if (node && node->getOpCodeValue() == TR::treetop)
         node = node->getFirstChild();

      if (node && node->getOpCode().isStoreDirect())
         {
         TR::Symbol *sym = node->getSymbolReference()->getSymbol();
         if (sym->isParm())
            {
            // parm is no longer invariant
            //
            int32_t index = sym->getParmSymbol()->getOrdinal();
            _parmInfo[index] = 1;
            }
         }
      }

   _arraylengthNodes.deleteAll();

   vcount_t visitCount = comp()->incVisitCount();
   TR::TreeTop *curTree = comp()->getStartTree();
   while (curTree)
      {
      if (curTree->getNode()) collectArraylengthNodes(curTree->getNode(), visitCount, &_arraylengthNodes);
      curTree = curTree->getNextTreeTop();
      }

   if (_isGlobalPropagation)
      {
      // Determine the size of the _globalConstraintsHashTable based on the number of nodes
      //
      uint32_t targetSize = (uint32_t)(comp()->getNodeCount() / 5); // that 5 was experimentally determined
      const uint32_t globalConstraintsHashTableSizeUpperBound = 2048; // must be power of two
      uint32_t globalConstraintsHashTableSize = 128; // starting value; must be power of two
      if (targetSize >= globalConstraintsHashTableSizeUpperBound) // test for upper bound
         globalConstraintsHashTableSize = globalConstraintsHashTableSizeUpperBound;
      else
         while (targetSize > globalConstraintsHashTableSize) // guaranteed to end
            globalConstraintsHashTableSize <<= 1;
      _globalConstraintsHTMaxBucketIndex = globalConstraintsHashTableSize - 1;

      _globalConstraintsHashTable = (GlobalConstraint**)trMemory()->allocateStackMemory(globalConstraintsHashTableSize*sizeof(GlobalConstraint*));
      memset(_globalConstraintsHashTable, 0, globalConstraintsHashTableSize*sizeof(GlobalConstraint*));

      _edgeConstraintsHashTable = (EdgeConstraints**)trMemory()->allocateStackMemory(VP_HASH_TABLE_SIZE*sizeof(EdgeConstraints*));
      memset(_edgeConstraintsHashTable, 0, VP_HASH_TABLE_SIZE*sizeof(EdgeConstraints*));

      _loopDefsHashTable = (LoopDefsHashTableEntry**)trMemory()->allocateStackMemory(VP_HASH_TABLE_SIZE*sizeof(LoopDefsHashTableEntry*));
      memset(_loopDefsHashTable, 0, VP_HASH_TABLE_SIZE*sizeof(LoopDefsHashTableEntry*));
      }
   else
      {
      _globalConstraintsHashTable = NULL;
      _edgeConstraintsHashTable = NULL;
      _loopDefsHashTable = NULL;
      }

   _visitCount = comp()->incVisitCount();

   _edgesToBeRemoved = new (trStackMemory()) TR_Array<TR::CFGEdge *>(trMemory(), 8, false, stackAlloc);
   _blocksToBeRemoved = new (trStackMemory()) TR_Array<TR::CFGNode*>(trMemory(), 8, false, stackAlloc);
   _curDefinedOnAllPaths = NULL;
   if (_isGlobalPropagation)
      _definedOnAllPaths = new (trStackMemory()) DefinedOnAllPathsMap(std::less<TR::CFGEdge *>(), trMemory()->currentStackRegion());
   else
      _definedOnAllPaths = NULL;
   _defMergedNodes = new (trStackMemory()) TR_BitVector(0, trMemory(), stackAlloc, growable);
   _vcHandler.setRoot(_curConstraints, NULL);

   _relationshipCache.setFirst(NULL);
   _storeRelationshipCache.setFirst(NULL);
   _valueConstraintCache = new (trStackMemory()) TR_Stack<ValueConstraint*>(trMemory(), 256, false, stackAlloc);
   _loopInfo = NULL;

   // unresolved symbols so we can track when they are known to be resolved.
   // After these extra value numbers we allocate value numbers to represent
   // induction variables.
   //
      {
      static const char *e = feGetEnv("TR_maxValueNumber");
      _firstUnresolvedSymbolValueNumber  = e ? atoi(e) : (comp()->getOption(TR_ProcessHugeMethods) ? 200000 : 100000);

      _syncValueNumber = _firstUnresolvedSymbolValueNumber - 1;
      _firstInductionVariableValueNumber = _firstUnresolvedSymbolValueNumber * 2;
      _numValueNumbers = _firstInductionVariableValueNumber;
      }

   // Enable preexistence if recompiling and this is not a profiling
   // compilation
   //
   static char *disablePREX     = feGetEnv("TR_disablePREX");
   static char *disablePREXinVP = feGetEnv("TR_disablePREXinVP");
   if (0 && !disablePREX && !disablePREXinVP &&
       comp()->getMethodHotness() >= warm &&
       comp()->couldBeRecompiled() &&
       !comp()->getRecompilationInfo()->isProfilingCompilation() &&
       !comp()->getOption(TR_DisableCHOpts))
      {
      enablePreexistence();
      }

   _reachedMaxRelationDepth = false;
   _propagationDepth = 0;
   static const char *pEnv = feGetEnv("TR_VPMaxRelDepth");
   _maxPropagationDepth = pEnv ? atoi(pEnv) : MAX_RELATION_DEPTH;
   if (comp()->getMethodHotness() > warm)
      _maxPropagationDepth = _maxPropagationDepth * 3;

   if (((comp()->getMethodHotness() > warm) || !comp()->isOptServer()) &&
       (!comp()->getOption(TR_DisableBlockVersioner)))
      {
      if (!comp()->getFlowGraph()->getStructure() &&
          (((comp()->mayHaveLoops() && optimizer()->getLastRun(OMR::loopVersioner)) || !comp()->mayHaveLoops()) &&
           optimizer()->getLastRun(OMR::basicBlockExtension)))
         {

         dumpOptDetails(comp(), "   (Doing structural analysis)\n");

         TR_SingleTimer myTimer;
         bool doTiming = comp()->getOption(TR_Timing);

         if (doTiming)
            {
            myTimer.initialize("structural analysis", trMemory());
            myTimer.startTiming(comp());
            }

         optimizer()->doStructuralAnalysis();

         if ( doTiming)
            {
            myTimer.stopTiming(comp());
            if (comp()->getOutFile() != NULL)
               {
               trfprintf(comp()->getOutFile(), "Time taken for %s = ", myTimer.title());
               trfprintf(comp()->getOutFile(), "%9.6f seconds\n", myTimer.secondsTaken());
               }
#ifdef OPT_TIMING
            statStructuralAnalysisTiming.update((double)myTimer.timeTaken()*1000.0/TR::Compiler->vm.getHighResClockResolution());
#endif
            }
         }

      if (comp()->getFlowGraph()->getStructure())
         {
         _enableVersionBlocks = true;
         _bndChecks = new (trStackMemory()) TR_ScratchList<TR::Node>(trMemory());
         //The bitVector is growable /bc when we devirtaulize calls we add symbol references for the call and the parameters.
         _seenDefinedSymbolReferences = new (trStackMemory()) TR_BitVector(comp()->getSymRefCount(), trMemory(), stackAlloc, growable);
         _blocksToBeVersioned = new (trStackMemory()) TR_LinkHead<BlockVersionInfo>();
         _firstLoads = new (trStackMemory()) TR_LinkHead<FirstLoadOfNonInvariant>();
         _startEBB = NULL;
         }
      }

     _unsafeArrayAccessNodes = new (trStackMemory()) TR_BitVector(comp()->getNodeCount(), trMemory(), stackAlloc, growable);
   }

OMR::ValuePropagation::Relationship *OMR::ValuePropagation::copyRelationships(Relationship *first)
   {
   TR_LinkHeadAndTail<Relationship> list;
   for (Relationship *rel = first; rel; rel = rel->getNext())
      {
      Relationship *newRel = createRelationship(rel->relative, rel->constraint);
      list.append(newRel);
      }
   return list.getFirst();
   }

OMR::ValuePropagation::StoreRelationship *OMR::ValuePropagation::copyStoreRelationships(StoreRelationship *first)
   {
   TR_LinkHeadAndTail<StoreRelationship> list;
   for (StoreRelationship *rel = first; rel; rel = rel->getNext())
      {
      StoreRelationship *newRel = createStoreRelationship(rel->symbol, copyRelationships(rel->relationships.getFirst()));
      list.append(newRel);
      }
   return list.getFirst();
   }

OMR::ValuePropagation::ValueConstraint *OMR::ValuePropagation::createValueConstraint(int32_t valueNumber, Relationship *relationships, StoreRelationship *storeRelationships)
   {
   ValueConstraint *vc;
   if (!_valueConstraintCache->isEmpty())
      vc = _valueConstraintCache->pop();
   else
      vc = new (trStackMemory()) ValueConstraint(valueNumber);
   vc->initialize(valueNumber, relationships, storeRelationships);
   return vc;
   }

void OMR::ValuePropagation::freeValueConstraint(ValueConstraint *vc)
   {
   freeRelationships(vc->relationships);
   freeStoreRelationships(vc->storeRelationships);
   _valueConstraintCache->push(vc);
   }

int32_t OMR::ValuePropagation::getValueNumber(TR::Node *node)
   {
   static const char *disableLocalVPConstNumbering = feGetEnv("TR_DisableLocalVPConstNumbering");
   if (_isGlobalPropagation)
      return _valueNumberInfo->getValueNumber(node);
   else
      {
      if (node->getOpCode().isStore())
         {
         if (node->getOpCode().isIndirect())
            return node->getSecondChild()->getGlobalIndex();
         else
            return node->getFirstChild()->getGlobalIndex();
         }
      else if (node->getOpCode().isLoadConst()
               && node->canGet64bitIntegralValue()
               && !disableLocalVPConstNumbering)
         {
         int64_t value = node->get64bitIntegralValue();
         CS2::HashIndex index;
         if (_constNodeInfo.Locate(value, index))
            {
            TR::list<TR::Node *>* nodeList = _constNodeInfo[index];
            TR_ASSERT(nodeList, "NodeList must be non-NULL");
            TR::Node *result = NULL;
            for(auto nodeCursor = nodeList->begin(); (result == NULL) && (nodeCursor != nodeList->end()); ++nodeCursor)
               {
               TR::Node *candidate = *nodeCursor;
               if (candidate->getDataType() == node->getDataType())
                  result = candidate;
               }
            if (!result)
               {
               nodeList->push_back(node);
               return node->getGlobalIndex();
               }
            return result->getGlobalIndex();
            }
         else
            {
            // we have not seen this constant before so setup a constant table entry
            // and use the current node's global index as the value number
            TR::list<TR::Node *>* nodeList = new (comp()->trHeapMemory()) TR::list<TR::Node*>(getTypedAllocator<TR::Node*>(comp()->allocator()));
            nodeList->push_back(node);
            _constNodeInfo.Add(value, nodeList);
            return node->getGlobalIndex();
            }
         }
      else
         return node->getGlobalIndex();
      }
   }

TR::VPConstraint *OMR::ValuePropagation::getConstraint(TR::Node *node, bool &isGlobal, TR::Node *relative)
   {
   /*
     If we return a non-null constraint, isGlobal will be set accordingly.
     If we return a null constraint, isGlobal defaults to true: it is safe to globally assume
     that we know nothing about this node.
    */
   isGlobal = true;

   // See if there is an existing constraint for this node
   //
   int32_t valueNumber = getValueNumber(node);
   int32_t relativeVN  = relative ? getValueNumber(relative) : AbsoluteConstraint;
   // Look for an existing local constraint. If we find it we are done.
   //
   TR::VPConstraint *constraint = NULL;
   Relationship *rel = findConstraint(valueNumber, relativeVN);
   if (rel)
      {
      if (trace())
         {
         traceMsg(comp(), "   %s [%p] has existing constraint:", node->getOpCode().getName(), node);
         rel->print(this, valueNumber, 1);
         }
      isGlobal = false;
      constraint = rel->constraint;
      }

   // If the node is a use node, look at its def points and merge constraints
   // from them.
   //
   else
      {
      constraint = mergeDefConstraints(node, relativeVN, isGlobal);
      }

   // If looking at the defs gave us a valid constraint, see if it can
   // be further constrained by global constraints, and then set it up as a
   // block or global constraint as appropriate.
   //
   if (constraint)
      {
      TR::VPConstraint *betterConstraint = applyGlobalConstraints(node, valueNumber, constraint, relativeVN);
      addBlockOrGlobalConstraint(node, betterConstraint, isGlobal, relative);
      return constraint;
      }

   // Look for an existing global constraint.
   //
   rel = findGlobalConstraint(valueNumber, relativeVN);
   if (rel)
      {
      if (trace())
         {
         traceMsg(comp(), "   %s [%p] has existing global constraint:", node->getOpCode().getName(), node);
         rel->print(this, valueNumber, 1);
         }
      isGlobal = true;
      constraint = rel->constraint;
      }

   return constraint;
   }

void OMR::ValuePropagation::removeNode(TR::Node *node, bool anchorIt)
   {
   // If the node has a reference count more than one, anchor it in a
   // treetop _preceeding_ the current treetop to avoid swing-down problems.
   //
   if (node->getReferenceCount() > 1)
      {
//       if (anchorIt &&
//           !node->getOpCode().isLoadConst() &&
//           _curTree->getNextTreeTop() != _curBlock->getExit())
//          TR::TreeTop::create(comp(), _curTree, TR::Node::create( TR::treetop, 1, node));
//       node->decReferenceCount();

      // The above is wrong, put it _before_ the current tree.  By definition, children
      // must be evaluated before the parent, so they belong before the parent tree.
      // Also, we have to do this even if the curtree is the last tree in the block, since the node may
      // be commoned in the extended basic block following
      //
      if (anchorIt && !node->getOpCode().isLoadConst())
         TR::TreeTop::create(comp(), _curTree->getPrevTreeTop(), TR::Node::create(TR::treetop, 1, node));
      node->decReferenceCount();
      }

   // Otherwise this is the one and only use of the node. Prepare it
   // for removal from the trees.
   //
   else
      {
      removeChildren(node, anchorIt);
      if (optimizer()->prepareForNodeRemoval(node, /* deferInvalidatingUseDefInfo = */ true))
         invalidateUseDefInfo();

      if (node->getOpCode().isCheck())
         setChecksRemoved();

      // Make sure the node is not a valid def node any more
      //
      node->setUseDefIndex(0);
      node->decReferenceCount();
      }
   }

void OMR::ValuePropagation::removeChildren(TR::Node *node, bool anchorIt)
   {
   for (int32_t i = node->getNumChildren()-1; i >= 0; i--)
      {
      removeNode(node->getChild(i), anchorIt);
      }
   node->setNumChildren(0);
   }



static bool canCauseOSR(TR::TreeTop *tt, TR::Compilation *comp)
   {
   if (comp->isPotentialOSRPointWithSupport(tt))
      return true;

   return false;
   }

void OMR::ValuePropagation::processTrees(TR::TreeTop *startTree, TR::TreeTop *endTree)
   {
   TR::TreeTop *treeTop;
   TR::TreeTop *lastRealTreeTop = _curBlock->getLastRealTreeTop();
   bool lastTtIsBndchk = false;
   bool nextBlockIsExtentionOfThis = false;
   int32_t checkIfNextBlockReachable = 1; // yes
   ValueConstraint *prevVC = NULL;
   if (_enableVersionBlocks && lastTimeThrough() &&
       (startTree->getNode()->getOpCodeValue() == TR::BBStart))
      {
      TR::Block *block = startTree->getNode()->getBlock();
      TR_BlockStructure *blockStructure = NULL;
      blockStructure = block->getStructureOf();
      _disableVersionBlockForThisBlock = false;

      if (!block->isExtensionOfPreviousBlock())
         _startEBB = block;

      if (lastRealTreeTop->getNode()->getOpCode().isBndCheck())
         lastTtIsBndchk = true;

      if ( block->isCatchBlock() ||
           (blockStructure && blockStructure->getContainingLoop() && !optimizer()->getLastRun(OMR::loopVersioner)) ||
           ( block->isCold()))
         _disableVersionBlockForThisBlock = true;


      TR::TreeTop *nextStartBlock = block->getExit()->getNextTreeTop();

      if (nextStartBlock && nextStartBlock->getNode()->getBlock()->isExtensionOfPreviousBlock())
         nextBlockIsExtentionOfThis = true;

      }

   for (treeTop = startTree; ((treeTop != endTree) && (treeTop != _curBlock->getExit())); treeTop = _curTree->getNextTreeTop())
      {
      _curTree = treeTop;
      TR::Node *treeTopNode = treeTop->getNode();
      if (trace())
         traceMsg(comp(), "Processing ttNode n%in %s\n", treeTopNode->getGlobalIndex(),
               treeTopNode->getOpCode().getName());

      if (_enableVersionBlocks && !_disableVersionBlockForThisBlock && treeTop == lastRealTreeTop &&
          !lastTtIsBndchk && lastTimeThrough())
         {
         if (!nextBlockIsExtentionOfThis)
            {
            createNewBlockInfoForVersioning(_startEBB);
            }
         else
            {
            // next block is an extension of this
            // block; unreachability is known only
            // after this tree is processed
            checkIfNextBlockReachable = -1; // maybe
            prevVC = copyValueConstraints(_curConstraints);
            }
         }


      TR::Node *oldTreeTopNode = treeTopNode;
      bool changedNode = false;
      static char *launchChild = feGetEnv("TR_enableLaunchFirstChild");
      if (launchChild && treeTopNode->getOpCodeValue() == TR::treetop)
         {
         setCurrentParent(treeTopNode);
         treeTopNode = treeTopNode->getFirstChild();
         changedNode = true;
         }

      if (canCauseOSR(treeTop, comp()))
         createExceptionEdgeConstraints(TR::Block::CanCatchOSR, NULL, treeTopNode);

      launchNode(treeTopNode, NULL, 0);

      if (changedNode)
         {
         setCurrentParent(oldTreeTopNode);
         }

      // check if the next block has become
      // unreachable; if so, allow block versioner
      // to collect info for this ebb below
      if (checkIfNextBlockReachable < 0)
         {
         if (isUnreachablePath(_curConstraints))
            checkIfNextBlockReachable = 0; // its unreachable
         }

      // launchNode may have changed _curTree so that processing is resumed
      // after the new _curTree value.

      // If the tree is to be removed, do that now
      //
      if (!treeTop->getNode())
         {
         if (_curTree == treeTop)
            _curTree = treeTop->getPrevTreeTop();
         TR::TransformUtil::removeTree(comp(), treeTop);
         }

      if (_reachedMaxRelationDepth)
         return;
      }

   if (_enableVersionBlocks && !_disableVersionBlockForThisBlock && lastTimeThrough())
      {
      if (lastTtIsBndchk && !nextBlockIsExtentionOfThis)
         {
         createNewBlockInfoForVersioning(_startEBB);
         }
      else if (checkIfNextBlockReachable == 0)
         {
         // temporarily zap the _curConstraints to
         // the previous value to allow block versioner
         // to collect info
         ValueConstraint *oldCurVC = copyValueConstraints(_curConstraints);
         _vcHandler.setRoot(_curConstraints, prevVC);
         createNewBlockInfoForVersioning(_startEBB);
         _vcHandler.setRoot(_curConstraints, oldCurVC);
         }
      }
   }

int32_t OMR::ValuePropagation::getPrimitiveArrayType(char primitiveArrayChar)
   {
   switch (primitiveArrayChar)
      {
      case 'B': return 8;
      case 'C': return 5;
      case 'D': return 7;
      case 'F': return 6;
      case 'I': return 10;
      case 'J': return 11;
      case 'S': return 9;
      case 'Z': return 4;
      default: return 1;
      }
   }

bool OMR::ValuePropagation::canTransformArrayCopyCallForSmall(TR::Node *node, int32_t &srcLength, int32_t &dstLength, int32_t &elementSize, TR::DataType &type )
   {
   TR::Node *srcArrayNode = node->getFirstChild();
   TR::Node *srcOffsetNode = node->getSecondChild();
   TR::Node *dstArrayNode = node->getChild(2);
   TR::Node *dstOffsetNode = node->getChild(3);
   TR::Node *lengthNode= node->getChild(4);

   int32_t srcSigLength, srcType;
   int32_t dstSigLength, dstType;
   static uint8_t primitiveArrayTypeToElementSize[] = {1, 2, 4, 8, 1, 2, 4, 8};
   static TR::DataType primitiveArrayToTRDataType[] = {TR::Int8, TR::Int16, TR::Float, TR::Double, TR::Int8, TR::Int16, TR::Int32, TR::Int64};

   const char *srcSig = srcArrayNode->getTypeSignature(srcSigLength);
   const char *dstSig = dstArrayNode->getTypeSignature(dstSigLength);

   // search for primitive arrays by looking at signature's
   // if signature is null then see if array was created in this method (newarray).

   if (srcSig && srcSigLength >= 2 && *srcSig == '[')
      {
      srcType = getPrimitiveArrayType(srcSig[1]);
      }
   else if (srcArrayNode->getOpCodeValue() == TR::newarray)
      {
      srcType = srcArrayNode->getSecondChild()->getInt();
      srcLength = srcArrayNode->getFirstChild()->getOpCode().isLoadConst() ? srcArrayNode->getFirstChild()->getInt() : -1;
      }
   else
      {
      srcType = -1;
      }

   if (dstSig && dstSigLength >= 2 && *dstSig == '[')
      {
      dstType = getPrimitiveArrayType(dstSig[1]);
      }
   else if (dstArrayNode->getOpCodeValue() == TR::newarray)
      {
      dstType = dstArrayNode->getSecondChild()->getInt();
      dstLength = dstArrayNode->getFirstChild()->getOpCode().isLoadConst() ? dstArrayNode->getFirstChild()->getInt() : -1;
      }
   else
      {
      dstType = -1;
      }

   // check if both arrays are primitive types and of the same type, if so then can transform the call for small (ie a primitive array copy)
   if (srcType >= 4 && dstType >=4 && dstType == srcType)
      {
      elementSize = primitiveArrayTypeToElementSize[srcType-4];
      type = primitiveArrayToTRDataType[srcType-4];
      return true;
      }
   else
      {
      return false;
      }
   }


#ifdef J9_PROJECT_SPECIFIC
static
TR_ResolvedMethod * findResolvedClassMethod(TR::Compilation * comp, char * className, char * methodName, char * methodSig)
   {
   TR_OpaqueClassBlock * classHandle = comp->fe()->getClassFromSignature(className, strlen(className), comp->getCurrentMethod());

   if (classHandle)
      {
      TR_ScratchList<TR_ResolvedMethod> classMethods(comp->trMemory());
      comp->fej9()->getResolvedMethods(comp->trMemory(), classHandle, &classMethods);

      ListIterator<TR_ResolvedMethod> it(&classMethods);
      TR_ResolvedMethod *method;
      int methodNameLen = strlen(methodName);
      int methodSigLen  = strlen(methodSig);
      for (method = it.getCurrent(); method; method = it.getNext())
         {
         if (!strncmp(method->nameChars(), methodName, methodNameLen) && !strncmp(method->signatureChars(), methodSig, methodSigLen))
            return method;
         }
      }
   return NULL;
   }
#endif


void OMR::ValuePropagation::removeArrayCopyNode(TR::TreeTop *arraycopyTree)
   {
   ListElement<TR_TreeTopWrtBarFlag> *elem = _unknownTypeArrayCopyTrees.getListHead();
   ListElement<TR_TreeTopWrtBarFlag> *prev = NULL;
   while (elem)
      {
      if (elem->getData()->_treetop == arraycopyTree)
         break;
      prev = elem;
      elem = elem->getNextElement();
      }

   if (elem)
      {
      if (prev)
         prev->setNextElement(elem->getNextElement());
      else
         _unknownTypeArrayCopyTrees.setListHead(elem->getNextElement());
      }

   // Reference arrays list
   //
   elem = _referenceArrayCopyTrees.getListHead();
   prev = NULL;

   while (elem)
      {
      if (elem->getData()->_treetop == arraycopyTree)
         break;
      prev = elem;
      elem = elem->getNextElement();
      }

   if (elem)
      {
      if (prev)
         prev->setNextElement(elem->getNextElement());
      else
         _referenceArrayCopyTrees.setListHead(elem->getNextElement());
      }

   // Array copy spine check list
   //
   ListElement<TR_ArrayCopySpineCheck> *acscElem = _arrayCopySpineCheck.getListHead();
   ListElement<TR_ArrayCopySpineCheck> *acscPrev = NULL;
   while (acscElem)
      {
      if (acscElem->getData()->_arraycopyTree == arraycopyTree)
         break;
      acscPrev = acscElem;
      acscElem = acscElem->getNextElement();
      }

   if (acscElem)
      {
      if (acscPrev)
         acscPrev->setNextElement(acscElem->getNextElement());
      else
         _arrayCopySpineCheck.setListHead(acscElem->getNextElement());
      }

   }
TR::Node * createHdrSizeNode(TR::Compilation *comp, TR::Node *n)
   {
   TR::Node *hdrSize = NULL;
   if (TR::Compiler->target.is64Bit())
      {
      hdrSize = TR::Node::create(n, TR::lconst);
      hdrSize->setLongInt((int64_t)TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }
   else
      hdrSize = TR::Node::create(n, TR::iconst, 0, (int32_t)TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

   return hdrSize;
   }

TR::Node* generateLenForArrayCopy(TR::Compilation *comp, int32_t elementSize, TR::Node *stride,TR::Node *srcObjNode,TR::Node *copyLenNode,TR::Node *n)
   {
   bool is64BitTarget = TR::Compiler->target.is64Bit() ? true : false;

   TR::Node *len = NULL;
   if (elementSize == 1)
      {
      len = copyLenNode->createLongIfNeeded();
      }
   else if (elementSize == 0)
      {
#ifdef J9_PROJECT_SPECIFIC
      if (!stride)
         stride = TR::TransformUtil::generateArrayElementShiftAmountTrees(comp, srcObjNode);
#endif

      if (is64BitTarget)
         {
         if (stride->getType().isInt32())
            stride = TR::Node::create(TR::i2l, 1, stride);

         if (copyLenNode->getType().isInt32())
            {
            TR::Node *i2lNode = TR::Node::create(TR::i2l, 1, copyLenNode);
            len = TR::Node::create(TR::lshl, 2, i2lNode, stride);
            }
         else
            len = TR::Node::create(TR::lshl, 2, copyLenNode, stride);
         }
      else
         len = TR::Node::create(TR::ishl, 2, copyLenNode, stride);
      }
   else
      {
      if (is64BitTarget)
         {
         if (!stride)
            {
            stride = TR::Node::create(n , TR::lconst);
            stride->setLongInt(elementSize);
            }
         else if (stride->getType().isInt32())
            stride = TR::Node::create(TR::i2l, 1, stride);

         if (copyLenNode->getType().isInt32())
            {
            TR::Node *i2lNode = TR::Node::create(TR::i2l, 1, copyLenNode);
            len = TR::Node::create(TR::lmul, 2, i2lNode, stride);
            }
         else
            len = TR::Node::create(TR::lmul, 2, copyLenNode, stride);
         }
      else
         {
         if (!stride)
            stride = TR::Node::create(n, TR::iconst, 0, elementSize);
         len = TR::Node::create(TR::imul, 2, copyLenNode, stride);
         }
      }

   return len;
   }


bool OMR::ValuePropagation::canRunTransformToArrayCopy()
   {
   if (!lastTimeThrough())
      return false;

   if ((comp()->getMethodHotness() >= hot) &&
       !_isGlobalPropagation &&
       !getLastRun())
      return false;

   return true;
   }

bool OMR::ValuePropagation::transformUnsafeCopyMemoryCall(TR::Node *arraycopyNode)
   {
   if (!canRunTransformToArrayCopy())
      return false;

   TR::TreeTop *tt = _curTree;
   TR::Node *ttNode = tt->getNode();

#ifdef J9_PROJECT_SPECIFIC
   if (comp()->canTransformUnsafeCopyToArrayCopy()
         && arraycopyNode->getOpCode().isCall()
         && arraycopyNode->getSymbol()->isMethod()
         && arraycopyNode->getSymbol()->castToMethodSymbol()->getRecognizedMethod() == TR::sun_misc_Unsafe_copyMemory)
      {

      if ((ttNode->getOpCodeValue() == TR::treetop || ttNode->getOpCode().isResolveOrNullCheck())
            && performTransformation(comp(), "%sChanging call Unsafe.copyMemory [%p] to arraycopy\n", OPT_DETAILS, arraycopyNode))

         {
         TR::Node *unsafe     = arraycopyNode->getChild(0);
         TR::Node *src        = arraycopyNode->getChild(1);
         TR::Node *srcOffset  = arraycopyNode->getChild(2);
         TR::Node *dest       = arraycopyNode->getChild(3);
         TR::Node *destOffset = arraycopyNode->getChild(4);
         TR::Node *len        = arraycopyNode->getChild(5);

         int64_t srcOffLow;
         int64_t srcOffHigh;
         int64_t dstOffLow;
         int64_t dstOffHigh;
         int64_t copyLenLow;
         int64_t copyLenHigh;

         bool isGlobal;
         TR::VPConstraint *srcOffsetConstraint = getConstraint(srcOffset, isGlobal);
         TR::VPConstraint *dstOffsetConstraint = getConstraint(destOffset, isGlobal);
         TR::VPConstraint *copyLenConstraint   = getConstraint(len, isGlobal);

         srcOffLow   = srcOffsetConstraint ? srcOffsetConstraint->getLowInt() : TR::getMinSigned<TR::Int32>();
         srcOffHigh  = srcOffsetConstraint ? srcOffsetConstraint->getHighInt() : TR::getMaxSigned<TR::Int32>();
         dstOffLow   = dstOffsetConstraint ? dstOffsetConstraint->getLowInt() : TR::getMinSigned<TR::Int32>();
         dstOffHigh  = dstOffsetConstraint ? dstOffsetConstraint->getHighInt() : TR::getMaxSigned<TR::Int32>();
         copyLenLow  = copyLenConstraint   ? copyLenConstraint->getLowInt() : TR::getMinSigned<TR::Int32>();
         copyLenHigh = copyLenConstraint   ? copyLenConstraint->getHighInt() : TR::getMaxSigned<TR::Int32>();

         if (TR::Compiler->target.is64Bit())
            {
            src  = TR::Node::create(TR::aladd, 2, src, srcOffset);
            dest = TR::Node::create(TR::aladd, 2, dest, destOffset);
            }
         else
            {
            srcOffset  = TR::Node::create(TR::l2i, 1, srcOffset);
            destOffset = TR::Node::create(TR::l2i, 1, destOffset);
            len        = TR::Node::create(TR::l2i, 1, len);
            src  = TR::Node::create(TR::aiadd, 2, src, srcOffset);
            dest = TR::Node::create(TR::aiadd, 2, dest, destOffset);
            }

         TR::Node    *oldArraycopyNode = arraycopyNode;
         TR::TreeTop *oldTT = tt;

         arraycopyNode = TR::Node::createArraycopy(src, dest, len);
         TR::Node    *treeTopNode = TR::Node::create(TR::treetop, 1, arraycopyNode);
         tt = TR::TreeTop::create(comp(), treeTopNode);

         oldTT->insertAfter(tt);

         if (ttNode->getOpCode().isNullCheck())
            ttNode->setAndIncChild(0, TR::Node::create(TR::PassThrough, 1, unsafe));
         else
            ttNode->setAndIncChild(0, unsafe);

         removeNode(oldArraycopyNode);

         if ((srcOffLow >= dstOffHigh) || (srcOffHigh+copyLenHigh) <= dstOffLow)
            arraycopyNode->setForwardArrayCopy(true);

         return true;
         }
      }
#endif
   return false;

   }

void OMR::ValuePropagation::transformArrayCopyCall(TR::Node *node)
   {
   bool is64BitTarget = TR::Compiler->target.is64Bit();

   // Check to see if this is a call to java/lang/System.arraycopy
   if (!node->isArrayCopyCall())
      return;

   if (!canRunTransformToArrayCopy())
      return;

   // Sanity check
   TR_ASSERT(node->getNumChildren() == 5, "Wrong number of arguments for arraycopy.");

   TR::Node *srcObjNode = node->getFirstChild();
   TR::Node *srcOffNode = node->getSecondChild();
   TR::Node *dstObjNode = node->getChild(2);
   TR::Node *dstOffNode = node->getChild(3);
   TR::Node *copyLenNode= node->getChild(4);

   int32_t srcOffLow;
   int32_t srcOffHigh;
   int32_t dstOffLow;
   int32_t dstOffHigh;
   int32_t copyLenLow;
   int32_t copyLenHigh;

   int32_t srcLength = -1;
   int32_t dstLength = -1;
   int32_t elementSize = 0;
   int32_t arraySpineShift = 0;
   TR::DataType type = TR::NoType;

   bool transformTheCall = false;
   bool primitiveArray1 = false;
   bool referenceArray1 = false;
   bool primitiveArray2 = false;
   bool referenceArray2 = false;
   bool needArrayCheck = true;
   bool needArrayStoreCheck = true;
   bool needWriteBarrier = true;
   bool needReadBarrier = TR::Compiler->om.readBarrierType() != gc_modron_readbar_none;

   bool primitiveTransform = cg()->getSupportsPrimitiveArrayCopy();
   bool referenceTransform = cg()->getSupportsReferenceArrayCopy();

   bool isSrcPossiblyNull = true; // pessimistic
   bool isDstPossiblyNull = true;
   bool needSameLeafCheckForSrc = false;
   bool needSameLeafCheckForDst = false;
   bool isMultiLeafArrayCopy = false;
   bool isRecognizedMultiLeafArrayCopy = false;

   if (trace() && comp()->generateArraylets())
      traceMsg(comp(), "Detected arraylet arraycopy: %p\n", node);

   bool isGlobal;
   TR::VPConstraint *srcObject = getConstraint(srcObjNode, isGlobal);
   TR::VPConstraint *srcOffset = getConstraint(srcOffNode, isGlobal);
   TR::VPConstraint *dstObject = getConstraint(dstObjNode, isGlobal);
   TR::VPConstraint *dstOffset = getConstraint(dstOffNode, isGlobal);
   TR::VPConstraint *copyLen   = getConstraint(copyLenNode, isGlobal);

   int32_t srcVN = getValueNumber(srcObjNode);
   int32_t dstVN = getValueNumber(dstObjNode);

   if (srcVN == dstVN)
      {
      needArrayStoreCheck = false;
      switch (TR::Compiler->om.writeBarrierType())
         {
         case gc_modron_wrtbar_cardmark:
         case gc_modron_wrtbar_cardmark_and_oldcheck:
         case gc_modron_wrtbar_cardmark_incremental:
            break;
         default:
            needWriteBarrier = false;
            break;
         }
      }

   switch (TR::Compiler->om.writeBarrierType())
      {
      case gc_modron_wrtbar_oldcheck:
      case gc_modron_wrtbar_cardmark:
      case gc_modron_wrtbar_cardmark_and_oldcheck:
      case gc_modron_wrtbar_cardmark_incremental:
         break;
      default:
         needWriteBarrier = false;
         break;
      }

   TR::VPArrayInfo *srcArrayInfo;
   TR::VPArrayInfo *dstArrayInfo;

   srcOffLow = srcOffset ? srcOffset->getLowInt() : TR::getMinSigned<TR::Int32>();
   srcOffHigh = srcOffset ? srcOffset->getHighInt() : TR::getMaxSigned<TR::Int32>();
   dstOffLow = dstOffset ? dstOffset->getLowInt() : TR::getMinSigned<TR::Int32>();
   dstOffHigh = dstOffset ? dstOffset->getHighInt() : TR::getMaxSigned<TR::Int32>();
   copyLenLow = copyLen ? copyLen->getLowInt() : TR::getMinSigned<TR::Int32>();
   copyLenHigh = copyLen ? copyLen->getHighInt() : TR::getMaxSigned<TR::Int32>();

   // If the call must fail, don't transform it.  The rest of the block can be
   // removed.
   //
   if ((srcObject && srcObject->isNullObject())  ||
       (dstObject && dstObject->isNullObject())  ||
       srcOffHigh < 0 || dstOffHigh < 0 || copyLenHigh < 0 ||
       (srcObject && srcObject->getClassType() && srcObject->getClassType()->asFixedClass() && (srcObject->getClassType()->asFixedClass()->isArray() == TR_no)) ||
       (dstObject && dstObject->getClassType() && dstObject->getClassType()->asFixedClass() && (dstObject->getClassType()->asFixedClass()->isArray() == TR_no)) )
      {
      createExceptionEdgeConstraints(TR::Block::CanCatchUserThrows, NULL, node);
      mustTakeException();
      return;
      }

   srcArrayInfo = srcObject ? srcObject->getArrayInfo() : NULL;
   dstArrayInfo = dstObject ? dstObject->getArrayInfo() : NULL;

   TR::RecognizedMethod recognizedMethod = node->getSymbol()->castToMethodSymbol()->getRecognizedMethod();

#ifdef J9_PROJECT_SPECIFIC
   bool isStringCompressedArrayCopy =
         recognizedMethod == TR::java_lang_String_compressedArrayCopy_BIBII ||
         recognizedMethod == TR::java_lang_String_compressedArrayCopy_BICII ||
         recognizedMethod == TR::java_lang_String_compressedArrayCopy_CIBII ||
         recognizedMethod == TR::java_lang_String_compressedArrayCopy_CICII;

   bool isStringDecompressedArrayCopy =
         recognizedMethod == TR::java_lang_String_decompressedArrayCopy_BIBII ||
         recognizedMethod == TR::java_lang_String_decompressedArrayCopy_BICII ||
         recognizedMethod == TR::java_lang_String_decompressedArrayCopy_CIBII ||
         recognizedMethod == TR::java_lang_String_decompressedArrayCopy_CICII;

   // If it is OK to convert this call to a possible call to the (fast)
   // arraycopy helper, do it.
   //
   if ((primitiveTransform || referenceTransform) &&
       !comp()->getOption(TR_DisableArrayCopyOpts) &&
       !node->isDontTransformArrayCopyCall() &&
        comp()->fej9()->callTheJitsArrayCopyHelper() &&
        !comp()->getOption(TR_DisableInliningOfNatives) &&
        !(srcObject && srcObject->isNullObject()) &&
        !(dstObject && dstObject->isNullObject()) &&
        srcOffHigh >= 0 && dstOffHigh >= 0 && copyLenHigh >= 0)
      {
      transformTheCall = primitiveTransform && referenceTransform;

      if (srcObject && srcObject->getClassType())
         {
         primitiveArray1 = srcObject->getClassType()->isPrimitiveArray(comp());
         referenceArray1 = srcObject->getClassType()->isReferenceArray(comp());
         }

      if (dstObject && dstObject->getClassType())
         {
         primitiveArray2 = dstObject->getClassType()->isPrimitiveArray(comp());
         referenceArray2 = dstObject->getClassType()->isReferenceArray(comp());
         }

      if (primitiveArray1 || primitiveArray2)
         {
         transformTheCall = primitiveTransform;
         }

      if (referenceArray1 || referenceArray2)
         {
         transformTheCall = referenceTransform;
         }

      if (comp()->generateArraylets() &&
          !comp()->getOption(TR_DisableMultiLeafArrayCopy))
         {
         TR_ResolvedMethod *caller = node->getSymbolReference()->getOwningMethod(comp());
         char *sig = "multiLeafArrayCopy";
         if (caller && strncmp(caller->nameChars(), sig, strlen(sig)) == 0)
            {
            if (trace())
               traceMsg(comp(), "Detected real-time same leaf arraycopy: %p from java helper\n", node);

            isRecognizedMultiLeafArrayCopy = true;
            }
         }

      if (!isRecognizedMultiLeafArrayCopy)
         {
         // For compressed array copies we pretend the array types are equivalent
         if (isStringCompressedArrayCopy || isStringDecompressedArrayCopy)
            {
            needArrayCheck = false;
            }
         else if (primitiveArray1)
            {
            if (primitiveArray2)
               {
               if (srcObject->getClassType() == dstObject->getClassType() ||
                  (srcObject->getClassType() && dstObject->getClassType() &&
                   srcObject->getClassType()->asResolvedClass() &&
                   dstObject->getClassType()->asResolvedClass() &&
                   srcObject->getClassType()->asResolvedClass()->getClass() == dstObject->getClassType()->asResolvedClass()->getClass()))
                  {
                  needArrayCheck = false;
                  }
               else
                  {
                  // Array types are different types so the arraycopy will fail
                  transformTheCall = false;
                  }
               }
            else if (referenceArray2)
               {
               // Array types are different types so the arraycopy will fail
               transformTheCall = false;
               }
            }
         else if (referenceArray1)
            {
            if (referenceArray2)
               {
               if (dstObject && dstObject->getClass())
                  {
                  int32_t len;
                  const char *sig = dstObject->getClassSignature(len);
                  if (sig && sig[0] == '[')
                     {
                     // If the array is known to be a fixed array of Object,
                     // the check can be removed
                     // TODO -  get a pointer to the Object class from somewhere and
                     // compare object pointers instead of signatures
                     //
                     if (len == 19 && dstObject->isFixedClass() &&
                         !strncmp(sig, "[Ljava/lang/Object;", 19))
                        {
                        needArrayStoreCheck = false;
                        }
                     // If the object's class is resolved too, see if we can prove the
                     // check will succeed.
                     //
                     else if (srcObject && srcObject->getClass())
                        {
                        TR_YesNoMaybe isInstance = TR_maybe;
                        isInstance = fe()->isInstanceOf(srcObject->getClass(), dstObject->getClass(), srcObject->isFixedClass(), dstObject->isFixedClass());

                        if (isInstance == TR_yes)
                           needArrayStoreCheck = false;
                        }
                     }
                  }

               needArrayCheck = false;
               }
            else if (primitiveArray2)
               {
               // Array types are different types so the arraycopy will fail
               transformTheCall = false;
               }
            else if (TR::Compiler->om.writeBarrierType() == gc_modron_wrtbar_always)
               {
               transformTheCall = false;
               }
            }
         }

      if (primitiveArray1 || primitiveArray2)
         {
         type = primitiveArray1 ?
            srcObject->getClassType()->getPrimitiveArrayDataType() :
            dstObject->getClassType()->getPrimitiveArrayDataType();

         elementSize = TR::Symbol::convertTypeToSize(type);
         }

      if (referenceArray1 || referenceArray2)
         {
         type = TR::Address;

         elementSize = TR::Compiler->om.sizeofReferenceField();
         }

      if (isStringCompressedArrayCopy)
         {
         type = TR::Int8;

         elementSize = TR::Symbol::convertTypeToSize(type);

         // VP may not know anything about the types of the objects we are copying, hence primitiveArray1 and
         // and primitiveArray2 would both return false. However because we are dealing with a recognized method we
         // know that the types must be primitive. As such we we can safely set the following two variables without
         // repercussions.
         primitiveArray1 = true;
         primitiveArray2 = true;
         }

      if (isStringDecompressedArrayCopy)
         {
         type = TR::Int16;

         elementSize = TR::Symbol::convertTypeToSize(type);

         // VP may not know anything about the types of the objects we are copying, hence primitiveArray1 and
         // and primitiveArray2 would both return false. However because we are dealing with a recognized method we
         // know that the types must be primitive. As such we we can safely set the following two variables without
         // repercussions.
         primitiveArray1 = true;
         primitiveArray2 = true;
         }

      if (comp()->getOptions()->realTimeGC() &&
          comp()->requiresSpineChecks() &&
          (referenceArray1 || referenceArray2 || !(primitiveArray1 || primitiveArray2)))
         transformTheCall = false;


      if (transformTheCall && comp()->generateArraylets())
         {
         if (!( (primitiveArray1 || primitiveArray2) ||
                ( !comp()->getOption(TR_DisableRefArraycopyRT) && (referenceArray1 || referenceArray2))))
            {
            transformTheCall = false;
            }
         else
            {
            //printf("primitiveArray1=%d, primitiveArray2=%d, referenceArray1=%d , referenceArray2=%d\n",primitiveArray1,primitiveArray2,referenceArray1,referenceArray2);
            arraySpineShift = fe()->getArraySpineShift(elementSize);
            bool sameLeafSrc = false;
            bool sameLeafDst = false;

            //finding if on the same leaf
            if ((srcOffLow<0) || (srcOffHigh > TR::getMaxSigned<TR::Int32>() - copyLenHigh))
               sameLeafSrc = false;
            else if (fe()->getArrayletLeafIndex(srcOffLow,elementSize) == fe()->getArrayletLeafIndex(srcOffHigh+copyLenHigh,elementSize))
               sameLeafSrc = true;

            if ((dstOffLow<0) || (dstOffHigh > TR::getMaxSigned<TR::Int32>() - copyLenHigh))
               sameLeafDst = false;
            else if (fe()->getArrayletLeafIndex(dstOffLow,elementSize) == fe()->getArrayletLeafIndex(dstOffHigh+copyLenHigh,elementSize))
               sameLeafDst = true;

            if (!sameLeafSrc)
               {
               if ((copyLenNode->getOpCode().isLoadConst() &&
                    srcOffNode->getOpCode().isLoadConst()) &&
                   !isRecognizedMultiLeafArrayCopy)
                  {
                  if (!comp()->getOption(TR_DisableMultiLeafArrayCopy))
                     isMultiLeafArrayCopy = true;
                  else
                     transformTheCall = false;
                  }
               else if (!isRecognizedMultiLeafArrayCopy)
                  needSameLeafCheckForSrc = true;
               }

            if (!sameLeafDst)
               {
               if ((copyLenNode->getOpCode().isLoadConst() &&
                    dstOffNode->getOpCode().isLoadConst()) &&
                   !isRecognizedMultiLeafArrayCopy)
                  {
                  if (!comp()->getOption(TR_DisableMultiLeafArrayCopy))
                     isMultiLeafArrayCopy = true;
                  else
                     transformTheCall = false;
                  }
               else if (!isRecognizedMultiLeafArrayCopy)
                  needSameLeafCheckForDst = true;
               }
            }
         }
      }
#else
   bool isStringCompressedArrayCopy = false;
   bool isStringDecompressedArrayCopy = false;
#endif

   if (transformTheCall && performTransformation(comp(), "%sChanging call %s [%p] to arraycopy\n", OPT_DETAILS, node->getOpCode().getName(), node))
      {
      TR::ResolvedMethodSymbol* methodSymbol = comp()->getMethodSymbol();

      bool canSkipAllChecksOnArrayCopy = methodSymbol->safeToSkipChecksOnArrayCopies() || isRecognizedMultiLeafArrayCopy || isStringCompressedArrayCopy || isStringDecompressedArrayCopy;

      if (!canSkipAllChecksOnArrayCopy)
         {
         canSkipAllChecksOnArrayCopy = node->isNodeRecognizedArrayCopyCall();
         node->setNodeIsRecognizedArrayCopyCall(false);
         }

      if (comp()->getMethodHotness() == hot &&
            comp()->getRecompilationInfo() &&
            optimizer()->switchToProfiling())
         {
         dumpOptDetails(comp(), "%s enabling profiling: it is useful to profile arraycopy calls\n", OPT_DETAILS);
         }

      bool zeroLengthCopy = (copyLen == _constantZeroConstraint);

      TR::TreeTop *arraycopyTree = _curTree;
      TR::TreeTop *insertAfter = _curTree->getPrevTreeTop();
      TR::TreeTop *prevTree = insertAfter;
      TR::Node *srcArrayLength = NULL;
      TR::Node *dstArrayLength = NULL;
      TR::Node *tempNode;
      TR::Node *src = NULL, *dst = NULL, *len = NULL;

      bool isStringFwdArrayCopy = false;

      if (!canSkipAllChecksOnArrayCopy)
         {
         // -------------------------------------------------------------------
         // Null check the source and destination object references
         // -------------------------------------------------------------------

         isSrcPossiblyNull = !(srcObject && srcObject->isNonNullObject());
         isDstPossiblyNull = !(dstObject && dstObject->isNonNullObject());

         // The String.value char array should never alias with the destination or source
         // array.  The String.value array is also never null.
         //
#ifdef J9_PROJECT_SPECIFIC
         if (srcObjNode->getOpCode().hasSymbolReference() &&
               srcObjNode->getSymbolReference()->getSymbol() &&
               srcObjNode->getSymbolReference()->getSymbol()->getRecognizedField() == TR::Symbol::Java_lang_String_value)
            {
            isSrcPossiblyNull = false;
            isStringFwdArrayCopy = true;
            }

         if (dstObjNode->getOpCode().hasSymbolReference() &&
               dstObjNode->getSymbolReference()->getSymbol() &&
               dstObjNode->getSymbolReference()->getSymbol()->getRecognizedField() == TR::Symbol::Java_lang_String_value)
            {
            isDstPossiblyNull = false;
            isStringFwdArrayCopy = true;
            }
#endif

         if (isSrcPossiblyNull)
            {
            srcArrayLength = TR::Node::create(TR::arraylength, 1, srcObjNode);
            srcArrayLength->setIsNonNegative(true);
            prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createWithSymRef(TR::NULLCHK, 1, 1, srcArrayLength, comp()->getSymRefTab()->findOrCreateNullCheckSymbolRef(comp()->getMethodSymbol())));
            }

         if (isDstPossiblyNull)
            {
            dstArrayLength = TR::Node::create(TR::arraylength, 1, dstObjNode);
            dstArrayLength->setIsNonNegative(true);
            prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createWithSymRef(TR::NULLCHK, 1, 1, dstArrayLength, comp()->getSymRefTab()->findOrCreateNullCheckSymbolRef(comp()->getMethodSymbol())));
            }

         // -------------------------------------------------------------------
         // Check the array types for compatibility
         // -------------------------------------------------------------------

         if (needArrayCheck)
            {
            tempNode = TR::Node::createWithSymRef(TR::ArrayCHK, 2, 2, srcObjNode, dstObjNode, comp()->getSymRefTab()->findOrCreateArrayStoreExceptionSymbolRef(comp()->getMethodSymbol()));
            tempNode->setArrayChkPrimitiveArray1(primitiveArray1);
            tempNode->setArrayChkReferenceArray1(referenceArray1);
            tempNode->setArrayChkPrimitiveArray2(primitiveArray2);
            tempNode->setArrayChkReferenceArray2(referenceArray2);
            prevTree = TR::TreeTop::create(comp(), prevTree, tempNode);
            }

         // -------------------------------------------------------------------
         // Create up to 5 ArrayCopyBNDCHK nodes:
         //    copyLen >= 0
         //    srcOffset >= 0
         //    dstOffset >= 0
         //    src.arraylength >= srcOffset + copyLen
         //    dst.arraylength >= dstOffset + copyLen
         // -------------------------------------------------------------------

         TR::SymbolReference *bndChkSymRef = comp()->getSymRefTab()->findOrCreateArrayBoundsCheckSymbolRef(comp()->getMethodSymbol());

         TR::Node *checkNode;
         TR::Node *zeroConst = NULL;
         if (copyLenLow < 0)
            {
            if (!zeroConst)
               zeroConst = TR::Node::create(node, TR::iconst, 0, 0);
            checkNode = TR::Node::create(TR::ArrayCopyBNDCHK, 2, copyLenNode, zeroConst);
            checkNode->setSymbolReference(bndChkSymRef);
            prevTree = TR::TreeTop::create(comp(), prevTree, checkNode);
            setEnableSimplifier();
            }

         if (srcOffLow < 0)
            {
            if (!zeroConst)
               zeroConst = TR::Node::create(node, TR::iconst, 0, 0);
            checkNode = TR::Node::create(TR::ArrayCopyBNDCHK, 2, srcOffNode, zeroConst);
            checkNode->setSymbolReference(bndChkSymRef);
            prevTree = TR::TreeTop::create(comp(), prevTree, checkNode);
            setEnableSimplifier();
            }

         if (dstOffLow < 0)
            {
            if (!zeroConst)
               zeroConst = TR::Node::create(node, TR::iconst, 0, 0);
            checkNode = TR::Node::create(TR::ArrayCopyBNDCHK, 2, dstOffNode, zeroConst);
            checkNode->setSymbolReference(bndChkSymRef);
            prevTree = TR::TreeTop::create(comp(), prevTree, checkNode);
            setEnableSimplifier();
            }

         bool needSrcBndChk = !srcArrayInfo
            || srcOffHigh > TR::getMaxSigned<TR::Int32>() - copyLenHigh
            || srcOffHigh + copyLenHigh > srcArrayInfo->lowBound();

         bool needDstBndChk = !dstArrayInfo
            || dstOffHigh > TR::getMaxSigned<TR::Int32>() - copyLenHigh
            || dstOffHigh + copyLenHigh > dstArrayInfo->lowBound();

         if (needSrcBndChk)
            {
            if (!srcArrayLength)
               {
               srcArrayLength = TR::Node::create(TR::arraylength, 1, srcObjNode);
               srcArrayLength->setIsNonNegative(true);
               }
            checkNode = TR::Node::create(TR::isub, 2, srcArrayLength, copyLenNode);
            checkNode = TR::Node::create(TR::ArrayCopyBNDCHK, 2, checkNode, srcOffNode);
            checkNode ->setSymbolReference(bndChkSymRef);
            prevTree = TR::TreeTop::create(comp(), prevTree, checkNode);
            setEnableSimplifier();
            }

         if (needDstBndChk)
            {
            if (!dstArrayLength)
               {
               dstArrayLength = TR::Node::create(TR::arraylength, 1, dstObjNode);
               dstArrayLength->setIsNonNegative(true);
               }
            checkNode = TR::Node::create(TR::isub, 2, dstArrayLength, copyLenNode);
            checkNode = TR::Node::create(TR::ArrayCopyBNDCHK, 2, checkNode, dstOffNode);
            checkNode ->setSymbolReference(bndChkSymRef);
            prevTree = TR::TreeTop::create(comp(), prevTree, checkNode);
            setEnableSimplifier();
            }
         }

      if (srcArrayLength)
         {
         int32_t stride = 0;

         if (referenceArray1)
            stride = TR::Compiler->om.sizeofReferenceField();
         else if (srcArrayInfo)
            stride = srcArrayInfo->elementSize();

         if (stride != 0)
            srcArrayLength->setArrayStride(stride);
         }

      if (dstArrayLength)
         {
         int32_t stride = 0;

         if (referenceArray2)
            stride = TR::Compiler->om.sizeofReferenceField();
         else if (dstArrayInfo)
            stride = dstArrayInfo->elementSize();

         if (stride != 0)
            dstArrayLength->setArrayStride(stride);
         }

      // -------------------------------------------------------------------
      // Process the newly-inserted trees
      // -------------------------------------------------------------------

      processTrees(insertAfter->getNextTreeTop(), arraycopyTree);

      // If the arraycopy was removed from the trees (can happen if one of the newly
      // inserted check trees was proven to always fail), we should not continue
      // as we may affect the reference counts of srcObjNode etc. (the removed arraycopy
      // could be a parent of a node still in the trees, see code below that uses srcObjNode etc.)
      //
      if (node->getReferenceCount() < 1)
         {
         return;
         }

      _curTree = arraycopyTree;

      // If the length to be copied is zero, remove the arraycopy itself
      //
      if (zeroLengthCopy && performTransformation(comp(), "%sRemoving zero length call %s [%p]\n", OPT_DETAILS, node->getOpCode().getName(), node))
         {
         removeArrayCopyNode(arraycopyTree);
         removeNode(node);
         arraycopyTree->setNode(NULL);
         return;
         }

      // -------------------------------------------------------------------
      // The arraycopy transformation will proceed assuming the array is
      // contiguous.
      // -------------------------------------------------------------------

      if (comp()->requiresSpineChecks())
         {

         // TODO: skip spine check if copy is provably contiguous

         _arrayCopySpineCheck.add(new (trStackMemory())
            TR_ArrayCopySpineCheck(
               arraycopyTree,
               node->getSymbolReference(),
               srcObjNode,
               srcOffNode,
               dstObjNode,
               dstOffNode,
               copyLenNode));
         }

      if (isMultiLeafArrayCopy)
         {
         uint8_t flag = 0;

         _needMultiLeafArrayCopy.add(
               new (trStackMemory()) TR_RealTimeArrayCopy(arraycopyTree, type, flag));

         goto createExceptionEdgeConstraintsAndReturn;
         }
      else if (((needSameLeafCheckForSrc || needSameLeafCheckForDst) ||
               (comp()->generateArraylets() && comp()->getOptions()->realTimeGC() &&
                !comp()->getOption(TR_DisableRefArraycopyRT) &&
                (referenceArray1 || referenceArray2 ))))
         {
         uint8_t flag = 0;
         if (needSameLeafCheckForSrc)
            flag |= NEED_RUNTIME_TEST_FOR_SRC;

         if (needSameLeafCheckForDst)
            flag |= NEED_RUNTIME_TEST_FOR_DST;

         if (needArrayStoreCheck)
            flag |= NEED_ARRAYSTORE_CHECK;

         if (needWriteBarrier)
            flag |= NEED_WRITE_BARRIER;

         bool isForwardArrayCopy = false;
         if (((dstOffHigh == 0) ||
                  (srcOffLow >= dstOffHigh) ||
                  ((srcOffHigh <= TR::getMaxSigned<TR::Int32>() - copyLenHigh) && ((srcOffHigh+copyLenHigh) <= dstOffLow))) ||
               isStringFwdArrayCopy)
            flag |= FORWARD_ARRAYCOPY;

         _needRunTimeCheckArrayCopy.add(new (trStackMemory()) TR_RealTimeArrayCopy(arraycopyTree,type,flag));

         goto createExceptionEdgeConstraintsAndReturn;
         }

      // Change the call node into an arraycopy. There are two forms of this node:
      // When it is known that a simple byte copy can be done there are 3
      // children (with a 4th hidden child that gives the element datatype)
      //    1) the source element address
      //    2) the destination element address
      //    3) the length in bytes
      // When it is possible that an element-by-element copy must be done there
      // are 5 children:
      //    1) the original source object reference
      //    2) the original destination object reference
      //    3) the source element address
      //    4) the destination element address
      //    5) the length in bytes
      //
      // The node can be told:
      //    whether the copy is non-degenerate (i.e. the length is known to be
      //       greater than 0)
      //    whether the copy must be a forward copy
      //    whether the arrays are known to be reference arrays
      //

      TR::Node* hdrSize = createHdrSizeNode(comp(), node);

      TR::Node *stride = NULL;

      node->setNodeIsRecognizedArrayCopyCall(false); // flag conflicts with isForwardArrayCopy
      TR::Node::recreate(node, TR::arraycopy);

      if (!comp()->generateArraylets())
         {
         // -------------------------------------------------------------------
         // Calculate source address node
         // -------------------------------------------------------------------

         src = generateArrayAddressTree(comp(), node, srcOffHigh, srcOffNode, srcObjNode, elementSize, stride, hdrSize);

         // -------------------------------------------------------------------
         // Calculate destination address node
         // -------------------------------------------------------------------

         dst = generateArrayAddressTree(comp(), node, dstOffHigh, dstOffNode, dstObjNode, elementSize, stride, hdrSize);
         }
      else
         {
         //aiadd
         //   iaload
         //       aiadd
         //          aload a
         //          iadd
         //             ishl
         //                 ishr
         //                    i
         //                    spineShift
         //                 shift
         //             hdrsize
         //    ishl
         //       iand
         //          iconst mask
         //          i
         //       iconst strideShift
         //

         int32_t shift = TR::TransformUtil::convertWidthToShift(TR::Compiler->om.sizeofReferenceField());
         TR::Node *shiftNode = TR::Node::create(node, TR::iconst, 0, (int32_t)shift);

         int32_t strideShift = TR::TransformUtil::convertWidthToShift(elementSize);
         TR::Node *strideShiftNode = strideShift ? TR::Node::create(node, TR::iconst, 0, (int32_t)strideShift) : NULL;

         TR::Node *srcOff = srcOffNode->createLongIfNeeded();
         TR::Node *dstOff = dstOffNode->createLongIfNeeded();

         TR::Node *spineShiftNode = TR::Node::create(node, TR::iconst, 0, (int32_t)arraySpineShift);

         // -------------------------------------------------------------------
         // Calculate source address node
         // -------------------------------------------------------------------

         src = generateArrayletAddressTree(comp(), node, type, srcOff, srcObjNode, spineShiftNode, shiftNode, strideShiftNode, hdrSize);

         // -------------------------------------------------------------------
         // Calculate destination address node
         // -------------------------------------------------------------------

         dst = generateArrayletAddressTree(comp(), node, type , dstOff, dstObjNode, spineShiftNode, shiftNode, strideShiftNode, hdrSize);

         if (comp()->useCompressedPointers())
            {
            TR_ASSERT((src->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the src arraylet pointer");
            TR_ASSERT((dst->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the dst arraylet pointer");

            prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(src->getFirstChild()));
            prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dst->getFirstChild()));
            }
         }

      // -------------------------------------------------------------------
      // Arraycopy length node
      // -------------------------------------------------------------------

      len = generateLenForArrayCopy(comp(), elementSize, stride, srcObjNode, copyLenNode, node);

      if (primitiveArray1 || primitiveArray2)
         {
         // Must be a simple byte copy.
         //

         //removeChildren might setNumChildren on src,dst,len if reference count reaches 0
         src->incReferenceCount();
         dst->incReferenceCount();
         len->incReferenceCount();

         removeChildren(node);

         node->setChild(0, src);
         node->setChild(1, dst);
         node->setChild(2, len);
         node->setChild(3, NULL);
         node->setChild(4, NULL);
         node->setNumChildren(3);
         node->setArrayCopyElementType(type);
         }
      else
         {
         // Not known to be a simple byte copy
         //

         //removeChildren might setNumChildren on src,dst,len if reference count reaches 0
         srcObjNode->incReferenceCount();
         dstObjNode->incReferenceCount();
         src->incReferenceCount();
         dst->incReferenceCount();
         len->incReferenceCount();

         removeChildren(node);

         node->setChild(0, srcObjNode);
         node->setChild(1, dstObjNode);
         node->setChild(2, src);
         node->setChild(3, dst);
         node->setChild(4, len);
         node->setNumChildren(5);
         node->setArrayCopyElementType(type);
         }

      len->getByteCodeInfo().setDoNotProfile(0);

      if (isStringFwdArrayCopy)
         {
         node->setForwardArrayCopy(true);
         }

      if (elementSize == 2 && !primitiveTransform)
         node->setHalfWordElementArrayCopy(true);
      else if (elementSize > 2 && !primitiveTransform)
         node->setWordElementArrayCopy(true);

      if ((dstOffHigh == 0) ||
            (srcOffLow >= dstOffHigh) ||
            ((srcOffHigh <= TR::getMaxSigned<TR::Int32>() - copyLenHigh) && ((srcOffHigh+copyLenHigh) <= dstOffLow)))
         node->setForwardArrayCopy(true);

      // -------------------------------------------------------------------
      // Check and transform the existing array copy node to a primitive
      // arraycopy if neither an arraystore check nor a write barrier are
      // required. Also does not transform if a read barrier is required.
      // -------------------------------------------------------------------

      bool changeExistingNodeToPrimitiveCopy = false;
      bool existingNodeIsReferenceArraycopy = false;

      uint8_t flag = 0;
      if (needWriteBarrier)
         flag |= NEED_WRITE_BARRIER;

      if (needArrayStoreCheck)
         flag |= NEED_ARRAYSTORE_CHECK;

      if (referenceArray1 || referenceArray2)
         {
         if (needArrayStoreCheck)
            {
            _referenceArrayCopyTrees.add(new (trStackMemory()) TR_TreeTopWrtBarFlag(arraycopyTree, flag));
            }
         else
            {
            if (needWriteBarrier || needReadBarrier)
               {
               node->setNoArrayStoreCheckArrayCopy(true);
               }
            else
               {
               changeExistingNodeToPrimitiveCopy = true;
               existingNodeIsReferenceArraycopy = true;
               }
            }
         }
      else if (!primitiveArray1 && !primitiveArray2)
         {
         if (elementSize == 0 || needArrayStoreCheck || needWriteBarrier)
            _unknownTypeArrayCopyTrees.add(new (trStackMemory()) TR_TreeTopWrtBarFlag(arraycopyTree, flag));
         else
            changeExistingNodeToPrimitiveCopy = true;
         }

      if (changeExistingNodeToPrimitiveCopy)
         {
         node->setChild(0, src);
         node->setChild(1, dst);
         node->setChild(2, len);
         node->setChild(3, NULL);
         node->setChild(4, NULL);
         node->setNumChildren(3);

         // PMR 06446,67D,760 - Jazz103 PR 101275 - swapped the if with the else if
         if (existingNodeIsReferenceArraycopy)
            node->setArrayCopyElementType(TR::Address);
         else if (srcArrayInfo)
            {
             // Default to NoType to prevent puting garbage in element datatype
            TR::DataType newType = TR::NoType;
            // Java spec says arraycopies need to be atomic for the element size
            // So we need to make sure we set the element datatype appropriately to enforce that
            switch (srcArrayInfo->elementSize())
               {
               case 8:
                  newType = TR::Int64;
                  break;
               case 4:
                  newType = TR::Int32;
                  break;
               case 2:
                  newType = TR::Int16;
                  break;
               case 1:
                  newType = TR::Int8;
                  break;
               default:
                  TR_ASSERT(0, "Unhandled element size\n");
               }
            node->setArrayCopyElementType(newType);
            }

         srcObjNode->recursivelyDecReferenceCount();
         dstObjNode->recursivelyDecReferenceCount();
         }
      }

   createExceptionEdgeConstraintsAndReturn:

   createExceptionEdgeConstraints(TR::Block::CanCatchUserThrows, NULL, node);

   // We can now make assertions about the arguments to the arraycopy call
   //
   addBlockConstraint(srcObjNode, TR::VPNonNullObject::create(this));
   addBlockConstraint(dstObjNode, TR::VPNonNullObject::create(this));
   srcOffHigh = srcArrayInfo ? srcArrayInfo->highBound() : TR::getMaxSigned<TR::Int32>();
   addBlockConstraint(srcOffNode, TR::VPIntRange::create(this, 0, srcOffHigh));
   dstOffHigh = dstArrayInfo ? dstArrayInfo->highBound() : TR::getMaxSigned<TR::Int32>();
   addBlockConstraint(dstOffNode, TR::VPIntRange::create(this, 0, dstOffHigh));
   addBlockConstraint(copyLenNode, TR::VPIntRange::create(this, 0, TR::getMaxSigned<TR::Int32>()));

   }




TR::Node *generateArrayAddressTree(
   TR::Compilation* comp,
   TR::Node *node,
   int32_t  offHigh,
   TR::Node *offNode,
   TR::Node *objNode,
   int32_t  elementSize,
   TR::Node * &stride,
   TR::Node *hdrSize)
   {

   bool is64BitTarget = TR::Compiler->target.is64Bit() ? true : false;

   TR::Node *array;

   if (offHigh > 0)
      {
      if (elementSize == 1)
         {
         array = offNode->createLongIfNeeded();
         }
      else if (elementSize == 0)
         {
#ifdef J9_PROJECT_SPECIFIC
         if (!stride)
            stride = TR::TransformUtil::generateArrayElementShiftAmountTrees(comp, objNode);
#endif

         if (is64BitTarget)
            {
            if (stride->getType().isInt32())
               stride = TR::Node::create(TR::i2l, 1, stride);

            if (offNode->getType().isInt32())
               {
               TR::Node *i2lNode = TR::Node::create(TR::i2l, 1, offNode);
               array = TR::Node::create(TR::lshl, 2, i2lNode, stride);
               }
            else
               array = TR::Node::create(TR::lshl, 2, offNode, stride);
            }
         else
            array = TR::Node::create(TR::ishl, 2, offNode, stride);
         }
      else
         {
         if (is64BitTarget)
            {
            if (!stride)
               {
               stride = TR::Node::create(node, TR::lconst);
               stride->setLongInt(elementSize);
               }
            else if (stride->getType().isInt32())
               stride = TR::Node::create(TR::i2l, 1, stride);

            if (offNode->getType().isInt32())
               {
               TR::Node *i2lNode = TR::Node::create(TR::i2l, 1, offNode);
               array = TR::Node::create(TR::lmul, 2, i2lNode, stride);
               }
            else
               array = TR::Node::create(TR::lmul, 2, offNode, stride);
            }
         else
            {
            if (!stride)
               stride = TR::Node::create(node, TR::iconst, 0, elementSize);
            array = TR::Node::create(TR::imul, 2, offNode, stride);
            }
         }

      array = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, array, hdrSize);
      }
   else
      {
      array = hdrSize;
      }

   array = TR::Node::create(is64BitTarget ? TR::aladd : TR::aiadd, 2, objNode, array);
   array->setIsInternalPointer(true);

   return array;
   }

TR::TreeTop* OMR::ValuePropagation::createPrimitiveOrReferenceCompareNode(TR::Node* node)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::Node *vftLoad = TR::Node::createWithSymRef(TR::aloadi, 1, 1, node, comp()->getSymRefTab()->findOrCreateVftSymbolRef());
   TR::Node *componentTypeLoad = TR::Node::createWithSymRef(TR::aloadi, 1, 1, vftLoad, comp()->getSymRefTab()->findOrCreateArrayComponentTypeSymbolRef());
   TR::Node *romClassLoad = TR::Node::createWithSymRef(TR::aloadi, 1, 1, componentTypeLoad, comp()->getSymRefTab()->findOrCreateClassRomPtrSymbolRef());
   TR::Node *isArrayField = TR::Node::createWithSymRef(TR::iloadi, 1, 1, romClassLoad, comp()->getSymRefTab()->findOrCreateClassIsArraySymbolRef());
   TR::Node *andConstNode = TR::Node::create(isArrayField, TR::iconst, 0, TR::Compiler->cls.flagValueForPrimitiveTypeCheck(comp()));
   TR::Node * andNode   = TR::Node::create(TR::iand, 2, isArrayField, andConstNode);
   TR::Node *cmp = TR::Node::createif(TR::ificmpne, andNode, andConstNode, NULL);
   TR::TreeTop *cmpTree = TR::TreeTop::create(comp(), cmp);
   return cmpTree;
#else
   return NULL;
#endif
   }

TR::TreeTop* OMR::ValuePropagation::createArrayStoreCompareNode(TR::Node *src, TR::Node *dst)
   {
   TR::Node *vftDst = TR::Node::createWithSymRef(TR::aloadi, 1, 1, dst, comp()->getSymRefTab()->findOrCreateVftSymbolRef());
   TR::Node *instanceofNode = TR::Node::createWithSymRef(TR::instanceof, 2, 2, src, vftDst, comp()->getSymRefTab()->findOrCreateInstanceOfSymbolRef(comp()->getMethodSymbol()));
   TR::Node *cmp =  TR::Node::createif(TR::ificmpeq, instanceofNode, TR::Node::create(dst, TR::iconst, 0, 0), NULL);
   TR::TreeTop *cmpTree = TR::TreeTop::create(comp(), cmp);
   return cmpTree;
   }

TR::TreeTop* OMR::ValuePropagation::createPrimitiveArrayNodeWithoutFlags(TR::TreeTop* tree, TR::TreeTop* newTree, TR::SymbolReference* srcRef, TR::SymbolReference* dstRef, TR::SymbolReference * lenRef, bool useFlagsOnOriginalArraycopy, bool isOptimizedReferenceArraycopy)
   {
   TR::Node* root = tree->getNode()->getFirstChild();
   TR::Node* len = TR::Node::createLoad(root, lenRef);
   TR::Node* src;

   if (srcRef)
      src = TR::Node::createLoad(root, srcRef);
   else
      {
      if (root->getNumChildren() == 3)
         src = root->getFirstChild()->duplicateTree();
      else
         src = root->getChild(2)->duplicateTree();
      }

   TR::Node* dst;
   if (dstRef)
      dst = TR::Node::createLoad(root, dstRef);
   else
      {
     if (root->getNumChildren() == 3)
         dst = root->getSecondChild()->duplicateTree();
      else
         dst = root->getChild(3)->duplicateTree();
      }

   TR::Node* node = TR::Node::createArraycopy(src, dst, len);
   node->setNumChildren(3);

   node->setSymbolReference(root->getSymbolReference());

   if (isOptimizedReferenceArraycopy)
      {
      // If the root node is a reference arraycopy that needs neither array store check
      // nor write barrier, it can be simplified into a three-child arraycopy, but the
      // array element type must be TR::Address.
      node->setArrayCopyElementType(TR::Address);
      }
   else
      {
      // If the root node is an arraycopy of unknown type, a run-time check will have
      // been inserted, and on this path we are generating a truly primitive arraycopy,
      // but the element type remains unknown. root->getArrayCopyElementType()
      // CANNOT be used, since the root node does not have that information (we would
      // not be here if it did).
      //
      // FIXME: Ideally we would like to set the type to TR::NoType here, to make it
      // clear that the code generator should not assume the arrays in question
      // are byte arrays. Assuming byte-sized elements can cause atomicity issues
      // if the primitive arrays are of a wider type (e.g. short, int). However,
      // currently not all codegens (e.g. z) handle an unknown-type arraycopy correctly.
      node->setArrayCopyElementType(TR::Int8);
      }

   if (useFlagsOnOriginalArraycopy)
      {
      //node->setNonDegenerateArrayCopy(root->isNonDegenerateArrayCopy());
      node->setForwardArrayCopy(root->isForwardArrayCopy());
      node->setBackwardArrayCopy(root->isBackwardArrayCopy());
      }
   ///node->setReferenceArrayCopy(false);

   if (trace())
      traceMsg(comp(), "Created 3-child arraycopy %s from root node %s, type = %s\n",
              comp()->getDebug()->getName(node),
              comp()->getDebug()->getName(root),
              TR::DataType::getName(node->getArrayCopyElementType()));

   // duplicate the tree just to copy either the ResolveCHK or the tree-top
   TR::Node* treeNode = tree->getNode()->duplicateTree();
   treeNode->setAndIncChild(0, node);

   newTree->setNode(treeNode);
   return newTree;
   }

TR::TreeTop *createStoresForArraycopyChildren(TR::Compilation *comp, TR::TreeTop *arrayTree, TR::SymbolReference * &srcObjRef, TR::SymbolReference * &dstObjRef, TR::SymbolReference * &srcRef, TR::SymbolReference * &dstRef, TR::SymbolReference * &lenRef)
   {
   TR::Node *node = arrayTree->getNode();
   if (node->getOpCodeValue() != TR::arraycopy)
      node = node->getFirstChild();

   TR::TreeTop *insertBefore = arrayTree;
   TR::TreeTop *storeTree = NULL;

   TR::Node* srcObject = NULL;
   TR::Node* dstObject = NULL;
   TR::Node* src = NULL;
   TR::Node* dst = NULL;
   TR::Node* len = NULL;
   if (node->getNumChildren() == 3)
      {
      src = node->getChild(0);
      dst = node->getChild(1);
      len = node->getChild(2);
      }
   else
      {
      srcObject = node->getChild(0);
      dstObject = node->getChild(1);
      src = node->getChild(2);
      dst = node->getChild(3);
      len = node->getChild(4);
      }

   storeTree = len->createStoresForVar(lenRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;
   storeTree = dst->createStoresForVar(dstRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;
   storeTree = src->createStoresForVar(srcRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   if (dstObject)
      {
      storeTree = dstObject->createStoresForVar(dstObjRef,insertBefore, true);
       if (storeTree)
          insertBefore = storeTree;

      }

   if (srcObject)
      {
      storeTree = srcObject->createStoresForVar(srcObjRef,insertBefore, true);
       if (storeTree)
          insertBefore = storeTree;
      }

   return insertBefore;
   }

#ifdef J9_PROJECT_SPECIFIC
void OMR::ValuePropagation::generateArrayTranslateNode(TR::TreeTop *callTree,TR::TreeTop *arrayTranslateTree, TR::SymbolReference *srcRef, TR::SymbolReference *dstRef, TR::SymbolReference *srcOffRef, TR::SymbolReference *dstOffRef, TR::SymbolReference *lenRef,TR::SymbolReference *tableRef, bool hasTable )
   {

   bool is64BitTarget = TR::Compiler->target.is64Bit();
   TR::Node* callNode = callTree->getNode()->getFirstChild();
   TR::MethodSymbol *symbol = callNode->getSymbol()->castToMethodSymbol();
   const TR::RecognizedMethod rm = symbol->getRecognizedMethod();
   TR_ResolvedMethod *m = symbol->getResolvedMethodSymbol()->getResolvedMethod();

   bool isISO88591Encoder = (rm == TR::sun_nio_cs_ISO_8859_1_Encoder_encodeISOArray
                             || rm == TR::java_lang_StringCoding_implEncodeISOArray);
   bool isISO88591Decoder = (rm == TR::sun_nio_cs_ISO_8859_1_Decoder_decodeISO8859_1);
   bool isSBCSEncoder = (rm == TR::sun_nio_cs_ext_SBCS_Encoder_encodeSBCS)? true:false;
   bool isSBCSDecoder = (rm == TR::sun_nio_cs_ext_SBCS_Decoder_decodeSBCS)? true:false;
   bool isEncodeUtf16 = (rm == TR::sun_nio_cs_UTF_16_Encoder_encodeUTF16Big || rm == TR::sun_nio_cs_UTF_16_Encoder_encodeUTF16Little);


   int32_t childId = callNode->getFirstArgumentIndex();
   if (callNode->getChild(childId)->getType().isAddress() && callNode->getChild(childId+1)->getType().isAddress())
      childId++;

   TR::Node *srcObj = srcRef ? TR::Node::createLoad(callNode, srcRef): callNode->getChild(childId++)->duplicateTree();
   TR::Node *srcOffNode = srcOffRef ? TR::Node::createLoad(callNode, srcOffRef): callNode->getChild(childId++)->duplicateTree();
   TR::Node *srcOff = srcOffNode->createLongIfNeeded();
   TR::Node *lenNode = NULL;
   TR::Node *len = NULL;
   if (!isISO88591Encoder)
      {
      lenNode = lenRef? TR::Node::createLoad(callNode, lenRef):callNode->getChild(childId++)->duplicateTree();
      len = lenNode->createLongIfNeeded();
      }

   TR::Node* dstObj = dstRef ? TR::Node::createLoad(callNode, dstRef): callNode->getChild(childId++)->duplicateTree();
   TR::Node* dstOffNode =  dstOffRef ? TR::Node::createLoad(callNode, dstOffRef): callNode->getChild(childId++)->duplicateTree();
   TR::Node *dstOff = dstOffNode->createLongIfNeeded();

   if (isISO88591Encoder)
      {
      lenNode = lenRef? TR::Node::createLoad(callNode, lenRef):callNode->getChild(childId++)->duplicateTree();
      len = lenNode->createLongIfNeeded();
      }
   TR::Node * tableNode = NULL;
   //hasTable is set if the table was passed in as an argument to the method, i.e. passed by the java call

   //FIX: might not need to pass hasTable as it is dependent on the method
   //if (hasTable && tableRef)
     // tableNode = TR::Node::createLoad(callNode, tableRef);

   bool encode = false;

   TR::Node* hdrSize = createHdrSizeNode(comp(), callNode);

    TR::Node* arrayTranslateNode = TR::Node::create(arrayTranslateTree->getNode()->getFirstChild(), TR::arraytranslate, 6);
    removeNode(arrayTranslateTree->getNode()->getFirstChild());


/*
   TR::Node* arrayTranslateNode = arrayTranslateTree->getNode()->getFirstChild();
   TR::Node::recreate(arrayTranslateNode, TR::arraytranslate);
   arrayTranslateNode->setNumChildren(6);
*/
   arrayTranslateNode->setSymbolReference(comp()->getSymRefTab()->findOrCreateArrayTranslateSymbol());

   TR::Node * node, * src, *dst ;

   TR::Node * termCharNode;
   TR::Node * stoppingNode;
   int stopIndex = -1;
   int termchar = -1;
   TR::Node *strideNode = NULL;
   if (is64BitTarget)
      {
      strideNode = TR::Node::create(callNode, TR::lconst);
      strideNode->setLongInt(2);
      }
   else
      strideNode = TR::Node::create(callNode, TR::iconst, 0, 2);

   if ( isISO88591Encoder || isSBCSEncoder || isEncodeUtf16 ||
       (rm == TR::sun_nio_cs_US_ASCII_Encoder_encodeASCII)         ||
       (rm == TR::sun_nio_cs_UTF_8_Encoder_encodeUTF_8))
       encode = true;

   if (encode)
      {
      node = TR::Node::create(is64BitTarget ? TR::lmul : TR::imul, 2, srcOff, strideNode);
      node = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, node, hdrSize);
      src = TR::Node::create(is64BitTarget? TR::aladd : TR::aiadd, 2, srcObj, node);
      node = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, dstOff, hdrSize);
      dst = TR::Node::create(is64BitTarget? TR::aladd : TR::aiadd, 2, dstObj, node);
      arrayTranslateNode->setSourceIsByteArrayTranslate(false);
      arrayTranslateNode->setTargetIsByteArrayTranslate(true);
      }
   else
      {
      node = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, srcOff, hdrSize);
      src = TR::Node::create(is64BitTarget? TR::aladd : TR::aiadd, 2, srcObj, node);
      node = TR::Node::create(is64BitTarget ? TR::lmul : TR::imul, 2, dstOff, strideNode);
      node = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, node, hdrSize);
      dst = TR::Node::create(is64BitTarget? TR::aladd : TR::aiadd, 2, dstObj, node);
      arrayTranslateNode->setSourceIsByteArrayTranslate(true);
      arrayTranslateNode->setTargetIsByteArrayTranslate(false);
      }

   if (encode && !isSBCSEncoder)
      {
      arrayTranslateNode->setTableBackedByRawStorage(true);
      if (isISO88591Encoder)
         {
         arrayTranslateNode->setTermCharNodeIsHint(true);
         arrayTranslateNode->setSourceCellIsTermChar(true);
         }
      else
         {
         arrayTranslateNode->setTermCharNodeIsHint(false);
         arrayTranslateNode->setSourceCellIsTermChar(false);
         }
      if (cg()->getSupportsArrayTranslateTRTO255()|| cg()->getSupportsArrayTranslateTRTO())
         {
         tableNode = TR::Node::create(callNode, TR::aconst, 0, 0);
         if (isISO88591Encoder)
            termchar = 0xff00ff00;
         else
            termchar = 0xff80ff80;
         }
      else //z
         {
         bool genSIMD = comp()->cg()->getSupportsVectorRegisters() && !comp()->getOption(TR_DisableSIMDArrayTranslate);
         stopIndex = isISO88591Encoder ? 255: 127;
         termchar = isISO88591Encoder ? 0x0B: 0xff;

         if (genSIMD)
            {
            tableNode = TR::Node::create(callNode, TR::aconst, 0, 0); //dummy table node, it's not gonna be used
            }
         else
            {
            uint8_t *table = (uint8_t*)comp()->trMemory()->allocateMemory(65536, stackAlloc);
            int i;
            for (i = 0; i <= stopIndex; i++)
               table[i] = (uint8_t)i;
            for (i = stopIndex+1; i < 65536; i++)
               table[i] = (uint8_t)termchar;

            tableNode = createTableLoad(comp(), callNode, 16, 8, table, false);
            stopIndex=-1;
            }
         }
      termCharNode = TR::Node::create(callNode,TR::iconst, 0, termchar);
      }
   else if (!encode && !isSBCSDecoder)
      {
      arrayTranslateNode->setTermCharNodeIsHint(false);
      arrayTranslateNode->setSourceCellIsTermChar(false);
      arrayTranslateNode->setTableBackedByRawStorage(true);
      if (cg()->getSupportsArrayTranslateTROTNoBreak() ||cg()->getSupportsArrayTranslateTROT())
         {//X or P

         if (isISO88591Decoder)
            termchar = 0xFFFF;
         else
            termchar = 0x00;

         tableNode = TR::Node::create(callNode, TR::iconst, 0, 0); //dummy table node, it's not gonna be used
         }
      else
         {//Z
         bool genSIMD = comp()->cg()->getSupportsVectorRegisters() && !comp()->getOption(TR_DisableSIMDArrayTranslate);

          if (genSIMD)
             {
             tableNode = TR::Node::create(callNode, TR::aconst, 0, 0); //dummy table node, it's not gonna be used
             stopIndex = isISO88591Decoder ? 255: 127;
             }
          else
            {
            uint16_t table[256];
            int i;
            for (i = 0 ; i < 128; i++)
               table[i] = i;
            for (i = 128; i < 256; i++)
               table[i] = isISO88591Decoder ? i : -1;

            tableNode = createTableLoad(comp(), callNode, 8, 16, table, false);
            }
         }

      termCharNode = TR::Node::create(callNode,TR::iconst, 0, termchar);
      }
   else if (isSBCSEncoder) //only z
      {
      arrayTranslateNode->setTermCharNodeIsHint(false);
      arrayTranslateNode->setSourceCellIsTermChar(false);
      arrayTranslateNode->setTableBackedByRawStorage(true);
      termCharNode = TR::Node::create(callNode,TR::iconst, 0, 0);
      TR::Node *tableNodeAddr = tableRef? TR::Node::createLoad(callNode, tableRef):callNode->getChild(childId++)->duplicateTree();
      tableNode = TR::Node::create(is64BitTarget? TR::aladd : TR::aiadd, 2, tableNodeAddr, hdrSize);
      }

   else if (isSBCSDecoder) //only z
      {
      arrayTranslateNode->setTermCharNodeIsHint(true);
      arrayTranslateNode->setSourceCellIsTermChar(false);
      arrayTranslateNode->setTableBackedByRawStorage(false);
      termCharNode = TR::Node::create(callNode,TR::iconst, 0, 11);
      TR::Node *tableNodeAddr = tableRef? TR::Node::createLoad(callNode, tableRef):callNode->getChild(childId++)->duplicateTree();
      tableNode = tableNodeAddr;
      }

   stoppingNode = TR::Node::create(callNode,TR::iconst, 0, stopIndex);

   if (isEncodeUtf16)
      {
      TR::SymbolReference* transformedCallSymRef =
         comp()->getSymRefTab()->methodSymRefFromName(
            comp()->getMethodSymbol(),
            "com/ibm/jit/JITHelpers",
            const_cast<char*> (TR::sun_nio_cs_UTF_16_Encoder_encodeUTF16Big == rm ?
               "transformedEncodeUTF16Big" :
               "transformedEncodeUTF16Little"),
            "(JJI)I",
            TR::MethodSymbol::Static
            );

      arrayTranslateNode = TR::Node::createWithSymRef(callNode, callNode->getOpCodeValue(), 3, transformedCallSymRef);
      }

   arrayTranslateNode->setAndIncChild(0, src);
   arrayTranslateNode->setAndIncChild(1, dst);

   if (isEncodeUtf16)
      {
      arrayTranslateNode->setAndIncChild(2, len);
      }
   else
      {
      arrayTranslateNode->setAndIncChild(2, tableNode);
      arrayTranslateNode->setAndIncChild(3, termCharNode);
      arrayTranslateNode->setAndIncChild(4, len);
      arrayTranslateNode->setAndIncChild(5, stoppingNode);
      }

   //arrayTranslateNode->setChild(5, NULL);//* do I need this? what if there are more children?*/

   arrayTranslateTree->getNode()->setAndIncChild(0,arrayTranslateNode);
   len->getByteCodeInfo().setDoNotProfile(0);//why?
  }
#endif

TR::TreeTop* OMR::ValuePropagation::createConverterCallNodeAfterStores(
   TR::TreeTop         *tree,
   TR::TreeTop         *origTree,
   TR::SymbolReference *srcRef,
   TR::SymbolReference *dstRef,
   TR::SymbolReference *lenRef,
   TR::SymbolReference *srcOffRef,
   TR::SymbolReference *dstOffRef,
   TR::SymbolReference *thisRef,
   TR::SymbolReference *tableRef)
   {
   TR::Node *root = tree->getNode()->getFirstChild();
   TR::Node *origCallNode = origTree->getNode()->getFirstChild();

   TR::MethodSymbol *symbol = root->getSymbol()->castToMethodSymbol();
   TR::RecognizedMethod rm = symbol->getRecognizedMethod();
   TR_ResolvedMethod *m = symbol->getResolvedMethodSymbol()->getResolvedMethod();

#ifdef J9_PROJECT_SPECIFIC
   bool isISO88591Encoder = (rm == TR::sun_nio_cs_ISO_8859_1_Encoder_encodeISOArray
                             || rm == TR::java_lang_StringCoding_implEncodeISOArray);
#else
   bool isISO88591Encoder = false;
#endif
   int32_t childId = origTree->getNode()->getFirstChild()->getFirstArgumentIndex();
   bool hasReciever = symbol->isStatic() ? false : true;
   int32_t numberOfChildren = hasReciever ? childId+6: childId+5;
   if (tableRef)
       numberOfChildren++;
   root->setNumChildren(numberOfChildren);


   //childId will point to the location of first child after vft if exists
   TR::Node *len = NULL, *src = NULL, *dst = NULL, *srcOff = NULL, *dstOff = NULL, *thisNode = NULL, *table;
   if (hasReciever)
      {
      if (thisRef)
         thisNode = TR::Node::createLoad(root, thisRef);
      else
         thisNode = root->getChild(childId)->duplicateTree();
      root->setAndIncChild(childId++,thisNode);
      }

   //all indirect calls would have reciever computed.
   if(root->getOpCode().isCallIndirect())
      {
      TR::Node *vftLoad = TR::Node::createWithSymRef(TR::aloadi, 1, 1, thisNode, comp()->getSymRefTab()->findOrCreateVftSymbolRef());
      root->setAndIncChild(0,vftLoad);
      }



   if (srcRef)
      src = TR::Node::createLoad(root, srcRef);
   else
      src = root->getChild(childId)->duplicateTree();

   root->setAndIncChild(childId, src);

   if (srcOffRef)
      srcOff = TR::Node::createLoad(root, srcOffRef);
   else
      srcOff = root->getChild(childId+1)->duplicateTree();

   root->setAndIncChild(childId+1, srcOff);

   if (lenRef)
      len = TR::Node::createLoad(root, lenRef);
   else
      len = isISO88591Encoder?  root->getChild(childId+4)->duplicateTree() : root->getChild(childId+2)->duplicateTree();

   isISO88591Encoder? root->setAndIncChild(childId+4, len): root->setAndIncChild(childId+2, len);


   if (dstRef)
      dst = TR::Node::createLoad(root, dstRef);
   else
      dst = isISO88591Encoder ? root->getChild(childId+2)->duplicateTree(): root->getChild(childId+3)->duplicateTree();

   isISO88591Encoder?root->setAndIncChild(childId+2, dst): root->setAndIncChild(childId+3, dst);


   if (dstOffRef)
      dstOff = TR::Node::createLoad(root, dstOffRef);
   else
      dstOff = isISO88591Encoder ? root->getChild(childId+3)->duplicateTree(): root->getChild(childId+4)->duplicateTree();

   isISO88591Encoder?root->setAndIncChild(childId+3, dstOff): root->setAndIncChild(childId+4, dstOff);

   if (tableRef)
      {
      table = TR::Node::createLoad(root, tableRef);
      root->setAndIncChild(childId+5, table);
      }

   return tree;
   }

//-------------------------- realtime support
TR::TreeTop *OMR::ValuePropagation::buildSameLeafTest(TR::Node *offset,TR::Node *len,TR::Node *spineShiftNode)
   {
   TR::TreeTop *ifTree = TR::TreeTop::create(comp());
   TR::Node *ifNode;
   TR::Node *child1 = NULL, *child2 = NULL;

   bool is64BitTarget = TR::Compiler->target.is64Bit();

   child1 = TR::Node::create(is64BitTarget ? TR::lshr : TR::ishr, 2, offset, spineShiftNode);
   child2 = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, offset, len);
   child2 = TR::Node::create(is64BitTarget ? TR::lshr : TR::ishr, 2, child2, spineShiftNode);
   ifNode = TR::Node::createif(is64BitTarget ? TR::iflcmpne : TR::ificmpne, child1, child2);

   //check with this :ifNode = TR::Node::createif(TR::ificmpne, TR::Node::create(offset , TR::iconst, 0, 1), TR::Node::create(offset , TR::iconst, 1, 1));
   ifTree->setNode(ifNode);

   return ifTree;
   }


TR::Node *generateArrayletAddressTree(TR::Compilation* comp, TR::Node *vcallNode, TR::DataType type, TR::Node *off,TR::Node *obj, TR::Node *spineShiftNode,TR::Node *shiftNode, TR::Node *strideShiftNode, TR::Node *hdrSize)
   {
   bool is64BitTarget = TR::Compiler->target.is64Bit() ? true : false;

   uint32_t elementSize = TR::Symbol::convertTypeToSize(type);
   if (comp->useCompressedPointers() && (type == TR::Address))
      elementSize = TR::Compiler->om.sizeofReferenceField();

   TR::Node *node;

   node = TR::Node::create(is64BitTarget ? TR::lshr : TR::ishr, 2, off, spineShiftNode);
   node = TR::Node::create(is64BitTarget ? TR::lshl : TR::ishl, 2, node, shiftNode);
   node = TR::Node::create(is64BitTarget ? TR::ladd : TR::iadd, 2, node, hdrSize);
   node = TR::Node::create(is64BitTarget ? TR::aladd : TR::aiadd, 2, obj, node);
   node = TR::Node::createWithSymRef(TR::aloadi, 1, 1, node, comp->getSymRefTab()->findOrCreateArrayletShadowSymbolRef(type));

   TR::Node *leafOffset = NULL;

   if (is64BitTarget)
      {
      leafOffset = TR::Node::create(vcallNode , TR::lconst);
      leafOffset->setLongInt((int64_t) comp->fe()->getArrayletMask(elementSize));
      }
   else
      {
      leafOffset = TR::Node::create(vcallNode, TR::iconst, 0, (int32_t)comp->fe()->getArrayletMask(elementSize));
      }

   TR::Node *nodeLeafOffset = TR::Node::create(is64BitTarget ? TR::land : TR::iand, 2, leafOffset, off);

   if (strideShiftNode)
      {
      nodeLeafOffset = TR::Node::create(is64BitTarget ? TR::lshl : TR::ishl, 2, nodeLeafOffset, strideShiftNode);
      }

   node = TR::Node::create(is64BitTarget ? TR::aladd : TR::aiadd, 2, node, nodeLeafOffset);

   return node;
   }

void OMR::ValuePropagation::generateRTArrayNodeWithoutFlags(TR_RealTimeArrayCopy *rtArrayCopy,TR::TreeTop *dupArraycopyTree, TR::SymbolReference *srcRef, TR::SymbolReference *dstRef, TR::SymbolReference *srcOffRef, TR::SymbolReference *dstOffRef, TR::SymbolReference *lenRef, bool primitive)
   {
   TR::DataType type = rtArrayCopy->_type;
   uint32_t elementSize = TR::Symbol::convertTypeToSize(type);
   if (comp()->useCompressedPointers() && (type == TR::Address))
      elementSize = TR::Compiler->om.sizeofReferenceField();
   TR::Node* node = rtArrayCopy->_treetop->getNode()->getFirstChild();

   TR::Node* len;
   if (lenRef)
      len = TR::Node::createLoad(node, lenRef);
   else
      len = node->getChild(4)->duplicateTree();


   TR::Node* srcObj;
   if (srcRef)
      srcObj = TR::Node::createLoad(node, srcRef);
   else
      {
      srcObj = node->getChild(0)->duplicateTree();
      }

   TR::Node* dstObj;
   if (dstRef)
      dstObj = TR::Node::createLoad(node, dstRef);
   else
      {
      dstObj = node->getChild(2)->duplicateTree();
      }

   TR::Node * srcOffNode;
   if (srcOffRef)
      srcOffNode = TR::Node::createLoad(node, srcOffRef);
   else
      {
      srcOffNode = node->getChild(1)->duplicateTree();
      }
   TR::Node *srcOff = srcOffNode->createLongIfNeeded();


   TR::Node* dstOffNode;
   if (dstOffRef)
      dstOffNode = TR::Node::createLoad(node, dstOffRef);
   else
      {
      dstOffNode = node->getChild(3)->duplicateTree();
      }

   TR::Node *dstOff = dstOffNode->createLongIfNeeded();

   TR::Node* hdrSize = createHdrSizeNode(comp(), node);
   TR::Node *spineShiftNode = TR::Node::create(node, TR::iconst, 0, (int32_t)fe()->getArraySpineShift(elementSize));
   TR::Node *strideShiftNode = NULL;
   TR::Node *shiftNode = NULL;
   int32_t shift = TR::TransformUtil::convertWidthToShift(TR::Compiler->om.sizeofReferenceField());
   int32_t strideShift = TR::TransformUtil::convertWidthToShift(elementSize);

   shiftNode = TR::Node::create(node, TR::iconst, 0, (int32_t)shift);
   if (strideShift)
      strideShiftNode = TR::Node::create(node, TR::iconst, 0, (int32_t)strideShift);


   srcOff = generateArrayletAddressTree(comp(), node, type ,srcOff ,srcObj, spineShiftNode, shiftNode, strideShiftNode, hdrSize);
   dstOff = generateArrayletAddressTree(comp(), node, type, dstOff ,dstObj, spineShiftNode, shiftNode, strideShiftNode, hdrSize);
   len = generateLenForArrayCopy(comp(),elementSize,NULL,srcObj,len,node);

   //   TR::Node* arraycopyNode = TR::Node::createArraycopy(srcOff, dstOff, len);
   TR::Node* arraycopyNode = dupArraycopyTree->getNode()->getFirstChild();
   arraycopyNode->setNodeIsRecognizedArrayCopyCall(false); // flag conflicts with isForwardArrayCopy
   TR::Node::recreate(arraycopyNode, TR::arraycopy);
   if (primitive)
      {
      arraycopyNode->setAndIncChild(0, srcOff);
      arraycopyNode->setAndIncChild(1, dstOff);
      arraycopyNode->setAndIncChild(2, len);
      arraycopyNode->setChild(3, NULL);
      arraycopyNode->setChild(4, NULL);
      arraycopyNode->setNumChildren(3);
      }
   else
      {
      arraycopyNode->setAndIncChild(0, srcObj);
      arraycopyNode->setAndIncChild(1, dstObj);
      arraycopyNode->setAndIncChild(2, srcOff);
      arraycopyNode->setAndIncChild(3, dstOff);
      arraycopyNode->setAndIncChild(4, len);
      arraycopyNode->setNumChildren(5);
      }



   arraycopyNode->setArrayCopyElementType(type);
   //arraycopyNode->setSymbolReference(node->getSymbolReference());

   len->getByteCodeInfo().setDoNotProfile(0);

   if (rtArrayCopy->_flag & FORWARD_ARRAYCOPY)
      arraycopyNode->setForwardArrayCopy(true);

   bool    primitiveTransform = comp()->cg()->getSupportsPrimitiveArrayCopy();
   if (elementSize == 2 && !primitiveTransform)
      arraycopyNode->setHalfWordElementArrayCopy(true);
   else if (elementSize > 2 && !primitiveTransform)
      arraycopyNode->setWordElementArrayCopy(true);

   }


TR::TreeTop* OMR::ValuePropagation::createArrayCopyVCallNodeAfterStores(
   TR::TreeTop         *tree,
   TR::SymbolReference *srcRef,
   TR::SymbolReference *dstRef,
   TR::SymbolReference *lenRef,
   TR::SymbolReference *srcOffRef,
   TR::SymbolReference *dstOffRef)
   {
   TR::Node *root = tree->getNode()->getFirstChild();

   TR::Node *len, *src, *dst, *srcOff, *dstOff;

   if (lenRef)
      len = TR::Node::createLoad(root, lenRef);
   else
      len = root->getChild(4)->duplicateTree();

   if (srcRef)
      src = TR::Node::createLoad(root, srcRef);
   else
      src = root->getChild(0)->duplicateTree();

   if (dstRef)
      dst = TR::Node::createLoad(root, dstRef);
   else
      dst = root->getChild(2)->duplicateTree();

   if (srcOffRef)
      srcOff = TR::Node::createLoad(root, srcOffRef);
   else
      srcOff = root->getChild(1)->duplicateTree();

   if (dstOffRef)
      dstOff = TR::Node::createLoad(root, dstOffRef);
   else
      dstOff = root->getChild(1)->duplicateTree();

   // duplicate the tree just to copy either the ResolveCHK or the tree-top
   //root->removeAllChildren(); no need because the children were not countable for refCount

   root->setAndIncChild(0, src);
   root->setAndIncChild(1, srcOff);
   root->setAndIncChild(2, dst);
   root->setAndIncChild(3, dstOff);
   root->setAndIncChild(4, len);
   root->setNumChildren(5);

   return tree;

   }

TR::TreeTop *createStoresForConverterCallChildren(TR::Compilation *comp, TR::TreeTop *callTree, TR::SymbolReference * &srcRef, TR::SymbolReference * &dstRef, TR::SymbolReference * &srcOffRef, TR::SymbolReference * &dstOffRef, TR::SymbolReference * &lenRef,TR::SymbolReference * &thisRef, TR::TreeTop *insertBefore)
   {
   TR::Node *node = callTree->getNode()->getFirstChild();
   TR::MethodSymbol *symbol = node->getSymbol()->castToMethodSymbol();
   TR::RecognizedMethod rm = symbol->getRecognizedMethod();
   TR_ResolvedMethod *m = symbol->getResolvedMethodSymbol()->getResolvedMethod();
#ifdef J9_PROJECT_SPECIFIC
   bool isISO88591Encoder = (rm == TR::sun_nio_cs_ISO_8859_1_Encoder_encodeISOArray
                             || rm == TR::java_lang_StringCoding_implEncodeISOArray);
#else
   bool isISO88591Encoder = false;
#endif

   int32_t childId = node->getFirstArgumentIndex();
   bool hasReciever = symbol->isStatic() ? false : true;
   int32_t thisChildId = childId;
   if (hasReciever)
       childId++;


   TR::CFG *cfg = comp->getFlowGraph();

   TR::Node *src = node->getChild(childId++);
   TR::Node *srcOff = node->getChild(childId++);
   TR::Node *len = NULL;
   if (!isISO88591Encoder)
      len = node->getChild(childId++);
   TR::Node *dst = node->getChild(childId++);
   TR::Node *dstOff = node->getChild(childId++);
   if (isISO88591Encoder)
      len = node->getChild(childId++);

   TR::TreeTop *storeTree = NULL;

   storeTree = len->createStoresForVar(lenRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = dstOff->createStoresForVar(dstOffRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = dst->createStoresForVar(dstRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = srcOff->createStoresForVar(srcOffRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = src->createStoresForVar(srcRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   if (hasReciever)
      {
      storeTree = node->getChild(thisChildId)->createStoresForVar(thisRef,insertBefore);
      if (storeTree)
         insertBefore = storeTree;
      }
   return insertBefore;
   }



TR::TreeTop *createStoresForArraycopyVCallChildren(TR::Compilation *comp, TR::TreeTop *vcallTree, TR::SymbolReference * &srcRef, TR::SymbolReference * &dstRef, TR::SymbolReference * &srcOffRef, TR::SymbolReference * &dstOffRef, TR::SymbolReference * &lenRef, TR::TreeTop *insertBefore )
   {

   TR::Node *node = vcallTree->getNode();
   if (node->getOpCodeValue() != TR::call)
      node = node->getFirstChild();

   TR::CFG *cfg = comp->getFlowGraph();
   TR::Node *src = node->getChild(0);
   TR::Node *srcOff = node->getChild(1);
   TR::Node *dst = node->getChild(2);
   TR::Node *dstOff = node->getChild(3);
   TR::Node *len = node->getChild(4);

   TR::TreeTop *storeTree = NULL;
   storeTree = len->createStoresForVar(lenRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;
   storeTree = dstOff->createStoresForVar(dstOffRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = dst->createStoresForVar(dstRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = srcOff->createStoresForVar(srcOffRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   storeTree = src->createStoresForVar(srcRef,insertBefore);
   if (storeTree)
      insertBefore = storeTree;

   return insertBefore;
   }

void   createAndInsertTestBlock(TR::Compilation *comp,TR::TreeTop *ifTree, TR::TreeTop *tree, TR::Block *insertAfterBlock, TR::Block *dstBlock)
   {
   TR::CFG *cfg = comp->getFlowGraph();

   TR::Block * ifBlock = TR::Block::createEmptyBlock(tree->getNode(), comp, 0, insertAfterBlock);
   ifBlock->append(ifTree);

   TR::TreeTop::insertTreeTops(comp, insertAfterBlock->getExit(), ifBlock->getEntry(), ifBlock->getExit());
   ifTree->getNode()->setBranchDestination(dstBlock->getEntry());

   cfg->addNode(ifBlock);
   cfg->addEdge(TR::CFGEdge::createEdge(insertAfterBlock,  ifBlock, comp->trMemory()));
   cfg->addEdge(TR::CFGEdge::createEdge(ifBlock, dstBlock, comp->trMemory()));
   cfg->addEdge(TR::CFGEdge::createEdge(ifBlock,ifBlock->getNextBlock(), comp->trMemory()));
   cfg->removeEdge(insertAfterBlock,ifBlock->getNextBlock() );

   if (!insertAfterBlock->isCold())
      {
      ifBlock->setIsCold(false);
      ifBlock->setFrequency(insertAfterBlock->getFrequency());
      }

   }


// Check if the contiguous size field is zero, which indicates the array is discontiguous.
//
TR::TreeTop *OMR::ValuePropagation::createSpineCheckNode(TR::Node *node, TR::SymbolReference *objSymRef)
   {
   TR::Node *objNode = TR::Node::createLoad(node, objSymRef);
   TR::Node *sizeNode = TR::Node::createWithSymRef(TR::iloadi, 1, 1, objNode, comp()->getSymRefTab()->findOrCreateContiguousArraySizeSymbolRef());
   TR::Node *cmpNode = TR::Node::createif(TR::ificmpeq, sizeNode, TR::Node::create(node, TR::iconst, 0, 0), NULL);
   TR::TreeTop *cmpTree = TR::TreeTop::create(comp(), cmpNode);
   return cmpTree;
   }

// Spills the original arguments to System.arraycopy before the spine check.
//
TR::TreeTop *OMR::ValuePropagation::createAndInsertStoresForArrayCopySpineCheck(
   TR_ArrayCopySpineCheck *checkInfo)
   {
   TR::TreeTop *storeTree;

   TR::TreeTop *insertBeforeTree = checkInfo->_arraycopyTree;

   storeTree = checkInfo->_srcObjNode->createStoresForVar( checkInfo->_srcObjRef, insertBeforeTree, true);
   storeTree = checkInfo->_srcOffNode->createStoresForVar( checkInfo->_srcOffRef, insertBeforeTree);
   storeTree = checkInfo->_dstObjNode->createStoresForVar( checkInfo->_dstObjRef, insertBeforeTree, true);
   storeTree = checkInfo->_dstOffNode->createStoresForVar( checkInfo->_dstOffRef, insertBeforeTree);
   storeTree = checkInfo->_copyLenNode->createStoresForVar( checkInfo->_copyLenRef, insertBeforeTree);

   return storeTree;
   }


TR::TreeTop *OMR::ValuePropagation::createArrayCopyCallForSpineCheck(
   TR_ArrayCopySpineCheck *checkInfo)
   {
   TR::Node *srcObjNode, *srcOffNode, *dstObjNode, *dstOffNode, *lenNode;

   TR::Node *acNode = checkInfo->_arraycopyTree->getNode();

   if (acNode->getOpCodeValue() != TR::arraycopy)
      {
      acNode = acNode->getFirstChild();
//      TR_ASSERT((acNode->getOpCodeValue() == TR::arraycopy) || acNode->getOpCode().isStore(), "unexpected opcode");
      }

   TR_ASSERT(checkInfo->_srcObjRef, "missing src object sym ref");
   srcObjNode = TR::Node::createLoad(acNode, checkInfo->_srcObjRef);

   TR_ASSERT(checkInfo->_srcOffRef, "missing src offset sym ref");
   srcOffNode = TR::Node::createLoad(acNode, checkInfo->_srcOffRef);

   TR_ASSERT(checkInfo->_dstObjRef, "missing dst object sym ref");
   dstObjNode = TR::Node::createLoad(acNode, checkInfo->_dstObjRef);

   TR_ASSERT(checkInfo->_dstOffRef, "missing dst offset sym ref");
   dstOffNode = TR::Node::createLoad(acNode, checkInfo->_dstOffRef);

   TR_ASSERT(checkInfo->_copyLenRef, "missing len sym ref");
   lenNode = TR::Node::createLoad(acNode, checkInfo->_copyLenRef);

   TR::Node *callNode = TR::Node::createWithSymRef(acNode, TR::call, 5, checkInfo->_arraycopySymRef);
   callNode->setAndIncChild(0, srcObjNode);
   callNode->setAndIncChild(1, srcOffNode);
   callNode->setAndIncChild(2, dstObjNode);
   callNode->setAndIncChild(3, dstOffNode);
   callNode->setAndIncChild(4, lenNode);

   callNode->setDontTransformArrayCopyCall();

   TR::Node *ttNode = TR::Node::create(TR::treetop, 1, callNode);

   TR::TreeTop *acCallTree = TR::TreeTop::create(comp());
   acCallTree->setNode(ttNode);

   return acCallTree;
   }


void OMR::ValuePropagation::transformArrayCopySpineCheck(TR_ArrayCopySpineCheck *checkInfo)
   {

   TR::CFG *cfg = comp()->getFlowGraph();

//   comp()->dumpMethodTrees("Trees before arraycopy spine check");
//   cfg->comp()->getDebug()->print(cfg->comp()->getOutFile(), cfg);

   // Spill the arguments to the arraycopy call before the branch.
   //
   TR::TreeTop *lastTree = createAndInsertStoresForArrayCopySpineCheck(checkInfo);

   // Create the trees to dispatch to the arraycopy native.
   //
   TR::TreeTop *acCallTree = createArrayCopyCallForSpineCheck(checkInfo);

   // Spine check the source
   //
   TR::TreeTop *srcCheckIfTree = createSpineCheckNode(checkInfo->_srcObjNode, checkInfo->_srcObjRef);

   // Split the arraycopy block
   //
   TR::Block *arraycopyBlock = checkInfo->_arraycopyTree->getEnclosingBlock();

   // Spine check the destination
   //
   TR::TreeTop *dstCheckIfTree = createSpineCheckNode(checkInfo->_dstObjNode, checkInfo->_dstObjRef);

   TR::TreeTop *arraycopyTree = checkInfo->_arraycopyTree;

   // Split the original arraycopy block for the source check
   //
   cfg->setStructure(0);
   TR::Block *elseBlock1 = arraycopyBlock->split(arraycopyTree, cfg, true);
   arraycopyBlock->append(srcCheckIfTree);

   // Split the 'else' block for the dest check
   //
   TR::Block *elseBlock2 = elseBlock1->split(arraycopyTree, cfg, true);
   elseBlock1->append(dstCheckIfTree);

   // Split the second 'else' block to put the arraycopy in its own block
   // (i.e., the fall-thru case)
   //
   TR::Block *remainderBlock = elseBlock2->split(arraycopyTree->getNextTreeTop(), cfg, true);

   // Create the 'if' block to do the arraycopy call.
   //
   TR::Block *ifBlock = TR::Block::createEmptyBlock(arraycopyTree->getNode(), comp(), 0, remainderBlock);
   ifBlock->setFrequency(UNKNOWN_COLD_BLOCK_COUNT);
   ifBlock->setIsCold();
   cfg->addNode(ifBlock);
   cfg->findLastTreeTop()->join(ifBlock->getEntry());
   ifBlock->append(acCallTree);
   ifBlock->append(TR::TreeTop::create(comp(),
      TR::Node::create(arraycopyTree->getNode(), TR::Goto, 0, remainderBlock->getEntry())));

   srcCheckIfTree->getNode()->setBranchDestination(ifBlock->getEntry());
   dstCheckIfTree->getNode()->setBranchDestination(ifBlock->getEntry());

   cfg->addEdge(TR::CFGEdge::createEdge(arraycopyBlock,  ifBlock, trMemory()));
   cfg->addEdge(TR::CFGEdge::createEdge(elseBlock1,  ifBlock, trMemory()));
   cfg->addEdge(TR::CFGEdge::createEdge(ifBlock,  remainderBlock, trMemory()));

   cfg->copyExceptionSuccessors(arraycopyBlock, ifBlock);

//   comp()->dumpMethodTrees("Trees after arraycopy spine check");
//   cfg->comp()->getDebug()->print(cfg->comp()->getOutFile(), cfg);

   }


void OMR::ValuePropagation::transformRealTimeArrayCopy(TR_RealTimeArrayCopy *rtArrayCopyTree)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::CFG *cfg = comp()->getFlowGraph();

   TR::TreeTop *vcallTree = rtArrayCopyTree->_treetop;
   TR::Node *vcallNode = vcallTree->getNode()->getFirstChild();
   //maybe the call node was removed.
   if (vcallNode->getReferenceCount() < 1)
      return;

   TR::Block *origCallBlock = rtArrayCopyTree->_treetop->getEnclosingBlock();
   TR::DataType type = rtArrayCopyTree->_type;
   uint32_t elementSize = TR::Symbol::convertTypeToSize(type);
   if (comp()->useCompressedPointers() && (type == TR::Address))
      elementSize = TR::Compiler->om.sizeofReferenceField();
   //------------------------------------------------
   TR::TreeTop *dupVCallTree = TR::TreeTop::create(comp());

   TR::Node* dupVCallNode = vcallTree->getNode()->duplicateTree();
   dupVCallTree->setNode(dupVCallNode);
   if (dupVCallNode->getOpCodeValue()!= TR::call)
      dupVCallNode = dupVCallNode->getFirstChild();

   dupVCallNode->removeAllChildren();
   //dupVCallNode->setDontTransformArrayCopyCall(comp());

   //for testing purpose can create a dummy slow version without call (instead of the real call)
   //TR::Node *dummyNode1 = TR::Node::create(TR::treetop, 1, TR::Node::create( vcallNode, TR::iconst, 0, 0));
   //dupVCallTree->setNode(dummyNode1);

   TR::TreeTop* dupArraycopyTree  = TR::TreeTop::create(comp());
   TR::Node* dupArraycopyNode = vcallTree->getNode()->duplicateTree();
   dupArraycopyTree->setNode(dupArraycopyNode);
   if (dupArraycopyNode->getOpCodeValue()!= TR::call)
      dupArraycopyNode = dupArraycopyNode->getFirstChild();

   dupArraycopyNode->removeAllChildren();

   //  TR::Node *dummyNode2 = TR::Node::create(TR::treetop, 1, TR::Node::create(vcallNode, TR::iconst, 0, 0));
   //  dupArraycopyTree->setNode(dummyNode2);

   //------------------------------------------------
   //create ifNodes
   TR::Node *spineShiftNode = TR::Node::create(vcallNode, TR::iconst, 0, (int32_t)fe()->getArraySpineShift(elementSize));
   TR::TreeTop *ifTree;
   TR::Node *ifNode;
   TR::Node *srcOff = NULL, *dstOff = NULL;
   TR::Node *len = NULL;
   TR::SymbolReference *child1Ref = NULL, *child2Ref = NULL;
   //   printf("******transformReatimeArraycopy %p\n",vcallNode);

   bool doMultiLeafArrayCopy = (!comp()->getOption(TR_DisableMultiLeafArrayCopy) &&
                                ((rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_SRC) ||
                                 (rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_DST)));
   TR::TreeTop *origDupVCallTree = dupVCallTree;

   dumpOptDetails(comp(), "Insert runtime tests for arraycopy %p\n", vcallNode);
   if (type != TR::Address || !comp()->getOptions()->realTimeGC())
      {
      len = vcallNode->getChild(4)->createLongIfNeeded();
      if (rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_SRC)
         {
         //      printf("needsSrc\n");
         srcOff = vcallNode->getChild(1)->createLongIfNeeded();
         }
      if (rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_DST)
         {
         //      printf("needsDst\n");
         if (srcOff)
            dstOff = vcallNode->getChild(3);
         else
            dstOff = vcallNode->getChild(3)->createLongIfNeeded();
         }

      //get one if need to split the blocks.
      if (srcOff)
         ifNode = srcOff;
      else if (dstOff)
         ifNode = dstOff;
      else
         {
         TR_ASSERT(0,"should have srcOff or dstOff set\n");
         }

      ifTree = buildSameLeafTest(ifNode,len,spineShiftNode);
      }
   else
      {
      //printf("reference arraycopy\n");fflush(stdout);
      // srcOFf dstOff are only indicators for runtime tests needed, so make sure they are not null without incrementing ref count
      if (rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_SRC)
         {
         //      printf("needsSrc\n");
         srcOff = vcallNode->getChild(1);
         }
      if (rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_DST)
         {
         //      printf("needsDst\n");
         dstOff = vcallNode->getChild(3);
         }

      TR::Node *fragmentParent = TR::Node::createWithSymRef(vcallNode, TR::aload, 0, comp()->getSymRefTab()->findOrCreateFragmentParentSymbolRef());
      if (TR::Compiler->target.is64Bit())
         {
         TR::Node *globalFragment = TR::Node::createWithSymRef(TR::lloadi, 1, 1, fragmentParent, comp()->getSymRefTab()->findOrCreateGlobalFragmentSymbolRef());
         ifTree = TR::TreeTop::create(comp(), TR::Node::createif(TR::iflcmpne, globalFragment,TR::Node::create(vcallNode, TR::lconst, 0, 0)));
         }
      else
         {
         TR::Node *globalFragment = TR::Node::createWithSymRef(TR::iloadi, 1, 1, fragmentParent, comp()->getSymRefTab()->findOrCreateGlobalFragmentSymbolRef());
         ifTree = TR::TreeTop::create(comp(), TR::Node::createif(TR::ificmpne, globalFragment,TR::Node::create( vcallNode, TR::iconst, 0, 0)));
         }

      if (doMultiLeafArrayCopy)
         {
         dupVCallTree = TR::TreeTop::create(comp());

         dupVCallNode = vcallTree->getNode()->duplicateTree();
         dupVCallTree->setNode(dupVCallNode);
         if (dupVCallNode->getOpCodeValue()!= TR::call)
            dupVCallNode = dupVCallNode->getFirstChild();

         dupVCallNode->removeAllChildren();
         dupVCallNode->setDontTransformArrayCopyCall();
         }
      }



   //------------------------------------------------

   TR::SymbolReference *srcRef = NULL, *dstRef = NULL;
   TR::SymbolReference *srcOffRef = NULL, *dstOffRef = NULL;
   TR::SymbolReference *lenRef = NULL;



   TR::TreeTop *dummyTT = TR::TreeTop::create(comp());
   TR::Node *dummyNode = TR::Node::create(TR::treetop, 1, TR::Node::create(vcallNode, TR::iconst, 0, 0));
   dummyTT->setNode(dummyNode);
   dummyTT->join(vcallTree->getNextTreeTop());
   vcallTree->join(dummyTT);
   //   if (trace())
   //  comp()->dumpMethodTrees("Trees before createConditional");
   origCallBlock->createConditionalBlocksBeforeTree(dummyTT, ifTree, dupVCallTree , dupArraycopyTree, cfg, false);

   // if (trace())
   //  comp()->dumpMethodTrees("Trees after createConditional");
   createStoresForArraycopyVCallChildren(comp(), vcallTree, srcRef, dstRef, srcOffRef, dstOffRef, lenRef, vcallTree);

   // if (trace())
   // comp()->dumpMethodTrees("Trees after StoresCreated");

   //fix arraycopy
   //this is the fast path
   //if this is tarok, depending on wrtbar we create a primitive arraycopy or reference arraycopy with no store check flag.
   //for realtime the fast path is primitive (if we got there there is no need for wrtbar)
   if (!comp()->getOptions()->realTimeGC() &&  ((rtArrayCopyTree->_flag & NEED_WRITE_BARRIER) != 0))
      {
      //tarok, need wrtbar
      generateRTArrayNodeWithoutFlags(rtArrayCopyTree, dupArraycopyTree, srcRef, dstRef, srcOffRef, dstOffRef, lenRef,0);
      dupArraycopyTree->getNode()->getFirstChild()->setNoArrayStoreCheckArrayCopy(true);
      if (comp()->useCompressedPointers())
         {
         TR::TreeTop *prevTree = dupArraycopyTree->getPrevTreeTop();
         TR_ASSERT((dupArraycopyTree->getNode()->getFirstChild()->getChild(2)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the src arraylet pointer");
         TR_ASSERT((dupArraycopyTree->getNode()->getFirstChild()->getChild(3)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the dst arraylet pointer");

         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupArraycopyTree->getNode()->getFirstChild()->getChild(2)->getFirstChild()));
         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupArraycopyTree->getNode()->getFirstChild()->getChild(3)->getFirstChild()));
         }

      }
   else
      {
      //ether realtime or tarok with no wrtbar
      generateRTArrayNodeWithoutFlags(rtArrayCopyTree, dupArraycopyTree, srcRef, dstRef, srcOffRef, dstOffRef, lenRef,1);
      if (comp()->useCompressedPointers())
         {
         TR::TreeTop *prevTree = dupArraycopyTree->getPrevTreeTop();
         TR_ASSERT((dupArraycopyTree->getNode()->getFirstChild()->getChild(0)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the src arraylet pointer");
         TR_ASSERT((dupArraycopyTree->getNode()->getFirstChild()->getChild(1)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the dst arraylet pointer");

         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupArraycopyTree->getNode()->getFirstChild()->getChild(0)->getFirstChild()));
         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupArraycopyTree->getNode()->getFirstChild()->getChild(1)->getFirstChild()));
         }

      }
   createArrayCopyVCallNodeAfterStores(dupVCallTree, srcRef, dstRef, lenRef, srcOffRef, dstOffRef);

   TR::Block *fastArraycopyBlock = dupArraycopyTree->getEnclosingBlock();
   TR::Block *slowArraycopyBlock = dupVCallTree->getEnclosingBlock();

   if (type == TR::Address && comp()->getOptions()->realTimeGC() && doMultiLeafArrayCopy)
      {
      createArrayCopyVCallNodeAfterStores(origDupVCallTree, srcRef, dstRef, lenRef, srcOffRef, dstOffRef);
      slowArraycopyBlock = TR::Block::createEmptyBlock(origDupVCallTree->getNode(), comp(), origCallBlock->getFrequency(), origCallBlock);
      slowArraycopyBlock->append(origDupVCallTree);

      TR::Block *succ = fastArraycopyBlock->getNextBlock();
      slowArraycopyBlock->append(TR::TreeTop::create(comp(), TR::Node::create(origDupVCallTree->getNode(), TR::Goto, 0, succ->getEntry())));

      cfg->addNode(slowArraycopyBlock);
      cfg->findLastTreeTop()->join(slowArraycopyBlock->getEntry());
      cfg->addEdge(TR::CFGEdge::createEdge(slowArraycopyBlock,  succ, trMemory()));
      cfg->copyExceptionSuccessors(fastArraycopyBlock, slowArraycopyBlock);
      }

   if (!origCallBlock->isCold())
      {
      fastArraycopyBlock->setIsCold(false);
      fastArraycopyBlock->setFrequency(origCallBlock->getFrequency());
      }

   if ((type == TR::Address) &&
       (rtArrayCopyTree->_flag & NEED_ARRAYSTORE_CHECK))
      {
      //printf("Need arraystore check\n");fflush(stdout);
      //creating test for instanceof.
      //We know there is no wrtbar needed, so the fast path will be arraycopy with 3 children, while the slow will be arraycopy with 5 children with arrayStore check.

      TR::TreeTop* dupRefArraycopyTree  = TR::TreeTop::create(comp());
      TR::Node* arraycopyNode = dupArraycopyTree->getNode();
      if (arraycopyNode->getOpCodeValue()!= TR::arraycopy)
         arraycopyNode = arraycopyNode->getFirstChild();

      TR::Node* dupRefArraycopyNode;
      if (!comp()->getOptions()->realTimeGC() &&  ((rtArrayCopyTree->_flag & NEED_WRITE_BARRIER) != 0))
         {
         dupRefArraycopyNode = TR::Node::createArraycopy(dupVCallNode->getChild(0)->duplicateTree(), dupVCallNode->getChild(2)->duplicateTree(), arraycopyNode->getChild(2)->duplicateTree(), arraycopyNode->getChild(3)->duplicateTree(), arraycopyNode->getChild(4)->duplicateTree());
         }else
         {//consturct from primitive
         dupRefArraycopyNode = TR::Node::createArraycopy(dupVCallNode->getChild(0)->duplicateTree(), dupVCallNode->getChild(2)->duplicateTree(), arraycopyNode->getChild(0)->duplicateTree(), arraycopyNode->getChild(1)->duplicateTree(), arraycopyNode->getChild(2)->duplicateTree());
         }
      dupRefArraycopyNode->setNumChildren(5);
      dupRefArraycopyNode->setSymbolReference(arraycopyNode->getSymbolReference());
      dupRefArraycopyNode->setFlags(arraycopyNode->getFlags());
      dupRefArraycopyTree->setNode(TR::Node::create(TR::treetop, 1, dupRefArraycopyNode));

      TR::Block *referenceArraycopyBlock = TR::Block::createEmptyBlock(dupRefArraycopyNode, comp(), origCallBlock->getFrequency(), origCallBlock);
      if (!(rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_SRC || rtArrayCopyTree->_flag & NEED_RUNTIME_TEST_FOR_DST))
         {
         referenceArraycopyBlock->setFrequency(UNKNOWN_COLD_BLOCK_COUNT);
         referenceArraycopyBlock->setIsCold();
         }

      referenceArraycopyBlock->append(dupRefArraycopyTree);

      if (comp()->useCompressedPointers())
         {
         TR::TreeTop *prevTree = dupRefArraycopyTree->getPrevTreeTop();
         TR_ASSERT((dupRefArraycopyTree->getNode()->getFirstChild()->getChild(2)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the src arraylet pointer");
         TR_ASSERT((dupRefArraycopyTree->getNode()->getFirstChild()->getChild(3)->getFirstChild()->getOpCodeValue() == TR::aloadi),"didn't find the dst arraylet pointer");

         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupRefArraycopyTree->getNode()->getFirstChild()->getChild(2)->getFirstChild()));
         prevTree = TR::TreeTop::create(comp(), prevTree, TR::Node::createCompressedRefsAnchor(dupRefArraycopyTree->getNode()->getFirstChild()->getChild(3)->getFirstChild()));
         }

      TR::Block *succ = fastArraycopyBlock->getNextBlock();
      referenceArraycopyBlock->append(TR::TreeTop::create(comp(), TR::Node::create(dupArraycopyNode, TR::Goto, 0, succ->getEntry())));

      cfg->addNode(referenceArraycopyBlock);
      cfg->findLastTreeTop()->join(referenceArraycopyBlock->getEntry());
      cfg->addEdge(TR::CFGEdge::createEdge(referenceArraycopyBlock,  succ, trMemory()));
      cfg->copyExceptionSuccessors(fastArraycopyBlock, referenceArraycopyBlock);

      TR::Node* srcObject = TR::Node::createLoad(vcallNode, srcRef);
      TR::Node* dstObject = TR::Node::createLoad(vcallNode, dstRef);
      srcObject = srcObject->createLongIfNeeded();
      dstObject = dstObject->createLongIfNeeded();

      TR::TreeTop *newIfTree = createArrayStoreCompareNode(srcObject, dstObject);
      createAndInsertTestBlock(comp(),newIfTree,dupArraycopyTree,origCallBlock,referenceArraycopyBlock);
      }

   //taking care of dst
   if ( dstOff && ((type == TR::Address) || srcOff ))
      {
      TR::Node *spineShiftNode2 = TR::Node::create(vcallNode, TR::iconst, 0, (int32_t)fe()->getArraySpineShift(elementSize));
      TR::Node *dstOffTemp = TR::Node::createLoad(vcallNode, dstOffRef);
      TR::Node *lenTemp = TR::Node::createLoad(vcallNode, lenRef);
      dstOff = dstOffTemp->createLongIfNeeded();
      len = lenTemp->createLongIfNeeded();
      TR::TreeTop *ifDstTree = buildSameLeafTest(dstOff,len,spineShiftNode2);
      createAndInsertTestBlock(comp(),ifDstTree,dupArraycopyTree,origCallBlock,slowArraycopyBlock);
      }

   //taking care of src if needed
   if ((type == TR::Address) && srcOff && comp()->getOptions()->realTimeGC())
      {
      TR::Node *spineShiftNode2 = TR::Node::create(vcallNode, TR::iconst, 0, (int32_t)fe()->getArraySpineShift(elementSize));
      TR::Node *srcOffTemp = TR::Node::createLoad(vcallNode, srcOffRef);
      TR::Node *lenTemp = TR::Node::createLoad(vcallNode, lenRef);
      srcOff = srcOffTemp->createLongIfNeeded();
      len = lenTemp->createLongIfNeeded();
      TR::TreeTop *ifSrcTree = buildSameLeafTest(srcOff,len,spineShiftNode2);
      createAndInsertTestBlock(comp(), ifSrcTree, dupArraycopyTree,origCallBlock,slowArraycopyBlock);

      }

   //remove original vcall
   vcallTree->getNode()->removeAllChildren();
   vcallTree->getPrevTreeTop()->join(vcallTree->getNextTreeTop());

   uint8_t flag = 0;
   if (doMultiLeafArrayCopy)
      _needMultiLeafArrayCopy.add(new (trStackMemory()) TR_RealTimeArrayCopy(origDupVCallTree, type, flag));
   else
      origDupVCallTree->getNode()->getFirstChild()->setDontTransformArrayCopyCall();

   slowArraycopyBlock->setIsCold(false);
   slowArraycopyBlock->setFrequency(origCallBlock->getFrequency());

   return;
#endif
   }

#ifdef J9_PROJECT_SPECIFIC
void OMR::ValuePropagation::transformRTMultiLeafArrayCopy(TR_RealTimeArrayCopy *rtArrayCopyTree)
   {
   //copied from transformRealTimeArrayCopy
   TR::CFG *cfg = comp()->getFlowGraph();

   TR::TreeTop *vcallTree = rtArrayCopyTree->_treetop;
   TR::Node *vcallNode = vcallTree->getNode()->getFirstChild();

   //maybe the call node was removed.
   if (vcallNode->getReferenceCount() < 1)
      return;

   if (trace())
      traceMsg(comp(), "Transforming multi-leaf array copy: %p\n", vcallNode);

   TR::TreeTop *prevTree = vcallTree->getPrevTreeTop();
   TR::DataType type = rtArrayCopyTree->_type;
   intptrj_t elementSize = TR::Symbol::convertTypeToSize(type);
   intptrj_t leafSize = comp()->fe()->getArrayletMask(elementSize) + 1;

   TR_ResolvedMethod *method = comp()->getCurrentMethod();
   TR::ResolvedMethodSymbol *methodSymbol = comp()->getOwningMethodSymbol(method);
   TR_OpaqueClassBlock *helperClass = comp()->getSystemClassPointer(); //fe()->getClassFromSignature("RTArrayCopy", 11, method);

   if (trace())
      traceMsg(comp(), " class = %p\n", helperClass);
   if (!helperClass)
     return;

   TR_ScratchList<TR_ResolvedMethod> helperMethods(trMemory());
   comp()->fej9()->getResolvedMethods(trMemory(), helperClass, &helperMethods);
   ListIterator<TR_ResolvedMethod> it(&helperMethods);
   TR::SymbolReference *helperSymRef = NULL;
   for (TR_ResolvedMethod *m = it.getCurrent(); m && !helperSymRef; m = it.getNext())
      {
      char *sig = m->nameChars();
      if (trace())
         traceMsg(comp(), " sig = %s\n", sig);

      if (!strncmp(sig, "multiLeafArrayCopy", 18))
         helperSymRef = getSymRefTab()->findOrCreateMethodSymbol(JITTED_METHOD_INDEX, -1, m, TR::MethodSymbol::Static);
      }

   if (trace())
      traceMsg(comp(), " helper sym = %p\n", helperSymRef);
   if (!helperSymRef) return;



   TR::Node *extraChild1 = TR::Node::iconst(vcallNode, elementSize);
   TR::Node *extraChild2 = TR::Node::iconst(vcallNode, leafSize);
   TR::Node *newCallNode = TR::Node::createWithSymRef(TR::call, 7, 7,
                                          vcallNode->getChild(0),
                                          vcallNode->getChild(1),
                                          vcallNode->getChild(2),
                                          vcallNode->getChild(3),
                                          vcallNode->getChild(4),
                                          extraChild1,
                                          extraChild2,
                                          helperSymRef);
   TR::TransformUtil::removeTree(comp(), vcallTree);
   TR::TreeTop *newCallTree = TR::TreeTop::create(comp(), prevTree,
         TR::Node::create(TR::treetop, 1, newCallNode));

   _multiLeafCallsToInline.add(newCallTree);
   if (comp()->getMethodHotness() >= warm)
      requestOpt(OMR::globalValuePropagation);
   }
#endif

static
const char* transformedTargetName (TR::RecognizedMethod rm)
   {
   switch ( rm )
      {
#ifdef J9_PROJECT_SPECIFIC
      case TR::sun_nio_cs_UTF_16_Encoder_encodeUTF16Big:
         return "icall  com/ibm/jit/JITHelpers.transformedEncodeUTF16Big(JJI)I";

      case TR::sun_nio_cs_UTF_16_Encoder_encodeUTF16Little:
         return "icall  com/ibm/jit/JITHelpers.transformedEncodeUTF16Little(JJI)I"  ;
#endif

      default:
         return "arraytranslate";
      }
   }

#ifdef J9_PROJECT_SPECIFIC
void OMR::ValuePropagation::transformObjectCloneCall(TR::TreeTop *callTree, OMR::ValuePropagation::ObjCloneInfo *cloneInfo)
   {
   TR_OpaqueClassBlock *j9class = cloneInfo->_clazz;
   static char *disableObjectCloneOpt = feGetEnv("TR_disableFastObjectClone");
   if (disableObjectCloneOpt)
      return;

   TR::Node *callNode = callTree->getNode()->getFirstChild();
   TR::Node *objNode = callNode->getFirstChild();

   TR::SymbolReference * symRef = callNode->getSymbolReference();
   TR::ResolvedMethodSymbol *method = symRef->getSymbol()->getResolvedMethodSymbol();
   if (method->getRecognizedMethod() == TR::java_lang_J9VMInternals_primitiveClone)
      objNode = callNode->getLastChild();

   if (!performTransformation(comp(), "%sInlining object clone call [%p] as new object and JITHelpers object copy\n", OPT_DETAILS, callNode))
      return;

   TR::DebugCounter::prependDebugCounter(comp(), TR::DebugCounter::debugCounterName(comp(), "inlineClone.location/object/(%s)", comp()->signature()), callTree);
   int32_t classNameLength;
   char *className = TR::Compiler->cls.classNameChars(comp(), j9class, classNameLength);
   TR::DebugCounter::prependDebugCounter(comp(), TR::DebugCounter::debugCounterName(comp(), "inlineClone.type/(%s)/(%s)/%s", className, comp()->signature(), comp()->getHotnessName(comp()->getMethodHotness())), callTree);

    // Preserve children for callNode
    anchorAllChildren(callNode, callTree);
    prepareToReplaceNode(callNode); // This will remove the usedef info, valueNumber info and all children of the node

    if (callTree->getNode()->getOpCode().isCheck())
      {
      TR_ASSERT(callTree->getNode()->getOpCodeValue() == TR::NULLCHK, "it only makes sense to do this object clone transform for checks with a NULLCHK - how did we get here otherwise??");
      TR::Node *passthrough = TR::Node::create(callNode, TR::PassThrough, 1, objNode);
      callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::createWithSymRef(callNode, callTree->getNode()->getOpCodeValue(), 1, passthrough, callTree->getNode()->getSymbolReference())));
      TR::Node::recreate(callTree->getNode(), TR::treetop);
      }

   if (!cloneInfo->_isFixed)
      {
      // split the block ahead of callTree
      // fall through will be fast case of inline clone
      // slow path from an inserted if will be a call to the VM clone
      TR::CFG *cfg = comp()->getFlowGraph();
      TR::Block *block = callTree->getEnclosingBlock();
      TR::Block *fastPath = block->split(callTree, cfg, true, true);
      TR::Block *remainder = fastPath->split(callTree->getNextTreeTop(), cfg, true, true);

      TR_BlockCloner *cloner = new (trStackMemory())TR_BlockCloner(cfg);
      TR::Block *slowPath = cloner->cloneBlocks(fastPath, fastPath);
      slowPath->append(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::Goto, 0, remainder->getEntry())));
      cfg->findLastTreeTop()->join(slowPath->getEntry());

      TR::TreeTop *compareTree =
         TR::TreeTop::create(comp(), TR::Node::createif(TR::ifacmpne,
            TR::Node::createWithSymRef(callNode, TR::aloadi, 1, objNode, comp()->getSymRefTab()->findOrCreateVftSymbolRef()),
            TR::Node::createWithSymRef(callNode, TR::loadaddr, 0, comp()->getSymRefTab()->findOrCreateClassSymbol(callNode->getSymbolReference()->getOwningMethodSymbol(comp()), 0, j9class)),
            slowPath->getEntry()));

      block->append(compareTree);

      cfg->setStructure(NULL);
      cfg->addEdge(TR::CFGEdge::createEdge(block, slowPath, trMemory()));
      cfg->addEdge(TR::CFGEdge::createEdge(slowPath, remainder, trMemory()));
      cfg->copyExceptionSuccessors(fastPath, slowPath);

      callTree = fastPath->getFirstRealTreeTop();
      }

   TR::Node *loadaddr = TR::Node::createWithSymRef(callNode, TR::loadaddr, 0, comp()->getSymRefTab()->findOrCreateClassSymbol(callNode->getSymbolReference()->getOwningMethodSymbol(comp()), 0, j9class));
   TR::Node::recreateWithoutProperties(callNode, TR::New, 1, comp()->getSymRefTab()->findOrCreateNewObjectSymbolRef(objNode->getSymbolReference()->getOwningMethodSymbol(comp())));
   callNode->setAndIncChild(0, loadaddr);

   TR::SymbolReference* helperAccessor =
         comp()->getSymRefTab()->methodSymRefFromName(
            comp()->getMethodSymbol(),
            "com/ibm/jit/JITHelpers",
            const_cast<char*>("jitHelpers"),
            const_cast<char*>("()Lcom/ibm/jit/JITHelpers;"),
            TR::MethodSymbol::Static);
   TR::SymbolReference* objCopySymRef =
         comp()->getSymRefTab()->methodSymRefFromName(
            comp()->getMethodSymbol(),
            "com/ibm/jit/JITHelpers",
            const_cast<char*>(TR::Compiler->target.is64Bit() ? "unsafeObjectShallowCopy64" : "unsafeObjectShallowCopy32"),
            const_cast<char*>(TR::Compiler->target.is64Bit() ? "(Ljava/lang/Object;Ljava/lang/Object;J)V" : "(Ljava/lang/Object;Ljava/lang/Object;I)V"),
            TR::MethodSymbol::Static);
   TR::Node *getHelpers = TR::Node::createWithSymRef(callNode, TR::acall, 0, helperAccessor);
   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, getHelpers)));
   TR::Node *objCopy = TR::Node::createWithSymRef(callNode, TR::call, 4, objCopySymRef);
   objCopy->setAndIncChild(0, getHelpers);
   objCopy->setAndIncChild(1, objNode);
   objCopy->setAndIncChild(2, callNode);
   objCopy->setAndIncChild(3, TR::Node::create(callNode, (TR::Compiler->target.is64Bit() ? TR::a2l : TR::a2i), 1, loadaddr));

   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, callNode)));
   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, objCopy)));

   return;
   }

void OMR::ValuePropagation::transformArrayCloneCall(TR::TreeTop *callTree, OMR::ValuePropagation::ArrayCloneInfo *cloneInfo)
   {
   static char *disableArrayCloneOpt = feGetEnv("TR_disableFastArrayClone");
   if (disableArrayCloneOpt || TR::Compiler->om.canGenerateArraylets())
      return;

   TR::Node *callNode = callTree->getNode()->getFirstChild();
   TR::Node *objNode = callNode->getFirstChild();

   TR::SymbolReference * symRef = callNode->getSymbolReference();
   TR::ResolvedMethodSymbol *method = symRef->getSymbol()->getResolvedMethodSymbol();

   if (method->getRecognizedMethod() == TR::java_lang_J9VMInternals_primitiveClone)
      objNode = callNode->getLastChild();

   if (!performTransformation(comp(), "%sInlining array clone call [%p] as new array and arraycopy\n", OPT_DETAILS, callNode))
      return;

   TR_OpaqueClassBlock *j9arrayClass = cloneInfo->_clazz;
   bool isFixedClass = cloneInfo->_isFixed;

   TR_OpaqueClassBlock *j9class = comp()->fe()->getComponentClassFromArrayClass(j9arrayClass);

   TR::DebugCounter::prependDebugCounter(comp(), TR::DebugCounter::debugCounterName(comp(), "inlineClone.location/array/(%s)", comp()->signature()), callTree);
   int32_t classNameLength;
   char *className = TR::Compiler->cls.classNameChars(comp(), j9arrayClass, classNameLength);
   TR::DebugCounter::prependDebugCounter(comp(), TR::DebugCounter::debugCounterName(comp(), "inlineClone.type/(%s)/(%s)/%s", className, comp()->signature(), comp()->getHotnessName(comp()->getMethodHotness())), callTree);
   TR::Node *lenNode = TR::Node::create(callNode, TR::arraylength, 1, objNode);
    // Preserve children for callNode
    anchorAllChildren(callNode, callTree);
    prepareToReplaceNode(callNode); // This will remove the usedef info, valueNumber info and all children of the node

   if (callTree->getNode()->getOpCode().isCheck())
      {
      TR_ASSERT(callTree->getNode()->getOpCodeValue() == TR::NULLCHK, "it only makes sense to do this array clone transform for checks with a NULLCHK - how did we get here otherwise??");
      TR::Node *passthrough = TR::Node::create(callNode, TR::PassThrough, 1, objNode);
      callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::createWithSymRef(callNode, callTree->getNode()->getOpCodeValue(), 1, passthrough, callTree->getNode()->getSymbolReference())));
      TR::Node::recreate(callTree->getNode(), TR::treetop);
      }

   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, lenNode)));

   if (TR::Compiler->cls.isPrimitiveClass(comp(), j9class))
      {
      TR::Node *typeConst = TR::Node::iconst(callNode, comp()->fe()->getNewArrayTypeFromClass(j9arrayClass));
      static char *disableSkipZeroInitInVP = feGetEnv("TR_disableSkipZeroInitInVP");
      if (disableSkipZeroInitInVP)
         {
         TR::SymbolReference *symRef = comp()->getSymRefTab()->findOrCreateNewArraySymbolRef(objNode->getSymbolReference()->getOwningMethodSymbol(comp()));
         TR::Node::recreateWithoutProperties(callNode, TR::newarray, 2, lenNode, typeConst, symRef);
         }
      else
         {
         TR::SymbolReference *symRef = comp()->getSymRefTab()->findOrCreateNewArrayNoZeroInitSymbolRef(objNode->getSymbolReference()->getOwningMethodSymbol(comp()));
         TR::Node::recreateWithoutProperties(callNode, TR::newarray, 2, lenNode, typeConst, symRef);
         callNode->setCanSkipZeroInitialization(true);
         }
      }
   else
      {
      TR::Node *classNode;
      if (isFixedClass)
         {
         classNode = TR::Node::createWithSymRef(callNode, TR::loadaddr, 0, comp()->getSymRefTab()->findOrCreateClassSymbol(callNode->getSymbolReference()->getOwningMethodSymbol(comp()), 0, j9class));
         }
      else
         {
         // Load the component class of the cloned array as 2nd child of anewarray as expected by the opcode.
         TR::Node * arrayClassNode = TR::Node::createWithSymRef(callNode, TR::aloadi, 1, objNode, comp()->getSymRefTab()->findOrCreateVftSymbolRef());
         classNode = TR::Node::createWithSymRef(callNode, TR::aloadi, 1, arrayClassNode, comp()->getSymRefTab()->findOrCreateArrayComponentTypeSymbolRef());
         }
      TR::SymbolReference *symRef = comp()->getSymRefTab()->findOrCreateANewArraySymbolRef(objNode->getSymbolReference()->getOwningMethodSymbol(comp()));
      TR::Node::recreateWithoutProperties(callNode, TR::anewarray, 2, lenNode, classNode, symRef);
      }
   TR::Node *newArray = callNode;
   newArray->setIsNonNull(true);

   int32_t elementSize = TR::Compiler->om.getSizeOfArrayElement(newArray);
   TR::Node *lengthInBytes = TR::Compiler->target.is64Bit() ?
      TR::Node::create(callNode, TR::lmul, 2, TR::Node::create(callNode, TR::i2l, 1, lenNode), TR::Node::lconst(lenNode, elementSize)) :
      TR::Node::create(callNode, TR::imul, 2, lenNode, TR::Node::iconst(lenNode, elementSize));


   TR::Node *srcStart = TR::Compiler->target.is64Bit() ?
      TR::Node::create(callNode, TR::aladd, 2, objNode, TR::Node::lconst(objNode, TR::Compiler->om.contiguousArrayHeaderSizeInBytes())) :
      TR::Node::create(callNode, TR::aiadd, 2, objNode, TR::Node::iconst(objNode, TR::Compiler->om.contiguousArrayHeaderSizeInBytes()));
   TR::Node *destStart = TR::Compiler->target.is64Bit() ?
      TR::Node::create(callNode, TR::aladd, 2, newArray, TR::Node::lconst(newArray, TR::Compiler->om.contiguousArrayHeaderSizeInBytes())) :
      TR::Node::create(callNode, TR::aiadd, 2, newArray, TR::Node::iconst(newArray, TR::Compiler->om.contiguousArrayHeaderSizeInBytes()));
   TR::Node *arraycopy = NULL;
   if (TR::Compiler->cls.isPrimitiveClass(comp(), j9class))
      arraycopy = TR::Node::createArraycopy(srcStart, destStart, lengthInBytes);
   else
      arraycopy = TR::Node::createArraycopy(objNode, newArray, srcStart, destStart, lengthInBytes);

   arraycopy->setByteCodeInfo(callNode->getByteCodeInfo());
   arraycopy->getByteCodeInfo().setDoNotProfile(1);
   arraycopy->setNoArrayStoreCheckArrayCopy(true);
   arraycopy->setForwardArrayCopy(true);
   if (TR::Compiler->cls.isPrimitiveClass(comp(), j9class))
      {
      switch (elementSize)
         {
         case 1: arraycopy->setArrayCopyElementType(TR::Int8); break;
         case 2: arraycopy->setArrayCopyElementType(TR::Int16); break;
         case 4: arraycopy->setArrayCopyElementType(TR::Int32); break;
         case 8: arraycopy->setArrayCopyElementType(TR::Int64); break;
         }

      switch (elementSize)
         {
         case 2:
            arraycopy->setHalfWordElementArrayCopy(true);
            break;

         case 4:
         case 8:
            arraycopy->setWordElementArrayCopy(true);
            break;
         }
      }
   else
      {
      arraycopy->setArrayCopyElementType(TR::Address);
      arraycopy->setWordElementArrayCopy(true);
      }

   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, newArray)));
   callTree->insertBefore(TR::TreeTop::create(comp(), TR::Node::create(callNode, TR::treetop, 1, arraycopy)));
   }

void OMR::ValuePropagation::transformConverterCall(TR::TreeTop *callTree)
   {

   if (callTree->getEnclosingBlock()->isCold()||
         (callTree->getEnclosingBlock()->getFrequency() == UNKNOWN_COLD_BLOCK_COUNT))
      return;

   //maybe the call node was removed.
   TR::Node *callNode = callTree->getNode()->getFirstChild();
   if (callNode->getReferenceCount() < 1)
      return;

   TR::MethodSymbol *symbol = callNode->getSymbol()->castToMethodSymbol();
   TR::RecognizedMethod rm = symbol->getRecognizedMethod();



   if (!performTransformation(comp(), "%sChanging call %s [%p] to %s \n", OPT_DETAILS, callNode->getOpCode().getName(), callNode, transformedTargetName(rm)))
      return;

   //bool is64BitTarget = TR::Compiler->target.is64Bit() ? true : false;
   TR::CFG *cfg = comp()->getFlowGraph();

    //dup call
   TR::Block *origCallBlock = callTree->getEnclosingBlock();
   TR::TreeTop *dupCallTree = TR::TreeTop::create(comp());

   TR::Node* dupCallNode = callTree->getNode()->duplicateTree();
   dupCallTree->setNode(dupCallNode);
   if (!(dupCallNode->getOpCode().isCall()))
      dupCallNode = dupCallNode->getFirstChild();

   dupCallNode->removeAllChildren();

   TR::TreeTop* arrayTranslateTree  = TR::TreeTop::create(comp());
   TR::Node* arrayTranslateNode = callTree->getNode()->duplicateTree();
   arrayTranslateTree->setNode(arrayTranslateNode);
   if (!(arrayTranslateNode->getOpCode().isCall()))
      arrayTranslateNode = arrayTranslateNode->getFirstChild();

   arrayTranslateNode->removeAllChildren();
   TR::Node * srcObjNode = NULL;
   TR::Node * dstObjNode = NULL;
   TR::Node * len = NULL;
   TR::Node * srcOff = NULL;
   TR::Node * dstOff = NULL;
   TR::Node * tableNode = NULL;
   bool hasTable = false;

   TR_ResolvedMethod *m = symbol->getResolvedMethodSymbol()->getResolvedMethod();
   bool isISO88591Encoder = (rm == TR::sun_nio_cs_ISO_8859_1_Encoder_encodeISOArray
                             || rm == TR::java_lang_StringCoding_implEncodeISOArray);
   int32_t childId = callNode->getFirstArgumentIndex();
   bool hasReciever = symbol->isStatic() ? false : true;
   if (hasReciever)
       childId++;



   srcObjNode = callNode->getChild(childId++);
   srcOff = callNode->getChild(childId++);//->createLongIfNeeded();


   if (!isISO88591Encoder)
      len = callNode->getChild(childId++);//->createLongIfNeeded();

   dstObjNode = callNode->getChild(childId++);
   dstOff = callNode->getChild(childId++);

   if (isISO88591Encoder)
      len = callNode->getChild(childId++);//->createLongIfNeeded();

   if ( (rm == TR::sun_nio_cs_ext_SBCS_Encoder_encodeSBCS) ||
         (rm == TR::sun_nio_cs_ext_SBCS_Decoder_decodeSBCS))
      {
      hasTable = true;
      tableNode = callNode->getChild(childId++);
      }


   dumpOptDetails(comp(), "Insert runtime tests for converter call transformation to arraytranslate (%p)\n", callNode);

   TR::Node *child1 = TR::Node::create(TR::iadd, 2, srcOff, len);
   TR::Node *child2 = TR::Node::create(TR::arraylength, 1, srcObjNode);
   // the source for the decompressed string version of this in StringCoding takes a byte array
   // used as a char array so convert the size appropriately for the check >> 1
   if (rm == TR::java_lang_StringCoding_implEncodeISOArray)
      child2 = TR::Node::create(TR::ishr, 2, child2, TR::Node::iconst(child2, 1));

   TR::TreeTop *ifTree = TR::TreeTop::create(comp());
   TR::Node *ifNode = TR::Node::createif(TR::ificmpgt, child1, child2);
   ifTree->setNode(ifNode);

   TR::SymbolReference *srcRef = NULL, *dstRef = NULL, *thisRef = NULL;
   TR::SymbolReference *srcOffRef = NULL, *dstOffRef = NULL;
   TR::SymbolReference *lenRef = NULL;
   TR::SymbolReference *tableRef = NULL;

   // Compute arguments and store them before the call
   createStoresForConverterCallChildren(comp(), callTree, srcRef, dstRef, srcOffRef, dstOffRef, lenRef, thisRef, callTree);
   if (hasTable)
      tableNode->createStoresForVar(tableRef,callTree, true);

   //create arraytranslate and cold call
   generateArrayTranslateNode(callTree, arrayTranslateTree, srcRef, dstRef, srcOffRef, dstOffRef, lenRef,tableRef,hasTable);
   createConverterCallNodeAfterStores(dupCallTree, callTree,srcRef, dstRef, lenRef, srcOffRef, dstOffRef,thisRef,tableRef);
   arrayTranslateNode = arrayTranslateTree->getNode()->getFirstChild();

   // transmute original call node to iload of a temporary (where we will store the result),
   // so that any references to it pick up the result regardless of execution path
   TR::SymbolReference *symRefConvert =
      getSymRefTab()->createTemporary(comp()->getMethodSymbol(), callNode->getDataType(), false);

   TR::Node::recreate(callNode, TR::iload);
   callNode->setSymbolReference(symRefConvert);
   callNode->setFlags(0);
   callNode->removeAllChildren();

   //split blocks
   origCallBlock->createConditionalBlocksBeforeTree(callTree, ifTree, dupCallTree, arrayTranslateTree, cfg, false);
   origCallBlock->split(ifTree, cfg, true, true);

   TR::Block *fastArraytranslateBlock = arrayTranslateTree->getEnclosingBlock();
   TR::Block *slowArraytranslateBlock = dupCallTree->getEnclosingBlock();

   //create Store of arraytranslate result
   TR::Node *storeArrayTranslate = TR::Node::createWithSymRef(TR::istore, 1, 1, arrayTranslateNode, symRefConvert);
   arrayTranslateTree->insertAfter(TR::TreeTop::create(comp(), storeArrayTranslate, NULL, NULL));

   //create store of cold call result
   TR::Node *storeCall =  TR::Node::createWithSymRef(TR::istore, 1, 1, dupCallTree->getNode()->getFirstChild(), symRefConvert);
   dupCallTree->insertAfter(TR::TreeTop::create(comp(), storeCall, NULL, NULL));

   //The async check is inserted because the converter call might have been the only
   //yield point in a loop due to asyn check removel.
   if (comp()->getOSRMode() != TR::involuntaryOSR && comp()->getHCRMode() != TR::osr)
      {
      TR::TreeTop *actt = TR::TreeTop::create(comp(), TR::Node::createWithSymRef(callNode, TR::asynccheck, 0,
                                          getSymRefTab()->findOrCreateAsyncCheckSymbolRef
                                          (comp()->getMethodSymbol())));
      fastArraytranslateBlock->append(actt);
      }

   fastArraytranslateBlock->setIsCold(false);
   fastArraytranslateBlock->setFrequency(origCallBlock->getFrequency());

   slowArraytranslateBlock->setIsCold(true);
   slowArraytranslateBlock->setFrequency(UNKNOWN_COLD_BLOCK_COUNT);

   //building the second test
   TR::Node *dstOffTemp = TR::Node::createLoad(callNode, dstOffRef);
   TR::Node *lenTemp = TR::Node::createLoad(callNode, lenRef);
   TR::Node *dstTemp = TR::Node::createLoad(callNode, dstRef);
   TR::Node *child1b = TR::Node::create( TR::iadd, 2, dstOffTemp, lenTemp);
   TR::Node *child2b = TR::Node::create(TR::arraylength, 1, dstTemp);

   TR::TreeTop *ifTreeb = TR::TreeTop::create(comp(), TR::Node::createif(TR::ificmpgt, child1b, child2b));
   createAndInsertTestBlock(comp(),ifTreeb,callTree, origCallBlock,slowArraytranslateBlock);

   // Null check on source
   TR::TreeTop *ifTreec = TR::TreeTop::create(comp(), TR::Node::createif(TR::ifacmpeq, TR::Node::createLoad(callNode, srcRef), TR::Node::aconst(callNode,0)));
   createAndInsertTestBlock(comp(), ifTreec, callTree, origCallBlock, slowArraytranslateBlock);

   // Null check on desitantion
   TR::TreeTop *ifTreed = TR::TreeTop::create(comp(), TR::Node::createif(TR::ifacmpeq, TR::Node::createLoad(callNode, dstRef), TR::Node::aconst(callNode,0)));
   createAndInsertTestBlock(comp(), ifTreed, callTree, origCallBlock, slowArraytranslateBlock);

   if (TR::Compiler->target.cpu.isZ())
      {
      // Task 110060:
      //
      // Threshold represents the length of the input array for which executing the Java method is
      // faster than the hardware accelerated arraytranslate. The constants used below are deduced
      // from an empirical study of all 11 converters on Z.
      //
      // It is also noteworthy to say that we use ifacmpeq and ificmpgt trees to perform the nullchecks
      // and the "bound checks" because the actual implementation in Java translates the arrays on an
      // element by element basis. This means if an ArrayIndexOutOfBounds were to occur we would have
      // an intermediate translation result. This cannot be achieved if we have a pre-emptive BNDCHK
      // node before the arraytranslate tree. Thus we must fall back to the Java implementation in
      // such cases.

      int32_t threshold;

      switch (rm)
         {
         case TR::sun_nio_cs_ISO_8859_1_Encoder_encodeISOArray:  threshold = 0; break;
         case TR::java_lang_StringCoding_implEncodeISOArray:  threshold = 0; break;
         case TR::sun_nio_cs_ISO_8859_1_Decoder_decodeISO8859_1: threshold = 0; break;

         case TR::sun_nio_cs_US_ASCII_Encoder_encodeASCII: threshold = 0; break;
         case TR::sun_nio_cs_US_ASCII_Decoder_decodeASCII: threshold = 0; break;

         case TR::sun_nio_cs_ext_SBCS_Encoder_encodeSBCS: threshold = 11; break;
         case TR::sun_nio_cs_ext_SBCS_Decoder_decodeSBCS: threshold = 0; break;

         case TR::sun_nio_cs_UTF_8_Decoder_decodeUTF_8: threshold = 0; break;
         case TR::sun_nio_cs_UTF_8_Encoder_encodeUTF_8: threshold = 0; break;

         default: threshold = 0; break;
         }

      static const char* overrideThreshold = feGetEnv("TR_ArrayTranslateOverrideThreshold");

      if (overrideThreshold)
      {
          threshold = atoi(overrideThreshold);

          traceMsg(comp(), "Overriding arraytranslate fallback threshold to %d\n", threshold);
      }

      if (threshold > 0)
         {
         TR::TreeTop* thresholdTestTree = TR::TreeTop::create(comp(), TR::Node::createif(TR::ificmplt, TR::Node::createLoad(callNode, lenRef), TR::Node::create(TR::iconst, 0, threshold)));
         createAndInsertTestBlock(comp(), thresholdTestTree, callTree, origCallBlock, slowArraytranslateBlock);

         TR_InlineCall inlineFallback(optimizer(), this);
         // Inline the duplicate call as this block is potentially not cold depending on input string length
         inlineFallback.inlineCall(dupCallTree);
         }
      }

   //Hybrid support
   if (comp()->requiresSpineChecks())
      {
      // TODO: skip spine check if copy is provably contiguous
      TR::TreeTop *ifTreeSrcContig = createSpineCheckNode(callNode, srcRef);
      createAndInsertTestBlock(comp(),ifTreeSrcContig,callTree, origCallBlock,slowArraytranslateBlock);

      TR::TreeTop *ifTreeDstContig = createSpineCheckNode(callNode, dstRef);
      createAndInsertTestBlock(comp(),ifTreeDstContig, callTree, origCallBlock, slowArraytranslateBlock);
      }

     if (trace())
         comp()->dumpMethodTrees("Trees after reducing converter call to intrinsic arraytranslate");

   return;
   }
#endif


//----------------------------------------------------------------

TR::LocalValuePropagation::LocalValuePropagation(TR::OptimizationManager *manager)
   : TR::ValuePropagation(manager)
   {
   _isGlobalPropagation = false;
   }

int32_t TR::LocalValuePropagation::perform()
   {
   if ((_firstUnresolvedSymbolValueNumber - 1) <= comp()->getNodeCount())
      {
      dumpOptDetails(comp(),
         "Can't do Local Value Propagation - too many nodes\n");
      }
   else
      {
      // Walk the trees and process each block
      //
      TR::TreeTop *treeTop = comp()->getStartTree();
      while (treeTop)
         {
         TR_ASSERT(treeTop->getNode()->getOpCodeValue() == TR::BBStart, "assertion failure");
         treeTop = processBlock(treeTop);
         if (_reachedMaxRelationDepth)
            break;
         }
      }

   return 1;
   }




int32_t TR::LocalValuePropagation::performOnBlock(TR::Block *block)
   {
   // Walk the trees and process
   //
   if ((_firstUnresolvedSymbolValueNumber - 1) <= comp()->getNodeCount())
      {
      dumpOptDetails(comp(),
         "Can't do Local Value Propagation on block %d - too many nodes\n",
         block->getNumber());
      }
   else
      {
      TR::TreeTop *treeTop = block->getEntry();
      while (treeTop)
         {
         TR_ASSERT(treeTop->getNode()->getOpCodeValue() == TR::BBStart, "assertion failure");
         TR::Block *blk = treeTop->getNode()->getBlock();

         if ((blk != block) &&
             !blk->isExtensionOfPreviousBlock())
            break;

         treeTop = processBlock(treeTop);
         if (_reachedMaxRelationDepth)
            break;
         }
      }

   return 0;
   }


void TR::LocalValuePropagation::prePerformOnBlocks()
   {
   TR::CFG *cfg = comp()->getFlowGraph();


   // Can't do this analysis if there is no CFG or no use/def info or no
   // value numbers
   //
   if (!cfg)
      {
      dumpOptDetails(comp(), "Can't do Local Value Propagation - there is no CFG\n");
      return;
      }
   _useDefInfo      = NULL;
   _valueNumberInfo = NULL;

   _bestRun = comp()->getMethodHotness() <= cold; // && getLastRun(); getLastRun doesn't work well yet for VP

   if (trace())
      {
      comp()->dumpMethodTrees("Trees before Local Value Propagation");
      }

   initialize();

   setIntersectionFailed(false);
   }


void TR::LocalValuePropagation::postPerformOnBlocks()
   {
   // Perform transformations that were delayed until the end of the analysis
   //
   doDelayedTransformations();

   if (_enableVersionBlocks)
      versionBlocks();

   // Enable other optimization passes which may now be useful
   //
   if (enableSimplifier())
      {
      requestOpt(OMR::treeSimplification);
      requestOpt(OMR::basicBlockExtension);
      }

   if (checksWereRemoved())
      requestOpt(OMR::catchBlockRemoval);


   if (trace())
      comp()->dumpMethodTrees("Trees after Local Value Propagation");

   // Invalidate usedef and value number information if necessary
   //
   if (useDefInfoInvalid() && optimizer()->getUseDefInfo())
      optimizer()->setUseDefInfo(NULL);
   if (valueNumberInfoInvalid() && optimizer()->getValueNumberInfo())
      optimizer()->setValueNumberInfo(NULL);

   }

TR::TreeTop *TR::LocalValuePropagation::processBlock(TR::TreeTop *startTree)
   {
   _constNodeInfo.MakeEmpty();
   TR::Node *node = startTree->getNode();
   _curBlock     = node->getBlock();

   if (_curBlock->isOSRCodeBlock() || _curBlock->isOSRCatchBlock() || _curBlock->isOSRInduceBlock())
      return _curBlock->getExit()->getNextTreeTop();

#if DEBUG
   static int32_t stopAtBlock = -1;
   if (_curBlock->getNumber() == stopAtBlock)
      {
      dumpOptDetails(comp(), "Stop here\n");
      }
#endif

   if (trace())
      traceMsg(comp(), "\nStarting block_%d\n", _curBlock->getNumber());

   // Go through the trees finding constraints
   //
   _lastTimeThrough = true;
   _booleanNegationInfo.setFirst(NULL);
   freeValueConstraints(_curConstraints);
   getParmValues();

   if (comp()->getOption(TR_EnableLocalVPSkipLowFreqBlock))
      {
      if (!((((comp()->getMethodHotness() < warm)  && (_curBlock->getFrequency() > 1500)) ||
             ((comp()->getMethodHotness() == warm) && (_curBlock->getFrequency() > 500)) ||
             ((comp()->getMethodHotness() > warm)  && (!_curBlock->isCold())))))
         {
         if (trace())
            traceMsg(comp(), "\nSkipping block_%d (low frequency)\n", _curBlock->getNumber());

         _curBlock = startTree->getExtendedBlockExitTreeTop()->getNode()->getBlock();
         TR::TreeTop *tt = _curBlock->getExit()->getNextTreeTop();
         if (tt)
            _curBlock = _curBlock->getNextBlock();
         return tt;
         }
      }

   while(1)
      {
      TR::TreeTop *endTree = _curBlock->getExit();
      processTrees(startTree, endTree);

      if (_reachedMaxRelationDepth)
         break;

      // Look for block extension
      //
      startTree = endTree->getNextTreeTop();
      if (!startTree)
         break;
      node = startTree->getNode();
      TR_ASSERT(node->getOpCodeValue() == TR::BBStart, "assertion failure");
      _curBlock = node->getBlock();
      if (!_curBlock->isExtensionOfPreviousBlock())
         break;

      // This is an extended block, carry the current constraints through
      // to it.
      // If the extended block is unreachable, remove it.
      //
      if (isUnreachablePath(_curConstraints))
         {
         if (trace())
            traceMsg(comp(), "\nSkipping unreachable block_%d (extension of previous block)\n", _curBlock->getNumber());

         _blocksToBeRemoved->add(_curBlock);
         startTree = _curBlock->getExit();
         continue;
         }

      if (trace())
         traceMsg(comp(), "\nStarting block_%d (extension of previous block)\n", _curBlock->getNumber());
      }

   return startTree;
   }

const char *
TR::LocalValuePropagation::optDetailString() const throw()
   {
   return "O^O LOCAL VALUE PROPAGATION: ";
   }

void OMR::ValuePropagation::launchNode(TR::Node *node, TR::Node *parent, int32_t whichChild)
   {
   TR::Node *newNode = node;
   if (node && node->getVisitCount() != _visitCount)
      {
      int32_t valueNumber = getValueNumber(node);
      TR_ASSERT(valueNumber < _firstUnresolvedSymbolValueNumber, "value number too big, valueNumber:%d _firstUnresolvedSymbolValueNumber:%d", valueNumber, _firstUnresolvedSymbolValueNumber);

      TR::Node *oldParent = _parentNode;
      _parentNode = parent;
      node->setVisitCount(_visitCount);
      ValuePropagationPtr handler = constraintHandlers[node->getOpCodeValue()];
      if (handler)
         {
         newNode = handler(this, node);

         // Replace the node in its parent if it has changed
         //
         if (newNode != node)
            {
            if (parent)
               parent->setChild(whichChild, newNode);
            else
               _curTree->setNode(newNode);
            }
         }
      _parentNode = oldParent;

      if (_enableVersionBlocks && !_disableVersionBlockForThisBlock && lastTimeThrough())
         collectDefSymRefs(newNode,parent);

      if (_isGlobalPropagation)
         {
         // Special handling for def nodes.
         // If this is a store set up store constraints for it.
         // Last time through, remember the loop that contains def nodes in the
         // loop defs table.
         //
         uint16_t useDefIndex = node->getUseDefIndex();
         if (useDefIndex && _useDefInfo->isDefIndex(useDefIndex))
            {
            if (node->getOpCode().isStore())
               createStoreConstraints(node);

            if (lastTimeThrough() && _loopInfo)
               {
               LoopDefsHashTableEntry *entry = findLoopDef(node);
               if (entry)
                  entry->region = _loopInfo->_loop;
               }
            }
         }
      }

   // Replace PassThrough with its child
   if (newNode
       && newNode->getOpCodeValue() == TR::PassThrough)
      {
      TR_ASSERT(parent, "PassThrough newNode %p must have a parent", newNode);
      if (!parent->getOpCode().isNullCheck()
          && performTransformation(comp(), "%sReplace PassThrough node [%p] with its child in its parent [%p]\n", OPT_DETAILS, newNode, parent))
         {
         TR::Node* child = newNode->getFirstChild();
         parent->setAndIncChild(whichChild, child);
         newNode->recursivelyDecReferenceCount();
         }
      }

   }

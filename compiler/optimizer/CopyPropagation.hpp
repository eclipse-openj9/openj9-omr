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

#ifndef COPYPROP_INCL
#define COPYPROP_INCL

#include <stddef.h>                           // for NULL
#include <stdint.h>                           // for int32_t
#include "env/TRMemory.hpp"                   // for Allocator, etc
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager
#include "optimizer/UseDefInfo.hpp"  // for TR_UseDefInfo, etc

class TR_BitVector;
namespace TR { class SymbolReference; }
namespace TR { class Block; }
namespace TR { class Node; }
namespace TR { class TreeTop; }

// Copy Propagation
//
// Uses the information computed by ReachingDefinitions (stored in
// UseDefInfo) to perform the transformation. If a use node has a
// unique corresponding definition node (that is equivalent to a
// copy statement), then all occurrences of the copy in the use
// node can be replaced by the original value (the one that was copied).
// Note that after copy propagation is done, UseDefInfo might change
// because the program has been altered as a result of copy propagation.
//
// Dead Store Elimination might be a good optimization to perform
// after copy propagation is done; this could possibly eliminate
// the copy (definition) statement altogether in some cases.
//

class TR_CopyPropagation : public TR::Optimization
   {
   public:

   // Performs copy propagation using the
   // use/def values of relevant nodes.
   //
   TR_CopyPropagation(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_CopyPropagation(manager);
      }

   virtual int32_t perform();

   private :
   void replaceCopySymbolReferenceByOriginalIn(TR::SymbolReference * copySymRef, TR::Node * rhsOfStoreDefNode, TR::Node * useNode, TR::Node *defNode, TR::Node * baseAddrNode = NULL,bool baseAddrAvail = false);
   void replaceCopySymbolReferenceByOriginalRegLoadIn(TR::Node *regLoadNode, TR::Node *useNode, TR::SymbolReference *copySymbolReference, TR::Node *node, TR::Node *parent, int32_t childNum);
   void adjustUseDefInfo(TR::Node *, TR::Node *, TR_UseDefInfo *);
   bool isUniqueDefinitionOfUse(TR_BitVector *, TR_UseDefInfo *, int32_t);
   bool isLoadNodeSuitableForPropagation(TR::Node * useNode, TR::Node * storeNode, TR::Node * loadNode);
   TR::Node *areAllDefsInCorrectForm(TR::Node*, const TR_UseDefInfo::BitVector &, TR_UseDefInfo *, int32_t, TR::Node * &, int32_t &, TR::Node * &);
   bool isSafeToPropagate(TR::Node *, TR::Node *);
   TR::Node* skipTreeTopAndGetNode (TR::TreeTop* tt);
   bool isCorrectToPropagate(TR::Node *, TR::Node *, TR::list< TR::Node *> &, TR::SparseBitVector &, int32_t, const TR_UseDefInfo::BitVector &, TR_UseDefInfo *);
   bool isCorrectToReplace(TR::Node *, TR::Node *, const TR_UseDefInfo::BitVector &, TR_UseDefInfo *);
   bool isRedefinedBetweenStoreTreeAnd(TR::list< TR::Node *> & checkNodes, TR::SparseBitVector &, TR::Node * storeNode, TR::TreeTop * exitTree, int32_t regNumber, const TR_UseDefInfo::BitVector &, TR_UseDefInfo *);
   bool recursive_isRedefinedBetweenStoreTreeAnd(TR::list< TR::Node *> & checkNodes, TR::SparseBitVector &, TR::Node * storeNode, TR::TreeTop * exitTree, int32_t regNumber, const TR_UseDefInfo::BitVector &, TR_UseDefInfo *);
   bool containsNode(TR::Node * node1, TR::Node * node2, bool searchClones = true);
   TR::Node *isLoadVarWithConst(TR::Node *);
   TR::Node *isIndirectLoadFromAuto(TR::Node *);
   TR::Node *isIndirectLoadFromRegister(TR::Node *, TR::Node * &);
   TR::Node *isValidRegLoad(TR::Node *, TR::TreeTop *, int32_t &);
   TR::Node *isCheapRematerializationCandidate(TR::Node * , TR::Node *);
   bool containsLoadOfSymbol(TR::Node * node,TR::SymbolReference * symRef, TR::Node ** loadNode);
   TR::Node * isBaseAddrAvailable(TR::Node *, TR::Node *, bool &);
   bool isNodeAvailableInBlock(TR::TreeTop *, TR::Node *);
   void findUseTree(TR::Node *);

   TR::TreeTop *findAnchorTree(TR::Node *, TR::Node *);
   void rematerializeIndirectLoadsFromAutos();
   void commonIndirectLoadsFromAutos();

   bool _canMaintainUseDefs;
   bool _cleanupTemps;
   int32_t _counter;
   int32_t _numStoreTreeTops;
   TR::TreeTop *_storeTree;
   TR::TreeTop *_useTree;
   TR::TreeTop **_storeTreeTopsAsArray;
   TR::Block *_storeBlock;
   bool _lookForOriginalDefs;
   bool _propagatingWholeExpression;
   };

#endif

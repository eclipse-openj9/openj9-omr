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

#ifndef CFGSIMPLIFIER_INCL
#define CFGSIMPLIFIER_INCL

#include <stdint.h>                           // for int32_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

namespace TR { class Block; }
namespace TR { class CFG; }
namespace TR { class CFGEdge; }
namespace TR { class Node; }
namespace TR { class TreeTop; }
template <class T> class ListElement;

// Control flow graph simplifier
//
// Look for opportunities to simplify control flow.
//
class TR_CFGSimplifier : public TR::Optimization
   {
   public:
   TR_CFGSimplifier(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_CFGSimplifier(manager);
      }

   virtual int32_t perform();

   private :

   bool simplify();
   bool simplifyBooleanStore();
   bool simplifyCondCodeBooleanStore(TR::Block *joinBlock, TR::Node *branchNode, TR::Node *store1Node, TR::Node *store2Node);
   TR::TreeTop *getNextRealTreetop(TR::TreeTop *treeTop, bool skipRestrictedRegSaveAndLoad = false);
   TR::TreeTop *getLastRealTreetop(TR::Block *block);
   TR::Block   *getFallThroughBlock(TR::Block *block);
   bool canReverseBranchMask();

   TR::CFG                  *_cfg;

   // Current block
   TR::Block                *_block;

   // First successor to the current block
   TR::CFGEdge              *_succ1;
   TR::Block                *_next1;

   // Second successor to the current block
   TR::CFGEdge              *_succ2;
   TR::Block                *_next2;
   };

#endif

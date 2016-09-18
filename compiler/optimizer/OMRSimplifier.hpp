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

#ifndef OMR_SIMPLIFIER_INCL
#define OMR_SIMPLIFIER_INCL

#include <stddef.h>                           // for NULL
#include <stdint.h>                           // for int32_t, uint32_t, etc
#include "cs2/hashtab.h"                      // for HashTable
#include "env/IO.hpp"                         // for POINTER_PRINTF_FORMAT
#include "env/TRMemory.hpp"                   // for Allocator
#include "il/DataTypes.hpp"                   // for DataTypes, etc
#include "il/ILOpCodes.hpp"                   // for ILOpCodes
#include "il/ILOps.hpp"                       // for ILOpCode
#include "il/Node.hpp"                        // for Node, etc
#include "il/Node_inlines.hpp"                // for Node::getFirstChild, etc
#include "infra/Assert.hpp"                   // for TR_ASSERT
#include "infra/HashTab.hpp"                  // for TR_HashTabInt
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class TR_RegionStructure;
class TR_UseDefInfo;
class TR_ValueNumberInfo;
namespace TR { class Block; }
namespace TR { class CodeGenerator; }
namespace TR { class Compilation; }
namespace TR { class SymbolReference; }
namespace TR { class TreeTop; }

namespace OMR
{

typedef enum
   {
   ConditionCode0 = 0,
   ConditionCode1 = 1,
   ConditionCode2 = 2,
   ConditionCode3 = 3,
   ConditionCodeInvalid = 4,
   ConditionCodeLast = ConditionCodeInvalid
   } TR_ConditionCodeNumber;

/*
 * Class Simplifier
 * ================
 *
 * Simple IL tree transformations like constant folding, strength reduction, 
 * canonicalizing expressions (e.g. an add with a constant child will have the 
 * constant as the second child), branch/switch simplification etc. 
 */

class Simplifier : public TR::Optimization
   {

   public:

   Simplifier(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) Simplifier(manager);
      }

   // Simplify the whole method.
   //
   virtual int32_t perform();

   virtual int32_t performOnBlock(TR::Block *);
   virtual void prePerformOnBlocks();
   virtual void postPerformOnBlocks();

   // Simplify a complete block.
   //
   void simplify(TR::Block *block);
   TR::TreeTop * simplifyExtendedBlock(TR::TreeTop *);

   // Convert a conditional branch into either a goto or an unconditional
   // fall-through.
   // Changes the "node" argument to be the replaced branch node, NULL if the branch has been eliminated.
   // Returns true if blocks were removed as a result of the change.
   //
   bool conditionalToUnconditional(TR::Node *&node, TR::Block *block, int takeBranch);

   //
   // These methods should only be used by the simplifier functions
   //

   // Simplify a treetop.
   // Returns the next treetop to be processed.
   //
   TR::TreeTop *simplify(TR::TreeTop *treeTop, TR::Block *block);

   // Simplify an expression sub-tree.
   // Returns the root node, or NULL if the node has been removed
   //
   TR::Node *simplify(TR::Node *node, TR::Block *block);

   void cleanupFlags(TR::Node *node);
   void setCC(TR::Node *n, TR_ConditionCodeNumber cc);
   TR_ConditionCodeNumber getCC(TR::Node *n);

   TR_RegionStructure *containingStructure() { return _containingStructure; }
   bool     reassociate() { return _reassociate; }

   using OMR::Optimization::prepareToReplaceNode;
   void prepareToReplaceNode(TR::Node * node);

   void anchorOrderDependentNodesInSubtree(TR::Node *node, TR::Node *replacement, TR::TreeTop* anchorTree);
   TR::Node * unaryCancelOutWithChild(TR::Node * node, TR::Node * firstChild, TR::TreeTop* anchorTree, TR::ILOpCodes opcode, bool anchorChildren=true);

   TR::TreeTop         *_curTree;
   TR_UseDefInfo      *_useDefInfo;      // Cached use/def info
   TR_ValueNumberInfo *_valueNumberInfo; // Cached value number info

   bool      _invalidateUseDefInfo;
   bool      _invalidateValueNumberInfo;
   bool      _alteredBlock;
   bool      _blockRemoved;
   bool      _reassociate;
   TR_RegionStructure  *_containingStructure;
   TR_HashTabInt  _hashTable;   // used by reassociation
   TR_HashTabInt  _ccHashTab; // used by zEmulator

   TR::TreeTop      *_performLowerTreeSimplifier;
   TR::Node         *_performLowerTreeNode;
   };

}

#endif

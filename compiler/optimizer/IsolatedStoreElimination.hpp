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

#ifndef ISOLSTOR_INCL
#define ISOLSTOR_INCL

#include <stdint.h>                           // for int32_t
#include "il/Node.hpp"                        // for Node, vcount_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class TR_BitVector;
class TR_RegionStructure;
class TR_Structure;
class TR_UseDefInfo;
namespace TR { class SymbolReference; }
namespace TR { class Block; }
namespace TR { class TreeTop; }
template <class T> class TR_Array;

// Isolated Store Elimination
//
// Extends local dead store elimination; removes stores to any
// locals that are never used in the entire method. Such local
// stores might be created after commoning (local and global) and
// local dead store elimination may not be able to catch all the cases.
//
class TR_IsolatedStoreElimination : public TR::Optimization
   {
   public:
   // Performs isolated store elimination
   //
   TR_IsolatedStoreElimination(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_IsolatedStoreElimination(manager);
      }

   virtual int32_t perform();

   protected:

   char *_optDetailsMessage;
   bool _mustUseUseDefInfo; // temporary

   private :

   bool _deferUseDefInfoInvalidation;  // true if useDefInfo is invalidated during performWithUseDefInfo() but useDefInfo is still used
                                       // (conservatively) later on in the optimization.

   int32_t performWithUseDefInfo();
   int32_t performWithoutUseDefInfo();
   bool    canRemoveStoreNode(TR::Node *node);
   void    collectDefParentInfo(int32_t defIndex,TR::Node *, TR_UseDefInfo *);
   bool    groupIsolatedStores(int32_t defIndex,TR_BitVector *,TR_UseDefInfo *);
   void    examineNode(TR::Node *, vcount_t visitCount, bool);

   void performDeadStructureRemoval(TR_UseDefInfo *);
   bool findStructuresAndNodesUsedIn(TR_UseDefInfo *, TR_Structure *, vcount_t, TR_BitVector *,bool *);
   bool markNodesAndLocateSideEffectIn(TR::Node *, vcount_t, TR_BitVector *);

   void analyzeSingleBlockLoop(TR_RegionStructure *, TR::Block *);

   void removeRedundantSpills();

   enum defStatus
      {
      notVisited =0,
      inTransit,
      notToBeRemoved,
      toBeRemoved,
      doNotExamine
      };

   TR_BitVector        *_usedSymbols, *_temp, *_temp1, *_temp2;
   TR_Array<TR::Node *> *_storeNodes;
   TR_Array<int32_t>  *_defParentOfUse;
   TR_Array<defStatus>  *_defStatus;
   TR_Array<TR_BitVector *> *_groupsOfStoreNodes;
   TR_BitVector       *_trivialDefs;

   TR::TreeTop *_currentTree;
   };

#endif

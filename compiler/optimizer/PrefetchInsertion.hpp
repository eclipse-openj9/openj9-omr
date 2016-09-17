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

#include "optimizer/LoopCanonicalizer.hpp"

#include <stdint.h>                           // for int32_t
#include "env/jittypes.h"                     // for intptrj_t
#include "infra/List.hpp"                     // for TR_ScratchList
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class TR_BasicInductionVariable;
class TR_PrimaryInductionVariable;
class TR_RegionStructure;
class TR_Structure;
namespace TR { class Block; }
namespace TR { class Node; }
namespace TR { class Optimization; }
namespace TR { class SymbolReference; }
namespace TR { class TreeTop; }

class TR_PrefetchInsertion : public TR_LoopTransformer
   {
   public:
   TR_PrefetchInsertion(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_PrefetchInsertion(manager);
      }

   virtual int32_t perform();

   private:
   struct ArrayAccessInfo
      {
      TR::TreeTop *_treeTop;
      TR::Node *_aaNode;
      TR::Node *_addressNode;
      TR::Node *_bivNode;
      TR_BasicInductionVariable *_biv;
      };

   TR_ScratchList<ArrayAccessInfo> _arrayAccessInfos;

   void collectLoops(TR_Structure *str);
   void insertPrefetchInstructions();
   TR::Node *createDeltaNode(TR::Node *node, TR::Node *pivNode, int32_t deltaOnBackEdge);
   void examineLoop(TR_RegionStructure *loop);
   void examineNode(TR::TreeTop *treeTop, TR::Block *block, TR::Node *node, intptrj_t visitCount, TR_RegionStructure *loop);
   TR_PrimaryInductionVariable *getClosestPIV(TR::Block *block);
   bool isBIV(TR::SymbolReference* symRef, TR::Block *block, TR_BasicInductionVariable* &biv);
   };

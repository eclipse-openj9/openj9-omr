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

#ifndef TRIVIALDEADBLOCKREMOVER_INCL
#define TRIVIALDEADBLOCKREMOVER_INCL

namespace TR { class TreeTop; }
namespace TR { class Block; }

#include "optimizer/Optimization.hpp"

class TR_TrivialDeadBlockRemover : public TR::Optimization {

   public:
      virtual int32_t perform();


      TR_TrivialDeadBlockRemover(TR::OptimizationManager *manager):
         TR::Optimization(manager) {};

      static TR::Optimization *create(TR::OptimizationManager *manager)
         {
         return new (manager->allocator()) TR_TrivialDeadBlockRemover(manager);
         }

   protected:
      bool isFoldable(TR::TreeTop* tt);
      bool foldIf(TR::Block* b);
      TR_YesNoMaybe  evaluateTakeBranch(TR::Node* n);

};

#endif


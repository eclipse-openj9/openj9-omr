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

#ifndef REORDER_INDEX_EXPR_INCL
#define REORDER_INDEX_EXPR_INCL

#include <stdint.h>                           // for int32_t
#include "il/Node.hpp"                        // for vcount_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class TR_PrimaryInductionVariable;
class TR_Structure;
namespace TR { class Node; }

class TR_IndexExprManipulator: public TR::Optimization
   {
   public:
   TR_IndexExprManipulator(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_IndexExprManipulator(manager);
      }

   virtual int32_t perform();
   void rewriteIndexExpression(TR_Structure *);

   private:
   void rewriteIndexExpression(TR_PrimaryInductionVariable *primeIV,TR::Node *parent,TR::Node *node,bool parentIsAiadd);
   vcount_t _visitCount;
   bool    _somethingChanged;
   bool    _debug;

};

#endif

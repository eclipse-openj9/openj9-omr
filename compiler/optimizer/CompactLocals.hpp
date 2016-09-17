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

#ifndef COMPACTLOCALS_INCL
#define COMPACTLOCALS_INCL

#include <stdint.h>                           // for int32_t
#include "il/Node.hpp"                        // for Node, vcount_t
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager

class TR_BitVector;
class TR_HashTabInt;
class TR_InterferenceGraph;
class TR_Liveness;
namespace TR { class AutomaticSymbol; }
namespace TR { class Block; }
namespace TR { class ResolvedMethodSymbol; }
template <class T> class TR_Array;

/*
 * Class TR_CompactLocals
 * ======================
 *
 * Compact locals tries to minimize the stack size required by the 
 * compiled method. This is done by computing interferences between the 
 * live ranges of the locals/temps in the method before code generation 
 * is done; based on these interferences, locals that are never live 
 * simultaneously can be mapped on to the same stack slot. 
 * Liveness information is used by this analysis.
 */

class TR_CompactLocals : public TR::Optimization
   {
   TR_BitVector                   *_liveVars;
   TR_BitVector                   *_prevLiveVars;
   TR_BitVector                   *_temp;
   TR_Array<TR::AutomaticSymbol *> *_localIndexToSymbolMap;
   TR_InterferenceGraph           *_localsIG;
   TR_HashTabInt                  *_visit;
   TR_Array<TR::AutomaticSymbol *> *_callerLiveSyms;

   void processNodeInPreorder(TR::Node *root, vcount_t visitCount, TR_Liveness *liveLocals, TR::Block *block, bool directChildOfTreeTop);
   void createInterferenceBetween(TR_BitVector *bv);
   void createInterferenceBetween(TR_BitVector *bv1, TR_BitVector *bv2);
   void createInterferenceBetweenLocals(int32_t localIndex);
   void doCompactLocals();

public:
   TR_CompactLocals(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_CompactLocals(manager);
      }

   bool eligibleLocal(TR::AutomaticSymbol * localSym);
   void assignColorsToSymbols(TR_BitVector *bv);

   virtual int32_t perform();
   };

#endif

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

#if DEBUG

#ifndef DOMINATORVERIFY_INCL
#define DOMINATORVERIFY_INCL

#include <stdint.h>                     // for int32_t
#include "compile/Compilation.hpp"      // for Compilation
#include "env/TRMemory.hpp"             // for TR_Memory, etc
#include "il/Node.hpp"                  // for vcount_t
#include "optimizer/DominatorsChk.hpp"  // for TR_DominatorsChk

class TR_BitVector;
class TR_Dominators;
namespace TR { class Block; }

// This class can be used to verify :
// 1. If the Dominators info computed by the simple O(n^2) algorithm
//    in Muchnick is correct. The Dominator info for each node is
//    verified by comparing with the dominator tree.
// 2. If the Dominators computed by the efficient algorithm and the
//    expensive O(n^2) algorithm are the same. The immediate
//    dominators computed by each of the algorithms are compared for each node
//    in the flow graph. Note that if the expensive algorithm is correct
//    (Verification 1) and the two algorithms are consistent
//    (Verification 2), then the efficient algorithm is proven correct.
//
class TR_DominatorVerifier
   {
   public:
   TR_ALLOC(TR_Memory::DominatorVerifier)

   TR_DominatorVerifier(TR_Dominators&);

   private:

   bool    bothImplementationsConsistent;
   bool    expensiveAlgorithmCorrect;

   bool    areBothImplementationsConsistent(TR_DominatorsChk&, TR_Dominators&);
   bool    isExpensiveAlgorithmCorrect(TR_DominatorsChk&);
   bool    dominates(TR::Block *, TR::Block *);
   void    compareWithPredsOf(TR::Block *, TR::Block *);

   TR::Compilation * comp()          {return _compilation;}
   TR_Memory *      trMemory()      { return comp()->trMemory(); }
   TR_StackMemory   trStackMemory() { return trMemory(); }

   TR::Compilation              *_compilation;
   TR_DominatorsChk::BBInfoChk *_dominatorsChkInfo;
   TR_BitVector                *_nodesSeenOnEveryPath;
   TR_BitVector                *_nodesSeenOnCurrentPath;
   int32_t                      _numBlocks;
   vcount_t                      _visitCount;
   TR_Dominators               *_dominators;
   };

#endif

#endif

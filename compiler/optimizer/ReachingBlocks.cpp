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

#include <stdint.h>                                 // for int32_t
#include "env/StackMemoryRegion.hpp"
#include "compile/Compilation.hpp"                  // for Compilation
#include "env/TRMemory.hpp"
#include "infra/BitVector.hpp"                      // for TR_BitVector
#include "infra/Cfg.hpp"                            // for CFG
#include "optimizer/DataFlowAnalysis.hpp"

class TR_BlockStructure;
class TR_Structure;
namespace TR { class Optimizer; }

TR_DataFlowAnalysis::Kind TR_ReachingBlocks::getKind()
   {
   return ReachingBlocks;
   }


bool TR_ReachingBlocks::supportsGenAndKillSets()
   {
   return true;
   }


TR_ReachingBlocks::TR_ReachingBlocks(TR::Compilation *comp, TR::Optimizer *optimizer, bool trace)
   : TR_UnionBitVectorAnalysis(comp, comp->getFlowGraph(), optimizer, trace)
   {
   _numberOfBlocks = comp->getFlowGraph()->getNextNodeNumber();
   }

int32_t TR_ReachingBlocks::perform()
   {
   // Allocate the block info before setting the stack mark - it will be used by
   // the caller
   //
   initializeBlockInfo();

   {
   TR::StackMemoryRegion stackMemoryRegion(*trMemory());

   TR_Structure *rootStructure = comp()->getFlowGraph()->getStructure();
   performAnalysis(rootStructure, false);
   } // scope of the stack memory region

   return 10; // actual cost
   }


int32_t TR_ReachingBlocks::getNumberOfBits()
   {
   return _numberOfBlocks;
   }


void TR_ReachingBlocks::analyzeBlockZeroStructure(TR_BlockStructure *blockStructure)
   {
   // Initialize the analysis info by making the initial parameter and field
   // definitions reach the method entry
   //
   //_blockInfo[0]->Empty();
   }


void TR_ReachingBlocks::initializeGenAndKillSetInfo()
   {
   // For each block in the CFG build the gen and kill set for this analysis.
   // Go in treetop order, which guarantees that we see the correct (i.e. first)
   // evaluation point for each node.
   //
   int32_t   blockNum;

   for (blockNum = 0; blockNum < _numberOfBlocks; blockNum++)
      {
      _regularGenSetInfo[blockNum] = new (trStackMemory()) TR_BitVector(getNumberOfBits(),trMemory(), stackAlloc);
      _regularGenSetInfo[blockNum]->set(blockNum);
      _exceptionGenSetInfo[blockNum] = new (trStackMemory()) TR_BitVector(getNumberOfBits(),trMemory(), stackAlloc);
      _exceptionGenSetInfo[blockNum]->set(blockNum);
      }
   }

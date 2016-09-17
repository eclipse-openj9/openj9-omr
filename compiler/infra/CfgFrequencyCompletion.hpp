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
 ******************************************************************************/

#ifndef CFGCOMPLETION_INCL
#define CFGCOMPLETION_INCL

#include <algorithm>                 // for std::max
#include <limits.h>                  // for INT_MAX
#include <stddef.h>                  // for NULL
#include <stdint.h>                  // for int32_t, uint16_t
#include "compile/Compilation.hpp"   // for Compilation
#include "env/TRMemory.hpp"
#include "il/Block.hpp"              // for Block
#include "infra/BitVector.hpp"       // for TR_BitVector
#include "infra/Cfg.hpp"             // for CFG
#include "infra/List.hpp"            // for List, ListElement, etc
#include "infra/TRCfgEdge.hpp"       // for CFGEdge
#include "infra/TRCfgNode.hpp"       // for CFGNode
#include "optimizer/Dominators.hpp"  // for TR_Dominators

class TR_RegionStructure;
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class TreeTop; }

enum EdgeDirection { OUTCOMING, INCOMING };

class TR_BlockFrequenciesCompletion
   {
private:
   TR::Compilation *_comp;
    TR::CFG *_cfg;
   TR_Dominators *_dominators;
   TR_Dominators *_postdominators;
   int32_t _maxBlockNumber;

   static TR::Block *getDominatorInRegion(TR::Block * block, TR_Dominators *dominators, TR_BitVector *regionBlocks)
      {
      TR::Block *dominator = dominators->getDominator(block);
      while (dominator && !regionBlocks->isSet(dominator->getNumber()))
         dominator = dominators->getDominator(dominator);
      return dominator;
      }

   TR::CFGEdge *getIfSingleEdge(TR::Block *block, bool isPredecessor)
      {
      // create a list of all (prede/suc)cessor edges for the block
      TR::list<TR::CFGEdge*> edgesList((isPredecessor) ? block->getPredecessors() : block->getSuccessors());
      if (isPredecessor)
         edgesList.insert(edgesList.end(), block->getExceptionPredecessors().begin(), block->getExceptionPredecessors().end());
      else
         edgesList.insert(edgesList.end(), block->getExceptionSuccessors().begin(), block->getExceptionSuccessors().end());
      // if the list contains only one entry => return it
      if (edgesList.empty())
         return 0;
      if (edgesList.size() == 1) // no next element
         return edgesList.front();
      else
         return 0;
      }

   static TR::CFGEdge *getSingleIncomingEdge(TR::Block *block)
      {
      if (block->getPredecessors().empty())
         return 0;
      if (block->getPredecessors().size() == 1)
         return block->getPredecessors().front();
      else
         return 0;
      }

   static TR::CFGEdge *getSingleOutcomingEdge(TR::Block *block)
      {
      TR::list<TR::CFGEdge*> &successors = block->getSuccessors();
      if (successors.size() == 1)
         return successors.front();
      else
         return 0;
      }
public:

   TR_BlockFrequenciesCompletion(TR::Compilation *comp, TR::ResolvedMethodSymbol *methodSymbol,  TR::CFG *cfg = 0) : _comp(comp), _cfg(cfg ? cfg : _comp->getFlowGraph())
   {
   if (methodSymbol)
      {
      _dominators = new (_comp->trHeapMemory()) TR_Dominators(_comp, methodSymbol);
      _postdominators = new (_comp->trHeapMemory()) TR_Dominators(_comp, methodSymbol, true);
      }
   else
      {
      _dominators = new (_comp->trHeapMemory()) TR_Dominators(_comp);
      _postdominators = new (_comp->trHeapMemory()) TR_Dominators(_comp, true);
      }
   _maxBlockNumber = 0;
   for (TR::CFGNode *node = _cfg->getFirstNode(); node; node = node->getNext())
      _maxBlockNumber = std::max(node->getNumber(), _maxBlockNumber);
   }

   void propogateToDominator(TR::Block * block, TR_BitVector *regionBlocks,  TR_Queue<TR::Block> *blockQueue, bool usePostDominators);
   void completeIfSingleEdge();
   void gatherFreqFromSingleEdgePredecessors(TR::Block *block, TR_Queue<TR::Block> *blockQueue);
   void gatherFreqFromSingleEdgeSuccessors(TR::Block *block, TR_Queue<TR::Block> *blockQueue);
   void completeRegion(TR_RegionStructure *region, int);
   }; // class BlockFrequencyCompletion

class TR_EdgeFrequenciesCompletion
   {
private:

   TR::Compilation *_comp;
    TR::CFG *_cfg;
   List<TR::CFGEdge> _edgeList;
   List<TR::CFGNode>  _blockWithUnknownFreqList;

   static const uint16_t NON_VISITED = 1;
   static const int32_t UNKNOWN_BLOCK_FREQ = INT_MAX;
   static TR::Block *getEdgeEnd(TR::CFGEdge *edge, EdgeDirection direction)
      {
      return (direction == INCOMING) ? edge->getTo()->asBlock() : edge->getFrom()->asBlock();
      }

public:
   TR_EdgeFrequenciesCompletion(TR::Compilation *comp,  TR::CFG *cfg = 0) : _comp(comp), _cfg(cfg ? cfg : comp->getFlowGraph())
,_edgeList(_comp->trMemory()),_blockWithUnknownFreqList(_comp->trMemory())
   {}
   void dumpEdge(TR::Compilation *comp, const char *title, TR::CFGEdge *edge);
   bool canCompletingEdge(TR::CFGEdge *edge, EdgeDirection direction, int32_t iterationNumber, int32_t *maxAllowedFrequency);
   bool completeBlockWithUnknownFreqFromEdges(TR::Block *block, EdgeDirection edgesDirection);
   bool setEdgesFrequency(TR::list<TR::CFGEdge*> &edgesList, int32_t frequency);
   bool setBlockWithUnknownFreqToZero (int32_t iterationNumber);
   bool giveFreqToEdgeEnteringInnerLoop (int32_t iterationNumber);
   void completeEdges();
   }; // EdgeFrequenciesCompletion

#endif

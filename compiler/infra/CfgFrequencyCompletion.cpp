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

#include "infra/CfgFrequencyCompletion.hpp"

#include <algorithm>                    // for std::min
#include "codegen/FrontEnd.hpp"         // for TR_FrontEnd
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "il/ILOpCodes.hpp"             // for ILOpCodes
#include "il/Node.hpp"                  // for Node
#include "il/Node_inlines.hpp"          // for Node::getFirstChild
#include "il/TreeTop.hpp"               // for TreeTop
#include "il/TreeTop_inlines.hpp"       // for TreeTop::getNextTreeTop, etc
#include "infra/Assert.hpp"             // for TR_ASSERT
#include "infra/Cfg.hpp"                // for TR_SuccessorIterator
#include "optimizer/Structure.hpp"      // for TR_RegionStructure, etc

static const char *edgeDirectionNames[] = {"outcoming", "incoming"};


//propagate frequency of block it's dominator in the same region
void TR_BlockFrequenciesCompletion::propogateToDominator(TR::Block * block, TR_BitVector *regionBlocks,  TR_Queue<TR::Block> *blockQueue,
      bool usePostDominators)
   {
   TR_Dominators *dominators = usePostDominators ? _postdominators : _dominators;
   const char *dominatorTypeName = usePostDominators ? "postdominator" : "dominator";

   int32_t blockFrequency = block->getFrequency();
   TR::Block *dominator = getDominatorInRegion(block, dominators, regionBlocks);
   if (!dominator)
      {
      return;
      }
   int32_t dominatorFrequency = dominator->getFrequency();

   if (dominatorFrequency < blockFrequency)
      {
      if (dominator->isGenAsmBlock() && usePostDominators)
         {
         //do not warm asm block as we do not know what it contains
         return;
         }

      dominator->setFrequency(blockFrequency);
      //insert dominator to the list
      blockQueue->enqueue(dominator);
      }
   }

void TR_BlockFrequenciesCompletion::gatherFreqFromSingleEdgePredecessors(TR::Block *block, TR_Queue<TR::Block> *blockQueue)
   {
   // Looking at predecessors of 'block', whose only successor is 'block'
   TR::list<TR::CFGEdge*> preds = block->getPredecessors();
   int32_t totalFrequency = 0;

   // no incoming/outgoing edges
   if (preds.empty())
      {
      return;
      }

   bool hasSingleOutgoingEdge = false;
   for (auto edge = preds.begin(); edge != preds.end(); ++edge)
      {
      TR::CFGEdge *e = getSingleOutcomingEdge((*edge)->getFrom()->asBlock());

      if (e)
         {
         hasSingleOutgoingEdge = true;
         if (e->getFrom()->getFrequency() > 0)
            {
            TR_ASSERT (e->getTo() == block,"");
            totalFrequency += e->getFrom()->getFrequency();
            }
         }
      }
   if (block->getFrequency() < totalFrequency)
      {
      block->setFrequency(totalFrequency);

      //If frequency changed, it may affect the frequency of 'block's successors/predecessors,
      //but only if 'block' has a single successor/predecessor
      TR::CFGEdge *edge = getSingleOutcomingEdge(block);
      if (edge)
         blockQueue->enqueue(edge->getTo()->asBlock());
      edge = getSingleIncomingEdge(block);
      if (edge)
         blockQueue->enqueue(edge->getFrom()->asBlock());
      }
   }

//We have block frequencies, but no edge frequencies yet. Attempt to improve the
//accuracy of block frequencies by summing up frequencies from successors whose only
//predecessor is 'block'
void TR_BlockFrequenciesCompletion::gatherFreqFromSingleEdgeSuccessors(TR::Block *block, TR_Queue<TR::Block> *blockQueue)
   {
   // Looking at successors of 'block', whose only successor is 'block'
   TR::list<TR::CFGEdge*> succs = block->getSuccessors();
   int32_t totalFrequency = 0;

   // no incoming/outgoing edges
   if (succs.empty())
      {
      return;
      }
   bool hasSingleIncomingEdge = false;
   for (auto edge = succs.begin(); edge != succs.end(); ++edge)
      {
      TR::CFGEdge *e = getSingleIncomingEdge((*edge)->getTo()->asBlock());

      if (e)
         {
         hasSingleIncomingEdge = true;
         if (e->getTo()->getFrequency() > 0)
            {
            TR_ASSERT (e->getFrom() == block,"");
            totalFrequency += e->getTo()->getFrequency();
            }
         }
      }

   if (block->getFrequency() < totalFrequency)
      {
      block->setFrequency(totalFrequency);

      //If frequency changed, it may affect the frequency of 'block's successors/predecessors,
      //but only if 'block' has a single successor/predecessor
      TR::CFGEdge *edge = getSingleIncomingEdge(block);
      if (edge)
         blockQueue->enqueue(edge->getFrom()->asBlock());

      edge = getSingleOutcomingEdge(block);
      if (edge)
         blockQueue->enqueue(edge->getTo()->asBlock());
      }
   }

//Propagate frequency from block to it's single predecessors or single successor
//if such exists
void TR_BlockFrequenciesCompletion::completeIfSingleEdge()
   {
   TR_Queue<TR::Block> *blockQueue = new (_comp->trHeapMemory()) TR_Queue<TR::Block>(_comp->trMemory());
   for (TR::CFGNode *node = _cfg->getFirstNode(); node; node = node->getNext())
      blockQueue->enqueue(node->asBlock());
   TR::Block *block;
   while (block = blockQueue->dequeue())
      {
      gatherFreqFromSingleEdgePredecessors(block, blockQueue);
      gatherFreqFromSingleEdgeSuccessors(block, blockQueue);
      }
   }

//do completion of block frequencies inside the regions
//the algorithm relies on the following property:
//frequency (dominator-of (BBi)) >= frequency (BBi)
//frequency (postdominator-of (BBi)) >= frequency (BBi))

void TR_BlockFrequenciesCompletion::completeRegion(TR_RegionStructure *region, int q)
   {
   TR_BitVector *regionBlocks = new (_comp->trHeapMemory()) TR_BitVector(_maxBlockNumber + 1, _comp->trMemory());
   TR_Queue<TR::Block> *blockQueue = new (_comp->trHeapMemory()) TR_Queue<TR::Block>(_comp->trMemory());

   TR_RegionStructure::Cursor si(*region);
   for (TR_StructureSubGraphNode *node = si.getCurrent(); node != NULL; node = si.getNext())
      {
      TR_Structure *structure = node->getStructure();
      TR_ASSERT(structure, "assumes structure in completeRegion");
      TR_BlockStructure *blockStruct = structure->asBlock();
      if (!blockStruct)
         completeRegion(structure->asRegion(), q++);
      else
         {
         TR::Block *block = blockStruct->getBlock();
         regionBlocks->set(block->getNumber());
         blockQueue->enqueue(block);
         }
      }
   if (region->containsInternalCycles())
      {
      return;
      }

   TR::Block *block;
   while (block = blockQueue->dequeue())
      {
      propogateToDominator(block, regionBlocks, blockQueue, false);
      propogateToDominator(block, regionBlocks, blockQueue, true);
      }
   }

//return true if the edge can be completed in this iteration.
//maxAllowedFrequency should hold the maximum frequency value it can have.
bool TR_EdgeFrequenciesCompletion::canCompletingEdge(TR::CFGEdge *edge, EdgeDirection direction, int32_t iterationNumber, int32_t *maxAllowedFrequency)
   {
   TR_ASSERT(edge->getVisitCount() == NON_VISITED, "");
   TR::Block *block = getEdgeEnd(edge, direction);
   int32_t blockFrequency = block->getFrequency();

   const char *incomingOrOutcoming = (direction == INCOMING) ? "incoming" : "outcoming";
   bool isOnlyNonVisitedEdge = true;
   TR::list<TR::CFGEdge*> neighbors((direction == INCOMING) ? block->getPredecessors() : block->getSuccessors());
   if (direction == INCOMING)
      neighbors.insert(neighbors.end(), block->getExceptionPredecessors().begin(), block->getExceptionPredecessors().end());
   else
      neighbors.insert(neighbors.end(), block->getExceptionSuccessors().begin(), block->getExceptionSuccessors().end());

   // calculate total frequency of visited edges
   int32_t totalVisitedFrequency = 0;
   for (auto otherEdge = neighbors.begin(); otherEdge != neighbors.end(); ++otherEdge)
      {
      if ((*otherEdge)->getVisitCount() >= iterationNumber)
         {
         // some adjacent edge was already completed in this iteration
         // we will complete this edge in the next iterations to get higher precision
         *maxAllowedFrequency = -1;
         return false;
         }
      if ((*otherEdge)->getVisitCount() == NON_VISITED)
         {
         if ((*otherEdge) != edge)
            {
            isOnlyNonVisitedEdge = false;
            }
         }
      else if ((*otherEdge)->getVisitCount() > NON_VISITED)
         totalVisitedFrequency += (*otherEdge)->getFrequency();
      }

   if (totalVisitedFrequency >= blockFrequency)
      {
      *maxAllowedFrequency = 0;
      return true;
      }

   if (blockFrequency == -1)
      {
      //we can not complete the edge with predecessors/successors but we want to give the chance for other
      //edges to complete the frequency with successors/predecessors.
      *maxAllowedFrequency = UNKNOWN_BLOCK_FREQ;
      return false;
      }
   *maxAllowedFrequency = blockFrequency - totalVisitedFrequency;
   TR_ASSERT(maxAllowedFrequency >= 0, "");
   return isOnlyNonVisitedEdge;
   }

//set the frequency of a block BLOCK based on it's incoming/outcoming edges
bool TR_EdgeFrequenciesCompletion::completeBlockWithUnknownFreqFromEdges(TR::Block *block, EdgeDirection edgesDirection)
   {
   if (block->getFrequency() != -1)
      return false;

   TR::list<TR::CFGEdge*>* neighbors;
   if(edgesDirection == INCOMING)
      neighbors = &(block->getPredecessors());
   else
      neighbors = &(block->getSuccessors());

   int32_t totalVisitedFrequency = 0;

   // no incoming/outcoming edges
   if (neighbors->empty())
      return false;

   for (auto otherEdge = neighbors->begin(); otherEdge != neighbors->end(); ++otherEdge)
      if ((*otherEdge)->getVisitCount() == NON_VISITED)
         return false;
      else
         totalVisitedFrequency += (*otherEdge)->getFrequency();

   block->setFrequency(totalVisitedFrequency);
   return true;
   }

void TR_EdgeFrequenciesCompletion::dumpEdge(TR::Compilation *comp, const char *title, TR::CFGEdge *edge)
   {
   TR::Block *from = edge->getFrom()->asBlock();
   TR::Block *to = edge->getTo()->asBlock();
   }

bool TR_EdgeFrequenciesCompletion::setEdgesFrequency(TR::list<TR::CFGEdge*> &edgesList, int32_t frequency)
   {
   bool anyEdgeSet = false;
   for (auto edge = edgesList.begin(); edge != edgesList.end(); ++edge)
      {
      (*edge)->setFrequency(frequency);
      anyEdgeSet = true;
      }
   return anyEdgeSet;
   }

bool TR_EdgeFrequenciesCompletion::setBlockWithUnknownFreqToZero (int32_t iterationNumber)
   {
   bool changesDone=false;

   ListElement<TR::CFGNode> *prev=NULL;
   ListElement<TR::CFGNode> *ptr=_blockWithUnknownFreqList.getListHead();
   TR::CFGNode *block=NULL;
   //it could be that some of the blocks with frequency -1 got real frequency
   //in completeBlockWithUnknownFreqFromEdges thus we need to carefully
   //find a block with frequency -1 in the list.
   while (ptr != NULL)
      {
      block = ptr->getData();
      ListElement<TR::CFGNode> *next = ptr->getNextElement();
      if (block->asBlock()->getFrequency() != -1)
         {
         // need to pop this guy off the list...no sense continuing to look at him
         _blockWithUnknownFreqList.setListHead(next);
         }
      else
         {
         // break out of the loop
         break;
         }
      block = NULL;
      ptr = next;
      }
   if (ptr != NULL)
      {
      block = ptr->getData();
      TR_ASSERT(block->asBlock()->getFrequency() == -1, "Error in setBlockWithUnknownFreqToZero");
      block->asBlock()->setFrequency(0);
      changesDone = true;
      _blockWithUnknownFreqList.setListHead(ptr->getNextElement());
      }
   return changesDone;
   }

//apply heuristic: if we could not complete an edge then
//give available frequency to an edge entering an inner loop even if
//this edge frequency can not be completely completed from it's neighbors
bool TR_EdgeFrequenciesCompletion::giveFreqToEdgeEnteringInnerLoop (int32_t iterationNumber)
   {
   bool changesDone = false;
   ListElement<TR::CFGEdge> *ptr;
   ListElement<TR::CFGEdge> *next;

   for (ptr=_edgeList.getListHead(); ptr != NULL; ptr = ptr->getNextElement())
      {
      TR::CFGEdge *edge = ptr->getData();

      TR_ASSERT(edge->getVisitCount() == NON_VISITED, "Error in giveFreqToEdgeEnteringInnerLoop");
      int32_t outcomingFrequency, incomingFrequency;
      bool completedAsOutcoming = canCompletingEdge(edge, OUTCOMING, iterationNumber, &outcomingFrequency);
      bool completedAsIncoming = canCompletingEdge(edge, INCOMING, iterationNumber, &incomingFrequency);

      TR_ASSERT(!completedAsIncoming && !completedAsOutcoming, "Error in completeEdges");
      int32_t allowedFrequency = std::min(incomingFrequency, outcomingFrequency);
      if (allowedFrequency < 0 || allowedFrequency == UNKNOWN_BLOCK_FREQ)
         continue;

      TR::Block *fromBlock = edge->getFrom()->asBlock();
      TR::Block *toBlock = edge->getTo()->asBlock();
      TR_Structure *loop = toBlock->getStructureOf()->getContainingLoop();
      if (loop && (fromBlock->getStructureOf()->getContainingLoop() == loop->getContainingLoop()))
         {
         edge->setVisitCount(iterationNumber);
         edge->setFrequency(allowedFrequency);
         changesDone = true;
         completeBlockWithUnknownFreqFromEdges(fromBlock, OUTCOMING);
         completeBlockWithUnknownFreqFromEdges(toBlock, INCOMING);
         _edgeList.remove(edge);
         break;
         }

      }
   return changesDone;
   }


//do completion of edges frequencies
void TR_EdgeFrequenciesCompletion::completeEdges()
   {
   _cfg->resetVisitCounts(NON_VISITED); // not visited yet

   int32_t iterationNumber = NON_VISITED;
   bool changesDone = true;

   for (TR::CFGNode *nextNode = _cfg->getFirstNode(); nextNode != NULL; nextNode = nextNode->getNext())
      {
      TR::Block *block = nextNode->asBlock();
      // set frequencies for all edges with freq 0
      if (block->getFrequency() == 0)
         {
         setEdgesFrequency(block->getPredecessors(), 0);
         setEdgesFrequency(block->getExceptionPredecessors(), 0);
         setEdgesFrequency(block->getSuccessors(), 0);
         setEdgesFrequency(block->getExceptionSuccessors(), 0);
         }
      //add blocks with frequency -1 to _blockWithUnknownFreqList
      if (block->getFrequency() == -1)
         {
         _blockWithUnknownFreqList.add(nextNode);
         }
      //create a list of the edges so we could iterate on them later
      TR_SuccessorIterator sit(nextNode);
      for (TR::CFGEdge * edge = sit.getFirst(); edge != NULL; edge=sit.getNext())
         {
         _edgeList.add(edge);
         }
      }

   while (changesDone)
      {
      changesDone = false;
      ++iterationNumber;

      ListElement<TR::CFGEdge> *next=NULL;
      ListElement<TR::CFGEdge> *ptr;
      TR::CFGEdge *edge;
      for (ptr =  _edgeList.getListHead(); ptr != NULL; ptr = ptr->getNextElement())
         {
         edge = ptr->getData();

         TR_ASSERT(edge->getVisitCount() == NON_VISITED, "Error in completeEdges");

         int32_t allowedFrequency;
         int32_t outcomingFrequency, incomingFrequency;
         //try to complete the frequency of the edge from it's neighbors
         bool completedAsOutcoming = canCompletingEdge(edge, OUTCOMING, iterationNumber, &outcomingFrequency);
         bool completedAsIncoming = canCompletingEdge(edge, INCOMING, iterationNumber, &incomingFrequency);
         if (incomingFrequency < 0 || outcomingFrequency < 0)
            continue;
         char *completedAsText;
         if (completedAsIncoming && completedAsOutcoming)
            {
            completedAsText = "incoming/outcoming";
            allowedFrequency = std::min(incomingFrequency, outcomingFrequency);
            }
         else if (completedAsIncoming)
            {
            completedAsText = "incoming";
            allowedFrequency = incomingFrequency;
            }
         else if (completedAsOutcoming)
            {
            completedAsText = "outcoming";
            allowedFrequency = outcomingFrequency;
            }
         else
            continue;

         TR::Block *fromBlock = edge->getFrom()->asBlock();
         TR::Block *toBlock = edge->getTo()->asBlock();
         _edgeList.remove(edge);
         changesDone = true;
         edge->setVisitCount(iterationNumber);
         edge->setFrequency(allowedFrequency);
         completeBlockWithUnknownFreqFromEdges(fromBlock, OUTCOMING);
         completeBlockWithUnknownFreqFromEdges(toBlock, INCOMING);
         }
      if (changesDone)
         continue;

      // if no changes done, try setting -1 blocks to 0, one at a time
      changesDone = setBlockWithUnknownFreqToZero (iterationNumber);

      if (changesDone)
         continue;

      // if no change done then try the heuristic of giving all the frequency to the edge entering the inner loop
      changesDone = giveFreqToEdgeEnteringInnerLoop(iterationNumber);

      }
   }

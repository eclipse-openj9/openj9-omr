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

#include "optimizer/StructuralAnalysis.hpp"

#include <stddef.h>                                   // for NULL
#include <stdint.h>                                   // for int32_t, etc
#include "env/StackMemoryRegion.hpp"
#include "codegen/FrontEnd.hpp"                       // for TR_FrontEnd
#include "compile/Compilation.hpp"                    // for Compilation
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "cs2/arrayof.h"                              // for StaticArrayOf
#include "cs2/bitvectr.h"
#include "cs2/sparsrbit.h"
#include "env/TRMemory.hpp"
#include "il/Block.hpp"                               // for Block, toBlock
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "infra/Assert.hpp"                           // for TR_ASSERT
#include "infra/Cfg.hpp"                              // for CFG
#include "infra/List.hpp"                             // for List, etc
#include "infra/Stack.hpp"                            // for TR_Stack
#include "infra/TRCfgEdge.hpp"                        // for CFGEdge
#include "infra/TRCfgNode.hpp"                        // for CFGNode
#include "optimizer/DominatorVerifier.hpp"
#include "optimizer/Structure.hpp"
#include "optimizer/Dominators.hpp"          // for TR_Dominators
#include "ras/Debug.hpp"                              // for TR_DebugBase

void TR_RegionAnalysis::simpleIterator (TR_Stack<int32_t>& workStack,
                                        SparseBitVector& vector,
                                        WorkBitVector &regionNodes,
                                        WorkBitVector &nodesInPath,
                                        bool &cyclesFound,
                                        TR::Block *hdrBlock,
                                        bool doThisCheck)
   {
   SparseBitVector::Cursor cursor(vector);
   for (cursor.SetToLastOne(); cursor.Valid(); cursor.SetToPreviousOne())
      {
      StructInfo &next = getInfo(cursor);

      // The exit block must only be made part of the region headed by the entry
      // block. It provides an exit for all other regions.
      //
      if (doThisCheck && next._succ.IsZero() &&
          next._originalBlock == _cfg->getEnd())
         {
         if (hdrBlock->getNumber() != 0)
            continue;
         }

      //short circuit the iteration
      //In the original version we would descend into another level of a recursion
      //but if we do it here, we would screw up big time since we can't push the same node three times.
      //We should be able to push the same node at most twice
      //so if we come across the same node the second time (nodesInPath[next]) we could say that we are done with this
      //particular node and move on
      if (regionNodes.ValueAt(cursor))
         {
         if (!cyclesFound && nodesInPath.ValueAt(cursor) &&
             _dominators.dominates(hdrBlock, next._originalBlock))
            {
            cyclesFound = true;
            if (trace())
               {
               traceMsg(comp(), "cycle found at node = %d\n", (uint32_t)cursor);
               }
            }
         }
      else
         {
         if (_dominators.dominates(hdrBlock, next._originalBlock))
            {
            workStack.push(cursor);
            }
         }
      }
   }

void TR_RegionAnalysis::StructInfo::initialize(TR::Compilation * comp, int32_t index, TR::Block *block)
   {
   _structure       = new (comp->trHeapMemory()) TR_BlockStructure(comp, block->getNumber(), block);
   _originalBlock   = block;
   _nodeIndex       = index;
   }

/**
 * Create a structure to represent each basic block. These will be the
 * leaves of the region graphs.
 * Create a structure information block for each such structure to control
 * analysis. The structure information blocks are ordered in depth-first
 * order of basic blocks, so we can do a postorder scan of the structures.
 */
void TR_RegionAnalysis::createLeafStructures(TR::CFG *cfg)
   {
   TR::CFGNode              *cfgNode;
   TR::Block                *b;
   int32_t                  nodeNum;

   _totalNumberOfNodes = 0;

   // We need two passes to initialize the array of StructInfo objects
   // because the order of nodes is not necessarily sequential.
   // The first constructs the objects in order and the second initializes them
   // (in a different order)
   // Note that TableOf entries are 1-based, not 0-based
   for (cfgNode = cfg->getFirstNode(); cfgNode; cfgNode = cfgNode->getNext())
      {
      	// Construct the right number of table entries
      _infoTable.AddEntry(_infoTable);
      _totalNumberOfNodes++;
      }

   for (cfgNode = cfg->getFirstNode(); cfgNode; cfgNode = cfgNode->getNext())
      {
      // Initialize each table entry (not necessarily sequential order)
      b = toBlock(cfgNode);
      nodeNum = _dominators._dfNumbers[b->getNumber()];
      StructInfo &si = getInfo(nodeNum);
      si.initialize(_compilation, nodeNum, b);

      // Initialize predecessors and successors for the leaf structures.
      //
      TR::Block *next;
      int32_t index;
      for (auto p = b->getPredecessors().begin(); p != b->getPredecessors().end(); ++p)
         {
         next = toBlock((*p)->getFrom());
         index = _dominators._dfNumbers[next->getNumber()];
         si._pred[index] = true;
         }
      for (auto p = b->getSuccessors().begin(); p != b->getSuccessors().end(); ++p)
         {
         next = toBlock((*p)->getTo());
         index = _dominators._dfNumbers[next->getNumber()];
         si._succ[index] = true;
         }
      for (auto p = b->getExceptionPredecessors().begin(); p != b->getExceptionPredecessors().end(); ++p)
         {
         next = toBlock((*p)->getFrom());
         index = _dominators._dfNumbers[next->getNumber()];
         si._exceptionPred[index] = true;
         }
      for (auto p = b->getExceptionSuccessors().begin(); p != b->getExceptionSuccessors().end(); ++p)
         {
         next = toBlock((*p)->getTo());
         index = _dominators._dfNumbers[next->getNumber()];
         si._exceptionSucc[index] = true;
         }
      }
   }

/**
 * Mainline for performing Region Analysis.
 */
TR_Structure *TR_RegionAnalysis::getRegions(TR::Compilation *comp, TR::ResolvedMethodSymbol* methSym)
   {
   TR::StackMemoryRegion stackMemoryRegion(*comp->trMemory());

   // Calculate dominators
   // This has the side-effect of renumbering the blocks in depth-first order
   //
   TR_Dominators *dominators = NULL;
   dominators = new (comp->trHeapMemory()) TR_Dominators(comp);

   #if DEBUG
   if (debug("verifyDominator"))
      {
      TR_DominatorVerifier verifyDominator(*dominators);
      }
   #endif

   TR::CFG *cfg = methSym->getFlowGraph();
   TR_ASSERT(cfg, "cfg is NULL\n");

   TR_RegionAnalysis ra(comp, *dominators, cfg, comp->allocator());
   ra._trace = comp->getOption(TR_TraceSA);

   ra._useNew = !comp->getOption(TR_DisableIterativeSA);
   if (ra.trace())
      {
      traceMsg(comp, "Blocks before Region Analysis:\n");
      comp->getDebug()->print(comp->getOutFile(), cfg);
      }

   ra.createLeafStructures(cfg);

   // Loop through the node set until there is only one node left - this is the
   // root of the control tree.
   //
   TR_Structure *result = ra.findRegions();

   return result;
   }


/**
 * Mainline for performing Region Analysis.
 */
TR_Structure *TR_RegionAnalysis::getRegions(TR::Compilation *comp)
   {
   TR::StackMemoryRegion stackMemoryRegion(*comp->trMemory());

   // Calculate dominators
   // This has the side-effect of renumbering the blocks in depth-first order
   //
   TR_Dominators dominators(comp);

   #if DEBUG
   if (debug("verifyDominator"))
      {
      TR_DominatorVerifier verifyDominator(dominators);
      }
   #endif

   TR::CFG *cfg = comp->getFlowGraph();

   TR_RegionAnalysis ra(comp, dominators, cfg, comp->allocator());
   ra._trace = comp->getOption(TR_TraceSA);

   ra._useNew = !comp->getOption(TR_DisableIterativeSA);
   if (ra.trace())
      {
      traceMsg(comp, "Blocks before Region Analysis:\n");
      comp->getDebug()->print(comp->getOutFile(), cfg);
      }

   ra.createLeafStructures(cfg);

   // Loop through the node set until there is only one node left - this is the
   // root of the control tree.
   //
   TR_Structure *result = ra.findRegions();

   return result;
   }

/**
 * Find regions in the flow graph and reduce them to structures.
 *
 * A region consists of one of:
 *    1) a natural loop (aka a proper cyclic region), or
 *    2) a proper acyclic region, or
 *    3) an improper region
 *
 * A proper acyclic region is a sub-graph with a single entry node and no cycles.
 * A proper cyclic region is a sub-graph with a single entry node and one or more
 * back-edges to the entry node, and no internal cycles.
 * An improper region is a sub-graph with a single entry node and which contains
 * cycles.
 *
 * There are two passes.
 * On the first pass we go bottom up in the flow graph looking for natural loops.
 * On the second pass we go bottom up in the flow graph looking for regions large
 * enough to consider.
 * The root region is returned.
 */
TR_Structure *TR_RegionAnalysis::findRegions()
   {
   // Create work areas
   TR::BitVector regionNodes(comp()->allocator());
   TR::BitVector nodesInPath(comp()->allocator());
   // Create the array to hold created cfg nodes
   SubGraphNodes cfgNodes(_totalNumberOfNodes, comp()->allocator());

   // Loop through the active nodes in depth-first postorder looking for natural
   // loops.
   //
   int32_t nodeIndex;
   for (nodeIndex = _totalNumberOfNodes-1; nodeIndex >= 0; nodeIndex--)
      {
      StructInfo &node = getInfo(nodeIndex);

      // Skip nodes that aren't in the flow graph any more
      //
      if (node._structure == NULL)
         continue;

      TR_RegionStructure *region;

      // Look for a natural loop. The nodes in a natural loop consist only
      // of the nodes on paths that flow through the back edges of the loop.
      //
      region = findNaturalLoop(node, regionNodes, nodesInPath);
      if (region != NULL)
      	 buildRegionSubGraph(region, node, regionNodes, cfgNodes);
      }

   // Loop through the active nodes in depth-first postorder looking for other
   // regions.
   //
   for (nodeIndex = _totalNumberOfNodes-1; nodeIndex >= 0; nodeIndex--)
      {
      StructInfo &node = getInfo(nodeIndex);

      // Skip nodes that aren't in the flow graph any more
      //
      if (node._structure == NULL)
         continue;

      TR_RegionStructure *region;

      // Look for a proper acyclic region or an improper region
      //
      region = findRegion(node, regionNodes, nodesInPath);

      if (region != NULL)
         buildRegionSubGraph(region, node, regionNodes,cfgNodes);
      }

   TR_ASSERT(getInfo(0)._structure, "Region Analysis, root region not found");
   return getInfo(0)._structure;
   }

/**
 * See if there is a natural loop at the given node and find its region nodes
 * A natural loop is a loop with one or more back-edges.
 * A back-edge is identified by the fact that the "from" node of the edge is
 * dominated by the entry node.
 */
TR_RegionStructure *TR_RegionAnalysis::findNaturalLoop(StructInfo &node,
                                                       WorkBitVector &regionNodes,
                                                       WorkBitVector &nodesInPath)
   {
   regionNodes.Clear();
   regionNodes[node._nodeIndex] = true;
   nodesInPath.Clear();
   bool cyclesFound = false;

   int32_t numBackEdges = 0;

   SparseBitVector::Cursor cursor(node._pred);
   for (cursor.SetToFirstOne(); cursor.Valid(); cursor.SetToNextOne())
      {
      StructInfo &backEdgeNode = getInfo(cursor);
      if (_dominators.dominates(node._originalBlock, backEdgeNode._originalBlock))
         {
         // A back-edge has been found. Add its loop nodes to the region
         //

         if (_useNew)
            {
	         addNaturalLoopNodesIterativeVersion(backEdgeNode, regionNodes, nodesInPath, cyclesFound, node._originalBlock);
	         }
         else
            {
            addNaturalLoopNodes(backEdgeNode, regionNodes, nodesInPath, cyclesFound, node._originalBlock);
            }

         numBackEdges++;
         }
      }

   if (numBackEdges == 0)
      return NULL;

   TR_RegionStructure *region = new (trHeapMemory()) TR_RegionStructure(_compilation, node._structure->getNumber() /* node._nodeIndex */);
   if (cyclesFound)
      {
      if (trace())
         traceMsg(comp(), "   Found improper cyclic region %d\n",node._nodeIndex);
      region->setContainsInternalCycles(true);
      }
   else
      {
      if (trace())
         traceMsg(comp(), "   Found natural loop region %d\n",node._nodeIndex);
      }
   return region;
   }


void TR_RegionAnalysis::addNaturalLoopNodesIterativeVersion(StructInfo &node, WorkBitVector &regionNodes, WorkBitVector &nodesInPath, bool &cyclesFound, TR::Block *hdrBlock)
   {
   //special case for addNaturalLoops
   if (regionNodes.ValueAt(node._nodeIndex))
      {
      if (nodesInPath.ValueAt(node._nodeIndex))
         {
         cyclesFound = true;
         if (trace())
            {
               traceMsg(comp(), "cycle found at node = %d\n", node._nodeIndex);
            }
         }
      return;
      }

   //We will be pushing elements backwards to emulate processing in a depth-first order
   //Also, we will be pushing the same node twice in order to update nodesInPath correctly
   TR_Stack<int32_t> workStack(comp()->trMemory(), 8, false, stackAlloc);
   workStack.push(node._nodeIndex);

   while (!workStack.isEmpty())
      {
      int32_t index = workStack.pop();

      if (nodesInPath.ValueAt(index))
         {
         // If node is in path, in region and cycles have been found then processing
         // the node again will not have any sideeffects, so we can skip it completely
         if (regionNodes[index] && cyclesFound)
            continue;
         nodesInPath[index] = false;
         continue;
         }
      else
         {
         workStack.push(index); //push again but now it will be marked in nodesInPath,
         //so the next time we come across this index we won't process it again
         regionNodes[index] = true;
         nodesInPath[index] = true;
         }

      if (trace())
         {
         traceMsg(comp(), "addNaturalLoopNodesIterativeVersion, index = %d\n", index);
         }

      StructInfo& next = getInfo(index);
      //process the preds of next
      simpleIterator(workStack, next._pred, regionNodes, nodesInPath,
            cyclesFound, hdrBlock);
      //process the exception preds of next
      simpleIterator(workStack, next._exceptionPred, regionNodes, nodesInPath,
            cyclesFound, hdrBlock);
      }
   }

void TR_RegionAnalysis::addNaturalLoopNodes(StructInfo &node, WorkBitVector &regionNodes, WorkBitVector &nodesInPath, bool &cyclesFound, TR::Block *hdrBlock)
   {
   int32_t index = node._nodeIndex;

   if (trace())
      {
      traceMsg(comp(), "addNaturalLoopNodes, index = %d\n", index);
      }

   // If the node was already found in the region we can stop tracking this path.
   //
   if (regionNodes.ValueAt(index))
      {
      if (nodesInPath.ValueAt(index))
         {
         cyclesFound = true;
         if (trace())
            {
               traceMsg(comp(), "cycle found at node = %d\n", index);
            }
         }
      return;
      }

   // Add this node to the region and to this path and look at its predecessors.
   //
   regionNodes[index] = true;
   nodesInPath[index] = true;

   SparseBitVector::Cursor cursor(node._pred);
   for (cursor.SetToFirstOne(); cursor.Valid(); cursor.SetToNextOne())
      {
      StructInfo &next = getInfo(cursor);
      if (_dominators.dominates(hdrBlock, next._originalBlock))
         addNaturalLoopNodes(next, regionNodes, nodesInPath, cyclesFound, hdrBlock);
      }
   SparseBitVector::Cursor eCursor(node._exceptionPred);
   for (eCursor.SetToFirstOne(); eCursor.Valid(); eCursor.SetToNextOne())
      {
      StructInfo &next = getInfo(eCursor);
      if (_dominators.dominates(hdrBlock, next._originalBlock))
         addNaturalLoopNodes(next, regionNodes, nodesInPath, cyclesFound, hdrBlock);
      }
   nodesInPath[index] = false;
   }


/**
 * Find the nodes that would be part of a region whose entry is the given node.
 *
 * The region will consist of the entry node and recursively all successors that
 * are dominated by the entry node.
 *
 * At the same time, determine whether there are cycles in the region.
 * Create a proper acyclic region or an improper region if there are enough nodes
 * to justify it.
 */
TR_RegionStructure *TR_RegionAnalysis::findRegion(StructInfo &node,
		                                          WorkBitVector &regionNodes,
		                                          WorkBitVector &nodesInPath)
   {

   bool cyclesFound = false;
   regionNodes.Clear();
   nodesInPath.Clear();

   if (_useNew)
      {
      addRegionNodesIterativeVersion(node, regionNodes, nodesInPath, cyclesFound, node._originalBlock);
      }
   else
      {
      addRegionNodes(node, regionNodes, nodesInPath, cyclesFound, node._originalBlock);
      }
   if (!cyclesFound && (node._nodeIndex > 0))
      {
      if (regionNodes.PopulationCount() < 100)
         return NULL;
      }

   TR_RegionStructure *region = new (trHeapMemory()) TR_RegionStructure(_compilation, node._structure->getNumber() /* node._nodeIndex */);
   if (cyclesFound)
      {
      if (trace())
         traceMsg(comp(), "   Found improper cyclic region %d\n",node._nodeIndex);
      region->setContainsInternalCycles(true);
      }
   else
      {
      if (trace())
         traceMsg(comp(), "   Found proper acyclic region %d\n",node._nodeIndex);
      }
   return region;
   }

void TR_RegionAnalysis::addRegionNodesIterativeVersion(StructInfo &node, WorkBitVector &regionNodes, WorkBitVector &nodesInPath, bool &cyclesFound, TR::Block *hdrBlock)
   {
   //We will be pushing elements backwards to emulate processing in a depth-first order
   //Also, we will be pushing the same node twice in order to update nodesInPath correctly
   TR_Stack<int32_t> workStack(comp()->trMemory(), 8, false, stackAlloc);
   workStack.push(node._nodeIndex);
   while (!workStack.isEmpty())
      {
      int32_t index = workStack.pop();
      if (nodesInPath.ValueAt(index))
         {
         nodesInPath[index] = false;
         continue;
         }
      else
         {
         workStack.push(index); //push again but now it will be marked in nodesInPath,
         //so the next time we come across this index we won't process it again
         regionNodes[index] = true;
         nodesInPath[index] = true;
         }

      if (trace())
         {
         traceMsg(comp(), "addRegionNodesIterativeVersion, index = %d\n", index);
         }

      StructInfo& next = getInfo(index);
      //process the succs of next
      simpleIterator(workStack, next._succ, regionNodes, nodesInPath,
            cyclesFound, hdrBlock, true);

      simpleIterator(workStack, next._exceptionSucc, regionNodes, nodesInPath,
            cyclesFound, hdrBlock);
      }
   }

void TR_RegionAnalysis::addRegionNodes(StructInfo &node, WorkBitVector &regionNodes, WorkBitVector &nodesInPath, bool &cyclesFound, TR::Block *hdrBlock)
   {
   int32_t index = node._nodeIndex;

	if (trace())
	{
	traceMsg(comp(), "addRegionNodes, index = %d\n",index);
	}

   // If the node was already found in the region we can stop tracking this path.
   //
   if (regionNodes.ValueAt(index))
      {
      if (nodesInPath.ValueAt(index))
         {
         cyclesFound = true;
         if (trace())
            {
               traceMsg(comp(), "cycle found at node = %d\n", index);
            }
         }
      return;
      }

   // Add this node to the region and to this path and look at its successors.
   //
   regionNodes[index] = true;
   nodesInPath[index] = true;

   SparseBitVector::Cursor cursor(node._succ);
   for (cursor.SetToFirstOne(); cursor.Valid(); cursor.SetToNextOne())
      {
      StructInfo &next = getInfo(cursor);

      // The exit block must only be made part of the region headed by the entry
      // block. It provides an exit for all other regions.
      //
      if ((next._succ.IsZero()) &&
          next._originalBlock == _cfg->getEnd())
         {
         if (hdrBlock->getNumber() != 0)
            continue;
         }
      if (_dominators.dominates(hdrBlock, next._originalBlock))
         addRegionNodes(next, regionNodes, nodesInPath, cyclesFound, hdrBlock);

      }
   SparseBitVector::Cursor eCursor(node._exceptionSucc);
   for (eCursor.SetToFirstOne(); eCursor.Valid(); eCursor.SetToNextOne())
      {
      StructInfo &next = getInfo(eCursor);
      if (_dominators.dominates(hdrBlock, next._originalBlock))
         addRegionNodes(next, regionNodes, nodesInPath, cyclesFound, hdrBlock);
      }
   nodesInPath[index] = false;
   }

/**
 * Build the sub-graph for a structure that represents a region and update the
 * flow graph.
 */
void TR_RegionAnalysis::buildRegionSubGraph(TR_RegionStructure *region,
                                            StructInfo &entryNode,
                                            WorkBitVector &regionNodes,
                                            SubGraphNodes &cfgNodes)
   {
   StructInfo               *toNode;
   int32_t                   fromIndex, toIndex;
   TR_StructureSubGraphNode *from, *to;
   TR::CFGEdge               *edge;

   // Initialize the array of cfg nodes to be used to represent the region subgraph.
   //
   for (fromIndex = 0; fromIndex < _totalNumberOfNodes; fromIndex++)
	  cfgNodes[fromIndex] = NULL;
   //memset(cfgNodes, 0, _totalNumberOfNodes*sizeof(TR_StructureSubGraphNode*));

   // Build the sub-graph representing the region, and update the flow graph by
   // removing internal edges and moving edges to external successors from the
   // non-entry node to the entry node.

   // Bits cannot be turned off while iterating over a CS2 sparse bit vector,
   // so they are accumulated in a separate vector and turned off after iteration
   SparseBitVector bitsToBeRemoved(_compilation->allocator());

   WorkBitVector::Cursor rCursor(regionNodes);
   for (rCursor.SetToFirstOne(); rCursor.Valid(); rCursor.SetToNextOne())
      {
      fromIndex = rCursor;
      StructInfo &fromNode = getInfo(fromIndex);

      if (cfgNodes[fromIndex] == NULL)
         cfgNodes[fromIndex] = new (trHeapMemory()) TR_StructureSubGraphNode(fromNode._structure);
      from = cfgNodes[fromIndex];
      region->addSubNode(from);

      SparseBitVector::Cursor cursor(fromNode._succ);
      for (cursor.SetToFirstOne(); cursor.Valid(); cursor.SetToNextOne())
         {
         toIndex = cursor;
         StructInfo &toNode = getInfo(toIndex);
         if (cfgNodes[toIndex] == NULL)
            {
            if (regionNodes.ValueAt(toIndex))
               cfgNodes[toIndex] = new (trHeapMemory()) TR_StructureSubGraphNode(toNode._structure);
            else
               cfgNodes[toIndex] = new (trHeapMemory()) TR_StructureSubGraphNode(toNode._structure->getNumber(), trMemory());
            }
         to = cfgNodes[toIndex];
         edge = TR::CFGEdge::createEdge(from,  to, trMemory());
         if (regionNodes.ValueAt(toIndex))
            {
            toNode._pred[fromIndex] = false;
            bitsToBeRemoved[toIndex] = true;
            }
         else
            {
            region->addExitEdge(edge);
            if (&fromNode != &entryNode)
                  {
                  // Change the successor edge to come from the new region
                  //
               toNode._pred[fromIndex] = false;
               toNode._pred[entryNode._nodeIndex] = true;
               entryNode._succ[toIndex] = true;
               }
            }
         }
      fromNode._succ -= bitsToBeRemoved;

      bitsToBeRemoved.Clear();
      SparseBitVector::Cursor eCursor(fromNode._exceptionSucc);
      for (eCursor.SetToFirstOne(); eCursor.Valid(); eCursor.SetToNextOne())
         {
         toIndex = eCursor;
         StructInfo &toNode = getInfo(toIndex);
         if (cfgNodes[toIndex] == NULL)
            {
            if (regionNodes.ValueAt(toIndex))
               cfgNodes[toIndex] = new (trHeapMemory()) TR_StructureSubGraphNode(toNode._structure);
            else
               cfgNodes[toIndex] = new (trHeapMemory()) TR_StructureSubGraphNode(toNode._structure->getNumber(), trMemory());
            }
         to = cfgNodes[toIndex];
         edge = TR::CFGEdge::createExceptionEdge(from,to,trMemory());
         if (regionNodes.ValueAt(toIndex))
            {
            toNode._exceptionPred[fromIndex] = false;
            bitsToBeRemoved[toIndex] = true;
            }
         else
            {
            region->addExitEdge(edge);
            if (&fromNode != &entryNode)
                  {
               toNode._exceptionPred[fromIndex] = false;
               toNode._exceptionPred[entryNode._nodeIndex] = true;
               entryNode._exceptionSucc[toIndex] = true;
               }
            }
         }
      fromNode._exceptionSucc -= bitsToBeRemoved;
      fromNode._structure = NULL;
      }

   // The new structure replaces the entry node.
   //
   entryNode._structure = region;

   // Grab the entry node before losing the cfgNode info.
   //
   region->setEntry(cfgNodes[entryNode._nodeIndex]);
   if (trace())
      {
      _compilation->getDebug()->print(_compilation->getOutFile(), region, 6);
      traceMsg(comp(), "   Structure after finding a region:\n");
      _compilation->getDebug()->print(_compilation->getOutFile(), this, 6);
      }
   }

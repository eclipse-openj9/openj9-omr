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

#ifndef STRUCTURALANALYSIS_INCL
#define STRUCTURALANALYSIS_INCL

#include <stdint.h>                 // for int32_t
#include "compile/Compilation.hpp"  // for Compilation
#include "cs2/arrayof.h"            // for StaticArrayOf
#include "cs2/tableof.h"            // for TableOf
#include "env/TRMemory.hpp"         // for Allocator, TR_Memory, etc
#include "il/Block.hpp"             // for Block
#include "infra/Cfg.hpp"            // for CFG
#include "infra/deque.hpp"

class TR_Dominators;
class TR_RegionStructure;
class TR_Structure;
class TR_StructureSubGraphNode;
namespace TR { class ResolvedMethodSymbol; }
template <class T> class TR_Stack;

/**
 * Perform Region Analysis to break the CFG down into sub-regions.
 */
class TR_RegionAnalysis
   {
   public:
   TR_ALLOC(TR_Memory::RegionAnalysis)

   TR::Compilation * comp() { return _compilation; }

   TR_Memory *   trMemory()     { return comp()->trMemory(); }
   TR_HeapMemory trHeapMemory() { return trMemory(); }
   bool trace() { return _trace; }

   static TR_Structure *getRegions(TR::Compilation *);
   static TR_Structure *getRegions(TR::Compilation *, TR::ResolvedMethodSymbol *);

   friend class TR_Debug;

   private:

   class StructInfo;
   typedef CS2::TableOf<StructInfo, TR::Allocator> InfoTable;
   typedef TR::SparseBitVector SparseBitVector;
   typedef TR::BitVector WorkBitVector;
   typedef TR::deque<TR_StructureSubGraphNode*> SubGraphNodes;

   void simpleIterator(TR_Stack<int32_t>& workStack,
                       SparseBitVector& vector,
                       WorkBitVector &regionNodes,
                       WorkBitVector &nodesInPath,
                       bool &cyclesFound,
                       TR::Block *hdrBlock,
                       bool doThisCheck = false);

   class StructInfo
      {
      public:
      StructInfo(const TR::Allocator &allocator)
         : _pred(allocator), _succ(allocator), _exceptionPred(allocator), _exceptionSucc(allocator)
         {
         }
      SparseBitVector     _pred;
      SparseBitVector     _succ;
      SparseBitVector     _exceptionPred;
      SparseBitVector     _exceptionSucc;
      TR_Structure       *_structure;
      TR::Block           *_originalBlock;
      int32_t             _nodeIndex;

      void initialize(TR::Compilation *, int32_t index, TR::Block *block);
      int32_t getNumber() { return _originalBlock ? _originalBlock->getNumber() : -1; }
      };

   TR_RegionAnalysis(TR::Compilation *comp, TR_Dominators &dominators, TR::CFG * cfg, const TR::Allocator &allocator) :
      _compilation(comp),
      _allocator(allocator),
      _infoTable(cfg->getNumberOfNodes(), _allocator),
      _dominators(dominators),
      _cfg(cfg)
      {
      }

   TR::Compilation *_compilation;

   TR::Allocator _allocator;

   /** The StructInfoTable is 1-based */
   StructInfo &getInfo(int32_t index) { return _infoTable[index+1]; }
   InfoTable       _infoTable;

   /** Total number of nodes in the flow graph */
   int32_t     _totalNumberOfNodes;

   /** Number of active nodes left in the flow graph */
   int32_t     _numberOfActiveNodes;

   /** Dominator information */
   TR_Dominators &_dominators;

   TR::CFG *      _cfg;

   bool _trace;
   bool _useNew;
   void createLeafStructures(TR::CFG *cfg);

   TR_Structure       *findRegions();
   TR_RegionStructure *findNaturalLoop(StructInfo &node,
                                       WorkBitVector &regionNodes,
                                       WorkBitVector &nodesInPath);
   void                addNaturalLoopNodes(StructInfo &node,
                                           WorkBitVector &regionNodes,
                                           WorkBitVector &nodesInPath,
                                           bool &cyclesFound,
                                           TR::Block *hdrBlock);

   void                addNaturalLoopNodesIterativeVersion(StructInfo &node,
                                                           WorkBitVector &regionNodes,
                                                           WorkBitVector &nodesInPath,
                                                           bool &cyclesFound,
                                                           TR::Block *hdrBlock);

   TR_RegionStructure *findRegion(StructInfo &node,
                                  WorkBitVector &regionNodes,
                                  WorkBitVector &nodesInPath);
   void                addRegionNodes(StructInfo &node,
                                      WorkBitVector &regionNodes,
                                      WorkBitVector &nodesInPath,
                                      bool &cyclesFound,
                                      TR::Block *hdrBlock);

   void                addRegionNodesIterativeVersion(StructInfo &node,
                                                      WorkBitVector &regionNodes,
                                                      WorkBitVector &nodesInPath,
                                                      bool &cyclesFound,
                                                      TR::Block *hdrBlock);

   void                buildRegionSubGraph(TR_RegionStructure *region,
                                           StructInfo &entryNode,
                                           WorkBitVector &regionNodes,
                                           SubGraphNodes &cfgNodes);

   };

#endif

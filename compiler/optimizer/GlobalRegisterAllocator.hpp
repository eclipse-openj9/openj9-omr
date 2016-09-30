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

#ifndef GLOBAL_REGISTER_ALLOCATOR_INCL
#define GLOBAL_REGISTER_ALLOCATOR_INCL

#include <stddef.h>                           // for NULL
#include <stdint.h>                           // for int32_t, uint32_t
#include "compile/Compilation.hpp"            // for Compilation
#include "cs2/hashtab.h"                      // for HashTable
#include "env/TRMemory.hpp"                   // for TR_Memory, etc
#include "il/DataTypes.hpp"                   // for DataTypes
#include "il/Node.hpp"                        // for Node, vcount_t
#include "infra/BitVector.hpp"                // for TR_BitVector
#include "infra/Link.hpp"                     // for TR_Link, TR_LinkHead
#include "infra/List.hpp"                     // for List, etc
#include "optimizer/Optimization.hpp"         // for Optimization
#include "optimizer/OptimizationManager.hpp"  // for OptimizationManager
#include "optimizer/UseDefInfo.hpp"           // for TR_UseDefInfo, etc

class TR_GlobalRegister;
class TR_NodeMappings;
class TR_RegisterCandidate;
class TR_RegisterCandidates;
class TR_StructureSubGraphNode;
namespace TR { class Block; }
namespace TR { class Symbol; }
namespace TR { class SymbolReference; }
namespace TR { class TreeTop; }
template <class T> class TR_Array;

struct StoresInBlockInfo : public TR_Link<StoresInBlockInfo>
   {
   TR_ALLOC(TR_Memory::GlobalRegisterAllocator)

   TR_GlobalRegister  *_gr;
   TR::TreeTop *_lastStore;
   bool _origStoreExists;
   };

struct TR_PairedSymbols
   {
   TR_ALLOC(TR_Memory::GlobalRegisterAllocator)

   TR_PairedSymbols(TR::SymbolReference *symRef1, TR::SymbolReference *symRef2) : _symRef1(symRef1), _symRef2(symRef2) {}

   TR::SymbolReference *_symRef1;
   TR::SymbolReference *_symRef2;
   };

struct TR_SymRefCandidatePair
   {
   TR_ALLOC(TR_Memory::GlobalRegisterAllocator)

   TR_SymRefCandidatePair(TR::SymbolReference *symRef, TR_RegisterCandidate *rc) : _symRef(symRef), _rc(rc) {}

   TR::SymbolReference *_symRef;
   TR_RegisterCandidate *_rc;
   };

struct TR_NodeTriple
   {
   TR_ALLOC(TR_Memory::GlobalRegisterAllocator)

   TR_NodeTriple(TR::Node *node1, TR::Node *node2, TR::Node *node3, int32_t childNum) : _node1(node1), _node2(node2), _node3(node3), _childNum(childNum) {}

   TR::Node *_node1;
   TR::Node *_node2;
   TR::Node *_node3;
   int32_t _childNum;
   };

class TR_LiveRangeSplitter : public TR::Optimization
   {
   public:
   TR_LiveRangeSplitter(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_LiveRangeSplitter(manager);
      }

   virtual int32_t perform();

   void splitLiveRanges();
   void splitLiveRanges(TR_StructureSubGraphNode *structureNode);
   void replaceAutosUsedIn(TR::TreeTop *currentTree, TR::Node *node, TR::Node *parent, TR::Block * block, List<TR::Block> *blocksInLoop, List<TR::Block> *exitBlocks, vcount_t visitCount, int32_t executionFrequency, TR_RegisterCandidate **registerCandidates, TR_SymRefCandidatePair **correspondingSymRefs, TR_BitVector *replacedAutosInCurrentLoop, TR_BitVector *autosThatCannotBereplacedInCurrentLoop, TR_StructureSubGraphNode *loop, TR::Block *loopInvariantBlock);
   void placeStoresInLoopExits(TR::Node *node, TR_StructureSubGraphNode *loop, List<TR::Block> *blocksInLoop, TR::SymbolReference *orig, TR::SymbolReference *replacement);
   TR_SymRefCandidatePair *splitAndFixPreHeader(TR::SymbolReference *symRef, TR_SymRefCandidatePair **correspondingSymRefs, TR::Block *loopInvariantBlock, TR::Node *node);
   void fixExitsAfterSplit(TR::SymbolReference *symRef, TR_SymRefCandidatePair *correspondingSymRefCandidate, TR_SymRefCandidatePair **correspondingSymRefs, TR::Block *loopInvariantBlock, List<TR::Block> *blocksInLoop, TR::Node *node, TR_RegisterCandidate **registerCandidates, TR_StructureSubGraphNode *loop, TR_BitVector *replacedAutosInCurrentLoop, TR::SymbolReference *origSymRef);
   void appendStoreToBlock(TR::SymbolReference *storeSymRef, TR::SymbolReference *loadSymRef, TR::Block *block, TR::Node *node);
   void prependStoreToBlock(TR::SymbolReference *storeSymRef, TR::SymbolReference *loadSymRef, TR::Block *block, TR::Node *node);

   private:
   List<TR::Block> _splitBlocks;
   int32_t _numberOfGPRs;
   int32_t _numberOfFPRs;
   TR_BitVector *_storedSymRefs;
   bool _changedSomething;
   TR::SymbolReference **_origSymRefs;
   int32_t _origSymRefCount;
   };

struct TR_ReplaceNode
   {
   TR_ALLOC(TR_Memory::GlobalRegisterAllocator)
   TR_ReplaceNode() : _old(NULL), _new(NULL) {};
   TR_ReplaceNode(TR::Node *o, TR::Node *n) : _old(o), _new(n) {};
   TR::Node *_old;
   TR::Node *_new;
   };


class TR_GlobalRegisterAllocator : public TR::Optimization
   {
public:
   TR_GlobalRegisterAllocator(TR::OptimizationManager *manager);
   static TR::Optimization *create(TR::OptimizationManager *manager)
      {
      return new (manager->allocator()) TR_GlobalRegisterAllocator(manager);
      }

   virtual int32_t perform();

   void walkTreesAndCollectSymbolDataTypes();
   void visitNodeForDataType(TR::Node*);

   bool candidateCouldNeedSignExtension(int32_t symRefNum)
      {
      return (_candidatesNeedingSignExtension &&
              _candidatesNeedingSignExtension->get(symRefNum));
      }

   TR_BitVector *signExtensionRequired() { return _signExtAdjustmentReqd; }
   void setSignExtensionRequired(bool b, int32_t regNum)
      {
      if (b)
         _signExtAdjustmentReqd->set(regNum);
      else
         _signExtAdjustmentReqd->reset(regNum);
      }

   TR_BitVector *signExtensionNotRequired() { return _signExtAdjustmentNotReqd; }
   void setSignExtensionNotRequired(bool b, int32_t regNum)
      {
      if (b)
         _signExtAdjustmentNotReqd->set(regNum);
      else
         _signExtAdjustmentNotReqd->reset(regNum);
      }

   TR::Node *           resolveTypeMismatch(TR::DataType oldType, TR::Node *newNode);
   TR::Node *           resolveTypeMismatch(TR::Node *oldNode, TR::Node *newNode);
   TR::Node *           resolveTypeMismatch(TR::DataType inputOldType, TR::Node *oldNode, TR::Node *newNode);

private:

   void                findIfThenRegisterCandidates();
   void                findLoopAutoRegisterCandidates();
   void                findLoopsAndCorrespondingAutos(TR_StructureSubGraphNode *, vcount_t, TR_RegisterCandidate **);
   void                findLoopsAndAutosNoStructureInfo(vcount_t visitCount, TR_RegisterCandidate **registerCandidates);
   void                initializeControlFlowInfo();
   void                markAutosUsedIn(TR::Node *, TR::Node *, TR::Node *, TR::Node **, TR::Block *, List<TR::Block> *, vcount_t, int32_t, TR_RegisterCandidate **, TR_BitVector *, TR_BitVector *, bool);
   void                signExtendAllDefNodes(TR::Node *, List<TR::Node> *);
   void                findSymsUsedInIndirectAccesses(TR::Node *, TR_BitVector *, TR_BitVector *, bool);

   void                offerAllAutosAndRegisterParmAsCandidates(TR::Block **, int32_t, bool onlySelectedCandidates = false);
   void                offerAllFPAutosAndParmsAsCandidates(TR::Block **, int32_t);

   bool                isDependentStore(TR::Node *, const TR_UseDefInfo::BitVector &, TR::SymbolReference *, bool *);

   bool                allocateForSymRef(TR::SymbolReference *symRef);
   bool                isSymRefAvailable(TR::SymbolReference *symRef);
   bool                isSymRefAvailable(TR::SymbolReference *symRef, List<TR::Block> *blocksInLoop);
   bool                allocateForType(TR::DataType dt);
   bool                isNodeAvailable(TR::Node *node);
   bool                isTypeAvailable(TR::SymbolReference *symref);

   void                assignRegisters();
   void                assignRegisters(TR::Block *);
   void                transformBlock(TR::TreeTop *);
   void                transformNode(TR::Node *, TR::Node *, int32_t, TR::TreeTop *, TR::Block * &, TR_Array<TR_GlobalRegister> &, TR_NodeMappings *);
   TR_GlobalRegister * getGlobalRegister(TR::Symbol *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   TR_GlobalRegister * getGlobalRegisterWithoutChangingCurrentCandidate(TR::Symbol *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   void                transformBlockExit(TR::TreeTop *, TR::Node *, TR::Block *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   void                transformMultiWayBranch(TR::TreeTop *, TR::Node *, TR::Block *, TR_Array<TR_GlobalRegister> &, bool regStarTransformDone = false);
   void                addCandidateReloadsToEntry(TR::TreeTop *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   void                addRegLoadsToEntry(TR::TreeTop *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   void                addGlRegDepToExit(TR_Array<TR::Node *> &, TR::Node *, TR_Array<TR_GlobalRegister> &, TR::Block *);
   void                addStoresForCatchBlockLoads(TR::TreeTop *appendPoint, TR_Array<TR_GlobalRegister> &extBlockRegisters, TR::Block *throwingBlock);
   TR::TreeTop *        findPrevTreeTop(TR::TreeTop * &, TR::Node * &, TR::Block *, TR::Block *);
   void                prepareForBlockExit(TR::TreeTop * &, TR::Node * &, TR::Block *, TR_Array<TR_GlobalRegister> &,
                                           TR::Block *, TR_Array<TR::Node *> &);
   bool                markCandidateForReloadInSuccessors(int32_t, TR_GlobalRegister *, TR_GlobalRegister *, TR::Block *, bool);
   void                reloadNonRegStarVariables(TR::TreeTop *, TR::Node *, TR::Block *, bool);

   bool                registerIsLiveAcrossEdge(TR::TreeTop *, TR::Node *, TR::Block *, TR_GlobalRegister *, TR::Block * &, int32_t);
   TR::Block *          createNewSuccessorBlock(TR::Block *, TR::Block *, TR::TreeTop *, TR::Node *, TR_RegisterCandidate * rc);
   TR::Block *          extendBlock(TR::Block *, TR::Block *);
   TR::Block *          createBlock(TR::Block *, TR::Block *);
   bool                tagCandidates(TR::Block *, bool);
   bool                tagCandidates(TR::Block *, uint32_t, bool);
   int32_t             numberOfRegistersLiveOnEntry(TR_Array<TR_GlobalRegister> &, bool);
   TR::Node *           createStoreToRegister();
   TR::Node *           createStoreFromRegister();
   TR::Node *           createLoadFromRegister();

   void                appendGotoBlock(TR::Block *gotoBlock, TR::Block *curBlock);
   TR::Block           *getAppendBlock(TR::Block *);

   CS2::HashTable<int32_t, TR::SymbolReference *, TR::Allocator> _defIndexToTempMap;

   TR_ScratchList<TR_PairedSymbols> _pairedSymbols;

   TR::SparseBitVector _allocatedPerformCellSymRefs;
   int32_t _performCellCount;

   TR_PairedSymbols   *findOrCreatePairedSymbols(TR::SymbolReference *symRef1, TR::SymbolReference *symRef2);
   TR_PairedSymbols   *findPairedSymbols(TR::SymbolReference *symRef1, TR::SymbolReference *symRef2);
   TR_ScratchList<TR_PairedSymbols>   *getPairedSymbols() { return &_pairedSymbols; }

   StoresInBlockInfo * findRegInStoreInfo(TR_GlobalRegister *);

   bool isSplittingCopy(TR::Node *node);
   void restoreOriginalSymbol(TR::Node *node, vcount_t visitCount);

   void findLoopsAndCreateAutosForSignExt(TR_StructureSubGraphNode *structureNode, vcount_t visitCount);
   void createStoresForSignExt(TR::Node *node, TR::Node *parent, TR::Node *grandParent, TR::TreeTop *treetop, TR::Node **currentArrayAccess, TR::Block * block, List<TR::Block> *blocksInLoop, vcount_t visitCount, bool hasCatchBlock);

   void populateSymRefNodes(TR::Node *node, vcount_t visitCount);

   /*
   void splitLiveRanges();
   void splitLiveRanges(TR_StructureSubGraphNode *structureNode, vcount_t visitCount, TR_RegisterCandidate **registerCandidates);
   void replaceAutosUsedIn(TR::Node *node, TR::Node *parent, TR::Block * block, List<TR::Block> *blocksInLoop, vcount_t visitCount, int32_t executionFrequency, TR_RegisterCandidate **registerCandidates, TR_SymRefCandidatePair **correspondingSymRefs, TR_BitVector *replacedAutosInCurrentLoop, TR_StructureSubGraphNode *loop, TR::Block *loopInvariantBlock);
   void placeStoresInPreHeaderAndInLoopExits(TR::Node *node, TR_StructureSubGraphNode *loop, TR::Block *loopInvariantBlock, List<TR::Block> *blocksInLoop, TR::SymbolReference *orig, TR::SymbolReference *replacement);
   void appendStoreToBlock(TR::SymbolReference *storeSymRef, TR::SymbolReference *loadSymRef, TR::Block *block, TR::Node *node);
   */

   vcount_t     _visitCount;
   int32_t      _firstGlobalRegisterNumber;
   int32_t      _lastGlobalRegisterNumber;
   bool         _gotoCreated;
   TR::Block    *_appendBlock;
   TR_RegisterCandidate **_registerCandidates;
   TR_ScratchList<TR::Block> _newBlocks;
   TR_BitVector *_temp, *_temp2;
   TR_BitVector *_candidatesNeedingSignExtension;
   TR_BitVector *_candidatesSignExtendedInThisLoop;
   List<TR::Node> _nodesToBeChecked;
   List<TR_NodeTriple> _nodeTriplesToBeChecked;
   List<TR::Node> _rejectedSignExtNodes;
   TR::SymbolReference *_storeSymRef;
   TR_LinkHead<StoresInBlockInfo> _storesInBlockInfo;
   TR_BitVector *_signExtAdjustmentReqd;
   TR_BitVector *_signExtAdjustmentNotReqd;
   TR_BitVector *_signExtDifference;
   TR_BitVector *_valueModifiedSymRefs;
   TR_RegisterCandidates * _candidates;
   TR_BitVector *_seenInternalMethods;

   TR::Node **_nodesForSymRefs;

   struct BlockInfo
      {
      TR_ALLOC(TR_Memory::GlobalRegisterAllocator)

      BlockInfo() : _alwaysReached(false) { }

      bool           _inALoop;
      bool           _alwaysReached;
      };

   BlockInfo & blockInfo(int32_t);
   BlockInfo *               _blockInfo;
   int32_t                   _origSymRefCount;
   TR::Block                  *_osrCatchSucc;
   };
#endif

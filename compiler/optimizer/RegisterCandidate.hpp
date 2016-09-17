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

#ifndef REGISTERCANDIDATE_INCL
#define REGISTERCANDIDATE_INCL

#include <stddef.h>                       // for NULL
#include <stdint.h>                       // for int32_t, uint32_t, uint8_t
#include "codegen/RegisterConstants.hpp"  // for TR_GlobalRegisterNumber, etc
#include "cs2/bitvectr.h"                 // for ABitVector, etc
#include "cs2/hashtab.h"                  // for HashTable, HashIndex
#include "cs2/sparsrbit.h"
#include "cs2/tableof.h"                  // for TableOf
#include "env/TRMemory.hpp"               // for Allocator, TR_Memory, etc
#include "il/DataTypes.hpp"               // for TR::DataType, DataTypes
#include "il/Node.hpp"                    // for Node (ptr only), etc
#include "infra/Array.hpp"                // for TR_Array
#include "infra/Assert.hpp"               // for TR_ASSERT
#include "infra/BitVector.hpp"            // for TR_BitVector, etc
#include "infra/Cfg.hpp"                  // for CFG, CFGBase::::EndBlock, etc
#include "infra/Link.hpp"                 // for TR_LinkHead, TR_Link
#include "infra/List.hpp"                 // for List

class TR_GlobalRegisterAllocator;
class TR_Structure;
namespace TR { class Block; }
namespace TR { class Compilation; }
namespace TR { class Symbol; }
namespace TR { class SymbolReference; }
namespace TR { class TreeTop; }

namespace TR
{

class GlobalSet
   {
public:
   GlobalSet(TR::Compilation * comp, const TR::Allocator & alloc);

   class Set;
   Set * operator[](uint32_t blockNum)
   {
    if(blockNum == TR::CFG::StartBlock || blockNum == TR::CFG::EndBlock)
        return NULL;
    return _refAutosPerBlock.ElementAt(blockNum);
   }

   void collectReferencedAutoSymRefs(TR::Block * BB);
   bool isEmpty() { return _refAutosPerBlock.IsEmpty(); }
   void makeEmpty() { _refAutosPerBlock.MakeEmpty(); }
   void initialize(uint32_t numSymRefs, int32_t numBlocks) {_maxEntries = numSymRefs * numBlocks;}

   //Set, SparseSet and DenseSet
   class Set
      {
   public:
      virtual bool get(uint32_t refNum)            = 0;
      virtual void set(uint32_t symRefNum)         = 0;
      virtual void print(TR::Compilation* comp)    = 0;
   };

private:

   class SparseSet: public Set
   {
   public:
   TR_ALLOC(TR_Memory::RegisterCandidates);
   SparseSet(TR::Allocator & alloc):_refs(alloc){}
   virtual bool get(uint32_t refId) {return _refs.ValueAt(refId); }
   virtual void set(uint32_t refId) {_refs[refId] = 1;     }
   virtual void print(TR::Compilation * comp);
   private:
   TR::SparseBitVector _refs;
   };

   class DenseSet: public Set
   {
   public:
   TR_ALLOC(TR_Memory::RegisterCandidates);
   DenseSet(TR::Allocator & alloc):_refs(alloc){}
   virtual bool get(uint32_t refId) {return _refs.ValueAt(refId); }
   virtual void set(uint32_t refId) {_refs[refId] = 1    ; }
   virtual void print(TR::Compilation * comp);
   private:
   CS2::ABitVector<TR::Allocator> _refs;
   };



   enum
   {
     MB50     = 0x19000000,
     MB100    = 0x32000000
   };

   void collectReferencedAutoSymRefs(TR::Node * node, Set * referencedAutos, vcount_t visitCount);

   CS2::TableOf<Set *, TR::Allocator> _refAutosPerBlock;
   TR::Compilation * _comp;
   uint32_t _maxEntries;
   };

}

enum TR_RegisterCandidateTypes
   {
   TR_InductionVariableCandidate = 0,
   TR_PRECandidate               = 1,
   TR_LastRegisterCandidateType  = TR_PRECandidate,
   TR_NumRegisterCandidateTypes  = TR_LastRegisterCandidateType+1
   };


class TR_RegisterCandidate : public TR_Link<TR_RegisterCandidate>
   {
public:
   TR_RegisterCandidate(TR::SymbolReference *, TR_Memory *, TR::Allocator);

   class BlockInfo {
     TR_BitVector _block_set;
     CS2::HashTable<uint32_t, uint32_t, TR::Allocator > _block_map;

   public:
     BlockInfo(TR_Memory *m, TR::Allocator &a) : _block_set(),
     _block_map(_block_map.DefaultSize, TR::Allocator(a)) {
       _block_set.init(1024, m, heapAlloc, growable);
     }

     void setNumberOfLoadsAndStores(uint32_t block, uint32_t count) {
       _block_set.set(block);
       CS2::HashIndex hi;
       if (_block_map.Locate(block, hi))
         _block_map[hi]=count;
       else if (count)
         _block_map.Add(block, count);
     }
     void incNumberOfLoadsAndStores(uint32_t block, uint32_t count) {
       _block_set.set(block);
       CS2::HashIndex hi;
       if (_block_map.Locate(block, hi))
         _block_map[hi]+=count;
       else if (count)
         _block_map.Add(block, count);
     }
     void removeBlock(uint32_t block) {
       _block_set.reset(block);
       CS2::HashIndex hi;
       if (_block_map.Locate(block, hi))
         _block_map.Remove(hi);
     }

     bool find(uint32_t block) {
       return (bool)_block_set.get(block);
     }
     uint32_t getNumberOfLoadsAndStores(uint32_t block) {
       CS2::HashIndex hi;
       if (find(block) && _block_map.Locate(block, hi))
         return _block_map[hi];
       return 0;
     }

     typedef TR_BitVectorIterator Cursor;
     Cursor all() {
       return Cursor(_block_set);
     }
   };

   struct LoopInfo : TR_Link<LoopInfo>
      {
      LoopInfo(TR_Structure * l, int32_t n) : _loop(l), _numberOfLoadsAndStores(n) { }
      TR_Structure * _loop;
      int32_t _numberOfLoadsAndStores;
      };

   void addAllBlocks()         { _allBlocks = true; }

   void addBlock(TR::Block * b, int32_t numberOfLoadsAndStores, TR_Memory *, bool ifNotFound = false);

   void addLoopExitBlock(TR::Block *b);
   int32_t removeBlock(TR::Block * b);
   void removeLoopExitBlock(TR::Block * b);
   int32_t countNumberOfLoadsAndStoresInBlocks(List<TR::Block> *);

#ifdef TRIM_ASSIGNED_CANDIDATES
   void addLoop(TR_Structure * l, int32_t numberOfLoadsAndStores, TR_Memory *);
   LoopInfo * find(TR_Structure * l);
   void countNumberOfLoadsAndStoresInLoops(TR_Memory * m);
   void printLoopInfo(TR::Compilation *comp);
   int32_t getNumberOfLoadsAndStoresInLoop (TR_Structure *);
#endif

   void addAllBlocksInStructure(TR_Structure *, TR::Compilation *, const char *description, vcount_t count = MAX_VCOUNT, bool recursiveCall = false);

   bool find(TR::Block * b);
   bool hasBlock(TR::Block * b) { return find(b) != 0; }
   bool hasLoopExitBlock(TR::Block * b);

   TR_GlobalRegisterNumber getGlobalRegisterNumber()  { return _lowRegNumber; }
   TR_GlobalRegisterNumber getHighGlobalRegisterNumber()  { return _highRegNumber; }
   TR_GlobalRegisterNumber getLowGlobalRegisterNumber()  { return _lowRegNumber; }
   bool hasSameGlobalRegisterNumberAs(TR::Node *node, TR::Compilation *comp);

   TR::SymbolReference *    getSymbolReference()       { return _symRef; }
   TR::SymbolReference *    getSplitSymbolReference()  { return _splitSymRef; }
   void setSplitSymbolReference(TR::SymbolReference *symRef)  { _splitSymRef = symRef; }
   TR::SymbolReference *    getRestoreSymbolReference()  { return _restoreSymRef; }
   void setRestoreSymbolReference(TR::SymbolReference *symRef)  { _restoreSymRef = symRef; }


   TR_BitVector &          getBlocksLiveOnEntry()     { return _liveOnEntry; }
   TR_BitVector &          getBlocksLiveOnExit()      { return _liveOnExit; }
   TR_BitVector &          getBlocksLiveWithinGenSetsOnly() { return _liveWithinGenSetsOnly; }
   TR::Symbol *            getSymbol();

   TR::DataTypes           getDataType();
   TR::DataType            getType();
   bool                    rcNeeds2Regs(TR::Compilation *);
   TR_RegisterKinds        getRegisterKinds();

   uint32_t                getWeight()                { return _weight; }
   int32_t                 is8BitGlobalGPR()          { return _8BitGlobalGPR; }
   int32_t                 getFailedToAssignToARegister() { return _failedToAssignToARegister; }

   bool symbolIsLive(TR::Block *);
   bool canBeReprioritized() { return (_reprioritized > 0); }
   bool getValueModified() { return _valueModified; }

   bool isLiveAcrossExceptionEdge() { return _liveAcrossExceptionEdge; }
   void setLiveAcrossExceptionEdge(bool b) { _liveAcrossExceptionEdge = b; }

   bool highWordZero() { return _highWordZero; }
   void setHighWordZero(bool b) { _highWordZero = b; }

   void setGlobalRegisterNumber(TR_GlobalRegisterNumber n) { _lowRegNumber = n; }
   void setLowGlobalRegisterNumber(TR_GlobalRegisterNumber n) { _lowRegNumber = n; }
   void setHighGlobalRegisterNumber(TR_GlobalRegisterNumber n) { _highRegNumber = n; }

   void setWeight(TR::Block * *, int32_t *, TR::Compilation *,
                  TR_Array<int32_t>&,TR_Array<int32_t>&, TR_Array<int32_t>&,
                  TR_BitVector *, TR_Array<TR::Block *>& startOfExtendedBB, TR_BitVector &, TR_BitVector &);
   void setIs8BitGlobalGPR(bool b) { _8BitGlobalGPR = b; }
   void setReprioritized() { _reprioritized--; }
   void setMaxReprioritized(uint8_t n) { _reprioritized = n;}
   uint8_t getReprioritized() { return  _reprioritized; }
   void setValueModified(bool b) { _valueModified = b; }
   void processLiveOnEntryBlocks(TR::Block * *, int32_t *, TR::Compilation *,
                                 TR_Array<int32_t>&,TR_Array<int32_t>&, TR_Array<int32_t>&,
                                 TR_BitVector *, TR_Array<TR::Block *>& startOfExtendedBB, bool removeUnusedLoops = false);
   void removeAssignedCandidateFromLoop(TR::Compilation *, TR_Structure *, int32_t,
                                        TR_BitVector *, TR_BitVector *, bool);

   void extendLiveRangesForLiveOnExit(TR::Compilation *, TR::Block **, TR_Array<TR::Block *>& startOfExtendedBBForBB);

   TR_BitVector *          getAvailableOnExit()     { return _availableOnExit; }
   TR_BitVector *          getBlocksVisited()       { return _blocksVisited; }

   void          setAvailableOnExit(TR_BitVector *b)     { _availableOnExit = b; }
   void          setBlocksVisited(TR_BitVector *b)       { _blocksVisited = b; }

   void recalculateWeight(TR::Block * *, int32_t *, TR::Compilation *,
                          TR_Array<int32_t>&,TR_Array<int32_t>&,TR_Array<int32_t>&,TR_BitVector *, TR_Array<TR::Block *>& startOfExtendedBB);
   bool prevBlockTooRegisterConstrained(TR::Compilation *comp, TR::Block *,TR_Array<int32_t>&,TR_Array<int32_t>&,TR_Array<int32_t>&);
   List<TR::TreeTop> & getStores() { return _stores; }

   List<TR_Structure> & getLoopsWithHoles() { return _loopsWithHoles; }
   void addLoopWithHole(TR_Structure *s) { if (!_loopsWithHoles.find(s)) _loopsWithHoles.add(s); }

   bool dontAssignVMThreadRegister() { return _dontAssignVMThreadRegister; }
   void setDontAssignVMThreadRegister(bool b) { _dontAssignVMThreadRegister = b; }

   bool extendedLiveRange() { return _extendedLiveRange; }
   void setExtendedLiveRange(bool b) { _extendedLiveRange = b; }

   bool initialBlocksWeightComputed() { return _initialBlocksWeightComputed; }
   void setInitialBlocksWeightComputed(bool b) { _initialBlocksWeightComputed = b; }

   void addStore(TR::TreeTop * tt)
      { _stores.add(tt); }

   TR::Node  *getMostRecentValue(){ return _mostRecentValue; }
   void      setMostRecentValue(TR::Node *node){ _mostRecentValue = node; }
   TR::Node  *getLastLoad(){ return _lastLoad; }
   void      setLastLoad(TR::Node *node){ _lastLoad = node; }

   bool canAllocateDespiteAliases(TR::Compilation *);

private:
   friend class TR_RegisterCandidates;
   friend class TR_GlobalRegisterAllocator;

   TR::SymbolReference *    _symRef;
   TR::SymbolReference *    _splitSymRef;
   TR::SymbolReference *    _restoreSymRef;
   uint32_t                _weight;
   TR_GlobalRegisterNumber _lowRegNumber;
   TR_GlobalRegisterNumber _highRegNumber;
   BlockInfo               _blocks;
   List<TR::Block>          _loopExitBlocks;
   TR_BitVector            _liveOnEntry;
   TR_BitVector            _liveOnExit;
   TR_BitVector            _originalLiveOnEntry;
   TR_BitVector            _liveWithinGenSetsOnly;
   TR_BitVector           *_availableOnExit;
   TR_BitVector           *_blocksVisited;
   TR_Array<uint32_t> *    _loadsAndStores;
   List<TR::TreeTop>        _stores;
   List<TR_Structure>      _loopsWithHoles;
   TR::Node                *_mostRecentValue;
   TR::Node                *_lastLoad;
   bool                    _allBlocks;
   bool                    _failedToAssignToARegister;
   bool                    _8BitGlobalGPR;
   uint8_t                 _reprioritized;
   bool                    _valueModified;
   bool                    _liveAcrossExceptionEdge;
   bool                    _highWordZero;
   bool                    _dontAssignVMThreadRegister;
   bool                    _extendedLiveRange;
   bool                    _initialBlocksWeightComputed;

#ifdef TRIM_ASSIGNED_CANDIDATES
   TR_LinkHead<LoopInfo>   _loops; // loops candidate is used in
#endif
   };

class TR_RegisterCandidates
   {
public:
   TR_ALLOC(TR_Memory::RegisterCandidates)

   TR_RegisterCandidates(TR::Compilation *);

   TR::Compilation *          comp()                        { return _compilation; }

   TR_Memory *               trMemory()                    { return _trMemory; }
   TR_StackMemory            trStackMemory()               { return _trMemory; }
   TR_HeapMemory             trHeapMemory()                { return _trMemory; }
   TR_PersistentMemory *     trPersistentMemory()          { return _trMemory->trPersistentMemory(); }

   TR_RegisterCandidate * findOrCreate(TR::SymbolReference * symRef);
   TR_RegisterCandidate * find(TR::SymbolReference * symRef);
   TR_RegisterCandidate * find(TR::Symbol * sym);

   TR::GlobalSet&      getReferencedAutoSymRefs() { return _referencedAutoSymRefsInBlock; }
   TR::GlobalSet::Set *getReferencedAutoSymRefsInBlock(int32_t i)
      {
      if (_referencedAutoSymRefsInBlock.isEmpty())
         return 0;
      return _referencedAutoSymRefsInBlock[i];
      }

   bool assign(TR::Block **, int32_t, int32_t &, int32_t &);
   void computeAvailableRegisters(TR_RegisterCandidate *, int32_t, int32_t, TR::Block **, TR_BitVector *);

   static int32_t getWeightForType(TR_RegisterCandidateTypes type)
      {
      return _candidateTypeWeights[type];
      }

   TR_RegisterCandidate * getFirst() { return _candidates.getFirst(); }

   friend class TR_GlobalRegisterAllocator;

   TR_RegisterCandidate *newCandidate(TR::SymbolReference *ref);

   void releaseCandidates() {
     _candidates.setFirst(0);
     _candidateTable.MakeEmpty();
   }

   void collectCfgProperties(TR::Block **, int32_t);

private:
   bool candidatesOverlap(TR::Block *, TR_RegisterCandidate *, TR_RegisterCandidate *, bool);
   void lookForCandidates(TR::Node *, TR::Symbol *, TR::Symbol *, bool &, bool &);
   bool prioritizeCandidate(TR_RegisterCandidate *, TR_RegisterCandidate * &);
   TR_RegisterCandidate * reprioritizeCandidates(TR_RegisterCandidate *, TR::Block * *, int32_t *, int32_t, TR::Block *, TR::Compilation *,
                                                 bool reprioritizeFP, bool onlyReprioritizeLongs, TR_BitVector *referencedBlocks,
                                                 TR_Array<int32_t> & blockGPRCount, TR_Array<int32_t> & blockFPRCount, TR_Array<int32_t> & blockVRFCount,
                                                 TR_BitVector *, bool);

   bool aliasesPreventAllocation(TR::Compilation *comp, TR::SymbolReference *symRef);

   TR::Compilation                   *_compilation;
   TR_Memory *                       _trMemory;
   TR_LinkHead<TR_RegisterCandidate> _candidates;
   CS2::TableOf<TR_RegisterCandidate, TR::Allocator> _candidateTable;

   static int32_t                    _candidateTypeWeights[TR_NumRegisterCandidateTypes];
   TR::GlobalSet            _referencedAutoSymRefsInBlock;
   TR_RegisterCandidate **  _candidateForSymRefs;
   uint32_t                 _candidateForSymRefsSize;
   TR_Array<TR::Block *> _startOfExtendedBBForBB;

   // scratch arrays for calculating register conflicts
   TR_Array<TR_BitVector>   _liveOnEntryConflicts;
   TR_Array<TR_BitVector>   _liveOnExitConflicts;
   TR_Array<TR_BitVector>   _exitEntryConflicts;
   TR_Array<TR_BitVector>   _entryExitConflicts;
   TR_Array<TR_BitVector>   _liveOnEntryUsage;
   TR_Array<TR_BitVector>   _liveOnExitUsage;
#ifdef TRIM_ASSIGNED_CANDIDATES
   TR_Array<TR_BitVector>   _availableBlocks;  // blocks available due to empty loops
#endif

   // Candidate invariant info based on CFG
   TR_BitVector                 _firstBlock;
   TR_BitVector                 _isExtensionOfPreviousBlock;

   // Aliasing candidate invariant data
   TR::SparseBitVector _internalCalls;
   bool _internalCallsComputed;

public:
   struct coordinates {
     coordinates(uint32_t f, uint32_t l): first(f), last(l){}
     uint32_t first, last;
   };

   typedef CS2::HashTable<int32_t, struct coordinates, TR::Allocator> Coordinates;
   typedef CS2::TableOf<Coordinates, TR::Allocator > ReferenceTable;

private:
   ReferenceTable *overlapTable;
 };

class TR_GlobalRegister
   {
public:

   TR_ALLOC(TR_Memory::GlobalRegister)

   // no ctor so that it can be zero initialized by the TR_Array template

   TR_RegisterCandidate * getRegisterCandidateOnEntry()  { return _rcOnEntry; }
   TR_RegisterCandidate * getRegisterCandidateOnExit()   { return _rcOnExit; }
   TR_RegisterCandidate * getCurrentRegisterCandidate()  { return _rcCurrent; }
   TR::TreeTop *           getLastRefTreeTop()            { return _lastRef; }
   bool                   getAutoContainsRegisterValue();
   TR::Node *              getValue() { return _value; }
   TR_NodeMappings &      getMappings() { return _mappings; }
   TR::TreeTop *           optimalPlacementForStore(TR::Block *, TR::Compilation *);

   void setRegisterCandidateOnEntry(TR_RegisterCandidate * rc) { _rcOnEntry = rc; }
   void setRegisterCandidateOnExit(TR_RegisterCandidate * rc) { _rcOnExit = rc; }
   void setCurrentRegisterCandidate(TR_RegisterCandidate * rc, vcount_t, TR::Block *, int32_t, TR::Compilation *, bool resetOtherHalfOfLong = true);
     //void setCurrentLongRegisterCandidate(TR_RegisterCandidate * rc, TR_GlobalRegister *otherReg);
   void copyCurrentRegisterCandidate(TR_GlobalRegister *);
   void setValue(TR::Node * n)  { _value = n; }
   void setAutoContainsRegisterValue(bool b) { _autoContainsRegisterValue = b; }
   void setLastRefTreeTop(TR::TreeTop * tt)   { _lastRef = tt; }
   void setReloadRegisterCandidateOnEntry(TR_RegisterCandidate *rc, bool flag=true) { if(rc == _rcOnEntry) _reloadOnEntry=flag; }
   bool getReloadRegisterCandidateOnEntry() { return _reloadOnEntry; }
   void setUnavailable(bool flag=true) { _unavailable=flag; }
   void setUnavailableResolved(bool flag=true) { _unavailableResolved=flag; }
   bool isUnavailable() { return _unavailable; }
   bool isUnavailableResolved() { return _unavailableResolved; }

   TR::Node * createStoreFromRegister(vcount_t, TR::TreeTop *, int32_t, TR::Compilation *, bool storeUnconditionally = false);
   TR::Node * createStoreToRegister(TR::TreeTop *, TR::Node *, vcount_t, TR::Compilation *, TR_GlobalRegisterAllocator *);
   TR::Node * createLoadFromRegister(TR::Node *, TR::Compilation *);

private:
   TR_RegisterCandidate * _rcOnEntry;
   TR_RegisterCandidate * _rcOnExit;
   TR_RegisterCandidate * _rcCurrent;
   TR::Node *              _value;
   TR::TreeTop *           _lastRef;
   bool                   _autoContainsRegisterValue;
   bool                   _reloadOnEntry; // reload the register as this is first block with stack available to reload killed reg
   bool                   _unavailable; // Live register killed but stack unavailable making it impossible to reload
   bool                   _unavailableResolved; // register is unavailable but a reload was found in all successors
   TR_NodeMappings        _mappings;
   };

#endif

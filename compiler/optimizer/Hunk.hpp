#pragma once

#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <unordered_set>
#include <map>
#include "jsmn/jsmn.hpp"

#ifdef J9_PROJECT_SPECIFIC

#include "compile/Compilation.hpp"
#include "../../VPConstraint.hpp"
#include "cs2/allocator.h"
#include "Utils.hpp"
#include "infra/SimpleRegex.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "compile/Method.hpp"
#include "compile/ResolvedMethod.hpp"
#include "infra/Cfg.hpp"                                  // for CFG
#include "infra/TRCfgNode.hpp"                            // for CFGNode
#include "infra/List.hpp" // for List

#endif

#define DFS_LIST

class TR_InlinerBase;
class Proposal_BitVectorImpl;
class MethodSummary;
class AbsVarArray;
class Hunk {
private:
  TR::CFG *_cfg;
public:
  struct HunkPtrOrder {
    bool operator()(Hunk *left, Hunk *right) {
      return left < right;
    }
  };
  typedef HunkPtrOrder HunkPtrComparator;
  typedef TR::typed_allocator< Hunk*, TR::Region & > HunkPtrAllocator;
  typedef std::vector<Hunk*, HunkPtrAllocator> HunkPtrVector;
  typedef std::deque<Hunk*, HunkPtrAllocator> HunkPtrDeque;
  typedef std::stack<Hunk*, HunkPtrDeque> HunkPtrStack;
  typedef std::priority_queue<Hunk*, HunkPtrVector, HunkPtrComparator> HunkPtrPriorityQueue;
  typedef HunkPtrPriorityQueue hunks_q;
  typedef std::map<Hunk*, bool, HunkPtrOrder, HunkPtrAllocator> HunkPtrUnorderedSet;
  Hunk(const char* name, const int bytecode_idx, TR::Compilation*, TR_InlinerBase *inliner);
  TR_ResolvedMethod *method;
  static Hunk* find(Hunk*, int);
  static Hunk* find_array_implementation(Hunk*, int);
  //static Hunk* findCallsite(Hunk*, const char *, size_t);
  //static Hunk* findContextSensitive(Hunk*, Hunk*, const char *, size_t, TR::Compilation* comp = NULL);
  inline int callee_idx() const { return this->_callee_idx; }
  int caller_idx() const;
  int find_max_callee_index(Hunk* root = NULL) ;
  int find_max_callee_index_array_implementation(Hunk* root = NULL) ;
  size_t bytecode_idx() const;
  bool is_leaf() const;
  inline bool is_leaf_array_implementation() const {
    return this->_subordinates_array == NULL || this->_subordinates_array[0] == NULL;
  }
  void bytecode_idx(size_t);
  Hunk* prerequisite() const;
  void enqueue_subordinates(Hunk::hunks_q*) const;
  void enqueue_subordinates_array_implementation(Hunk::hunks_q*) const;
  const char * name() const;
  char * table(TR::Compilation *comp, int whoami=-1, int whocallsme=-1, int* retChildId = NULL) const;
  char* concat(const char*, const char*, TR::Compilation *comp) const;
  char* format(TR::Compilation *comp, const int, const int, const char*, const size_t) const;
  const char * hotness() const;
  int numberOfParameters() const;
  Hunk* getCalleeIndex(int callee_idx, Hunk* root=NULL);
  Hunk* getCalleeIndex_array_implementation(int callee_idx, Hunk* root=NULL);
  Hunk* getCalleeIndex_array_implementation_binary_search(int callee_idx, Hunk* root=NULL);
  bool is_root() const;
  Hunk *root();
  Hunk *get_root();
  int cost() const;
  int recursive_cost();
  int recursive_cost_array_implementation();
  void cost(int __cost) { this->_cost = __cost; }
  inline int benefit() const {
    int temp = this->_benefit * (this->multiplier() / 1.0);
    temp = temp < 0 ? INT_MAX : temp;
    return temp == 0 ? 1 : temp;
  }
  void benefit(int __benefit) { this->_benefit = __benefit; }
  size_t count() const;
  size_t count_array_implementation() const;
  bool add_subordinate(Hunk*);
  bool add_subordinate_array_implementation(Hunk *subordinate);
  void hotness(const char * hotness);
  friend std::ostream& operator<<( std::ostream&, const Hunk& );
  friend bool operator<(const Hunk&, const Hunk&);
  friend bool operator>(const Hunk&, const Hunk&);
  //friend bool operator==(const Hunk&, const Hunk&);
  //friend bool operator!=(const Hunk&, const Hunk&);
  //Hunk& operator=(const Hunk&);
  //Hunk* next() const;
  //Hunk* _next() const;
  //Hunk* getNextSibling() const;
  //Hunk* getNextParent() const;
  //static Hunk* convertInlinedCallSiteToIDT(TR::ResolvedMethodSymbol *methodSymbol, TR::Compilation *comp, TR::Region&);
  static void convertAndPrint(TR::ResolvedMethodSymbol*, TR::Compilation*, const char*, TR::Region&);
  static void print(Hunk*, TR::Compilation*, const char*);
  void setResolvedMethodSymbol(TR::ResolvedMethodSymbol* rms);
  bool recursive(const char *);
  TR::ResolvedMethodSymbol* getResolvedMethodSymbol() const;
  void cfg(TR::CFG *cfg);
  TR::CFG *cfg();
  Hunk* findChildWithBytecodeIndex_array_implementation(size_t bcIndex) const;
  Hunk* findChildWithBytecodeIndex(size_t bcIndex) const;
  List<Hunk> findChildrenWithBytecodeIndex_array_implementation(size_t bcIndex) const;
  List<Hunk> findChildrenWithBytecodeIndex(size_t bcIndex) const;
  bool _isVirtual;
  void isVirtual(bool is) { this->_isVirtual = is; };
  bool isVirtual() { return this->_isVirtual; };
  TR::Block **arrayOfBlocks;
  int lengthOfArrayOfBlocks;
  List<Hunk> *dfs(List<Hunk> *children = NULL);
  List<Hunk> *dfs_array_implementation(List<Hunk> *children = NULL);
  void multiplier(float aMult) { this->benefit_multiplier = aMult; }
  float multiplier() const { return this->benefit_multiplier; }
  void setCallTarget(TR_CallTarget *calltarget) { this->_calltarget = calltarget; }
  TR_CallTarget *getCallTarget() { return this->_calltarget; }
  void setCallStack(TR_CallStack* callstack ) { this->_callstack = callstack ; }
  TR_CallStack *getCallStack() { return this->_callstack; }
  bool _visited;
  int _benefit;
  float _frequency;
  float _frequencyStart;
  int getFirstNodeFrequency() { return this->getResolvedMethodSymbol()->getFlowGraph()->getFirstNode()->getFrequency(); }
  void putAllInProposal(Proposal_BitVectorImpl &);
  void putAllInProposal_array_implementation(Proposal_BitVectorImpl &);
  int getInitialBlockFrequency() { return this->getResolvedMethodSymbol()->getFlowGraph()->getInitialBlockFrequency(); }
  int getForLoopBlockFrequency();
  int getFrequencyEntryAccordingToMe();
  void normalizeMultiplier(float normalizeBy);
  void normalizeMultiplier_array_implementation(float normalizeBy);
  float getLargestMultiplier();
  float getLargestMultiplier_array_implementation();
  void normalizeMultiplier();
  HunkPtrVector &subordinates();
  Hunk** _subordinates_array;
  size_t _array_size;
  MethodSummary* _methodSummary;
  AbsVarArray* _argumentConstraints;
  TR::Block **_array_of_blocks;
  int _inline_idx;
private:
  TR_CallTarget *_calltarget;
  TR_CallStack *_callstack;
  float benefit_multiplier;
#ifdef DFS_LIST
  List<Hunk> _dfs;
#endif
  const char* _name;
  TR_InlinerBase *_inliner;
  const char* _hotness;
  int _cost;
  int _caller_idx;
  int _callee_idx;
  int _array_filled;
  size_t _bytecode_idx;
  TR::Compilation *_comp;
  TR::ResolvedMethodSymbol* _rms;
  Hunk* _prerequisite;
  HunkPtrVector *_subordinates;
};

#undef DFS_LIST

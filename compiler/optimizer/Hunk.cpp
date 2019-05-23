#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <assert.h>

#include "Hunk.hpp"
#include "Proposal.hpp"
#include "jsmn/jsmn.hpp"

#ifdef J9_PROJECT_SPECIFIC
#include "infra/SimpleRegex.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/Block.hpp"                                   // for Block
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "compile/Method.hpp"
#include "compile/ResolvedMethod.hpp"
#include "optimizer/Inliner.hpp"
#include "infra/Cfg.hpp"                                  // for CFG
#include "infra/TRCfgNode.hpp"                            // for CFGNode
#include "infra/List.hpp" // for List
#else 
#include <regex>
using ::std::regex;
#endif

using ::std::ifstream;
using ::std::string;
using ::std::ostream;
using ::std::ostringstream;
using ::std::istringstream;
using ::std::vector;
using ::std::priority_queue;
using ::std::greater;
using ::std::getline;
using ::jsmn::jsmn_parser;
using ::jsmn::jsmn_init;
using ::jsmn::JSMN_OBJECT;
using ::jsmn::jsmntok_t;
using ::jsmn::jsoneq;

#ifndef TRACE
#define TRACE(COND, M, ...)
//#define TRACE(COND, M, ...) \
//if ((COND)) { traceMsg(comp, "(%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__); }
#endif

#define DFS_LIST

Hunk::Hunk(const char *name, const int bytecode_idx, TR::Compilation *comp, TR_InlinerBase *inliner) : 
    _subordinates(NULL),
#ifdef DFS_LIST
        _dfs(inliner->_dfsRegion),
#endif
        _inliner(inliner),
        _array_filled(0),
        _argumentConstraints(NULL),
        _array_of_blocks(NULL),
        benefit_multiplier(0.0),
        _calltarget(NULL),
        _callstack(NULL),
        _frequency(-1),
        _frequencyStart(-1),
        _array_size(50),
        _subordinates_array(NULL),
        _methodSummary(NULL),
        _inline_idx(-2),
        method(NULL)
{
    this->_name = name;
    this->_bytecode_idx = bytecode_idx;
        this->_prerequisite = NULL;
        this->_comp = comp;
        this->_isVirtual = false;
        this->_cfg = NULL;
        this->arrayOfBlocks = NULL;
        this->lengthOfArrayOfBlocks = 0;
      this->_callee_idx = -1;
        this->_visited = false;
}

void
Hunk::cfg(TR::CFG *cfg) {
  this->_cfg = cfg;
}

void
Hunk::putAllInProposal_array_implementation(Proposal_BitVectorImpl &proposal) {
  proposal.push_back(this);
  if (this->is_leaf_array_implementation()) return;
  auto subordinates = this->_subordinates_array;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = subordinates[i];
    if (hunk == NULL) return;
    hunk->putAllInProposal(proposal);
  }
}

void
Hunk::putAllInProposal(Proposal_BitVectorImpl &proposal) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
     return this->putAllInProposal_array_implementation(proposal);
  proposal.push_back(this);
  auto subordinates = this->subordinates();
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    Hunk* hunk = *i;
    hunk->putAllInProposal(proposal);
  }
}

TR::CFG *
Hunk::cfg() {
  return this->_cfg;
}

int
Hunk::caller_idx() const {
  return this->_caller_idx;
}

size_t
Hunk::bytecode_idx() const {
  return this->_bytecode_idx;
}

void
Hunk::bytecode_idx(size_t bc) {
  this->_bytecode_idx = bc;
}

const char *
Hunk::hotness() const {
  return this->_hotness;
}

void
Hunk::hotness(const char * __hotness) {
  this->_hotness = __hotness;
}


void
Hunk::enqueue_subordinates_array_implementation(Hunk::hunks_q *queue) const {
  if (this->is_leaf_array_implementation()) return;
  auto subordinates = this->_subordinates_array;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = subordinates[i];
    if (hunk == NULL) return;

    queue->push(hunk);
  }
}

void
Hunk::enqueue_subordinates(Hunk::hunks_q *queue) const {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
     return this->enqueue_subordinates_array_implementation(queue);

  auto subordinates = *this->_subordinates;
  for(auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    queue->push(*i);
  }
}


bool
Hunk::is_leaf() const {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
     return this->is_leaf_array_implementation();

  auto subordinates = *this->_subordinates;
  return subordinates.empty();
}

Hunk::HunkPtrVector&
Hunk::subordinates() {
  return *this->_subordinates;
}

Hunk*
Hunk::prerequisite() const {
  return this->_prerequisite;
}

const char*
Hunk::name() const {
  return this->_name;
}

char*
Hunk::concat(const char* aCharPtr, const char* bCharPtr, TR::Compilation* comp) const {
  auto aCharPtrLen = strlen(aCharPtr);
  auto bCharPtrLen = strlen(bCharPtr);
  auto cCharPtrLen = aCharPtrLen + bCharPtrLen;
  char* cCharPtr = new (comp->region()) char[cCharPtrLen + 1];
  strcpy(cCharPtr, aCharPtr);
  cCharPtr[aCharPtrLen] = '\0';
  strcat(cCharPtr, bCharPtr);
  return cCharPtr;
}

char*
Hunk::format(TR::Compilation *comp, const int calleeIdx, const int callerIdx, const char* name, const size_t bytecode) const {
  auto nameLen = strlen(name);
  const size_t calleeIdxBuff = 5; // safe assumption number won't be bigger than 10000
  const size_t callerIdxBuff = 5; // safe assumption number won't be bigger than 10000
  const size_t bytecodeIdxBuff = 5;
  size_t buffSize = nameLen + calleeIdxBuff + callerIdxBuff + bytecodeIdxBuff;
  char* calleeIdxScript = new (comp->region()) char[buffSize];
  int retval = snprintf(calleeIdxScript, buffSize, "%s,\t%d,\t%d,\t%d\n", name, calleeIdx, callerIdx, bytecode);
  if (retval < 0) {
    // error? 
  }
  return calleeIdxScript;
}
int
Hunk::getForLoopBlockFrequency() {
    for (TR::CFGNode *node = this->getResolvedMethodSymbol()->getFlowGraph()->getFirstNode(); node; node->getNext()) {
      if (node->asBlock() != NULL && node->asBlock()->getEntry() != NULL) { return node->asBlock()->getFrequency(); }
    }
    return -1;
  }

char*
Hunk::table(TR::Compilation *comp, const int whoami, const int whocallsme, int* retChildId) const {
  const char* name = this->name();
  char* out = this->format(comp, whoami, whocallsme, name, this->bytecode_idx());
  int childId = whoami;
  int nextChildId;
  auto subordinates = *this->_subordinates;
  for(auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    auto subordinate = *i;
    char* out2 = subordinate->table(comp, ++childId, whoami, &nextChildId);
    childId = nextChildId;
    out = concat(out, out2, comp);
  }
  if (retChildId != NULL) *retChildId = childId;
  return out;
}


bool
Hunk::add_subordinate_array_implementation(Hunk * subordinate) {
  if (!subordinate) return false;

  if (!this->_subordinates_array) {
        this->_subordinates_array = new (_inliner->_hunkArrayRegion) Hunk *[this->_array_size];
        for (int i = 0; i < this->_array_size; i++) {
           this->_subordinates_array[i] = NULL;
        }
  }

  auto subordinates = this->_subordinates_array;
  auto length = strnlen(subordinate->name(), 50);
  int i = 0;
  for (i = 0; i < this->_array_size; i++) {
    Hunk *aSubordinate = subordinates[i];
    if (aSubordinate == NULL) break;

    bool sameName = strncmp(aSubordinate->name(), subordinate->name(), length) == 0;
    bool samebcIndex = aSubordinate->bytecode_idx() == subordinate->bytecode_idx();
    bool same = sameName && samebcIndex;
    if (same) return false;
  }

  if (i == this->_array_size - 1) { return false; }
  subordinate->_caller_idx = this->callee_idx();
  auto max = this->find_max_callee_index();
  subordinate->_callee_idx = max + 1;
  subordinate->_prerequisite = this;
  this->_subordinates_array[i] = subordinate;
  this->_array_filled++;
  return true;
}

bool
Hunk::add_subordinate(Hunk* subordinate) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->add_subordinate_array_implementation(subordinate);

  if (!subordinate) return false;

  auto subordinates = this->subordinates();
  auto length = strnlen(subordinate->name(), 50);
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i)
  {
     auto aSubordinate = *i;
     bool sameName = strncmp(aSubordinate->name(), subordinate->name(), length) == 0;
     bool samebcIndex = aSubordinate->bytecode_idx() == subordinate->bytecode_idx();
     bool same = sameName && samebcIndex;
     if (same) {
       return false;
     }
  }
  subordinate->_caller_idx = this->callee_idx();
  auto max = this->find_max_callee_index();
  subordinate->_callee_idx = max + 1;
  subordinate->_prerequisite = this;
  subordinates.push_back(subordinate);
  return true;
}

int
Hunk::find_max_callee_index_array_implementation(Hunk* root) {
  if (!root) { root = this->root(); }
  auto retval = root->callee_idx();
  if (root->is_leaf_array_implementation()) return retval;
  auto subordinates = root->_subordinates_array;
  for (int i = 0; i < root->_array_size; i++) {
    Hunk *aSubordinate= subordinates[i];
    if (aSubordinate == NULL) break;
    auto temp_callee_idx = aSubordinate->find_max_callee_index(aSubordinate);
    retval = (temp_callee_idx > retval) ? temp_callee_idx : retval;
  }

  return retval;
}

int
Hunk::find_max_callee_index(Hunk* root) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
   return this->find_max_callee_index_array_implementation(root);

        if (!root) { root = this->root(); }
        auto retval = root->callee_idx();

    auto subordinates = root->subordinates();

    for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
        Hunk* hunk = *i;
        auto temp_callee_idx = hunk->find_max_callee_index(hunk);
        retval = (temp_callee_idx > retval) ? temp_callee_idx : retval;
    }

    return retval;
}


// TODO: rename total cost
int
Hunk::recursive_cost_array_implementation() {
  auto cost = this->cost();
  if (this->is_leaf_array_implementation()) return cost;
  auto subordinates = this->_subordinates_array;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk *aSubordinate= subordinates[i];
    if (aSubordinate == NULL) return cost;

    auto subcost = aSubordinate->recursive_cost();
    cost += subcost;
  }
  return cost;
}

// TODO: rename total cost
int
Hunk::recursive_cost() {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->recursive_cost_array_implementation();

  auto cost  = this->cost();
  auto subordinates = this->subordinates();
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    Hunk* hunk = *i;
    auto subcost = hunk->recursive_cost();
    cost += subcost;
  }
  return cost;
}

int
Hunk::cost() const {
  return this->_cost > 0 ? this->_cost : 1;
}

//TODO: rename to isRecursive
bool
Hunk::recursive(const char* name) {
  if (strcmp(name, this->name()) == 0) { return true; }
  if (this->is_root()) { return false; }

  return this->_prerequisite->recursive(name);
}



void
Hunk::normalizeMultiplier_array_implementation(float largest) {
  this->benefit_multiplier = this->benefit_multiplier / largest;
  if (this->is_leaf_array_implementation()) return;
  auto sub = this->_subordinates_array;
  for (int i = 0 ; i < this->_array_size; i++) {
    Hunk * hunk = sub[i];
    if(hunk == NULL) break;

    hunk->normalizeMultiplier(largest);
  }
}

void
Hunk::normalizeMultiplier(float largest) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) {
    this->normalizeMultiplier_array_implementation(largest);
    return;
  }
  this->benefit_multiplier = this->benefit_multiplier / largest;
  auto sub = this->subordinates();
  for(auto i = sub.cbegin(), e = sub.cend(); i != e; ++i) {
        auto hunk = *i;
        hunk->normalizeMultiplier(largest);
  }
}


float
Hunk::getLargestMultiplier_array_implementation() {
  float largest = this->benefit_multiplier;
  if (this->is_leaf_array_implementation()) return largest;
  auto sub = this->_subordinates_array;

  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = sub[i];
    if (hunk == NULL) return largest;

    float bm_subordinate = hunk->getLargestMultiplier();
    largest = bm_subordinate > largest ? bm_subordinate: largest;
  }

  return largest;
}

float
Hunk::getLargestMultiplier() {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->getLargestMultiplier_array_implementation();
  float largest = this->benefit_multiplier;
  auto sub = this->subordinates();
  for(auto i = sub.cbegin(), e = sub.cend(); i != e; ++i) {
        auto hunk = *i;
        float bm_subordinate = hunk->getLargestMultiplier();
        largest = bm_subordinate > largest ? bm_subordinate : largest;
  }

  return largest;
}

int
Hunk::getFrequencyEntryAccordingToMe() {
   for (TR::CFGNode *node = this->getResolvedMethodSymbol()->getFlowGraph()->getFirstNode(); node; node = node->getNext())
   {
      TR::Block *cfgBlock = node->asBlock();
      //TODO: Is this constant defined somewhere?
      if (cfgBlock->getNumber() == 4) {
        return cfgBlock->getFrequency();
      }
   }
   return -1;
}

void
Hunk::normalizeMultiplier() {
  float largestMultiplier = this->getLargestMultiplier();
  //TODO: Replace 10,000 with constant and if it isn't define 
  //my own constant
  this->normalizeMultiplier(largestMultiplier / 10000);
}

/*
Hunk*
Hunk::getNextSibling() const {
  auto root = this->_prerequisite;
  auto siblings = root->subordinates();
  auto returnOnNextOne = false;
  for (auto i = siblings.cbegin(), e = siblings.cend(); i != e; ++i) {
        auto candidate = *i;
        const auto returnThisOne = returnOnNextOne;
        if (returnThisOne) {
          return candidate;
        }
       returnOnNextOne = candidate == this;
  }
  // if we are here, that means that current Hunk is last of children
  return NULL;
}
*/

/*
Hunk*
Hunk::getNextParent() const {
  return this->_prerequisite;
}
*/

/*
Hunk*
Hunk::_next() const {
  Hunk* curr_parent = this->getNextParent();
  Hunk* curr_sibling = this->getNextSibling();
  while (!curr_sibling && !curr_parent->is_root()) {
    curr_sibling = curr_parent->getNextSibling();
    curr_parent = curr_parent->getNextParent();
  }
  return curr_sibling;
  
}
*/

/*
Hunk*
Hunk::next() const {
  auto hasChildren = this->count() > 1;
  // if it has children, return first children
  if (hasChildren) {
    return this->_subordinates[0];
  }
  // if it doesn't have children and it is not root, return first sibling
  auto isRoot = this->is_root();
  auto getNextSibling = !isRoot && !hasChildren;
  Hunk* potential = getNextSibling ? this->getNextSibling() : NULL;
  if (potential) {
     return potential;
  }
  // if we are here, that means that current Hunk is last of children
  return this->_next();
}
*/

size_t
Hunk::count_array_implementation() const {
  size_t count = 1;
  if (this->is_leaf_array_implementation()) return count;
  auto sub = this->_subordinates_array;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = sub[i];
    if (hunk == NULL) break;

    count += hunk->count();
  }
  return count;


}

size_t
Hunk::count() const {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->count_array_implementation();

  size_t count = 1;
  auto sub = *this->_subordinates;
  for(auto i = sub.cbegin(), e = sub.cend(); i != e; ++i) {
    auto subordinate = *i;
    count += subordinate->count();
  }
  return count;
}

bool
Hunk::is_root() const {
  return this->_prerequisite == NULL;
}

ostream&
operator<<( ostream &os, const Hunk &hunk ) {
  os << "   " << hunk._callee_idx;
  os << "   " << hunk._caller_idx;
  os << "   " << hunk._bytecode_idx;
  os << "   " << hunk._name << "\n";

  auto subordinates = *hunk._subordinates;
  for(auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    auto subordinate = *i;
    os << *subordinate;
  }
  return os;
}

bool
operator<(const Hunk &left, const Hunk &right) {
  return left._cost < right._cost || left._benefit < right._benefit;
}

bool
operator>( const Hunk &left, const Hunk &right ) {
  return left._cost > right._cost || left._benefit > right._benefit;
}

/*
bool
operator==( const Hunk &left, const Hunk &right ) {
  auto name = left._name == right._name;
  auto cost = left._cost == right._cost;
  auto bnft = left._benefit == right._benefit;
  auto bcid = left._bytecode_idx == right._bytecode_idx;
  auto self = bcid && name && cost && bnft;
  if (!self) return false;
  auto how_many_right = right.count();
  auto how_many_left = left.count();
  auto have_same_num_subordinates = how_many_right && how_many_left;
  if (!have_same_num_subordinates) return false;
  auto lsub = left._subordinates;
  auto rsub = right._subordinates;
  
  return lsub == rsub;
}
bool
operator!=( const Hunk &left, const Hunk &right) {
  return !(left == right);
}
*/

/*
Hunk*
Hunk::findCallsite(Hunk* root, const char* name, size_t bytecode_idx) {
  bool isRoot = root->name() == name && root->bytecode_idx() == bytecode_idx;
  if (isRoot) return root;
  auto subordinates = root->subordinates();
  for(auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    auto subordinate = *i;
    Hunk* possible = Hunk::findCallsite(subordinate, name, bytecode_idx);
    if (possible) return possible;
  }
  return NULL;
}
Hunk*
Hunk::findContextSensitive(Hunk* root, Hunk* stackToFind, const char* name, size_t bytecode_idx, TR::Compilation* comp) {
  if (!root) {
     TRACE(OMR::inlining, "root is NULL");
     return NULL;
  }
  bool isRoot = (strcmp(root->name(), name) == 0) && root->bytecode_idx() == bytecode_idx;
  if (isRoot) {
     TRACE(OMR::inlining, "root is what we are looking for");
     return root;
  }
  if (!stackToFind) {
    TRACE(OMR::inlining, "stackToFind is NULL");
    return NULL;
  }
*/
/*
  auto removeCallFrame = (stackToFind->name() == root->name() && stackToFind->bytecode_idx() == root->bytecode_idx());
  if (!removeCallFrame) {
     TRACE(OMR::inlining, "removeCallFrame is NULL");
     return NULL;
  }
*/
/*
  auto newStackToFind = stackToFind->subordinates().empty() ? NULL : stackToFind->subordinates()[stackToFind->subordinates().size()-1];
  if (!newStackToFind) {
    TRACE(OMR::inlining, "newStackToFind is NULL");
    return NULL;
  }
  auto subordinates = root->subordinates();
  for( auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    auto subordinate = *i;
    if (!subordinate) {
        TRACE(OMR::inlining, "subordinate is NULL");
        return NULL;
    }
    
    TRACE(OMR::inlining, "subordinate's name is %s, subordinate's bytecode_idx is %d", subordinate->name(), subordinate->bytecode_idx());
    TRACE(OMR::inlining, "newStackToFind's name is %s, newStackToFind's bytecode_idx is %d", newStackToFind->name(), newStackToFind->bytecode_idx());
    TRACE(OMR::inlining, "name is %s, bytecode_idx is %d", name, bytecode_idx);
    bool isWhatWeAreLookingFor = strcmp(subordinate->name(), name) == 0 && subordinate->bytecode_idx() == bytecode_idx;
    if (isWhatWeAreLookingFor) {
      TRACE(OMR::inlining, "returns where we just changed");
      return subordinate;
    }
    auto isChild = (strcmp(subordinate->name(), newStackToFind->name()) == 0) && subordinate->bytecode_idx() == newStackToFind->bytecode_idx();
    if (!isChild) continue;
    auto possible = Hunk::findContextSensitive(subordinate, newStackToFind, name, bytecode_idx, comp);
    return possible ? possible : NULL;
  }
  return NULL;
}
*/

Hunk*
Hunk::find_array_implementation(Hunk* root, int caller_idx) {

  auto callee_idx = root->callee_idx();
  auto is_root = caller_idx == callee_idx;
  if (is_root) return root;
  if (root->is_leaf_array_implementation()) return NULL;

  auto sub = root->_subordinates_array;
  for (int i = 0; i < root->_array_size; i++) {
    Hunk* hunk = sub[i];
    if (hunk == NULL) return NULL;
    Hunk* possible = Hunk::find(hunk, caller_idx);
    if (possible) return possible;
  }

  return NULL;
}

Hunk*
Hunk::find(Hunk* root, int caller_idx) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return Hunk::find_array_implementation(root, caller_idx);

  auto callee_idx = root->callee_idx();
  auto is_root = caller_idx == callee_idx;
  if (is_root) return root;

  auto subordinates = root->subordinates();
  for(auto i = subordinates.begin(), e = subordinates.end(); i != e; ++i) {
    auto subordinate = *i;
    Hunk* possible = Hunk::find(subordinate, caller_idx);
    if (possible) return possible;
  }

  return NULL;
}


/*
Hunk*
Hunk::convertInlinedCallSiteToIDT( TR::ResolvedMethodSymbol *methodSymbol, TR::Compilation* comp, TR::Region& region) {
  if (!methodSymbol)
    methodSymbol = comp->getMethodSymbol();
  auto rootSignature = methodSymbol->signature(comp->trMemory());
  const int rootBCIndex = 0;
  Hunk* rootHunk = new(region) Hunk(rootSignature, rootBCIndex, comp);
  if (comp->getMethodSymbol() == methodSymbol && comp->getNumInlinedCallSites() > 0)
    {
    auto inlinedCallSites = comp->getNumInlinedCallSites();
    Hunk* hunks[inlinedCallSites];
    for (int32_t i = 0; i < inlinedCallSites; ++i)
      {
         TR_InlinedCallSite & ics = comp->getInlinedCallSite(i);
         TR_ResolvedMethod  *method = comp->getInlinedResolvedMethod(i);
         auto signature = method->signature(comp->trMemory());
         auto callerIndex = ics._byteCodeInfo.getCallerIndex();
         auto byteCodeIndex = ics._byteCodeInfo.getByteCodeIndex();
         hunks[i] = new(region) Hunk(signature, byteCodeIndex, comp);
         if (callerIndex < 0) {
            rootHunk->add_subordinate(hunks[i]);
         } else {
            hunks[callerIndex]->add_subordinate(hunks[i]);
         }
      } 
    }
   return rootHunk;
}
*/

List<Hunk>*
Hunk::dfs_array_implementation(List<Hunk>* children) {
  if (!children && this->_dfs.isEmpty()) { children = & this->_dfs; }
  else if (!children && !this->_dfs.isEmpty()) { return &this->_dfs; }

  if (this->is_leaf_array_implementation()) return children;
  auto subordinates = this->_subordinates_array;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* child = subordinates[i];
    if (child == NULL) break;
    children->add(child);
    child->dfs(children);
  }
  return children;
}

List<Hunk>*
Hunk::dfs(List<Hunk>* children) {
#ifdef DFS_LIST
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->dfs_array_implementation(children);

  if (!children && this->_dfs.isEmpty()) { children = &this->_dfs; }
  else if (!children && !this->_dfs.isEmpty()) { return &this->_dfs; }

  auto subordinates = this->subordinates();
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
    auto child = *i;
    children->add(child);
    child->dfs(children);
  }

  return children;
#else
  return NULL;
#endif
}

Hunk*
Hunk::root() {
  Hunk* root = this;
  while (!root->is_root()) {
   root = root->prerequisite();
 }

 return root;
}

Hunk*
Hunk::getCalleeIndex_array_implementation(int callee_idx, Hunk* aRoot) {
  Hunk* root = aRoot == NULL ? this->root() : aRoot;
  if (root->callee_idx() == callee_idx) {
    return root;
  }


  auto sub = root->_subordinates_array;
  if (!sub) return NULL;
  //int prev = root->callee_idx();
  //Hunk* prevHunk = NULL;
  for (int i = 0; i < root->_array_size; i++) {
    Hunk* hunk = sub[i];
    if (hunk == NULL) break;

    //int current = hunk->callee_idx();
    //if (current == callee_idx) return hunk;
    //TR_ASSERT(prev < current, "IT IS NOT IN ORDER\n");
    //prev = current;
    //prevHunk = hunk;
    //if (prevHunk && callee_idx < current) { return prevHunk->getCalleeIndex(callee_idx, prevHunk); }
    auto retval = hunk->getCalleeIndex(callee_idx, hunk);
    if (retval) return retval;
  }

  return NULL;
}

Hunk*
Hunk::getCalleeIndex_array_implementation_binary_search(int callee_idx, Hunk* aRoot) {
  Hunk* root = aRoot == NULL ? this->root() : aRoot;
  if (root->callee_idx() == callee_idx) {
    return root;
  }

  if (root->is_leaf_array_implementation()) {
    return NULL;
  }

  auto sub = this->_subordinates_array;
  Hunk* prevHunk = sub[0]; // guaranteed to be non null by is_leaf_array_implementation
  for (int i = 1; i < root->_array_size; i++) {
    Hunk* hunk = sub[i];
    if (hunk == NULL) break;

    int current = hunk->callee_idx();
    if (current == callee_idx) { return hunk; }

    if (current > callee_idx) { break; }
    prevHunk = hunk;
  }

  // for the last one
  return prevHunk->getCalleeIndex_array_implementation_binary_search(callee_idx, prevHunk);
}

Hunk*
Hunk::getCalleeIndex(int callee_idx, Hunk* aRoot) {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");

  if (arrayImplementation) {
    //auto aHunk = Hunk::getCalleeIndex_array_implementation(callee_idx, aRoot);
    auto bHunk = Hunk::getCalleeIndex_array_implementation_binary_search(callee_idx, aRoot);
    //TR_ASSERT(aHunk == bHunk, "implementations differ");
    return bHunk;
  }

  Hunk* root = aRoot == NULL ? this->root() : aRoot;
  if (root->callee_idx() == callee_idx) {
    return root;
  }
  
  auto subordinates = root->subordinates();
  for(auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
     auto hunk = *i;
     auto retval = hunk->getCalleeIndex(callee_idx, hunk);
     if (retval) {
       return retval;
     }
  }
  return NULL;
}

void
Hunk::convertAndPrint(TR::ResolvedMethodSymbol *method, TR::Compilation *comp, const char* filename, TR::Region& region) {
  //Hunk *hunks = Hunk::convertInlinedCallSiteToIDT(method, comp, region);
  //Hunk::print(hunks, comp, filename);
}

void
Hunk::print(Hunk* hunks, TR::Compilation *comp, const char* filename) {
  char* toPrint = hunks->table(comp);
  int i = 0;
  //TR::FILE* file = trfopen((char*)filename, "a+", false);
  //while( i < strlen(toPrint) ) {
     //i += trfprintf(file, "%s\n", toPrint);
  //   trfflush(file);
  //}
  //trfclose(file);

}

/*
Hunk&
Hunk::operator=(const Hunk &rhs) 
{
  auto rhssub = rhs._subordinates;
  this->_subordinates.erase(this->_subordinates.begin(), this->_subordinates.end());
  for(auto i = rhssub.cbegin(), e = rhssub.cend(); i != e; ++i) {
    this->_subordinates.push_back(*i);
  }
  this->_prerequisite = (Hunk*)rhs.prerequisite();
  this->_bytecode_idx = rhs.bytecode_idx();
  return *this;
}
*/

void
Hunk::setResolvedMethodSymbol(TR::ResolvedMethodSymbol* rms) {
  this->_rms = rms;
}

TR::ResolvedMethodSymbol*
Hunk::getResolvedMethodSymbol() const {
  return this->_rms;
}

List<Hunk>
Hunk::findChildrenWithBytecodeIndex_array_implementation(size_t bcIndex) const {
  List<Hunk> children(_inliner->_hunkArrayRegion);
  auto subordinates = this->_subordinates_array;
  if (!subordinates) return children;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = subordinates[i];
    if (hunk == NULL) break;
    auto hasSameBCIndex = hunk->bytecode_idx() == bcIndex;
    if (!hasSameBCIndex) { continue; }
    children.add(hunk);
  }

  return children;
}

List<Hunk>
Hunk::findChildrenWithBytecodeIndex(size_t bcIndex) const {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->findChildrenWithBytecodeIndex_array_implementation(bcIndex);

  List<Hunk> children(_inliner->_hunkArrayRegion);
  auto subordinates = *this->_subordinates;
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
     auto hunk = *i;
     auto hasSameBCIndex = hunk->bytecode_idx() == bcIndex;
     if (!hasSameBCIndex) { continue; }

      children.add(hunk);
  }
  return children;
}

Hunk*
Hunk::findChildWithBytecodeIndex_array_implementation(size_t bcIndex) const {
  auto subordinates = this->_subordinates_array;
  if (!subordinates) return NULL;
  for (int i = 0; i < this->_array_size; i++) {
    Hunk* hunk = subordinates[i];
    if (hunk == NULL) break;
    auto hasSameBCIndex = hunk->bytecode_idx() == bcIndex;
    if (hasSameBCIndex) { return hunk; }
  }
  return NULL;
}

Hunk*
Hunk::findChildWithBytecodeIndex(size_t bcIndex) const {
  static const char* arrayImplementation = feGetEnv("TR_HunkArray");
  if (arrayImplementation) 
    return this->findChildWithBytecodeIndex_array_implementation(bcIndex);
  auto subordinates = *this->_subordinates;
  for (auto i = subordinates.cbegin(), e = subordinates.cend(); i != e; ++i) {
     auto hunk = *i;
     auto hasSameBCIndex = hunk->bytecode_idx() == bcIndex;
     if (hasSameBCIndex) { return hunk; }
  }

  return NULL;
}

int
Hunk::numberOfParameters() const {
  TR::ResolvedMethodSymbol *hunkResolvedMethodSymbol = this->getResolvedMethodSymbol();
  TR_ResolvedMethod *hunkResolvedMethod = hunkResolvedMethodSymbol->getResolvedMethod();
  return hunkResolvedMethod->numberOfParameters();
}

#undef DFS_LIST

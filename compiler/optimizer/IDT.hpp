#pragma once

#include "infra/Cfg.hpp"
#include "infra/deque.hpp"
#include "env/TypedAllocator.hpp"
#include "env/Region.hpp"
#include "Inliner.hpp"

class IDTNode;

class IDT {
  friend class IDTNode;
private:
  TR::Region *_mem;
  TR_InlinerBase *_inliner;
  int _max_idx = -1;
  IDTNode *_root;
  int
  next_idx();
  // Returns NULL if 0 or > 1 children
  static IDTNode *
  getOnlyChild(IDTNode *owner);
  static void
  setOnlyChild(IDTNode *owner, IDTNode *child);
public:
  IDT(TR_InlinerBase *inliner,
      TR::Region *mem,
      TR::ResolvedMethodSymbol* rms);
  IDTNode *getRoot();
  IDTNode *
  addChildIfNotExists(
    IDTNode *prerequisite,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms
  );
  TR::Compilation *
  comp();
private:
  int
  size(IDTNode *node);
public:
  int
  size();
  void 
  printTrace();
  const char *
  getName(IDTNode *node);
};

class IDTNode {
  friend class IDT;
public:
  IDTNode(
    IDT *idt,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms);
  IDTNode();
  size_t
  numChildren();
private:
  typedef TR::deque<IDTNode, TR::Region&> Children;
  int _idx;
  int _callsite_bci;
  // NULL if 0, (IDTNode* & 1) if 1, otherwise a deque*
  Children *_children;  
  TR::ResolvedMethodSymbol* _rms;
  bool
  nodeSimilar(int32_t callsite_bci,TR::ResolvedMethodSymbol* rms) const;
  void 
  printNodeThenChildren(IDT *idt, int callerIndex);
  uint32_t
  getBcSz();
};

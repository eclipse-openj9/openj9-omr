#pragma once

#include "infra/Cfg.hpp"
#include "infra/Array.hpp"
#include "Inliner.hpp"

class IDTNode;

class IDT {
  friend class IDTNode;
private:
  TR_InlinerBase *_inliner;
  TR::Region *_mem;
  TR::CFG *_cfg2;
  IDTNode *_root;
  int _max_idx = -1;
  int next_idx();
public:
  IDT(TR_InlinerBase *inliner,
      TR::ResolvedMethodSymbol* rms);
  IDTNode *getRoot();
  IDTNode *
  addSubordinateIfNotExists(
    IDTNode *prerequisite,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms
  );
  TR::Compilation *
  comp();
  void printTrace();
};

class IDTNode {
  friend class IDT;
public:
  TR_ALLOC(TR_Memory::IDTNode);
  IDTNode(
    IDT *idt,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms);
  IDTNode();
private:
  int _idx;
  int _callsite_bci;
  TR_Array<IDTNode*> *_subordinates;  
  TR::ResolvedMethodSymbol* _rms;
  bool
  nodeSimilar(int32_t callsite_bci,TR::ResolvedMethodSymbol* rms) const;
  void printNodeThenChildren(IDT *idt, int indentLevel = 0);
};

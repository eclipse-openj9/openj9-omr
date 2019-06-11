#include "IDT.hpp"

#define INITIAL_NUM_SUBORDINATES (10)

IDTNode::IDTNode() {}

IDTNode::IDTNode(
    IDT *idt,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms):
    _idx(idt->next_idx()),
    _callsite_bci(callsite_bci),
    // TODO WHy is this called stackMemory?
    _subordinates(new (idt->comp()->trStackMemory()) TR_Array<IDTNode *>(
      idt->comp()->trMemory(),
      INITIAL_NUM_SUBORDINATES
    )),
    _rms(rms)
{
  
}

IDT::IDT(TR_InlinerBase *inliner, TR::ResolvedMethodSymbol* rms): 
    _inliner(inliner),
    _root(new (comp()->trStackMemory()) IDTNode(this, -1, rms))
{
  
}

void IDT::printTrace() {
  // TODO doesn't seem to be enabled
  if (comp()->trace(OMR::selectInliner) || true) {
    traceMsg(comp(), "----- BEGIN IDT FOR %s -----\n", getRoot()->_rms->getName());
    getRoot()->printNodeThenChildren(this);
    traceMsg(comp(), "----- END IDT -----\n");
  }
}

void IDTNode::printNodeThenChildren(IDT *idt, int indentLevel) {
  TR_ArrayIterator<IDTNode> iter(_subordinates);

  for (int i = 0; i < indentLevel; i++) {
    traceMsg(idt->comp(), "%");
  }

  traceMsg(idt->comp(), " %s\n", _rms->getName());

  while(!iter.pastEnd()) {
    IDTNode *next = iter.getNext();
    next->printNodeThenChildren(idt, indentLevel + 1);
  }

}

IDTNode *IDT::getRoot() {
  return _root;
}

int IDT::next_idx() {
  return _max_idx++;
}

IDTNode *
IDT::addSubordinateIfNotExists(
    IDTNode *prerequisite,
    int32_t callsite_bci,
    TR::ResolvedMethodSymbol* rms
  )
{
  TR_ArrayIterator<IDTNode> iter(prerequisite->_subordinates);

  while (!iter.pastEnd()) {
    IDTNode *next = iter.getNext();
    if (next->nodeSimilar(callsite_bci, rms)) {
      return NULL;
    }
  }
  IDTNode *created = new (comp()->trStackMemory()) IDTNode(this, callsite_bci, rms);
  prerequisite->_subordinates->add(created);
  return created;
}

bool
IDTNode::nodeSimilar(int32_t callsite_bci, TR::ResolvedMethodSymbol* rms) const {
  auto a = _rms->getResolvedMethod()->maxBytecodeIndex();
  auto b = rms->getResolvedMethod()->maxBytecodeIndex();
  
  return a == b && _callsite_bci == callsite_bci;
}

TR::Compilation *
IDT::comp() {
  return _inliner->comp();
}
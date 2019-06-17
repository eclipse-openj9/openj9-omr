#include "IDT.hpp"

#define INITIAL_NUM_children (10)

IDTNode::IDTNode() {}

IDTNode::IDTNode(IDT* idt, int32_t callsite_bci, TR::ResolvedMethodSymbol* rms)
  : _idx(idt->next_idx())
  , _callsite_bci(callsite_bci)
  , _children(nullptr)
  , _rms(rms)
{}


IDT::IDT(TR_InlinerBase* inliner, TR::Region *mem, TR::ResolvedMethodSymbol* rms)
  : _mem(mem)
  , _inliner(inliner)
  , _max_idx(-1)
  , _root(new (*_mem) IDTNode(this, -1, rms))
{}

void
IDT::printTrace()
{
  if (comp()->trace(OMR::selectInliner)) {
    TR_VerboseLog::writeLineLocked(TR_Vlog_SIP, "#IDT: %d candidate methods to inline into %s",
      size() - 1,
      getName(getRoot())
    );
    if (size() == 0) {
      return;
    }
    getRoot()->printNodeThenChildren(this, -2);
  }
}

void
IDTNode::printNodeThenChildren(IDT* idt, int callerIndex)
  {
  if (this != idt->getRoot()) {
    const char *nodeName = idt->getName(this);
    TR_VerboseLog::writeLineLocked(TR_Vlog_SIP, "#IDT: %d: %d inlinable @%d -> bcsz=%d %s", 
      _idx,
      callerIndex,
      _callsite_bci,
      getBcSz(),
      nodeName
    );
  }
  
  if (_children == NULL) {
    return;
  }

  // Print children
  IDTNode* child = IDT::getOnlyChild(this);

  if (child != nullptr)
    {
    child->printNodeThenChildren(idt, _idx);
    } 
  else
    {
    for (auto curr = _children->begin(); curr != _children->end(); ++curr)
      {
      curr->printNodeThenChildren(idt, _idx);
      }
    }
  }

uint32_t
IDTNode::getBcSz() {
  return _rms->getResolvedMethod()->maxBytecodeIndex();
}

IDTNode*
IDT::getOnlyChild(IDTNode* owner)
  {
  if (((size_t)owner->_children) & 1)
    {
    return (IDTNode *)(void*)(((size_t)(owner->_children)>>1)<<1);
    }
  return nullptr;
  }

void
IDT::setOnlyChild(IDTNode* owner, IDTNode* child)
  {
  owner->_children = (IDTNode::Children*)(void*)((size_t)child | 1);
  }

IDTNode*
IDT::getRoot()
  {
  return _root;
  }

int
IDT::next_idx()
  {
  return _max_idx++;
  }

IDTNode*
IDT::addChildIfNotExists(IDTNode* prerequisite,
                         int32_t callsite_bci,
                         TR::ResolvedMethodSymbol* rms)
  {
  // 0 Children
  if (prerequisite->_children == nullptr)
    {
    IDTNode* created = new (*_mem) IDTNode(this, callsite_bci, rms);
    TR_ASSERT_FATAL(!((size_t)created & 1), "Maligned memory address.\n");
    setOnlyChild(prerequisite, created);
    return created;
    }
  // 1 Child
  IDTNode* onlyChild = getOnlyChild(prerequisite);
  if (onlyChild != nullptr)
    {
    prerequisite->_children = new (*_mem) IDTNode::Children(*_mem);
    TR_ASSERT_FATAL(!((size_t)prerequisite->_children & 1), "Maligned memory address.\n");
    prerequisite->_children->push_back(*onlyChild);
    // Fall through to two child case.
    }

  // 2+ children
  IDTNode::Children* children = prerequisite->_children;

  for (auto curr = children->begin(); curr != children->end(); ++curr)
    {
    if (curr->nodeSimilar(callsite_bci, rms))
      {
      return nullptr;
      }
    }

  prerequisite->_children->push_back(IDTNode(this, callsite_bci, rms));
  return &prerequisite->_children->back();
}

bool
IDTNode::nodeSimilar(int32_t callsite_bci, TR::ResolvedMethodSymbol* rms) const
  {
  auto a = _rms->getResolvedMethod()->maxBytecodeIndex();
  auto b = rms->getResolvedMethod()->maxBytecodeIndex();

  return a == b && _callsite_bci == callsite_bci;
  }

TR::Compilation*
IDT::comp()
  {
  return _inliner->comp();
  }


int
IDT::size()
  {
    return size(getRoot());
  }

int
IDT::size(IDTNode *node)
  {
    if (node->_children == NULL) 
      {
      return 1;
      }
    IDTNode *onlyChild = getOnlyChild(node);
    if (onlyChild != NULL)
      {
      return 1 + size(onlyChild);
      }
    int count = 1;
    for (auto curr = node->_children->begin(); curr != node->_children->end(); ++curr)
      {
      count += size(&*curr);
      }
    return count;
  }

size_t
IDTNode::numChildren()
  {
    if (_children == NULL) {
      return 0;
    }
    if (IDT::getOnlyChild(this) != nullptr) {
      return 1;
    }
    return _children->size();
  }

const char *
IDT::getName(IDTNode *node) {
  return node->_rms->getResolvedMethod()->signature(comp()->trMemory());
}

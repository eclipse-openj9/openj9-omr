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
 ******************************************************************************/

#include "infra/ILWalk.hpp"

#include "compile/Compilation.hpp"
#include "il/Block.hpp"
#include "il/Node_inlines.hpp"
#include "il/TreeTop.hpp"
#include "infra/Cfg.hpp"
#include "optimizer/Optimization.hpp"
#include "optimizer/Optimization_inlines.hpp"

#include <stdarg.h>

//
// Hack markers
//

// If it weren't for the fact that an already-visited block could be removed
// and re-inserted into the block list, our algorithms would be more elegant.
//
#define REMOVED_BLOCKS_CAN_BE_REINSERTED (1)


//
// TreeTopIterator and TreeTopIteratorImpl
//

TR::TreeTopIteratorImpl::TreeTopIteratorImpl(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :_current(start),_opt(opt),_name(name)
   {
   if (opt && !name)
      _name = opt->name();
   }

TR::Node *TR::TreeTopIteratorImpl::currentNode()
   {
   return currentTree()->getNode();
   }

bool TR::TreeTopIteratorImpl::isAt(PreorderNodeIterator &other)
   {
   return other.isAt(currentTree());
   }

void TR::TreeTopIteratorImpl::logCurrentLocation()
   {
   // TODO: Log even without an _opt.

   if (_name && _opt && _opt->trace() && _opt->comp()->getOption(TR_TraceILWalks))
      {
      if (currentTree())
         {
         TR::Node *node = currentTree()->getNode();
         traceMsg(_opt->comp(), "TREE  %s @ %s n%dn [%p]\n", _name, node->getOpCode().getName(), node->getGlobalIndex(), node);
         }
      else
         {
         traceMsg(_opt->comp(), "TREE  %s finished\n", _name);
         }
      }
   }

void TR::TreeTopIteratorImpl::stepForward()
   {
   TR_ASSERT(_current, "Cannot stepForward a TreeTopIterator that has already moved beyond the first or last tree of the method");
   _current = _current->getNextTreeTop();
   }

void TR::TreeTopIteratorImpl::stepBackward()
   {
   TR_ASSERT(_current, "Cannot stepBackward a TreeTopIterator that has already moved beyond the first or last tree of the method");
   _current = _current->getPrevTreeTop();
   }

//
// Node iterators
//

TR::NodeIterator::NodeIterator(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :TreeTopIteratorImpl(start, opt, name)
   ,_checklist(opt->comp())
   ,_stack(opt->comp()->trMemory(), 5, false, stackAlloc)
   {}

TR::NodeIterator::NodeIterator(TR::TreeTop *start, TR::Compilation *comp)
   :TreeTopIteratorImpl(start, NULL, NULL)
   ,_checklist(comp)
   ,_stack(comp->trMemory(), 5, false, stackAlloc)
   {}

bool TR::NodeIterator::isAt(PreorderNodeIterator &other)
   {
   // One PreorderNodeIterator "is at" another if both follow the same node
   // child path from the same treetop.  Note that the checklist is explicitly
   // NOT compared.  This means that a.isAt(b) is no guarantee that a and b
   // will subsequently return the same sequence of nodes.  This is
   // intentional--isAt is not an equality test--and it could occur if the
   // iterations started at differnet points, or if there were differences in
   // timing between advancement of the iterator versus mutations in the IL.
   // This function explicitly disregards such differences.
   //
   if (currentTree() != other.currentTree())
      return false;
   if (stackDepth() != other.stackDepth())
      return false;
   for (int32_t i = 0; i < stackDepth(); i++)
      if (_stack[i] != other._stack[i])
         return false;
   return true;
   }

void TR::NodeIterator::logCurrentLocation()
   {
   // TODO: Log even without an _opt.

   if (_name && _opt && _opt->trace() && _opt->comp()->getOption(TR_TraceILWalks))
      {
      if (currentTree())
         {
         TR::Node *node = currentNode();
         traceMsg(_opt->comp(), "NODE  %s  ", _name);
         if (stackDepth() >= 2)
            {
            traceMsg(_opt->comp(), " ");
            for (int32_t i = 0; i < stackDepth()-2; i++)
               {
               if (_stack[i]._isBetweenChildren)
                  traceMsg(_opt->comp(), " |");
               else
                  traceMsg(_opt->comp(), "  ");
               }
            traceMsg(_opt->comp(), " %d: ", _stack[_stack.topIndex()-1]._child);
            }
         traceMsg(_opt->comp(), "%s n%dn [%p]\n", node->getOpCode().getName(), node->getGlobalIndex(), node);
         }
      else
         {
         // Usualy this one doesn't print, because when the iterator finishes
         // naturally, logCurrentLocation is not even called.
         //
         traceMsg(_opt->comp(), "NODE  %s finished\n", _name );
         }
      }
   }

TR::PreorderNodeIterator::PreorderNodeIterator(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :NodeIterator(start, opt, name)
   {
   push(start->getNode());
   }

TR::PreorderNodeIterator::PreorderNodeIterator(TR::TreeTop *start, TR::Compilation *comp)
   :NodeIterator(start, comp)
   {
   push(start->getNode());
   }

bool TR::PreorderNodeIterator::alreadyBeenPushed(TR::Node *node)
   {
   return _checklist.contains(node);
   }

void TR::PreorderNodeIterator::push(TR::Node *node)
   {
   TR_ASSERT(!alreadyBeenPushed(node), "Cannot push node n%dn that was already pushed", node->getGlobalIndex());
   _stack.push(WalkState(node));
   _stack.top()._isBetweenChildren = (node->getNumChildren() >= 2);
   _checklist.add(node);
   logCurrentLocation();
   }

void TR::PreorderNodeIterator::stepForward()
   {
   // Move down into the next un-visited child
   //
   for (int32_t i = _stack.top()._child; i < _stack.top()._node->getNumChildren(); i++)
      {
      TR::Node *child = _stack.top()._node->getChild(i);
      if (!alreadyBeenPushed(child))
         {
         _stack.top()._child = i;
         if (i == _stack.top()._node->getNumChildren()-1)
            _stack.top()._isBetweenChildren = false;
         push(child);
         return;
         }
      }

   // No more un-visited children; we're done with this node
   //
   _stack.pop();
   if (_stack.isEmpty())
      {
      // Step to the next tree, if any
      //
      while (_stack.isEmpty())
         {
         TreeTopIteratorImpl::stepForward();
         if (!currentTree())
            return;
         else if (!alreadyBeenPushed(currentTree()->getNode()))
            push(currentTree()->getNode());
         }
      }
   else
      {
      _stack.top()._child++;
      stepForward();
      }
   }

TR::PostorderNodeIterator::PostorderNodeIterator(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :NodeIterator(start, opt, name)
   {
   push(start->getNode());
   descend();
   }

TR::PostorderNodeIterator::PostorderNodeIterator(TR::TreeTop *start, TR::Compilation *comp)
   :NodeIterator(start, comp)
   {
   push(start->getNode());
   descend();
   }

bool TR::PostorderNodeIterator::alreadyBeenPushed(TR::Node *node)
   {
   return _checklist.contains(node);
   }

void TR::PostorderNodeIterator::push(TR::Node *node)
   {
   TR_ASSERT(!alreadyBeenPushed(node), "Cannot push node n%dn that was already pushed", node->getGlobalIndex());
   _stack.push(WalkState(node));
   _checklist.add(node);
   }

void TR::PostorderNodeIterator::descend()
   {
   // Push frames until we find the innermost, leftmost unvisited descendant.
   TR::Node *node = _stack.top()._node;
   for (;;)
      {
      int32_t i = _stack.top()._child;
      while (i < node->getNumChildren() && alreadyBeenPushed(node->getChild(i)))
         i++;

      _stack.top()._child = i;
      if (i == node->getNumChildren())
         break;

      node = node->getChild(i);
      push(node);
      }

   logCurrentLocation();
   }

void TR::PostorderNodeIterator::stepForward()
   {
   // We're done with the current node and all its descendants. Move back up
   // to the parent and attempt to descend into later subtrees.
   _stack.pop();
   if (!_stack.isEmpty())
      {
      _stack.top()._child++;
      _stack.top()._isBetweenChildren = true;
      descend();
      return;
      }

   // Step to the next tree, if any
   do
      TreeTopIteratorImpl::stepForward();
   while (currentTree() && alreadyBeenPushed(currentTree()->getNode()));

   if (!currentTree())
     return;

   push(currentTree()->getNode());
   descend();
   }

//
// Node occurrence iterators
//

TR::Node *TR::NodeOccurrenceIterator::currentNode()
   {
   if (stackDepth() == 0)
      return currentTree()->getNode();
   else
      return _stack.top()._node->getChild(_stack.top()._child);
   }

void TR::NodeOccurrenceIterator::logCurrentLocation()
   {
   // TODO: Log even without an _opt.

   if (_name && _opt && _opt->trace() && _opt->comp()->getOption(TR_TraceILWalks))
      {
      if (currentTree())
         {
         TR::Node *node = currentNode();
         traceMsg(_opt->comp(), "WALK  %s  ", _name);
         if (stackDepth() >= 1)
            {
            traceMsg(_opt->comp(), " ");
            for (int32_t i = 0; i < stackDepth()-1; i++)
               {
               if (_stack[i]._isBetweenChildren)
                  traceMsg(_opt->comp(), " |");
               else
                  traceMsg(_opt->comp(), "  ");
               }
            traceMsg(_opt->comp(), " %d: ", _stack[_stack.topIndex()]._child);
            }
         traceMsg(_opt->comp(), "%s n%dn [%p]\n", node->getOpCode().getName(), node->getGlobalIndex(), node);
         }
      else
         {
         // Usually this one doesn't print, because when the iterator finishes
         // naturally, logCurrentLocation is not even called.
         //
         traceMsg(_opt->comp(), "WALK  %s finished\n", _name );
         }
      }
   }

TR::PreorderNodeOccurrenceIterator::PreorderNodeOccurrenceIterator(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :NodeOccurrenceIterator(start, opt, name)
   {
   logCurrentLocation();
   }

bool TR::PreorderNodeOccurrenceIterator::alreadyPushedChildren(TR::Node *node)
   {
   return _checklist.contains(node);
   }

void TR::PreorderNodeOccurrenceIterator::push(TR::Node *node)
   {
   _stack.push(WalkState(node));
   _stack.top()._isBetweenChildren = (node->getNumChildren() >= 2);
   _checklist.add(node);
   logCurrentLocation();
   }

void TR::PreorderNodeOccurrenceIterator::stepForward()
   {
   if (currentNode()->getNumChildren() >= 1 && !alreadyPushedChildren(currentNode()))
      {
      // Pushing currentNode has the effect of moving down into its children
      //
      push(currentNode());
      }
   else if (_stack.isEmpty())
      {
      // Nullary treetop node; move on to the next treetop
      //
      TreeTopIteratorImpl::stepForward();
      logCurrentLocation();
      }
   else if (++_stack.top()._child < _stack.top()._node->getNumChildren())
      {
      // Our attempt to move to the next sibling was successful
      //
      if (_stack.top()._child == _stack.top()._node->getNumChildren()-1)
         _stack.top()._isBetweenChildren = false;
      logCurrentLocation();
      }
   else
      {
      // No more siblings; try moving to parent's siblings
      //
      _stack.pop();
      stepForward();
      }
   }

TR::PostorderNodeOccurrenceIterator::PostorderNodeOccurrenceIterator(TR::TreeTop *start, TR::Optimization *opt, const char *name)
   :NodeOccurrenceIterator(start, opt, name)
   {
   pushLeftmost(currentTree()->getNode());
   }

bool TR::PostorderNodeOccurrenceIterator::alreadyPushedChildren(TR::Node *node)
   {
   return _checklist.contains(node);
   }

void TR::PostorderNodeOccurrenceIterator::pushLeftmost(TR::Node *nodeArg)
   {
   for (TR::Node *node = nodeArg; node->getNumChildren() >= 1 && !alreadyPushedChildren(node); node = node->getFirstChild())
      {
      _stack.push(WalkState(node));
      _checklist.add(node);
      }
   logCurrentLocation();
   }

void TR::PostorderNodeOccurrenceIterator::stepForward()
   {
   if (_stack.isEmpty())
      {
      // Nullary treetop node; move on to the next treetop
      //
      TreeTopIteratorImpl::stepForward();
      if (currentTree())
         pushLeftmost(currentTree()->getNode());
      }
   else if (++_stack.top()._child < _stack.top()._node->getNumChildren())
      {
      // Our attempt to move to the next sibling was successful
      //
      _stack.top()._isBetweenChildren = true;
      pushLeftmost(currentNode());
      }
   else
      {
      // No more siblings; move to parent
      //
      _stack.pop();
      logCurrentLocation();
      }
   }

//
// Block iterators
//

TR::BlockIterator::BlockIterator(TR::Optimization *opt, const char *name)
   :_opt(opt),_name(name)
   {
   if (opt && !name)
      _name = opt->name();
   }

bool TR::BlockIterator::isLoggingEnabled()
   {
   return (_name && _opt && _opt->trace() && _opt->comp()->getOption(TR_TraceILWalks));
   }

TR::ReversePostorderSnapshotBlockIterator::ReversePostorderSnapshotBlockIterator(TR::Block *start, TR::Optimization *opt, const char *name)
   :BlockIterator(opt, name),
   _postorder(opt->comp()->trMemory(), 5, false, stackAlloc)
   {
   takeSnapshot(start);
   logCurrentLocation();
   }

TR::ReversePostorderSnapshotBlockIterator::ReversePostorderSnapshotBlockIterator(TR::CFG *cfg, TR::Optimization *opt, const char *name)
   :BlockIterator(opt, name),
   _postorder(opt->comp()->trMemory(), cfg->getNumberOfNodes(), false, stackAlloc)
   {
   takeSnapshot(cfg->getStart()->asBlock());
   if (isLoggingEnabled())
      {
      traceMsg(_opt->comp(), "BLOCK  %s Snapshot:", _name);
      for (int32_t i = _postorder.lastIndex(); i >= 0; --i)
         traceMsg(_opt->comp(), " %d", _postorder[i]->getNumber());
      traceMsg(_opt->comp(), "\n");
      }
   logCurrentLocation();
   }

void TR::ReversePostorderSnapshotBlockIterator::takeSnapshot(TR::Block *start)
   {
   // TODO: This should use an iterative algorithm with a worklist, but the recursive algo is so much easier to write...
   TR::BlockChecklist alreadyVisited(_opt->comp());
   visit(start, alreadyVisited);
   _currentIndex = _postorder.lastIndex();
   }

void TR::ReversePostorderSnapshotBlockIterator::visit(TR::Block *block, TR::BlockChecklist &alreadyVisited)
   {
   if (alreadyVisited.contains(block))
      return;
   alreadyVisited.add(block);

   TR_SuccessorIterator bi(block);
   for (TR::CFGEdge *edge = bi.getFirst(); edge != NULL; edge = bi.getNext())
      visit(edge->getTo()->asBlock(), alreadyVisited);
   if (block->getEntry()) // Don't add the special blocks with no trees to the snapshot
      _postorder.add(block);
   }

TR::Block *TR::ReversePostorderSnapshotBlockIterator::currentBlock()
   {
   if (0 <= _currentIndex && _currentIndex <= _postorder.lastIndex())
      return _postorder[_currentIndex];
   else
      return NULL;
   }

void TR::ReversePostorderSnapshotBlockIterator::stepForward()
   {
   TR_ASSERT(currentBlock(), "cannot stepForward a ReversePostorderSnapshotBlockIterator that has already finished its walk");
   do
      --_currentIndex;
   while (!isStepOperationFinished());
   logCurrentLocation();
   }

void TR::ReversePostorderSnapshotBlockIterator::stepBackward()
   {
   TR_ASSERT(currentBlock(), "cannot stepForward a ReversePostorderSnapshotBlockIterator that has already finished its walk");
   do
      ++_currentIndex;
   while (!isStepOperationFinished());
   logCurrentLocation();
   }

bool TR::ReversePostorderSnapshotBlockIterator::isStepOperationFinished()
   {
   if (!currentBlock())
      return true; // Reached the end
   else if (currentBlock()->isValid())
      return true; // Reached the next block in the walk

   if (isLoggingEnabled())
      traceMsg(_opt->comp(), "BLOCK  %s Skip block_%d removed during walk\n", _name, currentBlock()->getNumber());
   return false;
   }

void TR::ReversePostorderSnapshotBlockIterator::logCurrentLocation()
   {
   // TODO: Log even without an _opt.

   if (isLoggingEnabled())
      {
      if (currentBlock())
         traceMsg(_opt->comp(), "BLOCK  %s @ block_%d\n", _name, currentBlock()->getNumber());
      else
         traceMsg(_opt->comp(), "BLOCK  %s finished\n", _name);
      }
   }

TR::AllBlockIterator::AllBlockIterator(TR::CFG *cfg, TR::Optimization *opt, const char *name)
   :BlockIterator(opt, name),_cfg(cfg),_currentBlock(cfg->getFirstNode()->asBlock()),_nextBlock(_currentBlock->getNext()->asBlock()),_alreadyVisited(cfg->comp())
   {
   _alreadyVisited.add(_currentBlock);
   logCurrentLocation();
   }

TR::Block *TR::AllBlockIterator::currentBlock()
   {
   return _currentBlock;
   }

void TR::AllBlockIterator::stepForward()
   {
   TR_ASSERT(_currentBlock, "Cannot stepForward an AllBlockIterator that has already finished iterating");

   // INVARIANT: All visited blocks in the linked list of CFG nodes are at
   // the end of the list.  Therefore, when we hit an already-visited block,
   // we can quit scanning the list.

   // Note: it's crucially important that we use _currentBlock->getNext()->asBlock()
   // and definitely NOT _currentBlock->getNextBlock().  The latter returns
   // blocks in treetop order, which will not provide the "all blocks"
   // guarantee if new blocks are inserted physically before the current one.

   // Move forward, stopping when we hit the end of the list, or a block we've already seen
   //
   TR::CFGNode *next = _nextBlock;
   if (next == NULL || _alreadyVisited.contains(next->asBlock()))
      {
      // Start another lap of the list to catch newly added blocks we need to visit
      //
      next = _cfg->getFirstNode();
      TR_ASSERT(next != NULL, "AllBlockIterator not expecting a CFG with no blocks at all");
      if (_alreadyVisited.contains(next->asBlock()))
         {
         // Since newly added blocks always go at the front, and we never
         // insert a previously removed block, we're done as soon as we try
         // to take another lap and find that the first block in the list is
         // already visited.
         //
         next = NULL;
         }
      }

   if (REMOVED_BLOCKS_CAN_BE_REINSERTED && !next)
      {
      // Can't be certain about our "INVARIANT" after all.  Do another
      // pass looking for any blocks we haven't visited.
      //
      // This isn't the most efficient possible algorithm -- it is O(n^2) in the
      // worst case -- but it cleanly separates the cruft from the main algorithm.
      //
      for (next = _cfg->getFirstNode(); next; next = next->getNext())
         {
         if (!_alreadyVisited.contains(next->asBlock()))
            {
            // Found one!
            //
            if (isLoggingEnabled())
               traceMsg(_opt->comp(), "BLOCK  %s REMOVED_BLOCKS_CAN_BE_REINSERTED: block_%d found via extra scan\n", _name, next->asBlock()->getNumber());
            break;
            }
         }
      }

   if (next)
      {
      _currentBlock = next->asBlock();
      _nextBlock    = _currentBlock->getNext()? _currentBlock->getNext()->asBlock() : NULL;
      _alreadyVisited.add(_currentBlock);
      if (_currentBlock->getEntry())
         {
         logCurrentLocation();
         }
      else
         {
         // Skip the special blocks that have no trees.
         //
         stepForward();
         }
      }
   else
      {
      _currentBlock = NULL;
      }

   }

void TR::AllBlockIterator::logCurrentLocation()
   {
   // TODO: Log even without an _opt.

   if (isLoggingEnabled())
      {
      if (currentBlock())
         traceMsg(_opt->comp(), "BLOCK  %s @ block_%d\n", _name, currentBlock()->getNumber());
      else
         traceMsg(_opt->comp(), "BLOCK  %s finished\n", _name);
      }
   }

//
// ILValidator
//

TR::ILValidator::ILValidator(TR::Optimization *opt)
   :_opt(opt)
   ,_nodeStates(opt->comp()->trMemory())
   ,_liveNodes(_nodeStates, opt->comp()->trMemory())
   {
   }

TR::Compilation *TR::ILValidator::comp()
   {
   return _opt->comp();
   }

void TR::ILValidator::checkSoundness(TR::TreeTop *start, TR::TreeTop *stop)
   {
   soundnessRule(start, start != NULL, "Start tree must exist");
   soundnessRule(stop, !stop || stop->getNode() != NULL, "Stop tree must have a node");

   TR::NodeChecklist treetopNodes(comp()), ancestorNodes(comp()), visitedNodes(comp());

   // Can't use iterators here, because those presuppose the IL is sound.  Walk trees the old-fashioned way.
   //
   for (TR::TreeTop *currentTree = start; currentTree != stop; currentTree = currentTree->getNextTreeTop())
      {
      soundnessRule(currentTree, currentTree->getNode() != NULL, "Tree must have a node");
      soundnessRule(currentTree, !treetopNodes.contains(currentTree->getNode()), "Treetop node n%dn encountered twice", currentTree->getNode()->getGlobalIndex());

      treetopNodes.add(currentTree->getNode());

      TR::TreeTop *next = currentTree->getNextTreeTop();
      if (next)
         {
         soundnessRule(currentTree, next->getNode() != NULL, "Tree after n%dn must have a node", currentTree->getNode()->getGlobalIndex());
         soundnessRule(currentTree, next->getPrevTreeTop() == currentTree, "Doubly-linked treetop list must be consistent: n%dn->n%dn<-n%dn", currentTree->getNode()->getGlobalIndex(), next->getNode()->getGlobalIndex(), next->getPrevTreeTop()->getNode()->getGlobalIndex());
         }
      else
         {
         soundnessRule(currentTree, stop == NULL, "Reached the end of the trees after n%dn without encountering the stop tree n%dn", currentTree->getNode()->getGlobalIndex(), stop? stop->getNode()->getGlobalIndex() : NULL);
         checkNodeSoundness(currentTree, currentTree->getNode(), ancestorNodes, visitedNodes);
         }
      }
   }

void TR::ILValidator::checkNodeSoundness(TR::TreeTop *location, TR::Node *node, NodeChecklist &ancestorNodes, NodeChecklist &visitedNodes)
   {
   TR_ASSERT(node != NULL, "checkNodeSoundness requires that node is not NULL");
   if (visitedNodes.contains(node))
      return;
   visitedNodes.add(node);

   soundnessRule(location, !ancestorNodes.contains(node), "n%dn must not be its own ancestor", node->getGlobalIndex());
   ancestorNodes.add(node);

   for (int32_t i = 0; i < node->getNumChildren(); i++)
      {
      TR::Node *child = node->getChild(i);
      soundnessRule(location, child != NULL, "n%dn child %d must not be NULL", node->getGlobalIndex(), i);

      checkNodeSoundness(location, child, ancestorNodes, visitedNodes);
      }

   ancestorNodes.remove(node);
   }

void TR::ILValidator::soundnessRule(TR::TreeTop *location, bool condition, const char *formatStr, ...)
   {
   if (!condition)
      {
      // TODO: Beef this up to do smart things in debug and prod
      if (location && location->getNode())
         fprintf(stderr, "*** VALIDATION ERROR: IL is unsound at n%dn ***\nMethod: %s\n", location->getNode()->getGlobalIndex(), comp()->signature());
      else
         fprintf(stderr, "*** VALIDATION ERROR: IL is unsound ***\nMethod: %s\n", comp()->signature());
      va_list args;
      va_start(args, formatStr);
      vfprintf(stderr, formatStr, args);
      va_end(args);
      fprintf(stderr, "\n");
      static char *continueAfterValidationError = feGetEnv("TR_continueAfterValidationError");
      if (!continueAfterValidationError)
         {
         comp()->failCompilation<TR::CompilationException>("Validation error");
         }
      }
   }

void TR::ILValidator::validityRule(Location &location, bool condition, const char *formatStr, ...)
   {
   if (!condition)
      {
      // TODO: Beef this up to do smart things in debug and prod
      _isValidSoFar = false;
      TR::Node *node = location.currentNode();
      fprintf(stderr, "*** VALIDATION ERROR ***\nNode: %s n%dn\nMethod: %s\n", node->getOpCode().getName(), node->getGlobalIndex(), comp()->signature());
      va_list args;
      va_start(args, formatStr);
      vfprintf(stderr, formatStr, args);
      va_end(args);
      fprintf(stderr, "\n");
      static char *continueAfterValidationError = feGetEnv("TR_continueAfterValidationError");
      if (!continueAfterValidationError)
         {
         comp()->failCompilation<TR::CompilationException>("Validation error");
         }
      }
   }

void TR::ILValidator::updateNodeState(Location &newLocation)
   {
   TR::Node  *node = newLocation.currentNode();
   NodeState &state = _nodeStates[node];
   if (node->getReferenceCount() == state._futureReferenceCount)
      {
      // First occurrence -- do some bookkeeping
      //
      if (node->getReferenceCount() == 0)
         {
         validityRule(newLocation, node->getOpCode().isTreeTop(), "Only nodes with isTreeTop opcodes can have refcount == 0");
         }
      else
         {
         _liveNodes.add(node);
         }

      // Simulate decrement reference count on children
      //
      for (auto childIter = node->childIterator(); TR::Node *child = *childIter; ++childIter)
         {
         NodeState &childState = _nodeStates[child];
         if (_liveNodes.contains(child))
            {
            validityRule(newLocation, childState._futureReferenceCount >= 1, "Child %s n%dn already has reference count 0", child->getOpCode().getName(), child->getGlobalIndex());
            if (--childState._futureReferenceCount == 0)
               {
               _liveNodes.remove(child);
               }
            }
         else
            {
            validityRule(newLocation, node->getOpCode().isTreeTop(), "Child %s n%dn has already gone dead", child->getOpCode().getName(), child->getGlobalIndex());
            }
         }
      }

   if (isLoggingEnabled())
      {
      static const char *traceLiveNodesDuringValidation = feGetEnv("TR_traceLiveNodesDuringValidation");
      if (traceLiveNodesDuringValidation && !_liveNodes.isEmpty())
         {
         traceMsg(comp(), "    -- Live nodes: {");
         char *separator = "";
         for (LiveNodeWindow::Iterator lnwi(_liveNodes); lnwi.currentNode(); ++lnwi)
            {
            traceMsg(comp(), "%sn%dn", separator, lnwi.currentNode()->getGlobalIndex());
            separator = ", ";
            }
         traceMsg(comp(), "}\n");
         }
      }

   }

bool TR::ILValidator::treesAreValid(TR::TreeTop *start, TR::TreeTop *stop)
   {
   checkSoundness(start, stop);

   for (PostorderNodeOccurrenceIterator iter(start, _opt, "VALIDATOR"); iter != stop; ++iter)
      {
      updateNodeState(iter);

      // General node validation
      //
      validateNode(iter);

      //
      // Additional specific kinds of validation
      //

      TR::Node *node = iter.currentNode();
      if (node->getOpCodeValue() == TR::BBEnd)
         {
         // Determine whether this is the end of an extended block
         //
         bool isEndOfExtendedBlock = false;
         TR::TreeTop *nextTree = iter.currentTree()->getNextTreeTop();
         if (nextTree)
            {
            validityRule(iter, nextTree->getNode()->getOpCodeValue() == TR::BBStart, "Expected BBStart after BBEnd");
            isEndOfExtendedBlock = nextTree->getNode()->getBlock()->isExtensionOfPreviousBlock();
            }
         else
            {
            isEndOfExtendedBlock = true;
            }

         if (isEndOfExtendedBlock)
            validateEndOfExtendedBlock(iter);
         }
      }

   return _isValidSoFar;
   }

void TR::ILValidator::validateNode(Location &location)
   {
   }

void TR::ILValidator::validateEndOfExtendedBlock(Location &location)
   {
   // Ensure there are no nodes live across the end of a block
   //
   for (LiveNodeWindow::Iterator lnwi(_liveNodes); lnwi.currentNode(); ++lnwi)
      validityRule(location, false, "Node cannot live across block boundary at n%dn", lnwi.currentNode()->getGlobalIndex());

   // At the end of an extended block, no node we've already seen could ever be seen again.
   // Slide the live node window to keep its bitvector compact.
   //
   if (_liveNodes.isEmpty())
      _liveNodes.startNewWindow();
   }

TR::ILValidator::LiveNodeWindow::LiveNodeWindow(NodeSideTable<NodeState> &sideTable, TR_Memory *memory)
   :_sideTable(sideTable)
   ,_basis(0)
   ,_liveOffsets(10, memory, stackAlloc, growable)
   {
   }

bool TR::ILValidator::isLoggingEnabled()
   {
   // TODO: IL validation should have its own logging option.
   return (comp()->getOption(TR_TraceILWalks));
   }

void TR::JavaILValidator::validateNode(Location &location)
   {
   if (location.currentNode()->getOpCodeValue() == TR::asynccheck)
      validateAsynccheck(location);
   }

void TR::JavaILValidator::validateAsynccheck(Location &location)
   {
   for (LiveNodeWindow::Iterator lnwi(_liveNodes); lnwi.currentNode(); ++lnwi)
      {
      TR::Node *liveNode = lnwi.currentNode();
      if (liveNode->isInternalPointer())
         validityRule(location, liveNode->getPinningArrayPointer() != NULL, "Internal pointer node n%dn without pinning array pointer cannot live across asynccheck", liveNode->getGlobalIndex());
      }
   }

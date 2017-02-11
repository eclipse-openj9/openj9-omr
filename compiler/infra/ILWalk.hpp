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

#ifndef OMR_INFRA_ILWALK
#define OMR_INFRA_ILWALK

#include "infra/Checklist.hpp"
#include "infra/SideTable.hpp"
#include "infra/Stack.hpp"

namespace TR {

// "Imported" classes
//
class Block;
class CFG;
class Compilation;
class MethodSymbol;
class Node;
class Optimization;
class TreeTop;

// Classes declared here
//
class AllBlockIterator;
class ILValidator;
class PostorderNodeIterator;
class PostorderNodeOccurrenceIterator;
class PreorderNodeIterator;
class PreorderNodeOccurrenceIterator;
class ReversePostorderSnapshotBlockIterator;
class TreeTopIterator;


class TreeTopIteratorImpl
   {
   // This has essentially everything from TreeTopIterator except the actual
   // decision about when to call logCurrentLocation.  Different subclasses
   // have different conventions about the meaning of the "current location",
   // so they'll want to do this in different ways.

   protected:

   TR::TreeTop *_current;

   // For logging
   //
   const char       *_name; // NULL means we don't want logging
   TR::Optimization *_opt;

   void logCurrentLocation();

   void stepForward();
   void stepBackward();

   TreeTopIteratorImpl(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);

   public:

   TR::TreeTop *currentTree() { return _current; }
   TR::Node    *currentNode();
   bool         isAt(TR::TreeTop *tt) { return _current == tt; }
   void         jumpTo(TR::TreeTop *tt){ _current = tt; }

   bool isAt(TreeTopIteratorImpl &other) { return isAt(other.currentTree()); }
   bool isAt(PreorderNodeIterator &other);

   public: // operators

   bool operator ==(TR::TreeTop *tt) { return isAt(tt); }
   bool operator !=(TR::TreeTop *tt) { return !isAt(tt); }
   void operator ++(){ stepForward(); }
   void operator --(){ stepBackward(); }
   };

class TreeTopIterator: public TreeTopIteratorImpl
   {
   typedef TreeTopIteratorImpl Super;

   public:

   TreeTopIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL):
      TreeTopIteratorImpl(start, opt, name){ logCurrentLocation(); }

   void stepForward()  { Super::stepForward();  logCurrentLocation(); }
   void stepBackward() { Super::stepBackward(); logCurrentLocation(); }
   };

class NodeIterator: protected TreeTopIteratorImpl // abstract
   {
   typedef TreeTopIteratorImpl Super;

   protected:

   struct WalkState
      {
      TR::Node *_node;
      int32_t   _child;
      bool      _isBetweenChildren; // Makes logging pretty

      WalkState(TR::Node *node):_node(node),_child(0),_isBetweenChildren(false){}

      bool equals(struct WalkState &other){ return _node == other._node && _child == other._child; }
      bool operator==(struct WalkState &other){ return equals(other); }
      bool operator!=(struct WalkState &other){ return !equals(other); }
      };

   TR_Stack<WalkState> _stack;
   NodeChecklist       _checklist;

   NodeIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);
   NodeIterator(TR::TreeTop *start, TR::Compilation *comp);

   int32_t stackDepth()
      {
      // TODO: TR_Stack curently has an unsigned depth, which leads to all kinds
      // of havoc and undefined behaviour near zero.  It should have a signed
      // depth.  Until then, use this function instead of calling _stack.size()
      // directly.
      return (int32_t)_stack.size();
      }

   void logCurrentLocation();

   public:

   TR::TreeTop *currentTree()        { return Super::currentTree(); }
   TR::Node *currentNode()           { return _stack.top()._node; }
   bool isAt(TR::TreeTop *tt)        { return Super::isAt(tt); }
   bool isAt(TreeTopIterator &other) { return isAt(other.currentTree()); }
   bool isAt(PreorderNodeIterator &other);

   public: // operators

   bool operator ==(TR::TreeTop *tt) { return isAt(tt); }
   bool operator !=(TR::TreeTop *tt) { return !isAt(tt); }
   };

// Returns each node once, in the order in which they appear in the log
//
class PreorderNodeIterator: public NodeIterator
   {
   typedef class NodeIterator Super;

   bool alreadyBeenPushed(TR::Node *node);
   void push(TR::Node *node);

   public:

   PreorderNodeIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);
   PreorderNodeIterator(TR::TreeTop *start, TR::Compilation *comp);

   void         stepForward();

   public: // operators

   void operator ++() { stepForward(); }
   };

// Returns each node once, in postorder.
//
// This is the order in which nodes will be evaluated, except more strict.
// (Actual evaluation does not specify the order that siblings are visited.)
//
class PostorderNodeIterator: public NodeIterator
   {
   typedef class NodeIterator Super;

   bool alreadyBeenPushed(TR::Node *node);
   void push(TR::Node *node);
   void descend();

   public:

   PostorderNodeIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);
   PostorderNodeIterator(TR::TreeTop *start, TR::Compilation *comp);

   void         stepForward();

   public: // operators

   void operator ++() { stepForward(); }
   };

class NodeOccurrenceIterator: public NodeIterator
   {
   protected:

   NodeOccurrenceIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL):
      NodeIterator(start, opt, name){}

   void logCurrentLocation();

   public:

   TR::Node *currentNode();
   };

// Returns each node each time it appears, visiting nodes before their
// children, and visiting siblings in order.
//
// This is the order in which the nodes are listed in the logs.
//
class PreorderNodeOccurrenceIterator: public NodeOccurrenceIterator
   {
   typedef class NodeOccurrenceIterator Super;

   bool alreadyPushedChildren(TR::Node *node);
   void push(TR::Node *node);

   public:

   PreorderNodeOccurrenceIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);

   void stepForward();

   public: // operators

   void operator ++() { stepForward(); }

   };

// Returns each node each time it appears, visiting children before parents,
// and visiting siblings in order.
//
// This is the order in which nodes will be evaluated, except more strict.
// (Actual evaluation does not specify the order that siblings are visited.)
//
class PostorderNodeOccurrenceIterator: public NodeOccurrenceIterator
   {
   typedef class NodeOccurrenceIterator Super;

   bool alreadyPushedChildren(TR::Node *node);
   void pushLeftmost(TR::Node *node);

   public:

   PostorderNodeOccurrenceIterator(TR::TreeTop *start, TR::Optimization *opt=NULL, const char *name=NULL);

   void stepForward();

   public: // operators

   void operator ++() { stepForward(); }

   };

class BlockIterator
   {
   protected:

   TR::Optimization *_opt;
   const char       *_name;

   bool isLoggingEnabled();

   BlockIterator(TR::Optimization *opt, const char *name);
   };

// Scans blocks in reverse postorder from a given starting point, building up a
// list of blocks to visit.  Then returns each block in that order which is
// still part of the CFG at the time it is encountered.  Any block returned by
// this iterator is guaranteed to be in the CFG at the time it is returned.
//
class ReversePostorderSnapshotBlockIterator: protected BlockIterator
   {
   TR_Array<TR::Block*> _postorder;    // Note that this is opposite to the order we want, so we walk this array in reverse
   int32_t              _currentIndex; // can be 1 past the valid index range of _postorder: -1 or _postorder.lastIndex()+1

   void takeSnapshot(TR::Block *start);
   void visit(TR::Block *block, TR::BlockChecklist &alreadyVisited);

   void logCurrentLocation();

   bool isStepOperationFinished();

   public:

   ReversePostorderSnapshotBlockIterator(TR::Block *start, TR::Optimization *opt=NULL, const char *name=NULL);
   ReversePostorderSnapshotBlockIterator(TR::CFG *cfg, TR::Optimization *opt=NULL, const char *name=NULL);

   TR::Block   *currentBlock();
   bool         isAt(TR::Block *block){ return currentBlock() == block; }

   void stepForward();
   void stepBackward();

   public: // operators

   void operator ++() { stepForward();  }
   void operator --() { stepBackward(); }
   };

// Scans blocks in arbitrary order.  All blocks present in the CFG at the time
// the walk finishes will have been returned exactly once, and possibly some
// (but not necessarily all) blocks deleted during the walk.  Any block
// returned by this iterator is guaranteed to be in the CFG at the time it is
// returned.
//
// This iterator relies on the fact that CFGNodes are in a linked list into
// which new nodes are only ever prepended at the front.  Because of this, we
// can be guarantee that we will visit all nodes if we (1) start again at the
// front of the list whenever we reach the end, and (2) stop the first time we
// hit a block we've already visited.
//
class AllBlockIterator: protected BlockIterator
   {
   TR::CFG      *_cfg;
   TR::Block     *_currentBlock;
   TR::Block     *_nextBlock;
   BlockChecklist _alreadyVisited;

   void logCurrentLocation();

   public:

   AllBlockIterator(TR::CFG *cfg, TR::Optimization *opt=NULL, const char *name=NULL);

   TR::Block   *currentBlock();
   bool         isAt(TR::Block *block){ return currentBlock() == block; }

   void stepForward();

   public: // operators

   void operator ++() { stepForward(); }
   };

//
// Validator stuff (which should probably be in a different file)
//

class ILValidator
   {
   TR::Optimization *_opt; // TODO: Operate without an Optimization
   bool              _isValidSoFar;

   public:

   ILValidator(TR::Optimization *opt);

   bool isLoggingEnabled();

   // "Soundness" comprises the criteria required to make a IL iterators
   // function properly.  Compilation will abort if the trees are unsound
   // because we can't realistically continue when our most basic assumptions
   // are violated.
   // Note that the "stop" tree is not checked.
   //
   void checkSoundness(TR::TreeTop *start, TR::TreeTop *stop=NULL);

   // "Valid" comprises any kind of validity rules anyone might want to impose on the trees.
   //
   bool treesAreValid(TR::TreeTop *start, TR::TreeTop *stop=NULL);

   protected: // Soundness

   //
   // Soundness checks don't even try to pretend they can continue after a
   // failure.  If the trees are unsound, we don't want to try to continue
   // processing them.  This means that a soundness check can rely on all
   // previous checks succeeding.
   //

   void soundnessRule(TR::TreeTop *location, bool condition, const char *formatStr, ...); // returns isSoundSoFar for convenience
   void checkNodeSoundness(TR::TreeTop *location, TR::Node *node, NodeChecklist &ancestorNodes, NodeChecklist &visitedNodes);

   protected: // Validation

   typedef PostorderNodeOccurrenceIterator Location;

   struct NodeState
      {
      TR::Node    *_node;
      ncount_t     _futureReferenceCount;

      NodeState(TR::Node *node):_node(node),_futureReferenceCount(node->getReferenceCount()){}
      };

   class LiveNodeWindow
      {
      // This is like a NodeChecklist, but more compact.  Rather than track
      // node global indexes, which can be sparse, this tracks local
      // indexes, which are relatively dense.  Furthermore, the _basis field
      // allows us not to waste space on nodes we saw in prior blocks.

      NodeSideTable<NodeState> &_sideTable;
      int32_t                   _basis;
      TR_BitVector              _liveOffsets; // sideTable index = basis + offset

      public:

      LiveNodeWindow(NodeSideTable<NodeState> &sideTable, TR_Memory *memory);

      bool contains(TR::Node *node)
         {
         int32_t index = _sideTable.indexOf(node);
         if (index < _basis)
            return false;
         else
            return _liveOffsets.isSet(index - _basis);
         }

      void add(TR::Node *node)
         {
         int32_t index = _sideTable.indexOf(node);
         TR_ASSERT(index >= _basis, "Cannot mark node n%dn before basis %d live", node->getGlobalIndex(), _basis);
         _liveOffsets.set(index - _basis);
         }

      void remove(TR::Node *node)
         {
         int32_t index = _sideTable.indexOf(node);
         if (index >= _basis)
            _liveOffsets.reset(index - _basis);
         }

      bool isEmpty()
         {
         return _liveOffsets.isEmpty();
         }

      TR::Node *anyLiveNode()
         {
         Iterator iter(*this);
         return iter.currentNode();
         }

      void startNewWindow()
         {
         TR_ASSERT(_liveOffsets.isEmpty(), "Cannot close LiveNodeWindow when there are still live nodes");
         _basis = _sideTable.size();
         }

      public:

      class Iterator
         {
         LiveNodeWindow      &_window;
         TR_BitVectorIterator _bvi;
         int32_t              _currentOffset; // -1 means iteration is past end

         bool isPastEnd(){ return _currentOffset < 0; }

         public:

         Iterator(LiveNodeWindow &window):_window(window),_bvi(window._liveOffsets),_currentOffset(0xdead /* anything non-negative */)
            {
            stepForward();
            }

         TR::Node *currentNode()
            {
            if (isPastEnd())
               return NULL;
            else
               return _window._sideTable.getAt(_window._basis + _currentOffset)._node;
            }

         void stepForward()
            {
            TR_ASSERT(!isPastEnd(), "Can't stepForward a LiveNodeWindow::Iterator that's already past end");
            if (_bvi.hasMoreElements())
               _currentOffset = _bvi.getNextElement();
            else
               _currentOffset = -1;
            }

         public: // operators

         void operator ++() { stepForward(); }
         };
      };

   NodeSideTable<NodeState>  _nodeStates;
   LiveNodeWindow            _liveNodes;

   bool isValidSoFar(){ return _isValidSoFar; }
   void validityRule(Location &location, bool condition, const char *formatStr, ...);

   protected: // Individual validation routines

   // These return false to indicate a validity error was found.
   // Note that these are given verb names ("validateXxx") instead of adjective
   // names ("xxxIsValid") because generally they are stateful.

   // Called on every node, before all other validation routines
   //
   virtual void validateNode(Location &location);

   //
   // Additional validation hooks
   //

   virtual void validateEndOfExtendedBlock(Location &location);

   protected:

   TR::Compilation *comp();

   void updateNodeState(Location &newLocation);
   };

class JavaILValidator: public ILValidator
   {
   typedef ILValidator Super;

   virtual void validateNode(Location &location);

   public: // Java-specific validity checks

   void validateAsynccheck(Location &location);
   };

}

#endif

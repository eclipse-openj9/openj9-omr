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

#ifndef NODEPOOL_INCL
#define NODEPOOL_INCL

#include "cs2/bitvectr.h"    // for ABitVector
#include "cs2/hashtab.h"     // for HashTable
#include "cs2/tableof.h"     // for TableOf, TableIndex, etc
#include "env/TRMemory.hpp"  // for Allocator, TR_Memory, etc
#include "il/Node.hpp"       // for Node
#include "il/NodeUtils.hpp"  // for etc

namespace TR { class SymbolReference; }
namespace TR { class Compilation; }
template <class T> class TR_Array;

namespace TR {

class NodePool
   {
   public:
   typedef CS2::TableIndex NodeIndex;
   typedef CS2::TableOf<TR::Node,TR::Allocator, 8, CS2::ABitVector> Pool_t;
   typedef TR::SparseBitVector DeadNodes;
   typedef CS2::TableOf<TR::Node,TR::Allocator, 8, CS2::ABitVector>::Cursor NodeIter;

   TR_ALLOC(TR_Memory::Compilation)
   NodePool(TR::Compilation * comp, const TR::Allocator &allocator):
      _comp(comp),
      _pool(allocator),
      _globalIndex(0),
      _poolIndex(0),
      _disableGC(true),
      _optAttribGlobalIndex(0),
      _optAttribPool(allocator)
      {
      }

   TR::Node * allocate(ncount_t poolIndex = 0);
   bool      deallocate(TR::Node * node);
   TR::Node * getNodeAtPoolIndex(ncount_t poolIdx);
   void      removeNodeAndReduceGlobalIndex(TR::Node * node);
   bool      removeDeadNodes();
   void      enableNodeGC()  { _disableGC = false; }
   void      disableNodeGC() { _disableGC = true; }
   ncount_t  getLastGlobalIndex()     { return _globalIndex; }
   ncount_t  getLastPoolIndex()       { return _poolIndex; }
   ncount_t  getMaxIndex()           { return _globalIndex; }
   TR::Compilation * comp() { return _comp; }

   OMR::Node::OptAttributes * allocateOptAttributes();
   void FreeOptAttributes();

   void cleanUp();

   private:
   void     removeNode(NodeIndex globalIdx);

   TR::Compilation *     _comp;
   bool                  _disableGC;
   ncount_t              _globalIndex;
   ncount_t              _poolIndex;
   ncount_t              _optAttribGlobalIndex;

   Pool_t                _pool;

   CS2::TableOf<OMR::Node::OptAttributes,TR::Allocator, 8, CS2::ABitVector> _optAttribPool;
   };

}

#endif

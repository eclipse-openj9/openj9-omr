/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2017
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

   TR_ALLOC(TR_Memory::Compilation)
   NodePool(TR::Compilation * comp, const TR::Allocator &allocator);

   TR::Node * allocate();
   bool      deallocate(TR::Node * node);
   bool      removeDeadNodes();
   void      enableNodeGC()  { _disableGC = false; }
   void      disableNodeGC() { _disableGC = true; }
   ncount_t  getLastGlobalIndex()     { return _globalIndex; }
   ncount_t  getMaxIndex()           { return _globalIndex; }
   TR::Compilation * comp() { return _comp; }

   void cleanUp();

   private:
   TR::Compilation *     _comp;
   bool                  _disableGC;
   ncount_t              _globalIndex;

   TR::Region            _nodeRegion;
   };

}

#endif

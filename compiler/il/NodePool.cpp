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

#include "il/NodePool.hpp"

#include <stddef.h>                 // for NULL
#include "compile/Compilation.hpp"  // for comp
#include "il/ILOps.hpp"             // for ILOpCode
#include "il/Node.hpp"              // for Node
#include "il/Node_inlines.hpp"      // for Node::getNodePoolIndex, etc
#include "infra/Assert.hpp"         // for TR_ASSERT

#define OPT_DETAILS_NODEPOOL "O^O NODEPOOL :"

TR::NodePool::NodePool(TR::Compilation * comp, const TR::Allocator &allocator) :
   _comp(comp),
   _disableGC(true),
   _globalIndex(0),
   _poolIndex(0),
   _nodeRegion(comp->trMemory()->heapMemoryRegion())
   {
   }

void
TR::NodePool::cleanUp()
   {
   _nodeRegion.~Region();
   new (&_nodeRegion) TR::Region(_comp->trMemory()->heapMemoryRegion());
   }

TR::Node *
TR::NodePool::allocate()
   {
   TR::Node *newNode = static_cast<TR::Node*>(_nodeRegion.allocate(sizeof(TR::Node)));//_pool.ElementAt(poolIndex);
   memset(newNode, 0, sizeof(TR::Node));
   newNode->_poolIndex = ++_poolIndex;
   newNode->_globalIndex = ++_globalIndex;
   TR_ASSERT(_globalIndex < MAX_NODE_COUNT, "Reached TR::Node allocation limit");
   
   if (debug("traceNodePool"))
      {
      diagnostic("%sAllocating Node[%p] with Global Index %d, Pool Index %d\n", OPT_DETAILS_NODEPOOL, newNode, newNode->getGlobalIndex(), newNode->getNodePoolIndex());
      }
   return newNode;
   }

bool
TR::NodePool::deallocate(TR::Node * node)
   {
   if (_disableGC)
      {
      if (debug("traceNodePool"))
         {
         diagnostic("%s Node garbage collection disabled", OPT_DETAILS_NODEPOOL);
         }
       return false;
      }

   node->~Node();
   return true;
   }

void
TR::NodePool::removeNodeAndReduceGlobalIndex(TR::Node * node)
   {
   node->~Node();
   _globalIndex--;
   }

bool
TR::NodePool::removeDeadNodes()
   {
   if (_disableGC)
      {
      if (debug("traceNodePool"))
         {
         diagnostic("%s Node garbage collection disabled", OPT_DETAILS_NODEPOOL);
         }
       return false;
      }

   TR_ASSERT(false, "Node garbage colleciotn is not currently implemented");

   return false;
   }


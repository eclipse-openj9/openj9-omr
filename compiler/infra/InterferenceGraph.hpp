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

#ifndef INTERFERENCEGRAPH_INCL
#define INTERFERENCEGRAPH_INCL

#include <stddef.h>                 // for NULL
#include <stdint.h>                 // for int32_t
#include "env/TRMemory.hpp"         // for Allocator, TR_HeapMemory, etc
#include "env/jittypes.h"           // for uintptrj_t
#include "compile/Compilation.hpp"  // for Compilation
#include "cs2/bitvectr.h"           // for ABitVector
#include "infra/Array.hpp"          // for TR_Array
#include "infra/Assert.hpp"         // for TR_ASSERT
#include "infra/BitVector.hpp"      // for TR_BitVector
#include "infra/IGBase.hpp"         // for TR_IGBase, IMIndex
#include "infra/IGNode.hpp"         // for IGNodeColour, TR_IGNode, etc

template <class T> class TR_Stack;

class TR_InterferenceGraph : public TR_IGBase
   {
   struct HashTable;
   friend struct HashTable;

   // Ordered table of all allocated nodes.
   //
   TR_Array<TR_IGNode *> * _nodeTable;

   // FILO structure of nodes for simplify/select phases.
   //
   TR_Stack<TR_IGNode *> * _nodeStack;

   // Hash table mapping of entity addresses to IG nodes.
   //
   struct HashTableEntry
      {
      HashTableEntry *_next;
      TR_IGNode      *_igNode;
      };

   struct HashTable
      {
      int32_t          _numBuckets;
      HashTableEntry **_buckets;
      };

   HashTable _entityHash;

   virtual bool simplify();
   virtual bool select();

   protected:

   TR::Compilation * comp() {return _compilation;}

   TR_Memory *               trMemory()                    { return _trMemory; }
   TR_StackMemory            trStackMemory()               { return _trMemory; }
   TR_HeapMemory             trHeapMemory()                { return _trMemory; }

   void partitionNodesIntoDegreeSets(CS2::ABitVector<TR::Allocator> &workingSet,
                                     CS2::ABitVector<TR::Allocator> &colourableDegreeSet,
                                     CS2::ABitVector<TR::Allocator> &notColourableDegreeSet);

   TR::Compilation *_compilation;
   TR_Memory *_trMemory;

   public:

   TR_InterferenceGraph(TR::Compilation *comp) :
      _compilation(comp),
      _nodeTable(NULL),
      _nodeStack(NULL),
      _trMemory(comp->trMemory()),
      TR_IGBase() {}

   TR_InterferenceGraph(TR::Compilation *comp, int32_t estimatedNodes = 32);

   TR_IGNode *getNodeTable(IGNodeIndex i) {return ((*_nodeTable)[i]);}

   TR_Stack<TR_IGNode *> *getNodeStack()  {return _nodeStack;}
   TR_Stack<TR_IGNode *> *setNodeStack(TR_Stack<TR_IGNode *> *ns) {return(_nodeStack = ns);}

   int32_t entityHashBucket(void *entity) {return (int32_t)(((uintptrj_t)entity >> 2) % _entityHash._numBuckets);}
   void addIGNodeToEntityHash(TR_IGNode *igNode);

   TR_IGNode *add(void *entity, bool ignoreDuplicates = false);
   void addInterferenceBetween(void *entity1, void *entity2)
       {
       TR_IGNode *igNode1 = getIGNodeForEntity(entity1);
       TR_IGNode *igNode2 = getIGNodeForEntity(entity2);
       TR_ASSERT(igNode1,"addInterferenceBetween: entity1 %p does not exist in this interference graph\n", entity1);
       TR_ASSERT(igNode2,"addInterferenceBetween: entity2 %p does not exist in this interference graph\n", entity2);

       addInterferenceBetween(igNode1,igNode2);
       }

   void addInterferenceBetween(TR_IGNode *igNode1, TR_IGNode *igNode2);
   void removeAllInterferences(void *entity);

   TR_IGNode *getIGNodeForEntity(void *entity);
   bool hasInterference(void *entity1, void *entity2);
   bool hasInterference(TR_IGNode *node1, TR_IGNode *node2)
       {
       IMIndex bit = getNodePairToBVIndex(node1->getIndex(), node2->getIndex());
       return getInterferenceMatrix()->isSet(bit);
       }

   void removeInterferenceBetween(void *entity1, void *entity2)
       {
       TR_IGNode *igNode1 = getIGNodeForEntity(entity1);
       TR_IGNode *igNode2 = getIGNodeForEntity(entity2);

       TR_ASSERT(igNode1, "hasInterference: entity1 %p does not exist in this interference graph\n", entity1);
       TR_ASSERT(igNode2, "hasInterference: entity2 %p does not exist in this interference graph\n", entity2);
       removeInterferenceBetween(igNode1,igNode2);
       }
   void removeInterferenceBetween(TR_IGNode *node1, TR_IGNode *node2);

   void virtualRemoveEntityFromIG(void *entity);
   void virtualRemoveNodeFromIG(TR_IGNode *igNode);

   IGNodeDegree findMaxDegree();

   bool doColouring(IGNodeColour numColours);
   IGNodeColour findMinimumChromaticNumber();

#ifdef DEBUG
   void dumpIG(char *msg = NULL);
#else
   void dumpIG(char *msg = NULL) {}
#endif
   };
#endif

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

#ifndef IGNODE_INCL
#define IGNODE_INCL

#include <stddef.h>          // for NULL
#include <stdint.h>          // for int32_t, uint16_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "infra/Assert.hpp"  // for TR_ASSERT
#include "infra/Flags.hpp"   // for flags8_t
#include "infra/List.hpp"    // for List

namespace TR { class Compilation; }

typedef uint32_t IGNodeIndex;
typedef uint32_t IGNodeDegree;
typedef int32_t  IGNodeColour;

// Maximum number of neighbours of any node in the interference graph.
// The formula assumes the degree can not be negative values.
//
#define MAX_DEGREE          ((IGNodeDegree)((1L << (sizeof(IGNodeDegree) << 3)) - 1))

#define UNCOLOURED          -1
#define CANNOTCOLOR         -1

class TR_IGNode
   {
   void           *_pEntity;

   IGNodeIndex     _index;
   IGNodeDegree    _degree;
   IGNodeDegree    _workingDegree;
   IGNodeColour    _colour;
   List<TR_IGNode> _adjList;
   flags8_t        _flags;

   enum
      {
      MustBeSpilled   = 0x01,  // Node must be spilled
      IsRemovedFromIG = 0x02,   // Node has been removed from interference graph
      };

   public:

   TR_ALLOC(TR_Memory::IGNode)

   TR_IGNode(TR_Memory * m) :
      _pEntity(NULL),
      _index(0),
      _degree(0),
      _workingDegree(0),
      _colour(UNCOLOURED),
      _adjList(m),
      _flags(0) {}

   TR_IGNode(void *p, TR_Memory * m) :
      _pEntity(p),
      _index(0),
      _degree(0),
      _workingDegree(0),
      _colour(UNCOLOURED),
      _adjList(m),
      _flags(0) {}

   IGNodeColour getColour()       {return _colour;}
   void setColour(IGNodeColour c) {_colour = c;}
   bool isColoured()              {return (_colour == UNCOLOURED) ? false : true;}

   IGNodeIndex getIndex()       {return _index;}
   void setIndex(IGNodeIndex i) {_index = i;}

   List<TR_IGNode> &getAdjList() {return _adjList;}

   void *getEntity()       {return _pEntity;}
   void setEntity(void *p) {_pEntity = p;}

   IGNodeDegree getWorkingDegree()       {return _workingDegree;}
   void setWorkingDegree(IGNodeDegree d) {_workingDegree = d;}
   IGNodeDegree incWorkingDegree()
      {
      TR_ASSERT(_workingDegree < MAX_DEGREE, "incWorkingDegree: maximum node degree exceeded!");
      return ++_workingDegree;
      }

   IGNodeDegree incWorkingDegreeBy(IGNodeDegree d)
      {
      TR_ASSERT((MAX_DEGREE-_workingDegree) >= d, "incWorkingDegreeBy: maximum node degree exceeded!");
      return (_workingDegree += d);
      }

   IGNodeDegree decWorkingDegree()
      {
      TR_ASSERT(_workingDegree >= 1, "decWorkingDegree: working degree is less than 0! (_workingDegree=%d nodeIndex=%d)",_workingDegree, getIndex());
      return --_workingDegree;
      }

   IGNodeDegree decWorkingDegreeBy(IGNodeDegree d)
      {
      TR_ASSERT(_workingDegree >= d, "decWorkingDegreeBy: degree is less than 0!");
      return (_workingDegree -= d);
      }

   IGNodeDegree getDegree()       {return _degree;}
   void setDegree(IGNodeDegree d) {_degree = d;}

   IGNodeDegree incDegree()
      {
      TR_ASSERT(_degree < MAX_DEGREE, "incDegree: maximum node degree exceeded!");
      return ++_degree;
      }

   IGNodeDegree incDegreeBy(IGNodeDegree d)
      {
      TR_ASSERT((MAX_DEGREE-_degree) >= d, "incDegreeBy: maximum node degree exceeded!");
      return (_degree += d);
      }

   IGNodeDegree decDegree()
      {
      TR_ASSERT(_degree >= 1, "decDegree: degree is less than 0!");
      return --_degree;
      }

   IGNodeDegree decDegreeBy(IGNodeDegree d)
      {
      TR_ASSERT(_degree >= d, "decDegreeBy: degree is less than 0!");
      return (_degree -= d);
      }

   bool isRemovedFromIG()      {return _flags.testAny(IsRemovedFromIG);}
   void setIsRemovedFromIG()   {_flags.set(IsRemovedFromIG);}
   void resetIsRemovedFromIG() {_flags.reset(IsRemovedFromIG);}

   void decWorkingDegreeOfNeighbours();

#ifdef DEBUG
      void print(TR::Compilation *);
#endif

   };

#endif

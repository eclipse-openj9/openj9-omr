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

#include "infra/IGNode.hpp"

#include "compile/Compilation.hpp"  // for Compilation
#include "infra/List.hpp"           // for ListIterator, List

void TR_IGNode::decWorkingDegreeOfNeighbours()
   {
   ListIterator<TR_IGNode> iterator(&getAdjList());
   TR_IGNode *cursor = iterator.getFirst();

   while (cursor)
      {
      if (!cursor->isRemovedFromIG())
         {
         cursor->decWorkingDegree();
         }

      cursor = iterator.getNext();
      }

   setWorkingDegree(0);
   }


#ifdef DEBUG
void TR_IGNode::print(TR::Compilation * comp)
   {
   diagnostic("[ %5d ]  :  IGnode addr:   %p", _index, this);
   diagnostic("\n              data addr:     %p", _pEntity);
   diagnostic("\n              flags [%04x]:  ", _flags.getValue());

   if (_flags.getValue() == 0)
      {
      diagnostic("none");
      }
   else
      {
      if (isRemovedFromIG()) {diagnostic("isRemovedFromIG ");}
      }

   diagnostic("\n              ------------------------------------------------------------------");
   diagnostic("\n              colour:              ");

   if (_colour == UNCOLOURED)
      {
      diagnostic("uncoloured");
      }
   else
      {
      diagnostic("%08x", _colour);
      }

   diagnostic("\n              degree:              %d", _degree);

   diagnostic("\n              interfering ranges:");

   int32_t icount = 0;
   if (_degree > 0)
      {
#if 0
      TR_BitVector *adjSet = new (STACK_NEW) TR_BitVector(ig->getNumNodes(), stackAlloc);

      ListIterator<TR_IGNode> iterator(&getAdjList());
      TR_IGNode *adjCursor = iterator.getFirst();

      while (adjCursor)
         {
         adjSet->set(adjCursor->getIndex());
         adjCursor = iterator.getNext();
         }

      int32_t              igNodeIndex;
      TR_BitVectorIterator bvi(*adjSet);

      while (bvi.hasMoreElements())
         {
         igNodeIndex = bvi.getNextElement();
         diagnostic("\n                 #%-3d [ %5d : %p ]",
                     ++icount, igNodeIndex, ig->getNodeTable(igNodeIndex));
         }
#else
      ListIterator<TR_IGNode> iterator(&getAdjList());
      TR_IGNode *adjCursor = iterator.getFirst();

      while (adjCursor)
         {
         diagnostic("\n                 #%-3d [ %5d : %p ]", ++icount, adjCursor->getIndex(), adjCursor);
         adjCursor = iterator.getNext();
         }
#endif

      }
   else
      {
      diagnostic("\n                 [ none ]");
      }

   diagnostic("\n\n");
   }
#endif

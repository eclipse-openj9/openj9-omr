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

#ifndef OMR_TREETOP_INLINE_INCL
#define OMR_TREETOP_INLINE_INCL

#include "il/ILOps.hpp"          // for ILOpCode
#include "il/Node.hpp"           // for Node
#include "il/OMRTreeTop.hpp"     // for TreeTop
#include "il/TreeTop.hpp"        // for TreeTop
#include "infra/Assert.hpp"      // for TR_ASSERT

namespace TR { class Block; }

/*
 * Performance sensitive implementations are included here to
 * support inlining.
 *
 * This file will be included by OMRTreeTop.cpp
 */

inline TR::TreeTop *
OMR::TreeTop::self()
   { return static_cast<TR::TreeTop*>(this); }

inline TR::TreeTop *
OMR::TreeTop::getNextTreeTop()
   { return _pNext; }

inline TR::TreeTop *
OMR::TreeTop::setNextTreeTop(TR::TreeTop *p)
   { return (_pNext = p); }

inline TR::TreeTop *
OMR::TreeTop::getPrevTreeTop()
   { return _pPrev; }

inline TR::TreeTop *
OMR::TreeTop::setPrevTreeTop(TR::TreeTop *p)
   { return (_pPrev = p); }

inline TR::Node *
OMR::TreeTop::getNode()
   { return _pNode; }

inline TR::Node *
OMR::TreeTop::setNode(TR::Node *p)
   { return (_pNode = p); }

inline void
OMR::TreeTop::unlink(bool decRefCountRecursively)
   {
   TR::TreeTop *prevTT = self()->getPrevTreeTop();
   TR::TreeTop *nextTT = self()->getNextTreeTop();

   prevTT->setNextTreeTop(nextTT);
   nextTT->setPrevTreeTop(prevTT);
   if (decRefCountRecursively)
      self()->getNode()->recursivelyDecReferenceCount();
   }

inline void
OMR::TreeTop::join(TR::TreeTop * p)
   {
   if (self())
      self()->setNextTreeTop(p);
   if (p)
      p->setPrevTreeTop(self());
   }

inline TR::TreeTop *
OMR::TreeTop::insertAfter(TR::TreeTop *tt)
   {
   tt->join(self()->getNextTreeTop());
   self()->join(tt);
   return tt;
   }

inline TR::TreeTop *
OMR::TreeTop::insertBefore(TR::TreeTop *tt)
   {
   self()->getPrevTreeTop()->join(tt);
   tt->join(self());
   return tt;
   }

inline void
OMR::TreeTop::insertNewTreeTop(TR::TreeTop *beforeNewTreeTop, TR::TreeTop * afterNewTreeTop)
   {
   TR_ASSERT(beforeNewTreeTop->getNextTreeTop() == afterNewTreeTop && afterNewTreeTop->getPrevTreeTop() == beforeNewTreeTop, "beforeNewTreeTop and afterNewTreeTop must be consecutive");
   beforeNewTreeTop->join(self());
   self()->join(afterNewTreeTop);
   }

inline TR::TreeTop *
OMR::TreeTop::getNextRealTreeTop()
   {
   TR::TreeTop *treeTop;
   for (treeTop = self()->getNextTreeTop();
        treeTop && treeTop->getNode() && treeTop->getNode()->getOpCode().isExceptionRangeFence();
        treeTop = treeTop->getNextTreeTop())
      {}
   return treeTop;
   }

inline TR::TreeTop *
OMR::TreeTop::getPrevRealTreeTop()
   {
   TR::TreeTop *treeTop;
   for (treeTop = self()->getPrevTreeTop();
        treeTop && treeTop->getNode()->getOpCode().isExceptionRangeFence();
        treeTop = treeTop->getPrevTreeTop())
      {}
   return treeTop;
   }

inline TR::Block *
OMR::TreeTop::getEnclosingBlock( bool forward)
   {
   TR::TreeTop * tt = self();
   if (forward)
      while (tt->getNode()->getOpCodeValue() != TR::BBEnd)
         {
         tt = tt->getNextTreeTop();
         //TR_ASSERT(tt && tt->getNode(), "either tt or node on a tt null here, we will segfault");
         }
   else
      while (tt->getNode()->getOpCodeValue() != TR::BBStart)
         {
         tt = tt->getPrevTreeTop();
         //TR_ASSERT(tt && tt->getNode(), "either tt or node on a tt null here, we will segfault");
         }
   return tt->getNode()->getBlock();
   }

#endif

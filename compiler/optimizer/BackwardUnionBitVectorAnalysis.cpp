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
 *******************************************************************************/

#include <stddef.h>                                 // for NULL
#include <stdint.h>                                 // for int32_t
#include "optimizer/DataFlowAnalysis.hpp"

class TR_BitVector;

// This file defines the methods in class BackwardUnionBitVectorAnalysis.
//
template<class Container>void TR_BackwardUnionDFSetAnalysis<Container *>::compose(Container *firstBitVector, Container *secondBitVector)
   {
   *firstBitVector |= *secondBitVector;
   }

template<class Container>void TR_BackwardUnionDFSetAnalysis<Container *>::inverseCompose(Container *firstBitVector, Container *secondBitVector)
   {
   *firstBitVector &= *secondBitVector;
   }


template<class Container>void TR_BackwardUnionDFSetAnalysis<Container *>::initializeOutSetInfo()
   {
   int32_t i;
   for (i=0;i<this->_numberOfNodes;i++)
      this->_currentOutSetInfo[i]->empty();
   }


template<class Container>void TR_BackwardUnionDFSetAnalysis<Container *>::initializeCurrentGenKillSetInfo()
   {
   }


template<class Container>Container *TR_BackwardUnionDFSetAnalysis<Container *>::initializeInfo(Container *info)
   {
   Container *result = info;
   if (result == NULL)
      this->allocateContainer(&result, true);
   else
      result->empty();
   return result;
   }


template<class Container>Container * TR_BackwardUnionDFSetAnalysis<Container *>::inverseInitializeInfo(Container *info)
   {
   Container *result = info;
   if (result == NULL)
#if FLEX_USE_INVERTED_BIT_VECTORS
      this->allocateContainer(&result, true);
#else
      this->allocateContainer(&result, false);
#endif
   result->setAll(this->_numberOfBits);
   return result;
   }


template<class Container>TR_DataFlowAnalysis::Kind TR_BackwardUnionDFSetAnalysis<Container *>::getKind()
   {
   return TR_DataFlowAnalysis::BackwardUnionDFSetAnalysis;
   }


template class TR_BackwardUnionDFSetAnalysis<TR_BitVector *>;
template class TR_BackwardUnionDFSetAnalysis<TR_SingleBitContainer *>;

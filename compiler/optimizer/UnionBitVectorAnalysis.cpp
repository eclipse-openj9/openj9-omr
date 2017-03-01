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

#include <stddef.h>                                      // for NULL
#include "optimizer/DataFlowAnalysis.hpp"


class TR_BitVector;

// #define MAX_BLOCKS_FOR_STACK_ALLOCATION 128

// This file defines the methods in class UnionBitVectorAnalysis.
//
//
template<class Container>void TR_UnionDFSetAnalysis<Container *>::compose(Container *firstBitVector, Container *secondBitVector)
   {
   *firstBitVector |= *secondBitVector;
   }

template<class Container>void TR_UnionDFSetAnalysis<Container *>::inverseCompose(Container *firstBitVector, Container *secondBitVector)
   {
   *firstBitVector &= *secondBitVector;
   }

template<class Container>void TR_UnionDFSetAnalysis<Container *>::initializeInSetInfo()
   {
   this->_currentInSetInfo->empty();
   }

template<class Container>void TR_UnionDFSetAnalysis<Container *>::initializeCurrentGenKillSetInfo()
   {
   this->_currentRegularGenSetInfo->empty();
   this->_currentRegularKillSetInfo->setAll(this->_numberOfBits);
   }

template<class Container>Container * TR_UnionDFSetAnalysis<Container *>::initializeInfo(Container *info)
   {
   Container *result = info;
   if (result == NULL)
      this->allocateContainer(&result, true);
   else
      result->empty();
   return result;
   }

template<class Container>Container * TR_UnionDFSetAnalysis<Container *>::inverseInitializeInfo(Container *info)
   {
   Container *result = info;
   if (result == NULL)
      this->allocateContainer(&result, false);
   result->setAll(this->_numberOfBits);
   return result;
   }


template<class Container>TR_DataFlowAnalysis::Kind TR_UnionDFSetAnalysis<Container *>::getKind()
   {
   return TR_DataFlowAnalysis::UnionDFSetAnalysis;
   }


template class TR_UnionDFSetAnalysis<TR_BitVector *>;
template class TR_UnionDFSetAnalysis<TR_SingleBitContainer *>;

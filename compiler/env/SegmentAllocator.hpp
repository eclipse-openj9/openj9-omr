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

#ifndef OMR_SEGMENT_ALLOCATOR_HPP
#define OMR_SEGMENT_ALLOCATOR_HPP

#pragma once

#include "env/SegmentProvider.hpp"

namespace TR {

class SegmentAllocator : public SegmentProvider
   {
public:
   virtual size_t regionBytesAllocated() const throw() = 0;
   virtual size_t systemBytesAllocated() const throw() = 0;
   virtual size_t allocationLimit() const throw() = 0;
   virtual void setAllocationLimit(size_t) = 0;

protected:
   explicit SegmentAllocator(size_t defaultSegmentSize) :
      SegmentProvider(defaultSegmentSize)
      {
      }

   SegmentAllocator(const SegmentAllocator &other) :
      SegmentProvider(other)
      {
      }

   virtual ~SegmentAllocator() throw();
   };

}

#endif // OMR_SEGMENT_ALLOCATOR_HPP

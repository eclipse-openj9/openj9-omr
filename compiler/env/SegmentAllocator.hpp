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

#ifndef OMR_SEGMENT_ALLOCATOR_HPP
#define OMR_SEGMENT_ALLOCATOR_HPP

#pragma once

#include "env/RawAllocator.hpp"
#include "env/MemorySegment.hpp"
#include "infra/Assert.hpp"

namespace TR {

class SegmentAllocator
   {
public:

   explicit SegmentAllocator(::TR::RawAllocator allocator) :
      _rawAllocator(allocator)
      {
      TR_ASSERT(((pageSize() & (pageSize() - 1)) == 0), "Page size is not a power of 2, %llu", static_cast<unsigned long long>(pageSize()) );
      }

   SegmentAllocator(const SegmentAllocator &other) :
      _rawAllocator(other._rawAllocator)
      {
      TR_ASSERT(((pageSize() & (pageSize() - 1)) == 0), "Page size is not a power of 2, %llu", static_cast<unsigned long long>(pageSize()) );
      }

   friend bool operator ==(const SegmentAllocator &left, const SegmentAllocator &right)
      {
      return left._rawAllocator == right._rawAllocator;
      }

   friend bool operator !=(const SegmentAllocator &left, const SegmentAllocator &right)
      {
      return !(operator ==(left, right));
      }

   TR::MemorySegment allocate(size_t const segmentSize)
      {
      size_t const alignedSize = pageAlign(segmentSize);
      return TR::MemorySegment(
         _rawAllocator.allocate(alignedSize),
         alignedSize
         );
      }

   void deallocate(TR::MemorySegment unusedSegment) throw()
      {
      _rawAllocator.deallocate(unusedSegment.rawSegment());
      }

   /**
    * @brief pageSize determines the appropriate page size for which to align segment alloctions
    * @return appropriate page size for alignment (CURRENTLY HARDCODED TO 4KiB)
    */
   size_t const pageSize() { return 1 << 12; }
   size_t const pageAlign(size_t requestedSize)
      {
      return (requestedSize + pageSize() - 1) & ~(pageSize() - 1);
      }

private:
   TR::RawAllocator _rawAllocator;
   };

}

#endif // OMR_SEGMENT_ALLOCATOR_HPP

/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#ifndef OMR_DEBUG_SEGMENT_PROVIDER
#define OMR_DEBUG_SEGMENT_PROVIDER

#pragma once

#include <stddef.h>
#include <set>
#include "env/TypedAllocator.hpp"
#include "infra/ReferenceWrapper.hpp"
#include "env/SegmentAllocator.hpp"
#include "env/RawAllocator.hpp"

namespace TR {

/** @class DebugSegmentProvider
 *  @brief The DebugSegmentProvider class provides a facility for verifying the
 *  correctness of compiler scratch memory use.
 *
 *  Using the native facilities available on each platform, a DebugSegmentProvider
 *  provides an alternative allocation mechanism for the TR::MemorySegments used
 *  by the compiler's scratch memory regions.  Instead of releasing virtual address
 *  segments back to the operating system, this implementation instead either
 *  remaps the segment, freeing the underlying physical pages, and changing the
 *  memory protection to trap on any access [preferred], or, if such facilities
 *  are not available, paints the memory segment with a value that should cause
 *  pointer dereferences to be unaligned, and resolve to the high-half of the
 *  virtual address space often reserved for kernel / supervisor use.
 **/
class DebugSegmentProvider : public TR::SegmentAllocator
   {
public:
   DebugSegmentProvider(size_t defaultSegmentSize, TR::RawAllocator rawAllocator);
   ~DebugSegmentProvider() throw();
   virtual TR::MemorySegment &request(size_t requiredSize);
   virtual void release(TR::MemorySegment &segment) throw();
   virtual size_t bytesAllocated() const throw();
   virtual size_t regionBytesAllocated() const throw();
   virtual size_t systemBytesAllocated() const throw();
   virtual size_t allocationLimit() const throw();
   virtual void setAllocationLimit(size_t);

private:
   TR::RawAllocator _rawAllocator;
   size_t _bytesAllocated;
   typedef TR::typed_allocator<
      TR::MemorySegment,
      TR::RawAllocator
      > SegmentSetAllocator;

   std::set<
      TR::MemorySegment,
      std::less< TR::MemorySegment >,
      SegmentSetAllocator
      > _segments;

   };

} // namespace TR

#endif // OMR_DEBUG_SEGMENT_PROVIDER

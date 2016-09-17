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

#include "env/SystemSegmentProvider.hpp"
#include "env/MemorySegment.hpp"

OMR::SystemSegmentProvider::SystemSegmentProvider(size_t segmentSize, TR::RawAllocator rawAllocator) :
   TR::SegmentProvider(segmentSize),
   _rawAllocator(rawAllocator),
   _segments(std::less< TR::MemorySegment >(), SegmentSetAllocator(rawAllocator))
   {
   }

OMR::SystemSegmentProvider::~SystemSegmentProvider() throw()
   {
   }

TR::MemorySegment &
OMR::SystemSegmentProvider::request(size_t requiredSize)
   {
   size_t adjustedSize = ( ( requiredSize + (defaultSegmentSize() - 1) ) / defaultSegmentSize() ) * defaultSegmentSize();
   void *newSegmentArea = _rawAllocator.allocate(adjustedSize);
   try
      {
      auto result = _segments.insert( TR::MemorySegment(newSegmentArea, adjustedSize) );
      TR_ASSERT(result.first != _segments.end(), "Bad iterator");
      TR_ASSERT(result.second, "Insertion failed");
      _bytesAllocated += adjustedSize;
      return const_cast<TR::MemorySegment &>(*(result.first));
      }
   catch (...)
      {
      _rawAllocator.deallocate(newSegmentArea);
      throw;
      }
   }

void
OMR::SystemSegmentProvider::release(TR::MemorySegment &segment) throw()
   {
   auto it = _segments.find(segment);
   _rawAllocator.deallocate(segment.base());
   _bytesAllocated -= segment.size();
   TR_ASSERT(it != _segments.end(), "Segment lookup should never fail");
   _segments.erase(it);
   }

size_t
OMR::SystemSegmentProvider::bytesAllocated() const throw()
   {
   return _bytesAllocated;
   }

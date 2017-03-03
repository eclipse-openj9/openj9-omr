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

#include "env/MemorySegment.hpp"
#include "env/SegmentProvider.hpp"
#include "env/Region.hpp"
#include "infra/ReferenceWrapper.hpp"
#include "env/TRMemory.hpp"

namespace TR {

Region::Region(TR::SegmentProvider &segmentProvider, TR::RawAllocator rawAllocator) :
   _segmentProvider(segmentProvider),
   _rawAllocator(rawAllocator),
   _initialSegment(_initialSegmentArea.data, INITIAL_SEGMENT_SIZE),
   _currentSegment(TR::ref(_initialSegment)),
   _lastDestructable(NULL)
   {
   }

Region::Region(const Region &prototype) :
   _segmentProvider(prototype._segmentProvider),
   _rawAllocator(prototype._rawAllocator),
   _initialSegment(_initialSegmentArea.data, INITIAL_SEGMENT_SIZE),
   _currentSegment(TR::ref(_initialSegment)),
   _lastDestructable(NULL)
   {
   }

Region::~Region() throw()
   {
   /*
    * Destroy all object instances that depend on the region
    * to manage their lifetimes.
    */
   Destructable *lastDestructable = _lastDestructable;
   while (lastDestructable)
      {
      Destructable * const currentDestructable = lastDestructable;
      lastDestructable = currentDestructable->prev();
      currentDestructable->~Destructable();
      }

   for (
      TR::reference_wrapper<TR::MemorySegment> latestSegment(_currentSegment);
      latestSegment.get() != _initialSegment;
      latestSegment = _currentSegment
      )
      {
      _currentSegment = TR::ref(latestSegment.get().unlink());
      _segmentProvider.release(latestSegment);
      }
   TR_ASSERT(_currentSegment.get() == _initialSegment, "self-referencial link was broken");
   }

void *
Region::allocate(size_t const size, void *hint)
   {
   size_t const roundedSize = round(size);
   if (_currentSegment.get().remaining() >= roundedSize)
      {
      return _currentSegment.get().allocate(roundedSize);
      }
   TR::MemorySegment &newSegment = _segmentProvider.request(roundedSize);
   TR_ASSERT(newSegment.remaining() >= roundedSize, "Allocated segment is too small");
   newSegment.link(_currentSegment.get());
   _currentSegment = TR::ref(newSegment);
   return _currentSegment.get().allocate(roundedSize);
   }

void
Region::deallocate(void * allocation, size_t) throw()
   {
   }

size_t
Region::round(size_t bytes)
   {
   return (bytes+15) & (~15);
   }
}

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

#include "env/SegmentPool.hpp"
#include "env/MemorySegment.hpp"

TR::SegmentPool::SegmentPool(TR::SegmentProvider &backingProvider, size_t poolSize, TR::RawAllocator rawAllocator) :
   SegmentProvider(backingProvider.defaultSegmentSize()),
   _poolSize(poolSize),
   _storedSegments(0),
   _backingProvider(backingProvider),
   _segmentStack(StackContainer(DequeAllocator(rawAllocator)))
   {
   }

TR::SegmentPool::~SegmentPool() throw()
   {
   while (!_segmentStack.empty())
      {
      TR_ASSERT(_storedSegments > 0, "Too many segments");
      TR::MemorySegment &topSegment = _segmentStack.top().get();
      _segmentStack.pop();
      _backingProvider.release(topSegment);
      --_storedSegments;
      }
   TR_ASSERT(0 == _storedSegments, "Lost a segment");
   }

TR::MemorySegment &
TR::SegmentPool::request(size_t requiredSize)
   {
   if (
      requiredSize <= defaultSegmentSize()
      && !_segmentStack.empty()
      )
      {
      --_storedSegments;
      TR_ASSERT(0 <= _storedSegments, "We lost a segment");
      TR::MemorySegment &recycledSegment = _segmentStack.top().get();
      _segmentStack.pop();
      recycledSegment.reset();
      return recycledSegment;
      }
   return _backingProvider.request(requiredSize);
   }

void
TR::SegmentPool::release(TR::MemorySegment &segment) throw()
   {
   if (
      segment.size() == _defaultSegmentSize
      && _storedSegments < _poolSize
      )
      {
      try
         {
         _segmentStack.push(TR::ref(segment));
         ++_storedSegments;
         }
      catch (...)
         {
         _backingProvider.release(segment);
         }
      }
   else
      {
      _backingProvider.release(segment);
      }
   }

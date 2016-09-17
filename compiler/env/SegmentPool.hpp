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

#ifndef TR_SEGMENT_POOL
#define TR_SEGMENT_POOL

#pragma once

#include <deque>
#include <stack>
#include "env/TypedAllocator.hpp"
#include "infra/ReferenceWrapper.hpp"
#include "env/SegmentProvider.hpp"
#include "env/RawAllocator.hpp"

namespace TR {

/**
 * @brief The SegmentPool class maintains a pool of memory segments.
 */

class SegmentPool : public TR::SegmentProvider
   {
public:
   SegmentPool(TR::SegmentProvider &backingProvider, size_t cacheSize, TR::RawAllocator rawAllocator);
   ~SegmentPool() throw();

   virtual TR::MemorySegment &request(size_t requiredSize);
   virtual void release(TR::MemorySegment &) throw();

private:
   size_t const _poolSize;
   size_t _storedSegments;
   TR::SegmentProvider &_backingProvider;

   typedef TR::typed_allocator<
      TR::reference_wrapper<TR::MemorySegment>,
      TR::RawAllocator
      > DequeAllocator;

   typedef std::deque<
      TR::reference_wrapper<TR::MemorySegment>,
      DequeAllocator
      > StackContainer;

   typedef std::stack<
      TR::reference_wrapper<TR::MemorySegment>,
      StackContainer
      > SegmentStack;

   SegmentStack _segmentStack;

   };

}

#endif // TR_SEGMENT_POOL

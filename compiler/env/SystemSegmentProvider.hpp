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

#ifndef OMR_SYSTEM_SEGMENT_PROVIDER
#define OMR_SYSTEM_SEGMENT_PROVIDER

#pragma once

#ifndef TR_SYSTEM_SEGMENT_PROVIDER
#define TR_SYSTEM_SEGMENT_PROVIDER
namespace OMR { class SystemSegmentProvider; }
namespace TR { using OMR::SystemSegmentProvider; }
#endif

#include <stddef.h>
#include <set>
#include "env/TypedAllocator.hpp"
#include "infra/ReferenceWrapper.hpp"
#include "env/SegmentProvider.hpp"
#include "env/RawAllocator.hpp"

namespace OMR {

class SystemSegmentProvider : public TR::SegmentProvider
   {
public:
   SystemSegmentProvider(size_t segmentSize, TR::RawAllocator rawAllocator);
   ~SystemSegmentProvider() throw();
   virtual TR::MemorySegment &request(size_t requiredSize);
   virtual void release(TR::MemorySegment &segment) throw();
   size_t bytesAllocated() const throw();

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

#endif // OMR_SYSTEM_SEGMENT_PROVIDER

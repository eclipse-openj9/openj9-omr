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

#ifndef OMR_REGION_HPP
#define OMR_REGION_HPP

#pragma once

#include <stddef.h>
#include <deque>
#include <stack>
#include "infra/ReferenceWrapper.hpp"
#include "env/TypedAllocator.hpp"
#include "env/MemorySegment.hpp"
#include "env/RawAllocator.hpp"

namespace TR {

class SegmentProvider;

class Region
   {
public:
   Region(TR::SegmentProvider &segmentProvider, TR::RawAllocator rawAllocator);
   Region(const Region &prototype);
   virtual ~Region() throw();
   void * allocate(const size_t bytes, void * hint = 0);
   void deallocate(void * allocation, size_t = 0) throw();

   friend bool operator ==(const TR::Region &lhs, const TR::Region &rhs)
      {
      return &lhs == &rhs;
      }

   friend bool operator !=(const TR::Region &lhs, const TR::Region &rhs)
      {
      return !operator ==(lhs, rhs);
      }

   // Enable automatic conversion into a form compatible with C++ standard library containers
   template<typename T> operator TR::typed_allocator<T, Region& >()
      {
      return TR::typed_allocator<T, Region& >(*this);
      }

private:
   size_t round(size_t bytes);

   TR::SegmentProvider &_segmentProvider;
   TR::RawAllocator _rawAllocator;
   TR::MemorySegment _initialSegment;
   TR::reference_wrapper<TR::MemorySegment> _currentSegment;

   static const size_t INITIAL_SEGMENT_SIZE = 4096;

   union {
      char data[INITIAL_SEGMENT_SIZE];
      unsigned long long alignment;
      } _initialSegmentArea;
};

}

inline void * operator new(size_t size, TR::Region &region) { return region.allocate(size); }
inline void operator delete(void * p, TR::Region &region) { region.deallocate(p); }

inline void * operator new[](size_t size, TR::Region &region) { return region.allocate(size); }
inline void operator delete[](void * p, TR::Region &region) { region.deallocate(p); }

#endif // OMR_REGION_HPP

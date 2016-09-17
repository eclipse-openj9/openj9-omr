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

#ifndef OMR_RAW_ALLOCATOR_HPP
#define OMR_RAW_ALLOCATOR_HPP

#pragma once

#ifndef TR_RAW_ALLOCATOR
#define TR_RAW_ALLOCATOR
namespace OMR { class RawAllocator; }
namespace TR { typedef OMR::RawAllocator RawAllocator; }
#endif

#include <stddef.h>  // for size_t
#include <cstdlib>   // for free, malloc
#include <new>       // for bad_alloc, nothrow, nothrow_t

namespace OMR {

class RawAllocator
   {
public:
   RawAllocator()
      {
      }

   RawAllocator(const RawAllocator &other)
      {
      }

   void *allocate(size_t size, const std::nothrow_t tag, void * hint = 0) throw()
      {
      return malloc(size);
      }

   void * allocate(size_t size, void * hint = 0)
      {
      void * const alloc = allocate(size, std::nothrow, hint);
      if (!alloc) throw std::bad_alloc();
      return alloc;
      }

   void deallocate(void * p) throw()
      {
      free(p);
      }

   void deallocate(void * p, const size_t size) throw()
      {
      free(p);
      }

   friend bool operator ==(const RawAllocator &left, const RawAllocator &right)
      {
      return true;
      }

   friend bool operator !=(const RawAllocator &left, const RawAllocator &right)
      {
      return !operator ==(left, right);
      }

   };

}

inline void * operator new(size_t size, OMR::RawAllocator allocator)
   {
   return allocator.allocate(size);
   }

inline void operator delete(void *ptr, OMR::RawAllocator allocator) throw()
   {
   allocator.deallocate(ptr);
   }

inline void * operator new[](size_t size, OMR::RawAllocator allocator)
   {
   return allocator.allocate(size);
   }

inline void operator delete[](void *ptr, OMR::RawAllocator allocator) throw()
   {
   allocator.deallocate(ptr);
   }

inline void * operator new(size_t size, OMR::RawAllocator allocator, const std::nothrow_t& tag) throw()
   {
   return allocator.allocate(size, tag);
   }

inline void * operator new[](size_t size, OMR::RawAllocator allocator, const std::nothrow_t& tag) throw()
   {
   return allocator.allocate(size, tag);
   }

#endif // OMR_RAW_ALLOCATOR_HPP

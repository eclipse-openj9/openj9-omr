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

#ifndef OMR_PERSISTENT_ALLOCATOR
#define OMR_PERSISTENT_ALLOCATOR

#pragma once

#ifndef TR_PERSISTENT_ALLOCATOR
#define TR_PERSISTENT_ALLOCATOR

namespace OMR { class PersistentAllocator; }
namespace TR { using OMR::PersistentAllocator; }

#endif // TR_PERSISTENT_ALLOCATOR

#include <stddef.h>
#include <stdint.h>

#include "env/RawAllocator.hpp"  // for RawAllocator
#include "env/PersistentAllocatorKit.hpp" // for PersistentAllocatorKit

namespace OMR {

class PersistentAllocator
   {
public:
   PersistentAllocator(const TR::PersistentAllocatorKit &allocatorKit);

   void *allocate(size_t size, const std::nothrow_t tag, void * hint = 0) throw();
   void * allocate(size_t size, void * hint = 0);
   void deallocate(void * p, const size_t sizeHint = 0) throw();

   friend bool operator ==(const PersistentAllocator &left, const PersistentAllocator &right)
      {
      return left._rawAllocator == right._rawAllocator;
      }

   friend bool operator !=(const PersistentAllocator &left, const PersistentAllocator &right)
      {
      return !operator ==(left, right);
      }

private:
   PersistentAllocator(const PersistentAllocator &);

   TR::RawAllocator _rawAllocator;

   };

}

inline void * operator new(size_t size, OMR::PersistentAllocator &allocator)
   {
   return allocator.allocate(size);
   }

inline void * operator new(size_t size, OMR::PersistentAllocator &allocator, const std::nothrow_t& tag) throw()
   {
   return allocator.allocate(size, tag);
   }

inline void * operator new[](size_t size, OMR::PersistentAllocator &allocator)
   {
   return operator new(size, allocator);
   }

inline void * operator new[](size_t size, OMR::PersistentAllocator &allocator, const std::nothrow_t& tag) throw()
   {
   return operator new(size, allocator, tag);
   }

inline void operator delete(void *ptr, OMR::PersistentAllocator &allocator) throw()
   {
   allocator.deallocate(ptr);
   }

inline void operator delete(void *ptr, OMR::PersistentAllocator &allocator, const std::nothrow_t& tag) throw()
   {
   allocator.deallocate(ptr);
   }

inline void operator delete[](void *ptr, OMR::PersistentAllocator &allocator) throw()
   {
   operator delete(ptr, allocator);
   }

inline void operator delete[](void *ptr, OMR::PersistentAllocator &allocator, const std::nothrow_t& tag) throw()
   {
   operator delete(ptr, allocator, tag);
   }

#endif // OMR_PERSISTENT_ALLOCATOR


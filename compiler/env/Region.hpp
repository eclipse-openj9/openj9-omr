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

   /**
    * @brief The Destructable class provides a common base class, providing an
    * interface or invoking the destructor of allocated objects of arbitrary type.
    */
   class Destructable
      {
   public:
      Destructable(Destructable * const prev) :
         _prev(prev)
         {
         }

      Destructable * prev() { return _prev; }

      virtual ~Destructable() throw() {}

   private:
      Destructable * const _prev;
      };

   /**
    * @brief A class template allowing the creation of allocation instances of arbitrary
    * type, all of which have a predictable means of accessing their destructor.
    */
   template <typename T>
   class Instance : public Destructable
      {
   public:
      Instance(Destructable *prev, const T& value) :
         Destructable(prev),
         _instance(value)
         {
         }

      ~Instance() throw()
         {
         }

      T& value() { return _instance; }

   private:
      T _instance;
      };

public:
   Region(TR::SegmentProvider &segmentProvider, TR::RawAllocator rawAllocator);
   Region(const Region &prototype);
   virtual ~Region() throw();
   void * allocate(const size_t bytes, void * hint = 0);

   /**
    * @brief A function template to allocate a Region-managed object instance.
    *
    * @param[in] prototype A copy-constructible instance used to initialize the Region-managed instance.
    *
    * Object instances allocated in this form will be destroyed in LIFO order when the owning
    * Region is destroyed.\n
    * NOTE: If, using a region R0, a second Region R1 is allocated (directly or indirectly)
    * in this manner, any objects allocated in R0 after the allocation of R1 will have a
    * shorter life span than all the objects allocated in R1:\n
    * \n
    * Objects Allocated in R0 before R1 > Objects allocated in R1 > Objects allocated in R0 after R1\n
    * \n
    * This is most likely going to be invoked using a temporary:\n
    * ```region.allocate( MyClass(arg0, ...argN) );```\n
    * Or, if using the default constructor, use extra parentheses to avoid the 'most vexing parse':\n
    * ```region.allocate( (MyClass()) );```
    */
   template <typename T> inline T& allocate(const T& prototype);

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

   Destructable *_lastDestructable;

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

template <typename T>
T &
TR::Region::allocate(const T &value)
   {
   Instance<T> *instance = new (*this) Instance<T>(_lastDestructable, value);
   _lastDestructable = static_cast<Destructable *>(instance);
   return instance->value();
   }

#endif // OMR_REGION_HPP

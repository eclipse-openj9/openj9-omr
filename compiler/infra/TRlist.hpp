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

#ifndef TRLIST_INCL
#define TRLIST_INCL
#undef max
#undef min
#include <list>
#undef round
#include "env/TypedAllocator.hpp"
#include "env/TRMemory.hpp"  // for TR_Memory, etc
namespace TR
   {
   template <class T> class list : public std::list<T, TR::typed_allocator<T, TR::Allocator> >
      {
      public:
      list(TR::typed_allocator<T, TR::Allocator> ta) :
         std::list<T, TR::typed_allocator<T, TR::Allocator> > (ta)
         {
         }

#if defined(_LIBCPP_VERSION) && _LIBCPP_VERSION < 4000
      /* A bug in libc++ before 4.0 caused std::list::remove to call the
       * default constructor of TR::typed_allocator, which is not implemented.
       */
      void remove(const T& value)
         {
         this->remove_if([value](const T& value2){return value == value2;});
         }
#endif

      };
   }
#endif

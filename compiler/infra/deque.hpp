/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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

#ifndef DEQUE_HPP
#define DEQUE_HPP

#pragma once

#include <deque>
#include "env/TypedAllocator.hpp"
#include "env/TRMemory.hpp"

namespace TR {

template <typename T>
class deque : public std::deque<T, typed_allocator<T, Allocator> >
   {
public:
   typedef typed_allocator<T, Allocator> allocator_type;
   typedef std::deque<T, typed_allocator<T, Allocator> > container_type;
   typedef typename allocator_type::value_type value_type;
   typedef typename allocator_type::reference reference;
   typedef typename allocator_type::const_reference const_reference;
   /*
    * This would ideally use the parent deque's size_type.  However, such usage
    * runs into two-phase lookup problems when compiling with MSVC++ 2010.
    */
   typedef std::size_t size_type;

   explicit deque(const Allocator &allocator);
   explicit deque(size_type size, const Allocator &allocator);
   explicit deque(size_type size, const T& initialValue, const Allocator &allocator);
   template <typename InputIterator> deque(InputIterator first, InputIterator last, const Allocator &allocator);
   deque(const deque<T> &x);
   ~deque();

   reference operator[](size_type index);
   const_reference operator[](size_type index) const;
   };

}

template <typename T>
TR::deque<T>::deque(const Allocator &allocator) :
   container_type(allocator_type(allocator))
   {
   }

template <typename T>
TR::deque<T>::deque(size_type size, const Allocator &allocator) :
   container_type(size, T(), allocator_type(allocator))
   {
   }

template <typename T>
TR::deque<T>::deque(size_type size, const T& initialValue, const Allocator &allocator) :
   container_type(size, initialValue, allocator_type(allocator))
   {
   }

template <typename T>
template <typename InputIterator>
TR::deque<T>::deque(InputIterator first, InputIterator last, const Allocator &allocator) :
   container_type(first, last, allocator_type(allocator))
   {
   }

template <typename T>
TR::deque<T>::deque(const deque<T> &other) :
   container_type(other)
   {
   }

template <typename T>
TR::deque<T>::~deque()
   {
   }

template <typename T>
typename TR::deque<T>::reference
TR::deque<T>::operator [](size_type index)
   {
// In DEBUG, at() is used for correctness due to its bound checking
// whilst [] is used in PROD for performance
#if defined(DEBUG) || defined(PROD_WITH_ASSUMES)
   return this->at(index);
#else
   return container_type::operator[](index);
#endif // defined(DEBUG) || defined(PROD_WITH_ASSUMES)
   }

template <typename T>
typename TR::deque<T>::const_reference
TR::deque<T>::operator [](size_type index) const
   {
// In DEBUG, at() is used for correctness due to its bound checking
// whilst [] is used in PROD for performance
#if defined(DEBUG) || defined(PROD_WITH_ASSUMES)
   return this->at(index);
#else
   return container_type::operator[](index);
#endif // defined(DEBUG) || defined(PROD_WITH_ASSUMES)  
   }

#endif // DEQUE_HPP

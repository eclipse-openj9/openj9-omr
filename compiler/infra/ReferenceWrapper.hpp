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

#ifndef REFERENCE_WRAPPER_HPP
#define REFERENCE_WRAPPER_HPP

#pragma once

/*
 * XL 12 is incompatible with the TR1 headers on RHEL6,
 * and boost isn't currently an option,
 * so, for now, here's yet another wheel implementation.
 */

namespace TR {

template <typename T>
class reference_wrapper {
public:
  typedef T type;
  typedef T& reference_type;
  inline explicit reference_wrapper(reference_type ref) throw();
  inline reference_wrapper(const reference_wrapper &other) throw();
  inline reference_wrapper &operator =(const reference_wrapper &other) throw();
  inline reference_type get() throw();
  inline operator reference_type() throw();

private:
  T * m_ref;
};

template <typename T>
reference_wrapper<T>::reference_wrapper(reference_type ref) throw():
  m_ref(&ref)
{
}

template <typename T>
reference_wrapper<T>::reference_wrapper(const reference_wrapper &other) throw():
  m_ref(other.m_ref)
{
}

template <typename T>
reference_wrapper<T> &
reference_wrapper<T>::operator =(const reference_wrapper &other) throw()
{
  m_ref = other.m_ref;
  return *this;
}

template <typename T>
typename reference_wrapper<T>::reference_type
reference_wrapper<T>::get() throw()
{
  return *m_ref;
}

template <typename T>
reference_wrapper<T>::operator reference_type() throw()
{
  return *m_ref;
}

template <typename T>
reference_wrapper<T>
inline ref(T& reference)
{
  return reference_wrapper<T>(reference);
}

template <typename T>
reference_wrapper<T>
inline ref(reference_wrapper<T> wrapper)
{
  return wrapper;
}

template <typename T>
reference_wrapper<const T>
inline cref(const T& reference)
{
  return reference_wrapper<const T>(reference);
}

template <typename T>
reference_wrapper<const T>
inline cref(reference_wrapper<T> wrapper)
{
  return wrapper;
}

}

#endif // REFERENCE_WRAPPER_HPP


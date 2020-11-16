/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef OMR_CCDATA_INLINES_INCL
#define OMR_CCDATA_INLINES_INCL

#include <type_traits>

namespace TR
{

template <typename T>
CCData::key_t CCData::key(const T value)
   {
#if __cpp_static_assert && __cpp_lib_has_unique_object_representations
   // std::has_unique_object_representations is only available in C++17.
   static_assert(std::has_unique_object_representations<T>::value == true, "T must have unique object representations.");
#endif
   return key(&value, sizeof(value));
   }

template <typename T>
bool CCData::put(const T value, const key_t * const key, index_t &index)
   {
   // std::is_trivially_copyable is a C++11 type trait, but unfortunately there's no test macro for it.
   // static_assert is also a C++11 feature, so testing for that would hopefully be enough, but unfortunately some old compilers have static_assert but not is_trivially_copyable,
   // hence `#if __cpp_static_assert` is insufficient.
//#if __cpp_static_assert
//   static_assert(std::is_trivially_copyable<T>::value == true, "T must be trivially copyable.");
//#endif
   return put(&value, sizeof(value), alignof(value), key, index);
   }
   }

template <typename T>
bool CCData::get(const index_t index, T &value) const
   {
   // See above.
//#if __cpp_static_assert
//   static_assert(std::is_trivially_copyable<T>::value == true, "T must be trivially copyable.");
//#endif
   return get(index, &value, sizeof(value));
   }

template <typename T>
T* CCData::get(const index_t index) const
   {
   // Don't have to check if T is trivially_copyable here since we're not copying to/from a T.
   // The caller might, but it is then their responsibility to make sure.
   if (index >= _capacity)
      return NULL;

   return reinterpret_cast<T *>(_data + index);
   }

}

#endif // OMR_CCDATA_INLINES_INCL

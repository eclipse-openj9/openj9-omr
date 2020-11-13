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

#ifndef OMR_CCDATA_INCL
#define OMR_CCDATA_INCL

#include <map>
#include <string>
#include <cstddef>

namespace TR { class Monitor; }

namespace TR
{

/**
 * @class CCData
 *
 * @brief This class represents a table that can be used to implement a GOT or TOC or a PLT or similar.
 *
 * <TODO: A more detailed discussion about the design here.>
 */
class CCData
   {
   private:
      /** \typedef data_t Implementation detail. This type represents the units of the table. Typically bytes, but can be some other data type, as long as it can be default-constructed. */
      typedef uint8_t data_t;

   public:
      /** \typedef index_t This type represents the indices defined in the public interface of this class. They must behave like integral types. */
      typedef size_t index_t;

      /** \typedef key_t This type represents the keys defined in the public interface of this class. The constructor is unspecified, use CCData_t::key() to create keys. */
      typedef std::string key_t;

#if __cpp_static_assert
      static_assert(sizeof(key_t::value_type) == 1, "Unimplemented key unit size, remaining bytes need to be zeroed to support key units >1.");
#endif

   private:
      /** \typedef map_t Implementation detail. This type represents the associative container that maps keys to indices. It must behave like std::unordered_map. */
      typedef std::map<key_t, index_t> map_t;

   public:
      /**
       * @brief Creates a key_t from the given value. The value's type must have a unique object representation.
       *
       * Unique Object Representations
       *
       * Structs and classes usually have padding bytes which are going to be included in their memory representations.
       * Even if the padding is at the end of the instance, sizeof() will include it. Calculating a key based on the memory
       * representation is going to include the padding, which is incorrect. If the key includes the padding, two otherwise
       * equal objects will result in different keys if their padding bytes differ.
       *
       * This also applies to floats and doubles because of various "don't care" (DC) bits in the binary representation.
       * Logically equal float values might have different bit patterns and would result in different keys.
       *
       * Types that don't have any padding or DC bits have unique object representations in C++ standard parlance.
       *
       * @tparam T The type of the value to create the key from. The type must have a unique object representation. A static assertion checks for the std::has_unique_object_representations<T> type trait on capable compilers.
       * @param[In] value The value to create the key from.
       * @return The key.
       */
      template <typename T>
      static key_t key(const T value);

      /**
       * @brief Creates a key_t from the given buffer of data. The entire buffer will be used as input, including any unused bits/bytes that you may not be aware of. See key(const T value) for info on why this is important to note.
       *
       * @param[In] data A pointer to the buffer of data.
       * @param[In] sizeBytes The size (in bytes) of the data.
       * @return The key.
       */
      static key_t key(const uint8_t * const data, const size_t sizeBytes);

      /**
       * @brief Creates a key_t from the given null-terminated C string.

       * @param[In] data A pointer to the string.
       * @return The key.
       * \TODO: Decide if we need this; useful for string literals, but susceptible to bugs/attack because of the unbounded string.
       */
      static key_t key(const char * const str);

      /**
       * @brief Constructs a CCData.

       * @param[In] sizeBytes The amount of data (in bytes) that CCData can hold.
       */
      CCData(const size_t sizeBytes);

      /**
       * @brief Constructs a CCData and accepts a pointer to a memory buffer that will be used to hold the data.
       *
       * The lifetime of the memory buffer must exceed the lifetime of the resulting CCData object. The memory buffer will still be valid after the CCData object is destructed (i.e. the memory buffer will not be freed, do it yourself).
       *
       * @param[In] storage A pointer to a memory buffer that CCData will use.
       * @param[In] sizeBytes The amount of data (in bytes) that the buffer contains.
       */
      CCData(uint8_t * const storage, const size_t sizeBytes);

      ~CCData();

      /**
       * @brief Puts the given value in the table, optionally mapped to the given key (if any), aligned to the value's natural type, and returns the index to the value. Synchronized.

       * @tparam T The type of the value to put in the table. The type must be TriviallyCopyable. A static assertion checks for the std::is_trivially_copyable<T> type trait on capable compilers.
       * @param[In] value The value to put.
       * @param[In] key Optional. The key to map the resulting index to. Without a key the index is the only reference to the data.
       * @param[Out] index The index that refers to the value.
       */
      template <typename T>
      bool put(const T value, const key_t * const key, index_t &index);

      /**
       * @brief Puts the given value in the table, optionally mapped to the given key (if any), aligned to the given boundary (in bytes), and returns the index to the value. Synchronized.

       * @param[In] value Optional. A pointer to the value to put. If null, no data will be copied but the space will be allocated none the less.
       * @param[In] sizeBytes The size of the value pointed to.
       * @param[In] alignmentBytes The alignment (in bytes) to align the value to.
       * @param[In] key Optional. The key to map the resulting index to. Without a key the index is the only reference to the data. If the key is already mapped to an index the operation will return the index and true, but no data will be written.
       * @param[Out] index The index that refers to the value.
       * @return True if the value was placed in the table, or the key was already mapped to an index, false otherwise.
       */
      bool put(const uint8_t * const value, const size_t sizeBytes, const size_t alignmentBytes, const key_t * const key, index_t &index);

      /**
       * @brief Gets the value referred to by the index from the table.

       * @param[In] T The type of the value to get from the table. The type must be TriviallyCopyable. A static assertion checks for the std::is_trivially_copyable<T> type trait on capable compilers.
       * @param[In] index The index that refers to the value.
       * @param[out] value A reference to the value to write the result to.
       * @return True if the value was placed in the table, false otherwise.
       */
      template <typename T>
      bool get(const index_t index, T &value) const;

      /**
       * @brief Gets the value referred to by the index from the table.

       * @param[In] index The index that refers to the value.
       * @param[in] value A pointer to the value to write the result to. This parameter is ignored unless this function returns true.
       * @param[In] sizeBytes The size (in bytes) of the value pointed to. This parameter is ignored unless this function returns true.
       * @return True if the index refers to an existing value, false otherwise.
       */
      bool get(const index_t index, uint8_t * const value, const size_t sizeBytes) const;

      /**
       * @brief Gets a pointer to the value referred to by the index from the table.

       * @param[In] T The type of the value to get from the table. The type need not be TriviallyCopyable, since this function doesn't do any copying, but it probably should be for symmetry with the put() functions.
       * @param[In] index The index that refers to the value.
       * @return A pointer to the value if the index refers to an existing value, NULL otherwise.
       */
      template <typename T>
      T* get(const index_t index) const;

      /**
       * @brief Checks if the given key maps to an index in this table and returns the index. Synchronized.

       * @param[In] key The key to check.
       * @param[out] index Optional. A pointer to write the index to. This parameter is ignored unless this function returns true.
       * @return True if the given key maps to an index, false otherwise.
       */
      bool find(const key_t key, index_t * const index = NULL) const;

   private:
      /**
       * @brief Checks if the given key maps to an index in this table and returns the index.
       *        This function is NOT synchronized (hence unsafe).
       *
       * @param[In] key The key to check.
       * @param[out] index Optional. A pointer to write the index to. This parameter is ignored unless this function returns true.
       * @return True if the given key maps to an index, false otherwise.
       */
      bool find_unsafe(const key_t key, index_t * const index = NULL) const;

      /**
       * @brief Converts a size in units of bytes to a size in units of data_t.
       *
       * @param[In] sizeBytes A size in bytes.
       * @return A size in units of data_t.
       */
      static size_t dataSizeFromBytesSize(size_t sizeBytes);

      /**
       * @brief Converts an alignment in units of bytes to an alignment in units of data_t.
       *
       * @param[In] alignmentBytes An alignment in bytes.
       * @return An alignment in units of data_t.
       */
      static size_t dataAlignmentFromBytesAlignment(size_t alignmentBytes);

   private:
      data_t           *_data;
      size_t            _capacity;
      size_t            _putIndex;
      map_t             _mappings;
      TR::Monitor      *_lock;
      const bool        _releaseData;
   };

}

#endif // OMR_CCDATA_INCL

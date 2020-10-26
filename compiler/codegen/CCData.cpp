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

#include "codegen/CCData.hpp"

#include "infra/Monitor.hpp"
#include "infra/CriticalSection.hpp"

using OMR::CCData;

CCData::key_t CCData::key(const uint8_t * const data, const size_t sizeBytes)
   {
   return key_t(reinterpret_cast<const key_t::value_type *>(data), sizeBytes);
   }

CCData::key_t CCData::key(const char * const str)
   {
   return key_t(reinterpret_cast<const key_t::value_type *>(str));
   }

// Converts a size in units of bytes to a size in units of sizeof(data_t).
#define DATA_SIZE_FROM_BYTES_SIZE(x) (((x) + (sizeof(data_t)) - 1) / (sizeof(data_t)))

// Converts an alignment in units of bytes to an alignment in units of alignof(data_t).
#define DATA_ALIGNMENT_FROM_BYTES_ALIGNMENT(x) (((x) + (alignof(data_t)) - 1) / (alignof(data_t)))

CCData::CCData(const size_t sizeBytes)
: _data(new data_t[DATA_SIZE_FROM_BYTES_SIZE(sizeBytes)]), _capacity(DATA_SIZE_FROM_BYTES_SIZE(sizeBytes)), _putIndex(0), _lock(TR::Monitor::create("CCDataMutex")), _releaseData(false)
   {
   }

CCData::CCData(uint8_t * const storage, const size_t sizeBytes)
: CCData(alignStorage(storage, sizeBytes))
   {
   }

CCData::CCData(const storage_and_size_pair_t storageAndSize)
: _data(storageAndSize.first), _capacity(storageAndSize.second), _putIndex(0), _lock(TR::Monitor::create("CCDataMutex")), _releaseData(true)
   {
   }

CCData::~CCData()
   {
   // Memory for data can either be allocated by this class or passed in via
   // the constructor. If allocated, it has to be freed now; if passed in we can't free
   // it.
   // The _data member var is a smart pointer that will automatically
   // free the underlying memory when it goes out of scope. If memory was passed in
   // via the constructor we have to release it now to prevent the smart pointer from
   // freeing it.
   if (_releaseData)
      _data.release();
   }

const CCData::storage_and_size_pair_t CCData::alignStorage(uint8_t * const storage, size_t sizeBytes)
   {
   // This function takes buffer, aligns it for data_t, and converts the size in bytes to the size in units of data_t.
   // It returns the aligned pointer and adjusted size in an std::pair so that we can use it when calling a constructor in an initializer list.
   void *alignedStorage = storage;
   std::align(alignof(data_t), sizeof(data_t), alignedStorage, sizeBytes);
   return std::make_pair(reinterpret_cast<data_t *>(alignedStorage), DATA_SIZE_FROM_BYTES_SIZE(sizeBytes));
   }

bool CCData::put(const uint8_t * const value, const size_t sizeBytes, const size_t alignmentBytes, const key_t * const key, index_t &index)
   {
   const OMR::CriticalSection cs(_lock);

   /**
    * Multiple compilation threads may be attempting to update the same value with
    * the same key.  Once we have the mutex, check if the value already exists and
    * return successfully.
    */
   if (key && find_unsafe(*key, &index))
      {
      return true;
      }

   // The following feels like it could be simplified and calculated more efficiently. If you're reading this, have a go at it.
   const size_t sizeDataUnits = DATA_SIZE_FROM_BYTES_SIZE(sizeBytes);
   const size_t alignmentDataUnits = DATA_ALIGNMENT_FROM_BYTES_ALIGNMENT(alignmentBytes);
   const size_t alignmentMask = alignmentDataUnits - 1;
   const size_t alignmentPadding = (alignmentDataUnits - ((reinterpret_cast<uintptr_t>(_data.get() + _putIndex) / alignof(data_t)) & alignmentMask)) & alignmentMask;
   const size_t remainingCapacity = _capacity - _putIndex;

   if (sizeDataUnits + alignmentPadding > remainingCapacity)
      return false;

   _putIndex += alignmentPadding;
   index = _putIndex;

   if (key != nullptr)
      _mappings[*key] = _putIndex;

   if (value != nullptr)
      {
      std::copy(value,
                value + sizeBytes,
                reinterpret_cast<uint8_t *>(_data.get() + _putIndex));
      }

   _putIndex += sizeDataUnits;

   return true;
   }

bool CCData::get(const index_t index, uint8_t * const value, const size_t sizeBytes) const
   {
   if (index >= _capacity)
      return false;

   std::copy(reinterpret_cast<const uint8_t *>(_data.get() + index),
             reinterpret_cast<const uint8_t *>(_data.get() + index) + sizeBytes,
             value);

   return true;
   }

bool CCData::find(const key_t key, index_t * const index) const
   {
   const OMR::CriticalSection cs(_lock);
   return find_unsafe(key, index);
   }

bool CCData::find_unsafe(const key_t key, index_t * const index) const
   {
   auto e = _mappings.find(key);
   if (e != _mappings.cend())
      {
      if (index != nullptr)
         *index = e->second;
      return true;
      }

   return false;
   }

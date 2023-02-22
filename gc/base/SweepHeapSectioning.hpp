/*******************************************************************************
 * Copyright (c) 1991, 2023 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/


#if !defined(SWEEPSCHEMESECTIONING_HPP_)
#define SWEEPSCHEMESECTIONING_HPP_

#include "BaseVirtual.hpp"

#include "GCExtensionsBase.hpp"
#include "EnvironmentBase.hpp"

class MM_ParallelSweepChunk;
class MM_ParallelSweepChunkArray;
class MM_HeapRegionDescriptor;

/**
 * Support for sectioning the heap into chunks useable by sweep (and compact).
 * 
 * @ingroup GC_Base_Core
 */
class MM_SweepHeapSectioning : public MM_BaseVirtual {
private:
protected:
	uintptr_t _totalSize; /**< total elements available across all arrays */

	MM_ParallelSweepChunkArray* _head; /**< Head pointer to the list of chunk arrays (pointer to the base array). */
	MM_ParallelSweepChunkArray* _tail; /**< Tail pointer to the list of chunk arrays. */
	MM_GCExtensionsBase* _extensions;

	virtual bool initialize(MM_EnvironmentBase* env);
	void tearDown(MM_EnvironmentBase* env);

	virtual uintptr_t estimateTotalChunkCount(MM_EnvironmentBase* env);
	virtual uintptr_t calculateActualChunkNumbers() const = 0;
	virtual bool isReadyToSweep(MM_EnvironmentBase* env, MM_HeapRegionDescriptor* region) { return false; }

	bool initArrays(uintptr_t);

	/**
	 * If the chunk size is not set, then set it heuristically
	 * based on the max heap size and thread count.
	 * @param[in] env the current environment.
	 * @return void
	 */
	void initializeChunkSize(MM_EnvironmentBase* env);

	friend class MM_SweepHeapSectioningIterator;

public:
	virtual void kill(MM_EnvironmentBase* env);

	bool update(MM_EnvironmentBase* env);
	virtual uintptr_t reassignChunks(MM_EnvironmentBase* env);

	void* getBackingStoreAddress();
	uintptr_t getBackingStoreSize();


#if defined(J9VM_OPT_CRIU_SUPPORT)
	/**
	 * Update the sectioning data (sweep chuck size and pool)
	 * to reflect the adjusted thread count at restore.
	 * @param[in] env the current environment.
	 * @return boolean indicating if the chunk size and pool were
	 * successfully updated to accommodate the new thread count.
	 */
	bool reinitializeForRestore(MM_EnvironmentBase *env);
#endif /* defined(J9VM_OPT_CRIU_SUPPORT) */

	MM_SweepHeapSectioning(MM_EnvironmentBase* env)
		: _totalSize(0)
		, _head(NULL)
		, _tail(NULL)
		, _extensions(env->getExtensions())
	{
		_typeId = __FUNCTION__;
	}
};

/**
 * Iterator to traverse all chunks in the heap built by heap sectioning.
 * 
 * @ingroup GC_Base
 */
class MM_SweepHeapSectioningIterator {
private:
	MM_ParallelSweepChunkArray* _currentArray; /**< Current chunk array being traversed */
	uint32_t _currentIndex; /**< Current index of chunk within the current array being traversed */

public:
	MM_SweepHeapSectioningIterator(const MM_SweepHeapSectioning* sweepHeapSectioning)
		: _currentArray(sweepHeapSectioning->_head)
		, _currentIndex(0)
	{
	}

	MM_SweepHeapSectioningIterator()
		: _currentArray(NULL)
		, _currentIndex(0)
	{
	}

	/**
	 * Reinitialize the heap iterator with the given heap sectioning.
	 */
	void restart(MM_SweepHeapSectioning* sweepHeapSectioning)
	{
		_currentArray = sweepHeapSectioning->_head;
		_currentIndex = 0;
	}

	MM_ParallelSweepChunk* nextChunk();
};

#endif /* SWEEPSCHEMESECTIONING_HPP_ */

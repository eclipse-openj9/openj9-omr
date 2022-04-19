/*******************************************************************************
 * Copyright (c) 1991, 2022 IBM Corp. and others
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

/**
 * @file
 * @ingroup GC_Base
 */

#if !defined(TLHALLOCATIONSUPPORT_HPP_)
#define TLHALLOCATIONSUPPORT_HPP_

#include <string.h>

#include "omrcfg.h"
#include "omrcomp.h"
#include "modronbase.h"

#include "GCExtensionsBase.hpp"
#include "EnvironmentBase.hpp"
#include "HeapLinkedFreeHeader.hpp"
#include "LanguageThreadLocalHeap.hpp"
#if defined(OMR_GC_OBJECT_MAP)
#include "ObjectMap.hpp"
#endif /* defined(OMR_GC_OBJECT_MAP) */

class MM_AllocateDescription;
class MM_MemoryPool;
class MM_MemorySubSpace;
class MM_ObjectAllocationInterface;
struct OMR_VMThread;

#if defined(OMR_GC_THREAD_LOCAL_HEAP)

class MM_HeapLinkedFreeHeaderTLH : public MM_HeapLinkedFreeHeader
{
public:
	MM_MemorySubSpace *_memorySubSpace; /**< The memory subspace of a cached TLH. */
	MM_MemoryPool *_memoryPool; /**< The memory pool of a cached TLH. */
};

/**
 * Object allocation interface definition for TLH style allocators.
 * Implementation of the Thread Local Heap allocation style, each thread receiving a per-instance
 * version of the type to manage its caches.
 * @seealso MM_ObjectAllocationInterface
 */
class MM_TLHAllocationSupport
{
public:
protected:
private:
	OMR_VMThread * const _omrVMThread; /**< J9VMThread from caller's environment */
	MM_LanguageThreadLocalHeap _languageTLH;
	LanguageThreadLocalHeapStruct* const _tlh; /**< current TLH */

	uint8_t ** const _pointerToHeapAlloc; /**< pointer to Heap Allocation field for this TLH in J9VMThread structure (can be heapAlloc or nonZeroHeapAlloc) */
	uint8_t ** const _pointerToHeapTop; /**< pointer to Heap Top field for this TLH in J9VMThread structure (can be heapTop or nonZeroHeapTop) */
	intptr_t * const _pointerToTlhPrefetchFTA; /**< pointer to tlhPrefetchFTA field for this TLH in J9VMThread structure (can be tlhPrefetchFTA or nonZeroTlhPrefetchFTA) */

	MM_ObjectAllocationInterface *_objectAllocationInterface; /**< Pointer to TLHAllocationInterface instance */

	MM_HeapLinkedFreeHeaderTLH *_abandonedList; /**< List of abandoned TLHs. Shaped like a free list. */
	uintptr_t _abandonedListSize; /**< Number of entries in the abandoned list. */

	const bool _zeroTLH; /**< if true this TLH is primary (might be cleared by batchClearTLH), if false this is secondary TLH (and it would not be cleared ever) */

	uintptr_t _reservedBytesForGC; /**< Number of bytes reserved in the TLH by collector. If set, we are guaranteed to have this remaining size available when we flush/clear TLH. */
public:
protected:
private:
	/**
	 * Replenish the allocation interface TLH cache with new storage.
	 * This is a placeholder function for all non-TLH implementing configurations until a further revision of the code finally pushes TLH
	 * functionality down to the appropriate level, and not so high that all configurations must recognize it.
	 * For this implementation we simply redirect back to the supplied memory pool and if successful, populate the localized TLH and supplied
	 * AllocationDescription with the appropriate information.
	 * @return true on successful TLH replenishment, false otherwise.
	 */
	void *allocateTLH(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, MM_MemorySubSpace *memorySubSpace, MM_MemoryPool *memoryPool);

	void flushCache(MM_EnvironmentBase *env);

	MMINLINE void *getBase() { return (void *)_tlh->heapBase; };
	MMINLINE void setBase(void *basePtr) { _tlh->heapBase = (uint8_t *)basePtr; };

	/* CMVC 143597: setRealTop and getRealTop are fix to ensure we seal the
	 * TLH properly under frequent hook and unhook of ObjectAllocInstrumentable, which toggles
	 * enable/disable of TLH. If TLH is disabled, realHeapTop holds the true TLH top ptr.
	 */
	MMINLINE void setRealTop(void *realTopPtr) { _tlh->realHeapTop = (uint8_t *)realTopPtr; };
	MMINLINE void *getRealTop()
	{
		if (NULL != _tlh->realHeapTop) {
			return _tlh->realHeapTop;
		} else {
			return *_pointerToHeapTop;
		}
	};

	/* @return the number of bytes, which are available for allocation in the TLH */
	MMINLINE uintptr_t getRemainingSize() { return (uintptr_t)getRealTop() - (uintptr_t)getAlloc(); };
	/* @return the number of used bytes in the TLH */
	MMINLINE uintptr_t getUsedSize() { return (uintptr_t)getAlloc() - (uintptr_t)getBase(); };

	MMINLINE void *getAlloc() { return (void *) *_pointerToHeapAlloc; };
	MMINLINE void setAlloc(void *allocPtr) { *_pointerToHeapAlloc = (uint8_t *)allocPtr; };
	MMINLINE void *getTop() { return (void *) *_pointerToHeapTop; };
	MMINLINE void setTop(void *topPtr) { *_pointerToHeapTop = (uint8_t *)topPtr; };
	MMINLINE void setAllZeroes(void) { memset((void *)_tlh, 0, sizeof(LanguageThreadLocalHeapStruct)); };

	/**
	 * Determine how much space is left in the TLH
	 *
	 * @return the space remaining (in bytes)
	 */
	MMINLINE uintptr_t getSize() { return (uintptr_t)getTop() - (uintptr_t)getAlloc(); };

	MMINLINE uint32_t getObjectFlags() { return (uint32_t)_tlh->objectFlags; };
	MMINLINE void setObjectFlags(uint32_t flags) { _tlh->objectFlags = (uintptr_t)flags; };

	MMINLINE uintptr_t getRefreshSize() { return _tlh->refreshSize; };
	MMINLINE void setRefreshSize(uintptr_t size) { _tlh->refreshSize = size; };

	MMINLINE MM_MemorySubSpace *getMemorySubSpace() { return (MM_MemorySubSpace *)_tlh->memorySubSpace; };
	MMINLINE void setMemorySubSpace(MM_MemorySubSpace *memorySubSpace) { _tlh->memorySubSpace = (void *)memorySubSpace; };

	MMINLINE MM_MemoryPool *getMemoryPool() { return (MM_MemoryPool *)_tlh->memoryPool; };
	MMINLINE void setMemoryPool(MM_MemoryPool *memoryPool) { _tlh->memoryPool = memoryPool; };

	/**
	 * Report clearing of a full allocation cache
	 */
	void reportClearCache(MM_EnvironmentBase *env);

	/**
	 * Report allocation of a new allocation cache
	 */
	void reportRefreshCache(MM_EnvironmentBase *env);

	/**
	 * Purge the TLH data from the receiver.
	 * Remove any ownership of a heap area from the receivers TLH, and make sure the heap is left in a safe,
	 * or walkable state.
	 *
	 * @note The calling environment may not be the receivers owning environment.
	 */
	void clear(MM_EnvironmentBase *env);

	/**
	 * Reconnect the cache to the owning environment.
	 * The environment is either newly created or has had its properties changed such that the existing cache is
	 * no longer valid.  An example of this is a change of memory space.  Perform the necessary flushing and
	 * re-initialization of the cache details such that new allocations will occur in the correct fashion.
	 */
	void reconnect(MM_EnvironmentBase *env);

	/**
	 * Restart the cache from its current start to an appropriate base state.
	 * Reset the cache details back to a starting state that is appropriate for where it currently is.
	 * In this case, the state reset takes into account the ending refresh size and sets an appropriate
	 * new starting point.
	 *
	 * @note The previous cache contents are expected to have been flushed back to the heap.
	 */
	void restart(MM_EnvironmentBase *env);

	/**
	 * Reserve part (top) of TLH for GC if collector requires
	 */
	void reserveTLHTopForGC(MM_EnvironmentBase *env);

	/**
	 * Restore TLH Top if it was reserved for GC during refresh, and then notify the collector. TLH Top must be restored before it's flushed/cleared.
	 *
	 * @return Address of the last object in the TLH if one is allocated during this routine, otherwise return NULL
	 */
	void *restoreTLHTopForGC(MM_EnvironmentBase *env);

	/**
	 * Refresh the TLH.
	 */
	bool refresh(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, bool shouldCollectOnFailure);

	/**
	 * Attempt to allocate an object in this TLH.
	 */
	void *allocateFromTLH(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, bool shouldCollectOnFailure);

	void setupTLH(MM_EnvironmentBase *env, void *addrBase, void *addrTop, MM_MemorySubSpace *memorySubSpace, MM_MemoryPool *memoryPool);

	MMINLINE void wipeTLH(MM_EnvironmentBase *env)
	{
#if defined(OMR_GC_OBJECT_ALLOCATION_NOTIFY)
		objectAllocationNotify(env, _tlh->heapBase, getAlloc());
#endif /* OMR_GC_OBJECT_ALLOCATION_NOTIFY */
#if defined(OMR_GC_OBJECT_MAP)
		/* Mark all newly allocated objects from the TLH as valid objects */
		markValidObjectForRange(env, _tlh->heapBase, getAlloc());
#endif
		setupTLH(env, NULL, NULL, NULL, NULL);
		setRealTop(NULL);
	}

#if defined(OMR_GC_OBJECT_ALLOCATION_NOTIFY)
	void objectAllocationNotify(MM_EnvironmentBase *env, void *heapBase, void *heapTop);
#endif /* OMR_GC_OBJECT_ALLOCATION_NOTIFY */

#if defined(OMR_GC_OBJECT_MAP)
	/**
	 * Walk the TLH, marking all allocated objects in the object map.
	 */
	MMINLINE void
	markValidObjectForRange(MM_EnvironmentBase *env, void *heapBase, void *heapTop)
	{
		env->getExtensions()->getObjectMap()->markValidObjectForRange(env, heapBase, heapTop);
	}
#endif

	void updateFrequentObjectsStats(MM_EnvironmentBase *env);

	/**
	 * Create a ThreadLocalHeap object.
	 */
	MM_TLHAllocationSupport(MM_EnvironmentBase *env, bool zeroTLH) :
		_omrVMThread(env->getOmrVMThread()),
		_languageTLH(),
		_tlh(_languageTLH.getLanguageThreadLocalHeapStruct(env, zeroTLH)),
		_pointerToHeapAlloc(_languageTLH.getPointerToHeapAlloc(env, zeroTLH)),
		_pointerToHeapTop(_languageTLH.getPointerToHeapTop(env, zeroTLH)),
		_pointerToTlhPrefetchFTA(_languageTLH.getPointerToTlhPrefetchFTA(env, zeroTLH)),
		_objectAllocationInterface(NULL),
		_abandonedList(NULL),
		_abandonedListSize(0),
		_zeroTLH(zeroTLH),
		_reservedBytesForGC(0)
	{};

	/*
	 * friends
	 */
	friend class MM_TLHAllocationInterface;
};

#endif /* OMR_GC_THREAD_LOCAL_HEAP */
#endif /* TLHALLOCATIONSUPPORT_HPP_ */



/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2015
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


#include "omrcomp.h"
#include "modronopt.h"
#include "ModronAssertions.h"

#include <string.h>

#include "MemoryPoolAddressOrderedList.hpp"

#include "AllocateDescription.hpp"
#include "Debug.hpp"
#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"
#include "Collector.hpp"
#include "MemoryPool.hpp"
#include "MemorySubSpace.hpp"
//#include "mmhook_internal.h"
#include "HeapRegionDescriptor.hpp"
#include "LargeObjectAllocateStats.hpp"
#include "HeapLinkedFreeHeader.hpp"
#include "Heap.hpp"

#if defined(OMR_VALGRIND_MEMCHECK)
#include <valgrind/memcheck.h>
#endif /* defined(OMR_VALGRIND_MEMCHECK) */

/****************************************
 * Allocation
 ****************************************
 */

/****************************************
 * Free list building
 ****************************************
 */

void
MM_MemoryPoolAddressOrderedListBase::acquireResetLock(MM_EnvironmentBase* env)
{
	_resetLock.acquire();
}

void
MM_MemoryPoolAddressOrderedListBase::releaseResetLock(MM_EnvironmentBase* env)
{
	_resetLock.release();
}

/* (non-doxygen)
 * @see MM_MemoryPool::createFreeEntry()
 */
bool
MM_MemoryPoolAddressOrderedListBase::createFreeEntry(MM_EnvironmentBase* env, void* addrBase, void* addrTop,
													  MM_HeapLinkedFreeHeader* previousFreeEntry, MM_HeapLinkedFreeHeader* nextFreeEntry)
{
	/* Sanity checks -- should never create an out of order link */
	assume0((NULL == nextFreeEntry) || (nextFreeEntry > addrTop));

#if defined(OMR_VALGRIND_MEMCHECK)	
#if defined(VALGRIND_REQUEST_LOGS)	
	VALGRIND_PRINTF_BACKTRACE("Removing heap range b/w %p and  %p\n", addrBase,addrTop);
#endif /* defined(VALGRIND_REQUEST_LOGS) */

	if(!env->getExtensions()->_allocatedObjects.empty() && (uintptr_t) addrTop> (uintptr_t) addrBase)
	{	
		std::set<uintptr_t>::iterator it;
		std::set<uintptr_t>::iterator setEnd = env->getExtensions()->_allocatedObjects.end();
		std::set<uintptr_t>::iterator lBound = env->getExtensions()->_allocatedObjects.lower_bound((uintptr_t)addrBase);
		std::set<uintptr_t>::iterator uBound = --env->getExtensions()->_allocatedObjects.end();	
		//addrTop is exclusive
		while(*uBound > ((uintptr_t) addrTop -1) && uBound !=  env->getExtensions()->_allocatedObjects.begin())
			uBound--;

#if defined(VALGRIND_REQUEST_LOGS)	
		VALGRIND_PRINTF("lBound = %lx, uBound = %lx\n",*lBound,*uBound);			
#endif /* defined(VALGRIND_REQUEST_LOGS) */			
		
		if(*uBound >= (uintptr_t) addrTop)
			{
#if defined(VALGRIND_REQUEST_LOGS)	
				VALGRIND_PRINTF("Skipping uBound %lx",*uBound);
#endif /* defined(VALGRIND_REQUEST_LOGS) */			
				goto skip_valgrind;
			}

		for(it = lBound;*it <= *uBound && it != setEnd;it++)
		{
#if defined(VALGRIND_REQUEST_LOGS)				
			int objSize = (int) ( (GC_ObjectModel)env->getExtensions()->objectModel ).getConsumedSizeInBytesWithHeader( (omrobjectptr_t) *it);
			VALGRIND_PRINTF("Clearing object at %lx of size %d = 0x%x\n", *it,objSize,objSize);
#endif /* defined(VALGRIND_REQUEST_LOGS) */				
			VALGRIND_MEMPOOL_FREE(env->getExtensions()->valgrindMempoolAddr,*it);
		}
		if(*uBound >= *lBound && uBound != setEnd)
		{
#if defined(VALGRIND_REQUEST_LOGS)				
			VALGRIND_PRINTF("Erasing set from lBound = %lx to uBound = %lx\n",*lBound,*uBound);		
#endif /* defined(VALGRIND_REQUEST_LOGS) */			
			env->getExtensions()->_allocatedObjects.erase(lBound,++uBound);//uBound is exclusive
		}
skip_valgrind:
#if defined(VALGRIND_REQUEST_LOGS)			
	VALGRIND_PRINTF("Marking area as noaccess at %lx of size %lx (to %lx)\n",(uintptr_t)addrBase,
		(uintptr_t) addrTop - (uintptr_t) addrBase - (uintptr_t)1,(uintptr_t) addrTop);	
#endif /* (VALGRIND_REQUEST_LOGS) */		
			
		VALGRIND_MAKE_MEM_NOACCESS(addrBase, (uintptr_t) addrTop - (uintptr_t) addrBase);
	}
#endif /* defined(OMR_VALGRIND_MEMCHECK) */
	
	if (internalRecycleHeapChunk(addrBase, addrTop, nextFreeEntry)) {

		/* The range is big enough for the free list, so link the previous to it */
		if (previousFreeEntry) {
			assume0(previousFreeEntry < addrBase);
			previousFreeEntry->setNext((MM_HeapLinkedFreeHeader*)addrBase);
		}
		return true;
	}

	/* The range was not big enough for the free list, so link previous to next */
	if (previousFreeEntry) {
		assume0((NULL == nextFreeEntry) || (nextFreeEntry > previousFreeEntry));
		previousFreeEntry->setNext(nextFreeEntry);
	}
	return false;
}

/* (non-doxygen)
 * @see MM_MemoryPool::createFreeEntry()
 */
bool
MM_MemoryPoolAddressOrderedListBase::createFreeEntry(MM_EnvironmentBase* env, void* addrBase, void* addrTop)
{
	return createFreeEntry(env, addrBase, addrTop, NULL, NULL);
}

/**
 * Add inner chunk memory piece to pool ("Create")
 * 
 * @param address free memory start address
 * @param size free memory size in bytes
 * @param previousFreeEntry previous element of list (if any) this memory must be connected with
 * @return true if free memory element accepted
 */
bool
MM_MemoryPoolAddressOrderedListBase::connectInnerMemoryToPool(MM_EnvironmentBase* env, void* address, uintptr_t size, void* previousFreeEntry)
{
	bool result = false;

	if (size >= getMinimumFreeEntrySize()) {
		/* Build the free header */
		createFreeEntry(env, (MM_HeapLinkedFreeHeader*)address, (uint8_t*)address + size, (MM_HeapLinkedFreeHeader*)previousFreeEntry, NULL);
		result = true;
	}
	return result;
}


/**
 * Connect leading/trailing chunk free memory piece ("Connect")
 *
 * @param address free memory start address
 * @param size free memory size in bytes
 * @param nextFreeEntry next element of list (if any) this memory must be connected with
 */
void
MM_MemoryPoolAddressOrderedListBase::connectOuterMemoryToPool(MM_EnvironmentBase* env, void* address, uintptr_t size, void* nextFreeEntry)
{
	Assert_MM_true((NULL == nextFreeEntry) || (address < nextFreeEntry));
	Assert_MM_true((NULL == address) || (size >= getMinimumFreeEntrySize()));

	/* Build the free header */
	createFreeEntry(env, (MM_HeapLinkedFreeHeader*)address, (uint8_t*)address + size, NULL, (MM_HeapLinkedFreeHeader*)nextFreeEntry);

	if (NULL == *_referenceHeapFreeList) {
		*_referenceHeapFreeList = (MM_HeapLinkedFreeHeader*)nextFreeEntry;
	}
}

/**
 * Connect leading/trailing chunk free memory piece ("Finalize")
 * temporary - need be separated because of implementation of subpools
 * 
 * @param address free memory start address
 * @param size free memory size in bytes
 */
void
MM_MemoryPoolAddressOrderedListBase::connectFinalMemoryToPool(MM_EnvironmentBase* env, void* address, uintptr_t size)
{
	Assert_MM_true((NULL == address) || (size >= getMinimumFreeEntrySize()));

	/* Build the free header */
	createFreeEntry(env, (MM_HeapLinkedFreeHeader*)address, (uint8_t*)address + size);
}

/**
 * Abandon memory piece
 * 
 * @param memoryPool current memory pool
 * @param address free memory start address
 * @param size free memory size in bytes
 */
void
MM_MemoryPoolAddressOrderedListBase::abandonMemoryInPool(MM_EnvironmentBase* env, void* address, uintptr_t size)
{
	abandonHeapChunk((MM_HeapLinkedFreeHeader*)address, (uint8_t*)address + size);
}

#if defined(OMR_GC_IDLE_HEAP_MANAGER)
uintptr_t
MM_MemoryPoolAddressOrderedListBase::releaseFreeEntryMemoryPages(MM_EnvironmentBase* env, MM_HeapLinkedFreeHeader* freeEntry)
{
	uintptr_t releasedMemory = 0;
	MM_HeapLinkedFreeHeader* currentFreeEntry = freeEntry;
	uintptr_t pageSize = env->getExtensions()->heap->getPageSize();
	while (NULL != currentFreeEntry) {
		/* skip entry less than page size */
		if (pageSize <= currentFreeEntry->getSize()) {
			uintptr_t addressBase = MM_Math::roundToCeiling(pageSize, (uintptr_t)currentFreeEntry + sizeof(MM_HeapLinkedFreeHeader));
			/* release/decommit memory after Header */
			uintptr_t totalFreePagesCount = (currentFreeEntry->getSize() - (addressBase - (uintptr_t)currentFreeEntry)) / pageSize;
			if (0 < totalFreePagesCount) {
				uintptr_t commitPagesCount = 0;
				uintptr_t decommitPagesCount = 0;
				if (0 < _extensions->idleMinimumFree) {
					commitPagesCount = totalFreePagesCount * _extensions->idleMinimumFree / 100;
				}
				decommitPagesCount = totalFreePagesCount - commitPagesCount;
				/* leave commited pages of memory aside header */
				addressBase += commitPagesCount * pageSize;
				/* now decommit pages of memory */
				if (0 < decommitPagesCount) {
					if (_extensions->heap->decommitMemory((void*)addressBase, decommitPagesCount * pageSize, NULL, currentFreeEntry->afterEnd())) {
						releasedMemory += decommitPagesCount * pageSize;
					}
				}
			}
		}
		currentFreeEntry = currentFreeEntry->getNext();
	}
	return releasedMemory;
}
#endif

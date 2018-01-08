/*******************************************************************************
 * Copyright (c) 1991, 2016 IBM Corp. and others
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

#include <string.h>
#include "omrport.h"
#include "omrutil.h"
#include "pool_api.h"
#include "algorithm_test_internal.h"

#define ROUND_TO(granularity, number) ( (((number) % (granularity)) ? ((number) + (granularity) - ((number) % (granularity))) : (number)))

/* Puddle list that will be shared by multiple pools in testPuddleListSharing(). */
static J9PoolPuddleList *sharedPuddleList = NULL;
static int32_t sharedPuddleListReferenceCount = 0;

#define NUM_POOLS_TO_SHARE_PUDDLE_LIST 16

#define FIRST_BYTE_MARKER 1
#define BYTE_MARKER 2
#define LAST_BYTE_MARKER 4

static J9Pool* createNewPool(OMRPortLibrary *portLib, PoolInputData *input);
static int32_t testPoolNewElement(OMRPortLibrary *portLib, PoolInputData *inputData, J9Pool *currentPool);
static int32_t testPoolWalkFunctions(OMRPortLibrary *portLib, PoolInputData *inputData, J9Pool *currentPool, uint32_t elementsAllocated);
static int32_t testPoolClear(OMRPortLibrary *portLib, J9Pool *currentPool);
static int32_t testPoolRemoveElement(OMRPortLibrary *portLib, J9Pool *currentPool);

static void *sharedPuddleListAlloc(OMRPortLibrary *portLib, uint32_t size, const char *callSite, uint32_t memoryCategory, uint32_t type, uint32_t *doInit);
static void sharedPuddleListFree(OMRPortLibrary *portLib, void *address, uint32_t type);

int32_t
createAndVerifyPool(OMRPortLibrary *portLib, PoolInputData *input)
{
	J9Pool *pool = createNewPool(portLib, input);
	int32_t elementCount = 0;
	int32_t rc = 0;

	if (NULL == pool) {
		return -1;
	}
	elementCount = testPoolNewElement(portLib, input, pool);
	if (elementCount < 0) {
		return elementCount;
	}
	rc = testPoolWalkFunctions(portLib, input, pool, elementCount);
	if (0 != rc) {
		return rc;
	}
	/* Clear the pools, and test new, walk, and remove again. */
	rc = testPoolClear(portLib, pool);
	if (0 != rc) {
		return rc;
	}
	elementCount = testPoolNewElement(portLib, input, pool);
	if (elementCount < 0) {
		return elementCount;
	}
	rc = testPoolWalkFunctions(portLib, input, pool, elementCount);
	if (0 != rc) {
		return rc;
	}
	rc = testPoolRemoveElement(portLib, pool);
	if (0 != rc) {
		return rc;
	}
	pool_kill(pool);

	return 0;
}

static void *
sharedPuddleListAlloc(OMRPortLibrary *portLib, uint32_t size, const char *callSite, uint32_t memoryCategory, uint32_t type, uint32_t *doInit)
{
	if (type == POOL_ALLOC_TYPE_PUDDLE_LIST) {
		*doInit = 0;
		if (sharedPuddleList == NULL) {
			sharedPuddleList = portLib->mem_allocate_memory(portLib, size, callSite, OMRMEM_CATEGORY_VM);
			*doInit = 1;
		}
		if (sharedPuddleList != NULL) {
			sharedPuddleListReferenceCount++;
		}
		return sharedPuddleList;
	} else {
		return portLib->mem_allocate_memory(portLib, size, callSite, OMRMEM_CATEGORY_VM);
	}
}

static void
sharedPuddleListFree(OMRPortLibrary *portLib, void *address, uint32_t type)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portLib);
	if (type == POOL_ALLOC_TYPE_PUDDLE_LIST) {
		sharedPuddleListReferenceCount--;
		if (sharedPuddleListReferenceCount == 0) {
			omrmem_free_memory(address);
			sharedPuddleList = NULL;
		}
	} else if (type == POOL_ALLOC_TYPE_PUDDLE) {
		if (sharedPuddleListReferenceCount == 1) {
			omrmem_free_memory(address);
		}
	} else {
		omrmem_free_memory(address);
	}
}

static J9Pool *
createNewPool(OMRPortLibrary *portLib, PoolInputData *input)
{
	uint32_t expectedNumElems = (input->numberElements == 0) ? 1 : input->numberElements;
	uint32_t expectedAlignment = (input->elementAlignment == 0) ? MIN_GRANULARITY : input->elementAlignment;

	J9Pool *pool = pool_new(
					input->structSize,
					input->numberElements,
					input->elementAlignment,
					input->poolFlags,
					OMR_GET_CALLSITE(),
					OMRMEM_CATEGORY_VM,
					POOL_FOR_PORT(portLib));

	/* Check that creation succeeded */
	if (NULL == pool) {
		return NULL;
	}

	if (pool->puddleAllocSize < ((expectedNumElems * ROUND_TO(expectedAlignment, input->structSize)) + ROUND_TO(expectedAlignment, sizeof(J9PoolPuddle)))) {
		pool_kill(pool);
		return NULL;
	}
	else if ((input->poolFlags & POOL_ROUND_TO_PAGE_SIZE) && (pool->puddleAllocSize < OS_PAGE_SIZE)) {
		pool_kill(pool);
		return NULL;
	}
	return pool;
}

static int32_t
testPoolNewElement(OMRPortLibrary *portLib, PoolInputData *inputData, J9Pool *currentPool)
{
	void *element = NULL;
	uint8_t *walkBytes;
	uint32_t elementsInPuddle = 0;
	uint32_t expectedNumElems = (inputData->numberElements == 0) ? 1 : inputData->numberElements;
	uint32_t expectedAlignment = (inputData->elementAlignment == 0) ? MIN_GRANULARITY : inputData->elementAlignment;


	J9PoolPuddleList* puddleList = J9POOL_PUDDLELIST(currentPool);
	J9PoolPuddle* initialPuddle = J9POOLPUDDLELIST_NEXTAVAILABLEPUDDLE(puddleList);
	J9PoolPuddle* currentPuddle = initialPuddle;

	/* Call pool_newElement until a new puddle is allocated... and then allocate a couple of new elements into the new puddle */
	while (initialPuddle == currentPuddle) {
		uint32_t j = 0;
		/* Get a single element and check that it's the right size and that each byte can be written to */
		element = pool_newElement(currentPool);
		if (NULL == element) {
			return -1;
		}

		/* Verify element alignment. Aligned elements are expected for use in AVL trees, for example. */
		if (((uintptr_t) element % expectedAlignment) != 0) {
			return -2;
		}

		if (inputData->poolFlags & POOL_NO_ZERO) {
			memset(element, 0, inputData->structSize);		/* Expect a crash if the element is too small */
		}

		/* Walk each byte of the returned element */
		walkBytes = (uint8_t *)element;
		for (j = 0; j < inputData->structSize; j++) {
			if (walkBytes[j] != 0) {
				return -3;
			}
			/* Test that we can write to each byte */
			if (j == 0) {
				walkBytes[j] = FIRST_BYTE_MARKER;
			} else {
				walkBytes[j] = BYTE_MARKER;
			}
		}

		if (inputData->structSize > 1) {
			walkBytes[inputData->structSize - 1] = LAST_BYTE_MARKER;
		}

		/* currentPuddle will become NULL when the puddle being used becomes FULL if there has been no deletions */
		elementsInPuddle++;

		currentPuddle = J9POOLPUDDLELIST_NEXTAVAILABLEPUDDLE(puddleList);
	}
	if (elementsInPuddle < expectedNumElems) {
		return -4;
	}
	return elementsInPuddle;
}

static int32_t
testPoolWalkFunctions(OMRPortLibrary *portLib, PoolInputData *inputData, J9Pool *currentPool, uint32_t elementsAllocated)
{
	pool_state state;
	uint8_t *element = NULL;
	uint32_t walkCount = 0;

	element = (uint8_t *)pool_startDo(currentPool, &state);
	do {
		if (element[0] != FIRST_BYTE_MARKER) {
			return -1;
		}
		if ((inputData->structSize > 1) && (element[inputData->structSize - 1] != LAST_BYTE_MARKER)) {
			return -2;
		}
		if (!pool_includesElement(currentPool, element)) {
			return -3;
		}
		walkCount++;
		element = pool_nextDo(&state);
	} while (element != NULL);

	if (walkCount != elementsAllocated) {
		return -4;
	}
	if (pool_numElements(currentPool) != elementsAllocated) {
		return -5;
	}
	return 0;
}

static int32_t
testPoolClear(OMRPortLibrary *portLib, J9Pool *currentPool)
{
	uintptr_t expectedCapacityAfterClear = pool_capacity(currentPool);

	pool_clear(currentPool);

	if (0 != pool_numElements(currentPool)) {
		return -1;
	} else if (expectedCapacityAfterClear != pool_capacity(currentPool)) {
		return -2;
	}
	return 0;
}

static int32_t
testPoolRemoveElement(OMRPortLibrary *portLib, J9Pool *currentPool)
{
	uintptr_t expectedNumElements = pool_numElements(currentPool);
	pool_state state;
	uintptr_t startCount, endCount;
	uintptr_t expectedCapacityAfterRemovals = currentPool->elementsPerPuddle;

	if (currentPool->flags & POOL_NEVER_FREE_PUDDLES) {
		expectedCapacityAfterRemovals = pool_capacity(currentPool);
	}

	do {
		uint8_t *element = (uint8_t *)pool_startDo(currentPool, &state);

		startCount = pool_numElements(currentPool);
		do {
			expectedNumElements--;
			if (!pool_includesElement(currentPool, element)) {
				return -1;
			}
			pool_removeElement(currentPool, element);
			if (pool_numElements(currentPool) != expectedNumElements) {
				return -2;
			}
			if (pool_includesElement(currentPool, element)) {
				return -3;
			}
			/* Remove every other element each time - this deliberately creates fragmentation */
			element = pool_nextDo(&state);
			if (NULL != element) {
				element = pool_nextDo(&state);
			}
		} while (NULL != element);
		endCount = pool_numElements(currentPool);
	} while ((endCount > 0) && (startCount != endCount));

	if (expectedNumElements != 0) {
		return -5;
	} else if (expectedCapacityAfterRemovals != pool_capacity(currentPool)) {
		return -6;
	}

	return 0;
}

int32_t
testPoolPuddleListSharing(OMRPortLibrary *portLib)
{
	uint32_t index = 0;
	uintptr_t numElements = 0;
	void *element = NULL;
	J9Pool *pool[NUM_POOLS_TO_SHARE_PUDDLE_LIST];
	pool_state state[NUM_POOLS_TO_SHARE_PUDDLE_LIST];
	int32_t result = 0;

	memset(pool, 0, sizeof(pool));

	for (index = 0; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
		pool[index] = pool_new(sizeof(U_64), 0, 0, 0,
							   OMR_GET_CALLSITE(), OMRMEM_CATEGORY_VM,
							   (omrmemAlloc_fptr_t) sharedPuddleListAlloc,
							   (omrmemFree_fptr_t) sharedPuddleListFree,
							   portLib);

		if (NULL == pool[index]) {
			result = -1;
			goto error;
		}

		/* Create an element every time we create a pool. */
		pool_newElement(pool[index]);

		/*
		 * Since the underlying puddle list is shared, the number of elements in the pool
		 * should be the same as the 1 + index of the pool we're on.
		 */
		numElements = pool_numElements(pool[index]);
		if (numElements != (1 + index)) {
			result = -2;
			goto error;
		}
	}

	/* Verify again that numElements is the same for all pools. */
	numElements = pool_numElements(pool[0]);
	for (index = 1; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
		if (numElements != pool_numElements(pool[index])) {
			result = -3;
			goto error;
		}
	}

	/* Iterate over the elements in each pool, and verify that they match. */
	element = pool_startDo(pool[0], &state[0]);
	/* Check the first element. */
	for (index = 1; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
		if (element != pool_startDo(pool[index], &state[index])) {
			result = -4;
			goto error;
		}
	}
	/* And all other elements. */
	while (element) {
		element = pool_nextDo(&state[0]);
		for (index = 1; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
			if (element != pool_nextDo(&state[index])) {
				result = -5;
				goto error;
			}
		}
	}

	/* Now, kill the pools. */
	for (index = 0; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
		if (numElements != pool_numElements(pool[index])) {
			result = -6;
			goto error;
		}
		pool_kill(pool[index]);
		pool[index] = NULL;
	}

	if (sharedPuddleList != NULL) {
		result = -7;
		goto error;
	}

	return result;

error:
	for (index = 0; index < NUM_POOLS_TO_SHARE_PUDDLE_LIST; index++) {
		if (NULL != pool[index]) {
			pool_kill(pool[index]);
			pool[index] = NULL;
		}
	}

	return result;
}

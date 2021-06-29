/*******************************************************************************
 * Copyright (c) 1991, 2021 IBM Corp. and others
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

#include "omrcfg.h"
#include "omrcomp.h"

#include "SweepHeapSectioningSegmented.hpp"
#include "SweepPoolManager.hpp"

#include "EnvironmentBase.hpp"
#include "Heap.hpp"
#include "HeapRegionIterator.hpp"
#include "NonVirtualMemory.hpp"
#include "MemoryPool.hpp"
#include "MemorySubSpace.hpp"
#include "ParallelDispatcher.hpp"
#include "ParallelSweepChunk.hpp"

/**
 * Return the expected total sweep chunks that will be used in the system.
 * Called during initialization, this routine looks at the maximum size of the heap and expected
 * configuration (generations, regions, etc) and determines the approximate maximum number of chunks
 * that will be required for a sweep at any given time.  It is safe to underestimate the number of chunks,
 * as the sweep sectioning mechnanism will compensate, but the the expectation is that by having all
 * chunk memory allocated in one go will keep the data localized and fragment system memory less.
 * @return estimated upper bound number of chunks that will be required by the system.
 */
uintptr_t
MM_SweepHeapSectioningSegmented::estimateTotalChunkCount(MM_EnvironmentBase *env)
{
	uintptr_t totalChunkCountEstimate = MM_SweepHeapSectioning::estimateTotalChunkCount(env);

#if defined(OMR_GC_MODRON_SCAVENGER)
	/* Because object memory segments have not been allocated yet, we cannot get the real numbers.
	 * Assume that if the scavenger is enabled, each of the semispaces will need an extra chunk */
	/* TODO: Can we make an estimate based on the number of leaf memory subspaces allocated (if they are already allocated but not inflated)??? */
	if(_extensions->scavengerEnabled) {
		totalChunkCountEstimate += 2;
	}
#endif /* OMR_GC_MODRON_SCAVENGER */

	return totalChunkCountEstimate;
}

/**
 * Walk all segments and calculate the maximum number of chunks needed to represent the current heap.
 * The chunk calculation is done on a per segment basis (no segment can represent memory from more than 1 chunk),
 * and partial sized chunks (ie: less than the chunk size) are reserved for any remaining space at the end of a
 * segment.
 * @return number of chunks required to represent the current heap memory.
 */
uintptr_t
MM_SweepHeapSectioningSegmented::calculateActualChunkNumbers() const
{
	uintptr_t totalChunkCount = 0;

	MM_HeapRegionDescriptor *region;
	MM_Heap *heap = _extensions->heap;
	MM_HeapRegionManager *regionManager = heap->getHeapRegionManager();
	GC_HeapRegionIterator regionIterator(regionManager);

	while((region = regionIterator.nextRegion()) != NULL) {
		if ((region)->isCommitted()) {
			/* TODO:  this must be rethought for Tarok since it treats all regions identically but some might require different sweep logic */
			MM_MemorySubSpace *subspace = region->getSubSpace();
			/* if this is a committed region, it requires a non-NULL subspace */
			Assert_MM_true(NULL != subspace);
			uintptr_t poolCount = subspace->getMemoryPoolCount();

			totalChunkCount += MM_Math::roundToCeiling(_extensions->parSweepChunkSize, region->getSize()) / _extensions->parSweepChunkSize;

			/* Add extra chunks if more than one memory pool */
			totalChunkCount += (poolCount - 1);
		}
	}

	return totalChunkCount;
}

/**
 * Allocate and initialize a new instance of the receiver.
 * @return pointer to the new instance.
 */
MM_SweepHeapSectioningSegmented *
MM_SweepHeapSectioningSegmented::newInstance(MM_EnvironmentBase *env)
{
	MM_SweepHeapSectioningSegmented *sweepHeapSectioning = (MM_SweepHeapSectioningSegmented *)env->getForge()->allocate(sizeof(MM_SweepHeapSectioningSegmented), OMR::GC::AllocationCategory::FIXED, OMR_GET_CALLSITE());
	if (sweepHeapSectioning) {
		new(sweepHeapSectioning) MM_SweepHeapSectioningSegmented(env);
		if (!sweepHeapSectioning->initialize(env)) {
			sweepHeapSectioning->kill(env);
			return NULL;
		}
	}
	return sweepHeapSectioning;
}

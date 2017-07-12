/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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
 *******************************************************************************/

/**
 * @file
 * @ingroup GC_Base
 */

#include "MarkMapSegmentChunkIterator.hpp"

#include "Heap.hpp"
#include "HeapRegionManager.hpp"

/**
 * @see GC_MarkMapSegmentChunkIterator::nextChunk()
 */
bool
GC_MarkMapSegmentChunkIterator::nextChunk(MM_HeapMap *markMap, UDATA **base, UDATA **top)
{
	while (_segmentBytesRemaining > 0) {
		UDATA thisChunkSize = OMR_MIN(_segmentBytesRemaining, _chunkSize);
		UDATA *chunkTop = (UDATA *)((U_8 *)_nextChunkBase + thisChunkSize);
		_segmentBytesRemaining -= thisChunkSize;
		
		/* There may not be a marked object in this chunk */
		/* If that is the case, we move on to the next chunk */
		_markedObjectIterator.reset(markMap, _nextChunkBase, chunkTop);
		omrobjectptr_t firstObject = _markedObjectIterator.nextObject();

		_nextChunkBase = chunkTop;

		if (firstObject != NULL) {
			*base = (UDATA *)firstObject;
			*top = chunkTop;
			if (_extensions->isVLHGC()) {
				/* while this code should work in phase2, the markedObjectIterator can give back a chunkTop which points after an unused region */
				MM_HeapRegionManager *manager = _extensions->getHeap()->getHeapRegionManager();
				MM_HeapRegionDescriptor *desc = manager->tableDescriptorForAddress(firstObject);
				MM_HeapRegionDescriptor *checkDesc = manager->tableDescriptorForAddress((void *)((UDATA)chunkTop - 1));
				Assert_MM_true(desc->_headOfSpan == checkDesc->_headOfSpan);
				Assert_MM_true(desc->isCommitted());
			}
			return true;
		}
	}
	return false;
}


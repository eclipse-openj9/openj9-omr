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

#if !defined(MARKMAPSEGMENTCHUNKITERATOR_HPP_)
#define MARKMAPSEGMENTCHUNKITERATOR_HPP_

#include "omr.h"
#include "omrcfg.h"

#include "GCExtensionsBase.hpp"
#include "HeapMapIterator.hpp"

class MM_GCExtensionsBase;
class MM_HeapMap;

/**
 * Iterate over chunks of an area of memory by splitting the extent into even size chunks,
 * then using the mark map to find the first object in each chunk.
 * @note the mark map must be valid in order to use this iterator
 * @ingroup GC_Base
 */
class GC_MarkMapSegmentChunkIterator
{
private:
	MM_GCExtensionsBase * const _extensions; /**< the GC extensions for the JVM */
	UDATA _chunkSize;
	UDATA _segmentBytesRemaining;
	MM_HeapMapIterator _markedObjectIterator;
	UDATA *_nextChunkBase;

public:
	void *operator new(size_t size, void *memoryPtr) { return memoryPtr; };

	GC_MarkMapSegmentChunkIterator(MM_GCExtensionsBase *extensions, void *lowAddress, void *highAddress, UDATA chunkSize) :
		_extensions(extensions),
		_chunkSize(chunkSize),
		_segmentBytesRemaining((UDATA)highAddress - (UDATA)lowAddress),
		_markedObjectIterator(extensions),
		_nextChunkBase((UDATA *)lowAddress)
	{};

	/**
	 * @note Any chunk returned from this method must have either an object or a free header beginning at
	 * the first slot in the chunk. 
	 * 
	 * @param markMap[in] The mark map to use when finding the next chunk
	 * @param base (OUT parameter) a pointer to the base of the next chunk will be stored into this address
	 * @param top (OUT parameter) a pointer to the top of the next chunk will be stored into this address
	 * @return true if there was a chunk available
	 * @return false if there were no more chunks
	 */
	bool nextChunk(MM_HeapMap *markMap, UDATA **base, UDATA **top);
};

#endif /* MARKMAPSEGMENTCHUNKITERATOR_HPP_ */


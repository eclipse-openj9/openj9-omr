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

#if !defined(PARALLELOBJECTHEAPITERATOR_HPP_)
#define PARALLELOBJECTHEAPITERATOR_HPP_

#include "omr.h"
#include "omrcfg.h"

#include "Dispatcher.hpp"
#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"
#include "MarkMapSegmentChunkIterator.hpp"
#include "MemorySubSpace.hpp"
#include "ObjectHeapBufferedIterator.hpp"
#include "ObjectHeapIterator.hpp"
#include "Task.hpp"

class MM_MarkMap;

/**
 * Multi-threaded iterator for iterating over objects in a memory segment.
 * @note MM_MemorySubSpace::prepareSegmentForWalk must be called on the
 * segment before using this iterator.
 * @note Also assumes that the environment passed in has already been set up
 * with a parallel task, and slave threads are active.
 * @ingroup GC_Base
 */
class GC_ParallelObjectHeapIterator : public GC_ObjectHeapIterator
{
	/*
	 * Data members
	 */
private:
	MM_EnvironmentBase *_env;
	GC_ObjectHeapBufferedIterator _objectHeapIterator;
	GC_MarkMapSegmentChunkIterator _segmentChunkIterator;
	void *_topAddress;
	MM_MarkMap *_markMap;
	UDATA *_chunkBase;
	UDATA *_chunkTop;

protected:
public:
	
	/* 
	 * Function members
	 */
private:
	bool getNextChunk();
protected:
public:
	virtual omrobjectptr_t nextObject();
	virtual omrobjectptr_t nextObjectNoAdvance();
	virtual void advance(UDATA size);
	virtual void reset(UDATA *base, UDATA *top);
	
	GC_ParallelObjectHeapIterator(MM_EnvironmentBase *env, MM_HeapRegionDescriptor *region, void *base, void *top, MM_MarkMap *markMap, UDATA parallelChunkSize)
		: GC_ObjectHeapIterator()
		, _env(env)
		, _objectHeapIterator(env->getExtensions(), region, base, top, false, 1)
		, _segmentChunkIterator(env->getExtensions(), base, top, parallelChunkSize)
		, _topAddress(top)
		, _markMap(markMap)
		, _chunkBase(NULL)
		, _chunkTop(NULL)
	{
		/* Metronome currently has no notion of address-ordered-list */
		Assert_MM_true(!env->getExtensions()->isMetronomeGC());
		if (!getNextChunk()) {
			_objectHeapIterator.reset(NULL, NULL);
		}
	}
};

#endif /* PARALLELOBJECTHEAPITERATOR_HPP_ */


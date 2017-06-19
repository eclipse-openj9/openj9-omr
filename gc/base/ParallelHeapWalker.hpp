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

#if !defined(PARALLEL_HEAP_WALKER_HPP_)
#define PARALLEL_HEAP_WALKER_HPP_

#include "omr.h"
#include "omrcfg.h"

#include "HeapWalker.hpp"

class MM_EnvironmentBase;
class MM_ParallelGlobalGC;
class MM_MarkMap;

class MM_ParallelHeapWalker : public MM_HeapWalker
{
	/*
	 * Data members
	 */
private:
	MM_MarkMap *_markMap;
	MM_ParallelGlobalGC *_globalCollector;
protected:
public:
	
	/*
	 * Function members
	 */
private:
protected:
public:	
	/**
	 * Walk through all live objects of the heap in parallel and apply the provided function.
	 */
	void allObjectsDoParallel(MM_EnvironmentBase *env, MM_HeapWalkerObjectFunc function, void *userData, UDATA walkFlags);

	/**
	 * Walk through all live objects of the heap and apply the provided function.
	 * If parallel is set to true, task is dispatched to GC threads and walks the heap segments in parallel,
	 * otherwise walk all objects in the heap in a single threaded linear fashion.
	 */
	virtual void allObjectsDo(MM_EnvironmentBase *env, MM_HeapWalkerObjectFunc function, void *userData, UDATA walkFlags, bool parallel, bool prepareHeapForWalk);

	MM_MarkMap *getMarkMap() {
		return _markMap;
	}
	void setMarkMap(MM_MarkMap *markMap) {
		_markMap = markMap;
	}

	static MM_ParallelHeapWalker *newInstance(MM_ParallelGlobalGC *globalCollector, MM_MarkMap *markMap, MM_EnvironmentBase *env); 	
	
	/**
	 * constructor of Parallel Heap Walker
	 */
	MM_ParallelHeapWalker(MM_ParallelGlobalGC *globalCollector, MM_MarkMap *markMap)
		: MM_HeapWalker()
		, _markMap(markMap)
		, _globalCollector(globalCollector)
	{
		_typeId = __FUNCTION__;
	}

	/*
	 * Friends
	 */
	friend class MM_ParallelObjectDoTask;
};

#endif /* PARALLEL_HEAP_WALKER_HPP_ */


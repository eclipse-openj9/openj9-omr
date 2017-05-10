/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017
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

#if !defined(MARKINGDELEGATE_HPP_)
#define MARKINGDELEGATE_HPP_

#include "objectdescription.h"
#include "omrgcconsts.h"

#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"
#include "MixedObjectScanner.hpp"
#include "Task.hpp"

class MM_EnvironmentBase;
class MM_MarkingScheme;

/**
 * Provides language-specific support for marking.
 */
class MM_MarkingDelegate
{
	/*
	 * Data members
	 */
private:

protected:
	GC_ObjectModel *_objectModel;
	MM_MarkingScheme *_markingScheme;

public:

	/*
	 * Function members
	 */
private:

protected:

public:
	/**
	 * Initialize the delegate.
	 *
	 * @param env environment for calling thread
	 * @param markingScheme the MM_MarkingScheme that the delegate is bound to
	 * @return true if delegate initialized successfully
	 */
	MMINLINE bool
	initialize(MM_EnvironmentBase *env, MM_MarkingScheme *markingScheme)
	{
		_objectModel = &(env->getExtensions()->objectModel);
		_markingScheme = markingScheme;
		return true;
	}

	/**
	 * This method is called on the master garbage collection thread at the beginning of a GC cycle. Any
	 * language-specific setup that is required to support marking can be performed here.
	 *
	 * @param env The environment for the calling thread
	 */
	MMINLINE void masterSetupForGC(MM_EnvironmentBase *env) { }

	/**
	 * This method will be called on the master garbage collection thread in place of masterSetupForGC()
	 * if the heap is being walked without performing an actual garbage collection cycle.
	 *
	 * @param env the current environment
	 */
	MMINLINE void masterSetupForWalk(MM_EnvironmentBase *env) { }

	/**
	 * This method is called on the master garbage collection thread at the end of a GC cycle. Any
	 * language-specific finalization that is required to support marking can be performed here.
	 *
	 * @param env the current environment
	 */
	void masterCleanupAfterGC(MM_EnvironmentBase *env);

	/**
	 * This method is called on each active thread to commence root scanning. Each thread should scan its own
	 * stack to identify heap object references, as well as participate in identifying additional heap references
	 * that would not be discovered in the subsequent traversal of the object reference graph depending from the
	 * root set.
	 *
	 * For each root object identified, MM_MarkingScheme::scanObject() must be called via _markingScheme.
	 *
	 * @param env The environment for the calling thread
	 */
	void scanRoots(MM_EnvironmentBase *env);

	/**
	 * This method is called for every live object discovered during marking. It must return an object scanner instance that
	 * is appropriate for the type of object to be scanned.
	 *
	 * @param env The environment for the calling thread
	 * @param objectPtr Points to the heap object to be scanned
	 * @param reason Enumerator identifying the reason for scanning this object
	 * @param sizeToDo For pointer arrays (which may be split into multiple subsegments for parallelization), stop after scanning this many bytes
	 * @return An object scanner instance that is appropriate for the type of object to be scanned
	 *
	 * @see GC_ObjectScanner
	 * @see GC_ObjectModel::getSizeInBytesWithHeader()
	 */
	MMINLINE GC_ObjectScanner *
	getObjectScanner(MM_EnvironmentBase *env, omrobjectptr_t objectPtr, void *scannerSpace, MM_MarkingSchemeScanReason reason, uintptr_t *sizeToDo)
	{
		GC_MixedObjectScanner *objectScanner = GC_MixedObjectScanner::newInstance(env, objectPtr, scannerSpace, 0);
		*sizeToDo = sizeof(fomrobject_t) + objectScanner->getBytesRemaining();
		return objectScanner;

	}

	/**
	 * This method is called after the object graph depending from the root set has been traversed and all live
	 * heap objects included in the root set and dependent graph have been marked. If there are other heap objects
	 * that may not have been marked in the object reference graph traversal they should be marked here and, when
	 * this additional marking is complete, MM_MarkingScheme::completeScan() must be called to mark live objects
	 * depending from additional marked objects.
	 *
	 * @param env The environment for the calling thread
	 */
	MMINLINE void completeMarking(MM_EnvironmentBase *env) { }


	/**
	 * This method is called on each active thread after it has completed its live marking scan and before
	 * initiating the final (clearable) marking phase. The called thread should flush any buffers that it is
	 * holding. This method requires that all GC threads be synchronized before returning, and any single-
	 * threaded tasks that are required should be performed in the synchronization block. If there are other
	 * objects to be scanned and marked they should be scanned here, after synchonizing. The OMR runtime
	 * will subsequently call MM_MarkingScheme::completeScan() to mark any objects depending from objects
	 * marked here.
	 *
	 * If there is no subsequent marking do be done, synchronization is not required. Synchronization is
	 * included in this example for illustration only.
	 *
	 * @param env The environment for the calling thread
	 */
	MMINLINE void markLiveObjectsComplete(MM_EnvironmentBase *env)
	{
		/* All threads flush buffers here, before synchronization */
		if (env->_currentTask->synchronizeGCThreadsAndReleaseSingleThread(env, UNIQUE_ID)) {
			/* Perform single-threaded tasks here */
			env->_currentTask->releaseSynchronizedGCThreads(env);
		}
		/* Start scanning any remaining unscanned objects here, with all threads participating */
	}

	/**
	 * Constructor.
	 */
	MMINLINE MM_MarkingDelegate()
		: _objectModel(NULL)
		, _markingScheme(NULL)
	{ }
};

#endif /* MARKINGDELEGATE_HPP_ */

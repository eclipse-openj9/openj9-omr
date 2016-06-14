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

#ifndef ENVIRONMENTLANGUAGEINTERFACE_HPP_
#define ENVIRONMENTLANGUAGEINTERFACE_HPP_

#include "omrcfg.h"

#include "BaseVirtual.hpp"
#include "EnvironmentBase.hpp"

#include "objectdescription.h"
#include "omr.h"
#include "omrvm.h"

#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
class MM_ConcurrentGCStats;
#endif /* defined(OMR_GC_MODRON_CONCURRENT_MARK) */

class MM_Collector;

class MM_EnvironmentLanguageInterface : public MM_BaseVirtual
{
private:
protected:
	OMR_VMThread *_omrThread;  /**< Associated OMR_VMThread */
	OMR_VM *_omrVM;
	OMRPortLibrary *_portLibrary;
	MM_EnvironmentBase *_env;  /**< Associated Environment */
public:

private:
protected:
	MM_EnvironmentLanguageInterface(MM_EnvironmentBase *env)
		: MM_BaseVirtual()
		,_omrThread(env->getOmrVMThread())
		,_omrVM(env->getOmrVM())
		,_portLibrary(env->getPortLibrary())
		,_env(env)
	{
		_typeId = __FUNCTION__;
	};

public:
	virtual void kill(MM_EnvironmentBase *env) = 0;

	/**
	 * Acquire shared VM access.
	 */
	virtual void
	acquireVMAccess()
	{
	}

	/**
	 * Releases shared VM access.
	 */
	virtual void
	releaseVMAccess()
	{
	}

	/**
	 * Acquire exclusive VM access.
	 */
	virtual void
	acquireExclusiveVMAccess()
	{
		_omrThread->exclusiveCount = 1;
		omrthread_monitor_enter(_env->getOmrVM()->_vmThreadListMutex);
	}

	/**
	 * Try and acquire exclusive access if no other thread is already requesting it.
	 * Make an attempt at acquiring exclusive access if the current thread does not already have it.  The
	 * attempt will abort if another thread is already going for exclusive, which means this
	 * call can return without exclusive access being held.  As well, this call will block for any other
	 * requesting thread, and so should be treated as a safe point.
	 * @note call can release VM access.
	 * @return true if exclusive access was acquired, false otherwise.
	 */
	virtual bool
	tryAcquireExclusiveVMAccess()
	{
		this->acquireExclusiveVMAccess();
		return true;
	}

	/**
	 * Releases exclusive VM access.
	 */
	virtual void
	releaseExclusiveVMAccess()
	{
		_omrThread->exclusiveCount = 0;
		omrthread_monitor_exit(_env->getOmrVM()->_vmThreadListMutex);
	}

	/**
	 * TODO fill in description
	 */
	virtual void exclusiveAccessForGCBeatenByOtherThread() {}
	virtual void exclusiveAccessForGCObtainedAfterBeatenByOtherThread() {}
	virtual void releaseCriticalHeapAccess(uintptr_t *data) {}
	virtual void reacquireCriticalHeapAccess(uintptr_t data) {}

	/**
	 * Give up exclusive access in preparation for transferring it to a collaborating thread (i.e. main-to-master or master-to-main)
	 * @return the exclusive count of the current thread before relinquishing 
	 */
	virtual uintptr_t relinquishExclusiveVMAccess() { return 0; }
	
	/**
	 * Assume exclusive access from a collaborating thread i.e. main-to-master or master-to-main)
	 * @param exclusiveCount the exclusive count to be restored 
	 */
	virtual void assumeExclusiveVMAccess(uintptr_t exclusiveCount) {}

	/**
	 * Checks to see if any thread has requested exclusive access
	 * @return true if a thread is waiting on exclusive access, false if not.
	 */
	virtual bool isExclusiveAccessRequestWaiting() { return false; }


	/**
	 * Saves the given object in the calling VMThread's allocateObjectSavePrivate1 field so that it will be kept alive and can be updated if moved during a GC.
	 *
	 * @param objectPtr The pointer to store (note that this MUST be an OBJECT POINTER and not a HEAP POINTER)
	 * @return true if the object was stored, false if the slot was already in use
	 */
	virtual bool saveObjects(omrobjectptr_t objectPtr) = 0;

	/**
	 * Fetches and clears the calling VMThread's allocateObjectSavePrivate1 field stored by a previous call to saveObjects.
	 *
	 * @param objectPtrIndirect The pointer to the slot which would be restored (note that the restored slot will be an OBJECT POINTER and not a HEAP POINTER)
	 */
	virtual void restoreObjects(omrobjectptr_t *objectPtrIndirect) = 0;

#if defined (OMR_GC_THREAD_LOCAL_HEAP)
	/**
	 * Disable inline TLH allocates by hiding the real heap allocation address from
	 * JIT/Interpreter in realHeapAlloc and setting heapALloc == HeapTop so TLH
	 * looks full.
	 *
	 */
	virtual void disableInlineTLHAllocate() = 0;

	/**
	 * Re-enable inline TLH allocate by restoring heapAlloc from realHeapAlloc
	 */
	virtual void enableInlineTLHAllocate() = 0;

	/**
	 * Determine if inline TLH allocate is enabled; its enabled if realheapAlloc is NULL.
	 * @return TRUE if inline TLH allocates currently enabled for this thread; FALSE otherwise
	 */
	virtual bool isInlineTLHAllocateEnabled() = 0;
#endif /* OMR_GC_THREAD_LOCAL_HEAP */

	virtual void parallelMarkTask_setup(MM_EnvironmentBase *env) = 0;
	virtual void parallelMarkTask_cleanup(MM_EnvironmentBase *env) = 0;

};

#endif /* ENVIRONMENTLANGUAGEINTERFACE_HPP_ */

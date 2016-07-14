/*******************************************************************************
 * Copyright (c) 2016 IBM Corporation
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    IBM Corporation - initial API and implementation and/or initial documentation
 *******************************************************************************/


#if !defined(GC_BASE_STANDARD_CONCURRENTSCAVENGETASK_HPP_)
#define GC_BASE_STANDARD_CONCURRENTSCAVENGETASK_HPP_

#include "omrcfg.h"

#if defined(OMR_GC_CONCURRENT_SCAVENGER)

#include "modronopt.h"

#include "ParallelScavengeTask.hpp"

class MM_ConcurrentScavengeTask : public MM_ParallelScavengeTask
{
	/* Data Members */
private:
	UDATA const _bytesToScan;	/**< The number of bytes that this must scan before it will stop trying to do more work */
	UDATA volatile _bytesScanned;	/**< The number of bytes scanned by this */
	volatile bool * const _forceExit;	/**< Shared state concurrently updated by an external thread to force the receiver to cause all threads to yield (by setting the destination of the pointer to true) */
protected:
public:

	enum ConcurrentAction {
		SCAVENGE_ALL = 1,
		SCAVENGE_ROOTS,
		SCAVENGE_SCAN,
		SCAVENGE_COMPLETE
	};

	/* _action should be private */
	ConcurrentAction _action;

	/* Member Functions */
private:
protected:
public:
	virtual UDATA getVMStateID()
	{
		return J9VMSTATE_GC_CONCURRENT_SCAVENGER;
	}

	UDATA getBytesScanned()
	{
		return _bytesScanned;
	}
	virtual void setup(MM_EnvironmentBase *env)
	{
		MM_ParallelScavengeTask::setup(env);
	}
	virtual void run(MM_EnvironmentBase *env);
	virtual void cleanup(MM_EnvironmentBase *env)
	{
		MM_ParallelScavengeTask::cleanup(env);
	}
	virtual bool shouldYieldFromTask(MM_EnvironmentBase *env)
	{
		return false;
	}

	MM_ConcurrentScavengeTask(MM_EnvironmentBase *env,
			MM_Dispatcher *dispatcher,
			MM_Scavenger *scavenger,
			ConcurrentAction action,
			uintptr_t bytesToScan,
			volatile bool *forceExit,
			MM_CycleState *cycleState) :
		MM_ParallelScavengeTask(env, dispatcher, scavenger, cycleState)
		, _bytesToScan(bytesToScan)
		, _bytesScanned(0)
		, _forceExit(forceExit)
		, _action(action)
	{
		_typeId = __FUNCTION__;
	};
};

#endif /* OMR_GC_CONCURRENT_SCAVENGER */
#endif /* GC_BASE_STANDARD_CONCURRENTSCAVENGETASK_HPP_ */

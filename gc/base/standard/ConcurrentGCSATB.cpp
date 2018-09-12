/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

/**
 * @file
 * @ingroup GC_Modron_Standard
 */

#include "omrcfg.h"

#if defined(OMR_GC_MODRON_CONCURRENT_MARK)

#define J9_EXTERNAL_TO_VM

#include "mmprivatehook.h"
#include "modronbase.h"
#include "modronopt.h"
#include "ModronAssertions.h"
#include "omr.h"

#include <string.h>

#include "ConcurrentGCSATB.hpp"
#include "AllocateDescription.hpp"

/**
 * Create new instance of ConcurrentGCIncrementalUpdate object.
 *
 * @return Reference to new MM_ConcurrentGCSATB object or NULL
 */
MM_ConcurrentGCSATB *
MM_ConcurrentGCSATB::newInstance(MM_EnvironmentBase *env)
{
	MM_ConcurrentGCSATB *concurrentGC =
			(MM_ConcurrentGCSATB *)env->getForge()->allocate(sizeof(MM_ConcurrentGCSATB),
					OMR::GC::AllocationCategory::FIXED, OMR_GET_CALLSITE());
	if (NULL != concurrentGC) {
		new(concurrentGC) MM_ConcurrentGCSATB(env);
		if (!concurrentGC->initialize(env)) {
			concurrentGC->kill(env);
			concurrentGC = NULL;
		}
	}

	return concurrentGC;
}

/**
 * Destroy instance of an ConcurrentGCIncrementalUpdate object.
 *
 */
void
MM_ConcurrentGCSATB::kill(MM_EnvironmentBase *env)
{
	tearDown(env);
	env->getForge()->free(this);
}


/**
 * Teardown a MM_ConcurrentGCSATB object
 * Destroy referenced objects and release
 * all allocated storage before MM_ConcurrentGCSATB object is freed.
 */
void
MM_ConcurrentGCSATB::tearDown(MM_EnvironmentBase *env)
{
	/* ..and then tearDown our super class */
	MM_ConcurrentGC::tearDown(env);
}

void
MM_ConcurrentGCSATB::reportConcurrentHalted(MM_EnvironmentBase *env)
{}

uintptr_t
MM_ConcurrentGCSATB::localMark(MM_EnvironmentBase *env, uintptr_t sizeToTrace)
{
	omrobjectptr_t objectPtr;
	uintptr_t gcCount = _extensions->globalGCStats.gcCount;

	env->_workStack.reset(env, _markingScheme->getWorkPackets());
	Assert_MM_true(env->_cycleState == NULL);
	Assert_MM_true(CONCURRENT_OFF < _stats.getExecutionMode());
	Assert_MM_true(_concurrentCycleState._referenceObjectOptions == MM_CycleState::references_default);
	env->_cycleState = &_concurrentCycleState;

	uintptr_t sizeTraced = 0;
	while(NULL != (objectPtr = (omrobjectptr_t)env->_workStack.popNoWait(env))) {
		/* Check for array scanPtr..if we find one ignore it*/
		if ((uintptr_t)objectPtr & PACKET_ARRAY_SPLIT_TAG){
			continue;
		} else {
			/* Else trace the object */
			sizeTraced += _markingScheme->scanObject(env, objectPtr, SCAN_REASON_PACKET, (sizeToTrace - sizeTraced));
		}

		/* Have we done enough tracing ? */
		if(sizeTraced >= sizeToTrace) {
			break;
		}

		/* Before we do any more tracing check to see if GC is waiting */
		if (env->isExclusiveAccessRequestWaiting()) {
			/* suspend con helper thread for pending GC */
			uintptr_t conHelperRequest = switchConHelperRequest(CONCURRENT_HELPER_MARK, CONCURRENT_HELPER_WAIT);
			Assert_MM_true(CONCURRENT_HELPER_MARK != conHelperRequest);
			break;
		}
	}

	/* Pop the top of the work packet if its a partially processed array tag */
	if ( ((uintptr_t)((omrobjectptr_t)env->_workStack.peek(env))) & PACKET_ARRAY_SPLIT_TAG) {
		env->_workStack.popNoWait(env);
	}

	/* STW collection should not occur while localMark is working */
	Assert_MM_true(gcCount == _extensions->globalGCStats.gcCount);

	flushLocalBuffers(env);
	env->_cycleState = NULL;

	return sizeTraced;
}

#endif /* OMR_GC_MODRON_CONCURRENT_MARK */

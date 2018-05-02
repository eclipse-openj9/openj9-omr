/*******************************************************************************
 * Copyright (c) 1991, 2018 IBM Corp. and others
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

#define J9_EXTERNAL_TO_VM

#include "omrcfg.h"

#include "EnvironmentStandard.hpp"
#include "GCExtensionsBase.hpp"
#if defined(OMR_GC_CONCURRENT_SCAVENGER)
#include "Scavenger.hpp"
#endif

MM_EnvironmentStandard *
MM_EnvironmentStandard::newInstance(MM_GCExtensionsBase *extensions, OMR_VMThread *omrVMThread)
{
	void *envPtr;
	MM_EnvironmentStandard *env = NULL;
	
	envPtr = (void *)pool_newElement(extensions->environments);
	if (envPtr) {
		env = new(envPtr) MM_EnvironmentStandard(omrVMThread);
		if (!env->initialize(extensions)) {
			env->kill();
			env = NULL;	
		}
	}	

	return env;
}

bool
MM_EnvironmentStandard::initialize(MM_GCExtensionsBase *extensions)
{
#if defined(OMR_GC_MODRON_SCAVENGER)
	_scavengerRememberedSet.count = 0;
	_scavengerRememberedSet.fragmentCurrent = NULL;
	_scavengerRememberedSet.fragmentTop = NULL;
	_scavengerRememberedSet.fragmentSize = (uintptr_t)OMR_SCV_REMSET_FRAGMENT_SIZE;
	_scavengerRememberedSet.parentList = &extensions->rememberedSet;
#endif

#if defined(OMR_GC_CONCURRENT_SCAVENGER)
	if (extensions->concurrentScavenger) {
		extensions->scavenger->mutatorSetupForGC(this);
	}
#endif

	/* initialize base class */
	return MM_EnvironmentBase::initialize(extensions);
}

void
MM_EnvironmentStandard::tearDown(MM_GCExtensionsBase *extensions)
{
	/* If we are in a middle of a concurrent GC, we may want to flush GC caches (if thread happens to do GC work) */
	flushGCCaches();
	/* tearDown base class */
	MM_EnvironmentBase::tearDown(extensions);
}

void
MM_EnvironmentStandard::flushNonAllocationCaches()
{
	MM_EnvironmentBase::flushNonAllocationCaches();

#if defined(OMR_GC_MODRON_SCAVENGER)
	if (getExtensions()->scavengerEnabled) {
		if (MUTATOR_THREAD == getThreadType()) {
			flushRememberedSet();
		}
	}
#endif /* OMR_GC_MODRON_SCAVENGER */
}

void
MM_EnvironmentStandard::flushGCCaches()
{
#if defined(OMR_GC_CONCURRENT_SCAVENGER)
	if (getExtensions()->concurrentScavenger) {

		/* When a GC Slave thread is created, a Thread object is created and is stored to a (Java) ThreadPool structure.
		 * If this GC Slave thread created during active scavenger cycle and doesn't do any actual work,
		 * it can still indirectly trigger a read barrier (if it's the first of 'mutator' threads to access ThreadPool),
		 * and copy ThreadPool instance into its copy cache.
		 * This special inactive GC thread will have non-empty copy cache at the beginning of the final STW phase of CS cycle,
		 * and should be treated same as a mutator thread.
		 */
		if ((GC_SLAVE_THREAD == getThreadType() && !isGCSlaveThreadActivated()) ||
				MUTATOR_THREAD == getThreadType()) {
			if (NULL != getExtensions()->scavenger) {
				getExtensions()->scavenger->threadFinalReleaseCopyCaches(this, this);
			}
		}
	}
#endif /* OMR_GC_CONCURRENT_SCAVENGER */
}

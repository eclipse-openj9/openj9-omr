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


/**
 * @file
 * @ingroup GC_Base
 */

#if !defined(OBJECTALLOCATIONINTERFACE_HPP_)
#define OBJECTALLOCATIONINTERFACE_HPP_

#include "omrcfg.h"
#include "modronbase.h"
#include "ModronAssertions.h"

#include "BaseVirtual.hpp"
#include "AllocationStats.hpp"

class MM_AllocateDescription;
class MM_EnvironmentBase;
class MM_FrequentObjectsStats;
class MM_MemoryPool;
class MM_MemorySpace;
class MM_MemorySubSpace;

/**
 * Abstract class definition for the object allocation interface.
 * This class represents the per-thread type associated to each MM_Environment that controls how a
 * thread allocates objects.  This control includes any caching measures (e.g., TLH).
 */
class MM_ObjectAllocationInterface : public MM_BaseVirtual
{
/*
 * Data members
 */
private:

protected:
	MM_EnvironmentBase *_owningEnv;  /**< The environment with which the receiver is associated */
	MM_AllocationStats _stats; /**< Allocation statistics for this allocation interface. */
	MM_FrequentObjectsStats* _frequentObjectsStats;

public:

/*
 * Function members
 */
private:

protected:
	/**
	 * Provide an initialization for the class
	 *
	 * @param env environment
	 * @return true if an initialization is successful
	 */
	virtual bool initialize(MM_EnvironmentBase *env) = 0;

	/**
	 * Tear Down for the class
	 *
	 * @param env environment
	 */
	virtual void tearDown(MM_EnvironmentBase *env) = 0;

	MM_ObjectAllocationInterface(MM_EnvironmentBase *env) :
		MM_BaseVirtual(),
		_owningEnv(env)
		,_stats()
		,_frequentObjectsStats(NULL)
	{
		_typeId = __FUNCTION__;
	};

public:
	MM_AllocationStats* getAllocationStats() { return &_stats; }
	MM_FrequentObjectsStats* getFrequentObjectsStats() { return _frequentObjectsStats; }
	MM_EnvironmentBase *getOwningEnv() { return _owningEnv; }

	virtual void kill(MM_EnvironmentBase *env) = 0;

	virtual bool forceRefreshTLH(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription) {
		return true;
	}

	virtual void *allocateObject(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription, MM_MemorySpace *memorySpace, bool shouldCollectOnFailure) = 0;
	virtual void *allocateArray(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription, MM_MemorySpace *memorySpace, bool shouldCollectOnFailure) = 0;
	/**
	 * Allocate the arraylet spine.
	 */
	virtual void *
	allocateArrayletSpine(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription, MM_MemorySpace *memorySpace, bool shouldCollectOnFailure)
	{
		Assert_MM_unreachable();
		return NULL;
	}

	/**
	 * Allocate an arraylet leaf.
	 */
	virtual void *
	allocateArrayletLeaf(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription, MM_MemorySpace *memorySpace, bool shouldCollectOnFailure)
	{
		Assert_MM_unreachable();
		return NULL;
	}

#if defined(OMR_GC_THREAD_LOCAL_HEAP)
	virtual void *allocateTLH(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, MM_MemorySubSpace *memorySubSpace, MM_MemoryPool *memoryPool);
#endif /* OMR_GC_THREAD_LOCAL_HEAP */

	virtual void flushCache(MM_EnvironmentBase *env);
	virtual void restartCache(MM_EnvironmentBase *env);
	
	virtual void enableCachedAllocations(MM_EnvironmentBase* env) {};
	virtual void disableCachedAllocations(MM_EnvironmentBase* env) {};
	virtual bool cachedAllocationsEnabled(MM_EnvironmentBase* env) { return true; }
	
};
#endif /* OBJECTALLOCATIONINTERFACE_HPP_ */

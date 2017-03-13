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
 * @ingroup GC_Modron_Metronome
 */

#if !defined(CONFIGURATIONSEGREGATED_HPP_)
#define CONFIGURATIONSEGREGATED_HPP_

#include "omrcfg.h"

#include "Configuration.hpp"

#if defined(OMR_GC_SEGREGATED_HEAP)

class MM_EnvironmentBase;
class MM_GlobalAllocationManagerSegregated;
class MM_GlobalCollector;
class MM_Heap;
class MM_SegregatedAllocationTracker;

class MM_ConfigurationSegregated : public MM_Configuration
{
	/*
	 * Data members
	 */
public:
protected:
private:
	static const uintptr_t SEGREGATED_REGION_SIZE_BYTES = (64 * 1024);
	static const uintptr_t SEGREGATED_ARRAYLET_LEAF_SIZE_BYTES = OMR_SIZECLASSES_MAX_SMALL_SIZE_BYTES;

	/*
	 * Function members
	 */
public:
	static MM_Configuration *newInstance(MM_EnvironmentBase *env);

	virtual MM_GlobalCollector *createGlobalCollector(MM_EnvironmentBase *env);
	virtual MM_Heap *createHeapWithManager(MM_EnvironmentBase *env, uintptr_t heapBytesRequested, MM_HeapRegionManager *regionManager);
	virtual MM_HeapRegionManager *createHeapRegionManager(MM_EnvironmentBase *env);
	virtual MM_MemorySpace *createDefaultMemorySpace(MM_EnvironmentBase *env, MM_Heap *heap, MM_InitializationParameters *parameters);
	virtual J9Pool *createEnvironmentPool(MM_EnvironmentBase *env);
	virtual MM_Dispatcher *createDispatcher(MM_EnvironmentBase *env, omrsig_handler_fn handler, void* handler_arg, uintptr_t defaultOSStackSize);
	
	virtual void defaultMemorySpaceAllocated(MM_GCExtensionsBase *extensions, void* defaultMemorySpace);
	
	MM_ConfigurationSegregated(MM_EnvironmentBase *env)
		: MM_Configuration(env, gc_policy_metronome, mm_regionAlignment, SEGREGATED_REGION_SIZE_BYTES, SEGREGATED_ARRAYLET_LEAF_SIZE_BYTES, gc_modron_wrtbar_none, gc_modron_allocation_type_segregated)
	{
		_typeId = __FUNCTION__;
	};

protected:
	virtual bool initialize(MM_EnvironmentBase *env);
	virtual void tearDown(MM_EnvironmentBase *env);
	virtual MM_EnvironmentBase *allocateNewEnvironment(MM_GCExtensionsBase *extensions, OMR_VMThread *omrVMThread);
	virtual bool initializeEnvironment(MM_EnvironmentBase *env);

private:
};

#endif /* OMR_GC_SEGREGATED_HEAP */

#endif /* CONFIGURATIONSEGREGATED_HPP_ */

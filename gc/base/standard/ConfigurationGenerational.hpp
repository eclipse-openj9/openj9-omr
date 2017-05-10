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
 * @ingroup GC_Modron_Standard
 */

#if !defined(CONFIGURATIONGENERATIONAL_HPP_)
#define CONFIGURATIONGENERATIONAL_HPP_

#include "omrcfg.h"

#include "ConfigurationStandard.hpp"

#if defined(OMR_GC_MODRON_SCAVENGER)

class MM_EnvironmentBase;
class MM_MemorySubSpaceSemiSpace;
class MM_Scavenger;

class MM_ConfigurationGenerational : public MM_ConfigurationStandard
{
/* Data members / Types */
public:
protected:
private:

/* Methods */
public:
	static MM_Configuration *newInstance(MM_EnvironmentBase *env);

	virtual MM_MemorySpace *createDefaultMemorySpace(MM_EnvironmentBase *env, MM_Heap *heap, MM_InitializationParameters *parameters);
	virtual MM_Heap *createHeapWithManager(MM_EnvironmentBase *env, UDATA heapBytesRequested, MM_HeapRegionManager *regionManager);

	virtual void defaultMemorySpaceAllocated(MM_GCExtensionsBase *extensions, void* defaultMemorySpace);
	
	MM_ConfigurationGenerational(MM_EnvironmentBase *env)
		: MM_ConfigurationStandard(env, gc_policy_gencon, calculateDefaultRegionSize(env))
	{
		_typeId = __FUNCTION__;
	};
	
protected:
	MM_MemorySubSpaceSemiSpace *createSemiSpace(MM_EnvironmentBase *envBase, MM_Heap *heap, MM_Scavenger *scavenger, MM_InitializationParameters *parameters, UDATA numaNode = UDATA_MAX);
private:
	uintptr_t calculateDefaultRegionSize(MM_EnvironmentBase *env);
};

#endif /* defined(OMR_GC_MODRON_SCAVENGER) */

#endif /* CONFIGURATIONGENERATIONAL_HPP_ */

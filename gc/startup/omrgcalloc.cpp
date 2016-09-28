/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
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

#include "omr.h"
#include "omrgc.h"
#include "objectdescription.h"

#include "AllocateInitialization.hpp"
#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"
#include "Heap.hpp"
#include "omrgcstartup.hpp"

omrobjectptr_t
OMR_GC_Allocate(OMR_VMThread * omrVMThread, uintptr_t allocationCategory, uintptr_t size, uintptr_t allocateFlags)
{
	MM_EnvironmentBase *env = MM_EnvironmentBase::getEnvironment(omrVMThread);
	uintptr_t adjustedSize = env->getExtensions()->objectModel.adjustSizeInBytes(size);
	uintptr_t adjustedFlags = allocateFlags & ~(uintptr_t)OMR_GC_ALLOCATE_OBJECT_NO_GC;

	omrobjectptr_t objectPtr = NULL;
	omr_error_t retry = OMR_ERROR_RETRY;
	while ((NULL == objectPtr) && (retry == OMR_ERROR_RETRY)) {
		MM_AllocateInitialization withGc(env, allocationCategory, adjustedSize, adjustedFlags);
		if (withGc.initializeAllocateDescription(env)) {
			objectPtr = withGc.allocateAndInitializeObject(omrVMThread);
			if ((NULL == objectPtr) && (NULL == env->getExtensions()->getGlobalCollector())) {
				/* Lazily create the collector and try to allocate again. */
				retry = OMR_GC_InitializeCollector(omrVMThread);
			}
		}
	}
	return objectPtr;
}

omrobjectptr_t
OMR_GC_AllocateNoGC(OMR_VMThread * omrVMThread, uintptr_t allocationCategory, uintptr_t size, uintptr_t allocateFlags)
{
	MM_EnvironmentBase *env = MM_EnvironmentBase::getEnvironment(omrVMThread);
	uintptr_t adjustedSize = env->getExtensions()->objectModel.adjustSizeInBytes(size);
	uintptr_t adjustedFlags = allocateFlags | (uintptr_t)OMR_GC_ALLOCATE_OBJECT_NO_GC;

	omrobjectptr_t objectPtr = NULL;
	MM_AllocateInitialization noGc(env, allocationCategory, adjustedSize, adjustedFlags);
	if (noGc.initializeAllocateDescription(env)) {
		objectPtr = noGc.allocateAndInitializeObject(omrVMThread);
	}
	return objectPtr;
}

omr_error_t
OMR_GC_SystemCollect(OMR_VMThread* omrVMThread, uint32_t gcCode)
{
	omr_error_t result = OMR_ERROR_NONE;
	MM_EnvironmentBase *env = MM_EnvironmentBase::getEnvironment(omrVMThread);
	MM_GCExtensionsBase *extensions = env->getExtensions();
	if (NULL == extensions->getGlobalCollector()) {
		result = OMR_GC_InitializeCollector(omrVMThread);
	}
	if (OMR_ERROR_NONE == result) {
		extensions->heap->systemGarbageCollect(env, gcCode);
	}
	return result;
}

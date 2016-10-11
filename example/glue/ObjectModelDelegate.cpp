/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2016
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

#include "EnvironmentBase.hpp"
#include "AllocateInitialization.hpp"
#include "GCExtensionsBase.hpp"
#include "ObjectAllocationModel.hpp"
#include "ObjectModel.hpp"

omrobjectptr_t
GC_ObjectModelDelegate::initializeAllocation(MM_EnvironmentBase *env, void *allocatedBytes, MM_AllocateInitialization *allocateInitialization)
{
	Assert_MM_true(MM_ObjectAllocationModel::allocation_category_example == allocateInitialization->getAllocationCategory());

	omrobjectptr_t objectPtr = NULL;
	if (NULL != allocatedBytes) {
		MM_ObjectAllocationModel *objectAllocationModel = (MM_ObjectAllocationModel *)allocateInitialization;
		objectPtr = objectAllocationModel->initializeObject(env, allocatedBytes);
	}
	return objectPtr;
}

#if defined(OMR_GC_MODRON_SCAVENGER)
void
GC_ObjectModelDelegate::calculateObjectDetailsForCopy(MM_EnvironmentBase *env, MM_ForwardedHeader *forwardedHeader, uintptr_t *objectCopySizeInBytes, uintptr_t *reservedObjectSizeInBytes, uintptr_t *hotFieldAlignmentDescriptor)
{
	*objectCopySizeInBytes = getForwardedObjectSizeInBytes(forwardedHeader);
	*reservedObjectSizeInBytes = env->getExtensions()->objectModel.adjustSizeInBytes(*objectCopySizeInBytes);
	*hotFieldAlignmentDescriptor = 0;
}
#endif /* defined(OMR_GC_MODRON_SCAVENGER) */

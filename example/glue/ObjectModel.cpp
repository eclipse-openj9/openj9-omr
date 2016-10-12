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

#include "ModronAssertions.h"

#include "AllocateDescription.hpp"
#include "AllocateInitialization.hpp"
#include "EnvironmentBase.hpp"
#include "ObjectModel.hpp"

omrobjectptr_t
GC_ObjectModel::initializeAllocation(MM_EnvironmentBase *env, void *allocatedBytes, MM_AllocateInitialization *allocateInitialization)
{
	Assert_MM_true(OMR_EXAMPLE_ALLOCATION_CATEGORY == allocateInitialization->getAllocationCategory());

	omrobjectptr_t objectPtr = (omrobjectptr_t)allocatedBytes;
	if (NULL != objectPtr) {
		setObjectSize(objectPtr, allocateInitialization->getAllocateDescription()->getBytesRequested(), false);
	}
	return objectPtr;
}

/*******************************************************************************
 * Copyright (c) 2015, 2017 IBM Corp. and others
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

#ifndef MM_OMRGCAPI_HPP_
#define MM_OMRGCAPI_HPP_

/*
 * @ddr_namespace: default
 */

#include "omr.h"
#include "objectdescription.h"
#include "omrcomp.h"
#include "j9nongenerated.h"

/* Runtime API (C) */
#ifdef __cplusplus
extern "C" {
#endif

/* Allocation description will be initialized in call */
omrobjectptr_t OMR_GC_AllocateObject(OMR_VMThread * omrVMThread, uintptr_t allocationCategory, uintptr_t requiredSizeInBytes, uintptr_t objectAllocationFlags);

omr_error_t OMR_GC_SystemCollect(OMR_VMThread* omrVMThread, uint32_t gcCode);

#ifdef __cplusplus
} /* extern "C" { */
#endif

/* Runtime API (C++) */
#ifdef __cplusplus
class MM_AllocateInitialization;
/* Caller is expected to initialize the allocation description (MM_AllocateInitialization::getAllocateDescription()) prior to call */
omrobjectptr_t OMR_GC_AllocateObject(OMR_VMThread * omrVMThread, MM_AllocateInitialization *allocator);
#endif

#endif /* MM_OMRGCAPI_HPP_ */

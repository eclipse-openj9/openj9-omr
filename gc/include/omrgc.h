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

#ifndef MM_OMRGCAPI_HPP_
#define MM_OMRGCAPI_HPP_

/*
 * @ddr_namespace: default
 */

#include "omr.h"
#include "objectdescription.h"
#include "omrcomp.h"
#include "j9nongenerated.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Runtime API */

/* Base GC allocation flag bits are defined in omrgc.h and languages can define additional flags. The
 * OMR_GC_Allocate*(...) methods receive these flags and pass them through to the OMR allocator with
 * the exception of the OMR_GC_ALLOCATE_NO_GC flag, which these methods set or clear as required.
 *
 * Allocation categories are expected to be defined in the language object model (glue/GC_ObjectModel).
 */
omrobjectptr_t OMR_GC_Allocate(OMR_VMThread * omrVMThread, uintptr_t allocationCategory, uintptr_t size, uintptr_t allocateFlags);

omrobjectptr_t OMR_GC_AllocateNoGC(OMR_VMThread * omrVMThread, uintptr_t allocationCategory, uintptr_t size, uintptr_t allocateFlagss);

omr_error_t OMR_GC_SystemCollect(OMR_VMThread* omrVMThread, uint32_t gcCode);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MM_OMRGCAPI_HPP_ */

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

class MM_AllocateInitialization;

#ifdef __cplusplus
extern "C" {
#endif

/* Runtime API */

/* Caller is expected to initialize the allocation description (MM_AllocateInitialization::getAllocateDescription()) prior to call */
omrobjectptr_t OMR_GC_AllocateObject(OMR_VMThread * omrVMThread, MM_AllocateInitialization *allocator);

omr_error_t OMR_GC_SystemCollect(OMR_VMThread* omrVMThread, uint32_t gcCode);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* MM_OMRGCAPI_HPP_ */

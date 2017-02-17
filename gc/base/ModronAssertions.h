/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2011, 2017
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

#if !defined(MODRONASSERTIONS_H__)
#define MODRONASSERTIONS_H__

/*
 * @ddr_namespace: default
 */

#include "omrcfg.h"
#include "omrcomp.h"
#include "omr.h"
#include "ut_j9mm.h"

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Currently, there are startup errors pointed out by these assertions so only enable them on combination spec until they are fixed for everyone */
#if defined(OMR_GC_DEBUG_ASSERTS)

extern void omrGcDebugAssertionOutput(OMR_VMThread *omrVMThread, const char *format, ...);

#define Assert_MM_true(arg) \
	do {\
		if (!(arg)) {\
			Assert_MM_true_internal(false && (arg));\
			assert(false && (arg));\
		}\
	} while(0)

#define Assert_MM_false(arg) \
	do {\
		if (arg) {\
			Assert_MM_false_internal(true || (arg));\
			assert(false && !(arg));\
		}\
	} while(0)

#define Assert_MM_unreachable() \
	do {\
        Assert_MM_unreachable_internal();\
        assert(0);\
	} while(0)

#define Assert_MM_unimplemented() \
	do {\
        Assert_MM_unimplemented_internal();\
        assert(0);\
	} while(0)

#define Assert_MM_invalidJNICall() \
	do {\
		Assert_MM_invalidJNICall_internal();\
        assert(0);\
	} while(0)

/* One printed parameter macro */
#define Assert_GC_true_with_message(__env__,__condition,__message,__parameter) \
do {\
	if(!(__condition)) {\
		omrGcDebugAssertionOutput(__env__->getOmrVMThread(), __message, __parameter);\
		Assert_MM_unreachable();\
	}\
} while (false)

/* Two printed parameters macro */
#define Assert_GC_true_with_message2(__env__,__condition,__message,__parameter1,__parameter2) \
do {\
	if(!(__condition)) {\
		omrGcDebugAssertionOutput(__env__->getOmrVMThread(), __message, __parameter1, __parameter2);\
		Assert_MM_unreachable();\
	}\
} while (false)

/* THree printed parameters macro */
#define Assert_GC_true_with_message3(__env__,__condition,__message,__parameter1,__parameter2,__parameter3) \
do {\
	if(!(__condition)) {\
		omrGcDebugAssertionOutput(__env__->getOmrVMThread(), __message, __parameter1, __parameter2, __parameter3);\
		Assert_MM_unreachable();\
	}\
} while (false)

/* Four printed parameters macro */
#define Assert_GC_true_with_message4(__env__,__condition,__message,__parameter1,__parameter2,__parameter3,__parameter4) \
do {\
	if(!(__condition)) {\
		omrGcDebugAssertionOutput(__env__->getOmrVMThread(), __message, __parameter1, __parameter2, __parameter3, __parameter4);\
		Assert_MM_unreachable();\
	}\
} while (false)

#define Assert_MM_objectAligned(__env__,__pointer) \
do {\
	if((uintptr_t)(__pointer) & (__env__->getObjectAlignmentInBytes() - 1)) {\
		Assert_GC_true_with_message2(__env__,false, "Pointer: %p has is not object aligned (to %zu bytes) \n", __pointer, __env__->getObjectAlignmentInBytes());\
	}\
} while(false)
#else /* defined(OMR_GC_DEBUG_ASSERTS) */


#define Assert_MM_true(arg) Assert_MM_true_internal(arg)
#define Assert_MM_false(arg) Assert_MM_false_internal(arg)
#define Assert_MM_unreachable() Assert_MM_unreachable_internal()
#define Assert_MM_unimplemented() Assert_MM_unimplemented_internal()
#define Assert_MM_invalidJNICall() Assert_MM_invalidJNICall_internal()
#define Assert_GC_true_with_message(__env__,__condition,__message,__parameter) Assert_MM_true(__condition)
#define Assert_GC_true_with_message2(__env__,__condition,__message,__parameter1,__parameter2) Assert_MM_true(__condition)
#define Assert_GC_true_with_message3(__env__,__condition,__message,__parameter1,__parameter2,__parameter3) Assert_MM_true(__condition)
#define Assert_GC_true_with_message4(__env__,__condition,__message,__parameter1,__parameter2,__parameter3,__parameter4) Assert_MM_true(__condition)
#define Assert_MM_objectAligned(__env__,__pointer) Assert_MM_true_internal((uintptr_t)(__pointer) & (__env__->getObjectAlignmentInBytes() - 1))
#endif /* defined(OMR_GC_DEBUG_ASSERTS) */


#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* MODRONASSERTIONS_H__ */

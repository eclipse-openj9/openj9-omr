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

#include <stdarg.h>

#include "omr.h"
#include "ModronAssertions.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(OMR_GC_DEBUG_ASSERTS)

#define ASSERTION_MESSAGE_BUFFER_SIZE	((uintptr_t)256)

/*
 * The purpose of moving of the body of macros GC Assertions with message to separate function is to prevent inlining
 * on fast path to keep stack frames small
 */

void
omrGcDebugAssertionOutput(OMR_VMThread *omrVMThread, const char *format, ...)
{
	char buffer[ASSERTION_MESSAGE_BUFFER_SIZE];
	OMRPortLibrary *portLibrary = omrVMThread->_vm->_runtime->_portLibrary;

	va_list args;
	va_start(args, format);
	portLibrary->str_vprintf(portLibrary, buffer, ASSERTION_MESSAGE_BUFFER_SIZE, format, args);
	va_end(args);

	Trc_MM_AssertionWithMessage_outputMessage(omrVMThread->_language_vmthread, buffer);
	portLibrary->tty_printf(portLibrary, "%s", buffer);
}

#endif /* defined(OMR_GC_DEBUG_ASSERTS) */

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

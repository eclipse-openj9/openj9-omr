/*******************************************************************************
 * Copyright IBM Corp. and others 2011
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
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
 * on fast path to keep stack frames small.
 */

/**
 * Print assertion message and duplicate it in the snap trace.
 * Internal implementation.
 *
 * @param portLibrary - pointer to Port Library
 * @param omrVMThread - pointer to current OMR VM Thread
 * @param format - pointer to format string
 * @param args - variadic list of parameters
 */
void
omrGcDebugAssertionOutput(OMRPortLibrary *portLibrary, OMR_VMThread *omrVMThread, const char *format, va_list args)
{
	char buffer[ASSERTION_MESSAGE_BUFFER_SIZE];

	portLibrary->str_vprintf(portLibrary, buffer, ASSERTION_MESSAGE_BUFFER_SIZE, format, args);

	if (NULL != omrVMThread) {
		/* omrVMThread might be NULL if Environment is partially initialized at startup time. */
		Trc_MM_AssertionWithMessage_outputMessage(omrVMThread->_language_vmthread, buffer);
	}
	portLibrary->tty_printf(portLibrary, "%s", buffer);
}

/**
 * Print assertion message and duplicate it in the snap trace.
 * Front end call for the case OMR_VMThread is passed.
 *
 * @param portLibrary - pointer to Port Library
 * @param omrVMThread - pointer to current OMR VM Thread
 * @param format - pointer to format string
 * @param ... - variadic list of parameters
 */
void
omrGcDebugAssertionOutputThread(OMRPortLibrary *portLibrary, OMR_VMThread *omrVMThread, const char *format, ...)
{
	if (NULL != portLibrary) {
		va_list args;
		va_start(args, format);
		omrGcDebugAssertionOutput(portLibrary, omrVMThread, format, args);
		va_end(args);
	}
}

/**
 * Print assertion message and duplicate it in the snap trace.
 * Front end call for the case OMR_VM is passed.
 * Current OMR VM thread is going to be read from TLS area.
 *
 * @param omrVM - pointer to OMR VM
 * @param format - pointer to format string
 * @param ... - variadic list of parameters
 */
void
omrGcDebugAssertionOutputVM(OMR_VM *omrVM, const char *format, ...)
{
	if ((NULL != omrVM) && (NULL != omrVM->_runtime)) {
		OMRPortLibrary *portLibrary = omrVM->_runtime->_portLibrary;

		if (NULL != portLibrary) {
			OMR_VMThread *omrVMThread = NULL;
			omrthread_t omrthread = omrthread_self();

			if ((NULL != omrthread) && (omrVM->_vmThreadKey > 0)) {
				/* Read omrVMThread from thread's TLS area. */
				omrVMThread = (OMR_VMThread *)omrthread_tls_get(omrthread, omrVM->_vmThreadKey);
			}

			/* NULL for omrVMThread is accepted. */
			va_list args;
			va_start(args, format);
			omrGcDebugAssertionOutput(portLibrary, omrVMThread, format, args);
			va_end(args);
		}
	}
}

#endif /* defined(OMR_GC_DEBUG_ASSERTS) */

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

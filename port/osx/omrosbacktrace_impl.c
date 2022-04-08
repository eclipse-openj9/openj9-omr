/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
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

/**
 * @file
 * @ingroup Port
 * @brief Stack backtracing support
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "omrport.h"
#include "omrportpriv.h"
#include "omrsignal_context.h"

#include <dlfcn.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include "omrintrospect.h"

static uintptr_t protectedBacktrace(OMRPortLibrary *port, void *arg);
static uintptr_t backtrace_sigprotect(OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, void **address_array, uintptr_t capacity);

typedef struct frameData {
	void **address_array;
	uintptr_t capacity;
} frameData;

/**
 * Handles segfaults where either an invalid address is called or a signal was received in kernel mode.
 * Both cases leave the ip register address invalid and libunwind fails to iterate past _sigtramp. This
 * function fetches previous frame information and restores correct register values to the unwind cursor.
 * It also records the stack frame addresses of the remaining frames.
 *
 * @param[in] cursor keeps track of stack unwind state
 * @param[out] array contains the backtrace addresses to be returned
 * @param[in] size size of the array
 * @param[in] nAddr number of addresses currently recorded in the array
 *
 * @return total number of addresses recorded in the array
 */
static uintptr_t
handle_sigtramp(unw_cursor_t *cursor, void **array, uintptr_t size, uintptr_t nAddr)
{
#if defined(OMR_ARCH_X86)
	unw_word_t prevFrame = 0;
	unw_word_t ip = 0;
	ucontext_t *uc = NULL;
	OMRUnixSignalInfo info;
	const char *name = NULL;
	void *reg = NULL;

	/* ucontext for frame prior to _sigtramp is stored in rbx. Fetch previous frame info from it.
	 * See https://opensource.apple.com/source/Libc/Libc-498/x86_64/sys/_sigtramp.s.auto.html
	 */
	unw_get_reg(cursor, UNW_X86_64_RBX, &prevFrame);
	uc = (ucontext_t *)prevFrame;
	memset(&info, 0, sizeof(info));
	info.platformSignalInfo.context = uc;

	/* Copy the necessary machine regs to cursor so we can iterate the previous frame.
	 * See https://opensource.apple.com/source/libunwind/libunwind-35.4/src/unw_getcontext.s.auto.html
	 */
	infoForControl(NULL, &info, OMRPORT_SIG_CONTROL_BP, &name, &reg);
	unw_set_reg(cursor, UNW_X86_64_RBP, *(unw_word_t *)reg);
	infoForControl(NULL, &info, OMRPORT_SIG_CONTROL_SP, &name, &reg);
	/* Store call-site sp (skip return address) into RSP. */
	unw_set_reg(cursor, UNW_X86_64_RSP, (*(unw_word_t *)reg) + 8);
	infoForControl(NULL, &info, OMRPORT_SIG_CONTROL_PC, &name, &reg);
	ip = *(unw_word_t *)reg;

	if ((size >= 2) && (nAddr <= (size - 2))) {
		/* libunwind failed to iterate past _sigtramp because of a call to invalid address or a signal
		 * received in kernel (right after syscall). In either case, the return address will be
		 * pointed to by rsp: in the first case, return address pushed to stack right before call,
		 * in the second case, syscall wrapper's caller pushed its return address. Record the ip and
		 * skip the frame.
		 */
		array[nAddr] = (void *)ip;
		nAddr += 1;
		infoForControl(NULL, &info, OMRPORT_SIG_CONTROL_SP, &name, &reg);
		ip = **(unw_word_t **)reg;

		/* Record the return address. */
		array[nAddr] = (void *)ip;
		nAddr += 1;
		unw_set_reg(cursor, UNW_REG_IP, ip);

		while ((nAddr < size) && (unw_step(cursor) > 0)) {
			unw_get_reg(cursor, UNW_REG_IP, &ip);
			array[nAddr] = (void *)ip;
			nAddr += 1;
		}
	}
#endif /* defined(OMR_ARCH_X86) */

	return nAddr;
}

/**
 * Obtains the stack trace for the current thread using libunwind and stores it in array. libunwind is used
 * instead of libc backtrace as it handles signal handler frames correctly, whereas backtrace may return an
 * incorrect frame for the frame before the signal handler.
 *
 * @param[out] array the array to hold the frame addresses
 * @param[in] size capacity of the array
 *
 * @return the number of addresses recorded in the array
 */
static uintptr_t
unw_backtrace(void **array, uintptr_t size)
{
	unw_cursor_t cursor;
	unw_cursor_t prev;
	unw_context_t uc;
	unw_word_t ip = 0;
	char buf[64];
	uintptr_t nAddr = 0;

	unw_getcontext(&uc);
	unw_init_local(&cursor, &uc);
	while ((nAddr < size) && (unw_step(&cursor) > 0)) {
		unw_get_reg(&cursor, UNW_REG_IP, &ip);
		array[nAddr] = (void *)ip;
		nAddr += 1;

		/* Save frame for check below. */
		memcpy(&prev, &cursor, sizeof(cursor));
	}

	/* Darwin's libunwind cannot handle sigtramp in some cases. If the final frame iterated is sigtramp,
	 * there are more frames but need to manually restore registers.
	 */
	if (0 == unw_get_proc_name(&prev, buf, sizeof(buf), &ip)) {
		if (0 == strcmp(buf, "_sigtramp")) {
			nAddr = handle_sigtramp(&prev, array, size, nAddr);
		}
	}

	return nAddr;
}

/**
 * NULL handler. We only care about preventing the signal from propagating up the call stack, no need to do
 * anything in the handler.
 *
 * @param[in] portLibrary a pointer to an initialized port library
 * @param[in] gpType type of signal
 * @param[in] gpInfo contains data about signal
 * @param[in] userData data passed in by user
 *
 * @return OMRPORT_SIG_EXCEPTION_RETURN indicating that a signal was received
 */
static uintptr_t
handler(OMRPortLibrary *portLibrary, uint32_t gpType, void *gpInfo, void *userData)
{
	return OMRPORT_SIG_EXCEPTION_RETURN;
}

/**
 * Wrapped call to libunwind backtrace functions.
 *
 * @param[in] port a pointer to an initialized port library
 * @param[out] arg a pointer to the addresses array and capacity
 *
 * @return the number of addresses recorded in the array
 */
static uintptr_t
protectedBacktrace(OMRPortLibrary *port, void *arg)
{
	frameData *addresses = (frameData *)arg;
	return unw_backtrace(addresses->address_array, addresses->capacity);
}

/**
 * Provides signal protection for the unw_backtrace call. If the stack walk causes a segfault then we check the output
 * array to see if we got any frames as we may be able to provide a partial backtrace. We also record that the stack walk
 * was incomplete so that information is available after the function returns.
 *
 * @param[in] portLibrary a pointer to an initialized port library
 * @param[out] threadInfo the thread structure to hold an error code, if any. Must not be NULL
 * @param[out] address_array the array to hold the frame addresses
 * @param[in] capacity capacity of the array
 *
 * @return the number of addresses recorded in the array
 */
static uintptr_t
backtrace_sigprotect(OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, void **address_array, uintptr_t capacity)
{
	uintptr_t ret = 0;

	if (NULL == omrthread_self()) {
		ret = unw_backtrace(address_array, capacity);
	} else {
		frameData args;
		args.address_array = address_array;
		args.capacity = capacity;

		if (0 != portLibrary->sig_protect(portLibrary, protectedBacktrace, &args, handler, NULL,
				OMRPORT_SIG_FLAG_SIGALLSYNC | OMRPORT_SIG_FLAG_MAY_RETURN, &ret)
		) {
			/* check to see if there were any addresses populated */
			for (ret = 0; (ret < args.capacity) && (NULL != address_array[ret]); ret++) {
				/* increment ret to the number of filled addresses */
			}

			threadInfo->error = FAULT_DURING_BACKTRACE;
		}
	}

	return ret;
}

/**
 * This function constructs a backtrace from a CPU context. Generally there are only one or two
 * values in the context that are actually used to construct the stack but these vary by platform
 * so arn't detailed here. If no heap is specified then this function will use malloc to allocate
 * the memory necessary for the stack frame structures which must be freed by the caller.
 *
 * @param portLibrary a pointer to an initialized port library
 * @param threadInfo the thread structure we want to attach the backtrace to. Must not be NULL.
 * @param heap a heap from which to allocate any necessary memory. If NULL malloc is used instead.
 * @param signalInfo a platform signal context. If not null the context held in threadInfo is replaced
 *  	  with the signal context before the backtrace is generated.
 *
 * @return the number of frames in the backtrace.
 */
uintptr_t
omrintrospect_backtrace_thread_raw(OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, J9Heap *heap, void *signalInfo)
{
	void *addresses[50];
	J9PlatformStackFrame **nextFrame = NULL;
	J9PlatformStackFrame *junkFrames = NULL;
	J9PlatformStackFrame *prevFrame = NULL;
	OMRUnixSignalInfo *sigInfo = (OMRUnixSignalInfo *)signalInfo;
	uintptr_t i = 0;
	uintptr_t discard = 0;
	uintptr_t ret = 0;
	const char *regName = NULL;
	void **faultingAddress = NULL;

	if (NULL == threadInfo) {
		return 0;
	}

	/* if we've been passed a port library wrapped signal, then extract info from there */
	if (NULL != sigInfo) {
		/* get the faulting address so we can discard frames that are part of the signal handling */
		infoForControl(portLibrary, sigInfo, OMRPORT_SIG_CONTROL_PC, &regName, (void **)&faultingAddress);
	}

	memset(addresses, 0, sizeof(addresses));
	ret = backtrace_sigprotect(portLibrary, threadInfo, addresses, sizeof(addresses) / sizeof(addresses[0]));

	nextFrame = &threadInfo->callstack;
	for (i = 0; i < ret; i++) {
		if (NULL != heap) {
			*nextFrame = portLibrary->heap_allocate(portLibrary, heap, sizeof(**nextFrame));
		} else {
			*nextFrame = portLibrary->mem_allocate_memory(portLibrary, sizeof(**nextFrame), OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		}

		if (NULL == *nextFrame) {
			if (0 == threadInfo->error) {
				threadInfo->error = ALLOCATION_FAILURE;
			}

			break;
		}

		(*nextFrame)->parent_frame = NULL;
		(*nextFrame)->symbol = NULL;
		(*nextFrame)->instruction_pointer = (uintptr_t)addresses[i];
		(*nextFrame)->stack_pointer = 0;
		(*nextFrame)->base_pointer = 0;

		nextFrame = &(*nextFrame)->parent_frame;

		/* Check to see if we should truncate the stack trace to omit handler frames. The faulting address may
		 * be one less than the address reported by core dump and backtrace on osx.
		 */
		if ((NULL != prevFrame)
			&& (NULL != faultingAddress)
			&& ((*faultingAddress == addresses[i]) || ((*faultingAddress + 1) == addresses[i]))
		) {
			/* discard all frames up to this point */
			junkFrames = threadInfo->callstack;
			threadInfo->callstack = prevFrame->parent_frame;

			/* break the link so we can free the junk */
			prevFrame->parent_frame = NULL;

			/* correct the next frame */
			nextFrame = &threadInfo->callstack->parent_frame;
			/* record how many frames we discard */
			discard = i + 1;
		}

		/* save the frame we've just set up so that we can blank it if necessary */
		if (NULL == prevFrame) {
			prevFrame = threadInfo->callstack;
		} else {
			prevFrame = prevFrame->parent_frame;
		}
	}

	/* if we discarded any frames then free them */
	while (NULL != junkFrames) {
		J9PlatformStackFrame *tmp = junkFrames;
		junkFrames = tmp->parent_frame;

		if (NULL != heap) {
			portLibrary->heap_free(portLibrary, heap, tmp);
		} else {
			portLibrary->mem_free_memory(portLibrary, tmp);
		}
	}

	return i - discard;
}

/**
 * This function takes a thread structure already populated with a backtrace by omrintrospect_backtrace_thread
 * and looks up the symbols for the frames. The format of the string generated is:
 * 		symbol_name (statement_id instruction_pointer [module+offset])
 * If it isn't possible to determine any of the items in the string then they are omitted. If no heap is specified
 * then this function will use malloc to allocate the memory necessary for the symbols which must be freed by the caller.
 *
 * @param portLibrary a pointer to an initialized port library
 * @param threadInfo a thread structure populated with a backtrace
 * @param heap a heap from which to allocate any necessary memory. If NULL malloc is used instead.
 *
 * @return the number of frames for which a symbol was constructed.
 */
uintptr_t
omrintrospect_backtrace_symbols_raw(OMRPortLibrary *portLibrary, J9PlatformThread *threadInfo, J9Heap *heap)
{
	J9PlatformStackFrame *frame = NULL;
	uintptr_t i = 0;

	for (i = 0, frame = threadInfo->callstack; NULL != frame; frame = frame->parent_frame, i++) {
		char output_buf[512];
		char *cursor = output_buf;
		Dl_info dlInfo;
		uintptr_t length = 0;
		uintptr_t iar = frame->instruction_pointer;
		uintptr_t symbol_offset = 0;
		uintptr_t module_offset = 0;
		const char *symbol_name = "";
		const char *module_name = "<unknown>";

		memset(&dlInfo, 0, sizeof(dlInfo));

		/* do the symbol resolution while we're here */
		if (0 != dladdr((void *)iar, &dlInfo)) {
			if (NULL != dlInfo.dli_sname) {
				uintptr_t sym = (uintptr_t)dlInfo.dli_saddr;
				symbol_name = dlInfo.dli_sname;
				symbol_offset = iar - sym;
			}

			if (NULL != dlInfo.dli_fname) {
				/* strip off the full path if possible */
				module_name = strrchr(dlInfo.dli_fname, '/');
				if (NULL != module_name) {
					module_name += 1;
				} else {
					module_name = dlInfo.dli_fname;
				}
			}

			if (NULL != dlInfo.dli_fbase) {
				/* set the module offset */
				module_offset = iar - (uintptr_t)dlInfo.dli_fbase;
			}
		}

		/* symbol_name+offset (id, instruction_pointer [module+offset]) */
		if ('\0' != symbol_name[0]) {
			cursor += omrstr_printf(portLibrary, cursor, sizeof(output_buf) - (cursor - output_buf), "%s", symbol_name);
			cursor += omrstr_printf(portLibrary, cursor, sizeof(output_buf) - (cursor - output_buf), "+0x%x ", symbol_offset);
		}
		cursor += omrstr_printf(portLibrary, cursor, sizeof(output_buf) - (cursor - output_buf), "(0x%p", frame->instruction_pointer);
		if ('\0' != module_name[0]) {
			cursor += omrstr_printf(portLibrary, cursor, sizeof(output_buf) - (cursor - output_buf), " [%s+0x%x]", module_name, module_offset);
		}
		cursor += omrstr_printf(portLibrary, cursor, sizeof(output_buf) - (cursor - output_buf), ")");

		length = (cursor - output_buf) + 1;
		if (NULL != heap) {
			frame->symbol = portLibrary->heap_allocate(portLibrary, heap, length);
		} else {
			frame->symbol = portLibrary->mem_allocate_memory(portLibrary, length, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		}

		if (NULL != frame->symbol) {
			memcpy(frame->symbol, output_buf, length);
		} else {
			frame->symbol = NULL;
			if (0 == threadInfo->error) {
				threadInfo->error = ALLOCATION_FAILURE;
			}
			i -= 1;
		}
	}

	return i;
}

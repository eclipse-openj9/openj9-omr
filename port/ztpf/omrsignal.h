/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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
 *    Multiple authors (IBM Corp.) - initial API and implementation and/or initial documentation
 *    Multiple authors (IBM Corp.) - z/TPF platform initial port to OMR environment
 *******************************************************************************/
#ifndef _ZTPF_OMRSIGNAL_H_INCLUDED
#define _ZTPF_OMRSIGNAL_H_INCLUDED
#include <setjmp.h>
#include <tpf/i_jdib.h>

/*
 *	The following structure and user-defined type originally came from
 *	omrsignal.c; apparently custom-implemented for each different kind of
 *	platform. We have moved these here because these are custom for and	
 *	possibly unique to z/TPF at some later time.
 */

struct J9SignalHandlerRecord {
	struct J9SignalHandlerRecord *previous;
	struct OMRPortLibrary *portLibrary;
	j9sig_handler_fn handler;
	void *handler_arg;
	jmp_buf	mark;			
	uint32_t flags;
};

typedef struct J9CurrentSignal {
	int	signal;
	siginfo_t *sigInfo;
	void *contextInfo;
	uintptr_t breakingEventAddr;
	uint32_t portLibSignalType;
	DIB	*ptrDIB;
} J9CurrentSignal;

void masterSynchSignalHandler(int signal, siginfo_t * sigInfo, void *contextInfo, uintptr_t breakingEventAddr);

#endif

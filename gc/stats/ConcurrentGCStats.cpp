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

#include "ConcurrentGCStats.hpp"
#include "EnvironmentBase.hpp"

const char*
MM_ConcurrentGCStats::getConcurrentStatusString(MM_EnvironmentBase *env, uintptr_t status, char *statusBuffer, uintptr_t statusBufferLength)
{
	OMRPORT_ACCESS_FROM_ENVIRONMENT(env);
	switch (status) {
	case CONCURRENT_OFF:
		omrstr_printf(statusBuffer, statusBufferLength, "off");
		break;
	case CONCURRENT_INIT_RUNNING:
		omrstr_printf(statusBuffer, statusBufferLength, "init running");
		break;
	case CONCURRENT_INIT_COMPLETE:
		omrstr_printf(statusBuffer, statusBufferLength, "init complete");
		break;
	case CONCURRENT_ROOT_TRACING:
		omrstr_printf(statusBuffer, statusBufferLength, "root tracing");
		break;
	case CONCURRENT_TRACE_ONLY:
		omrstr_printf(statusBuffer, statusBufferLength, "trace only");
		break;
	case CONCURRENT_CLEAN_TRACE:
		omrstr_printf(statusBuffer, statusBufferLength, "clean trace");
		break;
	case CONCURRENT_EXHAUSTED:
		omrstr_printf(statusBuffer, statusBufferLength, "exhausted");
		break;
	case CONCURRENT_FINAL_COLLECTION:
		omrstr_printf(statusBuffer, statusBufferLength, "final collection");
		break;
	default:
		if (CONCURRENT_ROOT_TRACING < status) {
			omrstr_printf(statusBuffer, statusBufferLength, "root tracing + %lld", (status - (uintptr_t)CONCURRENT_ROOT_TRACING));
		} else {
			omrstr_printf(statusBuffer, statusBufferLength, "unknown");
		}
		break;
	}
	statusBuffer[statusBufferLength - 1] = 0;
	return (const char *)statusBuffer;
}

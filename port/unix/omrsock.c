/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
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
 * @brief Sockets
 */

#include <netdb.h>
#include <string.h> 

#include "omrcfg.h"
#if defined(OMR_PORT_SOCKET_SUPPORT)
#include "omrport.h"
#include "omrporterror.h"
#include "omrsockptb.h"

int32_t
omrsock_startup(struct OMRPortLibrary *portLibrary)
{
	return omrsock_ptb_init(portLibrary);
}

int32_t
omrsock_getaddrinfo_create_hints(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t *hints, int32_t family, int32_t socktype, int32_t protocol, int32_t flags)
{
	*hints = NULL;
	omrsock_ptb_t ptBuffer = NULL;
	omr_os_addrinfo *ptbHints = NULL;

	/* Initialized the pt buffers if necessary */
	ptBuffer = omrsock_ptb_get(portLibrary);
	if (NULL == ptBuffer) {
		return OMRPORT_ERROR_SOCK_PTB_FAILED;
	}
    
	ptbHints = (ptBuffer->addrInfoHints).addrInfo;
	if (NULL == ptbHints) {
		ptbHints = portLibrary->mem_allocate_memory(portLibrary, sizeof(omr_os_addrinfo), OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL == ptbHints) {
			return OMRPORT_ERROR_SOCK_SYSTEM_FULL;
		}
	}
	memset(ptbHints, 0, sizeof(omr_os_addrinfo));

	ptbHints->ai_flags = flags;
	ptbHints->ai_family = family;
	ptbHints->ai_socktype = socktype;
	ptbHints->ai_protocol = protocol;

	(ptBuffer->addrInfoHints).addrInfo = ptbHints;
	(ptBuffer->addrInfoHints).length = 1;
	*hints = &ptBuffer->addrInfoHints;
	return 0;
}

int32_t
omrsock_getaddrinfo(struct OMRPortLibrary *portLibrary, char *node, char *service, omrsock_addrinfo_t hints, omrsock_addrinfo_t result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_getaddrinfo_length(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t *length)
{
	if (NULL == handle) {
		return OMRPORT_ERROR_INVALID_ARGUMENTS;
	}

	*length = handle->length;
	return 0;
}

int32_t
omrsock_getaddrinfo_family(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, int32_t *family, int32_t index)
{	
	if ((NULL == handle) || (NULL == handle->addrInfo) || (index < 0) || (index >= handle->length)) {
		return OMRPORT_ERROR_INVALID_ARGUMENTS;
	}

	omr_os_addrinfo *info = handle->addrInfo;
	int32_t i = 0;

	for (i = 0; i < index; i++) {
		info = info->ai_next;
		if (NULL == info) {
			return OMRPORT_ERROR_INVALID_ARGUMENTS;
		}
	}

	*family = info->ai_family;
	return 0;
}

int32_t
omrsock_getaddrinfo_socktype(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, int32_t *socktype, int32_t index)
{
	if ((NULL == handle) || (NULL == handle->addrInfo) || (index < 0) || (index >= handle->length)) {
		return OMRPORT_ERROR_INVALID_ARGUMENTS;
	}

	omr_os_addrinfo *info = handle->addrInfo;
	int32_t i = 0;
	
	for (i = 0; i < index; i++) {
		info = info->ai_next;
		if (NULL == info) {
			return OMRPORT_ERROR_INVALID_ARGUMENTS;
		}
	}

	*socktype = info->ai_socktype;
	return 0;
}

int32_t
omrsock_getaddrinfo_protocol(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, int32_t *protocol, int32_t index)
{
	if ((NULL == handle) || (NULL == handle->addrInfo) || (index < 0) || (index >= handle->length)) {
		return OMRPORT_ERROR_INVALID_ARGUMENTS;
	}

	omr_os_addrinfo *info = handle->addrInfo;
	int32_t i = 0;

	for (i = 0; i < index; i++) {
		info = info->ai_next;
		if (NULL == info) {
			return OMRPORT_ERROR_INVALID_ARGUMENTS;
		}
	}

	*protocol = info->ai_protocol;
	return 0;
}

int32_t
omrsock_freeaddrinfo(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_socket(struct OMRPortLibrary *portLibrary, omrsock_socket_t *sock, int32_t family, int32_t socktype, int32_t protocol)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_bind(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_sockaddr_t addr)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_listen(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, int32_t backlog)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_connect(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_sockaddr_t addr)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_accept(struct OMRPortLibrary *portLibrary, omrsock_socket_t serverSock, omrsock_sockaddr_t addrHandle, omrsock_socket_t *sockHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_send(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_sendto(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags, omrsock_sockaddr_t addrHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_recv(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_recvfrom(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags, omrsock_sockaddr_t addrHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_close(struct OMRPortLibrary *portLibrary, omrsock_socket_t *sock)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_shutdown(struct OMRPortLibrary *portLibrary)
{
	return omrsock_ptb_shutdown(portLibrary);
}

#endif /* defined(OMR_PORT_SOCKET_SUPPORT) */

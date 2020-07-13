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

#include "omrcfg.h"
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
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_getaddrinfo(struct OMRPortLibrary *portLibrary, const char *node, const char *service, omrsock_addrinfo_t hints, omrsock_addrinfo_t result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_addrinfo_length(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_addrinfo_family(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_addrinfo_socktype(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_addrinfo_protocol(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_addrinfo_address(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, omrsock_sockaddr_t result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_freeaddrinfo(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_sockaddr_init(struct OMRPortLibrary *portLibrary, omrsock_sockaddr_t handle, int32_t family, uint8_t *addrNetworkOrder, uint16_t portNetworkOrder)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_sockaddr_init6(struct OMRPortLibrary *portLibrary, omrsock_sockaddr_t handle, int32_t family, uint8_t *addrNetworkOrder, uint16_t portNetworkOrder, uint32_t flowinfo, uint32_t scope_id)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_socket_getfd(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock)
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
omrsock_pollfd_init(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t handle, omrsock_socket_t sock, int16_t events)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_get_pollfd_info(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t handle, omrsock_socket_t *sock, int16_t *revents)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_poll(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t fds, uint32_t nfds, int32_t timeoutMs)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

void
omrsock_fdset_zero(struct OMRPortLibrary *portLibrary, omrsock_fdset_t fdset)
{
}

void
omrsock_fdset_set(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
}

void
omrsock_fdset_clr(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
}

BOOLEAN
omrsock_fdset_isset(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
	return FALSE;
}

int32_t
omrsock_select(struct OMRPortLibrary *portLibrary, omrsock_fdset_t readfds, omrsock_fdset_t writefds, omrsock_fdset_t exceptfds, omrsock_timeval_t timeout)
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

uint16_t
omrsock_htons(struct OMRPortLibrary *portLibrary, uint16_t val)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

uint32_t
omrsock_htonl(struct OMRPortLibrary *portLibrary, uint32_t val)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_inet_pton(struct OMRPortLibrary *portLibrary, int32_t addrFamily, const char *addr, uint8_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_fcntl(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, int32_t arg)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_timeval_init(struct OMRPortLibrary *portLibrary, omrsock_timeval_t handle, uint32_t secTime, uint32_t uSecTime)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_linger_init(struct OMRPortLibrary *portLibrary, omrsock_linger_t handle, int32_t enabled, uint16_t timeout)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_setsockopt_int(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, int32_t *optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_setsockopt_linger(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_linger_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_setsockopt_timeval(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_timeval_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_getsockopt_int(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, int32_t *optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_getsockopt_linger(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_linger_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

int32_t
omrsock_getsockopt_timeval(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_timeval_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

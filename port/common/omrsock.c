/*******************************************************************************
 * Copyright (c) 2019, 2020 IBM Corp. and others
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
#include "omrportsock.h"

/**
 * Set up omrsock per thread buffer.
 *
 * Pass in user preference of IPv6 Support in omrsock_startup: TODO.
 *
 * @param[in] portLibrary The port library.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_startup(struct OMRPortLibrary *portLibrary)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Returns hints as a double pointer to an OMRAddInfoNode structure.
 * 
 * This hints structure is used to modify the results returned by a call to 
 * @ref omrsock_getaddrinfo. 
 *
 * @param[in] portLibrary The port library.
 * @param[out] hints The filled-in hints structure.
 * @param[in] family Address family type.
 * @param[in] socktype Socket type.
 * @param[in] protocol Protocol family.
 * @param[in] flags Flags for modifying the result. Pass multiple flags using the 
 * bitwise-OR operation.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_getaddrinfo_create_hints(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t *hints, int32_t family, int32_t socktype, int32_t protocol, int32_t flags)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers a list of addresses as an opaque struct in "result".
 * 
 * Use the following functions to extract the details:
 * @arg @ref omrsock_addrinfo_length
 * @arg @ref omrsock_addrinfo_family
 * @arg @ref omrsock_addrinfo_socktype
 * @arg @ref omrsock_addrinfo_protocol
 * @param[in] portLibrary The port library.
 * @param[in] node The name of the host in either host name format or in IPv4 or IPv6 accepted 
 * notations.
 * @param[in] service The port of the host in string form.
 * @param[in] hints Hints on what results are returned (can be NULL for default action). Use 
 * @ref omrsock_getaddrinfo_create_hints to create the hints.
 * @param[out] result An opaque pointer to a list of results (OMRAddrInfoNode must be preallocated).
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when hints or result arguments are invalid.
 * \arg OMRPORT_ERROR_SOCK_ADDRINFO_FAILED when system addrinfo fails.
 *
 * @note Must free the "result" structure with @ref omrsock_freeaddrinfo to free up memory.
 */
int32_t
omrsock_getaddrinfo(struct OMRPortLibrary *portLibrary, const char *node, const char *service, omrsock_addrinfo_t hints, omrsock_addrinfo_t result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers the number of results returned from @ref omrsock_getaddrinfo.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle The result structure returned by @ref omrsock_getaddrinfo.
 * @param[out] result The number of results.
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when handle or index arguments are invalid.
 */
int32_t
omrsock_addrinfo_length(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers the family type of the address at "index" in the structure returned from 
 * @ref omrsock_getaddrinfo, indexed starting at 0.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle The result structure returned by @ref omrsock_getaddrinfo.
 * @param[in] index The index into the structure returned by @ref omrsock_getaddrinfo.
 * @param[out] result The family at "index".
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when handle or index arguments are invalid.
 */
int32_t
omrsock_addrinfo_family(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers the socket type of the address at "index" in the structure returned from 
 * @ref omrsock_getaddrinfo, indexed starting at 0.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle The result structure returned by @ref omrsock_getaddrinfo.
 * @param[in] index The index into the structure returned by @ref omrsock_getaddrinfo.
 * @param[out] result The socket type at "index".
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when handle or index arguments are invalid.
 */
int32_t
omrsock_addrinfo_socktype(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers the protocol of the address at "index" in the structure returned from 
 * @ref omrsock_getaddrinfo, indexed starting at 0.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle The result structure returned by @ref omrsock_getaddrinfo.
 * @param[in] index The index into the structure returned by @ref omrsock_getaddrinfo.
 * @param[out] result The protocol family at "index".
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when handle or index arguments are invalid.
 */
int32_t
omrsock_addrinfo_protocol(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, int32_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers a OMRSockAddrStorage which is the address struct at "index" in the structure returned from
 * @ref omrsock_getaddrinfo, indexed starting at 0. OMRSockAddrStorage should be preallocated by user.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle The result structure returned by @ref omrsock_getaddrinfo.
 * @param[in] index The address index into the structure returned by @ref j9sock_getaddrinfo.
 * @param[out] result Pointer to OMRSockAddrStorage which contains address at "index", should
 * be preallocated.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_addrinfo_address(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle, uint32_t index, omrsock_sockaddr_t result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Frees the memory created by the call to @ref omrsock_getaddrinfo.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to results returned by @ref omrsock_getaddrinfo.
 *
 * @return 0, if no errors occurred, otherwise return an error. Error code values returned are
 * \arg OMRPORT_ERROR_INVALID_ARGUMENTS when handle argument is invalid.
 */
int32_t
omrsock_freeaddrinfo(struct OMRPortLibrary *portLibrary, omrsock_addrinfo_t handle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers an initalized OMRSockAddrStorage structure that contains AF_INET address. The only 
 * address family currently supported is AF_INET.
 *
 * @param[in] portLibrary The port library.
 * @param[in] family The address family.
 * @param[in] addrNetworkOrder The host address, in network order. Use @ref omrsock_htonl() and 
 * omrsock_inet_pton to convert the ip address from host to network byte order.
 * @param[in] portNetworkOrder The port, in network byte order. Use @ref omrsock_htons() to 
 * convert the port from host to network byte order.
 * @param[out] handle Pointer to the OMRSockAddrStorage struct, to be allocated.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_sockaddr_init(struct OMRPortLibrary *portLibrary, omrsock_sockaddr_t handle, int32_t family, uint8_t *addrNetworkOrder, uint16_t portNetworkOrder)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answers an initialized OMRSockAddrStorage structure. Supports both IPv4-mapped IPv6 addresses 
 * and IPv6 addresses.
 *
 * Pass in a omrsock_sockaddr_t, preallocated, with some parameters to initialize it appropriately.
 * Currently the only address family supported is OS_AF_INET6, with either IPv4-mapped IPv6 address,
 * or IPv6 address, which will be determined by family.
 *
 * @param[in] portLibrary The port library.
 * @param[in] addrNetworkOrder The IPv4 or IPv6 address in network byte order.
 * @param[in] family The address family.
 * @param[in] portNetworkOrder The target port, in network order. Use @ref omrsock_htons() to convert the port from host to network byte order.
 * @param[in] flowinfo The flowinfo value for IPv6 addresses in HOST order. Set to 0 if no flowinfo needs to be set for the address.
 * @param[in] scope_id The scope id for an IPv6 address in HOST order. Set to 0 for non-scoped IPv6 addresses.
 * @param[out] handle Pointer pointer to the OMRSockAddrStorage, to be allocated.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_sockaddr_init6(struct OMRPortLibrary *portLibrary, omrsock_sockaddr_t handle, int32_t family, uint8_t *addrNetworkOrder, uint16_t portNetworkOrder, uint32_t flowinfo, uint32_t scope_id)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Get socket file descriptor from a pointer to the socket structure.
 *
 * @param[in] portLibrary The port library.
 * @param[out] sock Pointer to the omrsocket.
 *
 * @return fd, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_socket_getfd(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Creates a new socket descriptor and any related resources.
 *
 * If non-blocking or async sockets are wanted, the user can OR the socktype with socket flag
 * they want to set. They may also set the socket to be non-blocking or async later in the code
 * using @ref omrsock_fcntl.
 *
 * @param[in] portLibrary The port library.
 * @param[out] sock Pointer to the omrsocket, to be allocated.
 * @param[in] family The address family.
 * @param[in] socktype Specifies what type of socket is created, for example stream
 * or datagram.
 * @param[in] protocol The Protocol family.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_socket(struct OMRPortLibrary *portLibrary, omrsock_socket_t *sock, int32_t family, int32_t socktype, int32_t protocol)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Used on an unconnected socket before subsequent calls to 
 * the @ref omrsock_connect or @ref omrsock_listen functions. When a socket is created 
 * with a call to @ref omrsock_socket, it exists in a name space (address family), but 
 * it has no name assigned to it. Use omrsock_bind to establish the local association 
 * of the socket by assigning a local name to an unnamed socket.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock The socket that will be be associated with the specified name.
 * @param[in] addr Address to bind to socket.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_bind(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_sockaddr_t addr)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set the socket to listen for incoming connection requests. This call is made prior to 
 * accepting requests, via the @ref omrsock_accept function. The backlog specifies the 
 * maximum length of the queue of pending connections, after which further requests are 
 * rejected.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket.
 * @param[in] backlog The maximum number of queued requests.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_listen(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, int32_t backlog)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Establish a connection to a peer.
 *
 * For stream sockets, it first binds the socket if it hasn't already been done. Then, it
 * tries to set up a connection.
 * 
 * For datagram sockets, omrsock_connect function will set up the peer information. No actual
 * connection is made. Subsequent calls to connect can be made to change destination address.
 *
 * It is not recommended that users call this function multiple times to determine when the 
 * connection attempt has succeeded. Instead, the @ref omrsock_select function can be used to 
 * determine when the socket is ready for reading or writing.
 * 
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the unconnected local socket.
 * @param[in] addr Pointer to the sockaddr, specifying remote host/port.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_connect(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_sockaddr_t addr)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Extracts the first connection on the queue of pending connections on socket serverSock. 
 * It then creates a new socket and returns a handle to the new socket. The newly created 
 * socket is the socket that will handle the actual the connection and has the same 
 * properties as the socket serverSock.  
 *
 * The omrsock_accept function can block the caller until a connection is present if no pending 
 * connections are present on the queue.
 *
 * @param[in] portLibrary The port library.
 * @param[in] serverSock An omrsock_socket_t that tries to accept a connection.
 * @param[in] addrHandle An optional pointer to a buffer that receives the address of the 
 * connecting entity, as known to the communications layer. The exact format of the addr 
 * parameter is determined by the address family established when the socket was created.
 * @param[out] sockHandle A pointer to an omrsock_socket_t which will point to the newly created 
 * socket once accept returns successfully.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_accept(struct OMRPortLibrary *portLibrary, omrsock_socket_t serverSock, omrsock_sockaddr_t addrHandle, omrsock_socket_t *sockHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Sends data to a connected socket. The successful completion of an omrsock_send does 
 * not indicate that the data was successfully delivered. If no buffer space is available 
 * within the transport system to hold the data to be transmitted, omrsock_send will block.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to send on.
 * @param[in] buf The bytes to be sent.
 * @param[in] nbyte The number of bytes to send.
 * @param[in] flags The flags to modify the send behavior.
 *
 * @return the total number of bytes sent if no error occured, which can be less than the 
 * 'nbyte' for nonblocking sockets, otherwise return an error.
 */
int32_t
omrsock_send(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Sends data to a datagram socket. The successful completion of an omrsock_sento does 
 * not indicate that the data was successfully delivered. If no buffer space is available 
 * within the transport system to hold the data to be transmitted, omrsock_sendto will block.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to send on.
 * @param[in] buf The bytes to be sent.
 * @param[in] nbyte The number of bytes to send.
 * @param[in] flags The flags to modify the send behavior.
 * @param[in] addrHandle The network address to send the datagram to.
 *
 * @return the total number of bytes sent if no error occured, otherwise return an error.
 */
int32_t
omrsock_sendto(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags, omrsock_sockaddr_t addrHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Receives data from a connected socket.  
 * 
 * This will return available information up to the size of the buffer supplied. 
 * 
 * Its behavior will depend on the blocking characteristic of the socket.
 * Blocking socket: If no incoming data is available, the call blocks and waits for data to arrive. 
 * Non-blocking socket: TODO.
 * 
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to read on.
 * @param[out] buf Pointer to the buffer where input bytes are written.
 * @param[in] nbyte The length of buf.
 * @param[in] flags The flags, to influence this read (in addition to the socket options).
 *
 * @return the number of bytes received if no error occured. If the connection has been 
 * gracefully closed, return 0. Otherwise, return an error.
 */
int32_t
omrsock_recv(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Receives data from a possibly connected socket. 
 * 
 * Calling omrsock_recvfrom will return available information up to the size of the buffer 
 * supplied. If the information is too large for the buffer, the excess will be discarded. 
 * If no incoming data is available at the socket, the omrsock_recvfrom call blocks and 
 * waits for data to arrive. It the address argument is not null, the address will be updated 
 * with address of the message sender.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to read on.
 * @param[out] buf Pointer to the buffer where input bytes are written.
 * @param[in] nbyte The length of buf.
 * @param[in] flags Tthe flags, to influence this read.
 * @param[out] addrHandle If provided, the address to be updated with the sender information.
 *
 * @return the number of bytes received if no error occured. If the connection has been 
 * gracefully closed, return 0. Otherwise, return an error.
 */
int32_t
omrsock_recvfrom(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, uint8_t *buf, int32_t nbyte, int32_t flags, omrsock_sockaddr_t addrHandle)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set the pollfd values in an user allocated OMRPollFd structure. This allows the user
 * to set the socket this pollfd is for as well as the events it would like @ref omrsock_poll
 * to observe. This initializes the OMRPollFd structure for @ref omrsock_poll function.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to OMRPollFd structure that is to be initialized.
 * @param[in] sock Pointer to the socket this OMRPollFd structure is for.
 * @param[in] events All events to be observed by @ref omrsock_poll, which is ORed before passing in.
 * \arg OMRSOCK_POLLIN
 * \arg OMRSOCK_POLLOUT
 * \arg OMRSOCK_POLLERR (Not available on AIX)
 * \arg OMRSOCK_POLLNVAL (Not available on AIX)
 * \arg OMRSOCK_POLLHUP (Not available on AIX)
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_pollfd_init(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t handle, omrsock_socket_t sock, int16_t events)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Get the pollfd revents values and the socket that the revents were set on.
 *
 * The revents are the returned events from the @ref omrsock_poll function, which is ORed events of which
 * the socket is ready for, whether it will be POLLIN or POLLOUT or error. User should use this information
 * to decide if a socket is ready for the communication operations.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to OMRPollFd structure.
 * @param[out] sock To set pointer to the socket that this OMRPollFd is for.
 * @param[out] revents To set revents returned by @ref omrsock_poll. This is ORed and should use AND
 * to decide which operation is ready.
 * \arg OMRSOCK_POLLIN
 * \arg OMRSOCK_POLLOUT
 * \arg OMRSOCK_POLLERR (Not available on AIX)
 * \arg OMRSOCK_POLLNVAL (Not available on AIX)
 * \arg OMRSOCK_POLLHUP (Not available on AIX)
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_get_pollfd_info(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t handle, omrsock_socket_t *sock, int16_t *revents)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Observe and waits for a set of sockets for I/O multiplexing operations.
 *
 * Watches an array of OMRPollFd structures, which is to be initalized using @ref omrsock_pollfd_init
 * and to be examined using @ref omrsock_get_pollfd_info.
 *
 * Similar to @ref omrsock_select, but uses a different interface. This is the new function and interface
 * for I/O multiplexing.
 *
 * @param[in] portLibrary The port library.
 * @param[out] fds Pointer to an OMRPollFd structure or an array of OMRPollFd structures.
 * @param[in] nfd The number of OMRPollFds inside the array fds.
 * @param[in] timeoutMs Poll timeout in milliseconds.
 *
 * @returns number of sockets that are ready for I/O, otherwise return an error.
 */
int32_t
omrsock_poll(struct OMRPortLibrary *portLibrary, omrsock_pollfd_t fds, uint32_t nfds, int32_t timeoutMs)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Zero the fdset. All sockets in fdset will be removed.
 *
 * @param[in] portLibrary The port library.
 * @param[out] fds Pointer to an OMRFdSet structure.
 */
void
omrsock_fdset_zero(struct OMRPortLibrary *portLibrary, omrsock_fdset_t fdset)
{
}

/**
 * Set a socket in the fdset. This socket will be watched in @ref omrsock_select.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to be set.
 * @param[out] fds Pointer to an OMRFdSet structure.
 */
void
omrsock_fdset_set(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
}

/**
 * Clear a socket in the fdset. This socket will be removed from being watched in @ref omrsock_select.
 * Other sockets in fdset will not be affected.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to be set.
 * @param[out] fds Pointer to an OMRFdSet structure.
 */
void
omrsock_fdset_clr(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
}

/**
 * Check if a socket in the fdset is ready for I/O or if it is in the fdset.
 *
 * @param[in] portLibrary The port library.
 * @param[in] sock Pointer to the socket to be set.
 * @param[in] fds Pointer to an OMRFdSet structure.
 * @return TRUE, if socket is SET, otherwise return FALSE.
 */
BOOLEAN
omrsock_fdset_isset(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, omrsock_fdset_t fdset) 
{
	return FALSE;
}

/**
 * Observe and waits for a set of sockets for I/O multiplexing operations.
 *
 * Select watches and monitors over three sets of fdsets.
 *
 * Similar to @ref omrsock_poll, but uses a different interface. This is the old way of
 * I/O multiplexing. BSD select() function takes an additional argument of nfds, the max socket
 * file descriptor in the fdsets. This value is taken care of inside the omrsock API by storing
 * the maximum fd inside OMRFdSet when user calls @ref omrsock_fdset_set.
 *
 * @param[in] portLibrary The port library.
 * @param[out] readfds Pointer to an read OMRFdSet structure, that contains all sockets to be read.
 * @param[out] writefds Pointer to an write OMRFdSet structure, that contains all sockets to write.
 * @param[out] exceptfds Pointer to except OMRFdSet structure, that contains all sockets to be checked
 * for error.
 * @param[in] timeout Pointer to OMRTimeVal struct which decides timeout for select. Initialize
 * timeout using @ref omrsock_timeval_init.
 *
 * @returns number of sockets that are ready for I/O, otherwise return an error.
 */
int32_t
omrsock_select(struct OMRPortLibrary *portLibrary, omrsock_fdset_t readfds, omrsock_fdset_t writefds, omrsock_fdset_t exceptfds, omrsock_timeval_t timeout)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}


/**
 * Closes a socket. Use it to release the socket so that further references 
 * to socket will fail.
 * 
 * @param[in] portLibrary The port library.
 * @param[in] sock The socket that will be closed.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_close(struct OMRPortLibrary *portLibrary, omrsock_socket_t *sock)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Shut down omrsock per thread buffer.
 *
 * @param[in] portLibrary The port library.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_shutdown(struct OMRPortLibrary *portLibrary)
{
	return 0;
}

/**
 * Answer the 16 bit host ordered argument, in network byte order.
 *
 * @param[in] portLibrary The port library.
 * @param[in] val The 16 bit host ordered number.
 *
 * @return the 16 bit network ordered number.
 */
uint16_t
omrsock_htons(struct OMRPortLibrary *portLibrary, uint16_t val)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answer the 32 bit host ordered argument, in network byte order.
 *
 * @param[in] portLibrary The port library.
 * @param[in] val The 32 bit host ordered number.
 *
 * @return the 32 bit network ordered number.
 */
uint32_t
omrsock_htonl(struct OMRPortLibrary *portLibrary, uint32_t val)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answer a uint8_t array representing the address in network byte order. 
 *
 * Takes a string of either dot-decimal format for OMRSOCK_AF_INET or the 8 hexadecimal format 
 * for OMRSOCK_AF_INET6 and converts it to network byte order.
 *
 * The addrNetworkOrder will either be 4 or 16 bytes depending on whether it is an OMRSOCK_AF_INET address 
 * or an OMRSOCK_AF_INET6 address. You should preallocating the "address" depending on address family,
 * whether to preallocate 4 or 16 bytes.
 * 
 * @param[in] portLibrary The port library.
 * @param[in] addrFamily The address family.
 * @param[in] addr The address string to be converted.
 * @param[out] result The address in network order.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_inet_pton(struct OMRPortLibrary *portLibrary, int32_t addrFamily, const char *addr, uint8_t *result)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set socket non_block and asychronous.
 *
 * @param[in] portLibrary The port library.
 * @param[out] sock Pointer to the socket structure.
 * @param[in] arg The socket flag you want to set on the socket.
 * \arg OMRSOCK_O_NON_BLOCK
 * \arg OMRSOCK_O_ASYNC
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_fcntl(struct OMRPortLibrary *portLibrary, omrsock_socket_t sock, int32_t arg)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Create a time structure, representing the timeout period defined in seconds & microSeconds. Timeval's could be 
 * used as timeout arguments in the @ref omrsock_setsockopt function.
 *
 * @param[in] portLibrary The port library.
 * @param[out] handle Pointer pointer to the OMRTimeval to be allocated.
 * @param[in] secTime The integer component of the timeout value (in seconds).
 * @param[in] uSecTime The fractional component of the timeout value (in microseconds).
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_timeval_init(struct OMRPortLibrary *portLibrary, omrsock_timeval_t handle, uint32_t secTime, uint32_t uSecTime)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Initializes a new linger structure, enabled or disabled, with the timeout as specified. Linger defines the
 * behavior when unsent messages exist for a socket that has been sent close.
 *
 * If linger is disabled, the default, close returns immediately and the stack attempts to deliver unsent messages.
 * If linger is enabled:
 * \arg if the timeout is 0, the close will block indefinitely until the messages are sent.
 * \arg if the timeout is set, the close will return after the messages are sent or the timeout period expired.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the linger struct to be accessed.
 * @param[in] enabled Aero to disable, a non-zero value to enable linger.
 * @param[in] timeout A positive timeout value from 0 to linger indefinitely (in seconds).
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_linger_init(struct OMRPortLibrary *portLibrary, omrsock_linger_t handle, int32_t enabled, uint16_t timeout)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set the integer value of the socket option.
 * Refer to the private get_os_socket_level & get_os_socket_option functions for details of the options
 * supported.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to set the option in.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to set.
 * @param[in] optval Pointer to the option value to update the socket option with.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_setsockopt_int(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, int32_t *optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set the linger value on the socket. 
 * See the @ref omrsock_linger_init for setting up the linger struct and details of the linger behavior.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to set the option in.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to set.
 * @param[in] optval Pointer to the option value to update the socket option with.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_setsockopt_linger(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_linger_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Set the timeval value on the socket. 
 * See the @ref omrsock_timeval_init for setting up the timeval struct details of the timeval behavior.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to set the option in.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to set.
 * @param[in] optval Pointer to the option value to update the socket option with.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_setsockopt_timeval(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_timeval_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answer the integer value of the socket option.
 * Refer to the private get_os_socket_level & get_os_socket_option functions for details of the options
 * supported.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to query for the option value.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to retrieve.
 * @param[out] optval Pointer to the pre-allocated space to update with the option value.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_getsockopt_int(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, int32_t *optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answer the linger value on the socket. 
 * See the @ref omrsock_linger_init for setting up the linger struct and details of the linger behavior.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to query for the option value.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to retrieve.
 * @param[out] optval Pointer to the pre-allocated space to update with the option value.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_getsockopt_linger(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_linger_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

/**
 * Answer the timeval value on the socket. 
 * See the @ref omrsock_timeval_init for setting up the timeval struct details of the timeval behavior.
 *
 * @param[in] portLibrary The port library.
 * @param[in] handle Pointer to the socket to query for the option value.
 * @param[in] optlevel The level within the IP stack at which the option is defined.
 * @param[in] optname The name of the option to retrieve.
 * @param[out] optval Pointer to the pre-allocated space to update with the option value.
 *
 * @return 0, if no errors occurred, otherwise return an error.
 */
int32_t
omrsock_getsockopt_timeval(struct OMRPortLibrary *portLibrary, omrsock_socket_t handle, int32_t optlevel, int32_t optname, omrsock_timeval_t optval)
{
	return OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM;
}

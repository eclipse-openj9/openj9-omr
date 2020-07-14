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

#if !defined(OMRPORTSOCK_H_)
#define OMRPORTSOCK_H_

/* Pointer to OMRAddInfoNode, a struct that contains addrinfo information. */
typedef struct OMRAddrInfoNode *omrsock_addrinfo_t;

/* Pointer to OMRSockAddrStorage, a struct that contains socket address
 * information. It has enough space for Ipv4 or IPv6 addresses. 
 */
typedef struct OMRSockAddrStorage *omrsock_sockaddr_t;

/* Pointer to OMRSocket, a struct that contains socket descriptor. */
typedef struct OMRSocket *omrsock_socket_t;

/* Pointer to OMRPollFd, a struct that contains struct pollfd. */
typedef struct OMRPollFd *omrsock_pollfd_t;

/* Pointer to OMRFdSet, a struct that contains struct fd_set. */
typedef struct OMRFdSet *omrsock_fdset_t;

/* Pointer to OMRTimeval, a struct that contains struct timeval. */
typedef struct OMRTimeval *omrsock_timeval_t;

/* Pointer to OMRLinger, a struct that contains struct linger.*/
typedef struct OMRLinger *omrsock_linger_t;

/* Bind to all available interfaces */
#define OMRSOCK_INADDR_ANY ((uint32_t)0)

/* Address Family */
#define OMRSOCK_AF_UNSPEC 0
#define OMRSOCK_AF_INET 1
#define OMRSOCK_AF_INET6 2

/* Socket types */
#define OMRSOCK_ANY 0
#define OMRSOCK_STREAM 1
#define OMRSOCK_DGRAM 2

/* Protocol Family and Socket Levels */
#define OMRSOCK_IPPROTO_DEFAULT 0
#define OMRSOCK_SOL_SOCKET 1
#define OMRSOCK_IPPROTO_TCP 2
#define OMRSOCK_IPPROTO_UDP 3

/* Socket Options */
#define OMRSOCK_SO_REUSEADDR 1
#define OMRSOCK_SO_KEEPALIVE 2
#define OMRSOCK_SO_LINGER 3
#define OMRSOCK_SO_RCVTIMEO 4
#define OMRSOCK_SO_SNDTIMEO 5
#define OMRSOCK_TCP_NODELAY 6

/* Socket Flags */
#define OMRSOCK_O_ASYNC 0x0100
#define OMRSOCK_O_NONBLOCK 0x1000

/* Poll Constants */
#define OMRSOCK_POLLIN 0x0001
#define OMRSOCK_POLLOUT 0x0002

#if !defined(OMR_OS_AIX) 
#define OMRSOCK_POLLERR 0x0004
#define OMRSOCK_POLLNVAL 0x0008
#define OMRSOCK_POLLHUP 0x0010
#endif

#endif /* !defined(OMRPORTSOCK_H_) */

/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "omrport.h"
#include "omrportsock.h"
#include "testHelpers.hpp"

/**
 * Start a server which includes create a socket, bind to an address/port and 
 * listen for clients.
 * 
 * @param[in] portLibrary
 * @param[in] testname The name of the calling test.
 * @param[in] addrStr The address of the server.
 * @param[in] port The server port.
 * @param[in] family Socket address family wanted.
 * @param[out] serverSocket A pointer to the server socket. 
 * @param[out] serverAddr The socket address of the server created.
 * 
 * @return 0 on success, return an error otherwise.
 */ 
int32_t
startServer(struct OMRPortLibrary *portLibrary, const char *testName, const char *addrStr, const char *port, int32_t family, omrsock_socket_t *serverSocket, omrsock_sockaddr_t serverAddr) 
{
    return OMRPORT_ERROR_NOTEXIST;
}

/**
 * Create the client socket then connect to the server.
 *  
 * @param[in] portLibrary
 * @param[in] testname The name of the calling test.
 * @param[in] addrStr The address of the server.
 * @param[in] port The server port.
 * @param[in] family Socket address family wanted.
 * @param[in] sessionClientSocket A pointer to the client socket. 
 * @param[in] sessionClientAddr The socket address of the client created. 
 * 
 * @return 0 on success, return an error otherwise.
 */ 
int32_t
connectClientToServer(struct OMRPortLibrary *portLibrary, const char* testName, const char *addrStr, const char *port, int32_t family, omrsock_socket_t *sessionClientSocket, omrsock_sockaddr_t sessionClientAddr) 
{
    return OMRPORT_ERROR_NOTEXIST;
}

/**
 * To test if Port Library is properly set up to run OMRSock tests. 
 *
 * This will iterate through all OMRSock functions in @ref omrsock.c and simply check 
 * that the location of the function is not NULL.
 * 
 * @return TEST_PASS on success, TEST_FAIL on error.
 *
 * @note Errors such as an OMRSock function call with a NULL address will be reported.
 */
TEST(PortSockTest, sock_test0)
{
    OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
    const char *testName = "omrsock_test0";

    reportTestEntry(OMRPORTLIB, testName);
    reportTestExit(OMRPORTLIB, testName);
}

/**
 * To test the omrsock per thread buffer.
 * 
 * In this test, per thread buffer related functions will be called to set up a buffer to store
 * information such as the hints structures created in @ref omrsock_getaddrinfo_create_hints.
 *
 * @return TEST_PASS on success, TEST_FAIL on error.
 */
TEST(PortSockTest, sock_test1)
{
    OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
    const char *testName = "omrsock_test1";

    reportTestEntry(OMRPORTLIB, testName);
    reportTestExit(OMRPORTLIB, testName);
}

/**
 * To test @ref omrsock_getaddrinfo_create_hints, @ref omrsock_getaddrinfo, 
 * @ref omrsock_freeaddrinfo and all functions that extracts details from
 * @ref omrsock_getaddrinfo results.
 * 
 * In this test, relevant variables will be set up to be passed into the 
 * @ref omrsock_getaddrinfo_create_hints. The generated hints will be passed into 
 * @ref omrsock_getaddrinfo. The results generated will be used to create a socket.
 *
 * Address families tested will include IPv4, IPv6(if supported on machine), 
 * unspecified family. Socket types tested will include Stream and Datagram.
 *
 * @return TEST_PASS on success, TEST_FAIL on error.
 *
 * @note Errors such as failed function calls, and/or returning the wrong family or 
 * socket type from @ref omrsock_getaddrinfo compared to the ones passed into hints, 
 * will be reported.
 */
TEST(PortSockTest, sock_test2)
{
    OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
    const char *testName = "omrsock_test2";

    reportTestEntry(OMRPORTLIB, testName);
    reportTestExit(OMRPORTLIB, testName);
}

/**
 * To test functions needed to set up connection by getting 2 sockets to talk to each other.
 *
 * In this test, first, the server will start and will be listening for connections. Then 
 * client will start, which make a request to connect to the server. The messages will be 
 * sent both ways, and it will be checked if the messages were sent correctly.
 * 
 * Address families tested will include IPv4 and IPv6 where IPv6 is supported. Socket types
 * tested will include stream and datagram.
 *
 * @return TEST_PASS on success, TEST_FAIL on error.
 *
 * @note Errors such as failed function calls, failure to create server and/or client, wrong 
 * message sent/received, will be reported.
 */
TEST(PortSockTest, sock_test3)
{
    OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
    const char *testName = "omrsock_test3";

    reportTestEntry(OMRPORTLIB, testName);
    reportTestExit(OMRPORTLIB, testName);
}

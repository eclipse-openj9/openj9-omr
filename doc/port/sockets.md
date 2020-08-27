# Linux to omrsock API mappings

For implementation example, please refer to `omr/fvtest/porttest/omrsockTest.cpp`.
For detailed description of the functions, please refer to `omr/port/common/omrsock.c`.


**There are three mappings included in this file:**
- Linux to omrsock API Function Mapping
- Linux to omsock API Implementations Mapping for the Functions
- Linux to omrsock API Error Code Mapping
___


### Linux to omrsock API Function Mapping

|Linux Function Name| omrsock API Function Name|
|---|---|
| - | omrsock_getaddrinfo_create_hints|
|getaddrinfo | omrsock_getaddrinfo|
| - | omrsock_addrinfo_length|
| - | omrsock_addrinfo_family|
| - | omrsock_addrinfo_socktype|
| - | omrsock_addrinfo_protocol|
|freeaddrinfo | omrsock_freeaddrinfo|
| - | omrsock_sockaddr_init|
| - | omrsock_sockaddr_init6|
|socket | omrsock_socket|
| - | omrsock_socket_getfd|
|bind | omrsock_bind|
|listen | omrsock_listen|
|accept | omrsock_accept|
|send | omrsock_send|
|sendto | omrsock_sendto|
|recv | omrsock_recv|
|recvfrom | omrsock_recvfrom|
| - | omrsock_pollfd_init|
| - | omrsock_get_pollfd_info|
|poll | omrsock_poll|
|FD_ZERO | omrsock_fdset_zero|
|FD_SET | omrsock_fdset_set|
|FD_CLR | omrsock_fdset_clr|
|FD_ISSET | sock_fdset_isset|
|select | omrsock_select|
|close | omrsock_close|
|htons | omrsock_htons|
|htonl | sock_htonl|
|inet_pton | omrsock_inet_pton|
|fcntl | omrsock_fcntl|
| - | omrsock_timeval_init|
| - | omrsock_linger_init|
|setsockopt | sock_setsockopt_int|
|setsockopt | sock_setsockopt_linger|
|setsockopt | sock_setsockopt_timeval|
|getsockopt | sock_getsockopt_int|
|getsockopt | sock_getsockopt_linger|
|getsockopt | sock_getsockopt_timeval|

___


### Linux to omsock API Implementations Mapping for Functions Above


Access to the OMR Port Library macro can be defined and the functions below can be accessed. Otherwise, the functions can also be accessed through `OMRPORTLIB->sock_function_name(OMRPORTLIB, arg...)` by defining pointer to OMR Port Library, `OMRPORTLIB`.


|Linux Implementations | omrsock API Implementations | omrsock API Memory Management|
|---|---|---|
|<img width=1000/>|<img width=500/>|<img width=200/>|
|struct addrinfo hints; <br> memset(&hints, 0, sizeof hints); <br> hints.ai_family = AF_INET; <br> hints.ai_socktype = SOCK_STREAM; <br> hints.ai_protocol = 0; | omrsock_addrinfo_t hintsPtr = NULL; <br> omrsock_getaddrinfo_create_hints(&hintsPtr, OMRSOCK_AF_INET, OMRSOCK_STREAM, OMRSOCK_IPPROTO_DEFAULT, 0); | omrsock API allocates the hints structure for user using per thread buffer defined in omr port library. User does not need to free this memory.|
|struct addrinfo *resultPtr; <br> getaddrinfo("localhost", "4930", &hints, &resultPtr); | omrsock_addrinfo_t resultPtr = NULL; <br> omrsock_getaddrinfo("localhost", "4930", hintsPtr, resultPtr); | omrsock API allocates addrinfo structure for user. Free with @ref omrsock_freeaddrinfo.|
|User needs to manually count. | uint32_t length = 0; <br> omrsock_addrinfo_length(&resultPtr, &length); | User needs to allocate length.|
|int32_t family = resultPtr->ai_family; | int32_t family = 0; <br> omrsock_addrinfo_family(&resultPtr, 0, &family); | User needs to allocate family.|
|int32_t socktype = resultPtr->ai_socktype; | int32_t socktype = 0; <br> omrsock_addrinfo_socktype(&resultPtr, 0, &socktype); | User needs to allocate socktype.|
|int32_t protocol = resultPtr->ai_protocol; | int32_t protocol = 0; <br> sock_addrinfo_protocol(&resultPtr, 0, &protocol); | User needs to allocate protocol.|
|freeaddrinfo(resultPtr); | omrsock_freeaddrinfo(&resultPtr); | Frees memory allocated in @ref omrsock_getaddrinfo.|
|struct sockaddr_in sockAddr; <br> sockAddr.sin_family = AF_INET; <br> sockAddr.sin_addr = networkOrderAddr; <br> sockAddr.sin_port = networkOrderPort;| OMRSockAddrStorage sockAddr; <br> omrsock_sockaddr_init(&sockAddr, OMRSOCK_AF_INET, networkOrderAddr, networkOrderPort); | User allocates socket address structure. For network order, refer to @ref omrsock_inet_pton, @ref omrsock_htons, and @ref omrsock_htonl.|
|struct sockaddr_in6 sockAddr; <br> sockAddr.sin_family = AF_INET6; <br> sockAddr.sin_addr = networkOrderAddr6; <br> sockAddr.sin_port = networkOrderPort;| OMRSockAddrStorage sockAddr; <br> omrsock_sockaddr_init6(&sockAddr, OMRSOCK_AF_INET, networkOrderAddr6, networkOrderPort); | User allocates socket address structure. For network order, refer to @ref omrsock_inet_pton, @ref omrsock_htons, and @ref omrsock_htonl.|
|No need for this function. | int32_t fd = omrsock_socket_getfd(socket); | - |
|fd = socket(AF_INET, SOCK_STREAM, 0); | omrsock_socket_t socket; <br> omrsock_socket(OMRSOCK_AF_INET, OMRSOCK_STREAM, OMRSOCK_IPPROTO_DEFAULT); | omrsock API allocates the socket structure for user. Free with @ref omrsock_close to close socket.|
|bind(socketFd, (struct sockaddr*)&addr, sizeof addr); | omrsock_bind(socket, &addr); | - |
|listen(sockFd, 10); | omrsock_listen(serverSocket, 10); | - |
|struct sockaddr_storage newAddr; <br> socklen_t newAddrSize = sizeof newAddr; <br> int32_t newFd = accept(serverFd, (struct sockaddr *)&newAddr, &newAddrSize); | OMRSockAddrStorage newAddr; <br> omrsock_socket_t newSocket = NULL; <br> omrsock_accept(serverSocket, &newAddr, &newSocket); | User allocates both socket address structure and socket structure for newly returned connected server socket.|
|int32_t bytesSent = send(socketFd, msg, bytesToSend, 0); | int32_t bytesSent = omrsock_send(socket, (uint8_t *)msg, bytesToSend, 0); | - |
|char buf[SIZE]; <br> int32_t bytesRecv = recv(socketFd, buf, bytesToRecv, 0); | char buf[SIZE]; <br> int32_t bytesRecv = omrsock_recv(socket, (uint8_t *)buf, bytesToRecv, 0); | User allocates buffer array. |
|int32_t bytesSent = sendto(socketFd, msg, bytesToSend, 0, recvAddr, recvAddrLen); | int32_t bytesSent = omrsock_sendto(sendSocket, (uint8_t *)msg, bytesToSend, 0, &recvAddr); | - |
|char buf[SIZE]; <br> int32_t bytesRecv = recvfrom(recvSocket, buf, bytesToRecv, 0, &sentFromAddr, &sentFromAddrLen); | char buf[SIZE]; <br> int32_t bytesRecv = sock_recvfrom(recvSocket, (uint8_t *)buf, bytesToRecv, 0, &sentFromAddr); | - |
|struct pollfd pollArray[N]; <br> pollArray[0].fd = 0; <br> pollArray[0].events = POLLIN;| OMRPollFd pollArray[N]; <br> omrsock_pollfd_init(&pollArray[0], socket, OMRSOCK_POLLIN); | User allocates poll fd structure.|
|int32_t sockFd = pollArray[0].fd; <br> int16_t revents = pollArray[0].revents; | omrsock_socket_t socketPtr = NULL; <br> int16_t revents = 0; <br> omrsock_get_pollfd_info(&pollArray[0], &socketPtr, &revents); | User allocates socketPtr and revents to be returned.|
|int32_t numEvents = poll(pfds, N, time); | int32_t numReady = omrsock_poll(pollArray, N, time); | - |
|fd_set fdset; <br> FD_ZERO(&fdset); | OMRFdSet fdSet; <br> omrsock_fdset_zero(&fdSet); | User allocates fdset structure.|
|fd_set fdset; <br> FD_SET(socketFd, &fdset); | OMRFdSet fdSet; <br> omrsock_fdset_set(socket, &fdSet); | User allocates fdset structure.|
|fd_set fdset; <br> FD_CLR(socketFd, &fdset); | OMRFdSet fdSet; <br> omrsock_fdset_clr(socket, &fdSet); | User allocates fdset structure.|
|fd_set fdset; <br> bool socketSet = FD_ISSET(socketFd, &fdset); | OMRFdSet fdSet; <br> bool socketSet = omrsock_fdset_isset(socket, &fdSet); | User allocates fdset structure.|
|int32_t numSet = select(nfds, &readfds, NULL, NULL, &timeOut); | int32_t numSet = omrsock_select(&readfds, NULL, NULL, &timeOut); | - |
|close(socketFd); | omrsock_close(socket); | Frees memory allocated in @ref omrsock_socket.|
|uint16_t networkOrder = htons(val); | uint16_t networkOrder = omrsock_htons(val); | - |
|uint32_t networkOrder = htonl(val); | uint32_t networkOrder = omrsock_htonl(val); | - |
|int32_t addr; <br> inet_pton(AF_INET, "127.0.0.1", &addr); | uint8_t addr[4]; <br> omrsock_inet_pton(OMRSOCK_AF_INET, "127.0.0.1", &addr); | - |
|fcntl(socketFd, F_SETFL, O_NONBLOCK); | omrsock_fcntl(socket, OMRSOCK_O_NONBLOCK); | - |
|struct timeval time; <br> time.tv_sec = 2; <br> time.tv_usec = 500000;| OMRTimeval time; <br> omrsock_timeval_init(&time, 2, 500000); | User allocates timeval structure.|
|struct linger linger; <br> linger.l_onoff = 1; <br> linger.l_linger = 2;| OMRLinger linger; <br> omrsock_linger_init(&linger, 1, 2); | User allocates linger structure.|
|int32_t flag = 1; <br> setsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof flag);| int32_t flag = 1; <br> omrsock_setsockopt_int(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_KEEPALIVE, &flag); | - |
|struct linger linger; <br> linger.l_onoff = 1; <br> linger.l_linger = 2; <br> setsockopt(socketFd, SOL_SOCKET, SO_LINGER, &linger, sizeof linger); | OMRLinger linger; <br> omrsock_linger_init(&linger, 1, 2); <br> omrsock_setsockopt_linger(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_LINGER, &linger); | - |
|struct timeval time; <br> time.tv_sec = 2; <br> time.tv_usec = 500000; <br> setsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof time);| OMRTimeval time; <br> omrsock_timeval_init(&time, 2, 500000); <br> omrsock_setsockopt_timeval(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_RCVTIMEO, &timeVal); | - |
|int32_t flag, flagLen; <br> getsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &flag, &flagLen); | int32_t flag; <br> omrsock_getsockopt_int(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_KEEPALIVE, &flag); | - |
|struct linger linger; <br> int32_t len; <br> getsockopt(socketFd, SOL_SOCKET, SO_LINGER, &linger, &len); | OMRLinger linger; <br> omrsock_getsockopt_linger(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_LINGER, &linger); | - |
|struct timeval time; <br> int32_t len; <br> getsockopt(socketFd, SOL_SOCKET, SO_RCVTIMEO, &time, &len);| OMRTimeval time; <br> omrsock_getsockopt_timeval(socket, OMRSOCK_SOL_SOCKET, OMRSOCK_SO_RCVTIMEO, &time); | - |

___


### Linux to omrsock API Error Code Mapping

|Linux errno|omrsock API error|
|---|---|
|EACCES|OMRPORT_ERROR_SOCKET_NO_ACCESS|
|EADDRINUSE|OMRPORT_ERROR_SOCKET_ADDRINUSE|
|EADDRNOTAVAIL|OMRPORT_ERROR_SOCKET_ADDRNOTAVAIL|
|EAFNOSUPPORT|OMRPORT_ERROR_SOCKET_AF_UNSUPPORTED|
|EAGAIN| OMRPORT_ERROR_SOCKET_WOULDBLOCK|
|EALREADY|OMRPORT_ERROR_SOCKET_INPROGRESS|
|EINPROGRESS|OMRPORT_ERROR_SOCKET_INPROGRESS|
|EBADF|OMRPORT_ERROR_SOCKET_BAD_FILE_DESCRIPTOR|
|ECONNABORTED|OMRPORT_ERROR_SOCKET_CONNABORTED|
|ECONNREFUSED|OMRPORT_ERROR_SOCKET_CONNREFUSED|
|ECONNRESET|OMRPORT_ERROR_SOCKET_CONNRESET|
|EDESTADDRREQ|OMRPORT_ERROR_SOCKET_DESTADDRREQ|
|EDOM|OMRPORT_ERROR_SOCKET_DOMAIN|
|EFAULT|OMRPORT_ERROR_SOCKET_BAD_ADDRESS|
|EISDIR|OMRPORT_ERROR_SOCKET_BAD_ADDRESS|
|EINTR|OMRPORT_ERROR_SOCKET_INTERRUPTED|
|EINVAL|OMRPORT_ERROR_INVALID_ARGUMENTS|
|EIO|OMRPORT_ERROR_SOCKET_IO|
|ENXIO|OMRPORT_ERROR_SOCKET_IO|
|EISCONN|OMRPORT_ERROR_SOCKET_ALREADY_CONNECTED|
|ELOOP|OMRPORT_ERROR_SOCKET_PATH_NAME_LOOP|
|EMSGSIZE|OMRPORT_ERROR_SOCKET_MSGSIZE|
|EMFILE|OMRPORT_ERROR_SOCKET_MAX_FILE_OPENED_PROCESS|
|ENAMETOOLONG|OMRPORT_ERROR_SOCKET_NAMETOOLONG|
|ENETDOWN|OMRPORT_ERROR_SOCKET_NETDOWN|
|ENETUNREACH|OMRPORT_ERROR_SOCKET_NETUNREACH|
|ENFILE|OMRPORT_ERROR_SOCKET_MAX_FILE_OPENED_SYSTEM|
|ENOBUFS|OMRPORT_ERROR_SOCKET_NO_BUFFERS|
|ENODEV|OMRPORT_ERROR_SOCKET_NO_DEVICE|
|ENOENT|OMRPORT_ERROR_SOCKET_NO_FILE_ENTRY|
|ENOMEM|OMRPORT_ERROR_SOCKET_NOMEM|
|ENOSPC|OMRPORT_ERROR_SOCKET_NOMEM|
|ENOSYS|OMRPORT_ERROR_SOCKET_SYSTEM_FUNCTION_NOT_IMPLEMENTED|
|ENOTCONN|OMRPORT_ERROR_SOCKET_NOT_CONNECTED|
|ENOTDIR|OMRPORT_ERROR_SOCKET_NOTDIR|
|ENOTSOCK|OMRPORT_ERROR_SOCKET_NOTSOCK|
|EOPNOTSUPP|OMRPORT_ERROR_SOCKET_INVALID_OPERATION|
|EPERM|OMRPORT_ERROR_SOCKET_OPERATION_NOT_PERMITTED|
|EPIPE|OMRPORT_ERROR_SOCKET_BROKEN_PIPE|
|EPROTONOSUPPORT|OMRPORT_ERROR_SOCKET_PROTOCOL_UNSUPPORTED|
|EPROTOTYPE|OMRPORT_ERROR_SOCKET_BAD_PROTOCOL|
|EROFS|OMRPORT_ERROR_SOCKET_ROFS|
|ESOCKTNOSUPPORT|OMRPORT_ERROR_SOCKET_SOCKTYPE_UNSUPPORTED|
|ETIMEDOUT|OMRPORT_ERROR_SOCKET_TIMEOUT|
|EWOULDBLOCK|OMRPORT_ERROR_SOCKET_WOULDBLOCK|

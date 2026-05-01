/*******************************************************************************
 * Copyright IBM Corp. and others 2026
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

/**
 * @file
 * @ingroup Port
 * @brief Network information
 */

/**
 * windows.h defined UDATA. Ignore its definition to avoid redefinition errors.
 */
#define UDATA UDATA_win32_
#include <windows.h>
#undef UDATA
/**
 * To avoid WINSOCK redefinition errors.
 */
#if defined(OMR_OS_WINDOWS) && defined(_WINSOCKAPI_)
#undef _WINSOCKAPI_
#endif /* defined(OMR_OS_WINDOWS) && defined(_WINSOCKAPI_) */


#include "omrport.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

uintptr_t
omrnetwork_get_network_interfaces(struct OMRPortLibrary *portLibrary, OMRNetworkInterfaceCallback callback, void *userData)
{
	PMIB_IF_TABLE2 interfaceTable = NULL;
	DWORD getTableResult = 0;
	uintptr_t callbackResult = 0;
	DWORD interfaceIndex = 0;

	if (NULL == callback) {
		portLibrary->error_set_last_error_with_message(
				portLibrary,
				OMRPORT_ERROR_OPFAILED,
				"Callback function is NULL.");
		return (uintptr_t)OMRPORT_ERROR_OPFAILED;
	}

	/* Get the interface table. */
	getTableResult = GetIfTable2(&interfaceTable);
	if (NO_ERROR != getTableResult) {
		portLibrary->error_set_last_error_with_message(
				portLibrary,
				OMRPORT_ERROR_OPFAILED,
				"GetIfTable2() failed.");
		return (uintptr_t)OMRPORT_ERROR_OPFAILED;
	}

	/* Iterate through each interface. */
	for (interfaceIndex = 0; interfaceIndex < interfaceTable->NumEntries; interfaceIndex++) {
		MIB_IF_ROW2 *currentInterface = &interfaceTable->Table[interfaceIndex];
		char interfaceName[256];
		uint64_t rxBytes = 0;
		uint64_t txBytes = 0;

		/* Skip loopback and down interfaces. */
		if ((currentInterface->Type == IF_TYPE_SOFTWARE_LOOPBACK) 
			|| (currentInterface->OperStatus != IfOperStatusUp)
		) {
			continue;
		}

		/* Convert wide string alias to narrow string. */
		if (0 == WideCharToMultiByte(CP_UTF8, 0, currentInterface->Alias, -1, interfaceName, sizeof(interfaceName), NULL, NULL)) {
			continue;
		}

		/* Get statistics. */
		rxBytes = currentInterface->InOctets;
		txBytes = currentInterface->OutOctets;

		/* Invoke callback for this interface. */
		callbackResult = callback(interfaceName, rxBytes, txBytes, userData);
		if (0 != callbackResult) {
			break;
		}
	}

	FreeMibTable(interfaceTable);
	return callbackResult;
}

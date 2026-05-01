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
#include "omrport.h"

#if defined(LINUX) || defined(OSX)
#include <ifaddrs.h>
#endif /* defined(LINUX) || defined(OSX) */
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

uintptr_t
omrnetwork_get_network_interfaces(struct OMRPortLibrary *portLibrary, OMRNetworkInterfaceCallback callback, void *userData)
{
	uintptr_t callbackResult = 0;
	FILE *procNetDevFile = NULL;

	if (NULL == callback) {
		portLibrary->error_set_last_error_with_message(
				portLibrary,
				OMRPORT_ERROR_OPFAILED,
				"Callback function is NULL.");
		goto fail;
	}

	/* On Linux, read statistics from /proc/net/dev. */
	procNetDevFile = fopen("/proc/net/dev", "r");
	if (NULL != procNetDevFile) {
		char lineBuffer[256];
		/* Skip the first two header lines. */
		if (NULL == fgets(lineBuffer, sizeof(lineBuffer), procNetDevFile)) {
			goto fail;
		}
		if (NULL == fgets(lineBuffer, sizeof(lineBuffer), procNetDevFile)) {
			goto fail;
		}

		/* Process each interface line. */
		while (NULL != fgets(lineBuffer, sizeof(lineBuffer), procNetDevFile)) {
			char interfaceName[64];
			unsigned long long rxBytes = 0;
			unsigned long long rxPackets = 0;
			unsigned long long rxErrs = 0;
			unsigned long long rxDrop = 0;
			unsigned long long rxFifo = 0;
			unsigned long long rxFrame = 0;
			unsigned long long rxCompressed = 0;
			unsigned long long rxMulticast = 0;
			unsigned long long txBytes = 0;
			unsigned long long txPackets = 0;
			unsigned long long txErrs = 0;
			unsigned long long txDrop = 0;
			unsigned long long txFifo = 0;
			unsigned long long txColls = 0;
			unsigned long long txCarrier = 0;
			unsigned long long txCompressed = 0;

			/* Parse the line: interface: rxBytes rxPackets ... txBytes txPackets .... */
			int numStatsRead = sscanf(
					lineBuffer, "%63[^:]:%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
					interfaceName,
					&rxBytes, &rxPackets, &rxErrs, &rxDrop, &rxFifo, &rxFrame, &rxCompressed, &rxMulticast,
					&txBytes, &txPackets, &txErrs, &txDrop, &txFifo, &txColls, &txCarrier, &txCompressed);
			if (numStatsRead >= 9) {
				/* Remove leading/trailing whitespace from interface name. */
				char *trimmedName = interfaceName;
				char *nameEnd = NULL;
				while ((*trimmedName == ' ') || (*trimmedName == '\t')) {
					trimmedName++;
				}
				nameEnd = trimmedName + strlen(trimmedName) - 1;
				while ((nameEnd > trimmedName) && ((*nameEnd == ' ') || (*nameEnd == '\t'))) {
					*nameEnd = '\0';
					nameEnd--;
				}

				/* Invoke callback for this interface. */
				callbackResult = callback(trimmedName, (uint64_t)rxBytes, (uint64_t)txBytes, userData);
				if (0 != callbackResult) {
					break;
				}
			}
		}
		fclose(procNetDevFile);
	} else {
#if defined(LINUX) || defined(OSX)
		struct ifaddrs *interfaceList = NULL;
		struct ifaddrs *currentInterface = NULL;

		/* Get list of network interfaces. */
		if (-1 == getifaddrs(&interfaceList)) {
			portLibrary->error_set_last_error_with_message(
					portLibrary,
					OMRPORT_ERROR_OPFAILED,
					"getifaddrs() failed.");
			goto fail;
		}
		/* If /proc/net/dev is not available, just enumerate interfaces without statistics. */
		for (currentInterface = interfaceList; NULL != currentInterface; currentInterface = currentInterface->ifa_next) {
			uint64_t rxBytes = 0;
			uint64_t txBytes = 0;
			if (NULL == currentInterface->ifa_name) {
				continue;
			}
#if defined(OSX)
			if (NULL != currentInterface->ifa_data) {
				struct if_data *data = (struct if_data *)currentInterface->ifa_data;
				rxBytes = data->ifi_ibytes;
				txBytes = data->ifi_obytes;
			}
#endif /* defined(OSX) */
			callbackResult = callback(currentInterface->ifa_name, rxBytes, txBytes, userData);
			if (0 != callbackResult) {
				break;
			}
		}

		freeifaddrs(interfaceList);
#else /* defined(LINUX) || defined(OSX) */
		goto fail;
#endif /* defined(LINUX) || defined(OSX) */
	}

	return callbackResult;
fail:
	if (NULL != procNetDevFile) {
		fclose(procNetDevFile);
	}
	return (uintptr_t)OMRPORT_ERROR_OPFAILED;
}

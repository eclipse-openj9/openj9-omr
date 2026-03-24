/*******************************************************************************
 * Copyright IBM Corp. and others 2026
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/
#if defined(LINUX) || defined(OSX) || defined(OMR_OS_WINDOWS)

#include "omrcfg.h"
#include "omrport.h"
#include "testHelpers.hpp"

/**
 * Test data structure to collect network interface information.
 */
struct NetworkInterfaceTestData {
	uint32_t interfaceCount;
	bool foundLoopback;
	bool foundNonLoopback;
	uint64_t totalRxBytes;
	uint64_t totalTxBytes;
};

/**
 * Callback function for testing network_get_network_interfaces.
 * Collects statistics about discovered network interfaces.
 *
 * @param[in] name The name of the network interface.
 * @param[in] rxBytes Number of bytes received on this interface.
 * @param[in] txBytes Number of bytes transmitted on this interface.
 * @param[in] userData Pointer to NetworkInterfaceTestData structure.
 *
 * @return 0 to continue enumeration, non-zero to stop.
 */
static uintptr_t
networkInterfaceCallback(const char *name, uint64_t rxBytes, uint64_t txBytes, void *userData)
{
	NetworkInterfaceTestData *testData = static_cast<NetworkInterfaceTestData *>(userData);

	if (NULL == name) {
		return 1; /* Stop enumeration on error. */
	}

	testData->interfaceCount++;
	testData->totalRxBytes += rxBytes;
	testData->totalTxBytes += txBytes;

	/* Check for common loopback interface names. */
	if ((0 == strcmp(name, "lo")) || (0 == strcmp(name, "lo0"))) {
		testData->foundLoopback = true;
	} else {
		testData->foundNonLoopback = true;
	}

	return 0; /* Continue enumeration. */
}

/**
 * Callback function that stops enumeration after first interface.
 *
 * @param[in] name The name of the network interface.
 * @param[in] rxBytes Number of bytes received on this interface.
 * @param[in] txBytes Number of bytes transmitted on this interface.
 * @param[in] userData Pointer to NetworkInterfaceTestData structure.
 *
 * @return 1 to stop enumeration immediately.
 */
static uintptr_t
networkInterfaceCallbackStopEarly(const char *name, uint64_t rxBytes, uint64_t txBytes, void *userData)
{
	NetworkInterfaceTestData *testData = static_cast<NetworkInterfaceTestData *>(userData);

	if (NULL != name) {
		testData->interfaceCount++;
	}

	return 1; /* Stop enumeration after first interface. */
}

/**
 * Test if port library function pointer for network_get_network_interfaces is not NULL.
 */
TEST(PortNetworkTest, library_function_pointer_not_null)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	EXPECT_NE(OMRPORTLIB->network_get_network_interfaces, (void *)NULL);
}

/**
 * Test basic functionality of network_get_network_interfaces.
 *
 * This test verifies that:
 * - The function can be called successfully.
 * - At least one network interface is discovered.
 * - Interface names are provided.
 * - The callback is invoked for each interface.
 */
TEST(PortNetworkTest, get_network_interfaces_basic)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	NetworkInterfaceTestData testData = {0};
	uintptr_t result = OMRPORTLIB->network_get_network_interfaces(
		OMRPORTLIB,
		networkInterfaceCallback,
		&testData
	);

	EXPECT_EQ(result, (uintptr_t)0);
	EXPECT_GT(testData.interfaceCount, (uint32_t)0) << "Expected at least one network interface";
}

/**
 * Test that network_get_network_interfaces handles NULL callback gracefully.
 *
 * This test verifies that passing a NULL callback returns an appropriate error.
 */
TEST(PortNetworkTest, get_network_interfaces_null_callback)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	uintptr_t result = OMRPORTLIB->network_get_network_interfaces(
		OMRPORTLIB,
		NULL,
		NULL
	);

	EXPECT_NE(result, (uintptr_t)0) << "Expected error when callback is NULL";
}

/**
 * Test that callback can stop enumeration early.
 *
 * This test verifies that:
 * - The callback can return non-zero to stop enumeration.
 * - The function returns the callback's return value.
 * - Only one interface is counted when stopping early.
 */
TEST(PortNetworkTest, get_network_interfaces_early_stop)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	NetworkInterfaceTestData testData = {0};
	uintptr_t result = OMRPORTLIB->network_get_network_interfaces(
		OMRPORTLIB,
		networkInterfaceCallbackStopEarly,
		&testData
	);

	EXPECT_EQ(result, (uintptr_t)1) << "Expected callback return value to be propagated";
	EXPECT_EQ(testData.interfaceCount, (uint32_t)1) << "Expected enumeration to stop after first interface";
}

/**
 * Test that network interface statistics are reasonable.
 *
 * This test verifies that:
 * - RX and TX byte counts are non-negative (they're uint64_t, so always true).
 * - At least some interfaces report statistics (on supported platforms).
 *
 * Note: On some platforms or in some environments, statistics may not be available.
 * This test is lenient and only checks that the function completes successfully.
 */
TEST(PortNetworkTest, get_network_interfaces_statistics)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	NetworkInterfaceTestData testData = {0};
	uintptr_t result = OMRPORTLIB->network_get_network_interfaces(
		OMRPORTLIB,
		networkInterfaceCallback,
		&testData
	);

	EXPECT_EQ(result, (uintptr_t)0);
	EXPECT_GT(testData.interfaceCount, (uint32_t)0);

	/* Statistics may be zero on some platforms or if no traffic has occurred. */
	/* We just verify the function completed successfully. */
}

/**
 * Test that common network interfaces are discovered.
 *
 * This test verifies that:
 * - At least one interface is discovered.
 * - The loopback interface is typically present (though not guaranteed on all systems).
 *
 * Note: This test is lenient as interface availability varies by platform and configuration.
 */
TEST(PortNetworkTest, get_network_interfaces_common_interfaces)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());

	NetworkInterfaceTestData testData = {0};
	uintptr_t result = OMRPORTLIB->network_get_network_interfaces(
		OMRPORTLIB,
		networkInterfaceCallback,
		&testData
	);

	EXPECT_EQ(result, (uintptr_t)0);
	EXPECT_GT(testData.interfaceCount, (uint32_t)0);

	/* Most systems have a loopback interface, but we don't make it a hard requirement. */
	if (!testData.foundLoopback) {
		portTestEnv->log(LEVEL_WARN, "Warning: No loopback interface found. This may be expected on some systems.\n");
	}
}

#endif /* defined(LINUX) || defined(OSX) || defined(OMR_OS_WINDOWS) */

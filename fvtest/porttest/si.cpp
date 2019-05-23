/*******************************************************************************
 * Copyright (c) 1991, 2019 IBM Corp. and others
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

/*
 * $RCSfile: si.c,v $
 * $Revision: 1.64 $
 * $Date: 2012-12-05 05:27:54 $
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#if defined(OMR_OS_WINDOWS)
#include <direct.h>
#endif /* defined(OMR_OS_WINDOWS) */
#if !defined(OMR_OS_WINDOWS)
#include <grp.h>
#include <errno.h>
#if defined(J9ZOS390)
#include <limits.h>
#else
#define __STDC_LIMIT_MACROS
#include <stdint.h> /* For INT64_MAX. */
#endif /* defined(J9ZOS390) */
#include <sys/resource.h> /* For RLIM_INFINITY */
#endif /* !defined(OMR_OS_WINDOWS) */

#if defined(J9ZOS390)
#define _UNIX03_SOURCE
#include "atoe.h"
#endif

#include "testHelpers.hpp"

#if defined(OMR_OS_WINDOWS)
#define J9DIRECTORY_SEPARATOR_CHARACTER '\\'
#define J9FILE_EXTENSION ".exe"
#define J9FILE_EXTENSION_LENGTH (sizeof(J9FILE_EXTENSION) - 1)
#else
#define J9DIRECTORY_SEPARATOR_CHARACTER '/'
#endif /* defined(OMR_OS_WINDOWS) */

/* Under the standard compiler configuration, INT64_MAX is
 * not available; instead, LONGLONG_MAX is defined by xLC.
 */
#if defined(J9ZOS390) && !defined(INT64_MAX)
#define INT64_MAX LONGLONG_MAX
#endif /* defined(J9ZOS390) && !defined(INT64_MAX) */

/**
 * @brief Function takes an expected name for an executable and also the name that was actually
 * found using the Port API.  It then tries to match these, in a OS specific manner.  For example,
 * on Unices (and z/OS), executables names could be ./exec or full-path to the executable; on
 * windows we do not require ./ as long as the executable is in the current directory, and so on.
 *
 * @param expected The expected executable name (typically, argv[0]).
 * @param found The executable name as found by the Port API.
 *
 * @return Boolean true, if the names match; false, otherwise.
 */
BOOLEAN
validate_executable_name(const char *expected, const char *found)
{
	const char *expected_base_path = NULL;
	uintptr_t expected_length = 0;
	const char *found_base_path = NULL;
	uintptr_t found_length = 0;
	uintptr_t length = 0;

	if ((NULL == expected) || (NULL == found)) {
		return FALSE;
	}

	/* Extract the executable name from the expected path and path found.  Compare these. */
	expected_base_path = strrchr(expected, J9DIRECTORY_SEPARATOR_CHARACTER);
	/* Move past the directory separator, if found. */
	expected_base_path = (NULL != expected_base_path) ? (expected_base_path + 1) : expected;
	expected_length = strlen(expected_base_path);

	found_base_path = strrchr(found, J9DIRECTORY_SEPARATOR_CHARACTER);
	/* Move past the directory separator, if found. */
	found_base_path = (NULL != found_base_path) ? (found_base_path + 1) : found;
	found_length = strlen(found_base_path);

	length = found_length;

	/* On Windows, disregard comparing the extension, should this be dropped on the command
	 * line (API always returns executable name including the extension (.exe).
	 */
#if defined(OMR_OS_WINDOWS)
	/* Check whether argv0 ends with the extension ".exe".  If not, we need to reduce
	 * the number of characters to compare (against the executable name found by API).
	 */
	if (strncmp(expected_base_path + (expected_length - J9FILE_EXTENSION_LENGTH),
				J9FILE_EXTENSION,
				J9FILE_EXTENSION_LENGTH) != 0) {
		length -= J9FILE_EXTENSION_LENGTH;
	}
#endif /* defined(OMR_OS_WINDOWS) */
	if (length == expected_length) {
		if (strncmp(found_base_path, expected_base_path, length) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

TEST(PortSysinfoTest, sysinfo_test0)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	char *executable_name = NULL;
	const char *testName = "omrsysinfo_test0";
	char *argv0 = portTestEnv->_argv[0];

	reportTestEntry(OMRPORTLIB, testName);

#if defined(OMR_OS_WINDOWS)
	/* Remove extra "./" from the front of executable name. */
	if (0 == strncmp(argv0, "./", 2)) {
		argv0 = &argv0[2];
	}
#endif /* defined(OMR_OS_WINDOWS) */

	rc = omrsysinfo_get_executable_name(NULL, &executable_name);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "  Executable name test failed: (rc = %d).\n", rc);
		goto done;
	}

	if (validate_executable_name(argv0, 		 /* expected */
								 executable_name /* found through API */
								)
	) {
		portTestEnv->log("Executable name test passed.\n"
					  "Expected (argv0=%s).\n  Found=%s.\n",
					  argv0,
					  executable_name);
	} else {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Executable name test failed.\n"
						   "  Expected (argv0=%s).\n  Found=%s.\n",
						   argv0,
						   executable_name);
		goto done;
	}

#if 0
	/* TODO: Old tests that has yet to convert to the new format.
	 * Convert these tests to newer form, on an "as need" basis.
	 */
	uint32_t pid;
	int32_t bufSize = 575;		/*should sizeof(infoStirng), */
	U_16 classpathSeparator;
	uintptr_t number_CPUs;
	char *result;
	const char *osName, *osType, *osVersion,
		  char infoString[1000], envVar[100];

	HEADING(OMRPORTLIB, "SysInfo (si) test");


	pid = omrsysinfo_get_pid();
	portTestEnv->log("PLT pid=%d\n", pid);

	osVersion = omrsysinfo_get_OS_version();
	portTestEnv->log("OSversion=%s\n" osVersion ? osVersion : "NULL");

	strcpy(envVar, "PATH");		/*case matters?*/
	/*check rc before use: -1=BOGUS  0=good else nowrite, rc=varLength*/
	rc = omrsysinfo_get_env(envVar, infoString, bufSize);
	portTestEnv->log("%s=%s\nbufSize=%d rc=%d\n\n", envVar, infoString, bufSize, rc);
	/*
	 * note:  tty_printf has a implementation-dependent limit on out chars
	 * printf("\n\n%s=%s bufSize=%d rc=%d\n\n",envVar,infoString,bufSize,rc);
	 */

	osName = omrsysinfo_get_CPU_architecture();
	portTestEnv->log("CPU architecture=%s\n", osName ? osName : "NULL");

	osType = omrsysinfo_get_OS_type();
	portTestEnv->log("OS type=%s\n", osType ? osType : "NULL");

	/*SHOULD omrmem_free_memory() result, eh!!!!!! argv[0] not really used*/
	/*check rc before use: 0=good -1=error ???*/

	number_CPUs = omrsysinfo_get_number_CPUs_by_type(OMRPORT_CPU_ONLINE);
	portTestEnv->log("number of CPU(s)=%d\n", number_CPUs);


	portTestEnv->log("SI test done!\n\n");

#endif
done:
	reportTestExit(OMRPORTLIB, testName);
}

/*
 * Test various aspect of omrsysinfo_get_username
 */
TEST(PortSysinfoTest, sysinfo_test1)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
#define J9SYSINFO_TEST1_USERNAME_LENGTH 1024
	const char *testName = "omrsysinfo_test1";
	char username[J9SYSINFO_TEST1_USERNAME_LENGTH];
	intptr_t length, rc;

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_username(username, J9SYSINFO_TEST1_USERNAME_LENGTH);
	if (rc == -1) {
		portTestEnv->log(LEVEL_ERROR, "omrsysinfo_get_username returns -1.\n");
		portTestEnv->log(LEVEL_ERROR, "If this is a supported platform, consider this as a failure\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		char msg[256] = "";
		omrstr_printf(msg, sizeof(msg), "User name returned = \"%s\"\n", username);
		portTestEnv->log(msg);
	}

	length = strlen(username);
	rc = omrsysinfo_get_username(username, length - 1);

	if (length > rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Error if username buffer is too short: rc= %d, Expecting %d\n", rc, 1);
	}

	reportTestExit(OMRPORTLIB, testName);
}

TEST(PortSysinfoTest, hostname_test)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
#define J9SYSINFO_HOSTNAME_LENGTH 1024
	const char *testName = "omrsysinfo_hostname_test";
	char hostname[J9SYSINFO_HOSTNAME_LENGTH];
	intptr_t rc = 0;

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_hostname(hostname, J9SYSINFO_HOSTNAME_LENGTH);
	if (rc == -1) {
		portTestEnv->log(LEVEL_ERROR, "omrsysinfo_get_hostname returned -1.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		char msg[256] = "";
		omrstr_printf(msg, sizeof(msg), "Host name returned = \"%s\"\n", hostname);
		portTestEnv->log(msg);
	}
	/* Don't check for buffers that are too small, since the call to gethostname() will
	 * silently truncate the name and return 0 on some platforms, e.g. MacOS.
	 */
	reportTestExit(OMRPORTLIB, testName);
}

TEST(PortSysinfoTest, sysinfo_test2)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
#define J9SYSINFO_TEST2_GROUPNAME_LENGTH 1024
	const char *testName = "omrsysinfo_test2";
	char groupname[J9SYSINFO_TEST2_GROUPNAME_LENGTH];
	intptr_t length, rc;

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_groupname(groupname, J9SYSINFO_TEST2_GROUPNAME_LENGTH);
	if (rc == -1) {
		portTestEnv->log(LEVEL_ERROR, "omrsysinfo_get_groupname returns -1.\n");
		portTestEnv->log(LEVEL_ERROR, "If this is a supported platform, consider this as a failure\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		char msg[256] = "";
		omrstr_printf(msg, sizeof(msg), "Group name returned = \"%s\"\n", groupname);
		portTestEnv->log(msg);
	}

	length = strlen(groupname);
	rc = omrsysinfo_get_groupname(groupname, length - 1);

	if (length > rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Error if groupname buffer is too short: rc= %d, Expecting %d\n", rc, 1);
	}

	reportTestExit(OMRPORTLIB, testName);
}

TEST(PortSysinfoTest, sysinfo_get_OS_type_test)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
#define J9SYSINFO_TEST3_OSNAME_LENGTH 1024
	const char *testName = "omrsysinfo_get_OS_type_test";
	const char *osName = NULL;
	const char *osVersion = NULL;

	reportTestEntry(OMRPORTLIB, testName);

	osName = omrsysinfo_get_OS_type();
	if (osName == NULL) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_OS_type returned NULL - expected OS name.\n", 0, 1);
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		char msg[256];
		omrstr_printf(msg, sizeof(msg), "omrsysinfo_get_OS_type returned : \"%s\"\n", osName);
		portTestEnv->log(msg);
	}

	osVersion = omrsysinfo_get_OS_version();
	if (osVersion == NULL) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_OS_version returned NULL - expected OS name.\n", 0, 1);
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		char msg[256];
		omrstr_printf(msg, sizeof(msg), "omrsysinfo_get_OS_version returned : \"%s\"\n", osVersion);
		portTestEnv->log(msg);
	}

#if defined(OMR_OS_WINDOWS)
	if (NULL == strstr(osName, "Windows")) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_OS_version does not contain \"Windows\".\n", 0, 1);
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else if (0 == strcmp(osName, "Windows")) {
		/*
		 * This means we are running a new, unrecognized version of Windows.  We need to update  omrsysinfo_get_OS_type
		 * to recognize the specific version.
		 */
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_OS_version returned the default Windows version.\n", 0, 1);
		reportTestExit(OMRPORTLIB, testName);
		return;
	}
#endif /* defined(OMR_OS_WINDOWS) */
	reportTestExit(OMRPORTLIB, testName);
}

TEST(PortSysinfoTest, sysinfo_test3)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test3";
	J9PortSysInfoLoadData loadData;
	intptr_t rc;

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_load_average(&loadData);
	if (rc == -1) {
		portTestEnv->log(LEVEL_ERROR, "omrsysinfo_get_load_average returns -1.\n");
		portTestEnv->log(LEVEL_ERROR, "If this is a supported platform, consider this as a failure\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		portTestEnv->log("Returned data\n");
		portTestEnv->log("One Minute Average: %lf\n", loadData.oneMinuteAverage);
		portTestEnv->log("Five Minute Average: %lf\n", loadData.fiveMinuteAverage);
		portTestEnv->log("Fifteen Minute Average: %lf\n", loadData.fifteenMinuteAverage);
	}

	reportTestExit(OMRPORTLIB, testName);
}


TEST(PortSysinfoTest, sysinfo_test_sysinfo_ulimit_iterator)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_ulimit_iterator";
	int32_t rc = 0;

	J9SysinfoLimitIteratorState state;
	J9SysinfoUserLimitElement element;

	reportTestEntry(OMRPORTLIB, testName);
	portTestEnv->changeIndent(1);

	rc = omrsysinfo_limit_iterator_init(&state);

	if (0 != rc) {

		if (OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM == rc) {
			portTestEnv->log(LEVEL_ERROR, "This platform does not support the limit iterator\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_limit_iterator_init returned: %i\n", rc);
		}
		goto done;
	}

	while (omrsysinfo_limit_iterator_hasNext(&state)) {

		rc = omrsysinfo_limit_iterator_next(&state, &element);

		if (0 == rc) {
			portTestEnv->log("%s:\n", element.name);

			if (OMRPORT_LIMIT_UNLIMITED == element.softValue) {
				portTestEnv->log(" soft: unlimited\n");
			} else {
				portTestEnv->log(" soft: 0x%zX\n", element.softValue);
			}

			if (OMRPORT_LIMIT_UNLIMITED == element.hardValue) {
				portTestEnv->log(" hard: unlimited\n");
			} else {
				portTestEnv->log(" hard: 0x%zX\n", element.hardValue);
			}

		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_limit_iterator_next returned: %i when 0 was expected\n", rc);
		}
	}

done:

	portTestEnv->changeIndent(-1);
	reportTestExit(OMRPORTLIB, testName);
}


/**
 *
 * Test the iterator by supplying it with a:
 *
 * 1. NULL buffer
 * 2. Buffer that is big enough for the entire environment
 * 3. Buffer that is big enough to contain an env var, but not big enough for the entire environment
 * 4. non-null buffer that is not big enough to contain an env var.
 *
 * In cases but the first (null buffer) attempt to iterate.
 */
TEST(PortSysinfoTest, sysinfo_test_sysinfo_env_iterator)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_env_iterator";
	int32_t rc = 0;
	intptr_t envSize = 0;
	J9SysinfoEnvIteratorState state;
	J9SysinfoEnvElement element;
	void *buffer = NULL;
	uint32_t bufferSizeBytes = 0;
	int l = 0;
#undef SI_DEBUG

	reportTestEntry(OMRPORTLIB, testName);
	portTestEnv->changeIndent(1);

	/* Test 1: NULL buffer - Pass in NULL for buffer, make sure we get back a positive integer describing the size, or a crash */
	buffer = NULL;
	rc = omrsysinfo_env_iterator_init(&state, buffer, bufferSizeBytes);

	if (rc < 0) {
		if (OMRPORT_ERROR_NOT_SUPPORTED_ON_THIS_PLATFORM == rc) {
			portTestEnv->log(LEVEL_ERROR, "This platform does not support the env iterator\n");
		} else if (OMRPORT_ERROR_SYSINFO_ENV_INIT_CRASHED_COPYING_BUFFER == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tCrash passing in NULL buffer while running single-threaded. This is a failure because no-one else should have been able to modify it\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_init returned: %i\n", rc);
		}

		goto done;

	} else {
		envSize = rc;
#if defined(SI_DEBUG)
		portTestEnv->log("Need a buffer of size %x bytes\n", envSize);
#endif
	}

	/* Test 2: Buffer that is big enough for the entire environment */

	buffer = omrmem_allocate_memory(envSize, OMRMEM_CATEGORY_PORT_LIBRARY);
	if (NULL == buffer) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "OutOfMemory allocating buffer for test - where's all the memory?");
		goto done;
	}

	bufferSizeBytes = (uint32_t)envSize;
	rc = omrsysinfo_env_iterator_init(&state, buffer, bufferSizeBytes);

	if (rc != 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_init returned: %i\n", rc);
		goto done;
	}

	portTestEnv->log("Environment:\n\n");

	l = 0;
	while (omrsysinfo_env_iterator_hasNext(&state)) {
		rc = omrsysinfo_env_iterator_next(&state, &element);

		if (0 == rc) {
			portTestEnv->log("%s\n", element.nameAndValue);
			l++;
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_next returned: %i when 0 was expected\n", rc);
			goto done;
			break;
		}
	}

	/* Test3: Buffer that is big enough to contain an env var, but not big enough for the entire environment */
	omrmem_free_memory(buffer);

	bufferSizeBytes = (uint32_t)envSize - 100;
	buffer = omrmem_allocate_memory(bufferSizeBytes, OMRMEM_CATEGORY_PORT_LIBRARY);

	if (NULL == buffer) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "OutOfMemory allocating buffer for test - where's all the memory?");
		goto done;
	}

	rc = omrsysinfo_env_iterator_init(&state, buffer, bufferSizeBytes);

	if (rc < 0) {
		if (OMRPORT_ERROR_SYSINFO_ENV_INIT_CRASHED_COPYING_BUFFER == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tCrash passing in NULL buffer while running single-threaded. This is a failure because no-one else should have been able to modify it\n");
		}
		outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_init returned: %i\n", rc);
		goto done;
	} else {
		envSize = rc;
#if defined(SI_DEBUG)
		portTestEnv->log("Should have a buffer of size %x bytes, using one of size %x instead, which will result in a truncated environment\n", envSize, bufferSizeBytes);
#endif
	}

	l = 0;
	while (omrsysinfo_env_iterator_hasNext(&state)) {

		rc = omrsysinfo_env_iterator_next(&state, &element);

#if defined(SI_DEBUG)
		portTestEnv->log("si.c element.nameAndValue @ 0x%p: %s\n", element.nameAndValue, element.nameAndValue);
#endif

		if (0 == rc) {
			l++;
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_next returned: %i when 0 was expected\n", rc);
			goto done;
			break;
		}
	}

	/* Test 4. non-null buffer that is not big enough to contain an env var. */
	omrmem_free_memory(buffer);

	bufferSizeBytes = 1;
	buffer = omrmem_allocate_memory(bufferSizeBytes, OMRMEM_CATEGORY_PORT_LIBRARY);
	if (NULL == buffer) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "OutOfMemory allocating buffer for test - where's all the memory?");
		goto done;
	}

	rc = omrsysinfo_env_iterator_init(&state, buffer, bufferSizeBytes);

	if (rc < 0) {
		if (OMRPORT_ERROR_SYSINFO_ENV_INIT_CRASHED_COPYING_BUFFER == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tCrash passing in NULL buffer while running single-threaded. This is a failure because no-one else should have been able to modify it\n");
		}
		outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_init returned: %i\n", rc);
		goto done;
	} else {
		envSize = rc;
#if defined(SI_DEBUG)
		portTestEnv->log("Should have a buffer of size %x bytes, using one of size %x instead, which will result in a truncated environment\n", envSize, bufferSizeBytes);
#endif
	}

	l = 0;
	while (omrsysinfo_env_iterator_hasNext(&state)) {

		rc = omrsysinfo_env_iterator_next(&state, &element);

#if defined(SI_DEBUG)
		portTestEnv->log("si.c element.nameAndValue @ 0x%p: %s\n", element.nameAndValue, element.nameAndValue);
#endif

		if (0 == rc) {
			l++;
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "\tomrsysinfo_env_iterator_next returned: %i when 0 was expected\n", rc);
			goto done;
			break;
		}
	}

done:

	if (NULL != buffer) {
		omrmem_free_memory(buffer);
	}

	portTestEnv->changeIndent(-1);
	reportTestExit(OMRPORTLIB, testName);
}


/* sysinfo_set_limit and sysinfo_get_limit tests will not work on windows */
#if !defined(OMR_OS_WINDOWS)
#if !(defined(AIXPPC) || defined(J9ZOS390))
/**
 *
 * Test omrsysinfo_test_sysinfo_set_limit and omrsysinfo_test_sysinfo_get_limit
 * with resourceID OMRPORT_RESOURCE_ADDRESS_SPACE
 *
 */
TEST(PortSysinfoTest, sysinfo_test_sysinfo_set_limit_ADDRESS_SPACE)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_set_limit_ADDRESS_SPACE";
	intptr_t rc;
	uint64_t originalCurLimit;
	uint64_t originalMaxLimit;
	uint64_t currentLimit;
	const uint64_t as1 = 300000;

	reportTestEntry(OMRPORTLIB, testName);

	/* save original soft limit */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_ADDRESS_SPACE, &originalCurLimit);

	rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_ADDRESS_SPACE, as1);
	if (rc == -1) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_ADDRESS_SPACE, &currentLimit);

	if (as1 == currentLimit) {
		portTestEnv->log("omrsysinfo_set_limit set ADDRESS_SPACE soft max successful\n");
	} else {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set ADDRESS_SPACE soft max FAILED\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	/* save original hard limit */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, &originalMaxLimit);

	/* lowering the hard limit is irreversible unless privileged */
	if (geteuid()) {
		/* we should be able to set the hard limit to the current value as an unprivileged user
		   When the hard limit is set to unlimited (-1) a regular user can change it to
		   any value, but not back to unlimited. */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, originalMaxLimit);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1\n");
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, &currentLimit);
		if (originalMaxLimit == currentLimit) {
			portTestEnv->log("omrsysinfo_set_limit set ADDRESS_SPACE hard max successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set ADDRESS_SPACE hard max FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

	} else {
		/* now try setting the hard limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, as1 + 1);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, &currentLimit);

		if (as1 + 1 == currentLimit) {
			portTestEnv->log("omrsysinfo_set_limit set ADDRESS_SPACE hard max successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set ADDRESS_SPACE hard max FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}


		/* restore original hard limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_ADDRESS_SPACE | OMRPORT_LIMIT_HARD, originalMaxLimit);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original hard limit failed omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	}

	/* restore original soft limit
	   The soft limit is always <= the hard limit. If the hard limit is lowered to below the soft limit and
	   then raised again the soft limit isn't automatically raised. */
	rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_ADDRESS_SPACE, originalCurLimit);
	if (rc == -1) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original soft limit failed omrsysinfo_set_limit returns -1.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	reportTestExit(OMRPORTLIB, testName);
}
#endif /* !(defined(AIXPPC) || defined(J9ZOS390)) */

/**
 *
 * Test omrsysinfo_test_sysinfo_set_limit and omrsysinfo_test_sysinfo_get_limit
 * with resourceID OMRPORT_RESOURCE_CORE_FILE
 *
 */
TEST(PortSysinfoTest, sysinfo_test_sysinfo_set_limit_CORE_FILE)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_set_limit_CORE_FILE";
	intptr_t rc;
	uint64_t originalCurLimit;
	uint64_t originalMaxLimit;
	uint64_t currentLimit;
	const uint64_t coreFileSize = 42;

	reportTestEntry(OMRPORTLIB, testName);

	/* save original soft limit */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FILE, &originalCurLimit);

	rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE, coreFileSize);
	if (rc == -1) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FILE, &currentLimit);

	if (coreFileSize == currentLimit) {
		portTestEnv->log("omrsysinfo_set_limit set CORE_FILE soft max successful\n");
	} else {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set CORE_FILE soft max FAILED\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}


	/* save original hard limit */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, &originalMaxLimit);

	/* lowering the hard limit is irreversible unless privileged */
	if (geteuid()) {
		/* we should be able to set the hard limit to the current value as an unprivileged user
		   When the hard limit is set to unlimited (-1) a regular user can change it to
		   any value, but not back to unlimited. */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, originalMaxLimit);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1\n");
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, &currentLimit);
		if (originalMaxLimit == currentLimit) {
			portTestEnv->log("omrsysinfo_set_limit set CORE_FILE hard max successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set CORE_FILE hard max FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

	} else {

		/* now try setting the hard limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, coreFileSize + 1);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, &currentLimit);
		if (coreFileSize + 1 == currentLimit) {
			portTestEnv->log("omrsysinfo_set_limit set CORE_FILE hard max successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set CORE_FILE hard max FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* restore original hard limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE | OMRPORT_LIMIT_HARD, originalMaxLimit);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original hard limit failed omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	}

	/* restore original soft limit
	   The soft limit is always <= the hard limit. If the hard limit is lowered to below the soft limit and
	   then raised again the soft limit isn't automatically raised. */
	rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE, originalCurLimit);
	if (rc == -1) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original soft limit failed omrsysinfo_set_limit returns -1.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	reportTestExit(OMRPORTLIB, testName);
}

	/**
	 *
	 * Test omrsysinfo_test_sysinfo_set_limit and omrsysinfo_test_sysinfo_get_limit
	 * with resourceID OMRPORT_RESOURCE_FILE_DESCRIPTORS
	 *
	 */
	TEST(PortSysinfoTest, sysinfo_test_sysinfo_set_limit_FILE_DESCRIPTORS)
	{
		OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
		const char *testName = "omrsysinfo_test_sysinfo_set_limit_FILE_DESCRIPTORS";
		uint32_t rc = OMRPORT_LIMIT_UNKNOWN;
		uint64_t originalSoftLimit = 0;
		uint64_t finalSoftLimit = 0;
		uint64_t softSetToHardLimit = 0;
		uint64_t originalHardLimit = 0;
		uint64_t currentLimit = 0;
		const uint64_t descriptorLimit = 256;

		reportTestEntry(OMRPORTLIB, testName);

		/* save original soft limit */
		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, &originalSoftLimit);
		if (OMRPORT_LIMIT_UNKNOWN == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
		portTestEnv->log(LEVEL_ERROR, "originalSoftLimit=%llu\n", originalSoftLimit);
		finalSoftLimit = originalSoftLimit;

		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, descriptorLimit);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit FAILED rc=%d\n", rc);
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, &currentLimit);
		if (OMRPORT_LIMIT_UNKNOWN == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
		if (descriptorLimit == currentLimit) {
			portTestEnv->log("omrsysinfo_set_limit set FILE_DESCRIPTORS soft limit successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS,
					"omrsysinfo_set_limit set FILE_DESCRIPTORS soft limit FAILED originalSoftLimit=%lld Expected=%lld actual==%lld\n",
					originalSoftLimit, descriptorLimit, currentLimit);
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* save original hard limit */
		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, &originalHardLimit);
		if (OMRPORT_LIMIT_UNKNOWN == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
		portTestEnv->log(LEVEL_ERROR, "originalHardLimit=%llu\n", originalHardLimit);

		/* set soft limit to hard limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, originalHardLimit);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit soft = hard FAILED rc=%d\n", rc);
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* get new soft limit */
		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, &softSetToHardLimit);
		if (OMRPORT_LIMIT_UNKNOWN == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
		portTestEnv->log(LEVEL_ERROR, "soft set to hard limit=%llu\n", softSetToHardLimit);

		/* set soft limit to old value */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, originalSoftLimit);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit reset soft FAILED rc=%d\n", rc);
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, &currentLimit);
		if (currentLimit != originalHardLimit) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: hard limit changed\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* lowering the hard limit is irreversible unless privileged */
		if (0 != geteuid()) { /* normal user */
			/* setting the hard limit from unlimited to a finite value has unpredictable results:
			 * the actual value may be much smaller than requested.
			 * In that case, just try setting it to its current value (softSetToHardLimit) or a value slightly lower.
			 * Ensure that we don't try to set the hard limit to a value less than the current soft limit
			 * (i.e. originalSoftLimit).
			 */
			uint64_t newHardLimit =  ((OMRPORT_LIMIT_UNLIMITED == rc) || (originalSoftLimit == softSetToHardLimit))
					? softSetToHardLimit: softSetToHardLimit - 1;

			rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, newHardLimit);
			if (0 != rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set hard limit=%lld FAILED rc=%d\n", rc, newHardLimit);
				reportTestExit(OMRPORTLIB, testName);
				return;
			}

			rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, &currentLimit);
			if (OMRPORT_LIMIT_UNKNOWN == rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
				reportTestExit(OMRPORTLIB, testName);
				return;
			}
			if (newHardLimit == currentLimit) {
				portTestEnv->log("omrsysinfo_set_limit set FILE_DESCRIPTORS hard limit successful\n");
			} else {
				outputErrorMessage(PORTTEST_ERROR_ARGS,
						"omrsysinfo_set_limit set FILE_DESCRIPTORS hard limit FAILED originalHardLimit=%lld Expected=%lld actual==%lld\n",
						originalHardLimit, newHardLimit, currentLimit);
				reportTestExit(OMRPORTLIB, testName);
				return;
			}
			finalSoftLimit = (originalSoftLimit > newHardLimit)? newHardLimit: originalSoftLimit;
		} else { /* running as root */
			const uint64_t newHardLimit = 257;
			rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, newHardLimit);
			if (0 != rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit FAILED rc=%d\n", rc);
				reportTestExit(OMRPORTLIB, testName);
				return;
			}

			rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, &currentLimit);
			if (OMRPORT_LIMIT_UNKNOWN == rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_limit FAILED: OMRPORT_LIMIT_UNKNOWN\n");
				reportTestExit(OMRPORTLIB, testName);
				return;
			}
			if (newHardLimit == currentLimit) {
				portTestEnv->log("omrsysinfo_set_limit set FILE_DESCRIPTORS hard limit successful\n");
			} else {
				outputErrorMessage(PORTTEST_ERROR_ARGS,
					"omrsysinfo_set_limit set FILE_DESCRIPTORS hard max FAILED. Expected=%lld actual=%lld\n",
					(descriptorLimit + 1), currentLimit);
				reportTestExit(OMRPORTLIB, testName);
				return;
			}

			/* restore original hard limit */
			rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, originalHardLimit);
			if (0 != rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original hard limit FAILED rc=%d\n", rc);
				reportTestExit(OMRPORTLIB, testName);
				return;
			}
		}

	/* restore original soft limit
	   The soft limit is always <= the hard limit. If the hard limit is lowered to below the soft limit and
	   then raised again the soft limit isn't automatically raised. */
	rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, finalSoftLimit);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original soft limit FAILED rc=%d\n", rc);
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	reportTestExit(OMRPORTLIB, testName);
}

#if defined(AIXPPC)
/**
 *
 * Test omrsysinfo_test_sysinfo_set_limit and omrsysinfo_test_sysinfo_get_limit
 * with resourceID OMRPORT_RESOURCE_CORE_FLAGS
 *
 */
TEST(PortSysinfoTest, sysinfo_test_sysinfo_set_limit_CORE_FLAGS)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_set_limit_CORE_FLAGS";
	intptr_t rc;
	uint64_t currentLimit;
	uint64_t originalLimit;
	int32_t lastErrorNumber = 0;

	reportTestEntry(OMRPORTLIB, testName);

	if (geteuid()) {
		portTestEnv->log(LEVEL_ERROR, "You must be root to set core flags\n");

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FLAGS, &currentLimit);
		if (-1 == rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit get AIX full core value failed\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FLAGS, 1);
		lastErrorNumber = omrerror_last_error_number();

		if (OMRPORT_ERROR_FILE_NOPERMISSION == lastErrorNumber) {
			portTestEnv->log(LEVEL_ERROR, "omrsysinfo_set_limit CORE_FLAGS failed as expected because we aren't root.\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit CORE_FLAGS did not fail with the proper error.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

	} else {

		/* save original soft limit */
		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FLAGS, &originalLimit);

		/* try setting core flags to 0 */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FLAGS, 0);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FLAGS, &currentLimit);
		if ((rc == OMRPORT_LIMIT_LIMITED) &&
			(0 == currentLimit)) {
			portTestEnv->log("omrsysinfo_set_limit set AIX full core value to 0 successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set CORE_FLAGS FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* try setting core flags to 1 (unlimited) */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FLAGS, 1);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_CORE_FLAGS, &currentLimit);
		if ((OMRPORT_LIMIT_UNLIMITED == rc) &&
			(U_64_MAX == currentLimit)) {
			portTestEnv->log("omrsysinfo_set_limit set AIX full core value to 1 successful\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_set_limit set CORE_FLAGS FAILED\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}


		/* restore original limit */
		rc = omrsysinfo_set_limit(OMRPORT_RESOURCE_CORE_FILE, originalLimit);
		if (rc == -1) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "restoring the original AIX full core value failed omrsysinfo_set_limit returns -1.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	}

	reportTestExit(OMRPORTLIB, testName);
}
#endif /* defined(AIXPPC) */

/**
 * Test omrsysinfo_test_sysinfo_get_limit for resource OMRPORT_RESOURCE_FILE_DESCRIPTORS.
 * API available on all Unix platforms.
 */
TEST(PortSysinfoTest, sysinfo_test_sysinfo_get_limit_FILE_DESCRIPTORS)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_sysinfo_get_limit_FILE_DESCRIPTORS";
	uint32_t rc = 0;
	uint64_t curLimit = 0;
	uint64_t maxLimit = 0;

	reportTestEntry(OMRPORTLIB, testName);
	/* First, get the current (soft) limit on the resource nofiles. */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, &curLimit);
	if (OMRPORT_LIMIT_UNLIMITED == rc) { /* Not an error, just that it is not configured. */
		/* If the API reported this limit as set to "unlimited", the resource limit must be
		 * set to implementation-defined limit, RLIM_INFINITY.
		 */
		if (RLIM_INFINITY == curLimit) {
			portTestEnv->log(
				"omrsysinfo_get_limit(nofiles): soft limit=RLIM_INFINITY (unlimited).\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_get_limit(nofiles): soft limit (unlimited), bad maximum reported %lld.\n",
				((int64_t) curLimit));
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else if (OMRPORT_LIMIT_LIMITED == rc) {
		if ((((int64_t) curLimit) > 0) && (((int64_t) curLimit) <= INT64_MAX)) {
			portTestEnv->log("omrsysinfo_get_limit(nofiles) soft limit: %lld.\n",
				((int64_t) curLimit));
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_get_limit(nofiles) failed: bad limit received!\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else { /* The port library failed! */
		outputErrorMessage(PORTTEST_ERROR_ARGS, 
			"omrsysinfo_get_limit(nofiles): failed with error code=%d.\n",
			omrerror_last_error_number());
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	/* Now, for the hard limit. */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS | OMRPORT_LIMIT_HARD, &maxLimit);
	if (OMRPORT_LIMIT_UNLIMITED == rc) {
		/* Not an error, just that it is not configured.  Ok to compare!. */
		if (RLIM_INFINITY == maxLimit) {
			portTestEnv->log(
				"omrsysinfo_get_limit(nofiles): hard limit = RLIM_INFINITY (unlimited).\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_get_limit(nofiles): hard limit (unlimited), bad maximum reported %lld.\n",
				((int64_t) curLimit));
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else if (OMRPORT_LIMIT_LIMITED == rc) {
		if ((((int64_t) maxLimit) > 0) && (((int64_t) maxLimit) <= INT64_MAX)) {
			portTestEnv->log("omrsysinfo_get_limit(nofiles) hard limit: %lld.\n",
				((int64_t) maxLimit));
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_get_limit(nofiles) failed: bad limit received!\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else { /* The port library failed! */
		outputErrorMessage(PORTTEST_ERROR_ARGS, 
			"omrsysinfo_get_limit(nofiles): failed with error code=%d.\n",
			omrerror_last_error_number());
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	/* Ensure that the resource's current (soft) limit does not exceed the hard limit. */
	if (curLimit > maxLimit) {
		outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_get_limit(nofiles): current limit exceeds the hard limit.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}
	reportTestExit(OMRPORTLIB, testName);
}
#endif /* !defined(OMR_OS_WINDOWS) */

/* Since the processor and memory usage port library APIs are not available on zOS (neither
 * 31-bit not 64-bit) yet, so we exclude these tests from running on zOS. When the zOS
 * implementation is available, we must remove these guards so that they are built and
 * executed on the z as well. Remove this comment too.
 */
#if !defined(J9ZOS390)
/**
 * Test for omrsysinfo_get_memory_info() port library API. Check that we are indeed
 * able to retrieve memory statistics on all supported platforms and also, perform
 * some minimum sanity checks.
 */
TEST(PortSysinfoTest, sysinfo_testMemoryInfo)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_testMemoryInfo";
	intptr_t rc = 0;
	J9MemoryInfo memInfo = {0};

	reportTestEntry(OMRPORTLIB, testName);
	portTestEnv->changeIndent(1);

	rc = omrsysinfo_get_memory_info(&memInfo);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_memory_info() failed.\n");
	} else {
		/* If any of these parameters are set to OMRPORT_MEMINFO_NOT_AVAILABLE on platforms
		 * where they are supposed to be defined, flag an error to fail pltest.
		 */
		if ((OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.totalPhysical)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.availPhysical)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.totalSwap)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.availSwap)
#if defined(OMR_OS_WINDOWS) || defined(OSX)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.totalVirtual)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.availVirtual)
#else /* defined(OMR_OS_WINDOWS) || defined(OSX) */
			/* We do not check totalVirtual since it may be set to some value or -1, depending
			 * on whether there is a limit set for this or not on the box.
			 */
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE != memInfo.availVirtual)
#endif /* defined(OMR_OS_WINDOWS) || defined(OSX) */
#if defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined(OSX)
			/* Size of the file buffer area is not available on Windows, AIX and OSX. Therefore,
			 * it must be set to OMRPORT_MEMINFO_NOT_AVAILABLE.
			 */
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE != memInfo.buffered)
#else /* defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined(OSX) */
			/* On platforms where buffer area is defined, OMRPORT_MEMINFO_NOT_AVAILABLE is
			 * surely a failure!
			 */
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.buffered)
#endif /* defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined(OSX) */
#if defined (OSX)
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE != memInfo.cached)
#else /* defined (OSX) */
			|| (OMRPORT_MEMINFO_NOT_AVAILABLE == memInfo.cached)
#endif /* defined(OSX) */
		) {

			/* Fail pltest if one of these memory usage parameters were found inconsistent. */
			outputErrorMessage(PORTTEST_ERROR_ARGS, "Invalid memory usage statistics retrieved.\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}

		/* Validate the statistics that we obtained. */
		if ((memInfo.totalPhysical > 0) &&
			(memInfo.availPhysical <= memInfo.totalPhysical) &&
#if defined(OMR_OS_WINDOWS)
			/* Again, it does not make sense to do checks and comparisons on Virtual Memory
			 * on places other than Windows.
			 */
			(memInfo.totalVirtual > 0) &&
			(memInfo.availVirtual <= memInfo.totalVirtual) &&
#endif /* defined(OMR_OS_WINDOWS) */
			(memInfo.availSwap <= memInfo.totalSwap) &&
			(memInfo.timestamp > 0)) {

			/* Print out the memory usage statistics. */
			portTestEnv->log("Retrieved memory usage statistics.\n");
			portTestEnv->log("Total physical memory: %llu bytes.\n", memInfo.totalPhysical);
			portTestEnv->log("Available physical memory: %llu bytes.\n", memInfo.availPhysical);
#if defined(OMR_OS_WINDOWS) || defined(OSX)
			portTestEnv->log("Total virtual memory: %llu bytes.\n", memInfo.totalVirtual);
			portTestEnv->log("Available virtual memory: %llu bytes.\n", memInfo.availVirtual);
#else /* defined(OMR_OS_WINDOWS) || defined(OSX) */
			/* This may or may not be available depending on whether a limit is set. Print out if this
			 * is available or else, call this parameter "undefined".
			 */
			if (OMRPORT_MEMINFO_NOT_AVAILABLE != memInfo.totalVirtual) {
				portTestEnv->log("Total virtual memory: %llu bytes.\n", memInfo.totalVirtual);
			} else {
				portTestEnv->log("Total virtual memory: <undefined>.\n");
			}
			/* Leave Available Virtual memory parameter as it is on non-Windows Platforms. */
			portTestEnv->log("Available virtual memory: <undefined>.\n");
#endif /* defined(OMR_OS_WINDOWS) || defined(OSX) */
			portTestEnv->log("Total swap memory: %llu bytes.\n", memInfo.totalSwap);
			portTestEnv->log("Swap memory free: %llu bytes.\n", memInfo.availSwap);
#if defined(OSX)
			portTestEnv->log("Cache memory: <undefined>.\n");
#else /* defined(OSX) */
			portTestEnv->log("Cache memory: %llu bytes.\n", memInfo.cached);
#endif /* defined(OSX) */
#if defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined (OSX)
			portTestEnv->log("Buffers memory: <undefined>.\n");
#else /* defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined (OSX) */
			portTestEnv->log("Buffers memory: %llu bytes.\n", memInfo.buffered);
#endif /* defined(AIXPPC) || defined(OMR_OS_WINDOWS) || defined (OSX) */
			portTestEnv->log("Timestamp: %llu.\n", memInfo.timestamp);
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "Invalid memory usage statistics retrieved.\n");
		}
	}

	portTestEnv->changeIndent(-1);
	reportTestExit(OMRPORTLIB, testName);
}

/**
 * @internal
 * Internal function: Counts up the number of processors that are online as per the
 * records delivered by the port library routine omrsysinfo_get_processor_info().
 *
 * @param[in] procInfo Pointer to J9ProcessorInfos filled in with processor info records.
 *
 * @return Number (count) of online processors.
 */
static int32_t
onlineProcessorCount(const struct J9ProcessorInfos *procInfo)
{
	int32_t cntr = 0;
	int32_t n_onln = 0;

	for (cntr = 1; cntr < procInfo->totalProcessorCount + 1; cntr++) {
		if (OMRPORT_PROCINFO_PROC_ONLINE == procInfo->procInfoArray[cntr].online) {
			n_onln++;
		}
	}
	return n_onln;
}

#define CPU_BURNER_BUFF_SIZE 10000
static uintptr_t
cpuBurner(OMRPortLibrary *portLibrary, const char *myText)
{
	/* burn up CPU */
	uintptr_t counter = 0;
	char buffer[CPU_BURNER_BUFF_SIZE];
	char *result = NULL;
	for (counter = 0; counter < CPU_BURNER_BUFF_SIZE; ++counter) {
		buffer[counter] = 0;
	}
	for (counter = 0; (strlen(buffer) + strlen(myText) + 1) < CPU_BURNER_BUFF_SIZE; ++counter) {
		result = strcat(buffer, myText);
		if (NULL != strstr(result, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab")) {
			return 0;
		}
	}
	return 1;
}
/**
 * Test for omrsysinfo_get_processor_info() port library API. Ensure that we are
 * able to obtain processor usage data as also, that it is consistent.
 */
TEST(PortSysinfoTest, sysinfo_testProcessorInfo)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_testProcessorInfo";
	intptr_t rc = 0;
	intptr_t cntr = 0;
	intptr_t n_onln = 0;
	uint64_t deltaTotalIdleTime = 0;
	uint64_t deltaTotalBusyTime = 0;

	J9ProcessorInfos prevInfo = {0};
	J9ProcessorInfos currInfo = {0};

	reportTestEntry(OMRPORTLIB, testName);
	portTestEnv->changeIndent(1);

	/* Take a snapshot of processor usage - at t1 (the first iteration). */
	rc = omrsysinfo_get_processor_info(&prevInfo);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_processor_info() failed.\n");

		/* Should not try freeing memory unless it was actually allocated! */
		if (OMRPORT_ERROR_SYSINFO_MEMORY_ALLOC_FAILED != rc) {
			omrsysinfo_destroy_processor_info(&prevInfo);
		}
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	/* Sleep for 3 seconds before re-sampling processor usage stats.
	 * This allows other processes and the operating system to use the CPU and drive up the
	 * user and kernel utilization.  Use the result of the call to cpuBurner to ensure it isn't optimized out.
	 * Used for validating that the total
	 * processor usage time is approximately in the range of the time elapsed.
	 * Note that this assumption sees deviations of upto 50% at times when the system is lightly loaded
	 * but under much system load, the relation indeed becomes accurate:
	 * 		(Busy-time(t2) - Busy-time(t1)) + (Idle-time(t2) - Idle-time(t1)) ~ T2 - T1.
	 */
	omrthread_sleep(3000 + cpuBurner(OMRPORTLIB, "a"));

	rc = omrsysinfo_get_processor_info(&currInfo);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_processor_info() failed.\n");
		omrsysinfo_destroy_processor_info(&prevInfo);

		/* Should not try freeing memory unless it was actually allocated! */
		if (OMRPORT_ERROR_SYSINFO_MEMORY_ALLOC_FAILED != rc) {
			omrsysinfo_destroy_processor_info(&currInfo);
		}
		reportTestExit(OMRPORTLIB, testName);
		return;
	}

	n_onln = onlineProcessorCount(&currInfo);
	if ((currInfo.totalProcessorCount > 0) &&
		(n_onln > 0) &&
		(currInfo.totalProcessorCount >= n_onln) &&
		(prevInfo.timestamp > 0) &&
		(prevInfo.timestamp < currInfo.timestamp)) {

		/* Print out some vital statistics of processor usage - use the current snapshot. */
		portTestEnv->log("Available processors: %d.\n", currInfo.totalProcessorCount);
		portTestEnv->log("Online processors: %d.\n", n_onln);
		portTestEnv->log("Timestamp: %llu.\n", currInfo.timestamp);
	}

	/* First, get the diffs in the Totals over iterations t1 to t2. */
	deltaTotalIdleTime = currInfo.procInfoArray[0].idleTime - prevInfo.procInfoArray[0].idleTime;
	deltaTotalBusyTime = currInfo.procInfoArray[0].busyTime - prevInfo.procInfoArray[0].busyTime;

	portTestEnv->log("CPUID: Total\n");
	portTestEnv->changeIndent(1);
	portTestEnv->log("User time:   %lld.\n", currInfo.procInfoArray[0].userTime);
	portTestEnv->log("System time: %lld.\n", currInfo.procInfoArray[0].systemTime);
	portTestEnv->log("Idle time:   %lld.\n", currInfo.procInfoArray[0].idleTime);
#if defined(OMR_OS_WINDOWS) || defined(OSX)
	portTestEnv->log("Wait time:   <undefined>.\n");
#else /* Non-windows/OSX platforms */
	portTestEnv->log("tWait time:   %lld.\n", currInfo.procInfoArray[0].waitTime);
#endif /* defined(OMR_OS_WINDOWS) || defined(OSX) */
	portTestEnv->log("Busy time:   %lld.\n", currInfo.procInfoArray[0].busyTime);
	portTestEnv->changeIndent(-1);

	/* Start iterating from 1 since 0^th entry represents Totals - already accounted for above. */
	for (cntr = 1; cntr < currInfo.totalProcessorCount + 1; cntr++) {

		/* Sanity check. Ensure that we successfully retrieved processor usage data in various
		 * modes, or else, signal an error. Add platform-specific exceptions (undefined parameter).
		 */
		if (OMRPORT_PROCINFO_PROC_ONLINE == currInfo.procInfoArray[cntr].online) {
			if ((OMRPORT_PROCINFO_NOT_AVAILABLE != currInfo.procInfoArray[cntr].userTime) &&
				(OMRPORT_PROCINFO_NOT_AVAILABLE != currInfo.procInfoArray[cntr].systemTime) &&
				(OMRPORT_PROCINFO_NOT_AVAILABLE != currInfo.procInfoArray[cntr].idleTime) &&
#if defined(OMR_OS_WINDOWS) || defined(OSX)
				/* Windows and OSX do not have the notion of Wait times. */
				(OMRPORT_PROCINFO_NOT_AVAILABLE == currInfo.procInfoArray[cntr].waitTime) &&
#else /* Non-windows/OSX platforms */
				(OMRPORT_PROCINFO_NOT_AVAILABLE != currInfo.procInfoArray[cntr].waitTime) &&
#endif /* defined(OMR_OS_WINDOWS) || defined(OSX) */
				(OMRPORT_PROCINFO_NOT_AVAILABLE != currInfo.procInfoArray[cntr].busyTime)) {

				/* Print out processor times in each mode for each CPU that is online. */
				portTestEnv->log("CPUID: %d\n",  currInfo.procInfoArray[cntr].proc_id);
				portTestEnv->changeIndent(1);
				portTestEnv->log("User time:   %lld.\n", currInfo.procInfoArray[cntr].userTime);
				portTestEnv->log("System time: %lld.\n", currInfo.procInfoArray[cntr].systemTime);
				portTestEnv->log("Idle time:   %lld.\n", currInfo.procInfoArray[cntr].idleTime);
#if defined(OMR_OS_WINDOWS) || defined(OSX)
				portTestEnv->log("Wait time:   <undefined>.\n");
#else /* Non-windows/OSX platforms */
				portTestEnv->log("Wait time:   %lld.\n", currInfo.procInfoArray[cntr].waitTime);
#endif /* defined(OMR_OS_WINDOWS) || defined(OSX) */
				portTestEnv->log("Busy time:   %lld.\n", currInfo.procInfoArray[cntr].busyTime);
				portTestEnv->changeIndent(-1);
			} else {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "Invalid processor usage statistics retrieved.\n");
				goto _cleanup;
			}
		}
	} /* end for(;;) */

	/* Check whether the processor times have increased since the last iteration. This ensures a
	 * monotonically increasing nature of processor times. Note: We cannot do this for each individual
	 * processor since there may be architectures such as AIXPPC where times don't change for certain
	 * processors that otherwise seem online (actually are in sleep mode); not even the Idle ticks.
	 */
	if (0 < (deltaTotalBusyTime + deltaTotalIdleTime)) {
		portTestEnv->log("Processor times in monotonically increasing order.\n");
	} else {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "Unexpected change in processor time deltas\ndeltaTotalBusyTime=%lld deltaTotalIdleTime=%lld\n",
						   deltaTotalBusyTime, deltaTotalIdleTime);
	}

_cleanup:
	omrsysinfo_destroy_processor_info(&prevInfo);
	omrsysinfo_destroy_processor_info(&currInfo);
	portTestEnv->changeIndent(-1);
	reportTestExit(OMRPORTLIB, testName);
}

/**
 * Test omrsysinfo_get_number_CPUs_by_type() port library API for the flag OMRPORT_CPU_ONLINE.
 * We obtain the online processor count using other (indirect) method - calling the other
 * port library API omrsysinfo_get_processor_info() and cross-check against this.
 */
TEST(PortSysinfoTest, sysinfo_testOnlineProcessorCount2)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_testOnlineProcessorCount2";
	intptr_t rc = 0;
	J9ProcessorInfos procInfo = {0};

	reportTestEntry(OMRPORTLIB, testName);

	/* Call omrsysinfo_get_processor_info() to retrieve a set of processor records from
	 * which we may then ascertain the number of processors online. This will help us
	 * cross-check against the API currently under test.
	 */
	rc = omrsysinfo_get_processor_info(&procInfo);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_processor_info() failed.\n");

		/* Should not try freeing memory unless it was actually allocated! */
		if (OMRPORT_ERROR_SYSINFO_MEMORY_ALLOC_FAILED != rc) {
			omrsysinfo_destroy_processor_info(&procInfo);
		}
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		/* Call the port library API omrsysinfo_get_number_online_CPUs() to check that the online
		 * processor count received is valid (that is, it does not fail) and that this indeed
		 * matches the online processor count as per the processor usage retrieval API.
		 */
		intptr_t n_cpus_online = omrsysinfo_get_number_CPUs_by_type(OMRPORT_CPU_ONLINE);
		if (-1 == n_cpus_online) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_number_online_CPUs() failed.\n");
			goto _cleanup;
		}

		if ((n_cpus_online > 0) &&
			(onlineProcessorCount(&procInfo) == n_cpus_online)) {
			portTestEnv->log("Number of online processors: %d\n",  n_cpus_online);
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "Invalid online processor count found.\n");
		}
	}

_cleanup:
	omrsysinfo_destroy_processor_info(&procInfo);
	reportTestExit(OMRPORTLIB, testName);
}

/**
 * Test omrsysinfo_get_number_CPUs_by_type() port library API for the flag OMRPORT_CPU_PHYSICAL.
 * Validate the number of available (configured) logical CPUs by cross-checking with what is
 * obtained from invoking the other port library API omrsysinfo_get_processor_info().
 */
TEST(PortSysinfoTest, sysinfo_testTotalProcessorCount)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_testTotalProcessorCount";
	intptr_t rc = 0;
	J9ProcessorInfos procInfo = {0};

	reportTestEntry(OMRPORTLIB, testName);

	/* Call omrsysinfo_get_processor_info() to retrieve a set of processor records from
	 * which we may then ascertain the total number of processors configured. We then
	 * cross-check this against what the API currently under test returns.
	 */
	rc = omrsysinfo_get_processor_info(&procInfo);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_processor_info() failed.\n");

		/* Should not try freeing memory unless it was actually allocated! */
		if (OMRPORT_ERROR_SYSINFO_MEMORY_ALLOC_FAILED != rc) {
			omrsysinfo_destroy_processor_info(&procInfo);
		}
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		/* Ensure first that the API doesn't fail. If not, check that we obtained the correct total
		 * processor count by checking against what omrsysinfo_get_processor_info() returned.
		 */
		intptr_t n_cpus_total = omrsysinfo_get_number_CPUs_by_type(OMRPORT_CPU_PHYSICAL);
		if (-1 == n_cpus_total) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_number_total_CPUs() failed.\n");
			goto _cleanup;
		}

		if ((procInfo.totalProcessorCount > 0) &&
			(procInfo.totalProcessorCount == n_cpus_total)) {
			portTestEnv->log("Total number of processors: %d\n",  n_cpus_total);
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "Invalid processor count retrieved.\n");
		}
	}

_cleanup:
	omrsysinfo_destroy_processor_info(&procInfo);
	reportTestExit(OMRPORTLIB, testName);
}

TEST(PortSysinfoTest, sysinfo_test_get_CPU_utilization)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_get_CPU_utilization";
	J9SysinfoCPUTime OldUtil;
	J9SysinfoCPUTime NewUtil;
	intptr_t portLibraryStatus = 0;

	reportTestEntry(OMRPORTLIB, testName);

	/*
	 * Call omrsysinfo_get_CPU_utilization() to retrieve the current CPU utilization.
	 * Sanity check the results.
	 */
	portLibraryStatus = omrsysinfo_get_CPU_utilization(&OldUtil);
	if (0 != portLibraryStatus) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_test_get_CPU_utilization() non-zero return code.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		portTestEnv->log("Old utilization timestamp=%llu cpuTime=%lld numberOfCpus=%d.\n",
					  OldUtil.timestamp, OldUtil.cpuTime, OldUtil.numberOfCpus);
		if ((OldUtil.cpuTime < 0) || (OldUtil.numberOfCpus <= 0)) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_test_get_CPU_utilization() invalid results.\n");
		}
	}
	/* Sleep for 3 seconds before re-sampling processor usage stats.
	 * This allows other processes and the operating system to use the CPU and drive up the
	 * user and kernel utilization.
	 * The call to cpuBurner probably won't be optimized out, but use the result to make absolutely sure that it isn't.
	 */
	omrthread_sleep(3000 + cpuBurner(OMRPORTLIB, "a"));
	portLibraryStatus = omrsysinfo_get_CPU_utilization(&NewUtil);
	if (0 != portLibraryStatus) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_test_get_CPU_utilization() non-zero return code.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	} else {
		portTestEnv->log("New utilization timestamp=%llu cpuTime=%lld numberOfCpus=%d.\n",
					  NewUtil.timestamp, NewUtil.cpuTime, NewUtil.numberOfCpus);
		portTestEnv->log("timestamp delta=%llu cpuTime delta=%lld\n",
					  NewUtil.timestamp - OldUtil.timestamp, NewUtil.cpuTime -  OldUtil.cpuTime);
		if ((NewUtil.cpuTime < OldUtil.cpuTime) || (NewUtil.numberOfCpus != OldUtil.numberOfCpus)) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_test_get_CPU_utilization() invalid results.\n");
		}
	}

	reportTestExit(OMRPORTLIB, testName);
}

#endif /* !defined(J9ZOS390) */


/*
 * Test omrsysinfo_get_tmp when the buffer size == 0
 * Expected result size of buffer required
 */
TEST(PortSysinfoTest, sysinfo_test_get_tmp1)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_tmp1";

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_tmp(NULL, 0, FALSE);
	if (rc <= 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
	} else {
		char *buffer = (char *)omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		portTestEnv->log("rc = %d\n", rc);

		rc = omrsysinfo_get_tmp(buffer, rc, FALSE);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
		}
		omrmem_free_memory(buffer);
	}
	reportTestExit(OMRPORTLIB, testName);
}

/*
 * Test omrsysinfo_get_tmp when the buffer size is smaller then required
 * Expected result size of buffer required
 */
TEST(PortSysinfoTest, sysinfo_test_get_tmp2)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_tmp2";
	char *buffer = NULL;
	const uintptr_t smallBufferSize = 4;

	reportTestEntry(OMRPORTLIB, testName);

	buffer = (char *)omrmem_allocate_memory(smallBufferSize, OMRMEM_CATEGORY_PORT_LIBRARY);
	rc = omrsysinfo_get_tmp(buffer, smallBufferSize, FALSE);

	if (0 >= rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
	} else {
		portTestEnv->log("rc = %d\n", rc);
		omrmem_free_memory(buffer);

		buffer = (char *)omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		rc = omrsysinfo_get_tmp(buffer, rc, FALSE);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
		}
	}
	omrmem_free_memory(buffer);
	reportTestExit(OMRPORTLIB, testName);
}

/*
 * Test omrsysinfo_get_tmp when TMPDIR is non-ascii (Unicode on windows, UTF-8 on other platforms)
 * Expected result: Successfully create non-ascii directory, and create a file there.
 */
TEST(PortSysinfoTest, sysinfo_test_get_tmp3)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_get_tmp3";
	char *buffer = NULL;
	intptr_t rc = 0;
	const char *data = "Hello World!";
	intptr_t tmpFile = 0;

#if defined(OMR_OS_WINDOWS)
	wchar_t *origEnv = NULL;
	const unsigned char utf8[]       = {0x63, 0x3A, 0x5C, 0xD0, 0xB6, 0xD0, 0xB0, 0xD0, 0xB1, 0xD0, 0xB0, 0x5C, 0x00};
	const unsigned char utf8_file[]  = {0x63, 0x3A, 0x5C, 0xD0, 0xB6, 0xD0, 0xB0, 0xD0, 0xB1, 0xD0, 0xB0, 0x5C, 0x74, 0x65, 0x73, 0x74, 0x2E, 0x74, 0x78, 0x74, 0x00};
	const wchar_t unicode[] = {0x0063, 0x003A, 0x005C, 0x0436, 0x0430, 0x0431, 0x0430, 0x005C, 0x00};

	reportTestEntry(OMRPORTLIB, testName);

	origEnv = (wchar_t *)omrmem_allocate_memory(EsMaxPath, OMRMEM_CATEGORY_PORT_LIBRARY);
	wcscpy(origEnv, _wgetenv(L"TMP"));
	rc = _wputenv_s(L"TMP", unicode);
#else /* defined(OMR_OS_WINDOWS) */
	char *origEnv = NULL;
	const char *utf8 = "/tmp/test/";
	const char *utf8_file = "/tmp/test/test.txt";
	char *origEnvRef = getenv("TMPDIR");
#if defined(J9ZOS390)
	char *envVarInEbcdic = a2e_string("TMPDIR");
	char *origEnvInEbcdic = NULL;
	char *utf8InEbcdic = a2e_string(utf8);
#endif /* defined(J9ZOS390) */

	reportTestEntry(OMRPORTLIB, testName);

	if (NULL != origEnvRef) {
		origEnv = (char *)omrmem_allocate_memory(strlen(origEnvRef) + 1, OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL != origEnv) {
			strcpy(origEnv, origEnvRef);
#if defined(J9ZOS390)
			origEnvInEbcdic = a2e_string(origEnv);
#endif /* defined(J9ZOS390) */
		}
	}

#if defined(J9ZOS390)
	rc = setenv(envVarInEbcdic, utf8InEbcdic, 1);
#else /* defined(J9ZOS390) */
	rc = setenv("TMPDIR", (const char *)utf8, 1);
#endif /* defined(J9ZOS390) */

#endif /* defined(OMR_OS_WINDOWS) */

	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error to update environment variable rc: %d\n", rc);
	}

	buffer = (char *)omrmem_allocate_memory(EsMaxPath, OMRMEM_CATEGORY_PORT_LIBRARY);
	rc = omrsysinfo_get_tmp(buffer, EsMaxPath, FALSE);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to get temp directory rc: %d\n", rc);
	} else {
		portTestEnv->log("TMP = %s\n", buffer);
	}

	rc = memcmp(utf8, buffer, strlen((const char *)utf8));
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "invalid directory rc: %d buffer %s, utf8 = %s\n", rc, buffer, utf8);
	}

	rc = omrfile_mkdir((const char *)utf8);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to create temp directory rc: %d\n", rc);
	}

	tmpFile = omrfile_open((const char *)utf8_file, EsOpenWrite | EsOpenCreateNew, 0666);
	if (-1 == tmpFile) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to create temp file rc: %d\n", rc);
	}

	rc = omrfile_write(tmpFile, data, strlen(data));
	if (0 > rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to write to temp file rc: %d\n", rc);
	}

	rc = omrfile_close(tmpFile);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to close temp file rc: %d\n", rc);
	}

	rc = omrfile_unlink((const char *)utf8_file);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to delete temp file rc: %d\n", rc);
	}

	rc = omrfile_unlinkdir((const char *)utf8);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to delete temp directory rc: %d\n", rc);
	}

	if (NULL != origEnv) {
#if defined(OMR_OS_WINDOWS)
		_wputenv_s(L"TMP", origEnv);
#elif defined(J9ZOS390) /* defined(OMR_OS_WINDOWS) */
		setenv(envVarInEbcdic, origEnvInEbcdic, 1);
#else /* defined(J9ZOS390) */
		setenv("TMPDIR", origEnv, 1);
#endif /* defined(OMR_OS_WINDOWS) */
		omrmem_free_memory(origEnv);
	} else {
#if defined(OMR_OS_WINDOWS)
		_wputenv_s(L"TMP", L"");
#elif !defined(J9ZOS390) /* defined(OMR_OS_WINDOWS) */
		unsetenv("TMPDIR");
#endif /* defined(OMR_OS_WINDOWS) */
	}

#if defined(J9ZOS390)
	if (NULL != envVarInEbcdic) {
		free(envVarInEbcdic);
	}
	if (NULL != origEnvInEbcdic) {
		free(origEnvInEbcdic);
	}
	if (NULL != utf8InEbcdic) {
		free(utf8InEbcdic);
	}
#endif /* defined(J9ZOS390) */

	if (NULL != buffer) {
		omrmem_free_memory(buffer);
	}
	reportTestExit(OMRPORTLIB, testName);
}

#if !defined(OMR_OS_WINDOWS)
/*
 * Test omrsysinfo_get_tmp when ignoreEnvVariable is FALSE/TRUE
 * Expected result size of buffer required
 */
TEST(PortSysinfoTest, sysinfo_test_get_tmp4)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_tmp4";
	const char *envVar = "TMPDIR";
	char *oldTmpDir = NULL;
	char *oldTmpDirValue = NULL;
	const char *modifiedTmpDir = "omrsysinfo_test_get_tmp4_dir";
#if defined(J9ZOS390)
	char *envVarInEbcdic = a2e_string(envVar);
	char *oldTmpDirValueInEbcdic = NULL;
	char *modifiedTmpDirInEbcdic = a2e_string(modifiedTmpDir);
#endif /* defined(J9ZOS390) */

	reportTestEntry(OMRPORTLIB, testName);

	oldTmpDir = getenv(envVar);
	if (NULL != oldTmpDir) {
		oldTmpDirValue = (char *)omrmem_allocate_memory(strlen(oldTmpDir) + 1, OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL != oldTmpDirValue) {
			strcpy(oldTmpDirValue, oldTmpDir);
#if defined(J9ZOS390)
			oldTmpDirValueInEbcdic = a2e_string(oldTmpDirValue);
#endif /* defined(J9ZOS390) */
		}
	}

#if defined(J9ZOS390)
	rc = setenv(envVarInEbcdic, modifiedTmpDirInEbcdic, 1);
#else /* defined(J9ZOS390) */
	rc = setenv(envVar, modifiedTmpDir, 1);
#endif /* defined(J9ZOS390) */
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error in updating environment variable TMPDIR, rc: %zd\n", rc);
	}

	rc = omrsysinfo_get_tmp(NULL, 0, FALSE);
	if (rc <= 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %zd\n", rc);
	} else {
		char *buffer = (char *) omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL == buffer) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "failed to allocate memory for buffer\n");
		} else {
			rc = omrsysinfo_get_tmp(buffer, rc, FALSE);
			if (0 != rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_tmp failed with rc: %zd\n", rc);
			} else {
				if (strcmp(modifiedTmpDir, buffer)) {
					outputErrorMessage(PORTTEST_ERROR_ARGS, "expected omrsysinfo_get_tmp to return same value as TMPDIR. TMPDIR: %s, returned: %s\n", modifiedTmpDir, buffer);
				}
			}
			omrmem_free_memory(buffer);
		}
	}

	rc = omrsysinfo_get_tmp(NULL, 0, TRUE);
	if (rc <= 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
	} else {
		char *buffer = (char *) omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL == buffer) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "failed to allocate memory for buffer\n");
		} else {
			rc = omrsysinfo_get_tmp(buffer, rc, TRUE);
			if (0 != rc) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_tmp failed with rc: %d\n", rc);
			} else {
				if (!strcmp(modifiedTmpDir, buffer)) {
					outputErrorMessage(PORTTEST_ERROR_ARGS, "expected omrsysinfo_get_tmp to ignore TMPDIR. TMPDIR: %s, returned: %s\n", modifiedTmpDir, buffer);
				}
			}
			omrmem_free_memory(buffer);
		}
	}

	/* restore TMPDIR */
#if defined(J9ZOS390)
	if (NULL != oldTmpDirValue) {
		setenv(envVarInEbcdic, oldTmpDirValueInEbcdic, 1);
		omrmem_free_memory(oldTmpDirValue);
	}
	if (NULL != envVarInEbcdic) {
		free(envVarInEbcdic);
	}
	if (NULL != oldTmpDirValueInEbcdic) {
		free(oldTmpDirValueInEbcdic);
	}
	if (NULL != modifiedTmpDirInEbcdic) {
		free(modifiedTmpDirInEbcdic);
	}
#else /* defined(J9ZOS390) */
	if (NULL != oldTmpDirValue) {
		setenv(envVar, oldTmpDirValue, 1);
		omrmem_free_memory(oldTmpDirValue);
	} else {
		unsetenv(envVar);
	}
#endif /* defined(J9ZOS390) */

	reportTestExit(OMRPORTLIB, testName);
}
#endif /* !defined(OMR_OS_WINDOWS) */

/*
 * Test omrsysinfo_get_cwd when the buffer size == 0, then allocate required ammount of bites and try again.
 * Expected result size of buffer required
 */
TEST(PortSysinfoTest, sysinfo_test_get_cwd1)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_cwd1";

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_cwd(NULL, 0);
	if (rc <= 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
	} else {
		char *buffer = (char *)omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		portTestEnv->log("rc = %d\n", rc);

		rc = omrsysinfo_get_cwd(buffer, rc);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
		}
		omrmem_free_memory(buffer);
	}
	reportTestExit(OMRPORTLIB, testName);
}
/*
 * Test omrsysinfo_get_cwd when the buffer size is smaller then required
 * Expected result size of buffer required
 */
TEST(PortSysinfoTest, sysinfo_test_get_cwd2)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_cwd2";
	char *buffer = NULL;
	const uintptr_t smallBufferSize = 4;

	reportTestEntry(OMRPORTLIB, testName);

	buffer = (char *)omrmem_allocate_memory(smallBufferSize, OMRMEM_CATEGORY_PORT_LIBRARY);
	rc = omrsysinfo_get_cwd(buffer, smallBufferSize);

	if (0 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
	} else {
		portTestEnv->log("rc = %d\n", rc);
		omrmem_free_memory(buffer);

		buffer = (char *)omrmem_allocate_memory(rc, OMRMEM_CATEGORY_PORT_LIBRARY);
		rc = omrsysinfo_get_tmp(buffer, rc, TRUE);
		if (0 != rc) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "unexpected return code rc: %d\n", rc);
		}
	}
	omrmem_free_memory(buffer);
	reportTestExit(OMRPORTLIB, testName);
}

/*
 * Test omrsysinfo_get_cwd in not ascii directory.
 * Expected result: Successfully create not ascii directory, change current current directory, verify that omrsysinfo_get_cwd returns valid value.
 */
TEST(PortSysinfoTest, sysinfo_test_get_cwd3)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	const char *testName = "omrsysinfo_test_get_cwd3";
	char *buffer = NULL;
	char *orig_cwd = NULL;

#if defined(OMR_OS_WINDOWS)
	/* c:\U+6211 U+7684 U+7236 U+4EB2 U+662F U+6536 U+68D2 U+5B50 U+7684 */
	const wchar_t unicode[] = {0x0063, 0x003A, 0x005C, 0x6211, 0x7684, 0x7236, 0x4EB2, 0x662F, 0x6536, 0x68D2, 0x5B50, 0x7684, 0x005C, 0x00};
	const unsigned char utf8[]       = {0x63, 0x3A, 0x5C, 0xE6, 0x88, 0x91, 0xE7, 0x9A, 0x84, 0xE7, 0x88, 0xB6, 0xE4, 0xBA, 0xB2, 0xE6, 0x98, 0xAF, 0xE6, 0x94, 0xB6, 0xE6, 0xA3, 0x92, 0xE5, 0xAD, 0x90, 0xE7, 0x9A, 0x84, 0x5C, 0x00};

	reportTestEntry(OMRPORTLIB, testName);

	orig_cwd = (char *)omrmem_allocate_memory(EsMaxPath, OMRMEM_CATEGORY_PORT_LIBRARY);
	omrsysinfo_get_cwd(orig_cwd, EsMaxPath);

	rc = omrfile_mkdir((const char *)utf8);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to create directory rc: %d\n", rc);
	}
	rc = _wchdir(unicode);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to change current directory rc: %d\n", rc);
	}
#else
#if defined(OSX)
	/* On OSX, /tmp is a symbolic link to /private/tmp. For the cwd to match after chdir, use /private/tmp. */
	const char *utf8 = "/private/tmp/omrsysinfo_test_get_cwd3/";
#else /* defined(OSX) */
	const char *utf8 = "/tmp/omrsysinfo_test_get_cwd3/";
#endif /* defined(OSX) */

	reportTestEntry(OMRPORTLIB, testName);

	orig_cwd = (char *)omrmem_allocate_memory(EsMaxPath, OMRMEM_CATEGORY_PORT_LIBRARY);
	omrsysinfo_get_cwd(orig_cwd, EsMaxPath);

	rc = omrfile_mkdir(utf8);
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to create directory rc: %d\n", rc);
	} else {
		portTestEnv->log("mkdir %s\n", utf8);
	}
#if defined(J9ZOS390)
	rc = atoe_chdir(utf8);
#else
	rc = chdir(utf8);
#endif
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "cd %s failed rc: %d\n", utf8, rc);
	} else {
		portTestEnv->log("cd %s\n", utf8);
	}
#endif /* defined(OMR_OS_WINDOWS) */

	buffer = (char *)omrmem_allocate_memory(EsMaxPath, OMRMEM_CATEGORY_PORT_LIBRARY);
	rc = omrsysinfo_get_cwd(buffer, EsMaxPath);

	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to get current working directory rc: %d\n", rc);
	} else {
		portTestEnv->log("CWD = %s\n", buffer);
	}

	rc = memcmp(utf8, buffer, strlen(buffer));
	if (0 != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "invalid directory rc: %d\n", rc);
	}

#if defined(OMR_OS_WINDOWS)
	_chdir(orig_cwd); /* we need to exit current directory before deleting it*/
#elif defined(J9ZOS390)
	atoe_chdir(orig_cwd);
#else /* defined(OMR_OS_WINDOWS) */
	rc = chdir(orig_cwd);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error: failed to change to directory %s, errno: %d\n", (const char *)orig_cwd, errno);
	}
#endif /* defined(OMR_OS_WINDOWS) */

	rc = omrfile_unlinkdir((const char *)utf8);
	if (-1 == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "error failed to delete directory %s rc: %d\n", (const char *)utf8, rc);
	}

	omrmem_free_memory(orig_cwd);
	omrmem_free_memory(buffer);
	reportTestExit(OMRPORTLIB, testName);
}

#if !defined(OMR_OS_WINDOWS)
/**
 * Test omrsysinfo_get_groups.
 */
TEST(PortSysinfoTest, sysinfo_test_get_groups)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	intptr_t rc = 0;
	intptr_t i;
	const char *testName = "omrsysinfo_test_get_groups";
	uint32_t *gidList = NULL;

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_get_groups(&gidList, OMRMEM_CATEGORY_PORT_LIBRARY);
	if (-1 != rc) {
		struct group *grent = NULL;

		portTestEnv->log("group list size=%zi\n", rc);

		for (i = 0; i < rc; i++) {
			int error = 0;
			/* Set errno to 0 before calling getgrgid() to correctly handle NULL return values */
			errno = 0;
			/* No portlib api to get group name for a give group id */
			grent = getgrgid(gidList[i]);
			error = errno;
			portTestEnv->changeIndent(1);
			portTestEnv->log("gid=%u", gidList[i]);
			portTestEnv->changeIndent(-1);
			if (NULL == grent) {
				if (0 == error) {
					portTestEnv->changeIndent(1);
					portTestEnv->log("this group id is not found in group database (not an error as per getgrgid documentation)\n");
					portTestEnv->changeIndent(-1);
				} else {
					outputErrorMessage(PORTTEST_ERROR_ARGS, "\ngetgrgid() returned NULL with errno=%d for group id=%u\n", error, gidList[i]);
					break;
				}
			} else {
				char *group = grent->gr_name;
				if (NULL != group) {
					portTestEnv->changeIndent(1);
					portTestEnv->log("group name=%s\n", group);
					portTestEnv->changeIndent(-1);
				} else {
					outputErrorMessage(PORTTEST_ERROR_ARGS, "\ngetgrgid() returned NULL as the group name for group id=%u\n", gidList[i]);
					break;
				}
			}
		}
	} else {
		char *lastErrorMessage = (char *)omrerror_last_error_message();
		int32_t lastErrorNumber = omrerror_last_error_number();

		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_groups returned %zi\n"
						   "\tlastErrorNumber=%d, lastErrorMessage=%s\n", rc, lastErrorNumber, lastErrorMessage);
	}
	reportTestExit(OMRPORTLIB, testName);
}

#if defined(LINUX) || defined(AIXPPC)
/**
 * Test omrsysinfo_test_get_open_file_count.
 * Available only on Linux and AIX.
 */
TEST(PortSysinfoTest, sysinfo_test_get_open_file_count)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_get_open_file_count";
	int32_t ret = 0;
	uint64_t openCount = 0;
	uint64_t curLimit = 0;
	uint32_t rc = 0;

	reportTestEntry(OMRPORTLIB, testName);
	/* Get the number of files opened till this point. */
	ret = omrsysinfo_get_open_file_count(&openCount);
	if (ret < 0) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_get_open_file_count() failed.\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}
	portTestEnv->log("omrsysinfo_get_open_file_count(): Files opened by this process=%lld\n", 
		openCount);

	/* Now, get the current (soft) limit on the resource "nofiles".  We check the current
	 * number of files opened, against this.
	 */
	rc = omrsysinfo_get_limit(OMRPORT_RESOURCE_FILE_DESCRIPTORS, &curLimit);
	if (OMRPORT_LIMIT_UNLIMITED == rc) {
		/* Not really an error, just a sentinel.  Comparisons can still work! */
		if (RLIM_INFINITY == curLimit) {
			portTestEnv->log(
				"omrsysinfo_get_limit(nofiles): soft limit=RLIM_INFINITY (unlimited).\n");
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, 
				"omrsysinfo_get_limit(nofiles): soft limit (unlimited), bad maximum reported=%lld.\n",
				((int64_t) curLimit));
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else if (OMRPORT_LIMIT_LIMITED == rc) {
		/* Check that the limits received are sane, before comparing against files opened. */
		if ((((int64_t) curLimit) > 0) && (((int64_t) curLimit) <= INT64_MAX)) {
			portTestEnv->log("omrsysinfo_get_limit(nofiles): soft limit=%lld.\n", 
				((int64_t) curLimit));
		} else {
			outputErrorMessage(PORTTEST_ERROR_ARGS, 
				"omrsysinfo_get_limit(nofiles) failed: bad limits received!\n");
			reportTestExit(OMRPORTLIB, testName);
			return;
		}
	} else { /* The port library failed! */
		outputErrorMessage(PORTTEST_ERROR_ARGS, 
			"omrsysinfo_get_limit(nofiles): failed with error code=%d.\n",
			omrerror_last_error_number());
		reportTestExit(OMRPORTLIB, testName);
		return;
	}
	/* Sanity check: are more files reported as opened than the limit? */
	if (((int64_t) openCount) > ((int64_t) curLimit)) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, 
			"omrsysinfo_get_open_file_count() failed: reports more files opened than allowed!\n");
		reportTestExit(OMRPORTLIB, testName);
		return;
	}
	reportTestExit(OMRPORTLIB, testName);
	return;
}
#endif /* defined(LINUX) || defined(AIXPPC) */
#endif /* !defined(OMR_OS_WINDOWS) */

/**
 * Test omrsysinfo_test_get_os_description.
 */
TEST(PortSysinfoTest, sysinfo_test_get_os_description)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_get_os_description";

	intptr_t rc = 0;
	reportTestEntry(OMRPORTLIB, testName);

	struct OMROSDesc desc;
	rc =  omrsysinfo_get_os_description(&desc);

	for (int i = 0; i < OMRPORT_SYSINFO_OS_FEATURES_SIZE * 32; i++) {
		BOOLEAN feature = omrsysinfo_os_has_feature(&desc, i);
		portTestEnv->log(LEVEL_VERBOSE, "omrsysinfo_test_get_os_description() feature %d: value=%d, rc=%zi\n", i, feature, rc);
	}

	reportTestExit(OMRPORTLIB, testName);
	return;
}

/**
 * Test omrsysinfo_test_os_kernel_info.
 */
TEST(PortSysinfoTest, sysinfo_test_os_kernel_info)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_test_os_kernel_info";

	BOOLEAN rc = 0;
	struct OMROSKernelInfo kernelInfo = {0};

	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_os_kernel_info(&kernelInfo);

#if defined(LINUX)
	/* Throw an error if failure happens on Linux */
	if (FALSE == rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS,
				"omrsysinfo_os_kernel_info failed on Linux: kernelVersion=%zu, majorRevision=%zu, minorRevision=%zu\n", kernelInfo.kernelVersion, kernelInfo.majorRevision, kernelInfo.minorRevision);
		goto exit;
	} else {
		/* Throw an error if kernel version is 0 */
		if (0 == kernelInfo.kernelVersion) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_os_kernel_info failed on Linux - kernel version can't be 0 (unsupported)\n");
			goto exit;
		}
	}
#else /* defined(LINUX) */
	if (TRUE == rc) {
		/* Throw an error if omrsysinfo_os_kernel_info passes on an unsupported platform */
		outputErrorMessage(PORTTEST_ERROR_ARGS,	"omrsysinfo_os_kernel_info passed on an unsupported platform\n");
		goto exit;
	}
#endif /* defined(LINUX) */

exit:
	reportTestExit(OMRPORTLIB, testName);
	return;
}

/**
 * Test omrsysinfo_cgroup_get_memlimit.
 */
TEST(PortSysinfoTest, sysinfo_cgroup_get_memlimit)
{
	OMRPORT_ACCESS_FROM_OMRPORT(portTestEnv->getPortLibrary());
	const char *testName = "omrsysinfo_cgroup_get_memlimit";
	uint64_t cgroupMemLimit = 0;
	int32_t rc = 0;
	uint64_t enabledSubsystems = 0;
	
	reportTestEntry(OMRPORTLIB, testName);

	rc = omrsysinfo_cgroup_get_memlimit(&cgroupMemLimit);

#if !defined(LINUX)
	if (OMRPORT_ERROR_SYSINFO_CGROUP_UNSUPPORTED_PLATFORM != rc) {
		outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_cgroup_get_memlimit returned %d, expected %d on platform that does not support cgroups\n", rc, OMRPORT_ERROR_SYSINFO_CGROUP_UNSUPPORTED_PLATFORM);
	}
#endif

	/* Compare sysinfo_get_physical_memory and sysinfo_cgroup_get_memlimit after enabling memory subsystem */
	enabledSubsystems = omrsysinfo_cgroup_enable_subsystems(OMR_CGROUP_SUBSYSTEM_MEMORY);
	if (OMR_ARE_ALL_BITS_SET(enabledSubsystems, OMR_CGROUP_SUBSYSTEM_MEMORY)) {
		rc = omrsysinfo_cgroup_get_memlimit(&cgroupMemLimit);
		if ((0 != rc) && (OMRPORT_ERROR_SYSINFO_CGROUP_MEMLIMIT_NOT_SET != rc)) {
			outputErrorMessage(PORTTEST_ERROR_ARGS, "omrsysinfo_cgroup_get_memlimit failed with error code %d\n", rc);
		} else if (0 == rc) {
			uint64_t physicalMemLimit = 0;

			physicalMemLimit = omrsysinfo_get_physical_memory();
			if (cgroupMemLimit != physicalMemLimit) {
				outputErrorMessage(PORTTEST_ERROR_ARGS, "Expected omrsysinfo_cgroup_get_memlimit and omrsysinfo_get_physical_memory to return same value, but omrsysinfo_cgroup_get_memlimit returned %ld and omrsysinfo_get_physical_memory returned %ld\n", cgroupMemLimit, physicalMemLimit);
			}
		}
	}

	reportTestExit(OMRPORTLIB, testName);
	return;
}

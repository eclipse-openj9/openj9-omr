/*******************************************************************************
 * Copyright (c) 1991, 2015 IBM Corp. and others
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

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "omr.h"
#include "omrutil.h"
#include "omrport.h"

/* Answer non-zero on success
 */
uintptr_t
try_scan(char **scan_start, const char *search_string)
{
	char *scan_string = *scan_start;
	size_t search_length = strlen(search_string);

	if (strlen(scan_string) < search_length) {
		return 0;
	}

	if (0 == j9_cmdla_strnicmp(scan_string, search_string, search_length)) {
		*scan_start = &scan_string[search_length];
		return 1;
	}

	return 0;
}

/* Returns the trimmed input string, removing leading and trailing whitespace.
 * Returned string is malloc-ed and must be freed.
 *
 * @param portLibrary The portLibrary to use
 * @param input The buffer to trim.
 */
char * omr_trim(OMRPortLibrary *portLibrary, char *input)
{
	BOOLEAN whitespace = TRUE;
	uintptr_t index = 0;
	char * buf = (char *)portLibrary->mem_allocate_memory(portLibrary,
							      strlen(input)+1,
							      OMR_GET_CALLSITE(),
							      OMRMEM_CATEGORY_VM);

	while (whitespace == TRUE) {
		char c = input[0];
		switch (c) {
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			input++;
			break;

		default:
			whitespace = FALSE;
		}
	}

	strcpy(buf, input);
	index = strlen(buf) - 1;
	while (whitespace == TRUE) {
		char c = buf[index];
		switch (c) {
		case '\t':
		case '\n':
		case '\r':
		case ' ':
			buf[index] = 0;
			index--;
			break;

		default:
			whitespace = TRUE;
		}
	}

	return buf;
}

/* Copy the substring starting from the beginning of a string up to
 * (but not including) the first occurrence of the specified delimiter
 * character or end of line. The copied substring is malloc-ed and
 * must be freed by the sender.
 *
 * The address of the source string is given by a pointer to a string,
 * scanStart. If the substring copy is successfully malloc-ed,
 * scanStart is set to the start of the uncopied part of the source
 * string that is not the null character.
 *
 * @param portLibrary The portLibrary to use
 * @param scanStart A pointer to a string containing the substring
 *                  to be copied
 * @param delimiter The delimiting character that stops the scan
 */
char *omr_scan_to_delim(OMRPortLibrary *portLibrary, char **scanStart, char delimiter)
{
	char *scanString = *scanStart;
	char *subString;
	uintptr_t i = 0;

	while (scanString[i] && (scanString[i] != delimiter)) {
		i++;
	}

	subString = portLibrary->mem_allocate_memory(portLibrary,
                                                    i+1 /* for null */,
						     						OMR_GET_CALLSITE(),
						     						OMRMEM_CATEGORY_VM);

	if (subString) {
		memcpy(subString, scanString, i);
		subString[i] = 0;
		*scanStart = (scanString[i] ? &scanString[i+1] : &scanString[i]);
	}
	return subString;
}

/* Print an error message indicating that an option was not recognized.
 *
 * @param portLibrary The portLibrary to use
 * @param module The module the error emanates from
 * @param scanStart The string containing the error message
 */
void omr_scan_failed(OMRPortLibrary * portLibrary, const char* module, const char *scanStart)
{
	portLibrary->tty_printf(portLibrary,
                                "<%s: unrecognized option --> '%s'>\n",
                                module,
                                scanStart);
}

/* Scan a double from a string and store it to a double.
 * The scanStart pointer is 
 * Returns an error on overflow or malformed representation.
 *
 * @param scanStart The string to be scanned
 * @param result The pointer to the double, where the
 *               scanned double is to be stored
 */
uintptr_t omr_scan_double(char **scanStart, double *result)
{
	char *endPtr = NULL;

	*result = strtod(*scanStart, &endPtr);
	if (ERANGE == errno) {
		if ((HUGE_VAL == *result) || (-HUGE_VAL == *result)) {
			/* overflow */
			return OPTION_OVERFLOW;
		} else {
			/* underflow - value is so small that it cannot be represented as double.
			 * treat it as zero.
			*/
			*result = 0.0;
			return OPTION_OK;
		}
	} else if ((0.0 == *result) && (endPtr == *scanStart)) {
		/* no conversion */
		return OPTION_MALFORMED;
	}
	*scanStart = endPtr;
	return OPTION_OK;
}

/* Scan the next u32 from scanStart. Store the result in
 * *result and increment the scanStart pointer by the number of
 * characters scanned.
 *
 * Returns 0 on success and 1 if no digits were scanned from the
 * string.
 *
 * @param scanStart The string to be scanned
 * @param result A pointer to the uint64_t where the result
 *               is stored.
 */
uintptr_t omr_scan_udata(char **scanStart, uintptr_t* result)
{
	/* supporting 0x prefix might be nice (or octal) */
	uintptr_t total = 0, rc = 1;
	char *c = *scanStart;

	/* isdigit isn't properly supported everywhere */
	while ( *c >= '0' && *c <= '9' ) {
		uintptr_t digitValue = *c - '0';

		if (total > ((uintptr_t)-1) / 10 ) {
			return 2;
		}

		total *= 10;

		if ( total > ((uintptr_t)-1) - digitValue ) {
			return 2;
		}

		total += digitValue;

		rc = 0;	/* we found at least one digit */

		c++;
	}

	*scanStart = c;
	*result = total;
	return rc;
}

/* Scan the next u64 from scanStart. Store the result in
 * *result and increment the scanStart pointer by the number of
 * characters scanned.
 *
 * Returns 0 on success and 1 if no digits were scanned from the
 * string.
 *
 * @param scanStart The string to be scanned
 * @param result A pointer to the uint64_t where the result
 *               is stored.
 */
uintptr_t
omr_scan_u64(char **scanStart, uint64_t* result)
{
	/* supporting 0x prefix might be nice (or octal) */
	uint64_t total = 0;
	uintptr_t rc = 1;
	char *c = *scanStart;

	/* isdigit isn't properly supported everywhere */
	while ( *c >= '0' && *c <= '9' ) {
		uintptr_t digitValue = *c - '0';

		if (total > ((uint64_t)-1) / 10 ) {
			return 2;
		}

		total *= 10;

		if ( total > ((uint64_t)-1) - digitValue ) {
			return 2;
		}

		total += digitValue;

		rc = 0;	/* we found at least one digit */

		c++;
	}

	*scanStart = c;
	*result = total;
	return rc;
}

/* Scan the next u32 from scanStart. Store the result in
 * *result and increment the scanStart pointer by the number of
 * characters scanned.
 *
 * Returns 0 on success and 1 if no digits were scanned from the
 * string.
 *
 * @param scanStart The string to be scanned
 * @param result A pointer to the uint64_t where the result
 *               is stored.
 */
uintptr_t
omr_scan_u32(char **scanStart, uint32_t* result)
{
	/* supporting 0x prefix might be nice (or octal) */
	uint32_t total = 0;
	uintptr_t rc = 1;
	char *c = *scanStart;

	/* isdigit isn't properly supported everywhere */
	while ( *c >= '0' && *c <= '9' ) {
		uint32_t digitValue = *c - '0';

		if (total > ((uint32_t)-1) / 10 ) {
			return 2;
		}

		total *= 10;

		if ( total > ((uint32_t)-1) - digitValue ) {
			return 2;
		}

		total += digitValue;

		rc = 0;	/* we found at least one digit */

		c++;
	}

	*scanStart = c;
	*result = total;
	return rc;
}

/* Scan the next idata from scanStart. Because idata is signed, the
 * sign characters '+' and '-' are skipped if they precede the digits
 * in the string. Store the result in *result and increment the
 * scanStart pointer by the number of characters scanned. 
 *
 * Returns 0 if successful, 1 if the string contains no digits,
 * and 2 if the result is negative, and its binary representation 
 * is not equal to MIN_UDATA.
 *
 * @param scanStart The string to be scanned
 * @param result A pointer to the uint64_t where the result
 *               is stored.
 */
uintptr_t omr_scan_idata(char **scanStart, intptr_t *result)
{
	uintptr_t rc;
	char* newScanStart = *scanStart;

	char c = *newScanStart;

	if (c == '+' || c == '-') {
		newScanStart++;
	}

	rc = omr_scan_udata(&newScanStart, (uintptr_t *)result);

	if (rc == 0) {
		if (*result < 0) {
			if ( ( (uintptr_t)*result  == (uintptr_t)1 << (sizeof(uintptr_t) * 8 - 1) ) && ( c == '-' ) ) {
				/* this is MIN_UDATA */
				rc = 0;
			} else {
				rc = 2;
			}
		} else if (c == '-') {
			*result = -(*result);
		}
	}

	if (rc == 0) {
		*scanStart = (char *)newScanStart;
	}

	return rc;
}

/* Scan the next hexadecimal value from scanStart according
 * to scan_hex_caseflag (described below). The TRUE value
 * in the second argument indicates that the upper case hex 
 * digits A - F are to be scanned.
 *
 * @param scanStart The string to be scanned
 * @param result A pointer to the uintptr_t where the result is 
 *               stored
 */
uintptr_t omr_scan_hex(char **scanStart, uintptr_t* result)
{
	return	omr_scan_hex_caseflag(scanStart, TRUE, result);
}

/* Scan the next hexadecimal value from scanStart. Uppercase A-F are
 * recognized only if uppercaseAllowed is ture. Store the result in
 * *result and increment the scanStart pointer by the number of
 * characters scanned.
 *
 * Returns 0 on success and 1 if no digits were scanned from the
 * string.
 *
 * @param scanStart The string to be scanned
 * @param uppercaseAllowed Whether the scan recognizes upcase 
 *        hex digits.
 * @param result A pointer to the uintptr_t where the result
 *               is stored.
 */
uintptr_t omr_scan_hex_caseflag(char **scanStart, BOOLEAN uppercaseAllowed, uintptr_t* result)
{
	uintptr_t total = 0, delta = 0, rc = 1;
	char *hex = *scanStart, x;

	try_scan(&hex, "0x");

	while ( (x = *hex) != '\0' ) {
		/*
		 * Decode hex digit
		 */
		if (x >= '0' && x <= '9') {
			delta = (x - '0');
		} else if (x >= 'a' && x <= 'f') {
			delta = 10 + (x - 'a');
		} else if (x >= 'A' && x <= 'F' && uppercaseAllowed) {
			delta = 10 + (x - 'A');
		} else {
			break;
		}

		total = (total << 4) + delta;

		hex++; rc = 0;
	}

	*scanStart = hex;
	*result = total;

	return rc;
}

/**
 * Scan the hexadecimal uint64_t number and store the result in *result
 * @param[in] scanStart The string to be scanned
 * @param[in] uppercaseFlag Whether upper case letter is allowed
 * @param[out] result The result
 * @return the number of bits used to store the hexadecimal number or 0 on failure.
 */
uintptr_t
omr_scan_hex_u64(char **scanStart, uint64_t* result)
{
	return	omr_scan_hex_caseflag_u64(scanStart, TRUE, result);
}

/**
 * Scan the hexadecimal uint64_t number and store the result in *result
 * @param[in] scanStart The string to be scanned
 * @param[in] uppercaseFlag Whether uppercase letter is allowed
 * @param[out] result The result
 * @return the number of bits used to store the hexadecimal number or 0 on failure.
 */
uintptr_t
omr_scan_hex_caseflag_u64(char **scanStart, BOOLEAN uppercaseAllowed, uint64_t* result)
{
	uint64_t total = 0;
	uint64_t delta = 0;
	char *hex = *scanStart;
	uintptr_t bits = 0;

	try_scan(&hex, "0x");

	while (('\0' != *hex)
			&& (bits <= 60)
	) {
		/*
		 * Decode hex digit
		 */
		char x = *hex;
		if (x >= '0' && x <= '9') {
			delta = (x - '0');
		} else if (x >= 'a' && x <= 'f') {
			delta = 10 + (x - 'a');
		} else if (x >= 'A' && x <= 'F' && uppercaseAllowed) {
			delta = 10 + (x - 'A');
		} else {
			break;
		}

		total = (total << 4) + delta;
		bits += 4;
		hex++;
	}

	*scanStart = hex;
	*result = total;

	return bits;
}

/*
 * Print an error message indicating that an option was not recognized.
 *
 * @param portLibrary The portLibrary to be used
 * @param module The module the error emanates from
 * @param scanStart The string to be scanned
 */
void omr_scan_failed_incompatible(OMRPortLibrary * portLibrary, char* module, char *scanStart)
{
	portLibrary->tty_printf(portLibrary, "<%s: incompatible option --> '%s'>\n", module, scanStart);
}

/*
 * Print an error message indicating that an option was not recognized.
 *
 * @param portLibrary The portLibrary to be used
 * @param module The module the error emanates from
 * @param scanStart The string to be scanned
 */
void omr_scan_failed_unsupported(OMRPortLibrary * portLibrary, char* module, char *scanStart)
{
	portLibrary->tty_printf(portLibrary, "<%s: system configuration does not support option --> '%s'>\n", module, scanStart);
}

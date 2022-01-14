/*******************************************************************************
 * Copyright (c) 1991, 2021 IBM Corp. and others
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

#if defined(OMR_OS_WINDOWS)
#include <windows.h>
#endif /* defined(OMR_OS_WINDOWS) */
#include <time.h>

#include "omrportasserts.h"

#include <stdarg.h>
#include <string.h>
#if defined(OMR_OS_WINDOWS)
#include <malloc.h>
#elif defined(LINUX) || defined(AIXPPC) || defined(OSX)
#include <alloca.h>
#elif defined(J9ZOS390)
#include <stdlib.h>
#endif /* defined(OMR_OS_WINDOWS) */
#include <errno.h>

/*
#define J9STR_DEBUG
*/

#if defined(LINUX) || defined(AIXPPC) || defined(J9ZOS390) || defined(OSX)
#include <iconv.h>
typedef iconv_t charconvState_t;
#define J9STR_USE_ICONV
/* need to get the EBCDIC version of nl_langinfo */
#define J9_USE_ORIG_EBCDIC_LANGINFO 1

#include "omriconvhelpers.h"
#else /* defined(LINUX) || defined(AIXPPC) || defined(J9ZOS390) || defined(OSX) */
typedef void *charconvState_t; /*dummy type */
#endif /* defined(LINUX) || defined(AIXPPC) || defined(J9ZOS390) || defined(OSX) */

/* for sprintf, which is used for printing floats */
#include <stdio.h>

#include "omrutil.h"
#include "omrcomp.h"
#include "omrport.h"
#include "omrgetasid.h"
#include "omrgetjobid.h"
#include "omrgetjobname.h"
#include "omrgetsysname.h"
#include "omrstdarg.h"
#include "hashtable_api.h"

#define J9FTYPE_U64 1
#define J9FTYPE_U32 2
#define J9FTYPE_DBL 3
#define J9FTYPE_PTR 4
#define J9FTYPE_IMMEDIATE 5

#define J9FFLAG_DASH 0x01
#define J9FFLAG_HASH 0x02
#define J9FFLAG_ZERO 0x04
#define J9FFLAG_SPACE 0x08
#define J9FFLAG_PLUS 0x10

#define J9FSPEC_LL 0x20
#define J9FSPEC_L 0x40

#define J9F_NO_VALUE ((uint64_t)-1)
extern const char *utf8;
extern const char *utf16;
extern const char *ebcdic;

typedef union J9FormatValue {
	uint64_t u64;
	double dbl;
	void *ptr;
} J9FormatValue;

typedef struct J9FormatSpecifier {
	uint8_t tag;
	uint8_t index;
	uint8_t widthIndex;
	uint8_t precisionIndex;
	const char *type;
} J9FormatSpecifier;

#define J9F_MAX_SPECS 16
#define J9F_MAX_ARGS (J9F_MAX_SPECS * 3)

typedef struct J9FormatData {
	const char *formatString;
	J9FormatValue value[J9F_MAX_ARGS];
	uint8_t valueType[J9F_MAX_ARGS];
	J9FormatSpecifier spec[J9F_MAX_SPECS];
	uint8_t valueCount;
	uint8_t immediateCount;
	uint8_t specCount;
} J9FormatData;

typedef struct J9TimeInfo {
	uint32_t second;
	uint32_t minute;
	uint32_t hour;
	uint32_t day;
	uint32_t weekday;
	uint32_t month;
	uint32_t year;
} J9TimeInfo;

typedef struct J9TokenEntry {
	char *key;
	char *value;
	size_t keyLen; /* The length of the key string (excluding the \0) */
	uintptr_t valueLen; /* The length of the value string (excluding the \0) */
	uintptr_t memLen; /* The length of the allocated memory */
} J9TokenEntry;

/* The hash table does support growing beyond the specified size */
#define J9TOKEN_TABLE_INIT_SIZE 32
#define J9TOKEN_TABLE_ALIGNMENT sizeof(uintptr_t)
#define J9TOKEN_MAX_KEY_LEN 32

static const char digits_dec[] = "0123456789";
static const char digits_hex_lower[] = "0123456789abcdef";
static const char digits_hex_upper[] = "0123456789ABCDEF";

extern uint32_t encodeUTF8Char(uintptr_t unicode, uint8_t *result);

static const char *parseTagChar(const char *format, J9FormatData *result);
static void readValues(struct OMRPortLibrary *portLibrary, J9FormatData *result, va_list args);
static int parseFormatString(struct OMRPortLibrary *portLibrary, J9FormatData *result);
static uintptr_t writeDoubleToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, double value, uint8_t type, uint8_t tag);
static const char *parseModifier(const char *format, J9FormatData *result);
static const char *parseType(const char *format, J9FormatData *result);
static const char *parseWidth(const char *format, J9FormatData *result);
static uintptr_t writeFormattedString(struct OMRPortLibrary *portLibrary, J9FormatData *data, char *result, uintptr_t length);
static uintptr_t writeSpec(J9FormatData *data, J9FormatSpecifier *spec, char *result, uintptr_t length);
static const char *parseIndex(const char *format, uint8_t *result);
static uintptr_t writeStringToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, const char *value, uint8_t tag);
static const char *parsePrecision(const char *format, J9FormatData *result);
static uintptr_t writeIntToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, uint64_t value, uint8_t tag, int isSigned, const char *digits);
static uintptr_t writeUnicodeStringToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, const uint16_t *value, uint8_t tag);
static void convertUTCMillisToJ9Time(int64_t millisUTC, struct J9TimeInfo *tm, uint32_t flags);
static void setJ9TimeToEpoch(struct J9TimeInfo *tm);
static uint32_t omrstr_subst_time(struct OMRPortLibrary *portLibrary, char *buf, uint32_t bufLen, const char *format, int64_t timeMillis, uint32_t flags);
static intptr_t omrstr_set_token_from_buf(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens, const char *key, char *tokenBuf, uint32_t tokenLen);
static int32_t convertPlatformToMutf8(struct OMRPortLibrary *portLibrary, uint32_t codePage, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertMutf8ToPlatform(struct OMRPortLibrary *portLibrary, uint32_t codePage, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertWideToMutf8(const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertUtf8ToMutf8(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertPlatformToUtf8(struct OMRPortLibrary *portLibrary, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertMutf8ToWide(const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertPlatformToWide(struct OMRPortLibrary *portLibrary, charconvState_t encodingState, uint32_t codePage, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertWideToPlatform(struct OMRPortLibrary *portLibrary, charconvState_t encodingState, uint32_t codePage, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertLatin1ToMutf8(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);
static int32_t convertMutf8ToLatin1(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize);

/**
 * Write characters to a string as specified by format.
 *
 * @param[in] portLibrary The port library.
 * @param[in, out] buf The string buffer to be written.
 * @param[in] bufLen The size of the string buffer to be written.
 * @param[in] format The format of the string.
 * @param[in] ... Arguments for the format string.
 *
 * @return The number of characters printed not including the NUL terminator.
 *
 * @note When buf is NULL, the size of the buffer required to print to the string, including
 * the NUL terminator is returned.
 *
 * @internal @note Supported, portable format specifiers are described in the document entitled "PortLibrary printf"
 * in the "Inside J9" Lotus Notes database.
 */
uintptr_t
omrstr_printf(struct OMRPortLibrary *portLibrary, char *buf, uintptr_t bufLen, const char *format, ...)
{
	uintptr_t rc = 0;
	va_list args;
	va_start(args, format);
	rc = portLibrary->str_vprintf(portLibrary, buf, bufLen, format, args);
	va_end(args);
	return rc;
}

/**
 * This function converts strings between either the environment's platform encoding or Unicode (wide character),
 * and modified UTF-8.
 * Modified UTF-8 is described in the JVMTI specification.
 * Conversion between arbitrary character encodings is not supported.
 *
 * @param[in] portLibrary   The port library
 * @param[in] fromCode      Input string encoding.  Only the following encodings are allowed:
 *                              J9STR_CODE_MUTF8 (Modified UTF-8)
 *                              J9STR_CODE_WIDE (UTF-16)
 *                              J9STR_CODE_LATIN1
 *                              J9STR_CODE_PLATFORM_RAW (encoding used by the operating system)
 *                              J9STR_CODE_PLATFORM_OMR_INTERNAL (encoding used by certain operating system calls)
 *                              J9STR_CODE_WINTHREADACP, J9STR_CODE_WINDEFAULTACP (thread and default ANSI code page, Windows only)
 * @param[in] toCode        Output string encoding.  Only the encodings listed above are allowed.
 * @param[in] inBuffer      Input text to be converted.  May contain embedded null characters.
 * @param[in] inBufferSize  input string size in bytes, not including the terminating null.
 * @param[in] outBuffer     user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the required output buffer size)
 *
 * @return:
 * - positive value on success.
 *  	- the number of bytes required to hold the converted text if outBufferSize is 0.
 *		- the number of bytes written to the buffer indicated by outBuffer.
 * - negative value on failure:
 * 		- OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL, if the size of outBuffer is not large enough to hold the converted text.
 * 		- OMRPORT_ERROR_STRING_ICONV_OPEN_FAILED, if call iconv_open() failed.
 * 		- OMRPORT_ERROR_STRING_ILLEGAL_STRING if the input string is malformed
 *  	- OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING if the input or output encoding is not supported
 *  	- OMRPORT_ERROR_STRING_MEM_ALLOCATE_FAILED if the port library could not allocate a working buffer
 *  	The following translations are supported:
 *  	ANSI code page to modified UTF-8 (Windows only)
 *  	platform raw to [wide, modified UTF-8, UTF-8]
 *  	modified UTF-8 to [platform raw, wide, ISO Latin-1 (8859-1)]
 *  	wide to [modified UTF-8, platform raw]
 *  	[ISO Latin-1 (8859-1), UTF-8, Windows ANSI default and current code pages] to modified UTF-8
 * @note J9STR_CODE_PLATFORM_OMR_INTERNAL is an alias for other encodings depending on the platform.
 * @note J9STR_CODE_PLATFORM is deprecated. Use J9STR_CODE_PLATFORM_OMR_INTERNAL for results of system calls such as getenv (see stdlib.h) and
 * @note J9STR_CODE_PLATFORM_RAW where the port library does not do implicit translation.
 */
#ifndef OS_ENCODING_CODE_PAGE
/* placeholder on non-Windows systems */
#define OS_ENCODING_CODE_PAGE 0
#endif
int32_t
omrstr_convert(struct OMRPortLibrary *portLibrary, int32_t fromCode, int32_t toCode,
			   uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	int32_t result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;

	switch (fromCode) {
	case J9STR_CODE_PLATFORM_RAW: {
		switch (toCode) {
		case J9STR_CODE_MUTF8:
			result = convertPlatformToMutf8(portLibrary, OS_ENCODING_CODE_PAGE, inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
		case J9STR_CODE_WIDE:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		case J9STR_CODE_UTF8:
			result = convertPlatformToUtf8(portLibrary, (const uint8_t*)inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
	}
	break;
#if defined(OMR_OS_WINDOWS)
	case J9STR_CODE_WINTHREADACP:
	case J9STR_CODE_WINDEFAULTACP:
		switch (toCode) {
		case J9STR_CODE_MUTF8:
			result = convertPlatformToMutf8(portLibrary, (fromCode == J9STR_CODE_WINTHREADACP)? CP_THREAD_ACP: CP_ACP, inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
		break;
#endif /* defined(OMR_OS_WINDOWS) */
	case J9STR_CODE_MUTF8:
		switch (toCode) {
		case J9STR_CODE_PLATFORM_RAW:
			result = convertMutf8ToPlatform(portLibrary, OS_ENCODING_CODE_PAGE, inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
		case J9STR_CODE_LATIN1: {
			const uint8_t *mutf8Cursor = inBuffer;
			uintptr_t mutf8Remaining = inBufferSize;
			result = convertMutf8ToLatin1(portLibrary, &mutf8Cursor, &mutf8Remaining, outBuffer, outBufferSize);
			/*
			 * convertMutf8ToLatin1 is resumable so does not return error if buffer too small.
			 * In this case we should have consumed all the data
			 */
			if (mutf8Remaining > 0) {
				result = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}
		}
		break;
#if defined(OMR_OS_WINDOWS)
		case J9STR_CODE_WINTHREADACP:
			result = convertMutf8ToPlatform(portLibrary, CP_THREAD_ACP, inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
		case J9STR_CODE_WINDEFAULTACP:
			result = convertMutf8ToPlatform(portLibrary, CP_ACP, inBuffer, inBufferSize, outBuffer, outBufferSize);
			break;
#endif /* defined(OMR_OS_WINDOWS) */
		case J9STR_CODE_WIDE: {
			const uint8_t *mutf8Cursor = inBuffer;
			uintptr_t mutf8Remaining = inBufferSize;
			result = convertMutf8ToWide(&mutf8Cursor, &mutf8Remaining, outBuffer, outBufferSize);
			/*
			 * inBuffer parameters are updated to reflect data untranslated due to insufficient space in the output buffer.
			 * In this case, all input characters should have been consumed.
			 */
			if (mutf8Remaining > 0) {
				result = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}
		}
		break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
		break;
	case J9STR_CODE_UTF8:
		switch (toCode) {
		case J9STR_CODE_MUTF8: {
			const uint8_t *utf8Cursor = inBuffer;
			uintptr_t utf8Remaining = inBufferSize;
			result = convertUtf8ToMutf8(portLibrary, &utf8Cursor, &utf8Remaining, outBuffer, outBufferSize);
			/*
			 * convertUtf8ToMutf8 is resumable so does not return error if buffer too small.
			 * In this case we should have consumed all the data
			 */
			if (utf8Remaining > 0) {
				result = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}
		}
		break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
		break;
	case J9STR_CODE_WIDE:
		switch (toCode) {
		case J9STR_CODE_PLATFORM_RAW:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		case J9STR_CODE_MUTF8: {
			const uint8_t *wideCursor = inBuffer;
			uintptr_t wideRemaining = inBufferSize;
			result = convertWideToMutf8(&wideCursor, &wideRemaining, outBuffer, outBufferSize);
			/*
			 * inBuffer parameters are updated to reflect data untranslated due to insufficient space in the output buffer.
			 * In this case, all input characters should have been consumed.
			 */
			if (wideRemaining > 0) {
				result = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}
		}
		break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
		break;
	case J9STR_CODE_LATIN1:
		switch (toCode) {
		case J9STR_CODE_MUTF8: {
			const uint8_t *latin1Cursor = inBuffer;
			uintptr_t latin1Remaining = inBufferSize;
			result = convertLatin1ToMutf8(portLibrary, &latin1Cursor, &latin1Remaining, outBuffer, outBufferSize);
			/*
			 * convertLatin1ToMutf8 is resumable so does not return error if buffer too small.
			 * In this case we should have consumed all the data
			 */
			if (latin1Remaining > 0) {
				result = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}
		}
		break;
		default:
			result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
			break;
		}
		break;
	default:
		result = OMRPORT_ERROR_STRING_UNSUPPORTED_ENCODING;
		break;
	}

	return result;
}

/**
 * Write characters to a string as specified by format.
 *
 * @param[in] portLibrary The port library.
 * @param[in, out] buf The string buffer to be written.
 * @param[in] bufLen The size of the string buffer to be written.
 * @param[in] format The format of the string.
 * @param[in] args Arguments for the format string.
 *
 * @return The number of characters printed not including the NUL terminator.
 *
 * @note When buf is NULL, the size of the buffer required to print to the string, including
 * the NUL terminator is returned.
 *
 * @internal @note Supported, portable format specifiers are described in the document entitled "PortLibrary printf"
 * in the "Inside J9" Lotus Notes database.
 */
uintptr_t
omrstr_vprintf(struct OMRPortLibrary *portLibrary, char *buf, uintptr_t bufLen, const char *format, va_list args)
{
	J9FormatData formatData;
	memset(&formatData, 0, sizeof(formatData));

	formatData.formatString = format;
	parseFormatString(portLibrary, &formatData);
	readValues(portLibrary, &formatData, args);

	return writeFormattedString(portLibrary, &formatData, buf, bufLen);
}

static int
parseFormatString(struct OMRPortLibrary *portLibrary, J9FormatData *result)
{
	const char *format = result->formatString;

	while ('\0' != *format) {
		switch (*format) {
		case '%':
			format++;
			switch (*format) {
			case '%':
				/* literal '%' */
				format++;
				break;
			default:
				format = parseIndex(format, &result->spec[result->specCount].index);
				format = parseTagChar(format, result);
				format = parseWidth(format, result);
				format = parsePrecision(format, result);
				format = parseModifier(format, result);
				format = parseType(format, result);
				if (format == NULL) {
					return -1;
				}
				result->specCount++;
			}
			break;
		default:
			format++;
		}
	}

	return 0;
}

static const char *
parseTagChar(const char *format, J9FormatData *result)
{
	switch (*format) {
	case '0':
		result->spec[result->specCount].tag |= J9FFLAG_ZERO;
		format++;
		break;
	case ' ':
		result->spec[result->specCount].tag |= J9FFLAG_SPACE;
		format++;
		break;
	case '-':
		result->spec[result->specCount].tag |= J9FFLAG_DASH;
		format++;
		break;
	case '+':
		result->spec[result->specCount].tag |= J9FFLAG_PLUS;
		format++;
		break;
	case '#':
		result->spec[result->specCount].tag |= J9FFLAG_HASH;
		format++;
		break;
	}

	return format;
}

static const char *
parseWidth(const char *format, J9FormatData *result)
{
	uint8_t index = 0;

	if (*format == '*') {
		format = parseIndex(format + 1, &result->spec[result->specCount].widthIndex);
		index = result->spec[result->specCount].widthIndex;
		if (index == 0xFF) {
			index = result->valueCount;
			result->spec[result->specCount].widthIndex = index;
		}
		result->valueCount++;
		result->valueType[index] = J9FTYPE_U32;
		result->value[index].u64 = J9F_NO_VALUE;

		return format;
	} else {
		uint32_t width = 0;
		int anyDigits = 0;
		for (;;) {
			switch (*format) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				anyDigits = 1;
				width = width * 10 + (*format - '0');
				format += 1;
				break;
			default:
				index = J9F_MAX_ARGS - ++result->immediateCount;
				result->spec[result->specCount].widthIndex = index;
				result->valueType[index] = J9FTYPE_IMMEDIATE;
				if (anyDigits) {
					result->value[index].u64 = width;
				} else {
					result->value[index].u64 = J9F_NO_VALUE;
				}
				return format;
			}
		}
	}
}

static const char *
parsePrecision(const char *format, J9FormatData *result)
{
	uint8_t index = 0;

	if (*format == '.') {
		format += 1;
	} else {
		index = J9F_MAX_ARGS - ++result->immediateCount;
		result->spec[result->specCount].precisionIndex = index;
		result->valueType[index] = J9FTYPE_IMMEDIATE;
		result->value[index].u64 = J9F_NO_VALUE;

		return format;
	}

	if (*format == '*') {
		format = parseIndex(format + 1, &result->spec[result->specCount].precisionIndex);
		index = result->spec[result->specCount].precisionIndex;
		if (index == 0xFF) {
			index = result->valueCount;
			result->spec[result->specCount].precisionIndex = index;
		}
		result->valueCount++;
		result->valueType[index] = J9FTYPE_U32;
		result->value[index].u64 = J9F_NO_VALUE;

		return format;
	} else {
		uint32_t precision = 0;
		int anyDigits = 0;
		for (;;) {
			switch (*format) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				anyDigits = 1;
				precision = precision * 10 + (*format - '0');
				format += 1;
				break;
			default:
				index = J9F_MAX_ARGS - ++result->immediateCount;
				result->spec[result->specCount].precisionIndex = index;
				result->valueType[index] = J9FTYPE_IMMEDIATE;
				if (anyDigits) {
					result->value[index].u64 = precision;
				} else {
					result->value[index].u64 = J9F_NO_VALUE;
				}
				return format;
			}
		}
	}
}

static const char *
parseModifier(const char *format, J9FormatData *result)
{
	switch (*format) {
	case 'z':
		format++;
#ifdef OMR_ENV_DATA64
		result->spec[result->specCount].tag |= J9FSPEC_LL;
#endif
		break;
	case 'l':
		format++;
		if (*format == 'l') {
			format++;
			result->spec[result->specCount].tag |= J9FSPEC_LL;
		} else {
			result->spec[result->specCount].tag |= J9FSPEC_L;
		}
		break;
	}

	return format;
}

static const char *
parseType(const char *format, J9FormatData *result)
{
	const char *type = format++;
	uint8_t tag = result->spec[result->specCount].tag;
	uint8_t index = result->spec[result->specCount].index;

	if (index == 0xFF) {
		index = result->valueCount;
		result->spec[result->specCount].index = index;
	}
	result->valueCount++;

	result->spec[result->specCount].type = type;

	switch (*type) {
		/* integers */
	case 'c':
		result->valueType[index] = J9FTYPE_U32;
		break;
	case 'i':
	case 'd':
	case 'u':
	case 'x':
	case 'X':
		result->valueType[index] = tag & J9FSPEC_LL ? J9FTYPE_U64 : J9FTYPE_U32;
		break;

		/* pointers */
	case 'p':
	case 's':
		result->valueType[index] = J9FTYPE_PTR;
		break;

		/* floats */
	case 'f':
	case 'e':
	case 'E':
	case 'F':
	case 'g':
	case 'G':
		result->valueType[index] = J9FTYPE_DBL;
		break;

	default:
		return NULL;
	}

	return format;
}

static void
readValues(struct OMRPortLibrary *portLibrary, J9FormatData *result, va_list args)
{
	uint8_t index = 0;
	va_list argsCopy;

	COPY_VA_LIST(argsCopy, args);
	for (index = 0; index < result->valueCount; index++) {
		switch (result->valueType[index]) {
		case J9FTYPE_U64:
			result->value[index].u64 = va_arg(argsCopy, uint64_t);
			break;
		case J9FTYPE_U32:
			result->value[index].u64 = va_arg(argsCopy, uint32_t);
			break;
		case J9FTYPE_DBL:
			result->value[index].dbl = va_arg(argsCopy, double);
			break;
		case J9FTYPE_PTR:
			result->value[index].ptr = va_arg(argsCopy, void *);
			break;
		case J9FTYPE_IMMEDIATE:
			/* shouldn't be encountered -- these should all be at the end of the value array */
			break;
		}
	}
	END_VA_LIST_COPY(argsCopy);
}

static uintptr_t
writeSpec(J9FormatData *data, J9FormatSpecifier *spec, char *result, uintptr_t length)
{
	J9FormatValue *value = &data->value[spec->index];
	uint64_t width = data->value[spec->widthIndex].u64;
	uint64_t precision = data->value[spec->precisionIndex] .u64;
	uintptr_t index = 0;

	switch (*spec->type) {
	case 'i':
	case 'd':
		index = writeIntToBuffer(result, length, width, precision, value->u64, spec->tag, 1, digits_dec);
		break;
	case 'u':
		index = writeIntToBuffer(result, length, width, precision, value->u64, spec->tag, 0, digits_dec);
		break;
	case 'x':
		index = writeIntToBuffer(result, length, width, precision, value->u64, spec->tag, 0, digits_hex_lower);
		break;
	case 'X':
		index = writeIntToBuffer(result, length, width, precision, value->u64, spec->tag, 0, digits_hex_upper);
		break;

	case 'p':
		index = writeIntToBuffer(result, length, sizeof(uintptr_t) * 2, sizeof(uintptr_t) * 2, (uintptr_t)value->ptr, 0, 0, digits_hex_upper);
		break;

	case 'c':
		if (spec->tag & J9FSPEC_L) {
			char asUTF8[4];
			uint32_t numberWritten = encodeUTF8Char((uintptr_t)value->u64, (uint8_t *)asUTF8);

			/* what if width/precision is less than size of asUTF8? [truncate?] */
			asUTF8[numberWritten] = '\0';
			index = writeStringToBuffer(result, length, width, precision, asUTF8, spec->tag);
		} else {
			index = writeStringToBuffer(result, length, width, precision, " ", spec->tag);
			if (index <= length) {
				if (result) {
					result[index - 1] = (char)value->u64;
				}
			}
		}
		break;

	case 's':
		if (value->ptr) {
			if (spec->tag & J9FSPEC_L) {
				index = writeUnicodeStringToBuffer(result, length, width, precision, (const uint16_t *)value->ptr, spec->tag);
			} else {
				index = writeStringToBuffer(result, length, width, precision, (const char *)value->ptr, spec->tag);
			}
		} else {
			index = writeStringToBuffer(result, length, width, precision, "<NULL>", spec->tag);
		}
		break;

		/* floats */
	case 'f':
	case 'e':
	case 'E':
	case 'F':
	case 'g':
	case 'G':
		index = writeDoubleToBuffer(result, length, width, precision, value->dbl, *spec->type, spec->tag);
		break;
	}

	return index;
}

/**
 * @internal
 *
 * Writes the string representation of the specified value into the provided buffer
 * using the specified precision.
 *
 * @param[in] buf       The buffer into which results should be written.
 * @param[in] bufLen    The length of the buffer in bytes, never writes more than this number of bytes.
 * @param[in] precision The maximum number of digits to display, J9F_NO_VALUE for as many digits as required.
 * @param[in] value     The value to print.
 * @param[in] tag       One of the J9FFLAG_* constants that controls the output format.
 * @param[in] isSigned  Non-zero if the value is signed.
 * @param[in] digits    A string containing hex/decimal digits (starting at zero).
 *
 * @return The number of bytes written to buf.
 */
static uintptr_t
writeIntToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, uint64_t value, uint8_t tag, int isSigned, const char *digits)
{
	uint32_t index = 0;
	uint32_t length = 0;
	uint32_t rightSpace = 0;
	uint64_t temp = 0;
	size_t base = strlen(digits);
	int32_t actualPrecision = 0; /* precision is specified unsigned, but code may decrement temp values below zero */
	char signChar = 0;

	if (isSigned) {
		int64_t signedValue;
		if (tag & J9FSPEC_LL) {
			signedValue = (int64_t)value;
		} else {
			signedValue = (int32_t)value;
		}

		if (signedValue < 0) {
			signChar = '-';
			value = (uint64_t)(signedValue * -1);
		} else if (signedValue >= 0 && (tag & J9FFLAG_PLUS)) {
			signChar = '+';
		}
	}

	/* find the end of the number */
	temp = value;
	do {
		length += 1;
		temp /= base;
	} while (0 != temp);

	if (precision != J9F_NO_VALUE) {
		actualPrecision = (int32_t)precision;

		/* subtle: actualPrecision known to be non-negative (hence cast to uint32_t)
		 * for purposes of comparison.
		 */
		if ((uint32_t)actualPrecision > length) {
			length = actualPrecision;
		}
	}

	/* Account for "-" Must be after setting actualPrecision, before calculation of rightSpace */
	if (signChar) {
		length += 1;
	}

	if (width != J9F_NO_VALUE) {
		uint32_t actualWidth = (uint32_t)width;	/* shorten user-specified width to uint32_t */

		if (actualWidth > length) {
			if (tag & J9FFLAG_DASH) {
				rightSpace = actualWidth - length;
			}
			length = actualWidth;
		}
	}

	if (tag & J9FFLAG_ZERO) {
		actualPrecision = length - (signChar ? 1 : 0);
	}

	/* now write the number out backwards */
	for (; rightSpace != 0; rightSpace--) {
		length -= 1;
		if (bufLen > length) {
			if (buf) {
				buf[length] = ' ';
			}
			index += 1;
		}
	}

	/* write out the digits */
	temp = value;
	do {
		length -= 1;
		actualPrecision -= 1;
		if (bufLen > length) {
			if (buf) {
				buf[length] = digits[(int)(temp % base)];
			}
			index += 1;
		}
		temp /= base;
	} while (0 != temp);

	/* zero extend to the left according the the requested precision */
	while (length > 0) {
		length -= 1;
		actualPrecision -= 1;
		if (bufLen > length) {
			if (buf) {
				if (actualPrecision >= 0) {
					buf[length] = '0';
				} else {
					if (signChar) {
						buf[length] = signChar;
						/* only print the sign char once */
						signChar = 0;
					} else {
						buf[length] = ' ';
					}
				}
			}
			index += 1;
		}
	}

	return index;
}

static uintptr_t
writeStringToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, const char *value, uint8_t tag)
{
	size_t leftPadding = 0;
	size_t rightPadding = 0;

	if (precision == J9F_NO_VALUE) {
		precision = strlen(value);
	} else {
		int32_t n;
		/* detect if the string is shorter than precision */
		for (n = 0; n < precision; n++) {
			if (value[n] == 0) {
				precision = n;
				break;
			}
		}
	}

	if (width == J9F_NO_VALUE) {
		width = precision;
	}

	if (width > precision) {
		if (tag & J9FFLAG_DASH) {
			rightPadding = (size_t)(width - precision);
		} else {
			leftPadding = (size_t)(width - precision);
		}
	}

	if (leftPadding > bufLen) {
		leftPadding = bufLen;
	}
	if (0 != leftPadding) {
		if (NULL != buf) {
			memset(buf, ' ', leftPadding);
			buf += leftPadding;
		}
		bufLen -= leftPadding;
	}

	if (precision > bufLen) {
		precision = bufLen;
	}
	if (NULL != buf) {
		memcpy(buf, value, (size_t)precision);
		buf += (size_t)precision;
	}
	bufLen -= (size_t)precision;

	if (rightPadding > bufLen) {
		rightPadding = bufLen;
	}
	if ((NULL != buf) && (0 != rightPadding)) {
		memset(buf, ' ', rightPadding);
	}

	return leftPadding + (size_t)precision + rightPadding;
}

/*
 * @internal
 *
 * To determine size of buffer required for format string pass in a NULL buffer and the maximum
 * size willing to create.  For example for no restrictions result=NULL, length=(uint32_t)(-1), to restrict
 * the buffer to 2k, result=NULL, length=2048
 *
 * Value returned does not include space required for the null terminator
 */
static uintptr_t
writeFormattedString(struct OMRPortLibrary *portLibrary, J9FormatData *data, char *result, uintptr_t length)
{
	const char *format = data->formatString;
	uint8_t specIndex = 0;
	uintptr_t index = 0;

	if (NULL == result) {
		length = (uintptr_t)-1;
	} else if (0 == length) {
		/* empty buffer */
		return 0;
	}

	while (('\0' != *format) && (index < length - 1)) {
		switch (*format) {
		case '%':
			format++;
			switch (*format) {
			case '%':
				/* literal '%' */
				if (result) {
					result[index] = '%';
				}
				index++;
				format++;
				break;
			default:
				if (NULL != result) {
					index += writeSpec(data, &data->spec[specIndex], result + index, length - index);
				} else {
					index += writeSpec(data, &data->spec[specIndex], result, length);
				}

				format = data->spec[specIndex].type + 1;
				specIndex += 1;
				break;
			}
			break;
		default:
			if (NULL != result) {
				result[index] = *format;
			}
			format++;
			index++;
		}
	}

	/* writeSpec can return value > 1, so index does not increment sequentially */
	if (index > length - 1) {
		index = length - 1;
	}

	if (result) {
		result[index] = 0;
	}

	if (NULL == result) {
		return index + 1; /* For the NUL terminator */
	}

	return index;
}

static uintptr_t
writeDoubleToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, double value, uint8_t type, uint8_t tag)
{
	char format[sizeof("%+4294967295.4294967295f")];
	char *formatCursor = format;
	char *lastFormat = format + sizeof(format);
	char tempBuf[510]; /* 509 is maximum size of a converted double */

	*formatCursor++ = '%';

	if (tag & J9FFLAG_DASH) {
		*formatCursor++ = '-';
	} else if (tag & J9FFLAG_PLUS) {
		*formatCursor++ = '+';
	} else if (tag & J9FFLAG_SPACE) {
		*formatCursor++ = ' ';
	} else if (tag & J9FFLAG_ZERO) {
		*formatCursor++ = '0';
	} else if (tag & J9FFLAG_HASH) {
		*formatCursor++ = '#';
	}

	if (width != J9F_NO_VALUE) {
		formatCursor += writeIntToBuffer(formatCursor, lastFormat - formatCursor, J9F_NO_VALUE, J9F_NO_VALUE, width, 'u', 0, digits_dec);
	}
	if (precision != J9F_NO_VALUE) {
		*formatCursor++ = '.';
		formatCursor += writeIntToBuffer(formatCursor, lastFormat - formatCursor, J9F_NO_VALUE, J9F_NO_VALUE, precision, 'u', 0, digits_dec);
	}

	*formatCursor++ = type;
	*formatCursor = '\0';

	sprintf(tempBuf, format, value);

	if (buf) {
		strncpy(buf, tempBuf, bufLen);
		buf[bufLen - 1] = '\0';
		return strlen(buf);
	}

	return strlen(tempBuf);
}

/**
 * PortLibrary shutdown.
 *
 * This function is called during shutdown of the portLibrary.  Any resources that were created by @ref omrstr_startup
 * should be destroyed here.
 *
 * @param[in] portLibrary The port library.
 *
 * @note Most implementations will be empty.
 */
void
omrstr_shutdown(struct OMRPortLibrary *portLibrary)
{
}

/**
 * PortLibrary startup.
 *
 * This function is called during startup of the portLibrary.  Any resources that are required for
 * the string operations may be created here.  All resources created here should be destroyed
 * in @ref omrstr_shutdown.
 *
 * @param[in] portLibrary The port library.
 *
 * @return 0 on success, negative error code on failure.  Error code values returned are
 * \arg OMRPORT_ERROR_STARTUP_STR
 *
 * @note Most implementations will simply return success.
 */
int32_t
omrstr_startup(struct OMRPortLibrary *portLibrary)
{
	return 0;
}

static uintptr_t
writeUnicodeStringToBuffer(char *buf, uintptr_t bufLen, uint64_t width, uint64_t precision, const uint16_t *value, uint8_t tag)
{
	uint32_t numberOfUnicodeChar = 0;
	uint32_t numberOfUTF8Char = 0;
	uint32_t encodingLength = 0;
	size_t leftPadding = 0;
	size_t rightPadding = 0;
	const uint16_t *currentU16 = NULL;

	if (precision == J9F_NO_VALUE) {
		currentU16 = value;
		precision = 0;
		while (*currentU16++ != 0) {
			precision++;
		}
	} else {
		int32_t n = 0;
		/* detect if the string is shorter than precision */
		for (n = 0; n < precision; n++) {
			if (value[n] == 0) {
				precision = n;
				break;
			}
		}
	}

	currentU16 = value;
	while (numberOfUnicodeChar < precision) {
		encodingLength = encodeUTF8Char((uintptr_t)*currentU16++, NULL);
		if (numberOfUTF8Char + encodingLength > bufLen) {
			break;
		}

		/* If the character fits then increment counts */
		numberOfUnicodeChar++;
		numberOfUTF8Char += encodingLength;
	}

	if (width == J9F_NO_VALUE) {
		width = numberOfUTF8Char;
	}

	if (width > numberOfUTF8Char) {
		if (tag & J9FFLAG_DASH) {
			rightPadding = (size_t)(width - numberOfUTF8Char);
		} else {
			leftPadding = (size_t)(width - numberOfUTF8Char);
		}
	}

	if (leftPadding > bufLen) {
		leftPadding = bufLen;
	}
	if (0 != leftPadding) {
		if (NULL != buf) {
			memset(buf, ' ', leftPadding);
			buf += leftPadding;
		}
		bufLen -= leftPadding;
	}

	/* The space required for the UTF8 chars is guaranteed to be there */
	if (NULL != buf) {
		currentU16 = value;
		while (numberOfUnicodeChar-- > 0) {
			buf += encodeUTF8Char((uintptr_t)*currentU16++, (uint8_t *)buf);
		}
	}
	bufLen -= numberOfUTF8Char;

	if (rightPadding > bufLen) {
		rightPadding = bufLen;
	}
	if ((NULL != buf) && (0 != rightPadding)) {
		memset(buf, ' ', rightPadding);
	}

	return leftPadding + numberOfUTF8Char + rightPadding;
}

static const char *
parseIndex(const char *format, uint8_t *result)
{
	const char *start = format;
	uint8_t index = 0;

	for (;;) {
		switch (*format) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			index = index * 10 + (*format - '0');
			format += 1;
			break;
		case '$':
			if (index > 0) {
				*result = index - 1;
				return format + 1;
			}
			/* fall through */
		default:
			*result = 0xFF;
			return start;
		}
	}
}

/*
 * @internal
 *
 * sets tm to Epoch
 */
static void
setJ9TimeToEpoch(struct J9TimeInfo *tm)
{
	/* set the default value to epoch */
	tm->second = 0;
	tm->minute = 0;
	tm->hour = 0;
	tm->day = 1;
	tm->weekday = 4; /* Thursday */
	tm->month = 1;
	tm->year = 1970;
}

/**
 * @internal
 *
 * Converts the UTC time passed in as milli seconds since Epoch to local time as the J9-specific representation of time.
 *
 * @param[in] millisUTC the UTC time passed in as milli seconds since Epoch. Negative values are not allowed.
 * @param[in,out] the local time as the J9-specific representation of time. Will not pass back any values that occurred before Epoch.
 * @param[in] flags specifies if the time should be adjusted for the local time zone
 *
 * @return the local time or Epoch
 *
 * @note will not pass back any dates that occurred before Epoch
 */
static void convertUTCMillisToJ9Time(int64_t millisUTC, struct J9TimeInfo *omrtimeInfo, uint32_t flags)
{
	time_t secondsUTC = (time_t)(millisUTC / 1000);
	struct tm osTime;

	/* we don't allow dates/times prior to Epoch */
	if (millisUTC < 0) {
fail:
		setJ9TimeToEpoch(omrtimeInfo);
		return;
	}

	if (OMR_ARE_ANY_BITS_SET(flags, OMRSTR_FTIME_FLAG_UTC)) {
#if defined(OMR_OS_WINDOWS)
		if (0 != gmtime_s(&osTime, &secondsUTC)) {
			goto fail;
		}
#else /* defined(OMR_OS_WINDOWS) */
		if (NULL == gmtime_r(&secondsUTC, &osTime)) {
			goto fail;
		}
#endif /* defined(OMR_OS_WINDOWS) */
	} else {
#if defined(OMR_OS_WINDOWS)
		if (0 != localtime_s(&osTime, &secondsUTC)) {
			goto fail;
		}
#else /* defined(OMR_OS_WINDOWS) */
		if (NULL == localtime_r(&secondsUTC, &osTime)) {
			goto fail;
		}
#endif /* defined(OMR_OS_WINDOWS) */
	}

	/* Times less than Epoch are not allowed. */
	if (osTime.tm_year < 70) {
		setJ9TimeToEpoch(omrtimeInfo);
	} else {
		omrtimeInfo->second = osTime.tm_sec;
		omrtimeInfo->minute = osTime.tm_min;
		omrtimeInfo->hour = osTime.tm_hour;
		omrtimeInfo->day = osTime.tm_mday;
		omrtimeInfo->weekday = osTime.tm_wday;
		omrtimeInfo->month = osTime.tm_mon + 1;
		omrtimeInfo->year = osTime.tm_year + 1900;
	}
}

/**
 * @internal
 * Hash table callback for retrieving hash value of an entry
 *
 * @param[in] entry Entry to hash
 * @param[in] userData Data that can be passed along, unused in this callback
 */
static uintptr_t
tokenHashFn(void *entry, void *userData)
{
	J9TokenEntry *token = (J9TokenEntry *)entry;

	/* Because the token keys will mostly be single characters, using the first char as the hash
	 * value is pretty reasonable */
	return (uintptr_t)*(token->key);
}

/**
 * @internal
 * Hash table callback for entry comparison
 *
 * @param[in] lhsEntry Left entry to compare
 * @param[in] rhsEntry Right entry to compare
 * @param[in] userData Data that can be passed along, unused in this callback
 *
 * @return True if the entries are deemed equal, that is if the string representation of their
 * keys are identical.
 *
 * @note that the keys correspond to the tokens that show up in the format string
 */
static uintptr_t
tokenHashEqualFn(void *lhsEntry, void *rhsEntry, void *userData)
{
	J9TokenEntry *ltoken = (J9TokenEntry *)lhsEntry;
	J9TokenEntry *rtoken = (J9TokenEntry *)rhsEntry;

	return 0 == strcmp(ltoken->key, rtoken->key);
}

/**
 * @internal
 * Helper function for populating the hash table with default, platform independant tokens
 *
 * @param[in] portLibrary The port library in use
 * @param[in] tokens The token cookie that should be populated
 * @param[in] timeMillis The time in milliseconds that should be used as a basis for the time
 * tokens
 *
 * @return 0 on success, -1 on failure
 */
static intptr_t
populateWithDefaultTokens(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens, int64_t timeMillis)
{
#define USERNAME_BUF_LEN 128
#define JOBNAME_BUF_LEN 128
#define JOBID_BUF_LEN 16
#define ASID_BUF_LEN 16
#define SYSNAME_BUF_LEN 32

	uintptr_t pid = 0;
	char username[USERNAME_BUF_LEN];
	char jobname[JOBNAME_BUF_LEN];
	char jobid[JOBID_BUF_LEN];
	char asid[ASID_BUF_LEN];
	char sysname[SYSNAME_BUF_LEN];

	if (NULL == tokens) {
		return -1;
	}

	/* Let's get the token values */
	pid = portLibrary->sysinfo_get_pid(portLibrary);
	omrget_jobname(portLibrary, jobname, JOBNAME_BUF_LEN);
	omrget_jobid(portLibrary, jobid, JOBID_BUF_LEN);
	omrget_asid(portLibrary, asid, ASID_BUF_LEN);
	omrget_sysname(portLibrary, sysname, SYSNAME_BUF_LEN);

	portLibrary->str_set_time_tokens(portLibrary, tokens, timeMillis);

	if ((0 != portLibrary->str_set_token(portLibrary, tokens, "pid", "%u", pid))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "job", "%s", jobname))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "home", "%s", ""))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "last", "%s", ""))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "seq", "%04u", 0))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "jobid", "%s", jobid))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "asid", "%s", asid))
	||  (0 != portLibrary->str_set_token(portLibrary, tokens, "sysname", "%s", sysname))
	) {
		/* If any of the above fail, we're out of memory */
		return -1;
	}

	/* Let's add the username, note that this is done seperately since it
	 * may fail on some platforms, but in that event, we just don't add it */
	if (0 == portLibrary->sysinfo_get_username(portLibrary, username, USERNAME_BUF_LEN)) {
		portLibrary->str_set_token(portLibrary, tokens, "uid", "%s", username);
	}

	return 0;

#undef USERNAME_BUF_LEN
#undef JOBNAME_BUF_LEN
#undef JOBID_BUF_LEN
#undef ASID_BUF_LEN
#undef SYSNAME_BUF_LEN
}

/**
 * @internal
 * Hash table callback used when about to free the hash table, this ensures the memory
 * used by the token is freed up (and not just the hash table itself).
 *
 * @param[in] entry The entry to free
 * @param[in] portLibrary This is passed along as "user data" and is used for calling
 * mem_free_memory
 */
static uintptr_t
tokenDoFreeFn(void *entry, void *portLibrary)
{
	J9TokenEntry *token = (J9TokenEntry *)entry;

	((struct OMRPortLibrary *)portLibrary)->mem_free_memory(portLibrary, token->key);
	token->key = NULL;
	token->keyLen = 0;
	((struct OMRPortLibrary *)portLibrary)->mem_free_memory(portLibrary, token->value);
	token->value = NULL;
	token->valueLen = 0;

	return 0;
}

/**
 * @internal
 * Helper function for token expansion; reads key and determines if a token is present
 *
 * @param[in] tokens The tokens that should be used for expansion
 * @param[in] key The position of the '%' starting off a token (or a % escape)
 *
 * @return When a token is found, its entry with the value at expansion is returned. When
 * no token is found, NULL is returned.
 */
static J9TokenEntry *
consumeToken(J9StringTokens *tokens, const char *key)
{
	char tokenKey[J9TOKEN_MAX_KEY_LEN];
	J9TokenEntry lookupEntry;
	J9TokenEntry *entry = NULL;
	char *write = tokenKey;

	/* For consistency, we enforce starting on a %, this simplifies the calling code */
	if (*key != '%') {
		return NULL;
	}

	/* Setup the lookup entry */
	key++;
	memset(tokenKey, 0, J9TOKEN_MAX_KEY_LEN);
	lookupEntry.key = tokenKey;
	lookupEntry.keyLen = 0;

	/* Let's start by reading in the largest amount we can so longer token names can
	 * have precedence */
	while (('\0' != *key) && (lookupEntry.keyLen < (J9TOKEN_MAX_KEY_LEN - 1))) {
		if (*key == ' ') {
			/* End of possible token name */
			break;
		}

		if (*key == '%') {
			/* This may signify the end of the token name or the % token */
			if (lookupEntry.keyLen == 0) {
				*write = *key;
				lookupEntry.keyLen++;
			}
			break;
		}

		*write++ = *key++;
		lookupEntry.keyLen++;
	}

	/* Now let's start querying the hash table for tokens */
	while (0 != lookupEntry.keyLen) {
		entry = hashTableFind((J9HashTable *)tokens, &lookupEntry);
		if (NULL != entry) {
			return entry;
		}

		lookupEntry.keyLen--;
		lookupEntry.key[lookupEntry.keyLen] = '\0';
	}

	return NULL;
}

/* the largest 64 bit number has 20 digits */
#define TICK_BUF_SIZE 21

/**
 * Update the time tokens stored as local time, from the timeMillis parameter that was passed in as UTC.
 * The cookie must be freed with omrstr_tokens_destroy() when it is no longer needed.
 *
 * The time tokens always include:
 *
 * 	  %Y     year    1900..????		(local time, based on millis parameter)
 *	  %y     year of century  00..99
 *	  %m     month     01..12
 *	  %b     abbreviated month name in English
 *	  %d     day       01..31
 *	  %H     hour      00..23
 *	  %M     minute    00..59
 *	  %S     second    00..59
 *
 *	  %tick  high res timer
 *
 * @param[in]  portLib the port library
 * @param[out] tokens the tokens hashtable, with time in local time.
 * @param[in]  timeMillis the UTC time in ms to use for the date/time tokens
 *
 * @return 0 on success, otherwise -1 (out of memory)
 */
intptr_t
omrstr_set_time_tokens(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens, int64_t timeMillis)
{
	char buf[TICK_BUF_SIZE + 20]; /* TICK_BUF_SIZE + 4 + 2 + 2 + 2 + 2 + 2 + 2 + 3 + an extra byte */

	omrstr_subst_time(portLibrary, buf, TICK_BUF_SIZE + 20, "%Y%y%m%d%H%M%S%b%tick", timeMillis, OMRSTR_FTIME_FLAG_LOCAL);

	if ((0 != omrstr_set_token_from_buf(portLibrary, tokens, "Y", buf, 4))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "y", buf + 4, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "m", buf + 6, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "d", buf + 8, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "H", buf + 10, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "M", buf + 12, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "S", buf + 14, 2))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "b", buf + 16, 3))
	||  (0 != omrstr_set_token_from_buf(portLibrary, tokens, "tick", buf + 19, (uint32_t)strlen(buf + 19)))
	) {
		/* If any of the above fail, we're out of memory */
		return -1;
	}
	return 0;
}

/**
 * Create a token cookie for use with the token APIs.
 * The token cookie will be populated with the default tokens.
 * Time tokens will be initialized to the local time based on the UTC time that is passed in.
 *
 * The cookie must be freed with omrstr_tokens_destroy() when it is no longer needed.
 *
 * The default tokens always include:
 *
 * 	  %Y     year    1970..????
 *	  %y     year of century  00..99
 *	  %m     month     01..12
 *	  %b     abbreviated month name in English
 *	  %d     day       01..31
 *	  %H     hour      00..23
 *	  %M     minute    00..59
 *	  %S     second    00..59
 *
 *	  %tick  high res timer
 *
 *	  %pid   process id
 *
 *	  %uid   user name
 *
 * Some platforms may include additional tokens. In particular:
 *	  %job   job name	(z/OS only)

 * @param[in] portLib the port library
 * @param[in] timeMillis the UTC time in ms to use for the date/time tokens. This value must be greater than or equal to zero
 *
 * @return the opaque J9StringTokens structure, or NULL if out of memory. The date/time will never be less than Epoch,
 * which is January 1, 1970, 00:00:00 AM UTC.
 */
struct J9StringTokens *
omrstr_create_tokens(struct OMRPortLibrary *portLibrary, int64_t timeMillis)
{
	J9HashTable *tokens = NULL;
	J9TokenEntry percentEntry;

	percentEntry.key = NULL;
	percentEntry.value = NULL;

	/* The token cookie is actually a pointer to a hash table */
	tokens = hashTableNew(portLibrary, OMR_GET_CALLSITE(), J9TOKEN_TABLE_INIT_SIZE,
						  sizeof(J9TokenEntry), J9TOKEN_TABLE_ALIGNMENT, 0,  OMRMEM_CATEGORY_PORT_LIBRARY, tokenHashFn, tokenHashEqualFn,
						  NULL, NULL);

	if (NULL == tokens) {
		goto fail;
	}

	if (0 != populateWithDefaultTokens(portLibrary, (struct J9StringTokens *)tokens, timeMillis)) {
		goto fail;
	}

	/* Add the special "%" token to simplify the actual token expansion code. Note
	 * that the key and value are allocated on the heap so the free_tokens code
	 * doesn't have to handle a special case. */
	percentEntry.key = (char *)portLibrary->mem_allocate_memory(portLibrary, 2, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
	percentEntry.value = (char *)portLibrary->mem_allocate_memory(portLibrary, 2, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
	if ((NULL == percentEntry.key) || (NULL == percentEntry.value)) {
		goto fail;
	}

	percentEntry.key[0] = '%';
	percentEntry.key[1] = '\0';
	percentEntry.keyLen = 1;
	percentEntry.value[0] = '%';
	percentEntry.value[1] = '\0';
	percentEntry.valueLen = 1;
	if (NULL == hashTableAdd((J9HashTable *)tokens, &percentEntry)) {
		goto fail;
	}

	return (struct J9StringTokens *)tokens;
fail:
	portLibrary->mem_free_memory(portLibrary, percentEntry.key);
	portLibrary->mem_free_memory(portLibrary, percentEntry.value);
	portLibrary->str_free_tokens(portLibrary, (struct J9StringTokens *)tokens);

	return NULL;
}

/**
 * Add the specified key to the list of tokens.
 *
 * The token value is determined by expanding format and the var args using omrstr_vprintf.
 * The resulting value is used in a call to omrstr_set_token_from_buf
 *
 * Both key and value will be copied, so it is safe to free or overwrite them
 * once this API returns.
 *
 * @param[in] portLib the port library
 * @param[in/out] tokens the token cookie to be modified
 * @param[in] key the key to be modified (e.g. "H" to modify the hour key)
 * @param[in] format a printf format string which describes how to create the value for the token
 * @param[in] ... arguments which match the format string
 *
 * @return 0 on success, -1 on failure. Failure can occur if the key is too large, of size 0 or
 * if we're out of memory.
 */
intptr_t
omrstr_set_token(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens, const char *key, const char *format, ...)
{
	const char *cur = key;
	char *tokenBuf = NULL;
	uintptr_t tokenBufLen = 0;
	uintptr_t keyLen = 0;
	uintptr_t valueLen = 0;
	va_list args;

	/* Let's validate the key */
	while ((keyLen < J9TOKEN_MAX_KEY_LEN) && ('\0' != *cur)) {
		if ((*cur == ' ') || (*cur == '%')) {
			return -1;
		}
		++cur;
		++keyLen;
	}

	if ((0 == keyLen) || (keyLen >= J9TOKEN_MAX_KEY_LEN)) {
		return -1;
	}

	va_start(args, format);
	/* find out how large a buffer we need to format the strings */
	tokenBufLen = portLibrary->str_vprintf(portLibrary, NULL, 0, format, args);

	/* stack allocate the necessary storage */
	tokenBuf = (char *)alloca(tokenBufLen);

	valueLen = portLibrary->str_vprintf(portLibrary, tokenBuf, tokenBufLen, format, args);
	va_end(args);

	return omrstr_set_token_from_buf(portLibrary, tokens, key, tokenBuf, (uint32_t)valueLen);
}

/**
 * Add the specified key to the list of tokens.
 * If the key is already in the list, the new value supercedes the existing value.
 *
 * Both key and value will be copied, so it is safe to free or overwrite them
 * once this API returns.
 *
 * @param[in] portLibrary the port library
 * @param[in/out] tokens the token cookie to be modified
 * @param[in] key the key to be modified (e.g. "H" to modify the hour key)
 * @param[in] tokenBuf the token (need not be null terminated)
 * @param[in] tokenLen the length of the token in tokenBuf
 *
 * @return 0 on success, -1 on failure. Failure can occur if the key is too large, of size 0 or
 * if we're out of memory.
 *
 * @note 1. The maximum key size is 32 (including the NULL terminator).
 * 2. If the token value is a simple empty string "", then a
 * buffer of 511 bytes + 1 null terminator (512 in total) is allocated for holding data. This
 * is useful is RAS where memory must be pre-allocated, allowing strings up to 511 chars + null
 * terminator to be subsequently written into these keys.
 */
static intptr_t omrstr_set_token_from_buf(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens, const char *key, char *tokenBuf, uint32_t tokenLen)
{
	J9TokenEntry entry;
	J9TokenEntry *existingEntry = NULL;

#define TOKEN_BUF_LEN 511

	entry.memLen = tokenLen;
	if (entry.memLen == 0) {
		/* pre allocate a chunk for next use if empty string */
		entry.memLen = TOKEN_BUF_LEN;
	}

	entry.key = (char *) key;
	entry.keyLen = strlen(key);
	existingEntry = hashTableFind((J9HashTable *)tokens, &entry);

	if (NULL == existingEntry) {
		if (NULL == (entry.key = (char *)portLibrary->mem_allocate_memory(portLibrary, entry.keyLen + 1, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY))) {
			return -1;
		}
		memcpy(entry.key, key, entry.keyLen + 1); /* +1 for \0 */

		if (NULL == (entry.value = (char *)portLibrary->mem_allocate_memory(portLibrary, entry.memLen + 1, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY))) {
			portLibrary->mem_free_memory(portLibrary, entry.key);
			return -1;
		}

		memcpy(entry.value, tokenBuf, tokenLen);
		entry.valueLen = tokenLen;
		entry.value[tokenLen] = '\0';

		/* Finally, let's add the token entry to the table */
		if (NULL == hashTableAdd((J9HashTable *)tokens, &entry)) {
			portLibrary->mem_free_memory(portLibrary, entry.key);
			portLibrary->mem_free_memory(portLibrary, entry.value);
			return -1;
		}
	} else {
		/* don't need to remove this entry, just update its value ! */
		if (tokenLen > existingEntry->memLen) {
			/* try growing the memory for the value */
			if (NULL == (entry.value = (char *)portLibrary->mem_allocate_memory(portLibrary, entry.memLen + 1, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY))) {
				/* failed to allocate so truncate instead */
				tokenLen = (uint32_t)existingEntry->memLen;
			} else {
				portLibrary->mem_free_memory(portLibrary, existingEntry->value);
				existingEntry->value = entry.value;
				existingEntry->memLen = entry.memLen;
			}
		}

		strncpy(existingEntry->value, tokenBuf, tokenLen);

		/* ensure the value is always null terminated */
		existingEntry->value[tokenLen] = '\0';
		existingEntry->valueLen = tokenLen;
	}

	return 0;
}

/**
 * Writes a new string into buf by substituting tokens into format, writing the result into buf.
 *
 * format is a (typically user provided) string which may contain arbitrary tokens to be
 * substituted. A token is any character sequence starting with the % character.
 *
 * When a % character is encountered, this API will compare the following characters with the
 * token provided. The longest key which matches the characters in format will be selected.
 *
 * If buf is not long enough to hold the result, the required length will be returned. buf will
 * always be NUL terminated if it is longer than 0 bytes.
 *
 * @param[in] portLib the port library
 * @param[out] buf the characater buffer to write the result into
 * @param[in] bufLen the length of the buffer, in bytes
 * @param[in] format the format string to be processed
 *
 * @return The number of characters printed not including the NUL terminator.
 *
 * @note When buf is NULL or bufLen is too short, the size of the buffer required to print to
 * the string, including the NUL terminator is returned.
 */
uintptr_t
omrstr_subst_tokens(struct OMRPortLibrary *portLibrary, char *buf, uintptr_t bufLen, const char *format, struct J9StringTokens *tokens)
{
	J9TokenEntry *entry = NULL;
	uintptr_t cnt = 0;
	const char *read = format;
	char *write = buf;

	/* First case: "read only" mode, a buffer hasn't been supplied. */
	if (NULL == buf) {
		/* Let's parse the format string looking for tokens to replace */
		while ('\0' != *read) {
			if ((*read == '%') && (NULL != (entry = consumeToken(tokens, read)))) {
				/* We found an expandable token */
				read += entry->keyLen + 1; /* +1 because of the % */
				cnt += entry->valueLen;
			} else {
				++read;
				++cnt;
			}
		}

		++cnt; /* For the \0 */
	}

	/* Second case: we're actually writing to buf. */
	else if (bufLen > 0) {
		/* Let's parse the format string looking for tokens to replace */
		while ((cnt < bufLen) && ('\0' != *read)) {
			if ((*read == '%') && (NULL != (entry = consumeToken(tokens, read)))) {
				/* We found an expandable token */
				uintptr_t maxExpandLen = bufLen - cnt;
				uintptr_t lengthToCopy = (entry->valueLen < maxExpandLen) ? entry->valueLen : maxExpandLen;

				memcpy(write, entry->value, lengthToCopy);
				write += lengthToCopy;
				read += entry->keyLen + 1; /* +1 because of the % */
				cnt += lengthToCopy;
			} else {
				*write++ = *read++;
				++cnt;
			}
		}

		/* First sub-case: the buffer was large enough, let's NULL terminate it */
		if (cnt < bufLen) {
			*write = '\0';
		}

		/* Second sub-case: the buffer was too small, let's go back and NULL terminate
		 * it then set cnt to the minimum buf size required for a successful subst call */
		else {
			buf[bufLen - 1] = '\0';

			/* Calling subst_tokens with a NULL buffer returns the min buf size for success */
			cnt = portLibrary->str_subst_tokens(portLibrary, NULL, 0, format, tokens);
		}
	}

	return cnt;
}

/**
 * Discard a token cookie.
 * If tokens is NULL, no action is taken.
 *
 * @param[in] tokens the cookie to be freed
 */
void
omrstr_free_tokens(struct OMRPortLibrary *portLibrary, struct J9StringTokens *tokens)
{
	if (NULL == tokens) {
		return;
	}

	/* We must first free memory the entries point to (the actual key and value) */
	hashTableForEachDo((J9HashTable *)tokens, tokenDoFreeFn, portLibrary);

	/* We can now free the entries and table */
	hashTableFree((J9HashTable *)tokens);
}

/**
 * Returns the UTC time that was passed in as a formatted string according to
 * the 'format' and 'flags' parameters.
 *
 * @param[in] portLibrary  The port library.
 * @param[in,out] buf A pointer to a character buffer where the resulting time string will be stored.
 * @param[in] bufLen The length of the 'buf' character buffer.
 * @param[in] format The format string, ordinary characters placed in the format string are copied to
 *                   buf without conversion.  Conversion specifiers are introduced by a '%' character,
 *                   and are replaced in buf as follows:
 * <ul>
 * <li>%a The abbreviated weekday in English.
 * <li>%b The abbreviated month name in English.
 * <li>%d The day of the month as a decimal number (range 0 to 31).
 * <li>%H The hour as a decimal number using a 24-hour clock (range 00 to 23).
 * <li>%m The month as a decimal number (range 01 to 12).
 * <li>%M The minute as a decimal number.
 * <li>%S The second as a decimal number.
 * <li>%Y The year as a decimal number including the century.
 * <li>%y The year within the century.
 * <li>%% A literal '%' character.
 * <li>all other '%' specifiers will be ignored
 * </ul>
 * @param[in] timeMillisUTC The time value to format.  The value is expressed in milliseconds since January 1, 1970 UTC.
 * @param[in] flags  Bit-wise or of OMRSTR_FTIME_FLAG_* as defined in omrport.h.  Current options include mapping to the
 *                   local time zone (or not).
 *
 * @return The number of characters placed in the array buf, not including NULL terminator.
 *
 * If buf is too small, will return the minimum buf size required.
 */
uint32_t
omrstr_ftime_ex(struct OMRPortLibrary *portLibrary, char *buf, uint32_t bufLen, const char *format, int64_t timeMillisUTC, uint32_t flags)
{
	if ((NULL != buf) && (bufLen > 0)) {
		return omrstr_subst_time(portLibrary, buf, bufLen, format, timeMillisUTC, flags);
	}
	return 0;
}

/**
 * Returns the UTC time that was passed in as a formatted string in local time.  Formatted according to the 'format' parameter.
 *
 * @param[in] portLibrary The port library.
 * @param[in,out] buf A pointer to a character buffer where the resulting time string will be stored.
 * @param[in] bufLen The length of the 'buf' character buffer.
 * @param[in] format The format string, ordinary characters placed in the format string are copied to
 *                   buf without conversion.  Conversion specifiers are introduced by a '%' character,
 *                   and are replaced in buf as follows:
 * <ul>
 * <li>%a The abbreviated weekday in English
 * <li>%b The abbreviated month name in English
 * <li>%d The day  of the month as a decimal number (range 0 to 31).
 * <li>%H The hour as a decimal number using a 24-hour clock (range 00 to 23).
 * <li>%m The month as a decimal number (range 01 to 12).
 * <li>%M The minute as a decimal number.
 * <li>%S The second as a decimal number.
 * <li>%Y The year as a decimal number including the century.
 * <li>%y The year within the century.
 * <li>%% A literal '%' character.
 * <li>All other '%' specifiers will be ignored.
 * </ul>
 * @param[in] timeMillisUTC The time value to format.  The value is expressed in milliseconds since January 1, 1970 UTC.
 *
 * @return The number of characters placed in the array buf, not including NULL terminator.
 *
 * If buf is too small, will return the minimum buf size required.
 */
uint32_t
omrstr_ftime(struct OMRPortLibrary *portLibrary, char *buf, uint32_t bufLen, const char *format, int64_t timeMillisUTC)
{
	return omrstr_ftime_ex(portLibrary, buf, bufLen, format, timeMillisUTC, OMRSTR_FTIME_FLAG_LOCAL);
}

/**
 * Format the string from the timeMillis parameter that was passed in as UTC.
 *
 * The time values include:
 *
 * 	  %Y     year    1900..????		(local time, based on millis parameter)
 *	  %y     year of century  00..99
 *	  %m     month     01..12
 *	  %a     abbreviated weekday in English
 *	  %b     abbreviated month name in English
 *	  %d     day       01..31
 *	  %H     hour      00..23
 *	  %M     minute    00..59
 *	  %S     second    00..59
 *
 *	  %tick  high res timer
 *
 * @param[in] portLibrary the port library
 * @param[out] buf the string buffer to use
 * @param[in] bufLen the buffer length
 * @param[in] format the format string
 * @param[in] timeMillis the UTC time in milliseconds to use for the date/time tokens
 * @param[in] flags bit-wise or of OMRSTR_FTIME_FLAG_* as defined in omrport.h.
 *                  Current options include mapping to the local time zone (or not).
 *
 * @return The number of characters placed in the array buf, not including NULL terminator.
 *
 * If buf is too small, will return the minimum buf size required.
 */
static uint32_t
omrstr_subst_time(struct OMRPortLibrary *portLibrary, char *buf, uint32_t bufLen, const char *format, int64_t timeMillis, uint32_t flags)
{
	int8_t haveTick = 0;
	uint32_t count = 0;
	uint64_t curTick = 0;
	const char *read = format;
	char *write = buf;
	char ticks[TICK_BUF_SIZE];
	uint32_t tickSize = 0;
	J9TimeInfo tm;
	static const char abbWeekdayName[7][4] = {
			"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char abbMonthName[12][4] = {
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	convertUTCMillisToJ9Time(timeMillis, &tm, flags);

	while (0 != *read) {
		char c = *read;
		if (c == '%') {
			char key = read[1];
			switch (key) {
			case 'Y':
				count += 4;
				if (count <= bufLen) {
					writeIntToBuffer(write, 4, 4, J9F_NO_VALUE, tm.year, J9FFLAG_ZERO, 0, digits_dec);
					write += 4;
				}
				read += 2;
				break;
			case 'y':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.year % 100, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'm':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.month, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'd':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.day, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'H':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.hour, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'M':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.minute, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'S':
				count += 2;
				if (count <= bufLen) {
					writeIntToBuffer(write, 2, 2, J9F_NO_VALUE, tm.second, J9FFLAG_ZERO, 0, digits_dec);
					write += 2;
				}
				read += 2;
				break;
			case 'a':
				count += 3;
				if (count <= bufLen) {
					writeStringToBuffer(write, 3, J9F_NO_VALUE, J9F_NO_VALUE, abbWeekdayName[tm.weekday], 0);
					write += 3;
				}
				read += 2;
				break;
			case 'b':
				count += 3;
				if (count <= bufLen) {
					writeStringToBuffer(write, 3, J9F_NO_VALUE, J9F_NO_VALUE, abbMonthName[tm.month - 1], 0);
					write += 3;
				}
				read += 2;
				break;
			case '%':
				count += 1;
				if (count <= bufLen) {
					*write++ = '%';
				}
				read += 2;
				break;
			case 't':
				if (strncmp(read + 2, "ick", 3) == 0) {
					if (!haveTick) {
						curTick = portLibrary->time_hires_clock(portLibrary);
						haveTick = 1;
					}
					tickSize = (uint32_t)writeIntToBuffer(ticks, TICK_BUF_SIZE, J9F_NO_VALUE, J9F_NO_VALUE, curTick, J9FSPEC_L, 0, digits_dec);
					count += tickSize;
					if (count <= bufLen) {
						strncpy(write, ticks, tickSize);
						write += tickSize;
					}
					read += 5;
					break;
				}
				/* fall through */
			default:
				count += 1;
				if (count <= bufLen) {
					*write++ = *read;
				}
				read += 1;
				break;
			}
		} else {
			count += 1;
			if (count <= bufLen) {
				*write++ = *read;
			}
			read += 1;
		}
	}
	if (count < bufLen) {
		*write = '\0';
	} else {
		count += 1;
	}
	return count;
}

/**
 * Retrieve the offset of local time from UTC and the current time zone name. If the current time zone could not be determined, this function will
 * not modify its outputs.
 *
 *
 * @param[in] portLibrary the port library
 * @param[out] hourOffset the hour offset from UTC+0, which may be negative
 * @param[out] minuteOffset the minute offset from UTC+0, in the range [0, 59]
 * @param[out] buf the string buffer to be filled with the time zone name
 * @param[in] bufLen the buffer length
 *
 * @return If the time zone could be determined, returns the number of bytes written to buf, not including the NULL terminator. Otherwise returns -1.
 */
int32_t
omrstr_current_time_zone(struct OMRPortLibrary *portLibrary, int64_t *hourOffset, uint32_t *minuteOffset, char *buf, uint32_t bufLen)
{
	BOOLEAN zoneAvailable = FALSE;
	int32_t zoneSecondsEast = 0;
	const char *zoneName = NULL;

#if defined(WIN32)
	/* until a reliable mechanism is found, don't report timezone */
#elif defined(J9ZOS390) /* defined(WIN32) */
	time64_t timeNow = time64(NULL);
	struct tm utc;
	struct tm local;

	if ((NULL != gmtime64_r(&timeNow, &utc)) && (NULL != localtime64_r(&timeNow, &local))) {
		zoneAvailable = TRUE;
		zoneSecondsEast = (int32_t)difftime64(timeNow, mktime64(&utc));
		if (0 == local.tm_isdst) {
			zoneName = tzname[0];
		} else if (local.tm_isdst > 0) {
			zoneName = tzname[1];
			/* compensate for DST because difftime64() doesn't appear to do so */
			zoneSecondsEast += 60 * 60;
		}
	}
#else /* defined(WIN32) */
	time_t timeNow = time(NULL);
	struct tm utc;
	struct tm local;

	if ((NULL != gmtime_r(&timeNow, &utc)) && (NULL != localtime_r(&timeNow, &local))) {
		zoneAvailable = TRUE;
		zoneSecondsEast = (int32_t)difftime(timeNow, mktime(&utc));
		if (0 == local.tm_isdst) {
			zoneName = tzname[0];
		} else if (local.tm_isdst > 0) {
			zoneName = tzname[1];
			/* compensate for DST because difftime() doesn't appear to do so */
			zoneSecondsEast += 60 * 60;
		}
	}
#endif /* defined(WIN32) */

	if (zoneAvailable) {
		uint32_t offset = ((uint32_t)((zoneSecondsEast > 0) ? zoneSecondsEast : -zoneSecondsEast)) / 60;
		int64_t hours = offset / 60;
		uint32_t minutes = offset % 60;

		if (hourOffset) {
			*hourOffset = (zoneSecondsEast > 0) ? hours : -hours;
		}
		if (minuteOffset) {
			*minuteOffset = minutes;
		}

		const char *read = zoneName;
		char *write = buf;
		uint32_t idx = 0;
		if ((NULL != write) && (NULL != read)) {
			while ((0 != *read) && (idx < bufLen - 1)) {
				*write++ = *read;
				read += 1;
				idx += 1;
			}
			*write = '\0';
		}
		return idx;
	} else {
		return -1;
	}
}

/* ===================== Internal functions ======================== */

#define CONVERSION_BUFFER_SIZE 256
#define WIDE_CHAR_SIZE 2
#define BYTE_ORDER_MARK 0xFEFF

/**
 * Two step process: convert platform encoding to wide characters, then convert wide characters to modified UTF-8.
 * iconv() is resumable if the output buffer is full, but Windows MultiByteToWideChar() is not.
 * @param[in] codePage specify the "platform" code page: CP_THREAD_ACP, CP_ACP (Windows ANSI code pages) or OS_ENCODING_CODE_PAGE. Ignored on non-Windows systems
 * @param[in] inBuffer input string to be converted
 * @param[in] inBufferSize input string size in bytes.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if inBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the required output buffer size)
 * @return number of bytes generated, or required size of output buffer if outBufferSize is 0. Negative error code on failure.
 */
static int32_t
convertPlatformToMutf8(struct OMRPortLibrary *portLibrary, uint32_t codePage, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	uint8_t onStackBuffer[CONVERSION_BUFFER_SIZE];
	uint8_t *wideBuffer = onStackBuffer;
	uintptr_t wideBufferSize = sizeof(onStackBuffer);
	uintptr_t resultSize = 0; /* amount of the output buffer used, in bytes */
	charconvState_t encodingState = NULL;
	/* create mutable copies of buffer variables */
	const uint8_t *platformCursor = inBuffer;
	uintptr_t platformRemaining = inBufferSize;
	uint8_t *mutf8Cursor = outBuffer;
	uintptr_t mutf8Limit = outBufferSize;
	BOOLEAN firstConversion = TRUE;
	/* set up buffers and convertors */
#if defined(OMR_OS_WINDOWS)
	uintptr_t requiredBufferSize = 0;
	encodingState = NULL;
	/* MultiByteToWideChar is not resumable, so we need a buffer large enough to hold the entire intermediate result */
	requiredBufferSize = WIDE_CHAR_SIZE * MultiByteToWideChar(codePage, OS_ENCODING_MB_FLAGS, (LPCSTR)inBuffer, (int) inBufferSize,
						 NULL, 0); /* get required buffer size */
	if (requiredBufferSize > CONVERSION_BUFFER_SIZE) {
		wideBuffer = portLibrary->mem_allocate_memory(portLibrary, requiredBufferSize, OMR_GET_CALLSITE(), OMRMEM_CATEGORY_PORT_LIBRARY);
		if (NULL == wideBuffer) {
			return OMRPORT_ERROR_STRING_MEM_ALLOCATE_FAILED;
		}
		wideBufferSize = requiredBufferSize;
	}

#endif /* defined(OMR_OS_WINDOWS) */

#if defined(J9STR_USE_ICONV)
	/* use the EBCDIC string for UTF-16 on z/OS */
	encodingState = iconv_get(portLibrary, OMRPORT_LANG_TO_UTF16_ICONV_DESCRIPTOR, utf16, nl_langinfo(CODESET));
	if (J9VM_INVALID_ICONV_DESCRIPTOR == encodingState) {
		/* wideBuffer is always on stack - no need to free it */
		return OMRPORT_ERROR_STRING_ICONV_OPEN_FAILED;
	}
#endif
	/* do the conversion. outBufferSize==0 indicates that only the buffer size is required. */
	while ((platformRemaining > 0) && ((0 == outBufferSize) || (mutf8Limit > 0))) {
		int32_t wideBufferPartialSize = convertPlatformToWide(portLibrary, encodingState, codePage, &platformCursor, &platformRemaining, wideBuffer, wideBufferSize);
		const uint8_t *tempWideBuffer = wideBuffer; /* need a mutable copy */
		uintptr_t mutf8PartialSize = 0;
		uintptr_t tempWideBufferSize = wideBufferPartialSize;

		if (wideBufferPartialSize < 0) {
#if defined(OMR_OS_WINDOWS)
			/* the working buffer is dynamically allocated only on Windows */
			if (wideBuffer != onStackBuffer) {
				portLibrary->mem_free_memory(portLibrary, wideBuffer);
			}
#endif /* defined(OMR_OS_WINDOWS) */
#if defined(J9STR_USE_ICONV)
			iconv_free(portLibrary, OMRPORT_LANG_TO_UTF16_ICONV_DESCRIPTOR, encodingState);
#endif
			return wideBufferPartialSize; /* unrecoverable error */
		}
		if (firstConversion) {
			uint16_t firstChar = *((uint16_t *) wideBuffer);
			firstConversion = FALSE;
			if (BYTE_ORDER_MARK == firstChar) { /* ignore it */
				tempWideBuffer += 2;
				tempWideBufferSize -= 2;
			}
		}
		/* updates platformCursor to character after the last translated character */
		if (wideBufferPartialSize < 0) {
#if defined(OMR_OS_WINDOWS)
			/* the working buffer is dynamically allocated only on Windows */
			if (wideBuffer != onStackBuffer) {
				portLibrary->mem_free_memory(portLibrary, wideBuffer);
			}
#endif /* defined(OMR_OS_WINDOWS) */
#if defined(J9STR_USE_ICONV)
			iconv_free(portLibrary, OMRPORT_LANG_TO_UTF16_ICONV_DESCRIPTOR, encodingState);
#endif
			return wideBufferPartialSize; /* error occurred, e.g. bad code */
		} /* if (wideBufferPartialSize < 0) */
		/* Now convert the result to modified UTF-8 */
		mutf8PartialSize = convertWideToMutf8(&tempWideBuffer, &tempWideBufferSize, mutf8Cursor, mutf8Limit);
		if (0 != tempWideBufferSize) { /* should have consumed all the data */
#if defined(OMR_OS_WINDOWS)
			if (wideBuffer != onStackBuffer) {
				portLibrary->mem_free_memory(portLibrary, wideBuffer);
			}
#endif /* defined(OMR_OS_WINDOWS) */
#if defined(J9STR_USE_ICONV)
			iconv_free(portLibrary, OMRPORT_LANG_TO_UTF16_ICONV_DESCRIPTOR, encodingState);
#endif
			return OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
		} else {
			resultSize += mutf8PartialSize;
			if (0 != outBufferSize) {
				mutf8Cursor += mutf8PartialSize;
				mutf8Limit -= mutf8PartialSize;
			}
		}
	}
#if defined(OMR_OS_WINDOWS)
	if (wideBuffer != onStackBuffer) {
		portLibrary->mem_free_memory(portLibrary, wideBuffer);
	}
#endif /* defined(OMR_OS_WINDOWS) */
#if defined(J9STR_USE_ICONV)
	iconv_free(portLibrary, OMRPORT_LANG_TO_UTF16_ICONV_DESCRIPTOR, encodingState);
#endif
	/* convertWideToMutf8 does null-termination if possible */
	return (int32_t) resultSize;
}

/**
 * Two step process: convert modified UTF-8 to wide characters, then wide characters to platform encoding.
 * iconv() both is resumable if the output buffer is full, but Windows WideCharToMultiByte() is not.
 * @param [in] portLibrary port library
 * @param [in] codePage specify the "platform" code page: CP_THREAD_ACP, CP_ACP (Windows ANSI code pages) or OS_ENCODING_CODE_PAGE. Ignored on non-Windows systems
 * @param [in] inBuffer input string to be converted
 * @param [in] inBufferSize input string size in bytes
 * @param [in] outBuffer user-allocated output buffer that stores converted characters, ignored if inBufferSize is 0
 * @param [in] outBufferSize output buffer size in bytes (zero to request the required output buffer size)
 */
static int32_t
convertMutf8ToPlatform(struct OMRPortLibrary *portLibrary, uint32_t codePage, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	/* need mutable copies */
	const uint8_t *mutf8Cursor = inBuffer;
	uintptr_t mutf8Remaining = inBufferSize;
	uint8_t *outCursor = outBuffer;
	uintptr_t outLimit = outBufferSize;
	int32_t resultSize = 0;
	charconvState_t encodingState = NULL;

	/* set up convertor state.  Note: no convertor state is required on Windows */
#if defined(J9STR_USE_ICONV)
	encodingState = iconv_get(portLibrary, OMRPORT_UTF16_TO_LANG_ICONV_DESCRIPTOR, nl_langinfo(CODESET), utf16);
	if (J9VM_INVALID_ICONV_DESCRIPTOR == encodingState) {
		/* wideBuffer is always on stack - no need to free it */
		return OMRPORT_ERROR_STRING_ICONV_OPEN_FAILED;
	}
#elif defined(OMR_OS_WINDOWS)
	encodingState = NULL;
#endif /* defined(OMR_OS_WINDOWS) */
	while (mutf8Remaining > 0) { /* translated section by section as dictated by the size of wideBuffer */
		uint8_t wideBuffer[CONVERSION_BUFFER_SIZE];
		const uint8_t *wideBufferCursor = wideBuffer;
		uintptr_t wideBufferSize = sizeof(wideBuffer);
		uintptr_t wideBufferCount = 0;
		int32_t platformPartCount = 0;

		int32_t result = convertMutf8ToWide(&mutf8Cursor, &mutf8Remaining, wideBuffer, wideBufferSize);
		if (result < 0) { /* conversion error */
			return result;
		}
		wideBufferCount = (uintptr_t)result;

		platformPartCount = convertWideToPlatform(portLibrary, encodingState, codePage, &wideBufferCursor, &wideBufferCount, outCursor, outLimit);
		/*
		 * convertWideToPlatform may return OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL.
		 * convertWideToPlatform sets up to MAX_STRING_TERMINATOR_LENGTH bytes bytes after the converted characters to 0,
		 * depending on the length of the buffer
		 */
		if (platformPartCount < 0) { /* conversion error or output buffer too small */
#if defined(J9STR_USE_ICONV)
			iconv_free(portLibrary, OMRPORT_UTF16_TO_LANG_ICONV_DESCRIPTOR, encodingState);
#endif
			return platformPartCount;
		} else {
			resultSize += platformPartCount;
			if (0 != outLimit) {
				outCursor += platformPartCount;
				outLimit -= platformPartCount;
			}
		}
	} /* while */
#if defined(J9STR_USE_ICONV)
	iconv_free(portLibrary, OMRPORT_UTF16_TO_LANG_ICONV_DESCRIPTOR, encodingState);
#endif
	return resultSize;
}

/**
 * convert platform encoding to UTF-8.
 * @param[in] inBuffer      input string  to be converted.
 * @param[in] inBufferSize  input string size in bytes.
 * @param[in] outBuffer     user-allocated output buffer that stores converted characters, ignored if inBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the required output buffer size)
 */
static int32_t
convertPlatformToUtf8(struct OMRPortLibrary *portLibrary, const uint8_t *inBuffer, uintptr_t inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	int32_t resultSize = 0;
#if defined(J9STR_USE_ICONV)
	/* J9STR_USE_ICONV is defined on LINUX, AIXPPC and J9ZOS390 at the beginning of the file */
	char* inbuf = (char*)inBuffer;
	char* outbuf = (char*)outBuffer;
	size_t inbytesleft = inBufferSize;
	size_t outbytesleft = outBufferSize - 1; /* space for null-terminator */
	charconvState_t converter = iconv_get(portLibrary, OMRPORT_LANG_TO_UTF8_ICONV_DESCRIPTOR, utf8, nl_langinfo(CODESET));

	if (J9VM_INVALID_ICONV_DESCRIPTOR == converter) {
		/* no converter available for this code set. Just dump the platform chars */
		strncpy(outbuf, inbuf, outBufferSize);
		outbuf[outBufferSize - 1] = '\0';
		return OMRPORT_ERROR_STRING_ICONV_OPEN_FAILED;
	}

	while ((outbytesleft > 0) && (inbytesleft > 0)) {
		if ((size_t)-1 == iconv(converter, &inbuf, &inbytesleft, &outbuf, &outbytesleft)) {
			if (errno == E2BIG) {
				iconv_free(portLibrary, OMRPORT_LANG_TO_UTF8_ICONV_DESCRIPTOR, converter);
				return OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			}

			/* if we couldn't translate this character, copy one byte verbatim */
			*outbuf = *inbuf;
			outbuf++;
			inbuf++;
			inbytesleft--;
			outbytesleft--;
		}
	}

	iconv_free(portLibrary, OMRPORT_LANG_TO_UTF8_ICONV_DESCRIPTOR, converter);
	*outbuf = '\0';

	/* outbytesleft started at (outBufferSize -1).
	 * To find how much we wrote, subtract outbytesleft, then add 1 for the null terminator */
	resultSize = outBufferSize - 1 - outbytesleft + 1;
#endif /* defined(J9STR_USE_ICONV) */
	/* Do nothing on Windows as OS_ENCODING_CODE_PAGE is UTF-8 on Windows */
	return resultSize;
}

/**
 * Convert wide char (UTF-16) encoding to modified UTF-8.
 * May terminate early if the output buffer is too small.
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * Resumable, so it updates its inputs
 * @param[in,out]  inBuffer input string  to be converted.   Updated to first untranslated character.
 * @param[in,out]  inBufferSize input string size in bytes.  Updated to number of untranslated characters. Must be even.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the output buffer size).
 * @return number of bytes generated, or required size of output buffer if outBufferSize is 0.
 * @note callers are responsible for detecting buffer overflow.
 */
static int32_t
convertWideToMutf8(const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	uintptr_t wideRemaining = *inBufferSize; /* number of untranslated characters in inBuffer */
	const uint8_t *wideCursor = *inBuffer;
	int32_t resultSize = 0;

	Assert_PRT_true(0 == (wideRemaining % 2));
	if (0 == outBufferSize) { /* we just want the length */
		while (wideRemaining > 0) {
			uint32_t encodeResult = 0;
			uint16_t wideChar = *((uint16_t *) wideCursor);
			encodeResult = encodeUTF8CharN(wideChar, NULL, 3); /* convert one wide character, get the length, ignore the converted result */
			if (0 == encodeResult) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			resultSize += encodeResult;
			wideCursor += 2;
			wideRemaining -= 2;
		}
	} else {
		uintptr_t outputLimit = outBufferSize;
		uint8_t *outputCursor = outBuffer;
		while ((wideRemaining > 0) && (outputLimit > 0)) {
			uint16_t wideChar = *((uint16_t *) wideCursor);
			uint32_t encodeResult = encodeUTF8CharN(wideChar, outputCursor, 3);
			if (0 == encodeResult) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			resultSize += encodeResult;
			outputLimit -= encodeResult;
			outputCursor += encodeResult;
			wideCursor += 2;
			wideRemaining -= 2;
		}
		if (outputLimit > 0) {
			*outputCursor = '\0'; /* null terminate if possible */
		}
	}
	*inBufferSize = wideRemaining; /* update caller's arguments */
	*inBuffer = (uint8_t *) wideCursor;
	if ((outBufferSize > 0) && ((uintptr_t) resultSize < outBufferSize)) {
		outBuffer[resultSize] = 0; /* null terminate if possible */
	}
	return resultSize;
}

/**
 * Convert ISO Latin-1 (8859-1) encoding to modified UTF-8.
 * May terminate early if the output buffer is too small.
 * Resumable, so it updates its inputs.
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * @param[in,out]  inBuffer input string  to be converted.   Updated to first untranslated character.
 * @param[in,out]  inBufferSize input string size in bytes.  Updated to number of untranslated characters.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the output buffer size).
 * @return number of bytes generated, or required size of output buffer if outBufferSize is 0.
 * @note callers are responsible for detecting buffer overflow.
 */
static int32_t
convertLatin1ToMutf8(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	const uint8_t *latinString = *inBuffer;
	uintptr_t latinRemaining = *inBufferSize; /* number of untranslated characters in inBuffer */
	uint8_t *mutf8Cursor = outBuffer; /* Mutable copy */
	uintptr_t mutf8Remaining = outBufferSize; /* number of unused bytes in outBuffer */
	int32_t resultSize = 0;

	while ((latinRemaining > 0)
		&& ((mutf8Remaining > 0) || (0 == outBufferSize)) /* outBufferSize == 0 indicates we just want the length */
		&& (resultSize >= 0)
	) {
		/* still have input data, output space, and no errors */
		/* convert in CONVERSION_BUFFER_SIZE-size segments */
		uintptr_t wideLimit = (latinRemaining > CONVERSION_BUFFER_SIZE) ? CONVERSION_BUFFER_SIZE : latinRemaining;
		uintptr_t latinBytesConsumed = 0;
		uint16_t localWideBuffer[CONVERSION_BUFFER_SIZE]; /* handle short strings without allocating memory */
		uint16_t *wideBuffer = localWideBuffer;
		uintptr_t wideLength = wideLimit * 2;
		uintptr_t wideRemaining = wideLength;
		uintptr_t cursor = 0;
		int32_t mutf8Result = 0;

		/* copy the characters to an array of Unicode characters and transliterate from Unicode to modified UTF-8 */
		for (cursor = 0; cursor < wideLimit; ++cursor) {
			wideBuffer[cursor] = latinString[cursor] & 0x00ff;
		}

		mutf8Result = convertWideToMutf8((const uint8_t **) &wideBuffer, &wideRemaining, mutf8Cursor, mutf8Remaining);
		latinBytesConsumed = (wideLength - wideRemaining) / 2; /* 1 Latin byte per 2 wide characters */
		latinRemaining -= latinBytesConsumed;
		latinString += latinBytesConsumed; /* Now points to first unconsumed character */
		if (mutf8Result < 0) { /* error */
			resultSize = mutf8Result;
		} else {
			resultSize += mutf8Result;
			if (outBufferSize != 0) {
				mutf8Cursor += mutf8Result;
				mutf8Remaining -= mutf8Result;
			}
		}
	}
	if (resultSize >= 0) { /* no error */
		*inBuffer = latinString;
		*inBufferSize = latinRemaining; /* update caller's arguments */
		/* convertWideToMutf8 null terminated the string, if possible */
	}
	return resultSize;
}

/**
 * Convert modified UTF-8 encoding to ISO Latin-1 (8859-1)
 * May terminate early if the output buffer is too small.
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * Resumable, so it updates its inputs
 * @param[in,out]  inBuffer input string  to be converted. Updated to first untranslated character.
 * @param[in,out]  inBufferSize input string size in bytes. Updated to number of untranslated characters.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the output buffer size).
 * @return number of bytes generated, or required size of output buffer if outBufferSize is 0.
 * @note callers are responsible for detecting buffer overflow.
 */
static int32_t
convertMutf8ToLatin1(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	const uint8_t *mutf8String = *inBuffer;
	uintptr_t mutf8Remaining = *inBufferSize; /* number of untranslated characters in inBuffer */
	uint8_t *latinString = outBuffer; /* mutable copy */
	uintptr_t latinRemaining = outBufferSize; /* number of unused bytes in outBuffer */
	int32_t resultSize = 0;

	while ((mutf8Remaining > 0)
		&& ((latinRemaining > 0) || (0 == outBufferSize)) /* outBufferSize == 0 indicates we just want the length */
		&& (resultSize >= 0)
	) {
		/* still have input data, output space, and no errors */
		/* convert in CONVERSION_BUFFER_SIZE-size segments */
		uintptr_t wideLimit = (mutf8Remaining > CONVERSION_BUFFER_SIZE) ? CONVERSION_BUFFER_SIZE : mutf8Remaining;
		uint16_t localWideBuffer[CONVERSION_BUFFER_SIZE]; /* handle short strings without allocating memory */
		uint16_t *wideBuffer = localWideBuffer;
		uintptr_t wideLength = wideLimit * 2; /* wide conversion byte length */
		uintptr_t wideRemaining = wideLength;
		uintptr_t mutf8BytesConsumed = 0;

		/* convert from mutf8 to unicode */
		int32_t unicodeResultBytes = convertMutf8ToWide(&mutf8String, &mutf8Remaining, (uint8_t *)wideBuffer, wideRemaining);
		mutf8BytesConsumed = (wideLength - wideRemaining) / 2; /* 1 mutf8 byte per 2 wide characters */
		mutf8Remaining -= mutf8BytesConsumed;
		mutf8String += mutf8BytesConsumed; /* Now points to first unconsumed character */
		if (unicodeResultBytes < 0) {
			/* conversion error */
			resultSize = unicodeResultBytes;
		} else {
			int32_t latin1CharactersConverted = (unicodeResultBytes / 2); /* Latin1 characters take up only 1 of 2 wide bytes */
			resultSize += latin1CharactersConverted;
			if (outBufferSize != 0) {
				int32_t cursor = 0;
				/* copy Latin 1 characters from 16 bytes to 8 bytes */
				for (cursor = 0; cursor < latin1CharactersConverted; ++cursor) {
					latinString[cursor] = wideBuffer[cursor] & 0xff;
				}
				latinRemaining -= latin1CharactersConverted;
				latinString += latin1CharactersConverted;
			}

		}
	}
	if (resultSize >= 0) { /* no error */
		*inBuffer = mutf8String;
		*inBufferSize = mutf8Remaining; /* update caller's arguments */
		/* convertMutf8ToWide null terminated the string, if possible */
	}
	return resultSize;
}

/**
 * Test if the second and later bytes of a UTF-8 character are well-formed, i.e. 2 MSbs are 10
 * If they are and mutf8Bytes is not null, copy the bytes.
 * @param utf8Bytes pointer to the first byte of a UTF-8 character
 * @param numBytes number of bytes to check
 * @param mutf8Bytes pointer to the destination for the modified UTF-8 character
 * @return true if the bytes are well-formed
 */
static BOOLEAN
checkAndCopyUtfBytes(const uint8_t *utf8Bytes, uintptr_t numBytes, uint8_t *mutf8Bytes)
{
	uintptr_t cursor = 0;

	if (NULL != mutf8Bytes) {
		mutf8Bytes[0] = utf8Bytes[0];
	}
	for (cursor = 1; cursor < numBytes; ++cursor) {
		if ((0xC0 & utf8Bytes[cursor]) != 0x80) {
			return FALSE;
		} else if (NULL != mutf8Bytes) {
			mutf8Bytes[cursor] = utf8Bytes[cursor];
		}
	}
	return TRUE;
}

/**
 * Convert Unicode UTF8 (including supplementary characters) encoding to modified UTF-8.
 * May terminate early if the output buffer is too small.
 * Resumable, so it updates its inputs.
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * @param[in,out]  inBuffer input string  to be converted.   Updated to first untranslated character.
 * @param[in,out]  inBufferSize input string size in bytes.  Updated to number of untranslated characters.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the output buffer size).
 * @return number of bytes generated, or required size of output buffer if outBufferSize is 0, not including any final zero byte.
 * @note invalid characters are replaced with the Unicode code point U+FFFD	(ef bf bd)	(REPLACEMENT CHARACTER)
 * @note callers are responsible for detecting buffer overflow
 */
static int32_t
convertUtf8ToMutf8(struct OMRPortLibrary *portLibrary, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	BOOLEAN lengthOnly = (0 == outBufferSize);
	const uint8_t *utf8Buffer = *inBuffer;
	uint8_t *mutf8Buffer = outBuffer;
	uintptr_t utf8BufferSize = *inBufferSize;
	uintptr_t mutf8BufferSize = outBufferSize;
	int32_t producedTotal = 0;

	while ((utf8BufferSize > 0) && (lengthOnly || (mutf8BufferSize > 0))) {
		int32_t consumed = 0;
		int32_t produced = 0;
		if ((0 == utf8Buffer[0]) && (lengthOnly || (mutf8BufferSize > 1))) { /* null - convert to double-byte form */
			if (!lengthOnly) {
				mutf8Buffer[0] = 0xc0;
				mutf8Buffer[1] = 0x80;
			}
			consumed = 1;
			produced = 2;
		} else if (utf8Buffer[0] < 0x80) { /* Single byte UTF-8 */
			consumed = 1;
			produced = 1;
			if (!lengthOnly) {
				mutf8Buffer[0] = utf8Buffer[0];
			}
		} else if (((utf8Buffer[0] & 0xE0) == 0xC0) && (utf8BufferSize > 1) && (lengthOnly || (mutf8BufferSize > 1))) { /* Double byte UTF-8 */
			if (!checkAndCopyUtfBytes(utf8Buffer, 2, mutf8Buffer)) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			consumed = 2;
			produced = 2;
		} else if (((utf8Buffer[0] & 0xF0) == 0xE0) && (utf8BufferSize > 2) && (lengthOnly || (mutf8BufferSize > 2))) { /* Triple byte UTF-8 */
			if (!checkAndCopyUtfBytes(utf8Buffer, 3, mutf8Buffer)) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			consumed = 3;
			produced = 3;
		} else if (((utf8Buffer[0] & 0xF8) == 0xF0) && (utf8BufferSize > 3) && (lengthOnly || (mutf8BufferSize > 5))) { /* Quadruple byte UTF-8 */
			uint32_t unicodeValue = 0;
			if (!checkAndCopyUtfBytes(utf8Buffer, 4, NULL)) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}

			unicodeValue |= ((utf8Buffer[0] & 0x7) << 18);
			unicodeValue |= ((utf8Buffer[1] & 0x3F) << 12);
			unicodeValue |= ((utf8Buffer[2] & 0x3F) << 6);
			unicodeValue |= (utf8Buffer[3] & 0x3F);
			if (!lengthOnly) {
				mutf8Buffer[0] = 0xED;
				mutf8Buffer[1] = 0xA0 + (((unicodeValue >> 16) - 1) & 0x0f); /* (bits 20-16) - 1 */
				mutf8Buffer[2] = 0x80 | ((unicodeValue & 0x0FC00) >> 10) ; /* bits 15-10 */
				mutf8Buffer[3] = 0xED;
				mutf8Buffer[4] = 0xB0 | ((unicodeValue & 0x03f0) >> 6) ; /* bits 9-6 */
				mutf8Buffer[5] = 0x80 | (unicodeValue & 0x03f) ; /* bits 5-0 */
			}
			consumed = 4;
			produced = 6;
		} else {
			if (!lengthOnly) {
				mutf8Buffer[0] = 0xef;
				mutf8Buffer[1] = 0xbf;
				mutf8Buffer[2] = 0xbd;
			}
			consumed = 1;
			produced = 3;
		}

		if (!lengthOnly) {
			mutf8Buffer += produced;
			mutf8BufferSize -= produced;
		}
		utf8Buffer += consumed;
		utf8BufferSize -= consumed;
		producedTotal += produced;
	}
	if (mutf8BufferSize > 0) {
		*mutf8Buffer = 0; /* tack on a terminating null if possible */
	}
	*inBufferSize = utf8BufferSize;
	*inBuffer = utf8Buffer;
	return producedTotal;
}

/**
 * Convert modified UTF-8 encoding to wide char (UTF-16).
 * Resumable, so it updates its inputs
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * @param[in,out] inBuffer input string to be converted.  Updated to first untranslated character.
 * @param[in,out] inBufferSize  input string size in bytes. Updated to number of untranslated characters.
 * @param[in] outBuffer user-allocated output buffer that stores converted characters, ignored if outBufferSize is 0.
 * @param[in] outBufferSize output buffer size in bytes (zero to request the output buffer size).
 * @return number of bytes generated, or required size in bytes of output buffer if outBufferSize is 0. Negative error code on failure.
 * @note callers are responsible for detecting buffer overflow
 */
static int32_t
convertMutf8ToWide(const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	uintptr_t mutf8Remaining = *inBufferSize; /* number of untranslated bytes in inBuffer */
	const uint8_t *mutf8Cursor = *inBuffer;
	int32_t resultSize = 0;
	if (0 == outBufferSize) { /* we just want the length */
		uint16_t temp = 0;
		while (mutf8Remaining > 0) {
			uint32_t bytesConsumed = decodeUTF8CharN(mutf8Cursor, &temp, mutf8Remaining);
			if (0 == bytesConsumed) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			mutf8Cursor += bytesConsumed;
			mutf8Remaining -= bytesConsumed;
			resultSize += 2;
		} /* while */
	} else {
		uint16_t *wideBufferCursor = (uint16_t *)outBuffer;
		uintptr_t wideBufferLimit = outBufferSize; /* size in bytes */
		while ((wideBufferLimit > 0) && (mutf8Remaining > 0)) {
			uint32_t bytesConsumed = decodeUTF8CharN(mutf8Cursor, wideBufferCursor, mutf8Remaining);
			if (0 == bytesConsumed) {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
			mutf8Remaining -= bytesConsumed;
			mutf8Cursor += bytesConsumed;
			wideBufferCursor += 1;
			wideBufferLimit -= 2;
			resultSize += 2;
		} /* while */
		if (wideBufferLimit > 1) {
			*wideBufferCursor = 0; /* null terminate if possible */
		}
	} /* if */
	*inBuffer = mutf8Cursor; /* update caller's arguments */
	*inBufferSize = mutf8Remaining;
	if ((outBufferSize > 0) && ((outBufferSize - resultSize) >= 2)) {
		uint16_t *terminator = (uint16_t *) &outBuffer[resultSize];
		*terminator = 0;
	}
	return resultSize;
}

/**
 * transliterate from platform encoding to wide (UTF-16).  The input buffer pointer and size are updated (except on Windows) to allow
 * resumption if the translation failed due to output buffer too small.  The output buffer may not be null.
 * The output buffer may contain additional data such as a byte order mark (0xff, 0xfe or vice versa)
 * Resumable, so it updates its inputs
 * @param [in] portLibrary port library
 * @param [in] encodingState iconv_t on on Linux, z/OS and AIX, void * on Windows (unused)
 * @param[in] codePage specify the "platform" code page: CI_ACP (Windows ANSI code page) or OS_ENCODING_CODE_PAGE.  Ignored on non-Windows systems
 * @param [in,out] inBuffer point to start of buffer containing platform-encoded string.  Updated to point to start of first untranslated character
 * @param [in,out] inBufferSize size in characters of the platform-encoded string.  Updated to indicate number of remaining untranslated characters.
 * @param [in] outBuffer output buffer.
 * @param [in] outBufferSize size in bytes of wideBuffer
 * @return number of bytes generated, or negative in case of error
 * @note callers are responsible for detecting buffer overflow
 */
static int32_t
convertPlatformToWide(struct OMRPortLibrary *portLibrary, charconvState_t encodingState, uint32_t codePage, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	int32_t resultSize = -1;
#if defined(OMR_OS_WINDOWS)
	int32_t mbChars = (int32_t)MultiByteToWideChar(codePage, OS_ENCODING_MB_FLAGS, (LPCSTR)*inBuffer, (int)*inBufferSize, (LPWSTR)outBuffer, (int)outBufferSize);
	if ((outBufferSize > 0) && (0 == mbChars)) {
		resultSize = OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL; /* should not happen: caller should have allocated a sufficiently large buffer */
	} else {
		resultSize = WIDE_CHAR_SIZE * mbChars; /* number of bytes written */
		*inBufferSize = 0;
	}
#elif defined(J9STR_USE_ICONV)
	uint8_t *wideBufferCursor = outBuffer; /* make mutable copies */
	uintptr_t wideBufferLimit = outBufferSize;

	if ((size_t)-1 == iconv(encodingState, (char **)inBuffer, (size_t *)inBufferSize, (char **)&wideBufferCursor, (size_t *)&wideBufferLimit)) { /* iconv updates its inputs */
		if (EILSEQ == errno) {
			*((uint16_t *)wideBufferCursor) = UNICODE_REPLACEMENT_CHARACTER;
			*inBuffer += 1; /* Skip the invalid character */
			*inBufferSize -= 1;
			wideBufferLimit -= 2;
			resultSize = (outBufferSize - wideBufferLimit); /* number of bytes written */
		} else if (E2BIG == errno) {
			resultSize = (outBufferSize - wideBufferLimit); /* number of bytes written */
		} else {
			resultSize = OMRPORT_ERROR_STRING_ILLEGAL_STRING;
		}
	} else {
		resultSize = (outBufferSize - wideBufferLimit); /* number of bytes written */
	}
#endif /* defined(OMR_OS_WINDOWS) */
	if ((outBufferSize >= sizeof(uint16_t)) && (resultSize >= 0) && ((uintptr_t)resultSize <= (outBufferSize - sizeof(uint16_t)))) {
		uint16_t *terminator = (uint16_t *) &outBuffer[resultSize];
		*terminator = 0;
	}
	return resultSize;
}

/**
 * Transliterate from wide (UTF-16) to platform encoding. The input and output buffer pointers and sizes are updated (except on Windows) to allow
 * resumption if the translation failed due to output buffer too small.
 * Resumable, so it updates its inputs.
 * The output buffer may contain additional data such as a byte order mark (0xff, 0xfe or vice versa).
 * If the buffer is large enough, the byte after the last output character is set to 0.
 * @param[in] portLibrary port library
 * @param[in] encodingState iconv_t on Linux, z/OS and AIX, void * on Windows (unused)
 * @param[in] codePage specify the "platform" code page: CP_THREAD_ACP, CP_ACP (Windows ANSI code pages) or OS_ENCODING_CODE_PAGE. Ignored on non-Windows systems
 * @param[in,out] inBuffer input buffer. Updated to point to start of first untranslated character
 * @param[in,out] inBufferSize number of wide characters in wideBuffer. Updated to number of untranslated characters
 * @param[in] outBuffer point to start of buffer to receive platform-encoded string
 * @param[in] outBufferSize size in characters of the platformBuffer. Set to 0 to get the length of bytes required to hold the output
 * @return number of characters generated, or negative in case of error
 */
static int32_t
convertWideToPlatform(struct OMRPortLibrary *portLibrary, charconvState_t encodingState, uint32_t codePage, const uint8_t **inBuffer, uintptr_t *inBufferSize, uint8_t *outBuffer, uintptr_t outBufferSize)
{
	int32_t resultSize = -1;
#if defined(J9STR_USE_ICONV)
	uint8_t *platformCursor = (0 == outBufferSize) ? NULL : outBuffer;
	uintptr_t platformLimit = outBufferSize;
	uintptr_t wideRemaining = *inBufferSize;

	if (0 == outBufferSize) { /* get the required buffer size */
		char onStackBuffer[CONVERSION_BUFFER_SIZE];

		resultSize = 0;
		/* initialize convertor */
		while (wideRemaining > 0) {
			uintptr_t oldOsbSize = 0;
			uintptr_t iconvResult = 0;
			uintptr_t osbSize = sizeof(onStackBuffer);
			char *osbCursor = onStackBuffer;
			oldOsbSize = osbSize;
			iconvResult = iconv(encodingState, (char **)inBuffer, (size_t *) &wideRemaining, &osbCursor, (size_t *) &osbSize); /* input and output pointers and limits are updated */
			resultSize += oldOsbSize - osbSize;
			if (((size_t)-1 == iconvResult) && (E2BIG != errno)) {
				resultSize = OMRPORT_ERROR_STRING_ILLEGAL_STRING;
				break;
			}
		}
	} else {
		uintptr_t oldPlatformLimit = platformLimit;
		/* initialize convertor */
		uintptr_t iconvResult = iconv(encodingState, NULL, 0, (char **)&platformCursor, (size_t *)&platformLimit);
		iconvResult = iconv(encodingState, (char **)inBuffer, (size_t *)inBufferSize, (char **)&platformCursor, (size_t *)&platformLimit); /* input and output pointers and limits are updated */
		resultSize = oldPlatformLimit - platformLimit;
		if ((size_t)-1 == iconvResult) {
			if (E2BIG == errno) {
				return OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
			} else {
				return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
			}
		} /* if result < 0 */
		*inBuffer = platformCursor;
		*inBufferSize = platformLimit;
	}
#elif defined(OMR_OS_WINDOWS)
	LPCWSTR wideCursor = (LPCWSTR)*inBuffer;
	uintptr_t wideRemaining = *inBufferSize / WIDE_CHAR_SIZE;
	resultSize = WideCharToMultiByte(codePage, OS_ENCODING_MB_FLAGS, wideCursor,
									 (int)wideRemaining, (LPSTR)outBuffer, (int)outBufferSize, NULL, NULL);
	if (0 == resultSize) {
		DWORD error = GetLastError();
		if (ERROR_INSUFFICIENT_BUFFER == error) {
			return OMRPORT_ERROR_STRING_BUFFER_TOO_SMALL;
		} else {
			return OMRPORT_ERROR_STRING_ILLEGAL_STRING;
		}
	} else { /* 0 != result */
		*inBuffer = *inBuffer + *inBufferSize; /* advance the input pointer */
		*inBufferSize = 0;
	}
#endif
	if ((resultSize >= 0) && (outBufferSize > 0) && (((uintptr_t)resultSize) < outBufferSize)) {
		/* no error and we are doing an actual conversion and there is space */
		uintptr_t terminatorLength = outBufferSize - resultSize;
		if (terminatorLength > MAX_STRING_TERMINATOR_LENGTH) {
			terminatorLength = MAX_STRING_TERMINATOR_LENGTH;
		}
		memset(outBuffer + resultSize, 0, terminatorLength); /* null-terminate if possible */
	}

	return resultSize;
}

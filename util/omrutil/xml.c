/*******************************************************************************
 * Copyright IBM Corp. and others 1991
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

#include <string.h>

#include "omrcfg.h"
#include "omrport.h"
#include "omrutil.h"

uintptr_t
escapeXMLString(OMRPortLibrary *portLibrary, char *outBuf, uintptr_t outBufLen, const char *string, uintptr_t stringLen)
{
	uintptr_t stringPos = 0;

	if (0 != outBufLen) {
		uintptr_t outBufPos = 0;

		for (stringPos = 0; stringPos < stringLen; ++stringPos) {
			const char *xml = &string[stringPos];
			uintptr_t xmlLength = 1;
			const uint8_t ch = (uint8_t)*xml;

			switch (ch) {
			case '<':
				xml = "&lt;";
				xmlLength = 4;
				break;
			case '>':
				xml = "&gt;";
				xmlLength = 4;
				break;
			case '&':
				xml = "&amp;";
				xmlLength = 5;
				break;
			case '\'':
				xml = "&apos;";
				xmlLength = 6;
				break;
			case '\"':
				xml = "&quot;";
				xmlLength = 6;
				break;
			case 0x09:
				xml = "&#9;";
				xmlLength = 4;
				break;
			case 0x0A:
				xml = "&#xA;";
				xmlLength = 5;
				break;
			case 0x0D:
				xml = "&#xD;";
				xmlLength = 5;
				break;
			default:
				if (ch < 0x20) {
					/* use Unicode replacement for characters
					 * which are not legal in XML version 1.0
					 */
					xml = "&#xFFFD;";
					xmlLength = 8;
				}
				break;
			}

			if (xmlLength >= (outBufLen - outBufPos)) {
				/* there's not enough space in the output buffer */
				break;
			}

			if (1 == xmlLength) {
				outBuf[outBufPos] = *xml;
			} else {
				memcpy(&outBuf[outBufPos], xml, xmlLength);
			}
			outBufPos += xmlLength;
		}

		/* null terminate */
		outBuf[outBufPos] = '\0';
	}

	return stringPos;
}

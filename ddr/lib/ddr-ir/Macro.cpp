/*******************************************************************************
 * Copyright (c) 2015, 2018 IBM Corp. and others
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

#include "ddr/ir/Macro.hpp"

#include <stdlib.h>

#if defined(_MSC_VER)
/* Older Visual Studio compilers don't provide strtoll(). */
#define strtoll(str, end, base) _strtoi64((str), (end), (base))
#endif

using std::string;

DDR_RC
Macro::getNumeric(long long *ret) const
{
	DDR_RC rc = DDR_RC_ERROR;
	const string &value = getValue();
	const char * const whitespace = "\t ";

	for (size_t start = 0, end = value.length() - 1; start <= end;) {
		/* trim leading whitespace */
		start = value.find_first_not_of(whitespace, start);
		if (string::npos == start) {
			break;
		}

		/* trim trailing whitespace */
		end = value.find_last_not_of(whitespace, end);

		if (('(' == value[start]) && (')' == value[end])) {
			/* discard a pair of balanced parentheses and continue */
			++start;
			--end;
		} else {
			string trimmed = value.substr(start, end - start + 1);
			const char * c_trimmed = trimmed.c_str();
			char * end = NULL;
			/* strtoll parses prefixes like '0x' when the final argument is 0 */
			long long int valueNumeric = strtoll(c_trimmed, &end, 0);

			if ((NULL != end) && ('\0' == *end)) {
				/* strtoll consumed the whole string */
				if (NULL != ret) {
					*ret = valueNumeric;
				}
				rc = DDR_RC_OK;
			}
			break;
		}
	}

	return rc;
}

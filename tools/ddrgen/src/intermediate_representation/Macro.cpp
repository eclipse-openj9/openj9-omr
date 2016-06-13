/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#include "Macro.hpp"

#include <string.h>

using std::string;

#if defined(_MSC_VER)
#define strtoll _strtoi64
#endif /* defined(_MSC_VER) */

DDR_RC
Macro::getNumeric(long long *ret)
{
	DDR_RC rc = DDR_RC_ERROR;
	string value = getValue();

	if (value.length() > 0) {
		/* If the pre-processed macro contains no brackets, read it as a number. */
		if (string::npos == value.find('(')) {
			const char *str = value.c_str();
			char *endptr = NULL;
			long long valueNumeric = strtoll(str, &endptr, 0);
			if ((size_t)(endptr - str) == strlen(str)) {
				if (NULL != ret) {
					*ret = valueNumeric;
				}
				rc = DDR_RC_OK;
			}
		} else {
			/* For macros containing brackets, extract the number. This works for casts
			 * such as ((int)5).
			 */
			while (string::npos != value.find('(')) {
				size_t lastOpenParen = value.find_last_of('(');
				size_t nextCloseParen = value.find(')', lastOpenParen);
				if (string::npos == nextCloseParen) {
					break;
				}
				const char *substr = value.substr(lastOpenParen + 1, nextCloseParen - lastOpenParen - 1).c_str();
				char *endptr = NULL;
				long long valueNumeric = strtoll(substr, &endptr, 0);
				if ((size_t)(endptr - substr) == strlen(substr)) {
					if (NULL != ret) {
						*ret = valueNumeric;
					}
					rc = DDR_RC_OK;
					break;
				} else {
					value.replace(lastOpenParen, nextCloseParen, "");
				}
			}
		}
	}

	return rc;
}

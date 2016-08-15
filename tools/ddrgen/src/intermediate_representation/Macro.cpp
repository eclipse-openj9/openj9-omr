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

#include <sstream>
#include <string.h>

using std::string;
using std::stringstream;

DDR_RC
Macro::getNumeric(long long *ret)
{
	DDR_RC rc = DDR_RC_OK;
	string value = getValue();

	if (value.length() > 0) {
		/* If the pre-processed macro contains no brackets, read it as a number. */
		if (string::npos == value.find('(')) {
			stringstream ss;
			ss << value;
			long long valueNumeric = 0;
			ss >> valueNumeric;
			if (ss.fail()) {
				rc = DDR_RC_ERROR;
			} else if (NULL != ret) {
				*ret = valueNumeric;
			}
		} else {
			/* For macros containing brackets, extract the number. This works for casts
			 * such as ((int)5).
			 */
			rc = DDR_RC_ERROR;
			while (string::npos != value.find('(')) {
				size_t lastOpenParen = value.find_last_of('(');
				size_t nextCloseParen = value.find(')', lastOpenParen);
				if (string::npos == nextCloseParen) {
					break;
				}
				string substr = value.substr(lastOpenParen + 1, nextCloseParen - lastOpenParen - 1);
				stringstream ss;
				ss << substr;
				long long valueNumeric = 0;
				ss >> valueNumeric;
				if (!ss.fail()) {
					rc = DDR_RC_OK;
					if (NULL != ret) {
						*ret = valueNumeric;
					}
					break;
				} else {
					value.replace(lastOpenParen, nextCloseParen, "");
				}
			}
		}
	}

	return rc;
}

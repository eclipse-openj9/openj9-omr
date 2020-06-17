/*******************************************************************************
 * Copyright (c) 2017, 2020 IBM Corp. and others
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

#include "ddr/scanner/Scanner.hpp"

#include "ddr/ir/TextFile.hpp"

static bool
stringMatches(const char *candidate, const char *pattern)
{
	for (;;) {
		if ('*' == *pattern) {
			/* match successively longer prefixes of candidate against the asterisk */
			for (++pattern;; ++candidate) {
				if (stringMatches(candidate, pattern)) {
					return true;
				} else if ('\0' == *candidate) {
					break;
				}
			}
			break;
		} else if (*candidate != *pattern) {
			/* no match */
			break;
		} else if ('\0' == *pattern) {
			/* match: at the end of both the string and the pattern with no conflicts */
			return true;
		} else {
			++candidate;
			++pattern;
		}
	}

	return false;
}

static bool
stringMatchesAny(const string &candidate, const set<string> &patterns)
{
	const char *c_candidate = candidate.c_str();

	for (set<string>::const_iterator it = patterns.begin(); it != patterns.end(); ++it) {
		const string &pattern = *it;

		if (stringMatches(c_candidate, pattern.c_str())) {
			return true;
		}
	}

	return false;
}

bool
Scanner::checkExcludedType(const string &name) const
{
	bool excluded = false;

	/* Implicitly excluded are non-empty names that don't start
	 * with a letter or an underscore.
	 */
	if (!name.empty()) {
		char start = name[0];

		if (('_' == start)
				|| (('A' <= start) && (start <= 'I'))
				|| (('J' <= start) && (start <= 'R'))
				|| (('S' <= start) && (start <= 'Z'))
				|| (('a' <= start) && (start <= 'i'))
				|| (('j' <= start) && (start <= 'r'))
				|| (('s' <= start) && (start <= 'z'))) {
			excluded = stringMatchesAny(name, _excludedTypes);
		} else {
			excluded = true;
		}
	}

	return excluded;
}

bool
Scanner::checkExcludedFile(const string &name) const
{
	return stringMatchesAny(name, _excludedFiles);
}

DDR_RC
Scanner::loadExcludesFile(OMRPortLibrary *portLibrary, const char *path)
{
	/* Load the excludes file which contains a list of files and types
	 * to ignore and not add to the IR, such as system types.
	 */
	DDR_RC rc = DDR_RC_OK;

	if (NULL != path) {
		TextFile excludesFile(portLibrary);

		if (!excludesFile.openRead(path)) {
			ERRMSG("cannot open excludes file %s", path);
			rc = DDR_RC_ERROR;
		} else {
			string line;

			while (excludesFile.readLine(line)) {
				size_t end = line.length();

				if (end <= 5) {
					continue;
				}

				string tag = line.substr(0, 5);
				string pattern = line.substr(5, end);

				if ("file:" == tag) {
					_excludedFiles.insert(pattern);
				} else if ("type:" == tag) {
					_excludedTypes.insert(pattern);
				}
			}

			excludesFile.close();
		}
	}

	return rc;
}

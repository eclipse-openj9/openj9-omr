/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017
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

#include "ddr/config.hpp"

#include "ddr/scanner/Scanner.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>

using std::ios;
using std::ifstream;

static bool
checkStringMatch(string checked, set<string> *patterns)
{
	bool match = patterns->find(checked) != patterns->end();

	if (!match) {
		for (set<string>::iterator it = patterns->begin(); it != patterns->end(); it ++) {
			size_t wildcardPos = 0;
			size_t lastWildcardPos = 0;
			size_t lastSequencePos = 0;
			bool matchFound = false;
			while ((string::npos != wildcardPos) && (it->length() > wildcardPos)) {
				wildcardPos = it->find("*", lastWildcardPos);
				if (string::npos == wildcardPos) {
					wildcardPos = it->length();
				}
				string sequence = it->substr(lastWildcardPos, wildcardPos - lastWildcardPos);
				size_t sequencePos = checked.find(sequence);
				matchFound = sequence.empty()
					|| (0 == lastWildcardPos && 0 == sequencePos)
					|| ((string::npos != sequencePos) && (sequencePos > lastSequencePos) && (0 != lastWildcardPos));
				matchFound = matchFound
					&& (sequencePos + sequence.length() == checked.length()
					|| wildcardPos < it->length()
					|| sequence.empty());
				if (!matchFound) {
					break;
				}
				lastWildcardPos = wildcardPos + 1;
				lastSequencePos = sequencePos;
			}
			if (matchFound) {
				match = true;
				break;
			}
		}
	}
	return match;
}

bool
Scanner::checkBlacklistedType(string name)
{
	return checkStringMatch(name, &_blacklistedTypes);
}

bool
Scanner::checkBlacklistedFile(string name)
{
	return checkStringMatch(name, &_blacklistedFiles);
}

DDR_RC
Scanner::loadBlacklist(string path)
{
	/* Load the blacklist file. The blacklist contains a list of types
	 * to ignore and not add to the IR, such as system types.
	 */
	DDR_RC rc = DDR_RC_OK;
	string line = "";
	ifstream blackListInput(path.c_str(), ios::in);

	if (blackListInput.is_open()) {
		while (getline(blackListInput, line)) {
			string pattern = line.substr(5, line.length() - (string::npos == line.find_last_of("\n") ? 5 : 6));
			if (0 == line.find("file:")) {
				_blacklistedFiles.insert(pattern);
			} else if (0 == line.find("type:")) {
				_blacklistedTypes.insert(pattern);
			}
		}
	} else {
		ERRMSG("Could not open blackList.txt");
		rc = DDR_RC_ERROR;
	}
	blackListInput.close();

	return rc;
}
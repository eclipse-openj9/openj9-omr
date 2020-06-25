/*******************************************************************************
 * Copyright (c) 2014, 2020 IBM Corp. and others
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

#include "ArgParser.hpp"
#include "FileUtils.hpp"
#include "StringUtils.hpp"

RCType
ArgParser::parseOptions(int argc, char *argv[], J9TDFOptions *options)
{
	RCType rc = RC_OK;

	for (int i = 1; i < argc; i++) {
		bool parsingRoot = StringUtils::startsWithUpperLower(argv[i], "-root");
		if (parsingRoot || StringUtils::startsWithUpperLower(argv[i], "-file")) {
			i++;
			if (i >= argc) {
				if (parsingRoot) {
					FileUtils::printError("Root directory not specified\n");
				} else {
					FileUtils::printError("File not specified\n");
				}
				goto fail;
			} else {
				/* Construct linked list of root directories. */
				char *pathArgs = strdup(argv[i]);
				char *dirToken = NULL;
				if (NULL == pathArgs) {
					goto fail;
				}
				dirToken = strtok(pathArgs, ",");
				Path **current = NULL;
				if (parsingRoot) {
					current = &(options->rootDirectory);
				} else {
					current = &(options->files);
				}

				while (NULL != dirToken) {
					*current = (Path *)Port::omrmem_calloc(1, sizeof(Path));
					if (NULL == *current) {
						goto fail;
					}
					(*current)->path = strdup(dirToken);
					(*current)->next = NULL;
					current = &(*current)->next;

					dirToken = strtok(NULL, ",");
				}
				Port::omrmem_free((void **)&pathArgs);
			}
		} else if (StringUtils::startsWithUpperLower(argv[i], "-threshold")) {
			i++;
			if (i >= argc) {
				FileUtils::printError("TraceGen: threshold specified with no number aborting\n");
				goto fail;
			} else {
				options->threshold = atoi(argv[i]);
			}
		} else if (StringUtils::startsWithUpperLower(argv[i], "-majorversion")) {
			i++;
			if (i >= argc) {
				FileUtils::printError("TraceGen: majorversion specified with no number aborting\n");
				goto fail;
			} else {
				options->rasMajorVersion = atoi(argv[i]);
			}
		} else if (StringUtils::startsWithUpperLower(argv[i], "-minorversion")) {
			i++;
			if (i >= argc) {
				FileUtils::printError("TraceGen: minorversion specified with no number aborting\n");
				goto fail;
			} else {
				options->rasMinorVersion = atoi(argv[i]);
			}
		} else if (StringUtils::startsWithUpperLower(argv[i], "-verbose")) {
			options->verboseOutput = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-debug")) {
			options->debugOutput = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-w2cd"))	{
			options->writeToCurrentDir = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-generateCfiles")) {
			options->generateCFiles = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-treatWarningAsError")) {
			options->treatWarningAsError = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-statErrorsAreFatal")) {
			options->statErrorsAreFatal = true;
		} else if (StringUtils::startsWithUpperLower(argv[i], "-force")) {
			options->force = true;
		} else {
			FileUtils::printError("Unknown option: %s\n", argv[i]);
			goto fail;
		}
	}

	if ((NULL == options->rootDirectory) && (NULL == options->files)) {
		options->rootDirectory = (Path *)Port::omrmem_calloc(1, sizeof(Path));
		if (NULL == options->rootDirectory) {
			goto fail;
		}
		options->rootDirectory->path = strdup(".");
		options->rootDirectory->next = NULL;
	}

done:
	return rc;
fail:
	rc = RC_FAILED;
	goto done;
}

RCType
ArgParser::freeOptions(J9TDFOptions *options)
{
	Path *dir = options->rootDirectory;
	while (NULL != dir) {
		Path *tmpDir = dir->next;
		Port::omrmem_free((void **)&(dir->path));
		Port::omrmem_free((void **)&dir);
		dir = tmpDir;
	}
	return RC_OK;
}

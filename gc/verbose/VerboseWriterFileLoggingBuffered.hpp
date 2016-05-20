/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2016
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

#if !defined(VERBOSEWRITERFILELOGGINGBUFFERED_HPP_)
#define VERBOSEWRITERFILELOGGINGBUFFERED_HPP_

#include "omrcfg.h"

#include "VerboseWriterFileLogging.hpp"

/**
 * Ouptut agent which directs verbosegc output to file.
 */
class MM_VerboseWriterFileLoggingBuffered : public MM_VerboseWriterFileLogging
{
	/*
	 * Data members
	 */
public:
protected:
private:
	OMRFileStream *_logFileStream; /**< the filestream being written to */

	/*
	 * Function members
	 */
public:
	static MM_VerboseWriterFileLoggingBuffered *newInstance(MM_EnvironmentBase *env, MM_VerboseManager *manager, char* filename, uintptr_t fileCount, uintptr_t iterations);

	virtual void outputString(MM_EnvironmentBase *env, const char* string);

protected:
	MM_VerboseWriterFileLoggingBuffered(MM_EnvironmentBase *env, MM_VerboseManager *manager);

	virtual bool initialize(MM_EnvironmentBase *env, const char *filename, uintptr_t numFiles, uintptr_t numCycles);

private:
	virtual void tearDown(MM_EnvironmentBase *env);

	bool openFile(MM_EnvironmentBase *env);
	void closeFile(MM_EnvironmentBase *env);
};

#endif /* VERBOSEWRITERFILELOGGINGBUFFERED_HPP_ */

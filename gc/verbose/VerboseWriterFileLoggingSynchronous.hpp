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

#if !defined(VERBOSEWRITERFILELOGGINGSYNCHRONOUS_HPP_)
#define VERBOSEWRITERFILELOGGINGSYNCHRONOUS_HPP_
 
#include "omrcfg.h"

#include "VerboseWriterFileLogging.hpp"

/**
 * Ouptut agent which directs verbosegc output to file.
 */
class MM_VerboseWriterFileLoggingSynchronous : public MM_VerboseWriterFileLogging
{
	/*
	 * Data members
	 */
public:
protected:
private:
	intptr_t _logFileDescriptor; /**< the file being written to */

	/*
	 * Function members
	 */
public:
	static MM_VerboseWriterFileLoggingSynchronous *newInstance(MM_EnvironmentBase *env, MM_VerboseManager *manager, char* filename, uintptr_t fileCount, uintptr_t iterations);

	virtual void outputString(MM_EnvironmentBase *env, const char* string);

protected:
	MM_VerboseWriterFileLoggingSynchronous(MM_EnvironmentBase *env, MM_VerboseManager *manager);
	virtual bool initialize(MM_EnvironmentBase *env, const char *filename, uintptr_t numFiles, uintptr_t numCycles);

private:
	virtual void tearDown(MM_EnvironmentBase *env);

	bool openFile(MM_EnvironmentBase *env);
	void closeFile(MM_EnvironmentBase *env);

};

#endif /* VERBOSEWRITERFILELOGGINGSYNCHRONOUS_HPP_ */

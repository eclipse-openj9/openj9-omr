/*******************************************************************************
 * Copyright (c) 1991, 2016 IBM Corp. and others
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

/**
 * @file
 * @ingroup GC_Base_Core
 */

#if !defined(DISPATCHER_HPP_)
#define DISPATCHER_HPP_

#include "omrcfg.h"

#include "modronbase.h"

#include "BaseVirtual.hpp"

class MM_EnvironmentBase;
class MM_Task;
class MM_ParallelDispatcher;

/**
 * @todo Provide define documentation
 * @ingroup GC_Base_Core
 */
#define J9MODRON_HANDLE_NEXT_WORK_UNIT(envPtr) envPtr->_currentTask->handleNextWorkUnit(envPtr)

/**
 * @todo Provide class documentation
 * @ingroup GC_Base_Core
 */
class MM_Dispatcher : public MM_BaseVirtual
{
private:
	
protected:

public:
	virtual void kill(MM_EnvironmentBase *env) = 0;
	virtual bool startUpThreads() = 0;
	virtual void shutDownThreads() = 0;

	virtual bool condYieldFromGCWrapper(MM_EnvironmentBase *env, uint64_t timeSlack = 0) = 0;

	virtual uintptr_t threadCount() = 0;
	virtual uintptr_t threadCountMaximum() = 0;
	virtual uintptr_t activeThreadCount() = 0;
	virtual void setThreadCount(uintptr_t threadCount) = 0;

	virtual void run(MM_EnvironmentBase *env, MM_Task *task, uintptr_t threadCount = UDATA_MAX) = 0;
	virtual void reinitAfterFork(MM_EnvironmentBase *env, uintptr_t newThreadCount) = 0;

	/**
	 * Create a Dispatcher object.
	 */
	MM_Dispatcher(MM_EnvironmentBase *env) :
		MM_BaseVirtual()
	{
		_typeId = __FUNCTION__;
	};
	
	friend class MM_Task;
};

#endif /* DISPATCHER_HPP_ */

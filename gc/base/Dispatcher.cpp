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
 ******************************************************************************/

#include "omrcfg.h"
#include "omrcomp.h"

#include "Dispatcher.hpp"

#include "EnvironmentBase.hpp"
#include "Task.hpp"

MM_Dispatcher *
MM_Dispatcher::newInstance(MM_EnvironmentBase *env)
{
	MM_Dispatcher *dispatcher;
	
	dispatcher = (MM_Dispatcher *)env->getForge()->allocate(sizeof(MM_Dispatcher), MM_AllocationCategory::FIXED, OMR_GET_CALLSITE());
	if (dispatcher) {
		new(dispatcher) MM_Dispatcher(env);
		if(!dispatcher->initialize(env)) {
			dispatcher->kill(env);
			return NULL;
		}
	}
	return dispatcher;
}

void
MM_Dispatcher::kill(MM_EnvironmentBase *env)
{
	env->getForge()->free(this);
}

bool
MM_Dispatcher::initialize(MM_EnvironmentBase *env)
{
	return true;
}

void
MM_Dispatcher::prepareThreadsForTask(MM_EnvironmentBase *env, MM_Task *task)
{
	task->setThreadCount(1);
	_task = task;
}

void
MM_Dispatcher::acceptTask(MM_EnvironmentBase *env)
{
	env->_currentTask = _task;
	env->_currentTask->accept(env);
}

void
MM_Dispatcher::completeTask(MM_EnvironmentBase *env)
{
	MM_Task *currentTask = env->_currentTask;

	env->_currentTask = NULL;
	_task = NULL;

	currentTask->complete(env);
}

void
MM_Dispatcher::cleanupAfterTask(MM_EnvironmentBase *env)
{
}

void
MM_Dispatcher::run(MM_EnvironmentBase *env, MM_Task *task, uintptr_t newThreadCount)
{
	uintptr_t defaultThreadCount = threadCount();
	if (UDATA_MAX != newThreadCount) {
		/* Let tasks run with different (typically reduced) thread count. */
		setThreadCount(newThreadCount);
	}

	task->masterSetup(env);
	prepareThreadsForTask(env, task);
	acceptTask(env);
	task->run(env);
	completeTask(env);
	cleanupAfterTask(env);
	task->masterCleanup(env);

	/* restore the default thread count */
	setThreadCount(defaultThreadCount);
}

bool 
MM_Dispatcher::startUpThreads() 
{ 
	return true; 
}

void 
MM_Dispatcher::shutDownThreads() 
{
}

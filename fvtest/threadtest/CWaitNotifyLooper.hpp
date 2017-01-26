/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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


#ifndef J9THREADTEST_CWAITNOTIFYLOOPER_HPP_INCLUDED
#define J9THREADTEST_CWAITNOTIFYLOOPER_HPP_INCLUDED

#include "threadTestLib.hpp"

class CWaitNotifyLooper: public CThread
{
public:
	CWaitNotifyLooper(CMonitor& monitor, unsigned int* notifyCount, unsigned int* doneRunningCount) :
			m_monitor(monitor),
			m_keepRunning(true),
			m_notifyCount(notifyCount),
			m_doneRunningCount(doneRunningCount)
	{
		assert(m_notifyCount);
		assert(m_doneRunningCount);
	}

	void
	StopRunning(void)
	{
		m_keepRunning = false;
	}

protected:
	virtual intptr_t
	Run(void)
	{

		omrTestEnv->log(LEVEL_VERBOSE, "thread %p (%p) running\n", m_self, this);
		omrTestEnv->log(LEVEL_VERBOSE, "thread %p entering\n", m_self);
		m_monitor.Enter();
		omrTestEnv->log(LEVEL_VERBOSE, "thread %p entered\n", m_self);
		while (m_keepRunning) {
			omrTestEnv->log(LEVEL_VERBOSE, "%p N[\n", m_self);
			m_monitor.Notify();
			omrTestEnv->log(LEVEL_VERBOSE, "%p N]\n", m_self);
			*m_notifyCount = *m_notifyCount + 1;
			omrTestEnv->log(LEVEL_VERBOSE, "%p>W[ ", m_self);
			m_monitor.Wait();
			omrTestEnv->log(LEVEL_VERBOSE, "%p>W] ", m_self);
		}
		*m_doneRunningCount = *m_doneRunningCount + 1;
		omrTestEnv->log(LEVEL_VERBOSE, "thread %p exiting monitor\n", m_self);
		m_monitor.Exit();

		omrTestEnv->log(LEVEL_VERBOSE, "thread %p exiting\n", m_self);
		return 0;
	}

	CMonitor& m_monitor;
	volatile bool m_keepRunning;
	unsigned int *m_notifyCount;
	unsigned int *m_doneRunningCount;
};

#endif /* J9THREADTEST_CWAITNOTIFYLOOPER_HPP_INCLUDED */

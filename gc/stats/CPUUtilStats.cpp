/*******************************************************************************
 * Copyright IBM Corp. and others 2024
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

/**
 * @file
 * @ingroup GC_Stats
 */

#include <math.h>

#include "CPUUtilStats.hpp"

#include "CollectionStatistics.hpp"
#include "EnvironmentBase.hpp"
#include "ModronAssertions.h"

void
MM_CPUUtilStats::recordProcessAndCpuUtilization(MM_EnvironmentBase *env, MM_CollectionStatistics *stats)
{
	OMRPORT_ACCESS_FROM_OMRPORT(env->getPortLibrary());

	/* For process and cpu utilization, this is a start point (since we measure it between GC STW point.
	 * However for GC (user/system/stall) measurement this is a end point.
	 * Since some of the params like timestamp is shared for both measurements, the naming of some fields may be confusing, depending on perspective
	 */

	stats->_endTime = omrtime_hires_clock();

	J9SysinfoCPUTime cpuTime;
	intptr_t portLibraryStatus = omrsysinfo_get_CPU_utilization(&cpuTime);

	_validData = (0 == portLibraryStatus);

	intptr_t processRc = omrthread_get_process_times(&stats->_endProcessTimes);

	switch (processRc) {
	case -1: /* Error: Function un-implemented on architecture */
	case -2: /* Error: getrusage() or GetProcessTimes() returned error value */
		stats->_endProcessTimes._userTime = 0;
		stats->_endProcessTimes._systemTime = 0;
		_validData = false;
		break;
	case  0:
		break; /* Success */
	default:
		Assert_MM_unreachable();
	}

	_elapsedTime = stats->_endTime;

	if (_validData) {
		_processUserTime = stats->_endProcessTimes._userTime;
		_processSystemTime = stats->_endProcessTimes._systemTime;
		_cpuTime = cpuTime.cpuTime;

		/* Expose CPU stats data to the higher level stats struct (but with Collector specific instance), so it can be accessible via hooks */
		stats->_cpuUtilStats = *this;
	}
}

void
MM_CPUUtilStats::calculateProcessAndCpuUtilizationDelta(MM_EnvironmentBase *env, MM_CollectionStatistics *stats)
{
	OMRPORT_ACCESS_FROM_OMRPORT(env->getPortLibrary());

	/* For process and cpu utilization, this is an end point (since we measure it between GC STW point.
	 * However for GC (user/system/stall) measurement this is a start point.
	 * Since some of the params like timestamp is shared for both measurements, the naming of some fields may be confusing, depending on perspective
	 */

	intptr_t processRc = omrthread_get_process_times(&stats->_startProcessTimes);

	switch (processRc) {
	case -1: /* Error: Function un-implemented on architecture */
	case -2: /* Error: getrusage() or GetProcessTimes() returned error value */
		stats->_startProcessTimes._userTime = I_64_MAX;
		stats->_startProcessTimes._systemTime = I_64_MAX;
		break;
	case  0:
		break; /* Success */
	default:
		Assert_MM_unreachable();
	}

	J9SysinfoCPUTime cpuTime;
	intptr_t portLibraryStatus = omrsysinfo_get_CPU_utilization(&cpuTime);

	stats->_startTime = omrtime_hires_clock();

	if (_validData && (0 == portLibraryStatus) && (0 == processRc) && (_elapsedTime < stats->_startTime)) {
		int64_t elapsedTime = omrtime_hires_delta(_elapsedTime, stats->_startTime, OMRPORT_TIME_DELTA_IN_MICROSECONDS) * omrsysinfo_get_number_CPUs_by_type(OMRPORT_CPU_TARGET);
		int64_t cpuTimeDiff = 0;

		if (cpuTime.cpuTime > _cpuTime) {
			cpuTimeDiff += omrtime_hires_delta(_cpuTime, cpuTime.cpuTime, OMRPORT_TIME_DELTA_IN_MICROSECONDS);
		}

		int64_t processTimeDiff = 0;

		if (stats->_startProcessTimes._systemTime > _processSystemTime) {
			processTimeDiff += omrtime_hires_delta(_processSystemTime, stats->_startProcessTimes._systemTime, OMRPORT_TIME_DELTA_IN_MICROSECONDS);
		}

		if (stats->_startProcessTimes._userTime > _processUserTime) {
			processTimeDiff += omrtime_hires_delta(_processUserTime, stats->_startProcessTimes._userTime, OMRPORT_TIME_DELTA_IN_MICROSECONDS);
		}

		if (processTimeDiff > elapsedTime) {
			int64_t adjustedValue = (processTimeDiff + elapsedTime) / 2;
			Trc_MM_CPUUtilStats_processAndCpuUtilization_process_larger_than_elapsed(env->getLanguageVMThread(), processTimeDiff, elapsedTime, adjustedValue);
			processTimeDiff = adjustedValue;
			elapsedTime = adjustedValue;
		}
		if (cpuTimeDiff > elapsedTime) {
			Trc_MM_CPUUtilStats_processAndCpuUtilization_cpu_larger_than_elapsed(env->getLanguageVMThread(), cpuTimeDiff, elapsedTime);
			cpuTimeDiff = elapsedTime;
		}
		if (cpuTimeDiff < processTimeDiff) {
			Trc_MM_CPUUtilStats_processAndCpuUtilization_cpu_smaller_than_process(env->getLanguageVMThread(), cpuTimeDiff, processTimeDiff);
			cpuTimeDiff = processTimeDiff;
		}
		/* When calculating averages, take the new value with a weight < 1 */
		const float baseWeight = 0.5f;
		/* Since we calculate utilization on GC points, and intervals between GCs are not of constant duration, we will adjust the weight
		 * so it's larger than the base weight if the current internal is larger then the average interval duration (and vice versa for shorter current intervals)
		 * Still, make sure the adjusted weight remains within (0, 1) range.
		 */
		float utilWeight = 1.0f - powf(1.0f - baseWeight, elapsedTime / _avgInterval);
		_avgInterval = MM_Math::weightedAverage(_avgInterval, (float)elapsedTime, 1.0f - baseWeight);

		_avgCpuUtil = MM_Math::weightedAverage(_avgCpuUtil, cpuTimeDiff / (float)elapsedTime, 1.0f - utilWeight);
		_avgProcUtil = MM_Math::weightedAverage(_avgProcUtil, processTimeDiff / (float)elapsedTime, 1.0f - utilWeight);

		/* Expose CPU stats data to the higher level stats struct (but with Collector specific instance), so it can be accessible via hooks */
		stats->_cpuUtilStats = *this;
	}
}

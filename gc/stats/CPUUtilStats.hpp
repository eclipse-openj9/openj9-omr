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

#if !defined(CPUUTILSTATS_HPP_)
#define CPUUTILSTATS_HPP_

#include <stdint.h>

#include "Base.hpp"

class  MM_CollectionStatistics;
class  MM_EnvironmentBase;


/**
 * CPU and process utilization between GC STW points (excluding GC STW points)
 * Measured between end of STW GC and next start STW GC.
 * Process util is mostly application thread activity but also various VM (GC, JIT,...) background threads.
 */
class MM_CPUUtilStats : public MM_Base {
public:
	int64_t _processUserTime; /**< process user time snapshot at the start of measurement, at GC start */
	int64_t _processSystemTime; /**< process system time snapshot at the start of measurement, at GC start */
	int64_t _cpuTime; /**< cpu time snapshot at the start of measurement, at GC start */
	uint64_t _elapsedTime; /*< wall clock TS at the start of measurement, at GC start */
	float _avgProcUtil; /**< historically averaged values of process (user + system) times */
	float _avgCpuUtil; /**< historically averaged values of cpu time */
	float _avgInterval; /**< historically averaged values of intervals between GCs */
	bool _validData; /**< true, if both start and end points have valid data (no system calls failed) */

	MM_CPUUtilStats() :
		MM_Base()
		,_processUserTime(0)
		,_processSystemTime(0)
		,_cpuTime(0)
		,_elapsedTime(0)
		,_avgProcUtil(0.0)
		,_avgCpuUtil(0.0)
		,_avgInterval(0.0)
		,_validData(false)
	{}

	/**
	 * Called at STW GC end point to record starting values of CPU and process consumed times
	 * @param env calling GC thread env
	 * @param stats higher level, Collector specific stats struct
	 */
	void recordProcessAndCpuUtilization(MM_EnvironmentBase *env, MM_CollectionStatistics *stats);
	/**
	 * Called at STW GC start point to record end values of CPU and process consumed times,
	 * and than calculated the deltas (and their averages) since the previous 'record' call
	 * @param env calling GC thread env
	 * @param stats higher level, Collector specific stats struct
	 */
	void calculateProcessAndCpuUtilizationDelta(MM_EnvironmentBase *env, MM_CollectionStatistics *stats);
};

#endif /* CPUUTILSTATS_HPP_ */

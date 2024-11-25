/*******************************************************************************
 * Copyright IBM Corp. and others 1991
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

#if !defined(CLASSUNLOADSTATS_HPP_)
#define CLASSUNLOADSTATS_HPP_

#include "Base.hpp"

#include "omrcomp.h"

/**
 * Storage for stats relevant to the class unloading phase of a collection.
 * @ingroup GC_Stats
 */
class MM_ClassUnloadStats : public MM_Base {
public:
	uintptr_t _classLoaderUnloadedCount; /**< number of unloaded class loaders */
	uintptr_t _classLoaderCandidates; /**< number of class loaders visited */
	uintptr_t _classesUnloadedCount; /**< number of unloaded classes (including anonymous) */
	uintptr_t _anonymousClassesUnloadedCount; /**< number of anonymous classes unloaded */

	uintptr_t _classLoaderUnloadedCountCumulative; /**< number of unloaded class loaders since JVM start */
	uintptr_t _classesUnloadedCountCumulative; /**< number of unloaded classes since JVM start */
	uintptr_t _anonymousClassesUnloadedCountCumulative; /**< number of anonymous classes unloaded since JVM start */

	uint64_t _startTime; /**< Class unloading start time */
	uint64_t _endTime; /**< Class unloading end time */

	uint64_t _startSetupTime; /**< Class unloading setup start time */
	uint64_t _endSetupTime; /**< Class unloading setup end time */

	uint64_t _startScanTime; /**< Class unloading scan start time */
	uint64_t _endScanTime; /**< Class unloading scan end time */

	uint64_t _startPostTime; /**< Class unloading post-unloading start time */
	uint64_t _endPostTime; /**< Class unloading post-unloading end time */

	uint64_t _classUnloadMutexQuiesceTime; /**< Time the GC spends waiting on the classUnloadMutex */

	/**
	 * Set counters/times to zero at the beginning of cycle
	 */
	void clear();

	/**
	 * Update unloaded counters for cycle as well as cumulative counters
	 * @param[out] anonymous unloaded anonymous classes counter value
	 * @param[out] classes unloaded classes counter value (including anonymous)
	 * @param[out] classloaders unloaded classesloaders counter value
	 */
	void updateUnloadedCounters(uintptr_t anonymous, uintptr_t classes, uintptr_t classloaders);

	/**
	 * Returns cumulative counters
	 * @param[out] anonymous cumulative value pointer for unloaded anonymous classes
	 * @param[out] classes cumulative value pointer for unloaded classes (including anonymous)
	 * @param[out] classloaders cumulative value pointer for unloaded classesloaders
	 */
	void getUnloadedCountersCumulative(uintptr_t *anonymous, uintptr_t *classes, uintptr_t *classloaders);

	MM_ClassUnloadStats()
		: MM_Base()
		, _classLoaderUnloadedCount(0)
		, _classLoaderCandidates(0)
		, _classesUnloadedCount(0)
		, _anonymousClassesUnloadedCount(0)
		, _classLoaderUnloadedCountCumulative(0)
		, _classesUnloadedCountCumulative(0)
		, _anonymousClassesUnloadedCountCumulative(0)
		, _startTime(0)
		, _endTime(0)
		, _startSetupTime(0)
		, _endSetupTime(0)
		, _startScanTime(0)
		, _endScanTime(0)
		, _startPostTime(0)
		, _endPostTime(0)
		, _classUnloadMutexQuiesceTime(0) {};
};

#endif /* CLASSUNLOADSTATS_HPP_ */

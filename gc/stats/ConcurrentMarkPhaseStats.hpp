/*******************************************************************************
 * Copyright (c) 1991, 2021 IBM Corp. and others
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
 * @ingroup GC_Stats
 */
#ifndef CONCURRENTMARKPHASESTATS_HPP_
#define CONCURRENTMARKPHASESTATS_HPP_

#include "ConcurrentCardTableStats.hpp"
#include "ConcurrentGCStats.hpp"
#include "ConcurrentPhaseStatsBase.hpp"

/**
  * @ingroup GC_Stats ConcurrentMarkPhaseStats
 */
class MM_ConcurrentMarkPhaseStats : public MM_ConcurrentPhaseStatsBase
{
	/* Data Members */
private:
protected:
public:
	uint64_t _startTime;
	uint64_t _endTime;
	MM_ConcurrentCardTableStats *_cardTableStats;
	MM_ConcurrentGCStats *_collectionStats;

	/* Member Functions */
private:
protected:
public:
	virtual void clear() {
		MM_ConcurrentPhaseStatsBase::clear();
		_startTime = 0;
		_endTime = 0;
		_cardTableStats = NULL;
		_collectionStats = NULL;
	}

	MM_ConcurrentMarkPhaseStats()
		: MM_ConcurrentPhaseStatsBase(OMR_GC_CYCLE_TYPE_GLOBAL)
		, _startTime(0)
		, _endTime(0)
		, _cardTableStats(NULL)
		, _collectionStats(NULL)
		{}
}; 

#endif /* CONCURRENTMARKPHASESTATS_HPP_ */

/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

#if !defined(CONCURRENTGCSATB_HPP_)
#define CONCURRENTGCSATB_HPP_

#include "omrcfg.h"
#include "modronopt.h"

#include "omr.h"
#include "OMR_VM.hpp"
#if defined(OMR_GC_MODRON_CONCURRENT_MARK)

#include "ConcurrentGC.hpp"


/**
 * @todo Provide class documentation
 * @ingroup GC_Modron_Standard
 */
class MM_ConcurrentGCSATB : public MM_ConcurrentGC
{
	/*
	 * Data members
	 */
private:

public:
	
	/*
	 * Function members
	 */

protected:
	void tearDown(MM_EnvironmentBase *env);
	void virtual reportConcurrentHalted(MM_EnvironmentBase *env);
	uintptr_t virtual localMark(MM_EnvironmentBase *env, uintptr_t sizeToTrace);

public:
	virtual uintptr_t getVMStateID() { return OMRVMSTATE_GC_COLLECTOR_CONCURRENTGC; };
	static MM_ConcurrentGCSATB *newInstance(MM_EnvironmentBase *env);
	virtual void kill(MM_EnvironmentBase *env);

	MM_ConcurrentGCSATB(MM_EnvironmentBase *env)
		: MM_ConcurrentGC(env)
		{
			_typeId = __FUNCTION__;
		}
};

#endif /* OMR_GC_MODRON_CONCURRENT_MARK */

#endif /* CONCURRENTGCSATB_HPP_ */

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

#if !defined(PARALLELGLOBALGCTASK_HPP_)
#define PARALLELGLOBALGCTASK_HPP_

#include "omr.h"
#include "omrcfg.h"
#include "omrmodroncore.h"

#include "ParallelTask.hpp"

class MM_Dispatcher;
class MM_EnvironmentBase;
class MM_ParallelGlobalGC;

/**
 * @todo Provide class documentation
 * @ingroup GC_Modron_Standard
 */
class MM_ParallelGlobalGCTask : public MM_ParallelTask
{
private:
	MM_ParallelGlobalGC *_collector;

public:
	virtual UDATA getVMStateID() { return J9VMSTATE_GC_GLOBALGC; };
	
	virtual void run(MM_EnvironmentBase *env);

	/**
	 * Create a ParallelGlobalGCTask object.
	 */
	MM_ParallelGlobalGCTask(MM_EnvironmentBase *env, MM_Dispatcher *dispatcher, MM_ParallelGlobalGC *collector) :
		MM_ParallelTask(env, dispatcher),
		_collector(collector)
	{
		_typeId = __FUNCTION__;
	};
};

#endif /* PARALLELGLOBALGCTASK_HPP_ */

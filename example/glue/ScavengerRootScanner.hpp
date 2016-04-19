/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef SCAVENGERROOTSCANNER_HPP_
#define SCAVENGERROOTSCANNER_HPP_

#include "omr.h"
#include "omrcfg.h"
#include "omrExampleVM.hpp"
#include "omrhashtable.h"

#include "Base.hpp"
#include "EnvironmentStandard.hpp"
#include "Scavenger.hpp"

#if defined(OMR_GC_MODRON_SCAVENGER)

class MM_ScavengerRootScanner : public MM_Base
{
	/*
	 * Member data and types
	 */
private:
	MM_Scavenger *_scavenger;

protected:
public:

	/*
	 * Member functions
	 */
private:
protected:
public:
	MM_ScavengerRootScanner(MM_EnvironmentBase *env, MM_Scavenger *scavenger)
		: MM_Base()
		, _scavenger(scavenger)
	{
	};

	void
	scavengeRememberedSet(MM_EnvironmentStandard *env)
	{
		_scavenger->scavengeRememberedSet(env);
	}

	void
	pruneRememberedSet(MM_EnvironmentStandard *env)
	{
		_scavenger->pruneRememberedSet(env);
	}

	void
	scanRoots(MM_EnvironmentBase *env)
	{
		J9HashTableState state;
		OMR_VM_Example *omrVM = (OMR_VM_Example *)env->getOmrVM()->_language_vm;
		MM_EnvironmentStandard *envStd = MM_EnvironmentStandard::getEnvironment(env);
		RootEntry *rootEntry = (RootEntry *)hashTableStartDo(omrVM->rootTable, &state);
		while (rootEntry != NULL) {
			_scavenger->copyObjectSlot(envStd, (volatile omrobjectptr_t *) &rootEntry->rootPtr);
			rootEntry = (RootEntry *)hashTableNextDo(&state);
		}
	}

	void rescanThreadSlots(MM_EnvironmentStandard *env) { }

	void scanClearable(MM_EnvironmentBase *env) { }

	void flush(MM_EnvironmentStandard *env) { }
};

#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
#endif /* SCAVENGERROOTSCANNER_HPP_ */

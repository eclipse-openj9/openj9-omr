/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017
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
#include "omr.h"
#include "omrhashtable.h"

#include "EnvironmentBase.hpp"
#include "MarkingScheme.hpp"
#include "omrExampleVM.hpp"
#include "OMRVMThreadListIterator.hpp"

#include "MarkingDelegate.hpp"

#if defined(OMR_VALGRIND_MEMCHECK)
#include <valgrind/memcheck.h>
#endif /* defined(OMR_VALGRIND_MEMCHECK) */

void
MM_MarkingDelegate::scanRoots(MM_EnvironmentBase *env)
{
	OMR_VM_Example *omrVM = (OMR_VM_Example *)env->getOmrVM()->_language_vm;
	J9HashTableState state;
	RootEntry *rEntry = NULL;
	rEntry = (RootEntry *)hashTableStartDo(omrVM->rootTable, &state);
	while (rEntry != NULL) {
		_markingScheme->markObject(env, rEntry->rootPtr);
		rEntry = (RootEntry *)hashTableNextDo(&state);
	}
	OMR_VMThread *walkThread;
	GC_OMRVMThreadListIterator threadListIterator(env->getOmrVM());
	while((walkThread = threadListIterator.nextOMRVMThread()) != NULL) {
		if (NULL != walkThread->_savedObject1) {
			_markingScheme->markObject(env, (omrobjectptr_t)walkThread->_savedObject1);
		}
		if (NULL != walkThread->_savedObject2) {
			_markingScheme->markObject(env, (omrobjectptr_t)walkThread->_savedObject2);
		}
	}
}

void
MM_MarkingDelegate::masterCleanupAfterGC(MM_EnvironmentBase *env)
{
	OMRPORT_ACCESS_FROM_OMRVM(env->getOmrVM());
	J9HashTableState state;
	ObjectEntry *oEntry = NULL;
	OMR_VM_Example *omrVM = (OMR_VM_Example *)env->getOmrVM()->_language_vm;
	oEntry = (ObjectEntry *)hashTableStartDo(omrVM->objectTable, &state);
	while (oEntry != NULL) {
		if (!_markingScheme->isMarked(oEntry->objPtr)) {
#if defined(OMR_VALGRIND_MEMCHECK)
	VALGRIND_MEMPOOL_FREE(env->getExtensions()->valgrindMempoolAddr,oEntry->objPtr);
#endif /* defined(OMR_VALGRIND_MEMCHECK) */
			omrmem_free_memory((void *)oEntry->name);
			oEntry->name = NULL;
			hashTableDoRemove(&state);
		}
		oEntry = (ObjectEntry *)hashTableNextDo(&state);
	}
}


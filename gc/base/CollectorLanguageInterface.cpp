/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016
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

#include "objectdescription.h"

#include "CardTable.hpp"
#include "CollectorLanguageInterface.hpp"
#include "ConcurrentGC.hpp"
#include "EnvironmentStandard.hpp"
#include "ObjectModel.hpp"
#include "Scavenger.hpp"
#include "SlotObject.hpp"

struct OMR_VMThread;

void
MM_CollectorLanguageInterface::writeBarrier(OMR_VMThread *omrThread, omrobjectptr_t parentObject, omrobjectptr_t childObject)
{
#if defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_MODRON_CONCURRENT_MARK)
	MM_EnvironmentStandard *env = MM_EnvironmentStandard::getEnvironment(omrThread);
	MM_GCExtensionsBase *extensions = env->getExtensions();
#if defined(OMR_GC_MODRON_SCAVENGER)
	if (extensions->scavengerEnabled) {
		if (extensions->isOld(parentObject) && !extensions->isOld(childObject)) {
			if (extensions->objectModel.atomicSetRememberedState(parentObject, STATE_REMEMBERED)) {
				/* The object has been successfully marked as REMEMBERED - allocate an entry in the remembered set */
				extensions->scavenger->addToRememberedSetFragment(env, parentObject);
			}
		}
	}
#endif /* defined(OMR_GC_MODRON_SCAVENGER) */
#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
	if (extensions->concurrentMark) {
		extensions->cardTable->dirtyCard(env, parentObject);
	}
#endif /* defined(OMR_GC_MODRON_CONCURRENT_MARK) */
#endif /* defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_MODRON_CONCURRENT_MARK) */
}

void
MM_CollectorLanguageInterface::writeBarrierStore(OMR_VMThread *omrThread, omrobjectptr_t parentObject, fomrobject_t *parentSlot, omrobjectptr_t childObject)
{
	GC_SlotObject slotObject(omrThread->_vm, parentSlot);
	slotObject.writeReferenceToSlot(childObject);

	writeBarrier(omrThread, parentObject, childObject);
}

void
MM_CollectorLanguageInterface::writeBarrierUpdate(OMR_VMThread *omrThread, omrobjectptr_t parentObject, omrobjectptr_t childObject)
{
	writeBarrier(omrThread, parentObject, childObject);
}

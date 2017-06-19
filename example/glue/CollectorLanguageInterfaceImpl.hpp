/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2016
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

#ifndef COLLECTORLANGUAGEINTERFACEIMPL_HPP_
#define COLLECTORLANGUAGEINTERFACEIMPL_HPP_

#include "modronbase.h"
#include "omr.h"

#include "CollectorLanguageInterface.hpp"
#include "GCExtensionsBase.hpp"
#include "ParallelSweepScheme.hpp"
#include "WorkPackets.hpp"

class GC_ObjectScanner;
class MM_CompactScheme;
class MM_EnvironmentStandard;
class MM_ForwardedHeader;
class MM_MarkingScheme;
class MM_MemorySubSpaceSemiSpace;

/**
 * Class representing a collector language interface.  This implements the API between the OMR
 * functionality and the language being implemented.
 */
class MM_CollectorLanguageInterfaceImpl : public MM_CollectorLanguageInterface {
private:
protected:
	OMR_VM *_omrVM;
	MM_GCExtensionsBase *_extensions;
	MM_MarkingScheme *_markingScheme;
public:
	enum AttachVMThreadReason {
		ATTACH_THREAD = 0x0,
		ATTACH_GC_DISPATCHER_THREAD = 0x1,
		ATTACH_GC_HELPER_THREAD = 0x2,
		ATTACH_GC_MASTER_THREAD = 0x3,
	};

private:
protected:
	bool initialize(OMR_VM *omrVM);
	void tearDown(OMR_VM *omrVM);

	MM_CollectorLanguageInterfaceImpl(OMR_VM *omrVM)
		: MM_CollectorLanguageInterface()
		,_omrVM(omrVM)
		,_extensions(MM_GCExtensionsBase::getExtensions(omrVM))
	{
		_typeId = __FUNCTION__;
	}

public:
	static MM_CollectorLanguageInterfaceImpl *newInstance(MM_EnvironmentBase *env);
	virtual void kill(MM_EnvironmentBase *env);

	virtual void flushNonAllocationCaches(MM_EnvironmentBase *env);

	virtual OMR_VMThread *attachVMThread(OMR_VM *omrVM, const char *threadName, uintptr_t reason);
	virtual void detachVMThread(OMR_VM *omrVM, OMR_VMThread *omrVMThread, uintptr_t reason);

#if defined(OMR_GC_MODRON_CONCURRENT_MARK)
	virtual MM_ConcurrentSafepointCallback* concurrentGC_createSafepointCallback(MM_EnvironmentBase *env);
	virtual omrsig_handler_fn concurrentGC_getProtectedSignalHandler(void **signalHandlerArg) {*signalHandlerArg = NULL; return NULL;}
	virtual void concurrentGC_concurrentInitializationComplete(MM_EnvironmentStandard *env) {}
	virtual bool concurrentGC_forceConcurrentKickoff(MM_EnvironmentBase *env, uintptr_t gcCode, uintptr_t *languageKickoffReason)
	{
		if (NULL != languageKickoffReason) {
			*languageKickoffReason = NO_LANGUAGE_KICKOFF_REASON;
		}
		return false;
	}
	virtual uintptr_t concurrentGC_getNextTracingMode(uintptr_t executionMode);
	virtual uintptr_t concurrentGC_collectRoots(MM_EnvironmentStandard *env, uintptr_t concurrentStatus, bool *collectedRoots, bool *paidTax);
	virtual void concurrentGC_signalThreadsToTraceStacks(MM_EnvironmentStandard *env) {}
	virtual void concurrentGC_signalThreadsToDirtyCards(MM_EnvironmentStandard *env) {}
	virtual void concurrentGC_signalThreadsToStopDirtyingCards(MM_EnvironmentStandard *env) {}
	virtual void concurrentGC_kickoffCardCleaning(MM_EnvironmentStandard *env) {}
	virtual void concurrentGC_flushRegionReferenceLists(MM_EnvironmentBase *env) {}
	virtual void concurrentGC_flushThreadReferenceBuffer(MM_EnvironmentBase *env) {}
	virtual bool concurrentGC_isThreadReferenceBufferEmpty(MM_EnvironmentBase *env) {return true;}
	virtual bool concurrentGC_startConcurrentScanning(MM_EnvironmentStandard *env, uintptr_t *bytesTraced, bool *collectedRoots) {*bytesTraced = 0; *collectedRoots = false; return false;}
	virtual void concurrentGC_concurrentScanningStarted(MM_EnvironmentStandard *env, bool isConcurrentScanningComplete) {}
	virtual bool concurrentGC_isConcurrentScanningComplete(MM_EnvironmentBase *env) {return true;}
	virtual uintptr_t concurrentGC_reportConcurrentScanningMode(MM_EnvironmentBase *env) {return 0;}
	virtual void concurrentGC_scanThread(MM_EnvironmentBase *env) {}
#endif /* OMR_GC_MODRON_CONCURRENT_MARK */

	virtual bool collectorHeapRegionDescriptorInitialize(MM_EnvironmentBase *env, MM_HeapRegionDescriptor *region) {return true;}
	virtual void collectorHeapRegionDescriptorTearDown(MM_EnvironmentBase *env, MM_HeapRegionDescriptor *region) {}

	virtual void workPacketOverflow_overflowItem(MM_EnvironmentBase *env, omrobjectptr_t objectPtr) {}

#if defined(OMR_GC_MODRON_SCAVENGER)
	virtual void scavenger_reportObjectEvents(MM_EnvironmentBase *env);
	virtual void scavenger_masterSetupForGC(MM_EnvironmentBase *env);
	virtual void scavenger_workerSetupForGC_clearEnvironmentLangStats(MM_EnvironmentBase *env);
	virtual void scavenger_reportScavengeEnd(MM_EnvironmentBase * envBase, bool scavengeSuccessful);
	virtual void scavenger_mergeGCStats_mergeLangStats(MM_EnvironmentBase *envBase);
	virtual void scavenger_masterThreadGarbageCollect_scavengeComplete(MM_EnvironmentBase *envBase);
	virtual void scavenger_masterThreadGarbageCollect_scavengeSuccess(MM_EnvironmentBase *envBase);
	virtual bool scavenger_internalGarbageCollect_shouldPercolateGarbageCollect(MM_EnvironmentBase *envBase, PercolateReason *reason, uint32_t *gcCode);
	virtual GC_ObjectScanner *scavenger_getObjectScanner(MM_EnvironmentStandard *env, omrobjectptr_t objectPtr, void *allocSpace, uintptr_t flags);
	virtual void scavenger_flushReferenceObjects(MM_EnvironmentStandard *env);
	virtual bool scavenger_hasIndirectReferentsInNewSpace(MM_EnvironmentStandard *env, omrobjectptr_t objectPtr);
	virtual bool scavenger_scavengeIndirectObjectSlots(MM_EnvironmentStandard *env, omrobjectptr_t objectPtr);
	virtual void scavenger_backOutIndirectObjectSlots(MM_EnvironmentStandard *env, omrobjectptr_t objectPtr);
	virtual void scavenger_backOutIndirectObjects(MM_EnvironmentStandard *env);
	virtual void scavenger_reverseForwardedObject(MM_EnvironmentBase *env, MM_ForwardedHeader *forwardedHeader);
#if defined (OMR_INTERP_COMPRESSED_OBJECT_HEADER)
	virtual void scavenger_fixupDestroyedSlot(MM_EnvironmentBase *env, MM_ForwardedHeader *forwardedHeader, MM_MemorySubSpaceSemiSpace *subSpaceNew);
#endif /* OMR_INTERP_COMPRESSED_OBJECT_HEADER */
#endif /* OMR_GC_MODRON_SCAVENGER */

#if defined(OMR_GC_MODRON_COMPACTION)
	virtual void compactScheme_languageMasterSetupForGC(MM_EnvironmentBase *env);
	virtual void compactScheme_fixupRoots(MM_EnvironmentBase *env, MM_CompactScheme *compactScheme);
	virtual void compactScheme_workerCleanupAfterGC(MM_EnvironmentBase *env);
	virtual void compactScheme_verifyHeap(MM_EnvironmentBase *env, MM_MarkMap *markMap);
#endif /* OMR_GC_MODRON_COMPACTION */
};

#endif /* COLLECTORLANGUAGEINTERFACEIMPL_HPP_ */

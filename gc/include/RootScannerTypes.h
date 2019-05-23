/*******************************************************************************
 * Copyright (c) 1991, 2016 IBM Corp. and others
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

#if !defined(ROOTSCANNERTYPES_H__)
#define ROOTSCANNERTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum RootScannerEntity {
	RootScannerEntity_None = 0,
	RootScannerEntity_ScavengeRememberedSet,
	RootScannerEntity_Classes,
	RootScannerEntity_VMClassSlots,
	RootScannerEntity_PermanentClasses,
	RootScannerEntity_ClassLoaders,
	RootScannerEntity_Threads,
	RootScannerEntity_FinalizableObjects,
	RootScannerEntity_UnfinalizedObjects,
	RootScannerEntity_OwnableSynchronizerObjects,
	RootScannerEntity_StringTable,
	RootScannerEntity_JNIGlobalReferences,
	RootScannerEntity_JNIWeakGlobalReferences,
	RootScannerEntity_DoubleMappedObjects,
	RootScannerEntity_DebuggerReferences,
	RootScannerEntity_DebuggerClassReferences,
	RootScannerEntity_MonitorReferences,
	RootScannerEntity_WeakReferenceObjects,
	RootScannerEntity_SoftReferenceObjects,
	RootScannerEntity_PhantomReferenceObjects,
	RootScannerEntity_JVMTIObjectTagTables,
	RootScannerEntity_NonCollectableObjects,
	RootScannerEntity_RememberedSet,
	RootScannerEntity_MemoryAreaObjects,
	RootScannerEntity_MetronomeRememberedSet,
	RootScannerEntity_ClassesComplete,
	RootScannerEntity_WeakReferenceObjectsComplete,
	RootScannerEntity_SoftReferenceObjectsComplete,
	RootScannerEntity_PhantomReferenceObjectsComplete,
	RootScannerEntity_UnfinalizedObjectsComplete,
	RootScannerEntity_OwnableSynchronizerObjectsComplete,
	RootScannerEntity_MonitorLookupCaches,
	RootScannerEntity_MonitorLookupCachesComplete,
	RootScannerEntity_MonitorReferenceObjectsComplete,

	/* Must be last, do not use this entity! */
	RootScannerEntity_Count
} RootScannerEntity;

#ifdef __cplusplus
} /* extern "C" { */
#endif /* __cplusplus */

#endif /* !ROOTSCANNERTYPES_H__ */

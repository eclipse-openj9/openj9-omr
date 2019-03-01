/*******************************************************************************
 * Copyright (c) 1991, 2019 IBM Corp. and others
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

#ifndef COLLECTORLANGUAGEINTERFACEIMPL_HPP_
#define COLLECTORLANGUAGEINTERFACEIMPL_HPP_

#include "modronbase.h"
#include "omr.h"

#include "CollectorLanguageInterface.hpp"
#include "GCExtensionsBase.hpp"

class GC_ObjectScanner;
class MM_CompactScheme;
class MM_EnvironmentStandard;
class MM_ForwardedHeader;
class MM_MemorySubSpaceSemiSpace;

/**
 * Class representing a collector language interface.  This implements the API between the OMR
 * functionality and the language being implemented.
 * @ingroup GC_Base
 */
class MM_CollectorLanguageInterfaceImpl : public MM_CollectorLanguageInterface {
private:
protected:
	OMR_VM *_omrVM;
	MM_GCExtensionsBase *_extensions;
public:

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

	/* Read Barrier Verifier specific methods */
#if defined(OMR_ENV_DATA64) && !defined(OMR_GC_COMPRESSED_POINTERS)
	virtual void scavenger_poisonSlots(MM_EnvironmentBase *env);
	virtual void scavenger_healSlots(MM_EnvironmentBase *env);
#endif /* defined(OMR_ENV_DATA64) && !defined(OMR_GC_COMPRESSED_POINTERS) */

#if defined(OMR_GC_MODRON_COMPACTION)
	virtual CompactPreventedReason parallelGlobalGC_checkIfCompactionShouldBePrevented(MM_EnvironmentBase *env) {return COMPACT_PREVENTED_NONE;}
#endif /* OMR_GC_MODRON_COMPACTION */
};

#endif /* COLLECTORLANGUAGEINTERFACEIMPL_HPP_ */

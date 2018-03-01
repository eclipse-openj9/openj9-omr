/*******************************************************************************
 * Copyright (c) 2015, 2015 IBM Corp. and others
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

#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"

#include "ScavengerHotFieldStats.hpp"

#if defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_VLHGC)

MM_ScavengerHotFieldStats*
MM_ScavengerHotFieldStats::newInstance(MM_GCExtensionsBase *extensions)
{
	MM_ScavengerHotFieldStats *scavengerHotFieldStats = (MM_ScavengerHotFieldStats *)extensions->getForge()->allocate(sizeof(MM_ScavengerHotFieldStats), OMR::GC::AllocationCategory::FIXED, OMR_GET_CALLSITE());
	if (NULL != scavengerHotFieldStats) {
		new(scavengerHotFieldStats) MM_ScavengerHotFieldStats();
		if (!scavengerHotFieldStats->initialize(extensions)) {
			scavengerHotFieldStats->kill(extensions);
			scavengerHotFieldStats = NULL;
		}
	}
	return scavengerHotFieldStats;
}

bool
MM_ScavengerHotFieldStats::initialize(MM_GCExtensionsBase *extensions)
{
	_objectModel = &(extensions->objectModel);
	return true;
}

void
MM_ScavengerHotFieldStats::kill(MM_GCExtensionsBase *extensions)
{
	tearDown(extensions);
	extensions->getForge()->free(this);
}

#endif /* defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_VLHGC) */


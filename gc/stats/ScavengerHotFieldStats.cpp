/*******************************************************************************
 * Copyright (c) 2015 IBM Corporation
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    IBM Corporation - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "EnvironmentBase.hpp"
#include "GCExtensionsBase.hpp"

#include "ScavengerHotFieldStats.hpp"

#if defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_VLHGC)

bool
MM_ScavengerHotFieldStats::initialize(MM_EnvironmentBase *env)
{
	_objectModel = &(env->getExtensions()->objectModel);
	return true;
}

#endif /* defined(OMR_GC_MODRON_SCAVENGER) || defined(OMR_GC_VLHGC) */


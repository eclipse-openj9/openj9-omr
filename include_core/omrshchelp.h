/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
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
#ifndef OMRSHCHELP_H
#define OMRSHCHELP_H

/* @ddr_namespace: default */
#include "omrcfg.h"
#include "omrcomp.h"
#include "omrport.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OMRSH_ADDRMODE_32 32
#define OMRSH_ADDRMODE_64 64

#ifdef OMR_ENV_DATA64
#define OMRSH_ADDRMODE OMRSH_ADDRMODE_64
#else
#define OMRSH_ADDRMODE OMRSH_ADDRMODE_32
#endif

#define OMRSH_VERSION_PREFIX_CHAR 'C'
#define OMRSH_MODLEVEL_PREFIX_CHAR 'M'
#define OMRSH_FEATURE_PREFIX_CHAR 'F'
#define OMRSH_ADDRMODE_PREFIX_CHAR 'A'
#define OMRSH_PERSISTENT_PREFIX_CHAR 'P'
#define OMRSH_SNAPSHOT_PREFIX_CHAR 'S'
#define OMRSH_PREFIX_SEPARATOR_CHAR '_'

#define OMRPORT_SHR_CACHE_TYPE_PERSISTENT  1
#define OMRPORT_SHR_CACHE_TYPE_NONPERSISTENT  2
#define OMRPORT_SHR_CACHE_TYPE_VMEM  3
#define OMRPORT_SHR_CACHE_TYPE_CROSSGUEST  4
#define OMRPORT_SHR_CACHE_TYPE_SNAPSHOT  5

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* OMRSHCHELP_H */

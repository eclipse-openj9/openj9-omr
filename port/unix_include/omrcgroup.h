/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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
 *    Multiple authors (IBM Corp.) - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef omrcgroup_h
#define omrcgroup_h

#if defined(LINUX)

typedef struct OMRCgroupEntry {
	int32_t hierarchyId; /**< cgroup hierarch ID*/
	char *subsystem; /**< name of the subsystem*/
	char *cgroup; /**< name of the cgroup*/
	struct OMRCgroupEntry *next; /**< pointer to next OMRCgroupEntry*/
} OMRCgroupEntry;

#endif /* defined(LINUX) */

#endif /* omrcgroup_h */

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
 *    Multiple authors (IBM Corp.) - initial API and implementation and/or initial documentation
 *******************************************************************************/
#if !defined(OMRIARV64_H_)
#define OMRIARV64_H_

/**
 * On ZOS64 memory allocation above 2G bar can be:
 *	- general: no limitation for OS for memory range
 * 	- 2_TO_32: an allocation in range between 2 and 32 GB, available if 2_TO_32 or 2_TO_64 is supported by OS
 *  - 2_TO_64: an allocation in range between 2 and 64 GB, available if 2_TO_64 is supported by OS
 * !! Do not modify these values without correspondent changes in getUserExtendedPrivateAreaMemoryType.s
 */
typedef enum UserExtendedPrivateAreaType {
	ZOS64_VMEM_ABOVE_BAR_GENERAL = 0,
	ZOS64_VMEM_2_TO_32G = 1,
	ZOS64_VMEM_2_TO_64G = 2
} UserExtendedPrivateAreaType;

#endif /* !defined(OMRIARV64_H_) */

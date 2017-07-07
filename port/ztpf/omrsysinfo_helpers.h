/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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
 *    James Johnston (IBM Corp.)   - initial z/TPF Port Updates
 *******************************************************************************/

#ifndef OMRSYSINFO_HELPERS_H_
#define OMRSYSINFO_HELPERS_H_

#include "omrport.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * Retrieve z/Architecture facility bits.
 *
 * @param [in]  lastDoubleWord   Size of the bits array in number of uint64_t, minus 1.
 * @param [out] bits             Caller-supplied uint64_t array that gets populated with the facility bits.
 *
 * @return The index of the last valid uint64_t in the bits array.
 */
extern int getstfle(int lastDoubleWord, uint64_t *bits);

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* OMRSYSINFO_HELPERS_H_ */

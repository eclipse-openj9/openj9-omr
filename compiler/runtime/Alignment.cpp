/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
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

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t, uint32_t

// Note that this align function aligns UP using the alignment mask, not the
// alignment boundary.
//
// e.g., use align(ptr, 31) NOT align(ptr, 32) for 32-byte alignment
//
uint8_t *
align(uint8_t *ptr, uint32_t alignmentMask)
   {
   return (uint8_t*)((size_t)(ptr + alignmentMask) & (~(size_t)alignmentMask));
   }

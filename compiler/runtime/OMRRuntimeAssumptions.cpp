/******************************************************************************* 
 *
 * (c) Copyright IBM Corp. 2000, 2017
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

#include "runtime/OMRRuntimeAssumptions.hpp"
#include "env/jittypes.h"

#if defined(__IBMCPP__) && !defined(AIXPPC) && !defined(LINUXPPC)
#define ASM_CALL __cdecl
#else
#define ASM_CALL
#endif

#if defined(TR_HOST_S390) || defined(TR_HOST_X86) || defined(TR_HOST_ARM)
// on these platforms, _patchVirtualGuard is written in C++
extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag);
#else
extern "C" void ASM_CALL _patchVirtualGuard(uint8_t*, uint8_t*, uint32_t);
#endif


void TR::PatchNOPedGuardSite::compensate(bool isSMP, uint8_t *location, uint8_t *destination)
   {
   _patchVirtualGuard(location, destination, isSMP);
   }



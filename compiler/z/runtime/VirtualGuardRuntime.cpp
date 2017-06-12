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

#include <stdio.h>
#include <stdint.h>
#include "codegen/FrontEnd.hpp"
#include "compile/Compilation.hpp"
#include "env/jittypes.h"
#include "infra/Assert.hpp"

// Patch instruction at location.  It should be either a BRC or BRCL, and the offset field should
// already be set--just need to toggle bits 12 to 16 from 0x0 to 0xf to turn the BRC[L] from
// a nop to an unconditional branch.
// Since we're modifying 4 bits in the 2nd byte, it will not stradle a double word boundary and
// so the instruction should move from nop->branch atomically.
extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag){

   int64_t offset;
   static bool doTrace = (debug("traceVGNOP") != 0);

   if(doTrace)
      printf("####> patching VGNOP at %p (%x) destAddr: %p (%x), flag:%d\n",
              locationAddr,*(int32_t*)locationAddr,
              destinationAddr,*(int32_t*)destinationAddr,smpFlag);

   intptrj_t distance = destinationAddr - locationAddr;
   offset = destinationAddr - locationAddr;
   offset = offset / 2;

   if (distance >= -32768 && distance <= 32767)
      {
      locationAddr[0] = 0xa7;
      locationAddr[1] = 0xf4;
      locationAddr[2] = (offset >> 8)  & 0x00ff;
      locationAddr[3] = offset         & 0x00ff;
      }
   else
      {
      locationAddr[0] = 0xc0;
      locationAddr[1] = 0xf4;
      locationAddr[2] = (offset >> 24) & 0x000000ff;
      locationAddr[3] = (offset >> 16) & 0x000000ff;
      locationAddr[4] = (offset >> 8)  & 0x000000ff;
      locationAddr[5] = offset         & 0x000000ff;
      }

   TR_ASSERT(destinationAddr == (distance + locationAddr),
          "%p != %p(2*%ld+%p)\n",destinationAddr,distance+locationAddr,offset,locationAddr);
}

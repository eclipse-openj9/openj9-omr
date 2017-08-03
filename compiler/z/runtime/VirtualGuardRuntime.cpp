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
#include "codegen/OMRMachine.hpp"
#include "compile/Compilation.hpp"
#include "env/jittypes.h"
#include "infra/Assert.hpp"


/** \brief
  *    Patch an existing BRC / BRCL virtual guard at \p locationAddr to branch to \p destinationAddr or encode a BRC /
  *    BRCL at \p locationAddr to branch to \p destinationAddr if a NOP BRC / BRCL does not exist.
  *
  * \param locationAddr
  *    The location to patch.
  *
  * \param destinationAddr
  *    The destination of the branch target.
  *
  * \param smpFlag
  *    N/A.
  *
  * \note
  *    It is up to the caller to ensure that if a NOP BRC / BRCL does not exist at \p locationAddr then a non-atomic
  *    4 or 6 byte (depending on distance between \p locationAddr and \p destinationAddr) store to the instruction
  *    stream at \p locationAddr is safe. If an existing BRC / BRCL already exists at \p locationAddr then this
  *    function only stores a single byte to modify the BRC / BRCL mask value to an always taken branch mask, and hence
  *    the store will be atomic.
  */
extern "C" void _patchVirtualGuard(uint8_t* locationAddr, uint8_t* destinationAddr, int32_t smpFlag)
   {
   static bool debugTrace = debug("traceVGNOP") != NULL;

   if (debugTrace)
      {
      printf("####> Patching VGNOP at locationAddr %p (%x), destinationAddr %p (%x), smpFlag: %d\n", locationAddr, *reinterpret_cast<intptrj_t*>(locationAddr), destinationAddr, *reinterpret_cast<intptrj_t*>(destinationAddr), smpFlag);
      }

   int64_t displacement = static_cast<int64_t>(destinationAddr - locationAddr) / 2;

   if (locationAddr[0] == 0xA7 || locationAddr[0] == 0xC0)
      {
      TR_ASSERT_FATAL((locationAddr[1] & 0x04) == 0x04, "Expected to find BRC (0xA704) or BRCL (0xC004) instruction at %p but instead found %x\n", locationAddr, *reinterpret_cast<int16_t*>(locationAddr + 2));

      if (locationAddr[0] == 0xA7)
         {
         TR_ASSERT_FATAL(*reinterpret_cast<int16_t*>(locationAddr + 2) == displacement, "Branch RI displacement at %p (%x) does not match the displacement calculated from argument %p (%x)\n", locationAddr, *reinterpret_cast<int16_t*>(locationAddr + 2), destinationAddr, displacement);
         }
      else
         {
         TR_ASSERT_FATAL(*reinterpret_cast<int32_t*>(locationAddr + 2) == displacement, "Branch RI displacement at %p (%x) does not match the displacement calculated from argument %p (%x)\n", locationAddr, *reinterpret_cast<int32_t*>(locationAddr + 2), destinationAddr, displacement);
         }

      // Modify the mask value to an always taken branch
      locationAddr[1] = 0xF4;
      }
   else
      {
      if (displacement >= MIN_IMMEDIATE_VAL && displacement <= MAX_IMMEDIATE_VAL)
         {
         locationAddr[0] = 0xA7;
         locationAddr[1] = 0xF4;
         locationAddr[2] = (displacement & 0xFF00) >> 8;
         locationAddr[3] = (displacement & 0x00FF);
         }
      else
         {
         locationAddr[0] = 0xC0;
         locationAddr[1] = 0xF4;
         locationAddr[2] = (displacement & 0xFF000000) >> 24;
         locationAddr[3] = (displacement & 0x00FF0000) >> 16;
         locationAddr[4] = (displacement & 0x0000FF00) >> 8;
         locationAddr[5] = (displacement & 0x000000FF);
         }
      }
   }

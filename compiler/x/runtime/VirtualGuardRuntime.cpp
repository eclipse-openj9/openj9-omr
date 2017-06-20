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

#include <stdint.h>
#include "infra/Assert.hpp"

#define IS_32BIT_SIGNED(x)   ((x) == ( int32_t)(x))

extern "C" void _patchingFence16(void *startAddr);

extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag)
   {
   intptr_t destinationDistance = destinationAddr - locationAddr;
   TR_ASSERT(IS_32BIT_SIGNED(destinationDistance), "Destination address must be in range of 5-byte jmp instruction");

   if (-126 <= destinationDistance && destinationDistance <= 129)
      {
      // Two-byte jmp instruction
      //
      intptr_t displacement = destinationDistance-2;
      *(uint16_t*)locationAddr = 0xeb + (displacement << 8);
      }
   else
      {
      // Five-byte jmp instruction
      //
      intptr_t displacement = destinationDistance-5;

      // Self-loop
      *(uint16_t*)locationAddr = 0xfeeb;

      _patchingFence16(locationAddr);

      // Bytes 2-4
      locationAddr[2] = (displacement >> 8);
      locationAddr[3] = (displacement >> 16);
      locationAddr[4] = (displacement >> 24);

      // Bytes 0-1; unlock the self-loop/JMP+3
      _patchingFence16(locationAddr);
      *(uint16_t*)locationAddr = 0xe9 + ((displacement & 0xff) << 8);
      }
   }

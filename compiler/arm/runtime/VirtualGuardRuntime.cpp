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

#include "codegen/ARMInstruction.hpp"

extern uint32_t encodeBranchDistance(uint32_t from, uint32_t to);
extern void armCodeSync(uint8_t *, uint32_t);

extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag)
   {
   int32_t newInstr;
   int32_t distance = (int32_t)destinationAddr - (int32_t)locationAddr;
   TR_ASSERT(distance <= BRANCH_FORWARD_LIMIT && distance >= BRANCH_BACKWARD_LIMIT, "_patchVirtualGuard: Destination too far.\n");
   newInstr = 0xEA000000 | encodeBranchDistance((uint32_t)locationAddr, (uint32_t)destinationAddr);
   *(int32_t *)locationAddr = newInstr;
   armCodeSync((uint8_t *)locationAddr, ARM_INSTRUCTION_LENGTH);
   }

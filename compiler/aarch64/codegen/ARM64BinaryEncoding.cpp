/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include <stddef.h>
#include <stdint.h>

#include "codegen/ARM64ConditionCode.hpp"
#include "codegen/ARM64Instruction.hpp"
#include "codegen/CodeGenerator.hpp"

uint8_t *OMR::ARM64::Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

int32_t OMR::ARM64::Instruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(ARM64_INSTRUCTION_LENGTH);
   return currentEstimate + self()->getEstimatedBinaryLength();
   }

uint8_t *TR::ARM64ImmInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   *(int32_t *)cursor = (int32_t)getSourceImmediate();
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64LabelInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   TR::LabelSymbol *label = getLabelSymbol();

   TR_ASSERT(false, "Not implemented yet.");

   return cursor;
   }

int32_t TR::ARM64LabelInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return currentEstimate + getEstimatedBinaryLength();
   }

uint8_t *TR::ARM64ConditionalBranchInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   TR::LabelSymbol *label = getLabelSymbol();

   TR_ASSERT(false, "Not implemented yet.");

   return cursor;
   }

int32_t TR::ARM64ConditionalBranchInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return currentEstimate + getEstimatedBinaryLength();
   }

uint8_t *TR::ARM64AdminInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();

   TR_ASSERT(false, "Not implemented yet.");

   setBinaryLength(0);
   setBinaryEncoding(instructionStart);

   return instructionStart;
   }

int32_t TR::ARM64AdminInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(0);
   return currentEstimate;
   }

uint8_t *TR::ARM64Trg1Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64Trg1ImmInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   insertImmediateField(toARM64Cursor(cursor));
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64Trg1Src1Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   insertSource1Register(toARM64Cursor(cursor));
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64Trg1Src1ImmInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   insertSource1Register(toARM64Cursor(cursor));
   insertImmediateField(toARM64Cursor(cursor));
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64Trg1Src2Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   insertSource1Register(toARM64Cursor(cursor));
   insertSource2Register(toARM64Cursor(cursor));
   cursor += ARM64_INSTRUCTION_LENGTH;
   setBinaryLength(ARM64_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::ARM64Trg1MemInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toARM64Cursor(cursor));
   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor - instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }

int32_t TR::ARM64Trg1MemInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(getMemoryReference()->estimateBinaryLength(*cg()));
   return currentEstimate + getEstimatedBinaryLength();
   }

uint8_t *TR::ARM64MemInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor - instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }

int32_t TR::ARM64MemInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(getMemoryReference()->estimateBinaryLength(*cg()));
   return(currentEstimate + getEstimatedBinaryLength());
   }

uint8_t *TR::ARM64MemSrc1Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertSource1Register(toARM64Cursor(cursor));
   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor - instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }

int32_t TR::ARM64MemSrc1Instruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(getMemoryReference()->estimateBinaryLength(*cg()));
   return(currentEstimate + getEstimatedBinaryLength());
   }

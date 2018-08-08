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

#include "codegen/MemoryReference.hpp"

#include <stddef.h>
#include <stdint.h>
#include "codegen/ARM64Instruction.hpp"
#include "codegen/CodeGenerator.hpp"


OMR::ARM64::MemoryReference::MemoryReference(
      TR::CodeGenerator *cg) :
   _baseRegister(NULL),
   _baseNode(NULL),
   _indexRegister(NULL),
   _indexNode(NULL),
   _unresolvedSnippet(NULL),
   _flag(0),
   _length(0),
   _scale(0)
   {
   _symbolReference = new (cg->trHeapMemory()) TR::SymbolReference(cg->comp()->getSymRefTab());
   _offset = _symbolReference->getOffset();
   }


OMR::ARM64::MemoryReference::MemoryReference(
      TR::Register *br,
      TR::Register *ir,
      TR::CodeGenerator *cg) :
   _baseRegister(br),
   _baseNode(NULL),
   _indexRegister(ir),
   _indexNode(NULL),
   _unresolvedSnippet(NULL),
   _flag(0),
   _length(0),
   _scale(0)
   {
   _symbolReference = new (cg->trHeapMemory()) TR::SymbolReference(cg->comp()->getSymRefTab());
   _offset = _symbolReference->getOffset();
   }


OMR::ARM64::MemoryReference::MemoryReference(
      TR::Register *br,
      int32_t disp,
      TR::CodeGenerator *cg) :
   _baseRegister(br),
   _baseNode(NULL),
   _indexRegister(NULL),
   _indexNode(NULL),
   _unresolvedSnippet(NULL),
   _flag(0),
   _length(0),
   _scale(0),
   _offset(disp)
   {
   _symbolReference = new (cg->trHeapMemory()) TR::SymbolReference(cg->comp()->getSymRefTab());
   }


bool OMR::ARM64::MemoryReference::useIndexedForm()
   {
   TR_ASSERT(false, "Not implemented yet.");

   return false;
   }


/* register offset */
static bool isRegisterOffsetInstruction(uint32_t enc)
   {
   return (enc & 0x3b200c00 == 0x38200800);
   }


/* post-index/pre-index/unscaled immediate offset */
static bool isImm9OffsetInstruction(uint32_t enc)
   {
   return (enc & 0x3b200000 == 0x38000000);
   }


/* unsigned immediate offset */
static bool isImm12OffsetInstruction(uint32_t enc)
   {
   return (enc & 0x3b200000 == 0x39000000);
   }


uint8_t *OMR::ARM64::MemoryReference::generateBinaryEncoding(TR::Instruction *currentInstruction, uint8_t *cursor, TR::CodeGenerator *cg)
   {
   uint32_t *wcursor = (uint32_t *)cursor;
   TR::RealRegister *base = self()->getBaseRegister() ? toRealRegister(self()->getBaseRegister()) : NULL;
   TR::RealRegister *index = self()->getIndexRegister() ? toRealRegister(self()->getIndexRegister()) : NULL;
   TR::RealRegister *target = toRealRegister(currentInstruction->getMemoryDataRegister());

   if (self()->getUnresolvedSnippet())
      {
      TR_ASSERT(false, "Not implemented yet.");
      }
   else
      {
      int32_t displacement = self()->getOffset();

      TR::InstOpCode op = currentInstruction->getOpCode();
      uint32_t enc = (uint32_t)op.getOpCodeBinaryEncoding();

      if (index)
         {
         TR_ASSERT(displacement == 0, "Non-zero offset with index register.");

         if (isRegisterOffsetInstruction(enc))
            {
            base->setRegisterFieldRN(wcursor);
            index->setRegisterFieldRM(wcursor);
            target->setRegisterFieldRT(wcursor);

            if (self()->getScale() != 0)
               {
               TR_ASSERT(false, "Not implemented yet.");
               }

            cursor += ARM64_INSTRUCTION_LENGTH;
            }
         else
            {
            TR_ASSERT(false, "Unsupported instruction type.");
            }
         }
      else
         {
         /* no index register */
         base->setRegisterFieldRN(wcursor);
         target->setRegisterFieldRT(wcursor);

         if (isImm9OffsetInstruction(enc))
            {
            if (constantIsImmed9(displacement))
               {
               *wcursor |= (displacement & 0x1ff) << 12; /* imm9 */
               cursor += ARM64_INSTRUCTION_LENGTH;
               }
            else
               {
               /* Need additional instructions for large offset */
               TR_ASSERT(false, "Not implemented yet.");
               }
            }
         else if (isImm12OffsetInstruction(enc))
            {
            int32_t size = (enc >> 30) & 3; /* b=0, h=1, w=2, x=3 */
            int32_t shifted = displacement >> size;

            if (size > 0)
               {
               TR_ASSERT((displacement & ((1 << size) - 1)) == 0, "Non-aligned offset in halfword memory access.");
               }

            if (constantIsUnsignedImmed12(shifted))
               {
               *wcursor |= (shifted & 0xfff) << 10; /* imm12 */
               cursor += ARM64_INSTRUCTION_LENGTH;
               }
            else
               {
               /* Need additional instructions for large offset */
               TR_ASSERT(false, "Not implemented yet.");
               }
            }
         else
            {
            /* Register pair, literal, exclusive instructions to be supported */
            TR_ASSERT(false, "Not implemented yet.");
            }
         }
      }

   return cursor;
   }


uint32_t OMR::ARM64::MemoryReference::estimateBinaryLength()
   {
   if (self()->getUnresolvedSnippet() != NULL)
      {
      TR_ASSERT(false, "Not implemented yet.");
      }
   else
      {
      if (self()->getIndexRegister())
         {
         return ARM64_INSTRUCTION_LENGTH;
         }
      else
         {
         TR_ASSERT(false, "Not implemented yet.");
         }
      }

   return 0;
   }

/*******************************************************************************
 * Copyright (c) 2019, 2020 IBM Corp. and others
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

#include <riscv.h>
#include "codegen/RVInstruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/AutomaticSymbol.hpp"
#include "il/ParameterSymbol.hpp"
#include "infra/Bit.hpp"

/*
 * Common helper to truncate integer values to 8 or 16 bits.
 */
static void truncate(TR::Node *node, TR::Register *reg, TR::CodeGenerator *cg)
   {
   int bitwidth = 0;

   if (node->getDataType().isInt8())
      {
      bitwidth = 8;
      }
   else if (node->getDataType().isInt16())
      {
      bitwidth = 16;
      }
   else
      {
      // No need to truncate, short-circuit
      return;
      }

   generateITYPE(TR::InstOpCode::_slli, node, reg, reg, 64 - bitwidth, cg);
   generateITYPE(TR::InstOpCode::_srai, node, reg, reg, 64 - bitwidth, cg);
   }


/*
 * Common helper for binary operation using R-type instruction or I-type
 * instruction. The I-type instruction is used when second operand is a load
 * constant and constant value fits into I-type instruction immediate,
 * Otherwise, R-type instruction is used.
 */
static TR::Register *RorIhelper(TR::Node *node, TR::InstOpCode::Mnemonic opr, TR::InstOpCode::Mnemonic opi, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg = cg->allocateRegister();

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL && opi != TR::InstOpCode::bad)
      {
      int64_t value = secondChild->getLongInt();
      if (VALID_ITYPE_IMM(value))
         {
         /*
          * We have to convert _sub (_subw) to _addi (_addiw) since there's no
          * _subi (_subiw)
          */
         if (opr == TR::InstOpCode::_sub || opr == TR::InstOpCode::_subw)
            {
            value = -value;
            }
         generateITYPE(opi, node, trgReg, src1Reg, value, cg);
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         generateRTYPE(opr, node, trgReg, src1Reg, src2Reg, cg);
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateRTYPE(opr, node, trgReg, src1Reg, src2Reg, cg);
      }

   truncate(node, trgReg, cg);

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

/*
 * Common helper for binary operation using R-type instruction.
 */
static TR::Register *Rhelper(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg = cg->allocateRegister();

   generateRTYPE(op, node, trgReg, src1Reg, src2Reg, cg);

   truncate(node, trgReg, cg);

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

// also handles badd and sadd
TR::Register *
OMR::RV::TreeEvaluator::iaddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_addw, TR::InstOpCode::_addiw, cg);
   }

// also handles aiadd and aladd
TR::Register *
OMR::RV::TreeEvaluator::laddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg = cg->allocateRegister();

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      int64_t value = secondChild->getLongInt();
      if (VALID_ITYPE_IMM(value))
         {
         generateITYPE(TR::InstOpCode::_addi, node, trgReg, src1Reg, value, cg);
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         generateRTYPE(TR::InstOpCode::_add, node, trgReg, src1Reg, src2Reg, cg);
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateRTYPE(TR::InstOpCode::_add, node, trgReg, src1Reg, src2Reg, cg);
      }

   if ((node->getOpCodeValue() == TR::aiadd || node->getOpCodeValue() == TR::aladd) &&
       node->isInternalPointer())
      {
      if (node->getPinningArrayPointer())
         {
         trgReg->setContainsInternalPointer();
         trgReg->setPinningArrayPointer(node->getPinningArrayPointer());
         }
      else
         {
         TR::Node *firstChild = node->getFirstChild();
         if ((firstChild->getOpCodeValue() == TR::aload) &&
             firstChild->getSymbolReference()->getSymbol()->isAuto() &&
             firstChild->getSymbolReference()->getSymbol()->isPinningArrayPointer())
            {
            trgReg->setContainsInternalPointer();

            if (!firstChild->getSymbolReference()->getSymbol()->isInternalPointer())
               {
               trgReg->setPinningArrayPointer(firstChild->getSymbolReference()->getSymbol()->castToAutoSymbol());
               }
            else
               trgReg->setPinningArrayPointer(firstChild->getSymbolReference()->getSymbol()->castToInternalPointerAutoSymbol()->getPinningArrayPointer());
            }
         else if (firstChild->getRegister() &&
                  firstChild->getRegister()->containsInternalPointer())
            {
            trgReg->setContainsInternalPointer();
            trgReg->setPinningArrayPointer(firstChild->getRegister()->getPinningArrayPointer());
            }
         }
      }

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
	}

// also handles bsub and ssub
TR::Register *
OMR::RV::TreeEvaluator::isubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_subw, TR::InstOpCode::_addiw, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::lsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_sub, TR::InstOpCode::_addi, cg);
   }

// also handles bmul, smul and lmul
TR::Register *
OMR::RV::TreeEvaluator::imulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg;
   bool is32bit = node->getDataType().isInt32();

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      int64_t value = secondChild->getLongInt();
      if (value > 0 && cg->convertMultiplyToShift(node))
         {
         // The multiply has been converted to a shift.
         trgReg = cg->evaluate(node);
         return trgReg;
         }
      else
         {
         trgReg = cg->allocateRegister();
         if (value == 0)
            {
            TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
            generateITYPE(TR::InstOpCode::_addi, node, trgReg, zero, 0, cg);
            }
         else if (value == 1)
            {
            generateITYPE(TR::InstOpCode::_addi, node, trgReg, src1Reg, 0, cg);
            }
         else if (value == -1)
            {
            TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
            generateRTYPE(is32bit ? TR::InstOpCode::_subw : TR::InstOpCode::_sub, node, trgReg, zero, src1Reg, cg);
            }
         else
            {
            TR::Register *src2Reg = cg->evaluate(secondChild);
            trgReg = cg->allocateRegister();
            generateRTYPE(is32bit ? TR::InstOpCode::_mulw : TR::InstOpCode::_mul, node, trgReg, src1Reg, src2Reg, cg);
            }
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();
      generateRTYPE(is32bit ? TR::InstOpCode::_mulw : TR::InstOpCode::_mul, node, trgReg, src1Reg, src2Reg, cg);
      }

   truncate(node, trgReg, cg);

   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *
OMR::RV::TreeEvaluator::imulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
	{
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *src1Reg = cg->evaluate(firstChild);
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *src2Reg = cg->evaluate(secondChild);
   TR::Register *trgReg = cg->allocateRegister();

   generateRTYPE(TR::InstOpCode::_mul,  node, trgReg, src1Reg, src2Reg, cg);
   generateITYPE(TR::InstOpCode::_srai, node, trgReg, trgReg,  32, cg);

   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   node->setRegister(trgReg);
   return trgReg;
	}

// also handles bdiv and sdiv
TR::Register *
OMR::RV::TreeEvaluator::idivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_divw, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::iudivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_divuw, cg);
   }

// also handles brem and srem
TR::Register *
OMR::RV::TreeEvaluator::iremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_remw, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::iuremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_remuw, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::ldivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_div, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::ludivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_divu, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::lremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return Rhelper(node, TR::InstOpCode::_rem, cg);
   }

static TR::Register *shiftHelper(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::InstOpCode::Mnemonic opi, TR::CodeGenerator *cg)
   {
   // TODO: if second arg is an int constant, shall we make mask it so
   // that we're sure the only low 5 bits are set? This is the assumption
   // under which we can use RorIhelper. Is this guaranteed at this level?
   return RorIhelper(node, op, opi, cg);
   }

// also handles bshl and sshl
TR::Register *
OMR::RV::TreeEvaluator::ishlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sllw, TR::InstOpCode::_slliw, cg);
   }

// also handles bshr and sshr
TR::Register *
OMR::RV::TreeEvaluator::ishrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sraw, TR::InstOpCode::_sraiw, cg);
   }

// also handles bushr, sushr and lushr
TR::Register *
OMR::RV::TreeEvaluator::iushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();

   TR::Register *src1Reg;
   TR::Register *trgReg;

   int width = TR::DataType::getSize(node->getDataType()) * 8;

   if (secondChild->getOpCode().isLoadConst())
      {
      /*
       * If shift amount is a constant value, prefer srli / srliw.
       *
       * Only low bits 6 bits (5 for Int32, 4 for Int16, 3 for Int8)
       * are considered, higher bits are ignored. This is to be consistent
       * with behavior of srl / srlw.
       *
       * If data width is 8 or 16 bits, we have to truncate the value.
       * We do this by combining the requested right shift by a constant
       * with "truncation" right shift, for example:
       *
       *   For...
       *
       *     (bushr
       *        (<src1>)
       *        (iconst 5)
       *
       *   ...we generate:
       *
       *       slliw trg, src1, 24   ;; 24 = 32(reg width) - 8(data width)
       *       srliw trg, trg,  29   ;; 16 = 32(reg width) - 8(data width) + 5(requested shift amount)
       *
       * Finally, if constant is 0, we just pass-through the source value.
       */
      int64_t shamt = secondChild->getLongInt() & (width - 1);
      if (shamt != 0)
         {
         src1Reg = cg->evaluate(firstChild);
         trgReg = cg->allocateRegister(TR_GPR);

         if (width < 32)
            {
            generateITYPE(TR::InstOpCode::_slliw, node, trgReg, src1Reg, 32 - width,         cg);
            generateITYPE(TR::InstOpCode::_srliw, node, trgReg, trgReg,  32 - width + shamt, cg);
            }
         else
            {
            generateITYPE(width == 64 ? TR::InstOpCode::_srli : TR::InstOpCode::_srliw, node, trgReg, src1Reg, shamt, cg);
            }
         }
      else
         {
         trgReg = cg->evaluate(firstChild);
         }
      }
   else
      {
      /*
       * If shift amount is not a constant, use srl / srlw.
       *
       * Only low bits 6 bits (5 for Int32, 4 for Int16, 3 for Int8)
       * are considered, higher bits are ignored.
       *
       * If data width is 8 or 16 bits, we have to truncate the value
       * before doing the requested right shift (to get rid of ones
       * in higher 24 or 16 bits in case sign bit is set. If data width
       * is 8bits, use andi rather than shift left, shift right - this
       * save us one instruction.
       *
       * We also have to first clear high 57bits (49 for Int16) of shift
       * amount.
       *
       * Finally, after the actual shift, we truncate again - this is
       * for case the shift amount happens to be zero and value sign bit
       * is set (and then cleared by first truncation).
       */

      TR::Register *src2Reg = nullptr;

      src1Reg = cg->evaluate(firstChild);
      trgReg = cg->allocateRegister(TR_GPR);

      if (width < 32)
         {
         /*
          * Truncate value (src1Reg). We use trgReg to temporarily hold truncated
          * value.
          */
         if (width == 8)
            {
            generateITYPE(TR::InstOpCode::_andi, node, trgReg, src1Reg, 0xFF, cg);
            }
         else
            {
            generateITYPE(TR::InstOpCode::_slliw, node, trgReg, src1Reg, 16, cg);
            generateITYPE(TR::InstOpCode::_srliw, node, trgReg, trgReg, 16, cg);
            }
         src1Reg = trgReg;

         /*
          * And clear high 57bits (49 for Int16) of shift amount and store
          * into src2Reg.
          */
         src2Reg = cg->allocateRegister(TR_GPR);
         generateITYPE(TR::InstOpCode::_andi, node, src2Reg, cg->evaluate(secondChild), width-1, cg);
         }
      else
         {
         src2Reg = cg->evaluate(secondChild);
         }

      generateRTYPE(width == 64 ? TR::InstOpCode::_srl : TR::InstOpCode::_srlw, node, trgReg, src1Reg, src2Reg, cg);
      truncate(node, trgReg, cg);
      }

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

// also handles lrol
TR::Register *
OMR::RV::TreeEvaluator::irolEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR::Register *trgReg = cg->gprClobberEvaluate(firstChild);
   TR::Register *tmpReg = cg->allocateRegister();
   bool is64bit = node->getDataType().isInt64();
   TR::InstOpCode::Mnemonic op;

   /*
    * RISC-V Base Integer ISA lacks a native rotate instruction, so here we need
    * to use two shifts and or to implement a rotate, like (for 64bit):
    *
    *   sll tmp, src, shift
    *   srl dst, src, 64 - shift,
    *   or  dst, dst, tmp
    *
    * For 32bit, we must use .w instructions plus sign-extend the result with
    *
    *   sext.w dst  // aka addi.w dst, dst, 0
    *
    * TODO: in future, we may check for presence of B extension and use 'rot'
    * instruction (if it finds its way to B extensions)
    */
   if (secondChild->getOpCode().isLoadConst())
      {
      int32_t value = secondChild->getInt();
      uint32_t shift = is64bit ? (value & 0x3F) : (value & 0x1F);

      if (shift != 0)
         {
         if (is64bit)
            {
            generateITYPE(TR::InstOpCode::_slli, node, tmpReg, trgReg, shift, cg);
            generateITYPE(TR::InstOpCode::_srli, node, trgReg, trgReg, 64-shift, cg);
            generateRTYPE(TR::InstOpCode::_or,   node, trgReg, trgReg, tmpReg, cg);
            }
         else
            {
            generateITYPE(TR::InstOpCode::_slliw, node, tmpReg, trgReg, shift, cg);
            generateITYPE(TR::InstOpCode::_srliw, node, trgReg, trgReg, 32-shift, cg);
            generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
            generateITYPE(TR::InstOpCode::_addiw, node, trgReg, trgReg, 0, cg);
            }
         }
      }
   else
      {
      TR::Register *shift1Reg = cg->evaluate(secondChild);
      TR::Register *shift2Reg = cg->allocateRegister();
      TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);

      generateRTYPE(TR::InstOpCode::_subw, node, shift2Reg, zero, shift1Reg, cg);
      if (is64bit)
         {
         generateRTYPE(TR::InstOpCode::_sll,   node, tmpReg, trgReg, shift1Reg, cg);
         generateRTYPE(TR::InstOpCode::_srl,   node, trgReg, trgReg, shift2Reg, cg);
         generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
         }
      else
         {
         generateRTYPE(TR::InstOpCode::_sllw,  node, tmpReg, trgReg, shift1Reg, cg);
         generateRTYPE(TR::InstOpCode::_srlw,  node, trgReg, trgReg, shift2Reg, cg);
         generateRTYPE(TR::InstOpCode::_or,    node, trgReg, trgReg, tmpReg, cg);
         generateITYPE(TR::InstOpCode::_addiw, node, trgReg, trgReg, 0, cg);
         }
      cg->stopUsingRegister(shift2Reg);
      }
   cg->stopUsingRegister(tmpReg);

   node->setRegister(trgReg);
   firstChild->decReferenceCount();
   secondChild->decReferenceCount();
   return trgReg;
   }

TR::Register *
OMR::RV::TreeEvaluator::lshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sll, TR::InstOpCode::_slli, cg);
   }

TR::Register *
OMR::RV::TreeEvaluator::lshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return shiftHelper(node, TR::InstOpCode::_sra, TR::InstOpCode::_srai, cg);
   }

// also handles band, sand and land
TR::Register *
OMR::RV::TreeEvaluator::iandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_and, TR::InstOpCode::_andi, cg);
   }

// also handles bor, sor and lor
TR::Register *
OMR::RV::TreeEvaluator::iorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_or, TR::InstOpCode::_ori, cg);
   }

// also handles bxor, sxor and lxor
TR::Register *
OMR::RV::TreeEvaluator::ixorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return RorIhelper(node, TR::InstOpCode::_xor, TR::InstOpCode::_xori, cg);
   }

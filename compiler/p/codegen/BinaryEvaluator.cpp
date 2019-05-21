/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
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

#include <stdint.h>
#include <stdlib.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/CodeGeneratorUtils.hpp"
#include "codegen/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Machine.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterPair.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "codegen/PPCEvaluator.hpp"
#include "compile/Compilation.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "env/CompilerEnv.hpp"
#include "env/Processors.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/Symbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/symbol/AutomaticSymbol.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "infra/Assert.hpp"
#include "infra/Bit.hpp"
#include "p/codegen/GenerateInstructions.hpp"
#include "p/codegen/PPCInstruction.hpp"
#include "runtime/Runtime.hpp"

extern TR::Register *addConstantToInteger(TR::Node * node, TR::Register *srcReg, int32_t value, TR::CodeGenerator *cg);
extern TR::Register *addConstantToLong(TR::Node * node, TR::Register *srcReg, int64_t value, TR::Register *trgReg, TR::CodeGenerator *cg);
static void mulConstant(TR::Node *, TR::Register *trgReg, TR::Register *sourceReg, int32_t value, TR::CodeGenerator *cg);
static void mulConstant(TR::Node *, TR::Register *trgReg, TR::Register *sourceReg, int64_t value, TR::CodeGenerator *cg);

extern TR::Register *inlineIntegerRotateLeft(TR::Node *node, TR::CodeGenerator *cg);
extern TR::Register *inlineLongRotateLeft(TR::Node *node, TR::CodeGenerator *cg);

static TR::Register *ldiv64Evaluator(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *lrem64Evaluator(TR::Node *node, TR::CodeGenerator *cg);

void generateZeroExtendInstruction(TR::Node *node,
                                   TR::Register *trgReg,
                                   TR::Register *srcReg,
                                   int32_t bitsInTarget,
                                   TR::CodeGenerator *cg)
   {
   TR_ASSERT((TR::Compiler->target.is64Bit() && bitsInTarget > 0 && bitsInTarget < 64) ||
          (TR::Compiler->target.is32Bit() && bitsInTarget > 0 && bitsInTarget < 32),
          "invalid zero extension requested");
   int64_t mask = (uint64_t)(CONSTANT64(0xffffFFFFffffFFFF)) >> (64 - bitsInTarget);
   generateTrg1Src1Imm2Instruction(cg, TR::Compiler->target.is64Bit() ? TR::InstOpCode::rldicl : TR::InstOpCode::rlwinm, node, trgReg, srcReg, 0, mask);
   }

void generateSignExtendInstruction(TR::Node *node,
                                   TR::Register *trgReg,
                                   TR::Register *srcReg,
                                   TR::CodeGenerator *cg)
   {
   int32_t operandSize = node->getOpCode().getSize();
   TR::InstOpCode::Mnemonic signExtendOp = TR::InstOpCode::bad;
   switch (operandSize)
      {
      case 0x1:
         signExtendOp = TR::InstOpCode::extsb;
         break;
      case 0x2:
         signExtendOp = TR::InstOpCode::extsh;
         break;
      case 0x4:
         signExtendOp = TR::InstOpCode::extsw;
         break;
      default:
         TR_ASSERT(0,"unexpected operand size %d\n",operandSize);
         break;
      }
   generateTrg1Src1Instruction(cg, signExtendOp, node, trgReg, srcReg);
   }


// Do the work for evaluating integer or and exclusive or
// Also called for long or and exclusive or when the upper
// 32 bits of an immediate operand are known to be zero.
static inline TR::Register *iorTypeEvaluator(TR::Node *node,
                                            TR::InstOpCode::Mnemonic immedOp,
                                            TR::InstOpCode::Mnemonic immedShiftedOp,
                                            TR::InstOpCode::Mnemonic regOp,
                                            TR::InstOpCode::Mnemonic regOp_r,
                                            TR::CodeGenerator *cg)
   {
   TR::Register *trgReg         = cg->allocateRegister();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *src1Reg        = cg->evaluate(firstChild);
   int32_t      immValue;
   TR::ILOpCodes secondOp            = secondChild->getOpCodeValue();

   if (secondChild->getOpCode().isLoadConst() &&
       secondChild->getRegister() == NULL)
      {
      if (secondOp == TR::lconst || secondOp == TR::luconst)
         immValue = (int32_t)secondChild->getLongInt(); // upper 32 bits known to be zero
      else
         immValue = secondChild->get64bitIntegralValue();

      intParts localVal(immValue);
      TR::Node *firstChild = node->getFirstChild();
      if (localVal.getValue()==-1 && (node->getOpCodeValue()==TR::ixor))
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, trgReg, src1Reg, -1);
         }
      else if (localVal.getHighBits() == 0)
         {
         generateTrg1Src1ImmInstruction(cg, immedOp, node, trgReg, src1Reg, localVal.getLowBits());
         }
      else if (localVal.getLowBits() == 0)
         {
         generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg, src1Reg, localVal.getHighBits());
         }
      else
         {
         TR::Register *tempReg = cg->allocateRegister();
         generateTrg1Src1ImmInstruction(cg, immedOp, node, tempReg, src1Reg, localVal.getLowBits());
         generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg, tempReg, localVal.getHighBits());
         cg->stopUsingRegister(tempReg);
         }
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateTrg1Src2Instruction(cg, regOp, node, trgReg, src1Reg, src2Reg);
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

static bool genNullTestForCompressedPointers(TR::Node *node,
                                             TR::Register * &trgReg,
                                             TR::Register * &condReg,
                                             TR::Register *src1Reg,
                                             TR::Register * &src2Reg,
                                             TR::LabelSymbol * &doneSkipAdd,
                                             TR::CodeGenerator *cg)
   {
   if (cg->comp()->useCompressedPointers() &&
         node->containsCompressionSequence())
      {
      TR::Node *n = node;
      bool isNonZero = false;
      bool keepSrc1 = true;
      static bool useBranchless = feGetEnv("TR_UseBranchless") ? true : false;
      if (n->isNonZero())
         isNonZero = true;

      if (n->getOpCodeValue() == TR::ladd || n->getOpCodeValue() == TR::lsub)
         {
         if (n->getFirstChild()->isNonZero())
            isNonZero = true;

         if (n->getFirstChild()->getOpCodeValue() == TR::iu2l ||
             n->getFirstChild()->getOpCodeValue() == TR::a2l ||
             n->getFirstChild()->getOpCode().isShift() )
            {
            if (n->getFirstChild()->getFirstChild()->isNonZero())
               isNonZero = true;
            if (n->getFirstChild()->getReferenceCount() == 1 &&  n->getOpCodeValue() != TR::lsub)
               // need to investigate for TR::lsub as we might need to keep both
               // compressed and noncompressed regs alive for writebarriers etc
               keepSrc1 = false;
            }
         }

      if (keepSrc1)
         trgReg = cg->allocateRegister();
      else
         trgReg = src1Reg;
      if (!isNonZero && !useBranchless)
         {
         TR::LabelSymbol *startSkipAdd = NULL;
         // generate the null test,
         // the adds will be generated below
         //
         condReg = cg->allocateRegister(TR_CCR);
         startSkipAdd = generateLabelSymbol(cg);
         doneSkipAdd = generateLabelSymbol(cg);
         startSkipAdd->setStartInternalControlFlow();
         doneSkipAdd->setEndInternalControlFlow();
         generateLabelInstruction(cg, TR::InstOpCode::label, node, startSkipAdd);
         if (keepSrc1)
            // Initialize the trgReg with 0 in case the src1Reg was null, if it was not then the add
            // would store the correct value in trgReg
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
         }
      if (!node->getSecondChild()->getOpCode().isLoadConst() ||
            node->getSecondChild()->getRegister())
         src2Reg = cg->evaluate(node->getSecondChild());
      if (!isNonZero && !useBranchless)
         {
         if (n->getFirstChild()->getOpCode().isShift() && n->getFirstChild()->getFirstChild()->getRegister())
            {
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpli8, node, condReg,
                                           n->getFirstChild()->getFirstChild()->getRegister(), NULLVALUE);
            }
         else
            {
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpli8, node, condReg, src1Reg, NULLVALUE);
            }
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, doneSkipAdd, condReg);
         }
      return true;
      }
   return false;
   }

// Also handles TR::aiadd, TR::iuadd, aiuadd
TR::Register *OMR::Power::TreeEvaluator::iaddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *src1Reg = NULL;
   TR::Register *trgReg = NULL;
   TR::Node     *secondChild = node->getSecondChild();

   TR::Node *firstChild = node->getFirstChild();
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

  if (TR::Compiler->target.cpu.id() >= TR_PPCp9 &&
      (firstChild->getOpCodeValue() == TR::imul || firstChild->getOpCodeValue() == TR::iumul) &&
      firstChild->getReferenceCount() == 1 &&
      firstChild->getRegister() == NULL)
     {
     trgReg = cg->allocateRegister();
     TR::Register *src2Reg = cg->evaluate(secondChild);
     TR::Register *mulSrc1Reg = cg->evaluate(firstChild->getFirstChild());
     TR::Register *mulSrc2Reg = cg->evaluate(firstChild->getSecondChild());

     generateTrg1Src3Instruction(cg, TR::InstOpCode::maddld, node, trgReg, mulSrc1Reg, mulSrc2Reg, src2Reg);

     cg->decReferenceCount(firstChild->getFirstChild());
     cg->decReferenceCount(firstChild->getSecondChild());
     }
  else
     {
      src1Reg = cg->evaluate(firstChild);
      if ((secondOp == TR::iconst || secondOp == TR::iuconst) &&
          secondChild->getRegister() == NULL)
         {
         trgReg = addConstantToInteger(node, src1Reg, secondChild->getInt(), cg);
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         trgReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, src1Reg, src2Reg);
         }
     }

   if ((node->getOpCodeValue() == TR::aiadd || node->getOpCodeValue() == TR::aiuadd) &&
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
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }



TR::RegisterPair *addConstantToLong(TR::Node * node, TR::Register *srcHighReg, TR::Register *srclowReg,
                               int32_t highValue, int32_t lowValue,
                               TR::CodeGenerator *cg)
   {
   TR::Register *lowReg  = cg->allocateRegister();
   TR::Register *highReg = cg->allocateRegister();
   TR::RegisterPair *trgReg = cg->allocateRegisterPair(lowReg, highReg);

   if (lowValue >= 0 && lowValue <= UPPER_IMMED)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, lowReg, srclowReg, lowValue);
      }
   else if (lowValue >= LOWER_IMMED && lowValue < 0)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, lowReg, srclowReg, lowValue);
      }
   else   // constant won't fit
      {
      TR::Register *lowValueReg = cg->allocateRegister();
      loadConstant(cg, node, lowValue, lowValueReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::addc, node, lowReg, srclowReg, lowValueReg);
      cg->stopUsingRegister(lowValueReg);
      }
   if (highValue == 0)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, srcHighReg);
      }
   else if (highValue == -1)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::addme, node, highReg, srcHighReg);
      }
   else
      {
      TR::Register *highValueReg = cg->allocateRegister();
      loadConstant(cg, node, highValue, highValueReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::adde, node, highReg, srcHighReg, highValueReg);
      cg->stopUsingRegister(highValueReg);
      }
   return trgReg;
   }


static void genericLongAnalyzer(
   TR::CodeGenerator* cg,
   TR::Node*& child,
   TR::Register*& lowReg, TR::Register*& highReg,
   bool& highZero)
   {
   TR::Register *reg = child->getRegister();

   bool useHighOrder = false;

   if (child->isHighWordZero())
      {
      highZero = true;
      TR::ILOpCodes firstOp = child->getOpCodeValue();
      if (child->getReferenceCount() == 1 && reg == NULL)
         {
         if (firstOp == TR::iu2l || firstOp == TR::su2l ||
             (firstOp == TR::lushr &&
              (child->getSecondChild()->getOpCodeValue() == TR::iconst ||
               child->getSecondChild()->getOpCodeValue() == TR::iuconst) &&
              (child->getSecondChild()->getInt() & LONG_SHIFT_MASK) == 32))
            {
            child = child->getFirstChild();
            if (firstOp == TR::lushr)
               {
               useHighOrder = true;
               }
            }
         }
      }

   reg = cg->evaluate(child);

   if (!highZero)
      {
      lowReg = reg->getLowOrder();
      highReg = reg->getHighOrder();
      }
   else
      {
      if (reg->getRegisterPair())
         {
         if (useHighOrder)
            {
            lowReg = reg->getHighOrder();
            }
         else
            {
            lowReg = reg->getLowOrder();
            }
         }
      else
         lowReg = reg;

      highReg = 0;
      }
   }

static TR::Register *carrylessLongEvaluatorWithAnalyser(TR::Node *node, TR::CodeGenerator *cg,
                                                       TR::InstOpCode::Mnemonic lowRegRegOpCode,
                                                       TR::InstOpCode::Mnemonic highRegRegOpCode,
                                                       TR::InstOpCode::Mnemonic copyRegRegOpCode)
   {
   TR::Register *src1Low = NULL;
   TR::Register *src1High = NULL;
   TR::Register *src2Low = NULL;
   TR::Register *src2High = NULL;
   TR::Register *lowReg, *highReg, *trgReg;
   bool firstHighZero = false, secondHighZero = false;
   TR::Node* firstChild = node->getFirstChild();
   TR::Node* secondChild = node->getSecondChild();

   genericLongAnalyzer(cg, firstChild, src1Low, src1High, firstHighZero);
   genericLongAnalyzer(cg, secondChild, src2Low, src2High, secondHighZero);

   lowReg  = cg->allocateRegister();
   highReg = cg->allocateRegister();
   trgReg = cg->allocateRegisterPair(lowReg, highReg);

   generateTrg1Src2Instruction(cg, lowRegRegOpCode, node, lowReg, src1Low, src2Low);

   if (firstHighZero)
      {
      if (secondHighZero || node->getOpCodeValue() == TR::land)
         {
         loadConstant(cg, node, 0, highReg);
         }
      else
        {
        generateTrg1Src1Instruction(cg, copyRegRegOpCode, node, highReg, src2High);
        }
      }
   else if (secondHighZero)
      {
      if (node->getOpCodeValue() == TR::land)
         {
         loadConstant(cg, node, 0, highReg);
         }
      else
         {
         generateTrg1Src1Instruction(cg, copyRegRegOpCode, node, highReg, src1High);
         }
      }
   else
      {
      generateTrg1Src2Instruction(cg, highRegRegOpCode, node, highReg, src1High, src2High);
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
   }

static TR::Register *laddEvaluatorWithAnalyser(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *src1Low = NULL;
   TR::Register *src1High = NULL;
   TR::Register *src2Low = NULL;
   TR::Register *src2High = NULL;
   TR::Register *lowReg, *highReg, *trgReg;
   bool firstHighZero = false, secondHighZero = false;
   TR::Node* firstChild = node->getFirstChild();
   TR::Node* secondChild = node->getSecondChild();

   genericLongAnalyzer(cg, firstChild, src1Low, src1High, firstHighZero);
   genericLongAnalyzer(cg, secondChild, src2Low, src2High, secondHighZero);

   lowReg  = cg->allocateRegister();
   highReg = cg->allocateRegister();
   trgReg = cg->allocateRegisterPair(lowReg, highReg);

   generateTrg1Src2Instruction(cg, TR::InstOpCode::addc, node, lowReg, src1Low, src2Low);

   if (firstHighZero)
      {
      if (secondHighZero)
         {
         loadConstant(cg, node, 0, highReg);
         generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, highReg);
         }
      else
        {
        generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, src2High);
        }
      }
   else if (secondHighZero)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, src1High);
      }
   else
      {
      generateTrg1Src2Instruction(cg, TR::InstOpCode::adde, node, highReg, src1High, src2High);
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
   }

// Also handles TR::aladd for 64 bit target, luadd, aluadd
TR::Register *OMR::Power::TreeEvaluator::laddEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node     *secondChild = node->getSecondChild();
   TR::Register *src1Reg = NULL;
   TR::Register *trgReg = NULL;
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();
   bool setsOrReadsCC = NEED_CC(node) || (node->getOpCodeValue() == TR::luaddc);
   TR::InstOpCode::Mnemonic regToRegOpCode = TR::InstOpCode::addc;

   if (TR::Compiler->target.is32Bit())
      {
      if (!setsOrReadsCC && (secondOp == TR::lconst || secondOp == TR::luconst) &&
          secondChild->getRegister() == NULL)
         {
         src1Reg = cg->evaluate(firstChild);
         trgReg = addConstantToLong(node, src1Reg->getHighOrder(), src1Reg->getLowOrder(),
                                   secondChild->getLongIntHigh(), secondChild->getLongIntLow(), cg);
         }
      else
         {
         if (!setsOrReadsCC && (firstChild->isHighWordZero() || secondChild->isHighWordZero()))
            {
            return laddEvaluatorWithAnalyser(node, cg);
            }
         else
            {
            TR::Register *lowReg  = cg->allocateRegister();
            TR::Register *highReg = cg->allocateRegister();
            trgReg = cg->allocateRegisterPair(lowReg, highReg);
            TR::Register  *src2Reg    = cg->evaluate(secondChild);
            src1Reg = cg->evaluate(firstChild);
            TR::Register *carryReg = NULL;
            if ((node->getOpCodeValue() == TR::luaddc) && TR_PPCComputeCC::setCarryBorrow(node->getChild(2), false, &carryReg, cg))
               {
               // use adde rather than addc
               regToRegOpCode = TR::InstOpCode::adde;
               }
            generateTrg1Src2Instruction(cg, regToRegOpCode, node, lowReg, src1Reg->getLowOrder(), src2Reg->getLowOrder());
            generateTrg1Src2Instruction(cg, TR::InstOpCode::adde, node, highReg, src1Reg->getHighOrder(), src2Reg->getHighOrder());
            }
         }
      }
   else // 64 bit target
      {

      bool hasCompressedPointers = false;
      static bool useBranchless = feGetEnv("TR_UseBranchless") ? true : false;
      TR::Register *condReg = NULL;
      TR::LabelSymbol *doneSkipAdd = NULL;
      TR::Register *src2Reg = NULL;
      hasCompressedPointers = genNullTestForCompressedPointers(node, trgReg, condReg, src1Reg, src2Reg, doneSkipAdd, cg);

      if ( useBranchless && hasCompressedPointers )
         {
         TR::Register * decomprReg = src1Reg;
         if (node->getFirstChild()->getOpCode().isShift() && node->getFirstChild()->getFirstChild()->getRegister())
            {
            decomprReg = node->getFirstChild()->getFirstChild()->getRegister();
            }
         TR::Register* tReg = src1Reg != trgReg ? trgReg : cg->allocateRegister();
         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, tReg, decomprReg);
         generateTrg1Src1ImmInstruction (cg, TR::InstOpCode::sradi,  node, tReg, tReg, 63);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, tReg, tReg, src2Reg);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, tReg, src1Reg);
         node->setRegister(trgReg);
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         if (src1Reg == trgReg) cg->stopUsingRegister(tReg);
         return trgReg;
         }

      if (TR::Compiler->target.cpu.id() >= TR_PPCp9 &&
          !setsOrReadsCC &&
          (node->getOpCodeValue() == TR::ladd || node->getOpCodeValue() == TR::aladd) &&
          firstChild->getOpCodeValue() == TR::lmul &&
          firstChild->getReferenceCount() == 1 &&
          firstChild->getRegister() == NULL)
         {
         trgReg = cg->allocateRegister();
         src2Reg = cg->evaluate(secondChild);
         TR::Register *lmulSrc1Reg = cg->evaluate(firstChild->getFirstChild());
         TR::Register *lmulSrc2Reg = cg->evaluate(firstChild->getSecondChild());

         generateTrg1Src3Instruction(cg, TR::InstOpCode::maddld, node, trgReg, lmulSrc1Reg, lmulSrc2Reg, src2Reg);

         cg->decReferenceCount(firstChild->getFirstChild());
         cg->decReferenceCount(firstChild->getSecondChild());
         }
      else
         {
         src1Reg = cg->evaluate(firstChild);

         if (!setsOrReadsCC &&
             (secondOp == TR::lconst || secondOp == TR::luconst) &&
             secondChild->getRegister() == NULL)
            {
            trgReg = addConstantToLong(node, src1Reg, secondChild->getLongInt(), trgReg, cg);
            }
         // might not be true for aladd, since secondchild of the ladd is made
         // to be an lconst
         else if (!setsOrReadsCC &&
                  (secondOp == TR::iconst || secondOp == TR::iuconst) && // may be true if aladd?
                  secondChild->getRegister() == NULL)
            {
            trgReg = addConstantToLong(node, src1Reg, (int64_t)secondChild->getInt(), trgReg, cg);
            }
         else
            {
            if (!hasCompressedPointers)
               {
               src2Reg = cg->evaluate(secondChild);
               trgReg = cg->allocateRegister();
               }

            if (setsOrReadsCC)
               {
               TR_ASSERT(node->getOpCodeValue() == TR::ladd || node->getOpCodeValue() == TR::luadd || node->getOpCodeValue() == TR::luaddc,
                  "CC computation not supported for this node %p\n", node);
               TR::Register *carryReg = NULL;
               if ((node->getOpCodeValue() == TR::luaddc) && TR_PPCComputeCC::setCarryBorrow(node->getChild(2), false, &carryReg, cg))
                  {
                  // Currently, only the path that calculates the CC handles addc.  This is fine
                  // since the simplifier will lower addc to add in all other cases.
                  //
                  // use adde rather than addc
                  regToRegOpCode = TR::InstOpCode::adde;
                  }

               generateTrg1Src2Instruction(cg, regToRegOpCode, node, trgReg, src1Reg, src2Reg);
               }
            else
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, src1Reg, src2Reg);
               }
            }
         }

      if (hasCompressedPointers && doneSkipAdd)
         {
         int32_t numDeps = src2Reg ? 2 : 1;
         TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, numDeps, cg->trMemory());
         deps->addPostCondition(trgReg, TR::RealRegister::NoReg);
         if (src2Reg)
            deps->addPostCondition(src2Reg, TR::RealRegister::NoReg);
         cg->stopUsingRegister(condReg);
         generateDepLabelInstruction(cg, TR::InstOpCode::label, node, doneSkipAdd, deps);
         }

      if ((node->getOpCodeValue() == TR::aladd || node->getOpCodeValue() == TR::aluadd) &&
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
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }


// aiaddEvaluator handled by iaddEvaluator
// also handles TR::iusub and TR::asub
TR::Register *OMR::Power::TreeEvaluator::isubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild = node->getFirstChild();
   TR::Register *trgReg = NULL;
   TR::Register *src1Reg = NULL;
   int32_t value;
   TR::ILOpCodes firstOp = firstChild->getOpCodeValue();
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      src1Reg = cg->evaluate(firstChild);
      value = secondChild->getInt();
      trgReg = addConstantToInteger(node, src1Reg , -value, cg);
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);

      if (firstChild->getOpCode().isLoadConst() && firstChild->getRegister() == NULL)
         {
         trgReg = cg->allocateRegister();
         value = firstChild->getInt();
         if (value >= LOWER_IMMED && value <= UPPER_IMMED)
            {
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, trgReg, src2Reg, value);
            }
         else   // constant won't fit
            {
            src1Reg = cg->evaluate(firstChild);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, src2Reg, src1Reg);
            }
         }
      else  // no constants
         {
         src1Reg = cg->evaluate(firstChild);
         if (src1Reg->containsInternalPointer() ||
             !src1Reg->containsCollectedReference())
            {
            trgReg = cg->allocateRegister();
            if (src1Reg->containsInternalPointer())
               {
               trgReg->setPinningArrayPointer(src1Reg->getPinningArrayPointer());
               trgReg->setContainsInternalPointer();
               }
            }
         else
            trgReg = cg->allocateCollectedReferenceRegister();

         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, src2Reg, src1Reg);
         }
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::asubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   if (TR::Compiler->target.is64Bit())
      return TR::TreeEvaluator::lsubEvaluator(node, cg);
   else
      return TR::TreeEvaluator::isubEvaluator(node, cg);
   }

// also handles TR::asub  in 64-bit mode
TR::Register *lsub64Evaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild = node->getFirstChild();
   TR::Register *trgReg = NULL;
   TR::Register *src1Reg = NULL;
   int64_t value;
   TR::ILOpCodes firstOp = firstChild->getOpCodeValue();
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

   static bool useBranchless = feGetEnv("TR_UseBranchless") ? true : false;
   bool setsOrReadsCC = NEED_CC(node) || (node->getOpCodeValue() == TR::lusubb);
   TR::InstOpCode::Mnemonic regToRegOpCode = TR::InstOpCode::subfc;

   if (cg->comp()->useCompressedPointers() &&
         node->containsCompressionSequence())
      {
      src1Reg = cg->evaluate(firstChild);
      TR::Register *condReg = NULL;
      TR::Register *src2Reg = NULL;
      TR::LabelSymbol *doneSkipSub = NULL;
      genNullTestForCompressedPointers(node, trgReg, condReg, src1Reg, src2Reg, doneSkipSub, cg);

      if (useBranchless)
         {
         TR::Register * decomprReg = src1Reg;
         if (node->getFirstChild()->getOpCode().isShift() && node->getFirstChild()->getFirstChild()->getRegister())
            {
            decomprReg = node->getFirstChild()->getFirstChild()->getRegister();
            }
         TR::Register* tReg = src1Reg != trgReg ? trgReg : cg->allocateRegister();

         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, tReg, decomprReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::sradi,  node, tReg, tReg, 63);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, tReg, tReg, src2Reg);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tReg, src1Reg);

         node->setRegister(trgReg);
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         if (src1Reg == trgReg) cg->stopUsingRegister(tReg);

         return trgReg;
         }


      if (secondChild->getOpCode().isLoadConst() &&
            secondChild->getRegister() == NULL)
         trgReg = addConstantToLong(node, src1Reg, -secondChild->getLongInt(), trgReg, cg);
      else
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, src2Reg, src1Reg);

      if (doneSkipSub)
         {
         int32_t numDeps = src2Reg ? 2 : 1;
         TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, numDeps, cg->trMemory());
         deps->addPostCondition(trgReg, TR::RealRegister::NoReg);
         if (src2Reg)
            deps->addPostCondition(src2Reg, TR::RealRegister::NoReg);
         cg->stopUsingRegister(condReg);
         generateDepLabelInstruction(cg, TR::InstOpCode::label, node, doneSkipSub, deps);
         }
      }
   else
      {
      if (!setsOrReadsCC &&
          (secondChild->getOpCode().isLoadConst()) &&
         secondChild->getRegister() == NULL)
         {
         src1Reg = cg->evaluate(firstChild);
         value = secondChild->getLongInt();
         trgReg = addConstantToLong(node, src1Reg , -value, NULL, cg);
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);

         if (!setsOrReadsCC &&
             (firstChild->getOpCode().isLoadConst()) &&
             firstChild->getRegister() == NULL)
            {
            trgReg = cg->allocateRegister();
            value = firstChild->getLongInt();
            if (value >= LOWER_IMMED && value <= UPPER_IMMED)
               {
               generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, trgReg, src2Reg, value);
               }
            else   // constant won't fit
               {
               src1Reg = cg->evaluate(firstChild);
               generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, src2Reg, src1Reg);
               }
            }
         else  // no constants or setsOrReadsCC
            {
            src1Reg = cg->evaluate(firstChild);
            if (src1Reg->containsInternalPointer() ||
                !src1Reg->containsCollectedReference())
               {
               trgReg = cg->allocateRegister();
               if (src1Reg->containsInternalPointer())
                  {
                  trgReg->setPinningArrayPointer(src1Reg->getPinningArrayPointer());
                  trgReg->setContainsInternalPointer();
                  }
               }
            else
               trgReg = cg->allocateCollectedReferenceRegister();

            if (setsOrReadsCC)
               {
               TR_ASSERT(node->getOpCodeValue() == TR::lsub || node->getOpCodeValue() == TR::lusub || node->getOpCodeValue() == TR::lusubb,
                  "CC computation not supported for this node %p\n", node);
               TR::Register *borrowReg = NULL;
               if ((node->getOpCodeValue() == TR::lusubb) && TR_PPCComputeCC::setCarryBorrow(node->getChild(2), true, &borrowReg, cg))
                  {
                  // Currently, only the path that calculates the CC handles subb.  This is fine
                  // since the simplifier will lower subb to sub in all other cases.
                  //
                  // use subfe rather than subfc
                  regToRegOpCode = TR::InstOpCode::subfe;
                  }

               generateTrg1Src2Instruction(cg, regToRegOpCode, node, trgReg, src2Reg, src1Reg);
               }
            else
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, src2Reg, src1Reg);
               }
            }
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

// also handles lusub
TR::Register *OMR::Power::TreeEvaluator::lsubEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   if (TR::Compiler->target.is64Bit())
     return lsub64Evaluator(node, cg);

   TR::Node     *firstChild     = node->getFirstChild();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Register *trgReg = NULL;
   union  {
      int64_t longValue;
      struct  {
         int32_t highValue;
         int32_t lowValue;
      } x;
   } longVal;
   bool setsOrReadsCC = NEED_CC(node) || (node->getOpCodeValue() == TR::lusubb);
   TR::InstOpCode::Mnemonic regToRegOpCode = TR::InstOpCode::subfc;

   if (!setsOrReadsCC && (secondChild->getOpCodeValue() == TR::lconst || secondChild->getOpCodeValue() == TR::luconst) &&
       secondChild->getRegister() == NULL)
      {
      TR::Register *src1Reg   = cg->evaluate(firstChild);
      int64_t longValue = secondChild->getLongInt();
      longValue = -longValue;
      int32_t lowValue = (int32_t)longValue;
      int32_t highValue =  (int32_t)(longValue >> 32);
      trgReg = addConstantToLong(node, src1Reg->getHighOrder(), src1Reg->getLowOrder(),
                                         highValue, lowValue, cg);

      }
   else
      {
      TR::Register *lowReg  = cg->allocateRegister();
      TR::Register *highReg = cg->allocateRegister();
      if (!setsOrReadsCC && (firstChild->getOpCodeValue() == TR::lconst || firstChild->getOpCodeValue() == TR::luconst) &&
         firstChild->getRegister() == NULL)
         {
         TR::Register *src2Reg   = cg->evaluate(secondChild);
         int32_t highValue = firstChild->getLongIntHigh();
         int32_t lowValue = firstChild->getLongIntLow();

         if (lowValue >= LOWER_IMMED && lowValue <= UPPER_IMMED)
            {
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, lowReg, src2Reg->getLowOrder(), lowValue);
            }
         else
            {
            TR::Register *tempReg = cg->allocateRegister();
            loadConstant(cg, node, lowValue, tempReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, lowReg, src2Reg->getLowOrder(), tempReg);
            cg->stopUsingRegister(tempReg);
            }
         if (highValue == 0)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::subfze, node, highReg, src2Reg->getHighOrder());
            }
         else if (highValue == -1)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::subfme, node, highReg, src2Reg->getHighOrder());
            }
         else
            {
            TR::Register *tempReg = cg->allocateRegister();
            loadConstant(cg, node, highValue, tempReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, highReg, src2Reg->getHighOrder(), tempReg);
            cg->stopUsingRegister(tempReg);
            }
         }
      else
         {
         TR::Register *src1Reg   = cg->evaluate(firstChild);
         TR::Register *src2Reg   = cg->evaluate(secondChild);

         TR::Register *borrowReg = NULL;
         if ((node->getOpCodeValue() == TR::lusubb) && TR_PPCComputeCC::setCarryBorrow(node->getChild(2), true, &borrowReg, cg))
            {
            // use subfe rather than subfc
            regToRegOpCode = TR::InstOpCode::subfe;
            }

         generateTrg1Src2Instruction(cg, regToRegOpCode, node, lowReg, src2Reg->getLowOrder(), src1Reg->getLowOrder());
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, highReg, src2Reg->getHighOrder(), src1Reg->getHighOrder());
         }
      trgReg = cg->allocateRegisterPair(lowReg, highReg);
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

// also handles iumul
TR::Register *OMR::Power::TreeEvaluator::imulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *firstChild         = node->getFirstChild();
   TR::Node     *secondChild        = node->getSecondChild();
   TR::Register *src1Reg            = cg->evaluate(firstChild);
   TR::ILOpCodes secondOp                = secondChild->getOpCodeValue();

   if (secondOp == TR::iconst || secondOp == TR::iuconst)
      {
      int32_t value = secondChild->getInt();
      if (value > 0 && cg->convertMultiplyToShift(node))
         {
         // The multiply has been converted to a shift.
         // Note that we have restricted this to positive constant multipliers.
         // We can do it for negative ones too, but then the result of the shift
         // operation needs to be negated here after the shift node is evaluated.
         // The test above then becomes "if (value != 0 && ..."
         //
         trgReg = cg->evaluate(node);
         //
         // Add a negate here if the constant was negative
         //
         return(trgReg);
         }
      else
         {
         trgReg = cg->allocateRegister();
         mulConstant(node, trgReg, src1Reg, value, cg);
         }
      }
   else  // no constants
      {
      trgReg = cg->allocateRegister();
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, trgReg, src1Reg, cg->evaluate(secondChild));
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   node->setRegister(trgReg);
   return trgReg;
   }

static TR::Register *lmulEvaluatorWithAnalyser(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Node     *secondChild    = node->getSecondChild();

   TR::Register *src1Reg = firstChild->getRegister();
   TR::Register *src2Reg = secondChild->getRegister();

   TR::Register *src1Low = NULL;
   TR::Register *src1High = NULL;
   TR::Register *src2Low = NULL;
   TR::Register *src2High = NULL;


   bool firstHighZero = false;
   bool secondHighZero = false;
   bool secondUseHighOrder = false;

   genericLongAnalyzer(cg, firstChild, src1Low, src1High, firstHighZero);
   genericLongAnalyzer(cg, secondChild, src2Low, src2High, secondHighZero);


   TR::Register *lowReg  = cg->allocateRegister();
   TR::Register *highReg = cg->allocateRegister();
   TR::Register *trgReg = cg->allocateRegisterPair(lowReg, highReg);


   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, lowReg, src1Low, src2Low);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, highReg, src1Low, src2Low);

   TR::Register *temp1Reg = cg->allocateRegister();

   if (firstHighZero)
      {
      if (!secondHighZero)
         {
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp1Reg, src1Low, src2High);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, highReg, temp1Reg);
         }
      }
   else
      {
      TR_ASSERT(secondHighZero, "One of the long operands must have vacant high register");
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp1Reg, src2Low, src1High);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, highReg, temp1Reg);
      }
   cg->stopUsingRegister(temp1Reg);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   node->setRegister(trgReg);

   return trgReg;
   }

//
// 64bit version of dual multiply Helper
//
TR::Register *
OMR::Power::TreeEvaluator::dualMulHelper64(TR::Node * node, TR::Node * lmulNode, TR::Node * lumulhNode, TR::CodeGenerator * cg)
   {
   bool needsUnsignedHighMulOnly = (lmulNode == NULL);
   // Both parts of multiplication required if !needsUnsignedHighMulOnly
   // targetHigh:targetLow <-- firstChild * secondChild
   //
   //
   // firstChild is overwritten, secondChild is unchanged
   //
   // ignore whether children are constant or zero, which may be suboptimal
   TR::Node * firstChild =  lumulhNode->getFirstChild();
   TR::Node * secondChild = lumulhNode->getSecondChild();

   TR::Register * lumulhTargetRegister = cg->gprClobberEvaluate(firstChild);
   TR::Register * secondRegister = cg->evaluate(secondChild);
   if (!needsUnsignedHighMulOnly)
      {
      TR::Register * lmulTargetRegister = cg->allocateRegister();
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld , node, lmulTargetRegister,  lumulhTargetRegister, secondRegister);
      lmulNode->setRegister(lmulTargetRegister);
      }
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhdu, node, lumulhTargetRegister, lumulhTargetRegister, secondRegister);

   lumulhNode->setRegister(lumulhTargetRegister);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return node->getRegister();
   }

//
// 32bit version of dual multiply Helper
//
TR::Register *
OMR::Power::TreeEvaluator::dualMulHelper32(TR::Node * node, TR::Node * lmulNode, TR::Node * lumulhNode, TR::CodeGenerator * cg)
   {
    bool needsUnsignedHighMulOnly = (lmulNode == NULL);
   // requires:
   //   7 registers: (but clobbering both a and b register pairs may use more)
   //     al, ah=r4, bh, bl=r3, r2, r1, t
   //
   // entry:
   //   ah:al = a = evaluate(firstChild)
   //   bh:bl = b = evaluate(secondChild)
   //   ah=r4, bl=r3 is overwritten with result; al, bh are unchanged. (but they are clobbered anyway)
   // exit
   //   r4:r3:r2:r1 = r = a * b
   //
   TR::Node * firstChild =  lumulhNode->getFirstChild();
   TR::Node * secondChild = lumulhNode->getSecondChild();

   TR::RegisterPair *aReg = (TR::RegisterPair *) cg->gprClobberEvaluate(firstChild);
   TR::RegisterPair *bReg = (TR::RegisterPair *) cg->gprClobberEvaluate(secondChild);

   TR::Register *ahReg = aReg->getHighOrder();
   TR::Register *alReg = aReg->getLowOrder();
   TR::Register *bhReg = bReg->getHighOrder();
   TR::Register *blReg = bReg->getLowOrder();

   TR::Register *tReg = cg->allocateRegister();
   TR::Register *r1Reg = cg->allocateRegister();
   TR::Register *r2Reg = cg->allocateRegister();
   TR::Register *r3Reg = blReg;
   TR::Register *r4Reg = ahReg;

   TR::RegisterPair * lmulTargetRegister = NULL;
   if (!needsUnsignedHighMulOnly)
      {
      lmulTargetRegister  = cg->allocateRegisterPair(r1Reg, r2Reg);
      }
   TR::RegisterPair * lumulhTargetRegister = cg->allocateRegisterPair(r3Reg, r4Reg);

   // mullw   r1, al, bl     ;;     r1 =                                          (al * bl)l
   // mulhwu  r2, al, al     ;;     r2 =                              (al * bl)h
   // mullw    t, ah, bl     ;;      t =                              (ah * bl)l
   // addc    r2, r2,  t     ;; (r2,C) =                                r2 + t
   // mulhwu  r3, ah, bl     ;;     r3 = bl =             (ah * bl)h
   // mullw    t, ah, bh     ;;      t =                  (ah * bh)l
   // adde    r3, r3,  t     ;; (r3,C) =                  r3 + t + C
   // mulhwu  r4, ah, bh     ;;     r4 = ah = (ah * bh)h
   // addze   r4, r4         ;; (r4,C) =       r4 + C
   // mullw    t, al, bh     ;;      t =                              (al * bh)l
   // addc    r2, r2,  t     ;; (r2,C) =                                r2 + t
   // mulhwu   t, al, bh     ;;      t =                  (al * bh)h
   // adde    r3, r3,  t     ;; (r3,C) =                  r3 + t + C
   // addze   r4, r4         ;; (r4,C) =       r4 + C

   // mullw   r1, al, bl     ;;     r1 =      (al * bl)l
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, r1Reg, alReg, blReg);

   // mulhwu  r2, al, al     ;;     r2 =      (al * bl)h
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, r2Reg, alReg, blReg);

   // mullw    t, ah, bl     ;;      t =      (ah * bl)l
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node,  tReg, ahReg, blReg);

   // addc    r2, r2,  t     ;; (r2,C) =       r2 + t
   generateTrg1Src2Instruction(cg, TR::InstOpCode::addc , node, r2Reg, r2Reg,  tReg);

   // mulhwu  r3, ah, bl     ;;     r3 = bl =             (ah * bl)h
   // bl is overwritten here
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, r3Reg, ahReg, blReg);

   // mullw    t, ah, bh     ;;      t =                  (ah * bh)l
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node,  tReg, ahReg, bhReg);

   // adde    r3, r3,  t     ;; (r3,C) =                  r3 + t + C
   generateTrg1Src2Instruction(cg, TR::InstOpCode::adde , node, r3Reg, r3Reg,  tReg);

   // mulhwu  r4, ah, bh     ;;     r4 = ah = (ah * bh)h
   // ah is overwritten here
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, r4Reg, ahReg, bhReg);

   // addze   r4, r4         ;; (r4,C) =       r4 + C
   generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, r4Reg, r4Reg);

   // mullw    t, al, bh     ;;      t =                              (al * bh)l
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node,  tReg, alReg, bhReg);

   // addc    r2, r2,  t     ;; (r2,C) =                                r2 + t
   generateTrg1Src2Instruction(cg, TR::InstOpCode::addc , node, r2Reg, r2Reg,  tReg);

   // mulhwu   t, al, bh     ;;      t =                  (al * bh)h
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node,  tReg, alReg, bhReg);

   // adde    r3, r3,  t     ;; (r3,C) =                  r3 + t + C
   generateTrg1Src2Instruction(cg, TR::InstOpCode::adde , node, r3Reg, r3Reg,  tReg);

   // addze   r4, r4         ;; (r4,C) =       r4 + C
   generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, r4Reg, r4Reg);
   // stop using ...

   if (!needsUnsignedHighMulOnly)
      {
      lmulNode->setRegister(lmulTargetRegister);
      }
   else
      {
      cg->stopUsingRegister(r1Reg);
      cg->stopUsingRegister(r2Reg);
      }

   lumulhNode->setRegister(lumulhTargetRegister);

   // tReg is no longer needed,
   cg->stopUsingRegister(aReg);
   cg->stopUsingRegister(bReg);
   cg->stopUsingRegister(tReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return node->getRegister();
   }

//
// Evaluator for quad precision multiply using dual operators
//
TR::Register *
OMR::Power::TreeEvaluator::dualMulEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Compilation * comp = cg->comp();
   bool needsUnsignedHighMulOnly = (node->getOpCodeValue() == TR::lumulh) && !node->isDualCyclic();
   TR_ASSERT((node->getOpCodeValue() == TR::lumulh) || (node->getOpCodeValue() == TR::lmul), "Unexpected operator. Expected lumulh or lmul.");
   TR_ASSERT(node->isDualCyclic() || needsUnsignedHighMulOnly, "Should be either calculating cyclic dual or just the high part of the lmul.");
   if (node->isDualCyclic() && (node->getChild(2)->getReferenceCount() == 1))
      {
      // other part of this dual is not used, and is dead
      TR::Node *pair = node->getChild(2);
      // break dual into parts before evaluation
      // pair has only one reference, so need to avoid recursiveness removal of its subtree
      pair->incReferenceCount();
      node->removeChild(2);
      pair->removeChild(2);
      cg->decReferenceCount(pair->getFirstChild());
      cg->decReferenceCount(pair->getSecondChild());
      cg->decReferenceCount(pair);
      // evaluate this part again
      return cg->evaluate(node);
      }
   else
      {
	  TR::Node *lmulNode;
	  TR::Node *lumulhNode;
      if (!needsUnsignedHighMulOnly)
         {
         diagnostic("Found lmul/lumulh for node = %p\n", node);
         lmulNode = (node->getOpCodeValue() == TR::lmul) ? node : node->getChild(2);
         lumulhNode = lmulNode->getChild(2);
         TR_ASSERT((lumulhNode->getReferenceCount() > 1) && (lmulNode->getReferenceCount() > 1),
              "Expected both lumulh and lmul have external references.");
         // we only evaluate the lumulh children, and internal cycle does not indicate evaluation
         cg->decReferenceCount(lmulNode->getFirstChild());
         cg->decReferenceCount(lmulNode->getSecondChild());
         cg->decReferenceCount(lmulNode->getChild(2));
         cg->decReferenceCount(lumulhNode->getChild(2));
         }
      else
         {
         diagnostic("Found lumulh only node = %p\n", node);
         lumulhNode = node;
         lmulNode = NULL;
         }

   if (TR::Compiler->target.is64Bit())
         {
         return TR::TreeEvaluator::dualMulHelper64(node, lmulNode, lumulhNode, cg);
         }
      else
         {
         return TR::TreeEvaluator::dualMulHelper32(node, lmulNode, lumulhNode, cg);
         }
      }
   }

TR::Register *OMR::Power::TreeEvaluator::lmulEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Node     *secondChild    = node->getSecondChild();

   if (node->isDualCyclic())
      {
      return TR::TreeEvaluator::dualMulEvaluator(node, cg);
      }

   if (TR::Compiler->target.is64Bit())
      {
      TR::Register *trgReg;
      if (secondChild->getOpCodeValue() == TR::lconst || secondChild->getOpCodeValue() == TR::luconst)
         {
         int64_t value = secondChild->getLongInt();
         if (value > 0 && cg->convertMultiplyToShift(node))
            {
            // The multiply has been converted to a shift.
            // Note that we have restricted this to positive constant multipliers.
            // We can do it for negative ones too, but then the result of the shift
            // operation needs to be negated here after the shift node is evaluated.
            // The test above then becomes "if (value != 0 && ..."
            //
            trgReg = cg->evaluate(node);
            //
            // Add a negate here if the constant was negative
            //
            return(trgReg);
            }
         else
            {
            trgReg = cg->allocateRegister();
            TR::Register *src1Reg = cg->evaluate(firstChild);
            mulConstant(node, trgReg, src1Reg, value, cg);
            }
         }
      else  // no constants
         {
         trgReg = cg->allocateRegister();
         TR::Register *src1Reg = cg->evaluate(firstChild);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld, node, trgReg, src1Reg, cg->evaluate(secondChild));
         }

      cg->decReferenceCount(firstChild);
      cg->decReferenceCount(secondChild);
      node->setRegister(trgReg);
      return trgReg;
      }

   TR::Register *lowReg;
   TR::Register *highReg;
   TR::RegisterPair *trgReg;

   if ((secondChild->getOpCodeValue() == TR::lconst || secondChild->getOpCodeValue() == TR::luconst) &&
       secondChild->getRegister() == NULL)
      {
      TR::Register *src1Low = cg->evaluate(firstChild)->getLowOrder();
      TR::Register *src1High = cg->evaluate(firstChild)->getHighOrder();
      int32_t lowValue  = secondChild->getLongIntLow();
      int32_t highValue = secondChild->getLongIntHigh();

      if (!(lowValue == 0 && highValue == 0) &&
          highValue >= 0 &&
          cg->convertMultiplyToShift(node))
         {
         // The multiply has been converted to a shift.
         // Note that we have restricted this to positive constant multipliers.
         // We can do it for negative ones too, but then the result of the shift
         // operation needs to be negated here after the shift node is evaluated.
         //
         TR::Register *targetRegister = cg->evaluate(node);
         //
         // Add a negate here if the constant was negative
         //
         return targetRegister;
         }
      lowReg = cg->allocateRegister();
      highReg = cg->allocateRegister();
      if (lowValue == 0)
         {
         loadConstant(cg, node, 0, lowReg);
         mulConstant(node, highReg, src1Low, highValue, cg);
         }
      else if (lowValue == 1)
         {
         generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, lowReg, src1Low);
         if (highValue == 0)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, highReg, src1High);
            }
         else if (highValue == 1)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, src1High, src1Low);
            }
         else
            {
            TR::Register *temp1Reg = cg->allocateRegister();
            mulConstant(node, temp1Reg, src1Low, highValue, cg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, src1High, temp1Reg);
            cg->stopUsingRegister(temp1Reg);
            }
         }
      else if (lowValue == -1 && highValue == -1)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, lowReg, src1Low, 0);
         generateTrg1Src1Instruction(cg, TR::InstOpCode::subfze, node, highReg, src1High);
         }
      else
         {
         //  It is observed that using mulConstant for lmul can introduce 3 loads of the constant
         //  when the constant isn't a special value. The optimal solution is to call mulConstant
         //  so that we don't miss any opt opportunity and continue using the temp
         //  reg where the constant is loaded in mulConstant. For now, generate 2 mullw directly.

         TR::Register *temp1Reg = cg->allocateRegister();
         TR::Register *temp2Reg = cg->allocateRegister();
         TR::Register *temp3Reg = cg->allocateRegister();
         loadConstant(cg, node, lowValue, temp1Reg);
         // want the smaller of the sources in the RB position of a multiply
         // one crude measure of absolute size is the number of leading zeros
         if (leadingZeroes(abs(lowValue)) >= 24)
            {
            // the constant is fairly small, so put it in RB
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, lowReg, src1Low, temp1Reg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, temp2Reg, src1Low, temp1Reg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp3Reg, src1High, temp1Reg);
            }
         else
            {
            // the constant is fairly big, so put it in RA
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, lowReg, temp1Reg, src1Low);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, temp2Reg, temp1Reg, src1Low);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp3Reg, temp1Reg, src1High);
            }
         cg->stopUsingRegister(temp1Reg);

         if (highValue == 0)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, temp2Reg, temp3Reg);
            cg->stopUsingRegister(temp2Reg);
            cg->stopUsingRegister(temp3Reg);
            }
         else
            {
            TR::Register *temp4Reg = cg->allocateRegister();
            generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, temp4Reg, temp2Reg, temp3Reg);
            cg->stopUsingRegister(temp2Reg);
            cg->stopUsingRegister(temp3Reg);

            TR::Register *temp5Reg = cg->allocateRegister();
            mulConstant(node, temp5Reg, src1Low, highValue, cg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, temp4Reg, temp5Reg);
            cg->stopUsingRegister(temp4Reg);
            cg->stopUsingRegister(temp5Reg);
            }
         }
      }
   else
      {
      if (firstChild->isHighWordZero() || secondChild->isHighWordZero())
         {
         return lmulEvaluatorWithAnalyser(node, cg);
         }
      else
         {
         TR::Register *src1Low = cg->evaluate(firstChild)->getLowOrder();
         TR::Register *src1High = cg->evaluate(firstChild)->getHighOrder();
         TR::Register *src2Low = cg->evaluate(secondChild)->getLowOrder();
         TR::Register *src2High = cg->evaluate(secondChild)->getHighOrder();
         TR::Register *temp1Reg = cg->allocateRegister();
         TR::Register *temp2Reg = cg->allocateRegister();

         lowReg = cg->allocateRegister();
         highReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, lowReg, src1Low, src2Low);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhwu, node, temp1Reg, src1Low, src2Low);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp2Reg, src1High, src2Low);

         TR::Register *temp3Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, temp3Reg, temp1Reg, temp2Reg);
         cg->stopUsingRegister(temp1Reg);
         cg->stopUsingRegister(temp2Reg);

         TR::Register *temp4Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp4Reg, src1Low, src2High);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, highReg, temp3Reg, temp4Reg);
         cg->stopUsingRegister(temp3Reg);
         cg->stopUsingRegister(temp4Reg);
         }
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   trgReg = cg->allocateRegisterPair(lowReg, highReg);
   node->setRegister(trgReg);

   return trgReg;
   }

// also handles iumulh
TR::Register *OMR::Power::TreeEvaluator::imulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg = cg->allocateRegister();
   TR::Node     *firstChild         = node->getFirstChild();
   TR::Node     *secondChild        = node->getSecondChild();
   TR::Register *src1Reg            = cg->evaluate(firstChild);

   // imulh is generated for constant idiv and the second child is the magic number
   // assume magic number is usually a large odd number with little optimization opportunity
   if (secondChild->getOpCodeValue() == TR::iconst || secondChild->getOpCodeValue() == TR::iuconst)
      {
      int32_t value = secondChild->get64bitIntegralValue();
      TR::Register *tempReg = cg->allocateRegister();
      loadConstant(cg, node, value, tempReg);
      // want the smaller of the sources in the RB position of a multiply
      // put the large magic number into the RA position
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, trgReg, tempReg, src1Reg);
      cg->stopUsingRegister(tempReg);
      }
   else  // no constants
      {
      // want the smaller of the sources in the RB position of a multiply
      // the second child is assumed to be the large magic number
      // put the large magic number into the RA position
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, trgReg, cg->evaluate(secondChild), src1Reg);
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::lmulhEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();

   bool needsUnsignedHighMulOnly = (node->getOpCodeValue() == TR::lumulh) && !node->isDualCyclic();
   if (node->isDualCyclic() || needsUnsignedHighMulOnly)
      {
      return TR::TreeEvaluator::dualMulEvaluator(node, cg);
      }

   // lmulh is generated for constant ldiv and the second child is the magic number
   // assume magic number is usually a large odd number with little optimization opportunity
   if (TR::Compiler->target.is64Bit())
      {
      TR::Register *src1Reg = cg->evaluate(firstChild);
      TR::Register *trgReg = cg->allocateRegister();
      if (secondChild->getOpCodeValue() == TR::lconst  || secondChild->getOpCodeValue() == TR::luconst )
         {
         int64_t value = secondChild->getLongInt();
         TR::Register *tempReg = cg->allocateRegister();
         loadConstant(cg, node, value, tempReg);
         // want the smaller of the sources in the RB position of a multiply
         // put the large magic number into the RA position
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhd, node, trgReg, tempReg, src1Reg);
         cg->stopUsingRegister(tempReg);
         }
      else
         {
         // want the smaller of the sources in the RB position of a multiply
         // the second child is assumed to be the large magic number
         // put the large magic number into the RA position
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhd, node, trgReg, cg->evaluate(secondChild), src1Reg);
         }
      cg->decReferenceCount(firstChild);
      cg->decReferenceCount(secondChild);
      node->setRegister(trgReg);
      return (trgReg);
      }

   // 32-bit
   TR::Register *second_highReg;
   TR::Register *second_lowReg;
   TR::Register *first_highReg = cg->evaluate(firstChild)->getHighOrder();
   TR::Register *first_lowReg = cg->evaluate(firstChild)->getLowOrder();
   TR::Register *temp1Reg = cg->allocateRegister();
   TR::Register *temp2Reg = cg->allocateRegister();
   TR::Register *temp3Reg = cg->allocateRegister();
   TR::Register *lowReg = cg->allocateRegister();
   TR::Register *highReg = cg->allocateRegister();

   if (secondChild->getOpCodeValue() == TR::lconst || secondChild->getOpCodeValue() == TR::luconst)
      {
      int64_t value =  secondChild->getLongInt();
      int32_t lowValue = (int32_t)value;
      int32_t highValue =  (int32_t)(value >> 32);
      second_highReg = cg->allocateRegister();
      second_lowReg = cg->allocateRegister();
      loadConstant(cg, node, highValue, second_highReg);
      loadConstant(cg, node, lowValue, second_lowReg);
      }
   else
      {
      second_highReg = cg->evaluate(secondChild)->getHighOrder();
      second_lowReg = cg->evaluate(secondChild)->getLowOrder();
      }

   // want the smaller of the sources in the RB position of a multiply
   // the second child is assumed to be the large magic number
   // put the large magic number into the RA position
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, temp1Reg, second_lowReg, first_highReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, temp2Reg, second_highReg, first_lowReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, temp3Reg, second_highReg, first_highReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, highReg, second_highReg, first_highReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::addc, node, lowReg, temp1Reg, temp2Reg);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, highReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::addc, node, lowReg, lowReg, temp3Reg);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, highReg, highReg);
   if (secondChild->getOpCodeValue() == TR::lconst || secondChild->getOpCodeValue() == TR::luconst)
      {
      cg->stopUsingRegister(second_highReg);
      cg->stopUsingRegister(second_lowReg);
      }
   cg->stopUsingRegister(temp1Reg);
   cg->stopUsingRegister(temp2Reg);
   cg->stopUsingRegister(temp3Reg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   TR::RegisterPair *trgReg = cg->allocateRegisterPair(lowReg, highReg);
   node->setRegister(trgReg);
   return trgReg;
   }

static TR::Register *signedIntegerDivisionOrRemainderAnalyser(TR::Node          *node,
                                                             TR::CodeGenerator *cg,
                                                             TR::Register      *dividendReg,
                                                             int32_t           divisorValue,
                                                             bool              isRemainder,
                                                             TR::Register      *trgReg = NULL,
                                                             TR::Register      *divisorReg = NULL,
                                                             TR::Register      *tmp1Reg = NULL,
                                                             TR::Register      *tmp2Reg = NULL
                                                   )
   {
   // 1. trgReg may be the same register as dividendReg or divisorReg;
   // 2. this routine is called from canned sequence of ldiv or lrem such that be careful with adding new registers
   //    directly or indirectly;

   bool  freeTmp1=false, freeTmp2=false;

   if (trgReg==NULL && (divisorValue!=1 || isRemainder))
      trgReg = cg->allocateRegister();

   if (divisorValue == 1)
      {
      if (isRemainder)
         {
         generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
         }
      else
         {
         trgReg = dividendReg;
         }
      }
   else if (divisorValue == -1)
      {
      if (isRemainder)
         generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
      else
         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, dividendReg);
      }
   else if (isPowerOf2(divisorValue))
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg, dividendReg, trailingZeroes(divisorValue));
      generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, trgReg, trgReg);
      if (isRemainder)
         {
         generateShiftLeftImmediate(cg, node, trgReg, trgReg, trailingZeroes(divisorValue));
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, trgReg, dividendReg);
         }
      else
         {
         if (isNonPositivePowerOf2(divisorValue))
            generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, trgReg);
         }
      }
   else if (TR::Compiler->target.cpu.id() >= TR_PPCp9 && isRemainder)
      {
      if (divisorReg == NULL)
         divisorReg = cg->evaluate(node->getSecondChild());
      generateTrg1Src2Instruction(cg, TR::InstOpCode::modsw, node, trgReg, dividendReg, divisorReg);
      }
   else
      {
      if (tmp1Reg == NULL)
         {
         tmp1Reg = cg->allocateRegister();
         freeTmp1 = true;
         }

      if (tmp2Reg == NULL)
         {
         tmp2Reg = cg->allocateRegister();
         freeTmp2 = true;
         }

      int32_t     magicNumber, shiftAmount;

      cg->compute32BitMagicValues(divisorValue, &magicNumber, &shiftAmount);

      loadConstant(cg, node, magicNumber, tmp1Reg);
      // want the smaller of the sources in the RB position of a multiply
      // put the large magic number into the RA position
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, tmp1Reg, tmp1Reg, dividendReg);
      if ((divisorValue > 0) && (magicNumber < 0))
         {
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmp1Reg, dividendReg, tmp1Reg);
         }
      else if ((divisorValue < 0) && (magicNumber > 0))
         {
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmp1Reg, dividendReg, tmp1Reg);
         }
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, tmp1Reg, shiftAmount);
      if (divisorValue > 0)
         {
         generateShiftRightLogicalImmediate(cg, node, tmp2Reg, dividendReg, 31);
         }
      else
         {
         generateShiftRightLogicalImmediate(cg, node, tmp2Reg, tmp1Reg, 31);
         }
      if (isRemainder)
         {
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmp1Reg, tmp1Reg, tmp2Reg);
         if (divisorReg != NULL)
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, tmp2Reg, tmp1Reg, divisorReg);
         else
            mulConstant(node, tmp2Reg, tmp1Reg, divisorValue, cg);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tmp2Reg, dividendReg);
         }
      else
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, tmp1Reg, tmp2Reg);
      }

   if (freeTmp1)
      cg->stopUsingRegister(tmp1Reg);
   if (freeTmp2)
      cg->stopUsingRegister(tmp2Reg);
   return trgReg;
   }

static TR::Register *signedLongDivisionOrRemainderAnalyser(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *dividend       = node->getFirstChild();
   int64_t divisor         = node->getSecondChild()->getLongInt();
   TR::ILOpCodes rootOpCode = node->getOpCodeValue();
   TR::Register *dividendReg= cg->evaluate(dividend);


   if (divisor == CONSTANT64(1))
      {
      if (rootOpCode == TR::ldiv)
         {
         return dividendReg;
         }
         else
         {
         TR::Register *trgReg = cg->allocateRegister();
         generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
         return trgReg;
         }
      }
   if (divisor == CONSTANT64(-1))
      {
      TR::Register *trgReg = cg->allocateRegister();
      if (rootOpCode == TR::ldiv)
         {
         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, dividendReg);
         }
         else
         {
         generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
         }
      return trgReg;
      }
   if (isPowerOf2(divisor))
      {
      // The dividend is required in the remainder calculation
      TR::Register *temp1Reg = cg->allocateRegister();
      TR::Register *temp2Reg = cg->allocateRegister();

      if (rootOpCode == TR::lrem)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::sradi, node, temp1Reg, dividendReg, trailingZeroes(divisor));
         generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, temp2Reg, temp1Reg);
         cg->stopUsingRegister(temp1Reg);

         TR::Register *temp3Reg = cg->allocateRegister();
         generateShiftLeftImmediateLong(cg, node, temp3Reg, temp2Reg, trailingZeroes(divisor));
         cg->stopUsingRegister(temp2Reg);

         TR::Register *temp4Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, temp4Reg, temp3Reg, dividendReg);
         cg->stopUsingRegister(temp3Reg);
         return temp4Reg;
         }
      else // rootOpCode == TR::ldiv
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::sradi, node, temp1Reg, dividendReg, trailingZeroes(divisor));
         generateTrg1Src1Instruction(cg, TR::InstOpCode::addze, node, temp2Reg, temp1Reg);
         cg->stopUsingRegister(temp1Reg);

         if (isNonPositivePowerOf2(divisor)  )
            {
            TR::Register *temp3Reg = cg->allocateRegister();
            generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, temp3Reg, temp2Reg);
            cg->stopUsingRegister(temp2Reg);
            return temp3Reg;
            }
         else
            {
            return temp2Reg;
            }
         }
      }
   else
      {
      int64_t     magicNumber, shiftAmount;
      TR::Register *magicReg = cg->allocateRegister();
      TR::Register *temp1Reg = cg->allocateRegister();
      TR::Register *temp2Reg;

      cg->compute64BitMagicValues(divisor, &magicNumber, &shiftAmount);

      loadConstant(cg, node, magicNumber, magicReg);
      // want the smaller of the sources in the RB position of a multiply
      // put the large magic number into the RA position
      generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhd, node, temp1Reg, magicReg, dividendReg);
      cg->stopUsingRegister(magicReg);

      if ( (divisor > 0) && (magicNumber < 0) )
         {
         temp2Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, temp2Reg, dividendReg, temp1Reg);
         cg->stopUsingRegister(temp1Reg);
         }
      else if ( (divisor < 0) && (magicNumber > 0) )
         {
         temp2Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, temp2Reg, dividendReg, temp1Reg);
         cg->stopUsingRegister(temp1Reg);
         }
      else
         temp2Reg = temp1Reg;

      TR::Register *temp3Reg = cg->allocateRegister();
      TR::Register *temp4Reg = cg->allocateRegister();
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::sradi, node, temp3Reg, temp2Reg, shiftAmount);
      cg->stopUsingRegister(temp2Reg);

      if (divisor > 0)
         {
         if(TR::Compiler->target.is64Bit())
            generateShiftRightLogicalImmediateLong(cg, node, temp4Reg, dividendReg, 63);
         else
            generateShiftRightLogicalImmediate(cg, node, temp4Reg, dividendReg, 31);
         }
      else
         {
         if(TR::Compiler->target.is64Bit())
            generateShiftRightLogicalImmediateLong(cg, node, temp4Reg, temp3Reg, 63);
         else
            generateShiftRightLogicalImmediate(cg, node, temp4Reg, temp3Reg, 31);
         }

      TR::Register *temp5Reg = cg->allocateRegister();
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, temp5Reg, temp3Reg, temp4Reg);
      cg->stopUsingRegister(temp3Reg);
      cg->stopUsingRegister(temp4Reg);

      if (rootOpCode == TR::lrem)
         {
         TR::Register *temp6Reg = cg->allocateRegister();
         TR::Register *temp7Reg = cg->allocateRegister();
         mulConstant(node, temp6Reg, temp5Reg, divisor, cg);
         cg->stopUsingRegister(temp5Reg);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, temp7Reg, temp6Reg, dividendReg);
         cg->stopUsingRegister(temp6Reg);
         return temp7Reg;
         }
      else
         {
         return temp5Reg;
         }
      }
   }

// also handles iudiv
TR::Register *OMR::Power::TreeEvaluator::idivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation * comp = cg->comp();
   TR::Register *trgReg;
   TR::Node     *firstChild      = node->getFirstChild();
   TR::Node     *secondChild     = node->getSecondChild();
   TR::Register *dividendReg     = cg->evaluate(firstChild);
   uint32_t    divisor = 0;

   if ( secondChild->getOpCode().isLoadConst())
      divisor =  secondChild->getInt();
   else if ( firstChild->getOpCode().isLoadConst())
      {
      int32_t     dividend = firstChild->getInt();
      if (dividend != 0x80000000)
         {
         trgReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::divw, node, trgReg, dividendReg, cg->evaluate(secondChild));
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         node->setRegister(trgReg);
         return trgReg;
         }
      }

   // Signed division by a constant can be done cheaper
   if (divisor !=0 )
      {
      trgReg = signedIntegerDivisionOrRemainderAnalyser(node, cg, dividendReg, divisor, false, NULL, secondChild->getRegister());
      }
   else
      {
      bool testNeeded = (!(secondChild->isNonNegative()) && !(firstChild->isNonNegative()));
      TR::Register *condReg;
      TR::Register *divisorReg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();
      // Eventually the following test should be whether there is a register allocator that
      // can handle registers being alive across basic block boundaries.
      // For now we just generate pessimistic code.
      if (testNeeded)
         {
         TR::PPCControlFlowInstruction *cfop = (TR::PPCControlFlowInstruction *)
            generateControlFlowInstruction(cg, TR::InstOpCode::idiv, node);
         condReg = cg->allocateRegister(TR_CCR);
         cfop->addTargetRegister(condReg);
         cfop->addTargetRegister(trgReg);
         cfop->addSourceRegister(dividendReg);
         cfop->addSourceRegister(divisorReg);
         cfop->addSourceRegister(trgReg);
         cg->stopUsingRegister(condReg);
         }
      else
         {
         TR::LabelSymbol *doneLabel;
         // Right now testNeeded will always be false on this path.
         if (false /*testNeeded*/)
            {
            doneLabel = generateLabelSymbol(cg);
            condReg = cg->allocateRegister(TR_CCR);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, condReg, divisorReg, -1);
            generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, dividendReg);
            generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, doneLabel, condReg);
            cg->stopUsingRegister(condReg);
            }
         generateTrg1Src2Instruction(cg, TR::InstOpCode::divw, node, trgReg, dividendReg, divisorReg);
         if (false /*testNeeded*/)
            generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel);
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }


// long division for 64 bit target hardware
// handles ldiv and ludiv
static TR::Register *ldiv64Evaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *firstChild      = node->getFirstChild();
   TR::Node     *secondChild     = node->getSecondChild();
   TR::Register *dividendReg     = cg->evaluate(firstChild);
   uint64_t    divisor = 0;
   TR::Compilation * comp = cg->comp();

   TR_ASSERT(node->getOpCodeValue() != TR::ludiv, "TR::ludiv is not impelemented yet for 64-bit target\n");

   if ( secondChild->getOpCode().isLoadConst())
      divisor =  secondChild->getLongInt();
   else if ( firstChild->getOpCode().isLoadConst())
      {
      int64_t     dividend = firstChild->getLongInt();
      if (dividend != CONSTANT64(0x8000000000000000))
         {
         trgReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::divd, node, trgReg, dividendReg, cg->evaluate(secondChild));
         node->setRegister(trgReg);
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         return trgReg;
         }
      }

   // Signed division by a constant can be done cheaper
   if (divisor != CONSTANT64(0))
      {
      trgReg = signedLongDivisionOrRemainderAnalyser(node, cg);
      }
   else
      {
      bool testNeeded = (!(secondChild->isNonNegative()) && !(firstChild->isNonNegative()));
      TR::Register *condReg;
      TR::Register *divisorReg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();
      // Eventually the following test should be whether there is a register allocator that
      // can handle registers being alive across basic block boundaries.
      // For now we just generate pessimistic code.
      if (testNeeded)
         {
         TR::PPCControlFlowInstruction *cfop = (TR::PPCControlFlowInstruction *)
            generateControlFlowInstruction(cg, TR::InstOpCode::ldiv, node);
         condReg = cg->allocateRegister(TR_CCR);
         cfop->addTargetRegister(condReg);
         cfop->addTargetRegister(trgReg);
         cfop->addSourceRegister(dividendReg);
         cfop->addSourceRegister(divisorReg);
         cfop->addSourceRegister(trgReg);
         cg->stopUsingRegister(condReg);
         }
      else
         {
         TR::LabelSymbol *doneLabel;
         // Right now testNeeded will always be false on this path.
         if (false /*testNeeded*/)
            {
            doneLabel = generateLabelSymbol(cg);
            condReg = cg->allocateRegister(TR_CCR);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi8, node, condReg, divisorReg, -1);
            generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, dividendReg);
            generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, doneLabel, condReg);
            cg->stopUsingRegister(condReg);
            }
         generateTrg1Src2Instruction(cg, TR::InstOpCode::divd, node, trgReg, dividendReg, divisorReg);
         if (false /*testNeeded*/)
            generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel);
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

static void
strengthReducingLongDivideOrRemainder32BitMode(TR::Node *node,      TR::CodeGenerator   *cg,
                                               TR::RegisterDependencyConditions     *dependencies,
                                               TR::Register **dd_highReg, TR::Register **dd_lowReg,
                                               TR::Register **dr_highReg, TR::Register **dr_lowReg,
                                               bool          isSignedOp, bool          isRemainder)
   {
   TR::Node     *firstChild   = node->getFirstChild();
   TR::Node     *secondChild  = node->getSecondChild();
   TR::Register *dividend = cg->evaluate(firstChild);
   TR::Register *divisor = cg->evaluate(secondChild);
   TR::Register *dd_h, *dd_l, *dr_h, *dr_l, *tmp1Reg, *tmp2Reg, *cr0Reg;
   TR::LabelSymbol *doneLabel = generateLabelSymbol(cg);

  if (!cg->canClobberNodesRegister(firstChild))
      {
      dd_l = cg->allocateRegister();
      dd_h = cg->allocateRegister();
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, dd_l, dividend->getLowOrder());
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, dd_h, dividend->getHighOrder());
      }
   else
      {
      dd_l = dividend->getLowOrder();
      dd_h = dividend->getHighOrder();
      }
   *dd_highReg = dd_h; *dd_lowReg = dd_l;

   if (!cg->canClobberNodesRegister(secondChild))
      {
      dr_l = cg->allocateRegister();
      dr_h = cg->allocateRegister();
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, dr_l, divisor->getLowOrder());
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, dr_h, divisor->getHighOrder());
      }
   else
      {
      dr_l = divisor->getLowOrder();
      dr_h = divisor->getHighOrder();
      }
   *dr_highReg = dr_h; *dr_lowReg = dr_l;

   TR::addDependency(dependencies, dd_h, TR::RealRegister::gr3, TR_GPR, cg);
   TR::addDependency(dependencies, dd_l, TR::RealRegister::gr4, TR_GPR, cg);
   TR::addDependency(dependencies, dr_h, TR::RealRegister::gr5, TR_GPR, cg);
   TR::addDependency(dependencies, dr_l, TR::RealRegister::gr6, TR_GPR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::gr0, TR_GPR, cg);
   tmp1Reg = cg->allocateRegister();
   TR::addDependency(dependencies, tmp1Reg, TR::RealRegister::gr7, TR_GPR, cg);
   tmp2Reg = cg->allocateRegister();
   TR::addDependency(dependencies, tmp2Reg, TR::RealRegister::gr8, TR_GPR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::gr9, TR_GPR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::gr11, TR_GPR, cg);
   cr0Reg = cg->allocateRegister(TR_CCR);
   TR::addDependency(dependencies, cr0Reg, TR::RealRegister::cr0, TR_CCR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::cr1, TR_CCR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::cr5, TR_CCR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::cr6, TR_CCR, cg);
   TR::addDependency(dependencies, NULL, TR::RealRegister::cr7, TR_CCR, cg);

   // Trivial cases are caught by Simplifier or ValuePropagation. Runtime test is needed at this stage.
   int64_t ddConst = firstChild->getLongInt(), drConst = secondChild->getLongInt();
   bool isDividendImpossible32Bit = firstChild->getOpCode().isLoadConst() && (ddConst>(int64_t)TR::getMaxSigned<TR::Int32>() || ddConst<(int64_t)TR::getMinSigned<TR::Int32>());
   bool isDivisorImpossible32Bit = secondChild->getOpCode().isLoadConst() && (drConst>(int64_t)TR::getMaxSigned<TR::Int32>() || drConst<(int64_t)TR::getMinSigned<TR::Int32>());
   if (!isDividendImpossible32Bit && !isDivisorImpossible32Bit)
      {
      TR::LabelSymbol *callLabel = generateLabelSymbol(cg);

      if (secondChild->getOpCode().isLoadConst() && drConst<=(int64_t)TR::getMaxSigned<TR::Int32>() && drConst>=(int64_t)TR::getMinSigned<TR::Int32>() && drConst!=-1)
         {
         // 32bit magic sequence is applicable if dividend is a signed 32bit value

         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, dd_l, 31);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::cmpl4, node, cr0Reg, tmp1Reg, dd_h);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, callLabel, cr0Reg);

         signedIntegerDivisionOrRemainderAnalyser(node, cg, dd_l, drConst, isRemainder, isRemainder?dr_l:dd_l, dr_l, tmp1Reg, tmp2Reg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, isRemainder?dr_h:dd_h, isRemainder?dr_l:dd_l, 31);
         }
      else
         {
         // Use unsigned 32bit division if both dividend and divisor are positive 32bit numbers
         // Didn't use record-form for scheduling issue, but post-pass will turn it into record-form anyway

         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, tmp1Reg, dd_h, dr_h);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpli4, node, cr0Reg, tmp1Reg, 0);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, callLabel, cr0Reg);

         if (isRemainder)
            {
            if (TR::Compiler->target.cpu.id() >= TR_PPCp9)
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::moduw, node, dr_l, dd_l, dr_l);
               }
            else
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::divwu, node, tmp2Reg, dd_l, dr_l);
               generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, tmp1Reg, tmp2Reg, dr_l);
               generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, dr_l, tmp1Reg, dd_l);
               }
            }
         else
            generateTrg1Src2Instruction(cg, TR::InstOpCode::divwu, node, dd_l, dd_l, dr_l);
         }

      generateLabelInstruction(cg, TR::InstOpCode::b, node, doneLabel);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, callLabel);
      }

   TR_RuntimeHelper helper;

   if (TR::Compiler->target.cpu.id() >= TR_PPCp7 && !isDivisorImpossible32Bit)
      helper = isSignedOp ? TR_PPClongDivideEP : TR_PPCunsignedLongDivideEP;
   else
      helper = isSignedOp ? TR_PPClongDivide : TR_PPCunsignedLongDivide;

   TR::SymbolReference *helperSym = cg->symRefTab()->findOrCreateRuntimeHelper(helper, false, false, false);
   uintptrj_t addr = (uintptrj_t)helperSym->getMethodAddress();
   TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 0, cg->trMemory());

   generateDepImmSymInstruction(cg, TR::InstOpCode::bl, node, addr, deps, helperSym);
   generateDepLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel, dependencies);
   }

// also handles ludiv
TR::Register *OMR::Power::TreeEvaluator::ldivEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {

   if (TR::Compiler->target.is64Bit())
      return ldiv64Evaluator(node, cg);

   TR::Register *dd_lowReg, *dr_lowReg;
   TR::Register *dd_highReg, *dr_highReg;
   TR::RegisterDependencyConditions *dependencies = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(14, 14, cg->trMemory());
   bool signed_div = (node->getOpCodeValue() == TR::ldiv);

   strengthReducingLongDivideOrRemainder32BitMode(node, cg, dependencies, &dd_highReg, &dd_lowReg, &dr_highReg, &dr_lowReg, signed_div, false);
   dependencies->stopUsingDepRegs(cg, dd_lowReg, dd_highReg);

   TR::Register *trgReg = cg->allocateRegisterPair(dd_lowReg, dd_highReg);
   cg->machine()->setLinkRegisterKilled(true);
   node->setRegister(trgReg);
   cg->decReferenceCount(node->getFirstChild());
   cg->decReferenceCount(node->getSecondChild());
   return trgReg;
   }

// also handles iurem
TR::Register *OMR::Power::TreeEvaluator::iremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *firstChild      = node->getFirstChild();
   TR::Node     *secondChild     = node->getSecondChild();
   TR::Register *dividendReg     = cg->evaluate(firstChild);
   int32_t divisor = 0;
   TR::Compilation * comp = cg->comp();

   if (secondChild->getOpCode().isLoadConst())
      divisor = secondChild->getInt();
   else if ( firstChild->getOpCode().isLoadConst())
      {
      int32_t     dividend = firstChild->getInt();
      if (dividend != 0x80000000)
         {
         TR::Register *divisorReg = cg->evaluate(secondChild);
         trgReg = cg->allocateRegister();
         if(TR::Compiler->target.cpu.id() >= TR_PPCp9)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::modsw, node, trgReg, dividendReg, divisorReg);
            }
         else
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::divw, node, trgReg, dividendReg, divisorReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, trgReg, divisorReg, trgReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, trgReg, dividendReg);
            }
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         node->setRegister(trgReg);
         return trgReg;
         }
      }

   if (divisor !=0)
      {
      trgReg = signedIntegerDivisionOrRemainderAnalyser(node, cg, dividendReg, divisor, true, NULL, secondChild->getRegister());
      }
   else
      {
      bool testNeeded = (!(secondChild->isNonNegative()) && !(firstChild->isNonNegative()));
      TR::Register *condReg;
      TR::Register *divisorReg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();

      // Eventually the following test should be whether there is a register allocator that
      // can handle registers being alive across basic block boundaries.
      // For now we just generate pessimistic code.
      if (testNeeded)
         {
         TR::PPCControlFlowInstruction *cfop = (TR::PPCControlFlowInstruction *)
            generateControlFlowInstruction(cg, TR::InstOpCode::irem, node);
         condReg = cg->allocateRegister(TR_CCR);
         cfop->addTargetRegister(condReg);
         cfop->addTargetRegister(trgReg);
         cfop->addSourceRegister(dividendReg);
         cfop->addSourceRegister(divisorReg);
         cfop->addSourceRegister(trgReg);
         cg->stopUsingRegister(condReg);
         }
      else
         {
         TR::LabelSymbol *doneLabel;
         // Right now testNeeded will always be false on this path.
         if (false /*testNeeded*/)
            {
            doneLabel = generateLabelSymbol(cg);
            condReg = cg->allocateRegister(TR_CCR);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, condReg, divisorReg, -1);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
            generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, doneLabel, condReg);
            cg->stopUsingRegister(condReg);
            }
         if(TR::Compiler->target.cpu.id() >= TR_PPCp9)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::modsw, node, trgReg, dividendReg, divisorReg);
            }
         else
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::divw, node, trgReg, dividendReg, divisorReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, trgReg, divisorReg, trgReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, trgReg, dividendReg);
            }
         if (false /*testNeeded*/)
            generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel);
         }
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   node->setRegister(trgReg);
   return trgReg;
   }


// long remainder for 64 bit target hardware
TR::Register *lrem64Evaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *firstChild      = node->getFirstChild();
   TR::Node     *secondChild     = node->getSecondChild();
   TR::Register *dividendReg     = cg->evaluate(firstChild);
   int64_t divisor = 0;
   TR::Compilation * comp = cg->comp();

   if (secondChild->getOpCode().isLoadConst())
      divisor =  secondChild->getLongInt();
   else if ( firstChild->getOpCode().isLoadConst())
      {
      int64_t     dividend = firstChild->getLongInt();
      if (dividend != CONSTANT64(0x8000000000000000))
         {
         TR::Register *divisorReg = cg->evaluate(secondChild);
         trgReg = cg->allocateRegister();
         if(TR::Compiler->target.cpu.id() >= TR_PPCp9)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::modsd, node, trgReg, dividendReg, divisorReg);
            }
         else
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::divd, node, trgReg, dividendReg, divisorReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld, node, trgReg, divisorReg, trgReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, trgReg, dividendReg);
            }
         cg->decReferenceCount(firstChild);
         cg->decReferenceCount(secondChild);
         node->setRegister(trgReg);
         return trgReg;
         }
      }

   if (divisor !=0)
      {
      trgReg = signedLongDivisionOrRemainderAnalyser(node, cg);
      }
   else
      {
      bool testNeeded = (!(secondChild->isNonNegative()) && !(firstChild->isNonNegative()));
      TR::Register *condReg;
      TR::Register *divisorReg = cg->evaluate(secondChild);
      trgReg = cg->allocateRegister();

      // Eventually the following test should be whether there is a register allocator that
      // can handle registers being alive across basic block boundaries.
      // For now we just generate pessimistic code.
      if (testNeeded)
         {
         TR::PPCControlFlowInstruction *cfop = (TR::PPCControlFlowInstruction *)
            generateControlFlowInstruction(cg, TR::InstOpCode::lrem, node);
         condReg = cg->allocateRegister(TR_CCR);
         cfop->addTargetRegister(condReg);
         cfop->addTargetRegister(trgReg);
         cfop->addSourceRegister(dividendReg);
         cfop->addSourceRegister(divisorReg);
         cfop->addSourceRegister(trgReg);
         cg->stopUsingRegister(condReg);
         }
      else
         {
         TR::LabelSymbol *doneLabel;
         // Right now testNeeded will always be false on this path.
         if (false /*testNeeded*/)
            {
            doneLabel = generateLabelSymbol(cg);
            condReg = cg->allocateRegister(TR_CCR);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi8, node, condReg, divisorReg, -1);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
            generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, doneLabel, condReg);
            cg->stopUsingRegister(condReg);
            }
         if (TR::Compiler->target.cpu.id() >= TR_PPCp9)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::modsd, node, trgReg, dividendReg, divisorReg);
            }
         else
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::divd, node, trgReg, dividendReg, divisorReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld, node, trgReg, divisorReg, trgReg);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, trgReg, dividendReg);
            }
         if (false /*testNeeded*/)
            generateLabelInstruction(cg, TR::InstOpCode::label, node, doneLabel);
         }
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   node->setRegister(trgReg);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::lremEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   if (TR::Compiler->target.is64Bit())
      return lrem64Evaluator(node, cg);

   TR::Register *dd_lowReg, *dr_lowReg;
   TR::Register *dd_highReg, *dr_highReg;
   TR::RegisterDependencyConditions *dependencies = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(14, 14, cg->trMemory());
   bool signed_rem = (node->getOpCodeValue() == TR::lrem);

   strengthReducingLongDivideOrRemainder32BitMode(node, cg, dependencies, &dd_highReg, &dd_lowReg, &dr_highReg, &dr_lowReg, signed_rem, true);
   dependencies->stopUsingDepRegs(cg, dr_highReg, dr_lowReg);

   TR::Register *trgReg = cg->allocateRegisterPair(dr_lowReg, dr_highReg);
   node->setRegister(trgReg);
   cg->machine()->setLinkRegisterKilled(true);
   cg->decReferenceCount(node->getFirstChild());
   cg->decReferenceCount(node->getSecondChild());
   return trgReg;
   }

// also handles bshl,iushl
TR::Register *OMR::Power::TreeEvaluator::ishlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg         = cg->allocateRegister();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *src1Reg        = cg->evaluate(firstChild);
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();
   bool isByte = (node->getOpCodeValue() == TR::bshl);

   if (secondOp == TR::iconst || secondOp == TR::iuconst)
      {
      int32_t  shiftAmount = secondChild->getInt() & 0x1f;
      generateShiftLeftImmediate(cg, node, trgReg, src1Reg, shiftAmount);
      }
   else
      {
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, trgReg, src1Reg, src2Reg);
      }

   if (isByte)
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, trgReg, 0, (uint32_t)0x000000FF);

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

// also handles lushl
TR::Register *OMR::Power::TreeEvaluator::lshlEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *secondChild = node->getSecondChild();
   TR::Node     *firstChild  = node->getFirstChild();
   TR::Register *srcReg, *srcLowReg, *srcHighReg;
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

   if ((secondOp == TR::iconst  || secondOp == TR::lconst ||
        secondOp == TR::iuconst || secondOp == TR::luconst) &&
       secondChild->getRegister() == NULL)
      {
      int32_t shiftAmount;
      if (secondOp == TR::iconst || secondOp == TR::iuconst)
         shiftAmount = secondChild->getInt()&0x3f;
      else
         shiftAmount = secondChild->getLongInt()&0x3f;

      if (TR::Compiler->target.is64Bit())
         {
         trgReg = cg->allocateRegister();
         if (firstChild->getOpCodeValue() == TR::i2l &&
             firstChild->getReferenceCount() == 1 &&
             firstChild->getRegister() == NULL)
            {
            if (firstChild->getFirstChild()->isNonNegative())
               {
               srcReg = cg->evaluate(firstChild->getFirstChild());
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldic, node, trgReg, srcReg, shiftAmount, ((CONSTANT64(1)<<(32+shiftAmount)) - CONSTANT64(1))^((CONSTANT64(1)<<shiftAmount) - CONSTANT64(1)));
               cg->decReferenceCount(firstChild->getFirstChild());
               }
            else if (TR::Compiler->target.cpu.id() >= TR_PPCp9)
               {
               srcReg = cg->evaluate(firstChild->getFirstChild());
               generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::extswsli, node, trgReg, srcReg, shiftAmount);
               cg->decReferenceCount(firstChild->getFirstChild());
               }
            else
               {
               srcReg = cg->evaluate(firstChild);
               generateShiftLeftImmediateLong(cg, node, trgReg, srcReg, shiftAmount);
               }
            }
         else
            {
            srcReg = cg->evaluate(firstChild);
            generateShiftLeftImmediateLong(cg, node, trgReg, srcReg, shiftAmount);
            }
         }
      else // 32 bit target
         {
         srcReg = cg->evaluate(firstChild);
         srcLowReg = srcReg->getLowOrder();
         srcHighReg = srcReg->getHighOrder();
         if (shiftAmount != 0)
            trgReg = cg->allocateRegisterPair(cg->allocateRegister(),
                        cg->allocateRegister());
         if (shiftAmount == 0)
            {
            trgReg = srcReg;
            }
         else if (shiftAmount < 32)
            {
            TR::Register *temp1Reg = cg->allocateRegister();
            TR::Register *temp2Reg = cg->allocateRegister();
            generateShiftLeftImmediate(cg, node, temp1Reg, srcHighReg, shiftAmount);
            generateShiftLeftImmediate(cg, node, trgReg->getLowOrder(), srcLowReg, shiftAmount);
            generateShiftRightLogicalImmediate(cg, node, temp2Reg, srcLowReg, 32-shiftAmount);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getHighOrder(), temp1Reg, temp2Reg);
            cg->stopUsingRegister(temp1Reg);
            cg->stopUsingRegister(temp2Reg);
            }
         else  // shiftAmount > 32
            {
            generateShiftLeftImmediate(cg, node, trgReg->getHighOrder(), srcLowReg, shiftAmount-32);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg->getLowOrder(), 0);
            }
         }
      }
   else
      {
      srcReg = cg->evaluate(firstChild);
      if (TR::Compiler->target.is64Bit())
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);

         trgReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::sld, node, trgReg, srcReg, src2Reg);
         }
      else // 32 bit target
         {
         srcLowReg = srcReg->getLowOrder();
         srcHighReg = srcReg->getHighOrder();
         trgReg = cg->allocateRegisterPair(cg->allocateRegister(),
                     cg->allocateRegister());

         TR::Register *temp1Reg = cg->allocateRegister();
         TR::Register *temp2Reg = cg->allocateRegister();
         TR::Register *temp3Reg = cg->allocateRegister();
         TR::Register *shiftAmountReg = cg->evaluate(secondChild);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node,temp1Reg, srcHighReg, shiftAmountReg);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node,trgReg->getLowOrder(), srcLowReg, shiftAmountReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, temp2Reg, shiftAmountReg, 32);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node,temp3Reg, srcLowReg, temp2Reg);
         cg->stopUsingRegister(temp2Reg);

         TR::Register *temp4Reg = cg->allocateRegister();
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi2, node, temp4Reg, shiftAmountReg, -32);

         TR::Register *temp5Reg = cg->allocateRegister();
         TR::Register *temp6Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp5Reg, srcLowReg, temp4Reg);
         cg->stopUsingRegister(temp4Reg);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp6Reg, temp3Reg, temp5Reg);
         cg->stopUsingRegister(temp3Reg);
         cg->stopUsingRegister(temp5Reg);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getHighOrder(), temp1Reg, temp6Reg);
         cg->stopUsingRegister(temp1Reg);
         cg->stopUsingRegister(temp6Reg);
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::ishflEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register  *trgReg         = cg->allocateRegister();
   TR::Node      *secondChild    = node->getSecondChild();
   TR::Node      *firstChild     = node->getFirstChild();
   TR::Register  *src1Reg        = cg->evaluate(firstChild);
   TR::ILOpCodes secondOp        = secondChild->getOpCodeValue();

   // Const shift amount, we know which shift to use (left or right)
   if (secondOp == TR::iconst || secondOp == TR::iuconst ||
      ((secondOp == TR::b2i) && ((secondChild->getFirstChild()->getOpCodeValue()) == TR::bconst)) ||
      ((secondOp == TR::s2i) && ((secondChild->getFirstChild()->getOpCodeValue()) == TR::sconst)) )
      {
      int32_t  shiftAmount = secondChild->getInt();

      if (secondOp == TR::b2i)
         shiftAmount = secondChild->getFirstChild()->getByte();

      if (secondOp == TR::s2i)
         shiftAmount = secondChild->getFirstChild()->getShortInt();

      if (shiftAmount > 0)
        generateShiftLeftImmediate(cg, node, trgReg, src1Reg, shiftAmount);
      else
        generateShiftRightLogicalImmediate(cg, node, trgReg, src1Reg, -shiftAmount);
      }
   else
      {
      TR::Register *temp1Reg       = cg->allocateRegister(); // Shift left result
      TR::Register *temp2Reg       = cg->allocateRegister(); // Negate result
      TR::Register *temp3Reg       = cg->allocateRegister(); // Shift right result

      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp1Reg, src1Reg, src2Reg);

      // Negate (res in temp2Reg)
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, temp2Reg, src2Reg);

      // Shift Right (res in temp3Reg)
      generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp3Reg, src1Reg, temp2Reg);

      // OR (res in trg4Reg)
      generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg, temp1Reg, temp3Reg);

      // Clean up
      cg->stopUsingRegister(temp1Reg);
      cg->stopUsingRegister(temp2Reg);
      cg->stopUsingRegister(temp3Reg);
      }

   node->setRegister(trgReg);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }


void ClearForNegativeShiftAmount(TR::Node *node, TR::CodeGenerator *cg, TR::Register  *trgReg, TR::Register *shiftAmountReg);
TR::Register * ShiftRightLong32Bit (TR::Node *node, TR::CodeGenerator *cg, TR::Register *srcReg, TR::Register *shiftAmountReg)
   {
   TR::Register *trgReg;
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();

   int32_t     srcLowValue;
   int32_t     srcHighValue;
   bool        constShift = (firstChild->getOpCodeValue() == TR::lconst);

   trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

   if (constShift)
      {
      srcLowValue = firstChild->getLongIntLow();
      srcHighValue = firstChild->getLongIntHigh();
      }
   if (constShift && srcHighValue == 0)
      {
      TR::Register *tempReg = cg->allocateRegister();
      loadConstant(cg, node, srcLowValue, tempReg);
      generateTrg1ImmInstruction(cg, TR::InstOpCode::li,  node, trgReg->getHighOrder(), 0);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::srw,  node, trgReg->getLowOrder(), tempReg, shiftAmountReg);
      cg->stopUsingRegister(tempReg);
      }
   else
      {
      TR::Register *srcLowReg;
      TR::Register *srcHighReg;
      bool         killSrcHigh = false;

      if (constShift && srcLowValue==0)
         {
         srcHighReg = cg->allocateRegister();
         loadConstant(cg, node, srcHighValue,srcHighReg);
         killSrcHigh = true;
         }
      else
         {
         srcLowReg  = srcReg->getLowOrder();
         srcHighReg = srcReg->getHighOrder();
         }
      TR::Register *temp1Reg       = NULL;
      TR::Register *temp2Reg       = cg->allocateRegister();
      TR::Register *temp3Reg       = cg->allocateRegister();
      TR::Register *temp4Reg       = cg->allocateRegister();
      TR::Register *temp5Reg       = cg->allocateRegister();
      generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, trgReg->getHighOrder(), srcHighReg, shiftAmountReg );
      if (!(constShift && srcLowValue==0))
         {
         temp1Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp1Reg, srcLowReg, shiftAmountReg );
         }
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, temp2Reg, shiftAmountReg, 32);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp3Reg, srcHighReg, temp2Reg);
      cg->stopUsingRegister(temp2Reg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, temp4Reg, shiftAmountReg, -32);

      generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp5Reg, srcHighReg, temp4Reg);
      cg->stopUsingRegister(temp4Reg);

      if (killSrcHigh)
         cg->stopUsingRegister(srcHighReg);

      if (constShift && srcLowValue==0)
         {
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp3Reg, temp5Reg);
         }
      else
         {
         TR::Register *temp6Reg       = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp6Reg, temp3Reg, temp5Reg);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg, temp6Reg);
         cg->stopUsingRegister(temp1Reg);
         cg->stopUsingRegister(temp6Reg);
         }

      cg->stopUsingRegister(temp3Reg);
      cg->stopUsingRegister(temp5Reg);

      // Clear trgReg to 0 if the shift amount is negative
      ClearForNegativeShiftAmount (node,cg, trgReg, shiftAmountReg);
      }
   return trgReg;
   }

TR::Register * ShiftRightLongImm32Bit (TR::Node *node, TR::CodeGenerator *cg, TR::Register *srcReg, int32_t shiftAmount)
   {
   TR::Register *srcLowReg, *srcHighReg, *trgReg;

   srcLowReg   = srcReg->getLowOrder();
   srcHighReg  = srcReg->getHighOrder();

   // shiftAmount is not 0
   trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

   if (shiftAmount > 32)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, 31);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getLowOrder(), srcHighReg, shiftAmount-32);
      }
   else if (shiftAmount == 32)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg->getLowOrder(), srcHighReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, 31);
      }
   else // (shiftAmount < 32)
      {
      TR::Register *temp1Reg = cg->allocateRegister();
      TR::Register *temp2Reg = cg->allocateRegister();
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, shiftAmount);
      generateShiftRightLogicalImmediate(cg, node, temp1Reg, srcLowReg, shiftAmount);
      generateShiftLeftImmediate(cg, node, temp2Reg, srcHighReg, 32-shiftAmount);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg, temp2Reg);
      cg->stopUsingRegister(temp1Reg);
      cg->stopUsingRegister(temp2Reg);
   }

   return trgReg;
   }

void ClearForNegativeShiftAmount(TR::Node *node, TR::CodeGenerator *cg, TR::Register  *trgReg, TR::Register *shiftAmountReg){
   TR::Register  *tmp1Reg = cg->allocateRegister();
   TR::Register  *tmp2Reg = cg->allocateRegister();

   // Get either all 1's or all 0s into tmp2, all 1's for pozitive shft amounts, all 0s for negative
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp1Reg, shiftAmountReg, 26, 0x1);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, tmp2Reg, tmp1Reg);

   // Clear out trgReg (low order 32 and high order 32) for negative shift amounts
   generateTrg1Src2Instruction(cg, TR::InstOpCode::andc, node, trgReg->getLowOrder(), trgReg->getLowOrder(), tmp2Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::andc, node, trgReg->getHighOrder(), trgReg->getHighOrder(), tmp2Reg);

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

}

TR::Register * ShiftLeftLong32Bit (TR::Node *node, TR::CodeGenerator *cg, TR::Register  *srcReg, TR::Register *shiftAmountReg){
   TR::Register  *srcLowReg, *srcHighReg, *trgReg;

   srcLowReg  = srcReg->getLowOrder();
   srcHighReg = srcReg->getHighOrder();
   trgReg     = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

   TR::Register *tmp1Reg = cg->allocateRegister();
   TR::Register *tmp2Reg = cg->allocateRegister();
   TR::Register *tmp3Reg = cg->allocateRegister();

   // srcHigh << shftAmt
   generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node,tmp1Reg, srcHighReg, shiftAmountReg);
   // srcLow  << shftAmt
   generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node,trgReg->getLowOrder(), srcLowReg, shiftAmountReg);

   // srcLow >> (32 -shftAmt)
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, tmp2Reg, shiftAmountReg, 32);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node,tmp3Reg, srcLowReg, tmp2Reg);

   // srcLow << (shftAmt - 32)
   TR::Register *temp4Reg = cg->allocateRegister();
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, temp4Reg, shiftAmountReg, -32);
   TR::Register *temp5Reg = cg->allocateRegister();
   TR::Register *temp6Reg = cg->allocateRegister();
   generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp5Reg, srcLowReg, temp4Reg);
   cg->stopUsingRegister(temp4Reg);

   // [srcLow << (32 -shftAmt)] | [srcLow << (shftAmt - 32)
   generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp6Reg, tmp3Reg, temp5Reg);
   cg->stopUsingRegister(tmp3Reg);
   cg->stopUsingRegister(temp5Reg);

   // { [srcLow << (32 -shftAmt)] | [srcLow << (shftAmt - 32)} | [srcHigh << shftAmt]
   generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getHighOrder(), tmp1Reg, temp6Reg);
   cg->stopUsingRegister(temp6Reg);

   // Clear trgReg to 0 if the shift amount is negative
   ClearForNegativeShiftAmount (node,cg, trgReg, shiftAmountReg);

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

   return trgReg;
}
TR::Register *OMR::Power::TreeEvaluator::lshflEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *temp1Reg;
   TR::Register *temp2Reg;
   TR::Register *temp3Reg;
   TR::Register *trgReg;


   bool         is64BitTgt   = TR::Compiler->target.is64Bit();
   TR::Node      *secondChild = node->getSecondChild();
   TR::Node      *firstChild  = node->getFirstChild();
   TR::ILOpCodes secondOp     = secondChild->getOpCodeValue();
   TR::Register  *srcReg, *srcLowReg, *srcHighReg;

   srcReg = cg->evaluate(firstChild);

   // Second Op Constant
   if ((secondOp == TR::iconst  || secondOp == TR::lconst ||
        secondOp == TR::iuconst || secondOp == TR::luconst) &&
        secondChild->getRegister() == NULL)
      {
      int32_t shiftAmount;
      if (secondOp == TR::iconst || secondOp == TR::iuconst)
         shiftAmount = secondChild->getInt();
      else
         shiftAmount = secondChild->getLongInt();

      // 64 Bit, second op constant
      if (is64BitTgt)
         {
         trgReg   = cg->allocateRegister();

         // Shift left or right
         if (shiftAmount > 0)
           generateShiftLeftImmediateLong(cg, node, trgReg, srcReg, shiftAmount);
         else
           generateShiftRightLogicalImmediateLong(cg, node, trgReg, srcReg, -shiftAmount);
         }

      // 32 Bit, second op constant
      else
         {
         srcReg = cg->evaluate(firstChild);
         srcLowReg = srcReg->getLowOrder();
         srcHighReg = srcReg->getHighOrder();

         // Shift amount 0, no shift
         if (shiftAmount == 0)
            trgReg = srcReg;

         // Shift amount pozitive, shift right
         else if (shiftAmount < 0)
            {
            trgReg = ShiftRightLongImm32Bit(node, cg, srcReg, -shiftAmount);
            }

         // Shift amount pozitive, shift left
         else if (shiftAmount < 32)
            {
            trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());
            TR::Register *temp1Reg = cg->allocateRegister();
            TR::Register *temp2Reg = cg->allocateRegister();
            generateShiftLeftImmediate(cg, node, temp1Reg, srcHighReg, shiftAmount);
            generateShiftLeftImmediate(cg, node, trgReg->getLowOrder(), srcLowReg, shiftAmount);
            generateShiftRightLogicalImmediate(cg, node, temp2Reg, srcLowReg, 32-shiftAmount);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getHighOrder(), temp1Reg, temp2Reg);
            cg->stopUsingRegister(temp1Reg);
            cg->stopUsingRegister(temp2Reg);
            }
         // shiftAmount > 32, shift left
         else
            {
            trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());
            generateShiftLeftImmediate(cg, node, trgReg->getHighOrder(), srcLowReg, shiftAmount-32);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg->getLowOrder(), 0);
            }
         }
      }

   // Second Op Not Constant
   else
   {
      // 64bit target
      if (TR::Compiler->target.is64Bit()) {
         TR::Register *shiftAmountReg = cg->evaluate(secondChild);

         temp1Reg = cg->allocateRegister();
         temp2Reg = cg->allocateRegister();
         temp3Reg = cg->allocateRegister();

         // 1. Shift Left (res in temp1Reg)
         generateTrg1Src2Instruction(cg, TR::InstOpCode::sld, node, temp1Reg, srcReg, shiftAmountReg);

         // 2. Negate (res in temp2Reg)
         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, temp2Reg, srcReg);

         // 3. Shift Right (res in temp3Reg)
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srd, node, temp3Reg, srcReg, temp2Reg);

         // 4. OR (res in trgReg)
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg, temp1Reg, temp3Reg);

         // Clean up
         cg->stopUsingRegister(temp1Reg);
         cg->stopUsingRegister(temp2Reg);
         cg->stopUsingRegister(temp3Reg);
      }

      // 32 bit target
      else {
         // 1. Shift Left, result in temp1Reg (Lo,Hi)
         TR::Register *shiftAmountReg = cg->evaluate(secondChild);
         temp1Reg = ShiftLeftLong32Bit(node,cg, srcReg, shiftAmountReg);

         // 2. Negate, result in temp2Reg
         temp2Reg = cg->allocateRegister();
         generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, temp2Reg, shiftAmountReg);

         // 3. Shift Right, result on temp3Reg
         temp3Reg = ShiftRightLong32Bit(node,cg, srcReg, temp2Reg);

         // 4. OR (res in trgReg)
         trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg->getLowOrder(), temp3Reg->getLowOrder());
         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getHighOrder(), temp1Reg->getHighOrder(), temp3Reg->getHighOrder());

         // Clean Up
         cg->stopUsingRegister(shiftAmountReg);
         cg->stopUsingRegister(temp1Reg);
         cg->stopUsingRegister(temp2Reg);
         cg->stopUsingRegister(temp3Reg);
      }
   }

   node->setRegister(trgReg);

   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
   }

// Also handles TR::bshr, TR::sshr
TR::Register *OMR::Power::TreeEvaluator::ishrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg         = cg->allocateRegister();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *src1Reg;

   if (secondChild->getOpCode().isLoadConst())
      {
      int32_t shiftAmount = secondChild->get64bitIntegralValue() & 0x1f;

      if (firstChild->getOpCode().isAnd() &&
          firstChild->getSecondChild()->getOpCode().isLoadConst() &&
          contiguousBits(firstChild->getSecondChild()->getInt()) &&
          firstChild->getSecondChild()->getInt() & 0x80000000 == 0 &&
          firstChild->getReferenceCount() == 1 &&
          firstChild->getRegister() == NULL)
         {
         uint32_t mask = (firstChild->getSecondChild()->getInt() >> shiftAmount) & 0xFFFFFFFF;
         if (mask == 0)// can't be encoded into rlwinm
            {
            cg->evaluate(firstChild->getFirstChild());
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
            }
         else
            {
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, cg->evaluate(firstChild->getFirstChild()), 32 - shiftAmount, mask);
            }
         cg->decReferenceCount(firstChild->getFirstChild());
         cg->decReferenceCount(firstChild->getSecondChild());
         }
      else
         {
         src1Reg = cg->evaluate(firstChild);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg, src1Reg, shiftAmount);
         }
      }
   else
      {
      src1Reg = cg->evaluate(firstChild);
      TR::Register *src2Reg = cg->evaluate(secondChild);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::sraw, node, trgReg, src1Reg, src2Reg);
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::lshrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *src1Reg;
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

   if (secondOp == TR::iconst || secondOp == TR::iuconst)
      {
      TR::Register *srcLowReg, *srcHighReg;
      int32_t shiftAmount = secondChild->getInt()&0x3f;
      src1Reg = cg->evaluate(firstChild);

      if (TR::Compiler->target.is64Bit())
         {
         trgReg = cg->allocateRegister();
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::sradi, node, trgReg, src1Reg, shiftAmount);
         }
      else // 32 bit target
         {
         srcLowReg = src1Reg->getLowOrder();
         srcHighReg = src1Reg->getHighOrder();
         if (shiftAmount != 0)
            trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

         if (shiftAmount == 0)
            {
            trgReg = src1Reg;
            }
         else if (shiftAmount > 32)
            {
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, 31);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getLowOrder(), srcHighReg, shiftAmount-32);
            }
         else if (shiftAmount == 32)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg->getLowOrder(), srcHighReg);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, 31);
            }
         else // (shiftAmount < 32)
            {
            TR::Register *temp1Reg         = cg->allocateRegister();
            TR::Register *temp2Reg         = cg->allocateRegister();
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, trgReg->getHighOrder(), srcHighReg, shiftAmount);
            generateShiftRightLogicalImmediate(cg, node, temp1Reg, srcLowReg, shiftAmount);
            generateShiftLeftImmediate(cg, node, temp2Reg, srcHighReg, 32-shiftAmount);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg, temp2Reg);
            cg->stopUsingRegister(temp1Reg);
            cg->stopUsingRegister(temp2Reg);
            }
         }
      }
   else // no constant shift amount
      if (TR::Compiler->target.is64Bit())
         {
         trgReg = cg->allocateRegister();
         TR::Register *src2Reg = cg->evaluate(secondChild);
         src1Reg = cg->evaluate(firstChild);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srad, node, trgReg, src1Reg, src2Reg);
         }
      else // 32 bit target
         {
         int32_t srcLowValue;
         int32_t srcHighValue;
         bool    constShift = (firstChild->getOpCodeValue() == TR::lconst);

         trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

         if (constShift)
            {
            srcLowValue = firstChild->getLongIntLow();
            srcHighValue = firstChild->getLongIntHigh();
            }
         TR::Register *shiftAmountReg = cg->evaluate(secondChild);

         // Didn't figure out an easy way for case: high==-1 && low>=0 where shiftamount may be 0
         if (constShift && (srcHighValue == 0 || (srcHighValue==-1 && srcLowValue<0)))
            {
            TR::Register *tempReg = cg->allocateRegister();

            loadConstant(cg, node, srcLowValue, tempReg);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li,  node, trgReg->getHighOrder(), srcHighValue);
            if (srcHighValue == 0)
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::srw,  node, trgReg->getLowOrder(), tempReg, shiftAmountReg);
               }
            else
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::sraw,  node, trgReg->getLowOrder(), tempReg, shiftAmountReg);
               }

            cg->stopUsingRegister(tempReg);
            }
      else
         {
         TR::Register *srcLowReg;
         TR::Register *srcHighReg;
         bool         killSrcHigh = false;

         if (constShift && srcLowValue==0)
            {
            srcHighReg = cg->allocateRegister();
            loadConstant(cg, node, srcHighValue,srcHighReg);
            killSrcHigh = true;
            }
         else
            {
            src1Reg = cg->evaluate(firstChild);
            srcLowReg  = src1Reg->getLowOrder();
            srcHighReg = src1Reg->getHighOrder();
            }
         TR::Register *temp1Reg = NULL;
         TR::Register *temp2Reg = cg->allocateRegister();
         TR::Register *temp3Reg = cg->allocateRegister();
         TR::Register *temp4Reg = cg->allocateRegister();
         TR::Register *temp5Reg = cg->allocateRegister();
         TR::Register *temp6Reg = cg->allocateRegister();
         TR::Register *temp7Reg = cg->allocateRegister();
         TR::Register *temp8Reg = cg->allocateRegister();
         TR::Register *temp9Reg = cg->allocateRegister();
         TR::Register *temp10Reg = cg->allocateRegister();
         TR::Register *temp11Reg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, TR::InstOpCode::sraw, node, trgReg->getHighOrder(), srcHighReg, shiftAmountReg );
         if (!(constShift && srcLowValue==0))
            {
            temp1Reg = cg->allocateRegister();
            generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp1Reg, srcLowReg, shiftAmountReg );
            }
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, temp2Reg, shiftAmountReg, 32);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp3Reg, srcHighReg, temp2Reg);
         cg->stopUsingRegister(temp2Reg);

         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, temp4Reg, shiftAmountReg, -32);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp5Reg, srcHighReg, temp4Reg);
         cg->stopUsingRegister(temp4Reg);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp6Reg, temp3Reg, temp5Reg);
         cg->stopUsingRegister(temp3Reg);
         cg->stopUsingRegister(temp5Reg);

         if (!(constShift && srcLowValue==0))
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp7Reg, temp1Reg, temp6Reg);
            cg->stopUsingRegister(temp1Reg);
            }

         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, temp8Reg, shiftAmountReg, 26, 0x80000000);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, temp9Reg, srcHighReg, temp8Reg);
         if (killSrcHigh)
            cg->stopUsingRegister(srcHighReg);
         cg->stopUsingRegister(temp8Reg);

         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, temp10Reg, shiftAmountReg, 0, 0x1f);

         generateTrg1Src2Instruction(cg, TR::InstOpCode::sraw, node,  temp11Reg, temp9Reg, temp10Reg);
         cg->stopUsingRegister(temp9Reg);
         cg->stopUsingRegister(temp10Reg);

         if (constShift && srcLowValue==0)
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node,  trgReg->getLowOrder(), temp6Reg, temp11Reg);
            }
         else
            {
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node,  trgReg->getLowOrder(), temp7Reg, temp11Reg);
            }
         cg->stopUsingRegister(temp6Reg);
         cg->stopUsingRegister(temp7Reg);
         cg->stopUsingRegister(temp11Reg);
         }
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

// Also handles TR::bushr
TR::Register *OMR::Power::TreeEvaluator::iushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg         = cg->allocateRegister();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *src1Reg;
   bool isByte = (node->getOpCodeValue() == TR::bushr);
   if (secondChild->getOpCode().isLoadConst())
      {
      int32_t shiftAmount = secondChild->get64bitIntegralValue();
      if (isByte && (shiftAmount > 7))
         {
         cg->evaluate(firstChild);
         generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
         }
      else if (firstChild->getOpCode().isAnd() &&
                firstChild->getSecondChild()->getOpCode().isLoadConst() &&
                contiguousBits(firstChild->getSecondChild()->getInt()) &&
                firstChild->getReferenceCount() == 1 &&
                firstChild->getRegister() == NULL)
         {
         uint32_t mask = ((uint32_t) firstChild->getSecondChild()->getInt() >> shiftAmount) & 0xFFFFFFFF;
         if (mask == 0)// can't be encoded into rlwinm
            {
            cg->evaluate(firstChild->getFirstChild());
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
            }
         else
            {
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, cg->evaluate(firstChild->getFirstChild()), 32 - shiftAmount, mask);
            }
         cg->decReferenceCount(firstChild->getFirstChild());
         cg->decReferenceCount(firstChild->getSecondChild());
         }
      else if(isByte)
         {
         uint32_t mask = (1 << (8 - shiftAmount)) - 1;
         src1Reg = cg->evaluate(firstChild);
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, src1Reg, 32 - shiftAmount, mask);
         }
      else
         {
         src1Reg = cg->evaluate(firstChild);
         generateShiftRightLogicalImmediate(cg, node, trgReg, src1Reg, shiftAmount & 0x1f);
         }
      }
   else
      {
      src1Reg = cg->evaluate(firstChild);
      TR::Register *src2Reg = cg->evaluate(secondChild);
      if(isByte)
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, src1Reg, src1Reg, 0, (uint32_t)0x000000FF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, trgReg, src1Reg, src2Reg);
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::lushrEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg;
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::Register *srcReg;
   TR::ILOpCodes secondOp            = secondChild->getOpCodeValue();

   if (secondOp == TR::iconst || secondOp == TR::iuconst)
      {
      int32_t shiftAmount = secondChild->getInt()&0x3f;
      srcReg = cg->evaluate(firstChild);
      if (TR::Compiler->target.is64Bit())
         {
         trgReg = cg->allocateRegister();
         generateShiftRightLogicalImmediateLong(cg, node, trgReg, srcReg, shiftAmount);
         }
      else
         {
         TR::Register *srcLowReg, *srcHighReg;
         srcLowReg = srcReg->getLowOrder();
         srcHighReg = srcReg->getHighOrder();

         if (shiftAmount != 0)
            trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

         if (shiftAmount == 0)
            {
            trgReg = srcReg;
            }
         else if (shiftAmount > 32)
            {
            generateShiftRightLogicalImmediate(cg, node, trgReg->getLowOrder(), srcHighReg, shiftAmount-32);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg->getHighOrder(), 0);
            }
         else if (shiftAmount == 32)
            {
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg->getHighOrder(), 0);
            generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg->getLowOrder(), srcHighReg);
            }
         else // (shiftAmount < 32)
            {
            TR::Register *temp1Reg         = cg->allocateRegister();
            TR::Register *temp2Reg         = cg->allocateRegister();
            generateShiftRightLogicalImmediate(cg, node, trgReg->getHighOrder(), srcHighReg, shiftAmount);
            generateShiftRightLogicalImmediate(cg, node, temp1Reg, srcLowReg, shiftAmount);
            generateShiftLeftImmediate(cg, node, temp2Reg, srcHighReg, 32-shiftAmount);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg, temp2Reg);
            cg->stopUsingRegister(temp1Reg);
            cg->stopUsingRegister(temp2Reg);
            }
         }
      }
   else  // no constant shift amount
      {
      TR::Register *shiftAmountReg = cg->evaluate(secondChild);
      if (TR::Compiler->target.is64Bit())
         {
         trgReg = cg->allocateRegister();
         srcReg = cg->evaluate(firstChild);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::srd, node, trgReg, srcReg, shiftAmountReg);
         }
      else
         {
         int32_t srcLowValue;
         int32_t srcHighValue;
         bool    constShift = (firstChild->getOpCodeValue() == TR::lconst);

         trgReg = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());

         if (constShift)
            {
            srcLowValue = firstChild->getLongIntLow();
            srcHighValue = firstChild->getLongIntHigh();
            }
         if (constShift && srcHighValue == 0)
            {
            TR::Register *tempReg = cg->allocateRegister();
            loadConstant(cg, node, srcLowValue, tempReg);
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li,  node, trgReg->getHighOrder(), 0);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::srw,  node, trgReg->getLowOrder(), tempReg, shiftAmountReg);
            cg->stopUsingRegister(tempReg);
            }
         else
            {
            TR::Register *srcLowReg;
            TR::Register *srcHighReg;
            bool         killSrcHigh = false;
            if (constShift && srcLowValue==0)
               {
               srcHighReg = cg->allocateRegister();
               loadConstant(cg, node, srcHighValue,srcHighReg);
               killSrcHigh = true;
               }
            else
               {
               srcReg = cg->evaluate(firstChild);
               srcLowReg  = srcReg->getLowOrder();
               srcHighReg = srcReg->getHighOrder();
               }
            TR::Register *temp1Reg       = NULL;
            TR::Register *temp2Reg       = cg->allocateRegister();
            TR::Register *temp3Reg       = cg->allocateRegister();
            TR::Register *temp4Reg       = cg->allocateRegister();
            TR::Register *temp5Reg       = cg->allocateRegister();
            generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, trgReg->getHighOrder(), srcHighReg, shiftAmountReg );
            if (!(constShift && srcLowValue==0))
               {
               temp1Reg = cg->allocateRegister();
               generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp1Reg, srcLowReg, shiftAmountReg );
               }
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, temp2Reg, shiftAmountReg, 32);
            generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, temp3Reg, srcHighReg, temp2Reg);
            cg->stopUsingRegister(temp2Reg);

            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, temp4Reg, shiftAmountReg, -32);

            generateTrg1Src2Instruction(cg, TR::InstOpCode::srw, node, temp5Reg, srcHighReg, temp4Reg);
            cg->stopUsingRegister(temp4Reg);

            if (killSrcHigh)
               cg->stopUsingRegister(srcHighReg);

            if (constShift && srcLowValue==0)
               {
               generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp3Reg, temp5Reg);
               }
            else
               {
               TR::Register *temp6Reg       = cg->allocateRegister();
               generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, temp6Reg, temp3Reg, temp5Reg);
               generateTrg1Src2Instruction(cg, TR::InstOpCode::OR, node, trgReg->getLowOrder(), temp1Reg, temp6Reg);
               cg->stopUsingRegister(temp1Reg);
               cg->stopUsingRegister(temp6Reg);
               }

            cg->stopUsingRegister(temp3Reg);
            cg->stopUsingRegister(temp5Reg);
            }
         }
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::irolEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return inlineIntegerRotateLeft(node, cg);
   }

TR::Register *OMR::Power::TreeEvaluator::lrolEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return inlineLongRotateLeft(node, cg);
   }

void simplifyANDRegImm(TR::Node * node, TR::Register *trgReg, TR::Register *srcReg, int64_t value, TR::CodeGenerator * cg, TR::Node *constNode=NULL)
   {
   if (value == 0)
      {
      generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
      }
   else if (value == CONSTANT64(-1))
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg, srcReg);
      }
   else if ((value&CONSTANT64(0xffffffff00000000)) == 0) // no bits in upper word
      {
      int32_t lowWord = (int32_t)value;;
      if (contiguousBits(lowWord))
         {
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, srcReg, 0, lowWord);
         if (((lowWord&0x80000001)==0x80000001) && (lowWord!=0xffffffff))
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, trgReg, 0, 0xffffffff);
         }
      else
         simplifyANDRegImm(node, trgReg, srcReg, lowWord, cg, constNode);
      }
   else // bits in upper word
      {
      if (contiguousBits(value))
         {
         bool lz = ((value & CONSTANT64(0x8000000000000000))==0); // are there leading zeroes?
         bool tz = ((value & 1)==0);                    // are there trailing zeroes?
         if ((lz==false) && (tz==true))
            {
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicr, node, trgReg, srcReg, 0, value);
            return;
            }
         else if ((lz==true) && (tz==false))
            {
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicl, node, trgReg, srcReg, 0, value);
            return;
            }
         }
      TR::Register *tmpReg;
      if (constNode!=NULL)
         {
         tmpReg = cg->evaluate(constNode);
         }
      else
         {
         tmpReg = cg->allocateRegister();
         loadConstant(cg, node, value, tmpReg);
         }
      generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg, srcReg, tmpReg);
      cg->stopUsingRegister(tmpReg);
      }
   }

void simplifyANDRegImm(TR::Node * node, TR::Register *trgReg, TR::Register *srcReg, int32_t value, TR::CodeGenerator * cg, TR::Node *constNode)
   {
   intParts localVal(value);
   if (localVal.getValue() == 0)
      {
      generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
      }
   else if (localVal.getValue() == -1)
     {
     generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg, srcReg);
     }
   else if (contiguousBits(localVal.getValue()))
      {
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, srcReg, 0, localVal.getValue());
      }
   else if (localVal.getHighBits() == 0)
      {
      TR::Register *tmpReg = cg->allocateRegister(TR_CCR);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, trgReg, srcReg, tmpReg, localVal.getLowBits());
      cg->stopUsingRegister(tmpReg);
      }
   else if (localVal.getLowBits() == 0)
      {
      TR::Register *tmpReg = cg->allocateRegister(TR_CCR);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andis_r, node, trgReg, srcReg, tmpReg, localVal.getHighBits());
      cg->stopUsingRegister(tmpReg);
      }
   else
      {
      TR::Register *tmpReg;
      if (constNode!=NULL)
         {
         tmpReg = cg->evaluate(constNode);
         }
      else
         {
         tmpReg = cg->allocateRegister();
         loadConstant(cg, node, value, tmpReg);
         }
      generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg, srcReg, tmpReg);
      cg->stopUsingRegister(tmpReg);
      }
   }

// We want to ensure that constants don't get reloaded into registers multiple times, and also to ensure that constants are not loaded
// into registers when an immediate form of an instruction will do.  This method is used for 'long and' on a 32 bit platform.  We will
// call the normal simplifyANDRegImm for a 32 bit value for each of the high and low halves on as long as we don't expect both to load
// their respective constants into a register. In that case we evaluate the constant node so it can be commoned.
void simplifyANDRegImm(TR::Node *node, TR::Register *trgReg, TR::Register *srcReg, TR::Node *constNode, TR::CodeGenerator * cg)
   {
   int32_t lowValue;
   int32_t highValue;
   bool lowSpecial = false;
   bool highSpecial = false;

   lowValue = constNode->getLongIntLow();
   highValue = constNode->getLongIntHigh();

   intParts localLow(lowValue);
   intParts localHigh(highValue);

   if ((localLow.getValue() == -1) || (contiguousBits(localLow.getValue())) ||
      (localLow.getHighBits() == 0) || (localLow.getLowBits() == 0))
      {
      lowSpecial = true;
      }

   if ((localHigh.getValue() == -1) || (contiguousBits(localHigh.getValue())) ||
      (localHigh.getHighBits() == 0) || (localHigh.getLowBits() == 0))
      {
      highSpecial = true;
      }

   if (lowSpecial == true || highSpecial == true)
     {
     simplifyANDRegImm(node, trgReg->getLowOrder(), srcReg->getLowOrder(), constNode->getLongIntLow(), cg);
     simplifyANDRegImm(node, trgReg->getHighOrder(), srcReg->getHighOrder(), constNode->getLongIntHigh(), cg);
     }
   else
     {
     TR::Register *constReg = cg->evaluate(constNode);
     generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg->getLowOrder(), srcReg->getLowOrder(), constReg->getLowOrder());
     generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg->getHighOrder(), srcReg->getHighOrder(), constReg->getHighOrder());
     }
   }

TR::Register *OMR::Power::TreeEvaluator::landEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild  = node->getFirstChild();
   TR::Node *secondChild    = node->getSecondChild();
   TR::Register *trgReg  = NULL;
   TR::ILOpCodes secondOp = secondChild->getOpCodeValue();

   if (TR::Compiler->target.is64Bit())
      {
      TR::Register *src1Reg = cg->evaluate(firstChild);
      trgReg  = cg->allocateRegister();

      if ((secondOp == TR::lconst || secondOp == TR::luconst) &&
          secondChild->getRegister() == NULL)
         {
         simplifyANDRegImm(node, trgReg, src1Reg, secondChild->getLongInt(), cg, secondChild);
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg, src2Reg, src1Reg);
         }
      }
   else // 32 bit target
      {
      if ((secondOp == TR::lconst || secondOp == TR::luconst) &&
          secondChild->getRegister() == NULL)
         {
         TR::Register *src1Reg = cg->evaluate(firstChild);

         trgReg  = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());
         simplifyANDRegImm(node, trgReg, src1Reg, secondChild, cg);
         }
      else
         {
         if (firstChild->isHighWordZero() || secondChild->isHighWordZero())
            {
            return carrylessLongEvaluatorWithAnalyser(node, cg,
                                                        TR::InstOpCode::AND,
                                                        TR::InstOpCode::AND,
                                                        TR::InstOpCode::mr);
            }
         else
            {
            TR::Register *src1Reg = cg->evaluate(firstChild);
            TR::Register *src2Reg = cg->evaluate(secondChild);

            trgReg  = cg->allocateRegisterPair(cg->allocateRegister(), cg->allocateRegister());
            generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg->getLowOrder(), src2Reg->getLowOrder(), src1Reg->getLowOrder());
            generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg->getHighOrder(), src2Reg->getHighOrder(), src1Reg->getHighOrder());
            }
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

// Do the work for evaluating long or and exclusive or
static inline TR::Register *lorTypeEvaluator(TR::Node *node,
                                            TR::InstOpCode::Mnemonic immedOp,
                                            TR::InstOpCode::Mnemonic immedShiftedOp,
                                            TR::InstOpCode::Mnemonic regOp,
                                            TR::InstOpCode::Mnemonic regOp_r,
                                            TR::CodeGenerator *cg)
   {
   TR::Register *trgReg  = NULL;
   TR::Register *src1Reg  = NULL;
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild  = node->getFirstChild();
   TR::ILOpCodes  secondOp = secondChild->getOpCodeValue();

   if (TR::Compiler->target.is64Bit())
      {
      if ((secondOp == TR::lconst || secondOp == TR::luconst) &&
         secondChild->getRegister() == NULL)
         {
         uint64_t longConst = secondChild->getLongInt();
         if ((node->getOpCodeValue()==TR::lxor) && longConst==(int64_t)-1)
            {
            trgReg = cg->allocateRegister();
            src1Reg = cg->evaluate(firstChild);
            generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::subfic, node, trgReg, src1Reg, longConst);
            }
         else if ((longConst & 0xffffffff) == longConst) // upper 32 bits are all zero
            {
            return iorTypeEvaluator(node, immedOp, immedShiftedOp, regOp, regOp_r, cg);
            }
         }
      if (trgReg==NULL)
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         src1Reg = cg->evaluate(firstChild);
         trgReg = cg->allocateRegister();
         generateTrg1Src2Instruction(cg, regOp, node, trgReg, src1Reg, src2Reg);
         }
      }
   else // 32 bit target
      {
      trgReg  = cg->allocateRegisterPair(cg->allocateRegister(),
                                                             cg->allocateRegister());
      TR::Register *src1Reg = cg->evaluate(firstChild);

      if ((secondOp == TR::lconst || secondOp == TR::luconst) &&
          secondChild->getRegister() == NULL)
         {
         intParts localVal(secondChild->getLongIntLow());
         if (localVal.getValue() == 0)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::mr,node, trgReg->getLowOrder(), src1Reg->getLowOrder());
            }
         else if (localVal.getValue()==-1 && (node->getOpCodeValue()==TR::lor))
            {
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li,node, trgReg->getLowOrder(), -1);
            }
         else
            {
            if (localVal.getHighBits() == 0)
               {
               generateTrg1Src1ImmInstruction(cg, immedOp, node, trgReg->getLowOrder(), src1Reg->getLowOrder(), localVal.getLowBits());
               }
            else if (localVal.getLowBits() == 0)
               {
               generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg->getLowOrder(), src1Reg->getLowOrder(), localVal.getHighBits());
               }
            else
               {
               TR::Register *tempReg = cg->allocateRegister();
               generateTrg1Src1ImmInstruction(cg, immedOp, node, tempReg, src1Reg->getLowOrder(), localVal.getLowBits());
               generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg->getLowOrder(), tempReg, localVal.getHighBits());
               cg->stopUsingRegister(tempReg);
               }
            }
         localVal.setValue(secondChild->getLongIntHigh());
         if (localVal.getValue() == 0)
            {
            generateTrg1Src1Instruction(cg, TR::InstOpCode::mr,node, trgReg->getHighOrder(), src1Reg->getHighOrder());
            }
         else if (localVal.getValue()==-1 && (node->getOpCodeValue()==TR::lor))
            {
            generateTrg1ImmInstruction(cg, TR::InstOpCode::li,node, trgReg->getHighOrder(), -1);
            }
         else
            {
            if (localVal.getHighBits() == 0)
               {
               generateTrg1Src1ImmInstruction(cg, immedOp, node, trgReg->getHighOrder(), src1Reg->getHighOrder(), localVal.getLowBits());
               }
            else if (localVal.getLowBits() == 0)
               {
               generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg->getHighOrder(), src1Reg->getHighOrder(), localVal.getHighBits());
               }
            else
               {
               TR::Register *tempReg = cg->allocateRegister();
               generateTrg1Src1ImmInstruction(cg, immedOp, node, tempReg, src1Reg->getHighOrder(), localVal.getLowBits());
               generateTrg1Src1ImmInstruction(cg, immedShiftedOp, node, trgReg->getHighOrder(), tempReg, localVal.getHighBits());
               cg->stopUsingRegister(tempReg);
               }
            }
         }
      else
         {
         TR::Register *src2Reg = cg->evaluate(secondChild);
         generateTrg1Src2Instruction(cg, regOp, node, trgReg->getLowOrder(), src2Reg->getLowOrder(), src1Reg->getLowOrder());
         generateTrg1Src2Instruction(cg, regOp, node, trgReg->getHighOrder(), src2Reg->getHighOrder(), src1Reg->getHighOrder());
         }
      }
   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }


TR::Register *OMR::Power::TreeEvaluator::lorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::ILOpCodes secondOp = node->getSecondChild()->getOpCodeValue();

   if ((node->getFirstChild()->isHighWordZero() || node->getSecondChild()->isHighWordZero()) &&
       !((secondOp == TR::lconst || secondOp == TR::luconst) && node->getSecondChild()->getRegister() == NULL) &&
       !(TR::Compiler->target.is64Bit()))
      {
      return carrylessLongEvaluatorWithAnalyser(node, cg,
                                                    TR::InstOpCode::OR,
                                                    TR::InstOpCode::OR,
                                                    TR::InstOpCode::mr);
      }
   else
      {
      return lorTypeEvaluator(node, TR::InstOpCode::ori, TR::InstOpCode::oris, TR::InstOpCode::OR, TR::InstOpCode::or_r, cg);
      }
   }

TR::Register *OMR::Power::TreeEvaluator::lxorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::ILOpCodes secondOp = node->getSecondChild()->getOpCodeValue();

   if ((node->getFirstChild()->isHighWordZero() || node->getSecondChild()->isHighWordZero()) &&
       !((secondOp == TR::lconst || secondOp == TR::luconst) && node->getSecondChild()->getRegister() == NULL) &&
       !(TR::Compiler->target.is64Bit()))
      {
      return carrylessLongEvaluatorWithAnalyser(node, cg,
                                                    TR::InstOpCode::XOR,
                                                    TR::InstOpCode::XOR,
                                                    TR::InstOpCode::mr);
      }
   else
      {
      return lorTypeEvaluator(node, TR::InstOpCode::xori, TR::InstOpCode::xoris, TR::InstOpCode::XOR, TR::InstOpCode::xor_r, cg);
      }
   }

// also handles iuand
TR::Register *OMR::Power::TreeEvaluator::iandEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *trgReg         = cg->allocateRegister();
   TR::Node     *secondChild    = node->getSecondChild();
   TR::Node     *firstChild     = node->getFirstChild();
   TR::ILOpCodes secondOp       = secondChild->getOpCodeValue();

   if (secondChild->getOpCode().isLoadConst() &&
       secondChild->getRegister() == NULL)
      {
      if (cg->isRotateAndMask(node))
         {
         if (firstChild->getOpCodeValue() == TR::imul ||
             firstChild->getOpCodeValue() == TR::iumul)
            {
            int32_t multiplier = firstChild->getSecondChild()->getInt();
            int32_t shiftAmount = 0;
            while ((multiplier = ((uint32_t)multiplier) >> 1))
               ++shiftAmount;
            if( (secondChild->getInt() & (0xffffffff << shiftAmount)) == 0 )
               generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, trgReg, 0);
            else
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, cg->evaluate(firstChild->getFirstChild()), shiftAmount, secondChild->getInt() & (0xffffffff << shiftAmount));
            }
         else  // ishr or iushr
            {
            int32_t shiftAmount = firstChild->getSecondChild()->getInt();
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, trgReg, cg->evaluate(firstChild->getFirstChild()), 32 - shiftAmount, secondChild->getInt() & (((uint32_t) 0xffffffff) >> shiftAmount));
            }
         cg->decReferenceCount(firstChild->getFirstChild());
         cg->decReferenceCount(firstChild->getSecondChild());
         }
      else
         simplifyANDRegImm(node, trgReg, cg->evaluate(firstChild), (int32_t)secondChild->get64bitIntegralValue(), cg, secondChild);
      }
   else
      {
      generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, trgReg, cg->evaluate(firstChild), cg->evaluate(secondChild));
      }

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::iorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return iorTypeEvaluator(node, TR::InstOpCode::ori, TR::InstOpCode::oris, TR::InstOpCode::OR, TR::InstOpCode::or_r, cg);
   }

TR::Register *OMR::Power::TreeEvaluator::ixorEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return iorTypeEvaluator(node, TR::InstOpCode::xori, TR::InstOpCode::xoris, TR::InstOpCode::XOR, TR::InstOpCode::xor_r, cg);
   }

// Multiply a register by a constant
static void mulConstant(TR::Node * node, TR::Register *trgReg, TR::Register *sourceReg, int32_t value, TR::CodeGenerator *cg)
   {
   if (value == 0)
      {
      loadConstant(cg, node, 0, trgReg);
      }
   else if (value == 1)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg, sourceReg);
      }
   else if (value == -1)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, sourceReg);
      }
   else if (isNonNegativePowerOf2(value) || value==TR::getMinSigned<TR::Int32>())
      {
      generateShiftLeftImmediate(cg, node, trgReg, sourceReg, trailingZeroes(value));
      }
   else if (isNonPositivePowerOf2(value))
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediate(cg, node, tempReg, sourceReg, trailingZeroes(value));
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (isNonNegativePowerOf2(value-1) || (value-1)==TR::getMinSigned<TR::Int32>())
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediate(cg, node, tempReg, sourceReg, trailingZeroes(value-1));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, tempReg, sourceReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (isNonNegativePowerOf2(value+1))
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediate(cg, node, tempReg, sourceReg, trailingZeroes(value+1));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, sourceReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (value >= LOWER_IMMED && value <= UPPER_IMMED)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::mulli, node, trgReg, sourceReg, value);
      }
   else   // constant won't fit
      {
      TR::Register *tempReg = cg->allocateRegister();
      loadConstant(cg, node, value, tempReg);
      // want the smaller of the sources in the RB position of a multiply
      // one crude measure of absolute size is the number of leading zeros
      if (leadingZeroes(abs(value)) >= 24)
         // the constant is fairly small, so put it in RB
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, trgReg, sourceReg, tempReg);
      else
         // the constant is fairly big, so put it in RA
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, trgReg, tempReg, sourceReg);
      cg->stopUsingRegister(tempReg);
      }
   }


// Multiply a register by a constant
static void mulConstant(TR::Node * node, TR::Register *trgReg, TR::Register *sourceReg, int64_t value, TR::CodeGenerator *cg)
   {
   if (value == 0)
      {
      loadConstant(cg, node, 0, trgReg);
      }
   else if (value == 1)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, trgReg, sourceReg);
      }
   else if (value == -1)
      {
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, sourceReg);
      }
   else if (isNonNegativePowerOf2(value) || value==TR::getMinSigned<TR::Int64>())
      {
      generateShiftLeftImmediateLong(cg, node, trgReg, sourceReg, trailingZeroes(value));
      }
   else if (isNonPositivePowerOf2(value))
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediateLong(cg, node, tempReg, sourceReg, trailingZeroes(value));
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, trgReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (isNonNegativePowerOf2(value-1) || (value-1)==TR::getMinSigned<TR::Int64>())
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediateLong(cg, node, tempReg, sourceReg, trailingZeroes(value-1));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, trgReg, tempReg, sourceReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (isNonNegativePowerOf2(value+1))
      {
      TR::Register *tempReg = cg->allocateRegister();
      generateShiftLeftImmediateLong(cg, node, tempReg, sourceReg, trailingZeroes(value+1));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, sourceReg, tempReg);
      cg->stopUsingRegister(tempReg);
      }
   else if (value >= LOWER_IMMED && value <= UPPER_IMMED)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::mulli, node, trgReg, sourceReg, value);
      }
   else   // constant won't fit
      {
      TR::Register *tempReg = cg->allocateRegister();
      uint64_t abs_value = value >= 0 ? value : -value;
      loadConstant(cg, node, value, tempReg);
      // want the smaller of the sources in the RB position of a multiply
      // one crude measure of absolute size is the number of leading zeros
      if (leadingZeroes(abs_value) >= 56)
         // the constant is fairly small, so put it in RB
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld, node, trgReg, sourceReg, tempReg);
      else
         // the constant is fairly big, so put it in RA
         generateTrg1Src2Instruction(cg, TR::InstOpCode::mulld, node, trgReg, tempReg, sourceReg);
      cg->stopUsingRegister(tempReg);
      }
   }


TR::Register *OMR::Power::TreeEvaluator::ixfrsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node * firstChild  = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();
   TR::Register *src1Reg  = cg->evaluate(firstChild);
   TR::Register *src2Reg  = cg->evaluate(secondChild);

   TR::Register *trgReg   = cg->allocateRegister();
   TR::Register *tmp1Reg  = cg->allocateRegister();
   TR::Register *tmp2Reg  = cg->allocateRegister();

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, src1Reg, 31);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp2Reg, src2Reg, 31);
   // trg = absolute value of the first child
   generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg, src1Reg, tmp1Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tmp1Reg, trgReg);
   // apply sign of the second child to trg
   generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg, trgReg, tmp2Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tmp2Reg, trgReg);

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }


TR::Register *OMR::Power::TreeEvaluator::lxfrsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node * firstChild  = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();
   TR::Register *src1Reg  = cg->evaluate(firstChild);
   TR::Register *src2Reg  = cg->evaluate(secondChild);
   TR::Register *tmp1Reg  = cg->allocateRegister();
   TR::Register *tmp2Reg  = cg->allocateRegister();
   TR::Register  *trgReg;

   if (TR::Compiler->target.is32Bit())
      {
      TR::Register *lowReg  = cg->allocateRegister();
      TR::Register *highReg = cg->allocateRegister();
      trgReg = cg->allocateRegisterPair(lowReg, highReg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, src1Reg->getHighOrder(), 31);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp2Reg, src2Reg->getHighOrder(), 31);
      // trg = absolute value of the first child
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg->getLowOrder(), src1Reg->getLowOrder(), tmp1Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg->getHighOrder(), src1Reg->getHighOrder(), tmp1Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, trgReg->getLowOrder(), tmp1Reg, trgReg->getLowOrder());
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, trgReg->getHighOrder(), tmp1Reg, trgReg->getHighOrder());
      // apply sign of the second child to trg
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg->getLowOrder(), trgReg->getLowOrder(), tmp2Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg->getHighOrder(), trgReg->getHighOrder(), tmp2Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, trgReg->getLowOrder(), tmp2Reg, trgReg->getLowOrder());
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, trgReg->getHighOrder(), tmp2Reg, trgReg->getHighOrder());
      }
   else
      {
      trgReg   = cg->allocateRegister();
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, src1Reg, 31);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp2Reg, src2Reg, 31);
      // trg = absolute value of the first child
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg, src1Reg, tmp1Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tmp1Reg, trgReg);
      // apply sign of the second child to trg
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, trgReg, trgReg, tmp2Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, trgReg, tmp2Reg, trgReg);
      }

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   return trgReg;
   }

TR::Register *OMR::Power::TreeEvaluator::idozEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{

   TR::Node * firstChild  = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();
   TR::Register *src1Reg  = cg->evaluate(firstChild);  // a
   TR::Register *src2Reg = cg->evaluate(secondChild);  // b

   TR::Register *tmp1Reg = cg->allocateRegister();
   TR::Register *tmp2Reg = cg->allocateRegister();

   // Flip the sign bit: tmp1 = 2^31 + a; tmp2 = 2^31 + b
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::xoris, node, tmp1Reg, src1Reg, 0x8000);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::xoris, node, tmp2Reg, src2Reg, 0x8000);

   // tmp1Reg = a - b
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, tmp1Reg, tmp2Reg, tmp1Reg);

   // tmp2Reg = -1 if a <= b
   //           0  if a >  b
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, tmp2Reg, tmp1Reg, tmp1Reg);

   TR::Register  *trgReg = cg->allocateRegister();
   generateTrg1Src2Instruction(cg, TR::InstOpCode::andc, node, trgReg, tmp1Reg, tmp2Reg);

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
}

TR::Register *OMR::Power::TreeEvaluator::muloverEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{

   TR::Node * firstChild  = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();
   TR::Register *src1Reg  = cg->evaluate(firstChild);  // x
   TR::Register *src2Reg = cg->evaluate(secondChild);  // y

   TR::ILOpCodes secondOp            = secondChild->getOpCodeValue();

   TR::Register *tmp1Reg = cg->allocateRegister();
   TR::Register *tmp2Reg = cg->allocateRegister();
   TR::Register *trgReg = cg->allocateRegister();

   if ((secondOp == TR::iuconst) || (secondOp == TR::iuload))
   {
     // x, y, unsigned, x*y overflows if hi(x*y) !=0;
     // Compute tmp1 = hi(x,y)
     generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, tmp1Reg, src1Reg, src2Reg);

   }
   else
   {
     // x, y, signed, x*y overflows if hi(x*y) != [ low(x,y) >> 31] i.e. [hi(x*y) - low(x,y)>>31] != 0
     // Calculate [hi(x*y) - low(x,y)>>31]

    //Compute tmp1 = low(x,y) >> 31
     generateTrg1Src2Instruction(cg, TR::InstOpCode::mullw, node, tmp1Reg, src1Reg, src2Reg);
     generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp1Reg, tmp1Reg, 31);

     //Compute tmp2 = hi(x*y)
     generateTrg1Src2Instruction(cg, TR::InstOpCode::mulhw, node, tmp2Reg, src1Reg, src2Reg);

     //tmp1 = tmp2 - tmp1
     generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmp1Reg, tmp2Reg, tmp1Reg);
   }

   // result false if tmp1 is 0, 1 otherwise
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, tmp2Reg, tmp1Reg, 0xFFFF);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, trgReg, tmp2Reg, tmp1Reg);

   cg->stopUsingRegister(tmp1Reg);
   cg->stopUsingRegister(tmp2Reg);

   node->setRegister(trgReg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return trgReg;
}

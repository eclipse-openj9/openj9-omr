/*******************************************************************************
 * Copyright (c) 2018, 2019 IBM Corp. and others
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

#include "z/codegen/S390Peephole.hpp"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "codegen/CodeGenPhase.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/CodeGenerator_inlines.hpp"
#include "codegen/ConstantDataSnippet.hpp"
#include "env/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterPair.hpp"
#include "codegen/Snippet.hpp"
#include "il/LabelSymbol.hpp"
#include "ras/Debug.hpp"
#include "ras/DebugCounter.hpp"
#include "ras/Delimiter.hpp"
#include "runtime/Runtime.hpp"
#include "z/codegen/CallSnippet.hpp"
#include "z/codegen/OpMemToMem.hpp"
#include "z/codegen/S390Evaluator.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"
#include "z/codegen/S390Instruction.hpp"
#include "z/codegen/S390OutOfLineCodeSection.hpp"
#include "z/codegen/SystemLinkage.hpp"

TR_S390Peephole::TR_S390Peephole(TR::Compilation* comp)
   : _fe(comp->fe()),
     _outFile(comp->getOutFile()),
     _cursor(comp->cg()->getFirstInstruction()),
     _cg(comp->cg())
   {
   }

///////////////////////////////////////////////////////////////////////////////

TR::Instruction* realInstruction(TR::Instruction* inst, bool forward)
   {
   while (inst && (inst->getKind() == TR::Instruction::IsPseudo      ||
                   inst->getKind() == TR::Instruction::IsNotExtended ||
                   inst->getKind() == TR::Instruction::IsLabel))
      {
      inst = forward ? inst->getNext() : inst->getPrev();
      }

   return inst;
   }

TR::Instruction* realInstructionWithLabels(TR::Instruction* inst, bool forward)
   {
   while (inst && (inst->getKind() == TR::Instruction::IsPseudo ||
                   inst->getKind() == TR::Instruction::IsNotExtended))
      {
      inst = forward ? inst->getNext() : inst->getPrev();
      }

   return inst;
   }

bool
TR_S390Peephole::isBarrierToPeepHoleLookback(TR::Instruction *current)
   {
   if (NULL == current) return true;

   TR::Instruction *s390current = current;

   if (s390current->isLabel()) return true;
   if (s390current->isCall())  return true;
   if (s390current->isBranchOp()) return true;
   if (s390current->getOpCodeValue() == TR::InstOpCode::DCB) return true;

   return false;
   }

bool
TR_S390Peephole::clearsHighBitOfAddressInReg(TR::Instruction *inst, TR::Register *targetReg)
   {
   if (inst->defsRegister(targetReg))
      {
      if (inst->getOpCodeValue() == TR::InstOpCode::LA)
         {
         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "LA inst 0x%x clears high bit on targetReg 0x%x (%s)\n", inst, targetReg, targetReg->getRegisterName(comp()));
         return true;
         }

      if (inst->getOpCodeValue() == TR::InstOpCode::LAY)
         {
         if (comp()->getOption(TR_TraceCG))
            traceMsg(comp(), "LAY inst 0x%x clears high bit on targetReg 0x%x (%s)\n", inst, targetReg, targetReg->getRegisterName(comp()));
         return true;
         }

      if (inst->getOpCodeValue() == TR::InstOpCode::NILH)
         {
         TR::S390RIInstruction * NILH_RI = (TR::S390RIInstruction *)inst;
         if (NILH_RI->isImm() &&
             NILH_RI->getSourceImmediate() == 0x7FFF)
            {
            if (comp()->getOption(TR_TraceCG))
               traceMsg(comp(), "NILH inst 0x%x clears high bit on targetReg 0x%x (%s)\n", inst, targetReg, targetReg->getRegisterName(comp()));
            return true;
            }
         }
      }
   return false;
   }

static TR::Instruction *
findActiveCCInst(TR::Instruction *curr, TR::InstOpCode::Mnemonic op, TR::Register *ccReg)
   {
   TR::Instruction * next = curr;
   while (next = next->getNext())
      {
      if (next->getOpCodeValue() == op)
         return next;
      if (next->usesRegister(ccReg) ||
          next->isLabel() ||
          next->getOpCode().setsCC() ||
          next->isCall() ||
          next->getOpCode().setsCompareFlag())
         break;
      }
   return NULL;
   }

bool swapOperands(TR::Register * trueReg, TR::Register * compReg, TR::Instruction * curr)
   {
   TR::InstOpCode::S390BranchCondition branchCond;
   uint8_t mask;
   switch (curr->getKind())
      {
      case TR::Instruction::IsRR:
         branchCond = ((TR::S390RRInstruction *) curr)->getBranchCondition();
         ((TR::S390RRInstruction *) curr)->setBranchCondition(getReverseBranchCondition(branchCond));
         ((TR::S390RRInstruction *) curr)->setRegisterOperand(2,trueReg);
         ((TR::S390RRInstruction *) curr)->setRegisterOperand(1,compReg);
         break;
      case TR::Instruction::IsRIE:
         branchCond = ((TR::S390RIEInstruction *) curr)->getBranchCondition();
         ((TR::S390RIEInstruction *) curr)->setBranchCondition(getReverseBranchCondition(branchCond));
         ((TR::S390RIEInstruction *) curr)->setRegisterOperand(2,trueReg);
         ((TR::S390RIEInstruction *) curr)->setRegisterOperand(1,compReg);
         break;
      case TR::Instruction::IsRRS:
         branchCond = ((TR::S390RRSInstruction *) curr)->getBranchCondition();
         ((TR::S390RRSInstruction *) curr)->setBranchCondition(getReverseBranchCondition(branchCond));
         ((TR::S390RRSInstruction *) curr)->setRegisterOperand(2,trueReg);
         ((TR::S390RRSInstruction *) curr)->setRegisterOperand(1,compReg);
         break;
      case TR::Instruction::IsRRD: // RRD is encoded use RRF
      case TR::Instruction::IsRRF:
         branchCond = ((TR::S390RRFInstruction *) curr)->getBranchCondition();
         ((TR::S390RRFInstruction *) curr)->setBranchCondition(getReverseBranchCondition(branchCond));
         ((TR::S390RRFInstruction *) curr)->setRegisterOperand(2,trueReg);
         ((TR::S390RRFInstruction *) curr)->setRegisterOperand(1,compReg);
         break;
      case TR::Instruction::IsRRF2:
         mask = ((TR::S390RRFInstruction *) curr)->getMask3();
         ((TR::S390RRFInstruction *) curr)->setMask3(getReverseBranchMask(mask));
         ((TR::S390RRFInstruction *) curr)->setRegisterOperand(2,trueReg);
         ((TR::S390RRFInstruction *) curr)->setRegisterOperand(1,compReg);
         break;
      default:
         // unsupport instruction type, bail
        return false;
      }
      return true;
   }

void insertLoad(TR::Compilation * comp, TR::CodeGenerator * cg, TR::Instruction * i, TR::Register * r)
   {
   switch(r->getKind())
     {
     case TR_FPR:
       new (comp->trHeapMemory()) TR::S390RRInstruction(TR::InstOpCode::LDR, i->getNode(), r, r, i, cg);
       break;
     default:
       new (comp->trHeapMemory()) TR::S390RRInstruction(TR::InstOpCode::LR, i->getNode(), r, r, i, cg);
       break;
     }
   }

bool hasDefineToRegister(TR::Instruction * curr, TR::Register * reg)
   {
   TR::Instruction * prev = curr->getPrev();
   prev = realInstruction(prev, false);
   for(int32_t i = 0; i < 3 && prev; ++i)
      {
      if (prev->defsRegister(reg))
         {
         return true;
         }

      prev = prev->getPrev();
      prev = realInstruction(prev, false);
      }
   return false;
   }

/**
 * z10 specific hw performance bug
 * On z10, applies to GPRs only.
 * There are cases where load of a GPR and its complemented value are required
 * in same grouping, causing pipeline flush + late load = perf hit.
 */
bool
TR_S390Peephole::trueCompEliminationForCompare()
   {
   // z10 specific
   if (!comp()->target().cpu.getSupportsArch(TR::CPU::z10) || comp()->target().cpu.getSupportsArch(TR::CPU::z196))
      {
      return false;
      }
   TR::Instruction * curr = _cursor;
   TR::Instruction * prev = _cursor->getPrev();
   TR::Instruction * next = _cursor->getNext();
   TR::Register * compReg;
   TR::Register * trueReg;

   prev = realInstruction(prev, false);
   next = realInstruction(next, true);

   switch(curr->getKind())
      {
      case TR::Instruction::IsRR:
         compReg = ((TR::S390RRInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RRInstruction *) curr)->getRegisterOperand(1);
         break;
      case TR::Instruction::IsRIE:
         compReg = ((TR::S390RIEInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RIEInstruction *) curr)->getRegisterOperand(1);
         break;
      case TR::Instruction::IsRRS:
         compReg = ((TR::S390RRSInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RRSInstruction *) curr)->getRegisterOperand(1);
         break;
      case TR::Instruction::IsRRD: // RRD is encoded use RRF
      case TR::Instruction::IsRRF:
         compReg = ((TR::S390RRFInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RRFInstruction *) curr)->getRegisterOperand(1);
         break;
      default:
         // unsupport instruction type, bail
         return false;
      }

   // only applies to GPR's
   if (compReg->getKind() != TR_GPR || trueReg->getKind() != TR_GPR)
      {
      return false;
      }

   if (!hasDefineToRegister(curr, compReg))
      {
      return false;
      }

   TR::Instruction * branchInst = NULL;
   // current instruction sets condition code or compare flag, check to see
   // if it has multiple branches using this condition code..if so, abort.
   TR::Instruction * nextInst = next;
   while(nextInst && !nextInst->isLabel() && !nextInst->getOpCode().setsCC() &&
         !nextInst->isCall() && !nextInst->getOpCode().setsCompareFlag())
      {
      if(nextInst->isBranchOp())
         {
         if(branchInst == NULL)
            {
            branchInst = nextInst;
            }
         else
            {
            // there are multiple branches using the same branch condition
            // just give up.
            // we can probably just insert load instructions here still, but
            // we have to sort out the wild branches first
            return false;
            }
         }
      nextInst = nextInst->getNext();
      }
   if (branchInst && prev && prev->usesRegister(compReg) && !prev->usesRegister(trueReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare case 1 at %p.\n",curr))
         {
         swapOperands(trueReg, compReg, curr);
         if(next && next->usesRegister(trueReg)) insertLoad (comp(), _cg, next->getPrev(), compReg);
         TR::InstOpCode::S390BranchCondition branchCond = ((TR::S390BranchInstruction *) branchInst)->getBranchCondition();
         ((TR::S390BranchInstruction *) branchInst)->setBranchCondition(getReverseBranchCondition(branchCond));
         return true;
         }
      else
         return false;
      }
   if (next && next->usesRegister(compReg) && !next->usesRegister(trueReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare case 2 at %p.\n",curr))
         {
         if (branchInst && prev && !prev->usesRegister(trueReg))
            {
            swapOperands(trueReg, compReg, curr);
            TR::InstOpCode::S390BranchCondition branchCond = ((TR::S390BranchInstruction *) branchInst)->getBranchCondition();
            ((TR::S390BranchInstruction *) branchInst)->setBranchCondition(getReverseBranchCondition(branchCond));
            }
         else
            {
            insertLoad (comp(), _cg, next->getPrev(), compReg);
            }
         return true;
         }
      else
         return false;
      }

   bool loadInserted = false;
   if (prev && prev->usesRegister(compReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare case 3 at %p.\n",curr))
         {
         insertLoad (comp(), _cg, prev, trueReg);
         loadInserted = true;
         }
      }
   if (next && next->usesRegister(compReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare case 4 at %p.\n",curr))
         {
         insertLoad (comp(), _cg, next->getPrev(), trueReg);
         loadInserted = true;
         }
      }
   return loadInserted ;
   }

/**
 * z10 specific hw performance bug
 * On z10, applies to GPRs only.
 * There are cases where load of a GPR and its complemented value are required
 * in same grouping, causing pipeline flush + late load = perf hit.
 */
bool
TR_S390Peephole::trueCompEliminationForCompareAndBranch()
   {
   // z10 specific
   if (!comp()->target().cpu.getSupportsArch(TR::CPU::z10) || comp()->target().cpu.getSupportsArch(TR::CPU::z196))
      {
      return false;
      }

   TR::Instruction * curr = _cursor;
   TR::Instruction * prev = _cursor->getPrev();
   prev = realInstruction(prev, false);

   TR::Instruction * next = _cursor->getNext();
   next = realInstruction(next, true);

   TR::Register * compReg;
   TR::Register * trueReg;
   TR::Instruction * btar = NULL;
   TR::LabelSymbol * ls = NULL;

   bool isWCodeCmpEqSwap = false;

   switch (curr->getKind())
      {
      case TR::Instruction::IsRIE:
         compReg = ((TR::S390RIEInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RIEInstruction *) curr)->getRegisterOperand(1);
         btar = ((TR::S390RIEInstruction *) curr)->getBranchDestinationLabel()->getInstruction();
         break;
      case TR::Instruction::IsRRS:
         compReg = ((TR::S390RRSInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RRSInstruction *) curr)->getRegisterOperand(1);
         break;
      case TR::Instruction::IsRRD: // RRD is encoded use RRF
      case TR::Instruction::IsRRF:
      case TR::Instruction::IsRRF2:
         compReg = ((TR::S390RRFInstruction *) curr)->getRegisterOperand(2);
         trueReg = ((TR::S390RRFInstruction *) curr)->getRegisterOperand(1);
         break;
      default:
         // unsupport instruction type, bail
         return false;
      }

   // only applies to GPR's
   if (compReg->getKind() != TR_GPR || trueReg->getKind() != TR_GPR)
      {
      return false;
      }

   if (!hasDefineToRegister(curr, compReg))
      {
      return false;
      }

   btar = realInstruction(btar, true);
   bool backwardBranch = false;
   if (btar)
      {
      backwardBranch = curr->getIndex() - btar->getIndex() > 0;
      }

   if (backwardBranch)
      {
      if ((prev && btar) &&
          (prev->usesRegister(compReg) || btar->usesRegister(compReg)) &&
          (!prev->usesRegister(trueReg) && !btar->usesRegister(trueReg)) )
         {
         if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
         if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 1 at %p.\n",curr))
            {
            swapOperands(trueReg, compReg, curr);
            if (next && next->usesRegister(trueReg)) insertLoad (comp(), _cg, next->getPrev(), compReg);
            return true;
            }
         else
            return false;
         }
      }
   else
      {
      if ((prev && next) &&
          (prev->usesRegister(compReg) || next->usesRegister(compReg)) &&
          (!prev->usesRegister(trueReg) && !next->usesRegister(trueReg)) )
         {
         if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
         if(!isWCodeCmpEqSwap && performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 2 at %p.\n",curr))
            {
            swapOperands(trueReg, compReg, curr);
            if (btar && btar->usesRegister(trueReg)) insertLoad (comp(), _cg, btar->getPrev(), compReg);
            return true;
            }
         else
            return false;
         }
      }
   if (prev && prev->usesRegister(compReg) && !prev->usesRegister(trueReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(!isWCodeCmpEqSwap && performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 3 at %p.\n",curr))
         {
         swapOperands(trueReg, compReg, curr);
         if (btar && btar->usesRegister(trueReg)) insertLoad (comp(), _cg, btar->getPrev(), compReg);
         if (next && next->usesRegister(trueReg)) insertLoad (comp(), _cg, next->getPrev(), compReg);
         return true;
         }
      else
         return false;
      }

   bool loadInserted = false;
   if (prev && prev->usesRegister(compReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 4 at %p.\n",curr) )
         {
         insertLoad (comp(), _cg, prev, trueReg);
         loadInserted = true;
         }
      }
   if (btar && btar->usesRegister(compReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 5 at %p.\n",curr) )
         {
         insertLoad (comp(), _cg, btar->getPrev(), trueReg);
         loadInserted = true;
         }
      }
   if (next && next->usesRegister(compReg))
      {
      if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
      if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for compare and branch case 6 at %p.\n",curr) )
         {
         insertLoad (comp(), _cg, next->getPrev(), trueReg);
         loadInserted = true;
         }
      }
   return loadInserted;
   }

bool
TR_S390Peephole::trueCompEliminationForLoadComp()
   {
   if (!comp()->target().cpu.getSupportsArch(TR::CPU::z10) || comp()->target().cpu.getSupportsArch(TR::CPU::z196))
      {
      return false;
      }
   TR::Instruction * curr = _cursor;
   TR::Instruction * next = _cursor->getNext();
   next = realInstruction(next, true);
   TR::Instruction * prev = _cursor->getPrev();
   prev = realInstruction(prev, false);

   TR::Register * srcReg = ((TR::S390RRInstruction *) curr)->getRegisterOperand(2);
   TR::RealRegister *tempReg = NULL;
   if((toRealRegister(srcReg))->getRegisterNumber() == TR::RealRegister::GPR1)
      {
      tempReg = _cg->machine()->getRealRegister(TR::RealRegister::GPR2);
      }
   else
      {
      tempReg = _cg->machine()->getRealRegister(TR::RealRegister::GPR1);
      }

   if (prev && prev->defsRegister(srcReg))
      {
      // src register is defined in the previous instruction, check to see if it's
      // used in the next instruction, if so, inject a load after the current insruction
      if (next && next->usesRegister(srcReg))
         {
         insertLoad (comp(), _cg, curr, tempReg);
         return true;
         }
      }

   TR::Instruction * prev2 = NULL;
   if (prev)
      {
      prev2 = realInstruction(prev->getPrev(), false);
      }
   if (prev2 && prev2->defsRegister(srcReg))
      {
      // src register is defined 2 instructions ago, insert a load before the current instruction
      // if the true value is used before or after
      if ((next && next->usesRegister(srcReg)) || (prev && prev->usesRegister(srcReg)))
         {
         if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
         if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for load complement at %p.\n",curr))
            {
            insertLoad (comp(), _cg, curr->getPrev(), tempReg);
            return true;
            }
         else
            return false;
         }
      }

   TR::Instruction * prev3 = NULL;
   if (prev2)
      {
      prev3 = realInstruction(prev2->getPrev(), false);
      }
   if (prev3 && prev3->defsRegister(srcReg))
      {
      // src registers is defined 3 instructions ago, insert a load before the current instruction
      // if the true value is use before
      if(prev && prev->usesRegister(srcReg))
         {
         if (comp()->getOption(TR_TraceCG)) { printInfo("\n"); }
         if(performTransformation(comp(), "O^O S390 PEEPHOLE: true complement elimination for load complement at %p.\n",curr))
            {
            insertLoad (comp(), _cg, curr->getPrev(), tempReg);
            return true;
            }
         else
            return false;
         }
      }
   return false;
   }

void
TR_S390Peephole::reloadLiteralPoolRegisterForCatchBlock()
   {
   // When dynamic lit pool reg is disabled, we lock R6 as dedicated lit pool reg.
   // This causes a failure when we come back to a catch block because the register context will not be preserved.
   // Hence, we can not assume that R6 will still contain the lit pool register and hence need to reload it.

   bool isZ10 = comp()->target().cpu.getSupportsArch(TR::CPU::z10);

   // we only need to reload literal pool for Java on older z architecture on zos when on demand literal pool is off
   if ( comp()->target().isZOS() && !isZ10 && !_cg->isLiteralPoolOnDemandOn())
      {
      // check to make sure that we actually need to use the literal pool register
      TR::Snippet * firstSnippet = _cg->getFirstSnippet();
      if ((_cg->getLinkage())->setupLiteralPoolRegister(firstSnippet) > 0)
         {
         // the imm. operand will be patched when the actual address of the literal pool is known at binary encoding phase
         TR::S390RILInstruction * inst = (TR::S390RILInstruction *) generateRILInstruction(_cg, TR::InstOpCode::LARL, _cursor->getNode(), _cg->getLitPoolRealRegister(), reinterpret_cast<void*>(0xBABE), _cursor);
         inst->setIsLiteralPoolAddress();
         }
      }
   }

void
TR_S390Peephole::perform()
   {
   TR::Delimiter d(comp(), comp()->getOption(TR_TraceCG), "Peephole");

   if (comp()->getOption(TR_TraceCG))
      printInfo("\nPeephole Optimization Instructions:\n");

   bool moveInstr;

      {
      while (_cursor != NULL)
         {
         if (_cursor->getNode() != NULL && _cursor->getNode()->getOpCodeValue() == TR::BBStart)
            {
            comp()->setCurrentBlock(_cursor->getNode()->getBlock());
            // reload literal pool for catch blocks that need it
            TR::Block * blk = _cursor->getNode()->getBlock();
            if (blk->isCatchBlock() && (blk->getFirstInstruction() == _cursor))
               reloadLiteralPoolRegisterForCatchBlock();
            }

         // this code is used to handle all compare instruction which sets the compare flag
         // we can eventually extend this to include other instruction which sets the
         // condition code and and uses a complemented register
         if (_cursor->getOpCode().setsCompareFlag() &&
            _cursor->getOpCodeValue() != TR::InstOpCode::CHLR &&
            _cursor->getOpCodeValue() != TR::InstOpCode::CLHLR)
            {
            trueCompEliminationForCompare();
            if (comp()->getOption(TR_TraceCG))
               printInst();
            }

         moveInstr = true;
         switch (_cursor->getOpCodeValue())
            {
            case TR::InstOpCode::CRJ:
               {
               trueCompEliminationForCompareAndBranch();

               if (comp()->getOption(TR_TraceCG))
                  printInst();

               break;
               }

            case TR::InstOpCode::CGRJ:
               {
               trueCompEliminationForCompareAndBranch();

               if (comp()->getOption(TR_TraceCG))
                  printInst();

               break;
               }

            case TR::InstOpCode::CRB:
            case TR::InstOpCode::CRT:
            case TR::InstOpCode::CGFR:
            case TR::InstOpCode::CGRT:
            case TR::InstOpCode::CLR:
               {
               trueCompEliminationForCompareAndBranch();

               if (comp()->getOption(TR_TraceCG))
                  printInst();

               break;
               }
            case TR::InstOpCode::CLRB:
            case TR::InstOpCode::CLRJ:
            case TR::InstOpCode::CLRT:
            case TR::InstOpCode::CLGRB:
            case TR::InstOpCode::CLGFR:
            case TR::InstOpCode::CLGRT:
               {
               trueCompEliminationForCompareAndBranch();

               if (comp()->getOption(TR_TraceCG))
                  printInst();

               break;
               }

            case TR::InstOpCode::LCGFR:
            case TR::InstOpCode::LCGR:
            case TR::InstOpCode::LCR:
               {
               trueCompEliminationForLoadComp();

               if (comp()->getOption(TR_TraceCG))
                  printInst();

               break;
               }

            default:
               {
               if (comp()->getOption(TR_TraceCG))
                  printInst();
               break;
               }
            }

         if (moveInstr == true)
            _cursor = _cursor->getNext();
         }
      }

   if (comp()->getOption(TR_TraceCG))
      printInfo("\n\n");
   }

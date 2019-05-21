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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/Linkage_inlines.hpp"
#include "codegen/Machine.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/RecognizedMethods.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterPair.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "codegen/S390Evaluator.hpp"
#include "compile/Compilation.hpp"
#include "compile/Method.hpp"
#include "compile/ResolvedMethod.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "compile/VirtualGuard.hpp"
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "env/CompilerEnv.hpp"
#ifdef J9_PROJECT_SPECIFIC
#include "env/CHTable.hpp"
#endif
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/Block.hpp"
#include "il/DataTypes.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/Symbol.hpp"
#include "il/SymbolReference.hpp"
#include "il/TreeTop.hpp"
#include "il/TreeTop_inlines.hpp"
#include "il/symbol/AutomaticSymbol.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "il/symbol/MethodSymbol.hpp"
#include "il/symbol/ResolvedMethodSymbol.hpp"
#include "infra/Assert.hpp"
#include "infra/Bit.hpp"
#include "infra/BitVector.hpp"
#include "infra/List.hpp"
#include "ras/Debug.hpp"
#include "ras/Delimiter.hpp"
#include "z/codegen/BinaryAnalyser.hpp"
#include "z/codegen/BinaryCommutativeAnalyser.hpp"
#include "z/codegen/CompareAnalyser.hpp"
#include "z/codegen/S390GenerateInstructions.hpp"
#include "z/codegen/S390HelperCallSnippet.hpp"
#include "z/codegen/S390Instruction.hpp"
#include "z/codegen/S390OutOfLineCodeSection.hpp"
#ifdef J9_PROJECT_SPECIFIC
#include "z/codegen/S390Register.hpp"
#endif
#include "z/codegen/TranslateEvaluator.hpp"

extern TR::Instruction *
generateS390PackedCompareAndBranchOps(TR::Node * node,
                                      TR::CodeGenerator * cg,
                                      TR::InstOpCode::S390BranchCondition fBranchOpCond,
                                      TR::InstOpCode::S390BranchCondition rBranchOpCond,
                                      TR::InstOpCode::S390BranchCondition &retBranchOpCond,
                                      TR::LabelSymbol *branchTarget = NULL);

extern TR::Register *
iDivRemGenericEvaluator(TR::Node * node, TR::CodeGenerator * cg, bool isDivision, TR::MemoryReference * divchkDivisorMR);

TR::InstOpCode::S390BranchCondition generateS390CompareOps(TR::Node * node, TR::CodeGenerator * cg, TR::InstOpCode::S390BranchCondition fBranchOpCond, TR::InstOpCode::S390BranchCondition rBranchOpCond);

TR::Instruction * generateS390CompareOps(TR::Node * node, TR::CodeGenerator * cg, TR::InstOpCode::S390BranchCondition fBranchOpCond, TR::InstOpCode::S390BranchCondition rBranchOpCond, TR::LabelSymbol *branchTarget);

void killRegisterIfNotLocked(TR::CodeGenerator * cg, TR::RealRegister::RegNum reg, TR::Instruction * instr , TR::RegisterDependencyConditions * deps = NULL)
   {
         TR::Register *dummy = NULL;
         if (cg->machine()->getRealRegister(reg)->getState() != TR::RealRegister::Locked)
            {
            if (deps == NULL)
               deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 1, cg);
            dummy = cg->allocateRegister();
            deps->addPostCondition(dummy, TR::RealRegister::GPR4);
            dummy->setPlaceholderReg();
            instr->setDependencyConditions(deps);
            cg->stopUsingRegister(dummy);
            }
   }

/**
 * Helper to generate virtual guard if node is so flagged.  In 32 bit mode the NOP instruction,
 * a BRC, has only a +ve displacement of 2^12 (4k), and so in some cases it is not safe to use it
 * on a G5 platform (no problems with BRCL).  At this point, since instructions have not yet
 * been generated, there is no good way to guage how far away the label will be, so we cannot
 * even determine if it is safe.  Will never be enabled for G5 hardware
 * Note that some instructions *must* have a virtual guard generated for them.
 */
static bool
virtualGuardHelper(TR::Node * node, TR::CodeGenerator * cg)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::Compilation *comp = cg->comp();
   if ((!node->isNopableInlineGuard() && !node->isHCRGuard() && !node->isOSRGuard()) ||
      !cg->getSupportsVirtualGuardNOPing())
      {
      return false;
      }

   TR_VirtualGuard * virtualGuard = comp->findVirtualGuardInfo(node);
   if (!node->isHCRGuard() && !node->isOSRGuard() && !(comp->performVirtualGuardNOPing() &&
         comp->isVirtualGuardNOPingRequired(virtualGuard)) &&
         virtualGuard->canBeRemoved())
      {
      return false;
      }

   if (node->getOpCodeValue() != TR::ificmpne && node->getOpCodeValue() != TR::iflcmpne && node->getOpCodeValue() != TR::ifacmpne)
      {
      //TR_ASSERT( 0, "virtualGuardHelper: not expecting reversed comparison");
      return false;
      }

   TR_VirtualGuardSite * site = NULL;
   if (comp->compileRelocatableCode())
      {
      site = (TR_VirtualGuardSite *)comp->addAOTNOPSite();
      TR_AOTGuardSite *aotSite = (TR_AOTGuardSite *)site;
      aotSite->setType(virtualGuard->getKind());
      aotSite->setNode(node);

      switch (virtualGuard->getKind())
         {
         case TR_DirectMethodGuard:
         case TR_NonoverriddenGuard:
         case TR_InterfaceGuard:
         case TR_MethodEnterExitGuard:
         case TR_HCRGuard:
         case TR_AbstractGuard:
            aotSite->setGuard(virtualGuard);
            break;

         case TR_ProfiledGuard:
            break;

         default:
            TR_ASSERT_FATAL(0, "got AOT guard in node but virtual guard not one of known guards supported for AOT. Guard: %d", virtualGuard->getKind());
            break;
         }
      }
   else if (!node->isSideEffectGuard())
      {
      TR_VirtualGuard * virtualGuard = comp->findVirtualGuardInfo(node);
      site = virtualGuard->addNOPSite();
      }
   else
      {
      site = comp->addSideEffectNOPSite();
      }

   TR::RegisterDependencyConditions * deps;
   if (node->getNumChildren() == 3)
      {
      TR::Node * third = node->getChild(2);
      cg->evaluate(third);
      deps = generateRegisterDependencyConditions(cg, third, 0);
      }
   else
      {
      deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions((uint16_t) 0, (uint16_t) 0, cg);
      }

   if(virtualGuard->shouldGenerateChildrenCode())
      cg->evaluateChildrenWithMultipleRefCount(node);

   generateVirtualGuardNOPInstruction(cg, node, site, deps, node->getBranchDestination()->getNode()->getLabel());

   cg->recursivelyDecReferenceCount(node->getFirstChild());
   cg->recursivelyDecReferenceCount(node->getSecondChild());

   traceMsg(comp, "virtualGuardHelper for %s %s\n",
         comp->getDebug()?comp->getDebug()->getVirtualGuardKindName(virtualGuard->getKind()):"???Guard" , virtualGuard->mergedWithHCRGuard()?"merged with HCRGuard":"");
   return true;
#else
   return false;
#endif
   }
//////////////////////////////////////////////////////////////////////////////////////////////////
//  Long Compare Helper code
//
#define CMP4BOOLEAN       true
#define CMP4CONTROLFLOW   false

static TR::Register *
generateS390lcmpEvaluator64(TR::Node * node, TR::CodeGenerator * cg, TR::InstOpCode::Mnemonic brOp, TR::InstOpCode::S390BranchCondition brCond, bool isBoolean)
   {
   TR::RegisterDependencyConditions * deps = NULL;
   TR::LabelSymbol * cFlowRegionStart = generateLabelSymbol(cg);
   TR::LabelSymbol * isTrue = generateLabelSymbol(cg);
   bool isUnsigned = node->getOpCode().isUnsignedCompare();

   TR::Register * targetRegister = cg->allocateRegister();
   TR::Node * firstChild = node->getFirstChild();

   TR_ASSERT(isBoolean, "Coparison node %p is not boolean\n",node);

   // Assume the condition is true.
   genLoadLongConstant(cg, node, 1, targetRegister);

   TR::Node * secondChild = node->getSecondChild();
   if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
      {
      deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 1, cg);
      int64_t long_value = secondChild->getLongInt();
      TR::Register * cmpRegister = cg->evaluate(firstChild);
      generateS390ImmOp(cg, isUnsigned ? TR::InstOpCode::CLG : TR::InstOpCode::CG, node, cmpRegister, cmpRegister, long_value);
      generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
      generateS390BranchInstruction(cg, brOp, brCond, node, isTrue);
      }
   else
      {
      // We should use the Binary analyzer here
      TR::Node * firstChild = node->getFirstChild();
      TR::Register * cmpRegister = cg->evaluate(firstChild);
      TR::Register * srcReg = cg->evaluate(secondChild);
      deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 3, cg);
      deps->addPostConditionIfNotAlreadyInserted(cmpRegister,TR::RealRegister::AssignAny);
      deps->addPostConditionIfNotAlreadyInserted(srcReg,TR::RealRegister::AssignAny);
      generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
      generateS390CompareAndBranchInstruction(cg, isUnsigned ? TR::InstOpCode::CLGR : TR::InstOpCode::CGR, node, cmpRegister, srcReg, brCond, isTrue, false, false);
      }

   cFlowRegionStart->setStartInternalControlFlow();
   deps->addPostConditionIfNotAlreadyInserted(targetRegister,TR::RealRegister::AssignAny);

   // FALSE
   genLoadLongConstant(cg, node, 0, targetRegister);
   // TRUE
   generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, isTrue, deps);
   isTrue->setEndInternalControlFlow();

   node->setRegister(targetRegister);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return targetRegister;
   }


/**
 * Generate code to perform a comparison that returns 1 , -1, or 0
 * Handles TR::fcmpl, TR::fcmpg, TR::dcmpl, and TR::dcmpg
 */
TR::Register *
OMR::Z::TreeEvaluator::fcmplEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::InstOpCode::Mnemonic branchOp;
   TR::InstOpCode::S390BranchCondition brCond ;

   // Create ICF start label
   TR::LabelSymbol * cFlowRegionStart = generateLabelSymbol(cg);
   // Create a label
   TR::LabelSymbol * cFlowRegionEnd = generateLabelSymbol(cg);
   // Create a register
   TR::Register * targetRegister = cg->allocateRegister();

   // Generate compare code, find out if ops were reversed
   brCond = generateS390CompareOps(node, cg, TR::InstOpCode::COND_BH, TR::InstOpCode::COND_BL);
   branchOp = TR::InstOpCode::BRC;

   // Assume A == B, set targetRegister value to 0
   // TODO: Can we allow setting the condition code here by moving the load before the compare?
   generateLoad32BitConstant(cg, node, 0, targetRegister, false);

   // done if A==B
   TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 1, cg);
   deps->addPostCondition(targetRegister,TR::RealRegister::AssignAny);

   generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
   cFlowRegionStart->setStartInternalControlFlow();
   generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, node, cFlowRegionEnd);

   // Found A != B, assume A > B, set targetRegister value to 1
   generateLoad32BitConstant(cg, node, 1, targetRegister, false);

   //done if A>B
   generateS390BranchInstruction(cg, branchOp, brCond, node, cFlowRegionEnd);
   //found either A<B or either of A and B is NaN
   //For TR::fcmpg instruction, done if either of A and B is NaN

   if (node->getOpCodeValue() == TR::fcmpg || node->getOpCodeValue() == TR::dcmpg)
      {
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_MASK1, node, cFlowRegionEnd);
      }

   //Got here? means either A<B or (TR::fcmpl instruction and (A==NaN || B==NaN))
   //set targetRegister value to -1
   generateLoad32BitConstant(cg, node, -1, targetRegister, true);

   // DONE
   generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionEnd, deps);
   cFlowRegionEnd->setEndInternalControlFlow();

   node->setRegister(targetRegister);
   return targetRegister;
   }

inline TR::InstOpCode::S390BranchCondition
generateS390Compare(TR::Node * node, TR::CodeGenerator * cg, TR::InstOpCode::Mnemonic branchOp, TR::InstOpCode::S390BranchCondition fBranchOpCond, TR::InstOpCode::S390BranchCondition rBranchOpCond)
   {
   // Generate compare code, find out if ops were reversed
   TR::InstOpCode::S390BranchCondition branchOpCond = generateS390CompareOps(node, cg, fBranchOpCond, rBranchOpCond);
   return branchOpCond;
   }

/**
 *  \brief
 *     Compares two values are returns the max/min of the two.
 *     ONLY SUPPORTS imax, imin, lmax, lmin
 *
 *  \param node
 *     The node representing a call to max or min.
 *
 *  \param cg
 *     The code generator used to generate the instructions.
 *
 *  \param isMax
 *     Determines the type of function, either a max or min call.
 *
 *  \return
 *     The register containing the max/min value.
 */
static TR::Register* maxMinHelper(TR::Node* node, TR::CodeGenerator* cg, bool isMax)
   {
   TR::Node* lhsNode = node->getChild(0);
   TR::Node* rhsNode = node->getChild(1);

   TR::Register* lhsReg = NULL;
   TR::Register* rhsReg = cg->evaluate(rhsNode);

   // Mask is 4 to pick rhs when lhs is less for max, 2 to pick rhs when lhs is greater for min
   const uint8_t mask = isMax ? 0x4 : 0x2;

   if (node->getOpCodeValue() == TR::imax || node->getOpCodeValue() == TR::imin)
      {
      if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z15))
         {
         lhsReg = cg->allocateRegister();

         // Load into a tmp instead of clobberEvaluating into lhsReg to avoid an extra register shuffle
         TR::Register* tmpRegister = cg->evaluate(lhsNode);

         generateRRInstruction(cg, TR::InstOpCode::CR, node, tmpRegister, rhsReg);
         generateRRFInstruction(cg, TR::InstOpCode::SELR, node, lhsReg, rhsReg, tmpRegister, mask);
         }
      else if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z196))
         {
         lhsReg = cg->gprClobberEvaluate(lhsNode);

         generateRRInstruction(cg, TR::InstOpCode::CR, node, lhsReg, rhsReg);
         generateRRFInstruction(cg, TR::InstOpCode::LOCR, node, lhsReg, rhsReg, mask, true);
         }
      else
         {
         lhsReg = cg->gprClobberEvaluate(lhsNode);

         TR::LabelSymbol* cFlowRegionStart = generateLabelSymbol(cg);
         TR::LabelSymbol* cFlowRegionEnd = generateLabelSymbol(cg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
         cFlowRegionStart->setStartInternalControlFlow();

         auto bc = isMax ?
            TR::InstOpCode::COND_BHR :
            TR::InstOpCode::COND_BLR;

         generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CR, node, lhsReg, rhsReg, bc, cFlowRegionEnd, false);

         generateRRInstruction(cg, TR::InstOpCode::LR, node, lhsReg, rhsReg);
         
         TR::RegisterDependencyConditions* deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 2, cg);

         deps->addPostConditionIfNotAlreadyInserted(lhsReg, TR::RealRegister::AssignAny);
         deps->addPostConditionIfNotAlreadyInserted(rhsReg, TR::RealRegister::AssignAny);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionEnd, deps);
         cFlowRegionEnd->setEndInternalControlFlow();
         }
      }
   else if (node->getOpCodeValue() == TR::lmax || node->getOpCodeValue() == TR::lmin)
      {
      if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z15))
         {
         lhsReg = cg->allocateRegister();

         // Load into a tmp instead of clobberEvaluating into lhsReg to avoid an extra register shuffle
         TR::Register* tmpRegister = cg->evaluate(lhsNode);

         generateRREInstruction(cg, TR::InstOpCode::CGR, node, tmpRegister, rhsReg);
         generateRRFInstruction(cg, TR::InstOpCode::SELGR, node, lhsReg, rhsReg, tmpRegister, mask);
         }
      else if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z196))
         {
         lhsReg = cg->gprClobberEvaluate(lhsNode);

         generateRREInstruction(cg, TR::InstOpCode::CGR, node, lhsReg, rhsReg);
         generateRRFInstruction(cg, TR::InstOpCode::LOCGR, node, lhsReg, rhsReg, mask, true);
         }
      else
         {
         lhsReg = cg->gprClobberEvaluate(lhsNode);

         TR::LabelSymbol* cFlowRegionStart = generateLabelSymbol(cg);
         TR::LabelSymbol* cFlowRegionEnd = generateLabelSymbol(cg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
         cFlowRegionStart->setStartInternalControlFlow();

         auto bc = isMax ?
            TR::InstOpCode::COND_BHR :
            TR::InstOpCode::COND_BLR;

         generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CGR, node, lhsReg, rhsReg, bc, cFlowRegionEnd, false);

         generateRRInstruction(cg, TR::InstOpCode::LGR, node, lhsReg, rhsReg);
         
         TR::RegisterDependencyConditions* deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 2, cg);

         deps->addPostConditionIfNotAlreadyInserted(lhsReg, TR::RealRegister::AssignAny);
         deps->addPostConditionIfNotAlreadyInserted(rhsReg, TR::RealRegister::AssignAny);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionEnd, deps);
         cFlowRegionEnd->setEndInternalControlFlow();
         }
      }
   else
      {
      TR_ASSERT_FATAL(node->getOpCodeValue(), "Opcode %s cannot be evaluated by maxMinHelper\n", node->getOpCode().getName());
      }

   node->setRegister(lhsReg);

   cg->decReferenceCount(lhsNode);
   cg->decReferenceCount(rhsNode);

   return lhsReg;
   }

TR::Register *
OMR::Z::TreeEvaluator::maxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return maxMinHelper(node, cg, true);
   }

TR::Register *
OMR::Z::TreeEvaluator::minEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return maxMinHelper(node, cg, false);
   }


/**
 * 64bit version lcmpEvaluator Helper: long compare (1 if child1 > child2, 0 if child1 == child2,
 *    -1 if child1 < child2 or unordered)
 */
TR::Register *
lcmpHelper64(TR::Node * node, TR::CodeGenerator * cg)
   {
   // TODO:There is probably a better way to implement this.  Should re-visit if we get the chance.
   // As things stand now, we will to LCR R1,R1 when R1=0 which is useless...  But still probably
   // cheaper than an extra branch.
   TR::LabelSymbol * cFlowRegionStart = generateLabelSymbol(cg);
   TR::LabelSymbol * labelGT = generateLabelSymbol(cg);
   TR::LabelSymbol * labelLT = generateLabelSymbol(cg);

   TR::Register * targetRegister = cg->allocateRegister();

   TR::Node * firstChild = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();

   TR::Register * src1Reg = NULL;
   if (firstChild->getReferenceCount() == 1)
      src1Reg = (TR::Register *) cg->evaluate(firstChild);
   else
      src1Reg = (TR::Register *) cg->gprClobberEvaluate(firstChild);

   TR::Register * src2Reg = NULL;
   TR::Instruction * cursor = NULL;

   if (secondChild->getOpCode().isLoadConst() && secondChild->getLongInt()==0)
      {
      generateRRInstruction(cg, TR::InstOpCode::LCGR, node, targetRegister, src1Reg);
      generateRSInstruction(cg, TR::InstOpCode::SRAG, node, src1Reg, src1Reg, 63);
      generateRSInstruction(cg, TR::InstOpCode::SRLG, node, targetRegister, targetRegister, 63);
      generateRRInstruction(cg, TR::InstOpCode::OGR, node, targetRegister, src1Reg);
      }
   else
      {
      // Assume LT
      generateRIInstruction(cg, TR::InstOpCode::LGHI, node, targetRegister, -1);

      if (secondChild->getOpCode().isLoadConst() && secondChild->getRegister() == NULL)
         {
         // Set the CC using a comp.
         generateS390ImmOp(cg, TR::InstOpCode::CG, node, src1Reg, src1Reg, secondChild->getLongInt());
         }
      else
         {
         // Get src2Reg if it isn't a const
         src2Reg = cg->evaluate(secondChild);

         // Set the CC using a comp.
         generateRRInstruction(cg, TR::InstOpCode::CGR, node, src1Reg, src2Reg);
         }
      // If LT we are done
      generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
      cFlowRegionStart->setStartInternalControlFlow();
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BL, node, labelLT);

      // If GT, we invert the result register
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BH, node, labelGT);
      //  High order words were equal

      // The two longs are equal
      generateRIInstruction(cg, TR::InstOpCode::LGHI, node, targetRegister, 0);

      // We can go through this path if GT, or if EQ.
      // We use LCR to avoid having to branch over this piece of code.
      generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, labelGT);
      generateRRInstruction(cg, TR::InstOpCode::LCGR, node, targetRegister, targetRegister);

      // We branch here when LT (no change to assumed -1 result)
      TR::RegisterDependencyConditions * dependencies = NULL;
      dependencies = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 1, cg);
      dependencies->addPostCondition(targetRegister, TR::RealRegister::AssignAny);
      generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, labelLT, dependencies);
      labelLT->setEndInternalControlFlow();
      }

   node->setRegister(targetRegister);

   // did we clobberEval earlier?
   if (!cg->canClobberNodesRegister(firstChild))
      cg->stopUsingRegister(src1Reg);
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);

   return targetRegister;
   }


TR::Register *
OMR::Z::TreeEvaluator::branchEvaluator(TR::Node * node, TR::CodeGenerator *cg)
   {
   return NULL;
   }
TR::Register *
OMR::Z::TreeEvaluator::ibranchEvaluator(TR::Node * node, TR::CodeGenerator *cg)
   {
   return NULL;
   }
TR::Register *
OMR::Z::TreeEvaluator::mbranchEvaluator(TR::Node * node, TR::CodeGenerator *cg)
   {
   return NULL;
   }

/**
 * goto label address
 */
TR::Register *
OMR::Z::TreeEvaluator::gotoEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Node * temp = node->getBranchDestination()->getNode();

   if (node->getNumChildren() > 0)
      {
      // GRA
      TR::Node * child = node->getFirstChild();
      cg->evaluate(child);
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BRC, node, temp->getLabel(), generateRegisterDependencyConditions(cg, child, 0));
      cg->decReferenceCount(child);
      }
   else
      {
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BRC, node, temp->getLabel());
      }

   return NULL;
   }

/**
 * Indirect goto to the address
 */
TR::Register *
OMR::Z::TreeEvaluator::igotoEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR_ASSERT( node->getNumChildren() >= 1, "at least one child expected for igoto");

   TR::Node * child = node->getFirstChild();
   TR::Register * dest = cg->evaluate(child);
   TR::RegisterDependencyConditions * deps = NULL;

   if (node->getNumChildren() > 1)
      {
      TR_ASSERT( node->getNumChildren() == 2 && node->getChild(1)->getOpCodeValue() == TR::GlRegDeps, "igoto has maximum of two children and second one must be global register dependency");
      TR::Node *glregdep = node->getChild(1);
      cg->evaluate(glregdep);
      deps = generateRegisterDependencyConditions(cg, glregdep, 0);
      cg->decReferenceCount(glregdep);
      }
   TR::Instruction* cursor;
   if (deps)
      cursor = generateS390RegInstruction(cg, TR::InstOpCode::BCR, node, dest, deps);
   else
      cursor = generateS390RegInstruction(cg, TR::InstOpCode::BCR, node, dest);

   ((TR::S390RegInstruction *)cursor)->setBranchCondition(TR::InstOpCode::COND_BCR);

   cg->decReferenceCount(child);

   return NULL;
   }

/**
 * Handles all types of return opcodes
 * (return, areturn, ireturn, lreturn, freturn, dreturn, iureturn, lureturn, oreturn)
 */
TR::Register *
OMR::Z::TreeEvaluator::returnEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Compilation *comp = cg->comp();

   if ((node->getOpCodeValue() == TR::Return) && node->isReturnDummy()) return NULL;

   TR::Register * returnAddressReg = cg->allocateRegister();
   TR::Register * returnValRegister = NULL;
   TR::Register * CAARegister = NULL;
   TR::Linkage * linkage = cg->getS390Linkage();

   if (node->getOpCodeValue() != TR::Return)
      returnValRegister = cg->evaluate(node->getFirstChild());

   // add register dependency for the return value register and the return register

   TR::RegisterDependencyConditions * dependencies = NULL ;

   if (node->getOpCode().isJumpWithMultipleTargets() &&
       (node->getNumChildren() > 1
       ) && node->getChild(node->getNumChildren() - 1)->getOpCodeValue() == TR::GlRegDeps)
      {
      TR::Node *glregdep = node->getChild(node->getNumChildren() - 1);
      cg->evaluate(glregdep);
      dependencies = generateRegisterDependencyConditions(cg, glregdep, 3);

      cg->decReferenceCount(glregdep);
      }

   dependencies= new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 3, cg);

#ifdef J9_PROJECT_SPECIFIC
   if ( node->getOpCodeValue() == TR::dereturn )
      dependencies= new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 4, cg);
#endif

   int regDepChildNum = 1;

   switch (node->getOpCodeValue())
      {
      case TR::areturn:
         comp->setReturnInfo(TR_ObjectReturn);
         dependencies->addPostCondition(returnValRegister, linkage->getIntegerReturnRegister());
         break;
      case TR::ireturn:
         dependencies->addPostCondition(returnValRegister, linkage->getIntegerReturnRegister());
         comp->setReturnInfo(TR_IntReturn);
         if (linkage->isNeedsWidening())
            new (cg->trHeapMemory()) TR::S390RRInstruction(TR::InstOpCode::LGFR, node, returnValRegister, returnValRegister, cg);
         break;
      case TR::iureturn:
         comp->setReturnInfo(TR_IntReturn);
         dependencies->addPostCondition(returnValRegister, linkage->getIntegerReturnRegister());
         if (linkage->isNeedsWidening())
            new (cg->trHeapMemory()) TR::S390RRInstruction(TR::InstOpCode::LLGFR, node, returnValRegister, returnValRegister, cg);
         break;
      case TR::lreturn:
      case TR::lureturn:
         comp->setReturnInfo(TR_LongReturn);

         if (TR::Compiler->target.is64Bit())
            {
            dependencies->addPostCondition(returnValRegister, linkage->getLongReturnRegister());
            }
         else
            {
            TR::Register * highRegister = cg->allocateRegister();

            generateRSInstruction(cg, TR::InstOpCode::SRLG, node, highRegister, returnValRegister, 32);

            dependencies->addPostCondition(returnValRegister, linkage->getLongLowReturnRegister());
            dependencies->addPostCondition(highRegister, linkage->getLongHighReturnRegister());

            cg->stopUsingRegister(highRegister);
            }
         break;
      case TR::freturn:
#ifdef J9_PROJECT_SPECIFIC
      case TR::dfreturn:
#endif
         comp->setReturnInfo(TR_FloatReturn);
         dependencies->addPostCondition(returnValRegister, linkage->getFloatReturnRegister());
         break;
      case TR::dreturn:
#ifdef J9_PROJECT_SPECIFIC
      case TR::ddreturn:
#endif
         comp->setReturnInfo(TR_DoubleReturn);
         dependencies->addPostCondition(returnValRegister, linkage->getDoubleReturnRegister());
         break;
#ifdef J9_PROJECT_SPECIFIC
      case TR::dereturn:
         comp->setReturnInfo(TR_DoubleReturn);
         dependencies->addPostCondition(returnValRegister->getHighOrder(), linkage->getLongDoubleReturnRegister0());
         dependencies->addPostCondition(returnValRegister->getLowOrder(), linkage->getLongDoubleReturnRegister2());
         break;
#endif
      case TR::Return:
         comp->setReturnInfo(TR_VoidReturn);
         break;
      }

   TR::Instruction * inst = generateS390PseudoInstruction(cg, TR::InstOpCode::RET, node, dependencies);
   if (cg->supportsBranchPreload())
      {
      int32_t frequency = comp->getCurrentBlock()->getFrequency();
      if (frequency >= cg->_hottestReturn._frequency)
         {
         cg->_hottestReturn._returnInstr = inst;
         cg->_hottestReturn._frequency = frequency;
         cg->_hottestReturn._returnBlock = comp->getCurrentBlock();
         }
      }

   cg->stopUsingRegister(returnAddressReg);
   if (node->getOpCodeValue() != TR::Return)
     cg->decReferenceCount(node->getFirstChild());

   return NULL;
   }

bool OMR::Z::TreeEvaluator::isCandidateForButestEvaluation(TR::Node * node)
   {
   return node->getOpCode().isIf() &&
          node->getFirstChild()->getOpCodeValue() == TR::butest &&
          node->getFirstChild()->isSingleRefUnevaluated() &&
          node->getSecondChild()->getOpCode().isLoadConst() &&
          node->getSecondChild()->getType().isInt32();
   }

bool OMR::Z::TreeEvaluator::isCandidateForCompareEvaluation(TR::Node * node)
   {
   return node->getOpCode().isIf() &&
          TR::TreeEvaluator::isSingleRefUnevalAndCompareOrBu2iOverCompare(node->getFirstChild()) &&
          node->getSecondChild()->getOpCode().isLoadConst() &&
          node->getSecondChild()->getType().isInt32();
   }

bool OMR::Z::TreeEvaluator::isSingleRefUnevalAndCompareOrBu2iOverCompare(TR::Node * node)
   {
   if (!node->isSingleRefUnevaluated())
      return false;

   if (node->getOpCodeValue() == TR::bu2i)
      return TR::TreeEvaluator::isSingleRefUnevalAndCompareOrBu2iOverCompare(node->getFirstChild());

   return false;
   }


/**
 * This helper is for generating a VGNOP for HCR or OSR guard when the guard it was merged with cannot be NOPed.
 * 1. HCR or OSR guard is merged with ProfiledGuard
 * 2. Any NOPable guards when node is switched from icmpne to icmpeq (can potentially happen during block reordering)
 *
 * When the condition is switched on node from eq to ne, HCR or OSR guard has to be patch to go to the fall through path
 */
static inline void generateMergedGuardCodeIfNeeded(TR::Node *node, TR::CodeGenerator *cg)
   {
#ifdef J9_PROJECT_SPECIFIC
   TR::Compilation *comp = cg->comp();
   if (node->isTheVirtualGuardForAGuardedInlinedCall() && cg->getSupportsVirtualGuardNOPing())
      {
      TR_VirtualGuard *virtualGuard = comp->findVirtualGuardInfo(node);

      if (virtualGuard && (virtualGuard->mergedWithOSRGuard() || virtualGuard->mergedWithHCRGuard()))
         {
         TR::RegisterDependencyConditions  *mergedGuardDeps = NULL;
         TR::Instruction *instr = cg->getAppendInstruction();
         if (instr && instr->getNode() == node && instr->getDependencyConditions())
            mergedGuardDeps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(instr->getDependencyConditions(), 1, 0, cg);

         TR_VirtualGuardSite *site = virtualGuard->addNOPSite();
         if (node->getOpCodeValue() == TR::ificmpeq ||
               node->getOpCodeValue() == TR::ifiucmpeq ||
               node->getOpCodeValue() == TR::ifacmpeq ||
               node->getOpCodeValue() == TR::iflcmpeq )

            {
            TR::LabelSymbol *fallThroughLabel = generateLabelSymbol(cg);

            TR::RegisterDependencyConditions  *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions((uint16_t) 0, (uint16_t) 0, cg);

            TR::Instruction *vgnopInstr = generateVirtualGuardNOPInstruction(cg, node, site, deps, fallThroughLabel, instr ? instr->getPrev() : NULL);
            vgnopInstr->setNext(instr);
            cg->setAppendInstruction(instr);
            generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, fallThroughLabel, mergedGuardDeps);
            }
         else
            {
            TR::LabelSymbol *label = node->getBranchDestination()->getNode()->getLabel();

            TR::Instruction *vgnopInstr = generateVirtualGuardNOPInstruction(cg, node, site, mergedGuardDeps, label, instr ? instr->getPrev() : NULL);
            vgnopInstr->setNext(instr);
            cg->setAppendInstruction(instr);
            }
         traceMsg(comp, "generateMergedGuardCodeIfNeeded for %s %s\n",
                  comp->getDebug()?comp->getDebug()->getVirtualGuardKindName(virtualGuard->getKind()):"???Guard" , virtualGuard->mergedWithHCRGuard()?"merged with HCRGuard": virtualGuard->mergedWithOSRGuard() ? "merged with OSRGuard" : "");

         }
      }
#endif
   }


/**
 * Integer compare and branch if equal
 *   - also handles ificmpne
 *   - handles ifacmpeq ifacmpne in 32bit mode
 */
TR::Register *
OMR::Z::TreeEvaluator::ificmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Compilation *comp = cg->comp();

   if (virtualGuardHelper(node, cg))
      {
      return NULL;
      }

   if (TR::TreeEvaluator::isCandidateForButestEvaluation(node))
      {
      TR::Node * cmpNode = node;
      TR::Node * butestNode = node->getFirstChild();
      TR::Node * valueNode = node->getSecondChild();

      return TR::TreeEvaluator::inlineIfButestEvaluator(butestNode, cg, cmpNode, valueNode);
      }

   if (TR::TreeEvaluator::isCandidateForCompareEvaluation(node))
      {
      TR::Node *ifNode = node;
      TR::Node *cmpNode = node->getFirstChild();
      if (cmpNode->getOpCodeValue() == TR::bu2i)
         cmpNode = cmpNode->getFirstChild();
      TR::Node *valueNode = node->getSecondChild();

      return TR::TreeEvaluator::inlineIfBifEvaluator(ifNode, cg, cmpNode, valueNode);
      }

   TR::Node * firstChild = node-> getFirstChild(), * secondChild = node->getSecondChild();

#ifdef J9_PROJECT_SPECIFIC
   if ((firstChild->getOpCodeValue() == TR::instanceof) &&
      !(comp->getOption(TR_OptimizeForSpace) || debug("noInlineIfInstanceOf")) &&
      (firstChild->getRegister() == NULL) &&
      (node->getReferenceCount() <= 1) &&
      secondChild->getOpCode().isLoadConst() &&
      (((secondChild->getInt() == 0 || secondChild->getInt() == 1))
       ))
      {
      if (TR::TreeEvaluator::VMifInstanceOfEvaluator(node, cg) == NULL)
         {
         cg->decReferenceCount(secondChild);
         return NULL;
         }
      }
#endif

   bool inlined = false;
   TR::Register *reg;

   reg = TR::TreeEvaluator::inlineIfArraycmpEvaluator(node, cg, inlined);
   if (inlined)
      {
      generateMergedGuardCodeIfNeeded(node, cg);
      return reg;
      }

   reg = TR::TreeEvaluator::inlineIfTestDataClassHelper(node, cg, inlined);
   if(inlined)
      {
      generateMergedGuardCodeIfNeeded(node, cg);
      return reg;
      }

#ifdef J9_PROJECT_SPECIFIC
   if (cg->profiledPointersRequireRelocation() &&
       node->isProfiledGuard() && secondChild->getOpCodeValue() == TR::aconst &&
       (secondChild->isClassPointerConstant() || secondChild->isMethodPointerConstant()))
      {
      TR_VirtualGuard *virtualGuard = comp->findVirtualGuardInfo(node);
      TR_AOTGuardSite *site = comp->addAOTNOPSite();
      site->setType(virtualGuard->getKind());
      site->setGuard(virtualGuard);
      site->setNode(node);
      site->setAconstNode(secondChild);
      }
#endif

   if (node->getOpCodeValue() == TR::ificmpeq ||
         node->getOpCodeValue() == TR::ifiucmpeq ||
         node->getOpCodeValue() == TR::ifacmpeq)
      {
      reg = generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      reg = generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   generateMergedGuardCodeIfNeeded(node, cg);
   return reg;
   }

/**
 * Helper for ificmp(ge, gt, lt, le)
 * this code was taken from ificmpeqEvaluator,
 * but since ificmpeqEvaluator has more folding going on,
 * it doesn't make sense to call this function from ificmpeqEvaluator
 */
TR::Register* OMR::Z::TreeEvaluator::ifFoldingHelper(TR::Node *node, TR::CodeGenerator *cg, bool &handledBIF)
   {
   handledBIF = true;
   TR::Node * firstChild = node->getFirstChild(), * secondChild = node->getSecondChild();


   bool inlined = false;
   TR::Register *reg = TR::TreeEvaluator::inlineIfTestDataClassHelper(node, cg, inlined);
   if(inlined)
      return reg;


   handledBIF = false;
   return NULL;
   }

/**
 * Integer compare and branch if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::ificmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   bool inlined = false;
   TR::Register *reg;


   reg = TR::TreeEvaluator::inlineIfArraycmpEvaluator(node, cg, inlined);
   if (inlined) return reg;

   bool handledBIF = false;
   TR::Register *result = TR::TreeEvaluator::ifFoldingHelper(node, cg, handledBIF);
   if (handledBIF)
      return result;

   return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BL, TR::InstOpCode::COND_BH);
   }

/**
 * Integer compare and branch if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ificmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   bool inlined = false;

   TR::Register *reg;


   reg = TR::TreeEvaluator::inlineIfArraycmpEvaluator(node, cg, inlined);
   if (inlined) return reg;

   bool handledBIF = false;
   TR::Register *result = TR::TreeEvaluator::ifFoldingHelper(node, cg, handledBIF);
   if (handledBIF)
      return result;

   return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNL, TR::InstOpCode::COND_BNH);
   }

/**
 * Integer compare and branch if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::ificmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   bool inlined = false;

   TR::Register *reg;


   reg = TR::TreeEvaluator::inlineIfArraycmpEvaluator(node, cg, inlined);
   if (inlined) return reg;

   bool handledBIF = false;
   TR::Register *result = TR::TreeEvaluator::ifFoldingHelper(node, cg, handledBIF);
   if (handledBIF)
      return result;

   return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BH, TR::InstOpCode::COND_BL);
   }

/**
 * Integer compare and branch if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ificmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   bool inlined = false;

   TR::Register *reg;

   reg = TR::TreeEvaluator::inlineIfArraycmpEvaluator(node, cg, inlined);
   if (inlined) return reg;

   bool handledBIF = false;
   TR::Register *result = TR::TreeEvaluator::ifFoldingHelper(node, cg, handledBIF);
   if (handledBIF)
      return result;

   return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNH, TR::InstOpCode::COND_BNL);
   }

/**
 * Long compare and branch if equal
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
   generateMergedGuardCodeIfNeeded(node, cg);
   return NULL;
   }

/**
 * Long compare and branch if not equal
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpneEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (virtualGuardHelper(node, cg))
      {
      return NULL;
      }

   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
   generateMergedGuardCodeIfNeeded(node, cg);
   return NULL;
   }

/**
 * Long compare and branch if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNH, TR::InstOpCode::COND_BNL);
   return NULL;
   }

/**
 * Long compare and branch if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BL, TR::InstOpCode::COND_BH);
   return NULL;
   }

/**
 * Long compare and branch if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNL, TR::InstOpCode::COND_BNH);
   return NULL;
   }

/**
 * Long compare and branch if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::iflcmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BH, TR::InstOpCode::COND_BL);
   return NULL;
   }

/**
 * Address compare if equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifacmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpeqEvaluator(node, cg);
   }

/**
 * Address compare if not equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifacmpneEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpeqEvaluator(node, cg);
   }

/**
 * Byte compare and branch if equal
 *   - also handles ifbcmpne
 */
TR::Register *
OMR::Z::TreeEvaluator::ifbcmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::ifbcmpeq || node->getOpCodeValue() == TR::ifbucmpeq)
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

///////////////////////////////////////////////////////////////////////////////////////
// ifbcmpneEvaluator (byte compare and branch if not equal) handled by ifbcmpeqEvaluator
///////////////////////////////////////////////////////////////////////////////////////

/**
 * Byte compare and branch if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifbcmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpltEvaluator(node, cg);
   }

/**
 * Byte compare and branch if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifbcmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgeEvaluator(node, cg);
   }

/**
 * Byte compare and branch if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifbcmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgtEvaluator(node, cg);
   }

/**
 * Byte compare and branch if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifbcmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpleEvaluator(node, cg);
   }

/**
 * Short integer compare and branch if equal
 *    - also handles ifscmpne (short integer compare and branch if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::ifscmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::ifscmpeq)
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Short integer compare and branch if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifscmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpltEvaluator(node, cg);
   }

/**
 * Short integer compare and branch if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifscmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgeEvaluator(node, cg);
   }

/**
 * Short integer compare and branch if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifscmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgtEvaluator(node, cg);
   }

/**
 * Short integer compare and branch if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifscmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpleEvaluator(node, cg);
   }

/**
 * Char compare and branch if equal
 *    - also handles ifsucmpne (char compare and branch if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::ifsucmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::ifsucmpeq)
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBranch(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Char compare and branch if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifsucmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpltEvaluator(node, cg);
   }

/**
 * Char compare and branch if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifsucmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgeEvaluator(node, cg);
   }

/**
 * Char compare and branch if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::ifsucmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpgtEvaluator(node, cg);
   }

/**
 * Char compare and branch if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::ifsucmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::ificmpleEvaluator(node, cg);
   }


/**
 * Integer compare if equal
 *    - also handles icmpne (integer compare if notequal)
 *    - and acmpne (address compare if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::icmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::icmpeq ||
         node->getOpCodeValue() == TR::iucmpeq ||
         node->getOpCodeValue() == TR::acmpeq)
      {
      // RXSBG only supported on z10+
      if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z10))
         {
         TR::Node* firstChild = node->getFirstChild();
         TR::Node* secondChild = node->getSecondChild();
         if (node->getOpCodeValue() != TR::acmpeq &&
             TR::Compiler->target.is64Bit() &&
             firstChild->getOpCodeValue() == TR::iand &&
             firstChild->getSecondChild()->getOpCodeValue() == TR::iconst &&
             secondChild->getOpCodeValue() == TR::iconst &&
             secondChild->getInt() == 0 &&
             performTransformation(cg->comp(), "O^O CODE GENERATION:  ===>   Use RXSBG to perform icmpeq  <==\n"))
            {
            int64_t val1 = node->getFirstChild()->getSecondChild()->getInt();
            if ((val1 & (val1 - 1)) == 0)
               {
               int32_t rotBy = (leadingZeroes(val1) + 1) & 0x3f; // 0-63
               TR::Register *source = NULL;
               if (firstChild->getRegister() == NULL)
                  {
                  source = cg->evaluate(firstChild->getFirstChild());
                  cg->recursivelyDecReferenceCount(firstChild);
                  }
               else
                  {
                  source = cg->evaluate(firstChild);
                  cg->decReferenceCount(firstChild);
                  }
               TR::Register *target = cg->allocateRegister();
               generateRIInstruction(cg, TR::InstOpCode::LHI, node, target, 1);
               generateRIEInstruction(cg, TR::InstOpCode::RXSBG, node, target, source, 63, 128+63, rotBy);
               node->setRegister(target);
               cg->decReferenceCount(secondChild);
               return target;
               }
            }
         }

      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Integer compare if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::icmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Node * firstChild = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();

   if (!node->getOpCode().isUnsignedCompare() &&
       firstChild->getReferenceCount() == 1 && firstChild->isNonNegative() && secondChild->isNonNegative() &&
       performTransformation(cg->comp(), "O^O CODE GENERATION:  ===>   Use SR/SRL to perform icmplt  <==\n"))
      {
      // Even if 1st child refcount is 1, the register may be shared with other nodes and still be live.
      // gprClobberEvaluate will do the right thing.
      TR::Register * firstReg = cg->gprClobberEvaluate(firstChild);
      TR::Register * secondReg = cg->evaluate(secondChild);
      generateRRInstruction(cg, TR::InstOpCode::SR, node, firstReg, secondReg);
      generateRSInstruction(cg, TR::InstOpCode::SRL, node, firstReg, 31);

      node->setRegister(firstReg);
      cg->decReferenceCount(firstChild);
      cg->decReferenceCount(secondChild);

      return firstReg;
      }

   return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BL, TR::InstOpCode::COND_BH);
   }

/**
 * Integer compare if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::icmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNL, TR::InstOpCode::COND_BNH);
   }

/**
 * Integer compare if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::icmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Node * firstChild = node->getFirstChild();
   TR::Node * secondChild = node->getSecondChild();

   if (!node->getOpCode().isUnsignedCompare() &&
       secondChild->getReferenceCount()==1 && firstChild->isNonNegative() && secondChild->isNonNegative() &&
       performTransformation(cg->comp(), "O^O CODE GENERATION:  ===>   Use SR/SRL to perform icmpgt  <==\n"))
      {
      TR::Register * firstReg = cg->evaluate(firstChild);
      // Even if 2nd child refcount is 1, the register may be shared with other nodes and still be live.
      // gprClobberEvaluate will do the right thing.
      TR::Register * secondReg = cg->gprClobberEvaluate(secondChild);
      generateRRInstruction(cg, TR::InstOpCode::SR, node, secondReg, firstReg);
      generateRSInstruction(cg, TR::InstOpCode::SRL, node, secondReg, 31);

      node->setRegister(secondReg);
      cg->decReferenceCount(firstChild);
      cg->decReferenceCount(secondChild);

      return secondReg;
      }

   return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BH, TR::InstOpCode::COND_BL);
   }

/**
 * Integer compare if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::icmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNH, TR::InstOpCode::COND_BNL);
   }

/**
 * Long compare if equal
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, CMP4BOOLEAN);
   }

/**
 * Long compare if not equal
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpneEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, CMP4BOOLEAN);
   }

/**
 * Long compare if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNH, CMP4BOOLEAN);
   }

/**
 * Long compare if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BL, CMP4BOOLEAN);
   }

/**
 * Long compare if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNL, CMP4BOOLEAN);
   }

/**
 * Long compare if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return generateS390lcmpEvaluator64(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BH, CMP4BOOLEAN);
   }

/**
 * Address compare if equal
 *    - also handles acmpne (address compare if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::acmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpeqEvaluator(node, cg);
   }

/**
 * ) handled by icmpeqEvaluator
 */

/**
 * Byte compare if equal
 *    - also handles bcmpne (byte compare if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::bcmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::bcmpeq || node->getOpCodeValue() == TR::bucmpeq)
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Byte compare if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::bcmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpltEvaluator(node, cg);
   }

/**
 * Byte compare if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::bcmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgeEvaluator(node, cg);
   }

/**
 * Byte compare if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::bcmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgtEvaluator(node, cg);
   }

/**
 * Byte compare if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::bcmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpleEvaluator(node, cg);
   }

/**
 * Short integer compare if equal
 *    - also handles scmpne (short integer compare if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::scmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::scmpeq)
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Short integer compare if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::scmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpltEvaluator(node, cg);
   }

/**
 * Short integer compare if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::scmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgeEvaluator(node, cg);
   }

/**
 * Short integer compare if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::scmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgtEvaluator(node, cg);
   }

/**
 * Short integer compare if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::scmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpleEvaluator(node, cg);
   }

/**
 * Char compare if equal
 * - also handles sucmpne (char compare if not equal)
 */
TR::Register *
OMR::Z::TreeEvaluator::sucmpeqEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   if (node->getOpCodeValue() == TR::sucmpeq)
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, TR::InstOpCode::COND_BE);
      }
   else
      {
      return generateS390CompareBool(node, cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      }
   }

/**
 * Char compare if less than
 */
TR::Register *
OMR::Z::TreeEvaluator::sucmpltEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpltEvaluator(node, cg);
   }

/**
 * Char compare if greater than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::sucmpgeEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgeEvaluator(node, cg);
   }

/**
 * Char compare if greater than
 */
TR::Register *
OMR::Z::TreeEvaluator::sucmpgtEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpgtEvaluator(node, cg);
   }

/**
 * Char compare if less than or equal
 */
TR::Register *
OMR::Z::TreeEvaluator::sucmpleEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::icmpleEvaluator(node, cg);
   }

/**
 * Long compare (1 if child1 > child2, 0 if child1 == child2,
 *  -1 if child1 < child2 or unordered)
 */
TR::Register *
OMR::Z::TreeEvaluator::lcmpEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return lcmpHelper64(node, cg);
   }

void OMR::Z::TreeEvaluator::tableEvaluatorCaseLabelHelper(TR::Node * node, TR::CodeGenerator * cg, tableKind tableKindToBeEvaluated, int32_t numBranchTableEntries, TR::Register * selectorReg, TR::Register * branchTableReg, TR::Register *reg1)
   {
   TR::Compilation *comp = cg->comp();
      {
      TR_ASSERT(reg1, "Java must have allocated a temp reg");
      TR_ASSERT( tableKindToBeEvaluated == AddressTable32bit || tableKindToBeEvaluated == AddressTable64bitIntLookup || tableKindToBeEvaluated == AddressTable64bitLongLookup, "For Java, must be using Address Table");

      new (cg->trHeapMemory()) TR::S390RIInstruction(TR::InstOpCode::BRAS, node, reg1, (4 + (sizeof(uintptrj_t) * numBranchTableEntries)) / 2, cg);

      // Generate the data constants with the target label addresses.
      for (int32_t i = 0; i < numBranchTableEntries; ++i)
         {
         TR::Node * caseNode = node->getChild(i + 2);
         TR::LabelSymbol * label = caseNode->getBranchDestination()->getNode()->getLabel();
         new (cg->trHeapMemory()) TR::S390LabelInstruction(TR::InstOpCode::DC, node, label, cg);
         }

      TR::MemoryReference * tempMR = generateS390MemoryReference(reg1, selectorReg, 0, cg);
      generateRXInstruction(cg, TR::InstOpCode::getLoadOpCode(), node, selectorReg, tempMR);
      tempMR->stopUsingMemRefRegister(cg);
      }
   }


/**
 * child1 is the selector, child2 the default destination, subsequent children
 * are the branch targets.
 * There may me other children after the branch targets, so use getCaseIndexUpperBound()
 * instead of getNumChildren() to determine the number of branch targets
 * Unlike lookuptable, this table is contiguous from 0 up to maxentry.
 */
TR::Register *
OMR::Z::TreeEvaluator::tableEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Compilation *comp = cg->comp();
   int32_t i;
   uint32_t upperBound = node->getCaseIndexUpperBound();
   uint32_t numBranchTableEntries = upperBound - 2;  // First two children are not branch cases

   TR::Register * selectorReg = cg->gprClobberEvaluate(node->getFirstChild());


   bool canSkipBoundTest = node->isSafeToSkipTableBoundCheck();
   TR::Node * secondChild = node->getSecondChild();
   TR::RegisterDependencyConditions * deps = NULL;
   bool isUnsigned = node->getFirstChild()->getOpCode().isUnsigned() || node->getFirstChild()->getOpCodeValue() == TR::bu2i; // TODO: check in  switchAnalyzer

   TR::Register * branchTableReg = NULL;
   TR::Register * reg1;
   // The branch table address may already be in a register
   if (upperBound < node->getNumChildren() &&
       (node->getChild(upperBound)->getOpCodeValue() == TR::loadaddr ||
        node->getChild(upperBound)->getOpCodeValue() == TR::aload ||
        node->getChild(upperBound)->getOpCodeValue() == TR::aRegLoad))
      {
      branchTableReg = node->getChild(upperBound)->getRegister();
      }
   if (branchTableReg != NULL)
      {
      // In this case reg1 is not needed
      reg1 = NULL;
      i = 2;   // Number of deps
      }
   else
      {
      reg1 = cg->allocateRegister();
      i = 3;   // Number of deps
      }

   if (secondChild->getNumChildren() > 0)
      {
      // GRA
      cg->evaluate(secondChild->getFirstChild());
      deps = generateRegisterDependencyConditions(cg, secondChild->getFirstChild(), i);
      if (reg1 != NULL)
         deps->addPostConditionIfNotAlreadyInserted(reg1, TR::RealRegister::AssignAny);
      deps->addPostConditionIfNotAlreadyInserted(selectorReg, TR::RealRegister::AssignAny);
      }
   else
      {
      deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, i, cg);
      if (reg1 != NULL)
         deps->addPostCondition(reg1, TR::RealRegister::AssignAny);

      deps->addPostCondition(selectorReg, TR::RealRegister::AssignAny);
      }

   // Determine if the selector register needs to be sign-extended on a 64-bit system



   tableKind tableKindToBeEvaluated = AddressTable32bit;

   if (TR::Compiler->target.is64Bit())
      {
      if (node->getFirstChild()->getType().isInt64())
         tableKindToBeEvaluated = AddressTable64bitLongLookup;
      else
         tableKindToBeEvaluated = AddressTable64bitIntLookup;

      TR::DataType selectorType = node->getFirstChild()->getType();
      if (selectorType.isInt8() || selectorType.isInt16() || selectorType.isInt32())
         generateRRInstruction(cg, TR::InstOpCode::LGFR, node, selectorReg, selectorReg);
      }
   else
      {
      TR_ASSERT( !node->getFirstChild()->getType().isInt64(), "Unhandled long selector type for table node %p\n",node);
      tableKindToBeEvaluated = AddressTable32bit;
      }

   /*
    * High level idea -- there are two modes, a relative table, where we load a relative offset from the table and branch to it, and an address table, where we load an address out of the table and branch to it
    * since cases are indexed by 1, we need to shift by either 4 or 8 to get the next value in memory.
    */

   switch (tableKindToBeEvaluated)
      {
      case AddressTable32bit:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CL, node, selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);       //make sure the case selector is within range of our case constants
         generateRSInstruction(cg, TR::InstOpCode::SLL, node, selectorReg, 2);
         }
         break;
      case AddressTable64bitIntLookup:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CL, node, selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);

         generateRSInstruction(cg, TR::InstOpCode::SLLG, node, selectorReg, selectorReg, 3);
         }
         //generateRRInstruction(cg, TR::InstOpCode::LGFR, node, selectorReg, selectorReg);
         break;
      case AddressTable64bitLongLookup:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CLG, node, selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);
         generateRSInstruction(cg, TR::InstOpCode::SLLG, node, selectorReg, selectorReg, 3);
         generateRRInstruction(cg, TR::InstOpCode::LGFR, node, selectorReg, selectorReg);
         }
         break;
      case RelativeTable32bit:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CL, node, selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);       //make sure the case selector is within range of our case constants
         generateRSInstruction(cg, TR::InstOpCode::SLL, node, selectorReg, 2);
         }
         break;
      case RelativeTable64bitIntLookup:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CL, node,selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);       //make sure the case selector is within range of our case constants
         generateRSInstruction(cg, TR::InstOpCode::SLL, node, selectorReg, 2);
         }
         break;
      case RelativeTable64bitLongLookup:
         {
         if(!node->chkCannotOverflow())
           generateS390CompareAndBranchInstruction(cg, TR::InstOpCode::CLG, node, selectorReg, numBranchTableEntries, TR::InstOpCode::COND_BNL, node->getSecondChild()->getBranchDestination()->getNode()->getLabel(), false, false);
         generateRSInstruction(cg, TR::InstOpCode::SLLG, node, selectorReg, selectorReg, 2);
         }
         break;
      }

   TR::TreeEvaluator::tableEvaluatorCaseLabelHelper(node,cg, tableKindToBeEvaluated, numBranchTableEntries, selectorReg, branchTableReg, reg1);

   TR::Instruction *cursor = generateS390RegInstruction(cg, TR::InstOpCode::BCR, node, selectorReg, deps);
   ((TR::S390RegInstruction *)cursor)->setBranchCondition(TR::InstOpCode::COND_BCR);


   for (i = 0; i < node->getNumChildren(); ++i)
      {
      cg->decReferenceCount(node->getChild(i));
      }

   // GRA
   if (secondChild->getNumChildren() > 0)
      {
      cg->decReferenceCount(secondChild->getFirstChild());
      }

   if (reg1 != NULL)
      cg->stopUsingRegister(reg1);
   cg->stopUsingRegister(selectorReg);

   return NULL;
   }

bool isSwitchFoldableNoncall(TR::Node * node)
   {
   bool isFoldable = false;
   if(node->getOpCodeValue() == TR::bitOpMem)
      {
      isFoldable = true;
      }
   return isFoldable;
   }

/**
 * Lookupswitch/trtLoopup (child1 is selector expression, child2
 * the default destination, subsequent children are case nodes)
 */
TR::Register *
OMR::Z::TreeEvaluator::lookupEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   //fold builtin if possible
   TR::Node* firstChild = node->getFirstChild();

   // Handle selector node
   TR::Node * selectorNode = node->getFirstChild();
   TR::Register * selectorReg = NULL;

   selectorReg = cg->gprClobberEvaluate(selectorNode);

   int32_t numChildren = node->getCaseIndexUpperBound();
   TR::Node * defaultChild = node->getSecondChild();
   TR::DataType type = selectorNode->getType();

   CASECONST_TYPE caseVal = 0;
   TR::Register *tempReg = NULL;
   TR::Instruction *cursor = NULL;

   if (node->getOpCodeValue() == TR::trtLookup)
      {
      tempReg = cg->allocateRegister();
      //No need to set up regDep for tempReg as no need to take care of its spill for any branch
      //which dest is out of trtLookup block.
      }

   // Handle non default cases
   for (int32_t ii = 2; ii < numChildren; ii++)
      {
      TR::Node * child = node->getChild(ii);

      // for 31bit mode with 64 bit values
      TR::LabelSymbol *skipLoCompareLabel = NULL;
      if (type.isInt64() && TR::Compiler->target.is32Bit())
         skipLoCompareLabel = generateLabelSymbol(cg);

      // Compare a key value
      TR::InstOpCode::S390BranchCondition brCond = TR::InstOpCode::COND_NOP;

      if (node->getOpCodeValue() == TR::trtLookup)
         {
         caseVal = child->getCaseConstant();
         //Need to check tryTwoBranchPattern for a proper conversion.
         TR_ASSERT(0, "Only caseCC and caseR2 supported\n");
         }
      else
         {
         if (type.isInt64())
            {
            generateS390ImmOp(cg, TR::InstOpCode::CG, node, selectorReg, selectorReg, child->getCaseConstant());
            }
         else
            {
            generateS390ImmOp(cg, TR::InstOpCode::C, node, selectorReg, selectorReg, child->getCaseConstant());
            }

         brCond = TR::InstOpCode::COND_BE;
         }

      // If key == val -> branch to target BB
      if (child->getNumChildren() > 0)
         {
         // GRA
         cg->evaluate(child->getFirstChild());
         if (type.isInt64() && TR::Compiler->target.is32Bit())
            {
            cursor = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, brCond, node, child->getBranchDestination()->getNode()->getLabel());
            generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, skipLoCompareLabel, generateRegisterDependencyConditions(cg, child->getFirstChild(), 0), cursor);
            }
         else
            cursor = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, brCond, node, child->getBranchDestination()->getNode()->getLabel(),
               generateRegisterDependencyConditions(cg, child->getFirstChild(), 0));
         }
      else
         {
         cursor = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, brCond, node, child->getBranchDestination()->getNode()->getLabel());
         if (type.isInt64() && TR::Compiler->target.is32Bit())
            generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, skipLoCompareLabel);
         }

      }

   bool needDefaultBranch = true;

   //Peek if the default branch is to the immediately following block, only for trtlookup
   if (node->getOpCodeValue() == TR::trtLookup)
      {
      TR::TreeTop *defaultDest = defaultChild->getBranchDestination();
      TR::Block *defaultDestBlock = defaultDest->getNode()->getBlock();
      TR::Block *fallThroughBlock = cg->getCurrentEvaluationBlock()->getNextBlock();
      needDefaultBranch = defaultDestBlock != fallThroughBlock;
      }

   // Branch to Default
   if (defaultChild->getNumChildren() > 0)
      {
      // GRA
      cg->evaluate(defaultChild->getFirstChild());

      if (needDefaultBranch)
         generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BRC, node, defaultChild->getBranchDestination()->getNode()->getLabel(),
               generateRegisterDependencyConditions(cg, defaultChild->getFirstChild(), 0));
      else if (cursor != NULL && cursor->getDependencyConditions() == NULL)
         cursor->setDependencyConditions(generateRegisterDependencyConditions(cg, defaultChild->getFirstChild(), 0));
      }
   else
      {
      if (needDefaultBranch)
         generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BRC, node, defaultChild->getBranchDestination()->getNode()->getLabel());
      }

   cg->decReferenceCount(selectorNode);
   cg->stopUsingRegister(selectorReg);

   if (tempReg)
      {
      cg->stopUsingRegister(tempReg);
      }

   return NULL;
   }

/**
 * This is a helper function used to generate the snippet
 */
TR::Register *OMR::Z::TreeEvaluator::evaluateNULLCHKWithPossibleResolve(TR::Node * node, bool needsResolve, TR::CodeGenerator * cg)
   {
   // NOTE:
   // If no code is generated for the null check, just evaluate the
   // child and decrement its use count UNLESS the child is a pass-through node
   // in which case some kind of explicit test or indirect load must be generated
   // to force the null check at this point.

   TR::Node * firstChild = node->getFirstChild();
   TR::ILOpCode & opCode = firstChild->getOpCode();
   TR::Compilation *comp = cg->comp();
   TR::Node * reference = NULL;
   TR::InstOpCode::S390BranchCondition branchOpCond = TR::InstOpCode::COND_BZ;

   bool hasCompressedPointers = false;

   TR::Node * n = firstChild;

   // NULLCHK has a special case with compressed pointers.
   // In the scenario where the first child is TR::l2a, the
   // node to be null checked is not the iiload, but its child.
   // i.e. aload, aRegLoad, etc.
   if (comp->useCompressedPointers() &&
         firstChild->getOpCodeValue() == TR::l2a)
      {
      hasCompressedPointers = true;
      TR::ILOpCodes loadOp = comp->il.opCodeForIndirectLoad(TR::Int32);
      TR::ILOpCodes rdbarOp = comp->il.opCodeForIndirectReadBarrier(TR::Int32);
      while (n->getOpCodeValue() != loadOp && n->getOpCodeValue() != rdbarOp)
         n = n->getFirstChild();
      reference = n->getFirstChild();
      }
   else
      {
      reference = node->getNullCheckReference();
      }

   // Skip the NULLCHK for TR::loadaddr nodes.
   //
   if(cg->getSupportsImplicitNullChecks() && reference->getOpCodeValue() == TR::loadaddr)
      {
      cg->evaluate(firstChild);
      cg->decReferenceCount(firstChild);
      return NULL;
      }

   bool needExplicitCheck  = true;
   bool needLateEvaluation = true;

   // Add the explicit check after this instruction
   //
   TR::Instruction *appendTo = NULL;

   // determine if an explicit check is needed
   if(cg->getSupportsImplicitNullChecks() && !firstChild->isUnneededIALoad())
      {
      if (opCode.isLoadVar() ||
               (TR::Compiler->target.is64Bit() && opCode.getOpCodeValue()==TR::l2i) ||
               (hasCompressedPointers && firstChild->getFirstChild()->getOpCode().getOpCodeValue() == TR::i2l))
         {
         TR::SymbolReference *symRef = NULL;

         if (opCode.getOpCodeValue()==TR::l2i)
            symRef = n->getFirstChild()->getSymbolReference();
         else
            symRef = n->getSymbolReference();

         // We prefer to generate an explicit NULLCHK vs an implicit one
         // to prevent potential costs of a cache miss on an unnecessary load.
         if (n->getReferenceCount() == 1 && !n->getSymbolReference()->isUnresolved())
            {
            // If the child is only used here, we don't need to evaluate it
            // since all we need is the grandchild which will be evaluated by
            // the generation of the explicit check below.
            needLateEvaluation = false;

            // at this point, n is the raw iiload (created by lowerTrees) and
            // reference is the aload of the object. node->getFirstChild is the
            // l2a sequence; as a result, n's refCount will always be 1
            // and node->getFirstChild's refCount will be at least 2 (one under the nullchk
            // and the other under the translate treetop)
            //
            if (hasCompressedPointers && node->getFirstChild()->getReferenceCount() > 2)
               needLateEvaluation = true;
            }

         // Check if offset from a NULL reference will fall into the inaccessible bytes,
         // resulting in an implicit trap being raised.
         else if (symRef &&
             (symRef->getSymbol()->getOffset() + symRef->getOffset()) < cg->getNumberBytesReadInaccessible())
            {
            needExplicitCheck = false;

            // If the child is an arraylength which has been reduced to an iiload,
            // and is only going to be used immediately in a BNDCHK, combine the checks.
            //
            TR::TreeTop *nextTreeTop = cg->getCurrentEvaluationTreeTop()->getNextTreeTop();
            if (n->getReferenceCount() == 2 && nextTreeTop)
               {
               TR::Node *nextTopNode = nextTreeTop->getNode();

               if (nextTopNode)
                  {
                  if (nextTopNode->getOpCode().isBndCheck())
                     {
                     if ((nextTopNode->getOpCode().isSpineCheck() && (nextTopNode->getChild(2) == n)) ||
                         (!nextTopNode->getOpCode().isSpineCheck() && (nextTopNode->getFirstChild() == n)))
                        {
                        needLateEvaluation = false;
                        nextTopNode->setHasFoldedImplicitNULLCHK(true);
                        traceMsg(comp, "\nMerging NULLCHK [%p] and BNDCHK [%p] of load child [%p]", node, nextTopNode, n);
                        }
                     }
                  else if (nextTopNode->getOpCode().isIf() &&
                           nextTopNode->isNonoverriddenGuard() &&
                           nextTopNode->getFirstChild() == firstChild)
                     {
                     needLateEvaluation = false;
                     needExplicitCheck = true;
                     reference->incReferenceCount(); // will be decremented again later
                     }
                  }
               }
            }
         }
      else if (opCode.isStore())
         {
         TR::SymbolReference *symRef = n->getSymbolReference();
         if (n->getOpCode().hasSymbolReference() &&
             symRef->getSymbol()->getOffset() + symRef->getOffset() < cg->getNumberBytesWriteInaccessible())
            {
            needExplicitCheck = false;
            }
         }
      else if (opCode.isCall()     &&
               opCode.isIndirect() &&
               cg->getNumberBytesReadInaccessible() > TR::Compiler->om.offsetOfObjectVftField())
         {
         needExplicitCheck = false;
         }
      else if (opCode.getOpCodeValue() == TR::iushr &&
               cg->getNumberBytesReadInaccessible() > cg->fe()->getOffsetOfContiguousArraySizeField())
         {
         // If the child is an arraylength which has been reduced to an iushr,
         // we must evaluate it here so that the implicit exception will occur
         // at the right point in the program.
         //
         // This can occur when the array length is represented in bytes, not elements.
         // The optimizer must intervene for this to happen.
         //
         cg->evaluate(n->getFirstChild());
         needExplicitCheck = false;
         }
      else if (opCode.getOpCodeValue() == TR::monent ||
               opCode.getOpCodeValue() == TR::monexit)
         {
         // The child may generate inline code that provides an implicit null check
         // but we won't know until the child is evaluated.
         //
         reference->incReferenceCount(); // will be decremented again later
         needLateEvaluation = false;
         cg->evaluate(reference);
         appendTo = cg->getAppendInstruction();
         cg->evaluate(firstChild);

         if (cg->getImplicitExceptionPoint() &&
             cg->getNumberBytesReadInaccessible() > cg->fe()->getOffsetOfContiguousArraySizeField())
            {
            needExplicitCheck = false;
            cg->decReferenceCount(reference);
            }
         }
      }

   // Generate the code for the null check
   //
   if(needExplicitCheck)
      {
      TR::Register * targetRegister = NULL;
      if (cg->getHasResumableTrapHandler())
         {

         // Use Load-And-Trap on zHelix if available.
         // This loads the field and performance a NULLCHK on the field value.
         // i.e.  o.f == NULL
         if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::zEC12) &&
             reference->getOpCode().isLoadVar() &&
             (reference->getOpCodeValue() != TR::ardbari) &&
             reference->getRegister() == NULL)
            {
            targetRegister = cg->allocateCollectedReferenceRegister();
            appendTo = generateRXInstruction(cg, TR::InstOpCode::getLoadAndTrapOpCode(), node, targetRegister, generateS390MemoryReference(reference, cg), appendTo);
            reference->setRegister(targetRegister);
            }
         else if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::zEC12) &&
                  reference->getRegister() == NULL &&
                  comp->useCompressedPointers() &&
                  reference->getOpCodeValue() == TR::l2a &&
                  reference->getFirstChild()->getOpCodeValue() == TR::iu2l &&
                  reference->getFirstChild()->getFirstChild()->getOpCode().isLoadVar() &&
                  TR::Compiler->om.compressedReferenceShiftOffset() == 0)
            {
            targetRegister = cg->evaluate(reference);

            appendTo = cg->getAppendInstruction();

            if (appendTo->getOpCodeValue() == TR::InstOpCode::LLGF)
               {
               appendTo->setOpCodeValue(TR::InstOpCode::LLGFAT);

               appendTo->setNode(node);
               }
            else
               {
               appendTo = generateRIEInstruction(cg, TR::InstOpCode::getCmpImmTrapOpCode(), node, targetRegister, (int16_t)0, TR::InstOpCode::COND_BE, appendTo);
               }
            }
         else
            {
            targetRegister = reference->getRegister();

            if (targetRegister == NULL)
               targetRegister = cg->evaluate(reference);

            appendTo = generateRIEInstruction(cg, TR::InstOpCode::getCmpImmTrapOpCode(), node, targetRegister, (int16_t)0, TR::InstOpCode::COND_BE, appendTo);
            }

          TR::Instruction* cursor = appendTo;
          cursor->setThrowsImplicitException();
          cursor->setExceptBranchOp();
          cg->setCanExceptByTrap(true);
          cursor->setNeedsGCMap(0x0000FFFF);
          if (TR::Compiler->target.isZOS())
             killRegisterIfNotLocked(cg, TR::RealRegister::GPR4, cursor);
         }
      else
         {
         // NULLCHK snippet label.
         TR::LabelSymbol * snippetLabel = generateLabelSymbol(cg);
         TR::SymbolReference *symRef = node->getSymbolReference();
         cg->addSnippet(new (cg->trHeapMemory()) TR::S390HelperCallSnippet(cg, node, snippetLabel, symRef));

         if (!firstChild->getOpCode().isCall() &&
               reference->getOpCode().isLoadVar() &&
               (reference->getOpCodeValue() != TR::ardbari) && // ardbari needs to be evaluated before being NULLCHK'ed because of its side effect.
               reference->getOpCode().hasSymbolReference() &&
               reference->getRegister() == NULL)
            {
            TR_ASSERT( NULLVALUE == 0, "Can not generate ICM if NULL is not 0");
            bool isInternalPointer = reference->getSymbolReference()->getSymbol()->isInternalPointer();
            if ((reference->getOpCode().isLoadIndirect() || reference->getOpCodeValue() == TR::aload) && !isInternalPointer)
               {
               targetRegister = cg->allocateCollectedReferenceRegister();
               }
            else
               {
               targetRegister = cg->allocateRegister();
               if (isInternalPointer)
                  {
                  targetRegister->setPinningArrayPointer(reference->getSymbolReference()->getSymbol()->castToInternalPointerAutoSymbol()->getPinningArrayPointer());
                  targetRegister->setContainsInternalPointer();
                  }
               }

            reference->setRegister(targetRegister);
            TR::MemoryReference * tempMR = generateS390MemoryReference(reference, cg);
            appendTo = generateRXInstruction(cg, TR::InstOpCode::getLoadTestOpCode(), reference, targetRegister, tempMR, appendTo);
            tempMR->stopUsingMemRefRegister(cg);

            appendTo = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, node, snippetLabel, appendTo);
            TR::Instruction *brInstr = appendTo;
            brInstr->setExceptBranchOp();
            }
         else
            {
            TR::Node *n = NULL;

            // After the NULLCHK is generated, the nodes are guaranteed
            // to be non-zero.  Mark the nodes, so that subsequent
            // evaluations can be optimized.
            if (comp->useCompressedPointers() &&
                reference->getOpCodeValue() == TR::l2a)
               {
               reference->setIsNonNull(true);
               n = reference->getFirstChild();
               TR::ILOpCodes loadOp = comp->il.opCodeForIndirectLoad(TR::Int32);
               while (n->getOpCodeValue() != loadOp)
                  {
                  n->setIsNonZero(true);
                  n = n->getFirstChild();
                  }
               n->setIsNonZero(true);
               }

            TR::InstOpCode::Mnemonic cmpOpCode = TR::InstOpCode::BAD;

            // For compressed pointers case, if we find the compressed value,
            // and it has already been evaluated into a register,
            // we can take advantage of the uncompressed value and evaluate
            // the compare result earlier.
            //
            // If it hasn't been evalauted yet, we want to evaluate the entire
            // l2a tree, which might generate LLGF.  In that case, the better
            // choice is to perform the NULLCHK on the decompressed address.
            if (n != NULL && n->getRegister() != NULL)
               {
               targetRegister = n->getRegister();
               cg->evaluate(reference);

               // For concurrent scavenge the source is loaded and shifted by the guarded load, thus we need to use CG
               // here for a non-zero compressedrefs shift value
               if (TR::Compiler->om.readBarrierType() != gc_modron_readbar_none)
                  {
                  cmpOpCode = TR::InstOpCode::getCmpOpCode();
                  }
               else
                  {
                  cmpOpCode = (n->getOpCode().getSize() > 4) ? TR::InstOpCode::CG: TR::InstOpCode::C;
                  }
               }
            else
               {
               targetRegister = cg->evaluate(reference);
               cmpOpCode = TR::InstOpCode::getCmpOpCode();   // reference is always an address type
               }

            appendTo = generateS390CompareAndBranchInstruction(cg, cmpOpCode, node, targetRegister, NULLVALUE, branchOpCond, snippetLabel, false, true, appendTo);
            TR::Instruction * cursor = appendTo;
            cursor->setExceptBranchOp();
            }

         }
      }


   if(needLateEvaluation)
      {
      // If this is a load with ref count of 1, just decrease the ref count
      // since it must have been evaluated. Otherwise, evaluate it.
      // for compressedPointers, the firstChild will have a refCount
      // of atleast 2 (the other under an anchor node)
      //
      if (opCode.isLoad() && firstChild->getReferenceCount() == 1 && !firstChild->getSymbolReference()->isUnresolved())
         {
         if (needExplicitCheck && firstChild->getRegister() == NULL)
            {
            // load with reference count 1 and no register, this means load is not evaluated
            // this load is only meaningful for explicit NULL CHK
            // it should not be evaluated yet. Its reference counter will be decrease later.
            cg->decReferenceCount(reference);
            }
         else
            {
            cg->decReferenceCount(firstChild);
            cg->decReferenceCount(reference);
            }
         }
      else
         {
         // cg->comp()->useCompressedPointers
         // this will end up evaluating either the l2a sequence (for loads)
         // or the iistore (in case of iastores)
         //
         // for stores under NULLCHKs, artificially bump
         // down the reference count before evaluation (since stores
         // return null as registers)
         //
         bool fixRefCount = false;
         if (comp->useCompressedPointers())
            {
            if (firstChild->getOpCode().isStoreIndirect() &&
                  firstChild->getReferenceCount() > 1)
               {
               firstChild->decReferenceCount();
               fixRefCount = true;
               }
            }
         cg->evaluate(firstChild);
         if (fixRefCount)
            firstChild->incReferenceCount();
         }
      }
   else if (needExplicitCheck)
      {
      cg->decReferenceCount(reference);
      }

   if (comp->useCompressedPointers())
      cg->decReferenceCount(node->getFirstChild());
   else
      cg->decReferenceCount(firstChild);

   // If an explicit check has not been generated for the null check, there is
   // an instruction that will cause a hardware trap if the exception is to be
   // taken. If this method may catch the exception, a GC stack map must be
   // created for this instruction. All registers are valid at this GC point
   // TODO - if the method may not catch the exception we still need to note
   // that the GC point exists, since maps before this point and after it cannot
   // be merged.
   //
   if (cg->getSupportsImplicitNullChecks() && !needExplicitCheck)
      {
      TR::Instruction *faultingInstruction = cg->getImplicitExceptionPoint();
      if (faultingInstruction)
         {
         faultingInstruction->setNeedsGCMap(0x0000FFFF);
         faultingInstruction->setThrowsImplicitNullPointerException();
         cg->setCanExceptByTrap(true);

         TR_Debug * debugObj = cg->getDebug();
         if (debugObj)
            {
            debugObj->addInstructionComment(faultingInstruction, "Throws Implicit Null Pointer Exception");
            }
         }
      }

   if (comp->useCompressedPointers() &&
         reference->getOpCodeValue() == TR::l2a)
      {
      reference->setIsNonNull(true);
      TR::Node *n = NULL;
      n = reference->getFirstChild();
      TR::ILOpCodes loadOp = comp->il.opCodeForIndirectLoad(TR::Int32);
      while (n->getOpCodeValue() != loadOp)
         {
         n->setIsNonZero(true);
         n = n->getFirstChild();
         }
      n->setIsNonZero(true);
      }

   reference->setIsNonNull(true);
   return NULL;
   }

/**
 * Null check a pointer.  child 1 is indirect reference. Symbolref
 * indicates failure action/destination
 */
TR::Register *
OMR::Z::TreeEvaluator::NULLCHKEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::evaluateNULLCHKWithPossibleResolve(node, false, cg);
   }

/**
 * Zero check an int.
 */
TR::Register *
OMR::Z::TreeEvaluator::ZEROCHKEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   // Labels for OOL path with helper and the merge point back from OOL path
   TR::LabelSymbol *slowPathOOLLabel = generateLabelSymbol(cg);;
   TR::LabelSymbol *doneLabel  = generateLabelSymbol(cg);;

   // Need to check the first child of ZEROCHK to determine whether we need to
   // make the helper call.
   // Note: for ZEROCHK the branch condition is reversed.
   TR::Node *valueToCheck = node->getFirstChild();
   if (valueToCheck->getOpCode().getOpCodeValue() == TR::acmpeq)
      {
      TR::InstOpCode::S390BranchCondition opBranchCond = generateS390CompareOps(valueToCheck, cg, TR::InstOpCode::COND_BNE, TR::InstOpCode::COND_BNE);
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, opBranchCond, node, slowPathOOLLabel);
      }
   else
      {
      TR::Register *valueReg = cg->evaluate(valueToCheck);
      generateRIInstruction(cg, TR::InstOpCode::CHI, node, valueReg, 0);
      generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BE, node, slowPathOOLLabel);
      }

   // Outlined instructions for check failure
   TR::Instruction * cursor = generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, slowPathOOLLabel);

   // For helper call, we need to pass the 2nd and any successive children as
   // parameters.  Temporarily hide the first child so it doesn't appear in the
   // outlined call.
   node->rotateChildren(node->getNumChildren()-1, 0);
   node->setNumChildren(node->getNumChildren()-1);

   TR_S390OutOfLineCodeSection *outlinedHelperCall = new (cg->trHeapMemory()) TR_S390OutOfLineCodeSection(node, TR::call, NULL, slowPathOOLLabel, doneLabel, cg);
   cg->getS390OutOfLineCodeSectionList().push_front(outlinedHelperCall);
   outlinedHelperCall->generateS390OutOfLineCodeSectionDispatch();

   // Restore the node and its original first child
   node->setNumChildren(node->getNumChildren()+1);
   node->rotateChildren(0, node->getNumChildren()-1);

   // Children other than the first are only for the outlined path; we don't need them here
   for (int32_t i = 1; i < node->getNumChildren(); i++)
      cg->recursivelyDecReferenceCount(node->getChild(i));

   // Merge Point back from OOL to mainline.
   generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, doneLabel);

   // We only need to decrement the first child, as 2nd and successive children
   cg->decReferenceCount(node->getFirstChild());

   return NULL;

   }

/**
 * resolveAndNULLCHKEvaluator - Resolve check a static, field or method and Null check
 *    the underlying pointer.  child 1 is reference to be resolved. Symbolref indicates
 *    failure action/destination
 */
TR::Register *
OMR::Z::TreeEvaluator::resolveAndNULLCHKEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   return TR::TreeEvaluator::evaluateNULLCHKWithPossibleResolve(node, true, cg);
   }


void OMR::Z::TreeEvaluator::addToRegDep(TR::RegisterDependencyConditions * daaDeps, TR::Register * reg, bool mightUseRegPair)
   {
      if (!reg)
         return;

      //if it is GPR5 then we don't need to create deps
      if (reg->getRealRegister())
         {
         if (((TR::RealRegister *)reg->getRealRegister())->getRegisterNumber() == TR::RealRegister::GPR5)
            return;
         }

      if (mightUseRegPair)
         {
            //if it is register pair:
            if (reg->getRegisterPair())
               {
               TR::RegisterPair * regPair = reg->getRegisterPair();
               daaDeps->addPostCondition(regPair, TR::RealRegister::EvenOddPair);
               daaDeps->addPostCondition(regPair->getHighOrder(), TR::RealRegister::LegalEvenOfPair);
               daaDeps->addPostCondition(regPair->getLowOrder(), TR::RealRegister::LegalOddOfPair);
               return;
               }
         }

         daaDeps->addPostConditionIfNotAlreadyInserted(reg, TR::RealRegister::AssignAny);
   }

void OMR::Z::TreeEvaluator::createDAACondDeps(TR::Node * node, TR::RegisterDependencyConditions * daaDeps,
                                              TR::Instruction * daaInstr, TR::CodeGenerator * cg)
{
   switch (daaInstr->getOpCodeValue())
   {
      case TR::InstOpCode::AP:
      case TR::InstOpCode::SP:
      case TR::InstOpCode::MP:
      case TR::InstOpCode::DP:
      case TR::InstOpCode::SRP:
      case TR::InstOpCode::ZAP:
      case TR::InstOpCode::EX:
      case TR::InstOpCode::CVB:
      case TR::InstOpCode::CVBG:
      case TR::InstOpCode::CVBY:
      case TR::InstOpCode::CP:
      // Vector packed decimals
      case TR::InstOpCode::VAP:
      case TR::InstOpCode::VCP:
      case TR::InstOpCode::VCVB:
      case TR::InstOpCode::VCVBG:
      case TR::InstOpCode::VCVD:
      case TR::InstOpCode::VCVDG:
      case TR::InstOpCode::VDP:
      case TR::InstOpCode::VMP:
      case TR::InstOpCode::VPKZ:
      case TR::InstOpCode::VPSOP:
      case TR::InstOpCode::VRP:
      case TR::InstOpCode::VSRP:
      case TR::InstOpCode::VSP:
         break;
      default:
         TR_ASSERT(0, "Target Instruction is not used by DAA.\n");
         break;
   }

   switch (daaInstr->getKind())
      {
      case TR::Instruction::IsRX: //CVB, CVD
         {
         TR::S390RXInstruction * instr = static_cast<TR::S390RXInstruction *>(daaInstr);
         TR::MemoryReference * mr = instr->getMemoryReference();

         TR::TreeEvaluator::addToRegDep(daaDeps, instr->getRegisterOperand(1), false);
         if (mr)
            {
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr->getBaseRegister()), false);
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr->getIndexRegister()), false);
            }
         break;
         }
      case TR::Instruction::IsRXY: //CVBY, CVBG
         {
         TR::S390RXYInstruction * instr = static_cast<TR::S390RXYInstruction *>(daaInstr);
         TR::MemoryReference * mr = instr->getMemoryReference();

         TR::TreeEvaluator::addToRegDep(daaDeps, instr->getRegisterOperand(1), true);
         if (mr)
            {
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr->getBaseRegister()), true);
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr->getIndexRegister()), true);
            }
            break;
         }
      case TR::Instruction::IsSS2: //ZAP, Arithmetics
         {
         TR::S390SS2Instruction * instr = static_cast<TR::S390SS2Instruction *>(daaInstr);
         TR::MemoryReference * mr1 = instr->getMemoryReference ();
         TR::MemoryReference * mr2 = instr->getMemoryReference2();

         if (mr1)
            {
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr1->getBaseRegister()), false);
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr1->getIndexRegister()), false);
            }

         if (mr2)
            {
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr2->getBaseRegister()), false);
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr2->getIndexRegister()), false);
            }
         break;
         }
      case TR::Instruction::IsVRIf: // VAP,VDP, VMP, VSP, VRP, [VMSP, VSDP unused]
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(2), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(3), false);
         break;
         }
      case TR::Instruction::IsVRIg: // VPSOP, VSRP
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(2), false);
         break;
         }
      case TR::Instruction::IsVRIi: // VCVD, VCVDG
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(2), false);
         break;
         }
      case TR::Instruction::IsVRRg: // VTP
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         break;
         }
      case TR::Instruction::IsVRRh: // VCP
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(2), false);
         break;
         }
      case TR::Instruction::IsVRRi: // VCVB, VCVBG
         {
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);
         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(2), false);
         break;
         }
      case TR::Instruction::IsVSI:  // VPKZ, VUPKZ
         {
         TR::S390VSIInstruction * instr = static_cast<TR::S390VSIInstruction *>(daaInstr);
         TR::MemoryReference * mr1 = instr->getMemoryReference();

         TR::TreeEvaluator::addToRegDep(daaDeps, daaInstr->getRegisterOperand(1), false);

         if (mr1)
            {
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr1->getBaseRegister()), false);
            TR::TreeEvaluator::addToRegDep(daaDeps, static_cast<TR::Register *>(mr1->getIndexRegister()), false);
            }
         break;
         }
      default:
         TR_ASSERT(0, "Inconsistent DAA Instruction format.\n");
         break;
      }
   }

TR::Node* OMR::Z::TreeEvaluator::DAAAddressPointer(TR::Node* callNode, TR::CodeGenerator* cg)
   {
   TR::Node* base = callNode->getChild(0);
   TR::Node* index = callNode->getChild(1);

   TR::Node * pdBufAddressNode = NULL;
   TR::Node * pdBufPositionNode = NULL;

#ifdef J9_PROJECT_SPECIFIC
   if (callNode->getSymbol()->getResolvedMethodSymbol())
      {
      if (callNode->getSymbol()->getResolvedMethodSymbol()->getRecognizedMethod())
         {
         if ((callNode->getSymbol()->getResolvedMethodSymbol()->getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToInteger_ByteBuffer_)
               || (callNode->getSymbol()->getResolvedMethodSymbol()->getRecognizedMethod() == TR::com_ibm_dataaccess_DecimalData_convertPackedDecimalToLong_ByteBuffer_))
            {
            pdBufAddressNode = callNode->getChild(4);
            pdBufPositionNode = callNode->getChild(6);
            return TR::Node::create(TR::l2a, 1, TR::Node::create(TR::ladd, 2, pdBufAddressNode, TR::Node::create(TR::i2l, 1, TR::Node::create(TR::iadd, 2, pdBufPositionNode, index))));
            }
         }
      }
#endif

   if (TR::Compiler->target.is64Bit())
      {
      TR::Node* addressBase   = base;
      TR::Node* addressIndex  = TR::Node::create(TR::i2l,    1, index);
      TR::Node* addressHeader = TR::Node::create(TR::lconst, 0, 0);
      TR::Node* addressOffset = TR::Node::create(TR::aladd,  2, addressHeader, addressIndex);

      // Update the address header size
      addressHeader->setLongInt(TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

      // Compute the final address as base + header + index
      return TR::Node::create(TR::aladd, 2, addressBase, addressOffset);
      }
   else
      {
      TR::Node* addressBase   = base;
      TR::Node* addressIndex  = index;
      TR::Node* addressHeader = TR::Node::create(TR::iconst, 0, 0);
      TR::Node* addressOffset = TR::Node::create(TR::aiadd,  2, addressHeader, addressIndex);

      // Update the address header size
      addressHeader->setInt(TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

      // Compute the final address as base + header + index
      return TR::Node::create(TR::aiadd, 2, addressBase, addressOffset);
      }
   }

TR::Register *
OMR::Z::TreeEvaluator::butestEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::TreeEvaluator::commonButestEvaluator(node, cg);
   TR::Register *ccReg = getConditionCode(node, cg);

   node->setRegister(ccReg);
   return ccReg;
   }

void
OMR::Z::TreeEvaluator::commonButestEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
   TR::Node *firstChild  = node->getFirstChild();
   TR::Node *secondChild = node->getSecondChild();
   TR_ASSERT(secondChild->getOpCode().isLoadConst() && secondChild->getType().isInt8(),
         "testUnderMask expects secondChild %s (%p) to be a 8 bit constant\n",
         secondChild->getOpCode().getName(),secondChild);

   uint8_t tmMask = secondChild->getUnsignedByte();
   if (firstChild->isSingleRefUnevaluated())
      {
      TR::MemoryReference * mr = generateS390MemoryReference(firstChild, cg);
      generateSIInstruction(cg, TR::InstOpCode::TM, node, mr, tmMask);
      }
   else
      {
      TR::Register *reg = cg->evaluate(firstChild);
      generateRIInstruction(cg, TR::InstOpCode::TMLL, node, reg, tmMask);
      }
   cg->decReferenceCount(firstChild);
   cg->decReferenceCount(secondChild);
   }

TR::Register *
OMR::Z::TreeEvaluator::inlineIfButestEvaluator(TR::Node * node, TR::CodeGenerator * cg, TR::Node * cmpOpNode, TR::Node * cmpValNode)
   {
   TR_ASSERT(cmpValNode->getOpCode().isLoadConst() && cmpValNode->getType().isInt32(), "Expecting constant second child for if node, found node %s (%p)\n",
      cmpValNode->getOpCode().getName(),cmpValNode);

   TR::RegisterDependencyConditions *deps = getGLRegDepsDependenciesFromIfNode(cg, cmpOpNode);

   TR::TreeEvaluator::commonButestEvaluator(node, cg);

   TR::InstOpCode::S390BranchCondition cond = getButestBranchCondition(cmpOpNode->getOpCodeValue(), cmpValNode->getInt());
   TR::Instruction * instr = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, cond, cmpOpNode, cmpOpNode->getBranchDestination()->getNode()->getLabel(), deps);

   cg->decReferenceCount(node);
   cg->decReferenceCount(cmpValNode);

   return NULL;
   }

TR::Register *
OMR::Z::TreeEvaluator::inlineIfBifEvaluator(TR::Node * ifNode, TR::CodeGenerator * cg, TR::Node * cmpOpNode, TR::Node * cmpValNode)
   {
   TR_ASSERT(cmpValNode->getOpCode().isLoadConst() && cmpValNode->getType().isInt32(), "Expecting constant second child for if node, found node %s (%p)\n",
      cmpValNode->getOpCode().getName(),cmpValNode);

   cg->decReferenceCount(cmpOpNode);
   if (ifNode->getFirstChild()->getOpCodeValue() == TR::bu2i)
      {
      // This occurs for ZInstlikecalls where we have:
      // ifnode
      //    bu2i
      //      cmpOpNode
      //    cmpValNode
      // We have previously verified that cmpOpNode isSingleRefUnevaluated(), so therefore the bu2i must also be isSingleRefEvaluated()
      // and we should dec ref count here
      cg->decReferenceCount(ifNode->getFirstChild());
      }

   return NULL;
   }

/**
 * Ternary Evaluator - evaluates all types of ternary opcodes
 */
TR::InstOpCode::S390BranchCondition OMR::Z::TreeEvaluator::getBranchConditionFromCompareOpCode(TR::ILOpCodes opCode)
   {
   switch (opCode)
      {
      case TR::icmpeq:
      case TR::iucmpeq:
      case TR::acmpeq:
      case TR::lcmpeq:
      case TR::lucmpeq:
         {
         return TR::InstOpCode::COND_BE;
         }
         break;
      case TR::icmpne:
      case TR::iucmpne:
      case TR::acmpne:
      case TR::lcmpne:
      case TR::lucmpne:
         {
         return TR::InstOpCode::COND_BNE;
         }
         break;
      case TR::icmplt:
      case TR::iucmplt:
      case TR::lcmplt:
      case TR::lucmplt:
         {
         return TR::InstOpCode::COND_BL;
         }
         break;
      case TR::icmple:
      case TR::iucmple:
      case TR::lcmple:
      case TR::lucmple:
         {
         return TR::InstOpCode::COND_BNH;
         }
         break;
      case TR::icmpgt:
      case TR::iucmpgt:
      case TR::lcmpgt:
      case TR::lucmpgt:
         {
         return TR::InstOpCode::COND_BH;
         }
         break;
      case TR::icmpge:
      case TR::iucmpge:
      case TR::lcmpge:
      case TR::lucmpge:
         {
         return TR::InstOpCode::COND_BNL;
         }
         break;
      default:
         {
         TR_ASSERT_FATAL(0, "Unsupported compare type under ternary");
         return TR::InstOpCode::COND_BE;      //not a valid return value.  We should never ever get here.
         }
         break;
      }
   }

/**
 * since load on condition loads the conditional value when one of the following is true,
 * we must do the opposite to mimic branch behaviour, which would branch around the load when one of the following is true.
 */
TR::InstOpCode::S390BranchCondition OMR::Z::TreeEvaluator::mapBranchConditionToLOCRCondition(TR::InstOpCode::S390BranchCondition incomingBc)
   {
   switch (incomingBc)
      {
      case TR::InstOpCode::COND_BE:
         {
         return TR::InstOpCode::COND_BNE;
         }
         break;
      case TR::InstOpCode::COND_BNE:
         {
         return TR::InstOpCode::COND_BE;
         }
         break;
      case TR::InstOpCode::COND_BL:
         {
         return TR::InstOpCode::COND_BNL;
         }
         break;
      case TR::InstOpCode::COND_BNH:
         {
         return TR::InstOpCode::COND_BH;
         }
         break;
      case TR::InstOpCode::COND_BH:
         {
         return TR::InstOpCode::COND_BNH;
         }
         break;
      case TR::InstOpCode::COND_BNL:
         {
         return TR::InstOpCode::COND_BL;
         }
         break;
      default:
         {
        TR_ASSERT_FATAL(0, "Unsupported compare mapping");
        return TR::InstOpCode::COND_BE;      //not a valid return value.  We should never ever get here.
        }
        break;
      }
   }

TR::InstOpCode::Mnemonic OMR::Z::TreeEvaluator::getCompareOpFromNode(TR::CodeGenerator *cg, TR::Node *node)
   {
   TR_ASSERT(node->getOpCode().isBooleanCompare() && node->getNumChildren() > 0, "node %p is not a compare opcode, or compare opcode has 0 children!\n",node);
   TR_ASSERT(node->getFirstChild()->getType().isIntegral() || node->getFirstChild()->getType().isAddress(), "node %p is neither an int nor an address!\n",node->getFirstChild());

   if (node->getFirstChild()->getSize() == 8)
      {
      if (node->getOpCode().isUnsigned())
         {
         return TR::InstOpCode::CLGR;
         }
      else
         {
         return TR::InstOpCode::CGR;
         }
      }
   else if (node->getFirstChild()->getSize() == 4)
      {
      if (node->getOpCode().isUnsigned())
         {
         return TR::InstOpCode::CLR;
         }
      else
         {
         return TR::InstOpCode::CR;
         }
      }

   TR_ASSERT(0, "Unhandled compare node type\n");
   return TR::InstOpCode::CR;

   }

int32_t OMR::Z::TreeEvaluator::countReferencesInTree(TR::Node *treeNode, TR::Node *node)
   {
   if (treeNode->getVisitCount() >= TR::comp()->getVisitCount())
      return 0;

   treeNode->setVisitCount(TR::comp()->getVisitCount());

   if (treeNode->getRegister())
      return 0;

   if (treeNode == node)
      return 1;

   int32_t sum=0;
   for(int32_t i = 0 ; i < treeNode->getNumChildren() ; i++)
      {
      sum += TR::TreeEvaluator::countReferencesInTree(treeNode->getChild(i), node);
      }
   return sum;
   }

bool
OMR::Z::TreeEvaluator::treeContainsAllOtherUsesForNode(TR::Node *treeNode, TR::Node *node)
   {
   static const char *x = feGetEnv("disableTernaryEvaluatorImprovement");
   if (x)
      {
      return false;
      }

   //traceMsg(cg->comp(), "node refcount = %d\n",node->getReferenceCount());

   int32_t numberOfInstancesToFind = node->getReferenceCount()-1;
   if (numberOfInstancesToFind == 0)
      return true;

   int32_t instancesFound = TR::TreeEvaluator::countReferencesInTree(treeNode, node);

   //traceMsg(cg->comp(), "numberOfInstancesToFind = %d instancesFound = %d\n",numberOfInstancesToFind,instancesFound);

   if (numberOfInstancesToFind == instancesFound)
      return true;
   else
      return false;
   }

TR::Register *
OMR::Z::TreeEvaluator::ternaryEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Compilation *comp = cg->comp();

   comp->incVisitCount();       // need this for treeContainsAllOtherUsesForNode
   TR::Node *condition  = node->getFirstChild();
   TR::Node *trueVal    = node->getSecondChild();
   TR::Node *falseVal   = node->getThirdChild();

   if (comp->getOption(TR_TraceCG))
      traceMsg(comp, "Starting evaluation of ternary node %p condition %p (in reg %p) trueVal %p (in reg %p) falseVal %p (in reg %p)\n",node,condition,condition->getRegister(), trueVal, trueVal->getRegister(), falseVal, falseVal->getRegister());

  TR::Register *trueReg = 0;
  if(TR::TreeEvaluator::treeContainsAllOtherUsesForNode(condition,trueVal) &&
     (!trueVal->isUnneededConversion() || TR::TreeEvaluator::treeContainsAllOtherUsesForNode(condition, trueVal->getFirstChild())))
     {
     if (comp->getOption(TR_TraceCG))
        traceMsg(comp, "Calling evaluate (instead of clobber evaluate for node %p because all other uses are in the compare tree)\n",trueVal);
     trueReg = cg->evaluate(trueVal);
     }
  else
     {
     trueReg =  cg->gprClobberEvaluate(trueVal); //cg->evaluate(trueVal);
     }

   TR::Register *falseReg = cg->evaluate(falseVal); //cg->gprClobberEvaluate(falseVal);

   if (comp->getOption(TR_TraceCG))
      traceMsg(comp, "Done evaluating child %p in reg %p and %p in reg %p\n",trueVal,trueReg,falseVal,falseReg);

   // I don't want to evaluate the first child as icmp evaluator places value in an integer, which we can short circuit.
   if (condition->getOpCode().isBooleanCompare() &&
       (condition->getFirstChild()->getType().isIntegral() ||
        condition->getFirstChild()->getType().isAddress()))
      {
      if (comp->getOption(TR_TraceCG))
         traceMsg(comp, "First Child %p is a compare\n",condition);

      TR::InstOpCode::Mnemonic compareOp = TR::TreeEvaluator::getCompareOpFromNode(cg, condition);

      TR::Node *firstChild = condition->getFirstChild();
      TR::Node *secondChild = condition->getSecondChild();

      TR::Register *firstReg = cg->evaluate(firstChild);
      TR::Register *secondReg = cg->evaluate(secondChild);

      auto bc = TR::TreeEvaluator::getBranchConditionFromCompareOpCode(condition->getOpCodeValue());

      if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z15))
         {
         generateRRInstruction(cg, compareOp, node, firstReg, secondReg);

         auto mnemonic = trueVal->getOpCode().is8Byte() ? TR::InstOpCode::SELGR : TR::InstOpCode::SELR;
         generateRRFInstruction(cg, mnemonic, node, trueReg, trueReg, falseReg, getMaskForBranchCondition(TR::TreeEvaluator::mapBranchConditionToLOCRCondition(bc)));
         }
      else if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z196))
         {
         generateRRInstruction(cg, compareOp, node, firstReg, secondReg);

         auto mnemonic = trueVal->getOpCode().is8Byte() ? TR::InstOpCode::LOCGR: TR::InstOpCode::LOCR;
         generateRRFInstruction(cg, mnemonic, node, trueReg, falseReg, getMaskForBranchCondition(TR::TreeEvaluator::mapBranchConditionToLOCRCondition(bc)), true);
         }
      else
         {
         TR::LabelSymbol * cFlowRegionStart = generateLabelSymbol( cg);
         TR::LabelSymbol * cFlowRegionEnd = generateLabelSymbol( cg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
         cFlowRegionStart->setStartInternalControlFlow();
         generateS390CompareAndBranchInstruction(cg, compareOp, node, firstReg, secondReg, bc, cFlowRegionEnd, false);

         TR::RegisterDependencyConditions* conditions = conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 4, cg);

         conditions->addPostCondition(firstReg, TR::RealRegister::AssignAny);

         if (secondReg != firstReg)
            {
            conditions->addPostCondition(secondReg, TR::RealRegister::AssignAny);
            }

         if (falseReg != firstReg && falseReg != secondReg)
            {
            conditions->addPostCondition(falseReg, TR::RealRegister::AssignAny);
            }

         if (trueReg != falseReg && trueReg != firstReg && trueReg != secondReg)
            {
            conditions->addPostCondition(trueReg, TR::RealRegister::AssignAny);
            }

         generateRRInstruction(cg, trueVal->getOpCode().is8Byte() ? TR::InstOpCode::LGR : TR::InstOpCode::LR, node, trueReg, falseReg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionEnd, conditions);
         cFlowRegionEnd->setEndInternalControlFlow();

         if (comp->getOption(TR_TraceCG))
            {
            traceMsg(comp, "firstReg = %p secondReg = %p trueReg = %p falseReg = %p trueReg->is64BitReg() = %d falseReg->is64BitReg() = %d isLongCompare = %d\n", firstReg, secondReg, trueReg, falseReg, trueReg->is64BitReg(), falseReg->is64BitReg(), condition->getOpCode().isLongCompare());
            }
         }

      cg->decReferenceCount(firstChild);
      cg->decReferenceCount(secondChild);
      }
   else
      {
      if (comp->getOption(TR_TraceCG))
         traceMsg(comp,"evaluating condition %p\n",condition);

      TR::Register *condReg = cg->evaluate(condition);

      if (comp->getOption(TR_TraceCG))
         traceMsg(comp, "emitting a compare with 0 instruction\n");

      TR::Instruction *compareInst = generateRILInstruction(cg,condition->getOpCode().isLongCompare() ? TR::InstOpCode::CGFI : TR::InstOpCode::CFI,condition,condition->getRegister(), 0);

      if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z15))
         {
         auto mnemonic = trueVal->getOpCode().is8Byte() ? TR::InstOpCode::SELGR : TR::InstOpCode::SELR;
         generateRRFInstruction(cg, mnemonic, node, trueReg, trueReg, falseReg, getMaskForBranchCondition(TR::InstOpCode::COND_BER));
         }
      else if (TR::Compiler->target.cpu.getSupportsArch(TR::CPU::z196))
         {
         auto mnemonic = trueVal->getOpCode().is8Byte() ? TR::InstOpCode::LOCGR: TR::InstOpCode::LOCR;
         generateRRFInstruction(cg, mnemonic, node, trueReg, falseReg, getMaskForBranchCondition(TR::InstOpCode::COND_BER), true);
         }
      else
         {
         TR::LabelSymbol * cFlowRegionStart = generateLabelSymbol( cg);
         TR::LabelSymbol * cFlowRegionEnd = generateLabelSymbol( cg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionStart);
         cFlowRegionStart->setStartInternalControlFlow();
         TR::Instruction *branchInst = generateS390BranchInstruction(cg, TR::InstOpCode::BRC, TR::InstOpCode::COND_BNE, node, cFlowRegionEnd);

         TR::RegisterDependencyConditions* conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 3, cg);

         conditions->addPostCondition(trueReg, TR::RealRegister::AssignAny);

         if (falseReg != trueReg)
            {
            conditions->addPostCondition(falseReg, TR::RealRegister::AssignAny);
            }

         if (condReg != trueReg && condReg != falseReg)
            {
            conditions->addPostCondition(condReg, TR::RealRegister::AssignAny);
            }

         generateRRInstruction(cg, trueVal->getOpCode().is8Byte() ? TR::InstOpCode::LGR : TR::InstOpCode::LR, node, trueReg, falseReg);

         generateS390LabelInstruction(cg, TR::InstOpCode::LABEL, node, cFlowRegionEnd, conditions);
         cFlowRegionEnd->setEndInternalControlFlow();
         }
      }

   if (!node->isNotCollected())
      trueReg->setContainsCollectedReference();

   if (comp->getOption(TR_TraceCG))
      traceMsg(comp, "Setting node %p register to %p\n",node,trueReg);
   node->setRegister(trueReg);

   cg->decReferenceCount(condition);
   cg->decReferenceCount(trueVal);
   cg->decReferenceCount(falseVal);

   return trueReg;
}

// Take advantage of VRF/FPR overlap and use vector compare and select instructions to implement dternary
TR::Register *
OMR::Z::TreeEvaluator::dternaryEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *conditionNode = node->getFirstChild();
   TR::Node *trueValueNode = node->getSecondChild();
   TR::Node *falseValueNode = node->getThirdChild();

   TR::Register *trueValueReg = cg->evaluate(trueValueNode);
   TR::Register *falseValueReg = cg->evaluate(falseValueNode);

   TR::Register *vectorSelReg = cg->allocateRegister(TR_VRF);
   TR::Register *returnReg = cg->allocateRegister(TR_FPR);

   if (conditionNode->getOpCode().isCompareDouble())
      {
      TR::Node *firstCmpValue = conditionNode->getFirstChild();
      TR::Node *secondCmpValue = conditionNode->getSecondChild();

      TR::Register *firstCmpReg = cg->evaluate(firstCmpValue);
      TR::Register *secondCmpReg = cg->evaluate(secondCmpValue);

      TR::InstOpCode::Mnemonic compareOp = TR::InstOpCode::BAD;
      TR::ILOpCodes ifOp = conditionNode->getOpCodeValue();

      switch (ifOp)
         {
         case TR::dcmpne:
         case TR::dcmpneu:
         case TR::dcmpeq:
         case TR::dcmpequ:
            compareOp = TR::InstOpCode::VFCE;
            break;
         case TR::dcmplt:
         case TR::dcmpgt:
         case TR::dcmpleu:
         case TR::dcmpgeu:
            compareOp = TR::InstOpCode::VFCH;
            break;
         case TR::dcmpltu:
         case TR::dcmpgtu:
         case TR::dcmple:
         case TR::dcmpge:
            compareOp = TR::InstOpCode::VFCHE;
            break;
         }

      // To handle unordered compares, we switch the compare operation, since the vector compare instructions return false for unordered.
      // For example, if the original compare is dcmpgeu:
      //    (x >= y [true if unordered]) ? a : b
      //    switch to (x < y) ? b : a;
      // So that only if x < y, do we select the "false value"

      if (ifOp == TR::dcmpgeu || ifOp == TR::dcmpgtu || ifOp == TR::dcmplt || ifOp == TR::dcmple)
         generateVRRcInstruction(cg, compareOp, node, vectorSelReg, secondCmpReg, firstCmpReg, 1, 0, 3);
      else
         generateVRRcInstruction(cg, compareOp, node, vectorSelReg, firstCmpReg, secondCmpReg, 1, 0, 3);

      // The above operation returns 0's for not equals, so we must swap the true and false values to select the correct "true value" for cmpne operations
      // Swap the values for unordered comparisons also (see above)
      if (ifOp == TR::dcmpne || conditionNode->getOpCode().isCompareTrueIfUnordered())
         generateVRReInstruction(cg, TR::InstOpCode::VSEL, node, returnReg, falseValueReg, trueValueReg, vectorSelReg);
      else
         generateVRReInstruction(cg, TR::InstOpCode::VSEL, node, returnReg, trueValueReg, falseValueReg, vectorSelReg);

      cg->stopUsingRegister(firstCmpReg);
      cg->stopUsingRegister(secondCmpReg);
      }
   else
      {
      TR::Register *conditionReg = cg->evaluate(conditionNode);
      TR::Register *tempReg = cg->allocateRegister(TR_FPR);
      TR::Register *vzeroReg = cg->allocateRegister(TR_VRF);

      // convert to floating point
      generateRRInstruction(cg, TR::InstOpCode::LDGR, node, tempReg, conditionReg);

      // generate compare with zero
      generateVRIaInstruction(cg, TR::InstOpCode::VGBM, node, vzeroReg, 0, 0);
      generateVRRcInstruction(cg, TR::InstOpCode::VFCE, node, vectorSelReg, tempReg, vzeroReg, 1, 0, 3);

      // generate select - if condition == 0, vectorSelReg will contain all 1s, so false and true are swapped
      generateVRReInstruction(cg, TR::InstOpCode::VSEL, node, returnReg, falseValueReg, trueValueReg, vectorSelReg);

      cg->stopUsingRegister(tempReg);
      cg->stopUsingRegister(vzeroReg);
      cg->stopUsingRegister(conditionReg);
      }

   node->setRegister(returnReg);

   cg->stopUsingRegister(trueValueReg);
   cg->stopUsingRegister(falseValueReg);
   cg->stopUsingRegister(vectorSelReg);

   cg->decReferenceCount(trueValueNode);
   cg->decReferenceCount(falseValueNode);
   cg->recursivelyDecReferenceCount(conditionNode);

   return returnReg;
   }

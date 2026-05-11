/*******************************************************************************
 * Copyright IBM Corp. and others 2019
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
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
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#include "codegen/RVInstruction.hpp"
#include "codegen/CodeGenerator.hpp"
#include "codegen/GenerateInstructions.hpp"
#include "codegen/Linkage.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/LabelSymbol.hpp"
#include "ras/Logger.hpp"

TR::Register *genericReturnEvaluator(TR::Node *node, TR::RealRegister::RegNum rnum, TR_RegisterKinds rk,
    TR_ReturnInfo i, TR::CodeGenerator *cg)
{
    TR::Node *firstChild = node->getFirstChild();
    TR::Register *returnRegister = cg->evaluate(firstChild);

    TR::RegisterDependencyConditions *deps = RegDeps(1, 0, cg);
    deps->addPreCondition(returnRegister, rnum);
    Inst_ADMIN(OP::retn, node, deps, cg);

    cg->comp()->setReturnInfo(i);
    cg->decReferenceCount(firstChild);

    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ireturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return genericReturnEvaluator(node, cg->getProperties().getIntegerReturnRegister(), TR_GPR, TR_IntReturn, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lreturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return genericReturnEvaluator(node, cg->getProperties().getLongReturnRegister(), TR_GPR, TR_LongReturn, cg);
}

TR::Register *OMR::RV::TreeEvaluator::areturnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return genericReturnEvaluator(node, cg->getProperties().getLongReturnRegister(), TR_GPR, TR_ObjectReturn, cg);
}

// void return
TR::Register *OMR::RV::TreeEvaluator::returnEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    Inst_ADMIN(OP::retn, node, cg);
    cg->comp()->setReturnInfo(TR_VoidReturn);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::gotoEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR::LabelSymbol *gotoLabel = node->getBranchDestination()->getNode()->getLabel();
    TR::RealRegister *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
    if (node->getNumChildren() > 0) {
        TR::Node *child = node->getFirstChild();
        cg->evaluate(child);
        Inst_JTYPE(OP::_jal, node, zero, gotoLabel, RegDeps(cg, child, 0), cg);
        cg->decReferenceCount(child);
    } else {
        Inst_JTYPE(OP::_jal, node, zero, gotoLabel, cg);
    }
    return NULL;
}

#ifdef J9_PROJECT_SPECIFIC
static bool virtualGuardHelper(TR::Node *node, TR::CodeGenerator *cg)
{
    if (!cg->willGenerateNOPForVirtualGuard(node)) {
        return false;
    }

    TR::Compilation *comp = cg->comp();
    TR_VirtualGuard *virtualGuard = comp->findVirtualGuardInfo(node);

    TR_VirtualGuardSite *site = NULL;

    if (cg->comp()->compileRelocatableCode()) {
        TR_UNIMPLEMENTED();
    } else if (!node->isSideEffectGuard()) {
        site = virtualGuard->addNOPSite();
    } else
        site = comp->addSideEffectNOPSite();

    TR::RegisterDependencyConditions *deps;
    if (node->getNumChildren() == 3) {
        TR::Node *third = node->getChild(2);
        cg->evaluate(third);
        deps = RegDeps(cg, third, 0);
    } else
        deps = RegDeps(0, 0, cg);

    if (virtualGuard->shouldGenerateChildrenCode())
        cg->evaluateChildrenWithMultipleRefCount(node);

    TR::LabelSymbol *label = node->getBranchDestination()->getNode()->getLabel();
    Inst_VGNOP(node, site, deps, label, cg);
    cg->recursivelyDecReferenceCount(node->getFirstChild());
    cg->recursivelyDecReferenceCount(node->getSecondChild());

    return true;
}
#endif // J9_PROJECT_SPECIFIC

static TR::Instruction *ificmpHelper(TR::InstOpCode::Mnemonic op, TR::Node *node, bool reverse, TR::CodeGenerator *cg)
{
#ifdef J9_PROJECT_SPECIFIC
    if (virtualGuardHelper(node, cg))
        return NULL;
#endif

    TR::Node *firstChild = node->getFirstChild();
    TR::Node *secondChild = node->getSecondChild();
    TR::Node *thirdChild = NULL;

    TR::Register *src1Reg = cg->evaluate(firstChild);
    TR::Register *src2Reg = cg->evaluate(secondChild);

    TR::LabelSymbol *dstLabel = node->getBranchDestination()->getNode()->getLabel();
    TR::Instruction *result;
    if (node->getNumChildren() == 3) {
        TR_UNIMPLEMENTED();
#if 0
      thirdChild = node->getChild(2);
      TR_ASSERT(thirdChild->getOpCodeValue() == TR::GlRegDeps, "The third child of a compare must be a TR::GlRegDeps");

      cg->evaluate(thirdChild);

      TR::RegisterDependencyConditions *deps = RegDeps(cg, thirdChild, 0);
      result = generateConditionalBranchInstruction(cg, TR::InstOpCode::b_cond, node, dstLabel, cc, deps);
#endif
    } else {
        if (reverse)
            result = Inst_BTYPE(op, node, dstLabel, src2Reg, src1Reg, cg);
        else
            result = Inst_BTYPE(op, node, dstLabel, src1Reg, src2Reg, cg);
    }

    firstChild->decReferenceCount();
    secondChild->decReferenceCount();
    if (thirdChild) {
        thirdChild->decReferenceCount();
    }
    return result;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_beq, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bne, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_blt, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bge, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_blt, node, true, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ificmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bge, node, true, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ifiucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bltu, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ifiucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bgeu, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ifiucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bltu, node, true, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::ifiucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bgeu, node, true, cg);
    return NULL;
}

// also handles ifacmpeq
TR::Register *OMR::RV::TreeEvaluator::iflcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_beq, node, false, cg);
    return NULL;
}

// also handles ifacmpne
TR::Register *OMR::RV::TreeEvaluator::iflcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bne, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::iflcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_blt, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::iflcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bge, node, false, cg);
    return NULL;
}

// also handles ifacmplt
TR::Register *OMR::RV::TreeEvaluator::iflucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bltu, node, false, cg);
    return NULL;
}

// also handles ifacmpge
TR::Register *OMR::RV::TreeEvaluator::iflucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bgeu, node, false, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::iflcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_blt, node, true, cg);
    return NULL;
}

TR::Register *OMR::RV::TreeEvaluator::iflcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bge, node, true, cg);
    return NULL;
}

// also handles ifacmpgt
TR::Register *OMR::RV::TreeEvaluator::iflucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bltu, node, true, cg);
    return NULL;
}

// also handles ifacmple
TR::Register *OMR::RV::TreeEvaluator::iflucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    ificmpHelper(OP::_bgeu, node, true, cg);
    return NULL;
}

static TR::Register *icmpHelper(TR::InstOpCode::Mnemonic op1, TR::InstOpCode::Mnemonic op2, uint32_t imm2,
    TR::Node *node, bool reverse, TR::CodeGenerator *cg)
{
    TR::Register *trgReg = cg->allocateRegister();
    TR::Node *firstChild = node->getFirstChild();
    TR::Node *secondChild = node->getSecondChild();
    TR::Register *src1Reg = cg->evaluate(firstChild);
    TR::Register *src2Reg = cg->evaluate(secondChild);
    TR::Instruction *result = nullptr;

    if (reverse)
        result = Inst_RTYPE(op1, node, trgReg, src2Reg, src1Reg, cg);
    else
        result = Inst_RTYPE(op1, node, trgReg, src1Reg, src2Reg, cg);

    if (op2 != OP::bad) {
        result = Inst_ITYPE(op2, node, trgReg, trgReg, imm2, cg, result);
    }

    node->setRegister(trgReg);
    firstChild->decReferenceCount();
    secondChild->decReferenceCount();
    return trgReg;
}

TR::Register *OMR::RV::TreeEvaluator::icmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_sub, OP::_sltiu, 1, node, false, cg);
}

TR::Register *OMR::RV::TreeEvaluator::icmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    /*
     * We cannot use icmpHelper() because that one generates
     * RTYPE followed by ITYPE instruction. Here we need to
     * generate
     *
     *     sub result, left, right
     *     sltu result, zero, result
     *
     * i.e., RTYPE followed by RTYPE
     */
    TR::Register *trgReg = cg->allocateRegister();
    TR::Node *firstChild = node->getFirstChild();
    TR::Node *secondChild = node->getSecondChild();
    TR::Register *src1Reg = cg->evaluate(firstChild);
    TR::Register *src2Reg = cg->evaluate(secondChild);
    TR::Register *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);
    TR::Instruction *result = nullptr;

    result = Inst_RTYPE(OP::_sub, node, trgReg, src1Reg, src2Reg, cg);
    result = Inst_RTYPE(OP::_sltu, node, trgReg, zero, trgReg, cg);

    node->setRegister(trgReg);
    firstChild->decReferenceCount();
    secondChild->decReferenceCount();
    return trgReg;
}

TR::Register *OMR::RV::TreeEvaluator::icmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_slt, OP::bad, 0, node, false, cg);
}

TR::Register *OMR::RV::TreeEvaluator::icmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_slt, OP::_sltiu, 1, node, true, cg);
}

TR::Register *OMR::RV::TreeEvaluator::icmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_slt, OP::_xori, 1, node, false, cg);
}

TR::Register *OMR::RV::TreeEvaluator::icmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_slt, OP::bad, 0, node, true, cg);
}

TR::Register *OMR::RV::TreeEvaluator::iucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_sltu, OP::bad, 0, node, false, cg);
}

TR::Register *OMR::RV::TreeEvaluator::iucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_sltu, OP::_xori, 1, node, true, cg);
}

TR::Register *OMR::RV::TreeEvaluator::iucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_sltu, OP::_xori, 1, node, false, cg);
}

TR::Register *OMR::RV::TreeEvaluator::iucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return icmpHelper(OP::_sltu, OP::bad, 0, node, true, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpeqEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpneEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpneEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpltEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpgeEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpgtEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return icmpleEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lucmpltEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return iucmpltEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lucmpgeEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return iucmpgeEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lucmpgtEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return iucmpgtEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lucmpleEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR_ASSERT(cg->comp()->target().is64Bit(), "RV32 not yet supported");
    return iucmpleEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::lcmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR::Node *firstChild = node->getFirstChild();
    TR::Node *secondChild = node->getSecondChild();
    TR::Register *src1Reg = cg->evaluate(firstChild);
    TR::Register *src2Reg = cg->evaluate(secondChild);
    TR::Register *trgReg = cg->allocateRegister();
    TR::Register *tmpReg = cg->allocateRegister();

    /* Here we set up registers such that
     *
     *  - trgReg = 1  iff  src1Reg < src2Reg
     *  - tmpReg = 1  iff  src1Reg > src2Reg
     *
     */
    Inst_RTYPE(OP::_slt, node, trgReg, src1Reg, src2Reg, cg);
    Inst_RTYPE(OP::_slt, node, tmpReg, src2Reg, src1Reg, cg);

    /*
     * There are three outcomes possible:
     *
     *   (i) trgReg = 0, tmpReg = 1  => lcmp =>  1
     *  (ii) trgReg = 1, tmpReg = 0  => lcmp => -1
     * (iii) trgReg = 0, tmpReg = 0  => lcmp =>  0
     */
    Inst_RTYPE(OP::_sub, node, trgReg, tmpReg, trgReg, cg);

    cg->stopUsingRegister(tmpReg);

    node->setRegister(trgReg);
    firstChild->decReferenceCount();
    secondChild->decReferenceCount();
    return trgReg;
}

TR::Register *OMR::RV::TreeEvaluator::acmpeqEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

// also handles lselect, aselect, bselect, sselect
TR::Register *OMR::RV::TreeEvaluator::iselectEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    TR::Node *condNode = node->getChild(0);
    TR::Node *trueNode = node->getChild(1);
    TR::Node *falseNode = node->getChild(2);

    TR::Register *condReg = cg->evaluate(condNode);
    TR::Register *trueReg = cg->gprClobberEvaluate(trueNode);
    TR::Register *falseReg = cg->evaluate(falseNode);
    TR::RealRegister *zero = cg->machine()->getRealRegister(TR::RealRegister::zero);

    // Internal pointers cannot be handled since we cannot set the pinning array
    // on the result register without knowing which side of the select will be
    // taken.
    TR_ASSERT_FATAL_WITH_NODE(node, !trueReg->containsInternalPointer() && !falseReg->containsInternalPointer(),
        "Select nodes cannot have children that are internal pointers");
    if (falseReg->containsCollectedReference()) {
        logprintf(cg->comp()->getOption(TR_TraceCG), cg->comp()->log(),
            "Setting containsCollectedReference on result of select node in register %s\n",
            cg->getDebug()->getName(trueReg));
        trueReg->setContainsCollectedReference();
    }

    TR::LabelSymbol *startLabel = generateLabelSymbol(cg);
    TR::LabelSymbol *joinLabel = generateLabelSymbol(cg);

    startLabel->setStartInternalControlFlow();
    joinLabel->setEndInternalControlFlow();

    TR::RegisterDependencyConditions *deps = RegDeps(0, 3, cg);
    deps->addPostCondition(condReg, TR::RealRegister::NoReg);
    deps->addPostCondition(trueReg, TR::RealRegister::NoReg);
    deps->addPostCondition(falseReg, TR::RealRegister::NoReg);

    Inst_LABEL(OP::label, node, startLabel, cg);
    Inst_BTYPE(OP::_bne, node, joinLabel, condReg, zero, cg);
    Inst_ITYPE(OP::_addi, node, trueReg, falseReg, 0, cg);
    Inst_LABEL(OP::label, node, joinLabel, deps, cg);

    node->setRegister(trueReg);
    cg->decReferenceCount(condNode);
    cg->decReferenceCount(trueNode);
    cg->decReferenceCount(falseNode);

    return trueReg;
}

TR::Register *OMR::RV::TreeEvaluator::lookupEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::tableEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::NULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::ZEROCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::ResolveAndNULLCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::DIVCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::BNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::ArrayCopyBNDCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::ArrayStoreCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

TR::Register *OMR::RV::TreeEvaluator::ArrayCHKEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return OMR::RV::TreeEvaluator::unImpOpEvaluator(node, cg);
}

static TR::Register *commonMinMaxEvaluator(TR::Node *node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator *cg)
{
    TR_ASSERT(node->getNumChildren() == 2, "The number of children for imax/imin/lmax/lmin must be 2.");

    TR::Node *firstChild = node->getFirstChild();
    TR::Register *src1Reg = cg->gprClobberEvaluate(firstChild);
    TR::Node *secondChild = node->getSecondChild();
    TR::Register *src2Reg = cg->evaluate(secondChild);

    TR::LabelSymbol *startLabel = generateLabelSymbol(cg);
    TR::LabelSymbol *joinLabel = generateLabelSymbol(cg);

    startLabel->setStartInternalControlFlow();
    joinLabel->setEndInternalControlFlow();

    TR::RegisterDependencyConditions *deps = RegDeps(0, 2, cg);
    deps->addPostCondition(src1Reg, TR::RealRegister::NoReg);
    deps->addPostCondition(src2Reg, TR::RealRegister::NoReg);

    Inst_LABEL(OP::label, node, startLabel, cg);
    Inst_BTYPE(op, node, joinLabel, src1Reg, src2Reg, cg);
    Inst_ITYPE(OP::_addi, node, src1Reg, src2Reg, 0, cg);
    Inst_LABEL(OP::label, node, joinLabel, deps, cg);

    node->setRegister(src1Reg);
    cg->decReferenceCount(firstChild);
    cg->decReferenceCount(secondChild);

    return src1Reg;
}

// Also handles lmax
TR::Register *OMR::RV::TreeEvaluator::imaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return commonMinMaxEvaluator(node, OP::_bge, cg);
}

// Also handles lumax
TR::Register *OMR::RV::TreeEvaluator::iumaxEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return commonMinMaxEvaluator(node, OP::_bgeu, cg);
}

// Also handles lmin
TR::Register *OMR::RV::TreeEvaluator::iminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return commonMinMaxEvaluator(node, OP::_blt, cg);
}

// Also handles lumin
TR::Register *OMR::RV::TreeEvaluator::iuminEvaluator(TR::Node *node, TR::CodeGenerator *cg)
{
    return commonMinMaxEvaluator(node, OP::_bltu, cg);
}


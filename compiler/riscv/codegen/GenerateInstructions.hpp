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

#ifndef GENERATE_INSTRUCTIONS_INCL
#define GENERATE_INSTRUCTIONS_INCL

#include <stddef.h>
#include <stdint.h>
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "env/jittypes.h"
#include "runtime/Runtime.hpp"

namespace TR {
class CodeGenerator;
class LabelSymbol;
class MemoryReference;
class Node;
class Register;
class RegisterDependencyConditions;
class Snippet;
class SymbolReference;
} // namespace TR
class TR_VirtualGuardSite;

/*
 * @brief Generates simple instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *Inst(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::CodeGenerator *cg,
    TR::Instruction *preced = NULL);

/*
 * @brief Generates label instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] sym : label symbol
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *Inst_LABEL(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym, TR::CodeGenerator *cg,
    TR::Instruction *preced = NULL);

/*
 * @brief Generates label instruction with register dependency
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] sym : label symbol
 * @param[in] cond : register dependency condition
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *Inst_LABEL(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym,
    TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg, TR::Instruction *preced = NULL);

/*
 * @brief Generates admin instruction
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] cg : CodeGenerator
 * @param[in] fenceNode : fence node
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *Inst_ADMIN(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::CodeGenerator *cg,
    TR::Node *fenceNode = NULL, TR::Instruction *preced = NULL);

/*
 * @brief Generates admin instruction with register dependency
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] cond : register dependency condition
 * @param[in] cg : CodeGenerator*
 * @param[in] fenceNode : fence node
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *Inst_ADMIN(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::RegisterDependencyConditions *cond,
    TR::CodeGenerator *cg, TR::Node *fenceNode = NULL, TR::Instruction *preced = NULL);

TR::Instruction *Inst_DATA(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR::CodeGenerator *cg,
    TR::Instruction *precedingInstruction = NULL);

TR::Instruction *Inst_RTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg, TR::Register *s1reg,
    TR::Register *s2reg, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_ITYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg, TR::Register *sreg,
    uint32_t imm, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_ITYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg, TR::Register *sreg,
    uint32_t imm, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_LOAD(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg, TR::MemoryReference *memRef,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_STYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *s1reg, TR::Register *s2reg,
    uint32_t imm, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_STORE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::MemoryReference *memRef, TR::Register *srcReg,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_BTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::LabelSymbol *sym, TR::Register *src1,
    TR::Register *src2, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_UTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, uint32_t imm, TR::Register *reg,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_JTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg, uintptr_t imm,
    TR::RegisterDependencyConditions *cond, TR::SymbolReference *sr, TR::Snippet *s, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL);

TR::Instruction *Inst_JTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg, TR::LabelSymbol *label,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_JTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg, TR::LabelSymbol *label,
    TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);

TR::Instruction *Inst_JTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg, TR::LabelSymbol *label,
    TR::Snippet *snippet, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL);

#ifdef J9_PROJECT_SPECIFIC
TR::Instruction *Inst_VGNOP(TR::Node *n, TR_VirtualGuardSite *site, TR::RegisterDependencyConditions *cond,
    TR::LabelSymbol *sym, TR::CodeGenerator *cg, TR::Instruction *previous = NULL);
#endif // J9_PROJECT_SPECIFIC

/*
 * Following functions are DEPRECATED and should not be used in new code.
 * The definitions here are only for backward compatibility!
 * */

static inline TR::Instruction *generateInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
    TR::Instruction *preced = NULL)
{
    return Inst(op, node, cg, preced);
}

static inline TR::Instruction *generateLABEL(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
    TR::LabelSymbol *sym, TR::Instruction *preced = NULL)
{
    return Inst_LABEL(op, node, sym, cg, preced);
}

static inline TR::Instruction *generateLABEL(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
    TR::LabelSymbol *sym, TR::RegisterDependencyConditions *cond, TR::Instruction *preced = NULL)
{
    return Inst_LABEL(op, node, sym, cond, cg, preced);
}

static inline TR::Instruction *generateADMIN(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
    TR::Node *fenceNode = NULL, TR::Instruction *preced = NULL)
{
    return Inst_ADMIN(op, node, cg, fenceNode, preced);
}

static inline TR::Instruction *generateADMIN(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
    TR::RegisterDependencyConditions *cond, TR::Node *fenceNode = NULL, TR::Instruction *preced = NULL)
{
    return Inst_ADMIN(op, node, cond, cg, fenceNode, preced);
}

static inline TR::Instruction *generateRTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg,
    TR::Register *s1reg, TR::Register *s2reg, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_RTYPE(op, n, treg, s1reg, s2reg, cg, previous);
}

static inline TR::Instruction *generateITYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg,
    TR::Register *sreg, uint32_t imm, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_ITYPE(op, n, treg, sreg, imm, cg, previous);
}

static inline TR::Instruction *generateITYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg,
    TR::Register *sreg, uint32_t imm, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL)
{
    return Inst_ITYPE(op, n, treg, sreg, imm, cond, cg, previous);
}

static inline TR::Instruction *generateLOAD(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *treg,
    TR::MemoryReference *memRef, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_LOAD(op, n, treg, memRef, cg, previous);
}

static inline TR::Instruction *generateSTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *s1reg,
    TR::Register *s2reg, uint32_t imm, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_STYPE(op, n, s1reg, s2reg, imm, cg, previous);
}

static inline TR::Instruction *generateSTORE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::MemoryReference *memRef,
    TR::Register *srcReg, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_STORE(op, n, memRef, srcReg, cg, previous);
}

static inline TR::Instruction *generateBTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::LabelSymbol *sym,
    TR::Register *src1, TR::Register *src2, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_BTYPE(op, n, sym, src1, src2, cg, previous);
}

static inline TR::Instruction *generateUTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, uint32_t imm, TR::Register *reg,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_UTYPE(op, n, imm, reg, cg, previous);
}

static inline TR::Instruction *generateJTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg,
    uintptr_t imm, TR::RegisterDependencyConditions *cond, TR::SymbolReference *sr, TR::Snippet *s,
    TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_JTYPE(op, n, trgReg, imm, cond, sr, s, cg, previous);
}

static inline TR::Instruction *generateJTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg,
    TR::LabelSymbol *label, TR::CodeGenerator *cg, TR::Instruction *previous = NULL)
{
    return Inst_JTYPE(op, n, trgReg, label, cg, previous);
}

static inline TR::Instruction *generateJTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg,
    TR::LabelSymbol *label, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL)
{
    return Inst_JTYPE(op, n, trgReg, label, cond, cg, previous);
}

static inline TR::Instruction *generateJTYPE(TR::InstOpCode::Mnemonic op, TR::Node *n, TR::Register *trgReg,
    TR::LabelSymbol *label, TR::Snippet *snippet, TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL)
{
    return Inst_JTYPE(op, n, trgReg, label, snippet, cond, cg, previous);
}

#ifdef J9_PROJECT_SPECIFIC
static inline TR::Instruction *generateVGNOP(TR::Node *n, TR_VirtualGuardSite *site,
    TR::RegisterDependencyConditions *cond, TR::LabelSymbol *sym, TR::CodeGenerator *cg,
    TR::Instruction *previous = NULL)
{
    return Inst_VGNOP(n, site, cond, sym, cg, previous);
}
#endif // J9_PROJECT_SPECIFIC

#endif

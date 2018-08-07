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

#ifndef GENERATE_INSTRUCTIONS_INCL
#define GENERATE_INSTRUCTIONS_INCL

#include <stddef.h>
#include <stdint.h>
#include "codegen/ARM64ConditionCode.hpp"
#include "codegen/ARM64ShiftCode.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "env/jittypes.h"
#include "runtime/Runtime.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class MemoryReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class Snippet; }
namespace TR { class SymbolReference; }


/*
 * @brief Generates simple instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates imm instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] imm : immediate value
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateImmInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   uint32_t imm,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates imm instruction with relocation
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] imm : immediate value
 * @param[in] relocationKind : relocation kind
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateImmInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   uint32_t imm,
                   TR_ExternalRelocationTargetKind relocationKind,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates imm instruction with symbol reference
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] imm : immediate value
 * @param[in] relocationKind : relocation kind
 * @param[in] sr : symbol reference
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateImmInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   uint32_t imm,
                   TR_ExternalRelocationTargetKind relocationKind,
                   TR::SymbolReference *sr,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates dep instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] cond : register dependency condition
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateDepInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::RegisterDependencyConditions *cond,
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
TR::Instruction *generateLabelInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::LabelSymbol *sym,
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
TR::Instruction *generateDepLabelInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::LabelSymbol *sym,
                   TR::RegisterDependencyConditions *cond,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates conditional branch instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] sym : label symbol
 * @param[in] cc : branch condition code
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateConditionalBranchInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::LabelSymbol *sym,
                   TR::ARM64ConditionCode cc,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates conditional branch instruction with register dependency
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] sym : label symbol
 * @param[in] cc : branch condition code
 * @param[in] cond : register dependency condition
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateDepConditionalBranchInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::LabelSymbol *sym,
                   TR::ARM64ConditionCode cc,
                   TR::RegisterDependencyConditions *cond,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates compare and branch instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] sreg : source register
 * @param[in] sym : label symbol
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateCompareBranchInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *sreg,
                   TR::LabelSymbol *sym,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates admin instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] fenceNode : fence node
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateAdminInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Node *fenceNode = NULL,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates imm-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] imm : immediate value
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1ImmInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   uint32_t imm,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src1-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src1Instruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src1-and-imm-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register
 * @param[in] imm : immediate value
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src1ImmInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   uint32_t imm,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src2-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register 1
 * @param[in] s2reg : source register 2
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src2Instruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   TR::Register *s2reg,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src2-to-trg instruction (shifted register)
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register 1
 * @param[in] s2reg : source register 2
 * @param[in] shiftType : shift type
 * @param[in] shiftAmount : shift amount
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src2ShiftedInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   TR::Register *s2reg,
                   TR::ARM64ShiftCode shiftType,
                   uint32_t shiftAmount,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src2-to-trg instruction (extended register)
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register 1
 * @param[in] s2reg : source register 2
 * @param[in] extendType : extend type
 * @param[in] shiftAmount : shift amount
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src2ExtendedInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   TR::Register *s2reg,
                   TR::ARM64ExtendCode extendType,
                   uint32_t shiftAmount,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src3-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] s1reg : source register 1
 * @param[in] s2reg : source register 2
 * @param[in] s3reg : source register 3
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1Src3Instruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::Register *s1reg,
                   TR::Register *s2reg,
                   TR::Register *s3reg,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates mem-to-trg instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] treg : target register
 * @param[in] mr : memory reference
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateTrg1MemInstruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::Register *treg,
                   TR::MemoryReference *mr,
                   TR::Instruction *preced = NULL);

/*
 * @brief Generates src-to-mem instruction
 * @param[in] cg : CodeGenerator
 * @param[in] op : instruction opcode
 * @param[in] node : node
 * @param[in] mr : memory reference
 * @param[in] sreg : source register
 * @param[in] preced : preceding instruction
 * @return generated instruction
 */
TR::Instruction *generateMemSrc1Instruction(
                   TR::CodeGenerator *cg,
                   TR::InstOpCode::Mnemonic op,
                   TR::Node *node,
                   TR::MemoryReference *mr,
                   TR::Register *sreg,
                   TR::Instruction *preced = NULL);

#endif

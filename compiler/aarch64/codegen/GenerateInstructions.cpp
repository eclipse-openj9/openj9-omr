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

#include "aarch64/codegen/GenerateInstructions.hpp"

#include <stdint.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/ARM64Instruction.hpp"

namespace TR { class LabelSymbol; }
namespace TR { class Node; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class SymbolReference; }

TR::Instruction *generateInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::Instruction(op, node, preced, cg);
   return new (cg->trHeapMemory()) TR::Instruction(op, node, cg);
   }

TR::Instruction *generateImmInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm,
                                       TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, cg);
   }

TR::Instruction *generateImmInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm,
                                       TR_ExternalRelocationTargetKind relocationKind, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, relocationKind, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, relocationKind, cg);
   }

TR::Instruction *generateImmInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm,
                                       TR_ExternalRelocationTargetKind relocationKind, TR::SymbolReference *sr, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, relocationKind, sr, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64ImmInstruction(op, node, imm, relocationKind, sr, cg);
   }

TR::Instruction *generateDepInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::RegisterDependencyConditions *cond, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64DepInstruction(op, node, cond, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64DepInstruction(op, node, cond, cg);
   }

TR::Instruction *generateLabelInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::LabelSymbol *sym, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64LabelInstruction(op, node, sym, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64LabelInstruction(op, node, sym, cg);
   }

TR::Instruction *generateDepLabelInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::LabelSymbol *sym, TR::RegisterDependencyConditions *cond, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64DepLabelInstruction(op, node, sym, cond, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64DepLabelInstruction(op, node, sym, cond, cg);
   }

TR::Instruction *generateConditionalBranchInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::LabelSymbol *sym, TR::ARM64ConditionCode cc, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64ConditionalBranchInstruction(op, node, sym, cc, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64ConditionalBranchInstruction(op, node, sym, cc, cg);
   }

TR::Instruction *generateDepConditionalBranchInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::LabelSymbol *sym, TR::ARM64ConditionCode cc, TR::RegisterDependencyConditions *cond, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64DepConditionalBranchInstruction(op, node, sym, cc, cond, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64DepConditionalBranchInstruction(op, node, sym, cc, cond, cg);
   }

TR::Instruction *generateCompareBranchInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *sreg, TR::LabelSymbol *sym, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64CompareBranchInstruction(op, node, sreg, sym, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64CompareBranchInstruction(op, node, sreg, sym, cg);
   }

TR::Instruction *generateAdminInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Node *fenceNode, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64AdminInstruction(op, node, fenceNode, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64AdminInstruction(op, node, fenceNode, cg);
   }

TR::Instruction *generateTrg1ImmInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, uint32_t imm, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1ImmInstruction(op, node, treg, imm, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1ImmInstruction(op, node, treg, imm, cg);
   }

TR::Instruction *generateTrg1Src1Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src1Instruction(op, node, treg, s1reg, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src1Instruction(op, node, treg, s1reg, cg);
   }

TR::Instruction *generateTrg1Src1ImmInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, uint32_t imm, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src1ImmInstruction(op, node, treg, s1reg, imm, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src1ImmInstruction(op, node,treg, s1reg, imm, cg);
   }

TR::Instruction *generateTrg1Src2Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, TR::Register *s2reg, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src2Instruction(op, node, treg, s1reg, s2reg, cg);
   }

TR::Instruction *generateTrg1Src2ShiftedInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, TR::Register *s2reg,
   TR::ARM64ShiftCode shiftType, uint32_t shiftAmount, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src2ShiftedInstruction(op, node, treg, s1reg, s2reg, shiftType, shiftAmount, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src2ShiftedInstruction(op, node, treg, s1reg, s2reg, shiftType, shiftAmount, cg);
   }

TR::Instruction *generateTrg1Src2ExtendtedInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, TR::Register *s2reg,
   TR::ARM64ExtendCode extendType, uint32_t shiftAmount, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src2ExtendedInstruction(op, node, treg, s1reg, s2reg, extendType, shiftAmount, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src2ExtendedInstruction(op, node, treg, s1reg, s2reg, extendType, shiftAmount, cg);
   }

TR::Instruction *generateTrg1Src3Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::Register *s1reg, TR::Register *s2reg, TR::Register *s3reg, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1Src3Instruction(op, node, treg, s1reg, s2reg, s3reg, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1Src3Instruction(op, node, treg, s1reg, s2reg, s3reg, cg);
   }

TR::Instruction *generateTrg1MemInstruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::Register *treg, TR::MemoryReference *mr, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64Trg1MemInstruction(op, node, treg, mr, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64Trg1MemInstruction(op, node, treg, mr, cg);
   }

TR::Instruction *generateMemSrc1Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node,
   TR::MemoryReference *mr, TR::Register *sreg, TR::Instruction *preced)
   {
   if (preced)
      return new (cg->trHeapMemory()) TR::ARM64MemSrc1Instruction(op, node, mr, sreg, preced, cg);
   return new (cg->trHeapMemory()) TR::ARM64MemSrc1Instruction(op, node, mr, sreg, cg);
   }

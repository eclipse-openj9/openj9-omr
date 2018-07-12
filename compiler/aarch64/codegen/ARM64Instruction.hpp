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

#ifndef ARM64INSTRUCTION_INCL
#define ARM64INSTRUCTION_INCL

#include <stddef.h>
#include <stdint.h>

#include "codegen/ARM64ConditionCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/MemoryReference.hpp"
#include "il/symbol/LabelSymbol.hpp"
#include "infra/Assert.hpp"

namespace TR { class SymbolReference; }

#define ARM64_INSTRUCTION_LENGTH 4

namespace TR
{

class ARM64ImmInstruction : public TR::Instruction
   {
   uint32_t _sourceImmediate;
   TR_ExternalRelocationTargetKind _reloKind;
   TR::SymbolReference *_symbolReference;

public:

   // Constructors without relocation types
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(TR_NoRelocation), _symbolReference(NULL)
      {
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(TR_NoRelocation), _symbolReference(NULL)
      {
      }

   // Constructors with relocation types
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(NULL)
      {
      setNeedsAOTRelocation(true);
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(NULL)
      {
      setNeedsAOTRelocation(true);
      }

   // Constructors with relocation typesand associated symbol references
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] sr : symbol reference
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::SymbolReference *sr, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(sr)
      {
      setNeedsAOTRelocation(true);
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] imm : immediate value
    * @param[in] relocationKind : relocation kind
    * @param[in] sr : symbol reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, uint32_t imm, TR_ExternalRelocationTargetKind relocationKind,
                        TR::SymbolReference *sr, TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _sourceImmediate(imm), _reloKind(relocationKind), _symbolReference(sr)
      {
      setNeedsAOTRelocation(true);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsImm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _sourceImmediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_sourceImmediate = si);}

   /**
    * @brief Gets relocation kind
    * @return relocation kind
    */
   TR_ExternalRelocationTargetKind getReloKind() { return _reloKind; }
   /**
    * @brief Sets relocation kind
    * @param[in] reloKind : relocation kind
    */
   void setReloKind(TR_ExternalRelocationTargetKind reloKind) { _reloKind = reloKind; }

   /**
    * @brief Gets symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *getSymbolReference() {return _symbolReference;}
   /**
    * @brief Sets symbol reference
    * @param[in] sr : symbol reference
    * @return symbol reference
    */
   TR::SymbolReference *setSymbolReference(TR::SymbolReference *sr)
      {
      return (_symbolReference = sr);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64DepInstruction : public TR::Instruction
   {
   TR::RegisterDependencyConditions *_conditions;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] cond : register dependency condition
    * @param[in] cg : CodeGenerator
    */
   ARM64DepInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node,
                       TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _conditions(cond)
      {
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] cond : register dependency condition
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64DepInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node,
                       TR::RegisterDependencyConditions *cond,
                       TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _conditions(cond)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsDep; }

   /**
    * @brief Gets register dependency condition
    * @return register dependency condition
    */
   virtual TR::RegisterDependencyConditions *getDependencyConditions()
      {
      return _conditions;
      }
   /**
    * @brief Sets register dependency condition
    * @param[in] cond : register dependency condition
    * @return register dependency condition
    */
   TR::RegisterDependencyConditions *setDependencyConditions(TR::RegisterDependencyConditions *cond)
      {
      return (_conditions = cond);
      }
   };

class ARM64LabelInstruction : public TR::Instruction
   {
   TR::LabelSymbol *_symbol;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cg : CodeGenerator
    */
   ARM64LabelInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _symbol(sym)
      {
      if (sym != NULL && op == TR::InstOpCode::label)
         sym->setInstruction(this);
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64LabelInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym,
                          TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _symbol(sym)
      {
      if (sym!=NULL && op==TR::InstOpCode::label)
         sym->setInstruction(this);
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsLabel; }

   /**
    * @brief Gets label symbol
    * @return label symbol
    */
   TR::LabelSymbol *getLabelSymbol() {return _symbol;}
   /**
    * @brief Sets label symbol
    * @param[in] sym : label symbol
    * @return label symbol
    */
   TR::LabelSymbol *setLabelSymbol(TR::LabelSymbol *sym)
      {
      return (_symbol = sym);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64DepLabelInstruction : public ARM64LabelInstruction
   {
   TR::RegisterDependencyConditions *_conditions;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cond : register dependency condition
    * @param[in] cg : CodeGenerator
    */
   ARM64DepLabelInstruction(TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::LabelSymbol *sym,
                             TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : ARM64LabelInstruction(op, node, sym, cg), _conditions(cond)
      {
      }
   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cond : register dependency condition
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64DepLabelInstruction(TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::LabelSymbol *sym,
                             TR::RegisterDependencyConditions  *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64LabelInstruction(op, node, sym, precedingInstruction, cg),
        _conditions(cond)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsDepLabel; }

   /**
    * @brief Gets register dependency condition
    * @return register dependency condition
    */
   virtual TR::RegisterDependencyConditions *getDependencyConditions()
      {
      return _conditions;
      }
   /**
    * @brief Sets register dependency condition
    * @param[in] cond : register dependency condition
    * @return register dependency condition
    */
   TR::RegisterDependencyConditions *setDependencyConditions(TR::RegisterDependencyConditions *cond)
      {
      return (_conditions = cond);
      }
   };

class ARM64ConditionalBranchInstruction : public ARM64LabelInstruction
   {
   int32_t _estimatedBinaryLocation;
   TR::ARM64ConditionCode _cc;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cc : branch condition code
    * @param[in] cg : CodeGenerator
    */
   ARM64ConditionalBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym,
                                     TR::ARM64ConditionCode cc,
                                     TR::CodeGenerator *cg)
      : ARM64LabelInstruction(op, node, sym, cg), _cc(cc),
        _estimatedBinaryLocation(0)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cc : branch condition code
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64ConditionalBranchInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::LabelSymbol *sym,
                                     TR::ARM64ConditionCode cc,
                                     TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64LabelInstruction(op, node, sym, precedingInstruction, cg), _cc(cc),
        _estimatedBinaryLocation(0)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsConditionalBranch; }

   /**
    * @brief Gets estimated binary location
    * @return estimated binary location
    */
   int32_t getEstimatedBinaryLocation() {return _estimatedBinaryLocation;}
   /**
    * @brief Sets estimated binary location
    * @param[in] l : estimated binary location
    * @return estimated binary location
    */
   int32_t setEstimatedBinaryLocation(int32_t l) {return (_estimatedBinaryLocation = l);}

   /**
    * @brief Gets condition code
    * @return condition code
    */
   TR::ARM64ConditionCode getConditionCode() {return _cc;}
   /**
    * @brief Sets condition code
    * @param[in] cc : condition code
    * @return condition code
    */
   TR::ARM64ConditionCode setConditionCode(TR::ARM64ConditionCode cc) {return (_cc = cc);}

   /**
    * @brief Sets immediate field in binary encoding
    * @param[in] instruction : instruction cursor
    * @param[in] distance : branch distance
    */
   void insertImmediateField(uint32_t *instruction, int32_t distance)
      {
      TR_ASSERT((distance & 0x3) == 0, "branch distance is not aligned");
      *instruction |= ((distance >> 2) & 0x7ffff) << 5;
      }

   /**
    * @brief Sets condition code in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertConditionCodeField(uint32_t *instruction)
      {
      *instruction |= _cc;
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64DepConditionalBranchInstruction : public ARM64ConditionalBranchInstruction
   {
   TR::RegisterDependencyConditions *_conditions;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cc : branch condition code
    * @param[in] cond : register dependency condition
    * @param[in] cg : CodeGenerator
    */
   ARM64DepConditionalBranchInstruction(
                             TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::LabelSymbol *sym,
                             TR::ARM64ConditionCode cc,
                             TR::RegisterDependencyConditions *cond, TR::CodeGenerator *cg)
      : ARM64ConditionalBranchInstruction(op, node, sym, cc, cg), _conditions(cond)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] sym : label symbol
    * @param[in] cc : branch condition code
    * @param[in] cond : register dependency condition
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64DepConditionalBranchInstruction(
                             TR::InstOpCode::Mnemonic op,
                             TR::Node *node,
                             TR::LabelSymbol *sym,
                             TR::ARM64ConditionCode cc,
                             TR::RegisterDependencyConditions *cond,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64ConditionalBranchInstruction(op, node, sym, cc, precedingInstruction, cg), _conditions(cond)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsDepConditionalBranch; }

   /**
    * @brief Gets register dependency condition
    * @return register dependency condition
    */
   virtual TR::RegisterDependencyConditions *getDependencyConditions()
      {
      return _conditions;
      }
   /**
    * @brief Sets register dependency condition
    * @param[in] cond : register dependency condition
    * @return register dependency condition
    */
   TR::RegisterDependencyConditions *setDependencyConditions(TR::RegisterDependencyConditions *cond)
      {
      return (_conditions = cond);
      }

   virtual TR::Register *getTargetRegister(uint32_t i)
      {
      TR_ASSERT(false, "Not implemented yet.");
      return NULL;
      }

   virtual TR::Register *getSourceRegister(uint32_t i)
      {
      TR_ASSERT(false, "Not implemented yet.");
      return NULL;
      }
   };

class ARM64AdminInstruction : public TR::Instruction
   {
   TR::Node *_fenceNode;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] fenceNode : fence node
    * @param[in] cg : CodeGenerator
    */
   ARM64AdminInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Node *fenceNode, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _fenceNode(fenceNode)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] fenceNode : fence node
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64AdminInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Node *fenceNode,
                          TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _fenceNode(fenceNode)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsAdmin; }

   /**
    * @brief Gets fence node
    * @return fence node
    */
   TR::Node *getFenceNode() { return _fenceNode; }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Instruction : public TR::Instruction
   {
   TR::Register *_target1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, cg), _target1Register(treg)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : TR::Instruction(op, node, precedingInstruction, cg), _target1Register(treg)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1; }

   /**
    * @brief Gets target register
    * @return target register
    */
   TR::Register *getTargetRegister() {return _target1Register;}
   /**
    * @brief Sets target register
    * @param[in] tr : target register
    * @return target register
    */
   TR::Register *setTargetRegister(TR::Register *tr) {return (_target1Register = tr);}

   /**
    * @brief Sets target register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertTargetRegister(uint32_t *instruction)
      {
      TR::RealRegister *target = toRealRegister(_target1Register);
      target->setRegisterFieldRD(instruction);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1ImmInstruction : public ARM64Trg1Instruction
   {
   uint32_t _sourceImmediate;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                            uint32_t imm, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _sourceImmediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                            uint32_t imm,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _sourceImmediate(imm)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Imm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _sourceImmediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_sourceImmediate = si);}

   /**
    * @brief Sets immediate field in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertImmediateField(uint32_t *instruction)
      {
      /* immediate width depends on InstOpCode */
      TR_ASSERT(false, "Not implemented yet.");
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src1Instruction : public ARM64Trg1Instruction
   {
   TR::Register *_source1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _source1Register(sreg)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1Instruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                             TR::Register *sreg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _source1Register(sreg)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src1; }

   /**
    * @brief Gets source register
    * @return source register
    */
   TR::Register *getSource1Register() {return _source1Register;}
   /**
    * @brief Sets source register
    * @param[in] sr : source register
    * @return source register
    */
   TR::Register *setSource1Register(TR::Register *sr) {return (_source1Register = sr);}

   /**
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if (i==0) return _source1Register; return NULL;}

   /**
    * @brief Sets source register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource1Register(uint32_t *instruction)
      {
      TR::RealRegister *source1 = toRealRegister(_source1Register);
      source1->setRegisterFieldRN(instruction);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src1ImmInstruction : public ARM64Trg1Src1Instruction
   {
   uint32_t _source1Immediate;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                                TR::Register *sreg, uint32_t imm, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, cg), _source1Immediate(imm)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] sreg : source register
    * @param[in] imm : immediate value
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src1ImmInstruction(TR::InstOpCode::Mnemonic op, TR::Node *node, TR::Register *treg,
                                TR::Register *sreg, uint32_t imm,
                                TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, sreg, precedingInstruction, cg), _source1Immediate(imm)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src1Imm; }

   /**
    * @brief Gets source immediate
    * @return source immediate
    */
   uint32_t getSourceImmediate() {return _source1Immediate;}
   /**
    * @brief Sets source immediate
    * @param[in] si : immediate value
    * @return source immediate
    */
   uint32_t setSourceImmediate(uint32_t si) {return (_source1Immediate = si);}

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1Src2Instruction : public ARM64Trg1Src1Instruction
   {
   TR::Register *_source2Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                              TR::Node *node,
                              TR::Register *treg,
                              TR::Register *s1reg,
                              TR::Register *s2reg, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, cg), _source2Register(s2reg)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] s1reg : source register 1
    * @param[in] s2reg : source register 2
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1Src2Instruction( TR::InstOpCode::Mnemonic op,
                              TR::Node *node,
                              TR::Register *treg,
                              TR::Register *s1reg,
                              TR::Register *s2reg,
                             TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Src1Instruction(op, node, treg, s1reg, precedingInstruction, cg),
                             _source2Register(s2reg)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Src2; }

   /**
    * @brief Gets source register 2
    * @return source register 2
    */
   TR::Register *getSource2Register() {return _source2Register;}
   /**
    * @brief Sets source register 2
    * @param[in] sr : source register 2
    * @return source register 2
    */
   TR::Register *setSource2Register(TR::Register *sr) {return (_source2Register = sr);}

   /**
    * @brief Gets i-th source register
    * @param[in] i : index
    * @return i-th source register or NULL
    */
   virtual TR::Register *getSourceRegister(uint32_t i) {if      (i==0) return getSource1Register();
                                               else if (i==1) return _source2Register; return NULL;}

   /**
    * @brief Sets source register 2 in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource2Register(uint32_t *instruction)
      {
      TR::RealRegister *source2 = toRealRegister(_source2Register);
      source2->setRegisterFieldRM(instruction);
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64Trg1MemInstruction : public ARM64Trg1Instruction
   {
   TR::MemoryReference *_memoryReference;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] mr : memory reference
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1MemInstruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::Register *treg,
                            TR::MemoryReference *mr, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, cg), _memoryReference(mr)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] treg : target register
    * @param[in] mr : memory reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64Trg1MemInstruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::Register *treg,
                            TR::MemoryReference *mr,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64Trg1Instruction(op, node, treg, precedingInstruction, cg), _memoryReference(mr)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsTrg1Mem; }

   /**
    * @brief Gets memory reference
    * @return memory reference
    */
   TR::MemoryReference *getMemoryReference() {return _memoryReference;}
   /**
    * @brief Sets memory reference
    * @param[in] mr : memory reference
    * @return memory reference
    */
   TR::MemoryReference *setMemoryReference(TR::MemoryReference *mr)
      {
      return (_memoryReference = mr);
      }

   /**
    * @brief Gets base register of memory reference
    * @return base register
    */
   virtual TR::Register *getMemoryBase() {return getMemoryReference()->getBaseRegister();}
   /**
    * @brief Gets index register of memory reference
    * @return index register
    */
   virtual TR::Register *getMemoryIndex() {return getMemoryReference()->getIndexRegister();}
   /**
    * @brief Gets offset of memory reference
    * @return offset
    */
   virtual int32_t getOffset() {return getMemoryReference()->getOffset();}

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

class ARM64MemInstruction : public TR::Instruction
   {
   TR::MemoryReference *_memoryReference;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] cg : CodeGenerator
    */
   ARM64MemInstruction(TR::InstOpCode::Mnemonic op,
                        TR::Node *node,
                        TR::MemoryReference *mr, TR::CodeGenerator *cg);

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64MemInstruction(TR::InstOpCode::Mnemonic op,
                        TR::Node *node,
                        TR::MemoryReference *mr,
                        TR::Instruction *precedingInstruction, TR::CodeGenerator *cg);

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsMem; }

   /**
    * @brief Gets memory reference
    * @return memory reference
    */
   TR::MemoryReference *getMemoryReference() {return _memoryReference;}
   /**
    * @brief Sets memory reference
    * @param[in] mr : memory reference
    * @return memory reference
    */
   TR::MemoryReference *setMemoryReference(TR::MemoryReference *mr)
      {
      return (_memoryReference = mr);
      }

   /**
    * @brief Gets base register of memory reference
    * @return base register
    */
   virtual TR::Register *getMemoryBase() {return getMemoryReference()->getBaseRegister();}
   /**
    * @brief Gets index register of memory reference
    * @return index register
    */
   virtual TR::Register *getMemoryIndex() {return getMemoryReference()->getIndexRegister();}
   /**
    * @brief Gets offset of memory reference
    * @return offset
    */
   virtual int32_t getOffset() {return getMemoryReference()->getOffset();}

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();

   };

class ARM64MemSrc1Instruction : public ARM64MemInstruction
   {
   TR::Register *_source1Register;

   public:

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] sreg : source register
    * @param[in] cg : CodeGenerator
    */
   ARM64MemSrc1Instruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::MemoryReference *mr,
                            TR::Register *sreg, TR::CodeGenerator *cg)
      : ARM64MemInstruction(op, node, mr, cg), _source1Register(sreg)
      {
      }

   /*
    * @brief Constructor
    * @param[in] op : instruction opcode
    * @param[in] node : node
    * @param[in] mr : memory reference
    * @param[in] sreg : source register
    * @param[in] precedingInstruction : preceding instruction
    * @param[in] cg : CodeGenerator
    */
   ARM64MemSrc1Instruction(TR::InstOpCode::Mnemonic op,
                            TR::Node *node,
                            TR::MemoryReference *mr,
                            TR::Register *sreg,
                            TR::Instruction *precedingInstruction, TR::CodeGenerator *cg)
      : ARM64MemInstruction(op, node, mr, precedingInstruction, cg), _source1Register(sreg)
      {
      }

   /**
    * @brief Gets instruction kind
    * @return instruction kind
    */
   virtual Kind getKind() { return IsMemSrc1; }

   /**
    * @brief Gets source register
    * @return source register
    */
   TR::Register *getSource1Register() {return _source1Register;}
   /**
    * @brief Sets source register
    * @param[in] sr : source register
    * @return source register
    */
   TR::Register *setSource1Register(TR::Register *sr) {return (_source1Register = sr);}

   /**
    * @brief Sets source register in binary encoding
    * @param[in] instruction : instruction cursor
    */
   void insertSource1Register(uint32_t *instruction)
      {
      TR::RealRegister *source1 = toRealRegister(_source1Register);
      TR_ASSERT(false, "Not implemented yet.");
      }

   /**
    * @brief Generates binary encoding of the instruction
    * @return instruction cursor
    */
   virtual uint8_t *generateBinaryEncoding();
   };

} // TR

#endif

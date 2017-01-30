/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef OMR_X86_INSTRUCTIONBASE_INCL
#define OMR_X86_INSTRUCTIONBASE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_INSTRUCTION_CONNECTOR
#define OMR_INSTRUCTION_CONNECTOR
namespace OMR { namespace X86 { class Instruction; } }
namespace OMR { typedef OMR::X86::Instruction InstructionConnector; }
#else
#error OMR::X86::Instruction expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRInstruction.hpp"

#include <stddef.h>                                    // for NULL
#include <stdint.h>                                    // for int32_t, etc
#include "codegen/InstOpCode.hpp"                      // for InstOpCode, etc
#include "codegen/RegisterConstants.hpp"
#include "x/codegen/X86Ops.hpp"                        // for TR_X86OpCode, etc

namespace TR { class X86ImmInstruction;   }
namespace TR { class X86LabelInstruction; }
namespace TR { class X86RegInstruction;   }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class MemoryReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }
struct TR_VFPState;

namespace OMR
{

namespace X86
{
class EnlargementResult
   {
   int32_t _patchGrowth; // the minimum guaratneed size of increase - we know we will grow this much
   int32_t _encodingGrowth; // the maximum size of increase - we have to accomodate growth of up to this much
   public:
   EnlargementResult(int32_t patchGrowth, int32_t encodingGrowth)
      : _patchGrowth(patchGrowth), _encodingGrowth(encodingGrowth) { }
   EnlargementResult(const EnlargementResult &other)
      : _patchGrowth(other._patchGrowth), _encodingGrowth(other._encodingGrowth) { }
   int32_t getPatchGrowth() { return _patchGrowth; }
   int32_t getEncodingGrowth() { return _encodingGrowth; }
   };

class OMR_EXTENSIBLE Instruction : public OMR::Instruction
   {
   private:
   uint8_t _rexRepeatCount;

   protected:
#if defined(TR_TARGET_64BIT)
   void setRexRepeatCount(uint8_t count) { _rexRepeatCount = count; }
#else
   void setRexRepeatCount(uint8_t count) { TR_ASSERT(0, "Forcing a rex repetition does not make sense in IA32 mode!"); }
#endif

   protected:

   Instruction(TR::CodeGenerator *cg, TR::InstOpCode::Mnemonic op, TR::Node *node);
   Instruction(TR::CodeGenerator *cg, TR::Instruction *precedingInstruction, TR::InstOpCode::Mnemonic op, TR::Node *node = 0);
   void initialize(TR::CodeGenerator *cg = NULL, TR::RegisterDependencyConditions *cond = NULL, TR_X86OpCodes op = BADIA32Op, bool flag = false);

   public:

   virtual char *description() { return "X86"; }
   virtual Kind getKind() { return IsNotExtended; }

   TR_X86OpCode& getOpCode() { return _opcode; }
   TR_X86OpCodes getOpCodeValue() { return _opcode.getOpCodeValue(); }
   TR_X86OpCodes setOpCodeValue(TR_X86OpCodes op) { return _opcode.setOpCodeValue(op); }

   virtual bool isBranchOp() {return _opcode.isBranchOp();}
   virtual bool isLabel();
   virtual bool isRegRegMove();
   virtual bool isPatchBarrier();

   TR::RegisterDependencyConditions *getDependencyConditions() {return _conditions;}
   TR::RegisterDependencyConditions *setDependencyConditions(TR::RegisterDependencyConditions *cond) { return (_conditions = cond); }

   virtual void assignRegisters(TR_RegisterKinds kindsToBeAssigned);
   virtual bool refsRegister(TR::Register *reg);
   virtual bool defsRegister(TR::Register *reg);
   virtual bool usesRegister(TR::Register *reg);
   virtual bool dependencyRefsRegister(TR::Register *reg);

   virtual uint8_t *generateBinaryEncoding();
   virtual int32_t estimateBinaryLength(int32_t currentEstimate);
   virtual EnlargementResult enlarge(int32_t requestedEnlargementSize, int32_t maxEnlargementSize, bool allowPartialEnlargement) { return EnlargementResult(0, 0); }

   enum TR_UpperHalfRefConditions
      {
      // These should be the first two so that booleans work properly
      TR_never,
      TR_always,

      // Some other conditions under which the top half of a register might be used
      TR_if64bitSource,
      TR_ifUses64bitTarget,
      TR_ifUses64bitSourceOrTarget, // A conservative default for when to setIsUpperHalfDead(true).  Shouldn't really be needed.
      TR_ifModifies32or64bitTarget,
      TR_ifModifies32or64bitSource,
      };

   bool registerRefKindApplies(TR_UpperHalfRefConditions when);
   void setIsUpperHalfDead(TR::Register *reg, bool value, TR_UpperHalfRefConditions when=TR_always);
   void aboutToAssignUsedRegister(TR::Register *reg, TR_UpperHalfRefConditions usesUpperHalf=TR_ifUses64bitSourceOrTarget);
   void aboutToAssignDefdRegister(TR::Register *reg, TR_UpperHalfRefConditions defsUpperHalf=TR_never);

   void aboutToAssignRegister(TR::Register *reg, TR_UpperHalfRefConditions usesUpperHalf=TR_ifUses64bitSourceOrTarget, TR_UpperHalfRefConditions defsUpperHalf=TR_never);

   void aboutToAssignMemRef(TR::MemoryReference *memref);

   void aboutToAssignRegDeps(TR_UpperHalfRefConditions usesUpperHalf=TR_ifUses64bitSourceOrTarget, TR_UpperHalfRefConditions defsUpperHalf=TR_never);

   virtual TR::X86RegInstruction *getIA32RegInstruction() { return NULL; }

   virtual TR::X86LabelInstruction *getIA32LabelInstruction() { return NULL; }

#if defined(DEBUG) || defined(PROD_WITH_ASSUMES)
   // The following safe virtual downcast method is used under debug only
   // for assertion checking.
   //
   virtual TR::X86ImmInstruction *getIA32ImmInstruction() { return NULL; }

   virtual uint32_t getNumOperandReferencedGPRegisters() { return 0; }
   virtual uint32_t getNumOperandReferencedFPRegisters() { return 0; }
   uint32_t totalReferencedGPRegisters(TR::CodeGenerator *);
   uint32_t totalReferencedFPRegisters(TR::CodeGenerator *);
#endif

   // AMD64-specific REX prefix calculations
   // Note the polymorphism
   //

   // Number of unnecessary REX bytes for instruction expansion and/or padding
   uint8_t rexRepeatCount();
   // Only repeated REX bytes for expansion/padding are generated
   uint8_t *generateRepeatedRexPrefix(uint8_t *cursor);

#if defined(TR_TARGET_64BIT)
   uint8_t operandSizeRexBits();
   // Each subclass should override this as necessary
   virtual uint8_t rexBits();
#else
   uint8_t rexBits() { return 0; }
#endif

   // Adjust the VFP state to reflect the execution of this instruction.
   //
   virtual void adjustVFPState(TR_VFPState *state, TR::CodeGenerator *cg);

   virtual TR::Register *getTargetRegister() { return NULL; }
   virtual TR::Register *getSourceRegister() { return NULL; }
   virtual TR::Register *getSourceRightRegister() { return NULL; }
   virtual TR::MemoryReference *getMemoryReference() { return NULL; }

   protected:

   // Facility for subclasses to compute effect of a call on the VFP state.
   // If _opcode->isCallOp(), adds vfpAdjustmentForCall; else, calls TR::Instruction ::adjustVFPState.
   //
   void adjustVFPStateForCall(TR_VFPState *state, int32_t vfpAdjustmentForCall, TR::CodeGenerator *cg);
   void clobberRegsForRematerialisation();

   private:

   TR_X86OpCode _opcode;
   TR::RegisterDependencyConditions *_conditions;
   void assumeValidInstruction();

   };

}

}

#endif

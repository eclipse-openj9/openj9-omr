/*******************************************************************************
 * Copyright (c) 2021, 2021 IBM Corp. and others
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

#ifndef OMR_X86_INSTOPCODE_INCL
#define OMR_X86_INSTOPCODE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_INSTOPCODE_CONNECTOR
#define OMR_INSTOPCODE_CONNECTOR
namespace OMR { namespace X86 { class InstOpCode; } }
namespace OMR { typedef OMR::X86::InstOpCode InstOpCodeConnector; }
#else
#error OMR::X86::InstOpCode expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRInstOpCode.hpp"
#include "x/codegen/X86Ops.hpp"

namespace OMR
{

namespace X86
{

class InstOpCode: public OMR::InstOpCode
   {
   enum TR_OpCodeVEX_L : uint8_t
      {
      VEX_L128 = 0x0,
      VEX_L256 = 0x1,
      VEX_L512 = 0x2,
      VEX_L___ = 0x3, // Instruction does not support VEX encoding
      };
   enum TR_OpCodeVEX_v : uint8_t
      {
      VEX_vNONE = 0x0,
      VEX_vReg_ = 0x1,
      };
   enum TR_InstructionREX_W : uint8_t
      {
      REX__ = 0x0,
      REX_W = 0x1,
      };
   enum TR_OpCodePrefix : uint8_t
      {
      PREFIX___ = 0x0,
      PREFIX_66 = 0x1,
      PREFIX_F3 = 0x2,
      PREFIX_F2 = 0x3,
      };
   enum TR_OpCodeEscape : uint8_t
      {
      ESCAPE_____ = 0x0,
      ESCAPE_0F__ = 0x1,
      ESCAPE_0F38 = 0x2,
      ESCAPE_0F3A = 0x3,
      };
   enum TR_OpCodeModRM : uint8_t
      {
      ModRM_NONE = 0x0,
      ModRM_RM__ = 0x1,
      ModRM_MR__ = 0x2,
      ModRM_EXT_ = 0x3,
      };
   enum TR_OpCodeImmediate : uint8_t
      {
      Immediate_0 = 0x0,
      Immediate_1 = 0x1,
      Immediate_2 = 0x2,
      Immediate_4 = 0x3,
      Immediate_8 = 0x4,
      Immediate_S = 0x7,
      };
   struct OpCode_t
      {
      uint8_t vex_l : 2;
      uint8_t vex_v : 1;
      uint8_t prefixes : 2;
      uint8_t rex_w : 1;
      uint8_t escape : 2;
      uint8_t opcode;
      uint8_t modrm_opcode : 3;
      uint8_t modrm_form : 2;
      uint8_t immediate_size : 3;
      inline uint8_t ImmediateSize() const
         {
         switch (immediate_size)
            {
            case Immediate_0:
               return 0;
            case Immediate_S:
            case Immediate_1:
               return 1;
            case Immediate_2:
               return 2;
            case Immediate_4:
               return 4;
            case Immediate_8:
               return 8;
            }
            TR_ASSERT_FATAL(false, "IMPOSSIBLE TO REACH HERE.");
         }
      // check if the instruction can be encoded as AVX
      inline bool supportsAVX() const
         {
         return vex_l != VEX_L___;
         }
      // check if the instruction is X87
      inline bool isX87() const
         {
         return (prefixes == PREFIX___) && (opcode >= 0xd8) && (opcode <= 0xdf);
         }
      // check if the instruction has mandatory prefix(es)
      inline bool hasMandatoryPrefix() const
         {
         return prefixes != PREFIX___;
         }
      // check if the instruction is part of Group 7 OpCode Extension
      inline bool isGroup07() const
         {
         return (escape == ESCAPE_0F__) && (opcode == 0x01);
         }
      // TBuffer should only be one of the two: Estimator when calculating length, and Writer when generating binaries.
      template <class TBuffer> typename TBuffer::cursor_t encode(typename TBuffer::cursor_t cursor, uint8_t rexbits) const;
      // finalize instruction prefix information, currently only in-use for AVX instructions for VEX.vvvv field
      void finalize(uint8_t* cursor) const;
      };
   template <typename TCursor>
   class BufferBase
      {
      public:
      typedef TCursor cursor_t;
      inline operator cursor_t() const
         {
         return cursor;
         }
      protected:
      inline BufferBase(cursor_t cursor) : cursor(cursor) {}
      cursor_t cursor;
      };
   // helper class to calculate length
   class Estimator : public BufferBase<uint8_t>
      {
      public:
      inline Estimator(cursor_t size) : BufferBase<cursor_t>(size) {}
      template <typename T> void inline append(T binaries)
         {
         cursor += sizeof(T);
         }
      };
   // helper class to write binaries
   class Writer : public BufferBase<uint8_t*>
      {
      public:
      inline Writer(cursor_t cursor) : BufferBase<cursor_t>(cursor) {}
      template <typename T> void inline append(T binaries)
         {
         *((T*)cursor) = binaries;
         cursor+= sizeof(T);
         }
      };
   // TBuffer should only be one of the two: Estimator when calculating length, and Writer when generating binaries.
   template <class TBuffer> inline typename TBuffer::cursor_t encode(typename TBuffer::cursor_t cursor, uint8_t rex) const
      {
      return isPseudoOp() ? cursor : info().encode<TBuffer>(cursor, rex);
      }
   // Instructions from Group 7 OpCode Extensions need special handling as they requires specific low 3 bits of ModR/M byte
   inline void CheckAndFinishGroup07(uint8_t* cursor) const;

   TR_X86OpCodes         _opCode;
   static const OpCode_t _binaries[];
   static const uint32_t _properties[];
   static const uint32_t _properties1[];

   protected:

   InstOpCode():  OMR::InstOpCode(bad), _opCode(BADIA32Op)  {}
   InstOpCode(Mnemonic m): OMR::InstOpCode(m), _opCode(BADIA32Op)  {}
   InstOpCode(TR_X86OpCodes op):  OMR::InstOpCode(bad), _opCode(op)        {}

   public:

   struct OpCodeMetaData
      {
      };

   static const OpCodeMetaData metadata[NumOpCodes];

   inline const OpCode_t& info() const                   {return _binaries[_opCode]; }
   inline TR_X86OpCodes getOpCodeValue() const           {return _opCode;}
   inline TR_X86OpCodes setOpCodeValue(TR_X86OpCodes op) {return (_opCode = op);}

   inline uint32_t modifiesTarget()                const {return _properties[_opCode] & IA32OpProp_ModifiesTarget;}
   inline uint32_t modifiesSource()                const {return _properties[_opCode] & IA32OpProp_ModifiesSource;}
   inline uint32_t usesTarget()                    const {return _properties[_opCode] & IA32OpProp_UsesTarget;}
   inline uint32_t singleFPOp()                    const {return _properties[_opCode] & IA32OpProp_SingleFP;}
   inline uint32_t doubleFPOp()                    const {return _properties[_opCode] & IA32OpProp_DoubleFP;}
   inline uint32_t gprOp()                         const {return (_properties[_opCode] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}
   inline uint32_t fprOp()                         const {return (_properties[_opCode] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}
   inline uint32_t hasByteImmediate()              const {return _properties[_opCode] & IA32OpProp_ByteImmediate;}
   inline uint32_t hasShortImmediate()             const {return _properties[_opCode] & IA32OpProp_ShortImmediate;}
   inline uint32_t hasIntImmediate()               const {return _properties[_opCode] & IA32OpProp_IntImmediate;}
   inline uint32_t hasLongImmediate()              const {return _properties1[_opCode] & IA32OpProp1_LongImmediate;}
   inline uint32_t hasSignExtendImmediate()        const {return _properties[_opCode] & IA32OpProp_SignExtendImmediate;}
   inline uint32_t testsZeroFlag()                 const {return _properties[_opCode] & IA32OpProp_TestsZeroFlag;}
   inline uint32_t modifiesZeroFlag()              const {return _properties[_opCode] & IA32OpProp_ModifiesZeroFlag;}
   inline uint32_t testsSignFlag()                 const {return _properties[_opCode] & IA32OpProp_TestsSignFlag;}
   inline uint32_t modifiesSignFlag()              const {return _properties[_opCode] & IA32OpProp_ModifiesSignFlag;}
   inline uint32_t testsCarryFlag()                const {return _properties[_opCode] & IA32OpProp_TestsCarryFlag;}
   inline uint32_t modifiesCarryFlag()             const {return _properties[_opCode] & IA32OpProp_ModifiesCarryFlag;}
   inline uint32_t testsOverflowFlag()             const {return _properties[_opCode] & IA32OpProp_TestsOverflowFlag;}
   inline uint32_t modifiesOverflowFlag()          const {return _properties[_opCode] & IA32OpProp_ModifiesOverflowFlag;}
   inline uint32_t testsParityFlag()               const {return _properties[_opCode] & IA32OpProp_TestsParityFlag;}
   inline uint32_t modifiesParityFlag()            const {return _properties[_opCode] & IA32OpProp_ModifiesParityFlag;}
   inline uint32_t hasByteSource()                 const {return _properties[_opCode] & IA32OpProp_ByteSource;}
   inline uint32_t hasByteTarget()                 const {return _properties[_opCode] & IA32OpProp_ByteTarget;}
   inline uint32_t hasShortSource()                const {return _properties[_opCode] & IA32OpProp_ShortSource;}
   inline uint32_t hasShortTarget()                const {return _properties[_opCode] & IA32OpProp_ShortTarget;}
   inline uint32_t hasIntSource()                  const {return _properties[_opCode] & IA32OpProp_IntSource;}
   inline uint32_t hasIntTarget()                  const {return _properties[_opCode] & IA32OpProp_IntTarget;}
   inline uint32_t hasLongSource()                 const {return _properties1[_opCode] & IA32OpProp1_LongSource;}
   inline uint32_t hasLongTarget()                 const {return _properties1[_opCode] & IA32OpProp1_LongTarget;}
   inline uint32_t hasDoubleWordSource()           const {return _properties1[_opCode] & IA32OpProp1_DoubleWordSource;}
   inline uint32_t hasDoubleWordTarget()           const {return _properties1[_opCode] & IA32OpProp1_DoubleWordTarget;}
   inline uint32_t hasXMMSource()                  const {return _properties1[_opCode] & IA32OpProp1_XMMSource;}
   inline uint32_t hasXMMTarget()                  const {return _properties1[_opCode] & IA32OpProp1_XMMTarget;}
   inline uint32_t isPseudoOp()                    const {return _properties1[_opCode] & IA32OpProp1_PseudoOp;}
   inline uint32_t needsRepPrefix()                const {return _properties1[_opCode] & IA32OpProp1_NeedsRepPrefix;}
   inline uint32_t needsLockPrefix()               const {return _properties1[_opCode] & IA32OpProp1_NeedsLockPrefix;}
   inline uint32_t needsXacquirePrefix()           const {return _properties1[_opCode] & IA32OpProp1_NeedsXacquirePrefix;}
   inline uint32_t needsXreleasePrefix()           const {return _properties1[_opCode] & IA32OpProp1_NeedsXreleasePrefix;}
   inline uint32_t clearsUpperBits()               const {return hasIntTarget() && modifiesTarget();}
   inline uint32_t setsUpperBits()                 const {return hasLongTarget() && modifiesTarget();}
   inline uint32_t hasTargetRegisterInOpcode()     const {return _properties[_opCode] & IA32OpProp_TargetRegisterInOpcode;}
   inline uint32_t hasTargetRegisterInModRM()      const {return _properties[_opCode] & IA32OpProp_TargetRegisterInModRM;}
   inline uint32_t hasTargetRegisterIgnored()      const {return _properties[_opCode] & IA32OpProp_TargetRegisterIgnored;}
   inline uint32_t hasSourceRegisterInModRM()      const {return _properties[_opCode] & IA32OpProp_SourceRegisterInModRM;}
   inline uint32_t hasSourceRegisterIgnored()      const {return _properties[_opCode] & IA32OpProp_SourceRegisterIgnored;}
   inline uint32_t isBranchOp()                    const {return _properties[_opCode] & IA32OpProp_BranchOp;}
   inline uint32_t hasRelativeBranchDisplacement() const { return isBranchOp() || isCallImmOp(); }
   inline uint32_t isShortBranchOp()               const {return isBranchOp() && hasByteImmediate();}
   inline uint32_t testsSomeFlag()                 const {return _properties[_opCode] & IA32OpProp_TestsSomeFlag;}
   inline uint32_t modifiesSomeArithmeticFlags()   const {return _properties[_opCode] & IA32OpProp_SetsSomeArithmeticFlag;}
   inline uint32_t setsCCForCompare()              const {return _properties1[_opCode] & IA32OpProp1_SetsCCForCompare;}
   inline uint32_t setsCCForTest()                 const {return _properties1[_opCode] & IA32OpProp1_SetsCCForTest;}
   inline uint32_t isShiftOp()                     const {return _properties1[_opCode] & IA32OpProp1_ShiftOp;}
   inline uint32_t isRotateOp()                    const {return _properties1[_opCode] & IA32OpProp1_RotateOp;}
   inline uint32_t isPushOp()                      const {return _properties1[_opCode] & IA32OpProp1_PushOp;}
   inline uint32_t isPopOp()                       const {return _properties1[_opCode] & IA32OpProp1_PopOp;}
   inline uint32_t isCallOp()                      const {return isCallOp(_opCode); }
   inline uint32_t isCallImmOp()                   const {return isCallImmOp(_opCode); }
   inline uint32_t supportsLockPrefix()            const {return _properties1[_opCode] & IA32OpProp1_SupportsLockPrefix;}
   inline bool     isConditionalBranchOp()         const {return isBranchOp() && testsSomeFlag();}
   inline uint32_t hasPopInstruction()             const {return _properties[_opCode] & IA32OpProp_HasPopInstruction;}
   inline uint32_t isPopInstruction()              const {return _properties[_opCode] & IA32OpProp_IsPopInstruction;}
   inline uint32_t sourceOpTarget()                const {return _properties[_opCode] & IA32OpProp_SourceOpTarget;}
   inline uint32_t hasDirectionBit()               const {return _properties[_opCode] & IA32OpProp_HasDirectionBit;}
   inline uint32_t isIA32Only()                    const {return _properties1[_opCode] & IA32OpProp1_IsIA32Only;}
   inline uint32_t sourceIsMemRef()                const {return _properties1[_opCode] & IA32OpProp1_SourceIsMemRef;}
   inline uint32_t targetRegIsImplicit()           const { return _properties1[_opCode] & IA32OpProp1_TargetRegIsImplicit;}
   inline uint32_t sourceRegIsImplicit()           const { return _properties1[_opCode] & IA32OpProp1_SourceRegIsImplicit;}
   inline uint32_t isFusableCompare()              const { return _properties1[_opCode] & IA32OpProp1_FusableCompare; }

   inline bool isSetRegInstruction() const
      {
      switch(_opCode)
         {
         case SETA1Reg:
         case SETAE1Reg:
         case SETB1Reg:
         case SETBE1Reg:
         case SETE1Reg:
         case SETNE1Reg:
         case SETG1Reg:
         case SETGE1Reg:
         case SETL1Reg:
         case SETLE1Reg:
         case SETS1Reg:
         case SETNS1Reg:
         case SETPO1Reg:
         case SETPE1Reg:
            return true;
         default:
            return false;
         }
      }

   uint8_t length(uint8_t rex = 0) const;
   uint8_t* binary(uint8_t* cursor, uint8_t rex = 0) const;
   void finalize(uint8_t* cursor) const;
   void convertLongBranchToShort()
      { // input must be a long branch in range JA4 - JMP4
      if (((int)_opCode >= (int)JA4) && ((int)_opCode <= (int)JMP4))
         _opCode = (TR_X86OpCodes)((int)_opCode - IA32LongToShortBranchConversionOffset);
      }

   inline uint8_t getModifiedEFlags() const {return getModifiedEFlags(_opCode); }
   inline uint8_t getTestedEFlags()   const {return getTestedEFlags(_opCode); }

   void trackUpperBitsOnReg(TR::Register *reg, TR::CodeGenerator *cg);

   inline static uint32_t singleFPOp(TR_X86OpCodes op)           {return _properties[op] & IA32OpProp_SingleFP;}
   inline static uint32_t doubleFPOp(TR_X86OpCodes op)           {return _properties[op] & IA32OpProp_DoubleFP;}
   inline static uint32_t gprOp(TR_X86OpCodes op)                {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}
   inline static uint32_t fprOp(TR_X86OpCodes op)                {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}
   inline static uint32_t testsZeroFlag(TR_X86OpCodes op)        {return _properties[op] & IA32OpProp_TestsZeroFlag;}
   inline static uint32_t modifiesZeroFlag(TR_X86OpCodes op)     {return _properties[op] & IA32OpProp_ModifiesZeroFlag;}
   inline static uint32_t testsSignFlag(TR_X86OpCodes op)        {return _properties[op] & IA32OpProp_TestsSignFlag;}
   inline static uint32_t modifiesSignFlag(TR_X86OpCodes op)     {return _properties[op] & IA32OpProp_ModifiesSignFlag;}
   inline static uint32_t testsCarryFlag(TR_X86OpCodes op)       {return _properties[op] & IA32OpProp_TestsCarryFlag;}
   inline static uint32_t modifiesCarryFlag(TR_X86OpCodes op)    {return _properties[op] & IA32OpProp_ModifiesCarryFlag;}
   inline static uint32_t testsOverflowFlag(TR_X86OpCodes op)    {return _properties[op] & IA32OpProp_TestsOverflowFlag;}
   inline static uint32_t modifiesOverflowFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesOverflowFlag;}
   inline static uint32_t testsParityFlag(TR_X86OpCodes op)      {return _properties[op] & IA32OpProp_TestsParityFlag;}
   inline static uint32_t modifiesParityFlag(TR_X86OpCodes op)   {return _properties[op] & IA32OpProp_ModifiesParityFlag;}
   inline static uint32_t isCallOp(TR_X86OpCodes op)             {return _properties1[op] & IA32OpProp1_CallOp;}
   inline static uint32_t isCallImmOp(TR_X86OpCodes op)          {return op == CALLImm4 || op == CALLREXImm4; }

   inline static uint8_t getModifiedEFlags(TR_X86OpCodes op)
      {
      uint8_t flags = 0;
      if (modifiesOverflowFlag(op)) flags |= IA32EFlags_OF;
      if (modifiesSignFlag(op))     flags |= IA32EFlags_SF;
      if (modifiesZeroFlag(op))     flags |= IA32EFlags_ZF;
      if (modifiesParityFlag(op))   flags |= IA32EFlags_PF;
      if (modifiesCarryFlag(op))    flags |= IA32EFlags_CF;
      return flags;
      }

   inline static uint8_t getTestedEFlags(TR_X86OpCodes op)
      {
      uint8_t flags = 0;
      if (testsOverflowFlag(op)) flags |= IA32EFlags_OF;
      if (testsSignFlag(op))     flags |= IA32EFlags_SF;
      if (testsZeroFlag(op))     flags |= IA32EFlags_ZF;
      if (testsParityFlag(op))   flags |= IA32EFlags_PF;
      if (testsCarryFlag(op))    flags |= IA32EFlags_CF;
      return flags;
      }

#if defined(DEBUG)
   const char *getOpCodeName(TR::CodeGenerator *cg);
   const char *getMnemonicName(TR::CodeGenerator *cg);
#endif
   };
}
}
#endif

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

namespace TR { class CodeGenerator; }
namespace TR { class Register; }

#define IA32LongToShortBranchConversionOffset ((int)OMR::InstOpCode::JMP4 - (int)OMR::InstOpCode::JMP1)
#define IA32LengthOfShortBranch               2

#ifdef TR_TARGET_64BIT
#define TARGET_PARAMETERIZED_OPCODE(name, op64, op32) \
inline OMR::InstOpCode::Mnemonic name(bool is64Bit = true) { return is64Bit ? op64 : op32; }
#else
#define TARGET_PARAMETERIZED_OPCODE(name, op64, op32) \
inline OMR::InstOpCode::Mnemonic name(bool is64Bit = false) { return is64Bit ? op64 : op32; }
#endif

TARGET_PARAMETERIZED_OPCODE(BSFRegReg      , OMR::InstOpCode::BSF8RegReg      , OMR::InstOpCode::BSF4RegReg      )
TARGET_PARAMETERIZED_OPCODE(BSWAPReg       , OMR::InstOpCode::BSWAP8Reg       , OMR::InstOpCode::BSWAP4Reg       )
TARGET_PARAMETERIZED_OPCODE(BSRRegReg      , OMR::InstOpCode::BSR8RegReg      , OMR::InstOpCode::BSR4RegReg      )
TARGET_PARAMETERIZED_OPCODE(BTRegReg       , OMR::InstOpCode::BT8RegReg       , OMR::InstOpCode::BT4RegReg       )
TARGET_PARAMETERIZED_OPCODE(CMOVBRegReg    , OMR::InstOpCode::CMOVB8RegReg    , OMR::InstOpCode::CMOVB4RegReg    )
TARGET_PARAMETERIZED_OPCODE(CMOVARegMem    , OMR::InstOpCode::CMOVA8RegMem    , OMR::InstOpCode::CMOVA4RegMem    )
TARGET_PARAMETERIZED_OPCODE(CMOVERegMem    , OMR::InstOpCode::CMOVE8RegMem    , OMR::InstOpCode::CMOVE4RegMem    )
TARGET_PARAMETERIZED_OPCODE(CMOVERegReg    , OMR::InstOpCode::CMOVE8RegReg    , OMR::InstOpCode::CMOVE4RegReg    )
TARGET_PARAMETERIZED_OPCODE(CMOVNERegMem   , OMR::InstOpCode::CMOVNE8RegMem   , OMR::InstOpCode::CMOVNE4RegMem   )
TARGET_PARAMETERIZED_OPCODE(CMOVNERegReg   , OMR::InstOpCode::CMOVNE8RegReg   , OMR::InstOpCode::CMOVNE4RegReg   )
TARGET_PARAMETERIZED_OPCODE(CMOVGERegMem   , OMR::InstOpCode::CMOVGE8RegMem   , OMR::InstOpCode::CMOVGE4RegMem   )
TARGET_PARAMETERIZED_OPCODE(CMOVGRegReg    , OMR::InstOpCode::CMOVG8RegReg    , OMR::InstOpCode::CMOVG4RegReg    )
TARGET_PARAMETERIZED_OPCODE(CMOVGERegReg   , OMR::InstOpCode::CMOVGE8RegReg   , OMR::InstOpCode::CMOVGE4RegReg   )
TARGET_PARAMETERIZED_OPCODE(CMOVLRegReg    , OMR::InstOpCode::CMOVL8RegReg    , OMR::InstOpCode::CMOVL4RegReg    )
TARGET_PARAMETERIZED_OPCODE(CMOVLERegReg   , OMR::InstOpCode::CMOVLE8RegReg   , OMR::InstOpCode::CMOVLE4RegReg   )
TARGET_PARAMETERIZED_OPCODE(CMOVPRegMem    , OMR::InstOpCode::CMOVP8RegMem    , OMR::InstOpCode::CMOVP4RegMem    )
TARGET_PARAMETERIZED_OPCODE(CMOVSRegReg    , OMR::InstOpCode::CMOVS8RegReg    , OMR::InstOpCode::CMOVS4RegReg    )
TARGET_PARAMETERIZED_OPCODE(CMPRegImms     , OMR::InstOpCode::CMP8RegImms     , OMR::InstOpCode::CMP4RegImms     )
TARGET_PARAMETERIZED_OPCODE(CMPRegImm4     , OMR::InstOpCode::CMP8RegImm4     , OMR::InstOpCode::CMP4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(CMPMemImms     , OMR::InstOpCode::CMP8MemImms     , OMR::InstOpCode::CMP4MemImms     )
TARGET_PARAMETERIZED_OPCODE(CMPMemImm4     , OMR::InstOpCode::CMP8MemImm4     , OMR::InstOpCode::CMP4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(MOVRegReg      , OMR::InstOpCode::MOV8RegReg      , OMR::InstOpCode::MOV4RegReg      )
TARGET_PARAMETERIZED_OPCODE(LEARegMem      , OMR::InstOpCode::LEA8RegMem      , OMR::InstOpCode::LEA4RegMem      )
TARGET_PARAMETERIZED_OPCODE(LRegMem        , OMR::InstOpCode::L8RegMem        , OMR::InstOpCode::L4RegMem        )
TARGET_PARAMETERIZED_OPCODE(SMemReg        , OMR::InstOpCode::S8MemReg        , OMR::InstOpCode::S4MemReg        )
TARGET_PARAMETERIZED_OPCODE(SMemImm4       , OMR::InstOpCode::S8MemImm4       , OMR::InstOpCode::S4MemImm4       )
TARGET_PARAMETERIZED_OPCODE(XRSMemImm4     , OMR::InstOpCode::XRS8MemImm4     , OMR::InstOpCode::XRS4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(XCHGRegReg     , OMR::InstOpCode::XCHG8RegReg     , OMR::InstOpCode::XCHG4RegReg     )
TARGET_PARAMETERIZED_OPCODE(NEGReg         , OMR::InstOpCode::NEG8Reg         , OMR::InstOpCode::NEG4Reg         )
TARGET_PARAMETERIZED_OPCODE(IMULRegReg     , OMR::InstOpCode::IMUL8RegReg     , OMR::InstOpCode::IMUL4RegReg     )
TARGET_PARAMETERIZED_OPCODE(IMULRegMem     , OMR::InstOpCode::IMUL8RegMem     , OMR::InstOpCode::IMUL4RegMem     )
TARGET_PARAMETERIZED_OPCODE(IMULRegRegImms , OMR::InstOpCode::IMUL8RegRegImms , OMR::InstOpCode::IMUL4RegRegImms )
TARGET_PARAMETERIZED_OPCODE(IMULRegRegImm4 , OMR::InstOpCode::IMUL8RegRegImm4 , OMR::InstOpCode::IMUL4RegRegImm4 )
TARGET_PARAMETERIZED_OPCODE(IMULRegMemImms , OMR::InstOpCode::IMUL8RegMemImms , OMR::InstOpCode::IMUL4RegMemImms )
TARGET_PARAMETERIZED_OPCODE(IMULRegMemImm4 , OMR::InstOpCode::IMUL8RegMemImm4 , OMR::InstOpCode::IMUL4RegMemImm4 )
TARGET_PARAMETERIZED_OPCODE(INCMem         , OMR::InstOpCode::INC8Mem         , OMR::InstOpCode::INC4Mem         )
TARGET_PARAMETERIZED_OPCODE(INCReg         , OMR::InstOpCode::INC8Reg         , OMR::InstOpCode::INC4Reg         )
TARGET_PARAMETERIZED_OPCODE(DECMem         , OMR::InstOpCode::DEC8Mem         , OMR::InstOpCode::DEC4Mem         )
TARGET_PARAMETERIZED_OPCODE(DECReg         , OMR::InstOpCode::DEC8Reg         , OMR::InstOpCode::DEC4Reg         )
TARGET_PARAMETERIZED_OPCODE(ADDMemImms     , OMR::InstOpCode::ADD8MemImms     , OMR::InstOpCode::ADD4MemImms     )
TARGET_PARAMETERIZED_OPCODE(ADDRegImms     , OMR::InstOpCode::ADD8RegImms     , OMR::InstOpCode::ADD4RegImms     )
TARGET_PARAMETERIZED_OPCODE(ADDRegImm4     , OMR::InstOpCode::ADD8RegImm4     , OMR::InstOpCode::ADD4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(ADCMemImms     , OMR::InstOpCode::ADC8MemImms     , OMR::InstOpCode::ADC4MemImms     )
TARGET_PARAMETERIZED_OPCODE(ADCRegImms     , OMR::InstOpCode::ADC8RegImms     , OMR::InstOpCode::ADC4RegImms     )
TARGET_PARAMETERIZED_OPCODE(ADCRegImm4     , OMR::InstOpCode::ADC8RegImm4     , OMR::InstOpCode::ADC4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(SUBMemImms     , OMR::InstOpCode::SUB8MemImms     , OMR::InstOpCode::SUB4MemImms     )
TARGET_PARAMETERIZED_OPCODE(SUBRegImms     , OMR::InstOpCode::SUB8RegImms     , OMR::InstOpCode::SUB4RegImms     )
TARGET_PARAMETERIZED_OPCODE(SBBMemImms     , OMR::InstOpCode::SBB8MemImms     , OMR::InstOpCode::SBB4MemImms     )
TARGET_PARAMETERIZED_OPCODE(SBBRegImms     , OMR::InstOpCode::SBB8RegImms     , OMR::InstOpCode::SBB4RegImms     )
TARGET_PARAMETERIZED_OPCODE(ADDMemImm4     , OMR::InstOpCode::ADD8MemImm4     , OMR::InstOpCode::ADD4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(ADDMemReg      , OMR::InstOpCode::ADD8MemReg      , OMR::InstOpCode::ADD4MemReg      )
TARGET_PARAMETERIZED_OPCODE(ADDRegReg      , OMR::InstOpCode::ADD8RegReg      , OMR::InstOpCode::ADD4RegReg      )
TARGET_PARAMETERIZED_OPCODE(ADDRegMem      , OMR::InstOpCode::ADD8RegMem      , OMR::InstOpCode::ADD4RegMem      )
TARGET_PARAMETERIZED_OPCODE(LADDMemReg     , OMR::InstOpCode::LADD8MemReg     , OMR::InstOpCode::LADD4MemReg     )
TARGET_PARAMETERIZED_OPCODE(LXADDMemReg    , OMR::InstOpCode::LXADD8MemReg    , OMR::InstOpCode::LXADD4MemReg    )
TARGET_PARAMETERIZED_OPCODE(ADCMemImm4     , OMR::InstOpCode::ADC8MemImm4     , OMR::InstOpCode::ADC4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(ADCMemReg      , OMR::InstOpCode::ADC8MemReg      , OMR::InstOpCode::ADC4MemReg      )
TARGET_PARAMETERIZED_OPCODE(ADCRegReg      , OMR::InstOpCode::ADC8RegReg      , OMR::InstOpCode::ADC4RegReg      )
TARGET_PARAMETERIZED_OPCODE(ADCRegMem      , OMR::InstOpCode::ADC8RegMem      , OMR::InstOpCode::ADC4RegMem      )
TARGET_PARAMETERIZED_OPCODE(SUBMemImm4     , OMR::InstOpCode::SUB8MemImm4     , OMR::InstOpCode::SUB4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(SUBRegImm4     , OMR::InstOpCode::SUB8RegImm4     , OMR::InstOpCode::SUB4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(SUBMemReg      , OMR::InstOpCode::SUB8MemReg      , OMR::InstOpCode::SUB4MemReg      )
TARGET_PARAMETERIZED_OPCODE(SUBRegReg      , OMR::InstOpCode::SUB8RegReg      , OMR::InstOpCode::SUB4RegReg      )
TARGET_PARAMETERIZED_OPCODE(SUBRegMem      , OMR::InstOpCode::SUB8RegMem      , OMR::InstOpCode::SUB4RegMem      )
TARGET_PARAMETERIZED_OPCODE(SBBMemImm4     , OMR::InstOpCode::SBB8MemImm4     , OMR::InstOpCode::SBB4MemImm4     )
TARGET_PARAMETERIZED_OPCODE(SBBRegImm4     , OMR::InstOpCode::SBB8RegImm4     , OMR::InstOpCode::SBB4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(SBBMemReg      , OMR::InstOpCode::SBB8MemReg      , OMR::InstOpCode::SBB4MemReg      )
TARGET_PARAMETERIZED_OPCODE(SBBRegReg      , OMR::InstOpCode::SBB8RegReg      , OMR::InstOpCode::SBB4RegReg      )
TARGET_PARAMETERIZED_OPCODE(SBBRegMem      , OMR::InstOpCode::SBB8RegMem      , OMR::InstOpCode::SBB4RegMem      )
TARGET_PARAMETERIZED_OPCODE(ORRegImm4      , OMR::InstOpCode::OR8RegImm4      , OMR::InstOpCode::OR4RegImm4      )
TARGET_PARAMETERIZED_OPCODE(ORRegImms      , OMR::InstOpCode::OR8RegImms      , OMR::InstOpCode::OR4RegImms      )
TARGET_PARAMETERIZED_OPCODE(ORRegMem       , OMR::InstOpCode::OR8RegMem       , OMR::InstOpCode::OR4RegMem       )
TARGET_PARAMETERIZED_OPCODE(XORRegMem      , OMR::InstOpCode::XOR8RegMem      , OMR::InstOpCode::XOR4RegMem      )
TARGET_PARAMETERIZED_OPCODE(XORRegImms     , OMR::InstOpCode::XOR8RegImms     , OMR::InstOpCode::XOR4RegImms     )
TARGET_PARAMETERIZED_OPCODE(XORRegImm4     , OMR::InstOpCode::XOR8RegImm4     , OMR::InstOpCode::XOR4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(CXXAcc         , OMR::InstOpCode::CQOAcc          , OMR::InstOpCode::CDQAcc          )
TARGET_PARAMETERIZED_OPCODE(ANDRegImm4     , OMR::InstOpCode::AND8RegImm4     , OMR::InstOpCode::AND4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(ANDRegReg      , OMR::InstOpCode::AND8RegReg      , OMR::InstOpCode::AND4RegReg      )
TARGET_PARAMETERIZED_OPCODE(ANDRegImms     , OMR::InstOpCode::AND8RegImms     , OMR::InstOpCode::AND4RegImms     )
TARGET_PARAMETERIZED_OPCODE(ORRegReg       , OMR::InstOpCode::OR8RegReg       , OMR::InstOpCode::OR4RegReg       )
TARGET_PARAMETERIZED_OPCODE(MOVRegImm4     , OMR::InstOpCode::MOV8RegImm4     , OMR::InstOpCode::MOV4RegImm4     )
TARGET_PARAMETERIZED_OPCODE(IMULAccReg     , OMR::InstOpCode::IMUL8AccReg     , OMR::InstOpCode::IMUL4AccReg     )
TARGET_PARAMETERIZED_OPCODE(IDIVAccMem     , OMR::InstOpCode::IDIV8AccMem     , OMR::InstOpCode::IDIV4AccMem     )
TARGET_PARAMETERIZED_OPCODE(IDIVAccReg     , OMR::InstOpCode::IDIV8AccReg     , OMR::InstOpCode::IDIV4AccReg     )
TARGET_PARAMETERIZED_OPCODE(DIVAccMem      , OMR::InstOpCode::DIV8AccMem      , OMR::InstOpCode::DIV4AccMem      )
TARGET_PARAMETERIZED_OPCODE(DIVAccReg      , OMR::InstOpCode::DIV8AccReg      , OMR::InstOpCode::DIV4AccReg      )
TARGET_PARAMETERIZED_OPCODE(NOTReg         , OMR::InstOpCode::NOT8Reg         , OMR::InstOpCode::NOT4Reg         )
TARGET_PARAMETERIZED_OPCODE(POPCNTRegReg   , OMR::InstOpCode::POPCNT8RegReg   , OMR::InstOpCode::POPCNT4RegReg   )
TARGET_PARAMETERIZED_OPCODE(ROLRegImm1     , OMR::InstOpCode::ROL8RegImm1     , OMR::InstOpCode::ROL4RegImm1     )
TARGET_PARAMETERIZED_OPCODE(ROLRegCL       , OMR::InstOpCode::ROL8RegCL       , OMR::InstOpCode::ROL4RegCL       )
TARGET_PARAMETERIZED_OPCODE(SHLMemImm1     , OMR::InstOpCode::SHL8MemImm1     , OMR::InstOpCode::SHL4MemImm1     )
TARGET_PARAMETERIZED_OPCODE(SHLMemCL       , OMR::InstOpCode::SHL8MemCL       , OMR::InstOpCode::SHL4MemCL       )
TARGET_PARAMETERIZED_OPCODE(SHLRegImm1     , OMR::InstOpCode::SHL8RegImm1     , OMR::InstOpCode::SHL4RegImm1     )
TARGET_PARAMETERIZED_OPCODE(SHLRegCL       , OMR::InstOpCode::SHL8RegCL       , OMR::InstOpCode::SHL4RegCL       )
TARGET_PARAMETERIZED_OPCODE(SARMemImm1     , OMR::InstOpCode::SAR8MemImm1     , OMR::InstOpCode::SAR4MemImm1     )
TARGET_PARAMETERIZED_OPCODE(SARMemCL       , OMR::InstOpCode::SAR8MemCL       , OMR::InstOpCode::SAR4MemCL       )
TARGET_PARAMETERIZED_OPCODE(SARRegImm1     , OMR::InstOpCode::SAR8RegImm1     , OMR::InstOpCode::SAR4RegImm1     )
TARGET_PARAMETERIZED_OPCODE(SARRegCL       , OMR::InstOpCode::SAR8RegCL       , OMR::InstOpCode::SAR4RegCL       )
TARGET_PARAMETERIZED_OPCODE(SHRMemImm1     , OMR::InstOpCode::SHR8MemImm1     , OMR::InstOpCode::SHR4MemImm1     )
TARGET_PARAMETERIZED_OPCODE(SHRMemCL       , OMR::InstOpCode::SHR8MemCL       , OMR::InstOpCode::SHR4MemCL       )
TARGET_PARAMETERIZED_OPCODE(SHRReg1        , OMR::InstOpCode::SHR8Reg1        , OMR::InstOpCode::SHR4Reg1        )
TARGET_PARAMETERIZED_OPCODE(SHRRegImm1     , OMR::InstOpCode::SHR8RegImm1     , OMR::InstOpCode::SHR4RegImm1     )
TARGET_PARAMETERIZED_OPCODE(SHRRegCL       , OMR::InstOpCode::SHR8RegCL       , OMR::InstOpCode::SHR4RegCL       )
TARGET_PARAMETERIZED_OPCODE(TESTMemImm4    , OMR::InstOpCode::TEST8MemImm4    , OMR::InstOpCode::TEST4MemImm4    )
TARGET_PARAMETERIZED_OPCODE(TESTRegImm4    , OMR::InstOpCode::TEST8RegImm4    , OMR::InstOpCode::TEST4RegImm4    )
TARGET_PARAMETERIZED_OPCODE(TESTRegReg     , OMR::InstOpCode::TEST8RegReg     , OMR::InstOpCode::TEST4RegReg     )
TARGET_PARAMETERIZED_OPCODE(TESTMemReg     , OMR::InstOpCode::TEST8MemReg     , OMR::InstOpCode::TEST4MemReg     )
TARGET_PARAMETERIZED_OPCODE(XORRegReg      , OMR::InstOpCode::XOR8RegReg      , OMR::InstOpCode::XOR4RegReg      )
TARGET_PARAMETERIZED_OPCODE(CMPRegReg      , OMR::InstOpCode::CMP8RegReg      , OMR::InstOpCode::CMP4RegReg      )
TARGET_PARAMETERIZED_OPCODE(CMPRegMem      , OMR::InstOpCode::CMP8RegMem      , OMR::InstOpCode::CMP4RegMem      )
TARGET_PARAMETERIZED_OPCODE(CMPMemReg      , OMR::InstOpCode::CMP8MemReg      , OMR::InstOpCode::CMP4MemReg      )
TARGET_PARAMETERIZED_OPCODE(CMPXCHGMemReg  , OMR::InstOpCode::CMPXCHG8MemReg  , OMR::InstOpCode::CMPXCHG4MemReg  )
TARGET_PARAMETERIZED_OPCODE(LCMPXCHGMemReg , OMR::InstOpCode::LCMPXCHG8MemReg , OMR::InstOpCode::LCMPXCHG4MemReg )
TARGET_PARAMETERIZED_OPCODE(REPSTOS        , OMR::InstOpCode::REPSTOSQ        , OMR::InstOpCode::REPSTOSD        )
// Floating-point
TARGET_PARAMETERIZED_OPCODE(MOVSMemReg     , OMR::InstOpCode::MOVSDMemReg     , OMR::InstOpCode::MOVSSMemReg     )
TARGET_PARAMETERIZED_OPCODE(MOVSRegMem     , OMR::InstOpCode::MOVSDRegMem     , OMR::InstOpCode::MOVSSRegMem     )
// FMA
TARGET_PARAMETERIZED_OPCODE(VFMADD231SRegRegReg, OMR::InstOpCode::VFMADD231SDRegRegReg, OMR::InstOpCode::VFMADD231SSRegRegReg)

#define TARGET_CARRY_PARAMETERIZED_OPCODE(name, op64, op32) \
inline OMR::InstOpCode::Mnemonic name(bool is64Bit, bool isWithCarry) { return isWithCarry ? op64(is64Bit) : op32(is64Bit); }

// Size and carry-parameterized opcodes
//
TARGET_CARRY_PARAMETERIZED_OPCODE(AddMemImms , ADCMemImms , ADDMemImms)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddRegImms , ADCRegImms , ADDRegImms)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddRegImm4 , ADCRegImm4 , ADDRegImm4)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddMemImm4 , ADCMemImm4 , ADDMemImm4)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddMemReg  , ADCMemReg  , ADDMemReg)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddRegReg  , ADCRegReg  , ADDRegReg)
TARGET_CARRY_PARAMETERIZED_OPCODE(AddRegMem  , ADCRegMem  , ADDRegMem)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubMemImms , SBBMemImms , SUBMemImms)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubRegImms , SBBRegImms , SUBRegImms)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubRegImm4 , SBBRegImm4 , SUBRegImm4)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubMemImm4 , SBBMemImm4 , SUBMemImm4)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubMemReg  , SBBMemReg  , SUBMemReg)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubRegReg  , SBBRegReg  , SUBRegReg)
TARGET_CARRY_PARAMETERIZED_OPCODE(SubRegMem  , SBBRegMem  , SUBRegMem)

#ifdef TR_TARGET_64BIT
#define SIZE_PARAMETERIZED_OPCODE(name, op64, op32, op16, op8)        \
inline OMR::InstOpCode::Mnemonic name(int32_t size = 8)               \
   {                                                                  \
   OMR::InstOpCode::Mnemonic op = OMR::InstOpCode::bad;               \
   switch (size)                                                      \
      {                                                               \
      case 8: return op64;                                            \
      case 4: return op32;                                            \
      case 2: return op16;                                            \
      case 1: return op8;                                             \
      default:                                                        \
         TR_ASSERT_FATAL(false, "Unsupported operand size %d", size); \
         return OMR::InstOpCode::bad;                                 \
      }                                                               \
   }
#else
#define SIZE_PARAMETERIZED_OPCODE(name, op64, op32, op16, op8)        \
inline OMR::InstOpCode::Mnemonic name(int32_t size = 4)               \
   {                                                                  \
   OMR::InstOpCode::Mnemonic op = OMR::InstOpCode::bad;               \
   switch (size)                                                      \
      {                                                               \
      case 8: return op64;                                            \
      case 4: return op32;                                            \
      case 2: return op16;                                            \
      case 1: return op8;                                             \
      default:                                                        \
         TR_ASSERT_FATAL(false, "Unsupported operand size %d", size); \
         return OMR::InstOpCode::bad;                                 \
      }                                                               \
   }
#endif

// Data type based opcodes
SIZE_PARAMETERIZED_OPCODE(MovRegReg      , OMR::InstOpCode::MOV8RegReg      , OMR::InstOpCode::MOV4RegReg      , OMR::InstOpCode::MOV2RegReg      , OMR::InstOpCode::MOV1RegReg  )
SIZE_PARAMETERIZED_OPCODE(IMulRegReg     , OMR::InstOpCode::IMUL8RegReg     , OMR::InstOpCode::IMUL4RegReg     , OMR::InstOpCode::IMUL2RegReg     , OMR::InstOpCode::IMUL1AccReg )
SIZE_PARAMETERIZED_OPCODE(IMulRegMem     , OMR::InstOpCode::IMUL8RegMem     , OMR::InstOpCode::IMUL4RegMem     , OMR::InstOpCode::IMUL2RegMem     , OMR::InstOpCode::IMUL1AccMem )
SIZE_PARAMETERIZED_OPCODE(IMulRegRegImms , OMR::InstOpCode::IMUL8RegRegImms , OMR::InstOpCode::IMUL4RegRegImms , OMR::InstOpCode::IMUL2RegRegImms , OMR::InstOpCode::bad         )
SIZE_PARAMETERIZED_OPCODE(IMulRegRegImm4 , OMR::InstOpCode::IMUL8RegRegImm4 , OMR::InstOpCode::IMUL4RegRegImm4 , OMR::InstOpCode::IMUL2RegRegImm2 , OMR::InstOpCode::bad         )
SIZE_PARAMETERIZED_OPCODE(IMulRegMemImms , OMR::InstOpCode::IMUL8RegMemImms , OMR::InstOpCode::IMUL4RegMemImms , OMR::InstOpCode::IMUL2RegMemImms , OMR::InstOpCode::bad         )
SIZE_PARAMETERIZED_OPCODE(IMulRegMemImm4 , OMR::InstOpCode::IMUL8RegMemImm4 , OMR::InstOpCode::IMUL4RegMemImm4 , OMR::InstOpCode::IMUL2RegMemImm2 , OMR::InstOpCode::bad         )

// Property flags
//
#define IA32OpProp_ModifiesTarget               0x00000001
#define IA32OpProp_ModifiesSource               0x00000002
#define IA32OpProp_UsesTarget                   0x00000004
#define IA32OpProp_SingleFP                     0x00000008
#define IA32OpProp_DoubleFP                     0x00000010
#define IA32OpProp_ByteImmediate                0x00000020
#define IA32OpProp_ShortImmediate               0x00000040
#define IA32OpProp_IntImmediate                 0x00000080
#define IA32OpProp_SignExtendImmediate          0x00000100
#define IA32OpProp_TestsZeroFlag                0x00000200
#define IA32OpProp_ModifiesZeroFlag             0x00000400
#define IA32OpProp_TestsSignFlag                0x00000800
#define IA32OpProp_ModifiesSignFlag             0x00001000
#define IA32OpProp_TestsCarryFlag               0x00002000
#define IA32OpProp_ModifiesCarryFlag            0x00004000
#define IA32OpProp_TestsOverflowFlag            0x00008000
#define IA32OpProp_ModifiesOverflowFlag         0x00010000
#define IA32OpProp_ByteSource                   0x00020000
#define IA32OpProp_ByteTarget                   0x00040000
#define IA32OpProp_ShortSource                  0x00080000
#define IA32OpProp_ShortTarget                  0x00100000
#define IA32OpProp_IntSource                    0x00200000
#define IA32OpProp_IntTarget                    0x00400000
#define IA32OpProp_TestsParityFlag              0x00800000
#define IA32OpProp_ModifiesParityFlag           0x01000000
#define IA32OpProp_TargetRegisterInOpcode       0x04000000
#define IA32OpProp_TargetRegisterInModRM        0x08000000
#define IA32OpProp_TargetRegisterIgnored        0x10000000
#define IA32OpProp_SourceRegisterInModRM        0x20000000
#define IA32OpProp_SourceRegisterIgnored        0x40000000
#define IA32OpProp_BranchOp                     0x80000000

// Since floating point instructions do not have immediate or byte forms,
// these flags can be overloaded.
//
#define IA32OpProp_HasPopInstruction            0x00000020
#define IA32OpProp_IsPopInstruction             0x00000040
#define IA32OpProp_SourceOpTarget               0x00000080
#define IA32OpProp_HasDirectionBit              0x00000100

#define IA32OpProp1_PushOp                    0x00000001
#define IA32OpProp1_PopOp                     0x00000002
#define IA32OpProp1_ShiftOp                   0x00000004
#define IA32OpProp1_RotateOp                  0x00000008
#define IA32OpProp1_SetsCCForCompare          0x00000010
#define IA32OpProp1_SetsCCForTest             0x00000020
#define IA32OpProp1_SupportsLockPrefix        0x00000040
#define IA32OpProp1_DoubleWordSource          0x00000100
#define IA32OpProp1_DoubleWordTarget          0x00000200
#define IA32OpProp1_XMMSource                 0x00000400
#define IA32OpProp1_XMMTarget                 0x00000800
#define IA32OpProp1_PseudoOp                  0x00001000
#define IA32OpProp1_NeedsRepPrefix            0x00002000
#define IA32OpProp1_NeedsLockPrefix           0x00004000
#define IA32OpProp1_CallOp                    0x00010000
#define IA32OpProp1_SourceIsMemRef            0x00020000
#define IA32OpProp1_SourceRegIsImplicit       0x00040000
#define IA32OpProp1_TargetRegIsImplicit       0x00080000
#define IA32OpProp1_FusableCompare            0x00100000
#define IA32OpProp1_NeedsXacquirePrefix       0x00400000
#define IA32OpProp1_NeedsXreleasePrefix       0x00800000
////////////////////
//
// AMD64 flags
//
#define IA32OpProp1_LongSource                0x80000000
#define IA32OpProp1_LongTarget                0x40000000
#define IA32OpProp1_LongImmediate             0x20000000 // MOV and DQ only

// For instructions not supported on AMD64
//
#define IA32OpProp1_IsIA32Only                0x08000000


// All testing properties, used to distinguish a conditional branch from an
// unconditional branch
//
#define IA32OpProp_TestsSomeFlag          (IA32OpProp_TestsZeroFlag     | \
                                           IA32OpProp_TestsSignFlag     | \
                                           IA32OpProp_TestsCarryFlag    | \
                                           IA32OpProp_TestsOverflowFlag | \
                                           IA32OpProp_TestsParityFlag)

#define IA32OpProp_SetsSomeArithmeticFlag (IA32OpProp_ModifiesZeroFlag     | \
                                           IA32OpProp_ModifiesSignFlag     | \
                                           IA32OpProp_ModifiesCarryFlag    | \
                                           IA32OpProp_ModifiesOverflowFlag)


typedef enum
   {
   IA32EFlags_OF = 0x01,
   IA32EFlags_SF = 0x02,
   IA32EFlags_ZF = 0x04,
   IA32EFlags_PF = 0x08,
   IA32EFlags_CF = 0x10
   } TR_EFlags;


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

   static const OpCode_t _binaries[];
   static const uint32_t _properties[];
   static const uint32_t _properties1[];

   protected:

   InstOpCode():  OMR::InstOpCode(OMR::InstOpCode::bad) {}
   InstOpCode(Mnemonic m): OMR::InstOpCode(m) {}

   public:

   struct OpCodeMetaData
      {
      };

   static const OpCodeMetaData metadata[NumOpCodes];

   inline const OpCode_t& info() const                   {return _binaries[_mnemonic]; }
   inline OMR::InstOpCode::Mnemonic getOpCodeValue() const           {return _mnemonic;}
   inline OMR::InstOpCode::Mnemonic setOpCodeValue(OMR::InstOpCode::Mnemonic op) {return (_mnemonic = op);}

   inline uint32_t modifiesTarget()                const {return _properties[_mnemonic] & IA32OpProp_ModifiesTarget;}
   inline uint32_t modifiesSource()                const {return _properties[_mnemonic] & IA32OpProp_ModifiesSource;}
   inline uint32_t usesTarget()                    const {return _properties[_mnemonic] & IA32OpProp_UsesTarget;}
   inline uint32_t singleFPOp()                    const {return _properties[_mnemonic] & IA32OpProp_SingleFP;}
   inline uint32_t doubleFPOp()                    const {return _properties[_mnemonic] & IA32OpProp_DoubleFP;}
   inline uint32_t gprOp()                         const {return (_properties[_mnemonic] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}
   inline uint32_t fprOp()                         const {return (_properties[_mnemonic] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}
   inline uint32_t hasByteImmediate()              const {return _properties[_mnemonic] & IA32OpProp_ByteImmediate;}
   inline uint32_t hasShortImmediate()             const {return _properties[_mnemonic] & IA32OpProp_ShortImmediate;}
   inline uint32_t hasIntImmediate()               const {return _properties[_mnemonic] & IA32OpProp_IntImmediate;}
   inline uint32_t hasLongImmediate()              const {return _properties1[_mnemonic] & IA32OpProp1_LongImmediate;}
   inline uint32_t hasSignExtendImmediate()        const {return _properties[_mnemonic] & IA32OpProp_SignExtendImmediate;}
   inline uint32_t testsZeroFlag()                 const {return _properties[_mnemonic] & IA32OpProp_TestsZeroFlag;}
   inline uint32_t modifiesZeroFlag()              const {return _properties[_mnemonic] & IA32OpProp_ModifiesZeroFlag;}
   inline uint32_t testsSignFlag()                 const {return _properties[_mnemonic] & IA32OpProp_TestsSignFlag;}
   inline uint32_t modifiesSignFlag()              const {return _properties[_mnemonic] & IA32OpProp_ModifiesSignFlag;}
   inline uint32_t testsCarryFlag()                const {return _properties[_mnemonic] & IA32OpProp_TestsCarryFlag;}
   inline uint32_t modifiesCarryFlag()             const {return _properties[_mnemonic] & IA32OpProp_ModifiesCarryFlag;}
   inline uint32_t testsOverflowFlag()             const {return _properties[_mnemonic] & IA32OpProp_TestsOverflowFlag;}
   inline uint32_t modifiesOverflowFlag()          const {return _properties[_mnemonic] & IA32OpProp_ModifiesOverflowFlag;}
   inline uint32_t testsParityFlag()               const {return _properties[_mnemonic] & IA32OpProp_TestsParityFlag;}
   inline uint32_t modifiesParityFlag()            const {return _properties[_mnemonic] & IA32OpProp_ModifiesParityFlag;}
   inline uint32_t hasByteSource()                 const {return _properties[_mnemonic] & IA32OpProp_ByteSource;}
   inline uint32_t hasByteTarget()                 const {return _properties[_mnemonic] & IA32OpProp_ByteTarget;}
   inline uint32_t hasShortSource()                const {return _properties[_mnemonic] & IA32OpProp_ShortSource;}
   inline uint32_t hasShortTarget()                const {return _properties[_mnemonic] & IA32OpProp_ShortTarget;}
   inline uint32_t hasIntSource()                  const {return _properties[_mnemonic] & IA32OpProp_IntSource;}
   inline uint32_t hasIntTarget()                  const {return _properties[_mnemonic] & IA32OpProp_IntTarget;}
   inline uint32_t hasLongSource()                 const {return _properties1[_mnemonic] & IA32OpProp1_LongSource;}
   inline uint32_t hasLongTarget()                 const {return _properties1[_mnemonic] & IA32OpProp1_LongTarget;}
   inline uint32_t hasDoubleWordSource()           const {return _properties1[_mnemonic] & IA32OpProp1_DoubleWordSource;}
   inline uint32_t hasDoubleWordTarget()           const {return _properties1[_mnemonic] & IA32OpProp1_DoubleWordTarget;}
   inline uint32_t hasXMMSource()                  const {return _properties1[_mnemonic] & IA32OpProp1_XMMSource;}
   inline uint32_t hasXMMTarget()                  const {return _properties1[_mnemonic] & IA32OpProp1_XMMTarget;}
   inline uint32_t isPseudoOp()                    const {return _properties1[_mnemonic] & IA32OpProp1_PseudoOp;}
   inline uint32_t needsRepPrefix()                const {return _properties1[_mnemonic] & IA32OpProp1_NeedsRepPrefix;}
   inline uint32_t needsLockPrefix()               const {return _properties1[_mnemonic] & IA32OpProp1_NeedsLockPrefix;}
   inline uint32_t needsXacquirePrefix()           const {return _properties1[_mnemonic] & IA32OpProp1_NeedsXacquirePrefix;}
   inline uint32_t needsXreleasePrefix()           const {return _properties1[_mnemonic] & IA32OpProp1_NeedsXreleasePrefix;}
   inline uint32_t clearsUpperBits()               const {return hasIntTarget() && modifiesTarget();}
   inline uint32_t setsUpperBits()                 const {return hasLongTarget() && modifiesTarget();}
   inline uint32_t hasTargetRegisterInOpcode()     const {return _properties[_mnemonic] & IA32OpProp_TargetRegisterInOpcode;}
   inline uint32_t hasTargetRegisterInModRM()      const {return _properties[_mnemonic] & IA32OpProp_TargetRegisterInModRM;}
   inline uint32_t hasTargetRegisterIgnored()      const {return _properties[_mnemonic] & IA32OpProp_TargetRegisterIgnored;}
   inline uint32_t hasSourceRegisterInModRM()      const {return _properties[_mnemonic] & IA32OpProp_SourceRegisterInModRM;}
   inline uint32_t hasSourceRegisterIgnored()      const {return _properties[_mnemonic] & IA32OpProp_SourceRegisterIgnored;}
   inline uint32_t isBranchOp()                    const {return _properties[_mnemonic] & IA32OpProp_BranchOp;}
   inline uint32_t hasRelativeBranchDisplacement() const { return isBranchOp() || isCallImmOp(); }
   inline uint32_t isShortBranchOp()               const {return isBranchOp() && hasByteImmediate();}
   inline uint32_t testsSomeFlag()                 const {return _properties[_mnemonic] & IA32OpProp_TestsSomeFlag;}
   inline uint32_t modifiesSomeArithmeticFlags()   const {return _properties[_mnemonic] & IA32OpProp_SetsSomeArithmeticFlag;}
   inline uint32_t setsCCForCompare()              const {return _properties1[_mnemonic] & IA32OpProp1_SetsCCForCompare;}
   inline uint32_t setsCCForTest()                 const {return _properties1[_mnemonic] & IA32OpProp1_SetsCCForTest;}
   inline uint32_t isShiftOp()                     const {return _properties1[_mnemonic] & IA32OpProp1_ShiftOp;}
   inline uint32_t isRotateOp()                    const {return _properties1[_mnemonic] & IA32OpProp1_RotateOp;}
   inline uint32_t isPushOp()                      const {return _properties1[_mnemonic] & IA32OpProp1_PushOp;}
   inline uint32_t isPopOp()                       const {return _properties1[_mnemonic] & IA32OpProp1_PopOp;}
   inline uint32_t isCallOp()                      const {return isCallOp(_mnemonic); }
   inline uint32_t isCallImmOp()                   const {return isCallImmOp(_mnemonic); }
   inline uint32_t supportsLockPrefix()            const {return _properties1[_mnemonic] & IA32OpProp1_SupportsLockPrefix;}
   inline bool     isConditionalBranchOp()         const {return isBranchOp() && testsSomeFlag();}
   inline uint32_t hasPopInstruction()             const {return _properties[_mnemonic] & IA32OpProp_HasPopInstruction;}
   inline uint32_t isPopInstruction()              const {return _properties[_mnemonic] & IA32OpProp_IsPopInstruction;}
   inline uint32_t sourceOpTarget()                const {return _properties[_mnemonic] & IA32OpProp_SourceOpTarget;}
   inline uint32_t hasDirectionBit()               const {return _properties[_mnemonic] & IA32OpProp_HasDirectionBit;}
   inline uint32_t isIA32Only()                    const {return _properties1[_mnemonic] & IA32OpProp1_IsIA32Only;}
   inline uint32_t sourceIsMemRef()                const {return _properties1[_mnemonic] & IA32OpProp1_SourceIsMemRef;}
   inline uint32_t targetRegIsImplicit()           const { return _properties1[_mnemonic] & IA32OpProp1_TargetRegIsImplicit;}
   inline uint32_t sourceRegIsImplicit()           const { return _properties1[_mnemonic] & IA32OpProp1_SourceRegIsImplicit;}
   inline uint32_t isFusableCompare()              const { return _properties1[_mnemonic] & IA32OpProp1_FusableCompare; }

   inline bool isSetRegInstruction() const
      {
      switch(_mnemonic)
         {
         case OMR::InstOpCode::SETA1Reg:
         case OMR::InstOpCode::SETAE1Reg:
         case OMR::InstOpCode::SETB1Reg:
         case OMR::InstOpCode::SETBE1Reg:
         case OMR::InstOpCode::SETE1Reg:
         case OMR::InstOpCode::SETNE1Reg:
         case OMR::InstOpCode::SETG1Reg:
         case OMR::InstOpCode::SETGE1Reg:
         case OMR::InstOpCode::SETL1Reg:
         case OMR::InstOpCode::SETLE1Reg:
         case OMR::InstOpCode::SETS1Reg:
         case OMR::InstOpCode::SETNS1Reg:
         case OMR::InstOpCode::SETPO1Reg:
         case OMR::InstOpCode::SETPE1Reg:
            return true;
         default:
            return false;
         }
      }

   uint8_t length(uint8_t rex = 0) const;
   uint8_t* binary(uint8_t* cursor, uint8_t rex = 0) const;
   void finalize(uint8_t* cursor) const;
   void convertLongBranchToShort()
      { // input must be a long branch in range OMR::InstOpCode::JA4 - OMR::InstOpCode::JMP4
      if (((int)_mnemonic >= (int)OMR::InstOpCode::JA4) && ((int)_mnemonic <= (int)OMR::InstOpCode::JMP4))
         _mnemonic = (OMR::InstOpCode::Mnemonic)((int)_mnemonic - IA32LongToShortBranchConversionOffset);
      }

   inline uint8_t getModifiedEFlags() const {return getModifiedEFlags(_mnemonic); }
   inline uint8_t getTestedEFlags()   const {return getTestedEFlags(_mnemonic); }

   void trackUpperBitsOnReg(TR::Register *reg, TR::CodeGenerator *cg);

   inline static uint32_t singleFPOp(OMR::InstOpCode::Mnemonic op)           {return _properties[op] & IA32OpProp_SingleFP;}
   inline static uint32_t doubleFPOp(OMR::InstOpCode::Mnemonic op)           {return _properties[op] & IA32OpProp_DoubleFP;}
   inline static uint32_t gprOp(OMR::InstOpCode::Mnemonic op)                {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}
   inline static uint32_t fprOp(OMR::InstOpCode::Mnemonic op)                {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}
   inline static uint32_t testsZeroFlag(OMR::InstOpCode::Mnemonic op)        {return _properties[op] & IA32OpProp_TestsZeroFlag;}
   inline static uint32_t modifiesZeroFlag(OMR::InstOpCode::Mnemonic op)     {return _properties[op] & IA32OpProp_ModifiesZeroFlag;}
   inline static uint32_t testsSignFlag(OMR::InstOpCode::Mnemonic op)        {return _properties[op] & IA32OpProp_TestsSignFlag;}
   inline static uint32_t modifiesSignFlag(OMR::InstOpCode::Mnemonic op)     {return _properties[op] & IA32OpProp_ModifiesSignFlag;}
   inline static uint32_t testsCarryFlag(OMR::InstOpCode::Mnemonic op)       {return _properties[op] & IA32OpProp_TestsCarryFlag;}
   inline static uint32_t modifiesCarryFlag(OMR::InstOpCode::Mnemonic op)    {return _properties[op] & IA32OpProp_ModifiesCarryFlag;}
   inline static uint32_t testsOverflowFlag(OMR::InstOpCode::Mnemonic op)    {return _properties[op] & IA32OpProp_TestsOverflowFlag;}
   inline static uint32_t modifiesOverflowFlag(OMR::InstOpCode::Mnemonic op) {return _properties[op] & IA32OpProp_ModifiesOverflowFlag;}
   inline static uint32_t testsParityFlag(OMR::InstOpCode::Mnemonic op)      {return _properties[op] & IA32OpProp_TestsParityFlag;}
   inline static uint32_t modifiesParityFlag(OMR::InstOpCode::Mnemonic op)   {return _properties[op] & IA32OpProp_ModifiesParityFlag;}
   inline static uint32_t isCallOp(OMR::InstOpCode::Mnemonic op)             {return _properties1[op] & IA32OpProp1_CallOp;}
   inline static uint32_t isCallImmOp(OMR::InstOpCode::Mnemonic op)          {return op == OMR::InstOpCode::CALLImm4 || op == OMR::InstOpCode::CALLREXImm4; }

   inline static uint8_t getModifiedEFlags(OMR::InstOpCode::Mnemonic op)
      {
      uint8_t flags = 0;
      if (modifiesOverflowFlag(op)) flags |= IA32EFlags_OF;
      if (modifiesSignFlag(op))     flags |= IA32EFlags_SF;
      if (modifiesZeroFlag(op))     flags |= IA32EFlags_ZF;
      if (modifiesParityFlag(op))   flags |= IA32EFlags_PF;
      if (modifiesCarryFlag(op))    flags |= IA32EFlags_CF;
      return flags;
      }

   inline static uint8_t getTestedEFlags(OMR::InstOpCode::Mnemonic op)
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

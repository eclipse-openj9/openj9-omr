/*******************************************************************************
 * Copyright (c) 2000, 2018 IBM Corp. and others
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

/*
 * This file will be included within a static table.  Only comments and
 * definitions are permitted.
 */


   {
   /* .mnemonic    = */ OMR::InstOpCode::ASM,
   /* .name        = */ "ASM",
   /* .description = */ "ASM WCode Support",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ASSOCREGS,
   /* .name        = */ "ASSOCREGS",
   /* .description = */ "Register Association",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },   

   {
   /* .mnemonic    = */ OMR::InstOpCode::BAD,
   /* .name        = */ "BAD",
   /* .description = */ "Bad Opcode",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },   

   {
   /* .mnemonic    = */ OMR::InstOpCode::BREAK,
   /* .name        = */ "BREAK",
   /* .description = */ "Breakpoint (debugger)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFRB,
   /* .name        = */ "CGFRB",
   /* .description = */ "Compare and Branch (64-32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xF4,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFRJ,
   /* .name        = */ "CGFRJ",
   /* .description = */ "Compare and Branch Relative (64-32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x74,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFRT,
   /* .name        = */ "CGFRT",
   /* .description = */ "Compare and Trap (64-32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x70,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHHRL,
   /* .name        = */ "CHHRL",
   /* .description = */ "Compare Halfword Relative Long (16)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFRB,
   /* .name        = */ "CLGFRB",
   /* .description = */ "Compare Logical And Branch (64-32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xF5,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFRJ,
   /* .name        = */ "CLGFRJ",
   /* .description = */ "Compare Logical And Branch Relative (64-32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x75,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFRT,
   /* .name        = */ "CLGFRT",
   /* .description = */ "Compare Logical And Trap (64-32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x71,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHHRL,
   /* .name        = */ "CLHHRL",
   /* .description = */ "Compare Logical Relative Long Halfword (16)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DC,
   /* .name        = */ "DC",
   /* .description = */ "DC",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ DC_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DC2,
   /* .name        = */ "DC2",
   /* .description = */ "DC2",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ DC_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DCB,
   /* .name        = */ "DCB",
   /* .description = */ "Debug Counter Bump",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DEPEND,
   /* .name        = */ "DEPEND",
   /* .description = */ "Someplace to hang dependencies",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DIDTR,
   /* .name        = */ "DIDTR",
   /* .description = */ "Divide to Integer (DFP64)",
   /* .opcode[0]   = */ 0xB4,
   /* .opcode[1]   = */ 0xC6,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DIRECTIVE,
   /* .name        = */ "DIRECTIVE",
   /* .description = */ "WCode DIR related",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DS,
   /* .name        = */ "DS",
   /* .description = */ "DS",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FENCE,
   /* .name        = */ "FENCE",
   /* .description = */ "Fence",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LABEL,
   /* .name        = */ "LABEL",
   /* .description = */ "Destination of a jump",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHHR,
   /* .name        = */ "LHHR",
   /* .description = */ "Load (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHLR,
   /* .name        = */ "LHLR",
   /* .description = */ "Load (High <- Low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLCHHR,
   /* .name        = */ "LLCHHR",
   /* .description = */ "Load Logical Character (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLCHLR,
   /* .name        = */ "LLCHLR",
   /* .description = */ "Load Logical Character (High <- low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLCLHR,
   /* .name        = */ "LLCLHR",
   /* .description = */ "Load Logical Character (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHFR,
   /* .name        = */ "LLHFR",
   /* .description = */ "Load (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHHHR,
   /* .name        = */ "LLHHHR",
   /* .description = */ "Load Logical Halfword (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHHLR,
   /* .name        = */ "LLHHLR",
   /* .description = */ "Load Logical Halfword (High <- low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHLHR,
   /* .name        = */ "LLHLHR",
   /* .description = */ "Load Logical Halfword (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRIC,
   /* .name        = */ "LRIC",
   /* .description = */ "Load Runtime Instrumentation Controls",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x60,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MRIC,
   /* .name        = */ "MRIC",
   /* .description = */ "Modify Runtime Instrumentation Controls",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x62,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NHHR,
   /* .name        = */ "NHHR",
   /* .description = */ "AND High (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NHLR,
   /* .name        = */ "NHLR",
   /* .description = */ "AND High (High <- Low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NLHR,
   /* .name        = */ "NLHR",
   /* .description = */ "AND High (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OHHR,
   /* .name        = */ "OHHR",
   /* .description = */ "OR High (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OHLR,
   /* .name        = */ "OHLR",
   /* .description = */ "OR High (High <- Low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OLHR,
   /* .name        = */ "OLHR",
   /* .description = */ "OR High (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PROC,
   /* .name        = */ "PROC",
   /* .description = */ "Entry to the method",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RET,
   /* .name        = */ "RET",
   /* .description = */ "Return",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RIEMIT,
   /* .name        = */ "RIEMIT",
   /* .description = */ "Runtime Instrumentation Emit",
   /* .opcode[0]   = */ 0xAA,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RINEXT,
   /* .name        = */ "RINEXT",
   /* .description = */ "Runtime Instrumentation Next",
   /* .opcode[0]   = */ 0xAA,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RIOFF,
   /* .name        = */ "RIOFF",
   /* .description = */ "Runtime Instrumentation Off",
   /* .opcode[0]   = */ 0xAA,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RION,
   /* .name        = */ "RION",
   /* .description = */ "Runtime Instrumentation On",
   /* .opcode[0]   = */ 0xAA,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SCHEDFENCE,
   /* .name        = */ "SCHEDFENCE",
   /* .description = */ "Scheduling Fence",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLLHH,
   /* .name        = */ "SLLHH",
   /* .description = */ "Shift Left Logical (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLLLH,
   /* .name        = */ "SLLLH",
   /* .description = */ "Shift Left Logical (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRLHH,
   /* .name        = */ "SRLHH",
   /* .description = */ "Shift Right Logical (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRLLH,
   /* .name        = */ "SRLLH",
   /* .description = */ "Shift Right Logical (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRIC,
   /* .name        = */ "STRIC",
   /* .description = */ "Store Runtime Instrumentation Controls",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x61,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TAILCALL,
   /* .name        = */ "TAILCALL",
   /* .description = */ "Tail Call",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_IsCall
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TCDT,
   /* .name        = */ "TCDT",
   /* .description = */ "Test Data Class (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRIC,
   /* .name        = */ "TRIC",
   /* .description = */ "Test Runtime Instrumentation Controls",
   /* .opcode[0]   = */ 0xAA,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGNOP,
   /* .name        = */ "VGNOP",
   /* .description = */ "ValueGuardNOP",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::WRTBAR,
   /* .name        = */ "WRTBAR",
   /* .description = */ "Write Barrier",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XHHR,
   /* .name        = */ "XHHR",
   /* .description = */ "Exclusive OR High (High <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XHLR,
   /* .name        = */ "XHLR",
   /* .description = */ "Exclusive OR High (High <- Low)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XLHR,
   /* .name        = */ "XLHR",
   /* .description = */ "Exclusive OR High (Low <- High)",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_TargetLW |
                        S390OpProp_SrcHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XPCALLDESC,
   /* .name        = */ "XPCALLDESC",
   /* .description = */ "zOS-31 LE Call Descriptor.",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_UnknownArchitecture,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::A,
   /* .name        = */ "A",
   /* .description = */ "Add",
   /* .opcode[0]   = */ 0x5A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AD,
   /* .name        = */ "AD",
   /* .description = */ "Add Normalized, Long",
   /* .opcode[0]   = */ 0x6A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ADB,
   /* .name        = */ "ADB",
   /* .description = */ "Add (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ADBR,
   /* .name        = */ "ADBR",
   /* .description = */ "Add (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ADR,
   /* .name        = */ "ADR",
   /* .description = */ "Add Normalized, Long",
   /* .opcode[0]   = */ 0x2A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AE,
   /* .name        = */ "AE",
   /* .description = */ "Add Normalized, Short",
   /* .opcode[0]   = */ 0x7A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AEB,
   /* .name        = */ "AEB",
   /* .description = */ "Add (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AEBR,
   /* .name        = */ "AEBR",
   /* .description = */ "Add (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AER,
   /* .name        = */ "AER",
   /* .description = */ "Add Normalized, Short",
   /* .opcode[0]   = */ 0x3A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGFR,
   /* .name        = */ "AGFR",
   /* .description = */ "Add (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x18,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGHI,
   /* .name        = */ "AGHI",
   /* .description = */ "Add Halfword Immediate",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGR,
   /* .name        = */ "AGR",
   /* .description = */ "Add (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AH,
   /* .name        = */ "AH",
   /* .description = */ "Add Halfword",
   /* .opcode[0]   = */ 0x4A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AHI,
   /* .name        = */ "AHI",
   /* .description = */ "Add Halfword Immediate",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AL,
   /* .name        = */ "AL",
   /* .description = */ "Add Logical",
   /* .opcode[0]   = */ 0x5E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALCGR,
   /* .name        = */ "ALCGR",
   /* .description = */ "Add Logical with Carry (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x88,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALCR,
   /* .name        = */ "ALCR",
   /* .description = */ "Add Logical with Carry (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x98,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGFR,
   /* .name        = */ "ALGFR",
   /* .description = */ "Add Logical (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGR,
   /* .name        = */ "ALGR",
   /* .description = */ "Add Logical (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALR,
   /* .name        = */ "ALR",
   /* .description = */ "Add Logical (32)",
   /* .opcode[0]   = */ 0x1E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AP,
   /* .name        = */ "AP",
   /* .description = */ "Add Decimal",
   /* .opcode[0]   = */ 0xFA,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AR,
   /* .name        = */ "AR",
   /* .description = */ "Add (32)",
   /* .opcode[0]   = */ 0x1A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AU,
   /* .name        = */ "AU",
   /* .description = */ "Add Unnormalized, Short",
   /* .opcode[0]   = */ 0x7E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AUR,
   /* .name        = */ "AUR",
   /* .description = */ "Add Unnormalized, Short",
   /* .opcode[0]   = */ 0x3E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AW,
   /* .name        = */ "AW",
   /* .description = */ "Add Unnormalized, Long",
   /* .opcode[0]   = */ 0x6E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AWR,
   /* .name        = */ "AWR",
   /* .description = */ "Add Unnormalized, Long",
   /* .opcode[0]   = */ 0x2E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AXBR,
   /* .name        = */ "AXBR",
   /* .description = */ "Add (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x4A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AXR,
   /* .name        = */ "AXR",
   /* .description = */ "Add Normalized, Extended",
   /* .opcode[0]   = */ 0x36,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BAKR,
   /* .name        = */ "BAKR",
   /* .description = */ "Branch and Stack",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x40,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BAL,
   /* .name        = */ "BAL",
   /* .description = */ "Branch and Link",
   /* .opcode[0]   = */ 0x45,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BALR,
   /* .name        = */ "BALR",
   /* .description = */ "Branch and Link",
   /* .opcode[0]   = */ 0x05,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BAS,
   /* .name        = */ "BAS",
   /* .description = */ "Branch and Save",
   /* .opcode[0]   = */ 0x4D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BASR,
   /* .name        = */ "BASR",
   /* .description = */ "Branch and Save",
   /* .opcode[0]   = */ 0x0D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BASSM,
   /* .name        = */ "BASSM",
   /* .description = */ "Branch and Save and Set Mode",
   /* .opcode[0]   = */ 0x0C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BC,
   /* .name        = */ "BC",
   /* .description = */ "Branch on Cond.",
   /* .opcode[0]   = */ 0x47,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_ReadsCC |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BCR,
   /* .name        = */ "BCR",
   /* .description = */ "Branch on Cond.",
   /* .opcode[0]   = */ 0x07,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_ReadsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BCT,
   /* .name        = */ "BCT",
   /* .description = */ "Branch on Count (32)",
   /* .opcode[0]   = */ 0x46,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BCTGR,
   /* .name        = */ "BCTGR",
   /* .description = */ "Branch on Count (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BCTR,
   /* .name        = */ "BCTR",
   /* .description = */ "Branch on Count (32)",
   /* .opcode[0]   = */ 0x06,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRAS,
   /* .name        = */ "BRAS",
   /* .description = */ "Branch and Save",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRASL,
   /* .name        = */ "BRASL",
   /* .description = */ "Branch Rel.and Save Long",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRC,
   /* .name        = */ "BRC",
   /* .description = */ "Branch Rel. on Cond.",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_ReadsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRCL,
   /* .name        = */ "BRCL",
   /* .description = */ "Branch Rel. on Cond. Long",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_ReadsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRCT,
   /* .name        = */ "BRCT",
   /* .description = */ "Branch Rel. on Count (32)",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRCTG,
   /* .name        = */ "BRCTG",
   /* .description = */ "Branch Relative on Count (64)",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRXH,
   /* .name        = */ "BRXH",
   /* .description = */ "Branch Rel. on Idx High",
   /* .opcode[0]   = */ 0x84,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_UsesTarget |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRXHG,
   /* .name        = */ "BRXHG",
   /* .description = */ "Branch Relative on Index High",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_UsesTarget |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRXLE,
   /* .name        = */ "BRXLE",
   /* .description = */ "Branch Rel. on Idx Low or Equal (32)",
   /* .opcode[0]   = */ 0x85,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRXLG,
   /* .name        = */ "BRXLG",
   /* .description = */ "Branch Relative on Index Equal or Low",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x45,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BSM,
   /* .name        = */ "BSM",
   /* .description = */ "Branch and Set Mode",
   /* .opcode[0]   = */ 0x0B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_IsCall |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BXH,
   /* .name        = */ "BXH",
   /* .description = */ "Branch on Idx High",
   /* .opcode[0]   = */ 0x86,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BXLE,
   /* .name        = */ "BXLE",
   /* .description = */ "Branch on Idx Low or Equal (32)",
   /* .opcode[0]   = */ 0x87,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::C,
   /* .name        = */ "C",
   /* .description = */ "Compare (32)",
   /* .opcode[0]   = */ 0x59,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CD,
   /* .name        = */ "CD",
   /* .description = */ "Compare, Long",
   /* .opcode[0]   = */ 0x69,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDB,
   /* .name        = */ "CDB",
   /* .description = */ "Compare (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x19,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDBR,
   /* .name        = */ "CDBR",
   /* .description = */ "Compare (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x19,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDFBR,
   /* .name        = */ "CDFBR",
   /* .description = */ "Convert from Fixed (LB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x95,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDFR,
   /* .name        = */ "CDFR",
   /* .description = */ "Convert from int32 to long HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xB5,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDGBR,
   /* .name        = */ "CDGBR",
   /* .description = */ "Convert from Fixed (LB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA5,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDGR,
   /* .name        = */ "CDGR",
   /* .description = */ "Convert from int64 to long HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC5,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDR,
   /* .name        = */ "CDR",
   /* .description = */ "Compare, Long",
   /* .opcode[0]   = */ 0x29,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDS,
   /* .name        = */ "CDS",
   /* .description = */ "Compare Double and Swap",
   /* .opcode[0]   = */ 0xBB,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CE,
   /* .name        = */ "CE",
   /* .description = */ "Compare, Short",
   /* .opcode[0]   = */ 0x79,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEB,
   /* .name        = */ "CEB",
   /* .description = */ "Compare (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEBR,
   /* .name        = */ "CEBR",
   /* .description = */ "Compare (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEFBR,
   /* .name        = */ "CEFBR",
   /* .description = */ "Convert from Fixed (SB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x94,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEFR,
   /* .name        = */ "CEFR",
   /* .description = */ "Convert from int32 to short HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xB4,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEGBR,
   /* .name        = */ "CEGBR",
   /* .description = */ "Convert from Fixed (SB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA4,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEGR,
   /* .name        = */ "CEGR",
   /* .description = */ "Convert from int64 to short HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC4,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CER,
   /* .name        = */ "CER",
   /* .description = */ "Compare, Short",
   /* .opcode[0]   = */ 0x39,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFC,
   /* .name        = */ "CFC",
   /* .description = */ "Compare and Form CodeWord",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFDBR,
   /* .name        = */ "CFDBR",
   /* .description = */ "Convert to Fixed (LB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x99,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFDR,
   /* .name        = */ "CFDR",
   /* .description = */ "Convert long HFP to int32",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xB9,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFEBR,
   /* .name        = */ "CFEBR",
   /* .description = */ "Convert to Fixed (SB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x98,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFER,
   /* .name        = */ "CFER",
   /* .description = */ "Convert short HFP to int32",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xB8,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFXBR,
   /* .name        = */ "CFXBR",
   /* .description = */ "Convert to Fixed (EB < 32), note here",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x9A,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFXR,
   /* .name        = */ "CFXR",
   /* .description = */ "Convert long HFP to int32",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xBA,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGDBR,
   /* .name        = */ "CGDBR",
   /* .description = */ "Convert to Fixed (64 < LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA9,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGDR,
   /* .name        = */ "CGDR",
   /* .description = */ "Convert long HFP to int64",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC9,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGEBR,
   /* .name        = */ "CGEBR",
   /* .description = */ "Convert to Fixed (64 < SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA8,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGER,
   /* .name        = */ "CGER",
   /* .description = */ "Convert short HFP to int64",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC8,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFR,
   /* .name        = */ "CGFR",
   /* .description = */ "Compare (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x30,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGHI,
   /* .name        = */ "CGHI",
   /* .description = */ "Compare Halfword Immediate (64)",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGR,
   /* .name        = */ "CGR",
   /* .description = */ "Compare (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x20,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGXBR,
   /* .name        = */ "CGXBR",
   /* .description = */ "Convert to Fixed (EB < 64), note here",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xAA,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGXR,
   /* .name        = */ "CGXR",
   /* .description = */ "Convert long HFP to int64",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xCA,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CH,
   /* .name        = */ "CH",
   /* .description = */ "Compare Halfword",
   /* .opcode[0]   = */ 0x49,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHI,
   /* .name        = */ "CHI",
   /* .description = */ "Compare Halfword Immediate (32)",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CKSM,
   /* .name        = */ "CKSM",
   /* .description = */ "Checksum",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x41,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesRegPairForSource |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CL,
   /* .name        = */ "CL",
   /* .description = */ "Compare Logical (32)",
   /* .opcode[0]   = */ 0x55,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLC,
   /* .name        = */ "CLC",
   /* .description = */ "Compare Logical (character)",
   /* .opcode[0]   = */ 0xD5,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_HasTwoMemoryReferences
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLCL,
   /* .name        = */ "CLCL",
   /* .description = */ "Compare Logical Long",
   /* .opcode[0]   = */ 0x0F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLCLE,
   /* .name        = */ "CLCLE",
   /* .description = */ "Compare Logical Long Extended",
   /* .opcode[0]   = */ 0xA9,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFR,
   /* .name        = */ "CLGFR",
   /* .description = */ "Compare Logical (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x31,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGR,
   /* .name        = */ "CLGR",
   /* .description = */ "Compare Logical (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x21,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLI,
   /* .name        = */ "CLI",
   /* .description = */ "Compare Logical Immediate",
   /* .opcode[0]   = */ 0x95,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLM,
   /* .name        = */ "CLM",
   /* .description = */ "Compare Logical Characters under Mask",
   /* .opcode[0]   = */ 0xBD,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLR,
   /* .name        = */ "CLR",
   /* .description = */ "Compare Logical (32)",
   /* .opcode[0]   = */ 0x15,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLST,
   /* .name        = */ "CLST",
   /* .description = */ "Compare Logical String",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x5D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CP,
   /* .name        = */ "CP",
   /* .description = */ "Compare Decimal",
   /* .opcode[0]   = */ 0xF9,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_HasTwoMemoryReferences
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CPYA,
   /* .name        = */ "CPYA",
   /* .description = */ "Copy Access Register",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CR,
   /* .name        = */ "CR",
   /* .description = */ "Compare (32)",
   /* .opcode[0]   = */ 0x19,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CS,
   /* .name        = */ "CS",
   /* .description = */ "Compare and Swap (32)",
   /* .opcode[0]   = */ 0xBA,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CSCH,
   /* .name        = */ "CSCH",
   /* .description = */ "Clear Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x30,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CUSE,
   /* .name        = */ "CUSE",
   /* .description = */ "Compare Until Substring Equal",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x57,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVB,
   /* .name        = */ "CVB",
   /* .description = */ "Convert to Binary",
   /* .opcode[0]   = */ 0x4F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVD,
   /* .name        = */ "CVD",
   /* .description = */ "Convert to Decimal (32)",
   /* .opcode[0]   = */ 0x4E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXBR,
   /* .name        = */ "CXBR",
   /* .description = */ "Compare (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x49,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXFBR,
   /* .name        = */ "CXFBR",
   /* .description = */ "Convert from Fixed (EB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x96,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXFR,
   /* .name        = */ "CXFR",
   /* .description = */ "Convert from int32 to extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xB6,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXGBR,
   /* .name        = */ "CXGBR",
   /* .description = */ "Convert from Fixed (EB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA6,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXGR,
   /* .name        = */ "CXGR",
   /* .description = */ "Convert from int64 to extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC6,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXR,
   /* .name        = */ "CXR",
   /* .description = */ "Compare, Extended",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x69,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::D,
   /* .name        = */ "D",
   /* .description = */ "Divide (32 < 64)",
   /* .opcode[0]   = */ 0x5D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DD,
   /* .name        = */ "DD",
   /* .description = */ "Divide, Long HFP",
   /* .opcode[0]   = */ 0x6D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DDB,
   /* .name        = */ "DDB",
   /* .description = */ "Divide (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1D,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DDBR,
   /* .name        = */ "DDBR",
   /* .description = */ "Divide (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DDR,
   /* .name        = */ "DDR",
   /* .description = */ "Divide, Long HFP",
   /* .opcode[0]   = */ 0x2D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DE,
   /* .name        = */ "DE",
   /* .description = */ "Divide, short HFP",
   /* .opcode[0]   = */ 0x7D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DEB,
   /* .name        = */ "DEB",
   /* .description = */ "Divide (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DEBR,
   /* .name        = */ "DEBR",
   /* .description = */ "Divide (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DER,
   /* .name        = */ "DER",
   /* .description = */ "Divide, Short HFP",
   /* .opcode[0]   = */ 0x3D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DIAG,
   /* .name        = */ "DIAG",
   /* .description = */ "Diagnose Macro",
   /* .opcode[0]   = */ 0x00,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DIDBR,
   /* .name        = */ "DIDBR",
   /* .description = */ "Divide to Integer (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x5B,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DIEBR,
   /* .name        = */ "DIEBR",
   /* .description = */ "Divide to Integer (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x53,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DLGR,
   /* .name        = */ "DLGR",
   /* .description = */ "Divide Logical",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x87,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DLR,
   /* .name        = */ "DLR",
   /* .description = */ "Divide",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x97,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DP,
   /* .name        = */ "DP",
   /* .description = */ "Divide Decimal",
   /* .opcode[0]   = */ 0xFD,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DR,
   /* .name        = */ "DR",
   /* .description = */ "Divide",
   /* .opcode[0]   = */ 0x1D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DSGFR,
   /* .name        = */ "DSGFR",
   /* .description = */ "Divide Single (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DSGR,
   /* .name        = */ "DSGR",
   /* .description = */ "Divide Single (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DXBR,
   /* .name        = */ "DXBR",
   /* .description = */ "Divide (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x4D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DXR,
   /* .name        = */ "DXR",
   /* .description = */ "Divide, extended HFP",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x2D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EAR,
   /* .name        = */ "EAR",
   /* .description = */ "Extract Access Register",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ED,
   /* .name        = */ "ED",
   /* .description = */ "Edit",
   /* .opcode[0]   = */ 0xDE,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EDMK,
   /* .name        = */ "EDMK",
   /* .description = */ "Edit and Mark",
   /* .opcode[0]   = */ 0xDF,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlySetsGPR1 |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EFPC,
   /* .name        = */ "EFPC",
   /* .description = */ "Extract FPC",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x8C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_ReadsFPC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EPAR,
   /* .name        = */ "EPAR",
   /* .description = */ "Extract Primary ASN",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EPSW,
   /* .name        = */ "EPSW",
   /* .description = */ "Extract PSW",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x8D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EREG,
   /* .name        = */ "EREG",
   /* .description = */ "Extract Stacked Registers",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x49,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EREGG,
   /* .name        = */ "EREGG",
   /* .description = */ "Extract Stacked Registers",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ESAR,
   /* .name        = */ "ESAR",
   /* .description = */ "Extract Secondary ASN",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x27,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ESEA,
   /* .name        = */ "ESEA",
   /* .description = */ "Extract and Set Extended Authority",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x9D,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ESTA,
   /* .name        = */ "ESTA",
   /* .description = */ "Extract Stacked State",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EX,
   /* .name        = */ "EX",
   /* .description = */ "Execute",
   /* .opcode[0]   = */ 0x44,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIDBR,
   /* .name        = */ "FIDBR",
   /* .description = */ "Load FP Integer (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x5F,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIDR,
   /* .name        = */ "FIDR",
   /* .description = */ "Load long HFP Integer (round toward 0)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x7F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIEBR,
   /* .name        = */ "FIEBR",
   /* .description = */ "Load FP Integer (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x57,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIER,
   /* .name        = */ "FIER",
   /* .description = */ "Load short HFP Integer (round toward 0)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x77,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIXBR,
   /* .name        = */ "FIXBR",
   /* .description = */ "Load FP Integer (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x47,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIXR,
   /* .name        = */ "FIXR",
   /* .description = */ "Load extended HFP Integer (round toward 0)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x67,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::HDR,
   /* .name        = */ "HDR",
   /* .description = */ "Halve, Long HFP",
   /* .opcode[0]   = */ 0x24,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::HER,
   /* .name        = */ "HER",
   /* .description = */ "Halve, short HFP",
   /* .opcode[0]   = */ 0x34,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::HSCH,
   /* .name        = */ "HSCH",
   /* .description = */ "Halt Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x31,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IAC,
   /* .name        = */ "IAC",
   /* .description = */ "Insert Address Space Control",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x24,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IC,
   /* .name        = */ "IC",
   /* .description = */ "Insert Character",
   /* .opcode[0]   = */ 0x43,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ICM,
   /* .name        = */ "ICM",
   /* .description = */ "Insert Character under Mask",
   /* .opcode[0]   = */ 0xBF,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IIHH,
   /* .name        = */ "IIHH",
   /* .description = */ "Insert Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IIHL,
   /* .name        = */ "IIHL",
   /* .description = */ "Insert Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IILH,
   /* .name        = */ "IILH",
   /* .description = */ "Insert Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IILL,
   /* .name        = */ "IILL",
   /* .description = */ "Insert Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IPK,
   /* .name        = */ "IPK",
   /* .description = */ "Insert PSW Key",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlySetsGPR2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IPM,
   /* .name        = */ "IPM",
   /* .description = */ "Insert Program mask",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x22,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ISKE,
   /* .name        = */ "ISKE",
   /* .description = */ "Insert Storage Key Extended",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x29,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IVSK,
   /* .name        = */ "IVSK",
   /* .description = */ "Insert Virtual Storage Key",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x23,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::L,
   /* .name        = */ "L",
   /* .description = */ "Load (32)",
   /* .opcode[0]   = */ 0x58,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LA,
   /* .name        = */ "LA",
   /* .description = */ "Load Address",
   /* .opcode[0]   = */ 0x41,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAE,
   /* .name        = */ "LAE",
   /* .description = */ "Load Address Extended",
   /* .opcode[0]   = */ 0x51,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAM,
   /* .name        = */ "LAM",
   /* .description = */ "Load Access Multiple",
   /* .opcode[0]   = */ 0x9A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LARL,
   /* .name        = */ "LARL",
   /* .description = */ "Load Address Relative Long",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCDBR,
   /* .name        = */ "LCDBR",
   /* .description = */ "Load Complement (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x13,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCDR,
   /* .name        = */ "LCDR",
   /* .description = */ "Load Complement, Long HFP",
   /* .opcode[0]   = */ 0x23,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCEBR,
   /* .name        = */ "LCEBR",
   /* .description = */ "Load Complement (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCER,
   /* .name        = */ "LCER",
   /* .description = */ "Load Complement, short HFP",
   /* .opcode[0]   = */ 0x33,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCGFR,
   /* .name        = */ "LCGFR",
   /* .description = */ "Load Complement (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x13,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCGR,
   /* .name        = */ "LCGR",
   /* .description = */ "Load Complement (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCR,
   /* .name        = */ "LCR",
   /* .description = */ "Load Complement (32)",
   /* .opcode[0]   = */ 0x13,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCTL,
   /* .name        = */ "LCTL",
   /* .description = */ "Load Control",
   /* .opcode[0]   = */ 0xB7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCXBR,
   /* .name        = */ "LCXBR",
   /* .description = */ "Load Complement (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x43,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCXR,
   /* .name        = */ "LCXR",
   /* .description = */ "Load Complement, extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x63,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LD,
   /* .name        = */ "LD",
   /* .description = */ "Load (L)",
   /* .opcode[0]   = */ 0x68,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_DoubleFP |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDE,
   /* .name        = */ "LDE",
   /* .description = */ "Load Lengthened short to long HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x24,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDEB,
   /* .name        = */ "LDEB",
   /* .description = */ "Load Lengthened (LB < SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDEBR,
   /* .name        = */ "LDEBR",
   /* .description = */ "Load Leeeengthened (LB < SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDER,
   /* .name        = */ "LDER",
   /* .description = */ "Load Rounded, long to short HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x24,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDR,
   /* .name        = */ "LDR",
   /* .description = */ "Load (L)",
   /* .opcode[0]   = */ 0x28,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDXBR,
   /* .name        = */ "LDXBR",
   /* .description = */ "Load Rounded (LB < EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x45,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDXR,
   /* .name        = */ "LDXR",
   /* .description = */ "Load Rounded, extended to long HFP",
   /* .opcode[0]   = */ 0x25,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LE,
   /* .name        = */ "LE",
   /* .description = */ "Load (S)",
   /* .opcode[0]   = */ 0x78,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SingleFP |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEDBR,
   /* .name        = */ "LEDBR",
   /* .description = */ "Load Rounded (SB < LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEDR,
   /* .name        = */ "LEDR",
   /* .description = */ "Load Rounded, long to short HFP",
   /* .opcode[0]   = */ 0x35,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LER,
   /* .name        = */ "LER",
   /* .description = */ "Load (S)",
   /* .opcode[0]   = */ 0x38,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEXBR,
   /* .name        = */ "LEXBR",
   /* .description = */ "Load Rounded (SB < EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEXR,
   /* .name        = */ "LEXR",
   /* .description = */ "Load Rounded, extended to short HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x66,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LFPC,
   /* .name        = */ "LFPC",
   /* .description = */ "Load FPC",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x9D,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGFR,
   /* .name        = */ "LGFR",
   /* .description = */ "(LongDisp) Load (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x14,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGHI,
   /* .name        = */ "LGHI",
   /* .description = */ "Load Halfword Immediate",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGR,
   /* .name        = */ "LGR",
   /* .description = */ "Load (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LH,
   /* .name        = */ "LH",
   /* .description = */ "Load Halfword",
   /* .opcode[0]   = */ 0x48,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHI,
   /* .name        = */ "LHI",
   /* .description = */ "Load Halfword Immediate",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGFR,
   /* .name        = */ "LLGFR",
   /* .description = */ "Load Logical Halfword(64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x16,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGTR,
   /* .name        = */ "LLGTR",
   /* .description = */ "Load Logical Thirty One Bits",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x17,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLIHH,
   /* .name        = */ "LLIHH",
   /* .description = */ "Load Logical Halfword Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLIHL,
   /* .name        = */ "LLIHL",
   /* .description = */ "Load Logical Halfword Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLILH,
   /* .name        = */ "LLILH",
   /* .description = */ "Load Logical Halfword Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLILL,
   /* .name        = */ "LLILL",
   /* .description = */ "Load Logical Halfword Immediate",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LM,
   /* .name        = */ "LM",
   /* .description = */ "Load Multiple",
   /* .opcode[0]   = */ 0x98,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNDBR,
   /* .name        = */ "LNDBR",
   /* .description = */ "Load Negative (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x11,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNDR,
   /* .name        = */ "LNDR",
   /* .description = */ "Load Negative, Long HFP",
   /* .opcode[0]   = */ 0x21,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNEBR,
   /* .name        = */ "LNEBR",
   /* .description = */ "Load Negative (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNER,
   /* .name        = */ "LNER",
   /* .description = */ "Load Negative, short HFP",
   /* .opcode[0]   = */ 0x31,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNGFR,
   /* .name        = */ "LNGFR",
   /* .description = */ "Load Negative",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x11,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNGR,
   /* .name        = */ "LNGR",
   /* .description = */ "Load Negative (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNR,
   /* .name        = */ "LNR",
   /* .description = */ "Load Negative (32)",
   /* .opcode[0]   = */ 0x11,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNXBR,
   /* .name        = */ "LNXBR",
   /* .description = */ "Load Negative (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x41,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNXR,
   /* .name        = */ "LNXR",
   /* .description = */ "Load Negative, extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x61,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPDBR,
   /* .name        = */ "LPDBR",
   /* .description = */ "Load Positive (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x10,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPDR,
   /* .name        = */ "LPDR",
   /* .description = */ "Load Positive, Long HFP",
   /* .opcode[0]   = */ 0x20,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPEBR,
   /* .name        = */ "LPEBR",
   /* .description = */ "Load Positive (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPER,
   /* .name        = */ "LPER",
   /* .description = */ "Load Positive, short HFP",
   /* .opcode[0]   = */ 0x30,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPGFR,
   /* .name        = */ "LPGFR",
   /* .description = */ "Load Positive (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x10,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPGR,
   /* .name        = */ "LPGR",
   /* .description = */ "Load Positive (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPR,
   /* .name        = */ "LPR",
   /* .description = */ "Load Positive (32)",
   /* .opcode[0]   = */ 0x10,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPSW,
   /* .name        = */ "LPSW",
   /* .description = */ "Load PSW",
   /* .opcode[0]   = */ 0x82,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPSWE,
   /* .name        = */ "LPSWE",
   /* .description = */ "Load PSW Extended",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xB2,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPXBR,
   /* .name        = */ "LPXBR",
   /* .description = */ "Load Positive (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x40,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPXR,
   /* .name        = */ "LPXR",
   /* .description = */ "Load Positive, extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x60,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LR,
   /* .name        = */ "LR",
   /* .description = */ "Load (32)",
   /* .opcode[0]   = */ 0x18,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRA,
   /* .name        = */ "LRA",
   /* .description = */ "Load Real Address",
   /* .opcode[0]   = */ 0xB1,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRVGR,
   /* .name        = */ "LRVGR",
   /* .description = */ "Load Reversed (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRVR,
   /* .name        = */ "LRVR",
   /* .description = */ "Load Reversed (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTDBR,
   /* .name        = */ "LTDBR",
   /* .description = */ "Load and Test (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTDR,
   /* .name        = */ "LTDR",
   /* .description = */ "Load and Test, Long HFP",
   /* .opcode[0]   = */ 0x22,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTEBR,
   /* .name        = */ "LTEBR",
   /* .description = */ "Load and Test (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTER,
   /* .name        = */ "LTER",
   /* .description = */ "Load and Test, Short HFP",
   /* .opcode[0]   = */ 0x32,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTGFR,
   /* .name        = */ "LTGFR",
   /* .description = */ "Load and Test (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTGR,
   /* .name        = */ "LTGR",
   /* .description = */ "Load and Test (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTR,
   /* .name        = */ "LTR",
   /* .description = */ "Load and Test (32)",
   /* .opcode[0]   = */ 0x12,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTXBR,
   /* .name        = */ "LTXBR",
   /* .description = */ "Load and Test (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x42,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTXR,
   /* .name        = */ "LTXR",
   /* .description = */ "Load and Test, extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x62,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LURA,
   /* .name        = */ "LURA",
   /* .description = */ "Load Using Real Address",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LURAG,
   /* .name        = */ "LURAG",
   /* .description = */ "Load Using Real Address",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXD,
   /* .name        = */ "LXD",
   /* .description = */ "Load Lengthened short to long HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXDB,
   /* .name        = */ "LXDB",
   /* .description = */ "Load Lengthened (EB < LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_DoubleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXDBR,
   /* .name        = */ "LXDBR",
   /* .description = */ "Load Leeeengthened (EB < DB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXDR,
   /* .name        = */ "LXDR",
   /* .description = */ "Load Lengthened, long to extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXE,
   /* .name        = */ "LXE",
   /* .description = */ "Load Lengthened short to long HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXEB,
   /* .name        = */ "LXEB",
   /* .description = */ "Load Lengthened (EB < SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXEBR,
   /* .name        = */ "LXEBR",
   /* .description = */ "Load Leeeengthened (EB < SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXER,
   /* .name        = */ "LXER",
   /* .description = */ "Load Rounded, short to extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXR,
   /* .name        = */ "LXR",
   /* .description = */ "Load Leeeengthened (EB < SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x65,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LZDR,
   /* .name        = */ "LZDR",
   /* .description = */ "Load Zero (L)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x75,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LZER,
   /* .name        = */ "LZER",
   /* .description = */ "Load Zero (S)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x74,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LZXR,
   /* .name        = */ "LZXR",
   /* .description = */ "Load Zero (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x76,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::M,
   /* .name        = */ "M",
   /* .description = */ "Multiply (64 < 32)",
   /* .opcode[0]   = */ 0x5C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MADB,
   /* .name        = */ "MADB",
   /* .description = */ "Multiply and Add (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1E,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MADBR,
   /* .name        = */ "MADBR",
   /* .description = */ "Multiply and Add (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1E,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAEB,
   /* .name        = */ "MAEB",
   /* .description = */ "Multiply and Add (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAEBR,
   /* .name        = */ "MAEBR",
   /* .description = */ "Multiply and Add (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MD,
   /* .name        = */ "MD",
   /* .description = */ "Multiply, long HFP source and result",
   /* .opcode[0]   = */ 0x6C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDB,
   /* .name        = */ "MDB",
   /* .description = */ "Multiply (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1C,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDBR,
   /* .name        = */ "MDBR",
   /* .description = */ "Multiply (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDE,
   /* .name        = */ "MDE",
   /* .description = */ "Multiply, short HFP source, long HFP result",
   /* .opcode[0]   = */ 0x7C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDER,
   /* .name        = */ "MDER",
   /* .description = */ "Multiply, short HFP source, long HFP result",
   /* .opcode[0]   = */ 0x3C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDR,
   /* .name        = */ "MDR",
   /* .description = */ "Multiply, long HFP source and result",
   /* .opcode[0]   = */ 0x2C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MEE,
   /* .name        = */ "MEE",
   /* .description = */ "Multiply short HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x37,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MEEB,
   /* .name        = */ "MEEB",
   /* .description = */ "Multiply (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x17,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MEEBR,
   /* .name        = */ "MEEBR",
   /* .description = */ "Multiply (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x17,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MEER,
   /* .name        = */ "MEER",
   /* .description = */ "Multiply, short HFP source and result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x37,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MGHI,
   /* .name        = */ "MGHI",
   /* .description = */ "Multiply Halfword Immediate",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MH,
   /* .name        = */ "MH",
   /* .description = */ "Multiply Halfword (32)",
   /* .opcode[0]   = */ 0x4C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MHI,
   /* .name        = */ "MHI",
   /* .description = */ "Multiply Halfword Immediate (32)",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MLGR,
   /* .name        = */ "MLGR",
   /* .description = */ "Multiply Logical ( 128<64 )",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x86,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MLR,
   /* .name        = */ "MLR",
   /* .description = */ "Multiply Logical ( 64<32 )",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x96,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MP,
   /* .name        = */ "MP",
   /* .description = */ "Multiple Decimal",
   /* .opcode[0]   = */ 0xFC,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MR,
   /* .name        = */ "MR",
   /* .description = */ "Multiple (64 < 32)",
   /* .opcode[0]   = */ 0x1C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MS,
   /* .name        = */ "MS",
   /* .description = */ "Multiply Single",
   /* .opcode[0]   = */ 0x71,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSCH,
   /* .name        = */ "MSCH",
   /* .description = */ "Modify Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x32,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSDB,
   /* .name        = */ "MSDB",
   /* .description = */ "Multiply and Subtract (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1F,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSDBR,
   /* .name        = */ "MSDBR",
   /* .description = */ "Multiply and Subtract (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1F,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSEB,
   /* .name        = */ "MSEB",
   /* .description = */ "Multiply and Subtract (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSEBR,
   /* .name        = */ "MSEBR",
   /* .description = */ "Multiply and Subtract (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGFR,
   /* .name        = */ "MSGFR",
   /* .description = */ "Multiply Single (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGR,
   /* .name        = */ "MSGR",
   /* .description = */ "Multiply Single (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSR,
   /* .name        = */ "MSR",
   /* .description = */ "Multiply Single Register",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x52,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSTA,
   /* .name        = */ "MSTA",
   /* .description = */ "Modify Stacked State",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x47,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVC,
   /* .name        = */ "MVC",
   /* .description = */ "Move (character)",
   /* .opcode[0]   = */ 0xD2,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCDK,
   /* .name        = */ "MVCDK",
   /* .description = */ "Move With Destination Key",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ SSE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCK,
   /* .name        = */ "MVCK",
   /* .description = */ "Move With Key",
   /* .opcode[0]   = */ 0xD9,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCL,
   /* .name        = */ "MVCL",
   /* .description = */ "Move Long",
   /* .opcode[0]   = */ 0x0E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCLE,
   /* .name        = */ "MVCLE",
   /* .description = */ "Move Long Extended",
   /* .opcode[0]   = */ 0xA8,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCP,
   /* .name        = */ "MVCP",
   /* .description = */ "Move to Primary",
   /* .opcode[0]   = */ 0xDA,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCS,
   /* .name        = */ "MVCS",
   /* .description = */ "Move to Secondary",
   /* .opcode[0]   = */ 0xDB,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCSK,
   /* .name        = */ "MVCSK",
   /* .description = */ "Move With Source Key",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ SSE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVI,
   /* .name        = */ "MVI",
   /* .description = */ "Move (Immediate)",
   /* .opcode[0]   = */ 0x92,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVN,
   /* .name        = */ "MVN",
   /* .description = */ "Move Numerics",
   /* .opcode[0]   = */ 0xD1,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVO,
   /* .name        = */ "MVO",
   /* .description = */ "Move With Offset",
   /* .opcode[0]   = */ 0xF1,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVST,
   /* .name        = */ "MVST",
   /* .description = */ "Move String",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVZ,
   /* .name        = */ "MVZ",
   /* .description = */ "Move Zones",
   /* .opcode[0]   = */ 0xD3,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MXBR,
   /* .name        = */ "MXBR",
   /* .description = */ "Multiply (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x4C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MXD,
   /* .name        = */ "MXD",
   /* .description = */ "Multiply, long HFP source, extended HFP result",
   /* .opcode[0]   = */ 0x67,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MXDR,
   /* .name        = */ "MXDR",
   /* .description = */ "Multiply, long HFP source, extended HFP result",
   /* .opcode[0]   = */ 0x27,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MXR,
   /* .name        = */ "MXR",
   /* .description = */ "Multiply, extended HFP source and result",
   /* .opcode[0]   = */ 0x26,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::N,
   /* .name        = */ "N",
   /* .description = */ "And (32)",
   /* .opcode[0]   = */ 0x54,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NC,
   /* .name        = */ "NC",
   /* .description = */ "And (character)",
   /* .opcode[0]   = */ 0xD4,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NGR,
   /* .name        = */ "NGR",
   /* .description = */ "And (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x80,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NI,
   /* .name        = */ "NI",
   /* .description = */ "And (Immediate)",
   /* .opcode[0]   = */ 0x94,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsStore |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NIHH,
   /* .name        = */ "NIHH",
   /* .description = */ "And Immediate (high high)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NIHL,
   /* .name        = */ "NIHL",
   /* .description = */ "And Immediate (high low)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NILH,
   /* .name        = */ "NILH",
   /* .description = */ "And Immediate (low high)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NILL,
   /* .name        = */ "NILL",
   /* .description = */ "And Immediate (low low)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NOP,
   /* .name        = */ "NOP",
   /* .description = */ "No-Op (for Labels)",
   /* .opcode[0]   = */ 0x07,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ PSEUDO,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NR,
   /* .name        = */ "NR",
   /* .description = */ "And (32)",
   /* .opcode[0]   = */ 0x14,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::O,
   /* .name        = */ "O",
   /* .description = */ "Or (32)",
   /* .opcode[0]   = */ 0x56,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OC,
   /* .name        = */ "OC",
   /* .description = */ "Or (character)",
   /* .opcode[0]   = */ 0xD6,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OGR,
   /* .name        = */ "OGR",
   /* .description = */ "Or (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x81,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OI,
   /* .name        = */ "OI",
   /* .description = */ "Or (Immediate)",
   /* .opcode[0]   = */ 0x96,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsStore |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OIHH,
   /* .name        = */ "OIHH",
   /* .description = */ "Or Immediate (high high)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OIHL,
   /* .name        = */ "OIHL",
   /* .description = */ "Or Immediate (high low)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OILH,
   /* .name        = */ "OILH",
   /* .description = */ "Or Immediate (low high)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OILL,
   /* .name        = */ "OILL",
   /* .description = */ "Or Immediate (low low)",
   /* .opcode[0]   = */ 0xA5,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OR,
   /* .name        = */ "OR",
   /* .description = */ "Or (32)",
   /* .opcode[0]   = */ 0x16,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PACK,
   /* .name        = */ "PACK",
   /* .description = */ "Pack",
   /* .opcode[0]   = */ 0xF2,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PALB,
   /* .name        = */ "PALB",
   /* .description = */ "Purge ALB",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x48,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PC,
   /* .name        = */ "PC",
   /* .description = */ "Program Call",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x18,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlySetsGPR3 |
                        S390OpProp_ImplicitlySetsGPR4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PKA,
   /* .name        = */ "PKA",
   /* .description = */ "Pack ASCII",
   /* .opcode[0]   = */ 0xE9,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PKU,
   /* .name        = */ "PKU",
   /* .description = */ "Pack Unicode",
   /* .opcode[0]   = */ 0xE1,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PLO,
   /* .name        = */ "PLO",
   /* .description = */ "Perform Locked Operation",
   /* .opcode[0]   = */ 0xEE,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsOperand4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PR,
   /* .name        = */ "PR",
   /* .description = */ "Program Return",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PT,
   /* .name        = */ "PT",
   /* .description = */ "Program Transfer",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x28,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RCHP,
   /* .name        = */ "RCHP",
   /* .description = */ "Reset Channel Path",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x3B,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RSCH,
   /* .name        = */ "RSCH",
   /* .description = */ "Resume Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x38,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::S,
   /* .name        = */ "S",
   /* .description = */ "Subtract (32)",
   /* .opcode[0]   = */ 0x5B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAC,
   /* .name        = */ "SAC",
   /* .description = */ "Set Address Control",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x19,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAL,
   /* .name        = */ "SAL",
   /* .description = */ "Set Address Limit",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x37,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAM24,
   /* .name        = */ "SAM24",
   /* .description = */ "Set 24 bit addressing mode",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAM31,
   /* .name        = */ "SAM31",
   /* .description = */ "Set 31 bit addressing mode",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAM64,
   /* .name        = */ "SAM64",
   /* .description = */ "Set 64 bit addressing mode",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SAR,
   /* .name        = */ "SAR",
   /* .description = */ "Set Access Register",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SCHM,
   /* .name        = */ "SCHM",
   /* .description = */ "Set Channel Monitor",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SCK,
   /* .name        = */ "SCK",
   /* .description = */ "Set Clock",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SCKC,
   /* .name        = */ "SCKC",
   /* .description = */ "Set Clock Comparator",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SD,
   /* .name        = */ "SD",
   /* .description = */ "Subtract Normalized,long HFP",
   /* .opcode[0]   = */ 0x6B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SDB,
   /* .name        = */ "SDB",
   /* .description = */ "Subtract (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x1B,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SDBR,
   /* .name        = */ "SDBR",
   /* .description = */ "Subtract (LB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x1B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SDR,
   /* .name        = */ "SDR",
   /* .description = */ "Subtract Normalized,long HFP",
   /* .opcode[0]   = */ 0x2B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SE,
   /* .name        = */ "SE",
   /* .description = */ "Subtract Normalized,short HFP",
   /* .opcode[0]   = */ 0x7B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SEB,
   /* .name        = */ "SEB",
   /* .description = */ "Subtract (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SEBR,
   /* .name        = */ "SEBR",
   /* .description = */ "Subtract (SB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SER,
   /* .name        = */ "SER",
   /* .description = */ "Subtract Normalized,short HFP",
   /* .opcode[0]   = */ 0x3B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SFPC,
   /* .name        = */ "SFPC",
   /* .description = */ "Set FPC",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x84,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsFPC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SGFR,
   /* .name        = */ "SGFR",
   /* .description = */ "Subtract (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x19,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SGR,
   /* .name        = */ "SGR",
   /* .description = */ "Subtract (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SH,
   /* .name        = */ "SH",
   /* .description = */ "Subtract Halfword",
   /* .opcode[0]   = */ 0x4B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SIGP,
   /* .name        = */ "SIGP",
   /* .description = */ "Signal Processor",
   /* .opcode[0]   = */ 0xAE,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SL,
   /* .name        = */ "SL",
   /* .description = */ "Subtract Logical (32)",
   /* .opcode[0]   = */ 0x5F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLA,
   /* .name        = */ "SLA",
   /* .description = */ "Shift Left Single (32)",
   /* .opcode[0]   = */ 0x8B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLBGR,
   /* .name        = */ "SLBGR",
   /* .description = */ "Subtract Logical With Borrow (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x89,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLBR,
   /* .name        = */ "SLBR",
   /* .description = */ "Subtract Logical With Borrow (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x99,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLDA,
   /* .name        = */ "SLDA",
   /* .description = */ "Shift Left Double",
   /* .opcode[0]   = */ 0x8F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLDL,
   /* .name        = */ "SLDL",
   /* .description = */ "Shift Left Double Logical",
   /* .opcode[0]   = */ 0x8D,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLGFR,
   /* .name        = */ "SLGFR",
   /* .description = */ "Subtract Logical (64 < 32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLGR,
   /* .name        = */ "SLGR",
   /* .description = */ "Subtract Logical (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLL,
   /* .name        = */ "SLL",
   /* .description = */ "Shift Left Single Logical",
   /* .opcode[0]   = */ 0x89,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLR,
   /* .name        = */ "SLR",
   /* .description = */ "Subtract Logical (32)",
   /* .opcode[0]   = */ 0x1F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SP,
   /* .name        = */ "SP",
   /* .description = */ "Subtract Decimal",
   /* .opcode[0]   = */ 0xFB,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SPKA,
   /* .name        = */ "SPKA",
   /* .description = */ "Set PSW Key From Address",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SPM,
   /* .name        = */ "SPM",
   /* .description = */ "Set Program Mask",
   /* .opcode[0]   = */ 0x04,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SPT,
   /* .name        = */ "SPT",
   /* .description = */ "Set CPU Timer",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SPX,
   /* .name        = */ "SPX",
   /* .description = */ "Set Prefix",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x10,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQD,
   /* .name        = */ "SQD",
   /* .description = */ "Square Root Long HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x35,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQDB,
   /* .name        = */ "SQDB",
   /* .description = */ "Square Root Long BFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x15,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQDBR,
   /* .name        = */ "SQDBR",
   /* .description = */ "Square Root Long BFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x15,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQDR,
   /* .name        = */ "SQDR",
   /* .description = */ "Square Root Long HFP",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQE,
   /* .name        = */ "SQE",
   /* .description = */ "Square Root short HFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x34,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQEB,
   /* .name        = */ "SQEB",
   /* .description = */ "Square Root Long BFP",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x14,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQEBR,
   /* .name        = */ "SQEBR",
   /* .description = */ "Square Root Long BFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x14,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQER,
   /* .name        = */ "SQER",
   /* .description = */ "Square Root short HFP",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x45,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQXBR,
   /* .name        = */ "SQXBR",
   /* .description = */ "Square Root (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x16,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SQXR,
   /* .name        = */ "SQXR",
   /* .description = */ "Square Root extended HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x36,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SR,
   /* .name        = */ "SR",
   /* .description = */ "Subtract (32)",
   /* .opcode[0]   = */ 0x1B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRA,
   /* .name        = */ "SRA",
   /* .description = */ "Shift Right Single (32)",
   /* .opcode[0]   = */ 0x8A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRDA,
   /* .name        = */ "SRDA",
   /* .description = */ "Shift Right Double",
   /* .opcode[0]   = */ 0x8E,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRDL,
   /* .name        = */ "SRDL",
   /* .description = */ "Shift Right Double Logical",
   /* .opcode[0]   = */ 0x8C,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRL,
   /* .name        = */ "SRL",
   /* .description = */ "Shift Right Single Logical",
   /* .opcode[0]   = */ 0x88,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRNM,
   /* .name        = */ "SRNM",
   /* .description = */ "Set BFP Rounding Mode",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x99,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRP,
   /* .name        = */ "SRP",
   /* .description = */ "Shift and Round Decimal",
   /* .opcode[0]   = */ 0xF0,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRST,
   /* .name        = */ "SRST",
   /* .description = */ "Search String",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x5E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SSAR,
   /* .name        = */ "SSAR",
   /* .description = */ "Set Secondary ASN",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SSCH,
   /* .name        = */ "SSCH",
   /* .description = */ "Start Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x33,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SSM,
   /* .name        = */ "SSM",
   /* .description = */ "Set System Mask",
   /* .opcode[0]   = */ 0x80,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ST,
   /* .name        = */ "ST",
   /* .description = */ "Store (32)",
   /* .opcode[0]   = */ 0x50,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STAM,
   /* .name        = */ "STAM",
   /* .description = */ "Set Access Multiple",
   /* .opcode[0]   = */ 0x9B,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STAP,
   /* .name        = */ "STAP",
   /* .description = */ "Store CPU Address",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STC,
   /* .name        = */ "STC",
   /* .description = */ "Store Character",
   /* .opcode[0]   = */ 0x42,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCK,
   /* .name        = */ "STCK",
   /* .description = */ "Store Clock",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCKC,
   /* .name        = */ "STCKC",
   /* .description = */ "Store Clock Comparator",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCKE,
   /* .name        = */ "STCKE",
   /* .description = */ "Store Clock Extended",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCM,
   /* .name        = */ "STCM",
   /* .description = */ "Store Character under Mask (low)",
   /* .opcode[0]   = */ 0xBE,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCPS,
   /* .name        = */ "STCPS",
   /* .description = */ "Store Channel Path Status",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x3A,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCRW,
   /* .name        = */ "STCRW",
   /* .description = */ "Store Channel Report Word",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x39,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCTL,
   /* .name        = */ "STCTL",
   /* .description = */ "Store Control",
   /* .opcode[0]   = */ 0xB6,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STD,
   /* .name        = */ "STD",
   /* .description = */ "Store (S)",
   /* .opcode[0]   = */ 0x60,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SingleFP |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STE,
   /* .name        = */ "STE",
   /* .description = */ "Store (L)",
   /* .opcode[0]   = */ 0x70,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_DoubleFP |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STFPC,
   /* .name        = */ "STFPC",
   /* .description = */ "Store FPC",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x9C,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STH,
   /* .name        = */ "STH",
   /* .description = */ "Store Halfword",
   /* .opcode[0]   = */ 0x40,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STIDP,
   /* .name        = */ "STIDP",
   /* .description = */ "Store CPU ID",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STM,
   /* .name        = */ "STM",
   /* .description = */ "Store Multiple (32)",
   /* .opcode[0]   = */ 0x90,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STNSM,
   /* .name        = */ "STNSM",
   /* .description = */ "Store Then and System Mask",
   /* .opcode[0]   = */ 0xAC,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STOSM,
   /* .name        = */ "STOSM",
   /* .description = */ "Store Then or System Mask",
   /* .opcode[0]   = */ 0xAD,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STPT,
   /* .name        = */ "STPT",
   /* .description = */ "Store CPU Timer",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STPX,
   /* .name        = */ "STPX",
   /* .description = */ "Store Prefix",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x11,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRAG,
   /* .name        = */ "STRAG",
   /* .description = */ "Store Real Address",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ SSE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STSCH,
   /* .name        = */ "STSCH",
   /* .description = */ "Store Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x34,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STURA,
   /* .name        = */ "STURA",
   /* .description = */ "Store Using Real Address",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STURG,
   /* .name        = */ "STURG",
   /* .description = */ "Store Using Real Address",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SU,
   /* .name        = */ "SU",
   /* .description = */ "Subtract Unnormalized,short HFP",
   /* .opcode[0]   = */ 0x7F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SUR,
   /* .name        = */ "SUR",
   /* .description = */ "Subtract Unnormalized,short HFP",
   /* .opcode[0]   = */ 0x3F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SVC,
   /* .name        = */ "SVC",
   /* .description = */ "Supervisor Call",
   /* .opcode[0]   = */ 0x0A,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ I_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SW,
   /* .name        = */ "SW",
   /* .description = */ "Subtract Unnormalized,long HFP",
   /* .opcode[0]   = */ 0x6F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SWR,
   /* .name        = */ "SWR",
   /* .description = */ "Subtract Unnormalized,long HFP",
   /* .opcode[0]   = */ 0x2F,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SXBR,
   /* .name        = */ "SXBR",
   /* .description = */ "Subtract (EB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x4B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SXR,
   /* .name        = */ "SXR",
   /* .description = */ "Subtract Normalized, extended HFP",
   /* .opcode[0]   = */ 0x37,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TAM,
   /* .name        = */ "TAM",
   /* .description = */ "Test addressing mode",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TAR,
   /* .name        = */ "TAR",
   /* .description = */ "Test Access",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x4C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TBDR,
   /* .name        = */ "TBDR",
   /* .description = */ "Convert HFP to BFP,long HFP,long BFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x51,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TBEDR,
   /* .name        = */ "TBEDR",
   /* .description = */ "Convert HFP to BFP,long HFP,short BFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x50,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TCDB,
   /* .name        = */ "TCDB",
   /* .description = */ "Test Data Class (LB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x11,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TCEB,
   /* .name        = */ "TCEB",
   /* .description = */ "Test Data Class (SB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x10,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TCXB,
   /* .name        = */ "TCXB",
   /* .description = */ "Test Data Class (EB)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::THDER,
   /* .name        = */ "THDER",
   /* .description = */ "Convert BFP to HFP,short BFP,long HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x58,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::THDR,
   /* .name        = */ "THDR",
   /* .description = */ "Convert BFP to HFP,long BFP,long HFP",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TM,
   /* .name        = */ "TM",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0x91,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMH,
   /* .name        = */ "TMH",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMHH,
   /* .name        = */ "TMHH",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMHL,
   /* .name        = */ "TMHL",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TML,
   /* .name        = */ "TML",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMLH,
   /* .name        = */ "TMLH",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_TargetLW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMLL,
   /* .name        = */ "TMLL",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xA7,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_TargetLW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TP,
   /* .name        = */ "TP",
   /* .description = */ "Test Decimal",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xC0,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TPI,
   /* .name        = */ "TPI",
   /* .description = */ "Test Pending Interruption",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x36,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TPROT,
   /* .name        = */ "TPROT",
   /* .description = */ "Test Protection",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ SSE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TR,
   /* .name        = */ "TR",
   /* .description = */ "Translate",
   /* .opcode[0]   = */ 0xDC,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRE,
   /* .name        = */ "TRE",
   /* .description = */ "Translate Extended",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xA5,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRT,
   /* .name        = */ "TRT",
   /* .description = */ "Translate and Test",
   /* .opcode[0]   = */ 0xDD,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlySetsGPR2 |
                        S390OpProp_ImplicitlySetsGPR1 |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TS,
   /* .name        = */ "TS",
   /* .description = */ "Test and Set",
   /* .opcode[0]   = */ 0x93,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TSCH,
   /* .name        = */ "TSCH",
   /* .description = */ "Test Subchannel",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x35,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::UNPK,
   /* .name        = */ "UNPK",
   /* .description = */ "Unpack",
   /* .opcode[0]   = */ 0xF3,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::UNPKA,
   /* .name        = */ "UNPKA",
   /* .description = */ "Unpack ASCII",
   /* .opcode[0]   = */ 0xEA,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::UNPKU,
   /* .name        = */ "UNPKU",
   /* .description = */ "Unpack Unicode",
   /* .opcode[0]   = */ 0xE2,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_SetsCC |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::UPT,
   /* .name        = */ "UPT",
   /* .description = */ "Update Tree",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlySetsGPR3 |
                        S390OpProp_ImplicitlySetsGPR5 |
                        S390OpProp_ImplicitlySetsGPR2 |
                        S390OpProp_ImplicitlySetsGPR1 |
                        S390OpProp_ImplicitlySetsGPR0 |
                        S390OpProp_ImplicitlyUsesGPR2 |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::X,
   /* .name        = */ "X",
   /* .description = */ "Exclusive Or (32)",
   /* .opcode[0]   = */ 0x57,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XC,
   /* .name        = */ "XC",
   /* .description = */ "Exclusive Or (character)",
   /* .opcode[0]   = */ 0xD7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS1_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XGR,
   /* .name        = */ "XGR",
   /* .description = */ "Exclusive Or (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x82,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XI,
   /* .name        = */ "XI",
   /* .description = */ "Exclusive Or (immediate)",
   /* .opcode[0]   = */ 0x97,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XR,
   /* .name        = */ "XR",
   /* .description = */ "Exclusive Or (32)",
   /* .opcode[0]   = */ 0x17,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RR_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ZAP,
   /* .name        = */ "ZAP",
   /* .description = */ "Zero and Add",
   /* .opcode[0]   = */ 0xF8,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z900,
   /* .properties  = */ S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AG,
   /* .name        = */ "AG",
   /* .description = */ "(LongDisp) Add (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGF,
   /* .name        = */ "AGF",
   /* .description = */ "(LongDisp) Add (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x18,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AHY,
   /* .name        = */ "AHY",
   /* .description = */ "(LongDisp) Add Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x7A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALC,
   /* .name        = */ "ALC",
   /* .description = */ "(LongDisp) Add Logical with Carry (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x98,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALCG,
   /* .name        = */ "ALCG",
   /* .description = */ "(LongDisp) Add Logical with Carry (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x88,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALG,
   /* .name        = */ "ALG",
   /* .description = */ "(LongDisp) Add Logical (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGF,
   /* .name        = */ "ALGF",
   /* .description = */ "(LongDisp) Add Logical (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALY,
   /* .name        = */ "ALY",
   /* .description = */ "(LongDisp) Add Logical",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x5E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AY,
   /* .name        = */ "AY",
   /* .description = */ "(LongDisp) Add",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x5A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BCTG,
   /* .name        = */ "BCTG",
   /* .description = */ "(LongDisp) Branch on Count (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BXHG,
   /* .name        = */ "BXHG",
   /* .description = */ "(LongDisp) Branch on Index High",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BXLEG,
   /* .name        = */ "BXLEG",
   /* .description = */ "(LongDisp) Branch on Index Low or Equ. (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x45,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDSG,
   /* .name        = */ "CDSG",
   /* .description = */ "Compare Double and Swap",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDSY,
   /* .name        = */ "CDSY",
   /* .description = */ "Compare Double and Swap",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x31,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CG,
   /* .name        = */ "CG",
   /* .description = */ "(LongDisp) Compare (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x20,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGF,
   /* .name        = */ "CGF",
   /* .description = */ "(LongDisp) Compare (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x30,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHY,
   /* .name        = */ "CHY",
   /* .description = */ "(LongDisp) Compare Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x79,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLCLU,
   /* .name        = */ "CLCLU",
   /* .description = */ "Compare Logical Long Unicode",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x8F,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLG,
   /* .name        = */ "CLG",
   /* .description = */ "(LongDisp) Compare Logical (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x21,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGF,
   /* .name        = */ "CLGF",
   /* .description = */ "(LongDisp) Compare Logical (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x31,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLIY,
   /* .name        = */ "CLIY",
   /* .description = */ "Compare Logical Immediate",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLMH,
   /* .name        = */ "CLMH",
   /* .description = */ "Compare Logical Characters under Mask High",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x20,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLMY,
   /* .name        = */ "CLMY",
   /* .description = */ "Compare Logical Characters under Mask Y Form",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x21,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLY,
   /* .name        = */ "CLY",
   /* .description = */ "(LongDisp) Compare Logical (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CSG,
   /* .name        = */ "CSG",
   /* .description = */ "(LongDisp) Compare and Swap (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x30,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CSY,
   /* .name        = */ "CSY",
   /* .description = */ "(LongDisp) Compare and Swap (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x14,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVBG,
   /* .name        = */ "CVBG",
   /* .description = */ "Convert to Binary",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVBY,
   /* .name        = */ "CVBY",
   /* .description = */ "(LongDisp) Convert to Binary",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVDG,
   /* .name        = */ "CVDG",
   /* .description = */ "Convert to Decimal (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x2E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CVDY,
   /* .name        = */ "CVDY",
   /* .description = */ "(LongDisp) Convert to Decimal (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CY,
   /* .name        = */ "CY",
   /* .description = */ "(LongDisp) Compare (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DL,
   /* .name        = */ "DL",
   /* .description = */ "(LongDisp) Divide",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x97,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DSG,
   /* .name        = */ "DSG",
   /* .description = */ "(LongDisp) Divide Single (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DSGF,
   /* .name        = */ "DSGF",
   /* .description = */ "(LongDisp) Divide Single (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1D,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ICMH,
   /* .name        = */ "ICMH",
   /* .description = */ "(LongDisp) Insert Characters under Mask (high)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x80,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ICMY,
   /* .name        = */ "ICMY",
   /* .description = */ "(LongDisp) Insert Character under Mask",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x81,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ICY,
   /* .name        = */ "ICY",
   /* .description = */ "(LongDisp) Insert Character",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KIMD,
   /* .name        = */ "KIMD",
   /* .description = */ "Compute Intermediate Message Digest",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KLMD,
   /* .name        = */ "KLMD",
   /* .description = */ "Compute Last Message Digest",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KM,
   /* .name        = */ "KM",
   /* .description = */ "Cipher Message",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x2E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMAC,
   /* .name        = */ "KMAC",
   /* .description = */ "Compute Message Authentication Code",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x1E,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMC,
   /* .name        = */ "KMC",
   /* .description = */ "Cipher Message with Chaining",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x2F,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAMY,
   /* .name        = */ "LAMY",
   /* .description = */ "(LongDisp) Load Access Multiple",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x9A,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAY,
   /* .name        = */ "LAY",
   /* .description = */ "(LongDisp) Load Address",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x71,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LB,
   /* .name        = */ "LB",
   /* .description = */ "(LongDisp) Load Byte (31) - note it is called LB in the PoP",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x76,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCTLG,
   /* .name        = */ "LCTLG",
   /* .description = */ "Load Control",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x2F,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDY,
   /* .name        = */ "LDY",
   /* .description = */ "(LongDisp) Load (L)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x65,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEY,
   /* .name        = */ "LEY",
   /* .description = */ "(LongDisp) Load (S)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x64,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LG,
   /* .name        = */ "LG",
   /* .description = */ "(LongDisp) Load (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGB,
   /* .name        = */ "LGB",
   /* .description = */ "(LongDisp) Load Byte (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x77,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGF,
   /* .name        = */ "LGF",
   /* .description = */ "(LongDisp) Load (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x14,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32To64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGH,
   /* .name        = */ "LGH",
   /* .description = */ "(LongDisp) Load Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x15,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHY,
   /* .name        = */ "LHY",
   /* .description = */ "(LongDisp)Load Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x78,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGC,
   /* .name        = */ "LLGC",
   /* .description = */ "(LongDisp) Load Logical Character",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x90,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGF,
   /* .name        = */ "LLGF",
   /* .description = */ "(LongDisp) Load Logical Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x16,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGH,
   /* .name        = */ "LLGH",
   /* .description = */ "(LongDisp) Load Logical Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x91,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGT,
   /* .name        = */ "LLGT",
   /* .description = */ "(LongDisp) Load Logical Thirty One Bits",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x17,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LMG,
   /* .name        = */ "LMG",
   /* .description = */ "(LongDisp) Load Multiple (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LMH,
   /* .name        = */ "LMH",
   /* .description = */ "Load Multiple High",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x96,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LMY,
   /* .name        = */ "LMY",
   /* .description = */ "(LongDisp) Load Multiple",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x98,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPQ,
   /* .name        = */ "LPQ",
   /* .description = */ "(LongDisp) Load Pair from Quadword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x8F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRAG,
   /* .name        = */ "LRAG",
   /* .description = */ "Load Real Address",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRAY,
   /* .name        = */ "LRAY",
   /* .description = */ "Load Real Address Y Form",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x13,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRV,
   /* .name        = */ "LRV",
   /* .description = */ "(LongDisp) Load Reversed (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRVG,
   /* .name        = */ "LRVG",
   /* .description = */ "(LongDisp) Load Reversed (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRVH,
   /* .name        = */ "LRVH",
   /* .description = */ "(LongDisp) Load Reversed Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LY,
   /* .name        = */ "LY",
   /* .description = */ "(LongDisp) Load (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x58,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAD,
   /* .name        = */ "MAD",
   /* .description = */ "Multiply and Add, long HFP sources and result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MADR,
   /* .name        = */ "MADR",
   /* .description = */ "Multiply and Add, long HFP sources and result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAE,
   /* .name        = */ "MAE",
   /* .description = */ "Multiply and Add, short HFP sources and result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x2E,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAER,
   /* .name        = */ "MAER",
   /* .description = */ "Multiply and Add, short HFP sources and result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x2E,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MLG,
   /* .name        = */ "MLG",
   /* .description = */ "(LongDisp) Multiply Logical ( 128<64 )",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x86,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSD,
   /* .name        = */ "MSD",
   /* .description = */ "Multiply and Subtract, long HFP sources and result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSDR,
   /* .name        = */ "MSDR",
   /* .description = */ "Multiply and Subtract, long HFP sources and result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSE,
   /* .name        = */ "MSE",
   /* .description = */ "Multiply and Subtract, short HFP sources and result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x2F,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSER,
   /* .name        = */ "MSER",
   /* .description = */ "Multiply and Subtract, short HFP sources and result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x2F,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSG,
   /* .name        = */ "MSG",
   /* .description = */ "(LongDisp) Multiply Single (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGF,
   /* .name        = */ "MSGF",
   /* .description = */ "(LongDisp) Multiply Single (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSY,
   /* .name        = */ "MSY",
   /* .description = */ "(LongDisp) Multiply Single",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x51,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVCLU,
   /* .name        = */ "MVCLU",
   /* .description = */ "Move Long Unicode",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x8E,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVIY,
   /* .name        = */ "MVIY",
   /* .description = */ "(LongDisp) Move (Immediate)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x52,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NG,
   /* .name        = */ "NG",
   /* .description = */ "(LongDisp) And (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x80,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NIY,
   /* .name        = */ "NIY",
   /* .description = */ "(LongDisp) And (Immediate)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsStore |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NY,
   /* .name        = */ "NY",
   /* .description = */ "(LongDisp) And (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OG,
   /* .name        = */ "OG",
   /* .description = */ "(LongDisp) Or (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x81,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OIY,
   /* .name        = */ "OIY",
   /* .description = */ "(LongDisp) Or (Immediate)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x56,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsStore |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OY,
   /* .name        = */ "OY",
   /* .description = */ "(LongDisp) Or (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x56,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RLL,
   /* .name        = */ "RLL",
   /* .description = */ "Rotate Left Single Logical (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x1D,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RLLG,
   /* .name        = */ "RLLG",
   /* .description = */ "Rotate Left Single Logical (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x1C,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SG,
   /* .name        = */ "SG",
   /* .description = */ "(LongDisp) Subtract (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SGF,
   /* .name        = */ "SGF",
   /* .description = */ "(LongDisp) Subtract (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x19,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SHY,
   /* .name        = */ "SHY",
   /* .description = */ "(LongDisp) Subtract Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x7B,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLAG,
   /* .name        = */ "SLAG",
   /* .description = */ "(LongDisp) Shift Left Single (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLB,
   /* .name        = */ "SLB",
   /* .description = */ "(LongDisp) Subtract Logical with Borrow (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x99,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLBG,
   /* .name        = */ "SLBG",
   /* .description = */ "(LongDisp) Subtract Logical With Borrow (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x89,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLG,
   /* .name        = */ "SLG",
   /* .description = */ "(LongDisp) Subtract Logical (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLGF,
   /* .name        = */ "SLGF",
   /* .description = */ "(LongDisp) Subtract Logical (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x1B,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLLG,
   /* .name        = */ "SLLG",
   /* .description = */ "(LongDisp) Shift Left Logical (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLY,
   /* .name        = */ "SLY",
   /* .description = */ "(LongDisp) Subtract Logical (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x5F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRAG,
   /* .name        = */ "SRAG",
   /* .description = */ "(LongDisp) Shift Right Single (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRLG,
   /* .name        = */ "SRLG",
   /* .description = */ "(LongDisp) Shift Right Logical (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STAMY,
   /* .name        = */ "STAMY",
   /* .description = */ "(LongDisp) Set Access Multiple",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x9B,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCMH,
   /* .name        = */ "STCMH",
   /* .description = */ "(LongDisp) Store Characters under Mask (high)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x2C,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCMY,
   /* .name        = */ "STCMY",
   /* .description = */ "(LongDisp) Store Characters under Mask (low)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x2D,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCTG,
   /* .name        = */ "STCTG",
   /* .description = */ "Store Control",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCY,
   /* .name        = */ "STCY",
   /* .description = */ "(LongDisp) Store Character",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x72,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STDY,
   /* .name        = */ "STDY",
   /* .description = */ "(LongDisp) Store (S)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x67,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STEY,
   /* .name        = */ "STEY",
   /* .description = */ "(LongDisp) Store (L)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x66,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STG,
   /* .name        = */ "STG",
   /* .description = */ "(LongDisp) Store (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x24,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STHY,
   /* .name        = */ "STHY",
   /* .description = */ "(LongDisp) Store Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x70,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STMG,
   /* .name        = */ "STMG",
   /* .description = */ "(LongDisp) Store Multiple (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x24,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STMH,
   /* .name        = */ "STMH",
   /* .description = */ "Store Multiple High",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STMY,
   /* .name        = */ "STMY",
   /* .description = */ "(LongDisp) Store Multiple",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x90,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STPQ,
   /* .name        = */ "STPQ",
   /* .description = */ "(LongDisp) Store Pair to Quadword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x8E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRV,
   /* .name        = */ "STRV",
   /* .description = */ "(LongDisp) Store (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRVG,
   /* .name        = */ "STRVG",
   /* .description = */ "(LongDisp) Store Reversed (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x2F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRVH,
   /* .name        = */ "STRVH",
   /* .description = */ "(LongDisp) Store Reversed Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STY,
   /* .name        = */ "STY",
   /* .description = */ "(LongDisp) Store (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x50,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SY,
   /* .name        = */ "SY",
   /* .description = */ "(LongDisp) Subtract (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x5B,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TMY,
   /* .name        = */ "TMY",
   /* .description = */ "Test under Mask",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x51,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XG,
   /* .name        = */ "XG",
   /* .description = */ "(LongDisp) Exclusive Or (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x82,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XIY,
   /* .name        = */ "XIY",
   /* .description = */ "(LongDisp) Exclusive Or (immediate)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x57,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XY,
   /* .name        = */ "XY",
   /* .description = */ "(LongDisp) Exclusive Or (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x57,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z990,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ADTR,
   /* .name        = */ "ADTR",
   /* .description = */ "Add (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD2,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AFI,
   /* .name        = */ "AFI",
   /* .description = */ "Add Immediate (32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGFI,
   /* .name        = */ "AGFI",
   /* .description = */ "Add Immediate (64<32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALFI,
   /* .name        = */ "ALFI",
   /* .description = */ "Add Logical Immediate (32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGFI,
   /* .name        = */ "ALGFI",
   /* .description = */ "Add Logical Immediate (64<32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AXTR,
   /* .name        = */ "AXTR",
   /* .description = */ "Add (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDA,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDGTR,
   /* .name        = */ "CDGTR",
   /* .description = */ "Convert from Fixed (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF1,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDSTR,
   /* .name        = */ "CDSTR",
   /* .description = */ "Convert from Signed Packed",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF3,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDTR,
   /* .name        = */ "CDTR",
   /* .description = */ "Compare (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE4,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDUTR,
   /* .name        = */ "CDUTR",
   /* .description = */ "Convert From Unsigned BCD (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF2,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEDTR,
   /* .name        = */ "CEDTR",
   /* .description = */ "Compare biased exponent (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF4,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CEXTR,
   /* .name        = */ "CEXTR",
   /* .description = */ "Compare biased exponent (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFC,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_UsesRegPairForSource
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CFI,
   /* .name        = */ "CFI",
   /* .description = */ "Compare Immediate (32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGDTR,
   /* .name        = */ "CGDTR",
   /* .description = */ "Convert to Fixed (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE1,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFI,
   /* .name        = */ "CGFI",
   /* .description = */ "Compare Immediate (64<32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGXTR,
   /* .name        = */ "CGXTR",
   /* .description = */ "Convert to Fixed (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE9,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFI,
   /* .name        = */ "CLFI",
   /* .description = */ "Compare Logical Immediate (32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFI,
   /* .name        = */ "CLGFI",
   /* .description = */ "Compare Logical Immediate (64<32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CPSDR,
   /* .name        = */ "CPSDR",
   /* .description = */ "Copy Sign",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x72,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CSDTR,
   /* .name        = */ "CSDTR",
   /* .description = */ "Convert to signed packed",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE3,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CSXTR,
   /* .name        = */ "CSXTR",
   /* .description = */ "Convert to signed packed(DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xEB,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CU14,
   /* .name        = */ "CU14",
   /* .description = */ "Convert UTF-8 to UTF-32",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xB0,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CU24,
   /* .name        = */ "CU24",
   /* .description = */ "Convert UTF-16 to UTF-32",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xB1,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CU41,
   /* .name        = */ "CU41",
   /* .description = */ "Convert UTF-32 to UTF-8",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xB2,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CU42,
   /* .name        = */ "CU42",
   /* .description = */ "Convert UTF-32 to UTF-16",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xB3,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CUDTR,
   /* .name        = */ "CUDTR",
   /* .description = */ "Convert to Unsigned BCD (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE2,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CUTFU,
   /* .name        = */ "CUTFU",
   /* .description = */ "Convert UTF-8 to Unicode",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xA7,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CUUTF,
   /* .name        = */ "CUUTF",
   /* .description = */ "Convert Unicode to UTF-8",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xA6,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CUXTR,
   /* .name        = */ "CUXTR",
   /* .description = */ "Convert to Unsigned BCD (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xEA,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXGTR,
   /* .name        = */ "CXGTR",
   /* .description = */ "Convert from Fixed (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF9,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXSTR,
   /* .name        = */ "CXSTR",
   /* .description = */ "Convert from Signed Packed to DFP128",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFB,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXTR,
   /* .name        = */ "CXTR",
   /* .description = */ "Compare (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xEC,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXUTR,
   /* .name        = */ "CXUTR",
   /* .description = */ "Convert From Unsigned BCD (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFA,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DDTR,
   /* .name        = */ "DDTR",
   /* .description = */ "Divide (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD1,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsFPC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::DXTR,
   /* .name        = */ "DXTR",
   /* .description = */ "Divide (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD9,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsFPC |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EEDTR,
   /* .name        = */ "EEDTR",
   /* .description = */ "Extract Biased Exponent (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE5,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EEXTR,
   /* .name        = */ "EEXTR",
   /* .description = */ "Extract Biased Exponent (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xED,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ESDTR,
   /* .name        = */ "ESDTR",
   /* .description = */ "Extract Significance (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE7,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ESXTR,
   /* .name        = */ "ESXTR",
   /* .description = */ "Extract Significance (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xEF,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIDTR,
   /* .name        = */ "FIDTR",
   /* .description = */ "Load FP Integer (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD7,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FIXTR,
   /* .name        = */ "FIXTR",
   /* .description = */ "Load FP Integer (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDF,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::FLOGR,
   /* .name        = */ "FLOGR",
   /* .description = */ "Find Leftmost One",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x83,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IEDTR,
   /* .name        = */ "IEDTR",
   /* .description = */ "Insert Biased Exponent (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF6,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IEXTR,
   /* .name        = */ "IEXTR",
   /* .description = */ "Insert Biased Exponent (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFE,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IIHF,
   /* .name        = */ "IIHF",
   /* .description = */ "Insert Immediate (high)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::IILF,
   /* .name        = */ "IILF",
   /* .description = */ "Insert Immediate (low)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KDTR,
   /* .name        = */ "KDTR",
   /* .description = */ "Compare (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE0,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KXTR,
   /* .name        = */ "KXTR",
   /* .description = */ "Compare (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xE8,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LBR,
   /* .name        = */ "LBR",
   /* .description = */ "Load Byte (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x26,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCDFR,
   /* .name        = */ "LCDFR",
   /* .description = */ "Load Complement (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDETR,
   /* .name        = */ "LDETR",
   /* .description = */ "Load Lengthened (64DFP < 32DFP)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD4,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDGR,
   /* .name        = */ "LDGR",
   /* .description = */ "Load FPR from GR (SB, DB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xC1,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LDXTR,
   /* .name        = */ "LDXTR",
   /* .description = */ "Load Rounded (64DFP < 128DFP)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDD,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LEDTR,
   /* .name        = */ "LEDTR",
   /* .description = */ "Load Rounded (32DFP < 64DFP)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD5,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGBR,
   /* .name        = */ "LGBR",
   /* .description = */ "Load Byte (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGDR,
   /* .name        = */ "LGDR",
   /* .description = */ "Load GR from FPR (SB, DB)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xCD,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_DoubleFP |
                        S390OpProp_SingleFP |
                        S390OpProp_DoubleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGFI,
   /* .name        = */ "LGFI",
   /* .description = */ "Load Immediate (64<32)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGHR,
   /* .name        = */ "LGHR",
   /* .description = */ "Load Halfword (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHR,
   /* .name        = */ "LHR",
   /* .description = */ "Load Halfword (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x27,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLC,
   /* .name        = */ "LLC",
   /* .description = */ "(LongDisp) Load Logical Character (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x94,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLCR,
   /* .name        = */ "LLCR",
   /* .description = */ "Load Logical Character (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x94,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGCR,
   /* .name        = */ "LLGCR",
   /* .description = */ "Load Logical Character (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x84,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGHR,
   /* .name        = */ "LLGHR",
   /* .description = */ "Load Logical Halfword(64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x85,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLH,
   /* .name        = */ "LLH",
   /* .description = */ "(LongDisp) Load Logical Halfword",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x95,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHR,
   /* .name        = */ "LLHR",
   /* .description = */ "Load Logical Halfword(32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x95,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLIHF,
   /* .name        = */ "LLIHF",
   /* .description = */ "Load Logical Immediate (high)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLILF,
   /* .name        = */ "LLILF",
   /* .description = */ "Load Logical Immediate (low)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LNDFR,
   /* .name        = */ "LNDFR",
   /* .description = */ "Load Negative (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x71,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPDFR,
   /* .name        = */ "LPDFR",
   /* .description = */ "Load Positive (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x70,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LT,
   /* .name        = */ "LT",
   /* .description = */ "(LongDisp) Load and Test (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTDTR,
   /* .name        = */ "LTDTR",
   /* .description = */ "Load and Test (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD6,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTG,
   /* .name        = */ "LTG",
   /* .description = */ "(LongDisp) Load and Test (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTXTR,
   /* .name        = */ "LTXTR",
   /* .description = */ "Load and Test (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDE,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsRegCopy |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LXDTR,
   /* .name        = */ "LXDTR",
   /* .description = */ "Load Lengthened(64DFP < 128DFP)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDC,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAY,
   /* .name        = */ "MAY",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3A,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAYH,
   /* .name        = */ "MAYH",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and high-order part of extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAYHR,
   /* .name        = */ "MAYHR",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and high-order part of extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAYL,
   /* .name        = */ "MAYL",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and low-order part of extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x38,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAYLR,
   /* .name        = */ "MAYLR",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and low-order part of extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x38,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MAYR,
   /* .name        = */ "MAYR",
   /* .description = */ "Multiply and Add Unnormalized, long HFP sources and extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3A,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MDTR,
   /* .name        = */ "MDTR",
   /* .description = */ "Multiply (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD0,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MXTR,
   /* .name        = */ "MXTR",
   /* .description = */ "Multiply (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD8,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MY,
   /* .name        = */ "MY",
   /* .description = */ "Multiply Unnormalized, long HFP sources and extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3B,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MYH,
   /* .name        = */ "MYH",
   /* .description = */ "Multiply Unnormalized, long HFP sources and high-order part of extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x3D,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MYHR,
   /* .name        = */ "MYHR",
   /* .description = */ "Multiply Unnormalized, long HFP sources and high-order part of extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3D,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MYL,
   /* .name        = */ "MYL",
   /* .description = */ "Multiply Unnormalized, long HFP sources and low-order part of extended HFP result",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x39,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MYLR,
   /* .name        = */ "MYLR",
   /* .description = */ "Multiply Unnormalized, long HFP sources and low-order part of extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x39,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MYR,
   /* .name        = */ "MYR",
   /* .description = */ "Multiply Unnormalized, long HFP sources and extended HFP result",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x3B,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NIHF,
   /* .name        = */ "NIHF",
   /* .description = */ "And Immediate (high)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NILF,
   /* .name        = */ "NILF",
   /* .description = */ "And Immediate (low)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OIHF,
   /* .name        = */ "OIHF",
   /* .description = */ "Or Immediate (high)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OILF,
   /* .name        = */ "OILF",
   /* .description = */ "Or Immediate (low)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PFPO,
   /* .name        = */ "PFPO",
   /* .description = */ "perform floating point operations.",
   /* .opcode[0]   = */ 0x01,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ E_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_ImplicitlyUsesGPR0
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::QADTR,
   /* .name        = */ "QADTR",
   /* .description = */ "Quantize (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF5,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsFPC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::QAXTR,
   /* .name        = */ "QAXTR",
   /* .description = */ "Quantize (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFD,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsFPC |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RRDTR,
   /* .name        = */ "RRDTR",
   /* .description = */ "Reround (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xF7,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RRXTR,
   /* .name        = */ "RRXTR",
   /* .description = */ "Reround (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xFF,
   /* .format      = */ RRF3_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SDTR,
   /* .name        = */ "SDTR",
   /* .description = */ "Subtract (DFP64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xD3,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SFASR,
   /* .name        = */ "SFASR",
   /* .description = */ "Set FPC And Signal",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x85,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsFPC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLDT,
   /* .name        = */ "SLDT",
   /* .description = */ "Shift Left Double (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x40,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLFI,
   /* .name        = */ "SLFI",
   /* .description = */ "Subtract Logical Immediate (32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLGFI,
   /* .name        = */ "SLGFI",
   /* .description = */ "Subtract Logical Immediate (64<32)",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLXT,
   /* .name        = */ "SLXT",
   /* .description = */ "Shift Left Double (DFP128)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x48,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRDT,
   /* .name        = */ "SRDT",
   /* .description = */ "Shift Right Double (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x41,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRNMT,
   /* .name        = */ "SRNMT",
   /* .description = */ "Set RoundingMode (DFP64)",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xB9,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRSTU,
   /* .name        = */ "SRSTU",
   /* .description = */ "Search String Unicode",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xBE,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRXT,
   /* .name        = */ "SRXT",
   /* .description = */ "Shift Right LongDouble (DFP128)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x49,
   /* .format      = */ RXF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesTarget |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SSKE,
   /* .name        = */ "SSKE",
   /* .description = */ "Set Storage Key Extended",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x2B,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCKF,
   /* .name        = */ "STCKF",
   /* .description = */ "Store Clock Fast",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0x7C,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SXTR,
   /* .name        = */ "SXTR",
   /* .description = */ "Subtract (DFP128)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xDB,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDCDT,
   /* .name        = */ "TDCDT",
   /* .description = */ "Test Data Class (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDCET,
   /* .name        = */ "TDCET",
   /* .description = */ "Test Data Class (DFP32)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x50,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDCXT,
   /* .name        = */ "TDCXT",
   /* .description = */ "Test Data Class (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x58,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDGDT,
   /* .name        = */ "TDGDT",
   /* .description = */ "Test Data Group (DFP64)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDGET,
   /* .name        = */ "TDGET",
   /* .description = */ "Test Data Group (DFP32)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x51,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TDGXT,
   /* .name        = */ "TDGXT",
   /* .description = */ "Test Data Group (DFP128)",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TROO,
   /* .name        = */ "TROO",
   /* .description = */ "Translate One to One",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x93,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TROT,
   /* .name        = */ "TROT",
   /* .description = */ "Translate One to Two",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x92,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRTO,
   /* .name        = */ "TRTO",
   /* .description = */ "Translate Two to One",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x91,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRTR,
   /* .name        = */ "TRTR",
   /* .description = */ "Translate and Test Reverse",
   /* .opcode[0]   = */ 0xD0,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR2 |
                        S390OpProp_ImplicitlySetsGPR2 |
                        S390OpProp_ImplicitlySetsGPR1 |
                        S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_HasTwoMemoryReferences
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRTT,
   /* .name        = */ "TRTT",
   /* .description = */ "Translate Two to Two",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x90,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_ImplicitlyUsesGPR0 |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XIHF,
   /* .name        = */ "XIHF",
   /* .description = */ "Exclusive Or Immediate (high)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XILF,
   /* .name        = */ "XILF",
   /* .description = */ "Exclusive Or Immediate (low)",
   /* .opcode[0]   = */ 0xC0,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z9,
   /* .properties  = */ S390OpProp_TargetLW |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGSI,
   /* .name        = */ "AGSI",
   /* .description = */ "Add Direct to Memory (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x7a,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGSI,
   /* .name        = */ "ALGSI",
   /* .description = */ "Add Logical Direct to Memory (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x7E,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALSI,
   /* .name        = */ "ALSI",
   /* .description = */ "Add Logical Direct to Memory",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x6E,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ASI,
   /* .name        = */ "ASI",
   /* .description = */ "Add Direct to Memory",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x6a,
   /* .format      = */ SIY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGFRL,
   /* .name        = */ "CGFRL",
   /* .description = */ "Compare Relative Long (32 < 64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32To64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGHRL,
   /* .name        = */ "CGHRL",
   /* .description = */ "Compare Halfword Relative Long (64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGHSI,
   /* .name        = */ "CGHSI",
   /* .description = */ "Compare Direct to Memory Halfword Immediate (64)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x58,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGIB,
   /* .name        = */ "CGIB",
   /* .description = */ "Compare Immediate And Branch (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xFC,
   /* .format      = */ RIS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGIJ,
   /* .name        = */ "CGIJ",
   /* .description = */ "Compare Immediate And Branch Relative (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x7C,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGIT,
   /* .name        = */ "CGIT",
   /* .description = */ "Compare Immidiate And Trap (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x70,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGRB,
   /* .name        = */ "CGRB",
   /* .description = */ "Compare And Branch (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xE4,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGRJ,
   /* .name        = */ "CGRJ",
   /* .description = */ "Compare And Branch Relative (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x64,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGRL,
   /* .name        = */ "CGRL",
   /* .description = */ "Compare Relative Long (64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CGRT,
   /* .name        = */ "CGRT",
   /* .description = */ "Compare And Trap (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x60,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHHSI,
   /* .name        = */ "CHHSI",
   /* .description = */ "Compare Direct to Memory Halfword Immediate (16)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHRL,
   /* .name        = */ "CHRL",
   /* .description = */ "Compare Halfword Relative Long (32)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHSI,
   /* .name        = */ "CHSI",
   /* .description = */ "Compare Direct to Memory Halfword Immediate (32)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x5C,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CIB,
   /* .name        = */ "CIB",
   /* .description = */ "Compare Immediate And Branch (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xFE,
   /* .format      = */ RIS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CIJ,
   /* .name        = */ "CIJ",
   /* .description = */ "Compare Immediate And Branch Relative(32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x7E,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CIT,
   /* .name        = */ "CIT",
   /* .description = */ "Compare Immidiate And Trap (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x72,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFHSI,
   /* .name        = */ "CLFHSI",
   /* .description = */ "Compare Logical Immediate (32)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x5D,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFIT,
   /* .name        = */ "CLFIT",
   /* .description = */ "Compare Logical Immidiate And Trap (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGFRL,
   /* .name        = */ "CLGFRL",
   /* .description = */ "Compare Logical Relative Long (32 < 64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGHRL,
   /* .name        = */ "CLGHRL",
   /* .description = */ "Compare Logical Relative Long Halfword (64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGHSI,
   /* .name        = */ "CLGHSI",
   /* .description = */ "Compare Logical Immediate (64)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGIB,
   /* .name        = */ "CLGIB",
   /* .description = */ "Compare Logical Immediate And Branch (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xFD,
   /* .format      = */ RIS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGIJ,
   /* .name        = */ "CLGIJ",
   /* .description = */ "Compare Logical Immediate And Branch Relative (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x7D,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGIT,
   /* .name        = */ "CLGIT",
   /* .description = */ "Compare Logical Immidiate And Trap (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x71,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGRB,
   /* .name        = */ "CLGRB",
   /* .description = */ "Compare Logical And Branch (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xE5,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGRJ,
   /* .name        = */ "CLGRJ",
   /* .description = */ "Compare Logical And Branch Relative (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x65,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGRL,
   /* .name        = */ "CLGRL",
   /* .description = */ "Compare Logical Relative Long (64)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGRT,
   /* .name        = */ "CLGRT",
   /* .description = */ "Compare Logical And Trap (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x61,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHHSI,
   /* .name        = */ "CLHHSI",
   /* .description = */ "Compare Logical Immediate (16)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHRL,
   /* .name        = */ "CLHRL",
   /* .description = */ "Compare Logical Relative Long Halfword (32)",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLIB,
   /* .name        = */ "CLIB",
   /* .description = */ "Compare Logical Immediate And Branch (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xFF,
   /* .format      = */ RIS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLIJ,
   /* .name        = */ "CLIJ",
   /* .description = */ "Compare Logical Immidiate And Branch Relative (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x7F,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLRB,
   /* .name        = */ "CLRB",
   /* .description = */ "Compare Logical And Branch (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xF7,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLRJ,
   /* .name        = */ "CLRJ",
   /* .description = */ "Compare Logical And Branch Relative (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x77,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLRL,
   /* .name        = */ "CLRL",
   /* .description = */ "Compare Logical Relative Long",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLRT,
   /* .name        = */ "CLRT",
   /* .description = */ "Compare Logical And Trap (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CRB,
   /* .name        = */ "CRB",
   /* .description = */ "Compare And Branch (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xF6,
   /* .format      = */ RRS_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CRJ,
   /* .name        = */ "CRJ",
   /* .description = */ "Compare And Branch Relative (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x76,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_BranchOp |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CRL,
   /* .name        = */ "CRL",
   /* .description = */ "Compare Relative Long",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CRT,
   /* .name        = */ "CRT",
   /* .description = */ "Compare And Trap (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x72,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ECAG,
   /* .name        = */ "ECAG",
   /* .description = */ "Extract Cache Attribute",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x4C,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::EXRL,
   /* .name        = */ "EXRL",
   /* .description = */ "Execute Relative Long",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAEY,
   /* .name        = */ "LAEY",
   /* .description = */ "Load Address Extended Y Form",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x75,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGFRL,
   /* .name        = */ "LGFRL",
   /* .description = */ "Load Relative Long (64 < 32)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x0C,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGRL,
   /* .name        = */ "LGRL",
   /* .description = */ "Load Relative Long (64)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGFRL,
   /* .name        = */ "LLGFRL",
   /* .description = */ "Load Logical Relative Long (64 < 32)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LRL,
   /* .name        = */ "LRL",
   /* .description = */ "Load Relative Long (32)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LTGF,
   /* .name        = */ "LTGF",
   /* .description = */ "(LongDisp) Load and Test (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x32,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is32To64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MC,
   /* .name        = */ "MC",
   /* .description = */ "Monitor Call",
   /* .opcode[0]   = */ 0xAF,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MFY,
   /* .name        = */ "MFY",
   /* .description = */ "Multiply (64 < 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x5C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MHY,
   /* .name        = */ "MHY",
   /* .description = */ "Multiply Halfword (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x7C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSFI,
   /* .name        = */ "MSFI",
   /* .description = */ "Multiply Single Immediate",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGFI,
   /* .name        = */ "MSGFI",
   /* .description = */ "Multiply Single Immediate",
   /* .opcode[0]   = */ 0xC2,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVGHI,
   /* .name        = */ "MVGHI",
   /* .description = */ "Move and store immediate (64)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x48,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVHHI,
   /* .name        = */ "MVHHI",
   /* .description = */ "Move and store immediate (16)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MVHI,
   /* .name        = */ "MVHI",
   /* .description = */ "Move and store immediate (32)",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x4C,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PFD,
   /* .name        = */ "PFD",
   /* .description = */ "Prefetch Data",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x36,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PFDRL,
   /* .name        = */ "PFDRL",
   /* .description = */ "Prefetch Data Relative Long",
   /* .opcode[0]   = */ 0xC6,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RISBG,
   /* .name        = */ "RISBG",
   /* .description = */ "Rotate Then Insert Selected Bits",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x55,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RNSBG,
   /* .name        = */ "RNSBG",
   /* .description = */ "Rotate Then AND Selected Bits",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x54,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ROSBG,
   /* .name        = */ "ROSBG",
   /* .description = */ "Rotate Then OR Selected Bits",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x56,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RXSBG,
   /* .name        = */ "RXSBG",
   /* .description = */ "Rotate Then XOR Selected Bits",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x57,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STGRL,
   /* .name        = */ "STGRL",
   /* .description = */ "Store Relative Long (64)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STRL,
   /* .name        = */ "STRL",
   /* .description = */ "Store Relative Long (32)",
   /* .opcode[0]   = */ 0xC4,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRTE,
   /* .name        = */ "TRTE",
   /* .description = */ "Translate and Test Extended",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xBF,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TRTRE,
   /* .name        = */ "TRTRE",
   /* .description = */ "Translate and Test Reversed Extended",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xBD,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z10,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_ImplicitlyUsesGPR1 |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGHIK,
   /* .name        = */ "AGHIK",
   /* .description = */ "Add Immediate (64 < 16)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xD9,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGRK,
   /* .name        = */ "AGRK",
   /* .description = */ "Add (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE8,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AHHHR,
   /* .name        = */ "AHHHR",
   /* .description = */ "Add High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xC8,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_Src2HW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AHHLR,
   /* .name        = */ "AHHLR",
   /* .description = */ "Add High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xD8,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_Src2LW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AHIK,
   /* .name        = */ "AHIK",
   /* .description = */ "Add Immediate (32 < 16)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xD8,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AIH,
   /* .name        = */ "AIH",
   /* .description = */ "Add Immediate High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGHSIK,
   /* .name        = */ "ALGHSIK",
   /* .description = */ "Add Logicial With Signed Immediate (64 < 16)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xDB,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALGRK,
   /* .name        = */ "ALGRK",
   /* .description = */ "Add Logical (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xEA,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALHHHR,
   /* .name        = */ "ALHHHR",
   /* .description = */ "Add Logical High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xCA,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_Src2HW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALHHLR,
   /* .name        = */ "ALHHLR",
   /* .description = */ "Add Logical High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xDA,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_Src2LW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALHSIK,
   /* .name        = */ "ALHSIK",
   /* .description = */ "Add Logicial With Signed Immediate (32 < 16)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0xDA,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALRK,
   /* .name        = */ "ALRK",
   /* .description = */ "Add Logical (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xFA,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALSIH,
   /* .name        = */ "ALSIH",
   /* .description = */ "Add Logical with Signed Immediate High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsZeroFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ALSIHN,
   /* .name        = */ "ALSIHN",
   /* .description = */ "Add Logical with Signed Immediate High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ARK,
   /* .name        = */ "ARK",
   /* .description = */ "Add (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF8,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BRCTH,
   /* .name        = */ "BRCTH",
   /* .description = */ "Branch Rel. on Count High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_BranchOp |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDLFBR,
   /* .name        = */ "CDLFBR",
   /* .description = */ "Convert from Logical (LB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x91,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDLGBR,
   /* .name        = */ "CDLGBR",
   /* .description = */ "Convert from Logical (LB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA1,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CELFBR,
   /* .name        = */ "CELFBR",
   /* .description = */ "Convert from Logical (SB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x90,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SingleFP |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CELGBR,
   /* .name        = */ "CELGBR",
   /* .description = */ "Convert from Logical (SB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA0,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHF,
   /* .name        = */ "CHF",
   /* .description = */ "(LongDisp) Compare High (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xCD,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHHR,
   /* .name        = */ "CHHR",
   /* .description = */ "Compare High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xCD,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CHLR,
   /* .name        = */ "CHLR",
   /* .description = */ "Compare High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xDD,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CIH,
   /* .name        = */ "CIH",
   /* .description = */ "Compare Immediate High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x0D,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFDBR,
   /* .name        = */ "CLFDBR",
   /* .description = */ "Convert to Logical (LB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x9D,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFEBR,
   /* .name        = */ "CLFEBR",
   /* .description = */ "Convert to Logical (SB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x9C,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLFXBR,
   /* .name        = */ "CLFXBR",
   /* .description = */ "Convert to Logical (EB < 32), note here",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x9E,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGDBR,
   /* .name        = */ "CLGDBR",
   /* .description = */ "Convert to Logical (LB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xAD,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGEBR,
   /* .name        = */ "CLGEBR",
   /* .description = */ "Convert to Logical (SB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xAC,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SingleFP |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGXBR,
   /* .name        = */ "CLGXBR",
   /* .description = */ "Convert to Logical (EB < 64), note here",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xAE,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHF,
   /* .name        = */ "CLHF",
   /* .description = */ "(LongDisp) Compare Logical High (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xCF,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHHR,
   /* .name        = */ "CLHHR",
   /* .description = */ "Compare Logical High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xCF,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLHLR,
   /* .name        = */ "CLHLR",
   /* .description = */ "Compare Logical High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xDF,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLIH,
   /* .name        = */ "CLIH",
   /* .description = */ "Compare Logical Immediate High (32)",
   /* .opcode[0]   = */ 0xCC,
   /* .opcode[1]   = */ 0x0F,
   /* .format      = */ RIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsCompare |
                        S390OpProp_SetsCompareFlag |
                        S390OpProp_IsExtendedImmediate |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXLFBR,
   /* .name        = */ "CXLFBR",
   /* .description = */ "Convert from Logical (EB < 32)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0x92,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXLGBR,
   /* .name        = */ "CXLGBR",
   /* .description = */ "Convert from Logical (EB < 64)",
   /* .opcode[0]   = */ 0xB3,
   /* .opcode[1]   = */ 0xA2,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMCTR,
   /* .name        = */ "KMCTR",
   /* .description = */ "Cipher Message with Counter",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x2D,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMF,
   /* .name        = */ "KMF",
   /* .description = */ "Cipher Message with CFB (Cipher Feedback)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x2A,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMO,
   /* .name        = */ "KMO",
   /* .description = */ "Cipher Message with OFB (Output Feedback)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x2B,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesRegPairForSource |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAA,
   /* .name        = */ "LAA",
   /* .description = */ "Load And Add (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF8,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAAG,
   /* .name        = */ "LAAG",
   /* .description = */ "Load And Add (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE8,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAAL,
   /* .name        = */ "LAAL",
   /* .description = */ "Load And Add Logical (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xFA,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAALG,
   /* .name        = */ "LAALG",
   /* .description = */ "Load And Add Logical (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xEA,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAN,
   /* .name        = */ "LAN",
   /* .description = */ "(LongDisp) Load And AND (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF4,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LANG,
   /* .name        = */ "LANG",
   /* .description = */ "(LongDisp) Load And AND (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE4,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAO,
   /* .name        = */ "LAO",
   /* .description = */ "(LongDisp) Load And OR (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF6,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAOG,
   /* .name        = */ "LAOG",
   /* .description = */ "(LongDisp) Load And OR (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE6,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAX,
   /* .name        = */ "LAX",
   /* .description = */ "(LongDisp) Load And Exclusive OR (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF7,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAXG,
   /* .name        = */ "LAXG",
   /* .description = */ "(LongDisp) Load And Exclusive OR (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE7,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_IsStore |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LBH,
   /* .name        = */ "LBH",
   /* .description = */ "(LongDisp) Load Byte High (32 < 8)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC0,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LFH,
   /* .name        = */ "LFH",
   /* .description = */ "(LongDisp) Load High (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xCA,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LHH,
   /* .name        = */ "LHH",
   /* .description = */ "(LongDisp) Load Halfword High (32 < 16)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC4,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLCH,
   /* .name        = */ "LLCH",
   /* .description = */ "(Long Disp)Load Logical Character High (32 < 8)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC2,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLHH,
   /* .name        = */ "LLHH",
   /* .description = */ "(LongDisp) Load Logical Halfword High (32 < 8)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC6,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOC,
   /* .name        = */ "LOC",
   /* .description = */ "(LongDisp) Load On Condition (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF2,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCG,
   /* .name        = */ "LOCG",
   /* .description = */ "(LongDisp) Load On Condition (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE2,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCGR,
   /* .name        = */ "LOCGR",
   /* .description = */ "Load On Condition (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE2,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCR,
   /* .name        = */ "LOCR",
   /* .description = */ "Load On Condition (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF2,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPD,
   /* .name        = */ "LPD",
   /* .description = */ "Load Pair Disjoint (32)",
   /* .opcode[0]   = */ 0xC8,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ SSF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LPDG,
   /* .name        = */ "LPDG",
   /* .description = */ "Load Pair Disjoint (64)",
   /* .opcode[0]   = */ 0xC8,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ SSF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_HasTwoMemoryReferences |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NGRK,
   /* .name        = */ "NGRK",
   /* .description = */ "And (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE4,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NRK,
   /* .name        = */ "NRK",
   /* .description = */ "And (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF4,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::OGRK,
   /* .name        = */ "OGRK",
   /* .description = */ "Or (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE6,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ORK,
   /* .name        = */ "ORK",
   /* .description = */ "Or (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF6,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::POPCNT,
   /* .name        = */ "POPCNT",
   /* .description = */ "Population Count",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE1,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RISBHG,
   /* .name        = */ "RISBHG",
   /* .description = */ "Rotate Then Insert Selected Bits High",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x5D,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RISBLG,
   /* .name        = */ "RISBLG",
   /* .description = */ "Rotate Then Insert Selected Bits Low",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x51,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_UsesTarget |
                        S390OpProp_TargetLW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SGRK,
   /* .name        = */ "SGRK",
   /* .description = */ "Subtract (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE9,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SHHHR,
   /* .name        = */ "SHHHR",
   /* .description = */ "Subtract High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xC9,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SHHLR,
   /* .name        = */ "SHHLR",
   /* .description = */ "Subtract High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xD9,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLAK,
   /* .name        = */ "SLAK",
   /* .description = */ "(LongDisp) Shift Left Single (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xDD,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLGRK,
   /* .name        = */ "SLGRK",
   /* .description = */ "Subtract (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xEB,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLHHHR,
   /* .name        = */ "SLHHHR",
   /* .description = */ "Subtract High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xCB,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcHW |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLHHLR,
   /* .name        = */ "SLHHLR",
   /* .description = */ "Subtract High (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xDB,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_TargetHW |
                        S390OpProp_SrcLW |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLLK,
   /* .name        = */ "SLLK",
   /* .description = */ "(LongDisp) Shift Left Logical (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xDF,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SLRK,
   /* .name        = */ "SLRK",
   /* .description = */ "Subtract (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xFB,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRAK,
   /* .name        = */ "SRAK",
   /* .description = */ "(LongDisp) Shift Right Single (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xDC,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRK,
   /* .name        = */ "SRK",
   /* .description = */ "Subtract (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF9,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SRLK,
   /* .name        = */ "SRLK",
   /* .description = */ "(LongDisp) Shift Right Logical (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xDE,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STCH,
   /* .name        = */ "STCH",
   /* .description = */ "(LongDisp) Store Character High (8)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC3,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STFH,
   /* .name        = */ "STFH",
   /* .description = */ "(LongDisp) Store High (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xCB,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STHH,
   /* .name        = */ "STHH",
   /* .description = */ "(LongDisp) Store Halfword High (16)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC7,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STOC,
   /* .name        = */ "STOC",
   /* .description = */ "(LongDisp) Store On Condition (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xF3,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is32Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STOCG,
   /* .name        = */ "STOCG",
   /* .description = */ "(LongDisp) Store On Condition (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE3,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_Is64Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XGRK,
   /* .name        = */ "XGRK",
   /* .description = */ "Exclusive Or (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE7,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::XRK,
   /* .name        = */ "XRK",
   /* .description = */ "Exclusive Or (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xF7,
   /* .format      = */ RRFa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z196,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsZeroFlag |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BPP,
   /* .name        = */ "BPP",
   /* .description = */ "Branch Prediction Preload",
   /* .opcode[0]   = */ 0xC7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ SMI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BPRP,
   /* .name        = */ "BPRP",
   /* .description = */ "Branch Prediction Relative Preload",
   /* .opcode[0]   = */ 0xC5,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ MII_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDZT,
   /* .name        = */ "CDZT",
   /* .description = */ "Convert Zoned to DFP Long",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAA,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLGT,
   /* .name        = */ "CLGT",
   /* .description = */ "Compare Logical and Trap (64)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x2B,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_Trap |
                        S390OpProp_IsLoad
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CLT,
   /* .name        = */ "CLT",
   /* .description = */ "Compare Logical and Trap (32)",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0x23,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsCompare |
                        S390OpProp_IsLoad |
                        S390OpProp_Trap
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXZT,
   /* .name        = */ "CXZT",
   /* .description = */ "Convert Zoned to DFP Extended",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAB,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CZDT,
   /* .name        = */ "CZDT",
   /* .description = */ "Convert DFP Long to Zoned",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xA8,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_SetsSignFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CZXT,
   /* .name        = */ "CZXT",
   /* .description = */ "Convert DFP Extended to Zoned",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xA9,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::ETND,
   /* .name        = */ "ETND",
   /* .description = */ "Extract Transaction Nesting Depth",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xEC,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LAT,
   /* .name        = */ "LAT",
   /* .description = */ "Load and Trap",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x9F,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_Trap |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LFHAT,
   /* .name        = */ "LFHAT",
   /* .description = */ "Load High and Trap",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0xC8,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Trap |
                        S390OpProp_LongDispSupported |
                        S390OpProp_TargetHW |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGAT,
   /* .name        = */ "LGAT",
   /* .description = */ "Load and Trap (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x85,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_Trap |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGFAT,
   /* .name        = */ "LLGFAT",
   /* .description = */ "Load Logical and Trap",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x9D,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_Trap |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGTAT,
   /* .name        = */ "LLGTAT",
   /* .description = */ "Load Logical Thirty One Bits and Trap",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x9C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_Trap |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NIAI,
   /* .name        = */ "NIAI",
   /* .description = */ "Next Instruction Access Intent",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xFA,
   /* .format      = */ IE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::NTSTG,
   /* .name        = */ "NTSTG",
   /* .description = */ "Nontransactional Store",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x25,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PPA,
   /* .name        = */ "PPA",
   /* .description = */ "Perform Processor Assist",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xE8,
   /* .format      = */ RRF2_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::RISBGN,
   /* .name        = */ "RISBGN",
   /* .description = */ "Rotate Then Insert Selected Bits",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TABORT,
   /* .name        = */ "TABORT",
   /* .description = */ "Transaction Abort",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xFC,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TBEGIN,
   /* .name        = */ "TBEGIN",
   /* .description = */ "Transaction Begin",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x60,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TBEGINC,
   /* .name        = */ "TBEGINC",
   /* .description = */ "Constrained Transaction Begin",
   /* .opcode[0]   = */ 0xE5,
   /* .opcode[1]   = */ 0x61,
   /* .format      = */ SIL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::TEND,
   /* .name        = */ "TEND",
   /* .description = */ "Transaction End",
   /* .opcode[0]   = */ 0xB2,
   /* .opcode[1]   = */ 0xF8,
   /* .format      = */ S_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_zEC12,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CDPT,
   /* .name        = */ "CDPT",
   /* .description = */ "Convert Packed to DFP Long",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAE,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CPDT,
   /* .name        = */ "CPDT",
   /* .description = */ "Convert DFP Long to Packed",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAC,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_SetsSignFlag
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CPXT,
   /* .name        = */ "CPXT",
   /* .description = */ "Convert DFP Extended to Packed",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAD,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsStore |
                        S390OpProp_SetsOperand2 |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::CXPT,
   /* .name        = */ "CXPT",
   /* .description = */ "Convert Packed to DFP Extended",
   /* .opcode[0]   = */ 0xED,
   /* .opcode[1]   = */ 0xAF,
   /* .format      = */ RSL_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_DoubleFP |
                        S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesRegPairForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LCBB,
   /* .name        = */ "LCBB",
   /* .description = */ "Load Count To Block Boundary",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x27,
   /* .format      = */ RXE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLZRGF,
   /* .name        = */ "LLZRGF",
   /* .description = */ "Load Logical and Zero Rightmost Byte",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x3A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCFH,
   /* .name        = */ "LOCFH",
   /* .description = */ "(LongDisp) Load High On Condition",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE0,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCFHR,
   /* .name        = */ "LOCFHR",
   /* .description = */ "Load High On Condition",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xE0,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_ReadsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCGHI,
   /* .name        = */ "LOCGHI",
   /* .description = */ "Load Halfword Immediate On Condition (64)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCHHI,
   /* .name        = */ "LOCHHI",
   /* .description = */ "Load Halfword High Immediate On Condition",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x4E,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_TargetHW |
                        S390OpProp_ReadsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LOCHI,
   /* .name        = */ "LOCHI",
   /* .description = */ "Load Halfword Immediate On Condition (32)",
   /* .opcode[0]   = */ 0xEC,
   /* .opcode[1]   = */ 0x42,
   /* .format      = */ RIE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_ReadsCC |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LZRF,
   /* .name        = */ "LZRF",
   /* .description = */ "Load and Zero Rightmost Byte",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x3B,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LZRG,
   /* .name        = */ "LZRG",
   /* .description = */ "Load and Zero Rightmost Byte",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x2A,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::PRNO,
   /* .name        = */ "PRNO",
   /* .description = */ "perform random number operation",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ RRE_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_None
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STOCFH,
   /* .name        = */ "STOCFH",
   /* .description = */ "(LongDisp) Store High On Condition",
   /* .opcode[0]   = */ 0xEB,
   /* .opcode[1]   = */ 0xE1,
   /* .format      = */ RSY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_TargetHW |
                        S390OpProp_ReadsCC |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VA,
   /* .name        = */ "VA",
   /* .description = */ "vector add",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF3,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VAC,
   /* .name        = */ "VAC",
   /* .description = */ "vector add with carry",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xBB,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VACC,
   /* .name        = */ "VACC",
   /* .description = */ "vector add compute carry",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF1,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VACCC,
   /* .name        = */ "VACCC",
   /* .description = */ "vector add with carry compute carry",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xB9,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VAVG,
   /* .name        = */ "VAVG",
   /* .description = */ "vector average",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF2,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VAVGL,
   /* .name        = */ "VAVGL",
   /* .description = */ "vector average logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF0,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCDG,
   /* .name        = */ "VCDG",
   /* .description = */ "vector floating-point convert from fixed 64-bit",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC3,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCDLG,
   /* .name        = */ "VCDLG",
   /* .description = */ "vector floating-point convert from logical 64-bit",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC1,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCEQ,
   /* .name        = */ "VCEQ",
   /* .description = */ "vector comp are equal CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF8,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCGD,
   /* .name        = */ "VCGD",
   /* .description = */ "vector floating-point convert to fixed 64-bit",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC2,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCH,
   /* .name        = */ "VCH",
   /* .description = */ "vector comp are high CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xFB,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCHL,
   /* .name        = */ "VCHL",
   /* .description = */ "vector comp are high logical CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF9,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCKSM,
   /* .name        = */ "VCKSM",
   /* .description = */ "vector checksum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x66,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCLGD,
   /* .name        = */ "VCLGD",
   /* .description = */ "vector floating-point convert to logical 64-bit",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC0,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCLZ,
   /* .name        = */ "VCLZ",
   /* .description = */ "vector count leading zeros",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x53,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCTZ,
   /* .name        = */ "VCTZ",
   /* .description = */ "vector count trailing zeros",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x52,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VEC,
   /* .name        = */ "VEC",
   /* .description = */ "vector element comp are CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xDB,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VECL,
   /* .name        = */ "VECL",
   /* .description = */ "vector element comp are logical CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD9,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VERIM,
   /* .name        = */ "VERIM",
   /* .description = */ "vector element rotate and insert under mask",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x72,
   /* .format      = */ VRId_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VERLL,
   /* .name        = */ "VERLL",
   /* .description = */ "vector element rotate left logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x33,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VERLLV,
   /* .name        = */ "VERLLV",
   /* .description = */ "vector element rotate left logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESL,
   /* .name        = */ "VESL",
   /* .description = */ "vector element shift left",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x30,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESLV,
   /* .name        = */ "VESLV",
   /* .description = */ "vector element shift left",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x70,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESRA,
   /* .name        = */ "VESRA",
   /* .description = */ "vector element shift right arithmetic",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x3A,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESRAV,
   /* .name        = */ "VESRAV",
   /* .description = */ "vector element shift right arithmetic",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x7A,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESRL,
   /* .name        = */ "VESRL",
   /* .description = */ "vector element shift right logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x38,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VESRLV,
   /* .name        = */ "VESRLV",
   /* .description = */ "vector element shift right logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x78,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFA,
   /* .name        = */ "VFA",
   /* .description = */ "vector floating-point add",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xE3,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFAE,
   /* .name        = */ "VFAE",
   /* .description = */ "vector find any element equal CC Set* (*: If CS bit != 0)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x82,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorStringOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFCE,
   /* .name        = */ "VFCE",
   /* .description = */ "vector floating-point comp are equal CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xE8,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFCH,
   /* .name        = */ "VFCH",
   /* .description = */ "vector floating-point comp are high CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xEB,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFCHE,
   /* .name        = */ "VFCHE",
   /* .description = */ "vector floating-point comp are high or equal CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xEA,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFD,
   /* .name        = */ "VFD",
   /* .description = */ "vector floating-point divide",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xE5,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFEE,
   /* .name        = */ "VFEE",
   /* .description = */ "vector find element equal CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x80,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorStringOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFENE,
   /* .name        = */ "VFENE",
   /* .description = */ "vector find element not equal CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x81,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorStringOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFI,
   /* .name        = */ "VFI",
   /* .description = */ "vector load floating-point integer",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC7,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFM,
   /* .name        = */ "VFM",
   /* .description = */ "vector floating-point multiply",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xE7,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFMA,
   /* .name        = */ "VFMA",
   /* .description = */ "vector floating-point multiply and add",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x8F,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFMS,
   /* .name        = */ "VFMS",
   /* .description = */ "vector floating-point multiply and subtract",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x8E,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFPSO,
   /* .name        = */ "VFPSO",
   /* .description = */ "vector floating-point perform sign operation",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xCC,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFS,
   /* .name        = */ "VFS",
   /* .description = */ "vector floating-point subtract",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xE2,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFSQ,
   /* .name        = */ "VFSQ",
   /* .description = */ "vector floating-point square root",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xCE,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFTCI,
   /* .name        = */ "VFTCI",
   /* .description = */ "vector floating-point test data class immediate CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x4A,
   /* .format      = */ VRIe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGBM,
   /* .name        = */ "VGBM",
   /* .description = */ "vector generate byte mask",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x44,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGEF,
   /* .name        = */ "VGEF",
   /* .description = */ "vector gather element (32)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x13,
   /* .format      = */ VRV_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGEG,
   /* .name        = */ "VGEG",
   /* .description = */ "vector gather element (64)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x12,
   /* .format      = */ VRV_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGFM,
   /* .name        = */ "VGFM",
   /* .description = */ "vector galois field multiply sum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xB4,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGFMA,
   /* .name        = */ "VGFMA",
   /* .description = */ "vector galois field multiply sum and accumulate",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xBC,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VGM,
   /* .name        = */ "VGM",
   /* .description = */ "vector generate mask",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x46,
   /* .format      = */ VRIb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VISTR,
   /* .name        = */ "VISTR",
   /* .description = */ "vector isolate string CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x5C,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorStringOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VL,
   /* .name        = */ "VL",
   /* .description = */ "vector load",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x06,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLBB,
   /* .name        = */ "VLBB",
   /* .description = */ "vector load to block boundary",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x07,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLC,
   /* .name        = */ "VLC",
   /* .description = */ "vector load complement",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xDE,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLDE,
   /* .name        = */ "VLDE",
   /* .description = */ "vector floating-point load lengthened",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC4,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEB,
   /* .name        = */ "VLEB",
   /* .description = */ "vector load element (8)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x00,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLED,
   /* .name        = */ "VLED",
   /* .description = */ "vector floating-point load rounded",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC5,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEF,
   /* .name        = */ "VLEF",
   /* .description = */ "vector load element (32)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x03,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEG,
   /* .name        = */ "VLEG",
   /* .description = */ "vector load element (64)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x02,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEH,
   /* .name        = */ "VLEH",
   /* .description = */ "vector load element (16)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x01,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEIB,
   /* .name        = */ "VLEIB",
   /* .description = */ "vector load element immediate (8)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x40,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEIF,
   /* .name        = */ "VLEIF",
   /* .description = */ "vector load element immediate (32)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x43,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEIG,
   /* .name        = */ "VLEIG",
   /* .description = */ "vector load element immediate (64)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x42,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLEIH,
   /* .name        = */ "VLEIH",
   /* .description = */ "vector load element immediate (16)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x41,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLGV,
   /* .name        = */ "VLGV",
   /* .description = */ "vector load gr from vr element",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x21,
   /* .format      = */ VRSc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_Is64Bit |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLL,
   /* .name        = */ "VLL",
   /* .description = */ "vector load with length",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x37,
   /* .format      = */ VRSb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLLEZ,
   /* .name        = */ "VLLEZ",
   /* .description = */ "vector load logical element and zero",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x04,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLM,
   /* .name        = */ "VLM",
   /* .description = */ "vector load multiple",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x36,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesRegRangeForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOperand2
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLP,
   /* .name        = */ "VLP",
   /* .description = */ "vector load positive",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xDF,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLR,
   /* .name        = */ "VLR",
   /* .description = */ "vector load",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x56,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsRegCopy |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLREP,
   /* .name        = */ "VLREP",
   /* .description = */ "vector load and replicate",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x05,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLVG,
   /* .name        = */ "VLVG",
   /* .description = */ "vector load vr element from gr",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x22,
   /* .format      = */ VRSb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_UsesTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_Is64Bit |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLVGP,
   /* .name        = */ "VLVGP",
   /* .description = */ "vector load vr from grs disjoint",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x62,
   /* .format      = */ VRRf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_Is64Bit
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMAE,
   /* .name        = */ "VMAE",
   /* .description = */ "vector multiply and add even",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAE,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMAH,
   /* .name        = */ "VMAH",
   /* .description = */ "vector multiply and add high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAB,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMAL,
   /* .name        = */ "VMAL",
   /* .description = */ "vector multiply and add low",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAA,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMALE,
   /* .name        = */ "VMALE",
   /* .description = */ "vector multiply and add logical even",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAC,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMALH,
   /* .name        = */ "VMALH",
   /* .description = */ "vector multiply and add logical high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA9,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMALO,
   /* .name        = */ "VMALO",
   /* .description = */ "vector multiply and add logical odd",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAD,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMAO,
   /* .name        = */ "VMAO",
   /* .description = */ "vector multiply and add odd",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xAF,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VME,
   /* .name        = */ "VME",
   /* .description = */ "vector multiply even",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA6,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMH,
   /* .name        = */ "VMH",
   /* .description = */ "vector multiply high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA3,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VML,
   /* .name        = */ "VML",
   /* .description = */ "vector multiply low",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA2,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMLE,
   /* .name        = */ "VMLE",
   /* .description = */ "vector multiply logical even",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA4,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMLH,
   /* .name        = */ "VMLH",
   /* .description = */ "vector multiply logical high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA1,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMLO,
   /* .name        = */ "VMLO",
   /* .description = */ "vector multiply logical odd",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA5,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMN,
   /* .name        = */ "VMN",
   /* .description = */ "vector minimum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xFE,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMNL,
   /* .name        = */ "VMNL",
   /* .description = */ "vector minimum logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xFC,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMO,
   /* .name        = */ "VMO",
   /* .description = */ "vector multiply odd",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xA7,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMRH,
   /* .name        = */ "VMRH",
   /* .description = */ "vector merge high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x61,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMRL,
   /* .name        = */ "VMRL",
   /* .description = */ "vector merge low",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x60,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMX,
   /* .name        = */ "VMX",
   /* .description = */ "vector maximum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xFF,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMXL,
   /* .name        = */ "VMXL",
   /* .description = */ "vector maximum logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xFD,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VN,
   /* .name        = */ "VN",
   /* .description = */ "vector and",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x68,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VNC,
   /* .name        = */ "VNC",
   /* .description = */ "vector and with complement",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x69,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VNO,
   /* .name        = */ "VNO",
   /* .description = */ "vector nor",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6B,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VO,
   /* .name        = */ "VO",
   /* .description = */ "vector or",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6A,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPDI,
   /* .name        = */ "VPDI",
   /* .description = */ "vector permute doubleword immediate",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x84,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPERM,
   /* .name        = */ "VPERM",
   /* .description = */ "vector permute",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x8C,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPK,
   /* .name        = */ "VPK",
   /* .description = */ "vector pack",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x94,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPKLS,
   /* .name        = */ "VPKLS",
   /* .description = */ "vector pack logical saturate CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x95,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPKS,
   /* .name        = */ "VPKS",
   /* .description = */ "vector pack saturate CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x97,
   /* .format      = */ VRRb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPOPCT,
   /* .name        = */ "VPOPCT",
   /* .description = */ "vector population count",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x50,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VREP,
   /* .name        = */ "VREP",
   /* .description = */ "vector replicate",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x4D,
   /* .format      = */ VRIc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VREPI,
   /* .name        = */ "VREPI",
   /* .description = */ "vector replicate immediate",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x45,
   /* .format      = */ VRIa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VS,
   /* .name        = */ "VS",
   /* .description = */ "vector subtract",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF7,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSBCBI,
   /* .name        = */ "VSBCBI",
   /* .description = */ "vector subtract with borrow compute borrow indication I",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xBD,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSBI,
   /* .name        = */ "VSBI",
   /* .description = */ "vector subtract with borrow indication I",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xBF,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSCBI,
   /* .name        = */ "VSCBI",
   /* .description = */ "vector subtract compute borrow indication I",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xF5,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSCEF,
   /* .name        = */ "VSCEF",
   /* .description = */ "vector scatter element (32)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x1B,
   /* .format      = */ VRV_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSCEG,
   /* .name        = */ "VSCEG",
   /* .description = */ "vector scatter element (64)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x1A,
   /* .format      = */ VRV_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSEG,
   /* .name        = */ "VSEG",
   /* .description = */ "vector sign extend to doubleword",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x5F,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSEL,
   /* .name        = */ "VSEL",
   /* .description = */ "vector select",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x8D,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSL,
   /* .name        = */ "VSL",
   /* .description = */ "vector shift left",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x74,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSLB,
   /* .name        = */ "VSLB",
   /* .description = */ "vector shift left by byte",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x75,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSLDB,
   /* .name        = */ "VSLDB",
   /* .description = */ "vector shift left double by byte",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x77,
   /* .format      = */ VRId_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSRA,
   /* .name        = */ "VSRA",
   /* .description = */ "vector shift right arithmetic",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x7E,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSRAB,
   /* .name        = */ "VSRAB",
   /* .description = */ "vector shift right arithmetic by byte",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x7F,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSRL,
   /* .name        = */ "VSRL",
   /* .description = */ "vector shift right logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x7C,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSRLB,
   /* .name        = */ "VSRLB",
   /* .description = */ "vector shift right logical by byte",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x7D,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VST,
   /* .name        = */ "VST",
   /* .description = */ "vector store",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x0E,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTEB,
   /* .name        = */ "VSTEB",
   /* .description = */ "vector store element (8)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x08,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTEF,
   /* .name        = */ "VSTEF",
   /* .description = */ "vector store element (32)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x0B,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTEG,
   /* .name        = */ "VSTEG",
   /* .description = */ "vector store element (64)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x0A,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTEH,
   /* .name        = */ "VSTEH",
   /* .description = */ "vector store element (16)",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x09,
   /* .format      = */ VRX_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTL,
   /* .name        = */ "VSTL",
   /* .description = */ "vector store with length",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ VRSb_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTM,
   /* .name        = */ "VSTM",
   /* .description = */ "vector store multiple",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x3E,
   /* .format      = */ VRSa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_UsesRegRangeForTarget
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTRC,
   /* .name        = */ "VSTRC",
   /* .description = */ "vector string range compare CC Set*",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x8A,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_VectorStringOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSUM,
   /* .name        = */ "VSUM",
   /* .description = */ "vector sum across word",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x64,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSUMG,
   /* .name        = */ "VSUMG",
   /* .description = */ "vector sum across doubleword",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x65,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSUMQ,
   /* .name        = */ "VSUMQ",
   /* .description = */ "vector sum across quadword",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x67,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VTM,
   /* .name        = */ "VTM",
   /* .description = */ "vector test under mask CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD8,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VUPH,
   /* .name        = */ "VUPH",
   /* .description = */ "vector unpack high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD7,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VUPL,
   /* .name        = */ "VUPL",
   /* .description = */ "vector unpack low",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD6,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VUPLH,
   /* .name        = */ "VUPLH",
   /* .description = */ "vector unpack logical high",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD5,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VUPLL,
   /* .name        = */ "VUPLL",
   /* .description = */ "vector unpack logical low",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xD4,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VX,
   /* .name        = */ "VX",
   /* .description = */ "vector exclusive or",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6D,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::WFC,
   /* .name        = */ "WFC",
   /* .description = */ "vector floating-point comp are scalar CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xCB,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::WFK,
   /* .name        = */ "WFK",
   /* .description = */ "vector floating-point comp are and signal scalar CC Set",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xCA,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z13,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_SetsCC |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::AGH,
   /* .name        = */ "AGH",
   /* .description = */ "add halfword (64 <- 16)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x38,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsSignFlag |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::BIC,
   /* .name        = */ "BIC",
   /* .description = */ "branch indirect on condition",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x47,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_BranchOp |
                        S390OpProp_ReadsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::KMA,
   /* .name        = */ "KMA",
   /* .description = */ "Cipher Message with Authentication",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0x29,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGG,
   /* .name        = */ "LGG",
   /* .description = */ "load guarded",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x4C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LGSC,
   /* .name        = */ "LGSC",
   /* .description = */ "load guarded storage controls",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x4D,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::LLGFSG,
   /* .name        = */ "LLGFSG",
   /* .description = */ "load logical and shift guarded (64 <- 32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x48,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_Is64Bit |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MG,
   /* .name        = */ "MG",
   /* .description = */ "multiply (128 <- 64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x84,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MGH,
   /* .name        = */ "MGH",
   /* .description = */ "multiply halfword (64 <- 16)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MGRK,
   /* .name        = */ "MGRK",
   /* .description = */ "multiply (128 <- 64)",
   /* .opcode[0]   = */ 0xE9,
   /* .opcode[1]   = */ 0xEC,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_UsesRegPairForTarget |
                        S390OpProp_SetsOperand3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSC,
   /* .name        = */ "MSC",
   /* .description = */ "multiply single (32)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x53,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGC,
   /* .name        = */ "MSGC",
   /* .description = */ "multiply single (64)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x83,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_UsesTarget |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSGRKC,
   /* .name        = */ "MSGRKC",
   /* .description = */ "multiply single (64)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xED,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::MSRKC,
   /* .name        = */ "MSRKC",
   /* .description = */ "multiply single (32)",
   /* .opcode[0]   = */ 0xB9,
   /* .opcode[1]   = */ 0xFD,
   /* .format      = */ RRF_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is32Bit |
                        S390OpProp_SetsOperand3 |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::SGH,
   /* .name        = */ "SGH",
   /* .description = */ "subtract halfword (64 <- 16)",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x39,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_Is64Bit |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_IsLoad |
                        S390OpProp_LongDispSupported |
                        S390OpProp_SetsOverflowFlag |
                        S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::STGSC,
   /* .name        = */ "STGSC",
   /* .description = */ "store guarded storage controls",
   /* .opcode[0]   = */ 0xE3,
   /* .opcode[1]   = */ 0x49,
   /* .format      = */ RXY_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsStore |
                        S390OpProp_LongDispSupported
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VAP,
   /* .name        = */ "VAP",
   /* .description = */ "vector Add Decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x71,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VBPERM,
   /* .name        = */ "VBPERM",
   /* .description = */ "vector bit permute",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x85,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCP,
   /* .name        = */ "VCP",
   /* .description = */ "vector Compare decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x77,
   /* .format      = */ VRRh_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCVB,
   /* .name        = */ "VCVB",
   /* .description = */ "vector convert to binary",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x50,
   /* .format      = */ VRRi_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCVBG,
   /* .name        = */ "VCVBG",
   /* .description = */ "vector convert to binary",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x52,
   /* .format      = */ VRRi_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCVD,
   /* .name        = */ "VCVD",
   /* .description = */ "vector convert to decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x58,
   /* .format      = */ VRIi_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VCVDG,
   /* .name        = */ "VCVDG",
   /* .description = */ "vector convert to decmial",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x5A,
   /* .format      = */ VRIi_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM4
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VDP,
   /* .name        = */ "VDP",
   /* .description = */ "vector divide decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x7A,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFLL,
   /* .name        = */ "VFLL",
   /* .description = */ "vector FP load lengthened",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC4,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFLR,
   /* .name        = */ "VFLR",
   /* .description = */ "vector FP load rounded",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xC5,
   /* .format      = */ VRRa_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM3 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFMAX,
   /* .name        = */ "VFMAX",
   /* .description = */ "vector FP maximum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xEF,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFMIN,
   /* .name        = */ "VFMIN",
   /* .description = */ "vector FP minimum",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xEE,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM4 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFNMA,
   /* .name        = */ "VFNMA",
   /* .description = */ "vector FP negative multiply and add",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x9F,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VFNMS,
   /* .name        = */ "VFNMS",
   /* .description = */ "vector FP negative multiply and subtract",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x9E,
   /* .format      = */ VRRe_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5 |
                        S390OpProp_UsesM6 |
                        S390OpProp_VectorFPOp |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLIP,
   /* .name        = */ "VLIP",
   /* .description = */ "vector load immediate decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x49,
   /* .format      = */ VRIh_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLRL,
   /* .name        = */ "VLRL",
   /* .description = */ "vector Load Rightmost with Length(Length is immediate value)",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x35,
   /* .format      = */ VSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VLRLR,
   /* .name        = */ "VLRLR",
   /* .description = */ "vector Load Rightmost with Length(Length is in register)",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x37,
   /* .format      = */ VRSd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsLoad |
                        S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMP,
   /* .name        = */ "VMP",
   /* .description = */ "vector multiply decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x78,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMSL,
   /* .name        = */ "VMSL",
   /* .description = */ "vector multiply sum logical",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0xB8,
   /* .format      = */ VRRd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5 |
                        S390OpProp_HasExtendedMnemonic
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VMSP,
   /* .name        = */ "VMSP",
   /* .description = */ "vector multiply and shift decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x79,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VNN,
   /* .name        = */ "VNN",
   /* .description = */ "vector NAND",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6E,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VNX,
   /* .name        = */ "VNX",
   /* .description = */ "vector not exclusive OR",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6C,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VOC,
   /* .name        = */ "VOC",
   /* .description = */ "vector OR with complement",
   /* .opcode[0]   = */ 0xE7,
   /* .opcode[1]   = */ 0x6F,
   /* .format      = */ VRRc_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPKZ,
   /* .name        = */ "VPKZ",
   /* .description = */ "vector pack zoned",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x34,
   /* .format      = */ VSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsOperand1
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VPSOP,
   /* .name        = */ "VPSOP",
   /* .description = */ "vector perform sign operation decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x5B,
   /* .format      = */ VRIg_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VRP,
   /* .name        = */ "VRP",
   /* .description = */ "vector remainder decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x7B,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSDP,
   /* .name        = */ "VSDP",
   /* .description = */ "vector shift and divide decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x7E,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSP,
   /* .name        = */ "VSP",
   /* .description = */ "vector subtract decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x73,
   /* .format      = */ VRIf_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSRP,
   /* .name        = */ "VSRP",
   /* .description = */ "vector shift and round decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x59,
   /* .format      = */ VRIg_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC |
                        S390OpProp_SetsOperand1 |
                        S390OpProp_UsesM5
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTRL,
   /* .name        = */ "VSTRL",
   /* .description = */ "vector Store Rightmost with Length(Length is immediate value)",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x3D,
   /* .format      = */ VSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VSTRLR,
   /* .name        = */ "VSTRLR",
   /* .description = */ "vector Store Rightmost with Length(Length is in register)",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x3F,
   /* .format      = */ VRSd_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsStore
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VTP,
   /* .name        = */ "VTP",
   /* .description = */ "vector test decimal",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x5F,
   /* .format      = */ VRRg_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_SetsCC
   },

   {
   /* .mnemonic    = */ OMR::InstOpCode::VUPKZ,
   /* .name        = */ "VUPKZ",
   /* .description = */ "vector unpack zoned",
   /* .opcode[0]   = */ 0xE6,
   /* .opcode[1]   = */ 0x3C,
   /* .format      = */ VSI_FORMAT,
   /* .minimumALS  = */ TR_S390ProcessorInfo::TR_z14,
   /* .properties  = */ S390OpProp_IsStore
   },

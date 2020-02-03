/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#ifndef TR_PPCOPSDEFINES_INCL
#define TR_PPCOPSDEFINES_INCL

enum PPCInstructionFormat : uint8_t
{

// Placeholder format for instructions that should not appear in the instruction stream during the
// binary encoding phase.
FORMAT_UNKNOWN,

// Format for pseudoinstructions that don't emit any actual instructions.
FORMAT_NONE,

// Format for instructions whose binary encoding is copied directly into the instruction stream
// without filling in any fields.
FORMAT_DIRECT,

// Format for the dd instruction, which is simply a single word placed into the instruction stream.
FORMAT_DD,

// IMPORTANT: The diagrams for instruction fields below only include fields which are filled in by
// the binary encoder. Some fields are filled in by the instruction opcode itself, as defined in
// OMRInstOpCodeProperties.hpp, but they are omitted as they are generally not important to the
// format of an opcode.

// Note also that the formats of instructions with condition register fields here may not exactly
// match what appears in the Power ISA, as the bottom two bits generally indicate which field in a
// CCR should be used. These bits are often part of the instruction itself, since we generally use
// the extended mnemonics.

// Format for I-Form instructions (see Power ISA):
//
// +------+-------------------------------------------------+-----+
// |      | LI                                              |     |
// | 0    | 6                                               | 30  |
// +------+-------------------------------------------------+-----+
FORMAT_I_FORM,

// Format for B-Form instructions (see Power ISA):
//
// +------+-----+-----+-----+-------------------------------+-----+
// |      | BO  | BI  |     | BD                            |     |
// | 0    | 6   | 11  | 14  | 16                            | 30  |
// +------+-----+-----+-----+-------------------------------+-----+
FORMAT_B_FORM,

// Format for XL-Form conditional branch instructions, with the BI field controlled by the condition
// register and the BO field controlled by the branch hint:
//
// +------+-----+-----+-------------------------------------------+
// |      | BO  | BI  |                                           |
// | 0    | 6   | 11  | 14                                        |
// +------+-----+-----+-------------------------------------------+
FORMAT_XL_FORM_BRANCH,

// Format for the mtfsfi X-form instruction, with the U field controlled by imm1 and the BF and W
// fields controlled by imm2:
//
// +------+-----+-----+----+-----+--------------------------------+
// |      | BF  |     | W  | U   |                                |
// | 0    | 6   | 9   | 15 | 16  | 20                             |
// +------+-----+-----+----+-----+--------------------------------+
FORMAT_MTFSFI,

// Format for the mtfsf XFL-form instruction, with the FRB field encoding the source register and
// the FLM field controlled by the unsigned immediate:
//
// +------+---------+-----+-------+-------------------------------+
// |      | FLM     |     | FRB   |                               |
// | 0    | 7       | 15  | 16    | 21                            |
// +------+---------+-----+-------+-------------------------------+
FORMAT_MTFSF,

// Format for instructions with an RS field encoding the source register:
//
// +------+-------+-----------------------------------------------+
// |      | RS    |                                               |
// | 0    | 6     | 11                                            |
// +------+-------+-----------------------------------------------+
FORMAT_RS,

// Format for instructions with an RA field encoding the source register and an SI field encoding
// the signed immediate:
//
// +-----------------+----------+---------------------------------+
// |                 | RA       | SI                              |
// | 0               | 11       | 16                              |
// +-----------------+----------+---------------------------------+
FORMAT_RA_SI,

// Format for instructions with an RA field encoding the source register and a 5-bit SI field
// encoding the signed immediate:
//
// +-----------------+----------+----------+----------------------+
// |                 | RA       | SI       |                      |
// | 0               | 11       | 16       | 21                   |
// +-----------------+----------+----------+----------------------+
FORMAT_RA_SI5,

// Format for instructions with an RS field encoding the source register and a FXM field encoding an
// unsigned immediate mask:
//
// +------+----------+----+----------+----------------------------+
// |      | RT       |    | FXM      |                            |
// | 0    | 6        | 11 | 12       | 20                         |
// +------+----------+----+----------+----------------------------+
//
// Note: The "1" variant of this format asserts that only one bit in FXM should be set.
FORMAT_RS_FXM,
FORMAT_RS_FXM1,

// Format for instructions with an RT field encoding the target register:
//
// +------+-------+-----------------------------------------------+
// |      | RT    |                                               |
// | 0    | 6     | 11                                            |
// +------+-------+-----------------------------------------------+
FORMAT_RT,

// Format for instructions with an RA field encoding the target register and an RS field encoding
// the only source register:
//
// +------+----------+----------+---------------------------------+
// |      | RS       | RA       |                                 |
// | 0    | 6        | 11       | 16                              |
// +------+----------+----------+---------------------------------+
FORMAT_RA_RS,

// Format for instructions with an RT field encoding the target register and an RA field encoding
// the only source register:
//
// +------+----------+----------+---------------------------------+
// |      | RT       | RA       |                                 |
// | 0    | 6        | 11       | 16                              |
// +------+----------+----------+---------------------------------+
FORMAT_RT_RA,

// Format for instructions with an FRT field encoding the target FP register and an FRB field
// encoding the only source FP register:
//
// +------+----------+----------+----------+----------------------+
// |      | FRT      |          | FRB      |                      |
// | 0    | 6        | 11       | 16       | 21                   |
// +------+----------+----------+----------+----------------------+
FORMAT_FRT_FRB,

// Format for instructions with a BF field encoding the target CC register and a BFA field encoding
// the only source CC register:
//
// +------+-----+-----+-----+-------------------------------------+
// |      | BF  |     | BFA |                                     |
// | 0    | 6   | 9   | 11  | 14                                  |
// +------+-----+-----+-----+-------------------------------------+
FORMAT_BF_BFA,

// Format for instructions with an RA field encoding the target register and an XS field encoding
// the only VSX source register:
//
// +------+----------+----------+----------------------------+----+
// |      | XS       | RA       |                            | XS |
// | 0    | 6        | 11       | 16                         | 31 |
// +------+----------+----------+----------------------------+----+
FORMAT_RA_XS,

// Format for instructions with an XT field encoding the target VSX target register and an RA field
// encoding the only source register:
//
// +------+----------+----------+----------------------------+----+
// |      | XT       | RA       |                            | XT |
// | 0    | 6        | 11       | 16                         | 31 |
// +------+----------+----------+----------------------------+----+
FORMAT_XT_RA,

// Format for instructions with an RT field encoding the target register and an BFA field encoding
// the only source CC register:
//
// +------+----------+-----+--------------------------------------+
// |      | RT       | BFA |                                      |
// | 0    | 6        | 11  | 14                                   |
// +------+----------+-----+--------------------------------------+
FORMAT_RT_BFA,

// Format for instructions with an VRT field encoding the target vector register and a VRB field
// encoding the only source vector register:
//
// +------+----------+----------+----------+----------------------+
// |      | VRT      |          | VRB      |                      |
// | 0    | 6        | 11       | 16       | 21                   |
// +------+----------+----------+----------+----------------------+
FORMAT_VRT_VRB,

// Format for instructions with an XT field encoding the target VSX register and an XB field
// encoding the only source VSX register:
//
// +------+----------+----------+----------+------------+----+----+
// |      | XT       |          | XB       |            | XB | XT |
// | 0    | 6        | 11       | 16       | 21         | 30 | 31 |
// +------+----------+----------+----------+------------+----+----+
FORMAT_XT_XB

};

#define PPCOpProp_None              0x00000000
#define PPCOpProp_HasRecordForm     0x00000001
#define PPCOpProp_SetsCarryFlag     0x00000002
#define PPCOpProp_SetsOverflowFlag  0x00000004
#define PPCOpProp_ReadsCarryFlag    0x00000008
#define PPCOpProp_TMAbort           0x00000010
#define PPCOpProp_BranchOp          0x00000040
#define PPCOpProp_CRLogical         0x00000080
#define PPCOpProp_DoubleFP          0x00000100
#define PPCOpProp_SingleFP          0x00000200
#define PPCOpProp_UpdateForm        0x00000400
#define PPCOpProp_AltFormat         0x00000800  // use alternate instruction format
#define PPCOpProp_AltFormatx        0x00001000  // use alternate instruction format
#define PPCOpProp_IsRecordForm      0x00002000
#define PPCOpProp_IsLoad            0x00004000
#define PPCOpProp_IsStore           0x00008000
#define PPCOpProp_IsRegCopy         0x00010000
#define PPCOpProp_Trap              0x00020000
#define PPCOpProp_SetsCtr           0x00040000
#define PPCOpProp_UsesCtr           0x00080000
#define PPCOpProp_DWord             0x00100000
#define PPCOpProp_UseMaskEnd        0x00200000  // ME or MB should be encoded
#define PPCOpProp_IsSync            0x00400000
#define PPCOpProp_IsRotateOrShift   0x00800000
#define PPCOpProp_CompareOp         0x01000000
#define PPCOpProp_SetsFPSCR         0x02000000
#define PPCOpProp_ReadsFPSCR        0x04000000
#define PPCOpProp_OffsetRequiresWordAlignment 0x08000000

#define PPCOpProp_BranchLikelyMask            0x00600000
#define PPCOpProp_BranchUnlikelyMask          0x00400000
#define PPCOpProp_BranchLikelyMaskCtr         0x01200000
#define PPCOpProp_BranchUnlikelyMaskCtr       0x01000000
#define PPCOpProp_IsBranchCTR                 0x04000000
#define PPCOpProp_BranchLikely                1
#define PPCOpProp_BranchUnlikely              0

#define PPCOpProp_LoadReserveAtomicUpdate     0x00000000
#define PPCOpProp_LoadReserveExclusiveAccess  0x00000001
#define PPCOpProp_NoHint                      0xFFFFFFFF

#define PPCOpProp_IsVMX                       0x08000000
#define PPCOpProp_SyncSideEffectFree          0x10000000 // syncs redundant if seperated by these types of ops
#define PPCOpProp_IsVSX                       0x20000000
#define PPCOpProp_UsesTarget                  0x40000000

#if defined(TR_TARGET_64BIT)
#define Op_st        std
#define Op_stu       stdu
#define Op_stx       stdx
#define Op_stux      stdux
#define Op_load      ld
#define Op_loadu     ldu
#define Op_loadx     ldx
#define Op_loadux    ldux
#define Op_cmp       cmp8
#define Op_cmpi      cmpi8
#define Op_cmpl      cmpl8
#define Op_cmpli     cmpli8
#define Op_larx      ldarx
#define Op_stcx_r    stdcx_r
#else
#define Op_st        stw
#define Op_stu       stwu
#define Op_stx       stwx
#define Op_stux      stwux
#define Op_load      lwz
#define Op_loadu     lwzu
#define Op_loadx     lwzx
#define Op_loadux    lwzux
#define Op_cmp       cmp4
#define Op_cmpi      cmpi4
#define Op_cmpl      cmpl4
#define Op_cmpli     cmpli4
#define Op_larx      lwarx
#define Op_stcx_r    stwcx_r
#endif

#endif //ifndef TR_PPCOPSDEFINES_INCL

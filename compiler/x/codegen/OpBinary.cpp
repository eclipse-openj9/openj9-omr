/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2017
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

#include "x/codegen/X86Ops.hpp"  // for TR_X86OpCode, ::IA32NumOpCodes, etc

// Heuristics for X87 second byte opcode
// It could be eliminated if GCC/MSVC fully support initializer list
#define X87_________________(x) (uint8_t)((x & 0xE0) >> 5), (uint8_t)((x & 0x18) >> 3), (uint8_t)(x & 0x07)

// see compiler/x/codegen/OMRInstruction.hpp for structural information.
const TR_X86OpCode::OpCode_t TR_X86OpCode::_binaries[] =
{
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xcc, 0, ModRM_NONE, Immediate_0 },    // BADIA32Op/int 3
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x14, 0, ModRM_NONE, Immediate_1 },    // ADC1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x15, 0, ModRM_NONE, Immediate_2 },    // ADC2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x15, 0, ModRM_NONE, Immediate_4 },    // ADC4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x15, 0, ModRM_NONE, Immediate_4 },    // ADC8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 2, ModRM_EXT_, Immediate_1 },    // ADC1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_2 },    // ADC2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_4 },    // ADC4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_4 },    // ADC8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 2, ModRM_EXT_, Immediate_1 },    // ADC1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_2 },    // ADC2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_4 },    // ADC4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 2, ModRM_EXT_, Immediate_4 },    // ADC8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 2, ModRM_EXT_, Immediate_S },    // ADC8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x12, 0, ModRM_RM__, Immediate_0 },    // ADC1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x12, 0, ModRM_RM__, Immediate_0 },    // ADC1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x13, 0, ModRM_RM__, Immediate_0 },    // ADC8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x10, 0, ModRM_MR__, Immediate_0 },    // ADC1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x11, 0, ModRM_MR__, Immediate_0 },    // ADC2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x11, 0, ModRM_MR__, Immediate_0 },    // ADC4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x11, 0, ModRM_MR__, Immediate_0 },    // ADC8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x04, 0, ModRM_NONE, Immediate_1 },    // ADD1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x05, 0, ModRM_NONE, Immediate_2 },    // ADD2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x05, 0, ModRM_NONE, Immediate_4 },    // ADD4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x05, 0, ModRM_NONE, Immediate_4 },    // ADD8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 0, ModRM_EXT_, Immediate_1 },    // ADD1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_2 },    // ADD2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_4 },    // ADD4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_4 },    // ADD8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 0, ModRM_EXT_, Immediate_1 },    // ADD1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_2 },    // ADD2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_4 },    // ADD4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 0, ModRM_EXT_, Immediate_4 },    // ADD8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 0, ModRM_EXT_, Immediate_S },    // ADD8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x02, 0, ModRM_RM__, Immediate_0 },    // ADD1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x02, 0, ModRM_RM__, Immediate_0 },    // ADD1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x03, 0, ModRM_RM__, Immediate_0 },    // ADD8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_MR__, Immediate_0 },    // ADD1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // ADD2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // ADD4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // ADD8MemReg (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDSSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDSSRegMem
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDPSRegReg
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDPSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDSDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDSDRegMem
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDPDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x58, 0, ModRM_RM__, Immediate_0 },    // ADDPDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_MR__, Immediate_0 },    // LADD1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // LADD2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // LADD4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x01, 0, ModRM_MR__, Immediate_0 },    // LADD8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xc0, 0, ModRM_MR__, Immediate_0 },    // LXADD1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xc1, 0, ModRM_MR__, Immediate_0 },    // LXADD2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xc1, 0, ModRM_MR__, Immediate_0 },    // LXADD4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xc1, 0, ModRM_MR__, Immediate_0 },    // LXADD8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x24, 0, ModRM_NONE, Immediate_1 },    // AND1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x25, 0, ModRM_NONE, Immediate_2 },    // AND2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x25, 0, ModRM_NONE, Immediate_4 },    // AND4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x25, 0, ModRM_NONE, Immediate_4 },    // AND8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 4, ModRM_EXT_, Immediate_1 },    // AND1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_2 },    // AND2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_4 },    // AND4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_4 },    // AND8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 4, ModRM_EXT_, Immediate_1 },    // AND1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_2 },    // AND2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_4 },    // AND4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 4, ModRM_EXT_, Immediate_4 },    // AND8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 4, ModRM_EXT_, Immediate_S },    // AND8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x22, 0, ModRM_RM__, Immediate_0 },    // AND1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x22, 0, ModRM_RM__, Immediate_0 },    // AND1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x23, 0, ModRM_RM__, Immediate_0 },    // AND8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x20, 0, ModRM_MR__, Immediate_0 },    // AND1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x21, 0, ModRM_MR__, Immediate_0 },    // AND2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x21, 0, ModRM_MR__, Immediate_0 },    // AND4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x21, 0, ModRM_MR__, Immediate_0 },    // AND8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbc, 0, ModRM_RM__, Immediate_0 },    // BSF2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbc, 0, ModRM_RM__, Immediate_0 },    // BSF4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbc, 0, ModRM_RM__, Immediate_0 },    // BSF8RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbd, 0, ModRM_RM__, Immediate_0 },    // BSR4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbd, 0, ModRM_RM__, Immediate_0 },    // BSR8RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xc8, 0, ModRM_NONE, Immediate_0 },    // BSWAP4Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xc8, 0, ModRM_NONE, Immediate_0 },    // BSWAP8Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xab, 0, ModRM_MR__, Immediate_0 },    // BTS4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xab, 0, ModRM_MR__, Immediate_0 },    // BTS4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xe8, 0, ModRM_NONE, Immediate_4 },    // CALLImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xcc, 0, ModRM_NONE, Immediate_0 },    // CALLREXImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 2, ModRM_MR__, Immediate_0 },    // CALLReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xcc, 0, ModRM_NONE, Immediate_0 },    // CALLREXReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 2, ModRM_MR__, Immediate_0 },    // CALLMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xcc, 0, ModRM_NONE, Immediate_0 },    // CALLREXMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x98, 0, ModRM_NONE, Immediate_0 },    // CBWAcc
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x98, 0, ModRM_NONE, Immediate_0 },    // CBWEAcc
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x47, 0, ModRM_RM__, Immediate_0 },    // CMOVA4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x42, 0, ModRM_RM__, Immediate_0 },    // CMOVB4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x44, 0, ModRM_RM__, Immediate_0 },    // CMOVE4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x44, 0, ModRM_RM__, Immediate_0 },    // CMOVE8RegMem (64-bit)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x4F, 0, ModRM_RM__, Immediate_0 },    // CMOVG4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x4C, 0, ModRM_RM__, Immediate_0 },    // CMOVL4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x45, 0, ModRM_RM__, Immediate_0 },    // CMOVNE4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x45, 0, ModRM_RM__, Immediate_0 },    // CMOVNE8RegMem (64-bit)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x41, 0, ModRM_RM__, Immediate_0 },    // CMOVNO4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x49, 0, ModRM_RM__, Immediate_0 },    // CMOVNS4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x40, 0, ModRM_RM__, Immediate_0 },    // CMOVO4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x48, 0, ModRM_RM__, Immediate_0 },    // CMOVS4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3c, 0, ModRM_NONE, Immediate_1 },    // CMP1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x3d, 0, ModRM_NONE, Immediate_2 },    // CMP2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3d, 0, ModRM_NONE, Immediate_4 },    // CMP4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x3d, 0, ModRM_NONE, Immediate_4 },    // CMP8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 7, ModRM_EXT_, Immediate_1 },    // CMP1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_2 },    // CMP2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_4 },    // CMP4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_4 },    // CMP8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 7, ModRM_EXT_, Immediate_1 },    // CMP1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_2 },    // CMP2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_4 },    // CMP4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 7, ModRM_EXT_, Immediate_4 },    // CMP8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 7, ModRM_EXT_, Immediate_S },    // CMP8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3a, 0, ModRM_RM__, Immediate_0 },    // CMP1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3a, 0, ModRM_RM__, Immediate_0 },    // CMP1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x3b, 0, ModRM_RM__, Immediate_0 },    // CMP8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x38, 0, ModRM_MR__, Immediate_0 },    // CMP1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x39, 0, ModRM_MR__, Immediate_0 },    // CMP2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x39, 0, ModRM_MR__, Immediate_0 },    // CMP4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x39, 0, ModRM_MR__, Immediate_0 },    // CMP8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb0, 0, ModRM_MR__, Immediate_0 },    // CMPXCHG1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // CMPXCHG2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // CMPXCHG4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // CMPXCHG8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xc7, 1, ModRM_EXT_, Immediate_0 },    // CMPXCHG8BMem (IA32 only)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xc7, 1, ModRM_EXT_, Immediate_0 },    // CMPXCHG16BMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb0, 0, ModRM_MR__, Immediate_0 },    // LCMPXCHG1MemReg NOTE: The lock prefix is added at binary encoding time
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // LCMPXCHG2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // LCMPXCHG4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // LCMPXCHG8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xc7, 1, ModRM_EXT_, Immediate_0 },    // LCMPXCHG8BMem (IA32 only)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xc7, 1, ModRM_EXT_, Immediate_0 },    // LCMPXCHG16BMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F2, REX_W, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // XALCMPXCHG8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F2, REX_W, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // XACMPXCHG8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // XALCMPXCHG4MemReg
    { VEX_L___, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0xb1, 0, ModRM_MR__, Immediate_0 },    // XACMPXCHG4MemReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SSRegReg4
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX_W, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SSRegReg8 (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX_W, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SSRegMem8 (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SDRegReg4
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX_W, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SDRegReg8 (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SDRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX_W, ESCAPE_0F__, 0x2a, 0, ModRM_RM__, Immediate_0 },    // CVTSI2SDRegMem8 (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SIReg4Reg
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX_W, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SIReg8Reg (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SIReg4Mem
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX_W, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SIReg8Mem (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SIReg4Reg
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX_W, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SIReg8Reg (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SIReg4Mem
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX_W, ESCAPE_0F__, 0x2c, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SIReg8Mem (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5a, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5a, 0, ModRM_RM__, Immediate_0 },    // CVTSS2SDRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5a, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5a, 0, ModRM_RM__, Immediate_0 },    // CVTSD2SSRegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x99, 0, ModRM_NONE, Immediate_0 },    // CWDAcc
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x99, 0, ModRM_NONE, Immediate_0 },    // CDQAcc
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x99, 0, ModRM_NONE, Immediate_0 },    // CQOAcc (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xfe, 1, ModRM_EXT_, Immediate_0 },    // DEC1Reg
#ifdef TR_TARGET_64BIT
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC4Reg
#else
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x48, 0, ModRM_NONE, Immediate_0 },    // DEC2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x48, 0, ModRM_NONE, Immediate_0 },    // DEC4Reg
#endif
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC8Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xfe, 1, ModRM_EXT_, Immediate_0 },    // DEC1Mem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC2Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC4Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xff, 1, ModRM_EXT_, Immediate_0 },    // DEC8Mem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe1) },    // FABSReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe1) },    // DABSReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xfa) },    // FSQRTReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xfa) },    // DSQRTReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xc0) },    // FADDRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xc0) },    // DADDRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xc0) },    // FADDPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x00) },    // FADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x00) },    // DADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x00) },    // FIADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x00) },    // DIADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x00) },    // FSADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x00) },    // DSADDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe0) },    // FCHSReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe0) },    // DCHSReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xf0) },    // FDIVRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xf0) },    // DDIVRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x30) },    // FDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x30) },    // DDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xf8) },    // FDIVPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x30) },    // FIDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x30) },    // DIDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x30) },    // FSDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x30) },    // DSDIVRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xf0) },    // FDIVRRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xf0) },    // DDIVRRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x38) },    // FDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x38) },    // DDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xf0) },    // FDIVRPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x38) },    // FIDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x38) },    // DIDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x38) },    // FSDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x38) },    // DSDIVRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x00) },    // FILDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x00) },    // DILDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x28) },    // FLLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x28) },    // DLLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x00) },    // FSLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x00) },    // DSLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x10) },    // FISTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x10) },    // DISTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x18) },    // FISTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0x18) },    // DISTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x38) },    // FLSTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x38) },    // DLSTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x10) },    // FSSTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x10) },    // DSSTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x18) },    // FSSTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0x18) },    // DSSTPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xed) },    // FLDLN2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xc0) },    // FLDRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xc0) },    // DLDRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x00) },    // FLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0x00) },    // DLDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xee) },    // FLD0Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xee) },    // DLD0Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe8) },    // FLD1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe8) },    // DLD1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x00) },    // FLDMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0x00) },    // DLDMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x28) },    // LDCWMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xc8) },    // FMULRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xc8) },    // DMULRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xc8) },    // FMULPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x08) },    // FMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x08) },    // DMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x08) },    // FIMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x08) },    // DIMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x08) },    // FSMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x08) },    // DSMULRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0xe2) },    // FNCLEX
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xf8) },    // FPREMRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xfd) },    // FSCALERegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x10) },    // FSTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0x10) },    // DSTMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0xd0) },    // FSTRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0xd0) },    // DSTRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x18) },    // FSTPMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0x18) },    // DSTPMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0xd8) },    // FSTPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0xd8) },    // DSTPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0x38) },    // STCWMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdd, X87_________________(0x38) },    // STSWMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0xe0) },    // STSWAcc
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xe0) },    // FSUBRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xe0) },    // DSUBRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x20) },    // FSUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x20) },    // DSUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xe8) },    // FSUBPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x20) },    // FISUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x20) },    // DISUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x20) },    // FSSUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x20) },    // DSSUBRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xe0) },    // FSUBRRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xe0) },    // DSUBRRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x28) },    // FSUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x28) },    // DSUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xe0) },    // FSUBRPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x28) },    // FISUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xda, X87_________________(0x28) },    // DISUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x28) },    // FSSUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0x28) },    // DSSUBRRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xe4) },    // FTSTReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xd0) },    // FCOMRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xd0) },    // DCOMRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x10) },    // FCOMRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x10) },    // DCOMRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0xd8) },    // FCOMPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd8, X87_________________(0x18) },    // FCOMPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdc, X87_________________(0x18) },    // DCOMPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xde, X87_________________(0xd9) },    // FCOMPP
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0xf0) },    // FCOMIRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdb, X87_________________(0xf0) },    // DCOMIRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xdf, X87_________________(0xf0) },    // FCOMIPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xf1) },    // FYL2X
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x2e, 0, ModRM_RM__, Immediate_0 },    // UCOMISSRegReg TODO
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x2e, 0, ModRM_RM__, Immediate_0 },    // UCOMISSRegMem
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x2e, 0, ModRM_RM__, Immediate_0 },    // UCOMISDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x2e, 0, ModRM_RM__, Immediate_0 },    // UCOMISDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd9, X87_________________(0xc8) },    // FXCHReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 7, ModRM_EXT_, Immediate_0 },    // IDIV1AccReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV2AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV4AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV8AccReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 6, ModRM_EXT_, Immediate_0 },    // DIV4AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 6, ModRM_EXT_, Immediate_0 },    // DIV8AccReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 7, ModRM_EXT_, Immediate_0 },    // IDIV1AccMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV2AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV4AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 7, ModRM_EXT_, Immediate_0 },    // IDIV8AccMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 6, ModRM_EXT_, Immediate_0 },    // DIV4AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 6, ModRM_EXT_, Immediate_0 },    // DIV8AccMem (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5e, 0, ModRM_RM__, Immediate_0 },    // DIVSSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5e, 0, ModRM_RM__, Immediate_0 },    // DIVSSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5e, 0, ModRM_RM__, Immediate_0 },    // DIVSDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5e, 0, ModRM_RM__, Immediate_0 },    // DIVSDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 5, ModRM_EXT_, Immediate_0 },    // IMUL1AccReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL2AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL4AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL8AccReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 5, ModRM_EXT_, Immediate_0 },    // IMUL1AccMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL2AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL4AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 5, ModRM_EXT_, Immediate_0 },    // IMUL8AccMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xaf, 0, ModRM_RM__, Immediate_0 },    // IMUL8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_2 },    // IMUL2RegRegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL2RegRegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_4 },    // IMUL4RegRegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_4 },    // IMUL8RegRegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL4RegRegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL8RegRegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_2 },    // IMUL2RegMemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL2RegMemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_4 },    // IMUL4RegMemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x69, 0, ModRM_RM__, Immediate_4 },    // IMUL8RegMemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL4RegMemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x6b, 0, ModRM_RM__, Immediate_S },    // IMUL8RegMemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 4, ModRM_EXT_, Immediate_0 },    // MUL1AccReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL2AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL4AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL8AccReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 4, ModRM_EXT_, Immediate_0 },    // MUL1AccMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL2AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL4AccMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 4, ModRM_EXT_, Immediate_0 },    // MUL8AccMem (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULSSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULSSRegMem
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULPSRegReg
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULPSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULSDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULSDRegMem
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULPDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x59, 0, ModRM_RM__, Immediate_0 },    // MULPDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xfe, 0, ModRM_EXT_, Immediate_0 },    // INC1Reg
#ifdef TR_TARGET_64BIT
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC4Reg
#else
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x40, 0, ModRM_NONE, Immediate_0 },    // INC2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x40, 0, ModRM_NONE, Immediate_0 },    // INC4Reg
#endif
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC8Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xfe, 0, ModRM_EXT_, Immediate_0 },    // INC1Mem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC2Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC4Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xff, 0, ModRM_EXT_, Immediate_0 },    // INC8Mem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x77, 0, ModRM_NONE, Immediate_1 },    // JA1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x73, 0, ModRM_NONE, Immediate_1 },    // JAE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x72, 0, ModRM_NONE, Immediate_1 },    // JB1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x76, 0, ModRM_NONE, Immediate_1 },    // JBE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x74, 0, ModRM_NONE, Immediate_1 },    // JE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x75, 0, ModRM_NONE, Immediate_1 },    // JNE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7f, 0, ModRM_NONE, Immediate_1 },    // JG1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7d, 0, ModRM_NONE, Immediate_1 },    // JGE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7c, 0, ModRM_NONE, Immediate_1 },    // JL1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7e, 0, ModRM_NONE, Immediate_1 },    // JLE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x70, 0, ModRM_NONE, Immediate_1 },    // JO1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x71, 0, ModRM_NONE, Immediate_1 },    // JNO1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x78, 0, ModRM_NONE, Immediate_1 },    // JS1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x79, 0, ModRM_NONE, Immediate_1 },    // JNS1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7b, 0, ModRM_NONE, Immediate_1 },    // JPO1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x7a, 0, ModRM_NONE, Immediate_1 },    // JPE1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xeb, 0, ModRM_NONE, Immediate_1 },    // JMP1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x87, 0, ModRM_NONE, Immediate_4 },    // JA4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x83, 0, ModRM_NONE, Immediate_4 },    // JAE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x82, 0, ModRM_NONE, Immediate_4 },    // JB4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x86, 0, ModRM_NONE, Immediate_4 },    // JBE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x84, 0, ModRM_NONE, Immediate_4 },    // JE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x85, 0, ModRM_NONE, Immediate_4 },    // JNE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8f, 0, ModRM_NONE, Immediate_4 },    // JG4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8d, 0, ModRM_NONE, Immediate_4 },    // JGE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8c, 0, ModRM_NONE, Immediate_4 },    // JL4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8e, 0, ModRM_NONE, Immediate_4 },    // JLE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x80, 0, ModRM_NONE, Immediate_4 },    // JO4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x81, 0, ModRM_NONE, Immediate_4 },    // JNO4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x88, 0, ModRM_NONE, Immediate_4 },    // JS4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x89, 0, ModRM_NONE, Immediate_4 },    // JNS4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8b, 0, ModRM_NONE, Immediate_4 },    // JPO4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x8a, 0, ModRM_NONE, Immediate_4 },    // JPE4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xe9, 0, ModRM_NONE, Immediate_4 },    // JMP4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 4, ModRM_MR__, Immediate_0 },    // JMPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 4, ModRM_MR__, Immediate_0 },    // JMPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xe3, 0, ModRM_NONE, Immediate_1 },    // JRCXZ1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xe2, 0, ModRM_NONE, Immediate_1 },    // LOOP1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x9f, 0, ModRM_NONE, Immediate_0 },    // LAHF
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0xf0, 0, ModRM_NONE, Immediate_0 },    // LDDQU
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x8d, 0, ModRM_RM__, Immediate_0 },    // LEA2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8d, 0, ModRM_RM__, Immediate_0 },    // LEA4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x8d, 0, ModRM_RM__, Immediate_0 },    // LEA8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x88, 0, ModRM_MR__, Immediate_0 },    // S1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x89, 0, ModRM_MR__, Immediate_0 },    // S2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x89, 0, ModRM_MR__, Immediate_0 },    // S4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x89, 0, ModRM_MR__, Immediate_0 },    // S8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc6, 0, ModRM_EXT_, Immediate_1 },    // S1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_2 },    // S2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_4 },    // S4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_4 },    // XRS4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_4 },    // S8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F3, REX_W, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_4 },    // XRS8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8a, 0, ModRM_RM__, Immediate_0 },    // L1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // L2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // L4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // L8RegMem (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x28, 0, ModRM_RM__, Immediate_0 },    // MOVAPSRegReg
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x28, 0, ModRM_RM__, Immediate_0 },    // MOVAPSRegMem
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x29, 0, ModRM_MR__, Immediate_0 },    // MOVAPSMemReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x28, 0, ModRM_RM__, Immediate_0 },    // MOVAPDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x28, 0, ModRM_RM__, Immediate_0 },    // MOVAPDRegMem
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x29, 0, ModRM_MR__, Immediate_0 },    // MOVAPDMemReg
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVUPSRegReg
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVUPSRegMem
    { VEX_L128, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x11, 0, ModRM_MR__, Immediate_0 },    // MOVUPSMemReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVUPDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVUPDRegMem
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x11, 0, ModRM_MR__, Immediate_0 },    // MOVUPDMemReg
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVSSRegReg
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVSSRegMem
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x11, 0, ModRM_MR__, Immediate_0 },    // MOVSSMemReg
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVSDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0x10, 0, ModRM_RM__, Immediate_0 },    // MOVSDRegMem
    { VEX_L128, VEX_vNONE, PREFIX_F2, REX__, ESCAPE_0F__, 0x11, 0, ModRM_MR__, Immediate_0 },    // MOVSDMemReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x51, 0, ModRM_RM__, Immediate_0 },    // SQRTSFRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x51, 0, ModRM_RM__, Immediate_0 },    // SQRTSDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x6e, 0, ModRM_RM__, Immediate_0 },    // MOVDRegReg4
    { VEX_L128, VEX_vNONE, PREFIX_66, REX_W, ESCAPE_0F__, 0x6e, 0, ModRM_RM__, Immediate_0 },    // MOVQRegReg8 (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x7e, 0, ModRM_MR__, Immediate_0 },    // MOVDReg4Reg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX_W, ESCAPE_0F__, 0x7e, 0, ModRM_MR__, Immediate_0 },    // MOVQReg8Reg (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x6f, 0, ModRM_RM__, Immediate_0 },    // MOVDQURegReg
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x6f, 0, ModRM_RM__, Immediate_0 },    // MOVDQURegMem
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x7f, 0, ModRM_MR__, Immediate_0 },    // MOVDQUMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8a, 0, ModRM_RM__, Immediate_0 },    // MOV1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // MOV2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // MOV4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // MOV8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x4f, 0, ModRM_RM__, Immediate_0 },    // CMOVG4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x4f, 0, ModRM_RM__, Immediate_0 },    // CMOVG8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x4c, 0, ModRM_RM__, Immediate_0 },    // CMOVL4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x4c, 0, ModRM_RM__, Immediate_0 },    // CMOVL8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x44, 0, ModRM_RM__, Immediate_0 },    // CMOVE4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x44, 0, ModRM_RM__, Immediate_0 },    // CMOVE8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x45, 0, ModRM_RM__, Immediate_0 },    // CMOVNE4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0x45, 0, ModRM_RM__, Immediate_0 },    // CMOVNE8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xb0, 0, ModRM_NONE, Immediate_1 },    // MOV1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xb8, 0, ModRM_NONE, Immediate_2 },    // MOV2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xb8, 0, ModRM_NONE, Immediate_4 },    // MOV4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc7, 0, ModRM_EXT_, Immediate_4 },    // MOV8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xb8, 0, ModRM_NONE, Immediate_8 },    // MOV8RegImm64 (AMD64)
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x12, 0, ModRM_RM__, Immediate_0 },    // MOVLPDRegMem
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x13, 0, ModRM_MR__, Immediate_0 },    // MOVLPDMemReg
    { VEX_L128, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0x7e, 0, ModRM_RM__, Immediate_0 },    // MOVQRegMem
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xd6, 0, ModRM_MR__, Immediate_0 },    // MOVQMemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa4, 0, ModRM_NONE, Immediate_0 },    // MOVSB
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // MOVSW
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // MOVSD
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // MOVSQ (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg2Reg1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg4Reg1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Reg1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbf, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg4Reg2
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbf, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Reg2 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x63, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Reg4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg2Mem1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg4Mem1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbe, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Mem1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xbf, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg4Mem2
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xbf, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Mem2 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x63, 0, ModRM_RM__, Immediate_0 },    // MOVSXReg8Mem4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg2Reg1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg4Reg1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg8Reg1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb7, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg4Reg2
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb7, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg8Reg2 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8b, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg8Reg4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg2Mem1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg4Mem1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb6, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg8Mem1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xb7, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg4Mem2
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_0F__, 0xb7, 0, ModRM_RM__, Immediate_0 },    // MOVZXReg8Mem2 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 3, ModRM_EXT_, Immediate_0 },    // NEG1Reg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG4Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG8Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 3, ModRM_EXT_, Immediate_0 },    // NEG1Mem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG2Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG4Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 3, ModRM_EXT_, Immediate_0 },    // NEG8Mem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 2, ModRM_EXT_, Immediate_0 },    // NOT1Reg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT2Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT4Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT8Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 2, ModRM_EXT_, Immediate_0 },    // NOT1Mem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT2Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT4Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 2, ModRM_EXT_, Immediate_0 },    // NOT8Mem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0c, 0, ModRM_NONE, Immediate_1 },    // OR1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x0d, 0, ModRM_NONE, Immediate_2 },    // OR2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0d, 0, ModRM_NONE, Immediate_4 },    // OR4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x0d, 0, ModRM_NONE, Immediate_4 },    // OR8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 1, ModRM_EXT_, Immediate_1 },    // OR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_2 },    // OR2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_4 },    // OR4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_4 },    // OR8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 1, ModRM_EXT_, Immediate_1 },    // OR1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_2 },    // OR2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_4 },    // OR4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 1, ModRM_EXT_, Immediate_4 },    // OR8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // LOR4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x09, 0, ModRM_MR__, Immediate_0 },    // LOR4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x09, 0, ModRM_MR__, Immediate_0 },    // LOR8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 1, ModRM_EXT_, Immediate_S },    // OR8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0a, 0, ModRM_RM__, Immediate_0 },    // OR1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0a, 0, ModRM_RM__, Immediate_0 },    // OR1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x0b, 0, ModRM_RM__, Immediate_0 },    // OR8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x08, 0, ModRM_MR__, Immediate_0 },    // OR1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x09, 0, ModRM_MR__, Immediate_0 },    // OR2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x09, 0, ModRM_MR__, Immediate_0 },    // OR4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x09, 0, ModRM_MR__, Immediate_0 },    // OR8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_____, 0x90, 0, ModRM_NONE, Immediate_0 },    // PAUSE
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x74, 0, ModRM_RM__, Immediate_0 },    // PCMPEQBRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0xd7, 0, ModRM_RM__, Immediate_0 },    // PMOVMSKB4RegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F38, 0x33, 0, ModRM_RM__, Immediate_0 },    // PMOVZXWD
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F38, 0x40, 0, ModRM_RM__, Immediate_0 },    // PMULLD
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0xfe, 0, ModRM_RM__, Immediate_0 },    // PADDD
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F38, 0x00, 0, ModRM_RM__, Immediate_0 },    // PSHUFBRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x70, 0, ModRM_RM__, Immediate_1 },    // PSHUFDRegRegImm1
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x70, 0, ModRM_RM__, Immediate_1 },    // PSHUFDRegMemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x73, 3, ModRM_EXT_, Immediate_1 },    // PSRLDQRegImm1
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F38, 0x30, 0, ModRM_RM__, Immediate_0 },    // PMOVZXxmm18Reg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX_F3, REX__, ESCAPE_0F__, 0xb8, 0, ModRM_RM__, Immediate_0 },    // POPCNT4RegReg
    { VEX_L___, VEX_vNONE, PREFIX_F3, REX_W, ESCAPE_0F__, 0xb8, 0, ModRM_RM__, Immediate_0 },    // POPCNT8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x58, 0, ModRM_NONE, Immediate_0 },    // POPReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x8f, 0, ModRM_MR__, Immediate_0 },    // POPMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x6a, 0, ModRM_NONE, Immediate_S },    // PUSHImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x68, 0, ModRM_NONE, Immediate_4 },    // PUSHImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x50, 0, ModRM_NONE, Immediate_0 },    // PUSHReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 6, ModRM_EXT_, Immediate_0 },    // PUSHRegLong
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xff, 6, ModRM_EXT_, Immediate_0 },    // PUSHMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 2, ModRM_EXT_, Immediate_1 },    // RCL1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 2, ModRM_EXT_, Immediate_1 },    // RCL4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 3, ModRM_EXT_, Immediate_1 },    // RCR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 3, ModRM_EXT_, Immediate_1 },    // RCR4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa4, 0, ModRM_NONE, Immediate_0 },    // REPMOVSB
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // REPMOVSW
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // REPMOVSD
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xa5, 0, ModRM_NONE, Immediate_0 },    // REPMOVSQ (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xaa, 0, ModRM_NONE, Immediate_0 },    // REPSTOSB
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // REPSTOSW
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // REPSTOSD
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // REPSTOSQ (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc3, 0, ModRM_NONE, Immediate_0 },    // RET
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc2, 0, ModRM_NONE, Immediate_2 },    // RETImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 0, ModRM_EXT_, Immediate_1 },    // ROL1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 0, ModRM_EXT_, Immediate_1 },    // ROL2RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 0, ModRM_EXT_, Immediate_1 },    // ROL4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 0, ModRM_EXT_, Immediate_1 },    // ROL8RegImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 0, ModRM_EXT_, Immediate_0 },    // ROL4RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 0, ModRM_EXT_, Immediate_0 },    // ROL8RegCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 1, ModRM_EXT_, Immediate_1 },    // ROR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 1, ModRM_EXT_, Immediate_1 },    // ROR2RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 1, ModRM_EXT_, Immediate_1 },    // ROR4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 1, ModRM_EXT_, Immediate_1 },    // ROR8RegImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x9e, 0, ModRM_NONE, Immediate_0 },    // SAHF
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 4, ModRM_EXT_, Immediate_1 },    // SHL1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 4, ModRM_EXT_, Immediate_0 },    // SHL1RegCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL2RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL2RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL8RegImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL4RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL8RegCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 4, ModRM_EXT_, Immediate_1 },    // SHL1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 4, ModRM_EXT_, Immediate_0 },    // SHL1MemCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL2MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL2MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL4MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 4, ModRM_EXT_, Immediate_1 },    // SHL8MemImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL4MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 4, ModRM_EXT_, Immediate_0 },    // SHL8MemCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 5, ModRM_EXT_, Immediate_1 },    // SHR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 5, ModRM_EXT_, Immediate_0 },    // SHR1RegCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR2RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR2RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR8RegImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR4RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR8RegCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 5, ModRM_EXT_, Immediate_1 },    // SHR1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 5, ModRM_EXT_, Immediate_0 },    // SHR1MemCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR2MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR2MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR4MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 5, ModRM_EXT_, Immediate_1 },    // SHR8MemImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR4MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 5, ModRM_EXT_, Immediate_0 },    // SHR8MemCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 7, ModRM_EXT_, Immediate_1 },    // SAR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 7, ModRM_EXT_, Immediate_0 },    // SAR1RegCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR2RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR2RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR4RegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR8RegImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR4RegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR8RegCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc0, 7, ModRM_EXT_, Immediate_1 },    // SAR1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd2, 7, ModRM_EXT_, Immediate_0 },    // SAR1MemCL
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR2MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR2MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR4MemImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xc1, 7, ModRM_EXT_, Immediate_1 },    // SAR8MemImm1 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR4MemCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xd3, 7, ModRM_EXT_, Immediate_0 },    // SAR8MemCL (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1c, 0, ModRM_NONE, Immediate_1 },    // SBB1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x1d, 0, ModRM_NONE, Immediate_2 },    // SBB2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1d, 0, ModRM_NONE, Immediate_4 },    // SBB4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x1d, 0, ModRM_NONE, Immediate_4 },    // SBB8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 3, ModRM_EXT_, Immediate_1 },    // SBB1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_2 },    // SBB2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_4 },    // SBB4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_4 },    // SBB8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 3, ModRM_EXT_, Immediate_1 },    // SBB1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_2 },    // SBB2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_4 },    // SBB4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 3, ModRM_EXT_, Immediate_4 },    // SBB8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 3, ModRM_EXT_, Immediate_S },    // SBB8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1a, 0, ModRM_RM__, Immediate_0 },    // SBB1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1a, 0, ModRM_RM__, Immediate_0 },    // SBB1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x1b, 0, ModRM_RM__, Immediate_0 },    // SBB8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x18, 0, ModRM_MR__, Immediate_0 },    // SBB1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x19, 0, ModRM_MR__, Immediate_0 },    // SBB2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x19, 0, ModRM_MR__, Immediate_0 },    // SBB4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x19, 0, ModRM_MR__, Immediate_0 },    // SBB8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x97, 0, ModRM_MR__, Immediate_0 },    // SETA1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x93, 0, ModRM_MR__, Immediate_0 },    // SETAE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x92, 0, ModRM_MR__, Immediate_0 },    // SETB1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x96, 0, ModRM_MR__, Immediate_0 },    // SETBE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x94, 0, ModRM_MR__, Immediate_0 },    // SETE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x95, 0, ModRM_MR__, Immediate_0 },    // SETNE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9f, 0, ModRM_MR__, Immediate_0 },    // SETG1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9d, 0, ModRM_MR__, Immediate_0 },    // SETGE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9c, 0, ModRM_MR__, Immediate_0 },    // SETL1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9e, 0, ModRM_MR__, Immediate_0 },    // SETLE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x98, 0, ModRM_MR__, Immediate_0 },    // SETS1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x99, 0, ModRM_MR__, Immediate_0 },    // SETNS1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9b, 0, ModRM_MR__, Immediate_0 },    // SETPO1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9a, 0, ModRM_MR__, Immediate_0 },    // SETPE1Reg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x97, 0, ModRM_MR__, Immediate_0 },    // SETA1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x93, 0, ModRM_MR__, Immediate_0 },    // SETAE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x92, 0, ModRM_MR__, Immediate_0 },    // SETB1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x96, 0, ModRM_MR__, Immediate_0 },    // SETBE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x94, 0, ModRM_MR__, Immediate_0 },    // SETE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x95, 0, ModRM_MR__, Immediate_0 },    // SETNE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9f, 0, ModRM_MR__, Immediate_0 },    // SETG1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9d, 0, ModRM_MR__, Immediate_0 },    // SETGE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9c, 0, ModRM_MR__, Immediate_0 },    // SETL1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x9e, 0, ModRM_MR__, Immediate_0 },    // SETLE1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x98, 0, ModRM_MR__, Immediate_0 },    // SETS1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x99, 0, ModRM_MR__, Immediate_0 },    // SETNS1Mem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xa4, 0, ModRM_MR__, Immediate_1 },    // SHLD4RegRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xa5, 0, ModRM_MR__, Immediate_0 },    // SHLD4RegRegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xa4, 0, ModRM_MR__, Immediate_1 },    // SHLD4MemRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xa5, 0, ModRM_MR__, Immediate_0 },    // SHLD4MemRegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xac, 0, ModRM_MR__, Immediate_1 },    // SHRD4RegRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xad, 0, ModRM_MR__, Immediate_0 },    // SHRD4RegRegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xac, 0, ModRM_MR__, Immediate_1 },    // SHRD4MemRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xad, 0, ModRM_MR__, Immediate_0 },    // SHRD4MemRegCL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xaa, 0, ModRM_NONE, Immediate_0 },    // STOSB
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // STOSW
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // STOSD
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xab, 0, ModRM_NONE, Immediate_0 },    // STOSQ (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2c, 0, ModRM_NONE, Immediate_1 },    // SUB1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x2d, 0, ModRM_NONE, Immediate_2 },    // SUB2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2d, 0, ModRM_NONE, Immediate_4 },    // SUB4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x2d, 0, ModRM_NONE, Immediate_4 },    // SUB8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 5, ModRM_EXT_, Immediate_1 },    // SUB1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_2 },    // SUB2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_4 },    // SUB4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_4 },    // SUB8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 5, ModRM_EXT_, Immediate_1 },    // SUB1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_2 },    // SUB2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_4 },    // SUB4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 5, ModRM_EXT_, Immediate_4 },    // SUB8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 5, ModRM_EXT_, Immediate_S },    // SUB8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2a, 0, ModRM_RM__, Immediate_0 },    // SUB1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2a, 0, ModRM_RM__, Immediate_0 },    // SUB1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x2b, 0, ModRM_RM__, Immediate_0 },    // SUB8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x28, 0, ModRM_MR__, Immediate_0 },    // SUB1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x29, 0, ModRM_MR__, Immediate_0 },    // SUB2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x29, 0, ModRM_MR__, Immediate_0 },    // SUB4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x29, 0, ModRM_MR__, Immediate_0 },    // SUB8MemReg (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5c, 0, ModRM_RM__, Immediate_0 },    // SUBSSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F3, REX__, ESCAPE_0F__, 0x5c, 0, ModRM_RM__, Immediate_0 },    // SUBSSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5c, 0, ModRM_RM__, Immediate_0 },    // SUBSDRegReg
    { VEX_L128, VEX_vReg_, PREFIX_F2, REX__, ESCAPE_0F__, 0x5c, 0, ModRM_RM__, Immediate_0 },    // SUBSDRegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa8, 0, ModRM_NONE, Immediate_1 },    // TEST1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa8, 0, ModRM_NONE, Immediate_1 },    // TEST1AccHImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xa9, 0, ModRM_NONE, Immediate_2 },    // TEST2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xa9, 0, ModRM_NONE, Immediate_4 },    // TEST4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xa9, 0, ModRM_NONE, Immediate_4 },    // TEST8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 0, ModRM_MR__, Immediate_1 },    // TEST1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_2 },    // TEST2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_4 },    // TEST4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_4 },    // TEST8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf6, 0, ModRM_MR__, Immediate_1 },    // TEST1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_2 },    // TEST2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_4 },    // TEST4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0xf7, 0, ModRM_MR__, Immediate_4 },    // TEST8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x84, 0, ModRM_MR__, Immediate_0 },    // TEST1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x84, 0, ModRM_MR__, Immediate_0 },    // TEST1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x85, 0, ModRM_MR__, Immediate_0 },    // TEST8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc6, 7, ModRM_EXT_, Immediate_1 },    // XABORT
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0xc7, 7, ModRM_EXT_, Immediate_2 },    // XBEGIN2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0xc7, 7, ModRM_EXT_, Immediate_4 },    // XBEGIN4
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x90, 0, ModRM_NONE, Immediate_0 },    // XCHG2AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x90, 0, ModRM_NONE, Immediate_0 },    // XCHG4AccReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x90, 0, ModRM_NONE, Immediate_0 },    // XCHG8AccReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x86, 0, ModRM_MR__, Immediate_0 },    // XCHG1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x86, 0, ModRM_RM__, Immediate_0 },    // XCHG1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x87, 0, ModRM_RM__, Immediate_0 },    // XCHG2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x87, 0, ModRM_RM__, Immediate_0 },    // XCHG4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x87, 0, ModRM_RM__, Immediate_0 },    // XCHG8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x86, 0, ModRM_MR__, Immediate_0 },    // XCHG1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x87, 0, ModRM_MR__, Immediate_0 },    // XCHG8MemReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x01, 2, ModRM_EXT_, Immediate_0 },    // XEND
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x34, 0, ModRM_NONE, Immediate_1 },    // XOR1AccImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x35, 0, ModRM_NONE, Immediate_2 },    // XOR2AccImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x35, 0, ModRM_NONE, Immediate_4 },    // XOR4AccImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x35, 0, ModRM_NONE, Immediate_4 },    // XOR8AccImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 6, ModRM_EXT_, Immediate_1 },    // XOR1RegImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_2 },    // XOR2RegImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR2RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_4 },    // XOR4RegImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_4 },    // XOR8RegImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR4RegImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR8RegImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x80, 6, ModRM_EXT_, Immediate_1 },    // XOR1MemImm1
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_2 },    // XOR2MemImm2
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR2MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_4 },    // XOR4MemImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x81, 6, ModRM_EXT_, Immediate_4 },    // XOR8MemImm4 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR4MemImms
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x83, 6, ModRM_EXT_, Immediate_S },    // XOR8MemImms (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x32, 0, ModRM_RM__, Immediate_0 },    // XOR1RegReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR2RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR4RegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR8RegReg (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x32, 0, ModRM_RM__, Immediate_0 },    // XOR1RegMem
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR2RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR4RegMem
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x33, 0, ModRM_RM__, Immediate_0 },    // XOR8RegMem (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x30, 0, ModRM_MR__, Immediate_0 },    // XOR1MemReg
    { VEX_L___, VEX_vNONE, PREFIX_66, REX__, ESCAPE_____, 0x31, 0, ModRM_MR__, Immediate_0 },    // XOR2MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x31, 0, ModRM_MR__, Immediate_0 },    // XOR4MemReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX_W, ESCAPE_____, 0x31, 0, ModRM_MR__, Immediate_0 },    // XOR8MemReg (AMD64)
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x57, 0, ModRM_RM__, Immediate_0 },    // XORPSRegReg
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x57, 0, ModRM_RM__, Immediate_0 },    // XORPSRegMem
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x57, 0, ModRM_RM__, Immediate_0 },    // XORPDRegReg
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xae, 6, ModRM_EXT_, Immediate_0 },    // MFENCE
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xae, 5, ModRM_EXT_, Immediate_0 },    // LFENCE
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0xae, 7, ModRM_EXT_, Immediate_0 },    // SFENCE
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F3A, 0x61, 0, ModRM_RM__, Immediate_1 },    // PCMPESTRIRegRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x18, 0, ModRM_EXT_, Immediate_0 },    // PREFETCHNTA
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x18, 1, ModRM_EXT_, Immediate_0 },    // PREFETCHT0
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x18, 2, ModRM_EXT_, Immediate_0 },    // PREFETCHT1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_0F__, 0x18, 3, ModRM_EXT_, Immediate_0 },    // PREFETCHT2
    { VEX_L128, VEX_vReg_, PREFIX___, REX__, ESCAPE_0F__, 0x55, 0, ModRM_RM__, Immediate_0 },    // ANDNPSRegReg
    { VEX_L128, VEX_vReg_, PREFIX_66, REX__, ESCAPE_0F__, 0x55, 0, ModRM_RM__, Immediate_0 },    // ANDNPDRegReg
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x73, 6, ModRM_EXT_, Immediate_1 },    // PSLLQRegImm1
    { VEX_L128, VEX_vNONE, PREFIX_66, REX__, ESCAPE_0F__, 0x73, 2, ModRM_EXT_, Immediate_1 },    // PSRLQRegImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // FENCE
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // VGFENCE
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // PROCENTRY
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_8 },    // DQImm64 (AMD64)
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_4 },    // DDImm4
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_2 },    // DWImm2
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_1 },    // DBImm1
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // WRTBAR
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // ASSOCREGS
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // FPREGSPILL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // VirtualGuardNOP
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // LABEL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // FCMPEVAL
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // RestoreVMThread
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // PPS_OPCOUNT
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // PPS_OPFIELD
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // AdjustFramePtr
    { VEX_L___, VEX_vNONE, PREFIX___, REX__, ESCAPE_____, 0x00, 0, ModRM_NONE, Immediate_0 },    // ReturnMarker
};

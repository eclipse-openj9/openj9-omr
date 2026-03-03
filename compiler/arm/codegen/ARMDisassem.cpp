/*******************************************************************************
 * Copyright IBM Corp. and others 2000
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

/******************************************************************************/
/*                                                                            */
/*  FUNCTION         = to disassemble one instruction into                    */
/*                     mnemonics and instruction format.                      */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/*  Entry points:                                                             */
/*                                                                            */
/*  void disassemble(int32_t* instrPtr, char* mBuf, char* iBuf);              */
/*                                                                            */
/******************************************************************************/

#ifndef TR_TARGET_ARM
int ARMDiss;
#else

#include <string.h>
#include <stdio.h>
#include "codegen/ARMDisassem.hpp"

unsigned getField(unsigned data, int loc, int len) { return (data >> loc) & ((1 << len) - 1); }

uint32_t rotateRight(uint32_t uval, uint32_t n)
{
    uint32_t tmp1 = uval >> n; // should be unsigned
    uint32_t tmp2 = uval << (32 - n);
    return tmp1 | tmp2;
}

uint32_t concatImm4Imm4(uint32_t hi4, uint32_t low4) { return hi4 << 4 | low4; }

const char *regStr(int regNum, bool W) // PC is a single purpose register.
{ // If a register value is to be updated by the instruction, it is printed in capitals.
    switch (regNum) {
        case 0:
            return (W ? "R0" : "r0");
        case 1:
            return (W ? "R1" : "r1");
        case 2:
            return (W ? "R2" : "r2");
        case 3:
            return (W ? "R3" : "r3");
        case 4:
            return (W ? "R4" : "r4");
        case 5:
            return (W ? "R5" : "r5");
        case 6:
            return (W ? "R6" : "r6");
        case 7:
            return (W ? "R7" : "r7");
        case 8:
            return (W ? "R8" : "r8");
        case 9:
            return (W ? "R9" : "r9");
        case 10:
            return (W ? "R10" : "r10");
        case 11:
            return (W ? "R11" : "r11");
        case 12:
            return (W ? "R12" : "r12");
        case 13:
            return (W ? "R13" : "r13");
        case 14:
            return (W ? "R14" : "r14");
        case 15:
            return (W ? "PC" : "pc");
        default:
            return (W ? "R??" : "r??");
    }
}
#ifdef __VFP_FP__
const char *doubleRegStr(uint32_t regNum, bool W)
{ // If a register value is to be updated by the instruction, it is printed in capitals.
    switch (regNum >> 1) {
        case 0:
            return (W ? "D0" : "d0");
        case 1:
            return (W ? "D1" : "d1");
        case 2:
            return (W ? "D2" : "d2");
        case 3:
            return (W ? "D3" : "d3");
        case 4:
            return (W ? "D4" : "d4");
        case 5:
            return (W ? "D5" : "d5");
        case 6:
            return (W ? "D6" : "d6");
        case 7:
            return (W ? "D7" : "d7");
        case 8:
            return (W ? "D8" : "d8");
        case 9:
            return (W ? "D9" : "d9");
        case 10:
            return (W ? "D10" : "d10");
        case 11:
            return (W ? "D11" : "d11");
        case 12:
            return (W ? "D12" : "d12");
        case 13:
            return (W ? "D13" : "d13");
        case 14:
            return (W ? "D14" : "d14");
        case 15:
            return (W ? "D15" : "d15");
        default:
            return (W ? "D??" : "d??");
    }
}

const char *singleRegStr(uint32_t regNum, bool W)
{ // If a register value is to be updated by the instruction, it is printed in capitals.
    switch (regNum) {
        case 0:
            return (W ? "S0" : "s0");
        case 1:
            return (W ? "S1" : "s1");
        case 2:
            return (W ? "S2" : "s2");
        case 3:
            return (W ? "S3" : "s3");
        case 4:
            return (W ? "S4" : "s4");
        case 5:
            return (W ? "S5" : "s5");
        case 6:
            return (W ? "S6" : "s6");
        case 7:
            return (W ? "S7" : "s7");
        case 8:
            return (W ? "S8" : "s8");
        case 9:
            return (W ? "S9" : "s9");
        case 10:
            return (W ? "S10" : "s10");
        case 11:
            return (W ? "S11" : "s11");
        case 12:
            return (W ? "S12" : "s12");
        case 13:
            return (W ? "S13" : "s13");
        case 14:
            return (W ? "S14" : "s14");
        case 15:
            return (W ? "S15" : "s15");
        case 16:
            return (W ? "S16" : "s16");
        case 17:
            return (W ? "S17" : "s17");
        case 18:
            return (W ? "S18" : "s18");
        case 19:
            return (W ? "S19" : "s19");
        case 20:
            return (W ? "S20" : "s20");
        case 21:
            return (W ? "S21" : "s21");
        case 22:
            return (W ? "S22" : "s22");
        case 23:
            return (W ? "S23" : "s23");
        case 24:
            return (W ? "S24" : "s24");
        case 25:
            return (W ? "S25" : "s25");
        case 26:
            return (W ? "S26" : "s26");
        case 27:
            return (W ? "S27" : "s27");
        case 28:
            return (W ? "S28" : "s28");
        case 29:
            return (W ? "S29" : "s29");
        case 30:
            return (W ? "S30" : "s30");
        case 31:
            return (W ? "S31" : "s31");
        default:
            return (W ? "S??" : "s??");
    }
}
#endif

void constant32(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
#ifdef LINUX
    snprintf(mBuf, mBufSize, ".long");
#else
    snprintf(mBuf, mBufSize, "DCD");
#endif
    snprintf(iBuf, iBufSize, "0x%08x", *instrPtr);
}

void branchExchange(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    BranchExchangeInstruction *instr = (BranchExchangeInstruction *)instrPtr;

    if (instr->SBO == 0xfff) {
        snprintf(mBuf, mBufSize, "b%sx%s", instr->L ? "l" : "", condName[instr->cond]);
        snprintf(iBuf, iBufSize, "%s", regStr(instr->Rm, false));
    } else {
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
    }
}

void branch(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    BranchInstruction *instr = (BranchInstruction *)instrPtr;

    snprintf(mBuf, mBufSize, "b%s%s", instr->L ? "l" : "", condName[instr->cond]);
    snprintf(iBuf, iBufSize, "0x%08x", instrPtr + instr->offset + 2);
}

void moveStatusReg(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    MSRInstruction *instr = (MSRInstruction *)instrPtr;
    InstructionBits *instrBits = (InstructionBits *)instrPtr;

    if (!instr->to) // Move from status register
    {
        if (!instr->c || !instr->x || !instr->s || !instr->f || instr->rotate_imm || instr->immLo4) {
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            return;
        }

        snprintf(mBuf, mBufSize, "mrs%s", condName[instr->cond]);
        snprintf(iBuf, iBufSize, "%s, %cPSR", regStr(instr->Rd, true), instrBits->bit22 ? 'S' : 'C');
    } else // Move to status register
    {
        if (instr->Rd != 0xf) {
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            return;
        }
        snprintf(mBuf, mBufSize, "msr%s", condName[instr->cond]);
        const size_t last_oprndSize = 11;
        char last_oprnd[last_oprndSize];
        if (!instr->immForm) {
            if (instr->rotate_imm) {
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
                return;
            }
            snprintf(last_oprnd, last_oprndSize, "%s", regStr(instr->immLo4, false));
        } else {
            snprintf(last_oprnd, last_oprndSize, "0x%x",
                rotateRight(concatImm4Imm4(instr->immHi4, instr->immLo4), instr->rotate_imm * 2));
        }
        snprintf(iBuf, iBufSize, "%cPSR_%s%s%s%s, %s", instr->R ? 'S' : 'C', instr->c ? "c" : "", instr->x ? "x" : "",
            instr->s ? "s" : "", instr->f ? "f" : "", last_oprnd);
    }
}

void miscInstr(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    MiscellaneousInstruction *instr = (MiscellaneousInstruction *)instrPtr;
    InstructionBits *instrBits = (InstructionBits *)instrPtr;

    switch (instr->groupOpcode) {
        case 0:
            moveStatusReg(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
        case 1:
            if (!instrBits->bit22) {
                branchExchange(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            } else {
                if (instr->SBO1 == 0xf && instr->SBO2 == 0xf) {
                    snprintf(mBuf, mBufSize, "clz%s", condName[instr->cond]);
                    snprintf(iBuf, iBufSize, "%s, %s", regStr(instr->Rd, true), regStr(instr->Rm, false));
                } else {
                    constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
                }
            }
            break;
        case 3:
            branchExchange(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
        case 5:
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Enhanced DSP add/substracts
            break;
        case 7: {
            snprintf(mBuf, mBufSize, "bkpt");
            BreakpointInstruction *bkptInstr = (BreakpointInstruction *)instrPtr;
            snprintf(iBuf, iBufSize, "0x%x%x%s", bkptInstr->immed12, bkptInstr->immed4,
                bkptInstr->cond != 14 ? "   ; UNPREDICTABLE" : "");
        } break;
        case 8:
        case 10:
        case 12:
        case 14:
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Enhanced DSP multiplies
            break;
        default:
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid instruction
    }
}

void dataProcessing(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    DataProcessingInstruction *instr = (DataProcessingInstruction *)(uint32_t *)instrPtr;
    InstructionBits *instrBits = (InstructionBits *)(uint32_t *)instrPtr;

    static char *opcodeName[] = { "and", "eor", "sub", "rsb", "add", "adc", "sbc", "rsc", "tst", "teq", "cmp", "cmn",
        "orr", "mov", "bic", "mvn" };
    bool test_cmp
        = instrBits->bits23_24 == 2; // instr->opcode: 0b1000 || 0b1001 || 0b1010 || 0b1011 (tst, teq, cmp, cmn)
    bool mov = instrBits->bit21 && (instrBits->bits23_24 == 3); // instr->opcode: 0b1101 || 0b1111 (mov, mvn)

    if (test_cmp && instr->Rd || mov && instr->Rn) {
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // This field for these instructions SBZ
        return;
    }

    snprintf(mBuf, mBufSize, "%s%s%s", opcodeName[instr->opcode], condName[instr->cond],
        (!test_cmp && instr->S) ? "s" : "");

    const size_t dest_src1Size = 9;
    char dest_src1[dest_src1Size];
    if (test_cmp || mov)
        snprintf(dest_src1, dest_src1Size, "%s", regStr(mov ? instr->Rd : instr->Rn, mov));
    else
        snprintf(dest_src1, dest_src1Size, "%s, %s", regStr(instr->Rd, true), regStr(instr->Rn, false));

    const size_t sh_oprndSize = 13;
    char sh_oprnd[sh_oprndSize];

    if (instrBits->bit25) // Immediate
    {
        DataProcessingImmedInstruction *instr = (DataProcessingImmedInstruction *)(uint32_t *)instrPtr;
        snprintf(sh_oprnd, sh_oprndSize, "#0x%x", rotateRight(instr->imm8, instr->rotate_imm * 2));
    } else if (instr->shifter == 0) //  Register  (bits[4..11] == 0)
    {
        snprintf(sh_oprnd, sh_oprndSize, "%s", regStr(instr->Rm, false));
    } else if (instr->shifter == 6) //  Rotate right with extend
    {
        snprintf(sh_oprnd, sh_oprndSize, "%s, RRX", regStr(instr->Rm, false));
    } else {
        static char *shift_rotate[] = { "LSL", "LSR", "ASR", "ROR" };
        if (instrBits->bit4) // Register shift
        {
            DataProcessingRegShiftInstruction *instr = (DataProcessingRegShiftInstruction *)(uint32_t *)instrPtr;
            snprintf(sh_oprnd, sh_oprndSize, "%s, %s %s", regStr(instr->Rm, false), shift_rotate[instr->shift],
                regStr(instr->Rs, false));
        } else // Immediate shift
        {
            DataProcessingImmShiftInstruction *instr = (DataProcessingImmShiftInstruction *)(uint32_t *)instrPtr;
            snprintf(sh_oprnd, sh_oprndSize, "%s, %s #%u", regStr(instr->Rm, false), shift_rotate[instr->shift],
                !instr->shift_imm && (instr->shift == 1 || instr->shift == 2) ? 32 : instr->shift_imm);
        }
    }

    snprintf(iBuf, iBufSize, "%s, %s", dest_src1, sh_oprnd);
}

void swap(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    SwapInstruction *instr = (SwapInstruction *)instrPtr;
    InstructionBits *instrBits = (InstructionBits *)instrPtr;

    if (instr->SBZ) {
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // for swp this field should be zero
    } else {
        snprintf(mBuf, mBufSize, "swp%s%s", condName[instr->cond], instr->B ? "b" : "");
        snprintf(iBuf, iBufSize, "%s, %s, [%s]", regStr(instr->Rd, true), regStr(instr->Rm, false),
            regStr(instr->Rn, false));
    }
}

void multiply(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    MultiplyInstruction *instr = (MultiplyInstruction *)instrPtr;
    InstructionBits *instrBits = (InstructionBits *)instrPtr;

    if (!instr->L) // MLA and MUL
    {
        if (!instr->A && instr->Rn) {
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // for mul this field should be zero
            return;
        }
        snprintf(mBuf, mBufSize, "%s%s%s", instr->A ? "mla" : "mul", condName[instr->cond], instr->S ? "s" : "");
        const size_t accumRegSize = 9;
        char accumReg[accumRegSize];
        snprintf(accumReg, accumRegSize, ", %s", regStr(instr->Rn, false));
        snprintf(iBuf, iBufSize, "%s, %s, %s%s", regStr(instr->Rd, true), regStr(instr->Rm, false),
            regStr(instr->Rs, false), instr->A ? accumReg : "");
    } else // long multiplies: SMLAL, SMULL, UMLAL, UMULL
    {
        snprintf(mBuf, mBufSize, "%c%sl%s%s", instr->U ? 's' : 'u', instr->A ? "mla" : "mul", condName[instr->cond],
            instr->A ? "s" : "");
        snprintf(iBuf, iBufSize, "%s, %s, %s, %s", regStr(instr->Rn, true), regStr(instr->Rd, true),
            regStr(instr->Rm, false), regStr(instr->Rs, false));
    }
}

void loadStore(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize, bool extra)
{
    InstructionBits *instrBits = (InstructionBits *)instrPtr;
    LoadStoreInstruction *instr = (LoadStoreInstruction *)instrPtr;

    if (extra && !instrBits->bit22 && instr->shift_imm != 1) { // this is a register form; bits11..7 should be 00001
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
        return;
    }

    if (!extra)
        snprintf(mBuf, mBufSize, "%s%s%s%s", instr->L ? "ldr" : "str", condName[instr->cond],
            instrBits->bit22 ? "b" : "", (!instr->P && instr->W) ? "t" : "");
    else
        snprintf(mBuf, mBufSize, "%s%s%s%s", instr->L ? "ldr" : "str", condName[instr->cond],
            instrBits->bit6 ? "s" : "", (instrBits->bit6 && !instrBits->bit5) ? "b" : "h");

    if ((!extra && !instrBits->bit25) || (extra && instrBits->bit22)) { // Immediate offset
        LoadStoreImmInstruction *instrImm = (LoadStoreImmInstruction *)instrPtr;
        LoadStoreImmMiscInstruction *instrImmMisc = (LoadStoreImmMiscInstruction *)instrPtr;
        snprintf(iBuf, iBufSize, "%s, [%s%s, #%s%u%s%s", regStr(instr->Rd, instr->L),
            regStr(instr->Rn, (!instr->P || instr->W)), instr->P ? "" : "]", instr->U ? "" : "-",
            extra ? concatImm4Imm4(instrImmMisc->immH, instrImmMisc->immL) : instrImm->immed, instr->P ? "]" : "",
            instr->W && instr->P ? "!" : "");
    } else if (extra || (instr->shift == 0 && instr->shift_imm == 0)) { //   Register offset/index
        snprintf(iBuf, iBufSize, "%s, [%s%s, %s%s%s%s", regStr(instr->Rd, instr->L),
            regStr(instr->Rn, (!instr->P || instr->W)), instr->P ? "" : "]", instr->U ? "" : "-",
            regStr(instr->Rm, false), instr->P ? "]" : "", instr->W ? "!" : "");
    } else // Scaled register offset/index
    {
        if (instr->shift == 3 && instr->shift_imm == 0) {
            snprintf(iBuf, iBufSize, "%s, [%s%s, %s%s, RRX%s%s", regStr(instr->Rd, instr->L),
                regStr(instr->Rn, (!instr->P || instr->W)), instr->P ? "" : "]", instr->U ? "" : "-",
                regStr(instr->Rm, false), instr->P ? "]" : "", instr->W ? "!" : "");
        } else {
            static char *sh_rot[] = { "LSL", "LSR", "ASR", "ROR" };
            snprintf(iBuf, iBufSize, "%s, [%s%s, %s%s, %s #%u%s%s", regStr(instr->Rd, instr->L),
                regStr(instr->Rn, (!instr->P || instr->W)), instr->P ? "" : "]", instr->U ? "" : "-",
                regStr(instr->Rm, false), sh_rot[instr->shift],
                (instr->shift_imm == 0 && (instr->shift == 1 || instr->shift == 2)) ? 32 : instr->shift_imm,
                instr->P ? "]" : "", instr->W ? "!" : "");
        }
    }
}

void mulAndExtraLoadStore(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    InstructionBits *instrBits = (InstructionBits *)instrPtr;
    uint32_t bits5_6 = instrBits->bit6 << 1 | instrBits->bit5;

    switch (bits5_6) {
        case 0:
            if (instrBits->bits23_24 == 2)
                swap(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            else
                multiply(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
        case 1:
            loadStore(instrPtr, mBuf, mBufSize, iBuf, iBufSize, true);
            break;
        case 2:
        case 3:
            if (instrBits->bit20)
                loadStore(instrPtr, mBuf, mBufSize, iBuf, iBufSize, true);
            else
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Enhanced DSP Extension
    }
}

void loadStoreMultiple(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    MultipleLoadStoreInstruction *instr = (MultipleLoadStoreInstruction *)instrPtr;

    static char *addr_mode[] = { "da", "ia", "db", "ib" };
    const size_t registersSize = 70;
    char registers[registersSize] = "";
    const size_t regSize = 6;
    char reg[regSize];

    for (int i = 0, pos = 0; i < 16; i++) {
        if (instr->register_list & (1 << i)) {
            snprintf(reg, regSize, "%s%s", pos ? "," : "", regStr(i, instr->L));
            snprintf(registers + pos, registersSize - pos, "%s", reg);
            pos += strlen(reg);
        }
    }
    int PU = instr->P << 1 | instr->U;
    snprintf(mBuf, mBufSize, "%s%s%s", instr->L ? "ldm" : "stm", condName[instr->cond], instr->W ? addr_mode[PU] : "");
    snprintf(iBuf, iBufSize, "%s%s, {%s}%s", regStr(instr->Rn, instr->W), instr->W ? "!" : "", registers,
        instr->S ? "^" : "");
}

#ifdef __VFP_FP__
static uint32_t getVFPDestination(VFPInstruction *instr) { return (instr->Fd << 1 | instr->D); }

static uint32_t getVFPFirstOperand(VFPInstruction *instr) { return (instr->Fn << 1 | instr->N); }

static uint32_t getVFPSecondOperand(VFPInstruction *instr) { return (instr->Fm << 1 | instr->M); }

void vfpTwoRegTransfer(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    VFPInstruction *instr = (VFPInstruction *)instrPtr;
    bool isDouble = (instr->cp == 11); // Coprocessor 11 is double, 10 is float

    if (instr->D == 1 && instr->N == 0 && instr->s == 0 && instr->xfer == 1) {
        if (isDouble) {
            if (instr->r) // L-bit
            {
                // MFRRD
                snprintf(mBuf, mBufSize, "%s%s", "mfrrd", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s, %s, %s", regStr(instr->Fd, true), regStr(instr->Fn, true),
                    doubleRegStr(getVFPSecondOperand(instr), false));
            } else {
                // MFDRR
                snprintf(mBuf, mBufSize, "%s%s", "mfdrr", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s, %s, %s", doubleRegStr(getVFPSecondOperand(instr), true),
                    regStr(instr->Fd, false), regStr(instr->Fn, false));
            }
        } else {
            uint32_t first = getVFPSecondOperand(instr);
            uint32_t next = (first == 31) ? 0 : first + 1;
            if (instr->r) // L-bit
            {
                // FMRRS
                snprintf(mBuf, mBufSize, "%s%s", "mfrrs", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s, %s, {%s, %s}", regStr(instr->Fd, true), regStr(instr->Fn, true),
                    singleRegStr(first, false), singleRegStr(next, false));
            } else {
                // FMSRR
                snprintf(mBuf, mBufSize, "%s%s", "mfsrr", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "{%s, %s}, %s, %s", singleRegStr(first, true), singleRegStr(next, true),
                    regStr(instr->Fd, false), regStr(instr->Fn, false));
            }
        }
    } else
        constant32((int32_t *)instrPtr, mBuf, mBufSize, iBuf, iBufSize);
}
#endif

void loadStoreCoprocessor(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    CoprocessorLoadStoreInstruction *instr = (CoprocessorLoadStoreInstruction *)instrPtr;
#ifdef __VFP_FP__
    const size_t tempStrSize = 32;
    char tempStr[tempStrSize] = "";
    uint32_t puw = (instr->P << 2 | instr->U << 1 | instr->W);
    bool isDouble = (instr->cp == 11); // Coprocessor 11 is double, 10 is float
    VFPInstruction *vfpInstr = (VFPInstruction *)instrPtr;
    if (puw == 0) {
        vfpTwoRegTransfer(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
        return;
    }

    switch (puw) {
        case 4:
        case 6:
            // FSTS/FSTD/FLDS/FLDD: U=0 negative offset
            snprintf(mBuf, mBufSize, "%s%s%s", instr->L ? "fld" : "fst", condName[instr->cond], isDouble ? "d" : "s");
            if (instr->offset8)
                snprintf(tempStr, tempStrSize, ", #%s%d", (instr->U) ? "+" : "-", instr->offset8 * 4);
            snprintf(iBuf, iBufSize, "%s, [%s%s]",
                isDouble ? doubleRegStr(getVFPDestination(vfpInstr), instr->L)
                         : singleRegStr(getVFPDestination(vfpInstr), instr->L),
                regStr(instr->Rn, false), tempStr);
            break;
        case 2:
        case 3:
        case 5: {
            // Unindexed(010) - FSTMS/FSTMD/FSTMX/FLDMS/FLDMD/FLDMX
            // Increment(011) - FSTMS/FSTMD/FSTMX/FLDMS/FLDMD/FLDMX
            // Decrement(101) - FSTMS/FSTMD/FSTMX/FLDMS/FLDMD/FLDMX
            uint32_t start = (instr->CR << 1 | instr->N);
            if (isDouble) {
                snprintf(mBuf, mBufSize, "%s%s%s%s", instr->L ? "fldm" : "fstm", (puw != 5) ? "ia" : "db",
                    (instr->offset8 % 2 == 0) ? "d" : "x", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s%s {%s%s%s}",
                    (puw == 2) ? regStr(instr->Rn, false) : regStr(instr->Rn, true), (puw != 2) ? "!," : ",",
                    singleRegStr(start, false), (instr->offset8 >= 2 * 2) ? ".." : "",
                    (instr->offset8 >= 2 * 2) ? singleRegStr(start + (instr->offset8 >> 1), false) : "");
            } else {
                snprintf(mBuf, mBufSize, "%s%s%s", instr->L ? "fldm" : "fstm", (puw != 5) ? "ias" : "dbs",
                    condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s%s {%s%s%s}",
                    (puw == 2) ? regStr(instr->Rn, false) : regStr(instr->Rn, true), (puw != 2) ? "!," : ",",
                    singleRegStr(start, false), (instr->offset8 >= 2 * 2) ? ".." : "",
                    (instr->offset8 >= 2 * 2) ? singleRegStr(start + instr->offset8, false) : "");
            }
            break;
        }
        case 1:
        case 7:
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
            // Case 0 is the two-register transfer instruction, captured at the beginning
    }
#else
    snprintf(mBuf, mBufSize, "%s%s%s", instr->L ? "ldc" : "stc", condName[instr->cond], instr->N ? "l" : "");
    snprintf(iBuf, iBufSize, "p%d, CR%d, [%s%s %s%s%d%s%s%s", instr->cp, instr->CR, regStr(instr->Rn, instr->W),
        instr->P ? "," : "],", (instr->P || instr->W) ? "#" : "{", (instr->P || instr->W) && !instr->U ? "-" : "",
        (instr->P || instr->W) ? 4 * instr->offset8 : instr->offset8, instr->P ? "]" : "",
        (instr->P && instr->W) ? "!" : "", (instr->P || instr->W) ? "" : "}");
#endif
}

#ifdef __VFP_FP__
void vfpExtension(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    VFPInstruction *instr = (VFPInstruction *)instrPtr;
    bool isDouble = (instr->cp == 11); // Coprocessor 11 is double, 10 is float
    switch (instr->Fn) {
        case 0:
            // FABSS/FABSD or FCPYS/FCPYD
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "fabs" : "fcpy", isDouble ? "d" : "s", condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s",
                isDouble ? doubleRegStr(getVFPDestination(instr), true) : singleRegStr(getVFPDestination(instr), true),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 1:
            // FNEGS/FNEGD or FSQRTS/FSQRTD
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "fsqrt" : "fneg", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s",
                isDouble ? doubleRegStr(getVFPDestination(instr), true) : singleRegStr(getVFPDestination(instr), true),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 2:
        case 3:
        case 6:
        case 9:
        case 10:
        case 11:
        case 14:
        case 15:
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
        case 4:
            // FCMPS/FCMPD or FCMPES/FCMPED
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "fcmpe" : "fcmp", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s",
                isDouble ? doubleRegStr(getVFPDestination(instr), false)
                         : singleRegStr(getVFPDestination(instr), false),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 5:
            // FCMPS/FCMPD or FCMPES/FCMPED
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "fcmpe" : "fcmp", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s",
                isDouble ? doubleRegStr(getVFPDestination(instr), false)
                         : singleRegStr(getVFPDestination(instr), false),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 7:
            if (instr->N) {
                // FCVTDS/FCVTSD
                snprintf(mBuf, mBufSize, "%s%s", isDouble ? "fcvtsd" : "fcvtds", condName[instr->cond]);
                snprintf(iBuf, iBufSize, "%s, %s",
                    isDouble ? singleRegStr(getVFPDestination(instr), true)
                             : singleRegStr(getVFPDestination(instr), true),
                    isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                             : singleRegStr(getVFPSecondOperand(instr), false));
            } else
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;
        case 8:
            // FUITOS/FUITOD or FSITOS/FSITOD
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "fsito" : "fuito", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s",
                isDouble ? doubleRegStr(getVFPDestination(instr), true) : singleRegStr(getVFPDestination(instr), true),
                isDouble ? singleRegStr(getVFPSecondOperand(instr), false)
                         : doubleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 12:
            // FTOUIS/FTOUID or FTOUIZS/FTOUIZD
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "ftouiz" : "ftoui", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s", singleRegStr(getVFPDestination(instr), true),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
        case 13:
            // FTOSIS/FTOSID or FTOSIZS/FTOSIZD
            snprintf(mBuf, mBufSize, "%s%s%s", instr->N ? "ftosiz" : "ftosi", isDouble ? "d" : "s",
                condName[instr->cond]);
            snprintf(iBuf, iBufSize, "%s, %s", singleRegStr(getVFPDestination(instr), true),
                isDouble ? doubleRegStr(getVFPSecondOperand(instr), false)
                         : singleRegStr(getVFPSecondOperand(instr), false));
            break;
    }
}

void vfpSingleRegTransfer(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    VFPInstruction *instr = (VFPInstruction *)instrPtr;
    uint32_t opCode = (uint32_t)(instr->p << 2 | instr->D << 1 | instr->q);
    bool isDouble = (instr->cp == 11); // Coprocessor 11 is double, 10 is float
    if (opCode == 0) {
        snprintf(mBuf, mBufSize, "%s%s", isDouble ? (instr->r ? "fmrs" : "fmsr") : (instr->r ? "fmrdl" : "fmdlr"),
            condName[instr->cond]);
        if (isDouble) {
            if (instr->r) // L-bit
                snprintf(iBuf, iBufSize, "%s, %s[31:0]", regStr(instr->Fd, true),
                    doubleRegStr(getVFPFirstOperand(instr), false));
            else
                snprintf(iBuf, iBufSize, "%s[31:0], %s", doubleRegStr(getVFPFirstOperand(instr), true),
                    regStr(instr->Fd, false));
        } else {
            if (instr->r) // L-bit
                snprintf(iBuf, iBufSize, "%s, %s", regStr(instr->Fd, true),
                    singleRegStr(getVFPFirstOperand(instr), false));
            else
                snprintf(iBuf, iBufSize, "%s, %s", singleRegStr(getVFPFirstOperand(instr), true),
                    regStr(instr->Fd, false));
        }
    } else if (opCode == 1) {
        if (isDouble) {
            snprintf(mBuf, mBufSize, "%s%s", instr->r ? "fmrdh" : "fmdhr", condName[instr->cond]);
            if (instr->r) // L-bit
                snprintf(iBuf, iBufSize, "%s, %s[63:32]", regStr(instr->Fd, true),
                    doubleRegStr(getVFPFirstOperand(instr), false));
            else
                snprintf(iBuf, iBufSize, "%s[63:32], %s", doubleRegStr(getVFPFirstOperand(instr), true),
                    regStr(instr->Fd, false));
        } else
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid condition
    } else if (opCode == 7) {
        if (!isDouble) {
            snprintf(mBuf, mBufSize, "%s%s", instr->r ? "fmrx" : "fmxr", condName[instr->cond]);
            if (instr->r) // L-bit
                snprintf(iBuf, iBufSize, "%s, %%s", regStr(instr->Fd, true),
                    ((instr->Fn << 1 | instr->N) == 0)       ? "fpsid"
                        : ((instr->Fn << 1 | instr->N) == 1) ? "fpscr"
                                                             : "fpexc");
            else
                snprintf(iBuf, iBufSize, "%s, %s",
                    ((instr->Fn << 1 | instr->N) == 0) ? "fpsid"
                                                       : (((instr->Fn << 1 | instr->N) == 1) ? "fpscr" : "fpexc"),
                    regStr(instr->Fd, false));
        } else
            constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid condition
    } else
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid condition
}

void vfpDataProcessing(int32_t *instrPtr, char *mBuf, size_t mBufSize, char *iBuf, size_t iBufSize)
{
    VFPInstruction *instr = (VFPInstruction *)instrPtr;
    uint32_t opCode = (uint32_t)(instr->p << 3 | instr->q << 2 | instr->r << 1 | instr->s);
    bool isDouble = (instr->cp == 11); // Coprocessor 11 is double, 10 is float

    if (instr->xfer) {
        // Single Register Transfer Instruction
        vfpSingleRegTransfer(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
        return;
    }

    if (isDouble && (instr->D || instr->M)) {
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid condition
        return;
    }

    if (opCode == 15)
        // Extension instruction
        vfpExtension(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
    else if (opCode >= 9 && opCode <= 14)
        // Undefined
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
    else {
        switch (opCode) {
            case 0:
                // FMACS/FMACD
                snprintf(mBuf, mBufSize, "fmac%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 1:
                // FNMACS/FNMACD
                snprintf(mBuf, mBufSize, "fnmac%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 2:
                // FMSCS/FMSCD
                snprintf(mBuf, mBufSize, "fmsc%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 3:
                // FNMSCS/FNMSCD
                snprintf(mBuf, mBufSize, "fnmsc%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 4:
                // FMULS/FMULD
                snprintf(mBuf, mBufSize, "fmul%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 5:
                // FNMULS/FNMULD
                snprintf(mBuf, mBufSize, "fnmul%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 6:
                // FADDS/FADDD
                snprintf(mBuf, mBufSize, "fadd%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 7:
                // FSUBS/FSUBD
                snprintf(mBuf, mBufSize, "fsub%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
            case 8:
                // FDIVS/FDIVD
                snprintf(mBuf, mBufSize, "fdiv%s%s", isDouble ? "d" : "s", condName[instr->cond]);
                break;
        }
        snprintf(iBuf, iBufSize, "%s, %s, %s",
            isDouble ? doubleRegStr(getVFPDestination(instr), true) : singleRegStr(getVFPDestination(instr), true),
            isDouble ? singleRegStr(getVFPFirstOperand(instr), false) : doubleRegStr(getVFPFirstOperand(instr), false),
            isDouble ? singleRegStr(getVFPSecondOperand(instr), false)
                     : doubleRegStr(getVFPSecondOperand(instr), false));
    }
}
#endif

void disassemble(int32_t *instrPtr, char *mBuf, char *iBuf)
{
    const size_t mBufSize = MIN_mbuffer;
    const size_t iBufSize = MIN_ibuffer;

    ARMInstruction *instr = (ARMInstruction *)instrPtr;
    if (instr->cond == 15) {
        constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Invalid condition
        return;
    }

    InstructionBits *instrBits = (InstructionBits *)instrPtr;
    switch (instr->groupOpcode) {
        case 0:
            if (instrBits->bit4 && instrBits->bit7)
                mulAndExtraLoadStore(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            else if (!instrBits->bit20 && instrBits->bits23_24 == 2)
                miscInstr(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            else
                dataProcessing(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;

        case 1:
            if (instrBits->bit20 || instrBits->bits23_24 != 2)
                dataProcessing(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            else if (instrBits->bit21)
                moveStatusReg(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            else
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Undefined instruction
            break;

        case 2:
            loadStore(instrPtr, mBuf, mBufSize, iBuf, iBufSize, false);
            break;

        case 3:
            if (!instrBits->bit4)
                loadStore(instrPtr, mBuf, mBufSize, iBuf, iBufSize, false);
            else
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Undefined instruction
            break;

        case 4:
            loadStoreMultiple(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;

        case 5:
            branch(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;

        case 6:
            loadStoreCoprocessor(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
            break;

        case 7:
            if (instrBits->bits23_24 > 1) // Check bit24 only: 0b10 or 0b11
            {
                snprintf(mBuf, mBufSize, "swi");
                snprintf(iBuf, iBufSize, "%u", (*instrPtr) & 0x00FFFFFF);
            } else {
#ifdef __VFP_FP__
                vfpDataProcessing(instrPtr, mBuf, mBufSize, iBuf, iBufSize);
#else
                constant32(instrPtr, mBuf, mBufSize, iBuf, iBufSize); // Coprocessor instruction
#endif
            }
    }
}
#endif

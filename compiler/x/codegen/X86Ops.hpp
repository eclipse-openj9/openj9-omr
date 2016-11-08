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

#ifndef X86OPS_INCL
#define X86OPS_INCL

#include <stdint.h>          // for uint32_t, uint8_t
#include "infra/Assert.hpp"  // for TR_ASSERT

namespace TR { class CodeGenerator; }
namespace TR { class Register; }

typedef enum {
   BADIA32Op,       // Illegal Opcode
   ADC1AccImm1,     // Add byte with Carry   AL, Immediate byte
   ADC2AccImm2,     // Add word with Carry   AX, Immediate word
   ADC4AccImm4,     // Add dword with Carry EAX, Immediate dword
   ADC8AccImm4,     // Add qword with Carry RAX, Immediate sign-extended dword (AMD64)
   ADC1RegImm1,     // Add byte with Carry  Register, Immediate byte
   ADC2RegImm2,     // Add word with Carry  Register, Immediate word
   ADC2RegImms,     // Add word with Carry  Register, Immediate sign-extended byte
   ADC4RegImm4,     // Add dword with Carry Register, Immediate dword
   ADC8RegImm4,     // Add qword with Carry Register, Immediate sign-extended dword (AMD64)
   ADC4RegImms,     // Add dword with Carry Register, Immediate sign-extended byte
   ADC8RegImms,     // Add qword with Carry Register, Immediate sign-extended byte (AMD64)
   ADC1MemImm1,     // Add byte with Carry  Memory, Immediate byte
   ADC2MemImm2,     // Add word with Carry  Memory, Immediate word
   ADC2MemImms,     // Add word with Carry  Memory, Immediate sign extended byte
   ADC4MemImm4,     // Add dword with Carry Memory, Immediate dword
   ADC8MemImm4,     // Add qword with Carry Memory, Immediate sign-extended dword (AMD64)
   ADC4MemImms,     // Add dword with Carry Memory, Immediate sign extended byte
   ADC8MemImms,     // Add qword with Carry Memory, Immediate sign extended byte (AMD64)
   ADC1RegReg,      // Add byte with Carry  Register, Register
   ADC2RegReg,      // Add word with Carry  Register, Register
   ADC4RegReg,      // Add dword with Carry Register, Register
   ADC8RegReg,      // Add qword with Carry Register, Register (AMD64)
   ADC1RegMem,      // Add byte with Carry  Register, Memory
   ADC2RegMem,      // Add word with Carry  Register, Memory
   ADC4RegMem,      // Add dword with Carry Register, Memory
   ADC8RegMem,      // Add qword with Carry Register, Memory (AMD64)
   ADC1MemReg,      // Add byte with Carry  Memory, Register
   ADC2MemReg,      // Add word with Carry  Memory, Register
   ADC4MemReg,      // Add dword with Carry Memory, Register
   ADC8MemReg,      // Add qword with Carry Memory, Register (AMD64)
   ADD1AccImm1,     // Add byte   AL, Immediate byte
   ADD2AccImm2,     // Add word   AX, Immediate word
   ADD4AccImm4,     // Add dword EAX, Immediate dword
   ADD8AccImm4,     // Add qword RAX, Immediate sign-extended dword (AMD64)
   ADD1RegImm1,     // Add byte  Register, Immediate byte
   ADD2RegImm2,     // Add word  Register, Immediate word
   ADD2RegImms,     // Add word  Register, Immediate sign-extended byte
   ADD4RegImm4,     // Add dword Register, Immediate dword
   ADD8RegImm4,     // Add qword Register, Immediate sign-extended dword (AMD64)
   ADD4RegImms,     // Add dword Register, Immediate sign-extended byte
   ADD8RegImms,     // Add qword Register, Immediate sign-extended byte (AMD64)
   ADD1MemImm1,     // Add byte  Memory, Immediate byte
   ADD2MemImm2,     // Add word  Memory, Immediate word
   ADD2MemImms,     // Add word  Memory, Immediate sign extended byte
   ADD4MemImm4,     // Add dword Memory, Immediate dword
   ADD8MemImm4,     // Add qword Memory, Immediate sign-extended dword (AMD64)
   ADD4MemImms,     // Add dword Memory, Immediate sign extended byte
   ADD8MemImms,     // Add qword Memory, Immediate sign extended byte (AMD64)
   ADD1RegReg,      // Add byte  Register, Register
   ADD2RegReg,      // Add word  Register, Register
   ADD4RegReg,      // Add dword Register, Register
   ADD8RegReg,      // Add qword Register, Register (AMD64)
   ADD1RegMem,      // Add byte  Register, Memory
   ADD2RegMem,      // Add word  Register, Memory
   ADD4RegMem,      // Add dword Register, Memory
   ADD8RegMem,      // Add qword Register, Memory (AMD64)
   ADD1MemReg,      // Add byte  Memory, Register
   ADD2MemReg,      // Add word  Memory, Register
   ADD4MemReg,      // Add dword Memory, Register
   ADD8MemReg,      // Add qword Memory, Register (AMD64)
   ADDSSRegReg,     // Add Scalar Single-FP Register, Register
   ADDSSRegMem,     // Add Scalar Single-FP Register, Memory
   ADDPSRegReg,     // Add Packed Single-FP Register, Register
   ADDPSRegMem,     // Add Packed Single-FP Register, Memory
   ADDSDRegReg,     // Add Scalar Double-FP Register, Register
   ADDSDRegMem,     // Add Scalar Double-FP Register, Memory
   ADDPDRegReg,     // Add Packed Double-FP Register, Register
   ADDPDRegMem,     // Add Packed Double-FP Register, Memory
   LADD1MemReg,     // Locked Add byte  Memory, Register
   LADD2MemReg,     // Locked Add word  Memory, Register
   LADD4MemReg,     // Locked Add dword Memory, Register
   LADD8MemReg,     // Locked Add qword Memory, Register (AMD64)
   LXADD1MemReg,    // Locked Exchange and Add byte  Memory, Register
   LXADD2MemReg,    // Locked Exchange and Add word  Memory, Register
   LXADD4MemReg,    // Locked Exchange and Add dword Memory, Register
   LXADD8MemReg,    // Locked Exchange and Add qword Memory, Register (AMD64)
   AND1AccImm1,     // Logical And byte   AL, Immediate byte
   AND2AccImm2,     // Logical And word   AX, Immediate word
   AND4AccImm4,     // Logical And dword EAX, Immediate dword
   AND8AccImm4,     // Logical And qword RAX, Immediate sign-extended dword (AMD64)
   AND1RegImm1,     // Logical And byte  Register, Immediate byte
   AND2RegImm2,     // Logical And word  Register, Immediate word
   AND2RegImms,     // Logical And word  Register, Immediate sign-extended byte
   AND4RegImm4,     // Logical And dword Register, Immediate dword
   AND8RegImm4,     // Logical And qword Register, Immediate sign-extended dword (AMD64)
   AND4RegImms,     // Logical And dword Register, Immediate sign-extended byte
   AND8RegImms,     // Logical And qword Register, Immediate sign-extended byte (AMD64)
   AND1MemImm1,     // Logical And byte  Memory, Immediate byte
   AND2MemImm2,     // Logical And word  Memory, Immediate word
   AND2MemImms,     // Logical And word  Memory, Immediate sign extended byte
   AND4MemImm4,     // Logical And dword Memory, Immediate dword
   AND8MemImm4,     // Logical And qword Memory, Immediate sign-extended dword (AMD64)
   AND4MemImms,     // Logical And dword Memory, Immediate sign extended byte
   AND8MemImms,     // Logical And qword Memory, Immediate sign extended byte (AMD64)
   AND1RegReg,      // Logical And byte  Register, Register
   AND2RegReg,      // Logical And word  Register, Register
   AND4RegReg,      // Logical And dword Register, Register
   AND8RegReg,      // Logical And qword Register, Register (AMD64)
   AND1RegMem,      // Logical And byte  Register, Memory
   AND2RegMem,      // Logical And word  Register, Memory
   AND4RegMem,      // Logical And dword Register, Memory
   AND8RegMem,      // Logical And qword Register, Memory (AMD64)
   AND1MemReg,      // Logical And byte  Memory, Register
   AND2MemReg,      // Logical And word  Memory, Register
   AND4MemReg,      // Logical And dword Memory, Register
   AND8MemReg,      // Logical And qword Memory, Register (AMD64)
   BSF2RegReg,      // Bit scan forward to find the least significant 1 bit
   BSF4RegReg,      //
   BSF8RegReg,
   BSR4RegReg,		// Bit scan forward to find the least significant 1 bit
   BSR8RegReg,
   BSWAP4Reg,       // Byte swap Register 32
   BSWAP8Reg,       // Byte swap Register 64 (AMD64)
   BTS4RegReg,      // Bit Test and Set dword Register, Register
   BTS4MemReg,      // Bit Test and Set dword Memory, Register
   CALLImm4,        // Call near, displacement relative to next instruction
   CALLREXImm4,     // Call near, displacement relative to next instruction, force Rex prefix (AMD64)
   CALLReg,         // Call near, indirect through register
   CALLREXReg,      // Call near, indirect through register, force Rex prefix (AMD64)
   CALLMem,         // Call near, indirect through Memory
   CALLREXMem,      // Call near, indirect through Memory, force Rex prefix (AMD64)
   CBWAcc,          // Sign extend AL into AX
   CBWEAcc,         // Sign extend AX into EAX
   CMOVA4RegMem,    // Conditional dword move if above (unsigned greater than) (CF=0 and ZF=0)
   CMOVB4RegMem,    // Conditional dword move if below (unsigned less than) (CF=1)
   CMOVE4RegMem,    // Conditional dword move if equal (ZF=1)
   CMOVE8RegMem,    // Conditional qword move if equal (ZF=1)
   CMOVG4RegMem,    // Conditional dword move if greater (ZF=0 and SF=OF)
   CMOVL4RegMem,    // Conditional dword move if less (SF!=OF)
   CMOVNE4RegMem,   // Conditional dword move if not equal (ZF=0)
   CMOVNE8RegMem,   // Conditional qword move if not equal (ZF=0)
   CMOVNO4RegMem,   // Conditional dword move if no overflow (OF=0)
   CMOVNS4RegMem,   // Conditional dword move if positive (SF=0)
   CMOVO4RegMem,    // Conditional dword move if overflow (OF=1)
   CMOVS4RegMem,    // Conditional dword move if negative (SF=1)
   CMP1AccImm1,     // Compare byte   AL, Immediate byte
   CMP2AccImm2,     // Compare word   AX, Immediate word
   CMP4AccImm4,     // Compare dword EAX, Immediate dword
   CMP8AccImm4,     // Compare qword RAX, Immediate sign-extended dword (AMD64)
   CMP1RegImm1,     // Compare byte  Register, Immediate byte
   CMP2RegImm2,     // Compare word  Register, Immediate word
   CMP2RegImms,     // Compare word  Register, Immediate sign-extended byte
   CMP4RegImm4,     // Compare dword Register, Immediate dword
   CMP8RegImm4,     // Compare qword Register, Immediate sign-extended dword (AMD64)
   CMP4RegImms,     // Compare dword Register, Immediate sign-extended byte
   CMP8RegImms,     // Compare qword Register, Immediate sign-extended byte (AMD64)
   CMP1MemImm1,     // Compare byte  Memory, Immediate byte
   CMP2MemImm2,     // Compare word  Memory, Immediate word
   CMP2MemImms,     // Compare word  Memory, Immediate sign extended byte
   CMP4MemImm4,     // Compare dword Memory, Immediate dword
   CMP8MemImm4,     // Compare qword Memory, Immediate sign-extended dword (AMD64)
   CMP4MemImms,     // Compare dword Memory, Immediate sign extended byte
   CMP8MemImms,     // Compare qword Memory, Immediate sign extended byte (AMD64)
   CMP1RegReg,      // Compare byte  Register, Register
   CMP2RegReg,      // Compare word  Register, Register
   CMP4RegReg,      // Compare dword Register, Register
   CMP8RegReg,      // Compare qword Register, Register (AMD64)
   CMP1RegMem,      // Compare byte  Register, Memory
   CMP2RegMem,      // Compare word  Register, Memory
   CMP4RegMem,      // Compare dword Register, Memory
   CMP8RegMem,      // Compare qword Register, Memory (AMD64)
   CMP1MemReg,      // Compare byte  Memory, Register
   CMP2MemReg,      // Compare word  Memory, Register
   CMP4MemReg,      // Compare dword Memory, Register
   CMP8MemReg,      // Compare qword Memory, Register (AMD64)
   CMPXCHG1MemReg,  // Compare and Exchange byte  Memory, Register
   CMPXCHG2MemReg,  // Compare and Exchange word  Memory, Register
   CMPXCHG4MemReg,  // Compare and Exchange dword Memory, Register
   CMPXCHG8MemReg,  // Compare and Exchange qword Memory, Register (AMD64)
   CMPXCHG8BMem,    // Compare and Exchange qword Memory with EDX:EAX and ECX:EBX register pairs (IA32 only)
   CMPXCHG16BMem,   // Compare and Exchange qword Memory with RDX:RAX and RCX:RBX register pairs (AMD64)
   LCMPXCHG1MemReg, // Locked Compare and Exchange byte  Memory, Register
   LCMPXCHG2MemReg, // Locked Compare and Exchange word  Memory, Register
   LCMPXCHG4MemReg, // Locked Compare and Exchange dword Memory, Register
   LCMPXCHG8MemReg, // Locked Compare and Exchange qword Memory, Register (AMD64)
   LCMPXCHG8BMem,   // Locked Compare and Exchange qword Memory with EDX:EAX and ECX:EBX register pairs (IA32 only)
   LCMPXCHG16BMem,  // Locked Compare and Exchange qword Memory with RDX:RAX and RCX:RBX register pairs (AMD64)
   XALCMPXCHG8MemReg, // Xacquire Locked Compare and Exchange qword Memory, Register (AMD64)
   XACMPXCHG8MemReg,  // Xacquire Compare and Exchange qword Memory, Register (AMD64)
   XALCMPXCHG4MemReg, // Xacquire Locked Compare and Exchange dword Memory, Register
   XACMPXCHG4MemReg,  // Xacquire Compare and Exchange dword Memory, Register
   CVTSI2SSRegReg4, // Signed INT32 to Scalar Single-FP Conversion  Register, Register
   CVTSI2SSRegReg8, // Signed INT64 to Scalar Single-FP Conversion  Register, Register (AMD64)
   CVTSI2SSRegMem,  // Signed INT32 to Scalar Single-FP Conversion  Register, Memory TODO:AMD64: "...Mem4"
   CVTSI2SSRegMem8, // Signed INT64 to Scalar Single-FP Conversion  Register, Memory (AMD64)
   CVTSI2SDRegReg4, // Signed INT32 to Scalar Double-FP Conversion  Register, Register
   CVTSI2SDRegReg8, // Signed INT64 to Scalar Double-FP Conversion  Register, Register (AMD64)
   CVTSI2SDRegMem,  // Signed INT32 to Scalar Double-FP Conversion  Register, Memory TODO:AMD64: "...Mem4"
   CVTSI2SDRegMem8, // Signed INT64 to Scalar Double-FP Conversion  Register, Memory (AMD64)
   CVTTSS2SIReg4Reg,// Scalar Single-FP to Signed INT32 Conversion (Truncate)  Register, Register
   CVTTSS2SIReg8Reg,// Scalar Single-FP to Signed INT64 Conversion (Truncate)  Register, Register (AMD64)
   CVTTSS2SIReg4Mem,// Scalar Single-FP to Signed INT32 Conversion (Truncate)  Register, Memory
   CVTTSS2SIReg8Mem,// Scalar Single-FP to Signed INT64 Conversion (Truncate)  Register, Memory (AMD64)
   CVTTSD2SIReg4Reg,// Scalar Double-FP to Signed INT32 Conversion (Truncate)  Register, Register
   CVTTSD2SIReg8Reg,// Scalar Double-FP to Signed INT64 Conversion (Truncate)  Register, Register (AMD64)
   CVTTSD2SIReg4Mem,// Scalar Double-FP to Signed INT32 Conversion (Truncate)  Register, Memory
   CVTTSD2SIReg8Mem,// Scalar Double-FP to Signed INT64 Conversion (Truncate)  Register, Memory (AMD64)
   CVTSS2SDRegReg,  // Scalar Single-FP to Scalar Double-FP Conversion  Register, Register
   CVTSS2SDRegMem,  // Scalar Single-FP to Scalar Double-FP Conversion  Register, Memory
   CVTSD2SSRegReg,  // Scalar Double-FP to Scalar Single-FP Conversion  Register, Register
   CVTSD2SSRegMem,  // Scalar Double-FP to Scalar Single-FP Conversion  Register, Memory
   CWDAcc,          // Sign-extend AX into DX:AX
   CDQAcc,          // Sign-extend EAX into EDX:EAX
   CQOAcc,          // Sign-extend RAX into RDX:RAX (AMD64)
   DEC1Reg,         // Decrement byte  Register
   DEC2Reg,         // Decrement word  Register
   DEC4Reg,         // Decrement dword Register
   DEC8Reg,         // Decrement qword Register (AMD64)
   DEC1Mem,         // Decrement byte  Memory
   DEC2Mem,         // Decrement word  Memory
   DEC4Mem,         // Decrement dword Memory
   DEC8Mem,         // Decrement qword Memory (AMD64)
   FABSReg,         // Float Absolute Value Register
   DABSReg,         // Double Absolute Value Register
   FSQRTReg,        // Float SQRT Value Register
   DSQRTReg,        // Double SQRT Value Register
   FADDRegReg,      // Float Add Register, Register
   DADDRegReg,      // Double Add Register, Register
   FADDPReg,        // Float/Double Add and Pop Register
   FADDRegMem,      // Float Add Register, Memory
   DADDRegMem,      // Double Add Register, Memory
   FIADDRegMem,     // Float + 32-bit Integer fRegister, Memory dword
   DIADDRegMem,     // Double + 32-bit Integer dRegister, Memory dword
   FSADDRegMem,     // Float + 16-bit Integer fRegister, Memory word
   DSADDRegMem,     // Double + 16-bit Integer dRegister, Memory word
   FCHSReg,         // Float change sign Register
   DCHSReg,         // Double change sign Register
   FDIVRegReg,      // Float Divide Register, Register
   DDIVRegReg,      // Double Divide Register, Register
   FDIVRegMem,      // Float Divide Register, Memory
   DDIVRegMem,      // Double Divide Register, Memory
   FDIVPReg,        // Float/Double Divide and Pop Register
   FIDIVRegMem,     // Float Divide by 32-bit integer fRegister, Memory dword
   DIDIVRegMem,     // Double Divide by 32-bit integer dRegister, Memory dword
   FSDIVRegMem,     // Float Divide by 16-bit integer fRegister, Memory word
   DSDIVRegMem,     // DoubleDivide by 16-bit integer dRegister, Memory word
   FDIVRRegReg,     // Float Reverse Divide Register, Register
   DDIVRRegReg,     // Double Reverse Divide Register, Register
   FDIVRRegMem,     // Float Reverse Divide Register, Memory
   DDIVRRegMem,     // Double Reverse Divide Register, Memory
   FDIVRPReg,       // Float/Double Reverse Divide and Pop Register
   FIDIVRRegMem,    // Float Reverse Divide by 32-bit integer fRegister, Memory dword
   DIDIVRRegMem,    // Double Reverse Divide by 32-bit integer dRegister, Memory dword
   FSDIVRRegMem,    // Float Reverse Divide by 16-bit integer fRegister, Memory word
   DSDIVRRegMem,    // Double Reverse Divide by 16-bit integer dRegister, Memory word
   FILDRegMem,      // Load 32-bit integer Memory dword and convert to Float Register
   DILDRegMem,      // Load 32-bit integer Memory dword and convert to Double Register
   FLLDRegMem,      // Load 64-bit integer Memory qword and convert to Float Register
   DLLDRegMem,      // Load 64-bit integer Memory qword and convert to Double Register
   FSLDRegMem,      // Load 16-bit integer Memory word and convert to Float Register
   DSLDRegMem,      // Load 16-bit integer Memory word and convert to Double Register
   FISTMemReg,      // Store float Register converted to 32-bit integer Memory dword
   DISTMemReg,      // Store double Register converted to 32-bit integer Memory dword
   FISTPMem,        // Store and Pop float Register converted to 32-bit integer Memory dword
   DISTPMem,        // Store and Pop double Register converted to 32-bit integer Memory dword
   FLSTPMem,        // Store and Pop float Register converted to 64-bit integer Memory qword
   DLSTPMem,        // Store and Pop double Register converted to 64-bit integer Memory qword
   FSSTMemReg,      // Store float Register converted to 16-bit integer Memory word
   DSSTMemReg,      // Store double Register converted to 16-bit integer Memory word
   FSSTPMem,        // Store and Pop float Register converted to 16-bit integer Memory word
   DSSTPMem,        // Store and Pop double Register converted to 16-bit integer Memory word
   FLDLN2,          // Load Double LOGe(2) to ST0
   FLDRegReg,       // Load Float Register from Float Register
   DLDRegReg,       // Load Double Register from Double Register
   FLDRegMem,       // Load Float Register from Float Memory
   DLDRegMem,       // Load Double Register from Double Memory
   FLD0Reg,         // Load Float 0.0 to Register
   DLD0Reg,         // Load Double 0.0 to Register
   FLD1Reg,         // Load Float 1.0 to Register
   DLD1Reg,         // Load Double 1.0 to Register
   FLDMem,          // Load Float from Memory to ST0
   DLDMem,          // Load Double from Memory to ST0
   LDCWMem,         // Load control word from Memory word
   FMULRegReg,      // Float Multiply Register, Register
   DMULRegReg,      // Double Multiply Register, Register
   FMULPReg,        // Float/Double Multiply and Pop Register
   FMULRegMem,      // Float Multiply Register, Memory
   DMULRegMem,      // Double Multiply Register, Memory
   FIMULRegMem,     // Float * 32-bit Integer fRegister, Memory dword
   DIMULRegMem,     // Double * 32-bit Integer dRegister, Memory dword
   FSMULRegMem,     // Float * 16-bit Integer fRegister, Memory word
   DSMULRegMem,     // Double * 16-bit Integer dRegister, Memory word
   FNCLEX,          // Clear FP exception flags without checking for pending unmasked exceptions
   FPREMRegReg,     // Float/Double Remainder of ST0/ST1
   FSCALERegReg,    // Float/Double Scale of ST0 by 2^ST1
   FSTMemReg,       // Store Float Register to Float Memory
   DSTMemReg,       // Store Double Register to Double Memory
   FSTRegReg,       // Store Float Register to Float Register
   DSTRegReg,       // Store Double Register to Double Register
   FSTPMemReg,      // Store Float Register to Float Memory and Pop
   DSTPMemReg,      // Store Double Register to Double Memory and Pop
   FSTPReg,         // Store Float Register to Float Register and Pop
   DSTPReg,         // Store Double Register to Float Register and Pop
   STCWMem,         // Store Control Word to Memory Word
   STSWMem,         // Store Status Word to Memory Word
   STSWAcc,         // Store Status Word to AX
   FSUBRegReg,      // Float Subtract Register, Register
   DSUBRegReg,      // Double Subtract Register, Register
   FSUBRegMem,      // Float Subtract Register, Memory
   DSUBRegMem,      // Double Subtract Register, Memory
   FSUBPReg,        // Float/Double Subtract and Pop Register
   FISUBRegMem,     // Float Subtract by 32-bit integer fRegister, Memory dword
   DISUBRegMem,     // Double Subtract by 32-bit integer dRegister, Memory dword
   FSSUBRegMem,     // Float Subtract by 16-bit integer fRegister, Memory word
   DSSUBRegMem,     // DoubleSubtract by 16-bit integer dRegister, Memory word
   FSUBRRegReg,     // Float Reverse Subtract Register, Register
   DSUBRRegReg,     // Double Reverse Subtract Register, Register
   FSUBRRegMem,     // Float Reverse Subtract Register, Memory
   DSUBRRegMem,     // Double Reverse Subtract Register, Memory
   FSUBRPReg,       // Float/Double Reverse Subtract and Pop Register
   FISUBRRegMem,    // Float Reverse Subtract by 32-bit integer fRegister, Memory dword
   DISUBRRegMem,    // Double Reverse Subtract by 32-bit integer dRegister, Memory dword
   FSSUBRRegMem,    // Float Reverse Subtract by 16-bit integer fRegister, Memory word
   DSSUBRRegMem,    // Double Reverse Subtract by 16-bit integer dRegister, Memory word
   FTSTReg,         // Float/Double Compare ST0 with 0.0
   FCOMRegReg,      // Float Compare Register, Register
   DCOMRegReg,      // Double Compare Register, Register
   FCOMRegMem,      // Float Compare Register, Memory
   DCOMRegMem,      // Double Compare Register, Memory
   FCOMPReg,        // Float/Double Compare Register, Register and Pop Register
   FCOMPMem,        // Float Compare Register, Register and Pop Register
   DCOMPMem,        // Double Compare Register, Register and Pop Register
   FCOMPP,          // Float/Double Compare Register, Register and Pop twice
   FCOMIRegReg,     // Float Compare Register, Register and set EFLAGS
   DCOMIRegReg,     // Double Compare Register, Register and set EFLAGS
   FCOMIPReg,       // Float/Double Compare Register-Register, Pop Register, and set EFLAGS
   FYL2X,           // Double Compute y * LOG2(x)
   UCOMISSRegReg,   // Unordered Compare Scalar Single-FP Register, Register and set EFLAGS
   UCOMISSRegMem,   // Unordered Compare Scalar Single-FP Register, Memory and set EFLAGS
   UCOMISDRegReg,   // Unordered Compare Scalar Double-FP Register, Register and set EFLAGS
   UCOMISDRegMem,   // Unordered Compare Scalar Double-FP Register, Memory and set EFLAGS
   FXCHReg,         // Float/Double exchange Register, Register
   IDIV1AccReg,     // Signed Divide word by byte AX, Register (AL=Quotient, AH=Remainder)
   IDIV2AccReg,     // Signed Divide dword by word DX:AX, Register (AX=Quotient, DX=Remainder)
   IDIV4AccReg,     // Signed Divide qword by word EDX:EAX, Register (EAX=Quotient, EDX=Remainder)
   IDIV8AccReg,     // Signed Divide qword by word RDX:RAX, Register (RAX=Quotient, RDX=Remainder) (AMD64)
   DIV4AccReg,      // Unsigned Divide qword by word EDX:EAX, Register (EAX=Quotient, EDX=Remainder)
   DIV8AccReg,      // Unsigned Divide qword by word RDX:RAX, Register (RAX=Quotient, RDX=Remainder) (AMD64)
   IDIV1AccMem,     // Signed Divide word by byte AX, Memory (AL=Quotient, AH=Remainder)
   IDIV2AccMem,     // Signed Divide dword by word DX:AX, Memory (AX=Quotient, DX=Remainder)
   IDIV4AccMem,     // Signed Divide qword by word EDX:EAX, Memory (EAX=Quotient, EDX=Remainder)
   IDIV8AccMem,     // Signed Divide qword by word RDX:RAX, Memory (RAX=Quotient, RDX=Remainder) (AMD64)
   DIV4AccMem,      // Signed Divide qword by word EDX:EAX, Memory (EAX=Quotient, EDX=Remainder)
   DIV8AccMem,      // Signed Divide qword by word RDX:RAX, Memory (RAX=Quotient, RDX=Remainder) (AMD64)
   DIVSSRegReg,     // Divide Scalar Single-FP Register, Register
   DIVSSRegMem,     // Divide Scalar Single-FP Register, Memory
   DIVSDRegReg,     // Divide Scalar Double-FP Register, Register
   DIVSDRegMem,     // Divide Scalar Double-FP Register, Memory
   IMUL1AccReg,     // Signed byte Multiply AX <- AL * Register
   IMUL2AccReg,     // Signed word Multiply DX:AX <- AX * Register
   IMUL4AccReg,     // Signed dword Multiply EDX:EAX <- EAX * Register
   IMUL8AccReg,     // Signed qword Multiply RDX:RAX <- RAX * Register (AMD64)
   IMUL1AccMem,     // Signed byte Multiply AX <- AL * Memory
   IMUL2AccMem,     // Signed word Multiply DX:AX <- AX * Memory
   IMUL4AccMem,     // Signed dword Multiply EDX:EAX <- EAX * Memory
   IMUL8AccMem,     // Signed qword Multiply RDX:RAX <- RAX * Memory (AMD64)
   IMUL2RegReg,     // Signed word Multiply Register, Register
   IMUL4RegReg,     // Signed dword Multiply Register, Register
   IMUL8RegReg,     // Signed qword Multiply Register, Register (AMD64)
   IMUL2RegMem,     // Signed word Multiply Register, Memory word
   IMUL4RegMem,     // Signed dword Multiply Register, Memory dword
   IMUL8RegMem,     // Signed qword Multiply Register, Memory qword (AMD64)
   IMUL2RegRegImm2, // Signed word Multiply Register1, Register2, Immediate word
   IMUL2RegRegImms, // Signed word Multiply Register1, Register2, Signed-Extended Immediate byte
   IMUL4RegRegImm4, // Signed dword Multiply Register1, Register2, Immediate dword
   IMUL8RegRegImm4, // Signed qword Multiply Register1, Register2, Immediate sign-extended dword (AMD64)
   IMUL4RegRegImms, // Signed dword Multiply Register1, Register2, Signed-Extended Immediate byte
   IMUL8RegRegImms, // Signed qword Multiply Register1, Register2, Signed-Extended Immediate byte (AMD64)
   IMUL2RegMemImm2, // Signed word Multiply Register, Memory word, Immediate word
   IMUL2RegMemImms, // Signed word Multiply Register, Memory word, Signed-Extended Immediate byte
   IMUL4RegMemImm4, // Signed dword Multiply Register, Memory dword, Immediate dword
   IMUL8RegMemImm4, // Signed qword Multiply Register, Memory qword, Immediate sign-extended dword (AMD64)
   IMUL4RegMemImms, // Signed dword Multiply Register, Memory dword, Signed-Extended Immediate byte
   IMUL8RegMemImms, // Signed qword Multiply Register, Memory qword, Signed-Extended Immediate byte (AMD64)
   MUL1AccReg,      // Unsigned byte Multiply AX <- AL * Register
   MUL2AccReg,      // Unsigned word Multiply DX:AX <- AX * Register
   MUL4AccReg,      // Unsigned dword Multiply EDX:EAX <- EAX * Register
   MUL8AccReg,      // Unsigned qword Multiply RDX:RAX <- RAX * Register (AMD64)
   MUL1AccMem,      // Unsigned byte Multiply AX <- AL * Memory
   MUL2AccMem,      // Unsigned word Multiply DX:AX <- AX * Memory
   MUL4AccMem,      // Unsigned dword Multiply EDX:EAX <- EAX * Memory
   MUL8AccMem,      // Unsigned qword Multiply RDX:RAX <- RAX * Memory (AMD64)
   MULSSRegReg,     // Multiply Scalar Single-FP Register, Register
   MULSSRegMem,     // Multiply Scalar Single-FP Register, Memory dword
   MULPSRegReg,     // Multiply Packed Single-FP Register, Register
   MULPSRegMem,     // Multiply Packed Single-FP Register, Memory dword
   MULSDRegReg,     // Multiply Scalar Double-FP Register, Register
   MULSDRegMem,     // Multiply Scalar Double-FP Register, Memory qword
   MULPDRegReg,     // Multiply Packed Double-FP Register, Register
   MULPDRegMem,     // Multiply Packed Double-FP Register, Memory qword
   INC1Reg,         // Increment byte Register
   INC2Reg,         // Increment word Register
   INC4Reg,         // Increment dword Register
   INC8Reg,         // Increment qword Register (AMD64)
   INC1Mem,         // Increment byte Memory
   INC2Mem,         // Increment word Memory
   INC4Mem,         // Increment dword Memory
   INC8Mem,         // Increment qword Memory (AMD64)
   JA1,             // Jump Relative byte if above (CF==0 && ZF==0)
   JAE1,            // Jump Relative byte if above or equal (CF==0)
   JB1,             // Jump Relative byte if below (CF==1)
   JBE1,            // Jump Relative byte if below or equal (CF==1 || ZF==1)
   JE1,             // Jump Relative byte if equal (ZF==1)
   JNE1,            // Jump Relative byte if not equal (ZF==0)
   JG1,             // Jump Relative byte if greater than  (ZF==0 && SF==OF)
   JGE1,            // Jump Relative byte if greater than or equal (SF==OF)
   JL1,             // Jump Relative byte if less than (SF!=OF)
   JLE1,            // Jump Relative byte if less than or equal (ZF==1 || SF!=OF)
   JO1,             // Jump Relative byte if overflow (OF=1)
   JNO1,            // Jump Relative byte if not overflow (OF=0)
   JS1,             // Jump Relative byte if signed (SF==1)
   JNS1,            // Jump Relative byte if not signed (SF==0)
   JPO1,            // Jump Relative byte if odd parity (PF==0)
   JPE1,            // Jump Relative byte if even parity (PF==1)
   JMP1,            // Jump Unconditional Relative byte
   JA4,             // Jump Relative dword if above (CF==0 && ZF==0)
   JAE4,            // Jump Relative dword if above or equal (CF==0)
   JB4,             // Jump Relative dword if below (CF==1)
   JBE4,            // Jump Relative dword if below or equal (CF==1 || ZF==1)
   JE4,             // Jump Relative dword if equal (ZF==1)
   JNE4,            // Jump Relative dword if not equal (ZF==0)
   JG4,             // Jump Relative dword if greater than  (ZF==0 && SF==OF)
   JGE4,            // Jump Relative dword if greater than or equal (SF==OF)
   JL4,             // Jump Relative dword if less than (SF!=OF)
   JLE4,            // Jump Relative dword if less than or equal (ZF==1 || SF!=OF)
   JO4,             // Jump Relative dword if overflow (OF=1)
   JNO4,            // Jump Relative dword if not overflow (OF=0)
   JS4,             // Jump Relative dword if signed (SF==1)
   JNS4,            // Jump Relative dword if not signed (SF==0)
   JPO4,            // Jump Relative dword if odd parity (PF==0)
   JPE4,            // Jump Relative dword if even parity (PF==1)
   JMP4,            // Jump Unconditional Relative dword
   JMPReg,          // Jump Indirect Unconditional Absolute Near dword through Register
   JMPMem,          // Jump Indirect Unconditional Absolute Near dword through Memory
   JRCXZ1,          // Jump Relative if RCX/ECX register is 0
   LOOP1,           // Loop according to RCX/ECX register
   LAHF,            // Load flags to AH; not on AMD64
   LDDQU,           // Load Unaligned Integer 128 Bits
   LEA2RegMem,      // Load word Register with Memory Address
   LEA4RegMem,      // Load dword Register with Memory Address
   LEA8RegMem,      // Load qword Register with Memory Address (AMD64)
   S1MemReg,        // Store byte Register to Memory byte
   S2MemReg,        // Store word Register to Memory word
   S4MemReg,        // Store dword Register to Memory dword
   S8MemReg,        // Store qword Register to Memory qword (AMD64)
   S1MemImm1,       // Store Immediate byte to Memory byte
   S2MemImm2,       // Store Immediate word to Memory word
   S4MemImm4,       // Store Immediate dword to Memory dword
   XRS4MemImm4,     // Xrelease Store Immediate dword to Memory dword
   S8MemImm4,       // Store Immediate sign-extended dword to Memory qword (AMD64)
   XRS8MemImm4,     // Xrelease Store Immediate sign-extended dword to Memory qword (AMD64)
   L1RegMem,        // Load byte Register from Memory byte
   L2RegMem,        // Load word Register from Memory word
   L4RegMem,        // Load dword Register from Memory dword
   L8RegMem,        // Load qword Register from Memory qword (AMD64)
   MOVAPSRegReg,    // Move Aligned Packed Single-FP Register, Register
   MOVAPSRegMem,    // Move Aligned Packed Single-FP Register, Memory
   MOVAPSMemReg,    // Move Aligned Packed Single-FP Memory, Register
   MOVAPDRegReg,    // Move Aligned Packed Double-FP Register, Register
   MOVAPDRegMem,    // Move Aligned Packed Double-FP Register, Memory
   MOVAPDMemReg,    // Move Aligned Packed Double-FP Memory, Register
   MOVUPSRegReg,    // Move Unaligned Packed Single-FP Register, Register
   MOVUPSRegMem,    // Move Unaligned Packed Single-FP Register, Memory
   MOVUPSMemReg,    // Move Unaligned Packed Single-FP Memory, Register
   MOVUPDRegReg,    // Move Unaligned Packed Double-FP Register, Register
   MOVUPDRegMem,    // Move Unaligned Packed Double-FP Register, Memory
   MOVUPDMemReg,    // Move Unaligned Packed Double-FP Memory, Register
   MOVSSRegReg,     // Move Scalar Single-FP Register, Register
   MOVSSRegMem,     // Move Scalar Single-FP Register, Memory
   MOVSSMemReg,     // Move Scalar Single-FP Memory, Register
   MOVSDRegReg,     // Move Scalar Double-FP Register, Register
   MOVSDRegMem,     // Move Scalar Double-FP Register, Memory
   MOVSDMemReg,     // Move Scalar Double-FP Memory, Register
   SQRTSFRegReg,    // SQRT Scalar Single-FP Register, Register
   SQRTSDRegReg,    // SQRT Scalar Double-FP Register, Register
   MOVDRegReg4,     // Move Doubleword XMM Register, Register
   MOVQRegReg8,     // Move Quadword XMM Register, Register (AMD64)
   MOVDReg4Reg,     // Move Doubleword Register, XMM Register
   MOVQReg8Reg,     // Move Quadword Register, XMM Register (AMD64)
   MOVDQURegReg,    // Move XMM Register, XMM Register
   MOVDQURegMem,    // Move XMM Register, Memory
   MOVDQUMemReg,    // Move Memory, XMM Register
   MOV1RegReg,      // Move byte Register, Register
   MOV2RegReg,      // Move word Register, Register
   MOV4RegReg,      // Move dword Register, Register
   MOV8RegReg,      // Move qword Register, Register (AMD64)
   CMOVG4RegReg,    // Conditional greater than move dword Register, Register
   CMOVG8RegReg,    // Conditional greater than move qword Register, Register (AMD64)
   CMOVL4RegReg,    // Conditional greater than move dword Register, Register
   CMOVL8RegReg,    // Conditional greater than move qword Register, Register (AMD64)
   CMOVE4RegReg,    // Conditional equal (zero) move dword Register, Register
   CMOVE8RegReg,    // Conditional equal (zero) move dword Register, Register (AMD64)
   CMOVNE4RegReg,   // Conditional not-equal (non-zero) move dword Register, Register
   CMOVNE8RegReg,   // Conditional not-equal (non-zero) move dword Register, Register (AMD64)
   MOV1RegImm1,     // Move Immediate byte to byte Register
   MOV2RegImm2,     // Move Immediate word to word Register
   MOV4RegImm4,     // Move Immediate dword to dword Register
   MOV8RegImm4,     // Move Immediate dword to qword Register (note: sign-extended) (AMD64)
   MOV8RegImm64,    // Move Immediate qword to qword Register (AMD64)
   MOVLPDRegMem,    // Load quadword into low order 64 bits of XMM reg (no zero extension to 128 bits)
   MOVLPDMemReg,    // Store low order 64 bits of XMM reg to quadword
   MOVQRegMem,      // Load quad XMM register from memory
   MOVQMemReg,      // Store quad XMM register to memory
   MOVSB,           // Move String Byte from [ESI] to [EDI]
   MOVSW,           // Move String Word from [ESI] to [EDI]
   MOVSD,           // Move String Dword from [ESI] to [EDI]
   MOVSQ,           // Move String Qword from [RSI] to [RDI] (AMD64)
   MOVSXReg2Reg1,   // Move Sign-Extended byte Register1 to word Register2
   MOVSXReg4Reg1,   // Move Sign-Extended byte Register1 to dword Register4
   MOVSXReg8Reg1,   // Move Sign-Extended byte Register1 to qword Register8 (AMD64)
   MOVSXReg4Reg2,   // Move Sign-Extended word Register2 to dword Register4
   MOVSXReg8Reg2,   // Move Sign-Extended word Register2 to qword Register8 (AMD64)
   MOVSXReg8Reg4,   // Move Sign-Extended dword Register4 to qword Register8 (AMD64)
   MOVSXReg2Mem1,   // Move Sign-Extended Memory1 byte to word Register2
   MOVSXReg4Mem1,   // Move Sign-Extended Memory1 byte to dword Register4
   MOVSXReg8Mem1,   // Move Sign-Extended Memory1 byte to qword Register8 (AMD64)
   MOVSXReg4Mem2,   // Move Sign-Extended Memory2 word to dword Register4
   MOVSXReg8Mem2,   // Move Sign-Extended Memory2 word to qword Register8 (AMD64)
   MOVSXReg8Mem4,   // Move Sign-Extended Memory4 dword to qword Register8 (AMD64)
   MOVZXReg2Reg1,   // Move Zero-Extended byte Register1 to word Register2
   MOVZXReg4Reg1,   // Move Zero-Extended byte Register1 to dword Register4
   MOVZXReg8Reg1,   // Move Zero-Extended byte Register1 to qword Register8 (AMD64)
   MOVZXReg4Reg2,   // Move Zero-Extended word Register2 to dword Register4
   MOVZXReg8Reg2,   // Move Zero-Extended word Register2 to qword Register8 (AMD64)
   MOVZXReg8Reg4,   // Move Zero-Extended word Register4 to qword Register8 (AMD64)
   MOVZXReg2Mem1,   // Move Zero-Extended Memory1 byte to word Register2
   MOVZXReg4Mem1,   // Move Zero-Extended Memory1 byte to dword Register4
   MOVZXReg8Mem1,   // Move Zero-Extended Memory1 byte to qword Register8 (AMD64)
   MOVZXReg4Mem2,   // Move Zero-Extended Memory2 word to dword Register4
   MOVZXReg8Mem2,   // Move Zero-Extended Memory2 word to qword Register8 (AMD64)
   NEG1Reg,         // 2's Complement Negate byte Register
   NEG2Reg,         // 2's Complement Negate word Register
   NEG4Reg,         // 2's Complement Negate dword Register
   NEG8Reg,         // 2's Complement Negate qword Register (AMD64)
   NEG1Mem,         // 2's Complement Negate Memory byte
   NEG2Mem,         // 2's Complement Negate Memory word
   NEG4Mem,         // 2's Complement Negate Memory dword
   NEG8Mem,         // 2's Complement Negate Memory qword (AMD64)
   NOT1Reg,         // 1's Complement byte Register
   NOT2Reg,         // 1's Complement word Register
   NOT4Reg,         // 1's Complement dword Register
   NOT8Reg,         // 1's Complement qword Register (AMD64)
   NOT1Mem,         // 1's Complement Memory byte
   NOT2Mem,         // 1's Complement Memory word
   NOT4Mem,         // 1's Complement Memory dword
   NOT8Mem,         // 1's Complement Memory qword (AMD64)
   OR1AccImm1,      // Logical Or byte   AL, Immediate byte
   OR2AccImm2,      // Logical Or word   AX, Immediate word
   OR4AccImm4,      // Logical Or dword EAX, Immediate dword
   OR8AccImm4,      // Logical Or qword RAX, Immediate sign-extended dword (AMD64)
   OR1RegImm1,      // Logical Or byte  Register, Immediate byte
   OR2RegImm2,      // Logical Or word  Register, Immediate word
   OR2RegImms,      // Logical Or word  Register, Immediate sign-extended byte
   OR4RegImm4,      // Logical Or dword Register, Immediate dword
   OR8RegImm4,      // Logical Or qword Register, Immediate sign-extended dword (AMD64)
   OR4RegImms,      // Logical Or dword Register, Immediate sign-extended byte
   OR8RegImms,      // Logical Or qword Register, Immediate sign-extended byte (AMD64)
   OR1MemImm1,      // Logical Or byte  Memory, Immediate byte
   OR2MemImm2,      // Logical Or word  Memory, Immediate word
   OR2MemImms,      // Logical Or word  Memory, Immediate sign extended byte
   OR4MemImm4,      // Logical Or dword Memory, Immediate dword
   OR8MemImm4,      // Logical Or qword Memory, Immediate sign-extended dword (AMD64)
   OR4MemImms,      // Logical Or dword Memory, Immediate sign extended byte
   LOR4MemImms,     // Locked Logical Or dword Memory, Immediate sign extended byte (used for memory fence)
   LOR4MemReg,      // Locked Logical Or dword Memory, dword Register
   LOR8MemReg,      // Locked Logical Or qword Memory, qword Register (AMD64)
   OR8MemImms,      // Logical Or qword Memory, Immediate sign extended byte (AMD64)
   OR1RegReg,       // Logical Or byte  Register, Register
   OR2RegReg,       // Logical Or word  Register, Register
   OR4RegReg,       // Logical Or dword Register, Register
   OR8RegReg,       // Logical Or qword Register, Register (AMD64)
   OR1RegMem,       // Logical Or byte  Register, Memory
   OR2RegMem,       // Logical Or word  Register, Memory
   OR4RegMem,       // Logical Or dword Register, Memory
   OR8RegMem,       // Logical Or qword Register, Memory (AMD64)
   OR1MemReg,       // Logical Or byte  Memory, Register
   OR2MemReg,       // Logical Or word  Memory, Register
   OR4MemReg,       // Logical Or dword Memory, Register
   OR8MemReg,       // Logical Or qword Memory, Register (AMD64)
   PAUSE,           // Pause spin loop hint (Pentium 4)
   PCMPEQBRegReg,   // Check if two registers contain the same binary sequence
   PMOVMSKB4RegReg, // Move the leading bit of each byte from source to destination
   PMOVZXWD,        // move unsigned short in lower 8 bytes to int
   PMULLD,          // mul int32 and keep low 32 bits
   PADDD,           // add int32 and keep low 32 bits
   PSHUFDRegRegImm1,// Shuffle Packed Doublewords Register, Register by Immediate config
   PSHUFDRegMemImm1,// Shuffle Packed Doublewords Register, Memory by Immediate config
   PSRLDQRegImm1,   // shift 128 xmm register
   PMOVZXxmm18Reg,  // Move a byte from register to XMM1 and zero extend
   POPCNT4RegReg,   // Count the number of bits set in the target register
   POPCNT8RegReg,
   POPReg,          // Pop stack to Register
   POPMem,          // Pop stack to Memory
   PUSHImms,        // Push Sign-Extended Immediate byte to stack
   PUSHImm4,        // Push Immediate dword to stack (sign-extended on AMD64)
   PUSHReg,         // Push Register to stack
   PUSHRegLong,     // Push Register to stack, force 2-byte instruction
   PUSHMem,         // Push address-sized Memory to stack
   RCL1RegImm1,     // Rotate through carry left byte by immediate byte # bits
   RCL4RegImm1,     // Rotate through carry left dword by immediate byte # bits
   RCR1RegImm1,     // Rotate through carry right byte by immediate byte # bits
   RCR4RegImm1,     // Rotate through carry right dword by immediate byte # bits
   REPMOVSB,        // Move ECX bytes from memory at [ESI] to memory at [EDI]
   REPMOVSW,        // Move ECX words from memory at [ESI] to memory at [EDI]
   REPMOVSD,        // Move ECX dwords from memory at [ESI] to memory at [EDI]
   REPMOVSQ,        // Move RCX qwords from memory at [RSI] to memory at [RDI]
   REPSTOSB,        // Store ECX copies of AL to memory at [EDI]
   REPSTOSW,        // Store ECX copies of AX to memory at [EDI]
   REPSTOSD,        // Store ECX copies of EAX to memory at [EDI] TODO:AMD64: Actually RCX copies
   REPSTOSQ,        // Store RCX copies of RAX to memory at [RDI]
   RET,             // Return near to caller
   RETImm2,         // Return near to caller and cleanup Imm2 bytes of args
   ROL1RegImm1,     // Rotate Left byte Register by Immediate byte # bits
   ROL2RegImm1,     // Rotate Left word Register by Immediate byte # bits
   ROL4RegImm1,     // Rotate Left dword Register by Immediate byte # bits
   ROL8RegImm1,     // Rotate Left qword Register by Immediate byte # bits (AMD64)
   ROL4RegCL,       // Rotate Left dword Register by CL bits
   ROL8RegCL,       // Rotate Left qword Register by CL bits (AMD64)
   ROR1RegImm1,     // Rotate Right byte Register by Immediate byte # bits
   ROR2RegImm1,     // Rotate Right word Register by Immediate byte # bits
   ROR4RegImm1,     // Rotate Right dword Register by Immediate byte # bits
   ROR8RegImm1,     // Rotate Right qword Register by Immediate byte # bits (AMD64)
   SAHF,            // Loads SF, ZF, AF, PF, and CF from AH into EFLAGS; not on AMD64
   SHL1RegImm1,     // Shift Left byte Register by Immediate byte # bits
   SHL1RegCL,       // Shift Left byte Register by CL bits
   SHL2RegImm1,     // Shift Left word Register by Immediate byte # bits
   SHL2RegCL,       // Shift Left word Register by CL bits
   SHL4RegImm1,     // Shift Left dword Register by Immediate byte # bits
   SHL8RegImm1,     // Shift Left qword Register by Immediate byte # bits (AMD64)
   SHL4RegCL,       // Shift Left dword Register by CL bits
   SHL8RegCL,       // Shift Left qword Register by CL bits (AMD64)
   SHL1MemImm1,     // Shift Left Memory byte by Immediate byte # bits
   SHL1MemCL,       // Shift Left Memory byte by CL bits
   SHL2MemImm1,     // Shift Left Memory word by Immediate byte # bits
   SHL2MemCL,       // Shift Left Memory word by CL bits
   SHL4MemImm1,     // Shift Left Memory dword by Immediate byte # bits
   SHL8MemImm1,     // Shift Left Memory qword by Immediate byte # bits (AMD64)
   SHL4MemCL,       // Shift Left Memory dword by CL bits
   SHL8MemCL,       // Shift Left Memory qword by CL bits (AMD64)
   SHR1RegImm1,     // Shift Right Logical byte Register by Immediate byte # bits
   SHR1RegCL,       // Shift Right Logical byte Register by CL bits
   SHR2RegImm1,     // Shift Right Logical word Register by Immediate byte # bits
   SHR2RegCL,       // Shift Right Logical word Register by CL bits
   SHR4RegImm1,     // Shift Right Logical dword Register by Immediate byte # bits
   SHR8RegImm1,     // Shift Right Logical qword Register by Immediate byte # bits (AMD64)
   SHR4RegCL,       // Shift Right Logical dword Register by CL bits
   SHR8RegCL,       // Shift Right Logical qword Register by CL bits (AMD64)
   SHR1MemImm1,     // Shift Right Logical Memory byte by Immediate byte # bits
   SHR1MemCL,       // Shift Right Logical Memory byte by CL bits
   SHR2MemImm1,     // Shift Right Logical Memory word by Immediate byte # bits
   SHR2MemCL,       // Shift Right Logical Memory word by CL bits
   SHR4MemImm1,     // Shift Right Logical Memory dword by Immediate byte # bits
   SHR8MemImm1,     // Shift Right Logical Memory qword by Immediate byte # bits (AMD64)
   SHR4MemCL,       // Shift Right Logical Memory dword by CL bits
   SHR8MemCL,       // Shift Right Logical Memory qword by CL bits (AMD64)
   SAR1RegImm1,     // Shift Right Arithmetic byte Register by Immediate byte # bits
   SAR1RegCL,       // Shift Right Arithmetic byte Register by CL bits
   SAR2RegImm1,     // Shift Right Arithmetic word Register by Immediate byte # bits
   SAR2RegCL,       // Shift Right Arithmetic word Register by CL bits
   SAR4RegImm1,     // Shift Right Arithmetic dword Register by Immediate byte # bits
   SAR8RegImm1,     // Shift Right Arithmetic qword Register by Immediate byte # bits (AMD64)
   SAR4RegCL,       // Shift Right Arithmetic dword Register by CL bits
   SAR8RegCL,       // Shift Right Arithmetic qword Register by CL bits (AMD64)
   SAR1MemImm1,     // Shift Right Arithmetic Memory byte by Immediate byte # bits
   SAR1MemCL,       // Shift Right Arithmetic Memory byte by CL bits
   SAR2MemImm1,     // Shift Right Arithmetic Memory word by Immediate byte # bits
   SAR2MemCL,       // Shift Right Arithmetic Memory word by CL bits
   SAR4MemImm1,     // Shift Right Arithmetic Memory dword by Immediate byte # bits
   SAR8MemImm1,     // Shift Right Arithmetic Memory qword by Immediate byte # bits (AMD64)
   SAR4MemCL,       // Shift Right Arithmetic Memory dword by CL bits
   SAR8MemCL,       // Shift Right Arithmetic Memory qword by CL bits (AMD64)
   SBB1AccImm1,     // Subtract byte with Borrow   AL, Immediate byte
   SBB2AccImm2,     // Subtract word with Borrow   AX, Immediate word
   SBB4AccImm4,     // Subtract dword with Borrow EAX, Immediate dword
   SBB8AccImm4,     // Subtract qword with Borrow RAX, Immediate sign-extended dword (AMD64)
   SBB1RegImm1,     // Subtract byte with Borrow  Register, Immediate byte
   SBB2RegImm2,     // Subtract word with Borrow  Register, Immediate word
   SBB2RegImms,     // Subtract word with Borrow  Register, Immediate sign-extended byte
   SBB4RegImm4,     // Subtract dword with Borrow Register, Immediate dword
   SBB8RegImm4,     // Subtract qword with Borrow Register, Immediate sign-extended dword (AMD64)
   SBB4RegImms,     // Subtract dword with Borrow Register, Immediate sign-extended byte
   SBB8RegImms,     // Subtract qword with Borrow Register, Immediate sign-extended byte (AMD64)
   SBB1MemImm1,     // Subtract byte with Borrow  Memory, Immediate byte
   SBB2MemImm2,     // Subtract word with Borrow  Memory, Immediate word
   SBB2MemImms,     // Subtract word with Borrow  Memory, Immediate sign extended byte
   SBB4MemImm4,     // Subtract dword with Borrow Memory, Immediate dword
   SBB8MemImm4,     // Subtract qword with Borrow Memory, Immediate sign-extended dword (AMD64)
   SBB4MemImms,     // Subtract dword with Borrow Memory, Immediate sign extended byte
   SBB8MemImms,     // Subtract qword with Borrow Memory, Immediate sign extended byte (AMD64)
   SBB1RegReg,      // Subtract byte with Borrow  Register, Register
   SBB2RegReg,      // Subtract word with Borrow  Register, Register
   SBB4RegReg,      // Subtract dword with Borrow Register, Register
   SBB8RegReg,      // Subtract qword with Borrow Register, Register (AMD64)
   SBB1RegMem,      // Subtract byte with Borrow  Register, Memory
   SBB2RegMem,      // Subtract word with Borrow  Register, Memory
   SBB4RegMem,      // Subtract dword with Borrow Register, Memory
   SBB8RegMem,      // Subtract qword with Borrow Register, Memory (AMD64)
   SBB1MemReg,      // Subtract byte with Borrow  Memory, Register
   SBB2MemReg,      // Subtract word with Borrow  Memory, Register
   SBB4MemReg,      // Subtract dword with Borrow Memory, Register
   SBB8MemReg,      // Subtract qword with Borrow Memory, Register (AMD64)
   SETA1Reg,        // Set Register byte if above (CF==0 && ZF==0)
   SETAE1Reg,       // Set Register byte if above or equal (CF==0)
   SETB1Reg,        // Set Register byte if below (CF==1)
   SETBE1Reg,       // Set Register byte if below or equal (CF==1 || ZF==1)
   SETE1Reg,        // Set Register byte if equal (ZF==1)
   SETNE1Reg,       // Set Register byte if not equal (ZF==0)
   SETG1Reg,        // Set Register byte if greater than  (ZF==0 && SF==OF)
   SETGE1Reg,       // Set Register byte if greater than or equal (SF==OF)
   SETL1Reg,        // Set Register byte if less than (SF!=OF)
   SETLE1Reg,       // Set Register byte if less than or equal (ZF==1 || SF!=OF)
   SETS1Reg,        // Set Register byte if signed (SF==1)
   SETNS1Reg,       // Set Register byte if not signed (SF==0)
   SETPO1Reg,       // Set Register byte if parity odd (PF==0)
   SETPE1Reg,       // Set Register byte if parity even (PF==1)
   SETA1Mem,        // Set Memory byte if above (CF==0 && ZF==0)
   SETAE1Mem,       // Set Memory byte if above or equal (CF==0)
   SETB1Mem,        // Set Memory byte if below (CF==1)
   SETBE1Mem,       // Set Memory byte if below or equal (CF==1 || ZF==1)
   SETE1Mem,        // Set Memory byte if equal (ZF==1)
   SETNE1Mem,       // Set Memory byte if not equal (ZF==0)
   SETG1Mem,        // Set Memory byte if greater than  (ZF==0 && SF==OF)
   SETGE1Mem,       // Set Memory byte if greater than or equal (SF==OF)
   SETL1Mem,        // Set Memory byte if less than (SF!=OF)
   SETLE1Mem,       // Set Memory byte if less than or equal (ZF==1 || SF!=OF)
   SETS1Mem,        // Set Memory byte if signed (SF==1)
   SETNS1Mem,       // Set Memory byte if not signed (SF==0)
   SHLD4RegRegImm1, // Shift Left Double Precision Register, Register by Immediate count
   SHLD4RegRegCL,   // Shift Left Double Precision Register, Register by CL count
   SHLD4MemRegImm1, // Shift Left Double Precision Memory, Register by Immediate count
   SHLD4MemRegCL,   // Shift Left Double Precision Memory, Register by CL count
   SHRD4RegRegImm1, // Shift Right Double Precision Register, Register by Immediate count
   SHRD4RegRegCL,   // Shift Right Double Precision Register, Register by CL count
   SHRD4MemRegImm1, // Shift Right Double Precision Memory, Register by Immediate count
   SHRD4MemRegCL,   // Shift Right Double Precision Memory, Register by CL count
   STOSB,           // Store AL in byte at [EDI]
   STOSW,           // Store AX in word at [EDI]
   STOSD,           // Store EAX in dword at [EDI]
   STOSQ,           // Store RAX in dword at [RDI] (AMD64)
   SUB1AccImm1,     // Subtract byte   AL, Immediate byte
   SUB2AccImm2,     // Subtract word   AX, Immediate word
   SUB4AccImm4,     // Subtract dword EAX, Immediate dword
   SUB8AccImm4,     // Subtract qword RAX, Immediate sign-extended dword (AMD64)
   SUB1RegImm1,     // Subtract byte  Register, Immediate byte
   SUB2RegImm2,     // Subtract word  Register, Immediate word
   SUB2RegImms,     // Subtract word  Register, Immediate sign-extended byte
   SUB4RegImm4,     // Subtract dword Register, Immediate dword
   SUB8RegImm4,     // Subtract qword Register, Immediate sign-extended dword (AMD64)
   SUB4RegImms,     // Subtract dword Register, Immediate sign-extended byte
   SUB8RegImms,     // Subtract qword Register, Immediate sign-extended byte (AMD64)
   SUB1MemImm1,     // Subtract byte  Memory, Immediate byte
   SUB2MemImm2,     // Subtract word  Memory, Immediate word
   SUB2MemImms,     // Subtract word  Memory, Immediate sign extended byte
   SUB4MemImm4,     // Subtract dword Memory, Immediate dword
   SUB8MemImm4,     // Subtract qword Memory, Immediate sign-extended dword (AMD64)
   SUB4MemImms,     // Subtract dword Memory, Immediate sign extended byte
   SUB8MemImms,     // Subtract qword Memory, Immediate sign extended byte (AMD64)
   SUB1RegReg,      // Subtract byte  Register, Register
   SUB2RegReg,      // Subtract word  Register, Register
   SUB4RegReg,      // Subtract dword Register, Register
   SUB8RegReg,      // Subtract qword Register, Register (AMD64)
   SUB1RegMem,      // Subtract byte  Register, Memory
   SUB2RegMem,      // Subtract word  Register, Memory
   SUB4RegMem,      // Subtract dword Register, Memory
   SUB8RegMem,      // Subtract qword Register, Memory (AMD64)
   SUB1MemReg,      // Subtract byte  Memory, Register
   SUB2MemReg,      // Subtract word  Memory, Register
   SUB4MemReg,      // Subtract dword Memory, Register
   SUB8MemReg,      // Subtract qword Memory, Register (AMD64)
   SUBSSRegReg,     // Subtract Scalar Single-FP Register, Register
   SUBSSRegMem,     // Subtract Scalar Single-FP Register, Memory
   SUBSDRegReg,     // Subtract Scalar Double-FP Register, Register
   SUBSDRegMem,     // Subtract Scalar Double-FP Register, Memory
   TEST1AccImm1,    // Test byte   AL, Immediate byte
   TEST1AccHImm1,   // Test byte   AH, Immediate byte
   TEST2AccImm2,    // Test word   AX, Immediate word
   TEST4AccImm4,    // Test dword EAX, Immediate dword
   TEST8AccImm4,    // Test qword RAX, Immediate sign-extended dword (AMD64)
   TEST1RegImm1,    // Test byte  Register, Immediate byte
   TEST2RegImm2,    // Test word  Register, Immediate word
   TEST4RegImm4,    // Test dword Register, Immediate dword
   TEST8RegImm4,    // Test qword Register, Immediate sign-extended dword (AMD64)
   TEST1MemImm1,    // Test byte  Memory, Immediate byte
   TEST2MemImm2,    // Test word  Memory, Immediate word
   TEST4MemImm4,    // Test dword Memory, Immediate dword
   TEST8MemImm4,    // Test qword Memory, Immediate sign-extended dword (AMD64)
   TEST1RegReg,     // Test byte  Register, Register
   TEST2RegReg,     // Test word  Register, Register
   TEST4RegReg,     // Test dword Register, Register
   TEST8RegReg,     // Test qword Register, Register (AMD64)
   TEST1MemReg,     // Test byte  Memory, Register
   TEST2MemReg,     // Test word  Memory, Register
   TEST4MemReg,     // Test dword Memory, Register
   TEST8MemReg,     // Test qword Memory, Register (AMD64)
   XABORT,          // transaction abort
   XBEGIN2,         // transaction begin
   XBEGIN4,         // transaction begin
   XCHG2AccReg,     // Exchange AX, word Register
   XCHG4AccReg,     // Exchange EAX, dword Register
   XCHG8AccReg,     // Exchange RAX, qword Register (AMD64)
   XCHG1RegReg,     // Exchange byte Register, byte Register
   XCHG2RegReg,     // Exchange word Register, word Register
   XCHG4RegReg,     // Exchange dword Register, dword Register
   XCHG8RegReg,     // Exchange qword Register, qword Register (AMD64)
   XCHG1RegMem,     // Exchange byte Register, Memory byte
   XCHG2RegMem,     // Exchange word Register, Memory word
   XCHG4RegMem,     // Exchange dword Register, Memory dword
   XCHG8RegMem,     // Exchange qword Register, Memory qword (AMD64)
   XCHG1MemReg,     // Exchange Memory byte, byte Register
   XCHG2MemReg,     // Exchange Memory word, word Register
   XCHG4MemReg,     // Exchange Memory dword, dword Register
   XCHG8MemReg,     // Exchange Memory qword, qword Register (AMD64)
   XEND,            // transaction end
   XOR1AccImm1,     // Exclusive Or byte   AL, Immediate byte
   XOR2AccImm2,     // Exclusive Or word   AX, Immediate word
   XOR4AccImm4,     // Exclusive Or dword EAX, Immediate dword
   XOR8AccImm4,     // Exclusive Or qword RAX, Immediate sign-extended dword (AMD64)
   XOR1RegImm1,     // Exclusive Or byte  Register, Immediate byte
   XOR2RegImm2,     // Exclusive Or word  Register, Immediate word
   XOR2RegImms,     // Exclusive Or word  Register, Immediate sign-extended byte
   XOR4RegImm4,     // Exclusive Or dword Register, Immediate dword
   XOR8RegImm4,     // Exclusive Or qword Register, Immediate sign-extended dword (AMD64)
   XOR4RegImms,     // Exclusive Or dword Register, Immediate sign-extended byte
   XOR8RegImms,     // Exclusive Or qword Register, Immediate sign-extended byte (AMD64)
   XOR1MemImm1,     // Exclusive Or byte  Memory, Immediate byte
   XOR2MemImm2,     // Exclusive Or word  Memory, Immediate word
   XOR2MemImms,     // Exclusive Or word  Memory, Immediate sign extended byte
   XOR4MemImm4,     // Exclusive Or dword Memory, Immediate dword
   XOR8MemImm4,     // Exclusive Or qword Memory, Immediate sign-extended dword (AMD64)
   XOR4MemImms,     // Exclusive Or dword Memory, Immediate sign extended byte
   XOR8MemImms,     // Exclusive Or qword Memory, Immediate sign extended byte (AMD64)
   XOR1RegReg,      // Exclusive Or byte  Register, Register
   XOR2RegReg,      // Exclusive Or word  Register, Register
   XOR4RegReg,      // Exclusive Or dword Register, Register
   XOR8RegReg,      // Exclusive Or qword Register, Register (AMD64)
   XOR1RegMem,      // Exclusive Or byte  Register, Memory
   XOR2RegMem,      // Exclusive Or word  Register, Memory
   XOR4RegMem,      // Exclusive Or dword Register, Memory
   XOR8RegMem,      // Exclusive Or qword Register, Memory (AMD64)
   XOR1MemReg,      // Exclusive Or byte  Memory, Register
   XOR2MemReg,      // Exclusive Or word  Memory, Register
   XOR4MemReg,      // Exclusive Or dword Memory, Register
   XOR8MemReg,      // Exclusive Or qword Memory, Register (AMD64)
   XORPSRegReg,     // Exclusive Or Packed Single-FP Register, Register
   XORPSRegMem,     // Exclusive Or Packed Single-FP Register, Memory
   XORPDRegReg,     // Exclusive Or Packed Double-FP Register, Register
   MFENCE,          // Memory fence
   LFENCE,          // Load fence
   SFENCE,          // Store fence
   PCMPESTRI,       // Packed Compare Explicit length STrings, Return Index
   PREFETCHNTA,     // Prefetch for non-temporal access
   PREFETCHT0,      // Prefetch to L1 cache
   PREFETCHT1,      // Prefetch to L2 cache
   PREFETCHT2,      // Prefetch to L3 cache
   ANDNPSRegReg,    // And not Packed Single-FP Register, Register
   ANDNPDRegReg,    // And not Packed Double-FP Register, Register
   PSLLQRegImm1,    // Shift left XMM register by number of bits
   PSRLQRegImm1,    // Shift right XMM register by number of bits
   FENCE,           // Address of binary is to be written to specified data address
                    // SymbolReference controls code motion across fence
   VGFENCE,         // Special Fence used for patching virtual guards
   PROCENTRY,       // Entry to the method
   DQImm64,         // Define 8 bytes (AMD64)
   DDImm4,          // Define 4 bytes
   DWImm2,          // Define 2 bytes
   DBImm1,          // Define 1 byte
   WRTBAR,          // Write barrier directive/macro for GCs that need them
   ASSOCREGS,       // Directive to change register associations
   FPREGSPILL,      // Directive to force the FP stack to be spilled
   VirtualGuardNOP, // A virtual guard encoded as a NOP instruction
   LABEL,           // Destination of a jump
   FCMPEVAL,        // Instruction selection of FP comparisons at register assignment time
   RestoreVMThread, // restore VM thread register from fs:[0]
   PPS_OPCOUNT,     // Phase profiling site (object access counting)
   PPS_OPFIELD,     // Phase profiling site (object field counting)
   AdjustFramePtr,  // Adjust frame pointer
   ReturnMarker,    // Return Marker for linkages that do not end with a RET instruction
   IA32LastOp = ReturnMarker,
   IA32NumOpCodes = IA32LastOp + 1
   } TR_X86OpCodes;

#define IA32LongToShortBranchConversionOffset ((int)JMP4 - (int)JMP1)
#define IA32LengthOfShortBranch               2
#define IA32OperandSizeOverridePrefix         0x66
#define IA32LockPrefix                        0xf0
#define IA32ScalarSSEPrefix                   0xf3
#define IA32ScalarSSE2Prefix                  0xf2
#define IA32RepPrefix                         0xf3
#define IA32XacquirePrefix                    0xf2
#define IA32XreleasePrefix                    0xf3
const uint8_t SSE42OpcodePrefix[] = { 0x66, 0x0f };

// Size-parameterized opcodes
//
inline TR_X86OpCodes BSFRegReg      (bool is64Bit){ return is64Bit? BSF8RegReg      : BSF4RegReg      ; }
inline TR_X86OpCodes BSWAPReg       (bool is64Bit){ return is64Bit? BSWAP8Reg       : BSWAP4Reg       ; }
inline TR_X86OpCodes BSRRegReg      (bool is64Bit){ return is64Bit? BSR8RegReg      : BSR4RegReg      ; }
inline TR_X86OpCodes CMOVERegMem    (bool is64Bit){ return is64Bit? CMOVE8RegMem    : CMOVE4RegMem    ; }
inline TR_X86OpCodes CMOVERegReg    (bool is64Bit){ return is64Bit? CMOVE8RegReg    : CMOVE4RegReg    ; }
inline TR_X86OpCodes CMOVNERegMem   (bool is64Bit){ return is64Bit? CMOVNE8RegMem   : CMOVNE4RegMem   ; }
inline TR_X86OpCodes CMOVNERegReg   (bool is64Bit){ return is64Bit? CMOVNE8RegReg   : CMOVNE4RegReg   ; }
inline TR_X86OpCodes CMPRegImms     (bool is64Bit){ return is64Bit? CMP8RegImms     : CMP4RegImms     ; }
inline TR_X86OpCodes CMPRegImm4     (bool is64Bit){ return is64Bit? CMP8RegImm4     : CMP4RegImm4     ; }
inline TR_X86OpCodes CMPMemImms     (bool is64Bit){ return is64Bit? CMP8MemImms     : CMP4MemImms     ; }
inline TR_X86OpCodes CMPMemImm4     (bool is64Bit){ return is64Bit? CMP8MemImm4     : CMP4MemImm4     ; }
inline TR_X86OpCodes MOVRegReg      (bool is64Bit){ return is64Bit? MOV8RegReg      : MOV4RegReg      ; }
inline TR_X86OpCodes LEARegMem      (bool is64Bit){ return is64Bit? LEA8RegMem      : LEA4RegMem      ; }
inline TR_X86OpCodes LRegMem        (bool is64Bit){ return is64Bit? L8RegMem        : L4RegMem        ; }
inline TR_X86OpCodes SMemReg        (bool is64Bit){ return is64Bit? S8MemReg        : S4MemReg        ; }
inline TR_X86OpCodes SMemImm4       (bool is64Bit){ return is64Bit? S8MemImm4       : S4MemImm4       ; }
inline TR_X86OpCodes XRSMemImm4     (bool is64Bit){ return is64Bit? XRS8MemImm4     : XRS4MemImm4     ; }
inline TR_X86OpCodes XCHGRegReg     (bool is64Bit){ return is64Bit? XCHG8RegReg     : XCHG4RegReg     ; }
inline TR_X86OpCodes NEGReg         (bool is64Bit){ return is64Bit? NEG8Reg         : NEG4Reg         ; }
inline TR_X86OpCodes IMULRegReg     (bool is64Bit){ return is64Bit? IMUL8RegReg     : IMUL4RegReg     ; }
inline TR_X86OpCodes IMULRegMem     (bool is64Bit){ return is64Bit? IMUL8RegMem     : IMUL4RegMem     ; }
inline TR_X86OpCodes IMULRegRegImms (bool is64Bit){ return is64Bit? IMUL8RegRegImms : IMUL4RegRegImms ; }
inline TR_X86OpCodes IMULRegRegImm4 (bool is64Bit){ return is64Bit? IMUL8RegRegImm4 : IMUL4RegRegImm4 ; }
inline TR_X86OpCodes IMULRegMemImms (bool is64Bit){ return is64Bit? IMUL8RegMemImms : IMUL4RegMemImms ; }
inline TR_X86OpCodes IMULRegMemImm4 (bool is64Bit){ return is64Bit? IMUL8RegMemImm4 : IMUL4RegMemImm4 ; }
inline TR_X86OpCodes INCMem         (bool is64Bit){ return is64Bit? INC8Mem         : INC4Mem         ; }
inline TR_X86OpCodes INCReg         (bool is64Bit){ return is64Bit? INC8Reg         : INC4Reg         ; }
inline TR_X86OpCodes DECMem         (bool is64Bit){ return is64Bit? DEC8Mem         : DEC4Mem         ; }
inline TR_X86OpCodes DECReg         (bool is64Bit){ return is64Bit? DEC8Reg         : DEC4Reg         ; }
inline TR_X86OpCodes ADDMemImms     (bool is64Bit){ return is64Bit? ADD8MemImms     : ADD4MemImms     ; }
inline TR_X86OpCodes ADDRegImms     (bool is64Bit){ return is64Bit? ADD8RegImms     : ADD4RegImms     ; }
inline TR_X86OpCodes ADDRegImm4     (bool is64Bit){ return is64Bit? ADD8RegImm4     : ADD4RegImm4     ; }
inline TR_X86OpCodes ADCMemImms     (bool is64Bit){ return is64Bit? ADC8MemImms     : ADC4MemImms     ; }
inline TR_X86OpCodes ADCRegImms     (bool is64Bit){ return is64Bit? ADC8RegImms     : ADC4RegImms     ; }
inline TR_X86OpCodes ADCRegImm4     (bool is64Bit){ return is64Bit? ADC8RegImm4     : ADC4RegImm4     ; }
inline TR_X86OpCodes SUBMemImms     (bool is64Bit){ return is64Bit? SUB8MemImms     : SUB4MemImms     ; }
inline TR_X86OpCodes SUBRegImms     (bool is64Bit){ return is64Bit? SUB8RegImms     : SUB4RegImms     ; }
inline TR_X86OpCodes SBBMemImms     (bool is64Bit){ return is64Bit? SBB8MemImms     : SBB4MemImms     ; }
inline TR_X86OpCodes SBBRegImms     (bool is64Bit){ return is64Bit? SBB8RegImms     : SBB4RegImms     ; }
inline TR_X86OpCodes ADDMemImm4     (bool is64Bit){ return is64Bit? ADD8MemImm4     : ADD4MemImm4     ; }
inline TR_X86OpCodes ADDMemReg      (bool is64Bit){ return is64Bit? ADD8MemReg      : ADD4MemReg      ; }
inline TR_X86OpCodes ADDRegReg      (bool is64Bit){ return is64Bit? ADD8RegReg      : ADD4RegReg      ; }
inline TR_X86OpCodes ADDRegMem      (bool is64Bit){ return is64Bit? ADD8RegMem      : ADD4RegMem      ; }
inline TR_X86OpCodes LADDMemReg     (bool is64Bit){ return is64Bit? LADD8MemReg     : LADD4MemReg     ; }
inline TR_X86OpCodes LXADDMemReg    (bool is64Bit){ return is64Bit? LXADD8MemReg    : LXADD4MemReg    ; }
inline TR_X86OpCodes ADCMemImm4     (bool is64Bit){ return is64Bit? ADC8MemImm4     : ADC4MemImm4     ; }
inline TR_X86OpCodes ADCMemReg      (bool is64Bit){ return is64Bit? ADC8MemReg      : ADC4MemReg      ; }
inline TR_X86OpCodes ADCRegReg      (bool is64Bit){ return is64Bit? ADC8RegReg      : ADC4RegReg      ; }
inline TR_X86OpCodes ADCRegMem      (bool is64Bit){ return is64Bit? ADC8RegMem      : ADC4RegMem      ; }
inline TR_X86OpCodes SUBMemImm4     (bool is64Bit){ return is64Bit? SUB8MemImm4     : SUB4MemImm4     ; }
inline TR_X86OpCodes SUBRegImm4     (bool is64Bit){ return is64Bit? SUB8RegImm4     : SUB4RegImm4     ; }
inline TR_X86OpCodes SUBMemReg      (bool is64Bit){ return is64Bit? SUB8MemReg      : SUB4MemReg      ; }
inline TR_X86OpCodes SUBRegReg      (bool is64Bit){ return is64Bit? SUB8RegReg      : SUB4RegReg      ; }
inline TR_X86OpCodes SUBRegMem      (bool is64Bit){ return is64Bit? SUB8RegMem      : SUB4RegMem      ; }
inline TR_X86OpCodes SBBMemImm4     (bool is64Bit){ return is64Bit? SBB8MemImm4     : SBB4MemImm4     ; }
inline TR_X86OpCodes SBBRegImm4     (bool is64Bit){ return is64Bit? SBB8RegImm4     : SBB4RegImm4     ; }
inline TR_X86OpCodes SBBMemReg      (bool is64Bit){ return is64Bit? SBB8MemReg      : SBB4MemReg      ; }
inline TR_X86OpCodes SBBRegReg      (bool is64Bit){ return is64Bit? SBB8RegReg      : SBB4RegReg      ; }
inline TR_X86OpCodes SBBRegMem      (bool is64Bit){ return is64Bit? SBB8RegMem      : SBB4RegMem      ; }
inline TR_X86OpCodes ORRegImm4      (bool is64Bit){ return is64Bit? OR8RegImm4      : OR4RegImm4      ; }
inline TR_X86OpCodes ORRegImms      (bool is64Bit){ return is64Bit? OR8RegImms      : OR4RegImms      ; }
inline TR_X86OpCodes ORRegMem       (bool is64Bit){ return is64Bit? OR8RegMem       : OR4RegMem       ; }
inline TR_X86OpCodes XORRegMem      (bool is64Bit){ return is64Bit? XOR8RegMem      : XOR4RegMem      ; }
inline TR_X86OpCodes XORRegImms     (bool is64Bit){ return is64Bit? XOR8RegImms     : XOR4RegImms     ; }
inline TR_X86OpCodes XORRegImm4     (bool is64Bit){ return is64Bit? XOR8RegImm4     : XOR4RegImm4     ; }
inline TR_X86OpCodes CXXAcc         (bool is64Bit){ return is64Bit? CQOAcc          : CDQAcc          ; }
inline TR_X86OpCodes ANDRegImm4     (bool is64Bit){ return is64Bit? AND8RegImm4     : AND4RegImm4     ; }
inline TR_X86OpCodes ANDRegReg      (bool is64Bit){ return is64Bit? AND8RegReg      : AND4RegReg      ; }
inline TR_X86OpCodes ANDRegImms     (bool is64Bit){ return is64Bit? AND8RegImms     : AND4RegImms     ; }
inline TR_X86OpCodes ORRegReg       (bool is64Bit){ return is64Bit? OR8RegReg       : OR4RegReg       ; }
inline TR_X86OpCodes MOVRegImm4     (bool is64Bit){ return is64Bit? MOV8RegImm4     : MOV4RegImm4     ; }
inline TR_X86OpCodes IMULAccReg     (bool is64Bit){ return is64Bit? IMUL8AccReg     : IMUL4AccReg     ; }
inline TR_X86OpCodes IDIVAccMem     (bool is64Bit){ return is64Bit? IDIV8AccMem     : IDIV4AccMem     ; }
inline TR_X86OpCodes IDIVAccReg     (bool is64Bit){ return is64Bit? IDIV8AccReg     : IDIV4AccReg     ; }
inline TR_X86OpCodes DIVAccMem      (bool is64Bit){ return is64Bit? DIV8AccMem      : DIV4AccMem      ; }
inline TR_X86OpCodes DIVAccReg      (bool is64Bit){ return is64Bit? DIV8AccReg      : DIV4AccReg      ; }
inline TR_X86OpCodes NOTReg         (bool is64Bit){ return is64Bit? NOT8Reg         : NOT4Reg         ; }
inline TR_X86OpCodes POPCNTRegReg   (bool is64Bit){ return is64Bit? POPCNT8RegReg   : POPCNT4RegReg   ; }
inline TR_X86OpCodes ROLRegImm1     (bool is64Bit){ return is64Bit? ROL8RegImm1     : ROL4RegImm1     ; }
inline TR_X86OpCodes ROLRegCL       (bool is64Bit){ return is64Bit? ROL8RegCL       : ROL4RegCL       ; }
inline TR_X86OpCodes SHLMemImm1     (bool is64Bit){ return is64Bit? SHL8MemImm1     : SHL4MemImm1     ; }
inline TR_X86OpCodes SHLMemCL       (bool is64Bit){ return is64Bit? SHL8MemCL       : SHL4MemCL       ; }
inline TR_X86OpCodes SHLRegImm1     (bool is64Bit){ return is64Bit? SHL8RegImm1     : SHL4RegImm1     ; }
inline TR_X86OpCodes SHLRegCL       (bool is64Bit){ return is64Bit? SHL8RegCL       : SHL4RegCL       ; }
inline TR_X86OpCodes SARMemImm1     (bool is64Bit){ return is64Bit? SAR8MemImm1     : SAR4MemImm1     ; }
inline TR_X86OpCodes SARMemCL       (bool is64Bit){ return is64Bit? SAR8MemCL       : SAR4MemCL       ; }
inline TR_X86OpCodes SARRegImm1     (bool is64Bit){ return is64Bit? SAR8RegImm1     : SAR4RegImm1     ; }
inline TR_X86OpCodes SARRegCL       (bool is64Bit){ return is64Bit? SAR8RegCL       : SAR4RegCL       ; }
inline TR_X86OpCodes SHRMemImm1     (bool is64Bit){ return is64Bit? SHR8MemImm1     : SHR4MemImm1     ; }
inline TR_X86OpCodes SHRMemCL       (bool is64Bit){ return is64Bit? SHR8MemCL       : SHR4MemCL       ; }
inline TR_X86OpCodes SHRRegImm1     (bool is64Bit){ return is64Bit? SHR8RegImm1     : SHR4RegImm1     ; }
inline TR_X86OpCodes SHRRegCL       (bool is64Bit){ return is64Bit? SHR8RegCL       : SHR4RegCL       ; }
inline TR_X86OpCodes TESTMemImm4    (bool is64Bit){ return is64Bit? TEST8MemImm4    : TEST4MemImm4    ; }
inline TR_X86OpCodes TESTRegImm4    (bool is64Bit){ return is64Bit? TEST8RegImm4    : TEST4RegImm4    ; }
inline TR_X86OpCodes TESTRegReg     (bool is64Bit){ return is64Bit? TEST8RegReg     : TEST4RegReg     ; }
inline TR_X86OpCodes TESTMemReg     (bool is64Bit){ return is64Bit? TEST8MemReg     : TEST4MemReg     ; }
inline TR_X86OpCodes XORRegReg      (bool is64Bit){ return is64Bit? XOR8RegReg      : XOR4RegReg      ; }
inline TR_X86OpCodes CMPRegReg      (bool is64Bit){ return is64Bit? CMP8RegReg      : CMP4RegReg      ; }
inline TR_X86OpCodes CMPRegMem      (bool is64Bit){ return is64Bit? CMP8RegMem      : CMP4RegMem      ; }
inline TR_X86OpCodes CMPMemReg      (bool is64Bit){ return is64Bit? CMP8MemReg      : CMP4MemReg      ; }
inline TR_X86OpCodes CMPXCHGMemReg  (bool is64Bit){ return is64Bit? CMPXCHG8MemReg  : CMPXCHG4MemReg  ; }
inline TR_X86OpCodes LCMPXCHGMemReg (bool is64Bit){ return is64Bit? LCMPXCHG8MemReg : LCMPXCHG4MemReg ; }
inline TR_X86OpCodes REPSTOS        (bool is64Bit){ return is64Bit? REPSTOSQ        : REPSTOSD        ; }
// Floating-point
inline TR_X86OpCodes MOVSMemReg     (bool is64Bit){ return is64Bit? MOVSDMemReg     : MOVSSMemReg     ; }
inline TR_X86OpCodes MOVSRegMem     (bool is64Bit){ return is64Bit? MOVSDRegMem     : MOVSSRegMem     ; }

// Size and carry-parameterized opcodes
//
inline TR_X86OpCodes AddMemImms (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCMemImms(is64Bit) : ADDMemImms(is64Bit) ; }
inline TR_X86OpCodes AddRegImms (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCRegImms(is64Bit) : ADDRegImms(is64Bit) ; }
inline TR_X86OpCodes AddRegImm4 (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCRegImm4(is64Bit) : ADDRegImm4(is64Bit) ; }
inline TR_X86OpCodes AddMemImm4 (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCMemImm4(is64Bit) : ADDMemImm4(is64Bit) ; }
inline TR_X86OpCodes AddMemReg  (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCMemReg(is64Bit)  : ADDMemReg(is64Bit)  ; }
inline TR_X86OpCodes AddRegReg  (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCRegReg(is64Bit)  : ADDRegReg(is64Bit)  ; }
inline TR_X86OpCodes AddRegMem  (bool is64Bit, bool isWithCarry)  { return isWithCarry   ? ADCRegMem(is64Bit)  : ADDRegMem(is64Bit)  ; }

inline TR_X86OpCodes SubMemImms (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBMemImms(is64Bit) : SUBMemImms(is64Bit) ; }
inline TR_X86OpCodes SubRegImms (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBRegImms(is64Bit) : SUBRegImms(is64Bit) ; }
inline TR_X86OpCodes SubRegImm4 (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBRegImm4(is64Bit) : SUBRegImm4(is64Bit) ; }
inline TR_X86OpCodes SubMemImm4 (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBMemImm4(is64Bit) : SUBMemImm4(is64Bit) ; }
inline TR_X86OpCodes SubMemReg  (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBMemReg(is64Bit)  : SUBMemReg(is64Bit)  ; }
inline TR_X86OpCodes SubRegReg  (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBRegReg(is64Bit)  : SUBRegReg(is64Bit)  ; }
inline TR_X86OpCodes SubRegMem  (bool is64Bit, bool isWithBorrow) { return isWithBorrow  ? SBBRegMem(is64Bit)  : SUBRegMem(is64Bit)  ; }

// Whole-register opcodes
//
extern TR_X86OpCodes ADDRegImms();
extern TR_X86OpCodes ADDRegImm4();
extern TR_X86OpCodes ADDRegMem();
extern TR_X86OpCodes ADDRegReg();
extern TR_X86OpCodes ADCRegImms();
extern TR_X86OpCodes ADCRegImm4();
extern TR_X86OpCodes ADCRegMem();
extern TR_X86OpCodes ADCRegReg();
extern TR_X86OpCodes ANDRegImm4();
extern TR_X86OpCodes ANDRegImms();
extern TR_X86OpCodes ANDRegReg();
extern TR_X86OpCodes CMOVERegMem();
extern TR_X86OpCodes CMOVERegReg();
extern TR_X86OpCodes CMOVNERegMem();
extern TR_X86OpCodes CMOVNERegReg();
extern TR_X86OpCodes CMPRegImms();
extern TR_X86OpCodes CMPRegImm4();
extern TR_X86OpCodes CMPMemImms();
extern TR_X86OpCodes CMPMemImm4();
extern TR_X86OpCodes CMPMemReg();
extern TR_X86OpCodes CMPRegReg();
extern TR_X86OpCodes CMPRegMem();
extern TR_X86OpCodes CMPXCHGMemReg();
extern TR_X86OpCodes LCMPXCHGMemReg();
extern TR_X86OpCodes LEARegMem();
extern TR_X86OpCodes LRegMem();
extern TR_X86OpCodes MOVRegImm4();
extern TR_X86OpCodes MOVRegReg();
extern TR_X86OpCodes ORRegImm4();
extern TR_X86OpCodes ORRegImms();
extern TR_X86OpCodes ORRegMem();
extern TR_X86OpCodes ORRegReg();
extern TR_X86OpCodes REPSTOS();
extern TR_X86OpCodes SHLRegImm1();
extern TR_X86OpCodes SARRegImm1();
extern TR_X86OpCodes SHRRegImm1();
extern TR_X86OpCodes SMemReg();
extern TR_X86OpCodes SMemImm4();
extern TR_X86OpCodes SUBRegImms();
extern TR_X86OpCodes SUBRegImm4();
extern TR_X86OpCodes SUBRegReg();
extern TR_X86OpCodes SBBRegImms();
extern TR_X86OpCodes SBBRegImm4();
extern TR_X86OpCodes SBBRegReg();
extern TR_X86OpCodes TESTRegReg();
extern TR_X86OpCodes TESTRegImm4();
extern TR_X86OpCodes TESTMemImm4();
extern TR_X86OpCodes XCHGRegReg();
extern TR_X86OpCodes XORRegReg();
extern TR_X86OpCodes XORRegMem();
extern TR_X86OpCodes XORRegImms();
extern TR_X86OpCodes XORRegImm4();


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
#define IA32OpProp_Needs16BitOperandPrefix      0x02000000
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

#define IA32OpProp2_PushOp                    0x00000001
#define IA32OpProp2_PopOp                     0x00000002
#define IA32OpProp2_ShiftOp                   0x00000004
#define IA32OpProp2_RotateOp                  0x00000008
#define IA32OpProp2_SetsCCForCompare          0x00000010
#define IA32OpProp2_SetsCCForTest             0x00000020
#define IA32OpProp2_SupportsLockPrefix        0x00000040
#define IA32OpProp2_NeedsScalarPrefix         0x00000080
#define IA32OpProp2_DoubleWordSource          0x00000100
#define IA32OpProp2_DoubleWordTarget          0x00000200
#define IA32OpProp2_XMMSource                 0x00000400
#define IA32OpProp2_XMMTarget                 0x00000800
#define IA32OpProp2_PseudoOp                  0x00001000
#define IA32OpProp2_NeedsRepPrefix            0x00002000
#define IA32OpProp2_NeedsLockPrefix           0x00004000
#define IA32OpProp2_CannotBeAssembled         0x00008000
#define IA32OpProp2_CallOp                    0x00010000
#define IA32OpProp2_SourceIsMemRef            0x00020000
#define IA32OpProp2_SourceRegIsImplicit       0x00040000
#define IA32OpProp2_TargetRegIsImplicit       0x00080000
#define IA32OpProp2_FusableCompare            0x00100000
#define IA32OpProp2_NeedsSSE42OpcodePrefix    0x00200000
#define IA32OpProp2_NeedsXacquirePrefix       0x00400000
#define IA32OpProp2_NeedsXreleasePrefix       0x00800000
////////////////////
//
// AMD64 flags
//
#define IA32OpProp2_LongSource                0x80000000
#define IA32OpProp2_LongTarget                0x40000000
#define IA32OpProp2_LongImmediate             0x20000000 // MOV and DQ only

// This flag indicates only that an instruction has 64-bit operands; however,
// all instructions accessing registers need a Rex prefix if they access new
// AMD64 registers, even if they do not have 64-bit operands.
//
#define IA32OpProp2_Needs64BitOperandPrefix   0x10000000

// For instructions not supported on AMD64
//
#define IA32OpProp2_IsIA32Only                0x08000000
#define IA32OpProp2_IsAMD64DeprecatedInc      0x04000000
#define IA32OpProp2_IsAMD64DeprecatedDec      0x02000000


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


class TR_X86OpCode
   {

   typedef struct
      {
      uint8_t _bytes[3];
      uint8_t _length;
      } TR_OpCodeBinaryEntry;

   TR_X86OpCodes                    _opCode;
   static const uint32_t             _properties[IA32NumOpCodes];
   static const uint32_t             _properties2[IA32NumOpCodes];
   static const TR_OpCodeBinaryEntry _binaryEncodings[IA32NumOpCodes];

   public:

   TR_X86OpCode()                  : _opCode(BADIA32Op) {}
   TR_X86OpCode(TR_X86OpCodes op) : _opCode(op)       {}

   TR_X86OpCodes getOpCodeValue()                  {return _opCode;}
   TR_X86OpCodes setOpCodeValue(TR_X86OpCodes op) {return (_opCode = op);}

   uint32_t modifiesTarget() {return _properties[_opCode] & IA32OpProp_ModifiesTarget;}

   uint32_t modifiesSource() {return _properties[_opCode] & IA32OpProp_ModifiesSource;}

   uint32_t usesTarget() {return _properties[_opCode] & IA32OpProp_UsesTarget;}

   uint32_t singleFPOp() {return _properties[_opCode] & IA32OpProp_SingleFP;}

   uint32_t doubleFPOp() {return _properties[_opCode] & IA32OpProp_DoubleFP;}

   uint32_t gprOp() {return (_properties[_opCode] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}

   uint32_t fprOp() {return (_properties[_opCode] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}

   uint32_t hasByteImmediate() {return _properties[_opCode] & IA32OpProp_ByteImmediate;}

   uint32_t hasShortImmediate() {return _properties[_opCode] & IA32OpProp_ShortImmediate;}

   uint32_t hasIntImmediate() {return _properties[_opCode] & IA32OpProp_IntImmediate;}

   uint32_t hasLongImmediate() {return _properties2[_opCode] & IA32OpProp2_LongImmediate;}

   uint32_t hasSignExtendImmediate() {return _properties[_opCode] & IA32OpProp_SignExtendImmediate;}

   uint32_t testsZeroFlag() {return _properties[_opCode] & IA32OpProp_TestsZeroFlag;}
   uint32_t modifiesZeroFlag() {return _properties[_opCode] & IA32OpProp_ModifiesZeroFlag;}
   uint32_t referencesZeroFlag() {return _properties[_opCode] & (IA32OpProp_ModifiesZeroFlag | IA32OpProp_TestsZeroFlag);}

   uint32_t testsSignFlag() {return _properties[_opCode] & IA32OpProp_TestsSignFlag;}
   uint32_t modifiesSignFlag() {return _properties[_opCode] & IA32OpProp_ModifiesSignFlag;}
   uint32_t referencesSignFlag() {return _properties[_opCode] & (IA32OpProp_ModifiesSignFlag | IA32OpProp_TestsSignFlag);}

   uint32_t testsCarryFlag() {return _properties[_opCode] & IA32OpProp_TestsCarryFlag;}
   uint32_t modifiesCarryFlag() {return _properties[_opCode] & IA32OpProp_ModifiesCarryFlag;}
   uint32_t referencesCarryFlag() {return _properties[_opCode] & (IA32OpProp_ModifiesCarryFlag | IA32OpProp_TestsCarryFlag);}

   uint32_t testsOverflowFlag() {return _properties[_opCode] & IA32OpProp_TestsOverflowFlag;}
   uint32_t modifiesOverflowFlag() {return _properties[_opCode] & IA32OpProp_ModifiesOverflowFlag;}
   uint32_t referencesOverflowFlag() {return _properties[_opCode] & (IA32OpProp_ModifiesOverflowFlag | IA32OpProp_TestsOverflowFlag);}

   uint32_t testsParityFlag() {return _properties[_opCode] & IA32OpProp_TestsParityFlag;}
   uint32_t modifiesParityFlag() {return _properties[_opCode] & IA32OpProp_ModifiesParityFlag;}
   uint32_t referencesParityFlag() {return _properties[_opCode] & (IA32OpProp_ModifiesParityFlag | IA32OpProp_TestsParityFlag);}

   uint32_t hasByteSource() {return _properties[_opCode] & IA32OpProp_ByteSource;}

   uint32_t hasByteTarget() {return _properties[_opCode] & IA32OpProp_ByteTarget;}

   uint32_t hasShortSource() {return _properties[_opCode] & IA32OpProp_ShortSource;}

   uint32_t hasShortTarget() {return _properties[_opCode] & IA32OpProp_ShortTarget;}

   uint32_t hasIntSource() {return _properties[_opCode] & IA32OpProp_IntSource;}

   uint32_t hasIntTarget() {return _properties[_opCode] & IA32OpProp_IntTarget;}

   uint32_t hasLongSource() {return _properties2[_opCode] & IA32OpProp2_LongSource;}

   uint32_t hasLongTarget() {return _properties2[_opCode] & IA32OpProp2_LongTarget;}

   uint32_t hasDoubleWordSource() {return _properties2[_opCode] & IA32OpProp2_DoubleWordSource;}

   uint32_t hasDoubleWordTarget() {return _properties2[_opCode] & IA32OpProp2_DoubleWordTarget;}

   uint32_t hasXMMSource() {return _properties2[_opCode] & IA32OpProp2_XMMSource;}

   uint32_t hasXMMTarget() {return _properties2[_opCode] & IA32OpProp2_XMMTarget;}

   uint32_t isPseudoOp() {return _properties2[_opCode] & IA32OpProp2_PseudoOp;}

   // Temporarily disable broken logic so that the proper instructions appear in JIT logs
   uint32_t cannotBeAssembled() {return false;} // {return _properties2[_opCode] & IA32OpProp2_CannotBeAssembled;}

   uint8_t getSourceOperandSize() {return getSourceOperandSize(_opCode); }

   uint8_t getTargetOperandSize() {return getTargetOperandSize(_opCode); }

   uint32_t needs16BitOperandPrefix() {return _properties[_opCode] & IA32OpProp_Needs16BitOperandPrefix;}

   uint32_t needs64BitOperandPrefix() {return _properties2[_opCode] & IA32OpProp2_Needs64BitOperandPrefix;}

   uint32_t needsScalarPrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsScalarPrefix;}

   uint32_t needsRepPrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsRepPrefix;}

   uint32_t needsLockPrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsLockPrefix;}

   uint32_t needsXacquirePrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsXacquirePrefix;}

   uint32_t needsXreleasePrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsXreleasePrefix;}

   uint32_t needsSSE42OpcodePrefix() {return _properties2[_opCode] & IA32OpProp2_NeedsSSE42OpcodePrefix;}

   uint32_t clearsUpperBits() {return hasIntTarget() && modifiesTarget();}

   uint32_t setsUpperBits() {return hasLongTarget() && modifiesTarget();}

   uint32_t hasTargetRegisterInOpcode() {return _properties[_opCode] & IA32OpProp_TargetRegisterInOpcode;}

   uint32_t hasTargetRegisterInModRM() {return _properties[_opCode] & IA32OpProp_TargetRegisterInModRM;}

   uint32_t hasTargetRegisterIgnored() {return _properties[_opCode] & IA32OpProp_TargetRegisterIgnored;}

   uint32_t hasSourceRegisterInModRM() {return _properties[_opCode] & IA32OpProp_SourceRegisterInModRM;}

   uint32_t hasSourceRegisterIgnored() {return _properties[_opCode] & IA32OpProp_SourceRegisterIgnored;}

   uint32_t isBranchOp() {return _properties[_opCode] & IA32OpProp_BranchOp;}
   uint32_t hasRelativeBranchDisplacement() { return isBranchOp() || isCallImmOp(); }

   uint32_t isShortBranchOp() {return isBranchOp() && hasByteImmediate();}

   uint32_t testsSomeFlag() {return _properties[_opCode] & IA32OpProp_TestsSomeFlag;}

   uint32_t modifiesSomeArithmeticFlags() {return _properties[_opCode] & IA32OpProp_SetsSomeArithmeticFlag;}
   bool modifiesAllArithmeticFlags() {return (_properties[_opCode] & IA32OpProp_SetsSomeArithmeticFlag) == IA32OpProp_SetsSomeArithmeticFlag;}

   uint32_t setsCCForCompare() {return _properties2[_opCode] & IA32OpProp2_SetsCCForCompare;}
   uint32_t setsCCForTest()    {return _properties2[_opCode] & IA32OpProp2_SetsCCForTest;}
   uint32_t isShiftOp()        {return _properties2[_opCode] & IA32OpProp2_ShiftOp;}
   uint32_t isRotateOp()       {return _properties2[_opCode] & IA32OpProp2_RotateOp;}
   uint32_t isPushOp()         {return _properties2[_opCode] & IA32OpProp2_PushOp;}
   uint32_t isPopOp()          {return _properties2[_opCode] & IA32OpProp2_PopOp;}
   uint32_t isCallOp()         {return isCallOp(_opCode); }
   uint32_t isCallImmOp()      {return isCallImmOp(_opCode); }

   uint32_t supportsLockPrefix() {return _properties2[_opCode] & IA32OpProp2_SupportsLockPrefix;}

   bool     isConditionalBranchOp() {return isBranchOp() && testsSomeFlag();}

   uint32_t hasPopInstruction() {return _properties[_opCode] & IA32OpProp_HasPopInstruction;}

   uint32_t isPopInstruction() {return _properties[_opCode] & IA32OpProp_IsPopInstruction;}

   uint32_t sourceOpTarget() {return _properties[_opCode] & IA32OpProp_SourceOpTarget;}

   uint32_t hasDirectionBit() {return _properties[_opCode] & IA32OpProp_HasDirectionBit;}

   uint32_t isAMD64DeprecatedDec() {return _properties2[_opCode] & IA32OpProp2_IsAMD64DeprecatedDec;}

   uint32_t isAMD64DeprecatedInc() {return _properties2[_opCode] & IA32OpProp2_IsAMD64DeprecatedInc;}

   uint32_t isIA32Only() {return _properties2[_opCode] & IA32OpProp2_IsIA32Only;}

   uint32_t sourceIsMemRef() {return _properties2[_opCode] & IA32OpProp2_SourceIsMemRef;}

   uint32_t targetRegIsImplicit() { return _properties2[_opCode] & IA32OpProp2_TargetRegIsImplicit;}
   uint32_t sourceRegIsImplicit() { return _properties2[_opCode] & IA32OpProp2_SourceRegIsImplicit;}

   uint32_t isFusableCompare(){ return _properties2[_opCode] & IA32OpProp2_FusableCompare; }

   bool     isSetRegInstruction() {return !( (( *(uint32_t *)&_binaryEncodings[_opCode] ) & 0x00fff0ff) ^ 0x00c0900f );}

   uint8_t *copyBinaryToBuffer(uint8_t *cursor) {return copyBinaryToBuffer(_opCode, cursor);}

   uint8_t getOpCodeLength() {return _binaryEncodings[_opCode]._length;}

   void convertLongBranchToShort()
      { // input must be a long branch in range JA4 - JMP4
      if (((int)_opCode >= (int)JA4) && ((int)_opCode <= (int)JMP4))
         _opCode = (TR_X86OpCodes)((int)_opCode - IA32LongToShortBranchConversionOffset);
      }

   uint8_t getModifiedEFlags() {return getModifiedEFlags(_opCode); }

   uint8_t getTestedEFlags() {return getTestedEFlags(_opCode); }

   void trackUpperBitsOnReg(TR::Register *reg, TR::CodeGenerator *cg);

   static uint32_t modifiesTarget(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesTarget;}

   static uint32_t modifiesSource(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesSource;}

   static uint32_t usesTarget(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_UsesTarget;}

   static uint32_t singleFPOp(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_SingleFP;}

   static uint32_t doubleFPOp(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_DoubleFP;}

   static uint32_t gprOp(TR_X86OpCodes op) {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP)) == 0;}

   static uint32_t fprOp(TR_X86OpCodes op) {return (_properties[op] & (IA32OpProp_DoubleFP | IA32OpProp_SingleFP));}

   static uint32_t hasByteImmediate(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ByteImmediate;}

   static uint32_t hasShortImmediate(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ShortImmediate;}

   static uint32_t hasIntImmediate(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_IntImmediate;}

   static uint32_t hasLongImmediate(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_LongImmediate;}

   static uint32_t hasSignExtendImmediate(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_SignExtendImmediate;}

   static uint32_t testsZeroFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TestsZeroFlag;}
   static uint32_t modifiesZeroFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesZeroFlag;}
   static uint32_t referencesZeroFlag(TR_X86OpCodes op) {return _properties[op] & (IA32OpProp_ModifiesZeroFlag | IA32OpProp_TestsZeroFlag);}

   static uint32_t testsSignFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TestsSignFlag;}
   static uint32_t modifiesSignFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesSignFlag;}
   static uint32_t referencesSignFlag(TR_X86OpCodes op) {return _properties[op] & (IA32OpProp_ModifiesSignFlag | IA32OpProp_TestsSignFlag);}

   static uint32_t testsCarryFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TestsCarryFlag;}
   static uint32_t modifiesCarryFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesCarryFlag;}
   static uint32_t referencesCarryFlag(TR_X86OpCodes op) {return _properties[op] & (IA32OpProp_ModifiesCarryFlag | IA32OpProp_TestsCarryFlag);}

   static uint32_t testsOverflowFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TestsOverflowFlag;}
   static uint32_t modifiesOverflowFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesOverflowFlag;}
   static uint32_t referencesOverflowFlag(TR_X86OpCodes op) {return _properties[op] & (IA32OpProp_ModifiesOverflowFlag | IA32OpProp_TestsOverflowFlag);}

   static uint32_t testsParityFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TestsParityFlag;}
   static uint32_t modifiesParityFlag(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ModifiesParityFlag;}
   static uint32_t referencesParityFlag(TR_X86OpCodes op) {return _properties[op] & (IA32OpProp_ModifiesParityFlag | IA32OpProp_TestsParityFlag);}

   static uint32_t hasByteSource(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ByteSource;}

   static uint32_t hasByteTarget(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ByteTarget;}

   static uint32_t hasShortSource(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ShortSource;}

   static uint32_t hasShortTarget(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_ShortTarget;}

   static uint32_t hasIntSource(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_IntSource;}

   static uint32_t hasIntTarget(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_IntTarget;}

   static uint32_t hasLongSource(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_LongSource;}

   static uint32_t hasLongTarget(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_LongTarget;}

   static uint32_t hasDoubleWordSource(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_DoubleWordSource;}

   static uint32_t hasDoubleWordTarget(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_DoubleWordTarget;}

   static uint32_t hasXMMSource(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_XMMSource;}

   static uint32_t hasXMMTarget(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_XMMTarget;}

   static uint32_t isPseudoOp(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_PseudoOp;}

   static uint8_t getSourceOperandSize(TR_X86OpCodes op)
      {
      if (hasByteSource(op) || hasByteImmediate(op) || hasSignExtendImmediate(op))
         return 1;
      else if (hasShortSource(op) || hasShortImmediate(op))
         return 2;
      else if (hasIntSource(op) || hasIntImmediate(op))
         return 4;
      else if (hasLongSource(op) || hasLongImmediate(op))
         return 8;
      else
         return getTargetOperandSize(op); // make best guess
      }

   static uint8_t getTargetOperandSize(TR_X86OpCodes op)
      {
      if (hasByteTarget(op))
         return 1;
      else if (hasShortTarget(op))
         return 2;
      else if (hasIntTarget(op))
         return 4;
      else if (hasLongTarget(op))
         return 8;
      else
         return 1; // least likely to cause an exception
      }

   static uint8_t getImmediateSize(TR_X86OpCodes op)
      {
      if (hasByteImmediate(op) || hasSignExtendImmediate(op))
         return 1;
      else if (hasShortImmediate(op))
         return 2;
      else if (hasIntImmediate(op))
         return 4;
      else if (hasLongImmediate(op))
         return 8;
      else
         return 0;
      }

   static uint32_t needs64BitOperandPrefix(TR_X86OpCodes op)
      {
      // This #ifdef should cause this function (and many of its callers) to
      // compile away on IA32.
      //
#ifdef TR_TARGET_64BIT
      return _properties2[op] & IA32OpProp2_Needs64BitOperandPrefix;
#else
      TR_ASSERT(!(_properties2[op] & IA32OpProp2_Needs64BitOperandPrefix), "Don't use 64-bit opcodes on 32-bit targets");
      return 0;
#endif
      }

   static uint32_t needs16BitOperandPrefix(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_Needs16BitOperandPrefix;}

   static uint32_t needsScalarPrefix(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_NeedsScalarPrefix;}

   static uint32_t needsRepPrefix(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_NeedsRepPrefix;}

   static uint32_t hasTargetRegisterInOpcode(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TargetRegisterInOpcode;}

   static uint32_t hasTargetRegisterInModRM(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TargetRegisterInModRM;}

   static uint32_t hasTargetRegisterIgnored(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_TargetRegisterIgnored;}

   static uint32_t hasSourceRegisterInModRM(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_SourceRegisterInModRM;}

   static uint32_t hasSourceRegisterIgnored(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_SourceRegisterIgnored;}

   static uint32_t isBranchOp(TR_X86OpCodes op) {return _properties[op] & IA32OpProp_BranchOp;}
   static uint32_t hasRelativeBranchDisplacement(TR_X86OpCodes op) { return isBranchOp(op) || isCallImmOp(op); }

   static uint32_t setsCCForCompare(TR_X86OpCodes op) {return _properties2[op] & IA32OpProp2_SetsCCForCompare;}
   static uint32_t setsCCForTest(TR_X86OpCodes op)    {return _properties2[op] & IA32OpProp2_SetsCCForTest;}
   static uint32_t isShiftOp(TR_X86OpCodes op)        {return _properties2[op] & IA32OpProp2_ShiftOp;}
   static uint32_t isRotateOp(TR_X86OpCodes op)       {return _properties2[op] & IA32OpProp2_RotateOp;}
   static uint32_t isPushOp(TR_X86OpCodes op)         {return _properties2[op] & IA32OpProp2_PushOp;}
   static uint32_t isPopOp(TR_X86OpCodes op)          {return _properties2[op] & IA32OpProp2_PopOp;}
   static uint32_t isCallOp(TR_X86OpCodes op)         {return _properties2[op] & IA32OpProp2_CallOp;}
   static uint32_t isCallImmOp(TR_X86OpCodes op)      {return op == CALLImm4 || op == CALLREXImm4; }

   static uint32_t supportsLockPrefix(TR_X86OpCodes op)     {return _properties2[op] & IA32OpProp2_SupportsLockPrefix;}

   static uint32_t targetRegIsImplicit(TR_X86OpCodes op)  {return _properties2[op] & IA32OpProp2_TargetRegIsImplicit;}
   static uint32_t sourceRegIsImplicit(TR_X86OpCodes op)  {return _properties2[op] & IA32OpProp2_SourceRegIsImplicit;}

   static uint32_t isFusableCompare(TR_X86OpCodes op){ return _properties2[op] & IA32OpProp2_FusableCompare; }

   static uint8_t *copyBinaryToBuffer(TR_X86OpCodes op, uint8_t *cursor)
      {
      *(uint32_t *)cursor = (*(uint32_t *)&_binaryEncodings[op]) & 0xffffff;
      return cursor + _binaryEncodings[op]._length;
      }

   static uint8_t getOpCodeLength(TR_X86OpCodes op) {return _binaryEncodings[op]._length;}

   static uint8_t getModifiedEFlags(TR_X86OpCodes op)
      {
      uint8_t flags = 0;

      if (modifiesOverflowFlag(op)) flags |= IA32EFlags_OF;
      if (modifiesSignFlag(op))     flags |= IA32EFlags_SF;
      if (modifiesZeroFlag(op))     flags |= IA32EFlags_ZF;
      if (modifiesParityFlag(op))   flags |= IA32EFlags_PF;
      if (modifiesCarryFlag(op))    flags |= IA32EFlags_CF;

      return flags;
      }

   static uint8_t getTestedEFlags(TR_X86OpCodes op)
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

#endif

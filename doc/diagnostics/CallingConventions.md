<!--
Copyright (c) 2020, 2020 IBM Corp. and others

This program and the accompanying materials are made available under
the terms of the Eclipse Public License 2.0 which accompanies this
distribution and is available at http://eclipse.org/legal/epl-2.0
or the Apache License, Version 2.0 which accompanies this distribution
and is available at https://www.apache.org/licenses/LICENSE-2.0.

This Source Code may also be made available under the following Secondary
Licenses when the conditions for such availability set forth in the
Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
version 2 with the GNU Classpath Exception [1] and GNU General Public
License, version 2 with the OpenJDK Assembly Exception [2].

[1] https://www.gnu.org/software/classpath/license.html
[2] http://openjdk.java.net/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
-->

# x86

## x86-64

### Linux

|Register|System Linkage|Caller Preserved<sup>1</sup>|
|--|--|--|
|RAX|1st int return value |:heavy_check_mark:|
|RBX|base pointer (optional) ||
|RCX|4th int argument |:heavy_check_mark:|
|RDX|3rd int argument; 2nd int return value |:heavy_check_mark:|
|RSP|stack pointer ||
|RBP|frame pointer (optional) ||
|RSI|2nd int argument |:heavy_check_mark:|
|RDI|1st int argument |:heavy_check_mark:|
|R8|5th int argument |:heavy_check_mark:|
|R9|6th int argument |:heavy_check_mark:|
|R10||:heavy_check_mark:|
|R11||:heavy_check_mark:|
|R12|||
|R13|||
|R14|||
|R15|||
|XMM0|1st float argument; 1st float return value |:heavy_check_mark:|
|XMM1|2nd float argument; 2nd float return value |:heavy_check_mark:|
|XMM2|3rd float argument |:heavy_check_mark:|
|XMM3|4th float argument |:heavy_check_mark:|
|XMM4|5th float argument |:heavy_check_mark:|
|XMM5|6th float argument |:heavy_check_mark:|
|XMM6|7th float argument |:heavy_check_mark:|
|XMM7|8th float argument |:heavy_check_mark:|
|XMM8||:heavy_check_mark:|
|XMM9||:heavy_check_mark:|
|XMM10||:heavy_check_mark:|
|XMM11||:heavy_check_mark:|
|XMM12||:heavy_check_mark:|
|XMM13||:heavy_check_mark:|
|XMM14||:heavy_check_mark:|
|XMM15||:heavy_check_mark:|


### Windows

|Register|System Linkage|Caller Preserved<sup>1</sup>|
|--|--|--|
|RAX|return value |:heavy_check_mark:|
|RBX|||
|RCX|1st argument |:heavy_check_mark:|
|RDX|2nd argument |:heavy_check_mark:|
|RSP|stack pointer ||
|RBP|frame pointer (optional) ||
|RSI|||
|RDI|||
|R8|3rd argument |:heavy_check_mark:|
|R9|4th argument |:heavy_check_mark:|
|R10||:heavy_check_mark:|
|R11||:heavy_check_mark:|
|R12|||
|R13|||
|R14|||
|R15|||
|XMM0|1st float argument; return value |:heavy_check_mark:|
|XMM1|2nd float argument |:heavy_check_mark:|
|XMM2|3rd float argument |:heavy_check_mark:|
|XMM3|4th float argument |:heavy_check_mark:|
|XMM4||:heavy_check_mark:|
|XMM5||:heavy_check_mark:|
|XMM6|||
|XMM7|||
|XMM8|||
|XMM9|||
|XMM10|||
|XMM11|||
|XMM12|||
|XMM13|||
|XMM14|||
|XMM15|||

<hr/>
1. The remaining registers are Callee Preserved.

## IA32

For historical reasons, there exist many different calling conventions for IA32, the most popular of which are cdecl (which stands for C declaration, caller clean-up call), stdcall (callee clean-up call) and thiscall (either caller or callee clean-up call).

# POWER

Scratch registers are not preserved across calls, while non-volatile registers are preserved by called functions.

|Register|System linkage|
|--|--|
|R0|Scratch|
|R1|System Stack Pointer|
|R2|Library TOC|
|R3|1st argument/return value|
|R4|2nd argument/low-order portion of 64-bit return values in 32-bit mode|
|R5-10|3rd-8th arguments; the 9th and above arguments are passed on the stack|
|R11|Scratch (frequently used as a temp for call target address)|
|R12|Scratch|
|R13|Non-volatile; OS dedicated (64-bit)|
|R14|Non-volatile|
|R15|Non-volatile|
|R16|Non-volatile|
|R17-31|Non-volatile|
|IAR|Instruction Address Register (a.k.a PC or NIP on Linux)|
|LR|Link Register (used to pass the return address to a caller)|
|CTR| Count Register (used for calling a far/variable target)|
|VSR32-63|Vector-Scala Register|
|CR0|Condition Register (used by compare, branch and record-form instructions)|
|CR1-7|Condition Register (used by compare and branch instructions)|
|FP0|1st Floating Point argument / Floating point return value|
|FP1-7|2nd-8th Floating Point argument|
|FP8-13|9th-14th Floating Point argument|
|FP14-31|Non-volatile|

<hr/>

Useful Links:
* [General PowerPC Architecture](https://cr.yp.to/2005-590/powerpc-cwg.pdf)
* More complete with pictorial examples of [AIX Subroutine Linkage Convention](https://www.ibm.com/support/knowledgecenter/ssw_aix_72/assembler/idalangref_subrutine_link_conv.html)
* R2 is TOC? What's a TOC? [Understanding and Programming the TOC](https://www.ibm.com/support/knowledgecenter/ssw_aix_72/assembler/idalangref_und_prg_toc.html)
* [Assembler Language Reference](https://www.ibm.com/support/knowledgecenter/ssw_aix_71/assembler/alangref_kickoff.html)

# Z

## zLinux

### 64-bit

|Register|System Linkage|Callee Preserved|
|--|--|--|
|R0|||
|R1|||
|R2|parameter/return value||
|R3|parameter||
|R4|parameter||
|R5|parameter||
|R6|parameter |:heavy_check_mark:|
|R7||:heavy_check_mark:|
|R8-R11||:heavy_check_mark:|
|R12|GOT |:heavy_check_mark:|
|R13|literal pool |:heavy_check_mark:|
|R14|return address||
|R15|stack pointer |:heavy_check_mark:|
|F0|parameter/return value||
|F1|||
|F2|parameter/return value||
|F3|||
|F4|parameter/return value||
|F5|||
|F6|parameter/return value||
|F7|||
|F8-F15||:heavy_check_mark:|


### 31-bit

|Register|System Linkage|Callee Preserved|
|--|--|--|
|R0|||
|R1|||
|R2|parameter/return value||
|R3|parameter/return value||
|R4|parameter||
|R5|parameter||
|R6|parameter |:heavy_check_mark:|
|R7||:heavy_check_mark:|
|R8-R11||:heavy_check_mark:|
|R12|GOT |:heavy_check_mark:|
|R13|literal pool |:heavy_check_mark:|
|R14|return address||
|R15|stack pointer |:heavy_check_mark:|
|F0|parameter/return value||
|F1|||
|F2|parameter/return value||
|F3|||
|F4||:heavy_check_mark:|
|F5|||
|F6||:heavy_check_mark:|
|F7|||
|F8-F15|||

## z/OS

|Register|System Linkage|Callee Preserved|
|--|--|--|
|R0|||
|R1|parameter/extended return value<sup>1</sup>||
|R2|parameter/extended return value<sup>1</sup>||
|R3|parameter/return value<sup>1</sup>||
|R4|biased DSA (address of stack frame minus 2048 bytes)||
|R5|address of callee environment or scope's stack frame (internal functions)||
|R6|entry point |:heavy_check_mark:|
|R7|return address |:heavy_check_mark:|
|R8-R11||:heavy_check_mark:|
|R12|CAA |:heavy_check_mark:|
|R13||:heavy_check_mark:|
|R14||:heavy_check_mark:|
|R15||:heavy_check_mark:|
|F0|parameter/return value||
|F1|||
|F2|parameter/return value||
|F3|||
|F4|parameter/return value||
|F5|||
|F6|parameter/return value||
|F7|||
|F8-F15||:heavy_check_mark:|

In XPLINK, GPR4 (the biased stack pointer) is not saved by the callee in storage. It's recalculated on callee return. So from the caller's POV, it's a preserved register.

<hr/>

1. The XPLINK specification states that, in AMODE31, integral and 32-bit pointer data types are returned in GPR3, and 64-bit integral types are returned in GPR2 (high half) and GPR3 (low half). In AMODE64, all integral and pointer data types are returned in GPR3. Aggregate or packed decimal types are left-adjusted, and returned in GPR1, but can occupy GPR2 and GPR3 as well if the size of the data exceeds 4 bytes (or 8 bytes for AMODE64).

Useful links:
* [z/Architecture Principles of Operation](http://publibfi.boulder.ibm.com/epubs/pdf/dz9zr008.pdf)
* [z/Architecture Reference Summary](http://publibfi.boulder.ibm.com/epubs/pdf/dz9zs006.pdf)

# 32-bit Arm

## Linux

Scratch registers are not preserved across calls, while non-volatile registers are preserved by called functions.
Usage of the VFP registers in this table assumes hard-float ABI.

|Register|System Linkage|
|--|--|
|R0|1st argument / return value|
|R1|2nd argument / return value|
|R2-3|3rd-4th arguments|
|R4-10|Non-volatile|
|R11|System Frame Pointer (FP)|
|R12|Scratch; Intra-Procedure-Call Register (IP)|
|R13|System Stack Pointer (SP)|
|R14|Link Register (LR)|
|R15|Program Counter (PC)|
|D0|1st floating point argument / floating point return value|
|D1-7|2nd-8th floating point arguments|
|D8-15|Non-volatile|
|D16-D31|Scratch<sup>1</sup>|

<hr/>

1. Unavailable in some VFP versions

# AArch64 (64-bit Arm)

## Linux

Scratch registers are not preserved across calls, while non-volatile registers are preserved by called functions.

|Register|System Linkage|
|--|--|
|R0|1st argument / return value|
|R1-7|2nd-8th arguments|
|R8-15|Scratch|
|R16-17|Scratch; Intra-Procedure-Call Registers (IP0-1)|
|R18|Scratch; Platform Register|
|R19-28|Non-volatile|
|R29|System Frame Pointer (FP)|
|R30|Link Register (LR)|
|SP|System Stack Pointer|
|PC|Program Counter (PC)|
|V0|1st floating point argument / floating point return value|
|V1-7|2nd-8th floating point arguments|
|V8-15|Non-volatile|
|V16-31|Scratch|

# RISC-V

| Register     | System Linkage         | Callee Preserved    |
|--------------|------------------------|---------------------|
| zero         | Zero                   |                     |
| ra           | Return address         |                     |
| sp           | Stack pointer          | :heavy_check_mark:  |
| gp           | Global pointer         |                     |
| tp           | Thread pointer         |                     |
| t0-t2        | Temporary register     |                     |
| s0-s1        | Callee-saved registers | :heavy_check_mark:  |
| a0           | Argument / Return reg. |                     |
| a1           | Argument / Return reg. |                     |
| a2-a7        | Argument registers     |                     |
| s2-s11       | Callee-saved registers | :heavy_check_mark:  |
| t3-t6        | Temporary registers    |                     |
|              |                        |                     |
| ft0-ft7      | Temporary registers    |                     |
| fs0-fs1      | Callee-saved registers | :heavy_check_mark:  |
| fa0          | Argument registers     |                     |
| fa1          | Argument registers     |                     |
| fa2-fa7      | Argument registers     |                     |
| fs2-fs11     | Callee-saved registers | :heavy_check_mark:  |
| ft8-ft11     | Temporary registers    |                     |

Useful links:

 * [RISC-V ELF psABI specification](https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#register-convention)
 * [RVSystemLinkage.cpp](https://github.com/eclipse/omr/blob/master/compiler/riscv/codegen/RVSystemLinkage.cpp)

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

/*
 * This file will be included within an enum.  Only comments and enumerator
 * definitions are permitted.
 */

NoReg = 0,

    // The order of the GPRS registers is defined by the linkage
    // convention, and the VM may rely on it (eg. for GC).
    eax = 1, FirstGPR = eax, ebx = 2, ecx = 3, edx = 4, Last8BitGPR = edx, edi = 5, esi = 6, ebp = 7,
    LastAssignableGPR = ebp, esp = 8, LastGPR = esp,

    vfp = 9,

    // st0Return is used for 32-bit linkages where floating point values are
    // returned in x87 register st0. It is a special register that cannot be
    // assigned.
    //
    st0Return = 10,

    xmm0 = 11, FirstXMMR = xmm0, xmm1 = 12, xmm2 = 13, xmm3 = 14, xmm4 = 15, xmm5 = 16, xmm6 = 17, xmm7 = 18,
    LastXMMR = xmm7,

    // Map the x87 registers st0-st7 over top of the xmm registers. This will
    // aid downstream removal of st0-st7. This mapping will be removed once
    // that refactoring is complete.
    //
    st0 = xmm0, FirstFPR = st0, st1 = xmm1, st2 = xmm2, st3 = xmm3, st4 = xmm4, st5 = xmm5, st6 = xmm6, st7 = xmm7,
    LastAssignableFPR = st7,

    ymm0 = xmm0, FirstYMMR = ymm0, ymm1 = xmm1, ymm2 = xmm2, ymm3 = xmm3, ymm4 = xmm4, ymm5 = xmm5, ymm6 = xmm6,
    ymm7 = xmm7, LastYMMR = ymm7,

    zmm0 = xmm0, FirstZMMR = zmm0, zmm1 = xmm1, zmm2 = xmm2, zmm3 = xmm3, zmm4 = xmm4, zmm5 = xmm5, zmm6 = xmm6,
    zmm7 = xmm7, LastZMMR = zmm7,

    // avx512 write mask registers
    // k0 -> reserved for unmasked operations
    k0 = 19, k1 = 20, k2 = 21, k3 = 22, k4 = 23, k5 = 24, k6 = 25, k7 = 26,

    ByteReg = 27, BestFreeReg = 28, SpilledReg = 29, NumRegisters = 30,

    NumXMMRegisters = LastXMMR - FirstXMMR + 1,
    MaxAssignableRegisters = NumXMMRegisters + (LastAssignableGPR - FirstGPR + 1) - 1 // -1 for stack pointer

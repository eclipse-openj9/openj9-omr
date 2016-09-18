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

/*
 * This file will be included within an enum.  Only comments and enumerator
 * definitions are permitted.
 */

   NoReg                   = 0,

   // The order of the GPRS registers is defined by the linkage
   // convention, and the VM may rely on it (eg. for GC).
   eax                     = 1,
   FirstGPR                = eax,
   ebx                     = 2,
   ecx                     = 3,
   edx                     = 4,
   edi                     = 5,
   esi                     = 6,
   ebp                     = 7,
   esp                     = 8,
   r8                      = 9,
   r9                      = 10,
   r10                     = 11,
   r11                     = 12,
   r12                     = 13,
   r13                     = 14,
   r14                     = 15,
   r15                     = 16,
   LastGPR                 = r15,
   Last8BitGPR             = r15,
   LastAssignableGPR       = r15,

   vfp                     = 17,

   FPRMaskOffset           = LastGPR,
   st0                     = 18,
   FirstFPR                = st0,
   st1                     = 19,
   st2                     = 20,
   st3                     = 21,
   st4                     = 22,
   st5                     = 23,
   st6                     = 24,
   st7                     = 25,
   LastFPR                 = st7,
   LastAssignableFPR       = st7,

   mm0                     = 26,
   FirstMMXR               = mm0,
   mm1                     = 27,
   mm2                     = 28,
   mm3                     = 29,
   mm4                     = 30,
   mm5                     = 31,
   mm6                     = 32,
   mm7                     = 33,
   LastMMXR                = mm7,

   XMMRMaskOffset          = LastGPR,
   xmm0                    = 34,
   FirstXMMR               = xmm0,
   xmm1                    = 35,
   xmm2                    = 36,
   xmm3                    = 37,
   xmm4                    = 38,
   xmm5                    = 39,
   xmm6                    = 40,
   xmm7                    = 41,
   xmm8                    = 42,
   xmm9                    = 43,
   xmm10                   = 44,
   FirstSpillReg           = xmm10,
   xmm11                   = 45,
   xmm12                   = 46,
   xmm13                   = 47,
   xmm14                   = 48,
   xmm15                   = 49,
   LastSpillReg            = xmm15,
   LastXMMR                = xmm15,

   AllFPRegisters          = 50,
   ByteReg                 = 51,
   BestFreeReg             = 52,
   SpilledReg              = 53,
   NumRegisters            = 54,

   NumXMMRegisters         = LastXMMR - FirstXMMR + 1,
   MaxAssignableRegisters  = NumXMMRegisters + (LastAssignableGPR - FirstGPR + 1) - 1 // -1 for stack pointer

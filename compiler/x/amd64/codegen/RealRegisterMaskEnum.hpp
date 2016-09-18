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

   noRegMask         = 0x00000000,

   // GPR
   //
   eaxMask           = 0x00000001,
   ebxMask           = 0x00000002,
   ecxMask           = 0x00000004,
   edxMask           = 0x00000008,
   ediMask           = 0x00000010,
   esiMask           = 0x00000020,
   ebpMask           = 0x00000040,
   espMask           = 0x00000080,
   r8Mask            = 0x00000100,
   r9Mask            = 0x00000200,
   r10Mask           = 0x00000400,
   r11Mask           = 0x00000800,
   r12Mask           = 0x00001000,
   r13Mask           = 0x00002000,
   r14Mask           = 0x00004000,
   r15Mask           = 0x00008000,
   AvailableGPRMask  = 0x0000FFFF,

   // FPR
   //
   st0Mask           = 0x00000001,
   st1Mask           = 0x00000002,
   st2Mask           = 0x00000004,
   st3Mask           = 0x00000008,
   st4Mask           = 0x00000010,
   st5Mask           = 0x00000020,
   st6Mask           = 0x00000040,
   st7Mask           = 0x00000080,
   AvailableFPRMask  = 0x000000FF,

   // MMXR
   //
   mm0Mask           = 0x00010000,
   mm1Mask           = 0x00020000,
   mm2Mask           = 0x00040000,
   mm3Mask           = 0x00080000,
   mm4Mask           = 0x00100000,
   mm5Mask           = 0x00200000,
   mm6Mask           = 0x00400000,
   mm7Mask           = 0x00800000,
   AvailableMMRMask  = 0x00FF0000,

   // XMMR
   //
   xmm0Mask          = 0x00000001 << XMMRMaskOffset,
   xmm1Mask          = 0x00000002 << XMMRMaskOffset,
   xmm2Mask          = 0x00000004 << XMMRMaskOffset,
   xmm3Mask          = 0x00000008 << XMMRMaskOffset,
   xmm4Mask          = 0x00000010 << XMMRMaskOffset,
   xmm5Mask          = 0x00000020 << XMMRMaskOffset,
   xmm6Mask          = 0x00000040 << XMMRMaskOffset,
   xmm7Mask          = 0x00000080 << XMMRMaskOffset,
   xmm8Mask          = 0x00000100 << XMMRMaskOffset,
   xmm9Mask          = 0x00000200 << XMMRMaskOffset,
   xmm10Mask         = 0x00000400 << XMMRMaskOffset,
   xmm11Mask         = 0x00000800 << XMMRMaskOffset,
   xmm12Mask         = 0x00001000 << XMMRMaskOffset,
   xmm13Mask         = 0x00002000 << XMMRMaskOffset,
   xmm14Mask         = 0x00004000 << XMMRMaskOffset,
   xmm15Mask         = 0x00008000 << XMMRMaskOffset,
   AvailableXMMRMask = 0x0000FFFF << XMMRMaskOffset,


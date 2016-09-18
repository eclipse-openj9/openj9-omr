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

#include "codegen/RealRegister.hpp"  // for RealRegister, etc

const OMR::X86::RealRegister::TR_RegisterBinaryEncoding OMR::X86::i386::RealRegister::_fullRegisterBinaryEncodings[TR::RealRegister::NumRegisters] =
   {
   // -----------id
   // |   --------needsRexPlusRXB
   // |   |  ------needsRexForByte
   // |   |  |  ----needsDisp
   // |   |  |  |  --needsSib
   // |   |  |  |  |
   // V   V  V  V  V
   {0x00, 0, 0},        // NoReg
   {0x00, 0, 0},        // eax
   {0x03, 0, 0},        // ebx
   {0x01, 0, 0},        // ecx
   {0x02, 0, 0},        // edx
   {0x07, 0, 1},        // edi
   {0x06, 0, 1},        // esi
   {0x05, 0, 1, 1, 0},  // ebp
   {0x04, 0, 1, 0, 1},  // esp
   {0x00},              // vfp
   {0x00},              // st0
   {0x01},              // st1
   {0x02},              // st2
   {0x03},              // st3
   {0x04},              // st4
   {0x05},              // st6
   {0x06},              // st6
   {0x07},              // st7
   {0x00},              // mm0
   {0x01},              // mm1
   {0x02},              // mm2
   {0x03},              // mm3
   {0x04},              // mm4
   {0x05},              // mm6
   {0x06},              // mm6
   {0x07},              // mm7
   {0x00, 0, 0},        // xmm0
   {0x01, 0, 0},        // xmm1
   {0x02, 0, 0},        // xmm2
   {0x03, 0, 0},        // xmm3
   {0x04, 0, 0},        // xmm4
   {0x05, 0, 0},        // xmm6
   {0x06, 0, 0},        // xmm6
   {0x07, 0, 0},        // xmm7
   };


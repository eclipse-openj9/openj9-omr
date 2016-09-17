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

#include "codegen/RealRegister.hpp"

const uint8_t OMR::ARM::RealRegister::fullRegBinaryEncodings[TR::RealRegister::NumRegisters] =
    {0x00, 	// NoReg
     0x00, 	// r0
     0x01, 	// r1
     0x02, 	// r2
     0x03, 	// r3
     0x04, 	// r4
     0x05, 	// r5
     0x06, 	// r6
     0x07, 	// r7
     0x08, 	// r8
     0x09, 	// r9
     0x0a, 	// r10
     0x0b, 	// r11
     0x0c, 	// r12
     0x0d, 	// r13
     0x0e, 	// r14
     0x0f, 	// r15
     0x00, 	// fp0
     0x01, 	// fp1
     0x02, 	// fp2
     0x03, 	// fp3
     0x04, 	// fp4
     0x05, 	// fp5
     0x06, 	// fp6
     0x07, 	// fp7
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
     0x08, 	// fp8
     0x09, 	// fp9
     0x0a, 	// fp10
     0x0b, 	// fp11
     0x0c, 	// fp12
     0x0d, 	// fp13
     0x0e, 	// fp14
     0x0f, 	// fp15
     // For Single-precision registers group (fs0~fs15/FSR)
     0x00, 	// fs0
     0x01, 	// fs1
     0x02, 	// fs2
     0x03, 	// fs3
     0x04, 	// fs4
     0x05, 	// fs5
     0x06, 	// fs6
     0x07, 	// fs7
     0x08, 	// fs8
     0x09, 	// fs9
     0x0a, 	// fs10
     0x0b, 	// fs11
     0x0c, 	// fs12
     0x0d, 	// fs13
     0x0e, 	// fs14
     0x0f, 	// fs15
#endif
     0x00       // SpilledReg
};

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

#include <stdint.h>              // for uint32_t
#include "x/codegen/X86Ops.hpp"  // for IA32OpProp_ModifiesTarget, etc

const uint32_t TR_X86OpCode::_properties[IA32NumOpCodes] =
   {
   0,                                          // BADIA32Op

   IA32OpProp_ModifiesTarget                 | // ADC1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADC8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADD8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDSSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDSSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDPSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDPSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDSDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDSDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDPDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ADDPDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LADD1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LADD2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LADD4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LADD8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LXADD1MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LXADD2MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LXADD4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LXADD8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // AND8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // BSF2RegReg
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortSource                    |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesZeroFlag,

   IA32OpProp_ModifiesTarget                 | // BSF4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,
   //
   IA32OpProp_ModifiesTarget                 | // BSF8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,
   //
   IA32OpProp_ModifiesTarget                 | // BSR4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,
   //
   IA32OpProp_ModifiesTarget                 | // BSR8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // BSWAP4Reg
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // BSWAP8Reg (AMD64)
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // BTS4RegReg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // BTS4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntImmediate                   | // CALLImm4
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_IntImmediate                   | // CALLREXImm4 (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_IntTarget                      | // CALLReg
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TargetRegisterInModRM,

   IA32OpProp_IntTarget                      | // CALLREXReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TargetRegisterInModRM,

   IA32OpProp_ModifiesOverflowFlag           | // CALLMem
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesOverflowFlag           | // CALLREXMem (AMD64)
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // CBWAcc
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CBWEAcc
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ShortSource                    |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVA4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVB4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVE4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVE8RegMem (64-bit)
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVG4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVL4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVNE4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVNE8RegMem (64-bit)
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVNO4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVNS4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVO4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CMOVS4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntTarget,

   IA32OpProp_TargetRegisterIgnored          | // CMP1AccImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // CMP4AccImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // CMP8AccImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // CMP1RegImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // CMP4RegImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // CMP8RegImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // CMP4RegImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // CMP8RegImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteTarget                     | // CMP1MemImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2MemImm2
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2MemImms
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntTarget                      | // CMP4MemImm4
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntImmediate                   | // CMP8MemImm4 (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntTarget                      | // CMP4MemImms
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SignExtendImmediate            | // CMP8MemImms (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // CMP1RegReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // CMP2RegReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntSource                      | // CMP4RegReg
   IA32OpProp_IntTarget                      |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // CMP8RegReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteSource                     | // CMP1RegMem
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2RegMem
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntSource                      | // CMP4RegMem
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesOverflowFlag           | // CMP8RegMem (AMD64)
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteSource                     | // CMP1MemReg
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // CMP2MemReg
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntSource                      | // CMP4MemReg
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesOverflowFlag           | // CMP8MemReg (AMD64)
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG1MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG2MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG8BMem (IA32 only)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CMPXCHG16BMem (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG1MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG2MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG8BMem (IA32 only)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LCMPXCHG16BMem (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XALCMPXCHG8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XACMPXCHG8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XALCMPXCHG4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XACMPXCHG4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SSRegReg4
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SSRegReg8 (AMD64)
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SSRegMem8 (AMD64)
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SDRegReg4
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SDRegReg8 (AMD64)
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTSI2SDRegMem8 (AMD64)
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // CVTTSS2SIReg4Reg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CVTTSS2SIReg8Reg (AMD64)
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTTSS2SIReg4Mem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CVTTSS2SIReg8Mem (AMD64)
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTTSD2SIReg4Reg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CVTTSD2SIReg8Reg (AMD64)
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTTSD2SIReg4Mem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // CVTTSD2SIReg8Mem (AMD64)
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // CVTSS2SDRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTSS2SDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CVTSD2SSRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CVTSD2SSRegMem
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // CWDAcc
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // CDQAcc
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // CQOAcc (AMD64)
   IA32OpProp_TargetRegisterIgnored,

   IA32OpProp_ModifiesTarget                 | // DEC1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC2Reg
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC4Reg
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC8Reg (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC1Mem
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC2Mem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC4Mem
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DEC8Mem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FABSReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DABSReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSQRTReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSQRTReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FADDRegReg
   IA32OpProp_UsesTarget                     |
   IA32OpProp_SingleFP                       |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // DADDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // FADDPReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FADDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DADDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FIADDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIADDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSADDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSADDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FCHSReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DCHSReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FDIVRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // DDIVRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // FDIVRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DDIVRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FDIVPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FIDIVRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIDIVRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSDIVRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSDIVRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FDIVRRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction              |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_SourceOpTarget,

   IA32OpProp_ModifiesTarget                 | // DDIVRRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction              |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_SourceOpTarget,

   IA32OpProp_ModifiesTarget                 | // FDIVRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DDIVRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FDIVRPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FIDIVRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIDIVRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSDIVRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSDIVRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FILDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // DILDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // FLLDRegMem
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DLLDRegMem
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FSLDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // DSLDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // FISTMemReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // DISTMemReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // FISTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // DISTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // FLSTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DLSTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FSSTMemReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // DSSTMemReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // FSSTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // DSSTPMem
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortTarget,

   0,                                          // FLDLN2

   IA32OpProp_ModifiesTarget                 | // FLDRegReg
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DLDRegReg
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FLDRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // DLDRegMem
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FLD0Reg
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DLD0Reg
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FLD1Reg
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DLD1Reg
   IA32OpProp_DoubleFP,

   IA32OpProp_UsesTarget                     | // FLDMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_UsesTarget                     | // DLDMem
   IA32OpProp_DoubleFP,

   IA32OpProp_ShortTarget,                     // LDCWMem

   IA32OpProp_ModifiesTarget                 | // FMULRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // DMULRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // FMULPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FMULRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DMULRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FIMULRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIMULRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSMULRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSMULRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   0,                                          // FNCLEX

   0,                                          // FPREMRegReg

   0,                                          // FSCALERegReg

   IA32OpProp_ModifiesTarget                 | // FSTMemReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // DSTMemReg
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FSTRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // DSTRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // FSTPMemReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // DSTPMemReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // FSTPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // DSTPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // STCWMem
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // STSWMem
   IA32OpProp_ShortTarget,

   IA32OpProp_ShortTarget                    | // STSWAcc
   IA32OpProp_TargetRegisterIgnored,

   IA32OpProp_ModifiesTarget                 | // FSUBRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // DSUBRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_HasPopInstruction,

   IA32OpProp_ModifiesTarget                 | // FSUBRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSUBRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSUBPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FISUBRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DISUBRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSSUBRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSSUBRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSUBRRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction              |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_SourceOpTarget,

   IA32OpProp_ModifiesTarget                 | // DSUBRRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget                     |
   IA32OpProp_HasPopInstruction              |
   IA32OpProp_HasDirectionBit                |
   IA32OpProp_SourceOpTarget,

   IA32OpProp_ModifiesTarget                 | // FSUBRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSUBRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSUBRPReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FISUBRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DISUBRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FSSUBRRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DSSUBRRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ShortSource                    |
   IA32OpProp_UsesTarget,

   0,                                          // FTSTReg

   IA32OpProp_HasPopInstruction              | // FCOMRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_HasPopInstruction              | // DCOMRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_HasPopInstruction              | // FCOMRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_HasPopInstruction              | // DCOMRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_HasPopInstruction              | // FCOMPReg
   IA32OpProp_IsPopInstruction               |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntSource                      | // FCOMPMem
   IA32OpProp_SingleFP,

   IA32OpProp_DoubleFP,                        // DCOMPMem

   0,                                          // FCOMPP

   IA32OpProp_HasPopInstruction              | // FCOMIRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,
//   IA32OpProp_UsesSource,

   IA32OpProp_HasPopInstruction              | // DCOMIRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,
//   IA32OpProp_UsesSource,

   IA32OpProp_IsPopInstruction               | // FCOMIPReg
   IA32OpProp_UsesTarget                     |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   0,                                          // FYL2X

   IA32OpProp_SourceRegisterInModRM          | // UCOMISSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SingleFP                       | // UCOMISSRegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // UCOMISDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_DoubleFP                       | // UCOMISDRegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // FXCHReg
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV1AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV2AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV4AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV8AccReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIV4AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIV8AccReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV1AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV2AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV4AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IDIV8AccMem (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIV4AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIV8AccMem (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIVSSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIVSSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIVSDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // DIVSDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL1AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL2AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL4AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL8AccReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL1AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL2AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL4AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL8AccMem (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegRegImm2
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegRegImms
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegRegImm4
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegRegImm4 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegRegImms
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegRegImms (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegMemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL2RegMemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegMemImm4
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegMemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL4RegMemImms
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // IMUL8RegMemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // MUL1AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL2AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL4AccReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL8AccReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL1AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL2AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL4AccMem
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MUL8AccMem (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULSSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULSSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULPSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULPSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULSDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULSDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULPDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // MULPDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC2Reg
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC4Reg
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC8Reg (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC1Mem
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC2Mem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC4Mem
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // INC8Mem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_UsesTarget,

   IA32OpProp_BranchOp                       | // JA1
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JAE1
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JB1
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JBE1
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JE1
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JNE1
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JG1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JGE1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JL1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JLE1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JO1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JNO1
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JS1
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JNS1
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JPO1
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JPE1
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JMP1
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // JA4
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JAE4
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JB4
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JBE4
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JE4
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JNE4
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JG4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JGE4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JL4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JLE4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JO4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JNO4
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JS4
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JNS4
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JPO4
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JPE4
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JMP4
   IA32OpProp_IntImmediate,

   IA32OpProp_BranchOp                       | // JMPReg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget,

   IA32OpProp_BranchOp,                        // JMPMem

   IA32OpProp_BranchOp                       | // JRCXZ1
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteImmediate,

   IA32OpProp_BranchOp                       | // LOOP1
   IA32OpProp_ByteImmediate,

   IA32OpProp_TestsSignFlag                  | // LAHF
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_TestsCarryFlag,

   IA32OpProp_UsesTarget                     | // LDDQU
   IA32OpProp_ModifiesTarget                 |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // LEA2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // LEA4RegMem
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget,                  // LEA8RegMem (AMD64)

   IA32OpProp_ModifiesTarget                 | // S1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // S2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // S4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget,                  // S8MemReg (AMD64)

   IA32OpProp_ModifiesTarget                 | // S1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate,

   IA32OpProp_ModifiesTarget                 | // S2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate,

   IA32OpProp_ModifiesTarget                 | // S4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // XRS4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // S8MemImm4 (AMD64)
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // XRS8MemImm4 (AMD64)
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // L1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // L2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // L4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget,                  // L8RegMem (AMD64)

   IA32OpProp_ModifiesTarget                 | // MOVAPSRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVAPSRegMem
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVAPSMemReg
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVAPDRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVAPDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVAPDMemReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVUPSRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVUPSRegMem
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVUPSMemReg
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVUPDRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVUPDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVUPDMemReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVSSRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVSSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // MOVSSMemReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSDRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // MOVSDRegMem
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // MOVSDMemReg
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // SQRTSFRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // SQRTSDRegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // MOVDRegReg4
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // MOVQRegReg8 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVDReg4Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVQReg8Reg (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix,

   IA32OpProp_ModifiesTarget                 | // MOVDQURegReg
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget,                  // MOVDQURegMem

   IA32OpProp_ModifiesTarget,                  // MOVDQUMemReg

   IA32OpProp_ModifiesTarget                 | // MOV1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // MOV2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // MOV4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOV8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // CMOVG4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVG8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVL4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVL8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag,


   IA32OpProp_ModifiesTarget                 | // CMOVE4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVE8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVNE4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // CMOVNE8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_TestsZeroFlag,

   IA32OpProp_ModifiesTarget                 | // MOV1RegImm1
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate,

   IA32OpProp_ModifiesTarget                 | // MOV2RegImm2
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate,

   IA32OpProp_ModifiesTarget                 | // MOV4RegImm4
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // MOV8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // MOV8RegImm64 (AMD64)
   IA32OpProp_TargetRegisterInOpcode,
   IA32OpProp_ModifiesTarget                 | // MOVLPDRegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // MOVLPDMemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_DoubleFP,

   IA32OpProp_ModifiesTarget                 | // MOVQRegMem
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // MOVQMemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SingleFP,

   0,                                          // MOVSB

   IA32OpProp_Needs16BitOperandPrefix,         // MOVSW

   0,                                          // MOVSD

   0,                                          // MOVSQ (AMD64)

   IA32OpProp_ModifiesTarget                 | // MOVSXReg2Reg1
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg4Reg1
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Reg1 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg4Reg2
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ShortSource                    |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Reg2 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Reg4 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg2Mem1
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg4Mem1
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Mem1 (AMD64)
   IA32OpProp_ByteSource,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg4Mem2
   IA32OpProp_ShortSource                    |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Mem2 (AMD64)
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // MOVSXReg8Mem4 (AMD64)
   IA32OpProp_IntSource,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg2Reg1
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg4Reg1
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg8Reg1 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg4Reg2
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ShortSource                    |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg8Reg2 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg8Reg4 (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg2Mem1
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg4Mem1
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg8Mem1 (AMD64)
   IA32OpProp_ByteSource,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg4Mem2
   IA32OpProp_ShortSource                    |
   IA32OpProp_IntTarget,

   IA32OpProp_ModifiesTarget                 | // MOVZXReg8Mem2 (AMD64)
   IA32OpProp_ShortSource,

   IA32OpProp_ModifiesTarget                 | // NEG1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG2Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG4Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG8Reg (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG1Mem
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG2Mem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG4Mem
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NEG8Mem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT2Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT4Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT8Reg (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT1Mem
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT2Mem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT4Mem
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // NOT8Mem (AMD64)
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LOR4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LOR4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // LOR8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // OR8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   0,                                          // PAUSE

   //IA32OpProp_Needs16BitOperandPrefix        | // PCMPEQBRegReg
   IA32OpProp_SourceRegisterInModRM,

   //IA32OpProp_ModifiesTarget                 | // PMOVMSKB4RegReg (AMD64)
   //IA32OpProp_UsesTarget                     |
   //IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_SourceRegisterInModRM,           // PMOVZXWD

   IA32OpProp_SourceRegisterInModRM,           // PMULLD

   IA32OpProp_SourceRegisterInModRM,           // PADDD

   IA32OpProp_ModifiesTarget                 | // PSHUFDRegRegImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // PSHUFDRegMemImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // PSRLDQRegImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesTarget                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // PMOVZXxmm18Reg
   IA32OpProp_UsesTarget                     |
   IA32OpProp_SourceRegisterInModRM,

   IA32OpProp_ModifiesTarget                 | // POPCNT4RegReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // POPCNT8RegReg
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_SingleFP,

   IA32OpProp_ModifiesTarget                 | // POPReg
#if !defined(TR_TARGET_64BIT)
   IA32OpProp_IntTarget                      |
#endif
   IA32OpProp_TargetRegisterInOpcode,

#if !defined(TR_TARGET_64BIT)
   IA32OpProp_IntTarget                      | // POPMem
#endif
   IA32OpProp_ModifiesTarget,

   IA32OpProp_SignExtendImmediate,             // PUSHImms

   IA32OpProp_IntImmediate,                    // PUSHImm4

   IA32OpProp_UsesTarget                     | // PUSHReg
#if !defined(TR_TARGET_64BIT)
   IA32OpProp_IntTarget                      |
#endif
   IA32OpProp_TargetRegisterInOpcode,

   IA32OpProp_UsesTarget                     | // PUSHRegLong
#if !defined(TR_TARGET_64BIT)
   IA32OpProp_IntTarget                      |
#endif
   IA32OpProp_TargetRegisterInOpcode,

#if !defined(TR_TARGET_64BIT)
   IA32OpProp_IntTarget                      | // PUSHMem
#endif
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // RCL1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // RCL4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // RCR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // RCR4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   0,                                          // REPMOVSB

   IA32OpProp_Needs16BitOperandPrefix,  // REPMOVSW

   0,                                          // REPMOVSD

   0,                                          // REPMOVSQ

   0,                                          // REPSTOSB

   IA32OpProp_Needs16BitOperandPrefix,  // REPSTOSW

   0,                                          // REPSTOSD

   0,                                          // REPSTOSQ

   0,                                          // RET

   IA32OpProp_ShortImmediate,                  // RETImm2

   IA32OpProp_ModifiesTarget                 | // ROL1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROL2RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROL4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROL8RegImm1 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROL4RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROL8RegCL (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROR2RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROR4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ROR8RegImm1 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesSignFlag               | // SAHF
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesTarget                 | // SHL1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL1RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL2RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL2RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL8RegImm1 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL4RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL8RegCL (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL1MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL2MemImm1
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL2MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL4MemImm1
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL8MemImm1 (AMD64)
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL4MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHL8MemCL (AMD64)
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR1RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR2RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR2RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR8RegImm1 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR4RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR8RegCL (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR1MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR2MemImm1
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR2MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR4MemImm1
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR8MemImm1 (AMD64)
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR4MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHR8MemCL (AMD64)
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR1RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR2RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR2RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR4RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR8RegImm1 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR4RegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR8RegCL (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR1MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR2MemImm1
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR2MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR4MemImm1
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR8MemImm1 (AMD64)
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR4MemCL
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SAR8MemCL (AMD64)
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SBB8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SETA1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETAE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETB1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETBE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETNE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETG1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETGE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETL1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETLE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETS1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETNS1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETPO1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETPE1Reg
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsParityFlag                |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETA1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETAE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETB1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETBE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_TestsCarryFlag                 |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETNE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETG1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETGE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETL1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETLE1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsOverflowFlag              |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_TestsZeroFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETS1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SETNS1Mem
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_TestsSignFlag                  |
   IA32OpProp_ByteTarget,

   IA32OpProp_ModifiesTarget                 | // SHLD4RegRegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHLD4RegRegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHLD4MemRegImm1
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHLD4MemRegCL
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHRD4RegRegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHRD4RegRegCL
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHRD4MemRegImm1
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SHRD4MemRegCL
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   0,                                          // STOSB

   IA32OpProp_Needs16BitOperandPrefix,         // STOSW

   0,                                          // STOSD

   0,                                          // STOSQ (AMD64)

   IA32OpProp_ModifiesTarget                 | // SUB1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUB8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUBSSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUBSSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_IntSource                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUBSDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // SUBSDRegMem
   IA32OpProp_DoubleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // TEST1AccImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // TEST1AccHImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // TEST2AccImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // TEST4AccImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterIgnored          | // TEST8AccImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // TEST1RegImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // TEST2RegImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // TEST4RegImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_TargetRegisterInModRM          | // TEST8RegImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteTarget                     | // TEST1MemImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_Needs16BitOperandPrefix        | // TEST2MemImm2
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntTarget                      | // TEST4MemImm4
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntImmediate                   | // TEST8MemImm4 (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // TEST1RegReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // TEST2RegReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // TEST4RegReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_SourceRegisterInModRM          | // TEST8RegReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteSource                     | // TEST1MemReg
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ShortSource                    | // TEST2MemReg
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_IntSource                      | // TEST4MemReg
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesOverflowFlag           | // TEST8MemReg (AMD64)
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ByteImmediate,                 //XABORT

   IA32OpProp_BranchOp                       |//XBEGIN2
   IA32OpProp_ShortImmediate,

   IA32OpProp_BranchOp                       |//XBEGIN4
   IA32OpProp_IntImmediate,

   IA32OpProp_ModifiesTarget                 | // XCHG2AccReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG4AccReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG8AccReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInOpcode         |
   IA32OpProp_SourceRegisterIgnored          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG1RegReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG2RegReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG4RegReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG8RegReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG1RegMem
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG2RegMem
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG4RegMem
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG8RegMem (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG1MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG2MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG4MemReg
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XCHG8MemReg (AMD64)
   IA32OpProp_ModifiesSource                 |
   IA32OpProp_UsesTarget,

   0,                                        // XEND

   IA32OpProp_ModifiesTarget                 | // XOR1AccImm1
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2AccImm2
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4AccImm4
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8AccImm4 (AMD64)
   IA32OpProp_TargetRegisterIgnored          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR1RegImm1
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2RegImm2
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4RegImm4
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8RegImm4 (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4RegImms
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8RegImms (AMD64)
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR1MemImm1
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2MemImm2
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ShortImmediate                 |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2MemImms
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4MemImm4
   IA32OpProp_IntTarget                      |
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8MemImm4 (AMD64)
   IA32OpProp_IntImmediate                   |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4MemImms
   IA32OpProp_IntTarget                      |
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8MemImms (AMD64)
   IA32OpProp_SignExtendImmediate            |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR1RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4RegReg
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8RegReg (AMD64)
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR1RegMem
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2RegMem
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4RegMem
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8RegMem (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR1MemReg
   IA32OpProp_ByteSource                     |
   IA32OpProp_ByteTarget                     |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR2MemReg
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_ShortSource                    |
   IA32OpProp_ShortTarget                    |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR4MemReg
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XOR8MemReg (AMD64)
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XORPSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XORPSRegMem
   IA32OpProp_SingleFP                       |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // XORPDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   0,                                          // MFENCE
   0,                                          // LFENCE
   0,                                          // SFENCE

   IA32OpProp_UsesTarget                     | // PCMPESTRI
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_ModifiesCarryFlag              |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesTarget                 |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_ByteImmediate,

   0,                                          // PREFETCHNTA
   0,                                          // PREFETCHT0
   0,                                          // PREFETCHT1
   0,                                          // PREFETCHT2

   IA32OpProp_ModifiesTarget                 | // ANDNPSRegReg
   IA32OpProp_SingleFP                       |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // ANDNPDRegReg
   IA32OpProp_DoubleFP                       |
   IA32OpProp_Needs16BitOperandPrefix        |
   IA32OpProp_SourceRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // PSLLQRegImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_UsesTarget,

   IA32OpProp_ModifiesTarget                 | // PSRLQRegImm1
   IA32OpProp_ByteImmediate                  |
   IA32OpProp_DoubleFP                       |
   IA32OpProp_TargetRegisterInModRM          |
   IA32OpProp_UsesTarget,

   0,                                          // FENCE

   0,                                          // VGFENCE

   0,                                          // PROCENTRY

   0,                                          // DQImm64 (AMD64)

   IA32OpProp_IntImmediate,                    // DDImm4

   IA32OpProp_ShortImmediate,                  // DWImm2

   IA32OpProp_ByteImmediate,                   // DBImm1

   IA32OpProp_UsesTarget                     | // WRTBAR
   IA32OpProp_ModifiesTarget                 |
   IA32OpProp_IntSource                      |
   IA32OpProp_IntTarget                      |
   IA32OpProp_ByteTarget,
   0,                                          // ASSOCREGS
   0,                                          // FPREGSPILL

   IA32OpProp_BranchOp                       | // VirtualGuardNOP
   IA32OpProp_TestsZeroFlag,

   0,                                          // LABEL
   0,                                          // FCMPEVAL
   0,                                          // RestoreVMThread

   IA32OpProp_UsesTarget                     | // PPS_OPCOUNT
   IA32OpProp_ModifiesOverflowFlag           |
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   IA32OpProp_ModifiesOverflowFlag           | // PPS_OPFIELD
   IA32OpProp_ModifiesSignFlag               |
   IA32OpProp_ModifiesZeroFlag               |
   IA32OpProp_ModifiesParityFlag             |
   IA32OpProp_ModifiesCarryFlag,

   0                                           // AdjustFramePtr
   };


const uint32_t TR_X86OpCode::_properties2[IA32NumOpCodes] =
   {
   IA32OpProp2_CannotBeAssembled,              // BADIA32Op

   IA32OpProp2_SetsCCForTest                 | // ADC1AccImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC2AccImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC4AccImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC8AccImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC1RegImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC2RegImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC2RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC4RegImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC8RegImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC4RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC8RegImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC1MemImm1
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC2MemImm2
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC2MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC4MemImm4
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC8MemImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC4MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC8MemImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC1RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC2RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC4RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC8RegReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC1RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC2RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC4RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADC8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC1MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADC8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1AccImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD2AccImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD4AccImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD8AccImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1RegImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD2RegImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD2RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD4RegImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD8RegImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD4RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD8RegImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1MemImm1
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD2MemImm2
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD2MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD4MemImm4
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD8MemImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD4MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD8MemImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD2RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD4RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD8RegReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD2RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD4RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // ADD8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD1MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // ADD8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // ADDSSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // ADDSSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // ADDPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SourceIsMemRef                | // ADDPSRegMem
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // ADDSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // ADDSDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // ADDPDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SourceIsMemRef                | // ADDPDRegMem
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SetsCCForTest                 | // LADD1MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LADD2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LADD4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LADD8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LXADD1MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LXADD2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LXADD4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LXADD8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_NeedsLockPrefix               |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // AND1AccImm1

   IA32OpProp2_SetsCCForTest,                  // AND2AccImm2

   IA32OpProp2_SetsCCForTest,                  // AND4AccImm4

   IA32OpProp2_SetsCCForTest                 | // AND8AccImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // AND1RegImm1

   IA32OpProp2_SetsCCForTest,                  // AND2RegImm2

   IA32OpProp2_SetsCCForTest,                  // AND2RegImms

   IA32OpProp2_SetsCCForTest,                  // AND4RegImm4

   IA32OpProp2_SetsCCForTest                 | // AND8RegImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // AND4RegImms

   IA32OpProp2_SetsCCForTest                 | // AND8RegImms (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND1MemImm1
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND2MemImm2
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND2MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND4MemImm4
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND8MemImm4 (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND4MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND8MemImms (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // AND1RegReg

   IA32OpProp2_SetsCCForTest,                  // AND2RegReg

   IA32OpProp2_SetsCCForTest,                  // AND4RegReg

   IA32OpProp2_SetsCCForTest                 | // AND8RegReg (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND1RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // AND2RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // AND4RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // AND8RegMem (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND1MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND2MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND4MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // AND8MemReg (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // BSF2RegReg

   0,                                          // BSF4RegReg
   //
   IA32OpProp2_LongSource                    | // BSF8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,
   //
   0,                                          // BSR4RegReg
   //
   IA32OpProp2_LongSource                    | // BSR8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // BSWAP4Reg

   IA32OpProp2_LongTarget                    | // BSWAP8Reg (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // BTS4RegReg

   IA32OpProp2_SetsCCForTest                 | // BTS4MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_CallOp,                         // CALLImm4

   IA32OpProp2_CallOp                        | // CALLREXImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_CallOp,                         // CALLReg

   IA32OpProp2_CallOp                        | // CALLREXReg (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_CallOp                        | // CALLMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_CallOp                        | // CALLREXMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // CBWAcc
   0,                                          // CBWEAcc
   IA32OpProp2_SourceIsMemRef,                 // CMOVA4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVB4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVE4RegMem

   IA32OpProp2_LongSource                    | // CMOVE8RegMem (64-bit)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SourceIsMemRef,                 // CMOVG4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVL4RegMem

   IA32OpProp2_SourceIsMemRef,                 // CMOVNE4RegMem

   IA32OpProp2_LongSource                    | // CMOVNE8RegMem (64-bit)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SourceIsMemRef,                 // CMOVNO4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVNS4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVO4RegMem
   IA32OpProp2_SourceIsMemRef,                 // CMOVS4RegMem

   IA32OpProp2_FusableCompare,                 // CMP1AccImm1
   IA32OpProp2_FusableCompare,                 // CMP2AccImm2
   IA32OpProp2_FusableCompare,                 // CMP4AccImm4

   IA32OpProp2_LongTarget                    | // CMP8AccImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,

   IA32OpProp2_FusableCompare,                 // CMP1RegImm1
   IA32OpProp2_FusableCompare,                 // CMP2RegImm2
   IA32OpProp2_FusableCompare,                 // CMP2RegImms
   IA32OpProp2_FusableCompare,                 // CMP4RegImm4

   IA32OpProp2_LongTarget                    | // CMP8RegImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,

   IA32OpProp2_FusableCompare,                 // CMP4RegImms

   IA32OpProp2_LongTarget                    | // CMP8RegImms (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,





   0,                                          // CMP1MemImm1
   0,                                          // CMP2MemImm2
   0,                                          // CMP2MemImms
   0,                                          // CMP4MemImm4

   IA32OpProp2_LongTarget                    | // CMP8MemImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // CMP4MemImms

   IA32OpProp2_LongTarget                    | // CMP8MemImms (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_FusableCompare,                 // CMP1RegReg
   IA32OpProp2_FusableCompare,                 // CMP2RegReg
   IA32OpProp2_FusableCompare,                 // CMP4RegReg

   IA32OpProp2_LongSource                    | // CMP8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,

   IA32OpProp2_SourceIsMemRef                | // CMP1RegMem
   IA32OpProp2_FusableCompare,

   IA32OpProp2_SourceIsMemRef                | // CMP2RegMem
   IA32OpProp2_FusableCompare,

   IA32OpProp2_SourceIsMemRef                | // CMP4RegMem
   IA32OpProp2_FusableCompare,

   IA32OpProp2_LongSource                    | // CMP8RegMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,

   IA32OpProp2_FusableCompare,                 // CMP1MemReg
   IA32OpProp2_FusableCompare,                 // CMP2MemReg
   IA32OpProp2_FusableCompare,                 // CMP4MemReg

   IA32OpProp2_LongSource                    | // CMP8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_FusableCompare,

   IA32OpProp2_SupportsLockPrefix,             // CMPXCHG1MemReg

   IA32OpProp2_SupportsLockPrefix,             // CMPXCHG2MemReg

   IA32OpProp2_SupportsLockPrefix,             // CMPXCHG4MemReg

   IA32OpProp2_SupportsLockPrefix            | // CMPXCHG8MemReg (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SupportsLockPrefix            | // CMPXCHG8BMem (IA32 only)
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // CMPXCHG16BMem (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsLockPrefix,                // LCMPXCHG1MemReg

   IA32OpProp2_NeedsLockPrefix,                // LCMPXCHG2MemReg

   IA32OpProp2_NeedsLockPrefix,                // LCMPXCHG4MemReg

   IA32OpProp2_LongSource                    | // LCMPXCHG8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_NeedsLockPrefix               |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsLockPrefix               | // LCMPXCHG8BMem (IA32 only)
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_NeedsLockPrefix               | // LCMPXCHG16BMem (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_LongSource                    | // XALCMPXCHG8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_NeedsLockPrefix               |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_NeedsXacquirePrefix,

   IA32OpProp2_SupportsLockPrefix            | // XACMPXCHG8MemReg (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_NeedsXacquirePrefix,

   IA32OpProp2_NeedsLockPrefix               | // XALCMPXCHG4MemReg
   IA32OpProp2_NeedsXacquirePrefix,

   IA32OpProp2_SupportsLockPrefix            | // XACMPXCHG4MemReg
   IA32OpProp2_NeedsXacquirePrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SSRegReg4
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SSRegReg8 (AMD64)
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SSRegMem8 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SDRegReg4
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SDRegReg8 (AMD64)
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSI2SDRegMem8 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSS2SIReg4Reg
   IA32OpProp2_XMMSource,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSS2SIReg8Reg (AMD64)
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             |  // CVTTSS2SIReg4Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSS2SIReg8Mem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSD2SIReg4Reg
   IA32OpProp2_XMMSource,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSD2SIReg8Reg (AMD64)
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSD2SIReg4Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_NeedsScalarPrefix             | // CVTTSD2SIReg8Mem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSS2SDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSS2SDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSD2SSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // CVTSD2SSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   0,                                          // CWDAcc
   0,                                          // CDQAcc

   IA32OpProp2_LongSource                    | // CQOAcc (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // DEC1Reg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // DEC2Reg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_IsAMD64DeprecatedDec,

   IA32OpProp2_SetsCCForTest                 | // DEC4Reg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_IsAMD64DeprecatedDec,

   IA32OpProp2_SetsCCForTest                 | // DEC8RegLong (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // DEC1Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // DEC2Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // DEC4Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // DEC8Mem (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_SourceRegIsImplicit,            // FABSReg
   IA32OpProp2_SourceRegIsImplicit,            // DABSReg
   IA32OpProp2_SourceRegIsImplicit,            // FSQRTReg
   IA32OpProp2_SourceRegIsImplicit,            // DSQRTReg
   0,                                          // FADDRegReg
   0,                                          // DADDRegReg
   0,                                          // FADDPReg

   IA32OpProp2_SourceIsMemRef                | // FADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FIADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DIADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSADDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceRegIsImplicit,            // FCHSReg
   IA32OpProp2_SourceRegIsImplicit,            // DCHSReg
   0,                                          // FDIVRegReg
   0,                                          // DDIVRegReg

   IA32OpProp2_SourceIsMemRef                | // FDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FDIVPReg

   IA32OpProp2_SourceIsMemRef                | // FIDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DIDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSDIVRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FDIVRRegReg
   0,                                          // DDIVRRegReg

   IA32OpProp2_SourceIsMemRef                | // FDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FDIVRPReg

   IA32OpProp2_SourceIsMemRef                | // FIDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DIDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSDIVRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FILDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DILDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FLLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DLLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceRegIsImplicit,            // FISTMemReg
   IA32OpProp2_SourceRegIsImplicit,            // DISTMemReg
   IA32OpProp2_SourceIsMemRef,                 // FISTPMem
   IA32OpProp2_SourceIsMemRef,                 // DISTPMem
   IA32OpProp2_SourceIsMemRef,                 // FLSTPMem
   IA32OpProp2_SourceIsMemRef,                 // DLSTPMem
   IA32OpProp2_SourceRegIsImplicit,            // FSSTMemReg
   IA32OpProp2_SourceRegIsImplicit,            // DSSTMemReg
   IA32OpProp2_SourceIsMemRef,                 // FSSTPMem
   IA32OpProp2_SourceIsMemRef,                 // DSSTPMem
   0,                                          // FLDLN2
   IA32OpProp2_TargetRegIsImplicit,            // FLDRegReg
   IA32OpProp2_TargetRegIsImplicit,            // DLDRegReg

   IA32OpProp2_SourceIsMemRef                | // FLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DLDRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_TargetRegIsImplicit,            // FLD0Reg
   IA32OpProp2_TargetRegIsImplicit,            // DLD0Reg
   IA32OpProp2_TargetRegIsImplicit,            // FLD1Reg
   IA32OpProp2_TargetRegIsImplicit,            // DLD1Reg
   0,                                          // FLDMem
   0,                                          // DLDMem
   0,                                          // LDCWMem
   0,                                          // FMULRegReg
   0,                                          // DMULRegReg
   0,                                          // FMULPReg

   IA32OpProp2_SourceIsMemRef                | // FMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FIMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DIMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSMULRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FNCLEX

   IA32OpProp2_SourceRegIsImplicit           | // FPREMRegReg
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceRegIsImplicit           | // FSCALERegReg
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceRegIsImplicit,            // FSTMemReg
   IA32OpProp2_SourceRegIsImplicit,            // DSTMemReg
   IA32OpProp2_SourceRegIsImplicit,            // FSTRegReg
   IA32OpProp2_SourceRegIsImplicit,            // DSTRegReg
   IA32OpProp2_SourceRegIsImplicit,            // FSTPMemReg
   IA32OpProp2_SourceRegIsImplicit,            // DSTPMemReg
   IA32OpProp2_SourceRegIsImplicit,            // FSTPReg
   IA32OpProp2_SourceRegIsImplicit,            // DSTPReg
   0,                                          // STCWMem
   0,                                          // STSWMem
   0,                                          // STSWAcc
   0,                                          // FSUBRegReg
   0,                                          // DSUBRegReg

   IA32OpProp2_SourceIsMemRef                | // FSUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FSUBPReg

   IA32OpProp2_SourceIsMemRef                | // FISUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DISUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSSUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSSUBRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FSUBRRegReg
   0,                                          // DSUBRRegReg

   IA32OpProp2_SourceIsMemRef                | // FSUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FSUBRPReg

   IA32OpProp2_SourceIsMemRef                | // FISUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DISUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // FSSUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DSSUBRRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_TargetRegIsImplicit,            // FTSTReg
   IA32OpProp2_TargetRegIsImplicit,            // FCOMRegReg
   IA32OpProp2_TargetRegIsImplicit,            // DCOMRegReg

   IA32OpProp2_SourceIsMemRef                | // FCOMRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // DCOMRegMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_TargetRegIsImplicit,            // FCOMPReg
   IA32OpProp2_TargetRegIsImplicit,            // FCOMPMem
   IA32OpProp2_TargetRegIsImplicit,            // DCOMPMem

   IA32OpProp2_SourceRegIsImplicit           | // FCOMPP
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // FCOMIRegReg
   0,                                          // DCOMIRegReg
   0,                                          // FCOMIPReg
   0,                                          // FYL2X

   IA32OpProp2_XMMSource                     | // UCOMISSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMTarget                     | // UCOMISSRegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_XMMSource                     | // UCOMISDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMTarget                     | // UCOMISDRegMem
   IA32OpProp2_SourceIsMemRef,

   0,                                          // FXCHReg
   IA32OpProp2_TargetRegIsImplicit,            // IDIV1AccReg
   IA32OpProp2_TargetRegIsImplicit,            // IDIV2AccReg
   IA32OpProp2_TargetRegIsImplicit,            // IDIV4AccReg

   IA32OpProp2_LongSource                    | // IDIV8AccReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // DIV4AccReg

   IA32OpProp2_LongSource                    | // DIV8AccReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef                | // IDIV1AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // IDIV2AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // IDIV4AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_LongSource                    | // IDIV8AccMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef,                 // DIV4AccMem

   IA32OpProp2_LongSource                    | // DIV8AccMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_NeedsScalarPrefix             | // DIVSSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // DIVSSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // DIVSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // DIVSDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_TargetRegIsImplicit,            // IMUL1AccReg
   IA32OpProp2_TargetRegIsImplicit,            // IMUL2AccReg
   IA32OpProp2_TargetRegIsImplicit,            // IMUL4AccReg

   IA32OpProp2_LongSource                    | // IMUL8AccReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // IMUL1AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // IMUL2AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_SourceIsMemRef                | // IMUL4AccMem
   IA32OpProp2_TargetRegIsImplicit,

   IA32OpProp2_LongSource                    | // IMUL8AccMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_TargetRegIsImplicit,

   0,                                          // IMUL2RegReg
   0,                                          // IMUL4RegReg

   IA32OpProp2_LongSource                    | // IMUL8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // IMUL2RegMem
   IA32OpProp2_SourceIsMemRef,                 // IMUL4RegMem

   IA32OpProp2_LongSource                    | // IMUL8RegMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // IMUL2RegRegImm2
   0,                                          // IMUL2RegRegImms
   0,                                          // IMUL4RegRegImm4

   IA32OpProp2_LongSource                    | // IMUL8RegRegImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // IMUL4RegRegImms

   IA32OpProp2_LongSource                    | // IMUL8RegRegImms (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // IMUL2RegMemImm2
   IA32OpProp2_SourceIsMemRef,                 // IMUL2RegMemImms
   IA32OpProp2_SourceIsMemRef,                 // IMUL4RegMemImm4

   IA32OpProp2_LongSource                    | // IMUL8RegMemImm4 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // IMUL4RegMemImms

   IA32OpProp2_LongSource                    | // IMUL8RegMemImms (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // MUL1AccReg
   0,                                          // MUL2AccReg
   0,                                          // MUL4AccReg

   IA32OpProp2_LongSource                    | // MUL8AccReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // MUL1AccMem
   IA32OpProp2_SourceIsMemRef,                 // MUL2AccMem
   IA32OpProp2_SourceIsMemRef,                 // MUL4AccMem

   IA32OpProp2_LongSource                    | // MUL8AccMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_NeedsScalarPrefix             | // MULSSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MULSSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MULPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SourceIsMemRef                | // MULPSRegMem
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MULSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MULSDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MULPDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SourceIsMemRef                | // MULPDRegMem
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SetsCCForTest                 | // INC1Reg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // INC2Reg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_IsAMD64DeprecatedInc,

   IA32OpProp2_SetsCCForTest                 | // INC4Reg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_IsAMD64DeprecatedInc,

   IA32OpProp2_SetsCCForTest                 | // INC8RegLong (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // INC1Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // INC2Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // INC4Mem
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // INC8Mem (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // JA1
   0,                                          // JAE1
   0,                                          // JB1
   0,                                          // JBE1
   0,                                          // JE1
   0,                                          // JNE1
   0,                                          // JG1
   0,                                          // JGE1
   0,                                          // JL1
   0,                                          // JLE1
   0,                                          // JO1
   0,                                          // JNO1
   0,                                          // JS1
   0,                                          // JNS1
   0,                                          // JPO1
   0,                                          // JPE1
   0,                                          // JMP1
   0,                                          // JA4
   0,                                          // JAE4
   0,                                          // JB4
   0,                                          // JBE4
   0,                                          // JE4
   0,                                          // JNE4
   0,                                          // JG4
   0,                                          // JGE4
   0,                                          // JL4
   0,                                          // JLE4
   0,                                          // JO4
   0,                                          // JNO4
   0,                                          // JS4
   0,                                          // JNS4
   0,                                          // JPO4
   0,                                          // JPE4
   0,                                          // JMP4
   0,                                          // JMPReg
   IA32OpProp2_SourceIsMemRef,                 // JMPMem
   0,                                          // JRCXZ1
   0,                                          // LOOP1
   IA32OpProp2_IsIA32Only,                     // LAHF

   IA32OpProp2_SourceIsMemRef                | // LDDQU
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsScalarPrefix,

   IA32OpProp2_SourceIsMemRef,                 // LEA2RegMem
   IA32OpProp2_SourceIsMemRef,                 // LEA4RegMem

   IA32OpProp2_LongSource                    | // LEA8RegMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // S1MemReg
   0,                                          // S2MemReg
   0,                                          // S4MemReg

   IA32OpProp2_LongSource                    | // S8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // S1MemImm1
   0,                                          // S2MemImm2
   0,                                          // S4MemImm4

   IA32OpProp2_NeedsXreleasePrefix,            // XRS4MemImm4

   IA32OpProp2_LongTarget                    | // S8MemImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_LongTarget                    | // XRS8MemImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_NeedsXreleasePrefix,

   IA32OpProp2_SourceIsMemRef,                 // L1RegMem
   IA32OpProp2_SourceIsMemRef,                 // L2RegMem
   IA32OpProp2_SourceIsMemRef,                 // L4RegMem

   IA32OpProp2_LongSource                    | // L8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_XMMSource                     | // MOVAPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVAPSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVAPSMemReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVAPDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVAPDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVAPDMemReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPSMemReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // MOVUPDMemReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSSMemReg
   IA32OpProp2_XMMSource,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // MOVSDMemReg
   IA32OpProp2_XMMSource,

   IA32OpProp2_NeedsScalarPrefix             | // SQRTSFRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // SQRTSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMTarget,                      // MOVDRegReg4

   IA32OpProp2_XMMTarget                     | // MOVQRegReg8 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_LongSource,

   IA32OpProp2_XMMSource,                      // MOVDReg4Reg

   IA32OpProp2_XMMSource                     | // MOVQReg8Reg (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_LongTarget,

   IA32OpProp2_XMMSource                     | // MOVDQURegReg
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsRepPrefix,

   IA32OpProp2_XMMSource                     | // MOVDQURegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsRepPrefix,

   IA32OpProp2_XMMSource                     | // MOVDQUMemReg
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsRepPrefix,

   0,                                          // MOV1RegReg
   0,                                          // MOV2RegReg
   0,                                          // MOV4RegReg

   IA32OpProp2_LongSource                    | // MOV8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // CMOVG4RegReg

   IA32OpProp2_LongSource                    | // CMOVG8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,
   0,                                          // CMOVL4RegReg

   IA32OpProp2_LongSource                    | // CMOVL8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // CMOVE4RegReg
   IA32OpProp2_LongSource                    | // CMOVE8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,
   0,                                          // CMOVNE4RegReg
   IA32OpProp2_LongSource                    | // CMOVNE8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,
   0,                                          // MOV1RegImm1
   0,                                          // MOV2RegImm2
   0,                                          // MOV4RegImm4

   IA32OpProp2_LongTarget                    | // MOV8RegImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_LongTarget                    | // MOV8RegImm64 (AMD64)
   IA32OpProp2_LongImmediate                 |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef                | // MOVLPDRegMem
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource,                      // MOVLPDMemReg

   IA32OpProp2_SourceIsMemRef                | // MOVQRegMem
   IA32OpProp2_NeedsScalarPrefix             |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource,                      // MOVQMemReg

   0,                                          // MOVSB
   0,                                          // MOVSW
   0,                                          // MOVSD
   0,                                          // MOVSQ (AMD64)

   0,                                          // MOVSXReg2Reg1
   0,                                          // MOVSXReg4Reg1

   IA32OpProp2_LongTarget                    | // MOVSXReg8Reg1 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // MOVSXReg4Reg2

   IA32OpProp2_LongTarget                    | // MOVSXReg8Reg2 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_LongTarget                    | // MOVSXReg8Reg4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix, // This acts like MOVSXReg4Reg4 without the Rex.

   IA32OpProp2_SourceIsMemRef,                 // MOVSXReg2Mem1
   IA32OpProp2_SourceIsMemRef,                 // MOVSXReg4Mem1

   IA32OpProp2_LongTarget                    | // MOVSXReg8Mem1 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // MOVSXReg4Mem2

   IA32OpProp2_LongTarget                    | // MOVSXReg8Mem2 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_LongTarget                    | // MOVSXReg8Mem4 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // MOVZXReg2Reg1
   0,                                          // MOVZXReg4Reg1

   IA32OpProp2_LongTarget                    | // MOVZXReg8Reg1 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // MOVZXReg4Reg2

   IA32OpProp2_LongTarget                    | // MOVZXReg8Reg2 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // MOVZXReg8Reg4 (AMD64)

   IA32OpProp2_SourceIsMemRef,                 // MOVZXReg2Mem1
   IA32OpProp2_SourceIsMemRef,                 // MOVZXReg4Mem1

   IA32OpProp2_LongTarget                    | // MOVZXReg8Mem1 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // MOVZXReg4Mem2

   IA32OpProp2_LongTarget                    | // MOVZXReg8Mem2 (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // NEG1Reg
   0,                                          // NEG2Reg
   0,                                          // NEG4Reg

   IA32OpProp2_LongTarget                    | // NEG8Reg (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_SupportsLockPrefix            | // NEG1Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NEG2Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NEG4Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NEG8Mem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix       |
   IA32OpProp2_SourceIsMemRef,

   0,                                          // NOT1Reg
   0,                                          // NOT2Reg
   0,                                          // NOT4Reg

   IA32OpProp2_LongTarget                    | // NOT8Reg (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,


   IA32OpProp2_SupportsLockPrefix            | // NOT1Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NOT2Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NOT4Mem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SupportsLockPrefix            | // NOT8Mem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // OR1AccImm1

   IA32OpProp2_SetsCCForTest,                  // OR2AccImm2

   IA32OpProp2_SetsCCForTest,                  // OR4AccImm4

   IA32OpProp2_SetsCCForTest                 | // OR8AccImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // OR1RegImm1

   IA32OpProp2_SetsCCForTest,                  // OR2RegImm2

   IA32OpProp2_SetsCCForTest,                  // OR2RegImms

   IA32OpProp2_SetsCCForTest,                  // OR4RegImm4

   IA32OpProp2_SetsCCForTest                 | // OR8RegImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // OR4RegImms

   IA32OpProp2_SetsCCForTest                 | // OR8RegImms (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR1MemImm1
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR2MemImm2
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR2MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR4MemImm4
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR8MemImm4 (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR4MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LOR4MemImms
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // LOR4MemReg
   IA32OpProp2_NeedsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR8MemReg (AMD64)
   IA32OpProp2_NeedsLockPrefix               |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR8MemImms (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // OR1RegReg

   IA32OpProp2_SetsCCForTest,                  // OR2RegReg

   IA32OpProp2_SetsCCForTest,                  // OR4RegReg

   IA32OpProp2_SetsCCForTest                 | // OR8RegReg (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR1RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // OR2RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // OR4RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // OR8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR1MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR2MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR4MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // OR8MemReg (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // PAUSE

   IA32OpProp2_NeedsSSE42OpcodePrefix|         // PCMPEQBRegReg
   IA32OpProp2_XMMSource |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsSSE42OpcodePrefix|         // PMOVMSKB4RegReg
   IA32OpProp2_XMMSource,
   //IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_XMMSource|                      // PMOVZXWD
   IA32OpProp2_XMMTarget|
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_XMMSource|                      // PMULLD
   IA32OpProp2_XMMTarget|
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_XMMSource|                      // PADDD
   IA32OpProp2_XMMTarget|
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_XMMSource                     | // PSHUFDRegRegImm1
   IA32OpProp2_XMMTarget,

   IA32OpProp2_SourceIsMemRef                | // PSHUFDRegMemImm1
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource|                      // PSRLDQRegImm1
   IA32OpProp2_XMMTarget|
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_NeedsSSE42OpcodePrefix        | // PMOVZXxmm18Reg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix,              // POPCNT4RegReg

   IA32OpProp2_Needs64BitOperandPrefix       | // POPCNT8RegReg
   IA32OpProp2_NeedsScalarPrefix             |
   IA32OpProp2_LongSource,

#if defined(TR_TARGET_64BIT)
   IA32OpProp2_LongTarget                    | // POPReg
#endif
   IA32OpProp2_PopOp,

#if defined(TR_TARGET_64BIT)
   IA32OpProp2_LongTarget                    | // POPMem
#endif
   IA32OpProp2_PopOp,

   IA32OpProp2_PushOp,                         // PUSHImms

   IA32OpProp2_PushOp,                         // PUSHImm4

#if defined(TR_TARGET_64BIT)
   IA32OpProp2_LongTarget                    | // PUSHReg
#endif
   IA32OpProp2_PushOp,

#if defined(TR_TARGET_64BIT)
   IA32OpProp2_LongTarget                    | // PUSHRegLong
#endif
   IA32OpProp2_PushOp,

#if defined(TR_TARGET_64BIT)
   IA32OpProp2_LongTarget                    | // PUSHMem
#endif
   IA32OpProp2_PushOp                        |
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_RotateOp,                       // RCL1RegImm1
   IA32OpProp2_RotateOp,                       // RCL4RegImm1

   IA32OpProp2_RotateOp,                       // RCR1RegImm1
   IA32OpProp2_RotateOp,                       // RCR4RegImm1

   0,                                          // REPMOVSB
   0,                                          // REPMOVSW
   0,                                          // REPMOVSD

   IA32OpProp2_Needs64BitOperandPrefix |       // REPMOVSQ
   IA32OpProp2_NeedsRepPrefix,

   0,                                          // REPSTOSB
   0,                                          // REPSTOSW
   0,                                          // REPSTOSD
   IA32OpProp2_Needs64BitOperandPrefix |       // REPSTOSQ
   IA32OpProp2_NeedsRepPrefix,
   0,                                          // RET
   0,                                          // RETImm2

   IA32OpProp2_RotateOp,                       // ROL1RegImm1

   IA32OpProp2_RotateOp,                       // ROL2RegImm1

   IA32OpProp2_RotateOp,                       // ROL4RegImm1

   IA32OpProp2_RotateOp                      | // ROL8RegImm1 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_RotateOp,                       // ROL4RegCL

   IA32OpProp2_RotateOp                      | // ROL8RegCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_RotateOp,                       // ROR1RegImm1

   IA32OpProp2_RotateOp,                       // ROR2RegImm1

   IA32OpProp2_RotateOp,                       // ROR4RegImm1

   IA32OpProp2_RotateOp                      | // ROR8RegImm1 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_IsIA32Only,                     // SAHF

   IA32OpProp2_SetsCCForTest                 | // SHL1RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHL1RegCL

   IA32OpProp2_SetsCCForTest                 | // SHL2RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHL2RegCL

   IA32OpProp2_SetsCCForTest                 | // SHL4RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SHL8RegImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SHL4RegCL

   IA32OpProp2_ShiftOp                       | // SHL8RegCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SHL1MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHL1MemCL

   IA32OpProp2_SetsCCForTest                 | // SHL2MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHL2MemCL

   IA32OpProp2_SetsCCForTest                 | // SHL4MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SHL8MemImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SHL4MemCL

   IA32OpProp2_ShiftOp                       | // SHL8MemCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SHR1RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHR1RegCL

   IA32OpProp2_SetsCCForTest                 | // SHR2RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHR2RegCL

   IA32OpProp2_SetsCCForTest                 | // SHR4RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SHR8RegImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SHR4RegCL

   IA32OpProp2_ShiftOp                       | // SHR8RegCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SHR1MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHR1MemCL

   IA32OpProp2_SetsCCForTest                 | // SHR2MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SHR2MemCL

   IA32OpProp2_SetsCCForTest                 | // SHR4MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SHR8MemImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SHR4MemCL

   IA32OpProp2_ShiftOp                       | // SHR8MemCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SAR1RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SAR1RegCL

   IA32OpProp2_SetsCCForTest                 | // SAR2RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SAR2RegCL

   IA32OpProp2_SetsCCForTest                 | // SAR4RegImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SAR8RegImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SAR4RegCL

   IA32OpProp2_ShiftOp                       | // SAR8RegCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SAR1MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SAR1MemCL

   IA32OpProp2_SetsCCForTest                 | // SAR2MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_ShiftOp,                        // SAR2MemCL

   IA32OpProp2_SetsCCForTest                 | // SAR4MemImm1
   IA32OpProp2_ShiftOp,

   IA32OpProp2_SetsCCForTest                 | // SAR8MemImm1 (AMD64)
   IA32OpProp2_ShiftOp                       |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_ShiftOp,                        // SAR4MemCL

   IA32OpProp2_ShiftOp                       | // SAR8MemCL (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1AccImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB2AccImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB4AccImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB8AccImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1RegImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB2RegImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB2RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB4RegImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB8RegImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB4RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB8RegImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1MemImm1
   IA32OpProp2_SetsCCForCompare              |

   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB2MemImm2
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB2MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB4MemImm4
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB8MemImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB4MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB8MemImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB2RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB4RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB8RegReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB2RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB4RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SBB8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB1MemReg
   IA32OpProp2_SetsCCForCompare              |

   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SBB8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // SETA1Reg
   0,                                          // SETAE1Reg
   0,                                          // SETB1Reg
   0,                                          // SETBE1Reg
   0,                                          // SETE1Reg
   0,                                          // SETNE1Reg
   0,                                          // SETG1Reg
   0,                                          // SETGE1Reg
   0,                                          // SETL1Reg
   0,                                          // SETLE1Reg
   0,                                          // SETS1Reg
   0,                                          // SETNS1Reg
   0,                                          // SETPO1Reg
   0,                                          // SETPE1Reg
   IA32OpProp2_SourceIsMemRef,                 // SETA1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETAE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETB1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETBE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETNE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETG1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETGE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETL1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETLE1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETS1Mem
   IA32OpProp2_SourceIsMemRef,                 // SETNS1Mem
   0,                                          // SHLD4RegRegImm1
   0,                                          // SHLD4RegRegCL
   0,                                          // SHLD4MemRegImm1
   0,                                          // SHLD4MemRegCL
   0,                                          // SHRD4RegRegImm1
   0,                                          // SHRD4RegRegCL
   0,                                          // SHRD4MemRegImm1
   0,                                          // SHRD4MemRegCL

   0,                                          // STOSB
   0,                                          // STOSW
   0,                                          // STOSD

   IA32OpProp2_Needs64BitOperandPrefix,        // STOSQ (AMD64)

   IA32OpProp2_SetsCCForTest                 | // SUB1AccImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB2AccImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB4AccImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB8AccImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB1RegImm1
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB2RegImm2
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB2RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB4RegImm4
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB8RegImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB4RegImms
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB8RegImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB1MemImm1
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB2MemImm2
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB2MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB4MemImm4
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB8MemImm4 (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB4MemImms
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB8MemImms (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB1RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB2RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB4RegReg
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB8RegReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB1RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB2RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB4RegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare,

   IA32OpProp2_SetsCCForTest                 | // SUB8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB1MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB2MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB4MemReg
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // SUB8MemReg (AMD64)
   IA32OpProp2_SetsCCForCompare              |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_NeedsScalarPrefix             | // SUBSSRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // SUBSSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // SUBSDRegReg
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_NeedsScalarPrefix             | // SUBSDRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_FusableCompare,                 // TEST1AccImm1
   IA32OpProp2_FusableCompare,                 // TEST1AccHImm1
   IA32OpProp2_FusableCompare,                 // TEST2AccImm2
   IA32OpProp2_FusableCompare,                 // TEST4AccImm4

   IA32OpProp2_LongTarget                    | // TEST8AccImm4 (AMD64)
   IA32OpProp2_FusableCompare                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_FusableCompare,                 // TEST1RegImm1
   IA32OpProp2_FusableCompare,                 // TEST2RegImm2
   IA32OpProp2_FusableCompare,                 // TEST4RegImm4

   IA32OpProp2_LongTarget                    | // TEST8RegImm4 (AMD64)
   IA32OpProp2_FusableCompare                |
   IA32OpProp2_Needs64BitOperandPrefix,


   0,                                          // TEST1MemImm1
   0,                                          // TEST2MemImm2
   0,                                          // TEST4MemImm4

   IA32OpProp2_LongTarget                    | // TEST8MemImm4 (AMD64)
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_FusableCompare,                 // TEST1RegReg
   IA32OpProp2_FusableCompare,                 // TEST2RegReg
   IA32OpProp2_FusableCompare,                 // TEST4RegReg

   IA32OpProp2_LongSource                    | // TEST8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_FusableCompare                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_FusableCompare,                 // TEST1MemReg
   IA32OpProp2_FusableCompare,                 // TEST2MemReg
   IA32OpProp2_FusableCompare,                 // TEST4MemReg

   IA32OpProp2_LongSource                    | // TEST8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_FusableCompare                |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // XABORT
   0,                                          // XBEGIN2
   0,                                          // XBEGIN4

   0,                                          // XCHG2AccReg
   0,                                          // XCHG4AccReg

   IA32OpProp2_LongSource                    | // XCHG8AccReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // XCHG1RegReg
   0,                                          // XCHG2RegReg
   0,                                          // XCHG4RegReg

   IA32OpProp2_LongSource                    | // XCHG8RegReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SourceIsMemRef,                 // XCHG1RegMem
   IA32OpProp2_SourceIsMemRef,                 // XCHG2RegMem
   IA32OpProp2_SourceIsMemRef,                 // XCHG4RegMem

   IA32OpProp2_LongSource                    | // XCHG8RegMem (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SupportsLockPrefix,             // XCHG1MemReg
   IA32OpProp2_SupportsLockPrefix,             // XCHG2MemReg
   IA32OpProp2_SupportsLockPrefix,             // XCHG4MemReg

   IA32OpProp2_LongSource                    | // XCHG8MemReg (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_Needs64BitOperandPrefix,

   0,                                          // XEND

   IA32OpProp2_SetsCCForTest,                  // XOR1AccImm1

   IA32OpProp2_SetsCCForTest,                  // XOR2AccImm2

   IA32OpProp2_SetsCCForTest,                  // XOR4AccImm4

   IA32OpProp2_SetsCCForTest                 | // XOR8AccImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // XOR1RegImm1

   IA32OpProp2_SetsCCForTest,                  // XOR2RegImm2

   IA32OpProp2_SetsCCForTest,                  // XOR2RegImms

   IA32OpProp2_SetsCCForTest,                  // XOR4RegImm4

   IA32OpProp2_SetsCCForTest                 | // XOR8RegImm4 (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // XOR4RegImms

   IA32OpProp2_SetsCCForTest                 | // XOR8RegImms (AMD64)
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR1MemImm1
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR2MemImm2
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR2MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR4MemImm4
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR8MemImm4 (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR4MemImms
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR8MemImms (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest,                  // XOR1RegReg

   IA32OpProp2_SetsCCForTest,                  // XOR2RegReg

   IA32OpProp2_SetsCCForTest,                  // XOR4RegReg

   IA32OpProp2_SetsCCForTest                 | // XOR8RegReg (AMD64)
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR1RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // XOR2RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // XOR4RegMem
   IA32OpProp2_SourceIsMemRef,

   IA32OpProp2_SetsCCForTest                 | // XOR8RegMem (AMD64)
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR1MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR2MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR4MemReg
   IA32OpProp2_SupportsLockPrefix,

   IA32OpProp2_SetsCCForTest                 | // XOR8MemReg (AMD64)
   IA32OpProp2_SupportsLockPrefix            |
   IA32OpProp2_LongSource                    |
   IA32OpProp2_LongTarget                    |
   IA32OpProp2_Needs64BitOperandPrefix,

   IA32OpProp2_XMMSource                     | // XORPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // XORPSRegMem
   IA32OpProp2_SourceIsMemRef                |
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // XORPDRegReg
   IA32OpProp2_XMMTarget,

   0,                                          // MFENCE
   0,                                          // LFENCE
   0,                                          // SFENCE

   IA32OpProp2_NeedsSSE42OpcodePrefix        | // PCMPESTRI
   IA32OpProp2_XMMSource                     |
   IA32OpProp2_XMMTarget,

   0,                                          // PREFETCHNTA
   0,                                          // PREFETCHT0
   0,                                          // PREFETCHT1
   0,                                          // PREFETCHT2

   IA32OpProp2_XMMSource                     | // ANDNPSRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // ANDNPDRegReg
   IA32OpProp2_XMMTarget,

   IA32OpProp2_XMMSource                     | // PSLLQRegImm1
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_XMMSource                     | // PSRLQRegImm1
   IA32OpProp2_XMMTarget                     |
   IA32OpProp2_NeedsSSE42OpcodePrefix,

   IA32OpProp2_CannotBeAssembled,              // FENCE
   IA32OpProp2_CannotBeAssembled,              // VGFENCE
   IA32OpProp2_CannotBeAssembled,              // PROCENTRY
   IA32OpProp2_LongImmediate,                  // DQImm64 (AMD64)
   0,                                          // DDImm4
   0,                                          // DWImm2
   0,                                          // DBImm1
   IA32OpProp2_PseudoOp                      | // WRTBAR
   IA32OpProp2_CannotBeAssembled,
   IA32OpProp2_PseudoOp                      | // ASSOCREGS
   IA32OpProp2_CannotBeAssembled,
   IA32OpProp2_PseudoOp                      | // FPREGSPILL
   IA32OpProp2_CannotBeAssembled,
   IA32OpProp2_CannotBeAssembled,              // VirtualGuardNOP
   0,                                          // LABEL
   IA32OpProp2_PseudoOp                      | // FCMPEVAL
   IA32OpProp2_CannotBeAssembled,
   IA32OpProp2_CannotBeAssembled,              // RestoreVMThread
   IA32OpProp2_CannotBeAssembled,              // PPS_OPCOUNT
   IA32OpProp2_CannotBeAssembled,              // PPS_OPFIELD
   IA32OpProp2_CannotBeAssembled,              // AdjustFramePtr
   IA32OpProp2_PseudoOp                      | // ReturnMarker
   IA32OpProp2_CannotBeAssembled,
   };

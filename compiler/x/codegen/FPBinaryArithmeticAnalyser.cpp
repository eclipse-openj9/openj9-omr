/*******************************************************************************
 * Copyright (c) 2000, 2021 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
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
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "x/codegen/FPBinaryArithmeticAnalyser.hpp"

#include <stddef.h>
#include <stdint.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/ConstantDataSnippet.hpp"
#include "env/FrontEnd.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "compile/Compilation.hpp"
#include "compile/ResolvedMethod.hpp"
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "il/ResolvedMethodSymbol.hpp"
#include "codegen/X86Instruction.hpp"
#include "codegen/InstOpCode.hpp"

uint8_t
TR_X86FPBinaryArithmeticAnalyser::getIA32FPOpPackage(TR::Node *node)
   {
   uint8_t     package;
   TR::ILOpCode op = node->getOpCode();

   switch (op.getOpCodeValue())
      {
      case TR::fadd:  package = kFADDpackage; break;
      case TR::dadd:  package = kDADDpackage; break;
      case TR::fsub:  package = kFSUBpackage; break;
      case TR::dsub:  package = kDSUBpackage; break;
      case TR::fmul:  package = kFMULpackage; break;
      case TR::dmul:  package = kDMULpackage; break;
      case TR::fdiv:  package = kFDIVpackage; break;
      case TR::ddiv:  package = kDDIVpackage; break;
      default:       package = kBADpackage;  break;
      }

   return package;
   }

void TR_X86FPBinaryArithmeticAnalyser::setInputs(TR::Node     *firstChild,
                                                  TR::Register *firstRegister,
                                                  TR::Node     *secondChild,
                                                  TR::Register *secondRegister)
   {

   if (firstRegister)
      {
      setReg1();
      }

   if (secondRegister)
      {
      setReg2();
      }

   if (firstChild->getOpCode().isMemoryReference() &&
       firstChild->getReferenceCount() == 1)
      {
      setMem1();
      }

   if (secondChild->getOpCode().isMemoryReference() &&
       secondChild->getReferenceCount() == 1)
      {
      setMem2();
      }

   if ((firstChild->getReferenceCount() == 1) &&
       isIntToFPConversion(firstChild))
      {
      setConv1();
      }

   if ((secondChild->getReferenceCount() == 1) &&
       isIntToFPConversion(secondChild))
      {
      setConv2();
      }

   if (firstChild->getReferenceCount() == 1)
      {
      setClob1();
      }

   if (secondChild->getReferenceCount() == 1)
      {
      setClob2();
      }
   }


uint8_t
TR_X86FPBinaryArithmeticAnalyser::getIA32FPOpPackage(TR::ILOpCodes op)
   {
   uint8_t package = 0;

   switch (op)
      {
      case TR::fadd:  package = kFADDpackage; break;
      case TR::dadd:  package = kDADDpackage; break;
      case TR::fsub:  package = kFSUBpackage; break;
      case TR::dsub:  package = kDSUBpackage; break;
      case TR::fmul:  package = kFMULpackage; break;
      case TR::dmul:  package = kDMULpackage; break;
      case TR::fdiv:  package = kFDIVpackage; break;
      case TR::ddiv:  package = kDDIVpackage; break;
      default:       package = kBADpackage;  break;
      }

   return package;
   }


bool
TR_X86FPBinaryArithmeticAnalyser::isIntToFPConversion(TR::Node *child)
   {
   /* conversion to float can't use the direct memory float int
      instructions because of precision adjustment. We have to
      load them in registers and do spill/reload to truncate the
      precision */
   if (/*child->getOpCodeValue() == TR::i2f ||
       child->getOpCodeValue() == TR::s2f ||*/
       child->getOpCodeValue() == TR::i2d ||
       child->getOpCodeValue() == TR::s2d)
      {
      TR::Node *convChild = child->getFirstChild();
      if (convChild->getRegister() == NULL && convChild->getReferenceCount() == 1)
         {
         if (convChild->getOpCode().isLoadVar())
            {
            return true;
            }
         }
      }

   return false;
   }

const TR::InstOpCode::Mnemonic TR_X86FPBinaryArithmeticAnalyser::_opCodePackage[kNumFPPackages][kNumFPArithVariants] =
   {
   // PACKAGE
   //        reg1Reg2      reg2Reg1      reg1Mem2      reg2Mem1
   //        reg1ConvS2    reg1ConvI2    reg2ConvS1    reg2ConvI1

   // Unknown
           { TR::InstOpCode::UD2,    TR::InstOpCode::UD2,    TR::InstOpCode::UD2,    TR::InstOpCode::UD2,
             TR::InstOpCode::UD2,    TR::InstOpCode::UD2,    TR::InstOpCode::UD2,    TR::InstOpCode::UD2 },

   // fadd
           { TR::InstOpCode::FADDRegReg,   TR::InstOpCode::FADDRegReg,   TR::InstOpCode::FADDRegMem,   TR::InstOpCode::FADDRegMem,
             TR::InstOpCode::FSADDRegMem,  TR::InstOpCode::FIADDRegMem,  TR::InstOpCode::FSADDRegMem,  TR::InstOpCode::FIADDRegMem },

   // dadd
           { TR::InstOpCode::DADDRegReg,   TR::InstOpCode::DADDRegReg,   TR::InstOpCode::DADDRegMem,   TR::InstOpCode::DADDRegMem,
             TR::InstOpCode::DSADDRegMem,  TR::InstOpCode::DIADDRegMem,  TR::InstOpCode::DSADDRegMem,  TR::InstOpCode::DIADDRegMem },

   // fmul
           { TR::InstOpCode::FMULRegReg,   TR::InstOpCode::FMULRegReg,   TR::InstOpCode::FMULRegMem,   TR::InstOpCode::FMULRegMem,
             TR::InstOpCode::FSMULRegMem,  TR::InstOpCode::FIMULRegMem,  TR::InstOpCode::FSMULRegMem,  TR::InstOpCode::FIMULRegMem },

   // dmul
           { TR::InstOpCode::DMULRegReg,   TR::InstOpCode::DMULRegReg,   TR::InstOpCode::DMULRegMem,   TR::InstOpCode::DMULRegMem,
             TR::InstOpCode::DSMULRegMem,  TR::InstOpCode::DIMULRegMem,  TR::InstOpCode::DSMULRegMem,  TR::InstOpCode::DIMULRegMem },

   // fsub
           { TR::InstOpCode::FSUBRegReg,   TR::InstOpCode::FSUBRRegReg,  TR::InstOpCode::FSUBRegMem,   TR::InstOpCode::FSUBRRegMem,
             TR::InstOpCode::FSSUBRegMem,  TR::InstOpCode::FISUBRegMem,  TR::InstOpCode::FSSUBRRegMem, TR::InstOpCode::FISUBRRegMem },

   // dsub
           { TR::InstOpCode::DSUBRegReg,   TR::InstOpCode::DSUBRRegReg,  TR::InstOpCode::DSUBRegMem,   TR::InstOpCode::DSUBRRegMem,
             TR::InstOpCode::DSSUBRegMem,  TR::InstOpCode::DISUBRegMem,  TR::InstOpCode::DSSUBRRegMem, TR::InstOpCode::DISUBRRegMem },

   // fdiv
           { TR::InstOpCode::FDIVRegReg,   TR::InstOpCode::FDIVRRegReg,  TR::InstOpCode::FDIVRegMem,   TR::InstOpCode::FDIVRRegMem,
             TR::InstOpCode::FSDIVRegMem,  TR::InstOpCode::FIDIVRegMem,  TR::InstOpCode::FSDIVRRegMem, TR::InstOpCode::FIDIVRRegMem },

   // ddiv
           { TR::InstOpCode::DDIVRegReg,   TR::InstOpCode::DDIVRRegReg,  TR::InstOpCode::DDIVRegMem,   TR::InstOpCode::DDIVRRegMem,
             TR::InstOpCode::DSDIVRegMem,  TR::InstOpCode::DIDIVRegMem,  TR::InstOpCode::DSDIVRRegMem, TR::InstOpCode::DIDIVRRegMem }
   };


const uint8_t TR_X86FPBinaryArithmeticAnalyser::_actionMap[NUM_ACTION_SETS] =
   {
   // This table occupies NUM_ACTION_SETS bytes of storage.

   // RT = Target in register       RS = Source in register
   // MT = Target in memory         MS = Source in memory
   // CT = Target is clobberable    CS = Source is clobberable
   // VT = Target is conversion     VS = Source is conversion

   // RT MT CT VT RS MS CS VS
   /* 0  0  0  0  0  0  0  0 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  0  0  0  1 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  0  0  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  0  0  0  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  0  0  1  0  0 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  0  1  0  1 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  0  1  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  0  0  1  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  0  1  0  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  1  0  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  1  0  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  0  1  0  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  0  1  1  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  1  1  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  0  1  1  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  0  1  1  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  1  0  0  0  0 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  0  0  0  1 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  0  0  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  1  0  0  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  1  0  1  0  0 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  0  1  0  1 */    kEvalTarget | kEvalSource | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  0  1  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  1  0  1  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  0  0  1  1  0  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  1  0  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  1  0  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  1  1  0  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  1  1  1  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  1  1  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  0  0  1  1  1  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  0  1  1  1  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  0  1  0  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  0  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  0  0  0  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  0  0  0  1  1 */    kEvalTarget | kReg1Conv2,
   /* 0  0  1  0  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  0  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  0  0  1  1  0 */    kEvalTarget | kReg1Mem2,
   /* 0  0  1  0  0  1  1  1 */    kEvalTarget | kReg1Mem2,
   /* 0  0  1  0  1  0  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  0  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  0  1  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  0  1  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  1  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  1  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  1  1  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  0  1  1  1  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  1  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  1  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  1  0  0  1  0 */    kEvalSource | kReg1Conv2 | kReverse,
   /* 0  0  1  1  0  0  1  1 */    kEvalTarget | kReg1Conv2,
   /* 0  0  1  1  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  1  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  0  1  1  0  1  1  0 */    kEvalTarget | kReg1Mem2,
   /* 0  0  1  1  0  1  1  1 */    kEvalTarget | kReg1Mem2,
   /* 0  0  1  1  1  0  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  1  1  0  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  1  1  0  1  0 */    kReg1Conv2 | kReverse,
   /* 0  0  1  1  1  0  1  1 */    kReg1Conv2 | kReverse,
   /* 0  0  1  1  1  1  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  1  1  1  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  0  1  1  1  1  1  0 */    kReg1Conv2 | kReverse,
   /* 0  0  1  1  1  1  1  1 */    kReg1Conv2 | kReverse,
   /* 0  1  0  0  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  0  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  0  0  0  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  0  0  0  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  0  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  0  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  0  0  1  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  0  0  1  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  0  1  0  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  0  1  0  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  0  1  0  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  0  1  0  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  0  1  1  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  0  1  1  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  0  1  1  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  0  1  1  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  1  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  1  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  1  0  0  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  1  0  0  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  1  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  1  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  0  1  0  1  1  0 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  1  0  1  1  1 */    kEvalTarget | kEvalSource | kReg1Reg2 | kReverse,
   /* 0  1  0  1  1  0  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  1  1  0  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  1  1  0  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  1  1  0  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  1  1  1  0  0 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  1  1  1  0  1 */    kEvalTarget | kCopyTarget | kReg1Reg2,
   /* 0  1  0  1  1  1  1  0 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  0  1  1  1  1  1 */    kEvalTarget | kReg1Reg2 | kReverse,
   /* 0  1  1  0  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  0  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  0  0  0  1  0 */    kEvalSource | kReg1Mem2 | kReverse,
   /* 0  1  1  0  0  0  1  1 */    kEvalSource | kReg1Mem2 | kReverse,
   /* 0  1  1  0  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  0  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  0  0  1  1  0 */    kEvalTarget | kReg1Mem2,
   /* 0  1  1  0  0  1  1  1 */    kEvalTarget | kReg1Mem2,
   /* 0  1  1  0  1  0  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  0  1  0  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  0  1  0  1  0 */    kReg1Mem2 | kReverse,
   /* 0  1  1  0  1  0  1  1 */    kReg1Mem2 | kReverse,
   /* 0  1  1  0  1  1  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  0  1  1  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  0  1  1  1  0 */    kReg1Mem2 | kReverse,
   /* 0  1  1  0  1  1  1  1 */    kReg1Mem2 | kReverse,
   /* 0  1  1  1  0  0  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  1  0  0  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  1  0  0  1  0 */    kEvalSource | kReg1Mem2 | kReverse,
   /* 0  1  1  1  0  0  1  1 */    kEvalSource | kReg1Mem2 | kReverse,
   /* 0  1  1  1  0  1  0  0 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  1  0  1  0  1 */    kEvalTarget | kEvalSource | kReg1Reg2,
   /* 0  1  1  1  0  1  1  0 */    kEvalTarget | kReg1Mem2,
   /* 0  1  1  1  0  1  1  1 */    kEvalTarget | kReg1Mem2,
   /* 0  1  1  1  1  0  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  1  1  0  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  1  1  0  1  0 */    kReg1Mem2 | kReverse,
   /* 0  1  1  1  1  0  1  1 */    kReg1Mem2 | kReverse,
   /* 0  1  1  1  1  1  0  0 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  1  1  1  0  1 */    kEvalTarget | kReg1Reg2,
   /* 0  1  1  1  1  1  1  0 */    kReg1Mem2 | kReverse,
   /* 0  1  1  1  1  1  1  1 */    kReg1Mem2 | kReverse,
   /* 1  0  0  0  0  0  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  0  0  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  0  0  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  0  0  0  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  0  0  1  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  0  1  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  0  1  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  0  0  1  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  0  1  0  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  1  0  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  1  0  1  0 */    kReg1Reg2 | kReverse,
   /* 1  0  0  0  1  0  1  1 */    kReg1Reg2 | kReverse,
   /* 1  0  0  0  1  1  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  1  1  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  0  1  1  1  0 */    kReg1Reg2 | kReverse,
   /* 1  0  0  0  1  1  1  1 */    kReg1Reg2 | kReverse,
   /* 1  0  0  1  0  0  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  0  0  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  0  0  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  1  0  0  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  1  0  1  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  0  1  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  0  1  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  1  0  1  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  0  0  1  1  0  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  1  0  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  1  0  1  0 */    kReg1Reg2 | kReverse,
   /* 1  0  0  1  1  0  1  1 */    kReg1Reg2 | kReverse,
   /* 1  0  0  1  1  1  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  1  1  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  0  0  1  1  1  1  0 */    kReg1Reg2 | kReverse,
   /* 1  0  0  1  1  1  1  1 */    kReg1Reg2 | kReverse,
   /* 1  0  1  0  0  0  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  0  0  0  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  0  0  0  1  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  0  0  0  1  1 */    kReg1Conv2,
   /* 1  0  1  0  0  1  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  0  0  1  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  0  0  1  1  0 */    kReg1Mem2,
   /* 1  0  1  0  0  1  1  1 */    kReg1Mem2,
   /* 1  0  1  0  1  0  0  0 */    kReg1Reg2,
   /* 1  0  1  0  1  0  0  1 */    kReg1Reg2,
   /* 1  0  1  0  1  0  1  0 */    kReg1Reg2,
   /* 1  0  1  0  1  0  1  1 */    kReg1Reg2,
   /* 1  0  1  0  1  1  0  0 */    kReg1Reg2,
   /* 1  0  1  0  1  1  0  1 */    kReg1Reg2,
   /* 1  0  1  0  1  1  1  0 */    kReg1Reg2,
   /* 1  0  1  0  1  1  1  1 */    kReg1Reg2,
   /* 1  0  1  1  0  0  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  1  0  0  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  1  0  0  1  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  1  0  0  1  1 */    kReg1Conv2,
   /* 1  0  1  1  0  1  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  1  0  1  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  0  1  1  0  1  1  0 */    kReg1Mem2,
   /* 1  0  1  1  0  1  1  1 */    kReg1Mem2,
   /* 1  0  1  1  1  0  0  0 */    kReg1Reg2,
   /* 1  0  1  1  1  0  0  1 */    kReg1Reg2,
   /* 1  0  1  1  1  0  1  0 */    kReg1Reg2,
   /* 1  0  1  1  1  0  1  1 */    kReg1Reg2,
   /* 1  0  1  1  1  1  0  0 */    kReg1Reg2,
   /* 1  0  1  1  1  1  0  1 */    kReg1Reg2,
   /* 1  0  1  1  1  1  1  0 */    kReg1Reg2,
   /* 1  0  1  1  1  1  1  1 */    kReg1Reg2,
   /* 1  1  0  0  0  0  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  0  0  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  0  0  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  0  0  0  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  0  0  1  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  0  1  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  0  1  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  0  0  1  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  0  1  0  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  1  0  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  1  0  1  0 */    kReg1Reg2 | kReverse,
   /* 1  1  0  0  1  0  1  1 */    kReg1Reg2 | kReverse,
   /* 1  1  0  0  1  1  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  1  1  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  0  1  1  1  0 */    kReg1Reg2 | kReverse,
   /* 1  1  0  0  1  1  1  1 */    kReg1Reg2 | kReverse,
   /* 1  1  0  1  0  0  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  0  0  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  0  0  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  1  0  0  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  1  0  1  0  0 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  0  1  0  1 */    kEvalSource | kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  0  1  1  0 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  1  0  1  1  1 */    kEvalSource | kReg1Reg2 | kReverse,
   /* 1  1  0  1  1  0  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  1  0  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  1  0  1  0 */    kReg1Reg2 | kReverse,
   /* 1  1  0  1  1  0  1  1 */    kReg1Reg2 | kReverse,
   /* 1  1  0  1  1  1  0  0 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  1  1  0  1 */    kCopyTarget | kReg1Reg2,
   /* 1  1  0  1  1  1  1  0 */    kReg1Reg2 | kReverse,
   /* 1  1  0  1  1  1  1  1 */    kReg1Reg2 | kReverse,
   /* 1  1  1  0  0  0  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  0  0  0  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  0  0  0  1  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  0  0  0  1  1 */    kReg1Conv2,
   /* 1  1  1  0  0  1  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  0  0  1  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  0  0  1  1  0 */    kReg1Mem2,
   /* 1  1  1  0  0  1  1  1 */    kReg1Mem2,
   /* 1  1  1  0  1  0  0  0 */    kReg1Reg2,
   /* 1  1  1  0  1  0  0  1 */    kReg1Reg2,
   /* 1  1  1  0  1  0  1  0 */    kReg1Reg2,
   /* 1  1  1  0  1  0  1  1 */    kReg1Reg2,
   /* 1  1  1  0  1  1  0  0 */    kReg1Reg2,
   /* 1  1  1  0  1  1  0  1 */    kReg1Reg2,
   /* 1  1  1  0  1  1  1  0 */    kReg1Reg2,
   /* 1  1  1  0  1  1  1  1 */    kReg1Reg2,
   /* 1  1  1  1  0  0  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  1  0  0  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  1  0  0  1  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  1  0  0  1  1 */    kReg1Conv2,
   /* 1  1  1  1  0  1  0  0 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  1  0  1  0  1 */    kEvalSource | kReg1Reg2,
   /* 1  1  1  1  0  1  1  0 */    kReg1Conv2,
   /* 1  1  1  1  0  1  1  1 */    kReg1Conv2,
   /* 1  1  1  1  1  0  0  0 */    kReg1Reg2,
   /* 1  1  1  1  1  0  0  1 */    kReg1Reg2,
   /* 1  1  1  1  1  0  1  0 */    kReg1Reg2,
   /* 1  1  1  1  1  0  1  1 */    kReg1Reg2,
   /* 1  1  1  1  1  1  0  0 */    kReg1Reg2,
   /* 1  1  1  1  1  1  0  1 */    kReg1Reg2,
   /* 1  1  1  1  1  1  1  0 */    kReg1Reg2,
   /* 1  1  1  1  1  1  1  1 */    kReg1Reg2
   };

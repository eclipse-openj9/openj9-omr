/*******************************************************************************
 * Copyright (c) 2000, 2022 IBM Corp. and others
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

#include "codegen/CodeGenerator.hpp"
#include "env/FrontEnd.hpp"
#include "codegen/LiveRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterConstants.hpp"
#include "codegen/TreeEvaluator.hpp"
#include "compile/Compilation.hpp"
#include "il/ILOpCodes.hpp"
#include "il/ILOps.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "codegen/X86Instruction.hpp"
#include "codegen/InstOpCode.hpp"
#include "env/CompilerEnv.hpp"

extern TR::Register *intOrLongClobberEvaluate(TR::Node *node, bool nodeIs64Bit, TR::CodeGenerator *cg);

TR::Register *OMR::X86::TreeEvaluator::unaryVectorArithmeticEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *valueNode = node->getChild(0);
   TR::Register *resultReg = cg->allocateRegister(TR_VRF);

   bool supportsAvx = cg->comp()->target().cpu.supportsAVX();
   TR::InstOpCode::Mnemonic regRegOpcode;
   TR::InstOpCode::Mnemonic regMemOpcode;

   switch (node->getOpCodeValue())
      {
      case TR::vdsqrt:
         regRegOpcode = supportsAvx ? OMR::InstOpCode::VSQRTPDRegReg : OMR::InstOpCode::SQRTPDRegReg;
         regMemOpcode = supportsAvx ? OMR::InstOpCode::VSQRTPDRegMem : OMR::InstOpCode::bad;
         // SSE RegMem instruction requires 16-byte alignment
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, 0, "Opcode not supported by unaryVectorArithmeticEvaluator");
         break;
      }

   if (valueNode->getRegister() == NULL && valueNode->getReferenceCount() == 1 && regMemOpcode != TR::InstOpCode::bad)
      {
      TR::MemoryReference *mr = generateX86MemoryReference(valueNode, cg);
      generateRegMemInstruction(regMemOpcode, node, resultReg, mr, cg);
      mr->decNodeReferenceCounts(cg);
      }
   else
      {
      TR_ASSERT_FATAL(regRegOpcode != TR::InstOpCode::bad, "Illegal opcode for unary operation");
      TR::Register *valueReg = cg->evaluate(valueNode);
      generateRegRegInstruction(regRegOpcode, node, resultReg, valueReg, cg);
      cg->decReferenceCount(valueNode);
      }

   node->setRegister(resultReg);

   return resultReg;
   }

TR::Register *OMR::X86::TreeEvaluator::bconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = TR::TreeEvaluator::loadConstant(node, node->getInt(), TR_RematerializableByte, cg);
   node->setRegister(reg);

   if (cg->enableRegisterInterferences())
      cg->getLiveRegisters(TR_GPR)->setByteRegisterAssociation(reg);

   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::sconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = TR::TreeEvaluator::loadConstant(node, node->getInt(), TR_RematerializableShort, cg);
   node->setRegister(reg);
   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::iconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = TR::TreeEvaluator::loadConstant(node, node->getInt(), TR_RematerializableInt, cg);
   node->setRegister(reg);
   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::negEvaluatorHelper(TR::Node        *node,
                                               TR::InstOpCode::Mnemonic  negInstr,
                                               TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *targetRegister = TR::TreeEvaluator::intOrLongClobberEvaluate(firstChild, TR::TreeEvaluator::getNodeIs64Bit(node, cg), cg);
   node->setRegister(targetRegister);
   cg->decReferenceCount(firstChild);
   generateRegInstruction(negInstr, node, targetRegister, cg);
   return targetRegister;
   }

TR::Register *OMR::X86::TreeEvaluator::integerNegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::negEvaluatorHelper(node, TR::InstOpCode::NEGReg(TR::TreeEvaluator::getNodeIs64Bit(node, cg)), cg);
   }

TR::Register *OMR::X86::TreeEvaluator::integerAbsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   auto child = node->getFirstChild();
   auto value = cg->evaluate(child);
   auto result = cg->allocateRegister(value->getKind());

   auto is64Bit = TR::TreeEvaluator::getNodeIs64Bit(node, cg);
   generateRegRegInstruction(TR::InstOpCode::MOVRegReg(is64Bit), node, result, value, cg);
   generateRegInstruction(TR::InstOpCode::NEGReg(is64Bit), node, result, cg);
   generateRegRegInstruction(TR::InstOpCode::CMOVSRegReg(is64Bit), node, result, value, cg);

   node->setRegister(result);
   cg->decReferenceCount(child);
   return result;
   }

TR::Register*
OMR::X86::TreeEvaluator::vnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::DataType type = node->getDataType();

   TR_ASSERT_FATAL_WITH_NODE(node, type.getVectorLength() == TR::VectorLength128,
                             "Only 128-bit vectors are supported right now\n");

   TR::Node *valueNode = node->getChild(0);
   TR::Register *resultReg = cg->allocateRegister(TR_VRF);
   TR::Register *valueReg = cg->evaluate(valueNode);

   // -valueReg = 0 - valueReg
   generateRegRegInstruction(TR::InstOpCode::PXORRegReg, node, resultReg, resultReg, cg);
   TR::InstOpCode::Mnemonic subOpcode;

   switch (type.getVectorElementType())
      {
      case TR::Int8:
         subOpcode = TR::InstOpCode::PSUBBRegReg;
         break;
      case TR::Int16:
         subOpcode = TR::InstOpCode::PSUBWRegReg;
         break;
      case TR::Int32:
         subOpcode = TR::InstOpCode::PSUBDRegReg;
         break;
      case TR::Int64:
         subOpcode = TR::InstOpCode::PSUBQRegReg;
         break;
      case TR::Float:
         subOpcode = TR::InstOpCode::SUBPSRegReg;
         break;
      case TR::Double:
         subOpcode = TR::InstOpCode::SUBPDRegReg;
         break;
      default:
         TR_ASSERT_FATAL_WITH_NODE(node, 0, "Unsupported data type for vneg opcode.");
         break;
      }

   generateRegRegInstruction(subOpcode, node, resultReg, valueReg, cg);

   node->setRegister(resultReg);
   cg->decReferenceCount(valueNode);

   return resultReg;
   }

TR::Register *OMR::X86::TreeEvaluator::bnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *targetRegister = TR::TreeEvaluator::negEvaluatorHelper(node, TR::InstOpCode::NEG1Reg, cg);

   if (cg->enableRegisterInterferences())
      cg->getLiveRegisters(TR_GPR)->setByteRegisterAssociation(targetRegister);

   return targetRegister;
   }

TR::Register *OMR::X86::TreeEvaluator::snegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::negEvaluatorHelper(node, TR::InstOpCode::NEG2Reg, cg);
   }

// also handles i2s, i2c, s2b, a2s, a2c, a2b, a2bu
TR::Register *OMR::X86::TreeEvaluator::i2bEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   static const char *narrowLoads = feGetEnv("TR_NarrowLoads");
   if (narrowLoads &&
       firstChild->getReferenceCount() == 1 &&
       firstChild->getRegister() == 0       &&
       firstChild->getOpCode().isLoadVar())
      {
      TR::ILOpCodes op = node->getOpCodeValue();
      if (op == TR::s2b)
         {
         if (firstChild->getOpCode().isLoadIndirect())
            {
            TR::Node::recreate(firstChild, TR::bloadi);
            }
         else
            {
            TR::Node::recreate(firstChild, TR::bload);
            }
         }
      }
   node->setRegister(cg->intClobberEvaluate(firstChild));
   cg->decReferenceCount(firstChild);

   if (cg->enableRegisterInterferences() && node->getOpCode().getSize() == 1)
      cg->getLiveRegisters(TR_GPR)->setByteRegisterAssociation(node->getRegister());

   return node->getRegister();
   }

TR::Register *OMR::X86::TreeEvaluator::a2lEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *child = node->getFirstChild();
   TR::Register *srcReg = cg->evaluate(child);

   if (child->getReferenceCount() > 1)
      {
      TR_RegisterKinds kind = srcReg->getKind();
      // dont allocate a collected register here
      // even if srcReg is collected
      //
      TR::Register *copyReg = cg->allocateRegister(kind);

      if (srcReg->containsInternalPointer())
         {
         copyReg->setPinningArrayPointer(srcReg->getPinningArrayPointer());
         copyReg->setContainsInternalPointer();
         }

      generateRegRegInstruction(TR::InstOpCode::MOVRegReg(), node, copyReg, srcReg, cg);
      srcReg = copyReg;
      }

   node->setRegister(srcReg);
   cg->decReferenceCount(child);
   return srcReg;
   }

TR::Register *OMR::X86::TreeEvaluator::l2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   // evaluator in use only for cg->comp()->useCompressedPointers
   //
   // pattern match the sequence under the l2a
   //    iaload f      l2a                       <- node
   //       aload O       ladd
   //                       lshl
   //                          i2l
   //                            iiload f        <- load
   //                               aload O
   //                          iconst shftKonst
   //                       lconst HB
   //
   // -or- if the load is known to be null or usingLowMemHeap
   //  l2a
   //    i2l
   //      iiload f
   //         aload O
   //
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *source = cg->evaluate(firstChild);
   if (cg->comp()->useCompressedPointers() && source && (TR::Compiler->om.compressedReferenceShift() == 0 || firstChild->containsCompressionSequence()) && !node->isl2aForCompressedArrayletLeafLoad())
      source->setContainsCollectedReference();
   node->setRegister(source);
   cg->decReferenceCount(node->getFirstChild());

   return source;
   }

TR::Register *OMR::X86::TreeEvaluator::i2aEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *source = cg->evaluate(node->getFirstChild());
   // i2a is not a valid pattern in Java,
   // and setting this as a collected reference is causing the GC to crash.
   // source->setContainsCollectedReference()
   node->setRegister(source);
   cg->decReferenceCount(node->getFirstChild());
   return source;
   }
//

// i2c handled by i2b

// i2s handled by i2b

// also handles l2b, l2s

// l2b handled by l2i

// l2s handled by l2i

TR::Register *OMR::X86::TreeEvaluator::b2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::InstOpCode::Mnemonic reg4mem1Op,reg4reg1Op;
   if(node->isUnneededConversion())
     {
     reg4mem1Op = TR::InstOpCode::MOVZXReg4Mem1; // these are slightly cheaper
     reg4reg1Op = TR::InstOpCode::MOVZXReg4Reg1; // and valid since upper bytes tossed anyways
     }
   else
     {
     reg4mem1Op = TR::InstOpCode::MOVSXReg4Mem1;
     reg4reg1Op = TR::InstOpCode::MOVSXReg4Reg1;
     }
   return TR::TreeEvaluator::conversionAnalyser(node, reg4mem1Op, reg4reg1Op, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::bu2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVZXReg4Mem1, TR::InstOpCode::MOVZXReg4Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::b2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVSXReg2Mem1, TR::InstOpCode::MOVSXReg2Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::bu2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVZXReg2Mem1, TR::InstOpCode::MOVZXReg2Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::s2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVSXReg4Mem2, TR::InstOpCode::MOVSXReg4Reg2, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::su2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVZXReg4Mem2, TR::InstOpCode::MOVZXReg4Reg2, cg);
   }

// s2b handled by i2b

TR::Register *OMR::X86::TreeEvaluator::c2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return TR::TreeEvaluator::conversionAnalyser(node, TR::InstOpCode::MOVZXReg4Mem2, TR::InstOpCode::MOVZXReg4Reg2, cg);
   }

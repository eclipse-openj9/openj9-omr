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

#include "codegen/CodeGenerator.hpp"      // for CodeGenerator
#include "codegen/FrontEnd.hpp"           // for feGetEnv, TR_FrontEnd
#include "codegen/LiveRegister.hpp"       // for TR_LiveRegisters
#include "codegen/Register.hpp"           // for Register
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds::TR_GPR, etc
#include "codegen/TreeEvaluator.hpp"      // for TreeEvaluator
#include "compile/Compilation.hpp"        // for Compilation
#include "il/ILOpCodes.hpp"               // for ILOpCodes, etc
#include "il/ILOps.hpp"                   // for ILOpCode
#include "il/Node.hpp"                    // for Node, etc
#include "il/Node_inlines.hpp"            // for Node::getFirstChild, etc
#include "codegen/X86Instruction.hpp"   // for generateRegRegInstruction, etc
#include "x/codegen/X86Ops.hpp"           // for MOVRegReg, TR_X86OpCodes, etc
#include "env/CompilerEnv.hpp"

extern TR::Register *intOrLongClobberEvaluate(TR::Node *node, bool nodeIs64Bit, TR::CodeGenerator *cg);

TR::Register *OMR::X86::TreeEvaluator::bconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = loadConstant(node, node->getInt(), TR_RematerializableByte, cg);
   node->setRegister(reg);

   if (cg->enableRegisterInterferences())
      cg->getLiveRegisters(TR_GPR)->setByteRegisterAssociation(reg);

   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::sconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = loadConstant(node, node->getInt(), TR_RematerializableShort, cg);
   node->setRegister(reg);
   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::cconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = loadConstant(node, node->getInt(), TR_RematerializableChar, cg);
   node->setRegister(reg);
   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::iconstEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *reg = loadConstant(node, node->getInt(), TR_RematerializableInt, cg);
   node->setRegister(reg);
   return reg;
   }

TR::Register *OMR::X86::TreeEvaluator::negEvaluatorHelper(TR::Node        *node,
                                               TR_X86OpCodes  negInstr,
                                               TR::CodeGenerator *cg)
   {
   TR::Node *firstChild = node->getFirstChild();
   TR::Register *targetRegister = intOrLongClobberEvaluate(firstChild, getNodeIs64Bit(node, cg), cg);
   node->setRegister(targetRegister);
   cg->decReferenceCount(firstChild);
   generateRegInstruction(negInstr, node, targetRegister, cg);
   return targetRegister;
   }

TR::Register *OMR::X86::TreeEvaluator::integerNegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return negEvaluatorHelper(node, NEGReg(getNodeIs64Bit(node, cg)), cg);
   }

TR::Register *OMR::X86::TreeEvaluator::integerAbsEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *child = node->getFirstChild();
   bool is64Bit = getNodeIs64Bit(node, cg);
   TR::Register *targetRegister = intOrLongClobberEvaluate(child, is64Bit, cg);
   node->setRegister(targetRegister);
   TR::Register *signRegister = cg->allocateRegister(TR_GPR);
   generateRegRegInstruction(MOVRegReg(is64Bit), node, signRegister, targetRegister, cg);
   generateRegImmInstruction(SARRegImm1(is64Bit), node, signRegister, is64Bit ? 63 : 31, cg);
   generateRegRegInstruction(ADDRegReg(is64Bit), node, targetRegister, signRegister, cg);
   generateRegRegInstruction(XORRegReg(is64Bit), node, targetRegister, signRegister, cg);
   cg->stopUsingRegister(signRegister);
   cg->decReferenceCount(child);
   return targetRegister;
   }

TR::Register *OMR::X86::TreeEvaluator::bnegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Register *targetRegister = negEvaluatorHelper(node, NEG1Reg, cg);

   if (cg->enableRegisterInterferences())
      cg->getLiveRegisters(TR_GPR)->setByteRegisterAssociation(targetRegister);

   return targetRegister;
   }

TR::Register *OMR::X86::TreeEvaluator::snegEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return negEvaluatorHelper(node, NEG2Reg, cg);
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

      generateRegRegInstruction(MOVRegReg(), node, copyReg, srcReg, cg);
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
   TR_X86OpCodes reg4mem1Op,reg4reg1Op;
   if(node->isUnneededConversion())
     {
     reg4mem1Op = MOVZXReg4Mem1; // these are slightly cheaper
     reg4reg1Op = MOVZXReg4Reg1; // and valid since upper bytes tossed anyways
     }
   else
     {
     reg4mem1Op = MOVSXReg4Mem1;
     reg4reg1Op = MOVSXReg4Reg1;
     }
   return conversionAnalyser(node, reg4mem1Op, reg4reg1Op, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::bu2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVZXReg4Mem1, MOVZXReg4Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::b2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVSXReg2Mem1, MOVSXReg2Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::bu2sEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVZXReg2Mem1, MOVZXReg2Reg1, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::s2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVSXReg4Mem2, MOVSXReg4Reg2, cg);
   }

TR::Register *OMR::X86::TreeEvaluator::su2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVZXReg4Mem2, MOVZXReg4Reg2, cg);
   }

// s2b handled by i2b

TR::Register *OMR::X86::TreeEvaluator::c2iEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   return conversionAnalyser(node, MOVZXReg4Mem2, MOVZXReg4Reg2, cg);
   }

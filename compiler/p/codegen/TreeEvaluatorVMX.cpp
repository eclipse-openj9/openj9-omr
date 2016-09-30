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

#include <stddef.h>                              // for size_t
#include <stdint.h>                              // for uint64_t, int32_t, etc
#include <stdio.h>                               // for NULL, fclose, feof, etc
#include <stdlib.h>                              // for strtol
#include <string.h>                              // for strchr, strstr
#include "codegen/BackingStore.hpp"              // for TR_BackingStore
#include "codegen/CodeGenerator.hpp"             // for CodeGenerator, etc
#include "codegen/FrontEnd.hpp"                  // for feGetEnv
#include "codegen/InstOpCode.hpp"                // for InstOpCode, etc
#include "codegen/Instruction.hpp"               // for Instruction
#include "codegen/Linkage.hpp"                   // for addDependency
#include "codegen/MemoryReference.hpp"           // for MemoryReference
#include "codegen/RealRegister.hpp"              // for RealRegister, etc
#include "codegen/Register.hpp"                  // for Register
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterDependency.hpp"
#include "codegen/RegisterDependencyStruct.hpp"  // for RegisterDependency
#include "codegen/TreeEvaluator.hpp"             // for TreeEvaluator
#include "env/CompilerEnv.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"                        // for intptrj_t
#include "il/DataTypes.hpp"                      // for DataTypes::Double, etc
#include "il/ILOpCodes.hpp"                      // for ILOpCodes::fconst
#include "il/ILOps.hpp"                          // for ILOpCode
#include "il/Node.hpp"                           // for Node
#include "il/Node_inlines.hpp"                   // for Node::getDataType, etc
#include "il/symbol/LabelSymbol.hpp"             // for generateLabelSymbol, etc
#include "infra/Assert.hpp"                      // for TR_ASSERT
#include "p/codegen/GenerateInstructions.hpp"

#if defined(AIXPPC)
#include <sys/systemcfg.h>
#endif

// Branch if all are true in VMX
#define PPCOp_bva TR::InstOpCode::blt
// Branch if at least one is false in VMX
#define PPCOp_bvna TR::InstOpCode::bge
// Branch if none is true in VMX
#define PPCOp_bvn TR::InstOpCode::beq
// Branch if at least one is true in VMX
#define PPCOp_bvnn TR::InstOpCode::bne

#define LBZ(dst,offs,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 1, cg))
#define LHZ(dst,offs,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lhz, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 2, cg))
#define LBZU(dst,offs,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 1, cg))
#define LHZU(dst,offs,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lhzu, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 2, cg))
#define LBZX(dst,idx,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(idx##Reg, base##Reg, 1, cg))
#define LHZX(dst,idx,base)	generateTrg1MemInstruction(cg, TR::InstOpCode::lhzx, node, dst##Reg, new (cg->trHeapMemory()) TR::MemoryReference(idx##Reg, base##Reg, 2, cg))
#define STB(src,offs,base)	generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 1, cg), src##Reg)
#define STH(src,offs,base)	generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 2, cg), src##Reg)
#define STBU(src,offs,base)	generateMemSrc1Instruction(cg, TR::InstOpCode::stbu, node, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 1, cg), src##Reg)
#define STHU(src,offs,base)	generateMemSrc1Instruction(cg, TR::InstOpCode::sthu, node, new (cg->trHeapMemory()) TR::MemoryReference(base##Reg, offs, 2, cg), src##Reg)
#define ADD(dst,s1,s2)		generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, dst##Reg, s1##Reg, s2##Reg)
#define SUBF(dst,s1,s2)		generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, dst##Reg, s1##Reg, s2##Reg)
#define ADDI(dst,src,imm)	generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, dst##Reg, src##Reg, imm)
#define ANDI_R(dst,src,imm)	generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, dst##Reg, src##Reg, imm)
#define RLWINM(dst,src,cnt,mask)generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, dst##Reg, src##Reg, cnt, mask)
#define CMP4(cnd,s1,s2)		generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cnd##Reg, s1##Reg, s2##Reg)
#define CMPL4(cnd,s1,s2)	generateTrg1Src2Instruction(cg, TR::InstOpCode::cmpl4, node, cnd##Reg, s1##Reg, s2##Reg)
#define CMPI4(cnd,s1,imm)	generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cnd##Reg, s1##Reg, imm)
#define LABEL(label)		generateLabelInstruction(cg, TR::InstOpCode::label, node, lbl##label)
#define B(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::b, node, lbl##label, cnd##Reg)
#define BEQ(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lbl##label, cnd##Reg)
#define BNE(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lbl##label, cnd##Reg)
#define BDZ(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::bdz, node, lbl##label, cnd##Reg)
#define BGT(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, node, lbl##label, cnd##Reg)
#define BLT(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, lbl##label, cnd##Reg)
#define BGE(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lbl##label, cnd##Reg)
#define BLE(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lbl##label, cnd##Reg)
#define BDNZ(cnd,label)		generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lbl##label, cnd##Reg)
#define MTCTR(src)		generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, src##Reg)
#define MR(dst,src)		generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, dst##Reg, src##Reg)

#define TRACE_VMX0(m)                   	do {if (traceVMX(cg)) printf(m);} while (0)
#define TRACE_VMX1(m,a1)                	do {if (traceVMX(cg)) printf(m,a1);} while (0)
#define TRACE_VMX2(m,a1,a2)             	do {if (traceVMX(cg)) printf(m,a1,a2);} while (0)
#define TRACE_VMX3(m,a1,a2,a3)          	do {if (traceVMX(cg)) printf(m,a1,a2,a3);} while (0)
#define TRACE_VMX4(m,a1,a2,a3,a4)       	do {if (traceVMX(cg)) printf(m,a1,a2,a3,a4);} while (0)
#define TRACE_VMX5(m,a1,a2,a3,a4,a5)    	do {if (traceVMX(cg)) printf(m,a1,a2,a3,a4,a5);} while (0)
#define TRACE_VMX6(m,a1,a2,a3,a4,a5,a6) 	do {if (traceVMX(cg)) printf(m,a1,a2,a3,a4,a5,a6);} while (0)
#define TRACE_VMX7(m,a1,a2,a3,a4,a5,a6,a7)	do {if (traceVMX(cg)) printf(m,a1,a2,a3,a4,a5,a6,a7);} while (0)
#define TRACE_VMX8(m,a1,a2,a3,a4,a5,a6,a7,a8)	do {if (traceVMX(cg)) printf(m,a1,a2,a3,a4,a5,a6,a7,a8);} while (0)

/*
 *  Prototype evaluaters for VMX instructions
 *
 * arraysetEvaluator:
 *   Description:
 *   Subfunctions:
 *     TR::TreeEvaluator::arraysetEvaluator
 *     arraysetConstElemAlign8ConstLen16N	Straight line code utilizing 16byte store
 *     arraysetConstLen				Straight line code
 *     arraysetConstElemAlign8
 *     arraysetGeneric			generic code
 *
 * arraytranslateEvaluator:
 *   Description:
 *   Subfunctions:
 *     TR::TreeEvaluator::arraytranslateEvaluator
 *     arraytranslateTROT
 *     arraytranslateTROTGeneric
 *     arraytranslateTROTSimple256Scalar
 *     arraytranslateTROTSimpleVMX
 *     arraytranslateTRTO
 *     arraytranslateTRTOGenericScalar
 *     arraytranslateTRTOSimpleScalar
 *     arraytranslateTRTOSimpleVMX
 *     arraytranslateTROO
 *     arraytranslateTRTT
 *     analyzeTRTOTable
 *     analyzeTROTTable
 *
 * arraytranslateAndTestEvaluator:
 *   Description:
 *     Emulating SRST instruction of S390
 *     by using either PPC scalar instructions or
 *     VMX instructions.
 *   Subfunctions:
 *     arraytranslateAndTestGenVMX
 *     arraytranslateAndTestGenScalar
 *     findBytesGenTRTCom
 *     findBytesGenSRSTScalar
 *     findBytesGenSRSTVMX
 *
 * arraycmpEvaluator:
 *   Description:
 *   Subfunctions:
 *     TR::TreeEvaluator::arraycmpEvaluator
 *     arraycmpGeneratorScalar
 *     arraycmpCommGeneratorVMXRuntime
 *     arraycmpLenCommGeneratorVMX
 *
 * countDigit10Evaluator:
 *   Description:
 *     Not implemented yet
 *   Subfunctions:
 *     TR::TreeEvaluator::countDigit10Evaluator
 *
 * long2Str10Evaluator:
 *   Description:
 *     Not implemented yet
 *   Subfunctions:
 *     TR::TreeEvaluator::long2Str10Evaluator
 */

#define ARRAYSET_LIMIT1	64			// In bytes
#define ARRAYSET_STORE_GATHER_MIN_ELEMS 4	// In elements

static int getOptVal(char *name, int undef_default, int empty_default);
static int VMXEnabled(TR::CodeGenerator *cg);

#if defined(TR_IDIOM_PPCVMX)
static int getCacheLineSize(void);
static int traceVMX(TR::CodeGenerator *cg);
static int getArraysetNop(TR::CodeGenerator *cg);
static int getVersionLimit(TR::CodeGenerator *cg);
static int getArraycmpUseVMXRuntime(TR::CodeGenerator *cg);
static uint32_t *createBitTable(uint8_t *byteTable, uint8_t *bitTable, TR::CodeGenerator *cg);
static uint32_t *createBitTable(uint16_t *charTable, uint16_t termChar, uint8_t *bitTable, TR::CodeGenerator *cg);
static int log2(int n);
static int arraycmpEstimateElementSize(TR::Node *lenNode, TR::CodeGenerator *cg);
static int getElementSize(TR::Node *elemNode, TR::CodeGenerator *cg);
static int64_t getIntegralValue(TR::Node * node);
static int getOffset(TR::Node *addrNode, TR::CodeGenerator *cg);
static void fillConst16(char *buf, TR::Node *elemNode, TR::CodeGenerator *cg);
#endif

//----------------------------------------------------------------------
// arraysetEvaluator
//----------------------------------------------------------------------
//
// Strategy:
//   - Generate different type of codes according to the condition
//     1. Straight-line code utilizing 16byte store instruction (stfdp or stvx)
//        - Address is known to be 8 byte-aligned (e.g. the first element of an array)
//        - Length is
//	    - known at compile time
//          - a multiple of 16
//          - in the range of [32, 256] bytes
//        - Element data is a constant
//        - stfdq or stvx is available (p6, gpul, cellpx)
//     2. Straight-line code
//        - Length is
//	    - known at compile time
//          - less-than or equal to ARRAYSET_LIMIT1 (64) bytes
//        - Element data is a constant
//     3. Generic (unrolled) code
//        - Othrewise
//
// TODO
//     - See if the length needs to be counted as #elements rather
//       than bytes.
//

#if defined(TR_IDIOM_PPCVMX)
static void arraysetConstElemAlign8ConstLen16N(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, int byteLen, TR::CodeGenerator *cg);
static void arraysetConstLen(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, int byteLen, TR::CodeGenerator *cg);
static void arraysetConstElemAlign8(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, TR::CodeGenerator *cg);
static void arraysetGeneric(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, TR::CodeGenerator *cg);
//static void arraysetNop(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, TR::CodeGenerator *cg);

//#define ARRAYSET_CONSTLEN16N_MAX	256	// bytes
#define ARRAYSET_CONSTLEN16N_MAX	1024	// bytes
#define ARRAYSET_CONSTLEN16N_MIN	32	// bytes
#endif

TR::Register *OMR::Power::TreeEvaluator::arraysetEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *addrNode = node->getFirstChild();
   TR::Node *fillNode = node->getSecondChild();
   TR::Node *lenNode = node->getChild(2);
   uint32_t fillNodeSize = fillNode->getSize();
   TR::DataType fillNodeType = fillNode->getDataType();
   bool constLength = lenNode->getOpCode().isLoadConst();
   int32_t length = constLength ? lenNode->getInt() : 0;
   bool constFill = fillNode->getOpCode().isLoadConst();

   TR::Register *addrReg = cg->gprClobberEvaluate(addrNode);
   TR::Register *fillReg = NULL;
   TR::Register *fp1Reg = NULL;
   TR::Register *lenReg   = cg->gprClobberEvaluate(lenNode);
   TR::Register *tempReg = cg->allocateRegister();
   TR::Register *condReg = cg->allocateRegister(TR_CCR);

   bool dofastPath = ((constLength && constFill && (length >= 64)) || (8 == fillNodeSize));

   if (constFill)
      {
      TR::Instruction *q[4];
      uint64_t doubleword = 0xdeadbeef;
      switch(fillNodeSize)
         {
         case 1:
            {
            uint64_t byte = (uint64_t)(fillNode->getByte() & 0xff);
            uint64_t halfword = (uint64_t)((byte << 8) | byte);
            if (dofastPath)
               {
               doubleword = (halfword << 48) | (halfword << 32) | (halfword << 16) | halfword;
               }
            fillReg = cg->allocateRegister();
            if (dofastPath && TR::Compiler->target.is64Bit())
               {
               generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, fillReg, halfword);
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, fillReg, fillReg,  16, 0xffff0000);
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldimi, node, fillReg, fillReg,  32, 0xffffffff00000000ULL);
               }
            else
               {
               generateTrg1ImmInstruction(cg, TR::InstOpCode::lis, node, fillReg, halfword);
               generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::ori, node, fillReg, fillReg, halfword);
               }
            }
            break;
         case 2:
            {
            uint64_t halfword = (uint64_t)(fillNode->getShortInt() & 0xffff);
            if (dofastPath)
               {
               doubleword = (halfword << 48) | (halfword << 32) | (halfword << 16) | halfword;
               }
            fillReg = cg->allocateRegister();
            if (dofastPath && TR::Compiler->target.is64Bit())
               {
               generateTrg1ImmInstruction(cg, TR::InstOpCode::li, node, fillReg, halfword);
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, fillReg, fillReg,  16, 0xffff0000);
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldimi, node, fillReg, fillReg,  32, 0xffffffff00000000ULL);
               }
            else
               {
               generateTrg1ImmInstruction(cg, TR::InstOpCode::lis, node, fillReg, halfword);
               generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::ori, node, fillReg, fillReg, halfword);
               }
            }
            break;
         case 4:
            {
            uint64_t word = (uint64_t)(fillNode->getOpCodeValue()==TR::fconst ? fillNode->getFloatBits() & 0xffffffff : fillNode->getUnsignedInt() & 0xffffffff);
            if (dofastPath)
               {
               doubleword = (word << 32) | word;
               }
            fillReg = cg->allocateRegister();
            loadConstant(cg, node, ((int32_t)word), fillReg);
            if (dofastPath && TR::Compiler->target.is64Bit())
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldimi, node, fillReg, fillReg,  32, 0xffffffff00000000ULL);
            }
            break;
         case 8:
            {
            if (fillNode->getDataType() == TR::Double)
               {
               fp1Reg = fillNode->getRegister();
               if (!fp1Reg)
                  fp1Reg = cg->evaluate(fillNode);
               fillReg = cg->allocateRegister();
               }
            else if (TR::Compiler->target.is64Bit())  //long: 64 bit target
               fillReg = cg->evaluate(fillNode);
            else                           //long: 32 bit target
               {
               uint64_t longInt = fillNode->getUnsignedLongInt();
               doubleword = longInt;
               fillReg = cg->allocateRegister();
               }
            }
            break;
         default:
            TR_ASSERT( false,"unrecognised address precision\n");
         }

      if (fillNode->getDataType() != TR::Double && dofastPath)
         {
         fp1Reg = cg->allocateRegister(TR_FPR);
         if (TR::Compiler->target.is32Bit())
            {
            fixedSeqMemAccess(cg, node, 0, q, fp1Reg, tempReg, TR::InstOpCode::lfd, 8, NULL, tempReg);
            cg->findOrCreateFloatConstant(&doubleword, TR::Double, q[0], q[1], q[2], q[3]);
            }
         }
      }
   else //variable fill value
      {
      switch(fillNodeSize)
         {
         case 1:
            {
            fillReg = cg->gprClobberEvaluate(fillNode);
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, fillReg, fillReg,  8, 0xff00);
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, fillReg, fillReg,  16, 0xffff0000);
            }
            break;
         case 2:
            {
            fillReg = cg->gprClobberEvaluate(fillNode);
            generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, fillReg, fillReg,  16, 0xffff0000);
            }
            break;
         case 4:
            {
            fillReg = cg->gprClobberEvaluate(fillNode);
            }
            break;
         case 8:
            {
            if (fillNode->getDataType() == TR::Double)
               {
               fp1Reg = cg->evaluate(fillNode);
               }
            else if (TR::Compiler->target.is64Bit())
               {
               fp1Reg = cg->allocateRegister();
               fillReg = cg->evaluate(fillNode);
               }
            else
               {
               TR::Register *valueReg = cg->evaluate(fillNode);
               fp1Reg = cg->allocateRegister(TR_FPR);
               TR_BackingStore * location;
               location = cg->allocateSpill(8, false, NULL);
               TR::MemoryReference *tempMRStore1 = new (cg->trHeapMemory()) TR::MemoryReference(node, location->getSymbolReference(), 4, cg);
               TR::MemoryReference *tempMRStore2 =  new (cg->trHeapMemory()) TR::MemoryReference(node, *tempMRStore1, 4, 4, cg);
               generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, tempMRStore1, valueReg->getHighOrder());
               generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, tempMRStore2, valueReg->getLowOrder());
               generateInstruction(cg, TR::InstOpCode::lwsync, node);
               generateTrg1MemInstruction(cg, TR::InstOpCode::lfd, node, fp1Reg, new (cg->trHeapMemory()) TR::MemoryReference(node, location->getSymbolReference(), 8, cg));
               cg->freeSpill(location, 8, 0);
               }
            if (!fillReg)
               fillReg = cg->allocateRegister();
            }
            break;
         default:
            TR_ASSERT( false,"unrecognised address precision\n");
         }
      }

   if (dofastPath)
      {
      TR::LabelSymbol *label0 = generateLabelSymbol(cg);
      TR::LabelSymbol *label1 = generateLabelSymbol(cg);
      TR::LabelSymbol *label2 = generateLabelSymbol(cg);
      TR::LabelSymbol *label3 = generateLabelSymbol(cg);
      TR::LabelSymbol *label4 = generateLabelSymbol(cg);
      TR::LabelSymbol *label5 = generateLabelSymbol(cg);
      TR::LabelSymbol *label6 = generateLabelSymbol(cg);
      TR::LabelSymbol *label7 = generateLabelSymbol(cg);
      TR::LabelSymbol *label8 = generateLabelSymbol(cg);
      TR::LabelSymbol *label9 = generateLabelSymbol(cg);
      TR::LabelSymbol *label10 = generateLabelSymbol(cg);
      TR::LabelSymbol *donelabel = generateLabelSymbol(cg);

      TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
      addDependency(deps, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
      deps->getPostConditions()->getRegisterDependency(0)->setExcludeGPR0();
      deps->getPreConditions()->getRegisterDependency(0)->setExcludeGPR0();
      addDependency(deps, fillReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(deps, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
      deps->getPostConditions()->getRegisterDependency(2)->setExcludeGPR0();
      deps->getPreConditions()->getRegisterDependency(2)->setExcludeGPR0();
      addDependency(deps, tempReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(deps, condReg, TR::RealRegister::cr0, TR_CCR, cg);
      addDependency(deps, fp1Reg, TR::RealRegister::NoReg, TR_FPR, cg);

      generateTrg1Src1ImmInstruction(cg, lenNode->getOpCode().isInt() ? TR::InstOpCode::cmpli4 : TR::InstOpCode::cmpli8, node, condReg, lenReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, donelabel, condReg);

      if (1 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, addrReg, 1); // 2 aligned??
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label0, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 1, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 1);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lenReg, lenReg, -1);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label0); // L0
         }

      if (1 == fillNodeSize || 2 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, lenNode->getOpCode().isInt() ? TR::InstOpCode::cmpli4 : TR::InstOpCode::cmpli8, node, condReg, lenReg, 2);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, label1, condReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, addrReg, 2); // 4 aligned??
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label1, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 2, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 2);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lenReg, lenReg, -2);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label1); // L1
         }

      if (1 == fillNodeSize || 2 == fillNodeSize || 4 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, lenNode->getOpCode().isInt() ? TR::InstOpCode::cmpli4 : TR::InstOpCode::cmpli8, node, condReg, lenReg, 4);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, label6, condReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, addrReg, 4); // 8 aligned??
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label6, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 4);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lenReg, lenReg, -4);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label6); // L6
         }

      TR::InstOpCode::Mnemonic dblStoreOp = TR::InstOpCode::stfd;
      TR::Register *dblFillReg= fp1Reg;
      if (TR::Compiler->target.is64Bit() && fillNode->getDataType() != TR::Double)
         {
         dblStoreOp = TR::InstOpCode::std;
         dblFillReg = fillReg;
         }

      //double word store, unroll 8 times
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi_r, node, tempReg, lenReg, 6);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label2, condReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tempReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label3); // L3
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 24, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 32, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 40, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 48, 8, cg), dblFillReg);
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 56, 8, cg), dblFillReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 64);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, label3, condReg);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 0, 63); // Mask
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label2); // L2

      //residual double words
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi_r, node, tempReg, lenReg, 3);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label5, condReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tempReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label7); // L7
      generateMemSrc1Instruction(cg, dblStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 8, cg), dblFillReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 8);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, label7, condReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label5); //L5

      //residual words
      if (1 == fillNodeSize || 2 == fillNodeSize || 4 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, lenReg, 4);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label4, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 4);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label4); // L4
         }

      //residual half words
      if (1 == fillNodeSize || 2 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, lenReg, 2);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label9, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 2, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 2);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label9); // L9
         }

      //residual bytes
      if (1 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, lenReg, 1);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, donelabel, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 1, cg), fillReg);
         }

      generateDepLabelInstruction(cg, TR::InstOpCode::label, node, donelabel, deps); // LdoneLabel
      cg->stopUsingRegister(dblFillReg);
      }
   else // regular word store path
      {
      TR::LabelSymbol *label1 = generateLabelSymbol(cg);
      TR::LabelSymbol *label2 = generateLabelSymbol(cg);
      TR::LabelSymbol *label3 = generateLabelSymbol(cg);
      TR::LabelSymbol *label4 = generateLabelSymbol(cg);
      TR::LabelSymbol *label5 = generateLabelSymbol(cg);
      TR::LabelSymbol *label6 = generateLabelSymbol(cg);
      TR::LabelSymbol *label9 = generateLabelSymbol(cg);
      TR::LabelSymbol *label10 = generateLabelSymbol(cg);
      TR::LabelSymbol *donelabel = generateLabelSymbol(cg);

      TR::RegisterDependencyConditions *deps = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(5, 5, cg->trMemory());
      addDependency(deps, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
      deps->getPostConditions()->getRegisterDependency(0)->setExcludeGPR0();
      deps->getPreConditions()->getRegisterDependency(0)->setExcludeGPR0();
      addDependency(deps, fillReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(deps, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
      deps->getPostConditions()->getRegisterDependency(2)->setExcludeGPR0();
      deps->getPreConditions()->getRegisterDependency(2)->setExcludeGPR0();
      addDependency(deps, tempReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(deps, condReg, TR::RealRegister::cr0, TR_CCR, cg);

      generateTrg1Src1ImmInstruction(cg, lenNode->getOpCode().isInt() ? TR::InstOpCode::cmpli4 : TR::InstOpCode::cmpli8, node, condReg, lenReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, donelabel, condReg);

      if (1 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, addrReg, 1); // 2 aligned??
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label6, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 1, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 1);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lenReg, lenReg, -1);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label6); // L6
         }

      if (1 == fillNodeSize || 2 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, lenNode->getOpCode().isInt() ? TR::InstOpCode::cmpli4 : TR::InstOpCode::cmpli8, node, condReg, lenReg, 2);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, label5, condReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, addrReg, 2); // 4 aligned??
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label5, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 2, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 2);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lenReg, lenReg, -2);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label5); // L5
         }

      TR::InstOpCode::Mnemonic wdStoreOp = TR::InstOpCode::stw;
      if (fillReg->getKind() == TR_FPR)
         {
    	 wdStoreOp = TR::InstOpCode::stfs;
         }

      //word store, unroll 8 times
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi_r, node, tempReg, lenReg, 5);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label2, condReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tempReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label3); // L3
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 12, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 20, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 24, 4, cg), fillReg);
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 28, 4, cg), fillReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 32);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, label3, condReg);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 0, 31); // Mask
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label2); // L2

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi_r, node, tempReg, lenReg, 2);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label4, condReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tempReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label10); // L10
      generateMemSrc1Instruction(cg, wdStoreOp, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), fillReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 4);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, label10, condReg);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 0, 3); // Mask
      generateLabelInstruction(cg, TR::InstOpCode::label, node, label4); // L4

      if (1 == fillNodeSize || 2 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, lenReg, 2);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, label9, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 2, cg), fillReg);
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 2);
         generateLabelInstruction(cg, TR::InstOpCode::label, node, label9); // L9
         }

      if (1 == fillNodeSize)
         {
         generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tempReg, lenReg, 1);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, donelabel, condReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 1, cg), fillReg);
         }

      generateDepLabelInstruction(cg, TR::InstOpCode::label, node, donelabel, deps); // LdoneLabel
      }

   cg->stopUsingRegister(fillReg);
   cg->stopUsingRegister(lenReg);
   cg->stopUsingRegister(tempReg);
   cg->stopUsingRegister(condReg);
   cg->stopUsingRegister(addrReg);
   if (dofastPath) cg->stopUsingRegister(fp1Reg);

   cg->decReferenceCount(addrNode);
   cg->decReferenceCount(fillNode);
   cg->decReferenceCount(lenNode);

   return NULL;
   }

// arraysetConstElemAlign8ConstLen16N --
//
// This is called if the following conditions are satisfied
//   - Address is known to be 8 byte-aligned
//   - Length is
//     - known at compile time
//     - in the range of [32..256]
//     - a multiple of 16
//   - Element data is a constant
//   - [[stfdp or stvx is available (p6, gpul, cellpx)]] <-- This restriction has been removed
//
// Algorithm:
//
//   1. Prepare 16-byte aligned 16-byte storage and fill it by duplicating the element value
//   2. Generate code to load the constant into (a) floating register (stfdp case), or
//      vetor register and floating register (VMX case)
//   3. Generate address calculation code
//   4. Generate 16-byte store instructions
//   5. Generate two 8-byte store instructions
//
// Generated code sequence:
//
// #if VMX
//    li addr[0],16*1
//    li addr[1],16*2
//    li addr[2],16*3
//    li addr[3],16*4
//    lfd fpA,CONST16_BUF
//    lvx vrA,CONST16_BUF
// #else if STFDP
//    lfdp fpA,CONST16_BUF
// #else /* STFD */
//    lfd fpA,CONST16_BUF  /* fpA and fpB must be an even/odd pair */
//    lfd fpB,CONST16_BUF
// #endif
//
//    rlwinm 4,3,28,0,0 # Move 8-byte alignment bit int bit0 (rldic)
//    rlwinm 3,3,0,0,27 # Round down to 16-byte alignment (rldic)
//    srawi 4,4,31	# -1 if 8-byte aligned, 0 if 16-byte (sradi)
//    andi. 4,4,160
//    add addr2,add2,addr1
//
// #if VMX
//    stvx vrA,(addr1,addr[0])
//    stvx vrA,(addr1,addr[1]); addi addr[0],add[0],16*4
//    stvx vrA,(addr1,addr[2]); addi addr[1],add[1],16*4
//    stvx vrA,(addr1,addr[3]); addi addr[2],add[2],16*4
//    stvx vrA,(addr1,addr[0]); addi addr[3],add[3],16*4
//    stvx vrA,(addr1,addr[1]); addi addr[0],add[9],16*4
//    stvx vrA,(addr1,addr[2])
//    stvx vrA,(addr1,addr[3])
//    stvx vrA,(addr1,addr[0])
// #else if STFDP
//    stfdp 0,16(addr1)
//    stfdp 0,32(addr1)
//    stfdp 0,48(addr1)
//    stfdp 0,64(addr1)
//    stfdp 0,80(addr1)
//    stfdp 0,96(addr1)
//    stfdp 0,112(addr1)
//    stfdp 0,128(addr1)
//    stfdp 0,144(addr1)
// #else /* STFD */
//    stfd fpA,16(addr1)
//    stfd fpB,16(addr1)
//    stfd fpA,32(addr1)
//    stfd fpB,40(addr1)
//    stfd fpA,48(addr1)
//    stfd fpB,56(addr1)
//    stfd fpA,64(addr1)
//    stfd fpB,72(addr1)
//    stfd fpA,80(addr1)
//    stfd fpB,88(addr1)
//    stfd fpA,96(addr1)
//    stfd fpB,104(addr1)
//    stfd fpA,112(addr1)
//    stfd fpB,120(addr1)
//    stfd fpA,128(addr1)
//    stfd fpB,136(addr1)
//    stfd fpA,144(addr1)
//    stfd fpB,152(addr1)
// #endif
//    stfd fpA,8(addr1)
//    stfd fpA,0(addr2)  # == 0(addr1) or 160(addr1)

#if defined(TR_IDIOM_PPCVMX)

#define ARRAYSET_INSN_VMX	1
#define ARRAYSET_INSN_STFDP	2
#define ARRAYSET_INSN_STFD	3

static void arraysetConstElemAlign8ConstLen16N(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, int byteLen, TR::CodeGenerator *cg)
   {
   TRACE_VMX1("@arraysetConstElemAlign8ConstLen16N: byteLen=%d\n", byteLen);
   TR_ASSERT(TR::Compiler->target.cpu.id() >= TR_PPCp6 || VMXEnabled(cg), "Neither stfdp nor VMX is available");
   TR_ASSERT((byteLen & 15) == 0 && byteLen >= ARRAYSET_CONSTLEN16N_MIN && byteLen <= ARRAYSET_CONSTLEN16N_MAX, "Invalid byteLen");
   int numBlocks = byteLen / 16;

   // On p6, stfdp is used by default, however, we can override it by setting TR_PPC_VMX.
   int insnType;
   if (VMXEnabled(cg))
      insnType = ARRAYSET_INSN_VMX;
   else if (TR::Compiler->target.cpu.id() >= TR_PPCp6)
      insnType = ARRAYSET_INSN_STFDP;
   else
      insnType = ARRAYSET_INSN_STFD;
   TRACE_VMX1("@arraysetConstElemAlign8ConstLen16N: Using %s instruction\n",
              insnType == ARRAYSET_INSN_VMX ? "VMX" :
              insnType == ARRAYSET_INSN_STFDP ? "stfdp" :
              "stfd");

   // Allocate 16-byte aligned 16-byte area in the read-only memory area
   char *const16Addr = (char *)TR_Memory::jitPersistentAlloc(16 + 15);
   const16Addr = (char *)(((uintptr_t)const16Addr + 15) & ~15);

   // Fill the buffer with data
   fillConst16(const16Addr, elemNode, cg);
   // Generate code to load constant value
   TR::Register *index0Reg;
#define NUM_INDEX_REGS	4
   TR::Register *indexReg[NUM_INDEX_REGS];
   TR::Register *fp1Reg;
   TR::Register *fp2Reg;
   TR::Register *vrReg;
   TR::Register *constBaseReg = cg->allocateRegister(TR_GPR);
   loadAddressConstant(cg, node, (intptrj_t)const16Addr, constBaseReg);
   switch (insnType)
      {
      case ARRAYSET_INSN_VMX:
         fp1Reg = cg->allocateRegister(TR_FPR);
         vrReg = cg->allocateRegister(TR_VRF);
         index0Reg = cg->allocateRegister(TR_GPR);
         loadConstant(cg, node, 0, index0Reg);
         for (int i = 0; i < NUM_INDEX_REGS; i++)
            {
            indexReg[i] = cg->allocateRegister(TR_GPR);
            loadConstant(cg, node, (i + 1) * 16, indexReg[i]);
      }
         generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vrReg, new (cg->trHeapMemory()) TR::MemoryReference(constBaseReg, index0Reg, 16, cg));
         generateTrg1MemInstruction(cg, TR::InstOpCode::lfd, node, fp1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constBaseReg, 0, 8, cg));
         break;
      case ARRAYSET_INSN_STFDP:
         // FIXME: Set appropriate constraint like TR_S390CodeGenerator::allocateConsecutiveRegisterPair()
         fp1Reg = cg->allocateRegister(TR_FPR);
         fp2Reg = cg->allocateRegister(TR_FPR);
         generateTrg1MemInstruction(cg, TR::InstOpCode::lfdp, node, fp1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constBaseReg, 0, 16, cg));
         break;
      case ARRAYSET_INSN_STFD:
         fp1Reg = cg->allocateRegister(TR_FPR);
         generateTrg1MemInstruction(cg, TR::InstOpCode::lfd, node, fp1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constBaseReg, 0, 8, cg));
         break;
   }

   // Generate code to setup address registers
   //    rlwinm 4,3,28,0,0	# Move 8-byte alignment bit into bit0 (rldic)
   //    rlwinm 3,3,0,0,27	# Round down to 16-byte alignment (rldic)
   //    srawi 4,4,31		# -1 if 8-byte aligned, 0 if 16-byte (sradi)
   //    andi. 4,4,160		# 160 if 8-byte aligned, 0 if 16-byte
   TR::Register *addr1Reg = cg->gprClobberEvaluate(addrNode);
   TR::Register *addr2Reg = cg->allocateRegister(TR_GPR);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, addr2Reg, addr1Reg,
                                   28, 0x80000000);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, addr1Reg, addr1Reg,
                                   0, 0xfffffff0);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, addr2Reg, addr2Reg, 31);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, addr2Reg, addr2Reg, byteLen);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, addr2Reg, addr2Reg, addr1Reg);

   // Store 16-bytes X (numBlocks - 1)
   switch (insnType)
      {
      case ARRAYSET_INSN_VMX:
         for (int i = 0; i < numBlocks - 1; i++)
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::stvx, node, new (cg->trHeapMemory()) TR::MemoryReference(addr1Reg, indexReg[i%NUM_INDEX_REGS], 16, cg), vrReg);
            if (i >= 1 && i <= numBlocks - 5)
               generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg[(i - 1) % NUM_INDEX_REGS], indexReg[(i - 1) % NUM_INDEX_REGS], 16*NUM_INDEX_REGS);
            }
         break;
      case ARRAYSET_INSN_STFDP:
         // Note on use of stfdp
         //   - stfdp is always the first in the dispatch group
         //   - stfdp does not fold, meaning that it performs once in a two cycle
         for (int i = 1; i < numBlocks; i++)
            generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addr1Reg, 16*i, 16, cg), fp1Reg);
         break;
      case ARRAYSET_INSN_STFD:
         for (int i = 1; i < numBlocks; i++)
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addr1Reg, 16*i, 8, cg), fp1Reg);
            generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addr1Reg, 16*i+8, 8, cg), fp1Reg);
            }
         break;
      }

   // Store a pair of double words which are either separated or paired
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addr1Reg, 8, 8, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addr2Reg, 0, 8, cg), fp1Reg);

   switch (insnType)
      {
      case ARRAYSET_INSN_VMX:
         cg->stopUsingRegister(fp1Reg);
         cg->stopUsingRegister(vrReg);
         cg->stopUsingRegister(index0Reg);
         for (int i = 0; i < NUM_INDEX_REGS; i++)
            cg->stopUsingRegister(indexReg[i]);
         break;
      case ARRAYSET_INSN_STFDP:
         {
         TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(2, 2, cg->trMemory());
         // Make sure two consecutive registers are allocated.
         addDependency(conditions, fp1Reg, TR::RealRegister::fp8, TR_FPR, cg);
         addDependency(conditions, fp2Reg, TR::RealRegister::fp9, TR_FPR, cg);
         generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
         conditions->stopUsingDepRegs(cg);
         }
         break;
      case ARRAYSET_INSN_STFD:
         cg->stopUsingRegister(fp1Reg);
         break;
      }
   cg->stopUsingRegister(addr1Reg);
   cg->stopUsingRegister(addr2Reg);
   cg->stopUsingRegister(constBaseReg);
   TRACE_VMX0("@arraysetConstElemAlign8ConstLen16N: Exitting\n");
   }

// arraysetConstLen --
//
// This evaluator is called when the loop count is constant and is less than or equal to ARRAYSET_LIMIT1.
// In this case, we generate straight line code.
//
// Conditions:
//   - 0 <= byteLen < ARRAYSET_LIMIT1
//   - addrReg is aligned at elemSize boundary
//   - elemReg can be any type, and constant or variable
//
// Other conditions:
//   - addrReg is aligned at elemSize boundary
//   - elemReg can be any type
//
// Strategy:
//   - Generate a series of store instructions corresponding to element type
//   - If element size is 1 or 2, and total bytes are greater than or equal to 4,
//     and the element is constant, stores are aggregated into 4-byte stores.
//
static void arraysetConstLen(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, int byteLen, TR::CodeGenerator *cg)
   {
   // See if we can combine address offset into array header offset
   TR::Register *addrReg = cg->gprClobberEvaluate(addrNode);
   TR::Register *elemReg;
   int elemSize = getElementSize(elemNode, cg);
   int elemIsConst = elemNode->getOpCode().isLoadConst();
   TR::DataType data_type = elemNode->getDataType();
   TR::DataType type = elemNode->getType();
   if (data_type == TR::Address)
      data_type = TR::Compiler->target.is64Bit() ? TR_SInt64 : TR_SInt32;
   bool two_reg = (TR::Compiler->target.is32Bit() && type.isInt64());
   int i;

   TRACE_VMX2("@arraysetConstLen: elemSize=%d elem is %s\n",
          elemSize, elemIsConst ? "constant" : "variable");
   if (cg->getDebug())
      {
      if (addrNode->getOpCodeValue() == TR::aiadd)
         TRACE_VMX3("@  node=[%s] child0=[%s] child1=[%s]\n",
                    cg->getDebug()->getName(addrNode->getOpCode()),
                    cg->getDebug()->getName(addrNode->getFirstChild()->getOpCode()),
                    cg->getDebug()->getName(addrNode->getSecondChild()->getOpCode()));
      else
         TRACE_VMX1("@  node=[%s]\n", cg->getDebug()->getName(addrNode->getOpCode()));
      }

   // If we have more than equal to 4 stores of 1-byte or 2-byte elements, we combine them into word-stores.
   if ((data_type == TR_Bool ||
        data_type == TR_SInt8 ||
        data_type == TR_UInt8 ||
        data_type == TR_SInt16 ||
        data_type == TR_UInt16) &&
       /*elemIsConst != 0 &&*/
       byteLen >= elemSize * ARRAYSET_STORE_GATHER_MIN_ELEMS)
      {

      if (elemIsConst != 0)
      {
         elemReg = cg->allocateRegister(TR_GPR);
         uint32_t val = (uint32_t)getIntegralValue(elemNode);
         switch (data_type)
            {
            case TR_Bool:
            case TR_SInt8:
            case TR_UInt8:
               val &= 0xff;
               val = (val << 24) | (val << 16) | (val << 8) | val;
               break;
            case TR_SInt16:
            case TR_UInt16:
               val &= 0xffff;
               val = (val << 16) | val;
               break;
            }
         loadConstant(cg, node, (int32_t)val, elemReg);
         }
      else
         {
         elemReg = cg->gprClobberEvaluate(elemNode);
         switch (data_type)
            {
            case TR_Bool:
            case TR_SInt8:
            case TR_UInt8:
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, elemReg, elemReg, 8, 0x0000FF00);
               // fallthru
            case TR_SInt16:
            case TR_UInt16:
               generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, elemReg, elemReg, 16, 0xFFFF0000);
               break;
            }
      }

      for (i = 0; i <= byteLen - 4; i += 4)
         {
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i, 4, cg), elemReg);
         }
      // Tailers 0..3 bytes
      if (byteLen - i >= 2)
         {
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i, 2, cg), elemReg);
         i += 2;
         }
      if (byteLen - i >= 1)
         {
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i, 1, cg), elemReg);
         i += 1;
         }
      }
   else // Process elements one-by-one
      {
      elemReg = cg->evaluate(elemNode); // we don't clobber elemReg
      for (i = 0; i < byteLen; i += elemSize)
         {
         TR::MemoryReference *memRef;
         TR::MemoryReference *memRefLow;

         if (two_reg)
            {
            memRef = new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i, 4, cg);
            memRefLow = new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i + 4, 4, cg);
            }
         else
            memRef = new (cg->trHeapMemory()) TR::MemoryReference(addrReg, i, elemSize, cg);
         switch (data_type)
            {
            case TR_Bool:
            case TR_SInt8:
            case TR_UInt8:
               generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node, memRef, elemReg);
               break;
            case TR_SInt16:
            case TR_UInt16:
               generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, memRef, elemReg);
               break;
            case TR_SInt32:
            case TR_UInt32:
               generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, memRef, elemReg);
               break;
            case TR_SInt64:
            case TR_UInt64:
               if (two_reg)
                  {
                  generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, memRef, elemReg->getHighOrder());
                  generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, memRefLow, elemReg->getLowOrder());
                  }
               else
                  {
                  TR_ASSERT(0, "PPC64 not supported yet.");
                  }
               break;
            case TR::Float:
               generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node, memRef, elemReg);
               break;
            case TR::Double:
               generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, memRef, elemReg);
               break;
            }
         }
      }

   // Register dependencies
   TR::RegisterDependencyConditions *conditions;
   if (two_reg)
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(3, 3, cg->trMemory());
   else
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(2, 2, cg->trMemory());
   // arguments
   addDependency(conditions, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   if (two_reg)
      {
      addDependency(conditions, elemReg->getHighOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, elemReg->getLowOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      }
   else
      addDependency(conditions, elemReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg);
   }

// arraysetConstElemAlign8 --
//
// Arguments:
//
//	addr	The address of the first element (aligned to 8-byte boundary)
//	elem	Element value to be stored (constant)
//	len	Length in bytes (32bit or 64bit. variable)
//
// This evaluator is called in the following cases:
//   - Element value is constant at compile time
//   - Loop count (supplied in bytes) is not constant
//   - Running on p6
//
// Generated code sequence:
//
//    if len != 160, skip len160
//    compute addr1 and addr2
//    stfdp 0,16(addr1)
//    stfdp 0,32(addr1)
//    stfdp 0,48(addr1)
//    stfdp 0,64(addr1)
//    stfdp 0,80(addr1)
//    stfdp 0,96(addr1)
//    stfdp 0,112(addr1)
//    stfdp 0,128(addr1)
//    stfdp 0,144(addr1)
//    stfd fpA,8(addr1)
//    stfd fpA,0(addr2)  # == 0(addr1) or 160(addr1)
//    goto out
//
//    skipLen160:
//    CTR <- len / 32
//    addi addr,-8
//    if CTR == 0, skip blockLoop
//    load fpr,CONST
//    loop:
//      stfd fpr,8(addr)
//      stfd fpr,16(addr)
//      stfd fpr,24(addr)
//      stfdu fpr,32(addr)
//      bdnz loop
//    skipBlockLoop:
//    addi addr,8
//    rest = len & ~31
//    if rest == 0, skip postLoop
//    evaluate elem
//    CTR <- rest / elemSize
//    postLoop:
//      store elem
//      addi addr,eleEize
//      bdnz posLoop
//
//    skipPostLoop:
//    out:

static void arraysetConstElemAlign8(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraysetConstElemAlign8:\n");

   if (cg->getDebug())
      TRACE_VMX2("    addrNode=[%s] firstChild=[%s]\n",
                 cg->getDebug()->getName(addrNode->getOpCode()),
                 addrNode->getNumChildren() >= 1 ? cg->getDebug()->getName(addrNode->getFirstChild()->getOpCode()) : "none");
   TR::Register *addrReg = cg->gprClobberEvaluate(addrNode); // aligned at 8-byte boundary
   TR::Register *addr2Reg = cg->allocateRegister(TR_GPR);
   TR::Register *elemReg = cg->evaluate(elemNode);
   TR::Register *lenReg = cg->gprClobberEvaluate(lenNode);

   TR::LabelSymbol *lblSkipLen160 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblBlockLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipBlockLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblPostLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPostLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblDone = generateLabelSymbol(cg);

   TR::Register *cndReg = cg->allocateRegister(TR_CCR);
   TR::Register *tmpReg = cg->allocateRegister(TR_GPR);
   TR::Register *fp1Reg = cg->allocateRegister(TR_FPR);
   TR::Register *fp2Reg = cg->allocateRegister(TR_FPR);
   TR::Register *constBaseReg = cg->allocateRegister(TR_GPR);

   TR::DataType data_type = elemNode->getDataType();
   TR::DataType type = elemNode->getType();
   if (data_type == TR::Address)
      data_type = TR::Compiler->target.is64Bit() ? TR_SInt64 : TR_SInt32;
   int elemSize = getElementSize(elemNode, cg);
   bool two_reg = (TR::Compiler->target.is32Bit() && type.isInt64());
   int storeElementSize;
   int blockLoopBodySize = 32; // stfp X 4

   // Allocate 16-byte aligned 16-byte area in the read-only memory area (8-byte boundary is required)
   // and fill he buffer with data
   char *const16Addr = (char *)TR_Memory::jitPersistentAlloc(16 + 15);
   const16Addr = (char *)(((uintptr_t)const16Addr + 15) & ~15);
   fillConst16(const16Addr, elemNode, cg);

   // Generate code to load constant value
   TR_ASSERT(TR::Compiler->target.cpu.id() >= TR_PPCp6, "arraysetConstElemAlign8: Must be p6 or newer");
   loadAddressConstant(cg, node, (intptrj_t)const16Addr, constBaseReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lfdp, node, fp1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constBaseReg, 0, 16, cg));

   // Check if the loop count is exactly 160-byte or not
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi8, node, cndReg, lenReg, 160);
   else
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 160);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, PPCOpProp_BranchUnlikely, node, lblSkipLen160, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblSkipLen160, cndReg);

   // End constElemAlign8len160 code

   // Generate code to setup address registers
   //    rlwinm 4,3,28,0,0	# Move 8-byte alignment bit into bit0 (rldic)
   //    rlwinm 3,3,0,0,27	# Round down to 16-byte alignment (rldic)
   //    srawi 4,4,31		# -1 if 8-byte aligned, 0 if 16-byte aligned (sradi)
   //    andi. 4,4,160		# 160 if 8-byte aligned, 0 if 16-byte
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, addr2Reg, addrReg,
                                   28, 0x80000000);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, addrReg, addrReg,
                                   0, 0xfffffff0);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, addr2Reg, addr2Reg, 31);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, addr2Reg, addr2Reg, 160);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, addr2Reg, addr2Reg, addrReg);

   // Store 16-bytes X 9
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*1, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*2, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*3, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*4, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*5, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*6, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*7, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*8, 16, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdp, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16*9, 16, cg), fp1Reg);

   // Store a pair of double words which are either separated or paired
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8, 8, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addr2Reg, 0, 8, cg), fp1Reg);

   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblDone);
   // End constElemAlign8len160 code

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkipLen160);
   // Compute block loop count
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicl_r, node, tmpReg, lenReg,
                                      64 - log2(blockLoopBodySize), (uint64_t)CONSTANT64(0xffffffffffffffff) >> log2(blockLoopBodySize));
   else
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, tmpReg, lenReg,
                                      32 - log2(blockLoopBodySize), 0xffffffffu >> log2(blockLoopBodySize));
   // Rewind the address pointer by 8-bytes
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, -8);
   // Load block loop count into CTR
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);
   // Skip the loop if the loop count is zero
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblSkipBlockLoop, cndReg); // Don't we need to bind cndReg to CR0 ?

   // Loop:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblBlockLoop);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8, 8, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 16, 8, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 24, 8, cg), fp1Reg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stfdu, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 32, 8, cg), fp1Reg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblBlockLoop, cndReg/*dummy*/);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkipBlockLoop);

   // Adjust the pointer
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 8);

   // Compute the post loop count
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicl_r, node, tmpReg, lenReg,
                                      64 - log2(elemSize), ((uint64_t)1 << (log2(blockLoopBodySize) - log2(elemSize))) - 1);
   else
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, tmpReg, lenReg,
                                      32 - log2(elemSize), ((uint32_t)1 << (log2(blockLoopBodySize) - log2(elemSize))) - 1);
   // Load the loop count into CTR
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);
   // Skip the loop if the loop count is zero
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblSkipPostLoop, cndReg); // Don't we need to bind cndReg to CR0 ?

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblPostLoop);
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt16:
      case TR_UInt16:
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt32:
      case TR_UInt32:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt64:
      case TR_UInt64:
         if (two_reg)
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4, 4, cg), elemReg->getLowOrder());
            }
         else
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
            }
         break;
      case TR::Float:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR::Double:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      default:
         TR_ASSERT(0, "arraysetConstElemAlign8: Unexpected data type 0x%x", elemNode->getDataType());
         break;
      }
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, elemSize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblPostLoop, cndReg/*dummy*/);

   // SkipPreLoop:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkipPostLoop);

   // Out
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblDone);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions;
   if (two_reg)
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(5, 5, cg->trMemory());
   else
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(4, 4, cg->trMemory());
   // arguments
   addDependency(conditions, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   if (two_reg)
      {
      addDependency(conditions, elemReg->getHighOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, elemReg->getLowOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      }
   else
      addDependency(conditions, elemReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   addDependency(conditions, fp1Reg, TR::RealRegister::NoReg, TR_FPR, cg);

   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg);
   if (two_reg)
      cg->stopUsingRegister(elemReg); // Don't know why this is needed
   cg->stopUsingRegister(addr2Reg);
   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(cndReg);
   cg->stopUsingRegister(constBaseReg);
   }

// arraysetGeneric --
//
// Arguments:
//
//	addr	The address of the first element
//	elem	Element value to be stored
//	len	Length in bytes (32bit or 64bit)
//
//
// This evaluator is called in the following cases:
//   - Loop count (supplied in bytes) is not constant
//   - Loop count is variable and we not predict the loop is short or long
//
// Other conditions:
//   - addrReg is aligned at least elemSize (1, 2, 4, or 8) boundary
//   - elemReg can be any type
//

static void arraysetGeneric(TR::Node *node, TR::Node *addrNode, TR::Node *elemNode, TR::Node *lenNode, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraysetGeneric:\n");

   if (cg->getDebug())
      TRACE_VMX2("    addrNode=[%s] firstChild=[%s]\n",
                 cg->getDebug()->getName(addrNode->getOpCode()),
                 addrNode->getNumChildren() >= 1 ? cg->getDebug()->getName(addrNode->getFirstChild()->getOpCode()) : "none");
   TR::Register *addrReg = cg->gprClobberEvaluate(addrNode);
   TR::Register *elemReg;
   TR::Register *lenReg = cg->gprClobberEvaluate(lenNode);

   TR::LabelSymbol *lblPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut = generateLabelSymbol(cg);

   TR::Register *cndReg = cg->allocateRegister(TR_CCR);
   TR::Register *tmpReg=cg->allocateRegister();

   TR::DataType data_type = elemNode->getDataType();
   TR::DataType type = elemNode->getType();
   if (data_type == TR::Address)
      data_type = TR::Compiler->target.is64Bit() ? TR_SInt64 : TR_SInt32;
   int elemSize = getElementSize(elemNode, cg);
   bool two_reg = (TR::Compiler->target.is32Bit() && type.isInt64());
   int storeElementSize;
   int loopBodySize;

   // We will destroy elemReg only if it is an integral type with size less than int.
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
      case TR_SInt16:
      case TR_UInt16:
         elemReg = cg->gprClobberEvaluate(elemNode);
         break;
      default:
         elemReg = cg->evaluate(elemNode);
      }

   // Determine loop block size (in bytes)
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
      case TR_SInt16:
      case TR_UInt16:
      case TR_SInt32:
      case TR_UInt32:
      case TR::Float:
         storeElementSize = 4; // FIXME: On 64bit, use 64bit store for all types
         break;
      case TR_SInt64:
      case TR_UInt64:
      case TR::Double:
         storeElementSize = 8;
         break;
      default:
         TR_ASSERT(0, "Unexpected datatype %d encountered on arrayset", data_type);
         break;
      }

   loopBodySize = storeElementSize * 4;

   // Exit if there is no data
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi8, node, cndReg, lenReg, 0);
   else
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblOut, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblOut, cndReg);

   // Compute pre-loop count, and skip pre-loop if the loop count is zero
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicl_r, node, tmpReg, lenReg,
                                      64 - log2(elemSize), ((uint64_t)1 << (log2(loopBodySize) - log2(elemSize))) - 1);
   else
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, tmpReg, lenReg,
                                      32 - log2(elemSize), ((uint32_t)1 << (log2(loopBodySize) - log2(elemSize))) - 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblSkipPreLoop, cndReg); // Don't we need to bind cndReg to CR0 ?

   // Load pre-loop count into CTR
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblPreLoop);
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stb, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt16:
      case TR_UInt16:
         generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt32:
      case TR_UInt32:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR_SInt64:
      case TR_UInt64:
         if (two_reg)
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, 4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4, 4, cg), elemReg->getLowOrder());
            }
         else
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
            }
         break;
      case TR::Float:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node,  new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      case TR::Double:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 0, elemSize, cg), elemReg);
         break;
      default:
         TR_ASSERT(0, "arraysetGeneric: Unexpected data type 0x%x", elemNode->getDataType());
         break;
      }
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, elemSize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblPreLoop, cndReg/*dummy*/);

   // SkipPreLoop:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkipPreLoop);

   // Compute unrolled loop count, and skip the loop if loop count is zero
   if (TR::Compiler->target.is64Bit())
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rldicl_r, node, tmpReg, lenReg,
                                      64 - log2(loopBodySize), (uint64_t)CONSTANT64(0xffffffffffffffff) >> log2(loopBodySize));
   else
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, tmpReg, lenReg,
                                      32 - log2(loopBodySize), 0xffffffffu >> log2(loopBodySize));
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);

   // Load the loop count into CTR
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblOut, cndReg);

   // XXX: The code from top to here must be 64bit safe, but not sure for the rest.

   // Duplicate element data to fill up whole word
   switch (elemNode->getDataType())
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, elemReg, elemReg, 8, 0x0000FF00);
         // fallthru
      case TR_SInt16:
      case TR_UInt16:
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwimi, node, elemReg, elemReg, 16, 0xFFFF0000);
         break;
      default:
         break;
      }

   // Loop:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblLoop);
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
      case TR_SInt16:
      case TR_UInt16:
      case TR_SInt32:
      case TR_UInt32:
         // FIXME: Use 64bit store on 64bit VM
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*0, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*1, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*2, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*3, 4, cg), elemReg);
         break;
      case TR_SInt64:
      case TR_UInt64:
         if (two_reg)
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*0,   4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*0+4, 4, cg), elemReg->getLowOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*1,   4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*1+4, 4, cg), elemReg->getLowOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*2,   4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*2+4, 4, cg), elemReg->getLowOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*3,   4, cg), elemReg->getHighOrder());
            generateMemSrc1Instruction(cg, TR::InstOpCode::stw, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*3+4, 4, cg), elemReg->getLowOrder());
            }
         else
            {
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*0, 8, cg), elemReg);
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*1, 8, cg), elemReg);
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*2, 8, cg), elemReg);
            generateMemSrc1Instruction(cg, TR::InstOpCode::std, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*3, 8, cg), elemReg);
            }
         break;
      case TR::Float:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*0, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*1, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*2, 4, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfs, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4*3, 4, cg), elemReg);
         break;
      case TR::Double:
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*0, 8, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*1, 8, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*2, 8, cg), elemReg);
         generateMemSrc1Instruction(cg, TR::InstOpCode::stfd, node, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 8*3, 8, cg), elemReg);
         break;
      default:
         break;
      }
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, loopBodySize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblLoop, cndReg/*dummy*/);

   // Out:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblOut);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions;
   if (two_reg)
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(4, 4, cg->trMemory());
   else
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(3, 3, cg->trMemory());
   // arguments
   addDependency(conditions, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   if (two_reg)
      {
      addDependency(conditions, elemReg->getHighOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, elemReg->getLowOrder(), TR::RealRegister::NoReg, TR_GPR, cg);
      }
   else
      addDependency(conditions, elemReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg);
   if (two_reg)
      cg->stopUsingRegister(elemReg); // Don't know why this is needed
   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(cndReg);
   }

#endif

//----------------------------------------------------------------------
// arraytranslate evaluator
//----------------------------------------------------------------------
//
// arraytranslate evaluator --
//
// Tree looks as follows:
//   arraytranslate
//      input ptr
//      output ptr
//      translation table
//      stop character
//      input length (in elements)
//
// Number of translated elements minus one is returned.

#if defined(TR_IDIOM_PPCVMX)
static TR::Register *arraytranslateTROT(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTROTGeneric(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTROTSimple256Scalar(TR::Node *node, TR::CodeGenerator *cg, int max);
static TR::Register *arraytranslateTROTSimple256ScalarOpt(TR::Node *node, TR::CodeGenerator *cg, int max);
static TR::Register *arraytranslateTROTSimpleVMX(TR::Node *node, TR::CodeGenerator *cg, int max);
static TR::Register *arraytranslateTRTO(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTRTOGenericScalarNoBreak(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTRTOGenericScalarWithBreak(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTRTOGenericScalarWithBreakTermChar(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTRTOSimpleScalar(TR::Node *node, TR::CodeGenerator *cg, int max);
static TR::Register *arraytranslateTRTOSimpleVMX(TR::Node *node, TR::CodeGenerator *cg, int max);
static TR::Register *arraytranslateTROO(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateTRTT(TR::Node *node, TR::CodeGenerator *cg);
static int analyzeTRTOTable(TR::Node *node, TR::CodeGenerator *cg);
static int analyzeTROTTable(TR::Node *node, TR::CodeGenerator *cg);
#endif

#if defined(TR_IDIOM_PPCVMX)
// arraytranslateTROT --
//
static TR::Register *arraytranslateTROT(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraytranslateTROT:\n");
   TR::Register *retReg;
   int max = analyzeTROTTable(node, cg);

   if (VMXEnabled(cg))
      {
      if (max >= 0)
         retReg = arraytranslateTROTSimpleVMX(node, cg, max);
      else
         retReg = arraytranslateTROTGeneric(node, cg);			// Generic case
      }
   else
      {
      if (max == 0xff)
         retReg = arraytranslateTROTSimple256Scalar(node, cg, max);	// ISO8859_1
      else
         retReg = arraytranslateTROTGeneric(node, cg);			// Generic case
      }
   TRACE_VMX1("@arraytranslateTROT: returning %p\n", retReg);
   return retReg;
   }


static TR::Register *arraytranslateTROTGeneric(TR::Node *node, TR::CodeGenerator *cg)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTROTGeneric: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);
   TR::Register *inputReg  	= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg 	= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *tableReg  	= cg->gprClobberEvaluate(node->getChild(2));
   TR::Register *termReg   	= cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    	= cg->gprClobberEvaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tableReg, tableReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   TR::Register *indexReg	= cg->allocateRegister();
   TR::Register *tmpReg		= cg->allocateRegister();
   TR::Register *ch0Reg		= cg->allocateRegister();
   TR::Register *ch1Reg		= cg->allocateRegister();
   TR::Register *ch2Reg		= cg->allocateRegister();
   TR::Register *ch3Reg		= cg->allocateRegister();
   TR::Register *idxReg		= cg->allocateRegister();
   TR::Register *xchReg		= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut1 = 0;
   TR::LabelSymbol *lblOut2 = 0;
   TR::LabelSymbol *lblOut3 = 0;
   if (termCharNodeIsHint == 0)
      {
      lblOut1 = generateLabelSymbol(cg);
      lblOut2 = generateLabelSymbol(cg);
      lblOut3 = generateLabelSymbol(cg);
      }

   loadConstant(cg, node, 0x0, indexReg);
   CMPI4(cnd,len,0);
   BEQ(cnd,Out0);
   ANDI_R(tmp,len,3);
   BEQ(cnd,SkipPreLoop);
   ADDI(input,input,-1);
   ADDI(output,output,-2);
   MTCTR(tmp);

   // PreLoop begin
   LABEL(PreLoop);
   LBZU(ch0,1,input);
   RLWINM(ch0,ch0,1, 0xfffffffe);
   LHZX(xch,table,ch0);
   CMP4(cnd,xch,term);
   BEQ(cnd,Out0);
   STHU(xch,2,output);
   ADDI(index,index,1);
   BDNZ(cnd,PreLoop);
   // PreLoop end
   ADDI(input,input,1);
   ADDI(output,output,2);
   // SkipPreLoop
   LABEL(SkipPreLoop);

   RLWINM(tmp,len,32-2,0x3fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,Out0);

   LBZ(ch0,0,input); LBZ(ch1,1,input); LBZ(ch2,2,input); LBZ(ch3,3,input);
   // Loop begin
   LABEL(Loop);
   /*              */ RLWINM(idx,ch0,1,0xfffffffe); LHZX(xch,table,idx); if (termCharNodeIsHint == 0) { CMP4(cnd,xch,term); BEQ(cnd,Out0); }
   STH(xch,0,output); RLWINM(idx,ch1,1,0xfffffffe); LHZX(xch,table,idx); if (termCharNodeIsHint == 0) { CMP4(cnd,xch,term); BEQ(cnd,Out1); }
   STH(xch,2,output); RLWINM(idx,ch2,1,0xfffffffe); LHZX(xch,table,idx); if (termCharNodeIsHint == 0) { CMP4(cnd,xch,term); BEQ(cnd,Out2); }
   STH(xch,4,output); RLWINM(idx,ch3,1,0xfffffffe); LHZX(xch,table,idx); if (termCharNodeIsHint == 0) { CMP4(cnd,xch,term); BEQ(cnd,Out3); }
   STH(xch,6,output); ADDI(input,input, 4); ADDI(output,output,8); ADDI(index,index,4);
   LBZ(ch0,0,input); LBZ(ch1,1,input); LBZ(ch2,2,input); LBZ(ch3,3,input);
   BDNZ(cnd,Loop);
   // Loop end

   if (termCharNodeIsHint == 0)
      {
      B(cnd,Out0);
      LABEL(Out1); ADDI(index,index,1); B(cnd,Out0);
      LABEL(Out2); ADDI(index,index,2); B(cnd,Out0);
      LABEL(Out3); ADDI(index,index,3); B(cnd,Out0);
      }
   LABEL(Out0); ADDI(index,index,-1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(13, 13, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, termReg,   TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch2Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch3Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, idxReg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xchReg,   	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,    TR::RealRegister::cr0,   TR_CCR, cg);

   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   // Release registers
   cg->stopUsingRegister(tmpReg);

   return indexReg;
   }

// arraytranslateTROTSimple256Scalar --
//
// Input:
//   uint8 *input;
//   uint16 *output;
//   uint16 table[256];
//   int    delim;
//   int    len;
//
// Pseudo code:
//   for (i = 0; i < len; i++)
//      output[i] = input[i];
//   return len - 1;
//
// Actual code:
// trot:
//         cmpi    cnd1Reg,0,lenReg,0              len == 0 ?
//         andi.   bytesReg,lenReg,0x0003          bytes = len & 3
//         rlwinm  countReg,lenReg,30,2,31         count = len / 4
//         addi    inputReg,inputReg,-1            input--
//         addi    outputReg,outputReg,-2          output--
//         bc      BO_IF_NOT,CR1_FEX,exit2         if len=0 -> exit2
//         cmpi    1,0,countReg,0                  count == 0 ?
//         bc      BO_IF,CR0_EQ,$+0x24             if bytes = 0, skip byte loop
//         mtspr   CTR,bytesReg                    initialize byte count
//
// byteLoop:
//         lbzu    chReg,1(inputReg)               load byte
//         sthu    chReg,2(outputReg)              store half-word
//         bc      BO_dCTR_NZERO,CR0_LT,byteLoop   repeat byte loop
//
//         bc      BO_IF,CR1_VX,exit2              if count=0, exit2
// skipByteLoop:
//         mtspr   CTR,countReg                    initilize unrolled loop count
//         lbz     chReg,1(inputReg)               read the first byte in the iteration
//         addi    input2Reg,inputReg,4            compute input address for the second iteration
//         sth     chReg,2(outputReg)              store the first halfword
//         lbz     chReg,2(inputReg)
//         sth     chReg,4(outputReg)
//         lbz     chReg,3(inputReg)
//         sth     chReg,6(outputReg)
//         lbz     chReg,4(inputReg)
//         bc      BO_dCTR_ZERO,CR0_LT,exit1      if this is the last iteration, exit1
//
// unrolledLoop:
//         sth     chReg,8(outputReg)
//         addi    outputReg,outputReg,8
//         lbz     chReg,1(input2Reg)
//         sth     chReg,2(outputReg)
//         lbz     chReg,2(input2Reg)
//         sth     chReg,4(outputReg)
//         lbz     chReg,3(input2Reg)
//         addi    input2Reg,input2Reg,4
//         sth     chReg,6(outputReg)
//         lbz     chReg,0(input2Reg)
//         bc      BO_dCTR_NZERO,CR0_LT,unrolledLoop
// exit1:
//         sth     chReg,8(outputReg)
// exit2:
//         addi    retReg,lenReg,-1

static TR::Register *arraytranslateTROTSimple256Scalar(TR::Node *node, TR::CodeGenerator *cg, int max)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTROTSimple256Scalar: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);

   // Evaluate all arguments
   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));

   // Allocate GPR and misc. registers
   TR::Register *chReg	= cg->allocateRegister();
   TR::Register *input2Reg = cg->allocateRegister();
   TR::Register *cnd1Reg = cg->allocateRegister(TR_CCR);

   TR::Register *countReg	= cg->allocateRegister();
   TR::Register *bytesReg	= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);

   TR::Register *retReg		= cg->allocateRegister();

   TR::LabelSymbol *lblByteLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipByteLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblUnrolledLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit1 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit2 = generateLabelSymbol(cg);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cnd1Reg, lenReg, 0);		// len == 0 ?
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, bytesReg, lenReg, 0x3);	// bytes = len & 3
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, countReg, lenReg, 32 - 2, 0x3fffffff);	// count = len / 4
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, inputReg, inputReg, -1);	// input--
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, outputReg, outputReg, -2);	// output--
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)				// if len==0, exit2
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblExit2, cnd1Reg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblExit2, cnd1Reg);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cnd1Reg, countReg, 0);		// count == 0 ?
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)				// if bytes == 0, skip byte loop
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblSkipByteLoop, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblSkipByteLoop, cndReg);

   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, bytesReg);				// initialize byte count

   // byte loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblByteLoop);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 1, 1, cg));	// load byte
   generateMemSrc1Instruction(cg, TR::InstOpCode::sthu, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 2, 2, cg), chReg);	// store half-word
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblByteLoop, cndReg);	// releat byte loop

   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)				// if count == 0, exit2
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblExit2, cnd1Reg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblExit2, cnd1Reg);

   // Skip byte loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkipByteLoop);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, countReg);				// initialize unrolled loop count
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 1, 1, cg));
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, input2Reg, inputReg, 4);	// compute input addrss for the second iteration
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 2, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 2, 1, cg));
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 4, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 3, 1, cg));
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 6, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 4, 1, cg));
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdz, node, lblExit1, cndReg);		// if only one iteration, exit1

   // unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblUnrolledLoop);
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 8, 2, cg), chReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, outputReg, outputReg, 8);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(input2Reg, 1, 1, cg));
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 2, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(input2Reg, 2, 1, cg));
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 4, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(input2Reg, 3, 1, cg));
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, input2Reg, input2Reg, 4);
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 6, 2, cg), chReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(input2Reg, 0, 1, cg));
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblUnrolledLoop, cndReg);

   // exit1
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit1);
   generateMemSrc1Instruction(cg, TR::InstOpCode::sth, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 8, 2, cg), chReg);

   // exit2
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit2);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, retReg, lenReg, -1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(7, 7, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,    TR::RealRegister::cr0,   TR_CCR, cg); // Need to bind to CR0 ?
   addDependency(conditions, cnd1Reg,   TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, input2Reg, TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, chReg,     TR::RealRegister::NoReg, TR_CCR, cg);
   // constants
   // variables
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg);

   cg->stopUsingRegister(countReg);
   cg->stopUsingRegister(bytesReg);

   return retReg;
   }

// Not yet accepted
static TR::Register *arraytranslateTROTSimple256ScalarOpt(TR::Node *node, TR::CodeGenerator *cg, int max)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTROTSimple256ScalarOpt: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);

   // Evaluate arguments
   TR::Register *inputReg  	= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg 	= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg    	= cg->gprClobberEvaluate(node->getChild(4));

   // Allocate GPR and misc. registers
   TR::Register *input1Reg  	= cg->allocateRegister();
   TR::Register *output1Reg 	= cg->allocateRegister();
   TR::Register *ch0Reg		= cg->allocateRegister();
   TR::Register *ch1Reg		= cg->allocateRegister();
   TR::Register *ch2Reg		= cg->allocateRegister();
   TR::Register *ch3Reg		= cg->allocateRegister();
   TR::Register *ch4Reg		= cg->allocateRegister();
   TR::Register *ch5Reg		= cg->allocateRegister();
   TR::Register *ch6Reg		= cg->allocateRegister();
   TR::Register *ch7Reg		= cg->allocateRegister();

   TR::Register *tmpReg		= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);

   TR::Register *retReg		= cg->allocateRegister();

   TR::LabelSymbol *lblPreLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut0	= generateLabelSymbol(cg);

   CMPI4(cnd,len,0);
   BEQ(cnd,Out0);
   ANDI_R(tmp,len,7);
   ADDI(input,input,-1);
   ADDI(output,output,-2);
   BEQ(cnd,SkipPreLoop);
   MTCTR(tmp);

   // PreLoop
   LABEL(PreLoop);
   LBZU(ch0,1,input);
   STHU(ch0,2,output);
   BDNZ(cnd,PreLoop);
   // PreLoop end

   ADDI(input,input,1);
   ADDI(output,output,2);
   LABEL(SkipPreLoop);

   RLWINM(tmp,len,32-3,0x1fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,Out0);

   /*c0*/                     LBZ(ch0, 0,input ); ADDI(input1 ,input , 0);
   /*c1*/                     LBZ(ch1, 1,input );
   /*c2*/                     LBZ(ch2, 2,input );
   /*c3*/                     LBZ(ch3, 3,input );
   LABEL(Loop);
   /*l1*/STH(ch0, 0,output ); LBZ(ch4, 4,input1); ADDI(input  ,input1 , 8);
   /*l2*/STH(ch1, 2,output ); LBZ(ch5, 5,input1); ADDI(output1,output , 0);
   /*l3*/STH(ch2, 4,output ); LBZ(ch6, 6,input1);
   /*l4*/STH(ch3, 6,output ); LBZ(ch7, 7,input1);
   /*l5*/STH(ch4, 8,output1); LBZ(ch0, 0,input ); ADDI(input1 ,input  , 0);
   /*l6*/STH(ch5,10,output1); LBZ(ch1, 1,input ); ADDI(output ,output1,16);
   /*l7*/STH(ch6,12,output1); LBZ(ch2, 2,input );
   /*l8*/STH(ch7,14,output1); LBZ(ch3, 3,input );
   BDNZ(cnd,Loop);
   // Loop end

   // Out0:
   LABEL(Out0);
   ADDI(ret,len,-1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(14, 14, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, input1Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, output1Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,    TR::RealRegister::cr0,   TR_CCR, cg); // Need to bind to CR0 ?
   addDependency(conditions, ch0Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch1Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch2Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch3Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch4Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch5Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch6Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, ch7Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   // constants
   // variables
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg);
   cg->stopUsingRegister(tmpReg);

   return retReg;
   }

// arraytranslateTROTSimpleVMX --
//
// VMX is used only when the following conditions are satisfied:
//   - table is compile time constant
//   - delim is compile time cnstant
//   - table[i] = (uint16)i for i=0..max (0 <= max <= 0xff)
//
// Note that when max=0xff, we don't need to check input data, but...
// Note also that in the current implmentation, check code for validating input
// data can handle arbitrary bitmap, while code translation is just removing the first byte
// of every pair of bytes.
//
static TR::Register *arraytranslateTROTSimpleVMX(TR::Node *node, TR::CodeGenerator *cg, int max)
   {
   //
   // tree looks as follows:
   // arraytranslate (TROT)
   //    input ptr
   //    output ptr
   //    translation table
   //    stop character			(Must be a TR::iconst node)
   //    input length (in elements)	(Usually TR::isub or i2l/isub)
   //
   // Number of elements translated is returned
   //
   TRACE_VMX0("@arraytranslateTROTSimpleVMX:\n");

   // Evaluate all arguments
   TR::Register *inputReg  = cg->evaluate(node->getFirstChild());
   TR::Register *outputReg = cg->evaluate(node->getSecondChild());
   TR::Register *tableReg  = cg->evaluate(node->getChild(2));
   TR::Register *delimReg  = cg->evaluate(node->getChild(3));
   TR::Register *lenReg    = cg->evaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tableReg, tableReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   TR::Node *delimNode = node->getChild(3);
   TRACE_VMX1("@arraytranslateTROTSimpleVMX: delimNode=%s\n",
              delimNode->getOpCodeValue() == TR::iconst ? "iconst" :
              delimNode->getOpCodeValue() == TR::lconst ? "lconst" : "expr");
   TR::Node *lenNode = node->getChild(4);
   TRACE_VMX2("@arraytranslateTROTSimpleVMX: lenNode=%s (0x%x)\n",
              lenNode->getOpCodeValue() == TR::iconst ? "iconst" :
              lenNode->getOpCodeValue() == TR::lconst ? "lconst" : "expr",
              lenNode->getOpCodeValue());

   // Allocate GPR and misc. registers
   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   TR::Register *tmp2Reg   = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);
   TR::Register *cr6Reg   = cg->allocateRegister(TR_CCR);

   // Prepare constants
   TR::Register *const0Reg = cg->allocateRegister();
   TR::Register *const16Reg = cg->allocateRegister();
   TR::Register *const32Reg = cg->allocateRegister();
   loadConstant(cg, node, 0x0, indexReg);
   loadConstant(cg, node, 0x0, const0Reg);
   loadConstant(cg, node, 0x10, const16Reg);
   loadConstant(cg, node, 0x20, const32Reg);

   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHeadv = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExitv = generateLabelSymbol(cg);

   // If the length is less than or equal to 48, skip VMX process
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 48);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblExitv, cndReg);

   /*
      some scalar loop to process unaligned data
      pseudo code

      while (i<limit)
      {
      char c = input[i];
      short d = delimiter_table[in];
      if (d == delimiter) break;
      output[i] = d;
      i++;
      }
   */
   // CTR = (16 - (pOutput & 0xF)) / 2
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tmp2Reg, outputReg, 0xF);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmp2Reg, tmp2Reg, const16Reg);
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp2Reg, tmp2Reg, 1);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp2Reg, tmp2Reg, 32 - 1, 0x7fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmp2Reg);

   // loop body, see an above pseudo code
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, indexReg, 1, cg));
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, tmpReg, tmpReg, const1Reg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 1, 0xfffffffe);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lhzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, tmpReg, 2, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, tmpReg, delimReg);
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, tmp2Reg, indexReg, const1Reg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp2Reg, indexReg, 1, 0xfffffffe);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblExit, cndReg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::sthx, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, tmp2Reg, 2, cg), tmpReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 0x1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead0, cndReg);
   // end loop body

   TR::Register *vtab0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vtab1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vconst0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vconst5Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vconst7FReg = cg->allocateRegister(TR_VRF);
   TR::Register *constTableReg = cg->allocateRegister();

   TR::Register *vtmpReg = cg->allocateRegister(TR_VRF);
   TR::Register *vpatReg = cg->allocateRegister(TR_VRF);
   TR::Register *vec1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vec2Reg = cg->allocateRegister(TR_VRF);
   TR::Register *poutReg = cg->allocateRegister();
   TR::Register *vbytesReg = cg->allocateRegister(TR_VRF);
   TR::Register *vcharsReg = cg->allocateRegister(TR_VRF);
   TR::Register *vbitsReg = cg->allocateRegister(TR_VRF);
   TR::Register *vresultReg = cg->allocateRegister(TR_VRF);
   TR::Register *vbitoffReg = cg->allocateRegister(TR_VRF);

   //XXX FIXME: Get rid of using vconstTable, instead, use vector compare
   // load constant
   uint32_t *vconstTable = (uint32_t*)TR_Memory::jitPersistentAlloc(12*sizeof(uint32_t) + (16 - 1));
   vconstTable = (uint32_t *)(((((uintptr_t)vconstTable-1) >> 4) + 1) << 4); // align to 16-byte boundary
   createBitTable((uint16_t *)(node->getChild(2)->getSymbolReference()->getSymbol()->getStaticSymbol()->getStaticAddress()), 0xFFFF, (uint8_t *)vconstTable, cg);
   loadAddressConstant(cg, node, (intptrj_t)vconstTable, constTableReg);
   for (int i= 8; i<12; i++) vconstTable[i] = 0x7F7F7F7F;

   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0Reg, 0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab0Reg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const16Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconst7FReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const32Reg, 16, cg));
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst5Reg, 5);

   // prepare for loop
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmp2Reg, indexReg, lenReg);
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmp2Reg, tmp2Reg, 4);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp2Reg, tmp2Reg, 32 - 4, 0x0fffffff);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmp2Reg, tmp2Reg, -1);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmp2Reg);

   //generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, tmp2Reg, indexReg, const1Reg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp2Reg, indexReg, 1, 0xfffffffe);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, poutReg, outputReg, tmp2Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmp2Reg, indexReg, inputReg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpatReg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vec1Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const0Reg, 16, cg));

   /*
      pseudo code of loop using VMX

      vec1 = vinput[i++];
      while (i<limit)
      {
      // load one vector
      vec2 = vinput[i];
      // concatinate two vectors into one to align output address
      vbytes = vec_perm(vec1, vec2, align);

      // is ther delimiter?
      vtmp = vec_perm(vdelimiter_table0, vdelimiter_table1, vbytes);
      vbitoff = vec_sr(vtmp, vconst5);
      vbits = vec_sl(vtmp, vbitoff);
      if (vec_any_gt(vbits, vconst7F)) break;

      // store unpacked vectors
      output[2*i  ] = vec_mergeh(vconst0, vbytes);
      output[2*i+1] = vec_mergel(vconst0, vbytes);

      vec1 = vec2;
      i++;
      }
   */
   // loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadv);

   // load one vector
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vec2Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const16Reg, 16, cg));
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytesReg, vec1Reg, vec2Reg, vpatReg);

   // check delimiter
   // FIXME: We don't need to care about general case where any character can be delimiters.
   // What actually important is just two cases where valid range is either 0x00-0x7f or 0x00-0xff.
   // Either case may simplify validation code here
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vtmpReg, vtab0Reg, vtab1Reg, vbytesReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoffReg, vbytesReg, vconst5Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbitsReg, vtmpReg, vbitoffReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbitsReg, vconst7FReg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblExitv, cr6Reg);

   // translate and store
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vmrghb, node, vcharsReg, vconst0Reg, vbytesReg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stvx, node, new (cg->trHeapMemory()) TR::MemoryReference(poutReg, const0Reg, 16, cg), vcharsReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vmrglb, node, vcharsReg, vconst0Reg, vbytesReg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::stvx, node, new (cg->trHeapMemory()) TR::MemoryReference(poutReg, const16Reg, 16, cg), vcharsReg);

   // advance pointers
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vor, node, vec1Reg, vec2Reg, vec2Reg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 0x10);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, poutReg, poutReg, 0x20);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmp2Reg, tmp2Reg, 0x10);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadv, cndReg);
   // end loop body

   // FIXME: the remaining should be also processed by VMX

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExitv);

   /*
      pseudo code

      while (i<limit)
      {
      char c = input[i];
      short d = delimiter_table[in];
      if (d == delimiter) break;
      output[i] = d;
      i++;
      }
   */
   // set CTR
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmp2Reg, indexReg, lenReg);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmp2Reg);

   // loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, indexReg, 1, cg));
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, tmpReg, tmpReg, const1Reg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 1, 0xfffffffe);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lhzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, tmpReg, 2, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, tmpReg, delimReg);
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::slw, node, tmp2Reg, indexReg, const1Reg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmp2Reg, indexReg, 1, 0xfffffffe);

   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblExit, cndReg);
   generateMemSrc1Instruction(cg, TR::InstOpCode::sthx, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, tmp2Reg, 2, cg), tmpReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 0x1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, -1);

   // Manage dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(20, 20, cg->trMemory());
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tmpReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tmp2Reg,   TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cr6Reg,   TR::RealRegister::cr6,   TR_CCR, cg);
   addDependency(conditions, const0Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const16Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const32Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, poutReg,   TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, vtab0Reg,  TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vtab1Reg,  TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconst0Reg,TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconst5Reg,TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconst7FReg,TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpatReg,   TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vec1Reg,   TR::RealRegister::NoReg, TR_VRF, cg);

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->stopUsingRegister(cndReg);
   cg->stopUsingRegister(constTableReg);
   cg->stopUsingRegister(vtmpReg);
   cg->stopUsingRegister(vbitsReg);
   cg->stopUsingRegister(vresultReg);
   cg->stopUsingRegister(vbitoffReg);
   cg->stopUsingRegister(vcharsReg);
   cg->stopUsingRegister(vbytesReg);
   cg->stopUsingRegister(vec2Reg);

   return indexReg;
   }

// arraytranslateTRTO --
//
//   Dispatch one of three methods according to the type of translation
//   table and availability of VMX
//
static TR::Register *arraytranslateTRTO(TR::Node *node, TR::CodeGenerator *cg)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTRTO: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);
   int max;
   TR::Register *retReg;

   max = analyzeTRTOTable(node, cg);
   if (max >= 0)
      {
      if (VMXEnabled(cg))
         retReg = arraytranslateTRTOSimpleVMX(node, cg, max);			// ISO8859_1: 822.7 cycles/16-chars1.4 cycles/char )
      else
         retReg = arraytranslateTRTOSimpleScalar(node, cg, max);		// ISO8859_1: 2.7 cycle/char
      }
   else
      {
      if (termCharNodeIsHint == false)
         retReg = arraytranslateTRTOGenericScalarWithBreak(node, cg);		// Cp037 3.2 cycle/char
      else
         {
         if (sourceCellIsTermChar == true)
            retReg = arraytranslateTRTOGenericScalarWithBreakTermChar(node, cg);// ??
         else
            retReg = arraytranslateTRTOGenericScalarNoBreak(node, cg);		// ??
         }
      }
   TRACE_VMX1("@arraytranslateTRTO: returning %p\n", retReg);
   return retReg;
   }

// arraytranslateTRTOGenericScalar --
//
// PPC scalar implementation
//
// Input:
//   uint16 *input;
//   uint8  *output;
//   uint8  table[65536];
//   int    delim;
//   int    len;
//
// Pseudo code:
//   int index = 0;
//   if (len != 0) {
//      CTR = len;
//      input--;
//      output--;
//      do {
//         uint16 ch = *++input;
//         uint8 xch = table[ch];
// #if TERM_CHAR_NODE_IS_HINT==0
//         if (xch == delim)
//            break;
// #else
// #  if SOUCE_CELL_IS_TERM_CHAR==1
//         if (xch == delim && ch >= 0x100)
//            break;
// #  endif
// #endif
//         *++output = ch;
//         index++;
//      } while (--CTR != 0);
//   }
//   return index - 1;
//

static TR::Register *arraytranslateTRTOGenericScalarNoBreak(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraytranslateTRTOGenericScalarNoBreak:\n");

   TR_ASSERT(node->getTermCharNodeIsHint() == true && node->getSourceCellIsTermChar() == false, "arraytranslateTRTOGenericScalarNoBreak");

   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   //TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   //TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));

   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *input1Reg  = cg->allocateRegister();
   TR::Register *output1Reg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   TR::Register *ch0Reg    = cg->allocateRegister();
   TR::Register *ch1Reg    = cg->allocateRegister();
   TR::Register *ch2Reg    = cg->allocateRegister();
   TR::Register *ch3Reg    = cg->allocateRegister();
   TR::Register *ch4Reg    = cg->allocateRegister();
   TR::Register *ch5Reg    = cg->allocateRegister();
   TR::Register *ch6Reg    = cg->allocateRegister();
   TR::Register *ch7Reg    = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut0 = generateLabelSymbol(cg);

   CMPI4(cnd,len,0);
   BEQ(cnd,Out0);
   ANDI_R(tmp,len,7);
   BEQ(cnd,SkipPreLoop);
   ADDI(input,input,-2);
   ADDI(output,output,-1);
   MTCTR(tmp);

   // PreLoop
   LABEL(PreLoop);
   LHZU(ch0,2,input);
   STBU(ch0,1,output);
   BDNZ(cnd,PreLoop);
   // PreLoop end

   ADDI(input,input,2);
   ADDI(output,output,1);

   // SkipPreLoop
   LABEL(SkipPreLoop);
   RLWINM(tmp,len,32 - 3, 0x1fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,Out0);

   /*c0*/                    LHZ(ch0, 0,input ); ADDI(input1 ,input , 0);
   /*c1*/                    LHZ(ch1, 2,input );
   /*c2*/                    LHZ(ch2, 4,input );
   /*c3*/                    LHZ(ch3, 6,input );
   LABEL(Loop);
   /*l1*/STB(ch0,0,output ); LHZ(ch4, 8,input1); ADDI(input  ,input1,16);
   /*l2*/STB(ch1,1,output ); LHZ(ch5,10,input1); ADDI(output1,output, 0);
   /*l3*/STB(ch2,2,output ); LHZ(ch6,12,input1);
   /*l4*/STB(ch3,3,output ); LHZ(ch7,14,input1);
   /*l5*/STB(ch4,4,output1); LHZ(ch0, 0,input ); ADDI(input1,input  , 0);
   /*l6*/STB(ch5,5,output1); LHZ(ch1, 2,input ); ADDI(output,output1, 8);
   /*l7*/STB(ch6,6,output1); LHZ(ch2, 4,input );
   /*l8*/STB(ch7,7,output1); LHZ(ch3, 6,input );
   BDNZ(cnd,Loop);
   // Loop end

   LABEL(Out0); ADDI(index,len,-1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(15, 15, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, input1Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, output1Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch2Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch3Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch4Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch5Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch6Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch7Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,     TR::RealRegister::cr0,   TR_CCR, cg);

   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   // Release registers
   cg->stopUsingRegister(tmpReg);

   return indexReg;
   }

static TR::Register *arraytranslateTRTOGenericScalarWithBreak(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraytranslateTRTOGenericScalarWithBreak:\n");

   TR_ASSERT(node->getTermCharNodeIsHint() == false, "arraytranslateTRTOGenericScalarWithBreak");

   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      ADDI(table,table,TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *input1Reg  = cg->allocateRegister();
   TR::Register *output1Reg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   //TR::Register *maxReg    = cg->allocateRegister();
   TR::Register *ch0Reg    = cg->allocateRegister();
   TR::Register *ch1Reg    = cg->allocateRegister();
   TR::Register *ch2Reg    = cg->allocateRegister();
   TR::Register *ch3Reg    = cg->allocateRegister();
   TR::Register *ch4Reg    = cg->allocateRegister();
   TR::Register *ch5Reg    = cg->allocateRegister();
   TR::Register *ch6Reg    = cg->allocateRegister();
   TR::Register *ch7Reg    = cg->allocateRegister();
   TR::Register *xch0Reg    = cg->allocateRegister();
   TR::Register *xch1Reg    = cg->allocateRegister();
   TR::Register *xch2Reg    = cg->allocateRegister();
   TR::Register *xch3Reg    = cg->allocateRegister();
   TR::Register *xch4Reg    = cg->allocateRegister();
   TR::Register *xch5Reg    = cg->allocateRegister();
   TR::Register *xch6Reg    = cg->allocateRegister();
   TR::Register *xch7Reg    = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd1Reg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd2Reg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd3Reg    = cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut1 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut2 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut3 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut4 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut5 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut6 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut7 = generateLabelSymbol(cg);

   loadConstant(cg, node, 0x0, indexReg);
   //loadConstant(cg, node, max, maxReg);
   CMPI4(cnd,len,0);
   BEQ(cnd,Out0);
   ANDI_R(tmp,len,7);
   BEQ(cnd,SkipPreLoop);
   ADDI(input,input,-2);
   ADDI(output,output,-1);
   MTCTR(tmp);

   // PreLoop
   LABEL(PreLoop);
   LHZU(ch0,2,input);
   LBZX(xch0,table,ch0);
   CMP4(cnd,xch0,delim);
   BGT(cnd,Out0);
   STBU(xch0,1,output);
   ADDI(index,index,1);
   BDNZ(cnd,PreLoop);
   // PreLoop end

   ADDI(input,input,2);
   ADDI(output,output,1);

   // SkipPreLoop
   LABEL(SkipPreLoop);
   RLWINM(tmp,len,32 - 3, 0x1fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,Out0);

   /*a0*/                                                                  LHZ(ch0, 0,input ); ADDI(input1 ,input , 0);
   /*a1*/                                                                  LHZ(ch1, 2,input );
   /*a2*/                                                                  LHZ(ch2, 4,input );
   /*a3*/                                                                  LHZ(ch3, 6,input );
   /*b0*/                                            LBZX(xch0,table,ch0); LHZ(ch4, 8,input1); ADDI(input  ,input1,16);
   /*b1*/                                            LBZX(xch1,table,ch1); LHZ(ch5,10,input1);
   /*b2*/                                            LBZX(xch2,table,ch2); LHZ(ch6,12,input1);
   /*b3*/                                            LBZX(xch3,table,ch3); LHZ(ch7,14,input1);
   /*c0*/                     CMP4(cnd ,xch0,delim); LBZX(xch4,table,ch4); LHZ(ch0, 0,input ); ADDI(input1 ,input , 0);
   /*c1*/                     CMP4(cnd1,xch1,delim); LBZX(xch5,table,ch5); LHZ(ch1, 2,input );
   /*c2*/                     CMP4(cnd2,xch2,delim); LBZX(xch6,table,ch6); LHZ(ch2, 4,input );
   /*c3*/                     CMP4(cnd3,xch3,delim); LBZX(xch7,table,ch7); LHZ(ch3, 6,input );
   LABEL(Loop);
   /*l0*/                                                                                                               BEQ(cnd ,Out0);
   /*l1*/STB(xch0,0,output ); CMP4(cnd ,xch4,delim); LBZX(xch0,table,ch0); LHZ(ch4, 8,input1); ADDI(input  ,input1,16); BEQ(cnd1,Out1);
   /*l2*/STB(xch1,1,output ); CMP4(cnd1,xch5,delim); LBZX(xch1,table,ch1); LHZ(ch5,10,input1); ADDI(output1,output, 0); BEQ(cnd2,Out2);
   /*l3*/STB(xch2,2,output ); CMP4(cnd2,xch6,delim); LBZX(xch2,table,ch2); LHZ(ch6,12,input1);                          BEQ(cnd3,Out3);
   /*l4*/STB(xch3,3,output ); CMP4(cnd3,xch7,delim); LBZX(xch3,table,ch3); LHZ(ch7,14,input1);                          BEQ(cnd ,Out4);
   /*l5*/STB(xch4,4,output1); CMP4(cnd ,xch0,delim); LBZX(xch4,table,ch4); LHZ(ch0, 0,input ); ADDI(input1,input  , 0); BEQ(cnd1,Out5);
   /*l6*/STB(xch5,5,output1); CMP4(cnd1,xch1,delim); LBZX(xch5,table,ch5); LHZ(ch1, 2,input ); ADDI(output,output1, 8); BEQ(cnd2,Out6);
   /*l7*/STB(xch6,6,output1); CMP4(cnd2,xch2,delim); LBZX(xch6,table,ch6); LHZ(ch2, 4,input );                          BEQ(cnd3,Out7);
   /*l8*/STB(xch7,7,output1); CMP4(cnd3,xch3,delim); LBZX(xch7,table,ch7); LHZ(ch3, 6,input ); ADDI(index ,index  , 8);
   BDNZ(cnd,Loop);
   // Loop end

   B(cnd,Out0);

   LABEL(Out1); ADDI(index,index,1); B(cnd,Out0);
   LABEL(Out2); ADDI(index,index,2); B(cnd,Out0);
   LABEL(Out3); ADDI(index,index,3); B(cnd,Out0);
   LABEL(Out4); ADDI(index,index,4); B(cnd,Out0);
   LABEL(Out5); ADDI(index,index,5); B(cnd,Out0);
   LABEL(Out6); ADDI(index,index,6); B(cnd,Out0);
   LABEL(Out7); ADDI(index,index,7); B(cnd,Out0);
   LABEL(Out0); ADDI(index,index,-1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(28, 28, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   //addDependency(conditions, maxReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, input1Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, output1Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch2Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch3Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch4Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch5Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch6Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch7Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch0Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch1Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch2Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch3Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch4Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch5Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch6Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, xch7Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,     TR::RealRegister::cr0,   TR_CCR, cg);
   addDependency(conditions, cnd1Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cnd2Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cnd3Reg,    TR::RealRegister::NoReg, TR_CCR, cg);

   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   // Release registers
   cg->stopUsingRegister(tmpReg);

   return indexReg;
   }

static TR::Register *arraytranslateTRTOGenericScalarWithBreakTermChar(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraytranslateTRTOGenericScalarWithBreakTermChar:\n");

   TR_ASSERT(node->getTermCharNodeIsHint() == true && node->getSourceCellIsTermChar() == true, "arraytranslateTRTOGenericScalarWithBreakTermChar");

   // Evaluate all arguments
   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tableReg, tableReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   // Allocate GPR and misc. registers
   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *chReg    = cg->allocateRegister();
   TR::Register *xchReg   = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);

   // Allocate labels
   TR::LabelSymbol *lblLoopHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoopCont = generateLabelSymbol(cg);
   TR::LabelSymbol *lblDone = generateLabelSymbol(cg);
   TR::LabelSymbol *lblCheck = generateLabelSymbol(cg);

   // index = 0;
   loadConstant(cg, node, 0x0, indexReg);

   // if (len <= 0) goto done
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, PPCOpProp_BranchUnlikely, node, lblDone, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblDone, cndReg);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // input--;
   // output--;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, inputReg, inputReg, -2);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, outputReg, outputReg, -1);

   // do {
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblLoopHead);
   // ch = *++input;
   generateTrg1MemInstruction(cg, TR::InstOpCode::lhzu, node, chReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 2, 2, cg));
   // xch = table[ch];
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, xchReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, chReg, 1, cg));

   // if (xch == delim) goto check routine, and return back condition not met
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmpl4, node, cndReg, xchReg, delimReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblCheck, cndReg);
   // We come back here if exit condition is not met
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblLoopCont);

   // *++output = xch;
   generateMemSrc1Instruction(cg, TR::InstOpCode::stbu, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 1, 1, cg), xchReg);
   // index++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   // } while (--CTR != 0)
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblLoopHead, cndReg);

   // Jump to lblDone
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblDone);
   // lblCheck:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblCheck);
   // if (ch < 0x100) go back loop
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, chReg, 0x100);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, lblLoopCont, cndReg);
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblDone);
   // index - 1;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, -1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(7, 7, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, chReg,     TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // Release all associated registers except indexReg

   // Release registers
   cg->stopUsingRegister(xchReg);
   cg->stopUsingRegister(cndReg);

   return indexReg;
   }

// arraytranslateTRTOSimpleScalar --
//
// This code gen is used when the translation table is an
// identity mapping with upper limit 'max'. Other entries in the table
// are all filled with stop character. So we can eliminate table loop up
// operation as follows:
//
//   uint16 *input;
//   uint8 *output;
//   for (index = 0; index < len; index++) {
//       int ch = input[index];
//       if (ch > max) break;
//       output[index] = ch;
//   }
//   return index - 1;
//
// Actual code is 8-times unrolled to get higher throughput.
//
// TRTOSimpleScalar:
//         loadConst indexReg,0		           ! index = 0
//	   loadConst maxReg,max			   ! max = upper limit
//         cmpi    cndReg,0,lenReg,0               ! len == 0 ?
//         beq     cndReg,out0                     ! if len=0 -> out0
//         andi.   tmpReg,lenReg,0x0007            ! tmp = len & 7
//         beq     cnd,skipPreLoop                 ! if bytes = 0, skip pre-loop
//         addi    inpReg,inpReg,-2                ! input--
//         addi    outpReg,outpReg,-1              ! output--
//         mtspr   CTR,tmpReg                      ! initialize pre loop count
//
// preLoop:
//         lhzu    ch0Reg,2(inpReg)                ! load byte
//	   cmp	   cnd0,ch0Reg,maxReg
//         bgt	   cnd0,out0
//         stbu    chReg,1(outReg)                 ! store half-word
//         bc      BO_dCTR_NZERO,CR0_LT,preLoop    ! repeat byte loop
//         addi    inpReg,inpReg,1                 ! input++
//         addi    outpReg,outpReg, 2              ! output++
// skipPreLoop:
//         rlwinm  tmpReg,lenReg,32-3,3,31         ! count = len / 8
//         mtspr   CTR,tmpReg
//         cmpi    cnd0,tpmReg,0                   ! count == 0 ?
//         beq	   cnd0,out0
// ! The following section of the code assumes Power4 type dispatch mechanism.
//
// ! Slot1                 Slot2                   Slot3                  Slot4                      Branch
// !-----------------------------------------------------------------------------------------------------------------
//                                                 lhz ch0Reg,0(inp0Reg); addi inp1Reg,inp0Reg,8
//                                                 lhz ch1Reg,2(inp0Reg)
//                                                 lhz ch2Reg,4(inp0Reg)
//                                                 lhz ch3Reg,6(inp0Reg)
//                         cmp cnd0,ch0Reg,maxReg; lhz ch4Reg,0(inp1Reg); addi inp0Reg,inp1Reg,8
//                         cmp cnd1,ch1Reg,maxReg; lhz ch5Reg,2(inp1Reg)
//                         cmp cnd2,ch2Reg,maxReg; lhz ch6Reg,4(inp1Reg)
//                         cmp cnd3,ch3Reg,maxReg; lhz ch7Reg,6(inp1Reg)
// loop:
// !--------------------------------------------------------------------------------------
//                                                                                                   bgt cnd0,out
// stb ch0Reg,0(outp0Reg); cmp cnd0,ch4Reg,maxReg; lhz ch0Reg,0(inp0Reg); addi inp1Reg,inp0Reg,8;    bgt cnd1,out1
// stb ch1Reg,1(outp0Reg); cmp cnd1,ch5Reg,maxReg; lhz ch1Reg,2(inp0Reg); addi outp1Reg, outp0Reg,4; bgt cnd2,out2
// stb ch2Reg,2(outp0Reg); cmp cnd2,ch6Reg,maxReg; lhz ch2Reg,4(inp0Reg);                            bgt cnd3,out3
// stb ch3Reg,3(outp0Reg); cmp cnd3,ch7Reg,maxReg; lhz ch3Reg,6(inp0Reg);                            bgt cnd0,out4
// stb ch4Reg,0(outp1Reg); cmp cnd0,ch0Reg,maxReg; lhz ch4Reg,0(inp1Reg); addi inp0Reg,inp1Reg,8;    bgt cnd1,out5
// stb ch5Reg,1(outp1Reg); cmp cnd1,ch1Reg,maxReg; lhz ch5Reg,2(inp1Reg); addi outp0Reg, outp1Reg,4; bgt cnd2,out6
// stb ch6Reg,2(outp1Reg); cmp cnd2,ch2Reg,maxReg; lhz ch6Reg,4(inp1Reg);                            bgt cnd3,out7
// stb ch7Reg,3(outp1Reg); cmp cnd3,ch3Reg,maxReg; lhz ch7Reg,6(inp1Reg); addi indexReg,indexReg,8;  bdnz loop
// !--------------------------------------------------------------------------------------
// b out
//
// out1: addi indexReg,indexReg,1; b out
// out2: addi indexReg,indexReg,2; b out
// out3: addi indexReg,indexReg,3; b out
// out4: addi indexReg,indexReg,4; b out
// out5: addi indexReg,indexReg,5; b out
// out6: addi indexReg,indexReg,6; b out
// out7: addi indexReg,indexReg,7
// out: addi indexReg,indexReg,-1
//
static TR::Register *arraytranslateTRTOSimpleScalar(TR::Node *node, TR::CodeGenerator *cg, int max)
   {
   TRACE_VMX1("@arraytranslateTRTOSimpleScalar: max=0x%x\n", max);

   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   //TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   //TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));

   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *input1Reg  = cg->allocateRegister();
   TR::Register *output1Reg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   TR::Register *maxReg    = cg->allocateRegister();
   TR::Register *ch0Reg    = cg->allocateRegister();
   TR::Register *ch1Reg    = cg->allocateRegister();
   TR::Register *ch2Reg    = cg->allocateRegister();
   TR::Register *ch3Reg    = cg->allocateRegister();
   TR::Register *ch4Reg    = cg->allocateRegister();
   TR::Register *ch5Reg    = cg->allocateRegister();
   TR::Register *ch6Reg    = cg->allocateRegister();
   TR::Register *ch7Reg    = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd1Reg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd2Reg    = cg->allocateRegister(TR_CCR);
   TR::Register *cnd3Reg    = cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut1 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut2 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut3 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut4 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut5 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut6 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblOut7 = generateLabelSymbol(cg);

   loadConstant(cg, node, 0x0, indexReg);
   loadConstant(cg, node, max, maxReg);
   CMPI4(cnd,len,0);
   BEQ(cnd,Out0);
   ANDI_R(tmp,len,7);
   BEQ(cnd,SkipPreLoop);
   ADDI(input,input,-2);
   ADDI(output,output,-1);
   MTCTR(tmp);

   // PreLoop
   LABEL(PreLoop);
   LHZU(ch0,2,input);
   CMP4(cnd,ch0,max);
   BGT(cnd,Out0);
   STBU(ch0,1,output);
   ADDI(index,index,1);
   BDNZ(cnd,PreLoop);
   // PreLoop end

   ADDI(input,input,2);
   ADDI(output,output,1);

   // SkipPreLoop
   LABEL(SkipPreLoop);
   RLWINM(tmp,len,32 - 3, 0x1fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,Out0);

   // prologue
   /*p0*/                                        LHZ(ch0, 0,input ); ADDI(input1,input , 0);
   /*p1*/                                        LHZ(ch1, 2,input );
   /*p2*/                                        LHZ(ch2, 4,input );
   /*p3*/                                        LHZ(ch3, 6,input );
   /*p4*/                    CMP4(cnd ,ch0,max); LHZ(ch4, 8,input1); ADDI(input ,input1,16);
   /*p5*/                    CMP4(cnd1,ch1,max); LHZ(ch5,10,input1);
   /*p6*/                    CMP4(cnd2,ch2,max); LHZ(ch6,12,input1);
   /*p7*/                    CMP4(cnd3,ch3,max); LHZ(ch7,14,input1);

   // Loop
   LABEL(Loop);
   /*l0*/                                                                                     BEQ(cnd ,Out0);
   /*l1*/STB(ch0,0,output ); CMP4(cnd ,ch4,max); LHZ(ch0, 0,input ); ADDI(input1 ,input , 0); BEQ(cnd1,Out1);
   /*l2*/STB(ch1,1,output ); CMP4(cnd1,ch5,max); LHZ(ch1, 2,input ); ADDI(output1,output, 0); BEQ(cnd2,Out2);
   /*l3*/STB(ch2,2,output ); CMP4(cnd2,ch6,max); LHZ(ch2, 4,input );                          BEQ(cnd3,Out3);
   /*l4*/STB(ch3,3,output ); CMP4(cnd3,ch7,max); LHZ(ch3, 6,input );                          BEQ(cnd ,Out4);
   /*l5*/STB(ch4,4,output1); CMP4(cnd ,ch0,max); LHZ(ch4, 8,input1); ADDI(input ,input1 ,16); BEQ(cnd1,Out5);
   /*l6*/STB(ch5,5,output1); CMP4(cnd1,ch1,max); LHZ(ch5,10,input1); ADDI(output,output1, 8); BEQ(cnd2,Out6);
   /*l7*/STB(ch6,6,output1); CMP4(cnd2,ch2,max); LHZ(ch6,12,input1);                          BEQ(cnd3,Out7);
   /*l8*/STB(ch7,7,output1); CMP4(cnd3,ch3,max); LHZ(ch7,14,input1); ADDI(index ,index  , 8);
   BDNZ(cnd,Loop);
   // Loop end

   B(cnd,Out0);

   LABEL(Out1); ADDI(index,index,1); B(cnd,Out0);
   LABEL(Out2); ADDI(index,index,2); B(cnd,Out0);
   LABEL(Out3); ADDI(index,index,3); B(cnd,Out0);
   LABEL(Out4); ADDI(index,index,4); B(cnd,Out0);
   LABEL(Out5); ADDI(index,index,5); B(cnd,Out0);
   LABEL(Out6); ADDI(index,index,6); B(cnd,Out0);
   LABEL(Out7); ADDI(index,index,7); B(cnd,Out0);
   LABEL(Out0); ADDI(index,index,-1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(19, 19, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   //addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   //addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, maxReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, input1Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, output1Reg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch2Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch3Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch4Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch5Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch6Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch7Reg,    TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,    TR::RealRegister::cr0,   TR_CCR, cg);
   addDependency(conditions, cnd1Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cnd2Reg,    TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cnd3Reg,    TR::RealRegister::NoReg, TR_CCR, cg);

   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   // Release registers
   cg->stopUsingRegister(tmpReg);

   return indexReg;
   }

// arraytranslateTRTOSimpleVMX --
//
// Since we know we have a simple table with maximum valid character 'max', the
// conversion process can be as follows:
//
//   uint16 input[];
//   uint8 output[];
//   int len;
//   uint16 max;
//
//   int i = 0;
//   while (i  < len) {
//       uint16 ch = input[i];
//       if (ch > max) break;
//       output[i] = ch;
//       i++;
//   }
//   return i - 1;
//
// Pseudo code:
//
//    index = 0;
//    if (len >= TRTO_MINIMUM_VMX_LEN) // 32 for now
//       {
//       // begin prealignment processing
//       if ((offs = (int)output & 15) != 0)
//          {
//          count = 16 - offs; // 1..15
//          input--;
//          output--;
//          do
//             {
//             uint16 ch = *++input;
//             if (ch > max) goto done;
//             *++output = ch;
//             index++;
//             }
//          while (--count > 0);
//          input++;
//          output++;
//          }
//       headLoopSkip:
//
//       // do vector processing
//       allocate 16-byte aligned 16-byte long storage for vmaxChars
//       fill vmaxChars with maxChars
//       count = (len - index) / 16;            // We loop at least twice
//       vin0 = vec_ld(0, input);               // may not be aligned
//       vpat = vec_lvsl(0, input);             // aligned pattern
//       do
//          {
//          vin1 = vec_ld(16, input);	        // unaligned load
//          vin2 = vec_ld(32, input);           // unaligned load
//          v0 = vec_perm(vin0, vin1, vpat);
//          v1 = vec_perm(vin1, vin2, vpat);
//          if (!vec_all_le(v0, vmaxChars)))
//              break;
//          if (!vec_all_le(v1, vmaxChars))
//              break;
//          vout = vec_pack(v0, v1);
//          vec_st(vout, 0, output);
//          vin0 = vin2;
//          index += 16;
//          increment input by a pair of vectors
//          increment output by a vector
//          }
//       while (--count > 0);
//       } // if (len >= TRTO_MINIMUM_VMX_LEN)
//    bodyLoopExit:
//
//    trailer:
//
//    count = len - index;
//    if (count > 0)
//       {
//       input--;
//       output--;
//       do
//          {
//          uint16 ch = *++input;
//          if (ch > max)
//             goto done; // or break;
//          *++output = ch;
//          index++;
//          }
//       while (--count > 0);
//       }
//    done:
//    return index - 1;
//
static TR::Register *arraytranslateTRTOSimpleVMX(TR::Node *node, TR::CodeGenerator *cg, int max)
   {
   TRACE_VMX0("@arraytranslateTRTOSimpleVMX:\n");

   // Evaluate all arguments
   TR::Node *inputNode  = node->getFirstChild();
   TR::Node *outputNode = node->getSecondChild();
   TR::Node *lenNode    = node->getChild(4);
   TR::Register *inputReg = cg->gprClobberEvaluate(inputNode);
   TR::Register *outputReg = cg->gprClobberEvaluate(outputNode);
   TR::Register *lenReg = cg->gprClobberEvaluate(lenNode);

   TR::Register *maxCharsMemReg = cg->allocateRegister();
   TR::Register *maxCharReg = cg->allocateRegister();
   TR::Register *countMinusOne = cg->allocateRegister();

   // allocate 16-byte aligned 16-byte long storage for vmaxChars
   uint16_t *vmaxCharsMemAddr = (uint16_t*)TR_Memory::jitPersistentAlloc(16 + (16 - 1));
   vmaxCharsMemAddr = (uint16_t *)(((uintptr_t)vmaxCharsMemAddr + 15) & ~15);
   // fill vmaxCharsMem with maxChars
   for (int i = 0; i < 8; i++)
      vmaxCharsMemAddr[i] = max;
   loadAddressConstant(cg, node, (intptrj_t)vmaxCharsMemAddr, maxCharsMemReg);
   loadConstant(cg, node, max, maxCharReg);

   // Build up the dependency conditions
   TR::RegisterDependencyConditions *conditions;
   conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(23, 23, cg->trMemory());
   addDependency(conditions, countMinusOne,	TR::RealRegister::gr3, TR_GPR, cg);
   addDependency(conditions, inputReg,		TR::RealRegister::gr4, TR_GPR, cg);
   addDependency(conditions, outputReg,		TR::RealRegister::gr5, TR_GPR, cg);
   addDependency(conditions, lenReg,		TR::RealRegister::gr6, TR_GPR, cg);
   addDependency(conditions, maxCharReg,	TR::RealRegister::gr7, TR_GPR, cg);
   addDependency(conditions, maxCharsMemReg,	TR::RealRegister::gr8, TR_GPR, cg);

   addDependency(conditions, NULL, 		TR::RealRegister::gr0, TR_GPR, cg);
   addDependency(conditions, NULL,		TR::RealRegister::gr9, TR_GPR, cg);
   addDependency(conditions, NULL,		TR::RealRegister::gr10, TR_GPR, cg);
   addDependency(conditions, NULL,		TR::RealRegister::gr11, TR_GPR, cg); // trampoline kills gr11
   addDependency(conditions, NULL,		TR::RealRegister::gr12, TR_GPR, cg);

   addDependency(conditions, NULL, 		TR::RealRegister::vr0, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr1, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr2, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr3, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr4, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr5, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr6, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr7, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr8, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr9, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr10, TR_VRF, cg);
   addDependency(conditions, NULL, 		TR::RealRegister::vr11, TR_VRF, cg);

   generateHelperBranchAndLinkInstruction(TR_PPCarrayTranslateTRTOSimpleVMX, node, dependencies, cg);

   conditions->stopUsingDepRegs(cg, countMinusOne);

   cg->stopUsingRegister(maxCharsMemReg);
   cg->stopUsingRegister(maxCharReg);

   cg->machine()->setLinkRegisterKilled(true);
   cg->setHasCall();
   return countMinusOne;
   }

// arraytranslateTROO --
// Input:
//   uint8 *input;
//   uint8 *output;
//   uint8 table[256];
//   int    delim;
//   int    len;
//
// Pseudo code:
//   int index = 0;
//   if (len != 0) {
//      CTR = len;
//      input--;
//      output--;
//      do {
//         uint8 ch = *++input;
//         uint8 xch = table[ch];
//         if (xch == delim)
//            break;
//         *++output = ch;
//         index++;
//      } while (--CTR != 0);
//   }
//   return index - 1;
//
static TR::Register *arraytranslateTROO(TR::Node *node, TR::CodeGenerator *cg)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTROO: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);

   // Evaluate all arguments
   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tableReg, tableReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   // Allocate GPR and misc. registers
   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblDone = generateLabelSymbol(cg);

   // If the length is zero, skip loop
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblDone, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblDone, cndReg);

   // index = 0
   loadConstant(cg, node, 0x0, indexReg);
   // Prepare for load/store update
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, inputReg, inputReg, -1);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, outputReg, outputReg, -1);
   // CTR = len
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // loop top
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 1, 1, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, tmpReg, 1, cg));
   if (termCharNodeIsHint == 0)
      {
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, tmpReg, delimReg);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblDone, cndReg);
      }
   generateMemSrc1Instruction(cg, TR::InstOpCode::stbu, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 1, 1, cg), tmpReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblDone);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, -1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(cndReg);

   return indexReg;
   }

// arraytranslate TRTT --
//
// Input:
//   uint16 *input;
//   uint16 *output;
//   uint16 table[65536];
//   int    delim;
//   int    len;
//
// Pseudo code:
//   int index = 0;
//   if (len != 0) {
//      CTR = len;
//      input--;
//      output--;
//      do {
//         uint16 ch = *++input;
//         uint16 xch = table[ch];
//         if (xch == delim)
//            break;
//         *++output = ch;
//         index++;
//      } while (--CTR != 0);
//   }
//   return index - 1;
//
static TR::Register *arraytranslateTRTT(TR::Node *node, TR::CodeGenerator *cg)
   {
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();
   TRACE_VMX2("@arraytranslateTRTT: termCharNodeIsHint=%d sourceCellIsTermChar=%d\n", termCharNodeIsHint, sourceCellIsTermChar);

   // Evaluate all arguments
   TR::Register *inputReg  = cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *outputReg = cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *tableReg  = cg->gprClobberEvaluate(node->getChild(2));
   TR::Register *delimReg  = cg->gprClobberEvaluate(node->getChild(3));
   TR::Register *lenReg    = cg->gprClobberEvaluate(node->getChild(4));
   if (!node->getTableBackedByRawStorage())
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tableReg, tableReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());
      }

   // Allocate GPR and misc. registers
   TR::Register *indexReg  = cg->allocateRegister();
   TR::Register *tmpReg    = cg->allocateRegister();
   TR::Register *cndReg    = cg->allocateRegister(TR_CCR);

   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblDone = generateLabelSymbol(cg);

   // If the length is zero, skip loop
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, PPCOpProp_BranchUnlikely, node, lblDone, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblDone, cndReg);

   // index = 0
   loadConstant(cg, node, 0x0, indexReg);
   // Prepare for load/store update
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, inputReg, inputReg, -2);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, outputReg, outputReg, -2);
   // CTR = len
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // loop top
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lhzu, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(inputReg, 2, 2, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lhzx, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, tmpReg, 2, cg));
   if (termCharNodeIsHint == 0)
      {
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, tmpReg, delimReg);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblDone, cndReg);
      }
   generateMemSrc1Instruction(cg, TR::InstOpCode::sthu, node, new (cg->trHeapMemory()) TR::MemoryReference(outputReg, 2, 2, cg), tmpReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblDone);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, -1);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
   // arguments
   addDependency(conditions, inputReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, outputReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,    TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   addDependency(conditions, indexReg,  TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg); // discard all associated registers except indexReg

   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(cndReg);

   return indexReg;
   }

// analyzeTRTOTable -- Determine if the given table is simple or not
//
// For given 8-bit wide and 16-bit depth table, the table is 'simple' iff
// there exist a number 'max' such that:
//
//   - 0 <= max <= 0xffff
//   - table[i] = (i & 0xff) for i=0..max
//   - table[max+1..0xffff] are all filled with the delimiter character
//   - table is TR::loadaddr (table is a compile time constant)
//   - delim is a constant
//
// Returns:
//   - 'max' if the table is simple, or
//   - if the table is not simple or can not be determined at compile, a -1 will be returned
//
static int analyzeTRTOTable(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *inputNode  = node->getFirstChild();
   TR::Node *outputNode = node->getSecondChild();
   TR::Node *tableNode  = node->getChild(2);
   TR::Node *delimNode  = node->getChild(3);
   TR::Node *lenNode    = node->getChild(4);

   TRACE_VMX0("@analyzeTRTOTable: called\n");
   if (tableNode->getOpCodeValue() != TR::loadaddr)
      {
      TRACE_VMX0("@analyzeTRTOTable: Probably user supplied table\n");
      return -1;	// The table may be user supplied one
      }
   if (!(delimNode->getOpCodeValue() == TR::iconst || delimNode->getOpCodeValue() == TR::lconst))
      {
      TRACE_VMX0("@analyzeTRTOTable: delimiter is not a constant\n");
      return -1;
      }
   int termChar = delimNode->getInt(); // TODO: handle 64bit
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();

   TRACE_VMX3("@analyzeTRTOTable: termChar=0x%04x termCharNodeIsHint=%d sourceCellIsTermChar=%d\n",
          termChar, termCharNodeIsHint, sourceCellIsTermChar);
#if 1 // Need confirmation with Toronto whether this is a right way
   if (termChar >= 0x100)
      {
      TRACE_VMX1("@analyzeTRTOTable: termChar=0x%04x is greater than 0xff. Forced as follows:\n", termChar);
      termChar &= 0xff;
      termCharNodeIsHint = true;
      sourceCellIsTermChar = true;
      TRACE_VMX3("@analyzeTRTOTable: termChar=0x%04x termCharNodeIsHint=%d sourceCellIsTermChar=%d\n",
             termChar, termCharNodeIsHint, sourceCellIsTermChar);
      }
#endif
   TR_ASSERT((termCharNodeIsHint == true && sourceCellIsTermChar == false) ||
          (termChar >= 0 && termChar <= 0xffff), "termChar out of range");

   uint8_t *trto_tab = (uint8_t *)
      (tableNode->getSymbolReference()->getSymbol()->getStaticSymbol()->getStaticAddress());

#define IS_TRTO_DELIM(tab, idx, _termChar, _termCharNodeIsHint, _sourceCellIsTermChar) \
         (tab[idx] == termChar && \
          (_termCharNodeIsHint == false || \
           _termCharNodeIsHint == true && _sourceCellIsTermChar == true && idx != _termChar))

   // Find the first delimiter in [1..0xffff]
   int i;
   for (i = 0x0000; i <= 0xffff; i++)
      if (IS_TRTO_DELIM(trto_tab, i, termChar, termCharNodeIsHint, sourceCellIsTermChar) == true)
         break;
   int firstDelim = i; // Can be 0, 1..0xffff, or 0x10000
   if (firstDelim == 0) // The first character in the table is a delimiter
      {
      TRACE_VMX0("@analyzeTRTOTable: Not simple because the first character is a delimiter\n");
      return -1;
      }
   TRACE_VMX1("@analyzeTRTOTable: firstDelim=0x%x\n", firstDelim);

   // Confirm there is no standard character afther the first delimiter
   int firstNonStandardChar = -1;
   for (i = firstDelim; i <= 0xffff; i++)
      if (IS_TRTO_DELIM(trto_tab, i, termChar, termCharNodeIsHint, sourceCellIsTermChar) == false)
         {
         TRACE_VMX1("@analyzeTRTOTable: Found standard character after delimiter. 0x%x\n", i);
         for (int j = 0; j < 0x110; j++)
            {
            if ((j & 15) == 0)
               TRACE_VMX1("%04x: ", j);
            TRACE_VMX1("%02x ", trto_tab[j]);
            if ((j & 15) == 15)
               TRACE_VMX0("\n");
            }
         return -1;
         }

   // Confirm that all standard characters are mapped into (ch & 0xff)
   for (i = 0; i <= firstDelim - 1; i++)
      if (trto_tab[i] != (i & 0xff))
         {
         TRACE_VMX2("@analyzeTRTOTable: ch 0x%x is mapped to %x\n", i, trto_tab[i]);
         return -1;
         }
   TRACE_VMX1("@analyzeTRTOTable: returning max=0x%x\n", firstDelim - 1);
   return firstDelim - 1; // 0 to 0xffff
   }

// analyzeTROTTable -- Determine if the given table is simple or not
//
// For given 16-bit wide and 8-bit depth table, the table is 'simple' iff
// there exist a number 'max' such that:
//
//   - 0 <= max <= 0xff
//   - table[i] = (i & 0xff) for i=0..max
//   - table[max+1..0xffff] are all filled with the delimiter character
//   - table is TR::loadaddr (table is a compile time constant)
//   - delim is a constant
//
// Returns:
//   - 'max' if the table is simple, or
//   - if the table is not simple or can not be determined at compile, a -1 will be returned
//
static int analyzeTROTTable(TR::Node *node, TR::CodeGenerator *cg)
   {
   TR::Node *inputNode  = node->getFirstChild();
   TR::Node *outputNode = node->getSecondChild();
   TR::Node *tableNode  = node->getChild(2);
   TR::Node *delimNode  = node->getChild(3);
   TR::Node *lenNode    = node->getChild(4);

   TRACE_VMX0("@analyzeTROTTable: called\n");
   if (tableNode->getOpCodeValue() != TR::loadaddr)
      {
      TRACE_VMX0("@analyzeTROTTable: Probably user supplied table\n");
      return -1;	// The table may be user supplied one
      }
   if (!(delimNode->getOpCodeValue() == TR::iconst || delimNode->getOpCodeValue() == TR::lconst))
      {
      TRACE_VMX0("@analyzeTROTTable: delimiter is not a constant\n");
      return -1;
      }
   int termChar = delimNode->getInt(); // TODO: handle 64bit
   int termCharNodeIsHint = node->getTermCharNodeIsHint();
   int sourceCellIsTermChar = node->getSourceCellIsTermChar();

   TRACE_VMX3("@analyzeTROTTable: termChar=0x%04x termCharNodeIsHint=%d sourceCellIsTermChar=%d\n",
          termChar, termCharNodeIsHint, sourceCellIsTermChar);
   TR_ASSERT((termCharNodeIsHint == true && sourceCellIsTermChar == false) ||
          (termChar >= 0 && termChar <= 0xffff), "termChar out of range");

   uint16_t *trot_tab = (uint16_t *)
      (tableNode->getSymbolReference()->getSymbol()->getStaticSymbol()->getStaticAddress());

#define IS_TROT_DELIM(tab, idx, _termChar, _termCharNodeIsHint, _sourceCellIsTermChar) \
         (tab[idx] == termChar && \
          (_termCharNodeIsHint == false || \
           _termCharNodeIsHint == true && _sourceCellIsTermChar == true && idx != _termChar))

   // Find the first delimiter in [1..0xff]
   int i;
   for (i = 0; i <= 0xff; i++)
      if (IS_TROT_DELIM(trot_tab, i, termChar, termCharNodeIsHint, sourceCellIsTermChar) == true)
         break;
   int firstDelim = i; // Can be 0, 1..0xff, or 0x100
   if (firstDelim == 0) // The first character in the table is a delimiter
      {
      TRACE_VMX0("@analyzeTROTTable: Not simple because the first character is a delimiter\n");
      return -1;
      }
   TRACE_VMX1("@analyzeTROTTable: firstDelim=0x%x\n", firstDelim);

   // Confirm there is no standard character afther the first delimiter
   int firstNonStandardChar = -1;
   for (i = firstDelim; i <= 0xff; i++)
      if (IS_TROT_DELIM(trot_tab, i, termChar, termCharNodeIsHint, sourceCellIsTermChar) == false)
         {
         TRACE_VMX1("@analyzeTROTTable: Found standard character after delimiter. 0x%x\n", i);
         for (int j = 0; j < 0x100; j++)
            {
            if ((j & 15) == 0)
               TRACE_VMX1("%04x: ", j);
            TRACE_VMX1("%04x ", trot_tab[j]);
            if ((j & 15) == 15)
               TRACE_VMX0("\n");
            }
         return -1;
         }

   // Confirm that all standard characters are mapped into (ch & 0xff)
   for (i = 0; i <= firstDelim - 1; i++)
      if (trot_tab[i] != (i & 0xff))
         {
         TRACE_VMX2("@analyzeTROTTable: ch 0x%x is mapped to %x\n", i, trot_tab[i]);
         return -1;
         }
   TRACE_VMX1("@analyzeTROTTable: returning max=0x%x\n", firstDelim - 1);
   return firstDelim - 1; // 0 to 0xff
   }
#endif

//----------------------------------------------------------------------
// arraytranslateAndTest
//----------------------------------------------------------------------
#if defined(TR_IDIOM_PPCVMX)
static TR::Register *findBytesGenSRSTScalar(TR::Node * node, TR::CodeGenerator * cg);
static TR::Register *findBytesGenSRSTVMX(TR::Node * node, TR::CodeGenerator * cg);
static TR::Register *findBytesGenTRTCom(TR::Node * node, TR::CodeGenerator * cg);
static TR::Register *arraytranslateAndTestGenScalar(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraytranslateAndTestGenVMX(TR::Node *node, TR::CodeGenerator *cg);
#endif

TR::Register *OMR::Power::TreeEvaluator::arraytranslateAndTestEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
#if defined(TR_IDIOM_PPCVMX)
   TRACE_VMX3("@arraytranslateAndTestEvaluator: node=%p isArrayTRT=%d isCharArrayTRT=%d\n", node, node->isArrayTRT(), node->isCharArrayTRT());
   TR::Node *tableNode	= node->getChild(2);
   bool useTRT = tableNode->getDataType() == TR::Address;
   TR::Register *retReg;

   if (!node->isArrayTRT())
      {
      // not findBytes
      // tree looks as follows:
      // arraytranslateAndTest
      //    input node
      //    stop byte
      //    input length (in bytes)
      //
      // Right now, this is only 1 style of tree - just for a simple 'test'
      //  that consists of only one byte
      if (VMXEnabled(cg))
         retReg = arraytranslateAndTestGenVMX(node, cg);
      else
         retReg = arraytranslateAndTestGenScalar(node, cg);
      node->setRegister(retReg);
      cg->decReferenceCount(node->getFirstChild());
      cg->decReferenceCount(node->getSecondChild());
      cg->decReferenceCount(node->getChild(2));
      }
   else
      {
      if (useTRT) // `tableNode' is a table
         {
         //XXX Must be separated into Scalar and VMX versions
         retReg = findBytesGenTRTCom(node, cg);
         }
      else // `tableNode' is a scalar
         if (VMXEnabled(cg))
            retReg = findBytesGenSRSTVMX(node, cg);
         else
            retReg = findBytesGenSRSTScalar(node, cg);
      node->setRegister(retReg);
      cg->decReferenceCount(node->getFirstChild());
      cg->decReferenceCount(node->getSecondChild());
      cg->decReferenceCount(node->getChild(2));
      cg->decReferenceCount(node->getChild(3));
      if (node->getNumChildren() == 5)
         cg->decReferenceCount(node->getChild(4)); // If we've been given "len" argument
      }

   return retReg;
#else
   return NULL;
#endif
   }

#if defined(TR_IDIOM_PPCVMX)
// emulate SRST using PPC scalar code
//
// pseudo code of an unrolled loop:
//
//   i = 0;
//   if (alen >= 4)
//      {
//      CTR = alen / 4;
//      chr = data[i]; // preload into register
//      do
//      {
//         if (chr == delimiter) return i;
//         if (data[i+1] == delimiter) return i+1;
//         if (data[i+2] == delimiter) return i+2;
//         if (data[i+3] == delimiter) return i+3;
//         chr = data[i+4];
//      } while (--CTR > 0);
//   residue = alen - i;
//
//
static TR::Register *arraytranslateAndTestGenScalar(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX1("@arraytranslateAndTestGenScalar: node=%p\n", node);

   int i;
   TR::Register *baseReg		= cg->gprClobberEvaluate(node->getFirstChild());		// address
   TR::Register *delmReg		= cg->gprClobberEvaluate(node->getSecondChild());	// delimiter
   TR::Register *lmtReg		= cg->gprClobberEvaluate(node->getChild(2));     	// length

   TR::Register *indexReg	= cg->allocateRegister();
   TR::Register *charReg		= cg->allocateRegister();
   TR::Register *addrReg		= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *char1Reg	= cg->allocateRegister();
   TR::Register *char2Reg	= cg->allocateRegister();
   TR::Register *char3Reg	= cg->allocateRegister();
   TR::Register *tmpReg		= cg->allocateRegister();

   TR::LabelSymbol *lblHead	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblHeadTrailer	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNoTrailer	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNoUnrolledLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFound	= generateLabelSymbol(cg);

   loadConstant(cg, node, 0x0, indexReg);

   // initialize counter register by iteration count
   // CTR = alen / 4
   // Check if we have at least one unrolled-loop iteration
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lmtReg, 4);
   // Skip the loop, if not
   generateConditionalBranchInstruction(cg, TR::InstOpCode::blt, node, lblNoUnrolledLoop, cndReg);

   // Setup loop count
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, lmtReg, 32 - 2, 0x3fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);

   // preload one byte
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, 0, 1, cg));
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, addrReg, indexReg, baseReg);

   generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, addrReg, baseReg);

   // loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char1Reg, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 1, 1, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char2Reg, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 2, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, charReg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char3Reg, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 3, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, char1Reg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);

   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, char2Reg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(addrReg, 4, 1, cg));//prefetch
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, addrReg, addrReg, 4);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);

   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, char3Reg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);

   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNoUnrolledLoop);

   // Trailer processing
   //    for (; i < alen; i++)
   //       if (data[i] == delimiter) return i;

   // Check if we have any data to process, and skip if none remining
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lmtReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblNoTrailer, cndReg);

   // Setup loop count
   // CTR = alen - index
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, indexReg, lmtReg);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);

   // loop body
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadTrailer);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, charReg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadTrailer, cndReg);
   // end loop body

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNoTrailer);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFound);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
   // arguments
   addDependency(conditions, baseReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delmReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lmtReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   // variables
   addDependency(conditions, indexReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, addrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, charReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->stopUsingRegister(cndReg);
   cg->stopUsingRegister(char1Reg);
   cg->stopUsingRegister(char2Reg);
   cg->stopUsingRegister(char3Reg);
   cg->stopUsingRegister(tmpReg);

   return indexReg;
   }

static TR::Register *arraytranslateAndTestGenVMX(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX1("@arraytranslateAndTestGenVMX: node=%p\n", node);
   TR_ASSERT(node->isArrayTRT(), "isArrayTRT() is true");

   int i;
   TR::Register *arrayReg	= cg->gprClobberEvaluate(node->getFirstChild()); // address
   TR::Node *delmNode		= node->getChild(1);
   TR::Register *delmReg		= cg->gprClobberEvaluate(delmNode);               // delimiter
   TR::Register *lmtReg		= cg->gprClobberEvaluate(node->getChild(2));       // length

   TR::Register *indexReg	= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *charReg		= cg->allocateRegister();
   TR::Register *lmt4Reg		= cg->allocateRegister();
   TR::LabelSymbol *lblHead	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead2	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblHeadSca	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec16	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec32	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec48	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFind1	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFind	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblScaLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNounrollLoop	= generateLabelSymbol(cg);

   loadConstant(cg, node, 0x0, indexReg);

   // emulate SRST with VMX
   // puts("arraytranslateAndTest by VMX");
   TR::Register *vtab0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vec0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vec1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vpatReg = cg->allocateRegister(TR_VRF);
   TR::Register *vcharsReg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars2Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars3Reg = cg->allocateRegister(TR_VRF);

   TR::Register *vconst0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vconstMaskReg = cg->allocateRegister(TR_VRF);
   TR::Register *vconstMergeReg = cg->allocateRegister(TR_VRF);
   TR::Register *const0Reg = cg->allocateRegister();
   TR::Register *const16Reg = cg->allocateRegister();
   TR::Register *const32Reg = cg->allocateRegister();
   TR::Register *const48Reg = cg->allocateRegister();
   TR::Register *const64Reg = cg->allocateRegister();

   TR::Register *vresultReg = cg->allocateRegister(TR_VRF);
   TR::Register *cr6Reg = cg->allocateRegister(TR_CCR);
   TR::Register *tmpReg = cg->allocateRegister();
   TR::Register *tmp2Reg = cg->allocateRegister();
   TR::Register *constTableReg = cg->allocateRegister();
   TR::Register *lmtvReg = cg->allocateRegister();
   TR::Register *lmtv4Reg = cg->allocateRegister();

   loadConstant(cg, node,  0, indexReg);

   // if target array is too small for VMX
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lmtReg, 32);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, false, node, lblScaLoop, cndReg);

   // return immediately if the 1st charcter is a delimiter
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, indexReg, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, charReg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, false, node, lblFind, cndReg);

   // prepare constants
   loadConstant(cg, node,  0, const0Reg);
   loadConstant(cg, node, 16, const16Reg);
   loadConstant(cg, node, 32, const32Reg);
   loadConstant(cg, node, 48, const48Reg);
   loadConstant(cg, node, 64, const64Reg);

   // load first two vectors from input array
   //generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmp2Reg, arrayReg, indexReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpatReg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vec0Reg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, const16Reg, 16, cg));
   // prepare simple vector constants
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0Reg, 0);

   // prepare complex constants in memory
   uint32_t *vconstTable = (uint32_t*)TR_Memory::jitPersistentAlloc(16*sizeof(uint32_t) + (16 - 1));
   vconstTable = (uint32_t *)(((((uintptr_t)vconstTable-1) >> 4) + 1) << 4); // align to 16-byte boundary
   loadAddressConstant(cg, node, (intptrj_t)vconstTable, constTableReg);
   for (i=0; i<16; i++) { ((uint8_t *)vconstTable)[i] = delmNode->getInt(); }
   for (i=4; i<8; i++) vconstTable[i] = 0x80200802;
   vconstTable[8] = 0x03070B0F;
   for (i=9; i<12; i++) vconstTable[i] = 0;

   // load prepared constants into vector registers
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab0Reg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMaskReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const16Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMergeReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const32Reg, 16, cg));

   // test the first 16 charcters
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vec1Reg, vec0Reg, vcharsReg, vpatReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vec1Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);

   /*
   pseudo code of an unrolled loop:

   vchars_1 = vdata[i/16]; // preload into vector register
   for (; i < limit-16*5; i += 16*4)  // process 64 chacters in one loop (16 byte * 4 vector)
      {
      if (vec_any_eq(vchars_1, vdelimiter))
         return find_position_in_vector();

      vchars_2 = vdata[i/16+1];
      if (vec_any_eq(vchars_2, vdelimiter))
         return find_position_in_vector();

      ....

      if (vec_any_eq(vchars_3, vdelimiter))
         return find_position_in_vector();

      vchars_1 = vdata[i/16+4];
      }
   */

   // force index align to 128-bit boundary
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tmpReg, arrayReg, 0xF);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, tmpReg, const16Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);

   // initialize counter register by iteration count
   // CTR = (limit - 16 * 5 - index) >> 6
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lmtv4Reg, lmtReg, -16*5);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, lmtv4Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lmtv4Reg);
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, lmtvReg, lmtvReg, 6);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lmtvReg, lmtvReg, 32 - 6, 0x03ffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);

   // break if index is equal or larger than limit
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblNounrollLoop, cndReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmpReg, tmpReg, arrayReg);

   // FIXME Needs work

   // loop body, see an above pseudo code
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars1Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const16Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vcharsReg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars2Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const32Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars1Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec16, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars3Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const48Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars2Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec32, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const64Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars3Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec48, cr6Reg);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16*4);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmpReg, tmpReg, 16*4);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body

   /*
   pseudo code of a loop without unrolling:

   for (; i < limit-16; i += 16) // process 16 chacters in one loop
      {
      vchars_1 = vdata[i/16];
      if (vec_any_gt(vchars_1, vdelimiter))
         return find_position_in_vector();
      }
   */
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNounrollLoop);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, lmtReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lmtvReg, 15);
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, lmtvReg, lmtvReg, 4);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lmtvReg, lmtvReg, 32 - 4, 0x0fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblScaLoop, cndReg);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead2);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, indexReg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vcharsReg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead2, cndReg);

   /*
   pseudo code of a non-VMX loop:

   for (; i < limit; i++)
      if (data[i] == delimiter) return i;
   */
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblScaLoop);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lmtReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, lmtReg);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblExit, cndReg);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadSca);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(arrayReg, indexReg, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, charReg, delmReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFind, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadSca, cndReg);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFind);

   // found delimiter
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec);

   // gather bits from each charcter to determine the position
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vand, node, vresultReg, vresultReg, vconstMaskReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vsum4sbs, node, vresultReg, vresultReg, vconst0Reg);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vresultReg, vresultReg, vresultReg, vconstMergeReg);

   // move data from vector register to GPR
   generateMemSrc1Instruction(cg, TR::InstOpCode::stvx, node, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const48Reg, 16, cg), vresultReg);
   // insert three no-ops to enforce load and store be in other dispatch groups
   if (TR::Compiler->target.cpu.id() >= TR_PPCgp)
      {
      cg->generateGroupEndingNop(node);
      }
   generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, 48, 4, cg));

   // determine a position of the delimiter by using coun-leading-zeros instruction
   generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, tmpReg);
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmpReg, tmpReg, 1);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 1, 0x7fffffff);

   // make index register to point an address of the found delimiter
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFind);

   // found a delimiter in the 2nd vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec16);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

   // found a delimiter in the 3rd vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec32);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 32);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

   // found a delimiter in the 4th vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec48);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 48);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFind1);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   // fall through

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFind);

   // Register dependencies
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(21, 21, cg->trMemory());
   // arguments
   addDependency(conditions, arrayReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delmReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lmtReg, TR::RealRegister::NoReg, TR_GPR, cg);
   // constants
   addDependency(conditions, const0Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const16Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const32Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const48Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const64Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, vconst0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconstMaskReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconstMergeReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vtab0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   // variables
   addDependency(conditions, indexReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, vpatReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, cr6Reg, TR::RealRegister::cr6, TR_CCR, cg);
   // commit
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->stopUsingRegister(cndReg);
   cg->stopUsingRegister(charReg);
   cg->stopUsingRegister(lmt4Reg);
   cg->stopUsingRegister(vec0Reg);
   cg->stopUsingRegister(vec1Reg);
   cg->stopUsingRegister(vcharsReg);
   cg->stopUsingRegister(vchars1Reg);
   cg->stopUsingRegister(vchars2Reg);
   cg->stopUsingRegister(vchars3Reg);
   cg->stopUsingRegister(vresultReg);
   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(tmp2Reg);
   cg->stopUsingRegister(constTableReg);
   cg->stopUsingRegister(lmtvReg);
   cg->stopUsingRegister(lmtv4Reg);

   return indexReg;
   }
#endif

//----------------------------------------------------------------------
// arraycmp
//----------------------------------------------------------------------

// arraycmpEvaluator
//
// This IL has two variants: One is arraycmp and the other is arraycmplen.
//
// arraycmp takes two byte arrays and length.
// length is a 32bit signed integer and can be zero or negative number.
// If so, the it must be treated if one is specified.
// arraycmp returns one if two arrays are identical, otherwise zero is returned.

// arraycmplen takes two byte arrays and length.
// Again, length must be treated as one if it is zero or negative.
// arraycmplen Returns the length if two arrays are identical.
// Otherwise, returns the offset of the first unmatch bytes.
//
// Pseudo code of isArrayCmpLen()==0 case
//
//      byte src0[], src1[];
//      int len;
//      len = len <= 0 ? 1 : len;
//      if (len == 0) return 0;
//      for (i = 0; i < len; i++) {
//        if (src0[i] != src1[i])
//          return 0;
//      }
//      return 1;
//
// Pseudo code of isArrayCmpLen() != 0 case
//
//      byte src0[], src1[];
//      int len;
//      len = len <= 0 ? 1 : len;
//      for (i = 0; i < len; i++) {
//        if (src0[i] != src1[i])
//          break;
//      }
//      return i;

#if defined(TR_IDIOM_PPCVMX)
static TR::Register *arraycmpGeneratorScalar(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraycmpLenGeneratorScalar(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraycmpCommGeneratorVMXRuntime(TR::Node *node, TR::CodeGenerator *cg, int returnsLength, int versionLimit);

static TR::Register *arraycmpGeneratorVMXInline(TR::Node *node, TR::CodeGenerator *cg);
static TR::Register *arraycmpLenGeneratorVMXInline(TR::Node *node, TR::CodeGenerator *cg);


TR::Register *OMR::Power::TreeEvaluator::arraycmpEvaluator(TR::Node *node, TR::CodeGenerator *cg)
   {
   //#if defined(TR_IDIOM_PPCVMX)
   TRACE_VMX2("@arraycmpEvaluator: node=%p retLen=%d\n", node, node->isArrayCmpLen());
   TR_ASSERT(!node->isArrayCmpLen() || !node->isArrayCmpSign(), "error. isArrayCmpLen and isArrayCmpSign can not be set simultaneously");
   TR_ASSERT(!node->isArrayCmpSign(), "error. isArrayCmpSign must be false"); // Temporary for PPC

   TR::Node *src0Node = node->getFirstChild();
   TR::Node *src1Node = node->getSecondChild();
   TR::Node *lenNode = node->getChild(2);
   TR::Register *retReg;

   int versionLimit = getVersionLimit(cg);
   int arraycmpUseVMXRuntime = getArraycmpUseVMXRuntime(cg);
   TRACE_VMX2("@arraycmp versionLimit=%d, useVMXRuntime=%d\n", versionLimit, arraycmpUseVMXRuntime);

   if (VMXEnabled(cg))
      if (arraycmpUseVMXRuntime == 0)
         if (node->isArrayCmpLen() == 0)
            retReg = arraycmpGeneratorVMXInline(node, cg);
         else
            retReg = arraycmpLenGeneratorVMXInline(node, cg);
      else
         if (node->isArrayCmpLen() == 0)
            retReg = arraycmpCommGeneratorVMXRuntime(node, cg, 0, versionLimit);
         else
            retReg = arraycmpCommGeneratorVMXRuntime(node, cg, 1, versionLimit);
   else
      if (node->isArrayCmpLen() == 0)
         retReg = arraycmpGeneratorScalar(node, cg);
      else
         retReg = arraycmpLenGeneratorScalar(node, cg);

   cg->decReferenceCount(src0Node);
   cg->decReferenceCount(src1Node);
   cg->decReferenceCount(lenNode);
   node->setRegister(retReg);
   return retReg;
   //#else
   traceMsg(cg->comp(), "Reached vmx arraycmp evaluator\n");
   return NULL;
   //#endif
   }

#endif


#if defined(TR_IDIOM_PPCVMX)
// arraycmpGeneratorScalar --
//
// Code is inlined.
//
//
// Generated code:
//      entry:
//		li	index,0
//              LD	ch0,1(src0)
//              LD	ch1,1(src1)
//              cmpw	ch0,ch1
//              bne	exit
//              cmpwi	len,ELEM_SIZE
//              ble	match
//	#if ELEM_IS_1BYTE
//	#else if ELEM_IS_2BYTE
//		rlwinm	len,len,32-1,0x7fffffff
//	#else if ELEM_IS_4BYTE
//		rlwinm	len,len,32-2,0x3fffffff
//	#endif
//              mtctr   len	// Positive number
//		addi	src0,src0,-ELEM_SIZE
//		addi	src1,src1,-ELEM_SIZE
//      loop:
//              cmpw	ch0,ch1
//              LDU	ch0,1(src0)
//              LDU	ch1,1(src1)
//              bne	exit
//              bdnz	loop
//              li      index,1
//		b	exit
//      match:
//              li      ret,1
//      exit:
static TR::Register *arraycmpGeneratorScalar(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraycmpGeneratorScalar:\n");

   // Evaluate all arguments
   TR::Register *src0Reg	= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *src1Reg	= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg	= cg->gprClobberEvaluate(node->getChild(2));

   // Allocate GPR and misc. registers
   TR::Register *indexReg = cg->allocateRegister();
   TR::Register *ch0Reg	= cg->allocateRegister();
   TR::Register *ch1Reg	= cg->allocateRegister();
   TR::Register *cndReg  = cg->allocateRegister(TR_CCR);

   // Allocate labels
   TR::LabelSymbol *lblFixLen = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblMatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);

   TR::InstOpCode::Mnemonic loadElemOp;
   TR::InstOpCode::Mnemonic loadElemUpdateOp;
   // We assume that the length (in byte) is a multiple of elemSize
   int32_t elemSize = arraycmpEstimateElementSize(node->getChild(2), cg);
   TRACE_VMX1("@elemSize=%d\n", elemSize);

   switch (elemSize)
      {
      case 0:
      case 1:
         elemSize = 1;
         loadElemOp = TR::InstOpCode::lbz;
         loadElemUpdateOp = TR::InstOpCode::lbzu;
         break;
      case 2:
         loadElemOp = TR::InstOpCode::lhz;
         loadElemUpdateOp = TR::InstOpCode::lhzu;
         break;
      case 4:
      case 8:
         elemSize = 4;
         loadElemOp = TR::InstOpCode::lwz;
         loadElemUpdateOp = TR::InstOpCode::lwzu;
         break;
      }

   loadConstant(cg, node, 0, indexReg);

   // Peel the first element.
   // We can safely do it  here since the loop repeats at least once regardless of length parameter
   generateTrg1MemInstruction(cg, loadElemOp, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 0, elemSize, cg));
   generateTrg1MemInstruction(cg, loadElemOp, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 0, elemSize, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblExit, cndReg);	// unmatch
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, elemSize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblMatch, cndReg); // match

   // Convert byte count into element count
   switch (elemSize)
      {
      case 1:
         break;
      case 2:
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 32-1, 0x7fffffff);
         break;
      case 4:
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 32-2, 0x3fffffff);
         break;
      }

   // CTR = loop count (>= 1)
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // Head
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   // ch0 == ch1 ? (executed twice for the 1st element)
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   // Prefetch ch0 and ch1
   generateTrg1MemInstruction(cg, loadElemUpdateOp, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, elemSize*1, elemSize, cg));
   generateTrg1MemInstruction(cg, loadElemUpdateOp, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, elemSize*1, elemSize, cg));
   // If not match, goto unmatch
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblExit, cndReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblMatch);
   // Match:
   // index = 1
   loadConstant(cg, node, 1, indexReg);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);

   // Exit
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);

   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
   addDependency(conditions, src0Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, src1Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);
   cg->stopUsingRegister(cndReg);
   return indexReg;
   }

static TR::Register *arraycmpLenGeneratorScalar(TR::Node *node, TR::CodeGenerator *cg)
      {
   TRACE_VMX0("@arraycmpLenGeneratorScalar:\n");

   // Evaluate all arguments
   TR::Register *src0Reg	= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *src1Reg	= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg	= cg->gprClobberEvaluate(node->getChild(2));

   // Allocate GPR and misc. registers
   TR::Register *indexReg = cg->allocateRegister();
   TR::Register *ch0Reg	= cg->allocateRegister();
   TR::Register *ch1Reg	= cg->allocateRegister();
   TR::Register *cndReg	= cg->allocateRegister(TR_CCR);

   // Allocate labels
   TR::LabelSymbol *lblFixLen = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblUnmatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);

   // If byte length is zero or negative, treat it as 1.
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, PPCOpProp_BranchLikely, node, lblFixLen, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, node, lblFixLen, cndReg);
   loadConstant(cg, node, 1, lenReg);
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFixLen);

   // CTR = loop count (>= 1)
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // ret = 0
   loadConstant(cg, node, 0, indexReg);

   // Prefetch ch0 and ch1
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 0, 1, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 0, 1, cg));

   // Head
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   // ch0 == ch1 ?
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   // Prefetch ch0 and ch1
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 1, 1, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 1, 1, cg));
   // If not match, break loop
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblUnmatch, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 0x1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblUnmatch);
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);

   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(6, 6, cg->trMemory());
   addDependency(conditions, src0Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, src1Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch0Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, ch1Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);
   cg->stopUsingRegister(cndReg);
   return indexReg;
      }

// arraycmpCommGeneratorVMXRuntime --
//
// Generated code:
//
//      entry:
//      #if CALL_RUNTIME
//              cmpwi	len,VERSION_LIMIT
//              bge	callRuntime
//      #endif
//              li	index,0
//              cmpwi	len,0
//              ble-	exit
//              mtctr   len
//		addi	src0,src0,-1
//		addi	src1,src1,-1
//      loop:
//              lbzu	ch0,1(src0)
//              lbzu	ch1,1(src1)
//              cmpw	ch0,ch1
//              bne	unmatch
//              addi	index,index,1
//              bdnz	loop
//      #if RETURNS_LEN
//      unmatch:
//		b	exit
//      #else
//              li	index,1
//		b	exit
//      unmatch:
//              li	index,0
//      #endif
//
//      #if CALL_RUNTIME
//		b	exit
//      callRuntime:
//		// call runtime
//      #endif
//      exit:
//
static TR::Register *arraycmpCommGeneratorVMXRuntime(TR::Node *node, TR::CodeGenerator *cg, int returnsLength, int versionLimit)
      {
   TRACE_VMX2("@arraycmpCommGeneratorVMXRuntime: returnsLength=%d vesionLimit=%d\n",
          returnsLength, versionLimit);

   // Evaluate all arguments
   TR::Register *src0Reg	= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *src1Reg	= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg	= cg->gprClobberEvaluate(node->getChild(2));

   // Allocate GPR and misc. registers
   TR::Register *indexReg = cg->allocateRegister();
   TR::Register *ch0Reg	= cg->allocateRegister();
   TR::Register *ch1Reg	= cg->allocateRegister();
   TR::Register *cndReg	= cg->allocateRegister(TR_CCR);
   TR::Register *cr6Reg	= NULL;
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(23, 23, cg->trMemory());
   if (versionLimit > 0) // When using VMX runtime
         {
      addDependency(conditions, src0Reg,	TR::RealRegister::gr4, TR_GPR, cg);
      addDependency(conditions, src1Reg,	TR::RealRegister::gr5, TR_GPR, cg);
      addDependency(conditions, lenReg,		TR::RealRegister::gr6, TR_GPR, cg);

      addDependency(conditions, indexReg,	TR::RealRegister::gr3, TR_GPR, cg);
      addDependency(conditions, cndReg, 	TR::RealRegister::cr0, TR_CCR, cg);

      addDependency(conditions, NULL,		TR::RealRegister::gr7, TR_GPR, cg);
      addDependency(conditions, NULL,		TR::RealRegister::gr8, TR_GPR, cg);
      addDependency(conditions, NULL,		TR::RealRegister::gr9, TR_GPR, cg);
      addDependency(conditions, NULL,		TR::RealRegister::gr11,TR_GPR, cg); // trampoline kills gr11
      addDependency(conditions, NULL, 		TR::RealRegister::vr0, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr1, TR_VRF, cg);
      //addDependency(conditions, NULL, 		TR::RealRegister::vr2, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr3, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr4, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr5, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr6, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr7, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr8, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr9, TR_VRF, cg);
      addDependency(conditions, NULL, 		TR::RealRegister::vr10,TR_VRF, cg);
         }
   else
      {
      addDependency(conditions, src0Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, src1Reg, 	TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, lenReg,        	TR::RealRegister::NoReg, TR_GPR, cg);

      addDependency(conditions, indexReg,      	TR::RealRegister::NoReg, TR_GPR, cg);
      //addDependency(conditions, cndReg, 	TR::RealRegister::cr0,   TR_CCR, cg);
      }

   // Allocate labels
   TR::LabelSymbol *lblFixLen = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblUnmatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblCallRuntime = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);

   // If count is zero or negative, we must treat it as if it were one.
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, 0);
   if (TR::Compiler->target.cpu.id() >= TR_FirstPPCASProcessor)
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, PPCOpProp_BranchLikely, node, lblFixLen, cndReg);
   else
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, node, lblFixLen, cndReg);
   loadConstant(cg, node, 1, lenReg);
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFixLen);

   // index = 0;
   loadConstant(cg, node, 0x0, indexReg);

   if (versionLimit > 0)
      {
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, versionLimit);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblCallRuntime, cndReg);
      cr6Reg = cg->allocateRegister(TR_CCR);
      addDependency(conditions, cr6Reg, 	TR::RealRegister::cr6,   TR_CCR, cg);
      }

   // CTR = len
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lenReg);

   // Adjust pointers for lbzu
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, -1);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, -1);

   // Head
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 1, 1, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzu, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 1, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblUnmatch, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 0x1);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);

   if (returnsLength)
      {
      // Unmatch
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblUnmatch);
      }
   else
      {
      // Match
      loadConstant(cg, node, 1, indexReg);
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);
      // Unmatch
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblUnmatch);
      loadConstant(cg, node, 0, indexReg);
      }

   if (versionLimit > 0)
      {
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblCallRuntime);

      TR_RuntimeHelper helper = returnsLength ? TR_PPCarrayCmpLenVMX : TR_PPCarrayCmpVMX;

      // Use dummy dependency condition so that we can use the true dependency condition at the end of this function
      TR::RegisterDependencyConditions *conditions_dummy = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(0, 0, cg->trMemory());

      generateHelperBranchAndLinkInstruction(helper, node, conditions_dummy, cg);

      conditions_dummy->stopUsingDepRegs(cg);

      cg->machine()->setLinkRegisterKilled(true);
      cg->setHasCall();
      }

   // Exit
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);
   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);

   conditions->stopUsingDepRegs(cg, indexReg);
   cg->stopUsingRegister(ch0Reg);
   cg->stopUsingRegister(ch1Reg);
   if (versionLimit <= 0) cg->stopUsingRegister(cndReg);

   return indexReg;
   }

// arraycmpGeneratorVMXInline --
//
// 'LD_ELEM' can be lbz, lhz, or lwz depending on the estimated element size.
// 'index' is actually a boolean flag.
//
// Generated code: (60 steps)
//      entry:
//		li	index,0
//              LD_ELEM	ch0,1(src0)
//              LD_ELEM	ch1,1(src1)
//              cmpw	ch0,ch1
//              bne	exit
//              cmpwi	len,ELEM_SIZE
//              ble	match
//		// Setup constants
//		rlwinm.	residue,len,32-4,0x0fffffff	// vector count
//		mtctr	residue
//		rlwinm	residue,len,0,0x0000000f	// residue bytes
//		beq	doResidue			// skip vector processing
//		cmpi4	cr0,reisidue,0
//		lvx	vin0a,(src0,const0)
//		lvx	vin1a,(src0,const0)
//		lvx	vin0b,(src0,const16)
//		lvx	vin1b,(src0,const16)
//		lvx	vpat0,(src0,const0)
//		lvx	vpat1,(src0,const0)
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0a,vin0b,vpat0
//		vperm	v1,vin1a,vin1b,vpat1
//		vcmpeubr v0,v0,v1
//		lvx	vin0a,(src0,const16)
//		lvx	vin1a,(src1,const16)
//	VMXLoopTop:
//		!---------- dispatch group
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0b,vin0a,vpat0
//		vperm	v1,vin1b,vin1a,vpat1
//		bvna	cr6,exit
//		!---------- dispatch group
//		vcmpeubr	v0,v0,v1
//		lvx	vin0b,(src0,const16)
//		lvx	vin1b,(src1,const16)
//		bdz	VMXLoopEnd
//		!---------- dispatch group
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0a,vin0b,vpat0
//		vperm	v1,vin1a,vin1b,vpat1
//		bvnz	cr6,exit
//		!---------- dispatch group
//		vcmpeubr	v0,v0,v1
//		lvx	vin0a,(src0,const16)
//		lvx	vin1a,(src1,const16)
//		bdnz	VMXLoopTop
//	VMXLoopEnd:
//		beq	cr0,match
//		addi	src0,src0,-16
//		addi	src1,src1,-16
//
//	doResidue:
//		addi	residue,residue,-1
//		vcmpgtub	vmask,vmask,vconst0x10
//		lvx	vin0a,(src0,const0)
//		lvx	vin1a,(src1,const0)
//		lvx	vin0b,(src0,const16)
//		lvx	vin1b,(src1,const16)
//		lvsl	vpat0,(src0,const0)
//		lvsl	vpat1,(src1,const0)
//		vperm	v0,vin0a,vin0b,vpat0
//		vperm	v1,vin1a,vin1b,vpat1
//		vcmpequb	v0,v0,v1
//		vor	v0,v0,vmask
//		vcmpgubr	v0,v0,vconst0x10
//	#if 1
//		bvna	cr6,exit
//		li	index,1
//	#else
//		mfcr	index,0xff
//		rlwimi	index,index,32-7,1
//	#endif
//		b	exit
//
//	match:
//		li	index,1
//      exit:
static TR::Register *arraycmpGeneratorVMXInline(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraycmpGeneratorVMXInline:\n");

   // Evaluate all arguments
   TR::Register *src0Reg		= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *src1Reg		= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg		= cg->gprClobberEvaluate(node->getChild(2));

   // Allocate GPR and misc. registers
   TR::Register *indexReg 	= cg->allocateRegister();
   TR::Register *residueReg 	= cg->allocateRegister();
      TR::Register *const0Reg=cg->allocateRegister();
      TR::Register *const16Reg=cg->allocateRegister();
   TR::Register *ch0Reg		= cg->allocateRegister();
   TR::Register *ch1Reg		= cg->allocateRegister();

   TR::Register *v0Reg		= cg->allocateRegister(TR_VRF);
   TR::Register *v1Reg		= cg->allocateRegister(TR_VRF);
   TR::Register *vconst0x10Reg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin0aReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin0bReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin1aReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin1bReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vmaskReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vpat0Reg	= cg->allocateRegister(TR_VRF);
   TR::Register *vpat1Reg	= cg->allocateRegister(TR_VRF);

   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *cr0Reg		= cg->allocateRegister(TR_CCR);
   TR::Register *cr6Reg		= cg->allocateRegister(TR_CCR);

   // Allocate labels
   TR::LabelSymbol *lblFixLen = generateLabelSymbol(cg);
   TR::LabelSymbol *lblVMXLoopTop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblVMXLoopEnd = generateLabelSymbol(cg);
   TR::LabelSymbol *lblResidue = generateLabelSymbol(cg);
   TR::LabelSymbol *lblUnmatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblMatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);

   TR::InstOpCode::Mnemonic loadElemOp;
   // We assume that the length (in byte) is a multiple of elemSize
   int32_t elemSize = arraycmpEstimateElementSize(node->getChild(2), cg);
   TRACE_VMX1("@elemSize=%d\n", elemSize);

   switch (elemSize)
      {
      case 0:
      case 1:
         elemSize = 1;
         loadElemOp = TR::InstOpCode::lbz;
         break;
      case 2:
         loadElemOp = TR::InstOpCode::lhz;
         break;
      case 4:
      case 8:
         elemSize = 4;
         loadElemOp = TR::InstOpCode::lwz;
         break;
      }

   loadConstant(cg, node, 0, indexReg);

   // Peel the first element.
   // We can safely do it  here since the loop repeats at least once regardless of length parameter
   generateTrg1MemInstruction(cg, loadElemOp, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 0, elemSize, cg));
   generateTrg1MemInstruction(cg, loadElemOp, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 0, elemSize, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblExit, cndReg);	// unmatch
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, elemSize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblMatch, cndReg); // match

   // const0 = 0
   loadConstant(cg, node, 0, const0Reg);
   // const16 = 16
   loadConstant(cg, node, 16, const16Reg);
   // vconst0x10Reg = (16)x10;
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0x10Reg, 8);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vaddubm, node, vconst0x10Reg, vconst0x10Reg, vconst0x10Reg);

   // CR0/CTR = len / 16;
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, residueReg, lenReg, 32 - 4, 0x0fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, residueReg);
   // residue = len % 16;
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, residueReg, lenReg, 0, 0x0000000f);
   // if (CTR == 0) goto residue;
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblResidue, cr0Reg);

   // CR0 = (residue == 0)
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cr0Reg, residueReg, 0);
   // vin0a = vec_ld(0, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vin1a = vec_ld(0, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // vin0b = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0bReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   // vin1b = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1bReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   // vpat0 = vec_lvsl(src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vpat1 = vec_lvsl(src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   // src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   // v0 = vec_perm(vin0a, vin0b, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0aReg, vin0bReg, vpat0Reg);
   // v1 = vec_perm(vin1a, vin1b, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1aReg, vin1bReg, vpat1Reg);
   // v0/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, v0Reg, v0Reg, v1Reg);
   // vin0a = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   // vin1a = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));

   // do {
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblVMXLoopTop);
   //     src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   //     src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   //     v0 = vec_perm(vin0b, vin0a, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0bReg, vin0aReg, vpat0Reg);
   //     v1 = vec_perm(vin1b, vin1a, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1bReg, vin1aReg, vpat1Reg);
   //     if (CR6.anyNE) goto Unmatch;
   generateConditionalBranchInstruction(cg, PPCOp_bvna, node, lblExit, cr6Reg);

   //     v0/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, v0Reg, v0Reg, v1Reg);
   //     vin0b = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0bReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   //     vin1b = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1bReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   //     if (--CTR == 0) goto VMXLoopEnd;
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdz, node, lblVMXLoopEnd, cndReg);

   //     src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   //     src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   //     v0 = vec_perm(vin0a, vin0b, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0aReg, vin0bReg, vpat0Reg);
   //     v1 = vec_perm(vin1a, vin1b, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1aReg, vin1bReg, vpat1Reg);
   //     if (CR6.anyNE) goto Unmatch;
   generateConditionalBranchInstruction(cg, PPCOp_bvna, node, lblExit, cr6Reg);

   //     v0/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, v0Reg, v0Reg, v1Reg);
   //     vin0a = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   //     vin1a = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   // } while (--CTR != 0);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblVMXLoopTop, cndReg);
   // VMXLoopEnd:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblVMXLoopEnd);
   // if (residue == 0) goto match;
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblMatch, cr0Reg);

   //     src0--;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, -16);
   //     src1--;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, -16);

   // residue: // process residue of 1..15 bytes using vector opration with mask
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblResidue);
   // vmask = vec_lvsr(0, residue - 1);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, residueReg, residueReg, -1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsr, node, vmaskReg, new (cg->trHeapMemory()) TR::MemoryReference(residueReg, const0Reg, 16, cg));
   // vmask = vec_cmpgtub(vmask, vconst0x10);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgtub, node, vmaskReg, vmaskReg, vconst0x10Reg);
   // vin0a = vec_ld(0, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vin1a = vec_ld(0, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // vin0b = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0bReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   // vin1b = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1bReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   // vpat0 = vec_lvsl(0, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vpat1 = vec_lvsl(0, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // v0 = vec_perm(vin0a, vin0b, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0aReg, vin0bReg, vpat0Reg);
   // v1 = vec_perm(vin1a, vin1b, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1aReg, vin1bReg, vpat1Reg);
   // v0 = vec_vcmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpequb, node, v0Reg, v0Reg, v1Reg);

   // v0 = vec_or(v0, vmask);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vor, node, v0Reg, v0Reg, vmaskReg);

   // v0/CR6 = vec_cmpgtub_r(v0, vconst0x10);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, v0Reg, v0Reg, vconst0x10Reg);
   // index = CR6.allOK;
   //generateTrg1ImmInstruction(cg, TR::InstOpCode::mfcr, node, indexReg, 0xff);
   generateTrg1ImmInstruction(cg, TR::InstOpCode::mfcr, node, indexReg, 0xff);

   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, indexReg, indexReg, 32-7, 1);
   // goto exit;
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);

   // match:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblMatch);
   // index = 1; // All vectors matched and we have no more trailers.
   loadConstant(cg, node, 1, indexReg);
   // goto exit;
   // fall through

   // exit:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);

   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(16, 16, cg->trMemory());
   addDependency(conditions, src0Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, src1Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,	TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, indexReg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const0Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const16Reg,	TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, vconst0x10Reg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin0aReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin0bReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin1aReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin1bReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpat0Reg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpat1Reg,	TR::RealRegister::NoReg, TR_VRF, cg);

   addDependency(conditions, cndReg, 	TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cr0Reg, 	TR::RealRegister::cr0,   TR_CCR, cg);
   addDependency(conditions, cr6Reg, 	TR::RealRegister::cr6,   TR_CCR, cg);

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->stopUsingRegister(residueReg);
   cg->stopUsingRegister(ch0Reg);
   cg->stopUsingRegister(ch1Reg);
   cg->stopUsingRegister(v0Reg);
   cg->stopUsingRegister(v1Reg);
   cg->stopUsingRegister(vmaskReg);

   return indexReg;
   }

// arraycmpLenGeneratorVMXInline --
//
// 'LD_ELEM' can be lbz, lhz, or lwz depending on the estimated element size.
// 'index' is actually a boolean flag.
//
// Generated code:
//      entry:
//              LD_ELEM	ch0,1(src0)
//              LD_ELEM	ch1,1(src1)
//              cmpw	ch0,ch1
//              bne	exit0
//              cmpwi	len,ELEM_SIZE
//              ble	exit1
//		mr	src0save,src0
//		// Setup constants
//		addi	tmp,len,15
//		rlwinm	tmp,tmp,32-4,0x0fffffff		// (len + 15) / 16
//		mtctr	tmp
//		lvx	vin0a,(src0,const0)
//		lvx	vin1a,(src0,const0)
//		lvx	vin0b,(src0,const16)
//		lvx	vin1b,(src0,const16)
//		lvx	vpat0,(src0,const0)
//		lvx	vpat1,(src0,const0)
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0a,vin0b,vpat0
//		vperm	v1,vin1a,vin1b,vpat1
//		vcmpeubr vcmp,v0,v1
//		lvx	vin0a,(src0,const16)
//		lvx	vin1a,(src1,const16)
//	VMXLoopTop:
//		!---------- dispatch group
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0b,vin0a,vpat0
//		vperm	v1,vin1b,vin1a,vpat1
//		bvna	cr6,unmatch
//		!---------- dispatch group
//		vcmpeubr	vcmp,v0,v1
//		lvx	vin0b,(src0,const16)
//		lvx	vin1b,(src1,const16)
//		bdz	VMXLoopEnd
//		!---------- dispatch group
//		addi	src0,src0,16
//		addi	src1,src1,16
//		vperm	v0,vin0a,vin0b,vpat0
//		vperm	v1,vin1a,vin1b,vpat1
//		bvna	cr6,unmatch
//		!---------- dispatch group
//		vcmpeubr	vcmp,v0,v1
//		lvx	vin0a,(src0,const16)
//		lvx	vin1a,(src1,const16)
//		bdnz	VMXLoopTop
//	VMXLoopEnd: //All vectors (including trailers) match
//		mr	index,len
//		b	exit
//
//	unmatch:
//		// We have comparison result in vcmp (need to find first 0x00 byte)
//		addi	src0,src0,-32 // rewind
//		sub	index,src0,src0Save // position of the first byte in the vector
//
//		lvx	vmask,
//		lvx	vmerge,
//		vandc	vcmp,vmask,vcmp
//		vsum4sbs vcmp,vcmp,vconst0x00
//		vperm	vcmp,vcmp,vcmp,vmerge
//		// retrive temporary area
//		stvewx	vcmp,WORK
//		nop
//		nop
//		nop
//		lwz	tmp,WORK
//		cntlzw	tmp,tmp
//		add	index,index,tmp
//
//		// index = min(index,len)
//		sub	index,index,len
//		srawi	tmp,index,31
//		and	index,index,tmp
//		add	index,index,len
//		b	exit
//
//	exit1:
//		li	index,ELEM_SIZE
//		b	exit
//
//	exit0:
//		li	index,0
//      exit:
static TR::Register *arraycmpLenGeneratorVMXInline(TR::Node *node, TR::CodeGenerator *cg)
   {
   TRACE_VMX0("@arraycmpLenGeneratorVMXInline:\n");
   TR::Compilation *comp = cg->comp();
   // Evaluate all arguments
   TR::Register *src0Reg		= cg->gprClobberEvaluate(node->getFirstChild());
   TR::Register *src1Reg		= cg->gprClobberEvaluate(node->getSecondChild());
   TR::Register *lenReg		= cg->gprClobberEvaluate(node->getChild(2));

   // Allocate GPR and misc. registers
   TR::Register *src0SaveReg	= cg->allocateRegister();
   TR::Register *indexReg 	= cg->allocateRegister();
   //TR::Register *residueReg 	= cg->allocateRegister();
   TR::Register *const0Reg 	= cg->allocateRegister();
   TR::Register *const16Reg 	= cg->allocateRegister();
   TR::Register *ch0Reg		= cg->allocateRegister();
   TR::Register *ch1Reg		= cg->allocateRegister();
   TR::Register *tmpReg		= cg->allocateRegister();
   TR::Register *constTableReg	= cg->allocateRegister();

   TR::Register *v0Reg		= cg->allocateRegister(TR_VRF);
   TR::Register *v1Reg		= cg->allocateRegister(TR_VRF);
   TR::Register *vcmpReg		= cg->allocateRegister(TR_VRF);
   TR::Register *vmaskReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vmergeReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin0aReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin0bReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin1aReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vin1bReg	= cg->allocateRegister(TR_VRF);
   TR::Register *vpat0Reg	= cg->allocateRegister(TR_VRF);
   TR::Register *vpat1Reg	= cg->allocateRegister(TR_VRF);
   TR::Register *vconst0x00Reg	= cg->allocateRegister(TR_VRF);

   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *cr0Reg		= cg->allocateRegister(TR_CCR);
   TR::Register *cr6Reg		= cg->allocateRegister(TR_CCR);

   TR::Register *workAddrReg;

   // Allocate work space (16 byte aligned 16 byte)
   TR::SymbolReference *symRef = comp->getSymRefTab()->createLocalPrimArray(16 + (16 - 1), comp->getMethodSymbol(), 8);
   symRef->setStackAllocatedArrayAccess();
   TR::Node *loadWorkAddrNode = TR::Node::createWithSymRef(node, TR::loadaddr, 0, symRef);
   loadWorkAddrNode->incReferenceCount();

   // Allocate labels
   TR::LabelSymbol *lblFixLen = generateLabelSymbol(cg);
   TR::LabelSymbol *lblVMXLoopTop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblVMXLoopEnd = generateLabelSymbol(cg);
   //TR::LabelSymbol *lblResidue = generateLabelSymbol(cg);
   TR::LabelSymbol *lblUnmatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblMatch = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit1 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit0 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);

   TR::InstOpCode::Mnemonic loadElemOp;
   // We assume that the length (in byte) is a multiple of elemSize
   int32_t elemSize = arraycmpEstimateElementSize(node->getChild(2), cg);
   TRACE_VMX1("@elemSize=%d\n", elemSize);

   switch (elemSize)
         {
      case 0:
      case 1:
         elemSize = 1;
         loadElemOp = TR::InstOpCode::lbz;
         break;
      case 2:
         loadElemOp = TR::InstOpCode::lhz;
         break;
      case 4:
      case 8:
         elemSize = 4;
         loadElemOp = TR::InstOpCode::lwz;
         break;
         }

   // Peel the first element.
   // We can safely do it  here since the loop repeats at least once regardless of length parameter
   generateTrg1MemInstruction(cg, loadElemOp, node, ch0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, 0, elemSize, cg));
   generateTrg1MemInstruction(cg, loadElemOp, node, ch1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, 0, elemSize, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, ch0Reg, ch1Reg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblExit0, cndReg);	// Unmatch
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lenReg, elemSize);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblExit1, cndReg); // match
   generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, src0SaveReg, src0Reg);

   // const0 = 0
   loadConstant(cg, node, 0, const0Reg);
   // const16 = 16
   loadConstant(cg, node, 16, const16Reg);
   // vconst0x00Reg
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0x00Reg, 0);

   // CTR = (len + 15) / 16;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmpReg, lenReg, 15);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm_r, node, tmpReg, tmpReg, 32 - 4, 0x0fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);

   // vin0a = vec_ld(0, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vin1a = vec_ld(0, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // vin0b = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0bReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   // vin1b = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1bReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   // vpat0 = vec_lvsl(src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat0Reg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const0Reg, 16, cg));
   // vpat1 = vec_lvsl(src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpat1Reg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const0Reg, 16, cg));
   // src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   // src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   // v0 = vec_perm(vin0a, vin0b, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0aReg, vin0bReg, vpat0Reg);
   // v1 = vec_perm(vin1a, vin1b, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1aReg, vin1bReg, vpat1Reg);
   // vcmp/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vcmpReg, v0Reg, v1Reg);
   // vin0a = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   // vin1a = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));

   // do {
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblVMXLoopTop);
   //     src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   //     src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   //     v0 = vec_perm(vin0b, vin0a, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0bReg, vin0aReg, vpat0Reg);
   //     v1 = vec_perm(vin1b, vin1a, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1bReg, vin1aReg, vpat1Reg);
   //     if (CR6.anyNE) goto unmatch;
   generateConditionalBranchInstruction(cg, PPCOp_bvna, node, lblUnmatch, cr6Reg);

   //     vcmp/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vcmpReg, v0Reg, v1Reg);
   //     vin0b = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0bReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   //     vin1b = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1bReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   //     if (--CTR == 0) goto VMXLoopEnd;
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdz, node, lblVMXLoopEnd, cndReg);

   //     src0++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, 16);
   //     src1++;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src1Reg, src1Reg, 16);
   //     v0 = vec_perm(vin0a, vin0b, vpat0);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v0Reg, vin0aReg, vin0bReg, vpat0Reg);
   //     v1 = vec_perm(vin1a, vin1b, vpat1);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, v1Reg, vin1aReg, vin1bReg, vpat1Reg);
   //     if (CR6.anyNE) goto unmatch;
   generateConditionalBranchInstruction(cg, PPCOp_bvna, node, lblUnmatch, cr6Reg);

   //     vcmp/CR6 = vec_cmpequb(v0, v1);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vcmpReg, v0Reg, v1Reg);
   //     vin0a = vec_ld(16, src0);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin0aReg, new (cg->trHeapMemory()) TR::MemoryReference(src0Reg, const16Reg, 16, cg));
   //     vin1a = vec_ld(16, src1);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vin1aReg, new (cg->trHeapMemory()) TR::MemoryReference(src1Reg, const16Reg, 16, cg));
   // } while (--CTR != 0);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblVMXLoopTop, cndReg);
   // VMXLoopEnd:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblVMXLoopEnd);
   generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, indexReg, lenReg);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblUnmatch);
   // src0--; src0--;
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, src0Reg, src0Reg, -32);
   // index = src0 - src0Save
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, indexReg, src0SaveReg, src0Reg);

   // prepare complex constants in memory
   uint32_t *vconstTable = (uint32_t*)TR_Memory::jitPersistentAlloc(8*sizeof(uint32_t) + (16 - 1));
   vconstTable = (uint32_t *)(((((uintptr_t)vconstTable-1) >> 4) + 1) << 4); // align to 16-byte boundary
   loadAddressConstant(cg, node, (intptrj_t)vconstTable, constTableReg);
   for (int i=0; i<4; i++) vconstTable[i] = 0x80200802;
   vconstTable[4] = 0x03070B0F; for (int i=5; i<8; i++) vconstTable[i] = 0;

   // Load vmask
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vmaskReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const0Reg, 16, cg));
   // Load vmerge
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vmergeReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const16Reg, 16, cg));

   // vandc	vcmp,vmask,vcmp
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vandc, node, vcmpReg, vmaskReg, vcmpReg);
   // vsum4sbs vcmp,vcmp,vconst0x00
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vsum4sbs, node, vcmpReg, vcmpReg, vconst0x00Reg);
   // vperm	vcmp,vcmp,vcmp,vmerge
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vcmpReg, vcmpReg, vcmpReg, vmergeReg);

   // Locate work space
   workAddrReg = cg->evaluate(loadWorkAddrNode);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, workAddrReg, workAddrReg, 15);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, workAddrReg, workAddrReg, 0, 0xfffffff0);

   generateMemSrc1Instruction(cg, TR::InstOpCode::stvx, node, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, const0Reg, 16, cg), vcmpReg);

   // Three NOPs to start new dispatch group
   if (TR::Compiler->target.cpu.id() >= TR_PPCgp)
         {
      cg->generateGroupEndingNop(node);
         }
   // Load first 32bit of the vector into GPR
   generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, 0, 4, cg));

   // Find the first bit set
   generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, tmpReg);
   // tmp /= 2 to get bit offset
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 1, 0x7fffffff);
   // index += tmp;
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);
   // index = min(index, len)
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, indexReg, lenReg, indexReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmpReg, indexReg, 31);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, indexReg, indexReg, tmpReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, lenReg);
   // goto exit
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);

   // exit1:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit1);
   // index = ELEM_SIZE;
   loadConstant(cg, node, elemSize, indexReg);
   // goto exit
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblExit);

   // exit0:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit0);
   // index = 0;
   loadConstant(cg, node, 0, indexReg);
   // goto exit (fall through)

   // exit:
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);

   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(18, 18, cg->trMemory());
   addDependency(conditions, src0Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, src1Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lenReg,	TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, src0SaveReg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const0Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const16Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, workAddrReg, TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, vconst0x00Reg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin0aReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin0bReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin1aReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vin1bReg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpat0Reg,	TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpat1Reg,	TR::RealRegister::NoReg, TR_VRF, cg);

   addDependency(conditions, cndReg, 	TR::RealRegister::NoReg, TR_CCR, cg);
   addDependency(conditions, cr0Reg, 	TR::RealRegister::cr0,   TR_CCR, cg);
   addDependency(conditions, cr6Reg, 	TR::RealRegister::cr6,   TR_CCR, cg);

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->decReferenceCount(loadWorkAddrNode);
   cg->stopUsingRegister(ch0Reg);
   cg->stopUsingRegister(ch1Reg);
   cg->stopUsingRegister(v0Reg);
   cg->stopUsingRegister(v1Reg);
   cg->stopUsingRegister(vmaskReg);
   cg->stopUsingRegister(constTableReg);
   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(vcmpReg);
   cg->stopUsingRegister(vmaskReg);
   cg->stopUsingRegister(vmergeReg);

   return indexReg;
   }

//----------------------------------------------------------------------
// findBytesGenTRTCom code generator
//----------------------------------------------------------------------
//
// `table' can be TR::loadaddr or TR::aiadd
// VMX requires compile-time constant table. Thus TR::loadaddr is mandatory.
//
//XXX: Must be separated into Scalar version and VMX version.
static TR::Register *findBytesGenTRTCom(TR::Node * node, TR::CodeGenerator * cg)
   {
   TRACE_VMX1("@findBytesGenTRTCom: node=%p\n", node);
   TR::Compilation *comp = cg->comp();
   int i;
   TR::Node *baseNode	= node->getFirstChild();
   TR::Node *indexNode	= node->getSecondChild();
   TR::Node *tableNode	= node->getChild(2);
   TR::Node *alenNode	= node->getChild(3);
   TR::Register *baseReg	= cg->gprClobberEvaluate(baseNode);
   TR::Register *indexReg	= cg->gprClobberEvaluate(indexNode);
   TR::Register *tableReg	= cg->gprClobberEvaluate(tableNode);
   TR::Register *alenReg		= cg->gprClobberEvaluate(alenNode);

   TR::Register *charReg = cg->allocateRegister();
   TR::Register *flagReg = cg->allocateRegister();
   TR::Register *lmt4Reg = cg->allocateRegister();
   TR::Register *cndReg = cg->allocateRegister(TR_CCR);

   TR::Register *lenReg = NULL;
   TR::Register *minReg = NULL;
   int numRegister;
   bool hasAdditionalLen = node->getNumChildren() == 5;
   bool elementChar = node->isCharArrayTRT();

   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblExit = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead2 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblHeadSca = generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec = generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec16= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec32= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFindVec48= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFind1 = generateLabelSymbol(cg);
   TR::LabelSymbol *lblFind = generateLabelSymbol(cg);
   TR::LabelSymbol *lblScaLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblNounrollLoop = generateLabelSymbol(cg);
   TR::LabelSymbol *lblException = TR::LabelSymbol::create(cg->trHeapMemory());

   cg->addSnippet(new (cg->trHeapMemory()) TR_PPCHelperCallSnippet(cg, node, lblException,
                                             comp->getSymRefTab()->findOrCreateArrayBoundsCheckSymbolRef(comp->getMethodSymbol())));

   // Determine the number of logical register
   numRegister = 4 + 4 + 1;	// (base, index, table, alen), (char, flag, lmt4, cndReg), workAddr
   if (hasAdditionalLen)
      numRegister += 2;	// lenReg and minReg
   if (!VMXEnabled(cg))
      numRegister += 0;
   else
      numRegister += 38;
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(numRegister, numRegister, cg->trMemory());

   addDependency(conditions, baseReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, tableReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, alenReg, TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, charReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, flagReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, lmt4Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg, TR::RealRegister::cr0,    TR_CCR, cg);

   // allocate temporary memory area in the stack
   // this area musy be aligned on 16-byte boundary
   TR::SymbolReference *symRef = comp->getSymRefTab()->createLocalPrimArray(16*2, comp->getMethodSymbol(), 8);
   symRef->setStackAllocatedArrayAccess();
   TR::Node *loadWorkAddrNode = TR::Node::createWithSymRef(node, TR::loadaddr, 0, symRef);
   loadWorkAddrNode->incReferenceCount();
   TR::Register *workAddrReg = cg->evaluate(loadWorkAddrNode);
      addDependency(conditions, workAddrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, workAddrReg, workAddrReg, 15);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, workAddrReg, workAddrReg, 0, 0xfffffff0);

   if (elementChar)
      {
      // Convert to byte counts
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, indexReg, indexReg, 1, 0xfffffffe);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, alenReg, alenReg, 1, 0xfffffffe);
      }

   if (!hasAdditionalLen)
      {
      minReg = lenReg = alenReg;
      }
   else
      {
      lenReg = cg->gprClobberEvaluate(node->getChild(4));
      if (elementChar) // convert to byte count
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 1, 0xfffffffe);
      minReg = cg->allocateRegister();
      addDependency(conditions, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, minReg, TR::RealRegister::NoReg, TR_GPR, cg);

      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, minReg, alenReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, minReg, lenReg);
      TR::LabelSymbol * lblSkip = TR::LabelSymbol::create(cg->trHeapMemory());
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblSkip, cndReg);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, minReg, lenReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkip);
      }

   // generate an address to the actual data
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, baseReg, baseReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

   // On z-series, we utilize CLCL when data type is char with fix-up process.
   // However on PPC/VMX, we can naively search for 16-byte data effectively.

   if (!(VMXEnabled(cg) && tableNode->getOpCodeValue() == TR::loadaddr))
      {
      TRACE_VMX0("@TRT by scalar\n");
      TR::Register *curBaseReg=cg->allocateRegister();
      TR::Register *char1Reg=cg->allocateRegister();
      TR::Register *char2Reg=cg->allocateRegister();
      TR::Register *char3Reg=cg->allocateRegister();

      /*
      pseudo code of an unrolled loop:

      chr1 = data[i]; // preload into register
         //for (; i < limit-5; i+=4)
         for (; i <= limit-4; i+=4) // fix by tm
         {
         if (!delimiter_table[chr1]) return i;
         if (!delimiter_table[data[i+1]]) return i+1;
         if (!delimiter_table[data[i+2]]) return i+2;
         if (!delimiter_table[data[i+3]]) return i+3;
         chr1 = data[i+4];
         }
      */

      // initialize counter register by iteration count
      // CTR = ((limit - 4) - index) >> 2
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lmt4Reg, minReg, -4);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lmt4Reg); // Use signed comparison
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmt4Reg, indexReg, lmt4Reg);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lmt4Reg, lmt4Reg, 32 - 2, 0x3fffffff);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmt4Reg);

      // skip unrolled loop if limit is too small
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, node, lblNounrollLoop, cndReg); // fix by tm bge -> bgt

      // preload one byte
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, curBaseReg, indexReg, baseReg);

      // loop body, see an above pseudo code
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char1Reg, new (cg->trHeapMemory()) TR::MemoryReference(curBaseReg, 1, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char2Reg, new (cg->trHeapMemory()) TR::MemoryReference(curBaseReg, 2, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, charReg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, char3Reg, new (cg->trHeapMemory()) TR::MemoryReference(curBaseReg, 3, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, char1Reg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, char2Reg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbz, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(curBaseReg, 4, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, curBaseReg, curBaseReg, 4);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, char3Reg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
      // end loop body

      /*
      pseudo code of a loop without unrolling:
      for (; i < limit; i++)
         if (!delimiter_table[data[i]]) return i;
      */

      // initialize counter register by iteration count
      // CTR = (limit - index)
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNounrollLoop);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, minReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, minReg, indexReg, minReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, minReg);

      // break if index is equal or larger than limit
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblExit, cndReg);

      // loop body, see an above pseudo code
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadSca);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, charReg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadSca, cndReg);
      // end loop body

      // reached the end of array without finding delimiter
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);

      // check for exception
      if (hasAdditionalLen)
         {
         // stop due to user specified end
         generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lenReg);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFind, cndReg);
         }
      // array bound violation
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblException);

      // found a delimiter
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFind);

      cg->stopUsingRegister(curBaseReg);
      cg->stopUsingRegister(char1Reg);
      cg->stopUsingRegister(char2Reg);
      cg->stopUsingRegister(char3Reg);
      }
   else
      {
      TRACE_VMX0("TRT by VMX\n");

      TR::Register *vtab0Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vtab1Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vec1Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vec2Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vpatReg  = cg->allocateRegister(TR_VRF);
      TR::Register *vcharsReg  = cg->allocateRegister(TR_VRF);
      TR::Register *vbytesReg = cg->allocateRegister(TR_VRF);
      TR::Register *vbitoffReg = cg->allocateRegister(TR_VRF);
      TR::Register *vbitsReg = cg->allocateRegister(TR_VRF);
      TR::Register *vchars2Reg  = cg->allocateRegister(TR_VRF);
      TR::Register *vbytes2Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbitoff2Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbits2Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vchars3Reg  = cg->allocateRegister(TR_VRF);
      TR::Register *vbytes3Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbitoff3Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbits3Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vchars4Reg  = cg->allocateRegister(TR_VRF);
      TR::Register *vbytes4Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbitoff4Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vbits4Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vresultReg = cg->allocateRegister(TR_VRF);

      TR::Register *cr6Reg = cg->allocateRegister(TR_CCR);
      TR::Register *tmpReg=cg->allocateRegister();
      TR::Register *tmp2Reg=cg->allocateRegister();
      TR::Register *constTableReg=cg->allocateRegister();
      TR::Register *lmtvReg = cg->allocateRegister();
      TR::Register *lmtv4Reg = cg->allocateRegister();

      // registers for constants
      TR::Register *vconst0Reg  = cg->allocateRegister(TR_VRF);
      TR::Register *vconst5Reg = cg->allocateRegister(TR_VRF);
      TR::Register *vconst7FReg = cg->allocateRegister(TR_VRF);
      TR::Register *vconstMaskReg  = cg->allocateRegister(TR_VRF);
      TR::Register *vconstMergeReg  = cg->allocateRegister(TR_VRF);
      TR::Register *const0Reg=cg->allocateRegister();
      TR::Register *const16Reg=cg->allocateRegister();
      TR::Register *const32Reg=cg->allocateRegister();
      TR::Register *const48Reg=cg->allocateRegister();
      TR::Register *const64Reg=cg->allocateRegister();

      // dependency for vector registers
      addDependency(conditions, vtab0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vtab1Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vec1Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vec2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vpatReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vcharsReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbytesReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbitoffReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbitsReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vchars2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbytes2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbitoff2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbits2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vchars3Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbytes3Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbitoff3Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbits3Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vchars4Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbytes4Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbitoff4Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vbits4Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vresultReg, TR::RealRegister::NoReg, TR_VRF, cg);

      // dependency for other registers
      addDependency(conditions, cr6Reg, TR::RealRegister::cr6, TR_CCR, cg);
      addDependency(conditions, tmpReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, tmp2Reg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, constTableReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, lmtvReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, lmtv4Reg, TR::RealRegister::NoReg, TR_GPR, cg);

      // dependency for constants
      addDependency(conditions, vconst0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vconst5Reg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vconst7FReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vconstMaskReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, vconstMergeReg, TR::RealRegister::NoReg, TR_VRF, cg);
      addDependency(conditions, const0Reg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, const16Reg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, const32Reg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, const48Reg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, const64Reg, TR::RealRegister::NoReg, TR_GPR, cg);

      // if target array is too small for VMX
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, minReg, 32);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, false, node, lblScaLoop, cndReg);

      // return immediately if the 1st charcter is a delimiter
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, charReg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, false, node, lblFind, cndReg);

      // prepare simple vector constants
      generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0Reg, 0);
      generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst5Reg, 5);

      // prepare constants
      loadConstant(cg, node,  0, const0Reg);
      loadConstant(cg, node, 16, const16Reg);
      loadConstant(cg, node, 32, const32Reg);

      // allocate read-only memory area
      // this area musy be aligned on 16-byte boundary
      uint32_t *vconstTable = (uint32_t*)TR_Memory::jitPersistentAlloc(20*sizeof(uint32_t) + (16 - 1));
      vconstTable = (uint32_t *)(((((uintptr_t)vconstTable-1) >> 4) + 1) << 4);
      loadAddressConstant(cg, node, (intptrj_t)vconstTable, constTableReg);

      // load first two vectors from input array
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmp2Reg, baseReg, indexReg);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpatReg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const0Reg, 16, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vec1Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const0Reg, 16, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(tmp2Reg, const16Reg, 16, cg));

      // prepare complex constants in memory
      createBitTable((uint8_t*)(tableNode->getSymbolReference()->getSymbol()->getStaticSymbol()->getStaticAddress()), (uint8_t *)vconstTable, cg);
      for (i= 8; i<12; i++) vconstTable[i] = 0x7F7F7F7F;
      for (i=12; i<16; i++) vconstTable[i] = 0x80200802;
      vconstTable[16] = 0x03070B0F;
      for (i=17; i<20; i++) vconstTable[i] = 0;

      // load prepared constants into vector registers
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab0Reg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const0Reg, 16, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab1Reg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const16Reg, 16, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconst7FReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const32Reg, 16, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, constTableReg, constTableReg, 48);

      // test the first 16 charcters
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vec2Reg, vec1Reg, vcharsReg, vpatReg);
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytesReg, vtab0Reg, vtab1Reg, vec2Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoffReg, vec2Reg, vconst5Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbitsReg, vbytesReg, vbitoffReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbitsReg, vconst7FReg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);

      /*
      pseudo code of an unrolled loop:

      vchars_1 = vdata[i/16]; // preload into vector register
         //FIXME check equal case
      for (; i < limit-16*5; i += 16*4)  // process 64 chacters in one loop (16 byte * 4 vector)
         {
         vbytes_1 = vec_perm(vdelimiter_table0, vdelimiter_table1, vchars_1);
         vbitoff_1 = vec_sr(vchars_1, vconst5);
         vbits_1 = vec_sl(vbytes_1, vbits_offs_1);
         if (vec_any_gt(vbits_1, vconst7F))
            return find_position_in_vector();

         vchars_2 = vdata[i/16+1];
         vbitoff_2 = vec_sr(vchars_2, vconst5);

         ....

         if (vec_any_gt(vbits_4, vconst7F))
            return find_position_in_vector();

         vchars_1 = vdata[i/16+4];
         }
      */

      // force index align to 128-bit boundary
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tmpReg, tmp2Reg, 0xF);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, tmpReg, const16Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmpReg, tmpReg, tmp2Reg);

      // initialize counter register by iteration count
      // CTR = (limit - 16 * 5 - index) >> 6
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, lmtv4Reg, minReg, -16*5);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, lmtv4Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lmtv4Reg);
      //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, lmtvReg, lmtvReg, 6);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lmtvReg, lmtvReg, 32 - 6, 0x03ffffff);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);

      // break if index is equal or larger than limit
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblNounrollLoop, cndReg);

      // additional constants
      loadConstant(cg, node, 48, const48Reg);
      loadConstant(cg, node, 64, const64Reg);

      // loop body, see an above pseudo code
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars2Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const16Reg, 16, cg));
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytesReg, vtab0Reg, vtab1Reg, vcharsReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoffReg, vcharsReg, vconst5Reg);
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytes2Reg, vtab0Reg, vtab1Reg, vchars2Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoff2Reg, vchars2Reg, vconst5Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbitsReg, vbytesReg, vbitoffReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbitsReg, vconst7FReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbits2Reg, vbytes2Reg, vbitoff2Reg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);

      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars3Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const32Reg, 16, cg));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbits2Reg, vconst7FReg);
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytes3Reg, vtab0Reg, vtab1Reg, vchars3Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoff3Reg, vchars3Reg, vconst5Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbits3Reg, vbytes3Reg, vbitoff3Reg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec16, cr6Reg);

      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars4Reg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const48Reg, 16, cg));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbits3Reg, vconst7FReg);
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytes4Reg, vtab0Reg, vtab1Reg, vchars4Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoff4Reg, vchars4Reg, vconst5Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbits4Reg, vbytes4Reg, vbitoff4Reg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec32, cr6Reg);

      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, const64Reg, 16, cg));
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbits4Reg, vconst7FReg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec48, cr6Reg);

      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16*4);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmpReg, tmpReg, 16*4);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
      // end loop body

      /*
      pseudo code of a loop without unrolling:

      for (; i < limit-16; i += 16)  // process 16 chacters in one loop
         {
         vchars_1 = vdata[i/16];
        vbytes_1 = vec_perm(vdelimiter_table0, vdelimiter_table1, vchars_1);
        vbitoff_1 = vec_sr(vchars_1, vconst5);
        vbits_1 = vec_sl(vbytes_1, vbits_offs_1);
        if (vec_any_gt(vbits_1, vconst7F))
            return find_position_in_vector();
         }
      */
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNounrollLoop);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, minReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, lmtvReg, 15);
      //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, lmtvReg, lmtvReg, 4);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lmtvReg, lmtvReg, 32 - 4, 0x0fffffff);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblScaLoop, cndReg);

      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead2);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 16, cg));
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vbytesReg, vtab0Reg, vtab1Reg, vcharsReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsrb, node, vbitoffReg, vcharsReg, vconst5Reg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vslb, node, vbitsReg, vbytesReg, vbitoffReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpgubr, node, vresultReg, vbitsReg, vconst7FReg);
      generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFindVec, cr6Reg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead2, cndReg);

      /*
      pseudo code of a non-VMX loop:

      for (; i < limit; i++)
         if (!delimiter_table[data[i]]) return i;
      */
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblScaLoop);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, minReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, lmtvReg, indexReg, minReg);
      generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, lmtvReg);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblExit, cndReg);

      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadSca);
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, flagReg, new (cg->trHeapMemory()) TR::MemoryReference(tableReg, charReg, 1, cg));
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, flagReg, 0);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bne, node, lblFind, cndReg);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadSca, cndReg);

      // reached the end of array without finding delimiter
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblExit);
      // check exception
      if (hasAdditionalLen)
         {
         // stop due to user specified end
         generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lenReg);
         generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFind, cndReg);
         }
      // array bound violation
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblException);

      // found a delimiter
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec);

      // load constants
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMaskReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const0Reg, 16, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMergeReg, new (cg->trHeapMemory()) TR::MemoryReference(constTableReg, const16Reg, 16, cg));

      // gather bits from each charcter to determine the position
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vand, node, vresultReg, vresultReg, vconstMaskReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::vsum4sbs, node, vresultReg, vresultReg, vconst0Reg);
      generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vresultReg, vresultReg, vresultReg, vconstMergeReg);

      // move data from vector register to GPR
      // write data to temporary area
      generateMemSrc1Instruction(cg, TR::InstOpCode::stvewx, node, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, const0Reg, 16, cg), vresultReg);

      // insert three no-ops to enforce load and store be in other dispatch groups
      if (TR::Compiler->target.cpu.id() >= TR_PPCgp)
         {
         cg->generateGroupEndingNop(node);
         }

      // read data from temporary area
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, 0, 4, cg));

      // determine a position of the delimiter by using coun-leading-zeros instruction
      generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, tmpReg);
      //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmpReg, tmpReg, 1);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 1, 0x7fffffff);

      // make index register to point an address of the found delimiter
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);

      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFind);

      // found a delimiter in the 2nd vector in unrolled loop
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec16);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

      // found a delimiter in the 3rd vector in unrolled loop
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec32);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 32);
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

      // found a delimiter in the 4th vector in unrolled loop
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFindVec48);
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 48);
      generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFindVec);

      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFind);

      }

   // Adjustment for character data type
   if (elementChar)
      {
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, indexReg, indexReg, 32 - 1, 0x7fffffff);
      }

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);

   cg->decReferenceCount(loadWorkAddrNode);

   return indexReg;
   }

//----------------------------------------------------------------------
// findBytesGenSRSTScalar code generator
//----------------------------------------------------------------------
static TR::Register *findBytesGenSRSTScalar(TR::Node * node, TR::CodeGenerator * cg)
   {
   TRACE_VMX1("@findBytesGenSRSTScalar: node=%p\n", node);
   TR::Compilation *comp = cg->comp();
   TR::Node *baseNode		= node->getFirstChild();
   TR::Node *indexNode		= node->getSecondChild();
   TR::Node *delimNode		= node->getChild(2);
   TR::Node *alenNode		= node->getChild(3);
   TR::Register *baseReg		= cg->gprClobberEvaluate(baseNode);
   TR::Register *indexReg	= cg->gprClobberEvaluate(indexNode);
   TR::Register *delimReg	= cg->gprClobberEvaluate(delimNode);
   TR::Register *alenReg		= cg->gprClobberEvaluate(alenNode);

   TR::Register *lenReg		= NULL; // Additional parameter
   TR::Register *curAddrReg	= cg->allocateRegister();
   TR::Register *charReg		= cg->allocateRegister();
   TR::Register *flagReg		= cg->allocateRegister();
   TR::Register *char1Reg	= cg->allocateRegister();
   TR::Register *char2Reg	= cg->allocateRegister();
   TR::Register *char3Reg	= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *limitReg	= NULL;
   TR::Register *tmpReg		= cg->allocateRegister();
   TR::RegisterDependencyConditions *conditions;

   bool hasAdditionalLen = node->getNumChildren() == 5;
   bool isCharElement = node->isCharArrayTRT();

   TR::LabelSymbol *lblPreLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblSkipPreLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNotFound	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFound	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblException	= TR::LabelSymbol::create(cg->trHeapMemory());

   cg->addSnippet(new (cg->trHeapMemory()) TR_PPCHelperCallSnippet(cg, node, lblException,
                                              comp->getSymRefTab()->findOrCreateArrayBoundsCheckSymbolRef(comp->getMethodSymbol())));
   if (hasAdditionalLen)
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(9 + 2, 9 + 2, cg->trMemory());
   else
      conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(9, 9, cg->trMemory());
   addDependency(conditions, baseReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, alenReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, curAddrReg,TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, charReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, char1Reg,	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, flagReg, 	TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg,	TR::RealRegister::cr0,   TR_CCR, cg);

   if (isCharElement)
      {
      // Convert element count to byte count
      RLWINM(index,index,1,0xfffffffe);
      RLWINM(alen,alen,1,0xfffffffe);
      }
   if (hasAdditionalLen)
      {
      lenReg = cg->gprClobberEvaluate(node->getChild(4));
      if (isCharElement) // convert element count to byte count
         RLWINM(len,len,1,0xfffffffe);
      limitReg = cg->allocateRegister();
      addDependency(conditions, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, limitReg, TR::RealRegister::NoReg, TR_GPR, cg);
#if 1
      // min = std::min(alen, len). You may want to use a magic code to eliminate conditional branch.
      TR::LabelSymbol * lblSkip = TR::LabelSymbol::create(cg->trHeapMemory());
      MR(limit,alen);
      CMPL4(cnd,limit,len); // unsigned comparison
      BLE(cnd,Skip);
      MR(limit,len);
      LABEL(Skip);
#endif
      }
   else
      {
      limitReg = lenReg = alenReg;
      }

   // Convert object addr into data addr
   ADDI(base,base,TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

   // Pseudo code of the following unrolled loop
   //
   // ptr = &data[i];
   // char  = ptr[0];
   // char1 = ptr[1];
   // for (; i <= limit-4; i+=4)
   // {
   //     char2 = ptr[2]; char3 = ptr[3];
   //     if (char == delimiter) goto found;
   //     i++; char = ptr[4];
   //     if (char1 == delimiter) goto found;
   //     i++;
   //     char1 = ptr[5];
   //     if (char2 == delimiter) goto found;
   //     i++;
   //     if (char3 == delimiter) goto found;
   //     i++;
   //     ptr += 4;
   // }
   //

   CMP4(cnd,index,limit);
   BEQ(cnd,NotFound);	// Should we treat as "Found" ?

   SUBF(tmp,index,limit);
   ANDI_R(tmp,tmp,3);
   BEQ(cnd,SkipPreLoop);
   MTCTR(tmp);

   // PreLoop
   LABEL(PreLoop);
   LBZX(char,index,base);
   CMP4(cnd,char,delim);
   BEQ(cnd,Found);
   ADDI(index,index,1);
   BDNZ(cnd,PreLoop);
   // PreLoop end

   // SkipPreLoop
   LABEL(SkipPreLoop);
   SUBF(tmp,index,limit); // Must always be a multiple of 4
   RLWINM(tmp,tmp,32 - 2, 0x3fffffff);
   MTCTR(tmp);
   CMPI4(cnd,tmp,0);
   BEQ(cnd,NotFound);

   ADD(curAddr,index,base);

   // Preload two bytes
   LBZ(char,0,curAddr);
   LBZ(char1,1,curAddr);

   // Unrolled loop
   LABEL(Loop);

   LBZ(char2,2,curAddr);
   LBZ(char3,3,curAddr);
   CMP4(cnd,char,delim);
   BEQ(cnd,Found);

   ADDI(index,index,1);
   LBZ(char,4,curAddr);
   CMP4(cnd,char1,delim);
   BEQ(cnd,Found);

   ADDI(index,index,1);
   LBZ(char1,5,curAddr);
   CMP4(cnd,char2,delim);
   BEQ(cnd,Found);

   ADDI(index,index,1);
   CMP4(cnd,char3,delim);
   BEQ(cnd,Found);

   ADDI(index,index,1);
   ADDI(curAddr,curAddr,4);
   BDNZ(cnd,Loop);
   // Loop end

   // fall through
   LABEL(NotFound);
   if (hasAdditionalLen)
      {
      CMP4(cnd,index,len);
      BEQ(cnd,Found); // Not actually found.
   }
   // Array bound violation
   B(cnd,Exception);

   LABEL(Found);
   if (isCharElement) // Convert back to char count
      RLWINM(index,index,32 - 1,0x7fffffff);

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);
   cg->stopUsingRegister(tmpReg);
   cg->stopUsingRegister(char2Reg);
   cg->stopUsingRegister(char3Reg);

   return indexReg;
   }

//----------------------------------------------------------------------
// findBytesGenSRSTVMX code generator
//----------------------------------------------------------------------
// XXX: FIXME remove code for scalar processing
static TR::Register *findBytesGenSRSTVMX(TR::Node * node, TR::CodeGenerator * cg)
   {
   TRACE_VMX1("@findBytesGenSRSTVMX: node=%p\n", node);
   TR::Compilation *comp = cg->comp();
   TR::Node *baseNode		= node->getFirstChild();
   TR::Node *indexNode		= node->getSecondChild();
   TR::Node *delimNode		= node->getChild(2);
   TR::Node *alenNode		= node->getChild(3);
   TR::Register *baseReg		= cg->gprClobberEvaluate(baseNode);
   TR::Register *indexReg	= cg->gprClobberEvaluate(indexNode);
   TR::Register *delimReg	= cg->gprClobberEvaluate(delimNode);
   TR::Register *alenReg		= cg->gprClobberEvaluate(alenNode);

   TR::Register *charReg		= cg->allocateRegister();
   TR::Register *flagReg		= cg->allocateRegister();
   TR::Register *cndReg		= cg->allocateRegister(TR_CCR);
   TR::Register *lenReg		= NULL; // Additional parameter
   TR::Register *limitReg		= NULL;
   TR::Register *tmpReg		= cg->allocateRegister();
   TR::Register *curAddrReg	= cg->allocateRegister();
   TR::Register *workAddrReg;
   TR::Node *loadWorkAddrNode;

   int numRegs;
   bool hasAdditionalLen = node->getNumChildren() == 5;
   bool isCharElement = node->isCharArrayTRT();

   TR::LabelSymbol *lblHead = generateLabelSymbol(cg);
   TR::LabelSymbol *lblNotFound	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblHead2	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblHeadSca	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFoundVec	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFoundVec16	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFoundVec32	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFoundVec48	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblFound	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblScaLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNoUnrollLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblNoUnrollVecLoop	= generateLabelSymbol(cg);
   TR::LabelSymbol *lblException	= TR::LabelSymbol::create(cg->trHeapMemory());

   cg->addSnippet(new (cg->trHeapMemory()) TR_PPCHelperCallSnippet(cg, node, lblException,
                                              comp->getSymRefTab()->findOrCreateArrayBoundsCheckSymbolRef(comp->getMethodSymbol())));

   // Determine the number of logical registers
   numRegs = 4 + 4;	// (base, index, delim, alen), (char, flag, cndReg, curAddr)
   if (hasAdditionalLen)
      numRegs += 2;		// lenReg and limitReg
   numRegs += 20;
   TR::RegisterDependencyConditions *conditions = new (cg->trHeapMemory()) TR::RegisterDependencyConditions(numRegs, numRegs, cg->trMemory());

   addDependency(conditions, baseReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, indexReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, delimReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, alenReg, TR::RealRegister::NoReg, TR_GPR, cg);

   addDependency(conditions, charReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, flagReg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, cndReg, TR::RealRegister::cr0,    TR_CCR, cg);
   addDependency(conditions, curAddrReg, TR::RealRegister::NoReg, TR_GPR, cg);

   // Allocate 16-byte aligned 16-byte long space in the stack at run time
   TR::SymbolReference *symRef = comp->getSymRefTab()->createLocalPrimArray(16 + (16 - 1), comp->getMethodSymbol(), 8);
   symRef->setStackAllocatedArrayAccess();
   loadWorkAddrNode = TR::Node::createWithSymRef(node, TR::loadaddr, 0, symRef);
   loadWorkAddrNode->incReferenceCount();
   workAddrReg = cg->evaluate(loadWorkAddrNode);
   addDependency(conditions, workAddrReg, TR::RealRegister::NoReg, TR_GPR, cg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, workAddrReg, workAddrReg, 15);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, workAddrReg, workAddrReg, 0, 0xfffffff0);

   if (isCharElement)
      {
      // Convert element count to byte count
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, indexReg, indexReg, 1, 0xfffffffe);
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, alenReg, alenReg, 1, 0xfffffffe);
      }
   if (!hasAdditionalLen)
      {
      limitReg = lenReg = alenReg;
      }
   else
      {
      lenReg = cg->gprClobberEvaluate(node->getChild(4));
      if (isCharElement) // convert element count to byte count
         generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, lenReg, lenReg, 1, 0xfffffffe);
      limitReg = cg->allocateRegister();
      addDependency(conditions, lenReg, TR::RealRegister::NoReg, TR_GPR, cg);
      addDependency(conditions, limitReg, TR::RealRegister::NoReg, TR_GPR, cg);
      // min = min(alen, len)
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, limitReg, alenReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, limitReg, lenReg);
      TR::LabelSymbol * lblSkip = TR::LabelSymbol::create(cg->trHeapMemory());
      generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblSkip, cndReg);
      generateTrg1Src1Instruction(cg, TR::InstOpCode::mr, node, limitReg, lenReg);
      generateLabelInstruction(cg, TR::InstOpCode::label, node, lblSkip);
      }

   // Convert object addr into data addr
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, baseReg, baseReg, TR::Compiler->om.contiguousArrayHeaderSizeInBytes());

   TR::Register *vtab0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vec1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vec2Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vpatReg = cg->allocateRegister(TR_VRF);
   TR::Register *vcharsReg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars1Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars2Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vchars3Reg = cg->allocateRegister(TR_VRF);

   TR::Register *vconst0Reg = cg->allocateRegister(TR_VRF);
   TR::Register *vconstMaskReg = cg->allocateRegister(TR_VRF);
   TR::Register *vconstMergeReg = cg->allocateRegister(TR_VRF);
   TR::Register *vresultReg = cg->allocateRegister(TR_VRF);

   TR::Register *cr6Reg = cg->allocateRegister(TR_CCR);

   TR::Register *const0Reg = cg->allocateRegister();
   TR::Register *const16Reg = cg->allocateRegister();
   TR::Register *const32Reg = cg->allocateRegister();
   TR::Register *const48Reg = cg->allocateRegister();
   TR::Register *const64Reg = cg->allocateRegister();
   TR::Register *constDelimReg = cg->allocateRegister();

   addDependency(conditions, vtab0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vec1Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vec2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vpatReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vcharsReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vchars1Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vchars2Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vchars3Reg, TR::RealRegister::NoReg, TR_VRF, cg);

   addDependency(conditions, vconst0Reg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconstMaskReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vconstMergeReg, TR::RealRegister::NoReg, TR_VRF, cg);
   addDependency(conditions, vresultReg, TR::RealRegister::NoReg, TR_VRF, cg);

   addDependency(conditions, cr6Reg, TR::RealRegister::cr6, TR_CCR, cg);

   addDependency(conditions, const0Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const16Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const32Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const48Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, const64Reg, TR::RealRegister::NoReg, TR_GPR, cg);
   addDependency(conditions, constDelimReg, TR::RealRegister::NoReg, TR_GPR, cg);

   // We use VMX iff we have at least two vectors
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, indexReg, limitReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::cmpi4, node, cndReg, tmpReg, 32);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, false, node, lblScaLoop, cndReg);

   // Prepare constants
   loadConstant(cg, node,  0, const0Reg);
   loadConstant(cg, node, 16, const16Reg);
   loadConstant(cg, node, 32, const32Reg);
   loadConstant(cg, node, 48, const48Reg);
   loadConstant(cg, node, 64, const64Reg);
   generateTrg1ImmInstruction(cg, TR::InstOpCode::vspltisb, node, vconst0Reg, 0);

   // Prepare complex constants in memory, and load them into vector registers
   uint32_t *vconstDelim = (uint32_t*)TR_Memory::jitPersistentAlloc(12*sizeof(uint32_t) + (16 - 1));
   //vconstDelim = (uint32_t *)(((((uint32_t)vconstDelim-1) >> 4) + 1) << 4); // align to 16-byte boundary
   vconstDelim = (uint32_t *)(((uintptr_t)vconstDelim + 15) & ~15);	// Align to 16-byte boundary
   loadAddressConstant(cg, node, (intptrj_t)vconstDelim, constDelimReg);
   //for (int i = 0; i < 16; i++) ((uint8_t *)vconstDelim)[i] = delimNode->getInt();
   for (int i = 0; i < 4; i++) vconstDelim[i] = (delimNode->getInt() << 24) | (delimNode->getInt() << 16) | (delimNode->getInt() << 8) | delimNode->getInt();
   for (int i = 4; i < 8; i++) vconstDelim[i] = 0x80200802;
   vconstDelim[8] = 0x03070b0f; for (int i = 9; i < 12; i++) vconstDelim[i] = 0;
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vtab0Reg, new (cg->trHeapMemory()) TR::MemoryReference(constDelimReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMaskReg, new (cg->trHeapMemory()) TR::MemoryReference(constDelimReg, const16Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vconstMergeReg, new (cg->trHeapMemory()) TR::MemoryReference(constDelimReg, const32Reg, 16, cg));

   // Load the first 16 bytes from input array
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, curAddrReg, baseReg, indexReg);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx,  node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const0Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars1Reg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const16Reg, 16, cg));
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvsl, node, vpatReg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const0Reg, 16, cg));
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vcharsReg, vcharsReg, vchars1Reg, vpatReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vcharsReg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec, cr6Reg);

   // Pseudo code of the unrolled loop
   // vchars_1 = vdata[i/16]; // preload into vector register
   // for (; i <= limit-16*4; i += 16*4)  // process 64 chacters in one loop (16 byte * 4 vector)
   // {
   //     if (vec_any_eq(vchars_1, vdelimiter))
   //         return find_position_in_vector();
   //
   //     vchars_2 = vdata[i/16+1];
   //     if (vec_any_eq(vchars_2, vdelimiter))
   //         return find_position_in_vector();
   //
   //     ....
   //
   //     if (vec_any_eq(vchars_3, vdelimiter))
   //         return find_position_in_vector();
   //
   //     vchars_1 = vdata[i/16+4];
   // }

   // We know that the delimiter is not contained in the first 16 bytes
   // Advance to the next 16-byte boundary
   //generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::andi_r, node, tmpReg, curAddrReg, 0xf);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, curAddrReg, 0, 0x0000000f);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, tmpReg, const16Reg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, curAddrReg, curAddrReg, tmpReg);

   // Initialize counter register by iteration count
   // Determine if unrolled loop repeats at least once or not
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmpReg, limitReg, -16 * 4);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, tmpReg);
   // CTR = (limit - index) >> 6
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, indexReg, limitReg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 6, 0x03ffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);
   // Skip unrolled loop
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bgt, node, lblNoUnrollVecLoop, cndReg);

   // Preload one vector
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const0Reg, 16, cg));

   // Unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars1Reg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const16Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vcharsReg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars2Reg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const32Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars1Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec16, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vchars3Reg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const48Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars2Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec32, cr6Reg);

   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(curAddrReg, const64Reg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vchars3Reg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec48, cr6Reg);

   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16 * 4);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, curAddrReg, curAddrReg, 16 * 4);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead, cndReg);
   // end loop body

   // Pseudo code of the non-unrolled loop
   // for (; i <= limit-16; i += 16)  // process 16 chacters in one loop
   // {
   //     vchars_1 = vdata[i/16];
   //     if (vec_any_gt(vchars_1, vdelimiter))
   //     return find_position_in_vector();
   // }

   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNoUnrollVecLoop);
   // Determine if unrolled loop repeats at least once or not
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, tmpReg, limitReg, -16);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, tmpReg);
   // CTR = (limit - index) >> 4
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, indexReg, limitReg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 4, 0x0fffffff);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);
   // Skip loop
   generateConditionalBranchInstruction(cg, TR::InstOpCode::ble, node, lblScaLoop, cndReg);

   // Non-unrolled VMX loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHead2);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lvx, node, vcharsReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 16, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vcmpeubr, node, vresultReg, vcharsReg, vtab0Reg);
   generateConditionalBranchInstruction(cg, PPCOp_bvnn, node, lblFoundVec, cr6Reg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHead2, cndReg);

   // Use scalar loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblScaLoop);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, limitReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, tmpReg, indexReg, limitReg);
   generateSrc1Instruction(cg, TR::InstOpCode::mtctr, node, tmpReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bge, node, lblNotFound, cndReg);

   // Non-unrolled scalar loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblHeadSca);
   generateTrg1MemInstruction(cg, TR::InstOpCode::lbzx, node, charReg, new (cg->trHeapMemory()) TR::MemoryReference(baseReg, indexReg, 1, cg));
   generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, charReg, delimReg);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 1);
   generateConditionalBranchInstruction(cg, TR::InstOpCode::bdnz, node, lblHeadSca, cndReg);
   // End loop body

   // Reached the end of array without finding delimiter
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblNotFound);

   // Check array bound exception
   if (hasAdditionalLen)
      {
      // Stopped due to user specified end
      generateTrg1Src2Instruction(cg, TR::InstOpCode::cmp4, node, cndReg, indexReg, lenReg);
      generateConditionalBranchInstruction(cg, TR::InstOpCode::beq, node, lblFound, cndReg); // Not actually found
      }
   // Array bound violation
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblException);

   // Found a delimiter with index containing byte offset, or stopped by user specified limit
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFoundVec);

   // Gather bits from each charcter to determine the position
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vand, node, vresultReg, vresultReg, vconstMaskReg);
   generateTrg1Src2Instruction(cg, TR::InstOpCode::vsum4sbs, node, vresultReg, vresultReg, vconst0Reg);
   generateTrg1Src3Instruction(cg, TR::InstOpCode::vperm, node, vresultReg, vresultReg, vresultReg, vconstMergeReg);

   // Move data from vector register to GPR via cache
   generateMemSrc1Instruction(cg, TR::InstOpCode::stvewx, node, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, const0Reg, 16, cg), vresultReg);
   // Insert three no-ops to enforce load and store be in other dispatch groups
   if (TR::Compiler->target.cpu.id() >= TR_PPCgp)
      {
      cg->generateGroupEndingNop(node);
      }
   // Read data from temporary area
   generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, tmpReg, new (cg->trHeapMemory()) TR::MemoryReference(workAddrReg, 0, 4, cg));

   // Determine the position of the delimiter by using count-leading-zeros instruction
   generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, tmpReg);
   generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 32 - 1, 0x7fffffff);

   // Make index register to point an address of the found delimiter
   generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, indexReg, indexReg, tmpReg);

   // Go to exit
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFound);

   // Found a delimiter in the 2nd vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFoundVec16);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 16);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFoundVec);

   // Found a delimiter in the 3rd vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFoundVec32);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 32);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFoundVec);

   // found a delimiter in the 4th vector in unrolled loop
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFoundVec48);
   generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addi, node, indexReg, indexReg, 48);
   generateLabelInstruction(cg, TR::InstOpCode::b, node, lblFoundVec);

   // Found a delimiter with index containing byte offset, or stopped by user specified limit
   generateLabelInstruction(cg, TR::InstOpCode::label, node, lblFound);

   // Adjustment for character data type
   if (isCharElement)
      {
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, indexReg, indexReg, 32 - 1, 0x7fffffff);
      }

   generateDepInstruction(cg, TR::InstOpCode::depend, node, conditions);
   conditions->stopUsingDepRegs(cg, indexReg);
   cg->stopUsingRegister(tmpReg);

   cg->decReferenceCount(loadWorkAddrNode);
   cg->stopUsingRegister(workAddrReg);

   return indexReg;
   }
#endif

// Counts number of digits
//
// Takes signed integer or signed long, and returnes
// the number of digits (excluding '-' character for negative case).
//
// Arguments:
//   1: input data either SIN32 or SINT64
//   2: conversion table (struct ppcDigit10TableEnt *)
//      Note: or 32bit idiom, 33rd entry of the table is passed.
//
// Pseudo code (C code)
//
//    int CountDecimalDigitInt(int32_t val, struct ppcDigit10TableEnt *tbl)
//    {
//       uint32 uval = ABS(val);
//       int zeros = CLZ(uval);
//       int digits = tbl[zeros].digits;
//       uint32 limit = tbl[zeros].limit;
//       if (uval > limit) digits++;
//       return digits;
//    }
//
//    int CountDecimalDigitLong(int64_t val, struct ppcDigit10TableEnt *tbl)
//    {
//       uint64 uval = ABS(val);
//       int zeros = CLZ(uval);
//       int digits = tbl[zeros].digits;
//       uint64 limit = tbl[zeros].limitLong;
//       if (uval > limit) digits++;
//       return digits;
//    }
//

TR::Register *OMR::Power::TreeEvaluator::countDigitsEvaluator(TR::Node * node, TR::CodeGenerator * cg)
   {
#if defined(TR_IDIOM_PPCVMX)
   TRACE_VMX1("@countDigitsEvaluator: node=%p\n", node);
   TR::Node *inputNode = node->getFirstChild();
   TR::Node *tableNode = node->getSecondChild();
   TR::Register *inputReg = cg->evaluate(inputNode);
   TR::Register *tableReg = cg->evaluate(tableNode);
   TR::Register *digitsReg = cg->allocateRegister();

   TR_ASSERT(inputNode->getDataType() == TR_SInt64 || inputNode->getDataType() == TR_SInt32, "child of TR::countDigits must be of integer type");
   bool isLong = (inputNode->getDataType() == TR_SInt64);

   if (!isLong)
      {
      // TODO: On PPC64, the upper word of inputReg may have garbage.

      TR::Register *absReg = cg->allocateRegister();
      TR::Register *tmpReg = cg->allocateRegister();
      TR::Register *limitReg = cg->allocateRegister();

      // Compute the absolute of input value, and treat it as an unsigned integer from now on.
      // Note minimum value 0x80000000 (-2147483648, 10-digits) is converted into 0x80000000u
      // (2147483648) which is also a 10-digits number.
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmpReg, inputReg, 31); // mask = DUP32(sign(input))
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, absReg, inputReg, tmpReg);  // abs = input XOR mask
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subf, node, absReg, tmpReg, absReg);   // abs = (input XOR mask) - mask

      // Count the number of leading zeros
      generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, absReg); // 0..32

      // Compute table offset
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 4, 0xfffffff0);
      // Get table entry address
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmpReg, tableReg, tmpReg);
      // Load the number of digits and the upper limit of 'digits' numbers
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, digitsReg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, 0, 4, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, limitReg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, 4, 4, cg));
      // Increment 'digits' if ABS(input) >u limit
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, tmpReg, absReg, limitReg); // Compute carry CA=ABS(num) > limit
      generateTrg1Src1Instruction(cg, TR::InstOpCode::subfze, node, digitsReg, digitsReg);     // -digits-CA
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, digitsReg, digitsReg);        // digits+CA

      cg->stopUsingRegister(inputReg);
      cg->stopUsingRegister(tableReg);
      cg->stopUsingRegister(absReg);
      cg->stopUsingRegister(tmpReg);
      cg->stopUsingRegister(limitReg);
      }
   else
   {
      // TODO: Need a simplified version for PPC64

      TR::Register *absLowReg = cg->allocateRegister();
      TR::Register *absHighReg = cg->allocateRegister();
      TR::Register *absReg = cg->allocateRegisterPair(absLowReg, absHighReg);
      TR::Register *tmpReg = cg->allocateRegister();
      TR::Register *tmp2Reg = cg->allocateRegister();
      TR::Register *tmp3Reg = cg->allocateRegister();
      TR::Register *limitLowReg = cg->allocateRegister();
      TR::Register *limitHighReg = cg->allocateRegister();
      TR::Register *limitReg = cg->allocateRegisterPair(limitLowReg, limitHighReg);

      // Compute the absolute of input value, and treat it as an unsigned long from now on.
      // Note minimum value 0x8000000000000000 is converted into 0x8000000000000000
      // which is also a 19-digits number.
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::srawi, node, tmpReg, inputReg->getHighOrder(), 31);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, absReg->getLowOrder(), inputReg->getLowOrder(), tmpReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::XOR, node, absReg->getHighOrder(), inputReg->getHighOrder(), tmpReg);
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, absReg->getLowOrder(), tmpReg, absReg->getLowOrder());
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, absReg->getHighOrder(), tmpReg, absReg->getHighOrder());

      // Count the number of leading zeros
      generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmpReg, absReg->getHighOrder());	// tmp = CLZ(countHigh)
      generateTrg1Src1Instruction(cg, TR::InstOpCode::cntlzw, node, tmp2Reg, absReg->getLowOrder());	// tmp2 = CLZ(countLow)
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::xori, node, tmp3Reg, tmpReg, 32);	// Invert 2^5 bit. tmp3 = (countHigh == 32) ? Zero : NonZero
      generateTrg1Src1ImmInstruction(cg, TR::InstOpCode::addic, node, tmp3Reg, tmp3Reg, -1);	// CA = (count_high== 32) ? 0 : 1
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, tmp3Reg, tmp3Reg, tmp3Reg);	// tmp3 = NOT(r3) + r3 + CA = 0xffffffff + CA = (countHigh == 32) ? 0xffffffff : 0
      generateTrg1Src2Instruction(cg, TR::InstOpCode::AND, node, tmp2Reg, tmp2Reg, tmp3Reg);	// tmp2 = countLow * mask
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmpReg, tmpReg, tmp2Reg);	// tmp = countHigh + countLow * mask

      // Compute table offset
      generateTrg1Src1Imm2Instruction(cg, TR::InstOpCode::rlwinm, node, tmpReg, tmpReg, 4, 0xfffffff0);
      // Get table entry address
      generateTrg1Src2Instruction(cg, TR::InstOpCode::add, node, tmpReg, tableReg, tmpReg);
      // Load the number of digits and the upper limit of 'digits' numbers
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, digitsReg, new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, 0, 4, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, limitReg->getHighOrder(), new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, 8, 4, cg));
      generateTrg1MemInstruction(cg, TR::InstOpCode::lwz, node, limitReg->getLowOrder(), new (cg->trHeapMemory()) TR::MemoryReference(tmpReg, 12, 4, cg));
      // Increment 'digits' if ABS(input) >u limit
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfc, node, tmpReg, absReg->getLowOrder(), limitReg->getLowOrder()); // Compute carry CA=ABS(num) > limit
      generateTrg1Src2Instruction(cg, TR::InstOpCode::subfe, node, tmpReg, absReg->getHighOrder(), limitReg->getHighOrder());

      generateTrg1Src1Instruction(cg, TR::InstOpCode::subfze, node, digitsReg, digitsReg);     // -digits-CA
      generateTrg1Src1Instruction(cg, TR::InstOpCode::neg, node, digitsReg, digitsReg);        // digits+CA

      cg->stopUsingRegister(inputReg);
      cg->stopUsingRegister(tableReg);
      cg->stopUsingRegister(absReg);
      cg->stopUsingRegister(tmpReg);
      cg->stopUsingRegister(tmp2Reg);
      cg->stopUsingRegister(tmp3Reg);
      cg->stopUsingRegister(limitReg);
     }

   cg->decReferenceCount(inputNode);
   cg->decReferenceCount(tableNode);
   node->setRegister(digitsReg);
   return digitsReg;
#else
   return NULL;
#endif
   }


//----------------------------------------------------------------------
// Utility functions
//----------------------------------------------------------------------

#if defined(TR_IDIOM_PPCVMX)
#if defined(__ibmxl__)
#define dcbz(x) __dcbz(x)
#elif defined(__xlC__)
// Code borrowed from j9memclr.c
void dcbz(char *);
#pragma mc_func dcbz  {"7c001fec"}  /* dcbz, 0, r3 */
#pragma reg_killed_by dcbz
#endif

static int getCacheLineSize(void)
{
#if defined(AIXPPC) || defined (LINUXPPC)
	char buf[1024];
	int i, ppcCacheLineSize;

	/* xlc -O3 inlines/unrolls this memset */
	memset(buf, 255, 1024);
#if defined(__xlC__)
	dcbz(&buf[512]);
#else
	__asm__ __volatile__("dcbz 0,%0" : /* no outputs */ : "r" (&buf[512]) );
#endif
	for (i = 0, ppcCacheLineSize = 0; i < 1024; i++)
		if (buf[i] == 0) ppcCacheLineSize++;
	return ppcCacheLineSize;
#else
	return 0;
#endif
}
// Code borrowed from j9memclr.c end
#endif

static int getOptVal(char *name, int undef_default, int empty_default)
   {
   static char *p = feGetEnv(name);
   int val;

   if (p == NULL)
      val = undef_default;
   else if (*p == '\0')
      val = empty_default;
   else
      val = strtol(p, NULL, 0);
   return val;
   }

#if defined(LINUX)
static int __power_vmx_linux()
   {
   // Code borrowed from frontend/j9/VMJ9.cpp
   FILE * fp ;
   char buffer[120];
   char *line_p;
   char *cpu_name = NULL;
   char *position_l, *position_r;
   size_t n=120;

   fp = fopen("/proc/cpuinfo","r");

   if ( fp == NULL )
      return 0;

   line_p = buffer;

   while (!feof(fp))
      {
      fgets(line_p, n, fp);
      position_l = strstr(line_p, "cpu");
      if (position_l)
         {
         position_l = strchr(position_l, ':');
         if (position_l==NULL) return 0;
         position_l++;
         while (*(position_l) == ' ') position_l++;

         position_r = strchr(line_p, '\n');
         if (position_r==NULL) return 0;
         while (*(position_r-1) == ' ') position_r--;

         if (position_l>=position_r) return 0;

         /* localize the cpu name */
         cpu_name = position_l;
         *position_r = '\000';

         break;
         }
      }
   fclose(fp);
   // Code borrowed from frontend/j9/VMJ9.cpp end
   return strstr(cpu_name, "altivec supported") != 0;
   }
#endif

static int VMXEnabled(TR::CodeGenerator *cg)
   {
   static int _VMXEnabledInited = 0;
   static int _VMXEnabled = 0;

   if (! _VMXEnabledInited)
      {
      int VMXCapable = 0;
      // for whatever reason, __power_vmx is unavailable in the system header files
      // disabled for now
      //
#if defined(AIXPPC)
      // A program can determine whether a system supports the vector
      // extension by reading the vmx_version field of the _system_configuration structure.
      // If this field is non-zero, then the system processors and operating system contain
      // support for the vector extension. A __power_vmx() macro is provided in
      // /usr/include/sys/systemcfg.h for performing this test.
      VMXCapable = 0; ///__power_vmx();
#elif defined(LINUX)
      VMXCapable = __power_vmx_linux();
#else
      VMXCapable = 0;
#endif
      _VMXEnabled = VMXCapable && getOptVal("TR_PPC_VMX", 0, 1) != 0; // FIXME: Use jit command line option
      _VMXEnabledInited = 1;
      }
   return _VMXEnabled;
   }

#if defined(TR_IDIOM_PPCVMX)
static int traceVMX(TR::CodeGenerator *cg)
   {
   static int _traceVMXInited = 0;
   static int _traceVMX = 0;

   if (! _traceVMXInited)
      {
      _traceVMX = getOptVal("TR_PPC_TRACE_VMX", 0, 1);
      _traceVMXInited = 1;
      }
   return _traceVMX;
   }

static int getArraysetNop(TR::CodeGenerator *cg)
   {
   static int _arraysetNopInited = 0;
   static int _arraysetNop = 0;
   if (! _arraysetNopInited)
      {
      _arraysetNop = getOptVal("TR_PPC_ARRAYSET_NOP", 0, 0);
      _arraysetNopInited = 1;
      }
   return _arraysetNop;
   }

static int getVersionLimit(TR::CodeGenerator *cg)
   {
   static int _versionLimitIinted = 0;
   static int _versionLimit = 0;
   if (! _versionLimitIinted)
      {
      _versionLimit = getOptVal("TR_PPC_ARRAYCMP_VERSION_LIMIT", 7, 7);
      _versionLimitIinted = 1;
      }
   return _versionLimit;
   }

static int getArraycmpUseVMXRuntime(TR::CodeGenerator *cg)
   {
   static int _arraycmpUseVMXRuntimeInited = 0;
   static int _arraycmpUseVMXRuntime = 0;
   if (! _arraycmpUseVMXRuntimeInited)
      {
      _arraycmpUseVMXRuntime = getOptVal("TR_PPC_ARRAYCMP_VMX_RUNTIME", 0, 1);
      _arraycmpUseVMXRuntimeInited = 1;
      }
   return _arraycmpUseVMXRuntime;
   }

/* for TRT */
static uint32_t *createBitTable(uint8_t *byteTable, uint8_t *bitTable, TR::CodeGenerator *cg)
   {
   TRACE_VMX2("@createBitTable for TRT: byteTable=%p bitTable=%p\n", byteTable, bitTable);
   int i, j;
   for (i=0; i<32; i++) bitTable[i] = 0;
   for (i=0; i<32; i++)
       {
       for (j=0; j<8; j++)
          if (byteTable[j*32+i] != 0) bitTable[i] |= 1 << (7-j);
       }
   return (uint32_t *)bitTable;
   }

/* for TROT */
static uint32_t *createBitTable(uint16_t *charTable, uint16_t termChar, uint8_t *bitTable, TR::CodeGenerator *cg)
   {
   TRACE_VMX3("@createBitTable for TROT: charTable=%p termChar=0x%x bitTable=%p\n", charTable, termChar, bitTable);
   int i, j;

   for (i=0; i<32; i++) bitTable[i] = 0;
   for (i=0; i<32; i++)
      {
      for (j=0; j<8; j++)
         if (charTable[j*32+i] == termChar) bitTable[i] |= 1 << (7-j);
      }
   return (uint32_t *)bitTable;
       }

// n must be one of 1, 2, 4, 8
static int log2(int n)
   {
   int i;
   if (n < 0)
      i = 31;
   else
      {
      for (i = 30; i >= 0; i--)
         if ((n & (1 << i)) != 0)
            break;
}
   return i;
}

// For arraycmp we can't directly determine the size of the element since two
// arguments are assumed to byte arrays.
// So we estimate it from the expression of the array size (in bytes).
// Returns 1, 2, 4, or 8, or 0 if the element size can not be determined.
static int arraycmpEstimateElementSize(TR::Node *lenNode, TR::CodeGenerator *cg)
   {
   TRACE_VMX2("@arraycmpEstimateElementSize: lenNode=%p, Op=0x%x\n",
              lenNode, lenNode->getOpCodeValue());
   int32_t size = 0;

   if (lenNode->getOpCodeValue() == TR::iadd)
      lenNode = lenNode->getFirstChild();

   if (lenNode->getOpCodeValue() == TR::imul)
      {
      TR::Node *child1Node = lenNode->getSecondChild();
      if (child1Node->getOpCodeValue() == TR::iconst)
         {
         size = child1Node->getInt();
         }
      }
   else if (lenNode->getOpCodeValue() == TR::ishl)
      {
      TR::Node *child1Node = lenNode->getSecondChild();
      if (child1Node->getOpCodeValue() == TR::iconst)
         {
         size = 1 << child1Node->getInt();
         }
      }
   TRACE_VMX1("@arraycmpEstimateElementSize: size=%d\n", size);
   return size;
   }

// Get the size of element
static int getElementSize(TR::Node *elemNode, TR::CodeGenerator *cg)
   {
   int sz;
   TR::DataType data_type = elemNode->getDataType();
   if (data_type == TR::Address)
      data_type = TR::Compiler->target.is64Bit() ? TR_SInt64 : TR_SInt32;
   switch (data_type)
      {
      case TR_Bool:
      case TR_SInt8:
      case TR_UInt8:
         sz = 1;
         break;
      case TR_UInt16:
      case TR_SInt16:
         sz = 2;
         break;
      case TR_SInt32:
      case TR_UInt32:
      case TR::Float:
         sz = 4;
         break;
      case TR_SInt64:
      case TR_UInt64:
      case TR::Double:
         sz = 8;
         break;
      default:
         TR_ASSERT(0, "Unexpected datatype %d encountered on arrayset", data_type);
         break;
      }
   return sz;
   }

// Takes an address node and returns non-negative number if
// the offset from the top of object header can be determined at compilation time.
// Otherwise -1 is returned.
//
// Currently we recognize only the following form:
//   (aiadd (iaload) (iconst <offset>))

//static int isFirstElement(TR::Node *addrNode, TR::CodeGenerator *cg)
static int getOffset(TR::Node *addrNode, TR::CodeGenerator *cg)
   {
   TR::Node *child0;
   TR::Node *child1;

   if (addrNode->getOpCodeValue() == TR::aiadd &&
       (child0 = addrNode->getFirstChild()) != NULL &&
       child0->getOpCodeValue() == TR::aloadi &&
       (child1 = addrNode->getSecondChild()) != NULL &&
       child1->getOpCodeValue() == TR::iconst)
      return (int32_t)getIntegralValue(child1);

   return -1;
   }

// Borrowed from s390/TreeEvaluator.cpp
static int64_t getIntegralValue(TR::Node * node)
   {
   int64_t value = 0;
   if (node->getOpCode().isLoadConst())
      {
      switch (node->getDataType())
         {
         case TR_Bool:
         case TR_SInt8:
            value = node->getByte();
            break;
         case TR_UInt8:
            value = node->getUnsignedByte();
            break;
         case TR_SInt16:
            value = node->getShortInt();
            break;
         case TR_UInt16:
            value = node->getConst<uint16_t>();
            break;
         case TR_SInt32:
            value = node->getInt();
            break;
         case TR_UInt32:
            value = node->getUnsignedInt();
            break;
         case TR::Address:
            value = node->getAddress();
            break;
         case TR_SInt64:
            value = node->getLongInt();
            break;
         case TR_UInt64:
            value = node->getUnsignedLongInt();
            break;
         default:
            TR_ASSERT(0, "Unexpected element load datatype encountered");
            break;
         }
      }
   return value;
   }

static void fillConst16(char *buf, TR::Node *elemNode, TR::CodeGenerator *cg)
   {
   TR_ASSERT(elemNode->getOpCode().isLoadConst(), "Element is not loadConst");
   int i;
   union const16_u
      {
      uint8_t	_byte[16];
      int8_t	_ubyte[16];
      int16_t	_short[8];
      uint16_t	_ushort[8];
      int32_t	_int[4];
      uint32_t	_uint[4];
      int64_t	_long[2];
      uint64_t	_ulong[2];
      float 	_float[4];
      double 	_double[2];
      };
   union const16_u *const16 = (union const16_u *)buf;

   switch (elemNode->getDataType())
      {
      case TR_Bool:
      case TR_SInt8:
         for (i = 0; i < 16; i++)
            const16->_byte[i] = elemNode->getByte();
         break;
      case TR_UInt8:
         for (i = 0; i < 16; i++)
            const16->_ubyte[i] = elemNode->getUnsignedByte();
         break;
      case TR_SInt16:
         for (i = 0; i < 8; i++)
            const16->_short[i] = elemNode->getShortInt();
         break;
      case TR_UInt16:
         for (i = 0; i < 8; i++)
            const16->_ushort[i] = elemNode->getConst<uint16_t>();
         break;
      case TR_SInt32:
         for (i = 0; i < 4; i++)
            const16->_int[i] = elemNode->getInt();
         break;
      case TR_UInt32:
         for (i = 0; i < 4; i++)
            const16->_int[i] = elemNode->getUnsignedInt();
         break;
      case TR::Address:
         if (TR::Compiler->target.is64Bit())
            for (i = 0; i < 2; i++)
               const16->_long[i] = (int64_t)elemNode->getAddress();
         else
            for (i = 0; i < 4; i++)
               const16->_int[i] = (int32_t)elemNode->getAddress();
         break;
      case TR_SInt64:
         for (i = 0; i < 2; i++)
            const16->_long[i] = elemNode->getLongInt();
         break;
      case TR_UInt64:
         for (i = 0; i < 2; i++)
            const16->_ulong[i] = elemNode->getUnsignedLongInt();
         break;
      case TR::Float:
         for (i = 0; i < 4; i++)
            const16->_float[i] = elemNode->getFloat();
      case TR::Double:
         for (i = 0; i < 2; i++)
            const16->_double[i] = elemNode->getDouble();
      default:
         TR_ASSERT(0, "Unexpected element type");
         break;
      }
   }
#endif

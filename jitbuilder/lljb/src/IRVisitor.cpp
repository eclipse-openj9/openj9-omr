/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "lljb/IRVisitor.hpp"

#include "ilgen/TypeDictionary.hpp"
#include "ilgen/BytecodeBuilder.hpp"

#include "llvm/Support/raw_ostream.h"

#include <cassert>
#include <vector>

namespace lljb
{

void
IRVisitor::visitInstruction(llvm::Instruction &I)
   {
   llvm::outs() << "Unimplemented instruction being visited: " << I << "\n";
   assert(0 && "Unimplemented instruction!");
   }

void
IRVisitor::visitReturnInst(llvm::ReturnInst &I)
   {
   if (!I.getNumOperands())
      {
      _builder->Return();
      }
   else
      {
      llvm::Value * value = I.getOperand(0);
      TR::IlValue * ilValue = getIlValue(value);
      _builder->Return(ilValue);
      }
   }

void
IRVisitor::visitAllocaInst(llvm::AllocaInst &I)
   {
   TR::IlValue * ilValue = nullptr;
   if (I.getAllocatedType()->isStructTy())
      {
      llvm::StructType * llvmStruct = llvm::dyn_cast<llvm::StructType>(I.getAllocatedType());
      ilValue = _builder->CreateLocalStruct(_methodBuilder->getIlType(_td,llvmStruct));
      unsigned elIndex = 0;
      for (auto elIter = llvmStruct->element_begin();
            elIter != llvmStruct->element_end(); ++elIter)
         {
         llvm::Type * fieldType = llvmStruct->getElementType(elIndex);
         if (fieldType->isArrayTy())
            {
            TR::IlValue * arrayField = _builder->CreateLocalArray(
               fieldType->getArrayNumElements(),
               _methodBuilder->getIlType(_td,fieldType));
            _builder->StoreAt(
               _builder->StructFieldInstanceAddress(
                  llvmStruct->getStructName().data(),
                  _methodBuilder->getMemberNameFromIndex(elIndex),
                  ilValue),
               arrayField);
            }
         elIndex++;
         }
      }
   else if (I.getAllocatedType()->isArrayTy())
      {
      llvm::Type * type = I.getAllocatedType();
      unsigned numElements = type->getArrayNumElements();
      type = type->getArrayElementType();
      while (1)
         {
         if (type->isArrayTy())
            {
            numElements *= type->getArrayNumElements();
            type = type->getArrayElementType();
            }
         else
            {
            break;
            }
         }
      ilValue = _builder->CreateLocalArray(
                           numElements,
                           _methodBuilder->getIlType(_td,type));
      }
   else
      {
      _methodBuilder->allocateLocal(
      &I,
      _methodBuilder->getIlType(_td, I.getAllocatedType()));
      }
   _methodBuilder->mapIRtoIlValue(&I, ilValue);
   }

void
IRVisitor::visitLoadInst(llvm::LoadInst &I)
   {
   llvm::Value * source = I.getPointerOperand();
   TR::IlValue * loadedVal = nullptr;
   if (_methodBuilder->isIndirectLoadOrStore(source))
      {
      TR::IlValue * ilSrc = getIlValue(source);
      loadedVal = _builder->LoadAt(
                              _td->PointerTo(_methodBuilder->getIlType(_td,I.getType())),
                              _builder->
                              IndexAt(
                                 _td->PointerTo(_methodBuilder->getIlType(_td,I.getType())),
                                 ilSrc,
                                 _builder->ConstInt32(0)));
      }
   else
      {
      loadedVal = _builder->Load(_methodBuilder->getLocalNameFromValue(source));
      }
   _methodBuilder->mapIRtoIlValue(&I, loadedVal);
   }

void
IRVisitor::visitStoreInst(llvm::StoreInst &I)
   {
   llvm::Value * dest = I.getPointerOperand();
   llvm::Value * value = I.getOperand(0);
   TR::IlValue * ilValue = getIlValue(value);
   if (_methodBuilder->isIndirectLoadOrStore(dest))
      {
      _builder->StoreAt(
               _builder->IndexAt(
                  _td->PointerTo(_methodBuilder->getIlType(_td,value->getType())),
                  _methodBuilder->getIlValue(dest),
                  _builder->ConstInt32(0)),
               ilValue);
      }
   else
      {
      _builder->Store(_methodBuilder->getLocalNameFromValue(dest), ilValue);
      }
   }

void
IRVisitor::visitBinaryOperator(llvm::BinaryOperator &I)
   {
   TR::IlValue * lhs = getIlValue(I.getOperand(0));
   TR::IlValue * rhs = getIlValue(I.getOperand(1));
   TR::IlValue * result = nullptr;
   switch (I.getOpcode())
      {
      case llvm::Instruction::BinaryOps::Add:
      case llvm::Instruction::BinaryOps::FAdd:
         result = _builder->Add(lhs,rhs);
         break;
      case llvm::Instruction::BinaryOps::Sub:
      case llvm::Instruction::BinaryOps::FSub:
         result = _builder->Sub(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::Mul:
      case llvm::Instruction::BinaryOps::FMul:
         result = _builder->Mul(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::SDiv:
      case llvm::Instruction::BinaryOps::FDiv:
      case llvm::Instruction::BinaryOps::UDiv:
         result = _builder->Div(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::Shl:
         if (!I.getOperand(1)->getType()->isIntegerTy(32))
               rhs = _builder->ConvertTo(_td->Int32, rhs);
         result = _builder->ShiftL(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::AShr:
         if (!I.getOperand(1)->getType()->isIntegerTy(32))
               rhs = _builder->ConvertTo(_td->Int32, rhs);
         result = _builder->ShiftR(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::LShr:
         if (!I.getOperand(1)->getType()->isIntegerTy(32))
               rhs = _builder->ConvertTo(_td->Int32, rhs);
         result = _builder->UnsignedShiftR(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::And:
         result = _builder->And(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::Or:
         result = _builder->Or(lhs, rhs);
         break;
      case llvm::Instruction::BinaryOps::Xor:
         result = _builder->Xor(lhs, rhs);
         break;
      default:
         llvm::outs() << "Instruction being visited: " << I << "\n";
         assert(0 && "Unknown binary operand");
         break;
      }
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

void
IRVisitor::visitCmpInst(llvm::CmpInst &I)
   {
   TR::IlValue * lhs = getIlValue(I.getOperand(0));
   TR::IlValue * rhs = getIlValue(I.getOperand(1));

   TR::IlValue * result = nullptr;

   switch (I.getPredicate())
      {
      case llvm::CmpInst::Predicate::ICMP_EQ:
      case llvm::CmpInst::Predicate::FCMP_OEQ:
      case llvm::CmpInst::Predicate::FCMP_UEQ:
         result = _builder->EqualTo(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_NE:
      case llvm::CmpInst::Predicate::FCMP_ONE:
      case llvm::CmpInst::Predicate::FCMP_UNE:
         result = _builder->NotEqualTo(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_UGT:
         result = _builder->UnsignedGreaterThan(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_UGE:
         result = _builder->UnsignedGreaterOrEqualTo(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_ULT:
         result = _builder->UnsignedLessThan(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_ULE:
         result = _builder->UnsignedLessOrEqualTo(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_SGT:
      case llvm::CmpInst::Predicate::FCMP_OGT:
      case llvm::CmpInst::Predicate::FCMP_UGT:
         result = _builder->GreaterThan(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_SGE:
      case llvm::CmpInst::Predicate::FCMP_OGE:
      case llvm::CmpInst::Predicate::FCMP_UGE:
         result = _builder->GreaterOrEqualTo(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_SLT:
      case llvm::CmpInst::Predicate::FCMP_OLT:
      case llvm::CmpInst::Predicate::FCMP_ULT:
         result = _builder->LessThan(lhs, rhs);
         break;
      case llvm::CmpInst::Predicate::ICMP_SLE:
      case llvm::CmpInst::Predicate::FCMP_OLE:
      case llvm::CmpInst::Predicate::FCMP_ULE:
         result = _builder->LessOrEqualTo(lhs, rhs);
         break;
      default:
         assert(0 && "Unknown CmpInst predicate");
         break;
      }
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

void
IRVisitor::visitBranchInst(llvm::BranchInst &I)
   {
   if (I.isUnconditional())
      {
      llvm::Value * dest = I.getSuccessor(0);
      TR::BytecodeBuilder * destBuilder = _methodBuilder->getByteCodeBuilder(dest);
      assert(destBuilder && "failed to find builder for target basic block");
      _builder->Goto(destBuilder);
      }
   else
      {
      TR::IlValue * condition = getIlValue(I.getCondition());
      TR::IlBuilder * ifTrue = _methodBuilder->getByteCodeBuilder(I.getSuccessor(0));
      TR::IlBuilder * ifFalse = _methodBuilder->getByteCodeBuilder(I.getSuccessor(1));
      assert(ifTrue && ifFalse && condition && "Failed to find destination blocks for ifThenElse");
      _builder->IfThenElse(&ifTrue, &ifFalse, condition);
      }
   }

void
IRVisitor::visitCallInst(llvm::CallInst &I)
   {
   TR::IlValue * result = nullptr;
   llvm::Function * callee = I.getCalledFunction();
   const char * calleeName = callee->getName().data();
   std::size_t numParams = I.arg_size();
   std::vector<TR::IlValue *> params;
   std::vector<TR::IlType *> paramTypes;
   for (int i = 0; i < numParams; i++)
      {
      TR::IlValue * parameter = getIlValue(I.getArgOperand(i));
      params.push_back(parameter);
      paramTypes.push_back(_methodBuilder->getIlType(_td,I.getArgOperand(i)->getType()));
      }
   _methodBuilder->defineFunction(callee,numParams, paramTypes.data());
   result = _builder->Call(calleeName, numParams, params.data());
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

void
IRVisitor::visitCastInst(llvm::CastInst &I)
   {
   TR::IlValue * srcVal = getIlValue(I.getOperand(0));
   TR::IlType * toIlType = _methodBuilder->getIlType(_td,I.getDestTy());
   TR::IlValue * result = _builder->ConvertTo(toIlType, srcVal);
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

void
IRVisitor::visitZExtInst(llvm::ZExtInst &I)
   {
   TR::IlValue * srcVal = getIlValue(I.getOperand(0));
   TR::IlType * toIlType = _methodBuilder->getIlType(_td,I.getDestTy());
   TR::IlValue * result = _builder->UnsignedConvertTo(toIlType, srcVal);
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

void
IRVisitor::visitGetElementPtrInst(llvm::GetElementPtrInst &I)
   {
   TR::IlValue * ilValue = nullptr;
   if (I.getSourceElementType()->isStructTy())
      {
      llvm::ConstantInt * indextConstantInt = llvm::dyn_cast<llvm::ConstantInt>(I.getOperand(2));
      unsigned elementIndex = indextConstantInt->getZExtValue();
      ilValue = _builder->StructFieldInstanceAddress(I.getSourceElementType()->getStructName().data(),
                                                               _methodBuilder->getMemberNameFromIndex(elementIndex),
                                                               getIlValue(I.getOperand(0)));
      }
   else if (I.getSourceElementType()->isArrayTy())
      {
      if (I.getSourceElementType()->getArrayElementType()->isArrayTy())
         {
         llvm::ConstantInt * indextConstantInt = llvm::dyn_cast<llvm::ConstantInt>(I.getOperand(2));
         int64_t elementIndex = indextConstantInt->getSExtValue();
         elementIndex *= I.getSourceElementType()->getArrayNumElements();
         ilValue =
               _builder->IndexAt(
                  _td->PointerTo(
                     _methodBuilder->getIlType(_td,I.getSourceElementType()->getArrayElementType())),
                  getIlValue(I.getOperand(0)),
                  _builder->ConstInt32(elementIndex));
         }
      else
         {
         TR::IlValue * elementIndex = getIlValue(I.getOperand(2));
         ilValue =
               _builder->IndexAt(
                  _td->PointerTo(
                     _methodBuilder->getIlType(_td,I.getSourceElementType()->getArrayElementType())),
                  getIlValue(I.getOperand(0)),
                  elementIndex);
         }
      }
   else
      {
      assert((I.getNumOperands() == 2) && "unhandled getElementPtr case");
      ilValue = _builder->IndexAt(
         _td->PointerTo(
               _methodBuilder->getIlType(_td, I.getSourceElementType())),
         getIlValue(I.getOperand(0)),
         getIlValue(I.getOperand(1)));
      }
   _methodBuilder->mapIRtoIlValue(&I, ilValue);
   }

void
IRVisitor::visitPHINode(llvm::PHINode &I)
   {
   unsigned incomingEdgeCount = I.getNumIncomingValues();
   llvm::DenseMap<llvm::BasicBlock *, TR::IlValue *> valueMap;
   TR::IlValue * ilValue = nullptr;
   llvm::ConstantInt * firstCondition = llvm::dyn_cast<llvm::ConstantInt>(I.getIncomingValue(0));
   unsigned isOr = firstCondition->getZExtValue();

   for (unsigned i = 0; i < incomingEdgeCount; i++)
      {
      llvm::BasicBlock * basicBlock = I.getIncomingBlock(i);
      llvm::Value * value = I.getIncomingValue(i);
      if (value->getValueID() == llvm::Value::ValueTy::ConstantIntVal)
         {
         llvm::BranchInst * branchInst = llvm::dyn_cast<llvm::BranchInst>(basicBlock->getTerminator());
         value = branchInst->getCondition();
         }
      valueMap[basicBlock] = getIlValue(value);
      }
   if (isOr) ilValue = _builder->Or(valueMap[I.getIncomingBlock(0)],valueMap[I.getIncomingBlock(1)]);
   else      ilValue = _builder->And(valueMap[I.getIncomingBlock(0)],valueMap[I.getIncomingBlock(1)]);

   if (incomingEdgeCount > 2)
      {
      unsigned currentCaseIndex = 2;
      while (currentCaseIndex < incomingEdgeCount)
         {
         if (isOr) ilValue = _builder->Or(ilValue, valueMap[I.getIncomingBlock(currentCaseIndex)]);
         else      ilValue = _builder->And(ilValue, valueMap[I.getIncomingBlock(currentCaseIndex)]);
         currentCaseIndex++;
         }
      }
   _methodBuilder->mapIRtoIlValue(&I, ilValue);
   }

void
IRVisitor::visitSelectInst(llvm::SelectInst &I)
   {
   TR::IlValue * condition = getIlValue(I.getCondition());
   TR::IlValue * ifTrue = getIlValue(I.getTrueValue());
   TR::IlValue * ifFalse = getIlValue(I.getFalseValue());
   TR::IlValue * result = _builder->Select(condition, ifTrue, ifFalse);
   _methodBuilder->mapIRtoIlValue(&I, result);
   }

TR::IlValue *
IRVisitor::createConstIntIlValue(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::ConstantInt * constInt = llvm::dyn_cast<llvm::ConstantInt>(value);
   int64_t signExtendedValue = constInt->getSExtValue();
   if (constInt->getBitWidth() <= 8) ilValue = _builder->ConstInt8(signExtendedValue);
   else if (constInt->getBitWidth() <= 16) ilValue = _builder->ConstInt16(signExtendedValue);
   else if (constInt->getBitWidth() <= 32) ilValue = _builder->ConstInt32(signExtendedValue);
   else if (constInt->getBitWidth() <= 64) ilValue = _builder->ConstInt64(signExtendedValue);

   return ilValue;
   }

TR::IlValue *
IRVisitor::createConstFPIlValue(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::ConstantFP * constFP = llvm::dyn_cast<llvm::ConstantFP>(value);
   llvm::APFloat apf = constFP->getValueAPF();
   if (apf.getSizeInBits(apf.getSemantics()) <= 32)
      {
      float rawValue = apf.convertToFloat();
      ilValue = _builder->ConstFloat(rawValue);
      }
   else
      {
      double rawValue = apf.convertToDouble();
      ilValue = _builder->ConstDouble(rawValue);
      }

   return ilValue;
}

TR::IlValue *
IRVisitor::loadParameter(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::Argument * arg = llvm::dyn_cast<llvm::Argument>(value);
   ilValue = _builder->Load(_methodBuilder->getParamNameFromIndex(arg->getArgNo()));
   return ilValue;
   }

TR::IlValue *
IRVisitor::loadGlobal(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::GlobalVariable * globalVar = llvm::dyn_cast<llvm::GlobalVariable>(value);
   ilValue = getIlValue(globalVar->getInitializer());
   return ilValue;
   }

TR::IlValue * IRVisitor::createConstExprIlValue(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::ConstantExpr * constExpr = llvm::dyn_cast<llvm::ConstantExpr>(value);
   ilValue = getIlValue(constExpr->getOperand(0));
   return ilValue;
   }

TR::IlValue *
IRVisitor::createConstantDataArrayVal(llvm::Value * value)
   {
   TR::IlValue * ilValue = nullptr;
   llvm::ConstantDataArray * constDataArray = llvm::dyn_cast<llvm::ConstantDataArray>(value);
   ilValue = _builder->ConstString(constDataArray->getAsCString().data());
   return ilValue;
   }

TR::IlValue *
IRVisitor::getIlValue(llvm::Value * value)
   {
   TR::IlValue * ilValue = _methodBuilder->getIlValue(value);
   if (ilValue) return ilValue;
   switch (value->getValueID())
      {
      /* Constants */
      case llvm::Value::ValueTy::ConstantExprVal:
         ilValue = createConstExprIlValue(value);
         break;
      case llvm::Value::ValueTy::GlobalVariableVal:
         ilValue = loadGlobal(value);
         break;
      case llvm::Value::ValueTy::FunctionVal:
      case llvm::Value::ValueTy::GlobalAliasVal:
      case llvm::Value::ValueTy::GlobalIFuncVal:
      case llvm::Value::ValueTy::BlockAddressVal:
         assert(0 && "Unsupported constant value type");
         break;

      /* Constant data value types */
      case llvm::Value::ValueTy::ConstantIntVal:
         ilValue = createConstIntIlValue(value);
         break;
      case llvm::Value::ValueTy::ConstantFPVal:
         ilValue = createConstFPIlValue(value);
         break;
      case llvm::Value::ValueTy::ConstantDataArrayVal:
         ilValue = createConstantDataArrayVal(value);
         break;
      case llvm::Value::ValueTy::ConstantAggregateZeroVal:
      case llvm::Value::ValueTy::ConstantDataVectorVal:
      case llvm::Value::ValueTy::ConstantPointerNullVal:
      case llvm::Value::ValueTy::ConstantTokenNoneVal:
         assert(0 && "Unsupported constant data value type");
         break;

      /* Constant aggregate value types */
      case llvm::Value::ValueTy::ConstantStructVal:
         //break; //todo uncomment when done implementing
      case llvm::Value::ValueTy::ConstantArrayVal:
      case llvm::Value::ValueTy::ConstantVectorVal:
         assert(0 && "Unsupported constant aggregate value type");
         break;

      /* Other value types */
      case llvm::Value::ValueTy::ArgumentVal:
         ilValue = loadParameter(value);
         break;
      case llvm::Value::ValueTy::BasicBlockVal:
         assert(0 && "Basicblock should not be looked up this way");
         break;

      default:
         break;
      }
   assert (ilValue && "failed to retrieve ilValue from llvm Value!");
   return ilValue;
   }

} // namespace lljb

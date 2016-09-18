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

#include "x/codegen/SubtractAnalyser.hpp"

#include <stdint.h>                         // for uint8_t
#include "codegen/Analyser.hpp"             // for NUM_ACTIONS
#include "codegen/CodeGenerator.hpp"        // for CodeGenerator, NEED_CC
#include "codegen/MemoryReference.hpp"
#include "codegen/Register.hpp"             // for Register
#include "codegen/RegisterConstants.hpp"
#include "codegen/RegisterPair.hpp"         // for RegisterPair
#include "codegen/TreeEvaluator.hpp"        // for TR_X86ComputeCC, etc
#include "codegen/X86Evaluator.hpp"
#include "compile/Compilation.hpp"          // for isSMP
#include "il/ILOpCodes.hpp"                 // for ILOpCodes::iconst, etc
#include "il/ILOps.hpp"                     // for ILOpCode
#include "il/Node.hpp"                      // for Node
#include "il/Node_inlines.hpp"              // for Node::getSecondChild, etc
#include "il/Symbol.hpp"                    // for Symbol
#include "il/SymbolReference.hpp"           // for SymbolReference
#include "infra/Assert.hpp"                 // for TR_ASSERT
#include "codegen/X86Instruction.hpp"
#include "x/codegen/X86Ops.hpp"             // for TR_X86OpCodes, etc

void TR_X86SubtractAnalyser::integerSubtractAnalyser(TR::Node       *root,
                                                      TR_X86OpCodes regRegOpCode,
                                                      TR_X86OpCodes regMemOpCode,
                                                      TR_X86OpCodes copyOpCode,
                                                      bool needsEflags, // false by default
                                                      TR::Node *borrow)  // 0 by default
   {
   // *this    swipeable for debugging purposes
   TR::Node *firstChild;
   TR::Node *secondChild;
   firstChild  = root->getFirstChild();
   secondChild = root->getSecondChild();
   TR::Register *firstRegister  = firstChild->getRegister();
   TR::Register *secondRegister = secondChild->getRegister();

   setInputs(firstChild, firstRegister, secondChild, secondRegister);

   bool loadedConst = false;

   needsEflags = needsEflags || NEED_CC(root);

   if (getEvalChild1())
      {
      // if firstChild and secondChild are the same node, then we should
      // evaluate (take the else path) so that the evaluate for the secondChild
      // below will get the correct/already-allocated register.
      if (firstRegister == 0 && firstChild->getOpCodeValue() == TR::iconst && (firstChild != secondChild))
         {
	   // An iconst may have to be generated.  The iconst will be generated after the
	   //    secondChild is evaluated.  Set the loadedConst flag to true.
	   loadedConst = true;
         }
      else
         {
         firstRegister = _cg->evaluate(firstChild);
         }
      }

   if (getEvalChild2())
      {
      secondRegister = _cg->evaluate(secondChild);
      if (firstChild->getRegister())
         {
         firstRegister = firstChild->getRegister();
         }
      else if (!loadedConst)
         {
         firstRegister = _cg->evaluate(firstChild);
         }
      }

   if (loadedConst)
      {
      if (firstRegister == 0)
         {
         // firstchild is an inconst and it has not been evaluated.
         //   Generate the code for an iconst.
         firstRegister = _cg->allocateRegister();
         TR::TreeEvaluator::insertLoadConstant(firstChild, firstRegister, firstChild->getInt(), TR_RematerializableInt, _cg);
         }
      else
         {
         // firstchild was evaluated.  The code for an iconst does not need to be generated.
         //   Set the loadConst flag to false.
         loadedConst = false;
         }
      }

   if (borrow != 0)
      TR_X86ComputeCC::setCarryBorrow(borrow, true, _cg);

   if (getCopyReg1())
      {
      if (firstChild->getReferenceCount() > 1)
         {
         TR::Register *thirdReg;
         if (firstChild->getOpCodeValue() == TR::iconst && loadedConst)
            {
            thirdReg = firstRegister;
            }
         else
            {
            if (secondChild->getReferenceCount() == 1 && secondRegister != 0 && !needsEflags && (borrow == 0))
               {
               // use one fewer registers if we negate the clobberable secondRegister and add
               // Don't do this though if condition codes are needed.  The sequence
               // depends on the carry flag being valid as if a sub was done.
               //
               bool nodeIs64Bit = TR_X86OpCode::hasLongSource(regRegOpCode);
               generateRegInstruction(NEGReg(nodeIs64Bit), secondChild, secondRegister, _cg);
               thirdReg       = secondRegister;
               secondRegister = firstRegister;
               regRegOpCode   = ADDRegReg(nodeIs64Bit);
               }
            else
               {
               thirdReg = _cg->allocateRegister();
               generateRegRegInstruction(copyOpCode, root, thirdReg, firstRegister, _cg);
               }
            }
         root->setRegister(thirdReg);
         if (getSubReg3Reg2())
            {
            generateRegRegInstruction(regRegOpCode, root, thirdReg, secondRegister, _cg);
            }
         else // assert getSubReg3Mem2() == true
            {
            TR::MemoryReference  *tempMR = generateX86MemoryReference(secondChild, _cg);
            generateRegMemInstruction(regMemOpCode, root, thirdReg, tempMR, _cg);
            tempMR->decNodeReferenceCounts(_cg);
            }
         }
      else
         {
         if (getSubReg3Reg2())
            {
            generateRegRegInstruction(regRegOpCode, root, firstRegister, secondRegister, _cg);
            }
         else // assert getSubReg3Mem2() == true
            {
            TR::MemoryReference  *tempMR = generateX86MemoryReference(secondChild, _cg);
            generateRegMemInstruction(regMemOpCode, root, firstRegister, tempMR, _cg);
            tempMR->decNodeReferenceCounts(_cg);
            }
         root->setRegister(firstRegister);
         }
      }
   else if (getSubReg1Reg2())
      {
      generateRegRegInstruction(regRegOpCode, root, firstRegister, secondRegister, _cg);
      root->setRegister(firstRegister);
      }
   else // assert getSubReg1Mem2() == true
      {
      TR::MemoryReference  *tempMR = generateX86MemoryReference(secondChild, _cg);
      generateRegMemInstruction(regMemOpCode, root, firstRegister, tempMR, _cg);
      root->setRegister(firstRegister);
      tempMR->decNodeReferenceCounts(_cg);
      }

   _cg->decReferenceCount(firstChild);
   _cg->decReferenceCount(secondChild);
   }

// Volatile memory operands are not allowed in long subtractions
// if we are compiling for an SMP machine, as the carry flag can
// get clobbered by the memory barrier immediately preceding the
// SBB4RegMem instruction.
//
static bool isVolatileMemoryOperand(TR::Node *node)
   {
   if (TR::Compiler->target.isSMP() && node->getOpCode().isMemoryReference())
      {
      TR_ASSERT(node->getSymbolReference(), "expecting a symbol reference\n");
      TR::Symbol *sym = node->getSymbolReference()->getSymbol();
      return (sym && sym->isVolatile());
      }
   return false;
   }

void TR_X86SubtractAnalyser::longSubtractAnalyser(TR::Node *root)
   {
   // *this    swipeable for debugging purposes
   TR::Node *firstChild;
   TR::Node *secondChild;
   firstChild  = root->getFirstChild();
   secondChild = root->getSecondChild();
   TR::Register *firstRegister  = firstChild->getRegister();
   TR::Register *secondRegister = secondChild->getRegister();

   bool firstHighZero      = false;
   bool secondHighZero     = false;
   bool useSecondHighOrder = false;

   TR_X86OpCodes regRegOpCode = SUB4RegReg;
   TR_X86OpCodes regMemOpCode = SUB4RegMem;

   bool needsEflags = NEED_CC(root) || (root->getOpCodeValue() == TR::lusubb);

   // Can generate better code for long adds when one or more children have a high order zero word
   // can avoid the evaluation when we don't need the result of such nodes for another parent.
   //
   if (firstChild->isHighWordZero() && !needsEflags)
      {
      firstHighZero = true;
      }

   if (secondChild->isHighWordZero() && !needsEflags)
      {
      secondHighZero = true;
      TR::ILOpCodes secondOp = secondChild->getOpCodeValue();
      if (secondChild->getReferenceCount() == 1 && secondRegister == 0)
         {
         if (secondOp == TR::iu2l || secondOp == TR::su2l ||
             secondOp == TR::bu2l ||
             (secondOp == TR::lushr &&
              secondChild->getSecondChild()->getOpCodeValue() == TR::iconst &&
              (secondChild->getSecondChild()->getInt() & TR::TreeEvaluator::shiftMask(true)) == 32))
            {
            secondChild    = secondChild->getFirstChild();
            secondRegister = secondChild->getRegister();
            if (secondOp == TR::lushr)
               {
               useSecondHighOrder = true;
               }
            }
         }
      }

   setInputs(firstChild, firstRegister, secondChild, secondRegister);

   if (isVolatileMemoryOperand(firstChild))
      resetMem1();

   if (isVolatileMemoryOperand(secondChild))
      resetMem2();

   if (getEvalChild1())
      {
      firstRegister = _cg->evaluate(firstChild);
      }

   if (getEvalChild2())
      {
      secondRegister = _cg->evaluate(secondChild);
      }

   if (secondHighZero && secondRegister && secondRegister->getRegisterPair())
      {
      if (!useSecondHighOrder)
         {
         secondRegister = secondRegister->getLowOrder();
         }
      else
         {
         secondRegister = secondRegister->getHighOrder();
         }
      }

   if (root->getOpCodeValue() == TR::lusubb &&
       TR_X86ComputeCC::setCarryBorrow(root->getChild(2), true, _cg))
      {
      // use SBB rather than SUB
      //
      regRegOpCode = SBB4RegReg;
      regMemOpCode = SBB4RegMem;
      }

   if (getCopyReg1())
      {
      TR::Register     *lowThird  = _cg->allocateRegister();
      TR::Register     *highThird = _cg->allocateRegister();
      TR::RegisterPair *thirdReg  = _cg->allocateRegisterPair(lowThird, highThird);
      root->setRegister(thirdReg);
      generateRegRegInstruction(MOV4RegReg, root, lowThird, firstRegister->getLowOrder(), _cg);

      if (firstHighZero)
         {
         generateRegRegInstruction(XOR4RegReg, root, highThird, highThird, _cg);
         }
      else
         {
         generateRegRegInstruction(MOV4RegReg, root, highThird, firstRegister->getHighOrder(), _cg);
         }

      if (getSubReg3Reg2())
         {
         if (secondHighZero)
            {
            generateRegRegInstruction(regRegOpCode, root, lowThird, secondRegister, _cg);
            generateRegImmInstruction(SBB4RegImms, root, highThird, 0, _cg);
            }
         else
            {
            generateRegRegInstruction(regRegOpCode, root, lowThird, secondRegister->getLowOrder(), _cg);
            generateRegRegInstruction(SBB4RegReg, root, highThird, secondRegister->getHighOrder(), _cg);
            }
         }
      else // assert getSubReg3Mem2() == true
         {
         TR::MemoryReference  *lowMR = generateX86MemoryReference(secondChild, _cg);
         /**
          * The below code is needed to ensure correct behaviour when the subtract analyser encounters a lushr bytecode that shifts
          * by 32 bits. This is the only case where the useSecondHighOrder bit is set.
          * When the first child of the lushr is in a register, code above handles the shift. When the first child of the lushr is in
          * memory, the below ensures that the upper part of the first child of the lushr is used as lowMR.
          */
         if (useSecondHighOrder)
            {
            TR_ASSERT(secondHighZero, "useSecondHighOrder should be consistent with secondHighZero. useSecondHighOrder subsumes secondHighZero");
        	   lowMR = generateX86MemoryReference(*lowMR, 4, _cg);
            }

         generateRegMemInstruction(regMemOpCode, root, lowThird, lowMR, _cg);
         if (secondHighZero)
            {
            generateRegImmInstruction(SBB4RegImms, root, highThird, 0, _cg);
            }
         else
            {
            TR::MemoryReference  *highMR = generateX86MemoryReference(*lowMR, 4, _cg);
            generateRegMemInstruction(SBB4RegMem, root, highThird, highMR, _cg);
            }
         lowMR->decNodeReferenceCounts(_cg);
         }
      }
   else if (getSubReg1Reg2())
      {
      if (secondHighZero)
         {
         generateRegRegInstruction(regRegOpCode, root, firstRegister->getLowOrder(), secondRegister, _cg);
         generateRegImmInstruction(SBB4RegImms, root, firstRegister->getHighOrder(), 0, _cg);
         }
      else
         {
         generateRegRegInstruction(regRegOpCode, root, firstRegister->getLowOrder(), secondRegister->getLowOrder(), _cg);
         generateRegRegInstruction(SBB4RegReg, root, firstRegister->getHighOrder(), secondRegister->getHighOrder(), _cg);
         }
      root->setRegister(firstRegister);
      }
   else // assert getSubReg1Mem2() == true
      {
      TR::MemoryReference  *lowMR = generateX86MemoryReference(secondChild, _cg);
      /**
       * The below code is needed to ensure correct behaviour when the subtract analyser encounters a lushr bytecode that shifts
       * by 32 bits. This is the only case where the useSecondHighOrder bit is set.
       * When the first child of the lushr is in a register, code above handles the shift. When the first child of the lushr is in
       * memory, the below ensures that the upper part of the first child of the lushr is used as lowMR.
       */
      if (useSecondHighOrder)
     	 lowMR = generateX86MemoryReference(*lowMR, 4, _cg);

      generateRegMemInstruction(regMemOpCode, root, firstRegister->getLowOrder(), lowMR, _cg);

      if (secondHighZero)
         {
         generateRegImmInstruction(SBB4RegImms, root, firstRegister->getHighOrder(), 0, _cg);
         }
      else
         {
         TR::MemoryReference  *highMR = generateX86MemoryReference(*lowMR, 4, _cg);
         generateRegMemInstruction(SBB4RegMem, root, firstRegister->getHighOrder(), highMR, _cg);
         }

      root->setRegister(firstRegister);
      lowMR->decNodeReferenceCounts(_cg);
      }

   _cg->decReferenceCount(firstChild);
   _cg->decReferenceCount(secondChild);
   }

const uint8_t TR_X86SubtractAnalyser::_actionMap[NUM_ACTIONS] =
   {                  // Reg1 Mem1 Clob1 Reg2 Mem2 Clob2
   EvalChild1 |       //  0    0     0    0    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    0    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    0    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    0    1     1
   CopyReg1   |
   SubReg3Mem2,

   EvalChild1 |       //  0    0     0    1    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    1    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    1    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     0    1    1     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    0     1    0    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    0    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    0    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    0    1     1
   SubReg1Mem2,

   EvalChild1 |       //  0    0     1    1    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    1    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    1    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    0     1    1    1     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     0    0    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    0    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    0    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    0    1     1
   CopyReg1   |
   SubReg3Mem2,

   EvalChild1 |       //  0    1     0    1    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    1    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    1    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     0    1    1     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  0    1     1    0    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    0    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    0    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    0    1     1
   SubReg1Mem2,

   EvalChild1 |       //  0    1     1    1    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    1    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    1    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  0    1     1    1    1     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     0    0    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    0    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    0    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    0    1     1
   CopyReg1   |
   SubReg3Mem2,

   EvalChild1 |       //  1    0     0    1    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    1    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    1    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     0    1    1     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    0     1    0    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    0    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    0    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    0    1     1
   SubReg1Mem2,

   EvalChild1 |       //  1    0     1    1    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    1    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    1    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    0     1    1    1     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     0    0    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    0    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    0    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    0    1     1
   CopyReg1   |
   SubReg3Mem2,

   EvalChild1 |       //  1    1     0    1    0     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    1    0     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    1    1     0
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     0    1    1     1
   EvalChild2 |
   CopyReg1   |
   SubReg3Reg2,

   EvalChild1 |       //  1    1     1    0    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    0    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    0    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    0    1     1
   SubReg1Mem2,

   EvalChild1 |       //  1    1     1    1    0     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    1    0     1
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    1    1     0
   EvalChild2 |
   SubReg1Reg2,

   EvalChild1 |       //  1    1     1    1    1     1
   EvalChild2 |
   SubReg1Reg2
   };

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

#ifndef S390COMPAREANALYSER_INCL
#define S390COMPAREANALYSER_INCL

#include <stdint.h>                   // for uint8_t
#include "codegen/Analyser.hpp"       // for NUM_ACTIONS, TR_Analyser
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "codegen/InstOpCode.hpp"     // for InstOpCode, etc

namespace TR { class LabelSymbol; }
namespace TR { class Machine; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }

#define EvalChild1   0x01
#define EvalChild2   0x02
#define CmpReg1Reg2  0x04
#define CmpReg1Mem2  0x08
#define CmpMem1Reg2  0x10

class TR_S390CompareAnalyser : public TR_Analyser
   {

   static const uint8_t _actionMap[NUM_ACTIONS];
   TR::CodeGenerator    *_cg;
   TR::Machine *_machine;

   public:

   TR_S390CompareAnalyser(TR::CodeGenerator *cg)
      : _cg(cg), _machine(cg->machine())
      {}

   void integerCompareAnalyser(TR::Node       *root,
                               TR::InstOpCode::Mnemonic regRegOpCode,
                               TR::InstOpCode::Mnemonic regMemOpCode,
                               TR::InstOpCode::Mnemonic memRegOpCode);

   TR::RegisterDependencyConditions*
        longOrderedCompareAndBranchAnalyser(TR::Node       *root,
                                            TR::InstOpCode::Mnemonic branchOpCode,
                                            TR::InstOpCode::S390BranchCondition lowBranchCond,
                                            TR::InstOpCode::S390BranchCondition highBranchCond,
                                            TR::InstOpCode::S390BranchCondition highReversedBranchCond,
					    TR::LabelSymbol *trueLabel,
					    TR::LabelSymbol *falseLabel,
                                            bool &internalControlFlowStarted);

   void longEqualityCompareAndBranchAnalyser(TR::Node        *root,
                                             TR::LabelSymbol *firstBranchLabel,
                                             TR::LabelSymbol *secondBranchLabel,
                                             TR::InstOpCode::Mnemonic  secondBranchOp);

   TR::Register *longEqualityBooleanAnalyser(TR::Node       *root,
                                            TR::InstOpCode::Mnemonic setOpCode,
                                            TR::InstOpCode::Mnemonic combineOpCode);

   TR::Register *longOrderedBooleanAnalyser(TR::Node       *root,
                                           TR::InstOpCode::Mnemonic highSetOpCode,
                                           TR::InstOpCode::Mnemonic lowSetOpCode);

   TR::Register *longCMPAnalyser(TR::Node *root);


   bool getEvalChild1()  {return (_actionMap[getInputs()] & EvalChild1)  ? true : false;}
   bool getEvalChild2()  {return (_actionMap[getInputs()] & EvalChild2)  ? true : false;}
   bool getCmpReg1Reg2() {return (_actionMap[getInputs()] & CmpReg1Reg2) ? true : false;}
   bool getCmpReg1Mem2() {return (_actionMap[getInputs()] & CmpReg1Mem2) ? true : false;}
   bool getCmpMem1Reg2() {return (_actionMap[getInputs()] & CmpMem1Reg2) ? true : false;}

   };

#endif










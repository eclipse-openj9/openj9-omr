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

#ifndef IA32COMPAREANALYSER_INCL
#define IA32COMPAREANALYSER_INCL

#include <stdint.h>                   // for uint8_t
#include "codegen/Analyser.hpp"       // for NUM_ACTIONS, TR_Analyser
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "x/codegen/X86Ops.hpp"       // for TR_X86OpCodes

#define EvalChild1   0x01
#define EvalChild2   0x02
#define CmpReg1Reg2  0x04
#define CmpReg1Mem2  0x08
#define CmpMem1Reg2  0x10

namespace TR { class LabelSymbol; }
namespace TR { class Machine; }
namespace TR { class Node; }
namespace TR { class Register; }

class TR_X86CompareAnalyser  : public TR_Analyser
   {

   static const uint8_t _actionMap[NUM_ACTIONS];
   TR::CodeGenerator    *_cg;
   TR::Machine *_machine;

   public:

   TR_X86CompareAnalyser(TR::CodeGenerator *cg)
      : _cg(cg), _machine(cg->machine())
      {}

   void integerCompareAnalyser(
      TR::Node       *root,
      TR_X86OpCodes regRegOpCode,
      TR_X86OpCodes regMemOpCode,
      TR_X86OpCodes memRegOpCode);

   void integerCompareAnalyser(
      TR::Node       *root,
      TR::Node       *firstChild,
      TR::Node       *secondChild,
      bool           determineEvaluationOrder,
      TR_X86OpCodes  regRegOpCode,
      TR_X86OpCodes  regMemOpCode,
      TR_X86OpCodes  memRegOpCode);

   void longOrderedCompareAndBranchAnalyser(TR::Node       *root,
                                            TR_X86OpCodes lowBranchOpCode,
                                            TR_X86OpCodes highBranchOpCode,
                                            TR_X86OpCodes highReversedBranchOpCode);


   void longEqualityCompareAndBranchAnalyser(TR::Node        *root,
                                             TR::LabelSymbol *firstBranchLabel,
                                             TR::LabelSymbol *secondBranchLabel,
                                             TR_X86OpCodes  secondBranchOp);

   TR::Register *longEqualityBooleanAnalyser(TR::Node       *root,
                                            TR_X86OpCodes setOpCode,
                                            TR_X86OpCodes combineOpCode);

   TR::Register *longOrderedBooleanAnalyser(TR::Node       *root,
                                           TR_X86OpCodes highSetOpCode,
                                           TR_X86OpCodes lowSetOpCode);

   TR::Register *longCMPAnalyser(TR::Node *root);


   bool getEvalChild1()  {return (_actionMap[getInputs()] & EvalChild1)  ? true : false;}
   bool getEvalChild2()  {return (_actionMap[getInputs()] & EvalChild2)  ? true : false;}
   bool getCmpReg1Reg2() {return (_actionMap[getInputs()] & CmpReg1Reg2) ? true : false;}
   bool getCmpReg1Mem2() {return (_actionMap[getInputs()] & CmpReg1Mem2) ? true : false;}
   bool getCmpMem1Reg2() {return (_actionMap[getInputs()] & CmpMem1Reg2) ? true : false;}

   TR::CodeGenerator *cg() { return _cg; }
   };

#endif

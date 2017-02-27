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

#ifndef IA32SUBTRACTANALYSER_INCL
#define IA32SUBTRACTANALYSER_INCL

#include <stdint.h>              // for uint8_t
#include "codegen/Analyser.hpp"  // for NUM_ACTIONS, TR_Analyser
#include "x/codegen/X86Ops.hpp"  // for TR_X86OpCodes

namespace TR { class CodeGenerator; }
namespace TR { class Node; }

#define EvalChild1   0x01
#define EvalChild2   0x02
#define CopyReg1     0x04
#define SubReg1Reg2  0x08
#define SubReg3Reg2  0x10
#define SubReg1Mem2  0x20
#define SubReg3Mem2  0x40

class TR_X86SubtractAnalyser  : public TR_Analyser
   {

   static const uint8_t _actionMap[NUM_ACTIONS];
   TR::CodeGenerator    *_cg;

   public:

   TR_X86SubtractAnalyser(TR::CodeGenerator *cg)
      : _cg(cg)
      {}

   void integerSubtractAnalyser(TR::Node      *root,
                                TR_X86OpCodes regRegOpCode,
                                TR_X86OpCodes regMemOpCode,
                                TR_X86OpCodes copyOpCode,
                                bool needsEflags = false,
                                TR::Node *borrow = 0);

   void integerSubtractAnalyserWithExplicitOperands(TR::Node      *root,
                                                    TR::Node      *firstChild,
                                                    TR::Node      *secondChild,
                                                    TR_X86OpCodes regRegOpCode,
                                                    TR_X86OpCodes regMemOpCode,
                                                    TR_X86OpCodes copyOpCode,
                                                    bool needsEflags = false,
                                                    TR::Node *borrow = 0);

   TR::Register *integerSubtractAnalyserImpl(TR::Node      *root,
                                         TR::Node      *firstChild,
                                         TR::Node      *secondChild,
                                         TR_X86OpCodes regRegOpCode,
                                         TR_X86OpCodes regMemOpCode,
                                         TR_X86OpCodes copyOpCode,
                                         bool needsEflags,
                                         TR::Node *borrow);

   void longSubtractAnalyser(TR::Node *root);
   void longSubtractAnalyserWithExplicitOperands(TR::Node *root, TR::Node *firstChild, TR::Node *secondChild);
   TR::Register* longSubtractAnalyserImpl(TR::Node *root, TR::Node *&firstChild, TR::Node *&secondChild);

   bool getEvalChild1()  {return (_actionMap[getInputs()] & EvalChild1)  ? true : false;}
   bool getEvalChild2()  {return (_actionMap[getInputs()] & EvalChild2)  ? true : false;}
   bool getCopyReg1()    {return (_actionMap[getInputs()] & CopyReg1)    ? true : false;}
   bool getSubReg1Reg2() {return (_actionMap[getInputs()] & SubReg1Reg2) ? true : false;}
   bool getSubReg3Reg2() {return (_actionMap[getInputs()] & SubReg3Reg2) ? true : false;}
   bool getSubReg1Mem2() {return (_actionMap[getInputs()] & SubReg1Mem2) ? true : false;}
   bool getSubReg3Mem2() {return (_actionMap[getInputs()] & SubReg3Mem2) ? true : false;}

   };

#endif

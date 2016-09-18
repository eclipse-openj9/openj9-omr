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

#ifndef IA32BINARYCOMMUTATIVEANALYSER_INCL
#define IA32BINARYCOMMUTATIVEANALYSER_INCL

#include <stdint.h>                   // for uint8_t
#include "codegen/Analyser.hpp"       // for NUM_ACTIONS, TR_Analyser
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "x/codegen/X86Ops.hpp"       // for TR_X86OpCodes

namespace TR { class Machine; }
namespace TR { class Node; }

#define EvalChild1  0x01
#define EvalChild2  0x02
#define CopyReg1    0x04
#define CopyReg2    0x08
#define OpReg1Reg2  0x10
#define OpReg2Reg1  0x20
#define OpReg1Mem2  0x40
#define OpReg2Mem1  0x80

class TR_X86BinaryCommutativeAnalyser  : public TR_Analyser
   {
   static const uint8_t _actionMap[NUM_ACTIONS];
   TR::CodeGenerator *_cg;
   bool _reversedOperands;
   TR::Machine *_machine;

   public:

   TR_X86BinaryCommutativeAnalyser(TR::CodeGenerator *cg)
      : _cg(cg),
        _machine(cg->machine()),
        _reversedOperands(false)
      {}

   void genericAnalyser(TR::Node       *root,
                        TR_X86OpCodes regRegOpCode,
                        TR_X86OpCodes regMemOpCode,
                        TR_X86OpCodes copyOpCode,
                        bool           nonClobberingDestination = false);

   void genericLongAnalyser(TR::Node       *root,
                            TR_X86OpCodes lowRegRegOpCode,
                            TR_X86OpCodes highRegRegOpCode,
                            TR_X86OpCodes lowRegMemOpCode,
                            TR_X86OpCodes lowRegMemOpCode2Byte,
                            TR_X86OpCodes lowRegMemOpCode1Byte,
                            TR_X86OpCodes highRegMemOpCode,
                            TR_X86OpCodes copyOpCode);

   void integerAddAnalyser(TR::Node       *root,
                           TR_X86OpCodes regRegOpCode,
                           TR_X86OpCodes regMemOpCode,
                           bool needsEflags = false,
                           TR::Node       *carry = 0);

   void longAddAnalyser(TR::Node *root);

   void longMultiplyAnalyser(TR::Node *root);
   void longDualMultiplyAnalyser(TR::Node *root);

   bool getReversedOperands()       {return _reversedOperands;}
   bool setReversedOperands(bool b) {return (_reversedOperands = b);}
   bool notReversedOperands()       {return (_reversedOperands = ((_reversedOperands == false) ? true : false));}

   bool getEvalChild1() {return (_actionMap[getInputs()] & EvalChild1) ? true : false;}
   bool getEvalChild2() {return (_actionMap[getInputs()] & EvalChild2) ? true : false;}
   bool getCopyReg1()   {return (_actionMap[getInputs()] & CopyReg1)   ? true : false;}
   bool getCopyReg2()   {return (_actionMap[getInputs()] & CopyReg2)   ? true : false;}
   bool getOpReg1Reg2() {return (_actionMap[getInputs()] & OpReg1Reg2) ? true : false;}
   bool getOpReg2Reg1() {return (_actionMap[getInputs()] & OpReg2Reg1) ? true : false;}
   bool getOpReg1Mem2() {return (_actionMap[getInputs()] & OpReg1Mem2) ? true : false;}
   bool getOpReg2Mem1() {return (_actionMap[getInputs()] & OpReg2Mem1) ? true : false;}
   bool getCopyRegs()   {return (_actionMap[getInputs()] & (CopyReg1 | CopyReg2)) ? true : false;}

   };

#endif

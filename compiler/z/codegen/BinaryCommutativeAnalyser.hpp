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

#ifndef S390BINARYCOMMUTATIVEANALYSER_INCL
#define S390BINARYCOMMUTATIVEANALYSER_INCL

#include <stddef.h>                // for NULL
#include <stdint.h>                // for uint8_t, uint16_t
#include "codegen/Analyser.hpp"    // for NUM_ACTIONS, TR_Analyser
#include "codegen/InstOpCode.hpp"  // for InstOpCode, InstOpCode::Mnemonic, etc

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }
namespace TR { class Register; }

#define EvalChild1  0x01
#define EvalChild2  0x02
#define CopyReg1    0x04
#define CopyReg2    0x08
#define OpReg1Reg2  0x10
#define OpReg2Reg1  0x20
#define OpReg1Mem2  0x40
#define OpReg2Mem1  0x80
#define OpReg3Mem1  0x001
#define OpReg3Mem2  0x002
#define InvalidBCACEMap 0x004

class TR_S390BinaryCommutativeAnalyser : public TR_Analyser
   {
   static const uint8_t actionMap[NUM_ACTIONS];
   static const uint16_t clobEvalActionMap[NUM_ACTIONS];

   TR::CodeGenerator    *_cg;
   bool                 reversedOperands;

   public:

   TR_S390BinaryCommutativeAnalyser(TR::CodeGenerator *cg)
      : _cg(cg),
        reversedOperands(false)
      {}


   void genericAnalyser(TR::Node       *root,
                        TR::InstOpCode::Mnemonic regToRegOpCode,
                        TR::InstOpCode::Mnemonic memToRegOpCode,
                        TR::InstOpCode::Mnemonic copyOpCode,
                        bool           nonClobberingDestination = false,
                        TR::LabelSymbol *targetLabel = NULL,
                        TR::InstOpCode::S390BranchCondition fBranchOpCond = TR::InstOpCode::COND_NOP,
                        TR::InstOpCode::S390BranchCondition rBranchOpCond = TR::InstOpCode::COND_NOP);

   void genericLongAnalyser(TR::Node       *root,
                            TR::InstOpCode::Mnemonic lowRegToRegOpCode,
                            TR::InstOpCode::Mnemonic highRegToRegOpCode,
                            TR::InstOpCode::Mnemonic lowMemToRegOpCode,
                            TR::InstOpCode::Mnemonic highMemToRegOpCode,
                            TR::InstOpCode::Mnemonic copyOpCode);

   void integerAddAnalyser(TR::Node       *root,
                           TR::InstOpCode::Mnemonic regToRegOpCode,
                           TR::InstOpCode::Mnemonic memToRegOpCode,
                           TR::InstOpCode::Mnemonic copyOpCode);

   void longAddAnalyser(TR::Node *root, TR::InstOpCode::Mnemonic copyOpCode);

   void floatBinaryCommutativeAnalyser(TR::Node       *root,
                        TR::InstOpCode::Mnemonic regToRegOpCode,
                        TR::InstOpCode::Mnemonic memToRegOpCode)
   {
   genericAnalyser(root,regToRegOpCode,memToRegOpCode,TR::InstOpCode::LER);
   }

   void doubleBinaryCommutativeAnalyser(TR::Node       *root,
                        TR::InstOpCode::Mnemonic regToRegOpCode,
                        TR::InstOpCode::Mnemonic memToRegOpCode)
   {
   genericAnalyser(root,regToRegOpCode,memToRegOpCode,TR::InstOpCode::LDR);
   }

   bool getReversedOperands()       {return reversedOperands;}
   bool setReversedOperands(bool b) {return reversedOperands = b;}
   bool notReversedOperands()       {return reversedOperands = ((reversedOperands == false) ? true : false);}

   bool getEvalChild1() {return (actionMap[getInputs()] & EvalChild1) ? true : false;}
   bool getEvalChild2() {return (actionMap[getInputs()] & EvalChild2) ? true : false;}

   bool conversionIsRemoved(TR::Node * root, TR::Node * &child);

   bool getCopyReg1();

   bool getCopyReg2();

   bool getOpReg1Reg2();

   bool getOpReg2Reg1();

   bool getOpReg1Mem2();

   bool getOpReg3Mem2();

   bool getOpReg2Mem1();

   bool getOpReg3Mem1();

   bool getCopyRegs();

   bool getInvalid();

   void remapInputs(TR::Node *, TR::Register *, TR::Node *, TR::Register *, bool nonClobberingDestination = false);

   TR::CodeGenerator *cg() {return _cg;}

   private:
   TR::Register* allocateAddSubRegister(TR::Node* node, TR::Register* src1Reg);


   };

#endif

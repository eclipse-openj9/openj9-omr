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

#ifndef ARMOPERAND2_INCL
#define ARMOPERAND2_INCL


typedef enum {
   ARMOp2Illegal,
   ARMOp2Immed8r,
   ARMOp2RegLSLImmed,
   ARMOp2RegLSRImmed,
   ARMOp2RegASRImmed,
   ARMOp2RegRORImmed,
   ARMOp2Reg,
   ARMOp2RegRRX,
   ARMOp2RegLSLReg,
   ARMOp2RegLSRReg,
   ARMOp2RegASRReg,
   ARMOp2RegRORReg
} TR_ARMOperand2Type;

class TR_Debug;

class TR_ARMOperand2
   {
   TR_ARMOperand2Type _opType;
   TR::Register *      _baseRegister;
   uint32_t           _shiftOrImmed;
   uint32_t           _immedShift;
   TR::Register *      _shiftRegister;

   friend class TR_Debug;

   public:
   TR_ALLOC(TR_Memory::ARMOperand)

   TR_ARMOperand2()
      : _opType(ARMOp2Illegal), _baseRegister(0), _shiftOrImmed(0), _immedShift(0), _shiftRegister(0) {}

   // immediates
   TR_ARMOperand2(uint32_t immed8, uint32_t shiftAmount)
      : _opType(ARMOp2Immed8r) , _baseRegister(0), _shiftOrImmed(immed8), _immedShift(shiftAmount), _shiftRegister(0) {}

   // reg or regRRX
   TR_ARMOperand2(TR_ARMOperand2Type opType, TR::Register *dst)
      : _opType(opType) , _baseRegister(dst), _shiftOrImmed(0), _immedShift(0), _shiftRegister(0) {}

   // reg SHIFTOP immed
   TR_ARMOperand2(TR_ARMOperand2Type opType, TR::Register *dst, uint32_t immed5)
      : _opType(opType) , _baseRegister(dst), _shiftOrImmed(immed5), _immedShift(0), _shiftRegister(0) {}

   // reg SHIFTOP reg
   TR_ARMOperand2(TR_ARMOperand2Type opType, TR::Register *dst, TR::Register *shiftReg)
      : _opType(opType) , _baseRegister(dst), _shiftOrImmed(0), _shiftRegister(shiftReg), _immedShift(0) {}

   void incTotalUseCount();
   void setBinaryEncoding(uint32_t *instruction);
   bool refsRegister(TR::Register *reg);
   void block();
   void unblock();
   void decFutureUseCountAndAdjustRegState();
   TR_ARMOperand2 *assignRegisters(TR::Instruction *currentInstruction, TR::CodeGenerator *cg);
   };
#endif

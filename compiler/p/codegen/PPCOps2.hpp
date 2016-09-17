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

#include "p/codegen/PPCOpsDefines.hpp"

typedef uint32_t TR_PPCOpCodeBinaryEntry;
extern const char * ppcOpCodeToNameMap[][2];

#ifndef OPS_MAX
#define OPS_MAX    PPCNumOpCodes
#endif

class TR_PPCOpCode
   {
   TR_PPCOpCodes                          _opCode;
   static const uint32_t                  properties[PPCNumOpCodes];
   static const TR_PPCOpCodeBinaryEntry   binaryEncodings[PPCNumOpCodes];

   public:

   TR_PPCOpCode()                 : _opCode(PPCOp_bad) {}
   TR_PPCOpCode(TR_PPCOpCodes op) : _opCode(op)        {}

   TR_PPCOpCodes getOpCodeValue()                 {return _opCode;}
   TR_PPCOpCodes setOpCodeValue(TR_PPCOpCodes op) {return (_opCode = op);}
   TR_PPCOpCodes getRecordFormOpCodeValue()       {return (TR_PPCOpCodes)(_opCode+1);}

   bool isRecordForm() {return (properties[_opCode] & PPCOpProp_IsRecordForm)!=0;}

   bool hasRecordForm() {return (properties[_opCode] & PPCOpProp_HasRecordForm)!=0;}

   bool singleFPOp() {return (properties[_opCode] & PPCOpProp_SingleFP)!=0;}

   bool doubleFPOp() {return (properties[_opCode] & PPCOpProp_DoubleFP)!=0;}

   bool gprOp() {return (properties[_opCode] & (PPCOpProp_DoubleFP | PPCOpProp_SingleFP))==0;}

   bool fprOp() {return (properties[_opCode] & (PPCOpProp_DoubleFP | PPCOpProp_SingleFP))!=0;}

   bool useAlternateFormat() {return (properties[_opCode] & PPCOpProp_AltFormat)!=0;}

   bool useAlternateFormatx() {return (properties[_opCode] & PPCOpProp_AltFormatx)!=0;}

   bool readsCarryFlag() {return (properties[_opCode] & PPCOpProp_ReadsCarryFlag)!=0;}

   bool setsCarryFlag() {return (properties[_opCode] & PPCOpProp_SetsCarryFlag)!=0;}

   bool setsOverflowFlag() {return (properties[_opCode] & PPCOpProp_SetsOverflowFlag)!=0;}

   bool usesCountRegister() {return (properties[_opCode] & PPCOpProp_UsesCtr)!=0;}

   bool setsCountRegister() {return (properties[_opCode] & PPCOpProp_SetsCtr)!=0;}

   bool isBranchOp() {return (properties[_opCode] & PPCOpProp_BranchOp)!=0;}

   bool isLoad() {return (properties[_opCode] & PPCOpProp_IsLoad)!=0;}

   bool isStore() {return (properties[_opCode] & PPCOpProp_IsStore)!=0;}

   bool isRegCopy() {return (properties[_opCode] & PPCOpProp_IsRegCopy)!=0;}

   bool isUpdate() {return (properties[_opCode] & PPCOpProp_UpdateForm)!=0;}

   bool isDoubleWord() {return (properties[_opCode] & PPCOpProp_DWord)!=0;}

   bool isCall() {return _opCode==PPCOp_bl;}

   bool isTrap() {return (properties[_opCode] & PPCOpProp_Trap)!=0;}

   bool isTMAbort() {return (properties[_opCode] & PPCOpProp_TMAbort)!=0;}

   bool isFloat() {return (properties[_opCode] & (PPCOpProp_SingleFP|PPCOpProp_DoubleFP))!=0;}

   bool isVMX() {return (properties[_opCode] & PPCOpProp_IsVMX)!=0;}

   bool isVSX() {return (properties[_opCode] & PPCOpProp_IsVSX)!=0;}

   bool usesTarget() {return (properties[_opCode] & PPCOpProp_UsesTarget)!=0;}

   bool useMaskEnd() {return (properties[_opCode] & PPCOpProp_UseMaskEnd)!=0;}

   bool isRotateOrShift() {return (properties[_opCode] & PPCOpProp_IsRotateOrShift)!=0;}

   bool isCompare() {return (properties[_opCode] & PPCOpProp_CompareOp)!=0;}

   bool readsFPSCR() {return (properties[_opCode] & PPCOpProp_ReadsFPSCR)!=0;}

   bool setsFPSCR() {return (properties[_opCode] & PPCOpProp_SetsFPSCR)!=0;}

   bool isSyncSideEffectFree() {return (properties[_opCode] & PPCOpProp_SyncSideEffectFree)!=0;}

   bool offsetRequiresWordAlignment() { return (properties[_opCode] & PPCOpProp_OffsetRequiresWordAlignment)!=0;}

   bool setsCTR() {return (properties[_opCode] & PPCOpProp_SetsCtr)!=0;}

   bool usesCTR() {return (properties[_opCode] & PPCOpProp_UsesCtr)!=0;}

   bool isLongRunningFPOp() {return _opCode==PPCOp_fdiv  ||
                                    _opCode==PPCOp_fdivs ||
                                    _opCode==PPCOp_fsqrt ||
                                    _opCode==PPCOp_fsqrts;}

   bool isFXMult() {return _opCode==PPCOp_mullw  ||
                           _opCode==PPCOp_mulli  ||
                           _opCode==PPCOp_mulhw  ||
                           _opCode==PPCOp_mulhd  ||
                           _opCode==PPCOp_mulhwu ||
                           _opCode==PPCOp_mulhdu;}

   bool isAdmin() {return _opCode==PPCOp_ret      ||
                          _opCode==PPCOp_fence    ||
                          _opCode==PPCOp_depend   ||
                          _opCode==PPCOp_proc     ||
                          _opCode==PPCOp_assocreg ||
                          _opCode==PPCOp_dd;}

   static const TR_PPCOpCodeBinaryEntry getOpCodeBinaryEncoding(TR_PPCOpCodes opCode)
      {return binaryEncodings[opCode];}
   const TR_PPCOpCodeBinaryEntry getOpCodeBinaryEncoding()
      {return getOpCodeBinaryEncoding(_opCode);}

   uint8_t *copyBinaryToBuffer(uint8_t *cursor)
      {
      *(uint32_t *)cursor = *(uint32_t *)&binaryEncodings[_opCode];
      return cursor;
      }
   };

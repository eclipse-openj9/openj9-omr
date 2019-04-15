/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef OMR_Z_SYSTEMLINKAGEZOS_INCL
#define OMR_Z_SYSTEMLINKAGEZOS_INCL

#include <stdint.h>
#include "codegen/Linkage.hpp"
#include "codegen/LinkageConventionsEnum.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/SystemLinkage.hpp"
#include "codegen/snippet/PPA1Snippet.hpp"
#include "codegen/snippet/PPA2Snippet.hpp"
#include "cs2/arrayof.h"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/SymbolReference.hpp"

class TR_EntryPoint;
class TR_zOSGlobalCompilationInfo;
namespace TR { class S390ConstantDataSnippet; }
namespace TR { class S390JNICallDataSnippet; }
namespace TR { class AutomaticSymbol; }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }
namespace TR { class ParameterSymbol; }
namespace TR { class RegisterDependencyConditions; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class Symbol; }
template <class T> class List;

enum TR_XPLinkFrameType
   {
   TR_XPLinkUnknownFrame,
   TR_XPLinkStackLeafFrame,
   TR_XPLinkNoStackLeafFrame,
   TR_XPLinkSmallFrame,
   TR_XPLinkIntermediateFrame,
   TR_XPLinkStackCheckFrame
   };

/**
 * XPLink call types whose value is encoded in NOP following call.
 * These values are encoded - so don't change
 */
enum TR_XPLinkCallTypes {
   TR_XPLinkCallType_BASR      =0,     ///< generic BASR
   TR_XPLinkCallType_BRAS7     =1,     ///< BRAS  r7,ep
   TR_XPLinkCallType_RESVD_2   =2,     //
   TR_XPLinkCallType_BRASL7    =3,     ///< BRASL r7,ep
   TR_XPLinkCallType_RESVD_4   =4,     //
   TR_XPLinkCallType_RESVD_5   =5,     //
   TR_XPLinkCallType_BALR1415  =6,     ///< BALR  r14,r15
   TR_XPLinkCallType_BASR33    =7      ///< BASR  r3,r3
   };

namespace TR {

class S390zOSSystemLinkage : public TR::SystemLinkage
   {
   public:
   
   enum
      {
      CEECAAPLILWS    = 640,
      CEECAAAESS      = 788,
      CEECAAOGETS     = 796,
      CEECAAAGTS      = 944,

      CEEDSAType1Size = 0x80,
      };

   S390zOSSystemLinkage(TR::CodeGenerator * cg);

   virtual void generateInstructionsForCall(TR::Node * callNode, TR::RegisterDependencyConditions * dependencies, intptrj_t targetAddress, TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel, TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint = true);

   virtual TR::Register * callNativeFunction(TR::Node * callNode, TR::RegisterDependencyConditions * dependencies, intptrj_t targetAddress, TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel, TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint = true);

   virtual TR::RealRegister::RegNum setEnvironmentPointerRegister (TR::RealRegister::RegNum r) { return _environmentPointerRegister = r; }
   virtual TR::RealRegister::RegNum getEnvironmentPointerRegister() { return _environmentPointerRegister; }
   virtual TR::RealRegister *getEnvironmentPointerRealRegister() {return getRealRegister(_environmentPointerRegister);}

   virtual int32_t getRegisterSaveOffset(TR::RealRegister::RegNum);

   virtual TR::RealRegister::RegNum setCAAPointerRegister (TR::RealRegister::RegNum r)         { return _CAAPointerRegister = r; }
   virtual TR::RealRegister::RegNum getCAAPointerRegister()         { return _CAAPointerRegister; }
   virtual TR::RealRegister *getCAAPointerRealRegister() {return getRealRegister(_CAAPointerRegister);}

   virtual TR::RealRegister::RegNum setParentDSAPointerRegister (TR::RealRegister::RegNum r)         { return _parentDSAPointerRegister = r; }
   virtual TR::RealRegister::RegNum getParentDSAPointerRegister()         { return _parentDSAPointerRegister; }
   virtual TR::RealRegister *getParentDSAPointerRealRegister() {return getRealRegister(_parentDSAPointerRegister);}

   virtual bool isAggregateReturnedInIntRegistersAndMemory(int32_t aggregateLenth);

   TR_XPLinkFrameType getFrameType() { return _frameType; }
   void setFrameType(enum TR_XPLinkFrameType type) { _frameType = type; }
   virtual bool getIsLeafRoutine() { return (getFrameType() == TR_XPLinkStackLeafFrame) || (getFrameType() == TR_XPLinkNoStackLeafFrame); }
   virtual int32_t getOutgoingParameterBlockSize();

   virtual TR::Register *loadEnvironmentIntoRegister(TR::Node * callNode, TR::RegisterDependencyConditions * deps, bool loadIntoEnvironmentReg);
   bool isAggregateReturnedInIntRegisters(int32_t aggregateLenth);

   uint32_t calculateInterfaceMappingFlags(TR::ResolvedMethodSymbol *method);

   static uint32_t shiftFloatParmDescriptorFlag(uint32_t fieldVal, int32_t floatParmNum)  { return (fieldVal) << (6*(3-floatParmNum)); } // accepts floatParmNum values 0,1,2,3
   bool updateFloatParmDescriptorFlags(uint32_t *parmDescriptorFields, TR::Symbol *funcSymbol, int32_t parmCount, int32_t argSize, TR::DataType dataType, int32_t *floatParmNum, uint32_t *lastFloatParmAreaOffset, uint32_t *parmAreaOffset);

   static uint32_t getFloatParmDescriptorFlag(uint32_t descriptorFields, int32_t floatParmNum)  { return  (descriptorFields >> (6*(3-floatParmNum))) & 0x3F; }
   uint32_t calculateReturnValueAdjustFlag(TR::DataType dataType, int32_t aggregateLength);
   static uint32_t isFloatDescriptorFlagUnprototyped(uint32_t flag)  { return flag == 0; }

   uint32_t calculateCallDescriptorFlags(TR::Node *callNode);

   public:

   virtual void setPrologueInfoCalculated(bool value)  { _prologueInfoCalculated = value; }
   virtual bool getPrologueInfoCalculated()  { return _prologueInfoCalculated; }
   void setAlternateStackPointerAdjust(int32_t adjust) { _alternateSPAdjust = adjust; }
   int32_t getAlternateStackPointerAdjust() { return _alternateSPAdjust; }
   void setSaveBackChain(bool value) { _saveBackChain = value; } // for use with XPLINK(BACKCHAIN) or other
   bool getSaveBackChain() { return _saveBackChain; }
   uint32_t getStackFrameBias() { return 2048; }

   // == Related to signature of method or call
   uint32_t  getInterfaceMappingFlags() { return _interfaceMappingFlags; }
   void setInterfaceMappingFlags(uint32_t flags) { _interfaceMappingFlags = flags; }

   // Size of guard page can affect generation of explicit stack checking code in noleaf routines.
   // For example, 0 size guard page means always generate stack checking in noleafs.
   void setGuardPageSize(uint32_t size) { _guardPageSize = size; }
   uint32_t getGuardPageSize() { return _guardPageSize; }

   virtual void calculatePrologueInfo(TR::Instruction * cursor);
   virtual TR::Instruction *buyFrame(TR::Instruction * cursor, TR::Node *node);
   virtual void createEntryPointMarker(TR::Instruction *cursor, TR::Node *node);

   // === Call or entry related
   TR_XPLinkCallTypes genWCodeCallBranchCode(TR::Node *callNode, TR::RegisterDependencyConditions * deps);

   /** \brief
    *     Gets the label instruction which marks the stack pointer update instruction following it.
    *
    *  \return
    *     The stack pointer update label instruction if it exists; \c NULL otherwise.
    */
   TR::Instruction* getStackPointerUpdate() const;
   
   /** \brief
    *     Gets the PPA1 (Program Prologue Area) snippet for this method body.
    */
   TR::PPA1Snippet* getPPA1Snippet() const;
   
   /** \brief
    *     Gets the PPA2 (Program Prologue Area) snippet for this method body.
    */
   TR::PPA2Snippet* getPPA2Snippet() const;

   virtual void createPrologue(TR::Instruction * cursor);
   virtual void createEpilogue(TR::Instruction * cursor);

   TR::Instruction * genCallNOPAndDescriptor(TR::Instruction * cursor, TR::Node *node, TR::Node *callNode, TR_XPLinkCallTypes callType);

   private:
   
   // TODO: There seems to be a lot of similarity between this relocation and PPA1OffsetToPPA2Relocation relocation.
   // It would be best if we common these up, perhaps adding an "offset" to to one of the existing relocation kinds
   // which perform similar relocations. See Relocation.hpp for which relocations may fit this criteria.
   class EntryPointMarkerOffsetToPPA1Relocation : public TR::LabelRelocation
      {
      public:

      EntryPointMarkerOffsetToPPA1Relocation(TR::Instruction* cursor, TR::LabelSymbol* ppa2);

      virtual void apply(TR::CodeGenerator* cg);

      private:

      TR::Instruction* _cursor;
      };

   enum TR_XPLinkFrameType _frameType;

   TR::RealRegister::RegNum _CAAPointerRegister;
   TR::RealRegister::RegNum _parentDSAPointerRegister;

   TR_zOSGlobalCompilationInfo* _globalCompilationInfo;

   TR::Instruction* _stackPointerUpdate;

   TR::PPA1Snippet* _ppa1;
   TR::PPA2Snippet* _ppa2;

   enum
      {
      fudgeFactor = 1800                  // see S390EarlyStackMap
                                         // 200 is too small for povray
      };
   uintptr_t _actualOffsetOfFirstIncomingParm;
   bool  _prologueInfoCalculated;        // if calculated prologue information already - i.e. multiple entry points
   int32_t _alternateSPAdjust;           // i.e. for alloca: alternateSP <- SP + adjust
   uint32_t _guardPageSize;              // byte size of guard page affecting explicit checking
   bool _saveBackChain;                  // GPR4 is saved
   uint32_t _interfaceMappingFlags;       // describing the method body
   
   TR::RealRegister::RegNum _environmentPointerRegister;
   };
}

#endif

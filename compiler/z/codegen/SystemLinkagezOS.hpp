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
   TR_XPLinkCallType_BRASL7    =3,     ///< BRASL r7,ep
   TR_XPLinkCallType_BASR33    =7      ///< BASR  r3,r3
   };

namespace TR {

class S390zOSSystemLinkage : public TR::SystemLinkage
   {
   public:

   /** \brief
    *     Size (in bytes) of the XPLINK stack frame bias as defined by the system linkage.
    */
   static const size_t XPLINK_STACK_FRAME_BIAS = 2048;

   public:

   S390zOSSystemLinkage(TR::CodeGenerator* cg);

   virtual void createEpilogue(TR::Instruction * cursor);
   virtual void createPrologue(TR::Instruction * cursor);

   virtual void generateInstructionsForCall(TR::Node * callNode, TR::RegisterDependencyConditions * dependencies, intptrj_t targetAddress, TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel, TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint = true);

   virtual TR::Register* callNativeFunction(TR::Node * callNode, TR::RegisterDependencyConditions * dependencies, intptrj_t targetAddress, TR::Register * methodAddressReg, TR::Register * javaLitOffsetReg, TR::LabelSymbol * returnFromJNICallLabel, TR::S390JNICallDataSnippet * jniCallDataSnippet, bool isJNIGCPoint = true);

   virtual TR::RealRegister::RegNum getENVPointerRegister();
   virtual TR::RealRegister::RegNum getCAAPointerRegister();
   
   virtual int32_t getRegisterSaveOffset(TR::RealRegister::RegNum);
   virtual int32_t getOutgoingParameterBlockSize();

   TR::Instruction * genCallNOPAndDescriptor(TR::Instruction * cursor, TR::Node *node, TR::Node *callNode, TR_XPLinkCallTypes callType);

   /** \brief
    *     Gets the label symbol representing the start of the Entry Point Marker in the prologue.
    *
    *  \return
    *     The Entry Point Marker label symbol if it exists; \c NULL otherwise.
    */
   TR::LabelSymbol* getEntryPointMarkerLabel() const;

   /** \brief
    *     Gets the label symbol representing the stack pointer update instruction following it.
    *
    *  \return
    *     The stack pointer update label symbol if it exists; \c NULL otherwise.
    */
   TR::LabelSymbol* getStackPointerUpdateLabel() const;
   
   /** \brief
    *     Gets the PPA1 (Program Prologue Area 1) snippet for this method body.
    */
   TR::PPA1Snippet* getPPA1Snippet() const;
   
   /** \brief
    *     Gets the PPA2 (Program Prologue Area 2) snippet for this method body.
    */
   TR::PPA2Snippet* getPPA2Snippet() const;

   private:

   virtual TR::Instruction* addImmediateToRealRegister(TR::RealRegister * targetReg, int32_t immediate, TR::RealRegister *tempReg, TR::Node *node, TR::Instruction *cursor, bool *checkTempNeeded=NULL);
   
   TR::Instruction* fillGPRsInEpilogue(TR::Node* node, TR::Instruction* cursor);
   TR::Instruction* fillFPRsInEpilogue(TR::Node* node, TR::Instruction* cursor);

   TR::Instruction* spillGPRsInPrologue(TR::Node* node, TR::Instruction* cursor);
   TR::Instruction* spillFPRsInPrologue(TR::Node* node, TR::Instruction* cursor);

   private:
   
   TR::LabelSymbol* _entryPointMarkerLabel;
   TR::LabelSymbol* _stackPointerUpdateLabel;

   TR::PPA1Snippet* _ppa1Snippet;
   TR::PPA2Snippet* _ppa2Snippet;
   };
}

#endif

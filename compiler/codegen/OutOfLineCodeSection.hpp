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

#ifndef OUTOFLINECODESECTION_INCL
#define OUTOFLINECODESECTION_INCL

#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds
#include "env/TRMemory.hpp"               // for TR_Memory, etc
#include "il/ILOpCodes.hpp"               // for ILOpCodes

namespace TR { class Block; }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }
namespace TR { class Register; }

class TR_OutOfLineCodeSection
   {

protected:

   TR::LabelSymbol       *_entryLabel;
   TR::LabelSymbol       *_restartLabel;
   TR::Instruction      *_firstInstruction;
   TR::Instruction      *_appendInstruction;

   TR::Block            *_block;
   TR::CodeGenerator    *_cg;

   TR::Node             *_callNode;
   TR::Register         *_targetReg;

   bool                 _hasBeenRegisterAssigned;

   void evaluateNodesWithFutureUses(TR::Node *node);

public:

   TR_ALLOC(TR_Memory::OutlinedCode)

   TR_OutOfLineCodeSection(TR::LabelSymbol *entryLabel, TR::CodeGenerator *cg);

   TR_OutOfLineCodeSection(TR::LabelSymbol *entryLabel, TR::LabelSymbol *restartLabel, TR::CodeGenerator *cg);

   void swapInstructionListsWithCompilation();

   // For calls
   //
   TR_OutOfLineCodeSection(TR::Node *callNode, TR::ILOpCodes callOp, TR::Register *targetReg, TR::LabelSymbol *entryLabel, TR::LabelSymbol *restartLabel, TR::CodeGenerator *cg);

   TR::LabelSymbol *getEntryLabel()         { return _entryLabel; }
   void setEntryLabel(TR::LabelSymbol *sym) { _entryLabel = sym; }

   TR::LabelSymbol *getRestartLabel()         { return _restartLabel; }
   void setRestartLabel(TR::LabelSymbol *sym) { _restartLabel = sym; }

   TR::Instruction *getFirstInstruction()         { return _firstInstruction; }
   void setFirstInstruction(TR::Instruction *ins) { _firstInstruction = ins; }

   TR::Instruction *getAppendInstruction()         { return _appendInstruction; }
   void setAppendInstruction(TR::Instruction *ins) { _appendInstruction = ins; }

   TR::Block *getBlock()       { return _block; }
   void setBlock(TR::Block *b) { _block = b; }

   bool hasBeenRegisterAssigned()          { return _hasBeenRegisterAssigned; }
   void setHasBeenRegisterAssigned(bool r) { _hasBeenRegisterAssigned = r; }

   TR::Node *createOutOfLineCallNode(TR::Node *callNode, TR::ILOpCodes callOp);

   TR::Node *getCallNode()       { return _callNode; }
   void setCallNode(TR::Node *n) { _callNode = n; }

   void preEvaluatePersistentHelperArguments();
   virtual void assignRegisters(TR_RegisterKinds kindsToBeAssigned) {}

   };

#endif
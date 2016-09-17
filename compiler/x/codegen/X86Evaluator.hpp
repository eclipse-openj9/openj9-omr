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

#ifndef X86_EVALUATOR_INCL
#define X86_EVALUATOR_INCL



#include <stddef.h>                         // for NULL
#include <stdint.h>                         // for uint8_t
#include "x/codegen/X86Ops.hpp"             // for TR_X86OpCodes
#include "codegen/TreeEvaluator.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class MemoryReference; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }


extern void constLengthArrayCopy(
   TR::Node *node,
   TR::CodeGenerator *cg,
   TR::Register *byteSrcReg,
   TR::Register *byteDstReg,
   TR::Node *byteLenNode,
   bool preserveSrcPointer,
   bool preserveDstPointer);

extern void genCodeToPerformLeftToRightAndBlockConcurrentOpIfNeeded(
   TR::Node *node,
   TR::MemoryReference *memRef,
   TR::Register *valueReg,
   TR::Register *tempReg,
   TR::Register *tempReg1,
   TR::Register *tempReg2,
   TR::LabelSymbol *nonLockedOpLabel,
   TR::LabelSymbol *&opDoneLabel,
   TR::RegisterDependencyConditions *&deps,
   uint8_t size,
   TR::CodeGenerator *cg,
   bool isLoad,
   bool genOutOfline,
   bool keepValueRegAlive = false,
   TR::LabelSymbol *startControlFlowLabel = NULL);



class TR_X86ComputeCC : public TR::TreeEvaluator
   {
   public:

   static void bitwise32(TR::Node *node, TR::Register *ccReg, TR::Register *target, TR::CodeGenerator *cg);
   static bool setCarryBorrow(TR::Node *flagNode, bool invertValue, TR::CodeGenerator *cg);

   };

#endif

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

#ifndef S390CALLSNIPPET_INCL
#define S390CALLSNIPPET_INCL

#include <stddef.h>                   // for NULL
#include <stdint.h>                   // for int32_t, uint8_t, uint32_t
#include "codegen/InstOpCode.hpp"     // for InstOpCode, etc
#include "il/DataTypes.hpp"           // for DataTypes
#include "runtime/Runtime.hpp"        // for TR_RuntimeHelper
#include "codegen/Snippet.hpp"  // for TR::S390Snippet, etc

#include "z/codegen/S390Instruction.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class LabelSymbol; }
namespace TR { class MethodSymbol; }
namespace TR { class Node; }
namespace TR { class RealRegister; }
namespace TR { class SymbolReference; }

namespace TR {

class S390CallSnippet : public TR::Snippet
   {
   int32_t  sizeOfArguments;
   TR::Instruction *branchInstruction;

   protected:
   TR::SymbolReference * _realMethodSymbolReference;
   TR_RuntimeHelper getInterpretedDispatchHelper(TR::SymbolReference *methodSymRef,
                                                 TR::DataType        type);
   public:

   S390CallSnippet(TR::CodeGenerator *cg, TR::Node *c, TR::LabelSymbol *lab, int32_t s)
      : TR::Snippet(cg, c, lab, false), sizeOfArguments(s),branchInstruction(NULL), _realMethodSymbolReference(NULL)
      {
      }

   S390CallSnippet(TR::CodeGenerator *cg, TR::Node *c, TR::LabelSymbol *lab, TR::SymbolReference *symRef, int32_t s)
      : TR::Snippet(cg, c, lab, false), sizeOfArguments(s),branchInstruction(NULL), _realMethodSymbolReference(symRef)
      {
      }

   virtual Kind getKind() { return IsCall; }

   virtual bool isCallSnippet() { return true; }

   /** Get call Return Address */
   virtual uint8_t *getCallRA();

   virtual uint8_t *emitSnippetBody();

   virtual uint32_t getLength(int32_t estimatedSnippetStart);

   int32_t getSizeOfArguments()          {return sizeOfArguments;}
   int32_t setSizeOfArguments(int32_t s) {return sizeOfArguments = s;}

   TR::Instruction *setBranchInstruction(TR::Instruction *i) {return branchInstruction=i;}
   TR::Instruction *getBranchInstruction() {return branchInstruction;}

   TR::SymbolReference *setRealMethodSymbolReference(TR::SymbolReference *s) {return _realMethodSymbolReference = s;}
   TR::SymbolReference *getRealMethodSymbolReference() {return _realMethodSymbolReference;}

   uint8_t *loadArgumentItem(TR::InstOpCode::Mnemonic op, uint8_t *buffer, TR::RealRegister *reg, int32_t offset);
   uint8_t *setUpArgumentsInRegister(uint8_t *buffer, TR::Node *callNode, int32_t argSize);


   static TR_RuntimeHelper getHelper(TR::MethodSymbol *, TR::DataType, TR::CodeGenerator *);

   static uint8_t *storeArgumentItem(TR::InstOpCode::Mnemonic op, uint8_t *buffer, TR::RealRegister *reg, int32_t offset, TR::CodeGenerator *cg);
   static uint8_t *S390flushArgumentsToStack(uint8_t *buffer, TR::Node *callNode, int32_t argSize, TR::CodeGenerator *cg);
   static int32_t instructionCountForArguments(TR::Node *callNode, TR::CodeGenerator *cg);

   };

}

#endif

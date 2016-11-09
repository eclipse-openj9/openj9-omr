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

#ifndef DIVIDECHECKSNIPPET_INCL
#define DIVIDECHECKSNIPPET_INCL

#include <stdint.h>                      // for int32_t, uint32_t, uint8_t
#include "codegen/Snippet.hpp"           // for TR::X86Snippet::Kind, etc
#include "il/DataTypes.hpp"              // for TR::DataType
#include "x/codegen/RestartSnippet.hpp"  // for TR::X86RestartSnippet
#include "x/codegen/X86Instruction.hpp"  // for TR::X86RegRegInstruction

namespace TR { class CodeGenerator; }
namespace TR { class ILOpCode; }
namespace TR { class LabelSymbol; }

namespace TR {

class X86DivideCheckSnippet  : public TR::X86RestartSnippet
   {
   public:

   X86DivideCheckSnippet(TR::LabelSymbol           *restartLabel,
                            TR::LabelSymbol           *snippetLabel,
                            TR::LabelSymbol           *divideLabel,
                            TR::ILOpCode               &divOp,
                            TR::DataType               type,
                            TR::X86RegRegInstruction  *divideInstruction,
                            TR::CodeGenerator         *cg)
      : TR::X86RestartSnippet(cg, divideInstruction->getNode(), restartLabel, snippetLabel, true),
        _divOp(divOp),
        _type(type),
        _divideLabel(divideLabel),
        _divideInstruction(divideInstruction)
      {}

   virtual Kind getKind() { return IsDivideCheck; }

   TR::ILOpCode &getOpCode() {return _divOp;}
   TR::DataType getType() {return _type;}

   TR::LabelSymbol *getDivideLabel()                  {return _divideLabel;}
   TR::LabelSymbol *setDivideLabel(TR::LabelSymbol *l) {return (_divideLabel = l);}

   TR::X86RegRegInstruction  *getDivideInstruction() {return _divideInstruction;}

   virtual uint8_t *emitSnippetBody();
   virtual uint32_t getLength(int32_t estimatedSnippetStart);

   private:

   TR::LabelSymbol          *_divideLabel;
   TR::X86RegRegInstruction *_divideInstruction;
   TR::ILOpCode              &_divOp;
   TR::DataType              _type;
   };

}

#endif

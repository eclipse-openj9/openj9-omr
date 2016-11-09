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

#ifndef X86HELPERCALLSNIPPET_INCL
#define X86HELPERCALLSNIPPET_INCL

#include <stdint.h>                      // for int32_t, uint8_t, uint32_t
#include "codegen/Snippet.hpp"           // for TR::X86Snippet::Kind, etc
#include "il/Node.hpp"                   // for Node
#include "x/codegen/RestartSnippet.hpp"  // for TR::X86RestartSnippet

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class SymbolReference; }

namespace TR {

class X86HelperCallSnippet : public TR::X86RestartSnippet
   {
   TR::Node            *_callNode;
   TR::SymbolReference *_destination;
   uint8_t            *_callInstructionBufferAddress;
   int32_t             _stackPointerAdjustment; // For helper calls that need a stack frame temporarily deallocated

   // If the displacement of the helper call instruction might be patched dynamically.
   //
   bool                _alignCallDisplacementForPatching;

   public:

   X86HelperCallSnippet(TR::CodeGenerator   *cg,
                        TR::Node            *node,
                        TR::LabelSymbol      *restartLabel,
                        TR::LabelSymbol      *snippetLabel,
                        TR::SymbolReference *helper,
                        int32_t             stackPointerAdjustment=0);

   X86HelperCallSnippet(TR::CodeGenerator   *cg,
                        TR::LabelSymbol      *restartLabel,
                        TR::LabelSymbol      *snippetLabel,
                        TR::Node            *callNode,
                        int32_t             stackPointerAdjustment=0);

   virtual Kind getKind() { return IsHelperCall; }

   TR::Node            *getCallNode()                         {return _callNode;}
   TR::SymbolReference *getDestination()                      {return _destination;}
   TR::SymbolReference *setDestination(TR::SymbolReference *s) {return (_destination = s);}
   int32_t             getStackPointerAdjustment()           {return _stackPointerAdjustment;}

   static int32_t branchDisplacementToHelper(uint8_t *nextInstructionAddress, TR::SymbolReference *helper, TR::CodeGenerator *cg);

   int32_t getOffset() {return _offset;}

   uint8_t *getCallInstructionBufferAddress() {return _callInstructionBufferAddress;}
   bool getAlignCallDisplacementForPatching() {return _alignCallDisplacementForPatching;}
   void setAlignCallDisplacementForPatching(bool a) {_alignCallDisplacementForPatching = a;}

   void addMetaDataForLoadAddrArg(uint8_t *buffer, TR::Node *child);

   virtual uint8_t *genHelperCall(uint8_t *buffer);

   virtual uint8_t *emitSnippetBody();
   virtual uint32_t getLength(int32_t estimatedSnippetStart);

   private:

   int32_t _offset; // special field for the jitReportMethodEnter helper
   };


class X86CheckAsyncMessagesSnippet : public TR::X86HelperCallSnippet
   {
   public:

   X86CheckAsyncMessagesSnippet(
      TR::Node            *node,
      TR::LabelSymbol      *restartLabel,
      TR::LabelSymbol      *snippetLabel,
      TR::CodeGenerator   *cg) :
         TR::X86HelperCallSnippet(cg, node, restartLabel, snippetLabel, node->getSymbolReference())
      {
      }

   virtual uint8_t *genHelperCall(uint8_t *buffer);
   };

}

#endif

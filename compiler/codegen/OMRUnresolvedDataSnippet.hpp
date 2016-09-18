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

#ifndef OMR_UNRESOLVEDDATASNIPPET_INCL
#define OMR_UNRESOLVEDDATASNIPPET_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_UNRESOLVEDDATASNIPPET_CONNECTOR
#define OMR_UNRESOLVEDDATASNIPPET_CONNECTOR
namespace OMR { class UnresolvedDataSnippet; }
namespace OMR { typedef OMR::UnresolvedDataSnippet UnresolvedDataSnippetConnector; }
#endif

#include "codegen/Snippet.hpp"

#include <stddef.h>         // for NULL
#include <stdint.h>         // for uint8_t, int32_t
#include "infra/Flags.hpp"  // for flag32_t

namespace TR { class SymbolReference; }
namespace TR { class CodeGenerator; }
namespace TR { class Instruction; }
namespace TR { class Node; }
namespace TR { class UnresolvedDataSnippet; }

namespace OMR
{

class UnresolvedDataSnippet : public TR::Snippet
   {

public:

   UnresolvedDataSnippet(TR::CodeGenerator * cg,
                         TR::Node * node,
                         TR::SymbolReference * symRef,
                         bool isStore,
                         bool isGCSafePoint);

   TR::UnresolvedDataSnippet * self();

   static TR::UnresolvedDataSnippet * create(TR::CodeGenerator * cg, TR::Node * node, TR::SymbolReference *s, bool isStore, bool canCauseGC);

   TR::SymbolReference * getDataSymbolReference()                        { return _dataSymbolReference; }
   TR::SymbolReference * setDataSymbolReference(TR::SymbolReference *sr) { return (_dataSymbolReference = sr); }

   TR::Instruction * getDataReferenceInstruction()                   { return _dataReferenceInstruction; }
   TR::Instruction * setDataReferenceInstruction(TR::Instruction *i) { return (_dataReferenceInstruction = i); }

   uint8_t * getAddressOfDataReference()           { return _addressOfDataReference; }
   uint8_t * setAddressOfDataReference(uint8_t *p) { return (_addressOfDataReference = p); }

   bool isUnresolvedStore()    { return _flags.testAll(TO_MASK32(IsUnresolvedStore)); }
   void setUnresolvedStore()   { _flags.set(TO_MASK32(IsUnresolvedStore)); }
   void resetUnresolvedStore() { _flags.reset(TO_MASK32(IsUnresolvedStore)); }

   virtual uint8_t *emitSnippetBody() { return NULL; }

   virtual uint32_t getLength(int32_t estimatedSnippetStart) { return 0; }

protected:

   enum
      {
      IsUnresolvedStore = TR::Snippet::NextSnippetFlag,
      NextSnippetFlag
      };

   static_assert((int32_t)NextSnippetFlag <= (int32_t)TR::Snippet::MaxSnippetFlag,
      "OMR::UnresolvedDataSnippet too many flag bits for flag width");

private:

   TR::Instruction * _dataReferenceInstruction;

   TR::SymbolReference * _dataSymbolReference;

   uint8_t * _addressOfDataReference;

   };

}

#endif

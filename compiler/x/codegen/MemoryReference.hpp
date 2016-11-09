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

#ifndef TR_MEMORYREFERENCE_INCL
#define TR_MEMORYREFERENCE_INCL

#include "codegen/OMRMemoryReference.hpp"

#include <stddef.h>        // for NULL
#include <stdint.h>        // for uint8_t
#include "env/jittypes.h"  // for intptrj_t

namespace TR { class IA32DataSnippet; }
class TR_ScratchRegisterManager;
namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }
namespace TR { class Register; }
namespace TR { class SymbolReference; }

namespace TR
{

class OMR_EXTENSIBLE MemoryReference : public OMR::MemoryReferenceConnector
   {
   public:

   MemoryReference(TR::CodeGenerator *cg) :
      OMR::MemoryReferenceConnector(cg) {}

   MemoryReference(TR::Register *br,
      TR::SymbolReference *sr,
      TR::Register *ir,
      uint8_t s,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, sr, ir, s, cg) {}

   MemoryReference(TR::Register *br,
      TR::Register *ir,
      uint8_t s,
      TR::CodeGenerator*cg) :
         OMR::MemoryReferenceConnector(br, ir, s, cg) {}

   MemoryReference(TR::Register *br,
      intptrj_t disp,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, disp, cg) {}

   MemoryReference(intptrj_t disp,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(disp, cg) {}

   MemoryReference(TR::Register *br,
      TR::Register *ir,
      uint8_t s,
      intptrj_t disp,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, ir, s, disp, cg) {}

   MemoryReference(TR::IA32DataSnippet *cds,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(cds, cg) {}

   MemoryReference(TR::LabelSymbol *label,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(label, cg) {}

   MemoryReference(TR::Node *rootLoadOrStore,
      TR::CodeGenerator *cg,
      bool canRematerializeAddressAdds) :
         OMR::MemoryReferenceConnector(rootLoadOrStore, cg, canRematerializeAddressAdds) {}

   MemoryReference(TR::SymbolReference *symRef,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(symRef, cg) {}

   MemoryReference(TR::SymbolReference *symRef,
      intptrj_t displacement,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(symRef, displacement, cg) {}

   MemoryReference(MemoryReference& mr,
      intptrj_t n,
      TR::CodeGenerator *cg,
      TR_ScratchRegisterManager *srm = NULL) :
         OMR::MemoryReferenceConnector(mr, n, cg) {}

   };
}

#endif

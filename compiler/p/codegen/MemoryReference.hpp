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

#include <stdint.h>                        // for uint32_t, int32_t, etc

namespace TR { class SymbolReference; }
namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class Register; }

namespace TR
{

class OMR_EXTENSIBLE MemoryReference : public OMR::MemoryReferenceConnector
   {
   public:

   MemoryReference(TR::CodeGenerator *cg) :
      OMR::MemoryReferenceConnector(cg) {}

   MemoryReference(TR::Register *br,
      TR::Register *ir,
      uint8_t len,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, ir, len, cg) {}

   MemoryReference(TR::Register *br,
      int32_t disp,
      uint8_t len,
      TR::CodeGenerator *cg) :
         OMR::MemoryReferenceConnector(br, disp, len, cg) {}

   MemoryReference(TR::Node *rootLoadOrStore, uint32_t len, TR::CodeGenerator *cg):
      OMR::MemoryReferenceConnector(rootLoadOrStore, len, cg) {}

   MemoryReference(TR::Node *node, TR::SymbolReference *symRef, uint32_t len, TR::CodeGenerator *cg):
      OMR::MemoryReferenceConnector(node, symRef, len, cg) {}

   MemoryReference(TR::Node *node, MemoryReference& mr, int32_t n, uint32_t len, TR::CodeGenerator *cg):
      OMR::MemoryReferenceConnector(node, mr, n, len, cg) {}
   };
}

#endif

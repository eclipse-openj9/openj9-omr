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

#ifndef OMR_SNIPPET_INCL
#define OMR_SNIPPET_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_SNIPPET_CONNECTOR
#define OMR_SNIPPET_CONNECTOR
namespace OMR { class Snippet; }
namespace OMR { typedef OMR::Snippet SnippetConnector; }
#endif

#include <stdint.h>                 // for int32_t, uint8_t, uint32_t
#include "env/FilePointerDecl.hpp"  // for FILE
#include "env/TRMemory.hpp"         // for TR_Memory, etc
#include "infra/Flags.hpp"
#include "infra/Annotations.hpp"    // for OMR_EXTENSIBLE
#include "codegen/SnippetGCMap.hpp"

class TR_Debug;
namespace TR { class Block; }
namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class LabelSymbol; }
namespace TR { class Snippet; }

namespace OMR
{

class OMR_EXTENSIBLE Snippet
   {
   public:

   TR_ALLOC(TR_Memory::Snippet)

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label);

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label, bool isGCSafePoint);

   TR::Snippet *self();

   TR::CodeGenerator *cg() { return _cg; }
   void setCodeGenerator(TR::CodeGenerator *cg) { _cg = cg; }

   TR::Node *getNode() { return _node; }
   void setNode(TR::Node *node) { _node = node; }

   TR::LabelSymbol *getSnippetLabel() {return _snippetLabel;}
   void setSnippetLabel(TR::LabelSymbol *label);

   virtual uint8_t *emitSnippet();
   virtual int32_t setEstimatedCodeLocation(int32_t p);
   virtual uint32_t getLength(int32_t estimatedSnippetStart) = 0;
   virtual uint8_t *emitSnippetBody() = 0;

   virtual void print(TR::FILE *, TR_Debug *debug)
      {
      // temporary until this becomes pure virtual
      }

   void prepareSnippetForGCSafePoint();

   /////////////////////////////////////////////////////////////////////////////
   //
   // Former mixin code -- this still needs to evolve into something better
   //
   public:

   TR::Block *getBlock() { return _block; }
   void setBlock(TR::Block *block) { _block = block; }

   bool isWarmSnippet() { return _flags.testAll(TO_MASK32(WarmSnippet)); }
   void setWarmSnippet() { _flags.set(TO_MASK32(WarmSnippet)); }

   bool needsExceptionTableEntry() { return _flags.testAll(TO_MASK32(NeedsExceptionTableEntry)); }
   void setNeedsExceptionTableEntry() { _flags.set(TO_MASK32(NeedsExceptionTableEntry)); }
   void resetNeedsExceptionTableEntry() { _flags.reset(TO_MASK32(NeedsExceptionTableEntry)); }

   TR::SnippetGCMap &gcMap() { return _gcMap; }

   protected:

   enum
      {
      NeedsExceptionTableEntry = 0,
      WarmSnippet,

      NextSnippetFlag,
      MaxSnippetFlag = (sizeof(uint32_t)*8)-1
      };

   static_assert(NextSnippetFlag <= MaxSnippetFlag, "OMR::SnippetFlags too many flag bits for flag width");

   flags32_t _flags;

   private:

   TR::SnippetGCMap _gcMap;

   /////////////////////////////////////////////////////////////////////////////

   private:

   TR::CodeGenerator *_cg;
   TR::LabelSymbol *_snippetLabel;
   TR::Node *_node;

   protected:

   TR::Block *_block;

   };

}

#endif

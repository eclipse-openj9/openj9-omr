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

#ifndef OMR_Z_S390SNIPPETS_INCL
#define OMR_Z_S390SNIPPETS_INCL

#include "codegen/Snippet.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace TR {

/**
 * Create trampolines from warm code to cold code, when branch targets is not
 * reachable by compare-and-branch instructions.
 */
class S390WarmToColdTrampolineSnippet : public TR::Snippet
   {
   TR::LabelSymbol * _targetLabel;

public:
   S390WarmToColdTrampolineSnippet(TR::CodeGenerator* cg, TR::Node *c, TR::LabelSymbol *lab, TR::LabelSymbol *targetLabel)
      : TR::Snippet(cg, c, lab, false), _targetLabel(targetLabel)
      {
      setWarmSnippet();
      }

   S390WarmToColdTrampolineSnippet(TR::CodeGenerator* cg, TR::Node *c, TR::LabelSymbol *lab, TR::Snippet *targetSnippet)
      : TR::Snippet(cg, c, lab, false), _targetLabel(targetSnippet->getSnippetLabel())
      {
      setWarmSnippet();
      }

   TR::LabelSymbol *getTargetLabel() { return _targetLabel; }
   TR::LabelSymbol *setTargetLabel ( TR::LabelSymbol * targetLabel)
      {
      return _targetLabel = targetLabel;
      }


   virtual uint32_t getLength(int32_t estimatedSnippetStart);
   virtual uint8_t *emitSnippetBody();
   virtual Kind getKind() { return IsWarmToColdTrampoline; }
   };


class S390RestoreGPR7Snippet : public TR::Snippet
   {
   TR::LabelSymbol * _targetLabel;

public:
   S390RestoreGPR7Snippet(TR::CodeGenerator* cg, TR::Node *c, TR::LabelSymbol *lab, TR::LabelSymbol *targetLabel)
      : TR::Snippet(cg, c, lab, false), _targetLabel(targetLabel)
      {
      }

   S390RestoreGPR7Snippet(TR::CodeGenerator* cg, TR::Node *c, TR::LabelSymbol *lab, TR::Snippet *targetSnippet)
      : TR::Snippet(cg, c, lab, false), _targetLabel(targetSnippet->getSnippetLabel())
      {
      }

   TR::LabelSymbol *getTargetLabel() { return _targetLabel; }
   TR::LabelSymbol *setTargetLabel ( TR::LabelSymbol * targetLabel)
      {
      return _targetLabel = targetLabel;
      }

   virtual uint32_t getLength(int32_t estimatedSnippetStart);
   virtual uint8_t *emitSnippetBody();
   virtual Kind getKind() { return IsRestoreGPR7; }

   };

}

#endif

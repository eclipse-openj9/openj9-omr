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

#include "codegen/OMRUnresolvedDataSnippet.hpp"

#include "codegen/CodeGenerator.hpp"                  // for CodeGenerator
#include "codegen/UnresolvedDataSnippet.hpp"          // for UnresolvedDataSnippet
#include "codegen/UnresolvedDataSnippet_inlines.hpp"  // for UnresolvedDataSnippet::self
#include "il/symbol/LabelSymbol.hpp"                  // for LabelSymbol

namespace TR { class Node; }
namespace TR { class SymbolReference; }

OMR::UnresolvedDataSnippet::UnresolvedDataSnippet(TR::CodeGenerator * cg,
                                                  TR::Node * node,
                                                  TR::SymbolReference * symRef,
                                                  bool isStore,
                                                  bool isGCSafePoint) :
   Snippet(cg, node, generateLabelSymbol(cg)),
      _dataSymbolReference(symRef),
      _dataReferenceInstruction(NULL),
      _addressOfDataReference(0)
   {
   if (isStore)
      {
      setUnresolvedStore();
      }

   if (isGCSafePoint)
      {
      self()->prepareSnippetForGCSafePoint();
      }
   }

TR::UnresolvedDataSnippet *
OMR::UnresolvedDataSnippet::create(TR::CodeGenerator * cg, TR::Node * node, TR::SymbolReference *s, bool isStore, bool canCauseGC)
   {
   return new (cg->trHeapMemory()) TR::UnresolvedDataSnippet(cg, node, s, isStore, canCauseGC);
   }

#ifndef J9_PROJECT_SPECIFIC

#include "ras/Debug.hpp"

/*
 * This function is needed because the debug printing infrastructure is separate
 * from this class hierarchy.  At present, the debug print capability is very
 * J9-specific and therefore a simple clean version is needed for non-J9 builds.
 */

void
TR_Debug::print(TR::FILE *pOutFile, TR::UnresolvedDataSnippet * snippet)
   {
   if (pOutFile == NULL)
      return;

   uint8_t *bufferPos = snippet->getSnippetLabel()->getCodeLocation();
   printSnippetLabel(pOutFile, snippet->getSnippetLabel(), bufferPos, getName(snippet));
   }

#endif


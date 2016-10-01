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

#include <stddef.h>                   // for NULL
#include "il/symbol/LabelSymbol.hpp"  // for LabelSymbol
#include "codegen/Snippet.hpp"
#include "codegen/CodeGenerator.hpp"

OMR::Snippet::Snippet(
      TR::CodeGenerator *cg,
      TR::Node *node,
      TR::LabelSymbol *label,
      bool isGCSafePoint) :
   _cg(cg), _node(node), _snippetLabel(label), _block(0), _flags(0)
   {
   self()->setSnippetLabel(label);

   if (isGCSafePoint)
      {
      self()->prepareSnippetForGCSafePoint();
      }

   self()->gcMap().setGCRegisterMask(0xFF00FFFF);
   }


OMR::Snippet::Snippet(
      TR::CodeGenerator *cg,
      TR::Node *node,
      TR::LabelSymbol *label) :
   _cg(cg), _node(node), _snippetLabel(label), _block(0), _flags(0)
   {
   self()->setSnippetLabel(label);
   self()->gcMap().setGCRegisterMask(0xFF00FFFF);
   }

TR::Snippet *
OMR::Snippet::self()
   {
   return static_cast<TR::Snippet *>(this);
   }

void
OMR::Snippet::setSnippetLabel(TR::LabelSymbol *label)
   {
   if (_snippetLabel)
      {
      _snippetLabel->setSnippet(NULL);
      }

   label->setSnippet(static_cast<TR::Snippet *>(this));
   _snippetLabel = label;
   }


int32_t
OMR::Snippet::setEstimatedCodeLocation(int32_t p)
   {
   return self()->getSnippetLabel()->setEstimatedCodeLocation(p);
   }


uint8_t *
OMR::Snippet::emitSnippet()
   {
   return self()->emitSnippetBody();
   }

void
OMR::Snippet::prepareSnippetForGCSafePoint()
   {
   self()->gcMap().setGCSafePoint();
   self()->setBlock(self()->cg()->getCurrentEvaluationBlock());
   self()->setNeedsExceptionTableEntry();
   }


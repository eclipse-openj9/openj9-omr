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

#ifndef TR_SNIPPET_INCL
#define TR_SNIPPET_INCL

#include "codegen/OMRSnippet.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class LabelSymbol; }
namespace TR { class Node; }

namespace TR
{

class OMR_EXTENSIBLE Snippet : public OMR::SnippetConnector
   {
   public:

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label, bool isGCSafePoint) :
      OMR::SnippetConnector(cg, node, label, isGCSafePoint) {}

   Snippet(TR::CodeGenerator *cg, TR::Node *node, TR::LabelSymbol *label) :
      OMR::SnippetConnector(cg, node, label) {}
   };

}

#endif

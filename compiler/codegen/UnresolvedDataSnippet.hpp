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

#ifndef TR_UNRESOLVEDDATASNIPPET_INCL
#define TR_UNRESOLVEDDATASNIPPET_INCL

#include "codegen/OMRUnresolvedDataSnippet.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Node; }
namespace TR { class SymbolReference; }

namespace TR
{

class UnresolvedDataSnippet : public OMR::UnresolvedDataSnippetConnector
   {

public:

   UnresolvedDataSnippet(TR::CodeGenerator *cg, TR::Node *node, TR::SymbolReference *symRef, bool isStore, bool isGCSafePoint) :
      OMR::UnresolvedDataSnippetConnector(cg, node, symRef, isStore, isGCSafePoint) { }
   };

}

#endif


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

#include "codegen/Snippet.hpp"
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator


OMR::Power::Snippet::Snippet(
      TR::CodeGenerator *cg,
      TR::Node *node,
      TR::LabelSymbol *label,
      bool isGCSafePoint) :
   OMR::Snippet(cg, node, label, isGCSafePoint)
   {
   }


OMR::Power::Snippet::Snippet(
      TR::CodeGenerator *cg,
      TR::Node *node,
      TR::LabelSymbol *label) :
   OMR::Snippet(cg, node, label)
   {
   }

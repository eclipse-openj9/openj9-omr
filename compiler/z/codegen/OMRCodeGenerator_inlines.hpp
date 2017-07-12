/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2017
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
 ******************************************************************************/

#ifndef OMR_Z_CODEGENERATOR_INLINES_INCL
#define OMR_Z_CODEGENERATOR_INLINES_INCL

#include "compiler/codegen/OMRCodeGenerator_inlines.hpp"
#include "codegen/OMRCodeGenerator.hpp"

template <class TR_AliasSetInterface>
bool OMR::Z::CodeGenerator::loadAndStoreMayOverlap(::TR::Node *store, size_t storeSize, ::TR::Node *load, size_t loadSize, TR_AliasSetInterface &storeAliases)
   {
   return storeAliases.contains(load->getSymbolReference(), self()->comp());
   }

#endif // OMR_Z_CODEGENERATOR_INLINES_INCL

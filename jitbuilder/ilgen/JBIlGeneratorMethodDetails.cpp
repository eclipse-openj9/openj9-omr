/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2014, 2016
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

#include "codegen/FrontEnd.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/IlInjector.hpp"
#include "env/FrontEnd.hpp"
#include "compile/Compilation.hpp"
#include "compile/Method.hpp"
#include "compile/InlineBlock.hpp"
#include "compile/ResolvedMethod.hpp"
#include "env/IO.hpp"

namespace JitBuilder
{

IlGeneratorMethodDetails::IlGeneratorMethodDetails(TR_ResolvedMethod *method) :
   _method(static_cast<TR::ResolvedMethod *>(method))
   {
   }


bool
IlGeneratorMethodDetails::sameAs(TR::IlGeneratorMethodDetails & other, TR_FrontEnd *fe)
   {
   return self()->getMethod() == other.getMethod();
   }


TR_IlGenerator *
IlGeneratorMethodDetails::getIlGenerator(TR::ResolvedMethodSymbol *methodSymbol,
                                         TR_FrontEnd * fe,
                                         TR::Compilation *comp,
                                         TR::SymbolReferenceTable *symRefTab,
                                         bool forceClassLookahead,
                                         TR_InlineBlocks *blocksToInline)
   {
   TR_ASSERT(forceClassLookahead == false, "IlGenerator does not support class lookahead");
   TR_ASSERT(blocksToInline == 0, "IlGenerator does not yet support partial inlining");
   TR::ResolvedMethod *method = static_cast<TR::ResolvedMethod *>(methodSymbol->getResolvedMethod());
   return (TR_IlGenerator *) method->getInjector(self(), methodSymbol, static_cast<TR::FrontEnd *>(fe), symRefTab);
   }


void
IlGeneratorMethodDetails::print(TR_FrontEnd *fe, TR::FILE *file)
   {
   if (file == NULL)
      return;

   trfprintf(file, "( %p )", self()->getMethod());
   }

} // namespace JitBuilder

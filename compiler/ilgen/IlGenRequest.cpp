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

#include "ilgen/IlGenRequest.hpp"

#include <stddef.h>                            // for NULL
#include "codegen/FrontEnd.hpp"
#include "ilgen/IlGen.hpp"                     // for TR_IlGenerator
#include "ilgen/IlGeneratorMethodDetails.hpp"
#include "env/IO.hpp"

namespace TR { class Compilation; }
namespace TR { class ResolvedMethodSymbol; }
namespace TR { class SymbolReferenceTable; }

namespace TR
{

TR_IlGenerator *
CompileIlGenRequest::getIlGenerator(
      TR::ResolvedMethodSymbol *methodSymbol,
      TR_FrontEnd *fe,
      TR::Compilation *comp,
      TR::SymbolReferenceTable *symRefTab)
   {
   return details().getIlGenerator(methodSymbol, fe, comp, symRefTab, false, NULL);
   }


void
CompileIlGenRequest::print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix)
   {
   trfprintf(file, "{");
   details().print(fe, file);
   trfprintf(file, "}%s", suffix);
   }


TR_IlGenerator *
InliningIlGenRequest::getIlGenerator(
      TR::ResolvedMethodSymbol *methodSymbol,
      TR_FrontEnd *fe,
      TR::Compilation *comp,
      TR::SymbolReferenceTable *symRefTab)
   {
   TR_IlGenerator *ilgen = details().getIlGenerator(methodSymbol, fe, comp, symRefTab, false, 0);
   ilgen->setCallerMethod(_callerSymbol);
   return ilgen;
   }


void
InliningIlGenRequest::print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix)
   {
   trfprintf(file, "{Inlining ");
   details().print(fe, file);
   trfprintf(file, "}%s", suffix);
   }


TR_IlGenerator *
PartialInliningIlGenRequest::getIlGenerator(
      TR::ResolvedMethodSymbol *methodSymbol,
      TR_FrontEnd *fe,
      TR::Compilation *comp,
      TR::SymbolReferenceTable *symRefTab)
   {
   TR_IlGenerator *ilgen = details().getIlGenerator(methodSymbol, fe, comp, symRefTab, false, _inlineBlocks);
   ilgen->setCallerMethod(_callerSymbol);
   return ilgen;
   }


void
PartialInliningIlGenRequest::print(TR_FrontEnd *fe, TR::FILE *file, const char *suffix)
   {
   trfprintf(file, "{Partial inlining ");
   details().print(fe, file);
   trfprintf(file, "}%s", suffix);
   }


}

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
 ******************************************************************************/

#ifndef TR_RESOLVEDMETHODSYMBOL_INCL
#define TR_RESOLVEDMETHODSYMBOL_INCL

#include "il/symbol/OMRResolvedMethodSymbol.hpp"

class TR_ResolvedMethod;
namespace TR { class Compilation; }

namespace TR
{

class OMR_EXTENSIBLE ResolvedMethodSymbol : public OMR::ResolvedMethodSymbolConnector
   {

protected:

   ResolvedMethodSymbol(TR_ResolvedMethod * m, TR::Compilation * c) :
      OMR::ResolvedMethodSymbolConnector(m, c) {}

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::ResolvedMethodSymbol;

   };

}

#endif


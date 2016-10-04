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

#ifndef TR_METHODSYMBOL_INCL
#define TR_METHODSYMBOL_INCL

#include "il/symbol/OMRMethodSymbol.hpp"

#include <stddef.h>                            // for NULL
#include "codegen/LinkageConventionsEnum.hpp"  // for TR_LinkageConventions, etc

class TR_Method;

namespace TR
{

class OMR_EXTENSIBLE MethodSymbol : public OMR::MethodSymbolConnector
   {

protected:

   MethodSymbol(TR_LinkageConventions lc = TR_Private, TR_Method* m = NULL) :
      OMR::MethodSymbolConnector(lc, m) { }

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::MethodSymbol;

   };

}

#endif


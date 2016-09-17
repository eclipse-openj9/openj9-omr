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

#ifndef TR_LABELSYMBOL_INCL
#define TR_LABELSYMBOL_INCL

#include "il/symbol/OMRLabelSymbol.hpp"

namespace TR { class Block; }
namespace TR { class CodeGenerator; }

namespace TR {

class LabelSymbol : public OMR::LabelSymbolConnector
   {

protected:

   LabelSymbol() :
      OMR::LabelSymbolConnector() { }

   LabelSymbol(TR::CodeGenerator *codeGen) :
      OMR::LabelSymbolConnector(codeGen) { }

   LabelSymbol(TR::CodeGenerator *codeGen, TR::Block *labb):
      OMR::LabelSymbolConnector(codeGen, labb) { }

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::LabelSymbol;

   };

}

#endif


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

#ifndef TR_AUTOMATICSYMBOL_INCL
#define TR_AUTOMATICSYMBOL_INCL

#include "il/symbol/OMRAutomaticSymbol.hpp"

#include <stdint.h>          // for uint32_t, int32_t
#include "il/DataTypes.hpp"  // for DataTypes

namespace TR { class Compilation; }

namespace TR
{

class OMR_EXTENSIBLE AutomaticSymbol : public OMR::AutomaticSymbolConnector
   {

protected:

   AutomaticSymbol(int32_t o = 0) :
      OMR::AutomaticSymbolConnector() { }

   AutomaticSymbol(TR::DataType d) :
      OMR::AutomaticSymbolConnector(d) { }

   AutomaticSymbol(TR::DataType d, uint32_t s) :
      OMR::AutomaticSymbolConnector(d, s) { }

   AutomaticSymbol(TR::DataType d, uint32_t s, const char * name) :
      OMR::AutomaticSymbolConnector(d, s, name) { }

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::AutomaticSymbol;

   };

}

#endif


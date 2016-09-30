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

#ifndef TR_STATICSYMBOL_INCL
#define TR_STATICSYMBOL_INCL

#include "il/symbol/OMRStaticSymbol.hpp"

#include <stdint.h>          // for uint32_t
#include "il/DataTypes.hpp"  // for DataTypes

/**
 * A symbol with an address
 */
namespace TR
{

class OMR_EXTENSIBLE StaticSymbol : public OMR::StaticSymbolConnector
   {

protected:

   StaticSymbol(TR::DataType d) :
      OMR::StaticSymbolConnector(d) { }

   StaticSymbol(TR::DataType d, void * address) :
      OMR::StaticSymbolConnector(d,address) { }

   StaticSymbol(TR::DataType d, uint32_t s) :
      OMR::StaticSymbolConnector(d, s) { }

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::StaticSymbol;

   };

}

#endif


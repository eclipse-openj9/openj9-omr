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

#ifndef TR_PARAMETERSYMBOL_INCL
#define TR_PARAMETERSYMBOL_INCL

#include "il/symbol/OMRParameterSymbol.hpp"

#include <stddef.h>          // for size_t
#include <stdint.h>          // for int32_t
#include "il/DataTypes.hpp"  // for DataTypes

namespace TR
{

class OMR_EXTENSIBLE ParameterSymbol : public OMR::ParameterSymbolConnector
   {

protected:

   ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot) :
      OMR::ParameterSymbolConnector(d, isUnsigned, slot) { }

   ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot, size_t size) :
      OMR::ParameterSymbolConnector(d, isUnsigned, slot, size) { }

private:

   // When adding another class to the heirarchy, add it as a friend here
   friend class OMR::ParameterSymbol;

   };

}

#endif


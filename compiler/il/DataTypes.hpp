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

#ifndef TR_DATATYPES_INCL
#define TR_DATATYPES_INCL

#include "il/OMRDataTypes.hpp"

namespace TR
{ 

class DataType : public OMR::DataTypeConnector
   { 

public:
   DataType() : OMR::DataTypeConnector() { }
   DataType(TR::DataTypes t) : OMR::DataTypeConnector(t) { }

   };

}

#endif

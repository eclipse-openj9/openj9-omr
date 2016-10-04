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

#ifndef TR_REGISTER_INCL
#define TR_REGISTER_INCL

#include "infra/List.hpp"

#include "codegen/OMRRegister.hpp"

#include <stdint.h>                       // for uint16_t, uint32_t
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds

namespace TR
{

class OMR_EXTENSIBLE Register: public OMR::RegisterConnector
   {
   public:

   Register(uint32_t f=0): OMR::RegisterConnector(f) {}
   Register(TR_RegisterKinds rk): OMR::RegisterConnector(rk) {}
   Register(TR_RegisterKinds rk, uint16_t ar): OMR::RegisterConnector(rk, ar) {}

   };

}

#endif /* TR_REGISTER_INCL */

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

#ifndef TR_REGISTER_PAIR_INCL
#define TR_REGISTER_PAIR_INCL

#include "codegen/RegisterConstants.hpp"         // for TR_RegisterKinds
#include "codegen/OMRRegisterPair.hpp"  // for RegisterPair

namespace TR { class Register; }

namespace TR
{

class OMR_EXTENSIBLE RegisterPair: public OMR::RegisterPairConnector
   {
   public:

   RegisterPair() {}
   RegisterPair(TR_RegisterKinds rk) : OMR::RegisterPairConnector(rk) {}
   RegisterPair(TR::Register *lo, TR::Register *ho) : OMR::RegisterPairConnector(lo, ho) {}

   };

}

#endif

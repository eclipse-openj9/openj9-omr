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

#ifndef OMR_Z_REGISTER_PAIR_INCL
#define OMR_Z_REGISTER_PAIR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_PAIR_CONNECTOR
#define OMR_REGISTER_PAIR_CONNECTOR
   namespace OMR { namespace Z { class RegisterPair; } }
   namespace OMR { typedef OMR::Z::RegisterPair RegisterPairConnector; }
#else
   #error OMR::Z::RegisterPair expected to be a primary connector, but a OMR connector is already defined
#endif


#include "compiler/codegen/OMRRegisterPair.hpp"

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE RegisterPair: public OMR::RegisterPair
   {
   protected:

   RegisterPair() {}
   RegisterPair(TR_RegisterKinds rk):  OMR::RegisterPair(rk) {}
   RegisterPair(TR::Register *lo, TR::Register *ho);

   public:

   TR::Register * setLowOrder(TR::Register *lo, TR::CodeGenerator *codeGen);
   TR::Register * setHighOrder(TR::Register *ho, TR::CodeGenerator *codeGen);


   };

}

}

#endif

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

//On zOS XLC linker can't handle files with same name at link time
//This workaround with pragma is needed. What this does is essentially
//give a different name to the codesection (csect) for this file. So it
//doesn't conflict with another file with same name.
#pragma csect(CODE,"OMRZRegPairBase#C")
#pragma csect(STATIC,"OMRZRegPairBase#S")
#pragma csect(TEST,"OMRZRegPairBase#T")

#include "z/codegen/OMRRegisterPair.hpp"

#include "codegen/CodeGenerator.hpp"
#include "codegen/LiveRegister.hpp"
#include "codegen/Register.hpp"
#include "codegen/RegisterPair.hpp"
#include "il/AliasSetInterface.hpp"
#include "il/SymbolReference.hpp"
#include "optimizer/Structure.hpp"

OMR::Z::RegisterPair::RegisterPair(TR::Register *lo, TR::Register *ho):
   OMR::RegisterPair(lo, ho)
   {
   // highword RA HPR upgrade
   if (lo)
      lo->setIsNotHighWordUpgradable(true);
   if (ho)
      ho->setIsNotHighWordUpgradable(true);
   }


TR::Register *
OMR::Z::RegisterPair::setLowOrder(TR::Register *lo, TR::CodeGenerator *codeGen)
   {
   // highword RA HPR upgrade
   if (lo) lo->setIsNotHighWordUpgradable(true);

   // call base class implementation
   return OMR::RegisterPair::setLowOrder(lo, codeGen);
   }


TR::Register *
OMR::Z::RegisterPair::setHighOrder(TR::Register *ho, TR::CodeGenerator *codeGen)
   {
   // highword RA HPR upgrade
   if (ho) ho->setIsNotHighWordUpgradable(true);

   // call base class implementation
   return OMR::RegisterPair::setHighOrder(ho, codeGen);
   }

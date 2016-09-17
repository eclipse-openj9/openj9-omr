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

#ifndef TR_REAL_REGISTER_INCL
#define TR_REAL_REGISTER_INCL

#include "codegen/OMRRealRegister.hpp"

#include <stddef.h>                       // for NULL
#include <stdint.h>                       // for uint16_t
#include "codegen/Register.hpp"           // for Register
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds
#include "infra/Assert.hpp"               // for TR_ASSERT

namespace TR { class CodeGenerator; }

namespace TR
{

class OMR_EXTENSIBLE RealRegister : public OMR::RealRegisterConnector
   {
   public:

   RealRegister(TR::CodeGenerator *cg): OMR::RealRegisterConnector(cg) {}

   RealRegister(TR_RegisterKinds rk, uint16_t w, RegState s, RegNum rn, RegMask m, TR::CodeGenerator * cg):
      OMR::RealRegisterConnector(rk, w, s, rn, m, cg) {}

   };

}

inline TR::RealRegister *toRealRegister(TR::Register *r)
   {
   TR_ASSERT(r == NULL || r->getRealRegister() != NULL, "trying to convert a non-real register to a real register");
   return static_cast<TR::RealRegister *>(r);
   }

#endif

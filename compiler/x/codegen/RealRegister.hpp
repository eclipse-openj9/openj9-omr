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

#include <stdint.h>                              // for uint16_t
#include "codegen/RegisterConstants.hpp"         // for TR_RegisterKinds

namespace TR { class CodeGenerator; }
namespace TR { class Register; }

namespace TR
{

class OMR_EXTENSIBLE RealRegister : public OMR::RealRegisterConnector
   {
   public:

   RealRegister(TR_RegisterKinds rk, uint16_t w, RegState s, TR::RealRegister::RegNum  ri, TR::RealRegister::RegMask m, TR::CodeGenerator *cg):
      OMR::RealRegisterConnector(rk, w, s, (RegNum)ri, (RegMask)m, cg) {}

   };

}

inline TR::RealRegister *toRealRegister(TR::Register *r)
   {
   return static_cast<TR::RealRegister *>(r);
   }


#endif

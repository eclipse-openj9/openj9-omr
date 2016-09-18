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

#ifndef OMR_X86_REAL_REGISTER_INCL
#define OMR_X86_REAL_REGISTER_INCL

#ifndef OMR_REAL_REGISTER_CONNECTOR
#define OMR_REAL_REGISTER_CONNECTOR
namespace OMR { namespace X86 { class RealRegister; } }
namespace OMR { typedef OMR::X86::RealRegister RealRegisterConnector; }
#endif

#include "compiler/codegen/OMRRealRegister.hpp"

#include <stdint.h>                       // for uint8_t, uint16_t, etc
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds
#include "infra/Assert.hpp"               // for TR_ASSERT

namespace TR { class CodeGenerator; }
namespace TR { class RealRegister; }

namespace OMR
{

namespace X86
{

class OMR_EXTENSIBLE RealRegister : public OMR::RealRegister
   {
   protected:

   RealRegister(TR::CodeGenerator *cg) : OMR::RealRegister(cg, NoReg) {}

   RealRegister(TR_RegisterKinds   rk,
                uint16_t           w,
                RegState           s,
                RegNum             ri,
                RegMask            m,
                TR::CodeGenerator *cg) :
      OMR::RealRegister(rk, w, s, (uint16_t)ri, ri, m, cg) {}

   public:

   // Constants for AMD64 Rex prefix calculations
   // TODO:AMD64: Consider putting this in an AMD64Register subclass
   //
   enum { REX=0x40, REX_W=0x08, REX_R=0x04, REX_X=0x02, REX_B=0x01 };

   static TR::RealRegister *regMaskToRealRegister(TR_RegisterMask mask, TR_RegisterKinds rk, TR::CodeGenerator *cg);
   static TR_RegisterMask getAvailableRegistersMask(TR_RegisterKinds rk);
   static RegMask getRealRegisterMask(TR_RegisterKinds rk, RegNum idx);
   using OMR::RealRegister::getRealRegisterMask;  // for getting the _registerMask member

   protected:

   // TODO:AMD64: Consider making this back into a plain old byte for consistency with other platforms.
   struct TR_RegisterBinaryEncoding
      {
      unsigned char id:3;              // 3-bit register identifier used in ModRM and SIB bytes
      unsigned char needsRexPlusRXB:1; // Always need a Rex prefix with the R, X, or B bit set
      unsigned char needsRexForByte:1; // Needs a Rex prefix when used as a byte register
      unsigned char needsDisp:1;       // Needs a displacement field (even if zero) when used as a base reg (ie. ebp/r13)
      unsigned char needsSIB:1;        // Needs a SIB byte when used as a base reg (ie. esp/r12)
      };
   };

}

}

#endif

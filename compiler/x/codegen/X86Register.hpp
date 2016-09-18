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

#ifndef X86REGISTER_INCL
#define X86REGISTER_INCL

#include "codegen/RealRegister.hpp"       // for RealRegister, etc
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds::TR_X87

class TR_X86FPStackRegister;
namespace TR { class CodeGenerator; }
namespace TR { class Register; }

// TODO:AMD64: Re-evaluate the safety of this function, because not
// all Registers are FPStackRegisters.
inline TR_X86FPStackRegister * toX86FPStackRegister(TR::Register *r)
   {
   return (TR_X86FPStackRegister *)r;
   }

class TR_X86FPStackRegister : public TR::RealRegister
   {
   public:

   typedef enum
      {
      fpStackEmpty      = -1,
      fp0               = 0,
      fpFirstStackReg   = fp0,
      fp1               = 1,
      fp2               = 2,
      fp3               = 3,
      fp4               = 4,
      fp5               = 5,
      fp6               = 6,
      fp7               = 7,
      fpLastStackReg    = fp7,
      fpStackFull       = 8,
      NumRegisters      = fp7 + 1
      } TR_X86FPStackRegisters;

   TR_X86FPStackRegister(RegState s, TR_X86FPStackRegisters rn, TR::RealRegister::RegNum ri, TR::CodeGenerator *cg) :
      TR::RealRegister(TR_X87, 0, s, ri, TR::RealRegister::noRegMask, cg), _fpStackRegisterNumber(rn) {}

   TR_X86FPStackRegisters getFPStackRegisterNumber() {return _fpStackRegisterNumber;}
   TR_X86FPStackRegisters setFPStackRegisterNumber(TR_X86FPStackRegisters rn) {return (_fpStackRegisterNumber = rn);}

   private:

   TR_X86FPStackRegisters  _fpStackRegisterNumber;

   };

#endif

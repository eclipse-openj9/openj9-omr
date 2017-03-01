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

#ifndef OMR_Z_REAL_REGISTER_INCL
#define OMR_Z_REAL_REGISTER_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REAL_REGISTER_CONNECTOR
#define OMR_REAL_REGISTER_CONNECTOR
namespace OMR { namespace Z { class RealRegister; } }
namespace OMR { typedef OMR::Z::RealRegister RealRegisterConnector; }
#else
#error OMR::Z::RealRegister expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRRealRegister.hpp"

#include <stdint.h>                              // for uint32_t, int32_t, etc
#include "codegen/RegisterConstants.hpp"         // for TR_RegisterKinds
#include "infra/Flags.hpp"                       // for flags8_t

namespace TR { class CodeGenerator; }
namespace TR { class RealRegister; }

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE RealRegister : public OMR::RealRegister
   {
   protected:

   RealRegister(TR::CodeGenerator *cg);
   RealRegister(TR_RegisterKinds, uint16_t, RegState, RegNum, RegMask, TR::CodeGenerator *);

   public:

   // getters/setters
   void setHighWordRegister(TR::RealRegister *r) {_highWordRegister = r;}
   void setLowWordRegister(TR::RealRegister *r) {_lowWordRegister = r;}

   TR::RealRegister * getHighWordRegister();
   TR::RealRegister * getLowWordRegister();
   TR::RealRegister * getSiblingWordRegister();

   bool isHighWordRegister();
   bool isLowWordRegister();



   // methods for manipulating flags
   bool getModified() {return _modified.testAny(isModified);}
   bool setModified(bool b)  { (b)? _modified.set(isModified) : _modified.reset(isModified);  return b; }

   bool getAssignedHigh() {return _realRegFlags.testAny(isAssignedHigh);}
   bool setAssignedHigh(bool b) {b? _realRegFlags.set(isAssignedHigh) : _realRegFlags.reset(isAssignedHigh); return b;}

   bool setHasBeenAssignedInMethod(bool b); // derived from base class



   // member methods that call static methods which set register field
   void setBaseRegisterField(uint32_t *instruction);
   void setIndexRegisterField(uint32_t *instruction);
   void setRegisterField(uint32_t *instruction);
   void setRegisterField(uint32_t *instruction, int32_t nibbleIndex); // nibbleIndex-> rightmost nibble=0, leftmost nibble=7
   void setRegister1Field(uint32_t *instruction);
   void setRegister2Field(uint32_t *instruction);
   void setRegister3Field(uint32_t *instruction);
   void setRegister4Field(uint32_t *instruction);

   // static methods for setting register field
   static void setRegisterRXBField(uint32_t *instruction, RegNum reg, int index);

   static void setBaseRegisterField(uint32_t *instruction,RegNum reg);
   static void setIndexRegisterField(uint32_t *instruction,RegNum reg);
   static void setRegisterField(uint32_t *instruction,RegNum reg);
   static void setRegisterField(uint32_t *instruction, int32_t nibbleIndex,RegNum reg);

   static void setRegister1Field(uint32_t *instruction,RegNum reg);
   static void setRegister2Field(uint32_t *instruction,RegNum reg);
   static void setRegister3Field(uint32_t *instruction,RegNum reg);
   static void setRegister4Field(uint32_t *instruction,RegNum reg);

   // static methods for checking register type
   static bool isPseudoRealReg(RegNum reg);
   static bool isRealReg(RegNum reg);
   static bool isGPR(RegNum reg);
   static bool isFPR(RegNum reg);
   static bool isAR(RegNum reg);
   static bool isHPR(RegNum reg);
   static bool isVRF(RegNum reg);

   static uint64_t getBitMask(RegNum reg);
   static uint64_t getBitMask(int32_t regNum);

   private:

   enum
      {
      isAssignedHigh  = 0x02,  // Indicates use of high word on 32-bit platform
      isModified      = 0x01,
      };

   TR::RealRegister *_highWordRegister;
   TR::RealRegister *_lowWordRegister;

   flags8_t        _modified;
   static const uint8_t _fullRegBinaryEncodings[NumRegisters];

   };

}

}


#endif

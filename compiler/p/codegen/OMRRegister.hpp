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

#ifndef OMR_POWER_REGISTER_INCL
#define OMR_POWER_REGISTER_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_CONNECTOR
#define OMR_REGISTER_CONNECTOR
namespace OMR { namespace Power { class Register; } }
namespace OMR { typedef OMR::Power::Register RegisterConnector; }
#else
#error OMR::Power::Register expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRRegister.hpp"

class TR_LiveRegisterInfo;

namespace OMR
{

namespace Power
{

class OMR_EXTENSIBLE Register: public OMR::Register
   {
   protected:

   Register(uint32_t f=0): OMR::Register(f) {_liveRegisterInfo._liveRegister = NULL;}
   Register(TR_RegisterKinds rk): OMR::Register(rk)  {_liveRegisterInfo._liveRegister = NULL;}
   Register(TR_RegisterKinds rk, uint16_t ar): OMR::Register(rk, ar) {_liveRegisterInfo._liveRegister = NULL;}


   public:
   /*
    * Getter/setters
    */
   TR_LiveRegisterInfo *getLiveRegisterInfo()                       {return _liveRegisterInfo._liveRegister;}
   TR_LiveRegisterInfo *setLiveRegisterInfo(TR_LiveRegisterInfo *p) {return (_liveRegisterInfo._liveRegister = p);}

   uint64_t getInterference()           {return _liveRegisterInfo._interference;}
   uint64_t setInterference(uint64_t i) {return (_liveRegisterInfo._interference = i);}


   /*
    * Method for manipulating flags
    */
   bool isFlippedCCR()  {return _flags.testAny(FlipBranchOffThisCCR);}
   void setFlippedCCR() {_flags.set(FlipBranchOffThisCCR);}
   void resetFlippedCCR() {_flags.reset(FlipBranchOffThisCCR);}


   private:

   enum
      {
      FlipBranchOffThisCCR          = 0x4000, // PPC may have swapped register positions in compare
      };

   // Both x and z also have this union but ppc uses uint64_t instead of uint32_t
   union
      {
      TR_LiveRegisterInfo *_liveRegister; // Live register entry representing this register
      uint64_t             _interference; // Real registers that interfere with this register
      } _liveRegisterInfo;

   };

}

}

#endif /* OMR_PPC_REGISTER_INCL */

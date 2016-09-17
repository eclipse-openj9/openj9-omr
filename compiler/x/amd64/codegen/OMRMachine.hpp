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

#ifndef OMR_AMD64_MACHINE_INCL
#define OMR_AMD64_MACHINE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_MACHINEBASE_CONNECTOR
#define OMR_MACHINEBASE_CONNECTOR
namespace OMR { namespace X86 { namespace AMD64 { class Machine; } } }
namespace OMR { typedef OMR::X86::AMD64::Machine MachineConnector; }
#else
#error OMR::X86::AMD64::Machine expected to be a primary connector, but a OMR connector is already defined
#endif

#include "x/codegen/OMRMachine.hpp"

#include <stdint.h>                           // for uint8_t, uint32_t
namespace TR { class CodeGenerator; }
namespace TR { class RealRegister; }
namespace TR { class Register; }
namespace TR { class Machine; }

namespace OMR
{

namespace X86
{

namespace AMD64
{

class OMR_EXTENSIBLE Machine : public OMR::X86::Machine
   {

   enum
      {
      AMD64_NUM_GPR              = 16,
      AMD64_NUM_FPR              = 8,  // x87 registers
      AMD64_NUM_XMMR             = 16,
      AMD64_MAX_GLOBAL_GPRS      = 14,
      AMD64_MAX_8BIT_GLOBAL_GPRS = AMD64_MAX_GLOBAL_GPRS,
      AMD64_MAX_GLOBAL_FPRS      = 16,
      };

   TR::RealRegister  *_registerFileStorage[TR_X86_REGISTER_FILE_SIZE];
   TR::Register         *_registerAssociationsStorage[TR_X86_REGISTER_FILE_SIZE];
   TR::Register         *_xmmGlobalRegisterStorage[AMD64_NUM_XMMR];
   uint32_t _globalRegisterNumberToRealRegisterMapStorage[AMD64_MAX_GLOBAL_GPRS + AMD64_MAX_GLOBAL_FPRS];

   public:

   Machine(TR::CodeGenerator *cg);

   static uint8_t numGPRRegsWithheld(TR::CodeGenerator *cg);
   static uint8_t numRegsWithheld(TR::CodeGenerator *cg);

   static bool disableNewPickRegister()
      {
      if (!_dnprIsInitialized)
         {
         _dnprIsInitialized = true;
         }
      return _disableNewPickRegister;
      }

   static bool enableNewPickRegister();

   private:

   static bool _disableNewPickRegister, _dnprIsInitialized;

   };

} // namespace AMD64

} // namespace X86

} // namespace OMR

#endif

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

#include "x/amd64/codegen/OMRMachine.hpp"

#include <stdint.h>                   // for uint8_t
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "compile/Compilation.hpp"    // for Compilation
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"

bool OMR::X86::AMD64::Machine::_disableNewPickRegister, OMR::X86::AMD64::Machine::_dnprIsInitialized=false;

OMR::X86::AMD64::Machine::Machine(TR::CodeGenerator *cg)
   : OMR::X86::Machine
      (
      AMD64_NUM_GPR, // TODO:AMD64: What do these actually do? Possibly
      AMD64_NUM_FPR, // clean up TR_Machine to remove them.
      cg,
      _registerFileStorage,
      _registerAssociationsStorage,
      TR::Machine::enableNewPickRegister()? (AMD64_MAX_GLOBAL_GPRS      - TR::Machine::numGPRRegsWithheld(cg)) : 8,
      TR::Machine::enableNewPickRegister()? (AMD64_MAX_8BIT_GLOBAL_GPRS - TR::Machine::numRegsWithheld(cg)) : 8,
      TR::Machine::enableNewPickRegister()? (AMD64_MAX_GLOBAL_FPRS      - TR::Machine::numRegsWithheld(cg)) : 8,
      _xmmGlobalRegisterStorage,
      _globalRegisterNumberToRealRegisterMapStorage
      )
   {};

uint8_t OMR::X86::AMD64::Machine::numGPRRegsWithheld(TR::CodeGenerator *cg)
   {
   return cg->comp()->getOption(TR_DisableRegisterPressureSimulation)? 2 : 0;
   }

uint8_t OMR::X86::AMD64::Machine::numRegsWithheld(TR::CodeGenerator *cg)
   {
   return cg->comp()->getOption(TR_DisableRegisterPressureSimulation)? 2 : 0;
   }

bool OMR::X86::AMD64::Machine::enableNewPickRegister()
   {
   return !TR::Machine::disableNewPickRegister();
   }

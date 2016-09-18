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

#include <stddef.h>                       // for NULL
#include "codegen/Machine.hpp"            // for Machine
#include "codegen/RealRegister.hpp"       // for RealRegister
#include "codegen/Register.hpp"           // for Register
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds, etc
#include "codegen/RegisterIterator.hpp"   // for RegisterIterator
#include "infra/Assert.hpp"               // for TR_ASSERT

OMR::X86::RegisterIterator::RegisterIterator(TR::Machine *machine, TR_RegisterKinds kind)
   {
   _machine = machine;
   if (kind == TR_GPR)
      {
      _firstRegIndex = TR::RealRegister::eax;
      _lastRegIndex = TR::RealRegister::LastAssignableGPR;
      }
   else if (kind == TR_FPR)
      {
      _firstRegIndex = TR::RealRegister::xmm0;
      _lastRegIndex = TR::RealRegister::LastXMMR;
      }
   else
      {
      TR_ASSERT(0, "Bad register kind for X86\n");
      }
   _cursor = _firstRegIndex;
   }

TR::Register *
OMR::X86::RegisterIterator::getFirst()
   {
   return _machine->getX86RealRegister((TR::RealRegister::RegNum)(_cursor = _firstRegIndex));
   }

TR::Register *
OMR::X86::RegisterIterator::getCurrent()
   {
   return _machine->getX86RealRegister((TR::RealRegister::RegNum)_cursor);
   }

TR::Register *
OMR::X86::RegisterIterator::getNext()
   {
   return _cursor == _lastRegIndex ? NULL : _machine->getX86RealRegister((TR::RealRegister::RegNum)(++_cursor));
   }

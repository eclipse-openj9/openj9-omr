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

#include "codegen/Machine.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/RegisterIterator.hpp"


OMR::Z::RegisterIterator::RegisterIterator ( TR::Machine * machine, TR_RegisterKinds kind )
   {
   _machine = machine;
   if (kind == TR_GPR)
      {
      _firstRegIndex = TR::RealRegister::FirstGPR;
      _lastRegIndex = TR::RealRegister::LastAssignableGPR;
      }
   else if (kind == TR_FPR)
      {
      _firstRegIndex = TR::RealRegister::FirstFPR;
      _lastRegIndex = TR::RealRegister::LastFPR;
      }
   else if (kind == TR_HPR)
      {
      _firstRegIndex = TR::RealRegister::FirstHPR;
      _lastRegIndex = TR::RealRegister::LastHPR;
      }
   else if (kind == TR_AR)
      {
      _firstRegIndex = TR::RealRegister::FirstAR;
      _lastRegIndex = TR::RealRegister::LastAR;
      }
   else if (kind == TR_VRF)
      {
      _firstRegIndex = TR::RealRegister::FirstVRF;
      _lastRegIndex  = TR::RealRegister::LastVRF;
      }
   else
      {
      TR_ASSERT(0, "Bad register kind for S390\n");
      }
   _cursor = _firstRegIndex;
   }

TR::Register *
OMR::Z::RegisterIterator::getFirst()
   {
   return _machine->getS390RealRegister(_cursor = _firstRegIndex);
   }

TR::Register *
OMR::Z::RegisterIterator::getCurrent()
   {
   return _machine->getS390RealRegister(_cursor);
   }

TR::Register *
OMR::Z::RegisterIterator::getNext()
   {
   return _cursor == _lastRegIndex ? NULL : _machine->getS390RealRegister(++_cursor);
   }

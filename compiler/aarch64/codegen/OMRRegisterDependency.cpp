/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include <stddef.h>
#include "codegen/CodeGenerator.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/RegisterDependency.hpp"
#include "il/Node.hpp"

// Replace this by #include "codegen/Instruction.hpp" when available for aarch64
namespace TR { class Instruction; }

OMR::ARM64::RegisterDependencyConditions::RegisterDependencyConditions(
                                       TR::CodeGenerator *cg,
                                       TR::Node          *node,
                                       uint32_t          extranum,
                                       TR::Instruction  **cursorPtr)
   {
   TR_ASSERT(false, "Not implemented yet.");
   }

void OMR::ARM64::RegisterDependencyConditions::unionNoRegPostCondition(TR::Register *reg, TR::CodeGenerator *cg)
   {
   addPostCondition(reg, TR::RealRegister::NoReg);
   }

bool OMR::ARM64::RegisterDependencyConditions::refsRegister(TR::Register *r)
   {
   for (uint16_t i = 0; i < _addCursorForPre; i++)
      {
      if (_preConditions->getRegisterDependency(i)->getRegister() == r &&
          _preConditions->getRegisterDependency(i)->getRefsRegister())
         {
         return true;
         }
      }
   for (uint16_t j = 0; j < _addCursorForPost; j++)
      {
      if (_postConditions->getRegisterDependency(j)->getRegister() == r &&
          _postConditions->getRegisterDependency(j)->getRefsRegister())
         {
         return true;
         }
      }
   return false;
   }

bool OMR::ARM64::RegisterDependencyConditions::defsRegister(TR::Register *r)
   {
   for (uint16_t i = 0; i < _addCursorForPre; i++)
      {
      if (_preConditions->getRegisterDependency(i)->getRegister() == r &&
          _preConditions->getRegisterDependency(i)->getDefsRegister())
         {
         return true;
         }
      }
   for (uint16_t j = 0; j < _addCursorForPost; j++)
      {
      if (_postConditions->getRegisterDependency(j)->getRegister() == r &&
          _postConditions->getRegisterDependency(j)->getDefsRegister())
         {
         return true;
         }
      }
   return false;
   }

bool OMR::ARM64::RegisterDependencyConditions::defsRealRegister(TR::Register *r)
   {
   for (uint16_t i = 0; i < _addCursorForPre; i++)
      {
      if (_preConditions->getRegisterDependency(i)->getRegister()->getAssignedRegister() == r &&
          _preConditions->getRegisterDependency(i)->getDefsRegister())
         {
         return true;
         }
      }
   for (uint16_t j = 0; j < _addCursorForPost; j++)
      {
      if (_postConditions->getRegisterDependency(j)->getRegister()->getAssignedRegister() == r &&
          _postConditions->getRegisterDependency(j)->getDefsRegister())
         {
         return true;
         }
      }
   return false;
   }

bool OMR::ARM64::RegisterDependencyConditions::usesRegister(TR::Register *r)
   {
   for (uint16_t i = 0; i < _addCursorForPre; i++)
      {
      if (_preConditions->getRegisterDependency(i)->getRegister() == r &&
          _preConditions->getRegisterDependency(i)->getUsesRegister())
         {
         return true;
         }
      }
   for (uint16_t j = 0; j < _addCursorForPost; j++)
      {
      if (_postConditions->getRegisterDependency(j)->getRegister() == r &&
          _postConditions->getRegisterDependency(j)->getUsesRegister())
         {
         return true;
         }
      }
   return false;
   }

TR::RegisterDependencyConditions *
OMR::ARM64::RegisterDependencyConditions::clone(
   TR::CodeGenerator *cg,
   TR::RegisterDependencyConditions *added)
   {
   TR_ASSERT(false, "Not implemented yet.");

   return NULL;
   }

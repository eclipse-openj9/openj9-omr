/*******************************************************************************
 * Copyright (c) 2000, 2016 IBM Corp. and others
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

#ifndef OMR_REGISTER_DEPENDENCY_INCL
#define OMR_REGISTER_DEPENDENCY_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_CONNECTOR
   namespace OMR { class RegisterDependencyConditions; }
   namespace OMR { typedef OMR::RegisterDependencyConditions RegisterDependencyConditionsConnector; }
#endif

#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "codegen/RegisterDependencyStruct.hpp"

namespace OMR
{

class RegisterDependencyConditions
   {
   protected:
   RegisterDependencyConditions() {};

   public:
   TR_ALLOC(TR_Memory::RegisterDependencyConditions)

   };

/** \brief
 *     Maps real register numbers to dependencies / virtuals to help speed up register dependency queries.
 *
 *  \note
 *     This class does not support heap allocation and is currently meant to be used only as a stack allocated object.
 */
class RegisterDependencyMap
   {
   public:

   RegisterDependencyMap(TR::RegisterDependency* deps, uint32_t numDeps)
      : deps(deps)
      {
      TR_ASSERT(numDeps <= SENTINEL, "Number of dependencies supplied (%d) cannot exceed %d!", numDeps, SENTINEL);

      for (auto i = 0; i < TR::RealRegister::NumRegisters; ++i)
         {
         targetTable[i] = SENTINEL;
         assignedTable[i] = SENTINEL;
         }
      }

   void addDependency(TR::RegisterDependency& dep, uint32_t index)
      {
      TR_ASSERT(&deps[index] == &dep, "Dep pointer/index mismatch");
      addDependency(dep.getRealRegister(), dep.getRegister()->getAssignedRealRegister(), index);
      }

   void addDependency(TR::RegisterDependency* dep, uint32_t index)
      {
      TR_ASSERT(&deps[index] == dep, "Dep pointer/index mismatch");
      addDependency(dep->getRealRegister(), dep->getRegister()->getAssignedRealRegister(), index);
      }
   
   TR::RegisterDependency* getDependencyWithAssigned(TR::RealRegister::RegNum rr)
      {
      TR_ASSERT(rr >= 0 && rr < TR::RealRegister::NumRegisters, "Register number used as index but out of range");
      return assignedTable[rr] != SENTINEL ? &deps[assignedTable[rr]] : NULL;
      }

   TR::RegisterDependency* getDependencyWithTarget(TR::RealRegister::RegNum rr)
      {
      TR_ASSERT(rr >= 0 && rr < TR::RealRegister::NumRegisters, "Register number used as index but out of range");
      TR_ASSERT(rr != TR::RealRegister::NoReg, "Multiple dependencies can map to 'NoReg', can't return just one");
      TR_ASSERT(rr != TR::RealRegister::SpilledReg, "Multiple dependencies can map to 'SpilledReg', can't return just one");
      return targetTable[rr] != SENTINEL ? &deps[targetTable[rr]] : NULL;
      }

   TR::Register* getVirtualWithAssigned(TR::RealRegister::RegNum rr)
      {
      TR::RegisterDependency *d = getDependencyWithAssigned(rr);
      return d ? d->getRegister() : NULL;
      }

   TR::Register* getVirtualWithTarget(TR::RealRegister::RegNum rr)
      {
      TR::RegisterDependency *d = getDependencyWithTarget(rr);
      return d ? d->getRegister() : NULL;
      }

   uint8_t getTargetIndex(TR::RealRegister::RegNum rr)
      {
      TR_ASSERT(targetTable[rr] != SENTINEL, "No such target register in dependency condition");
      return targetTable[rr];
      }

   private:

   void addDependency(TR::RealRegister::RegNum rr, TR::RealRegister *assignedReg, uint32_t index)
      {
      if (rr != TR::RealRegister::NoReg && rr != TR::RealRegister::SpilledReg)
         {
         TR_ASSERT(rr >= 0 && rr < TR::RealRegister::NumRegisters, "Register number %d used as index but out of range", rr);
         TR_ASSERT(targetTable[rr] == SENTINEL || deps[targetTable[rr]].getRegister() == deps[index].getRegister(),
            "Multiple virtual registers depend on a single real register %d", rr);
         targetTable[rr] = index;
         }

      if (assignedReg)
         {
         TR::RealRegister::RegNum arr = toRealRegister(assignedReg)->getRegisterNumber();
         TR_ASSERT(arr >= 0 && arr < TR::RealRegister::NumRegisters, "Register number %d used as index but out of range", arr);
         TR_ASSERT(assignedTable[arr] == SENTINEL || deps[assignedTable[arr]].getRegister() == deps[index].getRegister(),
            "Multiple virtual registers assigned to a single real register %d", arr);
         assignedTable[arr] = index;
         }
      }

   private:

   static const uint8_t SENTINEL = 255;

   TR::RegisterDependency *deps;
   uint8_t targetTable[TR::RealRegister::NumRegisters];
   uint8_t assignedTable[TR::RealRegister::NumRegisters];
   };
}

#endif

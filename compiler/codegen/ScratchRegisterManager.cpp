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

#include "codegen/ScratchRegisterManager.hpp"

#include <stddef.h>                        // for NULL
#include <stdint.h>                        // for int32_t
#include "codegen/CodeGenerator.hpp"       // for CodeGenerator
#include "codegen/Register.hpp"            // for Register
#include "codegen/RegisterConstants.hpp"   // for TR_RegisterKinds, etc
#include "codegen/RegisterDependency.hpp"
#include "compile/Compilation.hpp"         // for Compilation
#include "env/TRMemory.hpp"
#include "infra/Assert.hpp"                // for TR_ASSERT
#include "infra/List.hpp"                  // for ListIterator, List

TR_ScratchRegisterManager::TR_ScratchRegisterManager(int32_t capacity, TR::CodeGenerator *cg) :
   _capacity(capacity),
   _cg(cg),
   _msrList(cg->comp()->trMemory()),
   _cursor(0)
   {}

bool TR_ScratchRegisterManager::donateScratchRegister(TR::Register *reg)
   {
   if (_cursor >= _capacity)
      return false;

   TR_ManagedScratchRegister *msr = new (_cg->trHeapMemory()) TR_ManagedScratchRegister(reg, msrDonated);
   _msrList.add(msr);
   _cursor++;
   return true;
   }

bool TR_ScratchRegisterManager::reclaimScratchRegister(TR::Register *reg)
   {

   if (reg == NULL)
      {
      return false;
      }

   ListIterator<TR_ManagedScratchRegister> iterator(&_msrList);
   TR_ManagedScratchRegister *msr = iterator.getFirst();

   while (msr)
      {
      if (msr->_reg == reg)
         {
         msr->_state &= ~msrAllocated;
         return true;
         }

      msr = iterator.getNext();
      }

   return false;
   }


TR::Register *TR_ScratchRegisterManager::findOrCreateScratchRegister(TR_RegisterKinds rk)
   {
   TR_ASSERT(rk != TR_VRF,"VRF: RA (findOrCreateScratchRegister) unimplemented");
   // Check for free registers in the list.  If there are none then create
   // one if there is enough room.
   //
   ListIterator<TR_ManagedScratchRegister> iterator(&_msrList);
   TR_ManagedScratchRegister *msr = iterator.getFirst();

   while (msr)
      {
      if (msr->_reg->getKind() == rk && !(msr->_state & msrAllocated))
         {
         msr->_state |= msrAllocated;
         return msr->_reg;
         }

      msr = iterator.getNext();
      }

   if (_cursor >= _capacity)
      {
      traceMsg(_cg->comp(), "ERROR: cannot allocate any more scratch registers\n");
      return NULL;
      }

   TR::Register *reg = _cg->allocateRegister(rk);
   msr = new (_cg->trHeapMemory()) TR_ManagedScratchRegister(reg, msrAllocated);
   _msrList.add(msr);
   _cursor++;
   return reg;
   }


void TR_ScratchRegisterManager::addScratchRegistersToDependencyList(
   TR::RegisterDependencyConditions *deps)
   {
   ListIterator<TR_ManagedScratchRegister> iterator(&_msrList);
   TR_ManagedScratchRegister *msr = iterator.getFirst();

   while (msr)
      {
      deps->unionNoRegPostCondition(msr->_reg, _cg);
      msr = iterator.getNext();
      }
   }

// Terminate the live range of non-donated registers.
//
void TR_ScratchRegisterManager::stopUsingRegisters()
   {
   ListIterator<TR_ManagedScratchRegister> iterator(&_msrList);
   TR_ManagedScratchRegister *msr = iterator.getFirst();

   while (msr)
      {
      if (!(msr->_state & msrDonated))
         {
         _cg->stopUsingRegister(msr->_reg);
         }

      msr = iterator.getNext();
      }
   }

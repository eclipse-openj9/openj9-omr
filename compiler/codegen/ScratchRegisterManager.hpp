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

#ifndef SCRATCHREGISTERMANAGER_INCL
#define SCRATCHREGISTERMANAGER_INCL

#include <stdint.h>                       // for int32_t
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds, etc
#include "env/TRMemory.hpp"               // for TR_Memory, etc
#include "infra/List.hpp"                 // for List

namespace TR { class CodeGenerator; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }

enum TR_ManagedScratchRegisterStates
   {
   msrUnassigned = 0x00,    // no register associated
   msrAllocated  = 0x01,    // register has been allocated
   msrDonated    = 0x02     // register was donated to the list (not allocated)
   };

class TR_ManagedScratchRegister
   {
public:

   TR_ALLOC(TR_Memory::ScratchRegisterManager)

   TR::Register *_reg;
   int32_t      _state;

   TR_ManagedScratchRegister(TR::Register *preg, TR_ManagedScratchRegisterStates pstate)
      {
      _reg = preg;
      _state = pstate;
      }
   };

class TR_ScratchRegisterManager
   {

protected:

   TR::CodeGenerator *_cg;

   // No more than '_capacity' scratch registers will be allowed to be created.
   //
   int32_t _capacity;

   // Number of scratch registers available so far.
   //
   int32_t _cursor;

   List<TR_ManagedScratchRegister> _msrList;

public:

   TR_ALLOC(TR_Memory::ScratchRegisterManager)

   TR_ScratchRegisterManager(int32_t capacity, TR::CodeGenerator *cg);

   TR::Register *findOrCreateScratchRegister(TR_RegisterKinds rk = TR_GPR);
   bool donateScratchRegister(TR::Register *reg);
   bool reclaimScratchRegister(TR::Register *reg);

   int32_t getCapacity()          { return _capacity; }
   void    setCapacity(int32_t c) { _capacity = c; }

   int32_t numAvailableRegisters() { return _cursor; }

   void addScratchRegistersToDependencyList(TR::RegisterDependencyConditions *deps);
   void stopUsingRegisters();

   List<TR_ManagedScratchRegister>& getManagedScratchRegisterList() { return _msrList; }

   };

#endif

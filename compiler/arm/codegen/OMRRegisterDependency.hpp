/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#ifndef OMR_ARM_REGISTER_DEPENDENCY_INCL
#define OMR_ARM_REGISTER_DEPENDENCY_INCL


/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_REGISTER_DEPENDENCY_CONNECTOR
#define OMR_REGISTER_DEPENDENCY_CONNECTOR
   namespace OMR { namespace ARM { class RegisterDependencyConditions; } }
   namespace OMR { typedef OMR::ARM::RegisterDependencyConditions RegisterDependencyConditionsConnector; }
#else
   #error OMR::ARM::RegisterDependencyConditions expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRRegisterDependency.hpp"

#include "codegen/CodeGenerator.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/RegisterDependencyStruct.hpp"
#include "env/IO.hpp"

namespace TR { class Register; }

#define NUM_DEFAULT_DEPENDENCIES 1

class TR_ARMRegisterDependencyGroup
   {
   TR::RegisterDependency _dependencies[NUM_DEFAULT_DEPENDENCIES];

   // Use TR_ARMRegisterDependencyGroup::create to allocate an object of this type
   //
   void * operator new(size_t s, int32_t numDependencies, TR_Memory * m)
      {
      TR_ASSERT(numDependencies > 0, "operator new called with numDependencies == 0");
      if (numDependencies > NUM_DEFAULT_DEPENDENCIES)
         {
         s += (numDependencies-NUM_DEFAULT_DEPENDENCIES)*sizeof(TR::RegisterDependency);
         }
      return m->allocateHeapMemory(s);
      }

   public:

   TR_ALLOC_WITHOUT_NEW(TR_Memory::ARMRegisterDependencyGroup)

   TR_ARMRegisterDependencyGroup() {}

   static TR_ARMRegisterDependencyGroup *create(int32_t numDependencies, TR_Memory * m)
      {
      return numDependencies ? new (numDependencies, m) TR_ARMRegisterDependencyGroup : 0;
      }

   TR::RegisterDependency *getRegisterDependency(uint32_t index)
      {
      return &_dependencies[index];
      }

   void setDependencyInfo(uint32_t                                index,
                          TR::Register                            *vr,
                          TR::RealRegister::RegNum rr,
                          uint8_t                                 flag)
      {
      _dependencies[index].setRegister(vr);
      _dependencies[index].assignFlags(flag);
      _dependencies[index].setRealRegister(rr);
      }

   TR::Register *searchForRegister(TR::RealRegister::RegNum rr, uint32_t numberOfRegisters)
      {
      for (uint32_t i=0; i<numberOfRegisters; i++)
	 {
         if (_dependencies[i].getRealRegister() == rr)
            return(_dependencies[i].getRegister());
	 }
      return(NULL);
      }

   void assignRegisters(TR::Instruction  *currentInstruction,
                        TR_RegisterKinds kindToBeAssigned,
                        uint32_t         numberOfRegisters,
                        TR::CodeGenerator *cg);

   void blockRegisters(uint32_t numberOfRegisters)
      {
      for (uint32_t i = 0; i < numberOfRegisters; i++)
         {
         _dependencies[i].getRegister()->block();
         }
      }

   void unblockRegisters(uint32_t numberOfRegisters)
      {
      for (uint32_t i = 0; i < numberOfRegisters; i++)
         {
         _dependencies[i].getRegister()->unblock();
         }
      }
   };

namespace OMR
{
namespace ARM
{
class RegisterDependencyConditions: public OMR::RegisterDependencyConditions
   {
   TR_ARMRegisterDependencyGroup *_preConditions;
   TR_ARMRegisterDependencyGroup *_postConditions;
   uint8_t                        _numPreConditions;
   uint8_t                        _addCursorForPre;
   uint8_t                        _numPostConditions;
   uint8_t                        _addCursorForPost;

   public:
   TR_ALLOC(TR_Memory::ARMRegisterDependencyConditions)

   RegisterDependencyConditions()
      : _preConditions(NULL),
        _postConditions(NULL),
        _numPreConditions(0),
        _addCursorForPre(0),
        _numPostConditions(0),
        _addCursorForPost(0)
      {}

   RegisterDependencyConditions(uint8_t numPreConds, uint8_t numPostConds, TR_Memory * m)
      : _preConditions(TR_ARMRegisterDependencyGroup::create(numPreConds, m)),
        _postConditions(TR_ARMRegisterDependencyGroup::create(numPostConds, m)),
        _numPreConditions(numPreConds),
        _addCursorForPre(0),
        _numPostConditions(numPostConds),
        _addCursorForPost(0)
      {}

   RegisterDependencyConditions(TR::Node           *node,
                                      uint32_t           extranum,
                                      TR::Instruction   **cursorPtr,
                                      TR::CodeGenerator  *cg);

   TR::RegisterDependencyConditions *clone(TR::CodeGenerator *, TR::RegisterDependencyConditions *added=NULL);
   TR::RegisterDependencyConditions *cloneAndFix(TR::CodeGenerator *, TR::RegisterDependencyConditions *added=NULL);

   void unionNoRegPostCondition(TR::Register *reg, TR::CodeGenerator *cg); /* @@@@ */

   TR_ARMRegisterDependencyGroup *getPreConditions()  {return _preConditions;}

   uint32_t getNumPreConditions() {return _numPreConditions;}

   uint32_t setNumPreConditions(uint8_t n, TR_Memory * m)
      {
      if (_preConditions == NULL)
         _preConditions = TR_ARMRegisterDependencyGroup::create(n, m);

      if (_addCursorForPre > n)
         _addCursorForPre = n;

      return (_numPreConditions = n);
      }

   uint32_t getNumPostConditions() {return _numPostConditions;}

   uint32_t setNumPostConditions(uint8_t n, TR_Memory * m)
      {
      if (_postConditions == NULL)
         _postConditions = TR_ARMRegisterDependencyGroup::create(n, m);

      if (_addCursorForPost > n)
         _addCursorForPost = n;

      return (_numPostConditions = n);
      }

   uint32_t getAddCursorForPre() {return _addCursorForPre;}
   uint32_t setAddCursorForPre(uint8_t a) {return (_addCursorForPre = a);}

   uint32_t getAddCursorForPost() {return _addCursorForPost;}
   uint32_t setAddCursorForPost(uint8_t a) {return (_addCursorForPost = a);}

   void addPreCondition(TR::Register                            *vr,
                        TR::RealRegister::RegNum rr,
                        uint8_t                                 flag = UsesDependentRegister)
      {
      _preConditions->setDependencyInfo(_addCursorForPre++, vr, rr, flag);
      }

   TR_ARMRegisterDependencyGroup *getPostConditions() {return _postConditions;}

   void addPostCondition(TR::Register                            *vr,
                         TR::RealRegister::RegNum rr,
                         uint8_t                                 flag = UsesDependentRegister)
      {
      _postConditions->setDependencyInfo(_addCursorForPost++, vr, rr, flag);
      }

   void assignPreConditionRegisters(TR::Instruction *currentInstruction, TR_RegisterKinds kindToBeAssigned, TR::CodeGenerator *cg)
      {
      if (_preConditions != NULL)
         {
         _preConditions->assignRegisters(currentInstruction, kindToBeAssigned, _addCursorForPre, cg);
         }
      }

   void assignPostConditionRegisters(TR::Instruction *currentInstruction, TR_RegisterKinds kindToBeAssigned, TR::CodeGenerator *cg)
      {
      if (_postConditions != NULL)
         {
         _postConditions->assignRegisters(currentInstruction, kindToBeAssigned, _addCursorForPost, cg);
         }
      }

   TR::Register *searchPreConditionRegister(TR::RealRegister::RegNum rr)
      {
      return(_preConditions==NULL?NULL:_preConditions->searchForRegister(rr, _addCursorForPre));
      }

   TR::Register *searchPostConditionRegister(TR::RealRegister::RegNum rr)
      {
      return(_postConditions==NULL?NULL:_postConditions->searchForRegister(rr, _addCursorForPost));
      }

   void stopAddingPreConditions()
      {
      _numPreConditions = _addCursorForPre;
      }

   void stopAddingPostConditions()
      {
      _numPostConditions = _addCursorForPost;
      }

   void stopAddingConditions()
      {
      stopAddingPreConditions();
      stopAddingPostConditions();
      }

   bool refsRegister(TR::Register *r);
   bool defsRegister(TR::Register *r);
   bool defsRealRegister(TR::Register *r);
   bool usesRegister(TR::Register *r);

   void incRegisterTotalUseCounts(TR::CodeGenerator *);
   };
}
}

#endif

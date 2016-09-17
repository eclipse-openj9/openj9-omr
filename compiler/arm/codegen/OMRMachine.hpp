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

#ifndef OMR_ARM_MACHINE_INCL
#define OMR_ARM_MACHINE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_MACHINEBASE_CONNECTOR
#define OMR_MACHINEBASE_CONNECTOR

namespace OMR { namespace ARM { class Machine; } }
namespace OMR { typedef OMR::ARM::Machine MachineConnector; }

#else
#error OMR::ARM::Machine expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/codegen/OMRMachine.hpp"

#include "codegen/RealRegister.hpp"
#include "infra/TRlist.hpp"

namespace TR { class CodeGenerator; }
namespace TR { class Register; }
namespace TR { class RegisterDependencyConditions; }

#define NUM_ARM_GPR  16
#define NUM_ARM_MAXR 16
#define MAX_ARM_GLOBAL_GPRS       10
#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
#define NUM_ARM_FPR  16
#define MAX_ARM_GLOBAL_FPRS       16
#else
#define NUM_ARM_FPR  8
#define MAX_ARM_GLOBAL_FPRS       8
#endif
#define UPPER_IMMED12  ((1 << 12) - 1)
#define LOWER_IMMED12  (-(1 << 12) + 1)

namespace OMR
{

namespace ARM
{

class OMR_EXTENSIBLE Machine : public OMR::Machine
   {
   TR::RealRegister   *_registerFile[TR::RealRegister::NumRegisters];
   TR::CodeGenerator *_cg;

   static uint32_t       _globalRegisterNumberToRealRegisterMap[MAX_ARM_GLOBAL_GPRS + MAX_ARM_GLOBAL_FPRS];

   void initialiseRegisterFile();

   // For register snap shot
   uint16_t                    _registerFlagsSnapShot[TR::RealRegister::NumRegisters];
   TR::RealRegister::RegState  _registerStatesSnapShot[TR::RealRegister::NumRegisters];
   TR::Register                *_registerAssociationsSnapShot[TR::RealRegister::NumRegisters];
   TR::Register                *_assignedRegisterSnapShot[TR::RealRegister::NumRegisters];


   public:

   Machine(TR::CodeGenerator *cg);

   TR::RealRegister *getARMRealRegister(TR::RealRegister::RegNum regNum)
      {
      return _registerFile[regNum];
      }

   TR::RealRegister *findBestFreeRegister(TR_RegisterKinds rk,
                                            bool excludeGPR0 = false,
                                            bool considerUnlatched = false,
                                            bool isSinglePrecision = false);

   TR::RealRegister *freeBestRegister(TR::Instruction  *currentInstruction,
                                        TR_RegisterKinds rk,
                                        TR::RealRegister *forced = NULL,
                                        bool excludeGPR0 = false,
                                        bool isSinglePrecision = false);

   TR::RealRegister *reverseSpillState(TR::Instruction     *currentInstruction,
                                         TR::Register        *spilledRegister,
                                         TR::RealRegister *targetRegister = NULL,
                                         bool excludeGPR0 = false,
                                         bool isSinglePrecision = false);

   void coerceRegisterAssignment(TR::Instruction                           *currentInstruction,
                                 TR::Register                              *virtualRegister,
                                 TR::RealRegister::RegNum registerNumber);

   bool getLinkRegisterKilled()
      {
      return _registerFile[TR::RealRegister::gr14]->getHasBeenAssignedInMethod();
      }

   bool setLinkRegisterKilled(bool b)
      {
      return _registerFile[TR::RealRegister::gr14]->setHasBeenAssignedInMethod(b);
      }

   // Snap shot methods
   void takeRegisterStateSnapShot();
   void restoreRegisterStateFromSnapShot();
   TR::RegisterDependencyConditions  *createCondForLiveAndSpilledGPRs(bool cleanRegState, TR::list<TR::Register*> *spilledRegisterList = NULL);

   TR::RealRegister *assignSingleRegister(TR::Register *virtualRegister, TR::Instruction *currentInstruction);

   TR::CodeGenerator *cg()           {return _cg;}

   // TODO:Are these two still used? Are values correct?  What are they?
   static uint8_t getGlobalGPRPartitionLimit() {return 1;}
   static uint8_t getGlobalFPRPartitionLimit() {return 1;}

   static uint32_t *getGlobalRegisterTable()
      {
      return _globalRegisterNumberToRealRegisterMap;
      }

   static TR_GlobalRegisterNumber getLastGlobalGPRRegisterNumber()
         {return MAX_ARM_GLOBAL_GPRS - 1;}

   static TR_GlobalRegisterNumber getLast8BitGlobalGPRRegisterNumber()
         {return MAX_ARM_GLOBAL_GPRS - 1;}

   static TR_GlobalRegisterNumber getLastGlobalFPRRegisterNumber()
         {return MAX_ARM_GLOBAL_GPRS + MAX_ARM_GLOBAL_FPRS - 1;}
   };

}
}
#endif

/*******************************************************************************
 * Copyright (c) 2019, 2019 IBM Corp. and others
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

// On zOS XLC linker can't handle files with same name at link time.
// This workaround with pragma is needed. What this does is essentially
// give a different name to the codesection (csect) for this file. So it
// doesn't conflict with another file with the same name.
#pragma csect(CODE,"OMRZCPUBase#C")
#pragma csect(STATIC,"OMRZCPUBase#S")
#pragma csect(TEST,"OMRZCPUBase#T")

#include "env/CPU.hpp"

const char*
OMR::Z::CPU::getProcessorName(int32_t machineId)
   {
   const char* result = "";

   switch (machineId)
      {
      case 1090:
      case 1091:
         result = "zPDT";
      break;

      case 2066:
         result = "z800";
      break;

      case 2084:
         result = "z990";
      break;

      case 2086:
         result = "z890";
      break;

      case 2064:
         result = "z900";
      break;

      case 2094:
      case 2096:
         result = "z9";
      break;

      case 2097:
      case 2098:
         result = "z10";
      break;

      case 2817:
         result = "z196";
      break;
      
      case 2818:
         result = "z114";
      break;

      case 2827:
      case 2828:
         result = "zEC12";
      break;

      case 2964:
      case 2965:
         result = "z13";
      break;

      case 3906:
      case 3907:
         result = "z14";
      break;

      case 8561:
      case 8562:
         result = "z15";
      break;

      case 9998:
      case 9999:
         result = "zNext";
      break;

      default:
         result = "Unknown";
      break;
      }

   return result;
   }

OMR::Z::CPU::CPU()
   :
   OMR::CPU(),
   _supportedArch(z9)
   {}

bool
OMR::Z::CPU::getSupportsArch(Architecture arch)
   {
   return _supportedArch >= arch;
   }

bool
OMR::Z::CPU::setSupportsArch(Architecture arch)
   {
   return _supportedArch = _supportedArch >= arch ? _supportedArch : arch;
   }

bool
OMR::Z::CPU::getSupportsHardwareSQRT()
   {
   return true;
   }

bool
OMR::Z::CPU::hasPopulationCountInstruction()
   {
   return getSupportsMiscellaneousInstructionExtensions3Facility();
   }

bool
OMR::Z::CPU::getSupportsHighWordFacility()
   {
   return _flags.testAny(S390SupportsHPR);
   }

bool
OMR::Z::CPU::setSupportsHighWordFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsHPR);
      }
   else
      {
      _flags.reset(S390SupportsHPR);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsDecimalFloatingPointFacility()
   {
   return _flags.testAny(S390SupportsDFP);
   }

bool
OMR::Z::CPU::setSupportsDecimalFloatingPointFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsDFP);
      }
   else
      {
      _flags.reset(S390SupportsDFP);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsFloatingPointExtensionFacility()
   {
   return _flags.testAny(S390SupportsFPE);
   }

bool
OMR::Z::CPU::setSupportsFloatingPointExtensionFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsFPE);
      }
   else
      {
      _flags.reset(S390SupportsFPE);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsTransactionalMemoryFacility()
   {
   return _flags.testAny(S390SupportsTM);
   }

bool
OMR::Z::CPU::supportsTransactionalMemoryInstructions()
   {
   return self()->getSupportsTransactionalMemoryFacility();
   }


bool
OMR::Z::CPU::setSupportsTransactionalMemoryFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsTM);
      }
   else
      {
      _flags.reset(S390SupportsTM);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsRuntimeInstrumentationFacility()
   {
   return _flags.testAny(S390SupportsRI);
   }

bool
OMR::Z::CPU::setSupportsRuntimeInstrumentationFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsRI);
      }
   else
      {
      _flags.reset(S390SupportsRI);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsVectorFacility()
   {
   return _flags.testAny(S390SupportsVectorFacility);
   }

bool
OMR::Z::CPU::setSupportsVectorFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsVectorFacility);
      }
   else
      {
      _flags.reset(S390SupportsVectorFacility);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsVectorPackedDecimalFacility()
   {
   return _flags.testAny(S390SupportsVectorPackedDecimalFacility);
   }

bool
OMR::Z::CPU::setSupportsVectorPackedDecimalFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsVectorPackedDecimalFacility);
      }
   else
      {
      _flags.reset(S390SupportsVectorPackedDecimalFacility);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsGuardedStorageFacility()
   {
   return _flags.testAny(S390SupportsGuardedStorageFacility);
   }

bool
OMR::Z::CPU::setSupportsGuardedStorageFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsGuardedStorageFacility);
      }
   else
      {
      _flags.reset(S390SupportsGuardedStorageFacility);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsMiscellaneousInstructionExtensions3Facility()
   {
   return _flags.testAny(S390SupportsMIE3);
   }

bool
OMR::Z::CPU::setSupportsMiscellaneousInstructionExtensions3Facility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsMIE3);
      }
   else
      {
      _flags.reset(S390SupportsMIE3);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsVectorFacilityEnhancement2()
   {
   return _flags.testAny(S390SupportsVectorFacilityEnhancement2);
   }

bool
OMR::Z::CPU::setSupportsVectorFacilityEnhancement2(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsVectorFacilityEnhancement2);
      }
   else
      {
      _flags.reset(S390SupportsVectorFacilityEnhancement2);
      }

   return value;
   }

bool
OMR::Z::CPU::getSupportsVectorPackedDecimalEnhancementFacility()
   {
   return _flags.testAny(S390SupportsVectorPDEnhancementFacility);
   }

bool
OMR::Z::CPU::setSupportsVectorPackedDecimalEnhancementFacility(bool value)
   {
   if (value)
      {
      _flags.set(S390SupportsVectorPDEnhancementFacility);
      }
   else
      {
      _flags.reset(S390SupportsVectorPDEnhancementFacility);
      }

   return value;
   }

bool
OMR::Z::CPU::isTargetWithinBranchRelativeRILRange(intptrj_t targetAddress, intptrj_t sourceAddress)
   {
   return (targetAddress == sourceAddress + ((intptrj_t)((int32_t)((targetAddress - sourceAddress) / 2))) * 2) &&
            (targetAddress % 2 == 0);
   }

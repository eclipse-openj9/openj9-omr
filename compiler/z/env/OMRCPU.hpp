/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
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

#ifndef OMR_Z_CPU_INCL
#define OMR_Z_CPU_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CPU_CONNECTOR
#define OMR_CPU_CONNECTOR
namespace OMR { namespace Z { class CPU; } }
namespace OMR { typedef OMR::Z::CPU CPUConnector; }
#else
#error OMR::Z::CPU expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/env/OMRCPU.hpp"
#include "env/jittypes.h"
#include "env/ProcessorInfo.hpp"

namespace OMR
{

namespace Z
{

class CPU : public OMR::CPU
   {
   public:

   enum Architecture
      {
      TR_UNDEFINED_S390_MACHINE  =  0,
      TR_G5                      =  9672,
      TR_MULTIPRISE7000          =  7060,
      TR_FREEWAY                 =  2064,  // z900
      TR_Z800                    =  2066,  // z800 - entry-level, less powerful variant of the z900
      TR_MIRAGE                  =  1090,
      TR_MIRAGE2                 =  1091,
      TR_TREX                    =  2084,  // z990
      TR_Z890                    =  2086,  // z890 - kneecapped version of z990
      TR_GOLDEN_EAGLE            =  2094,  // z9
      TR_DANU_GA2                =  2094,  // doesn't change from GoldenEagle
      TR_Z9BC                    =  2096,  // z9 BC
      TR_Z10                     =  2097,
      TR_Z10BC                   =  2098,  // zMR
      TR_ZG                      =  2817,  // zGryphon HE - z196
      TR_ZGMR                    =  2818,  // zGryphon MR - z114
      TR_ZG_RESERVE              =  2819,  // reserve for zGryphon
      TR_ZEC12                   =  2827,  // zEC12 / zHelix
      TR_ZEC12MR                 =  2828,  // zHelix MR
      TR_ZEC12_RESERVE           =  2829,  // reserve for zHelix
      TR_Z13                     =  2964,  // z13
      TR_Z13s                    =  2965,  // z13s
      TR_Z14                     =  3906,
      TR_Z14s                    =  3907,
      TR_Z15                     =  8561,
      TR_Z15s                    =  8562,
      TR_ZNEXT                   =  9998,
      TR_ZNEXTs                  =  9999,
      TR_ZH                      =  2458,  // reserve for zHybrid
      TR_DATAPOWER               =  2462,  // reserve for DataPower on 2458
      TR_ZH_RESERVE1             =  2459,  // reserve for zHybrid
      TR_ZH_RESERVE2             =  2461,  // reserve for zHybrid

      
      S390SupportsUnknownArch = 0,
      S390SupportsZ900        = 1,
      S390SupportsZ990        = 2,
      S390SupportsZ9          = 3,
      S390SupportsZ10         = 4,
      S390SupportsZ196        = 5,
      S390SupportsZEC12       = 6,
      S390SupportsZ13         = 7,
      S390SupportsZ14         = 8,
      S390SupportsZ15         = 9,
      S390SupportsZNext       = 10,
      S390SupportsLatestArch  = S390SupportsZNext,

      TR_UnknownArchitecture = 0,
      TR_z900 = 1,
      TR_z990 = 2,
      TR_z9 = 3,
      TR_z10 = 4,
      TR_z196 = 5,
      TR_zEC12 = 6,
      TR_z13 = 7,
      TR_z14 = 8,
      TR_z15 = 9,
      TR_zNext = 10,

      TR_LatestArchitecture = TR_zNext
      };
protected:

   CPU() :
         OMR::CPU(),
      _supportedArch(TR_UNDEFINED_S390_MACHINE)
      {}

public:

   bool getS390SupportsArch(Architecture arch)
      {
      return _supportedArch >= arch;
      }

   bool setS390SupportsArch(Architecture arch)
      {
      return _supportedArch = _supportedArch >= arch ? _supportedArch : arch;
      }

   bool getSupportsHardwareSQRT() { return true; }

   bool getS390SupportsZ900() { return true; }

   bool getS390SupportsZ990() { return true; }

   bool getS390SupportsZ9() { return true; }

   bool getS390SupportsZ10() { return true; }

   bool getS390SupportsZ196() { return true; }

   bool getS390SupportsZEC12() { return false; }

   bool getS390SupportsZ13() { return false; }

   bool getS390SupportsZ14() { return false; }

   bool getS390SupportsZ15() { return false; }

   bool getS390SupportsZNext() { return false; }

   bool getS390SupportsHPR() { return false; }

   bool getS390SupportsDFP() { return false; }

   bool getS390SupportsFPE() {return false; }

   bool getS390SupportsTM() { return false; }

   bool getS390SupportsRI() { return false; }

   bool getS390SupportsVectorFacility() { return false; }

   bool getS390SupportsGuardedStorageFacility() { return false; }

   /**
    * @brief Answers whether the distance between a target and source address
    *        is within the reachable displacement range for a branch relative
    *        RIL-format instruction.
    *
    * @param[in] : targetAddress : the address of the target
    *
    * @param[in] : sourceAddress : the address of the branch relative RIL-format
    *                 instruction from which the displacement range is measured.
    *
    * @return true if the target is within range; false otherwise.
    */
   bool isTargetWithinBranchRelativeRILRange(intptrj_t targetAddress, intptrj_t sourceAddress)
      {
      return (targetAddress == sourceAddress + ((intptrj_t)((int32_t)((targetAddress - sourceAddress)/2)))*2) &&
             (targetAddress % 2 == 0);
      }

   private:

   Architecture _supportedArch;
   };

}

}

#endif

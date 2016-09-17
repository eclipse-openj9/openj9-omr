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

#ifndef OMR_POWER_CPU_INCL
#define OMR_POWER_CPU_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CPU_CONNECTOR
#define OMR_CPU_CONNECTOR
namespace OMR { namespace Power { class CPU; } }
namespace OMR { typedef OMR::Power::CPU CPUConnector; }
#else
#error OMR::Power::CPU expected to be a primary connector, but a OMR connector is already defined
#endif

#include "compiler/env/OMRCPU.hpp"

#include "infra/Assert.hpp"  // for TR_ASSERT

#define VALID_PROCESSOR TR_ASSERT(id() >= TR_FirstPPCProcessor && id() <= TR_LastPPCProcessor, "Not a valid PPC Processor Type")

namespace OMR
{

namespace Power
{

class CPU : public OMR::CPU
   {
protected:

   CPU() : OMR::CPU() {}

public:

   bool getSupportsHardwareSQRT() { VALID_PROCESSOR; return id() >= TR_FirstPPCHwSqrtProcessor; }
   bool getSupportsHardwareRound() { VALID_PROCESSOR; return id() >= TR_FirstPPCHwRoundProcessor; }
   bool getSupportsHardwareCopySign() { VALID_PROCESSOR; return id() >= TR_FirstPPCHwCopySignProcessor;}

   bool getPPCis64bit();
   bool getPPCSupportsVMX() { return false; }
   bool getPPCSupportsVSX() { return false; }
   bool getPPCSupportsAES() { return false; }
   bool getPPCSupportsTM()  { return false; }
   bool getPPCSupportsLM()  { return false; }
   };

}

}

#endif

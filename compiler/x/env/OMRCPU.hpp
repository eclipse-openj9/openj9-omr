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

#ifndef OMR_X86_CPU_INCL
#define OMR_X86_CPU_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_CPU_CONNECTOR
#define OMR_CPU_CONNECTOR
namespace OMR { namespace X86 { class CPU; } }
namespace OMR { typedef OMR::X86::CPU CPUConnector; }
#else
#error OMR::X86::CPU expected to be a primary connector, but a OMR connector is already defined
#endif

#include <stdint.h>
#include "compiler/env/OMRCPU.hpp"

struct TR_X86CPUIDBuffer;
namespace TR { class Compilation; }


namespace OMR
{

namespace X86
{

class CPU : public OMR::CPU
   {
protected:

   CPU() : OMR::CPU() {}

public:

   TR_X86CPUIDBuffer *queryX86TargetCPUID(TR::Compilation *comp);
   const char *getX86ProcessorVendorId(TR::Compilation *comp);
   uint32_t getX86ProcessorSignature(TR::Compilation *comp);
   uint32_t getX86ProcessorFeatureFlags(TR::Compilation *comp);
   uint32_t getX86ProcessorFeatureFlags2(TR::Compilation *comp);

   bool testOSForSSESupport(TR::Compilation *comp);
   bool getX86OSSupportsSSE(TR::Compilation *comp);
   bool getX86OSSupportsSSE2(TR::Compilation *comp);
   bool getX86SupportsTM(TR::Compilation *comp);

   };

}

}

#endif /* OMR_X86_CPU_BASE_INCL */

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
#include "env/ProcessorInfo.hpp"

namespace OMR
{

namespace Z
{

class CPU : public OMR::CPU
   {
protected:

   CPU() :
         OMR::CPU(),
      _s390MachineType(TR_UNDEFINED_S390_MACHINE)
      {}

public:
   bool getSupportsHardwareSQRT() { return true; }

   bool getS390SupportsZ900() { return true; }

   bool getS390SupportsZ990() { return true; }

   bool getS390SupportsZ9() { return true; }

   bool getS390SupportsZ10() { return true; }

   bool getS390SupportsZ196() { return true; }

   bool getS390SupportsZEC12() { return false; }

   bool getS390SupportsZ13() { return false; }

   bool getS390SupportsZNext() { return false; }

   bool getS390SupportsHPRDebug() { return false; }

   bool getS390SupportsDFP() { return false; }

   bool getS390SupportsFPE() {return false; }

   bool getS390SupportsTM() { return false; }

   bool getS390SupportsRI() { return false; }

   bool getS390SupportsVectorFacility() { return false; }
   
   bool getS390SupportsGuardedStorageFacility() { return false; }

   TR_S390MachineType getS390MachineType() const { return _s390MachineType; }
   void setS390MachineType(TR_S390MachineType t) { _s390MachineType = t; }

private:

   TR_S390MachineType _s390MachineType;
   };

}

}

#endif

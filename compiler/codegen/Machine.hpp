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

#ifndef OMR_MACHINE_INCL
#define OMR_MACHINE_INCL

#include "codegen/OMRMachine.hpp"  // for MachineBaseConnector

namespace TR { class CodeGenerator; }

namespace TR
{
class OMR_EXTENSIBLE Machine : public OMR::MachineConnector
   {
   public:

   Machine(TR::CodeGenerator *cg) :
      OMR::MachineConnector(cg) {};

   };
}

#endif

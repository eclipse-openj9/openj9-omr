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

#include "env/CPU.hpp"

#include "env/Processors.hpp"  // for TR_Processor, etc
#include "infra/Assert.hpp"    // for TR_ASSERT

bool
OMR::Power::CPU::getPPCis64bit()
   {
   TR_Processor p = id();
   TR_ASSERT(p >= TR_FirstPPCProcessor && p <= TR_LastPPCProcessor, "Not a valid PPC Processor Type");

   return (p >= TR_FirstPPC64BitProcessor)? true : false;
   }


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
 ******************************************************************************/

#ifndef TR_VMACCESSCRITICALSECTION_INCL
#define TR_VMACCESSCRITICALSECTION_INCL

#include "env/OMRVMAccessCriticalSection.hpp"
#include "infra/Annotations.hpp"

namespace TR { class Compilation; }

namespace TR
{

class OMR_EXTENSIBLE VMAccessCriticalSection : public OMR::VMAccessCriticalSectionConnector
   {
public:

   VMAccessCriticalSection(
         TR::Compilation *comp,
         VMAccessAcquireProtocol protocol = acquireVMAccessIfNeeded) :
      OMR::VMAccessCriticalSectionConnector(comp, protocol) {}

   };

}

#endif

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

#ifndef OMR_MEMREF_INCL
#define OMR_MEMREF_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_MEMREF_CONNECTOR
#define OMR_MEMREF_CONNECTOR
namespace OMR { class MemoryReference; }
namespace OMR { typedef OMR::MemoryReference MemoryReferenceConnector; }
#endif

#include "env/TRMemory.hpp"       // for TR_Memory, etc
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

namespace TR { class MemoryReference; }
class TR_ARMMemoryReference;

namespace OMR
{

class OMR_EXTENSIBLE MemoryReference
   {
   public:

   TR_ALLOC(TR_Memory::MemoryReference)

   TR::MemoryReference *self();

   virtual TR_ARMMemoryReference  *asARMMemoryReference()  { return 0; }

   protected:
   MemoryReference() {};
   };

}

#endif

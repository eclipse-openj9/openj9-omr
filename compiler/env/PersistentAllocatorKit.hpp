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

#ifndef PERSISTENTALLOCATORKIT_HPP
#define PERSISTENTALLOCATORKIT_HPP

#ifndef TR_PERSISTENT_ALLOCATOR_KIT
#define TR_PERSISTENT_ALLOCATOR_KIT

namespace OMR { class PersistentAllocatorKit; }
namespace TR { using OMR::PersistentAllocatorKit; }

#endif // TR_PERSISTENT_ALLOCATOR_KIT

#include "env/RawAllocator.hpp"


namespace OMR
{

struct PersistentAllocatorKit
   {
   PersistentAllocatorKit(TR::RawAllocator rawAllocator) :
      rawAllocator(rawAllocator)
      {
      }

   TR::RawAllocator rawAllocator;
   };

}

#endif // PERSISTENTALLOCATORKIT_HPP

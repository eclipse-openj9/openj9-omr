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

#include <algorithm>                    // for std::min
#include <stddef.h>                     // for size_t, NULL
#include <stdint.h>                     // for int32_t, uint32_t, uintptr_t, etc
#include <stdio.h>                      // for fflush, fprintf, stderr
#include <stdlib.h>                     // for free, malloc
#include <string.h>                     // for memset
#include "infra/CriticalSection.hpp"    // for CriticalSection
#include "control/Options.hpp"
#include "control/Options_inlines.hpp"  // for TR::Options, etc
#include "env/PersistentAllocator.hpp"  // for PersistentAllocator
#include "env/TRMemory.hpp"             // for TR_PersistentMemory, etc
#include "il/DataTypes.hpp"             // for pointer_cast
#include "infra/Assert.hpp"             // for TR_ASSERT
#include "infra/Monitor.hpp"            // for Monitor
#include "infra/MonitorTable.hpp"       // for MonitorTable

namespace TR { class Compilation; }
namespace TR { class PersistentInfo; }

extern TR::Monitor *memoryAllocMonitor;

namespace TR
   {
   namespace Internal
      {
      // See TRMemory.hpp for rationale.
      PersistentNewType persistent_new_object;
      }
   }


// **************************************************************************
//
// deprecated uses of the global trMemory (not thread safe)
//

TR_PersistentMemory * trPersistentMemory = NULL;

void * TR_MemoryBase::jitPersistentAlloc(size_t size, ObjectType ot) { return ::trPersistentMemory ? ::trPersistentMemory->allocatePersistentMemory(size, ot) : 0 ; }
void   TR_MemoryBase::jitPersistentFree(void * mem)                  { ::trPersistentMemory->freePersistentMemory(mem); }

TR::PersistentInfo * TR_PersistentMemory::getNonThreadSafePersistentInfo() { return ::trPersistentMemory->getPersistentInfo(); }

TR_PersistentMemory::TR_PersistentMemory(
   void *   jitConfig,
   TR::PersistentAllocator &persistentAllocator
   ) :
   TR_MemoryBase(jitConfig),
   _signature(MEMINFO_SIGNATURE),
   _persistentInfo(this),
   _persistentAllocator(TR::ref(persistentAllocator)),
   _totalPersistentAllocations()
   {
   }

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

#ifndef OMR_MONITORTABLE_INCL
#define OMR_MONITORTABLE_INCL

#ifndef OMR_MONITORTABLE_CONNECTOR
#define OMR_MONITORTABLE_CONNECTOR
namespace OMR { class MonitorTable; }
namespace OMR { typedef MonitorTable MonitorTableConnector; }
#endif

#include "infra/Annotations.hpp" // for OMR_EXTENSIBLE
#include "infra/Assert.hpp"      // for TR_ASSERT

namespace TR { class MonitorTable; }
namespace TR { class Monitor; }

namespace OMR
{
// singleton
class OMR_EXTENSIBLE MonitorTable
   {
   public:

   static TR::MonitorTable *get() { return _instance; }

   void free() { TR_ASSERT(false, "not implemented by project"); }
   void removeAndDestroy(TR::Monitor *monitor) { TR_ASSERT(false, "not implemented by project"); }

   TR::Monitor *getMemoryAllocMonitor() { return _memoryAllocMonitor; }
   TR::Monitor *getScratchMemoryPoolMonitor() { return _scratchMemoryPoolMonitor; }

   protected:

   TR::MonitorTable *self();

   static TR::MonitorTable *_instance;

   // Used by TR_PersistentMemory
   //
   TR::Monitor *_memoryAllocMonitor;

   // Used by SCRATCH segments allocations
   // A copy of this goes into TR_PersistentMemory as well
   //
   TR::Monitor *_scratchMemoryPoolMonitor;

   private:

   friend class TR::Monitor;
   friend class TR::MonitorTable;

   TR::Monitor *create(char *name) { TR_ASSERT(false, "not implemented by project"); return 0; }
   };

}

#endif

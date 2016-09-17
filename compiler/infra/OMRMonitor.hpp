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

#ifndef OMR_MONITOR_INCL
#define OMR_MONITOR_INCL

#ifndef OMR_MONITOR_CONNECTOR
#define OMR_MONITOR_CONNECTOR
namespace OMR { class Monitor; }
namespace OMR { typedef OMR::Monitor MonitorConnector; }
#endif

#include <stdint.h>          // for int32_t, int64_t
#include "infra/Assert.hpp"  // for TR_ASSERT

namespace TR { class Monitor; }

#define NOT_IMPL { TR_ASSERT(false, "not implemented by project"); return 0; }
#define NOT_IMPL_VOID { TR_ASSERT(false, "not implemented by project"); }

namespace OMR
{

class Monitor
   {
   public:

   static TR::Monitor *create(char *name) { TR_ASSERT(false, "not implemented by project"); return 0; }
   void enter() NOT_IMPL_VOID;
   int32_t try_enter() NOT_IMPL;
   int32_t exit() NOT_IMPL; // returns 0 on success
   void destroy() NOT_IMPL_VOID;
   void wait() NOT_IMPL_VOID;
   intptr_t wait_timed(int64_t millis, int32_t nanos) NOT_IMPL;
   void notify() NOT_IMPL_VOID;
   void notifyAll() NOT_IMPL_VOID;
   int32_t num_waiting() NOT_IMPL;
   };

}

#undef NOT_IMPL
#undef NOT_IMPL_VOID

#endif

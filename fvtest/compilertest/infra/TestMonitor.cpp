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

#include "infra/TestMonitor.hpp"

#include <pthread.h>
#include "env/TRMemory.hpp"
#include "infra/Assert.hpp"
#include "infra/Monitor.hpp"
#include "infra/MonitorTable.hpp"

// We don't currently use a monitor table, but we still have to declare these statics
//
TR::MonitorTable *OMR::MonitorTable::_instance = 0;

TR::Monitor *memoryAllocMonitor;

void *
Test::Monitor::operator new(size_t size)
   {
   return TR::globalAllocator().allocate(size);
   }

void
Test::Monitor::operator delete(void *p)
   {
   TR::globalAllocator().deallocate(p, sizeof(TR::Monitor));
   }

TR::Monitor *
Test::Monitor::create(char *name)
   {
   return new TR::Monitor(name);
   }

Test::Monitor::Monitor(char const *name) :
      _name(name)
   {
   int32_t rc = pthread_mutex_init(&_monitor, 0);
   TR_ASSERT(rc == 0, "error initializing monitor\n");
   }

Test::Monitor::~Monitor()
   {
   int32_t rc = pthread_mutex_destroy(&_monitor);
   TR_ASSERT(rc == 0, "error destroying monitor\n");
   }

void
Test::Monitor::enter()
   {
   int32_t rc = pthread_mutex_lock(&_monitor);
   TR_ASSERT(rc == 0, "error locking monitor\n");
   }

int32_t
Test::Monitor::exit()
   {
   int32_t rc = pthread_mutex_unlock(&_monitor);
   TR_ASSERT(rc == 0, "error unlocking monitor\n");
   return rc;
   }

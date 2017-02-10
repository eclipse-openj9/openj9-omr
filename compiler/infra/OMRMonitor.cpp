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

#include "infra/OMRMonitor.hpp"

#include "env/CompilerEnv.hpp"
#include "env/TRMemory.hpp"
#include "infra/Assert.hpp"
#include "infra/Monitor.hpp"
#include "infra/MonitorTable.hpp"

TR::MonitorTable *OMR::MonitorTable::_instance = 0;

TR::Monitor *memoryAllocMonitor = NULL;

void *
OMR::Monitor::operator new(size_t size)
   {
   return TR::Compiler->persistentAllocator().allocate(size);
   }

void
OMR::Monitor::operator delete(void *p)
   {
   TR::Compiler->persistentAllocator().deallocate(p);
   }

TR::Monitor *
OMR::Monitor::create(char *name)
   {
   TR::Monitor * monitor = new TR::Monitor();
   monitor->init(name);
   return monitor;
   }

OMR::Monitor::~Monitor()
   {
   self()->destroy();
   }

TR::Monitor *
OMR::Monitor::self()
   {
   return static_cast<TR::Monitor*>(this);
   }

bool
OMR::Monitor::init(char *name)
   {
   _name = name;
   MUTEX_INIT(_monitor);
   bool rc = MUTEX_INIT(_monitor);
   TR_ASSERT(rc == true, "error initializing monitor\n");
   return true;
   }

void
OMR::Monitor::destroy(TR::Monitor *monitor)
   {
   delete monitor;
   }

void
OMR::Monitor::destroy()
   {
#ifdef WINDOWS
   MUTEX_DESTROY(_monitor);
#else
   int32_t rc = MUTEX_DESTROY(_monitor);
   TR_ASSERT(rc == 0, "error destroying monitor\n");
#endif
   }

void
OMR::Monitor::enter()
   {
#ifdef WINDOWS
   MUTEX_ENTER(_monitor);
#else
   int32_t rc = MUTEX_ENTER(_monitor);
   TR_ASSERT(rc == 0, "error locking monitor\n");
#endif
   }

int32_t
OMR::Monitor::exit()
   {
#ifdef WINDOWS
   MUTEX_EXIT(_monitor);
   return 0;
#else
   int32_t rc = MUTEX_EXIT(_monitor);
   TR_ASSERT(rc == 0, "error unlocking monitor\n");
   return rc;
#endif
   }

char const *
OMR::Monitor::getName()
   {
   return _name;
   }

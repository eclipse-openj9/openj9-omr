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

#include "runtime/CodeCacheMemorySegment.hpp"
#include "runtime/CodeCacheManager.hpp"

TR::CodeCacheMemorySegment*
OMR::CodeCacheMemorySegment::self()
   {
   return static_cast<TR::CodeCacheMemorySegment*>(this);
   }


void
OMR::CodeCacheMemorySegment::adjustAlloc(int64_t adjust)
   {
   self()->setSegmentAlloc(self()->segmentAlloc() + adjust);
   }


void
OMR::CodeCacheMemorySegment::free(TR::CodeCacheManager *manager)
   {
   manager->freeMemory(_base);
   new (static_cast<TR::CodeCacheMemorySegment *>(this)) TR::CodeCacheMemorySegment();
   }


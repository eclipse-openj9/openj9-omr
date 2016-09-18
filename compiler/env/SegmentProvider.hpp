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

#ifndef TR_SEGMENT_PROVIDER
#define TR_SEGMENT_PROVIDER

#pragma once

#include <stddef.h>
#include <new>

namespace TR {

class MemorySegment;

class SegmentProvider
   {
public:
   virtual TR::MemorySegment& request(size_t requiredSize) = 0;
   virtual void release(TR::MemorySegment& segment) throw() = 0;
   size_t defaultSegmentSize() { return _defaultSegmentSize; }


protected:
   SegmentProvider(size_t defaultSegmentSize);
   SegmentProvider(const SegmentProvider &other);

   /*
    * Require knowledge of the concrete class in order to destroy SegmentProviders
    */
   virtual ~SegmentProvider() throw();

   size_t const _defaultSegmentSize;
   };

}

#endif // TR_SEGMENT_PROVIDER

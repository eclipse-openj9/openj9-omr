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

#ifndef TR_CODECACHEMEMORYSEGMENT_INCL
#define TR_CODECACHEMEMORYSEGMENT_INCL

#include "runtime/OMRCodeCacheMemorySegment.hpp"

#include <stddef.h>  // for size_t
#include <stdint.h>  // for uint8_t

namespace TR
{

#ifndef TR_CODECACHEMEMORYSEGMENT_COMPOSED
#define TR_CODECACHEMEMORYSEGMENT_COMPOSED

class OMR_EXTENSIBLE CodeCacheMemorySegment : public OMR::CodeCacheMemorySegmentConnector
   {
   public:
   CodeCacheMemorySegment()                              : OMR::CodeCacheMemorySegmentConnector()             { }
   CodeCacheMemorySegment(uint8_t *memory, size_t size)  : OMR::CodeCacheMemorySegmentConnector(memory, size) { }
   CodeCacheMemorySegment(uint8_t *memory, uint8_t *top) : OMR::CodeCacheMemorySegmentConnector(memory, top)  { }
   };

#endif

}

#endif
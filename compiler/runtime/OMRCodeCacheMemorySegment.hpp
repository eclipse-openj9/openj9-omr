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

#ifndef OMR_CODECACHEMEMORYSEGMEN_INCL
#define OMR_CODECACHEMEMORYSEGMEN_INCL

/*
 * The following #defines and typedefs must appear before any #includes in this file
 */

#ifndef OMR_CODECACHEMEMORYSEGMENT_COMPOSED
#define OMR_CODECACHEMEMORYSEGMENT_COMPOSED
namespace OMR { class CodeCacheMemorySegment; }
namespace OMR { typedef CodeCacheMemorySegment CodeCacheMemorySegmentConnector; }
#endif

#include <stddef.h>               // for size_t
#include <stdint.h>               // for uint8_t, int32_t, etc
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE
#include "runtime/CodeCacheTypes.hpp"


namespace TR { class CodeCacheMemorySegment; }
namespace TR { class CodeCacheManager; }

namespace OMR
{

class OMR_EXTENSIBLE CodeCacheMemorySegment
   {
public:
   CodeCacheMemorySegment() : _base(NULL), _alloc(NULL), _top(NULL) { }
   CodeCacheMemorySegment(uint8_t *memory, size_t size) : _base(memory), _alloc(memory), _top(memory+size) { }
   CodeCacheMemorySegment(uint8_t *memory, uint8_t *top) : _base(memory), _alloc(memory), _top(top) { }

   TR::CodeCacheMemorySegment *self();

   void *operator new(size_t size, TR::CodeCacheMemorySegment *segment) { return segment; }

   uint8_t *segmentBase()  { return _base; }
   uint8_t *segmentAlloc() { return _alloc; }
   uint8_t *segmentTop()   { return _top; }

   void adjustAlloc(int64_t adjust);

   void setSegmentBase(uint8_t *newBase)   { _base = newBase; }
   void setSegmentAlloc(uint8_t *newAlloc) { _alloc = newAlloc; }
   void setSegmentTop(uint8_t *newTop)     { _top = newTop; }

   // memory is backed by something else
   void free(TR::CodeCacheManager *manager);

   uint8_t *_base;
   uint8_t *_alloc;
   uint8_t *_top;
   };

}

#endif // OMR_CODECACHEMEMORYSEGMEN_INCL


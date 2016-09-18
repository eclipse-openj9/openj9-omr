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

#ifndef NODEEXTENSION_INCL
#define NODEEXTENSION_INCL

#include <stddef.h>                       // for size_t
#include <stdint.h>                       // for uintptr_t, uint16_t, etc
#include "env/TRMemory.hpp"               // for TR_ArenaAllocator

#define NUM_DEFAULT_ELEMS 1
#define NUM_DEFAULT_NODECHILDREN 2

typedef TR_ArenaAllocator TR_NodeExtAllocator;

namespace TR
{

/**
 * A flexible data storage class for associating information with nodes.
 *
 * \note This class works via overallocation, which I would prefer to see
 *       go away in lieu of a more dynamically sized object that is less
 *       discomforting.
 */
class NodeExtension
    {
public:
    NodeExtension(TR_NodeExtAllocator & alloc) :
       _allocator(alloc) {};

    void * operator new (size_t s, uint16_t arrayElems, TR_NodeExtAllocator & m)
       {
       s+= (arrayElems-NUM_DEFAULT_ELEMS) * sizeof(uintptr_t);
       return m.allocate(s);
       }

    void   operator delete(void * ptr, size_t s, TR_NodeExtAllocator & m)
       {
       m.deallocate(ptr,s);
       };

    template <class T>
    T  setElem(uint16_t index, T elem)
       {
       (T)(_data[index] = (uintptr_t)elem);
       return (T)_data[index];
       }

    template <class T>
    T  getElem(uint16_t index)
       {
       return (T)_data[index];
       }

    void
    freeExtension(size_t s)
       {
       _allocator.deallocate(this,s);
       }

  private:
      TR_NodeExtAllocator & _allocator;
      uintptr_t _data[NUM_DEFAULT_ELEMS];
    };

}

#endif

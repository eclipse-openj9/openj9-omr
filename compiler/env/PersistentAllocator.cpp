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

#include "env/PersistentAllocator.hpp"

OMR::PersistentAllocator::PersistentAllocator(const TR::PersistentAllocatorKit &allocatorKit) :
   _rawAllocator(allocatorKit.rawAllocator)
   {
   }

void *
OMR::PersistentAllocator::allocate(size_t size, const std::nothrow_t tag, void * hint) throw()
   {
   return _rawAllocator.allocate(size, tag, hint);
   }

void *
OMR::PersistentAllocator::allocate(size_t size, void * hint)
   {
   return _rawAllocator.allocate(size, hint);
   }

void
OMR::PersistentAllocator::deallocate(void * p, const size_t sizeHint) throw()
   {
   _rawAllocator.deallocate(p, sizeHint);
   }

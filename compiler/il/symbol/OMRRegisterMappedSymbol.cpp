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

#include "il/symbol/OMRRegisterMappedSymbol.hpp"

#include <stdint.h>                            // for int32_t, uint16_t, etc
#include "codegen/FrontEnd.hpp"                // for TR_FrontEnd
#include "env/TRMemory.hpp"
#include "il/Symbol.hpp"
#include "il/symbol/RegisterMappedSymbol.hpp"
#include "il/SymbolReference.hpp"
#include "infra/Assert.hpp"                    // for TR_ASSERT
#include "infra/Flags.hpp"                     // for flags32_t
#include "limits.h"                            // for USHRT_MAX


template <typename AllocatorType>
TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(AllocatorType m, int32_t o)
   {
   return new (m) TR::RegisterMappedSymbol(o);
   }

template <typename AllocatorType>
TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(AllocatorType m, TR::DataTypes d)
   {
   return new (m) TR::RegisterMappedSymbol(d);
   }

template <typename AllocatorType>
TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(AllocatorType m, TR::DataTypes d, uint32_t s)
   {
   return new (m) TR::RegisterMappedSymbol(d,s);
   }

OMR::RegisterMappedSymbol::RegisterMappedSymbol(int32_t o) :
   TR::Symbol(),
   _mappedOffset(o),
   _GCMapIndex(-1)
   {
   self()->setLiveLocalIndexUninitialized();
   }

OMR::RegisterMappedSymbol::RegisterMappedSymbol(TR::DataTypes d) :
   TR::Symbol(d),
   _mappedOffset(0),
   _GCMapIndex(-1)
   {
   self()->setLiveLocalIndexUninitialized();
   }

OMR::RegisterMappedSymbol::RegisterMappedSymbol(TR::DataTypes d, uint32_t s) :
   TR::Symbol(d, s),
   _mappedOffset(0),
   _GCMapIndex(-1)
   {
   self()->setLiveLocalIndexUninitialized();
   }

TR::RegisterMappedSymbol *
OMR::RegisterMappedSymbol::self()
   {
   return static_cast<TR::RegisterMappedSymbol*>(this);
   }

void
OMR::RegisterMappedSymbol::setLiveLocalIndex(uint16_t i, TR_FrontEnd * fe)
   {
   _liveLocalIndex = i;
   if (self()->isLiveLocalIndexUninitialized())
      {
      TR_ASSERT(0, "OMR::RegisterMappedSymbol::_liveLocalIndex == USHRT_MAX");
      fe->outOfMemory(0, "OMR::RegisterMappedSymbol::_liveLocalIndex == USHRT_MAX");
      }
   }

bool
OMR::RegisterMappedSymbol::isLiveLocalIndexUninitialized()
   {
   return _liveLocalIndex == USHRT_MAX;
   }

void
OMR::RegisterMappedSymbol::setLiveLocalIndexUninitialized()
   {
   _liveLocalIndex = USHRT_MAX;
   }

template <typename AllocatorType>
TR::RegisterMappedSymbol *
OMR::RegisterMappedSymbol::createMethodMetaDataSymbol(AllocatorType m, const char *name, TR_MethodMetaDataType type)
   {
   TR::RegisterMappedSymbol * sym = new (m) TR::RegisterMappedSymbol();
   sym->_type                     = type;
   sym->_name                     = name;
   sym->_flags.setValue(KindMask, IsMethodMetaData);
   return sym;
   }

/**
 * Explicit instantiation
 */

template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_HeapMemory m, int32_t o);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_HeapMemory m, TR::DataTypes d);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_HeapMemory m, TR::DataTypes d, uint32_t s);

template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_StackMemory m, int32_t o);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_StackMemory m, TR::DataTypes d);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(TR_StackMemory m, TR::DataTypes d, uint32_t s);

template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(PERSISTENT_NEW_DECLARE m, int32_t o);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(PERSISTENT_NEW_DECLARE m, TR::DataTypes d);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::create(PERSISTENT_NEW_DECLARE m, TR::DataTypes d, uint32_t s);

template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::createMethodMetaDataSymbol(TR_HeapMemory m, const char *name, TR_MethodMetaDataType type);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::createMethodMetaDataSymbol(TR_StackMemory m, const char *name, TR_MethodMetaDataType type);
template TR::RegisterMappedSymbol * OMR::RegisterMappedSymbol::createMethodMetaDataSymbol(PERSISTENT_NEW_DECLARE m, const char *name, TR_MethodMetaDataType type);

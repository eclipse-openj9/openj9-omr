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

#include "il/symbol/OMRParameterSymbol.hpp"

#include <stddef.h>                       // for size_t
#include <stdint.h>                       // for int32_t, uint32_t
#include <string.h>                       // for strlen
#include "env/KnownObjectTable.hpp"       // for KnownObjectTable, etc
#include "env/TRMemory.hpp"
#include "il/DataTypes.hpp"               // for DataTypes, etc
#include "il/Symbol.hpp"                  // for Symbol::IsParameter, etc
#include "il/symbol/ParameterSymbol.hpp"  // for ParameterSymbol
#include "infra/Flags.hpp"                // for flags32_t

TR::ParameterSymbol *
OMR::ParameterSymbol::self()
   {
   return static_cast<TR::ParameterSymbol*>(this);
   }

OMR::ParameterSymbol::ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot) :
   TR::RegisterMappedSymbol(d),
   _registerIndex(-1),
   _allocatedHigh(-1),
   _allocatedLow(-1),
   _fixedType(0),
   _isPreexistent(false),
   _isUnsigned(isUnsigned),
   _knownObjectIndex(TR::KnownObjectTable::UNKNOWN)
   {
   _flags.setValue(KindMask, IsParameter);
   _addressSize = TR::ParameterSymbol::convertTypeToSize(TR::Address);
   self()->setOffset(slot * TR::ParameterSymbol::convertTypeToSize(TR::Address));
   }

OMR::ParameterSymbol::ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot, size_t size) :
   TR::RegisterMappedSymbol(d, (uint32_t)size), // cast argument size explicitly \TODO: Document why?
   _registerIndex(-1),
   _allocatedHigh(-1),
   _allocatedLow(-1),
   _fixedType(0),
   _isPreexistent(false),
   _isUnsigned(isUnsigned),
   _knownObjectIndex(TR::KnownObjectTable::UNKNOWN)
   {
   _flags.setValue(KindMask, IsParameter);
   _addressSize = TR::ParameterSymbol::convertTypeToSize(TR::Address);
   self()->setOffset(slot * TR::ParameterSymbol::convertTypeToSize(TR::Address));
   }

void
OMR::ParameterSymbol::setParameterOffset(int32_t o)
   {
   self()->setOffset(o);
   }

int32_t
OMR::ParameterSymbol::getSlot()
   {
   return self()->getParameterOffset() / (uint32_t)_addressSize; // cast _addressSize explicity
   }

template <typename AllocatorType>
TR::ParameterSymbol * OMR::ParameterSymbol::create(AllocatorType m, TR::DataTypes d, bool isUnsigned, int32_t slot)
   {
   return new (m) TR::ParameterSymbol(d, isUnsigned, slot);
   }

template <typename AllocatorType>
TR::ParameterSymbol * OMR::ParameterSymbol::create(AllocatorType m, TR::DataTypes d, bool isUnsigned, int32_t slot, size_t size)
   {
   return new (m) TR::ParameterSymbol(d, isUnsigned, slot, size);
   }


template TR::ParameterSymbol * OMR::ParameterSymbol::create(TR_StackMemory, TR::DataTypes, bool, int32_t);
template TR::ParameterSymbol * OMR::ParameterSymbol::create(TR_StackMemory, TR::DataTypes, bool, int32_t, size_t);
template TR::ParameterSymbol * OMR::ParameterSymbol::create(TR_HeapMemory, TR::DataTypes, bool, int32_t);
template TR::ParameterSymbol * OMR::ParameterSymbol::create(TR_HeapMemory, TR::DataTypes, bool, int32_t, size_t);
template TR::ParameterSymbol * OMR::ParameterSymbol::create(PERSISTENT_NEW_DECLARE, TR::DataTypes, bool, int32_t);
template TR::ParameterSymbol * OMR::ParameterSymbol::create(PERSISTENT_NEW_DECLARE, TR::DataTypes, bool, int32_t, size_t);

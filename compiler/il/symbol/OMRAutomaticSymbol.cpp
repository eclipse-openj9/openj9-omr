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

#include "il/symbol/OMRAutomaticSymbol.hpp"

#include <stddef.h>                           // for NULL, size_t
#include <stdint.h>                           // for uint32_t, int32_t
#include "codegen/RegisterConstants.hpp"      // for TR_RegisterKinds
#include "env/TRMemory.hpp"                   // for PERSISTENT_NEW_DECLARE
#include "il/ILOpCodes.hpp"                   // for ILOpCodes::newarray, etc
#include "il/Node.hpp"                        // for rcount_t
#include "il/symbol/AutomaticSymbol.hpp"      // for AutomaticSymbolBase, etc
#include "infra/Assert.hpp"                   // for TR_ASSERT
#include "infra/Flags.hpp"                    // for flags8_t, flags32_t
#include "infra/IGNode.hpp"                   // for UNCOLOURED, etc
#include "infra/InterferenceGraph.hpp"        // for TR_InterferenceGraph

class TR_FrontEnd;
namespace TR { class Node; }
namespace TR { class SymbolReference; }

rcount_t
OMR::AutomaticSymbol::setReferenceCount(rcount_t i)
   {
   if (isVariableSizeSymbol() && i > 0)
      castToVariableSizeSymbol()->setIsReferenced();
   return (_referenceCount = i);
   }

rcount_t
OMR::AutomaticSymbol::incReferenceCount()
   {
   if (isVariableSizeSymbol())
      castToVariableSizeSymbol()->setIsReferenced();
   return ++_referenceCount;
   }

void
OMR::AutomaticSymbol::init()
   {
   _referenceCount = 0;
   _flags.setValue(KindMask, IsAutomatic);
   _name = NULL;
   }

TR::SymbolReference *
OMR::AutomaticSymbol::getClassSymbolReference()
   {
   TR_ASSERT(isLocalObject(), "Should be tagged as local object");
   return _kind != TR::newarray ? _classSymRef : 0;
   }

TR::SymbolReference *
OMR::AutomaticSymbol::setClassSymbolReference(TR::SymbolReference *s)
   {
   TR_ASSERT(isLocalObject(), "Should be tagged as local object");
   return (_classSymRef = s);
   }

int32_t
OMR::AutomaticSymbol::getArrayType()
   {
   TR_ASSERT(isLocalObject(), "Should be tagged as local object");
   return _kind == TR::newarray ? _arrayType : 0;
   }


template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::create(AllocatorType t)
   {
   return new (t) TR::AutomaticSymbol();
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::create(AllocatorType t,  TR::DataTypes d)
   {
   return new (t) TR::AutomaticSymbol(d);
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::create(AllocatorType t,  TR::DataTypes d, uint32_t s)
   {
   return new (t) TR::AutomaticSymbol(d,s);
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::create(AllocatorType t,  TR::DataTypes d, uint32_t s, const char * n)
   {
   return new (t) TR::AutomaticSymbol(d,s,n);
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createMarker(AllocatorType m, const char * name)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol();
   sym->_name = name;
   sym->setAutoMarkerSymbol();
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createRegisterSymbol(AllocatorType m, TR_RegisterKinds regKind, uint32_t globalRegNum, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol(d,s);
   sym->_regKind = regKind;
   sym->_globalRegisterNumber = globalRegNum;
   sym->setIsRegisterSymbol();
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(AllocatorType m, int32_t arrayType, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe)
   {
   TR::AutomaticSymbol * sym   = new (m) TR::AutomaticSymbol(d, s);

   sym->_kind                 = TR::newarray;
   sym->_referenceSlots       = NULL;
   sym->_arrayType            = arrayType;
   sym->setLocalObject();
   return sym;
   }



template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(AllocatorType m, TR::ILOpCodes kind, TR::SymbolReference * classSymRef, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe)
   {
   TR_ASSERT(kind == TR::newarray     ||
             kind == TR::New          ||
             kind == TR::anewarray,
             "Invalid kind passed to local object factory");
   TR::AutomaticSymbol * sym  = new (m) TR::AutomaticSymbol(d, s);
   sym->_kind                 = kind;
   sym->_referenceSlots       = NULL;
   sym->_classSymRef          = classSymRef;
   sym->setLocalObject();
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(AllocatorType m, TR::AutomaticSymbol *pinningArrayPointer)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol();
   sym->setInternalPointer();
   sym->setPinningArrayPointer(pinningArrayPointer);
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(AllocatorType m, TR::DataTypes d, TR::AutomaticSymbol *pinningArrayPointer)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol(d);
   sym->setInternalPointer();
   sym->setPinningArrayPointer(pinningArrayPointer);
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(AllocatorType m, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol(d,s);
   sym->setInternalPointer();
   sym->setPinningArrayPointer(NULL);
   return sym;
   }

template <typename AllocatorType>
TR::AutomaticSymbol * OMR::AutomaticSymbol::createVariableSized(AllocatorType m, uint32_t s)
   {
   TR::AutomaticSymbol * sym = new (m) TR::AutomaticSymbol(TR::NoType, s);
   sym->_activeSize = s;
   sym->_nodeToFreeAfter = NULL;
   sym->_variableSizeSymbolFlags = 0;
   sym->setVariableSizeSymbol();
   return sym;
   }



// Explicit instantiations.


template TR::AutomaticSymbol * OMR::AutomaticSymbol::createMarker(TR_HeapMemory m, const char * name) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createRegisterSymbol(TR_HeapMemory m, TR_RegisterKinds regKind, uint32_t globalRegNum, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(TR_HeapMemory m, int32_t arrayType, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(TR_HeapMemory m, TR::ILOpCodes kind, TR::SymbolReference * classSymRef, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_HeapMemory m, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_HeapMemory m, TR::DataTypes d, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_HeapMemory m, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createVariableSized(TR_HeapMemory m, uint32_t s);

template TR::AutomaticSymbol * OMR::AutomaticSymbol::createMarker(TR_StackMemory m, const char * name) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createRegisterSymbol(TR_StackMemory m, TR_RegisterKinds regKind, uint32_t globalRegNum, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(TR_StackMemory m, int32_t arrayType, TR::DataTypes   d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(TR_StackMemory m, TR::ILOpCodes kind, TR::SymbolReference * classSymRef, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_StackMemory m, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_StackMemory m, TR::DataTypes d, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(TR_StackMemory m, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createVariableSized(TR_StackMemory m, uint32_t s);

template TR::AutomaticSymbol * OMR::AutomaticSymbol::createMarker(PERSISTENT_NEW_DECLARE m, const char * name) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createRegisterSymbol(PERSISTENT_NEW_DECLARE m, TR_RegisterKinds regKind, uint32_t globalRegNum, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe) ;
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(PERSISTENT_NEW_DECLARE m, int32_t arrayType, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createLocalObject(PERSISTENT_NEW_DECLARE m, TR::ILOpCodes kind, TR::SymbolReference * classSymRef, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(PERSISTENT_NEW_DECLARE m, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(PERSISTENT_NEW_DECLARE m, TR::DataTypes d, TR::AutomaticSymbol *pinningArrayPointer);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createInternalPointer(PERSISTENT_NEW_DECLARE m, TR::DataTypes d, uint32_t s, TR_FrontEnd * fe);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::createVariableSized(PERSISTENT_NEW_DECLARE m, uint32_t s);


template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_StackMemory);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_StackMemory,  TR::DataTypes);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_StackMemory,  TR::DataTypes, uint32_t);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_StackMemory,  TR::DataTypes, uint32_t, const char *);

template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_HeapMemory);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_HeapMemory,  TR::DataTypes);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_HeapMemory,  TR::DataTypes, uint32_t);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(TR_HeapMemory,  TR::DataTypes, uint32_t, const char *);

template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(PERSISTENT_NEW_DECLARE);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(PERSISTENT_NEW_DECLARE,  TR::DataTypes);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(PERSISTENT_NEW_DECLARE,  TR::DataTypes, uint32_t);
template TR::AutomaticSymbol * OMR::AutomaticSymbol::create(PERSISTENT_NEW_DECLARE,  TR::DataTypes, uint32_t, const char *);

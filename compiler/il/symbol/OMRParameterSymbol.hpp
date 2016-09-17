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

#ifndef OMR_PARAMETERSYMBOL_INCL
#define OMR_PARAMETERSYMBOL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_PARAMETERSYMBOL_CONNECTOR
#define OMR_PARAMETERSYMBOL_CONNECTOR
namespace OMR { class ParameterSymbol; }
namespace OMR { typedef OMR::ParameterSymbol ParameterSymbolConnector; }
#endif

#include "il/symbol/RegisterMappedSymbol.hpp"

#include <stddef.h>                  // for size_t
#include <stdint.h>                  // for int32_t, int8_t, etc
#include "env/KnownObjectTable.hpp"  // for KnownObjectTable, etc
#include "il/DataTypes.hpp"          // for DataTypes

namespace TR { class ParameterSymbol; }

namespace OMR
{

class ParameterSymbol : public TR::RegisterMappedSymbol
   {

protected:

   ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot);

   ParameterSymbol(TR::DataTypes d, bool isUnsigned, int32_t slot, size_t size);

public:

   template <typename AllocatorType>
   static TR::ParameterSymbol * create(AllocatorType, TR::DataTypes, bool, int32_t);

   template <typename AllocatorType>
   static TR::ParameterSymbol * create(AllocatorType, TR::DataTypes, bool, int32_t, size_t);

   int32_t  getParameterOffset()               { return _mappedOffset; }
   void     setParameterOffset(int32_t o)      { setOffset(o); }

   int32_t  getSlot()                          { return getParameterOffset() / (uint32_t)_addressSize;} // cast _addressSize explicity

   int32_t  getOrdinal()                       { return _ordinal; }
   void     setOrdinal(int32_t o)              { _ordinal = o; }

   int8_t   getLinkageRegisterIndex()          { return _registerIndex; }
   void     setLinkageRegisterIndex(int8_t li) { _registerIndex = li; }
   bool     isParmPassedInRegister()           { return (_registerIndex >= 0) ? true : false; }

   int8_t   getAllocatedIndex()                { return _allocatedHigh; }
   void     setAllocatedIndex(int8_t ai)       { _allocatedHigh = ai; }

   int8_t   getAllocatedHigh()                 { return _allocatedHigh; }
   void     setAllocatedHigh(int8_t ai)        { _allocatedHigh = ai; }

   int8_t   getAllocatedLow()                  { return _allocatedLow; }
   void     setAllocatedLow(int8_t ai)         { _allocatedLow = ai; }

   void*    getFixedType()                     { return _fixedType; }
   void     setFixedType(void* t)              { _fixedType = t; }

   bool     getIsPreexistent()                 { return _isPreexistent; }
   void     setIsPreexistent(bool t)           { _isPreexistent = t; }

   bool     hasKnownObjectIndex() { return _knownObjectIndex != TR::KnownObjectTable::UNKNOWN; }
   void     setKnownObjectIndex(TR::KnownObjectTable::Index i) { _knownObjectIndex = i; }
   TR::KnownObjectTable::Index getKnownObjectIndex() { return _knownObjectIndex; }

   bool     getIsUnsigned()                    { return _isUnsigned; }
   void     setIsUnsigned(bool t)              { _isUnsigned = t; }

   void           setTypeSignature(const char * s, int32_t l) { _typeLength = l; _typeSignature = s; }
   const char *   getTypeSignature(int32_t & len) { len = _typeLength; return _typeSignature; }

private:

   const char *                _typeSignature;
   void *                      _fixedType;
   int32_t                     _typeLength;
   int32_t                     _ordinal;
   int8_t                      _registerIndex;
   int8_t                      _allocatedHigh;
   int8_t                      _allocatedLow;
   bool                        _isPreexistent;
   bool                        _isUnsigned;
   uintptr_t                   _addressSize;
   TR::KnownObjectTable::Index _knownObjectIndex;
   };

}

#endif


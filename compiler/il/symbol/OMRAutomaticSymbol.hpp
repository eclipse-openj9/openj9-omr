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

#ifndef OMR_AUTOMATICSYMBOL_INCL
#define OMR_AUTOMATICSYMBOL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_AUTOMATICSYMBOL_CONNECTOR
#define OMR_AUTOMATICSYMBOL_CONNECTOR
namespace OMR { class AutomaticSymbol; }
namespace OMR { typedef OMR::AutomaticSymbol AutomaticSymbolConnector; }
#endif

#include "il/symbol/RegisterMappedSymbol.hpp"

#include <stddef.h>                       // for size_t
#include <stdint.h>                       // for uint32_t, int32_t
#include "codegen/RegisterConstants.hpp"  // for TR_RegisterKinds
#include "il/DataTypes.hpp"               // for DataTypes
#include "il/ILOpCodes.hpp"               // for ILOpCodes
#include "il/Node.hpp"                    // for rcount_t
#include "infra/Assert.hpp"               // for TR_ASSERT
#include "infra/Flags.hpp"                // for flags8_t, flags32_t

class TR_FrontEnd;
namespace TR { class AutomaticSymbol; }
namespace TR { class SymbolReference; }

namespace OMR
{

class OMR_EXTENSIBLE AutomaticSymbol : public TR::RegisterMappedSymbol
   {

public:

   template <typename AllocatorType>
   static TR::AutomaticSymbol * create(AllocatorType);

   template <typename AllocatorType>
   static TR::AutomaticSymbol * create(AllocatorType, TR::DataType);

   template <typename AllocatorType>
   static TR::AutomaticSymbol * create(AllocatorType, TR::DataType, uint32_t);

   template <typename AllocatorType>
   static TR::AutomaticSymbol * create(AllocatorType, TR::DataType, uint32_t, const char *);

protected:

   AutomaticSymbol();

   AutomaticSymbol(TR::DataType d);

   AutomaticSymbol(TR::DataType d, uint32_t s);

   AutomaticSymbol(TR::DataType d, uint32_t s, const char *name);

   void init();

   TR::AutomaticSymbol * self();

public:

   /**
    * Reference counting methods.
    *
    * \note Since this data is only actually consumed and manipulated at
    *       codegen time, it should be incumbent on the code generators to
    *       maintain the reference count information.
    *
    * @{
    */
   rcount_t getReferenceCount()           { return _referenceCount; }
   rcount_t decReferenceCount()           { return --_referenceCount; }
   rcount_t setReferenceCount(rcount_t i);
   rcount_t incReferenceCount();
   /** @} */

private:

   rcount_t _referenceCount;

/** TR_AutomaticMarkerSymbol
 * @{
 */
public:

   /**
    * Factory to create an automatic marker symbol.
    */
   template <typename AllocatorType>
   static TR::AutomaticSymbol * createMarker(AllocatorType m, const char * name);

/** @} */

public:
/**
 * The class formerly known as TR_RegisterSymbol
 *
 * The following is to be used by register allocation. We have Hardware and
 * Symbolic register auto temps.
 *
 *  - A Hardware register auto means that a load/store from this register auto
 *  maps directly to the register it describes.
 *  - A symbolic register auto is left open and instruction level register
 *  allocation is free to choose any register for it as well as it can choose
 *  to map this register auto to a spill location.
 *
 * \todo Verify the above comment is correct, or at least remap the terms to something recognizable.
 *
 *
 */
   template <typename AllocatorType>
   static TR::AutomaticSymbol * createRegisterSymbol(AllocatorType m, TR_RegisterKinds regKind, uint32_t globalRegNum, TR::DataType d, uint32_t s, TR_FrontEnd * fe);

   TR_RegisterKinds  getRegisterKind() const                { return _regKind;                        }
   void              setGlobalRegisterNumber(uint32_t grn)  { _globalRegisterNumber = grn;            }
   uint32_t          getGlobalRegisterNumber() const        { return _globalRegisterNumber;           }
   void              setRealRegister()                      { return (_flags2.set(RealRegister));     }
   bool              isRealRegister()                       { return (_flags2.testAny(RealRegister)); }

private:

   TR_RegisterKinds _regKind;
   int32_t _globalRegisterNumber;
/** @} */

/**
 * TR_LocalObjectSymbol
 *
 * On stack object symbol
 *
 * @{
 */
public:

   /**
    * Local object symbol factory
    */
   template <typename AllocatorType>
   static TR::AutomaticSymbol * createLocalObject(AllocatorType  m,
                                                 int32_t          arrayType,
                                                 TR::DataType    d,
                                                 uint32_t         s,
                                                 TR_FrontEnd *    fe);

   /**
    * Local object symbol factory with specified kind.
    *
    * Currently, only supported kinds are TR::New, TR::newarray
    * and TR::anewarray
    */
   template <typename AllocatorType>
   static TR::AutomaticSymbol * createLocalObject(AllocatorType          m,
                                                   TR::ILOpCodes          kind,
                                                   TR::SymbolReference *  classSymRef,
                                                   TR::DataType          d,
                                                   uint32_t               s,
                                                   TR_FrontEnd *          fe);

   TR::ILOpCodes getKind()
      {
      TR_ASSERT(isLocalObject(), "Should be local object");
      return _kind;
      }

   TR::SymbolReference *getClassSymbolReference();
   TR::SymbolReference *setClassSymbolReference(TR::SymbolReference *s);

   int32_t getArrayType();

   int32_t *getReferenceSlots()                { return _referenceSlots; }
   void     setReferenceSlots(int32_t *fields) { _referenceSlots = fields; }


private:
   //! Array of indices of collected reference slots, zero terminated
   int32_t * _referenceSlots;
   union
      {
      TR::SymbolReference *_classSymRef;
      int32_t               _arrayType;
      };


   /**
    * Store the kind of this local object.
    *
    * \todo Since there are only a small subset of the ILOpCode types that are
    *       valid here, we should really map the opcode to an internal enum,
    *       and then handle these types using a switch. This would allow the
    *       compiler to help us catch unhandled cases.
    */
   TR::ILOpCodes  _kind;
/** @} */

/**
 * TR_InternalPointerAutomaticSymbol
 *
 * @{
 */
public:

   template <typename AllocatorType>
   static TR::AutomaticSymbol * createInternalPointer(AllocatorType m, TR::AutomaticSymbol *pinningArrayPointer = 0);

   template <typename AllocatorType>
   static TR::AutomaticSymbol * createInternalPointer(AllocatorType m, TR::DataType d, TR::AutomaticSymbol *pinningArrayPointer = 0);

   template <typename AllocatorType>
   static TR::AutomaticSymbol * createInternalPointer(AllocatorType m, TR::DataType d, uint32_t s, TR_FrontEnd * fe);

   TR::AutomaticSymbol *getPinningArrayPointer()
      {
      TR_ASSERT(isInternalPointer(), "Should be internal pointer");
      return _pinningArrayPointer;
      }

   // Expose base name on derived calls.
   using TR::Symbol::setPinningArrayPointer;

   TR::AutomaticSymbol *setPinningArrayPointer(TR::AutomaticSymbol *s)
      {
      TR_ASSERT(isInternalPointer(), "Should be internal pointer");
      return (_pinningArrayPointer = s);
      }

private:

   TR::AutomaticSymbol *_pinningArrayPointer;

   /** @} */

   /**
    * TR_VariableSizeSymbol
    *
    * @{
    */

public:

   #define TR_VSS_NAME "VTS"

   template <typename AllocatorType>
   static TR::AutomaticSymbol * createVariableSized(AllocatorType m, uint32_t s);

   uint32_t getActiveSize()
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _activeSize;
      }

   uint32_t setActiveSize(uint32_t s)
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return (_activeSize = s);
      }

   /**
    * Flags for variable size symbols
    */
   enum
      {
      IsReferenced                  = 0x01, ///< was this symbol ever actually used in any instruction? -- if not then it does not have to be mapped
      IsAddressTaken                = 0x02, ///< address taken symbols have to be kept alive longer
      IsSingleUse                   = 0x04, ///< a temp ref created for a one off privatization in a single evaluator (doesn't use refCounts so automatically freed)
      };

   bool isReferenced()
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _variableSizeSymbolFlags.testAny(IsReferenced);
      }

   void setIsReferenced(bool b = true)
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      _variableSizeSymbolFlags.set(IsReferenced, b);
      }

   bool isAddressTaken()
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _variableSizeSymbolFlags.testAny(IsAddressTaken);
      }

   void setIsAddressTaken(bool b = true)
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      _variableSizeSymbolFlags.set(IsAddressTaken, b);
      }

   bool isSingleUse()
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _variableSizeSymbolFlags.testAny(IsSingleUse);
      }

   void setIsSingleUse(bool b = true)
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      _variableSizeSymbolFlags.set(IsSingleUse, b);
      }

   TR::Node *getNodeToFreeAfter()
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _nodeToFreeAfter;
      }

   TR::Node *setNodeToFreeAfter(TR::Node *n)
      {
      TR_ASSERT(isVariableSizeSymbol(), "Should be variable sized symbol");
      return _nodeToFreeAfter = n;
      }

private:

   TR::Node *  _nodeToFreeAfter;
   uint32_t    _activeSize;
   flags8_t    _variableSizeSymbolFlags;

   /** @} */
   };
}

#endif

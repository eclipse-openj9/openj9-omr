/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2016
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


#ifndef OMR_TYPEDICTIONARY_INCL
#define OMR_TYPEDICTIONARY_INCL


#ifndef TR_ILTYPE_DEFINED
#define TR_ILTYPE_DEFINED
#define PUT_OMR_ILTYPE_INTO_TR
#endif

#ifndef TR_TYPEDICTIONARY_DEFINED
#define TR_TYPEDICTIONARY_DEFINED
#define PUT_OMR_TYPEDICTIONARY_INTO_TR
#endif


#include "ilgen/IlBuilder.hpp"

class TR_HashTabString;

namespace TR { typedef TR::SymbolReference IlReference; }

namespace TR { class SegmentProvider; }
namespace TR { class Region; }
class TR_Memory;


namespace OMR
{

class IlType
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   IlType(const char *name) :
      _name(name)
      { }
   IlType() :
      _name(0)
      { }

   const char *getName() { return _name; }
   virtual char *getSignatureName();

   virtual TR::DataType getPrimitiveType() { return TR::NoType; }

   virtual bool isArray() { return false; }
   virtual bool isPointer() { return false; }
   virtual TR::IlType *baseType() { return NULL; }

   virtual bool isStruct() {return false; }
   virtual size_t getStructSize(); 

protected:
   const char *_name;
   };


class TypeDictionary
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   TypeDictionary();
   ~TypeDictionary() throw();

   TR::IlType * LookupStruct(const char *structName);
   TR::IlType * DefineStruct(const char *structName);
   void DefineField(const char *structName, const char *fieldName, TR::IlType *type);
   void CloseStruct(const char *structName);
   TR::IlType * GetFieldType(const char *structName, const char *fieldName);

   TR::IlType *PrimitiveType(TR::DataType primitiveType)
      {
      return _primitiveType[primitiveType];
      }

   //TR::IlType *ArrayOf(TR::IlType *baseType);

   TR::IlType *PointerTo(TR::IlType *baseType);
   TR::IlType *PointerTo(const char *structName);
   TR::IlType *PointerTo(TR::DataType baseType)  { return PointerTo(_primitiveType[baseType]); }

   TR::IlReference *FieldReference(const char *structName, const char *fieldName);
   TR_Memory *trMemory() { return _trMemory; }

   //TR::IlReference *ArrayReference(TR::IlType *arrayType);

protected:
   TR::SegmentProvider *_segmentProvider;
   TR::Region *_memoryRegion;
   TR_Memory *_trMemory;
   TR_HashTabString * _structsByName;

   // convenience for primitive types
   TR::IlType       * _primitiveType[TR::NumOMRTypes];
   TR::IlType       * NoType;
   TR::IlType       * Int8;
   TR::IlType       * Int16;
   TR::IlType       * Int32;
   TR::IlType       * Int64;
   TR::IlType       * Word;
   TR::IlType       * Float;
   TR::IlType       * Double;
   TR::IlType       * Address;
   TR::IlType       * VectorInt8;
   TR::IlType       * VectorInt16;
   TR::IlType       * VectorInt32;
   TR::IlType       * VectorInt64;
   TR::IlType       * VectorFloat;
   TR::IlType       * VectorDouble;

   TR::IlType       * _pointerToPrimitiveType[TR::NumOMRTypes];
   TR::IlType       * pInt8;
   TR::IlType       * pInt16;
   TR::IlType       * pInt32;
   TR::IlType       * pInt64;
   TR::IlType       * pWord;
   TR::IlType       * pFloat;
   TR::IlType       * pDouble;
   TR::IlType       * pAddress;
   TR::IlType       * pVectorInt8;
   TR::IlType       * pVectorInt16;
   TR::IlType       * pVectorInt32;
   TR::IlType       * pVectorInt64;
   TR::IlType       * pVectorFloat;
   TR::IlType       * pVectorDouble;
   };

} // namespace OMR


#if defined(PUT_OMR_ILTYPE_INTO_TR)
namespace TR
{
class IlType : public OMR::IlType
   {
   public:
      IlType(const char *name)
         : OMR::IlType(name)
         { }
      IlType()
         : OMR::IlType()
         { }
   };
} // namespace TR
#endif // defined(PUT_OMR_ILTYPE_INTO_TR)


#if defined(PUT_OMR_TYPEDICTIONARY_INTO_TR)
namespace TR
{
class TypeDictionary : public OMR::TypeDictionary
   {
   public:
      TypeDictionary()
         : OMR::TypeDictionary()
         { }
   };
} // namespace TR
#endif // defined(PUT_OMR_TYPEDICTIONARY_INTO_TR)

#endif // !defined(OMR_TYPEDICTIONARY_INCL)

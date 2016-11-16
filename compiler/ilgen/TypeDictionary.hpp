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
   virtual size_t getSize();

protected:
   const char *_name;
   };


/**
 * @brief Convenience API for defining JitBuilder structs from C/C++ structs (PODs)
 * 
 * These macros simply allow the name of C/C++ structs and fields to be used to
 * define JitBuilder structs. This can help ensure a consistent API between a
 * VM struct and JitBuilder's representation of the same struct. Their definitions
 * expand to calls to `TR::TypeDictionary` methods that create a representation
 * corresponding to that specified type.
 * 
 * ## Usage
 * 
 * Given the following struct:
 * 
 * ```c++
 * struct MyStruct {
 *    int32_t id;
 *    int32_t* val;
 * };
 * 
 * a JitBuilder representation of this struct can be defined as follows:
 * 
 * ```c++
 * TR::TypeDictionary d;
 * d.DEFINE_STRUCT(MyStruct);
 * d.DEFINE_FIELD(MyStruct, id, Int32);
 * d.DEFINE_FIELD(MyStruct, val, pInt32);
 * d.CLOSE_STRUCT(MyStruct);
 * ```
 * 
 * This definition will expand to the following:
 * 
 * ```c++
 * TR::TypeDictionary d;
 * d.DefineStruct("MyStruct");
 * d.DefineField("MyStruct", "id", Int32, offsetof(MyStruct, id));
 * d.DefineField("MyStruct", "val", pInt32, offsetof(MyStruct, val));
 * d.CloseStruct("MyStruct", sizeof(MyStruct));
 * ```
 */
#define DEFINE_STRUCT(structName) \
   DefineStruct(#structName)
#define DEFINE_FIELD(structName, fieldName, filedIlType) \
   DefineField(#structName, #fieldName, filedIlType, offsetof(structName, fieldName))
#define CLOSE_STRUCT(structName) \
   CloseStruct(#structName, sizeof(structName))


class TypeDictionary
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   TypeDictionary();
   ~TypeDictionary() throw();

   TR::IlType * LookupStruct(const char *structName);

   /**
    * @brief Begin definition of a new structure type
    * @param structName the name of the new type
    * @return pointer to IlType instance of the new type being defined
    * 
    * The name of the new type will have to be used when specifying
    * fields of the type. This method must be invoked once before any
    * calls to `DefineField()` and `CloseStruct()`.
    */
   TR::IlType * DefineStruct(const char *structName);

   /**
    * @brief Define a member of a new structure type
    * @param structName the name of the struct type on which to define the field
    * @param fieldName the name of the field
    * @param type the IlType instance representing the type of the field
    * @param offset the offset of the field within the structure (in bytes)
    * 
    * Fields defined using this method must be defined in offset order.
    * Specifically, the `offset` on any call to this method must be greater
    * than or equal to the size of the new struct at the time of the call
    * (`getSize() <= offset`). Failure to meet this condition will result
    * in a runtime failure. This was done as an initial attempt to prevent
    * struct fields from overlapping in memory.
    * 
    * This method can only be called after a call to `DefineStruct` and
    * before a call to `CloseStruct` with the same `structName`.
    */
   void DefineField(const char *structName, const char *fieldName, TR::IlType *type, size_t offset);

   /**
    * @brief Define a member of a new structure type
    * @param structName the name of the struct type on which to define the field
    * @param fieldName the name of the field
    * @param type the IlType instance representing the type of the field
    * 
    * This is an overloaded method. Since no offset for the new struct field is
    * specified, it will be added to the end of the struct using alignment rules
    * internally defined by JitBuilder. These are not guaranteed match the rules
    * used by a C/C++ compiler as alignment rules are compiler specific. However,
    * the alignment should be the same in most cases.
    * 
    * This method can only be called after a call to `DefineStruct` and
    * before a call to `CloseStruct` with the same `structName`.
    */
   void DefineField(const char *structName, const char *fieldName, TR::IlType *type);

   /**
    * @brief End definition of a new structure type
    * @param structName the name of the new type of which the definition is ended
    * @param finalSize the final size (in bytes) of the type
    * 
    * The `finalSize` of the struct must be greater than or equal to the size of
    * the new struct at the time of the call (`getSize() <= finalSize`). If
    * `finalSize` is greater, the size of the struct will be adjusted. Failure to
    * meet this condition will result in a runtime failure. This was done as
    * done as an initial attempt to help ensure that adequate padding is added to
    * the end of the new struct for use in arrays and nested structs.
    */
   void CloseStruct(const char *structName, size_t finalSize);

   /**
    * @brief End definition of a new structure type
    * @param structName the name of the new type of which the definition is ended
    * 
    * This is an overloaded method. Since the final size of the struct is not
    * specified, the size of the new struct at the time of call to this method
    * will be the final size of the new struct type.
    */
   void CloseStruct(const char *structName);

   TR::IlType * GetFieldType(const char *structName, const char *fieldName);

   /**
    * @brief Returns the offset of a field in a struct
    * @param structName the name of the struct containing the field
    * @param fieldName the name of the field in the struct
    * @return the memory offset of the field in bytes
    */
   size_t OffsetOf(const char *structName, const char *fieldName);

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

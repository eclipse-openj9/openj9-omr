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


#include <stdlib.h>

#include "il/DataTypes.hpp"
#include "il/SymbolReference.hpp"
#include "compile/SymbolReferenceTable.hpp"
#include "compile/Compilation.hpp"
#include "env/FrontEnd.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "env/Region.hpp"
#include "env/SystemSegmentProvider.hpp"
#include "env/TRMemory.hpp"


namespace OMR
{

static const char *signatureNameForType[] =
   {
   "V",  // NoType
   "B",  // Int8
   "C",  // Int16
   "I",  // Int32
   "J",  // Int64
   "F",  // Float
   "D",  // Double
   "L",  // Address
   // TODO: vector types!
   };

char *
IlType::getSignatureName()
   {
   TR::DataTypes dt = getPrimitiveType();
   if (dt == TR::Address)
      return (char *)_name;
   return (char *) signatureNameForType[dt];
   }

size_t
IlType::getStructSize()
   {
   TR_ASSERT(0, "The input type is not a StructType\n");
   return 0;
   }

const uint8_t primitiveTypeAlignment[TR::NumOMRTypes] =
   {
   1,
   1,
   2,
   4,
   8,
#if TR_TARGET_64BIT // HOST?
   4,
#else
   8,
#endif
   16,
   16,
   16,
   16,
   16,
   16
   };


class PrimitiveType : public TR::IlType
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   PrimitiveType(const char * name, TR::DataTypes type) :
      TR::IlType(name),
      _type(type)
      { }

   virtual TR::DataTypes getPrimitiveType()
      {
      return _type;
      }

  virtual char *getSignatureName() { return (char *) signatureNameForType[_type]; }

protected:
   TR::DataTypes _type;
   };


class FieldInfo
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   FieldInfo(const char *name, uint32_t offset, TR::IlType *type) :
      _next(0),
      _name(name),
      _offset(offset),
      _type(type),
      _symRef(0)
      {
      }

   void cacheSymRef(TR::SymbolReference *symRef) { _symRef = symRef; }
   TR::SymbolReference *getSymRef()              { return _symRef; }

   TR::IlType *getType()                         { return _type; }

   TR::DataTypes getPrimitiveType()             { return _type->getPrimitiveType(); }

   uint32_t getOffset()                          { return _offset; }

   FieldInfo *getNext()                          { return _next; }
   void setNext(FieldInfo *next)                 { _next = next; }

//private:
   FieldInfo           * _next;
   const char          * _name;
   uint32_t              _offset;
   TR::IlType          * _type;
   TR::SymbolReference * _symRef;
   };


class StructType : public TR::IlType
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   StructType(const char *name) :
      TR::IlType(name),
      _firstField(0),
      _lastField(0),
      _size(0),
      _closed(false)
      { }

   TR::DataTypes getPrimitiveType()                 { return TR::Address; }
   void Close()                                      { _closed = true; };

   void AddField(const char *name, TR::IlType *fieldType);
   TR::IlType * getFieldType(const char *fieldName);

   TR::SymbolReference *getFieldSymRef(const char *name);
   bool isStruct() { return true; }
   size_t getStructSize();

protected:
   FieldInfo * findField(const char *fieldName);

   FieldInfo * _firstField;
   FieldInfo * _lastField;
   uint32_t    _size;
   bool        _closed;
   };

void
StructType::AddField(const char *name, TR::IlType *typeInfo)
   {
   if (_closed)
      return;

   TR::DataTypes primitiveType = typeInfo->getPrimitiveType();
   uint32_t align = primitiveTypeAlignment[primitiveType] - 1;
   _size = (_size + align) & (~align);

   FieldInfo *fieldInfo = new (PERSISTENT_NEW) FieldInfo(name, _size, typeInfo);
   if (0 != _lastField)
      _lastField->setNext(fieldInfo);
   else
      _firstField = fieldInfo;
   _lastField = fieldInfo;
   _size += TR::DataType::getSize(primitiveType);
   }

FieldInfo *
StructType::findField(const char *fieldName)
   {
   FieldInfo *info = _firstField;
   while (NULL != info)
      {
      if (strncmp(info->_name, fieldName, strlen(fieldName)) == 0)
         return info;
      info = info->_next;
      }
   return NULL;
   }

TR::IlType *
StructType::getFieldType(const char *fieldName)
   {
   FieldInfo *info = findField(fieldName);
   if (NULL == info)
      return NULL;
   return info->_type;
   }

TR::IlReference *
StructType::getFieldSymRef(const char *fieldName)
   {
   FieldInfo *info = findField(fieldName);
   if (NULL == info)
      return NULL;

   TR::SymbolReference *symRef = info->getSymRef();
   if (NULL == symRef)
      {
      TR::Compilation *comp = TR::comp();

      TR::DataTypes type = info->getPrimitiveType();

      TR::Symbol *symbol = NULL;
      if (TR::Int32 == type)
         symbol = comp->getSymRefTab()->findOrCreateGenericIntShadowSymbol();
      else
         symbol = TR::Symbol::createShadow(comp->trHeapMemory(), type);

      // TBD: should we create a dynamic "constant" pool for accesses made by the method being compiled?
      symRef = new (comp->trHeapMemory()) TR::SymbolReference(comp->getSymRefTab(), symbol, comp->getMethodSymbol()->getResolvedMethodIndex(), -1);
      symRef->setOffset(info->getOffset());

      // conservative aliasing
      int32_t refNum = symRef->getReferenceNumber();
      if (type == TR::Address)
         comp->getSymRefTab()->aliasBuilder.addressShadowSymRefs().set(refNum);
      else if (type == TR::Int32)
         comp->getSymRefTab()->aliasBuilder.intShadowSymRefs().set(refNum);
      else
         comp->getSymRefTab()->aliasBuilder.nonIntPrimitiveShadowSymRefs().set(refNum);

      info->cacheSymRef(symRef);
      }

   return (TR::IlReference *)symRef;
   }

size_t
StructType::getStructSize()
   {
   size_t size = 0;
   FieldInfo *currentField = _firstField;
   while (currentField != NULL)
      {
      size += TR::DataType::getSize(currentField->getPrimitiveType());
      currentField = currentField->getNext();
      }
   return size;
   }

class PointerType : public TR::IlType
   {
public:
   TR_ALLOC(TR_Memory::IlGenerator)

   PointerType(TR::IlType *baseType) :
      TR::IlType(_nameArray),
      _baseType(baseType),
      _symRef(0)
      {
      char *baseName = (char *)_baseType->getName();
      TR_ASSERT(strlen(baseName) < 45, "cannot store name of pointer type");
      sprintf(_nameArray, "L%s;", baseName);
      }
   virtual bool isPointer() { return true; }

   virtual TR::IlType *baseType() { return _baseType; }

   virtual const char *getName() { return _name; }

   virtual TR::DataTypes getPrimitiveType() { return TR::Address; }

   TR::SymbolReference *getSymRef();

protected:
   TR::IlType          * _baseType;
   TR::SymbolReference * _symRef;
   char                  _nameArray[48];
   };


TypeDictionary::TypeDictionary() :
   _segmentProvider( static_cast<TR::SegmentProvider *>(new(TR::Compiler->persistentAllocator()) TR::SystemSegmentProvider(1 << 16, TR::Compiler->rawAllocator)) ),
   _memoryRegion( new(TR::Compiler->persistentAllocator()) TR::Region(*_segmentProvider, TR::Compiler->rawAllocator) ),
   _trMemory( new(TR::Compiler->persistentAllocator()) TR_Memory(*::trPersistentMemory, *_memoryRegion) )
   {
   _structsByName = new (PERSISTENT_NEW) TR_HashTabString(trMemory());

   // primitive types
   NoType       = _primitiveType[TR::NoType]                = new (PERSISTENT_NEW) OMR::PrimitiveType("NoType", TR::NoType);
   Int8         = _primitiveType[TR::Int8]                  = new (PERSISTENT_NEW) OMR::PrimitiveType("Int8", TR::Int8);
   Int16        = _primitiveType[TR::Int16]                 = new (PERSISTENT_NEW) OMR::PrimitiveType("Int16", TR::Int16);
   Int32        = _primitiveType[TR::Int32]                 = new (PERSISTENT_NEW) OMR::PrimitiveType("Int32", TR::Int32);
   Int64        = _primitiveType[TR::Int64]                 = new (PERSISTENT_NEW) OMR::PrimitiveType("Int64", TR::Int64);
   Float        = _primitiveType[TR::Float]                 = new (PERSISTENT_NEW) OMR::PrimitiveType("Float", TR::Float);
   Double       = _primitiveType[TR::Double]                = new (PERSISTENT_NEW) OMR::PrimitiveType("Double", TR::Double);
   Address      = _primitiveType[TR::Address]               = new (PERSISTENT_NEW) OMR::PrimitiveType("Address", TR::Address);
   VectorInt8   = _primitiveType[TR::VectorInt8]            = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorInt8", TR::VectorInt8);
   VectorInt16  = _primitiveType[TR::VectorInt16]           = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorInt16", TR::VectorInt16);
   VectorInt32  = _primitiveType[TR::VectorInt32]           = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorInt32", TR::VectorInt32);
   VectorInt64  = _primitiveType[TR::VectorInt64]           = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorInt64", TR::VectorInt64);
   VectorFloat  = _primitiveType[TR::VectorFloat]           = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorFloat", TR::VectorFloat);
   VectorDouble = _primitiveType[TR::VectorDouble]          = new (PERSISTENT_NEW) OMR::PrimitiveType("VectorDouble", TR::VectorDouble);

   // pointer to primitive types
                   _pointerToPrimitiveType[TR::NoType]       = NULL;
   pInt8         = _pointerToPrimitiveType[TR::Int8]         = new (PERSISTENT_NEW) PointerType(Int8);
   pInt16        = _pointerToPrimitiveType[TR::Int16]        = new (PERSISTENT_NEW) PointerType(Int16);
   pInt32        = _pointerToPrimitiveType[TR::Int32]        = new (PERSISTENT_NEW) PointerType(Int32);
   pInt64        = _pointerToPrimitiveType[TR::Int64]        = new (PERSISTENT_NEW) PointerType(Int64);
   pFloat        = _pointerToPrimitiveType[TR::Float]        = new (PERSISTENT_NEW) PointerType(Float);
   pDouble       = _pointerToPrimitiveType[TR::Double]       = new (PERSISTENT_NEW) PointerType(Double);
   pAddress      = _pointerToPrimitiveType[TR::Address]      = new (PERSISTENT_NEW) PointerType(Address);
   pVectorInt8   = _pointerToPrimitiveType[TR::VectorInt8]   = new (PERSISTENT_NEW) PointerType(VectorInt8);
   pVectorInt16  = _pointerToPrimitiveType[TR::VectorInt16]  = new (PERSISTENT_NEW) PointerType(VectorInt16);
   pVectorInt32  = _pointerToPrimitiveType[TR::VectorInt32]  = new (PERSISTENT_NEW) PointerType(VectorInt32);
   pVectorInt64  = _pointerToPrimitiveType[TR::VectorInt64]  = new (PERSISTENT_NEW) PointerType(VectorInt64);
   pVectorFloat  = _pointerToPrimitiveType[TR::VectorFloat]  = new (PERSISTENT_NEW) PointerType(VectorFloat);
   pVectorDouble = _pointerToPrimitiveType[TR::VectorDouble] = new (PERSISTENT_NEW) PointerType(VectorDouble);

   if (TR::Compiler->target.is64Bit())
      {
      Word =  _primitiveType[TR::Int64]; 
      pWord =  _pointerToPrimitiveType[TR::Int64];
      }
   else
      {
      Word =  _primitiveType[TR::Int32];
      pWord =  _pointerToPrimitiveType[TR::Int32];
      }
   }

TypeDictionary::~TypeDictionary() throw()
   {
   _trMemory->~TR_Memory();
   ::operator delete(_trMemory, TR::Compiler->persistentAllocator());
   _memoryRegion->~Region();
   ::operator delete(_memoryRegion, TR::Compiler->persistentAllocator());
   static_cast<TR::SystemSegmentProvider *>(_segmentProvider)->~SystemSegmentProvider();
   ::operator delete(_segmentProvider, TR::Compiler->persistentAllocator());
   }

TR::IlType *
TypeDictionary::LookupStruct(const char *structName)
   {
   TR_HashId structID = 0;
   if (_structsByName->locate(structName, structID))
      return (TR::IlType *) _structsByName->getData(structID);
   return NULL; 
   }

TR::IlType *
TypeDictionary::DefineStruct(const char *structName)
   {
   TR_HashId structID=0;
   _structsByName->locate(structName, structID);

   StructType *newType = new (PERSISTENT_NEW) StructType(structName);
   _structsByName->add(structName, structID, (void *) newType);

   return (TR::IlType *) newType;
   }

void
TypeDictionary::DefineField(const char *structName, const char *fieldName, TR::IlType *type)
   {
   TR_HashId structID=0;
   if (_structsByName->locate(structName, structID))
      {
      StructType *structType = (StructType *) _structsByName->getData(structID);
      structType->AddField(fieldName, type);
      }
   }

TR::IlType *
TypeDictionary::GetFieldType(const char *structName, const char *fieldName)
   {
   TR_HashId structID = 0;
   _structsByName->locate(structName, structID);
   StructType *theStruct = (StructType *) _structsByName->getData(structID);
   return theStruct->getFieldType(fieldName);
   }

void
TypeDictionary::CloseStruct(const char *structName)
   {
   TR_HashId structID = 0;
   _structsByName->locate(structName, structID);
   StructType *theStruct = (StructType *) _structsByName->getData(structID);
   theStruct->Close();
   }

TR::IlType *
TypeDictionary::PointerTo(const char *structName)
   {
   TR_HashId structID = 0;
   _structsByName->locate(structName, structID);
   StructType *theStruct = (StructType *) _structsByName->getData(structID);
   return PointerTo(theStruct);
   }

TR::IlType *
TypeDictionary::PointerTo(TR::IlType *baseType)
   {
   return new (PERSISTENT_NEW) PointerType(baseType);
   }

TR::IlReference *
TypeDictionary::FieldReference(const char *structName, const char *fieldName)
   {
   TR_HashId structID = 0;
   _structsByName->locate(structName, structID);
   StructType *theStruct = (StructType *) _structsByName->getData(structID);
   return theStruct->getFieldSymRef(fieldName);
   }
} // namespace OMR

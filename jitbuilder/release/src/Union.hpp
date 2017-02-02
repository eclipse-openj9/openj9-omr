/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2017
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

#ifndef UNION_INCL
#define UNION_INCL

#include "ilgen/MethodBuilder.hpp"
#include "ilgen/TypeDictionary.hpp"

namespace TR { class TypeDictionary; }

union TestUnionInt8pChar
   {
   uint8_t v_uint8;
   const char* v_pchar;
   };

union TestUnionInt32Int32
   {
   uint32_t f1;
   uint32_t f2;
   };

union TestUnionInt16Double
   {
   uint16_t v_uint16;
   double v_double;
   };

typedef void (*SetUnionByteFunction)(TestUnionInt8pChar*, uint8_t);
typedef uint8_t (*GetUnionByteFunction)(TestUnionInt8pChar*);
typedef void (*SetUnionStrFunction)(TestUnionInt8pChar*, const char*);
typedef const char* (*GetUnionStrFunction)(TestUnionInt8pChar*);
typedef uint32_t (*TypePunInt32Int32Function)(TestUnionInt32Int32*, uint32_t, uint32_t);
typedef uint16_t (*TypePunInt16DoubleFunction)(TestUnionInt16Double*, uint16_t, double);

class UnionTypeDictionary : public TR::TypeDictionary
   {
   public:
   UnionTypeDictionary() :
      TR::TypeDictionary()
      {
      DefineUnion("TestUnionInt8pChar");
      UnionField("TestUnionInt8pChar", "v_uint8", toIlType<uint8_t>());
      UnionField("TestUnionInt8pChar", "v_pchar", toIlType<char*>());
      CloseUnion("TestUnionInt8pChar");

      DefineUnion("TestUnionInt32Int32");
      UnionField("TestUnionInt32Int32", "f1", Int32);
      UnionField("TestUnionInt32Int32", "f2", Int32);
      CloseUnion("TestUnionInt32Int32");

      DefineUnion("TestUnionInt16Double");
      UnionField("TestUnionInt16Double", "v_uint16", Int16);
      UnionField("TestUnionInt16Double", "v_double", Double);
      CloseUnion("TestUnionInt16Double");
      }
   };

class SetUnionByteBuilder : public TR::MethodBuilder
   {
   public:
   SetUnionByteBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class GetUnionByteBuilder : public TR::MethodBuilder
   {
   public:
   GetUnionByteBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class SetUnionStrBuilder : public TR::MethodBuilder
   {
   public:
   SetUnionStrBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class GetUnionStrBuilder : public TR::MethodBuilder
   {
   public:
   GetUnionStrBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class TypePunInt32Int32Builder : public TR::MethodBuilder
   {
   public:
   TypePunInt32Int32Builder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

class TypePunInt16DoubleBuilder : public TR::MethodBuilder
   {
   public:
   TypePunInt16DoubleBuilder(TR::TypeDictionary *);
   virtual bool buildIL();
   };

#endif // UNION_INCL

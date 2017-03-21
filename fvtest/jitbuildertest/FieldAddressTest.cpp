/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#include "JBTestUtil.hpp"

struct Struct
   {
   uint16_t f1;
   uint8_t f2;
   };

union Union
   {
   uint16_t f1;
   uint8_t f2;
   };

typedef uint8_t* (GetStructFieldAddressFunction)(Struct*);
typedef uint8_t* (GetUnionFieldAddressFunction)(Union*);

DEFINE_BUILDER( GetStructFieldAddressBuilder,
                PointerTo(GetFieldType("Struct", "f2")),
                PARAM("s", PointerTo(LookupStruct("Struct"))) )
   {
   Return(
          StructFieldInstanceAddress("Struct", "f2",
                             Load("s")));

   return true;
   }

DEFINE_BUILDER( GetUnionFieldAddressBuilder,
                toIlType<uint8_t *>(),
                PARAM("u", PointerTo(LookupUnion("Union"))) )
   {
   Return(
          UnionFieldInstanceAddress("Union", "f2",
                             Load("u")));

   return true;
   }

DEFINE_TYPES(StructTypeDictionary)
   {
   DEFINE_STRUCT(Struct);
   DEFINE_FIELD(Struct, f1, toIlType<uint16_t>());
   DEFINE_FIELD(Struct, f2, toIlType<uint8_t>());
   CLOSE_STRUCT(Struct);
   }

DEFINE_TYPES(UnionTypeDictionary)
   {
   DefineUnion("Union");
   UnionField("Union", "f1", toIlType<uint16_t>());
   UnionField("Union", "f2", toIlType<uint8_t>());
   CloseUnion("Union");
   }

class FieldAddressTest : public JitBuilderTest {};

TEST_F(FieldAddressTest, StructField)
   {
   GetStructFieldAddressFunction *getStructFieldAddress;
   ASSERT_COMPILE(StructTypeDictionary,GetStructFieldAddressBuilder,getStructFieldAddress);

   Struct s;
   s.f1 = 1;
   s.f2 = 2;
   auto structFieldAddress = getStructFieldAddress(&s);
   ASSERT_EQ(&(s.f2), structFieldAddress);
   }

TEST_F(FieldAddressTest, UnionField)
   {
   GetUnionFieldAddressFunction *getUnionFieldAddress;
   ASSERT_COMPILE(UnionTypeDictionary, GetUnionFieldAddressBuilder, getUnionFieldAddress);

   Union u;
   u.f2 = 2;
   auto unionFieldAddress = getUnionFieldAddress(&u);
   ASSERT_EQ(&(u.f2), unionFieldAddress);
   }

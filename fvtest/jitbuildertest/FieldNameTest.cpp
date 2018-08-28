/*******************************************************************************
 * Copyright (c) 2017, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "JBTestUtil.hpp"

struct MyStruct
   {
   uint8_t field;
   uint16_t field11;
   uint32_t field1;
   uint64_t f;
   };

union Union
   {
   uint8_t field;
   uint16_t field11;
   uint32_t field1;
   uint64_t f;
   };

typedef void (TestStructFieldNameFunction)();
typedef void (TestUnionFieldNameFunction)();

DEFINE_BUILDER( TestStructFieldNameBuilder,
                NoType )
   {
   EXPECT_EQ(toIlType<uint8_t>(), GetFieldType("MyStruct", "field")) << "Expected " << toIlType<uint8_t>()->getName() << " and " << GetFieldType("MyStruct", "field")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint16_t>(), GetFieldType("MyStruct", "field11")) << "Expected " << toIlType<uint16_t>()->getName() << " and " << GetFieldType("MyStruct", "field11")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint32_t>(), GetFieldType("MyStruct", "field1")) << "Expected " << toIlType<uint32_t>()->getName() << " and " << GetFieldType("MyStruct", "field1")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint64_t>(), GetFieldType("MyStruct", "f")) << "Expected " << toIlType<uint64_t>()->getName() << " and " << GetFieldType("MyStruct", "f")->getName() << " was returned";

   Return();

   return true;
   }

DEFINE_BUILDER( TestUnionFieldNameBuilder,
                NoType )
   {
   EXPECT_EQ(toIlType<uint8_t>(), UnionFieldType("MyUnion", "field")) << "Expected " << toIlType<uint8_t>()->getName() << " and " << UnionFieldType("MyUnion", "field")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint16_t>(), UnionFieldType("MyUnion", "field11")) << "Expected " << toIlType<uint16_t>()->getName() << " and " << UnionFieldType("MyUnion", "field11")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint32_t>(), UnionFieldType("MyUnion", "field1")) << "Expected " << toIlType<uint32_t>()->getName() << " and " << UnionFieldType("MyUnion", "field1")->getName() << " was returned";
   EXPECT_EQ(toIlType<uint64_t>(), UnionFieldType("MyUnion", "f")) << "Expected " << toIlType<uint64_t>()->getName() << " and " << UnionFieldType("MyUnion", "f")->getName() << " was returned";

   Return();

   return true;
   }

DEFINE_TYPES(MyStructTypeDictionary)
   {
   DefineStruct("MyStruct");
   DefineField("MyStruct", "field", toIlType<uint8_t>());
   DefineField("MyStruct", "field11", toIlType<uint16_t>());
   DefineField("MyStruct", "field1", toIlType<uint32_t>());
   DefineField("MyStruct", "f", toIlType<uint64_t>());
   CloseStruct("MyStruct");
   }

DEFINE_TYPES(MyUnionTypeDictionary)
   {
   DefineUnion("MyUnion");
   UnionField("MyUnion", "field", toIlType<uint8_t>());
   UnionField("MyUnion", "field11", toIlType<uint16_t>());
   UnionField("MyUnion", "field1", toIlType<uint32_t>());
   UnionField("MyUnion", "f", toIlType<uint64_t>());
   CloseUnion("MyUnion");
   }

class FieldNameTest : public JitBuilderTest {};

TEST_F(FieldNameTest, StructField)
   {
   TestStructFieldNameFunction *testStructNameAddress;
   ASSERT_COMPILE(MyStructTypeDictionary,TestStructFieldNameBuilder,testStructNameAddress);

   testStructNameAddress();
   }

TEST_F(FieldNameTest, UnionField)
   {
   TestUnionFieldNameFunction *testUnionNameAddress;
   ASSERT_COMPILE(MyUnionTypeDictionary,TestUnionFieldNameBuilder,testUnionNameAddress);

   testUnionNameAddress();
   }


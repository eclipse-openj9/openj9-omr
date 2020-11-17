/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
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

#include "tests/CCDataTest.hpp"

/**
 * Compile testing
 */

#include "compile/SymbolReferenceTable.hpp"
#include "ilgen/IlInjector.hpp"
#include "ilgen/MethodInfo.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "il/Block.hpp"
#include "il/Node.hpp"
#include "infra/ILWalk.hpp"
#include "ras/IlVerifier.hpp"
#include "OptTestDriver.hpp"

using namespace TR;
using namespace TestCompiler;

class TableInjector : public TR::IlInjector
   {
   public:

   TR_ALLOC(TR_Memory::IlGenerator)

   TableInjector(TR::TypeDictionary *types, TestDriver *test, const int32_t * const caseValues, const size_t numCaseValues)
   : _caseValues(caseValues), _numCaseValues(numCaseValues), TR::IlInjector(types, test)
      {
      }

   bool injectIL()
      {
      createBlocks(2 + _numCaseValues);

      generateToBlock(0);
      auto defaultCase = TR::Node::createCase(NULL, block(1)->getEntry());
      auto table = TR::Node::create(TR::table, 2 + _numCaseValues, parameter(0, Int32), defaultCase);
      for (auto i = 0; i < _numCaseValues; ++i)
         table->setAndIncChild(2 + i, TR::Node::createCase(NULL, block(2 + i)->getEntry()));
      genTreeTop(table);
      cfg()->addEdge(_currentBlock, block(1));
      for (auto i = 0; i < _numCaseValues; ++i)
         cfg()->addEdge(_currentBlock, block(2 + i));

      generateToBlock(1);
      returnValue(parameter(1, Int32));

      for (auto i = 0; i < _numCaseValues; ++i)
         {
         generateToBlock(2 + i);
         returnValue(iconst(_caseValues[i]));
         }

      return true;
      }

   private:
      const int32_t  *_caseValues;
      const size_t   _numCaseValues;
   };

class GeneratedMethodInfo : public MethodInfo
   {
   public:
      typedef int32_t (*compiled_method_t)(int32_t, int32_t);

      GeneratedMethodInfo(TestDriver *test, const int32_t * const caseValues, const size_t numCaseValues) : _ilInjector(&_types, test, caseValues, numCaseValues)
         {
         TR::IlType *int32 = _types.PrimitiveType(TR::Int32);
         _args[0] = {int32};
         _args[1] = {int32};
         DefineFunction(__FILE__, LINETOSTR(__LINE__), "TableTest", sizeof(_args) / sizeof(_args[0]), _args, int32);
         DefineILInjector(&_ilInjector);
         }
   private:
      TR::TypeDictionary   _types;
      TR::IlType           *_args[2];
      TableInjector        _ilInjector;
   };

class TableTest : public OptTestDriver
   {
   public:
      TableTest() : _caseValues(NULL), _numCaseValues(0) {}
      void setCaseValues(const int32_t * const caseValues, const size_t numCaseValues)
         {
         _caseValues = caseValues;
         _numCaseValues = numCaseValues;
         }
      virtual void invokeTests() override
         {
         EXPECT_TRUE(_caseValues != NULL);
         EXPECT_GT(_numCaseValues, 0);
         auto compiledMethod = getCompiledMethod<GeneratedMethodInfo::compiled_method_t>();
         for (auto i = 0; i < _numCaseValues; ++i)
            EXPECT_EQ(compiledMethod(i, -1), _caseValues[i]);
         EXPECT_EQ(compiledMethod(_numCaseValues, -1), -1);
         }
      private:
         const int32_t  *_caseValues;
         size_t         _numCaseValues;
   };

class TableVerifier : public TR::IlVerifier
   {
   public:
   TableVerifier(const size_t numCaseValues) : _numCaseValues(numCaseValues) {}
   bool verifyNode(TR::Node * const node) const
      {
      return node->getOpCodeValue() == TR::table
             && node->getNumChildren() == 2 + _numCaseValues;
      }
   virtual int32_t verify(TR::ResolvedMethodSymbol *sym) override
      {
      for (TR::PreorderNodeIterator iter(sym->getFirstTreeTop(), sym->comp()); iter.currentTree(); ++iter)
         {
         if (verifyNode(iter.currentNode()))
            return 0;
         }
      return 1;
      }
   private:
      const size_t _numCaseValues;
   };

TEST_F(TableTest, test_table)
   {
   const int32_t caseValues[] = {8, 6, 7, 5, 3, 0, 9};
   const size_t numCaseValues = sizeof(caseValues) / sizeof(caseValues[0]);
   this->setCaseValues(caseValues, numCaseValues);
   GeneratedMethodInfo info(this, caseValues, numCaseValues);
   setMethodInfo(&info);
   TableVerifier ilVerifier(numCaseValues);
   setIlVerifier(&ilVerifier);
   VerifyAndInvoke();
   }

/**
 * Unit testing
 */

#include "codegen/CCData.hpp"
#include "codegen/CCData_inlines.hpp"

using TR::CCData;

const CCData::index_t INVALID_INDEX = std::numeric_limits<CCData::index_t>::max();

template <typename T>
class CCDataTest : public testing::Test
   {
   };

struct odd_sized_t
   {
   odd_sized_t() {}
   odd_sized_t(const uint8_t x) { set_em_all(x); }
   odd_sized_t& operator=(const uint8_t x) { set_em_all(x); return *this; }
   odd_sized_t operator-() const { return odd_sized_t(-_data[0]); }
   bool operator==(const odd_sized_t &other) const { return _data[0] == other._data[0]; };
   private:
      void set_em_all(const uint8_t x)
         {
         for (auto i = 0; i < sizeof(_data) / sizeof(_data[0]); ++i)
            _data[i] = x;
         }
      uint8_t _data[7];
   };

using CCDataTestTypes = testing::Types<uint8_t, uint16_t, uint32_t, uint64_t,
                                       int8_t, int16_t, int32_t, int64_t, odd_sized_t
                                       /* Floats and doubles don't have unique object representations.
                                       See the documentation for CCData::key().
                                       The templated tests below would need to be re-written to create keys
                                       differently for at least floats and doubles.*/
                                       //,float, double
                                      >;

TYPED_TEST_CASE(CCDataTest, CCDataTestTypes);

TYPED_TEST(CCDataTest, test_basics_templated)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   const TypeParam      data = 99;
   // Generate a key from the data being stored.
   const CCData::key_t  key = CCData::key(data);

   // Nothing should be found by this key yet.
   EXPECT_FALSE(table.find(key));

   CCData::index_t index = INVALID_INDEX;

   // Put the data in the table, associate it with the key, retrieve the index.
   EXPECT_TRUE(table.put(data, &key, index));
   // Make sure the index was written.
   EXPECT_NE(index, INVALID_INDEX);

   const TypeParam * const dataPtr = table.get<TypeParam>(index);

   // Make sure the data was written.
   EXPECT_TRUE(dataPtr != NULL);
   // Make sure it was written to an aligned address.
   EXPECT_EQ(reinterpret_cast<size_t>(dataPtr) & (OMR_ALIGNOF(data) - 1), 0);
   // Make sure it was written correctly.
   EXPECT_EQ(*dataPtr, data);
   // We should be able to find something with this key now.
   EXPECT_TRUE(table.find(CCData::key(data)));

   TypeParam out_data = -data;

   // Retrieve the data via the index.
   EXPECT_TRUE(table.get(index, out_data));
   // Make sure it matches what was stored.
   EXPECT_EQ(data, out_data);
   // Make sure both copies generate equal keys.
   EXPECT_EQ(CCData::key(data), CCData::key(out_data));

   CCData::index_t find_index = INVALID_INDEX;

   // Find the index via the key.
   EXPECT_TRUE(table.find(CCData::key(data), &find_index));
   // Make sure it's the same index.
   EXPECT_EQ(index, find_index);

   // Make sure we can fill the table.
   while (table.put(data, NULL, index));
   }

TYPED_TEST(CCDataTest, test_arbitrary_data_templated)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   const TypeParam      d = 99;
   const TypeParam      data[3] = {d, d, d};
   // Generate a key from the data being stored.
   const CCData::key_t  key = CCData::key(&data[0], sizeof(data));

   // Nothing should be found by this key yet.
   EXPECT_FALSE(table.find(key));

   CCData::index_t index = INVALID_INDEX;

   // Put the data in the table, associate it with the key, retrieve the index.
   EXPECT_TRUE(table.put(&data[0], sizeof(data), OMR_ALIGNOF(data), &key, index));
   // Make sure the index was written.
   EXPECT_NE(index, INVALID_INDEX);

   const TypeParam * const dataPtr = table.get<TypeParam>(index);

   // Make sure the data was written.
   EXPECT_TRUE(dataPtr != NULL);
   // Make sure it was written to an aligned address.
   EXPECT_EQ(reinterpret_cast<size_t>(dataPtr) & (OMR_ALIGNOF(data) - 1), 0);
   // Make sure it was written correctly.
   EXPECT_TRUE(std::equal(data, data + sizeof(data)/sizeof(data[0]), dataPtr));
   // We should be able to find something with this key now.
   EXPECT_TRUE(table.find(CCData::key(&data[0], sizeof(data))));

   const TypeParam dminus = -d; // Negating `d` here avoids narrowing conversion warnings/errors on the next line.
   TypeParam out_data[3] = {dminus, dminus, dminus};

   // Retrieve the data via the index.
   EXPECT_TRUE(table.get(index, &out_data[0], sizeof(out_data)));
   // Make sure it matches what was stored.
   EXPECT_TRUE(std::equal(data, data + sizeof(data)/sizeof(data[0]), out_data));
   // Make sure both copies generate equal keys.
   EXPECT_EQ(CCData::key(&data[0], sizeof(data)), CCData::key(&out_data[0], sizeof(out_data)));

   CCData::index_t find_index = INVALID_INDEX;

   // Find the index via the key.
   EXPECT_TRUE(table.find(CCData::key(&data[0], sizeof(data)), &find_index));
   // Make sure it's the same index.
   EXPECT_EQ(index, find_index);

   // Make sure we can fill the table.
   while (table.put(reinterpret_cast<const uint8_t *>(&data[0]), sizeof(data), OMR_ALIGNOF(data), NULL, index));
   }

TYPED_TEST(CCDataTest, test_no_data_templated)
   {
   std::vector<char>    storage(sizeof(TypeParam), 0);
   CCData               table(&storage[0], storage.size());
   const TypeParam      data = 99;
   CCData::index_t      index = INVALID_INDEX;

   // Shouldn't be able to put.
   EXPECT_FALSE(table.put(NULL, sizeof(data), OMR_ALIGNOF(data), NULL, index));
   // Make sure the index didn't change.
   EXPECT_EQ(index, INVALID_INDEX);
   // Make sure storage wasn't written to.
   for (std::vector<char>::iterator i = storage.begin(); i != storage.end(); i++)
      {
      EXPECT_EQ(*i, 0);
      }
   }

TYPED_TEST(CCDataTest, test_no_data_reservation_templated)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   size_t               reservationSize = 32, reservationAlignment = 16;
   // Generate an arbitrary key.
   const CCData::key_t  key = CCData::key("It was the best of times, it was the worst of times.");

   // Nothing should be found by this key yet.
   EXPECT_FALSE(table.find(key));

   CCData::index_t index = INVALID_INDEX;
   CCData::index_t index2 = INVALID_INDEX;

   // Reserve space in the table, associate it with the key, retrieve the index.
   EXPECT_TRUE(table.reserve(reservationSize, reservationAlignment, &key, index));
   // Make sure the index was written.
   EXPECT_NE(index, INVALID_INDEX);
   // Try to update the value via the key.
   EXPECT_TRUE(table.reserve(reservationSize, reservationAlignment, &key, index2));
   // Make sure the index was written.
   EXPECT_EQ(index2, index);

   const TypeParam * const dataPtr = table.get<TypeParam>(index);

   // Make sure the reservation was done.
   EXPECT_TRUE(dataPtr != NULL);
   // Make sure we got an aligned address.
   EXPECT_EQ(reinterpret_cast<size_t>(dataPtr) & (reservationAlignment - 1), 0);
   // We should be able to find something with this key now.
   EXPECT_TRUE(table.find(key));

   CCData::index_t find_index = INVALID_INDEX;

   // Find the index via the key.
   EXPECT_TRUE(table.find(key, &find_index));
   // Make sure it's the same index.
   EXPECT_EQ(index, find_index);

   // Make sure we can fill the table.
   while (table.reserve(reservationSize, reservationAlignment, NULL, index));
   }

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

TYPED_TEST(CCDataTest, test_arbitrary_keys_templated)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   const TypeParam      data = 99;
   const CCData::key_t  keyFromData = CCData::key(data);
   const CCData::key_t  keyFromFileAndLine = CCData::key(__FILE__ ":" TOSTRING(__LINE__));

   // Nothing should be found by either key yet.
   EXPECT_FALSE(table.find(keyFromData));
   EXPECT_FALSE(table.find(keyFromFileAndLine));

   CCData::index_t index1 = INVALID_INDEX;
   CCData::index_t index2 = INVALID_INDEX;

   // Put the data in the table twice, associate it with each key, retrieve the indices.
   EXPECT_TRUE(table.put(data, &keyFromData, index1));
   EXPECT_TRUE(table.put(data, &keyFromFileAndLine, index2));
   // Make sure the indices were written.
   EXPECT_NE(index1, INVALID_INDEX);
   EXPECT_NE(index2, INVALID_INDEX);
   // We should be able to find something with both keys now.
   EXPECT_TRUE(table.find(CCData::key(data)));
   EXPECT_TRUE(table.find(keyFromFileAndLine));
   // The data should be in two different indices, based on key.
   EXPECT_NE(index1, index2);
   }

TYPED_TEST(CCDataTest, test_error_conditions_templated)
   {
      {
      std::vector<char> storage(sizeof(TypeParam), 0);
      CCData            zeroTable(&storage[0], 0);
      const TypeParam   data = 99;
      CCData::index_t   index = INVALID_INDEX;

      // Shouldn't be able to put.
      EXPECT_FALSE(zeroTable.put(data, NULL, index));
      // Make sure the index didn't change.
      EXPECT_EQ(index, INVALID_INDEX);
      // Make sure storage wasn't written to.
      for (std::vector<char>::iterator i = storage.begin(); i != storage.end(); i++)
         {
         EXPECT_EQ(*i, 0);
         }
      }

      {
      const size_t         smallSize = 16;
      std::vector<char>    storage(sizeof(TypeParam) * smallSize);
      CCData               smallTable(&storage[0], storage.size());
      const TypeParam      data = 99;
      CCData::index_t      lastIndex = INVALID_INDEX;
      CCData::index_t      curIndex = INVALID_INDEX;

      int count = 0;

      while (smallTable.put(data, NULL, curIndex))
         {
         // Make sure the index changed.
         EXPECT_NE(curIndex, lastIndex);
         lastIndex = curIndex;
         ++count;
         }

      // Make sure the index didn't change on the last iteration.
      EXPECT_EQ(curIndex, lastIndex);

      // Make sure we put at least some data in the table.
      EXPECT_GT(count, 0);
      }
   }

TYPED_TEST(CCDataTest, test_updating_templated)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   const TypeParam      data1 = 99;
   const TypeParam      data2 = -data1;
   const CCData::key_t  key = CCData::key("data");
   CCData::index_t      index1 = INVALID_INDEX;
   CCData::index_t      index2 = INVALID_INDEX;

   // Put the data in the table, associate it with the key, retrieve the index.
   EXPECT_TRUE(table.put(data1, &key, index1));
   // Make sure the index was written.
   EXPECT_NE(index1, INVALID_INDEX);
   // Update the value via the key.
   EXPECT_TRUE(table.put(data2, &key, index2));
   // Make sure the index was written.
   EXPECT_EQ(index2, index1);

   TypeParam * const dataPtr = table.get<TypeParam>(index1);

   // Make sure the data was written.
   EXPECT_TRUE(dataPtr != NULL);
   // Make sure it wasn't updated.
   EXPECT_EQ(*dataPtr, data1);
   // Change the value via a pointer.
   *dataPtr = data2;

   TypeParam out_data;
   // Make sure the data was written.
   EXPECT_TRUE(table.get(index1, out_data));
   // Make sure it matches.
   EXPECT_EQ(out_data, data2);
   }

TEST(AllTypesCCDataTest, test_basics)
   {
   std::vector<char>    storage(256);
   CCData               table(&storage[0], storage.size());
   const void           * const classAptr = reinterpret_cast<void *>(0x1), * const classBptr = reinterpret_cast<void *>(0x2);
   const void           * const funcAptr = reinterpret_cast<void *>(0x100), * const funcBptr = reinterpret_cast<void *>(0x200);
   const int            intA = 99, intB = 101;
   const short          shortA = 999, shortB = -999;
   const float          floatA = 10000.99f, floatB = 0.99999f;
   const double         doubleA = 1245.6789, doubleB = 3.14159;

   const CCData::key_t classAptrKey = CCData::key(classAptr);
   const CCData::key_t classBptrKey = CCData::key(classBptr);
   const CCData::key_t funcAptrKey = CCData::key(funcAptr);
   const CCData::key_t funcBptrKey = CCData::key(funcBptr);
   const CCData::key_t intAKey = CCData::key(intA);
   const CCData::key_t intBKey = CCData::key(intB);
   const CCData::key_t shortAKey = CCData::key(shortA);
   const CCData::key_t shortBKey = CCData::key(shortB);
   // Can't use float and double values for keys, see the documentation for CCData::key().
   const CCData::key_t floatAKey = CCData::key("floatA");
   const CCData::key_t floatBKey = CCData::key("floatB");
   const CCData::key_t doubleAKey = CCData::key("doubleA");
   const CCData::key_t doubleBKey = CCData::key("doubleB");

   EXPECT_FALSE(table.find(classAptrKey));
   EXPECT_FALSE(table.find(classBptrKey));
   EXPECT_FALSE(table.find(funcAptrKey));
   EXPECT_FALSE(table.find(funcBptrKey));
   EXPECT_FALSE(table.find(intAKey));
   EXPECT_FALSE(table.find(intBKey));
   EXPECT_FALSE(table.find(shortAKey));
   EXPECT_FALSE(table.find(shortBKey));
   EXPECT_FALSE(table.find(floatAKey));
   EXPECT_FALSE(table.find(floatBKey));
   EXPECT_FALSE(table.find(doubleAKey));
   EXPECT_FALSE(table.find(doubleBKey));

   CCData::index_t classAptrIndex = INVALID_INDEX;
   CCData::index_t classBptrIndex = INVALID_INDEX;
   CCData::index_t funcAptrIndex = INVALID_INDEX;
   CCData::index_t funcBptrIndex = INVALID_INDEX;
   CCData::index_t intAIndex = INVALID_INDEX;
   CCData::index_t intBIndex = INVALID_INDEX;
   CCData::index_t shortAIndex = INVALID_INDEX;
   CCData::index_t shortBIndex = INVALID_INDEX;
   CCData::index_t floatAIndex = INVALID_INDEX;
   CCData::index_t floatBIndex = INVALID_INDEX;
   CCData::index_t doubleAIndex = INVALID_INDEX;
   CCData::index_t doubleBIndex = INVALID_INDEX;

   EXPECT_TRUE(table.put(classAptr, &classAptrKey, classAptrIndex));
   EXPECT_TRUE(table.put(classBptr, &classBptrKey, classBptrIndex));
   EXPECT_TRUE(table.put(funcAptr, &funcAptrKey, funcAptrIndex));
   EXPECT_TRUE(table.put(funcBptr, &funcBptrKey, funcBptrIndex));
   EXPECT_TRUE(table.put(intA, &intAKey, intAIndex));
   EXPECT_TRUE(table.put(intB, &intBKey, intBIndex));
   EXPECT_TRUE(table.put(shortA, &shortAKey, shortAIndex));
   EXPECT_TRUE(table.put(shortB, &shortBKey, shortBIndex));
   EXPECT_TRUE(table.put(floatA, &floatAKey, floatAIndex));
   EXPECT_TRUE(table.put(floatB, &floatBKey, floatBIndex));
   EXPECT_TRUE(table.put(doubleA, &doubleAKey, doubleAIndex));
   EXPECT_TRUE(table.put(doubleB, &doubleBKey, doubleBIndex));

   EXPECT_NE(classAptrIndex, INVALID_INDEX);
   EXPECT_NE(classBptrIndex, INVALID_INDEX);
   EXPECT_NE(funcAptrIndex, INVALID_INDEX);
   EXPECT_NE(funcBptrIndex, INVALID_INDEX);
   EXPECT_NE(intAIndex, INVALID_INDEX);
   EXPECT_NE(intBIndex, INVALID_INDEX);
   EXPECT_NE(shortAIndex, INVALID_INDEX);
   EXPECT_NE(shortBIndex, INVALID_INDEX);
   EXPECT_NE(floatAIndex, INVALID_INDEX);
   EXPECT_NE(floatBIndex, INVALID_INDEX);
   EXPECT_NE(doubleAIndex, INVALID_INDEX);
   EXPECT_NE(doubleBIndex, INVALID_INDEX);

   EXPECT_TRUE(table.find(CCData::key(classAptr)));
   EXPECT_TRUE(table.find(CCData::key(classBptr)));
   EXPECT_TRUE(table.find(CCData::key(funcAptr)));
   EXPECT_TRUE(table.find(CCData::key(funcBptr)));
   EXPECT_TRUE(table.find(CCData::key(intA)));
   EXPECT_TRUE(table.find(CCData::key(intB)));
   EXPECT_TRUE(table.find(CCData::key(shortA)));
   EXPECT_TRUE(table.find(CCData::key(shortB)));
   EXPECT_TRUE(table.find(CCData::key("floatA")));
   EXPECT_TRUE(table.find(CCData::key("floatB")));
   EXPECT_TRUE(table.find(CCData::key("doubleA")));
   EXPECT_TRUE(table.find(CCData::key("doubleB")));

   const void* * const ptr_classAptr = table.get<const void*>(classAptrIndex);
   const void* * const ptr_classBptr = table.get<const void*>(classBptrIndex);
   const void* * const ptr_funcAptr = table.get<const void*>(funcAptrIndex);
   const void* * const ptr_funcBptr = table.get<const void*>(funcBptrIndex);
   const int * const ptr_intA = table.get<const int>(intAIndex);
   const int * const ptr_intB = table.get<const int>(intBIndex);
   const short * const ptr_shortA = table.get<const short>(shortAIndex);
   const short * const ptr_shortB = table.get<const short>(shortBIndex);
   const float * const ptr_floatA = table.get<const float>(floatAIndex);
   const float * const ptr_floatB = table.get<const float>(floatBIndex);
   const double * const ptr_doubleA = table.get<const double>(doubleAIndex);
   const double * const ptr_doubleB = table.get<const double>(doubleBIndex);

   EXPECT_EQ(reinterpret_cast<size_t>(ptr_classAptr) & (OMR_ALIGNOF(*ptr_classAptr) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_classBptr) & (OMR_ALIGNOF(*ptr_classBptr) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_funcAptr) & (OMR_ALIGNOF(*ptr_funcAptr) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_funcBptr) & (OMR_ALIGNOF(*ptr_funcBptr) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_intA) & (OMR_ALIGNOF(*ptr_intA) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_intB) & (OMR_ALIGNOF(*ptr_intB) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_shortA) & (OMR_ALIGNOF(*ptr_shortA) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_shortB) & (OMR_ALIGNOF(*ptr_shortB) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_floatA) & (OMR_ALIGNOF(*ptr_floatA) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_floatB) & (OMR_ALIGNOF(*ptr_floatB) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_doubleA) & (OMR_ALIGNOF(*ptr_doubleA) - 1), 0);
   EXPECT_EQ(reinterpret_cast<size_t>(ptr_doubleB) & (OMR_ALIGNOF(*ptr_doubleB) - 1), 0);

   void     *out_classAptr = NULL, *out_classBptr = NULL;
   void     *out_funcAptr = NULL, *out_funcBptr = NULL;
   int      out_intA = ~intA, out_intB = ~intB;
   short    out_shortA = ~shortA, out_shortB = ~shortB;
   float    out_floatA = -floatA, out_floatB = -floatB;
   double   out_doubleA = -doubleA, out_doubleB = -doubleB;

   EXPECT_TRUE(table.get(classAptrIndex, out_classAptr));
   EXPECT_TRUE(table.get(classBptrIndex, out_classBptr));
   EXPECT_TRUE(table.get(funcAptrIndex, out_funcAptr));
   EXPECT_TRUE(table.get(funcBptrIndex, out_funcBptr));
   EXPECT_TRUE(table.get(intAIndex, out_intA));
   EXPECT_TRUE(table.get(intBIndex, out_intB));
   EXPECT_TRUE(table.get(shortAIndex, out_shortA));
   EXPECT_TRUE(table.get(shortBIndex, out_shortB));
   EXPECT_TRUE(table.get(floatAIndex, out_floatA));
   EXPECT_TRUE(table.get(floatBIndex, out_floatB));
   EXPECT_TRUE(table.get(doubleAIndex, out_doubleA));
   EXPECT_TRUE(table.get(doubleBIndex, out_doubleB));

   EXPECT_EQ(classAptr, out_classAptr);
   EXPECT_EQ(classBptr, out_classBptr);
   EXPECT_EQ(funcAptr, out_funcAptr);
   EXPECT_EQ(funcBptr, out_funcBptr);
   EXPECT_EQ(intA, out_intA);
   EXPECT_EQ(intB, out_intB);
   EXPECT_EQ(shortA, out_shortA);
   EXPECT_EQ(shortB, out_shortB);
   EXPECT_EQ(floatA, out_floatA);
   EXPECT_EQ(floatB, out_floatB);
   EXPECT_EQ(doubleA, out_doubleA);
   EXPECT_EQ(doubleB, out_doubleB);

   EXPECT_EQ(CCData::key(classAptr), CCData::key(out_classAptr));
   EXPECT_EQ(CCData::key(classBptr), CCData::key(out_classBptr));
   EXPECT_EQ(CCData::key(funcAptr), CCData::key(out_funcAptr));
   EXPECT_EQ(CCData::key(funcBptr), CCData::key(out_funcBptr));
   EXPECT_EQ(CCData::key(intA), CCData::key(out_intA));
   EXPECT_EQ(CCData::key(intB), CCData::key(out_intB));
   EXPECT_EQ(CCData::key(shortA), CCData::key(out_shortA));
   EXPECT_EQ(CCData::key(shortB), CCData::key(out_shortB));

   CCData::index_t find_classAptrIndex = INVALID_INDEX;
   CCData::index_t find_classBptrIndex = INVALID_INDEX;
   CCData::index_t find_funcAptrIndex = INVALID_INDEX;
   CCData::index_t find_funcBptrIndex = INVALID_INDEX;
   CCData::index_t find_intAIndex = INVALID_INDEX;
   CCData::index_t find_intBIndex = INVALID_INDEX;
   CCData::index_t find_shortAIndex = INVALID_INDEX;
   CCData::index_t find_shortBIndex = INVALID_INDEX;
   CCData::index_t find_floatAIndex = INVALID_INDEX;
   CCData::index_t find_floatBIndex = INVALID_INDEX;
   CCData::index_t find_doubleAIndex = INVALID_INDEX;
   CCData::index_t find_doubleBIndex = INVALID_INDEX;

   EXPECT_TRUE(table.find(CCData::key(classAptr), &find_classAptrIndex));
   EXPECT_TRUE(table.find(CCData::key(classBptr), &find_classBptrIndex));
   EXPECT_TRUE(table.find(CCData::key(funcAptr), &find_funcAptrIndex));
   EXPECT_TRUE(table.find(CCData::key(funcBptr), &find_funcBptrIndex));
   EXPECT_TRUE(table.find(CCData::key(intA), &find_intAIndex));
   EXPECT_TRUE(table.find(CCData::key(intB), &find_intBIndex));
   EXPECT_TRUE(table.find(CCData::key(shortA), &find_shortAIndex));
   EXPECT_TRUE(table.find(CCData::key(shortB), &find_shortBIndex));
   EXPECT_TRUE(table.find(CCData::key("floatA"), &find_floatAIndex));
   EXPECT_TRUE(table.find(CCData::key("floatB"), &find_floatBIndex));
   EXPECT_TRUE(table.find(CCData::key("doubleA"), &find_doubleAIndex));
   EXPECT_TRUE(table.find(CCData::key("doubleB"), &find_doubleBIndex));

   EXPECT_EQ(classAptrIndex, find_classAptrIndex);
   EXPECT_EQ(classBptrIndex, find_classBptrIndex);
   EXPECT_EQ(funcAptrIndex, find_funcAptrIndex);
   EXPECT_EQ(funcBptrIndex, find_funcBptrIndex);
   EXPECT_EQ(intAIndex, find_intAIndex);
   EXPECT_EQ(intBIndex, find_intBIndex);
   EXPECT_EQ(shortAIndex, find_shortAIndex);
   EXPECT_EQ(shortBIndex, find_shortBIndex);
   EXPECT_EQ(floatAIndex, find_floatAIndex);
   EXPECT_EQ(floatBIndex, find_floatBIndex);
   EXPECT_EQ(doubleAIndex, find_doubleAIndex);
   EXPECT_EQ(doubleBIndex, find_doubleBIndex);
   }

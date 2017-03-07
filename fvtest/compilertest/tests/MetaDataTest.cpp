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
 *******************************************************************************/

#include "gtest/gtest.h"
#include "avl_api.h"
#include "env/TRMemory.hpp"
#include "runtime/CodeMetaDataManager.hpp"
#include "runtime/CodeMetaDataPOD.hpp"
#include <vector>

namespace TestCompiler {
/**
 * The design of the CodeMetaData manager is such that it expects each code
 * cache to be added to the CodeMetaDataManager. In order to avoid that
 * coupling in this test, the below class allows allocating 'fake' homes for
 * tree elements
 *
 * As well, this resets the singleton on destruction.
 */
class TestEnhancedCodeMetaDataManager : public TR::CodeMetaDataManager {
   public:
   TestEnhancedCodeMetaDataManager() : TR::CodeMetaDataManager() {}

   /**
    * Reset the singleton on destruction.
    */
   ~TestEnhancedCodeMetaDataManager()
      {
      TR_Memory::jitPersistentFree(_codeMetaDataManager); 
      _codeMetaDataManager = NULL;
      }

   TR::MetaDataHashTable* addFalseCacheInfo(uintptr_t base, uintptr_t top) {

      TR::MetaDataHashTable *newTable = allocateCodeMetaDataHash(base,top);

      if (newTable)
         {
         avl_insert(_metaDataAVL, (J9AVLTreeNode *) newTable);
         }

      return newTable;
   }
};

}

const static int DEFAULT_CACHE_BASE = 0;
const static int DEFAULT_CACHE_TOP  = 1000;

/*
 * Ensure the fixture setup works correctly.
 * Doesn't use the fixture, as the fixture depends on this test working.
 */
TEST(MetaDataFixture, FixturePrereqs) {
   TestCompiler::TestEnhancedCodeMetaDataManager manager;
   ASSERT_TRUE(manager.initializeCodeMetaDataManager());
   ASSERT_TRUE(NULL != manager.addFalseCacheInfo(DEFAULT_CACHE_BASE, DEFAULT_CACHE_TOP));
}

/*
 * Fixture sets up a metadata test manager, initializing it before the test
 * runs. The manager is torn down as part of the destructor.
 */
class MetaDataTest : public ::testing::Test {
 public:
   TestCompiler::TestEnhancedCodeMetaDataManager manager;
   TR::MethodMetaDataPOD default_data;
   const int default_startPC;
   const int default_endPC;
   const char * default_data_name;


   MetaDataTest() :
      manager(),
      default_startPC(DEFAULT_CACHE_BASE + 10),
      default_endPC(DEFAULT_CACHE_BASE + 100),
      default_data_name("default_data_name")
   {
      manager.initializeCodeMetaDataManager();
      manager.addFalseCacheInfo(DEFAULT_CACHE_BASE, DEFAULT_CACHE_TOP);

      default_data.startPC = default_startPC;
      default_data.endPC   = default_endPC;
      default_data.name    = default_data_name;
   }
};

/**
 * Ensure inserted metadata is found when looked up
 */
TEST_F(MetaDataTest, InsertionAndContainment) {
   ASSERT_TRUE(manager.insertMetaData(  &default_data));
   ASSERT_TRUE(manager.containsMetaData(&default_data));
}



/**
 * Ensure removed metadata isn't found
 */
TEST_F(MetaDataTest, Removal) {
   manager.insertMetaData(  &default_data);
   manager.containsMetaData(&default_data);
   ASSERT_TRUE(manager.removeMetaData(&default_data));
   ASSERT_FALSE(manager.containsMetaData(&default_data));
}

/**
 * The metadata manager doesn't take ownership of pointers.
 * A side effect of this is that containment queries are done
 * on pointer value, not contents.
 *
 * (However, findMetaDataForPC can be used for lookup based
 * on PC).
 */
TEST_F(MetaDataTest, ValueSemantics) {
   ASSERT_TRUE(manager.insertMetaData(  &default_data));
   TR::MethodMetaDataPOD newData = default_data;
   ASSERT_FALSE(manager.containsMetaData(&newData));
}



TEST_F(MetaDataTest, ExactInsertion) {
   manager.insertMetaData(  &default_data);
   auto* metadata = manager.findMetaDataForPC(default_data.startPC);
   ASSERT_TRUE(NULL != metadata);
   ASSERT_STREQ(default_data_name, metadata->name);
}

TEST_F(MetaDataTest, NonExactLookup) {
   manager.insertMetaData(&default_data);

   std::vector<int> test_points = {default_startPC, default_startPC + 1, default_endPC -1, };
   for (auto i : test_points) {
      auto* metadata = manager.findMetaDataForPC(i);
      ASSERT_TRUE(NULL != metadata) << "Didn't find pc" << i;
      ASSERT_TRUE(NULL !=  metadata->name);
      ASSERT_STREQ(default_data.name, metadata->name);
   }

   ASSERT_FALSE(manager.findMetaDataForPC(default_data.endPC));
}


/**
 * Need to make sure that the MetaDataManager works
 * when there are multiple code caches connected.
 */
TEST_F(MetaDataTest, MultipleCaches) {
   ASSERT_TRUE(NULL != manager.addFalseCacheInfo(2000,10000));
   ASSERT_TRUE(NULL != manager.addFalseCacheInfo(40000,90000));

   manager.insertMetaData(&default_data);

   // This should land in the second cache
   TR::MethodMetaDataPOD data2;
   data2.startPC = 2000;
   data2.endPC   = 8000;
   data2.name    = "second";
   ASSERT_TRUE(manager.insertMetaData(&data2));

   TR::MethodMetaDataPOD data3;
   data3.startPC = 45000;
   data3.endPC   = 45010;
   data3.name    = "third";
   ASSERT_TRUE(manager.insertMetaData(&data3));

   std::vector<int> test_points = {50, 3000, 45005};
   std::vector<const char*> names = {default_data_name, "second", "third"};
   int iter = 0;
   for (auto i : test_points) {
      auto* metadata = manager.findMetaDataForPC(i);
      ASSERT_TRUE(NULL != metadata) << "Didn't find pc" << i;
      ASSERT_TRUE(NULL !=  metadata->name);
      ASSERT_STREQ(names[iter++], metadata->name);
      ASSERT_TRUE(manager.removeMetaData(metadata));
      ASSERT_TRUE(NULL == manager.findMetaDataForPC(i));
   }

}

/**
 * empty elements can't be inserted (because they can't be found) 
 */
TEST_F(MetaDataTest, EmptyInsertion) {
   TR::MethodMetaDataPOD emptyRange;
   emptyRange.startPC = DEFAULT_CACHE_BASE + 100; 
   emptyRange.endPC   = DEFAULT_CACHE_BASE + 100; 

   ASSERT_FALSE(manager.insertMetaData(  &emptyRange));
}

TEST_F(MetaDataTest, InsertionOutsideCodeCache) {
   TR::MethodMetaDataPOD outside_data;
   outside_data.startPC = DEFAULT_CACHE_TOP + default_startPC;
   outside_data.startPC = DEFAULT_CACHE_TOP + default_endPC;
   // Not setting name because it shouldn't matter.
   ASSERT_DEATH(manager.insertMetaData(&outside_data), ".*Assertion failed.*");
}

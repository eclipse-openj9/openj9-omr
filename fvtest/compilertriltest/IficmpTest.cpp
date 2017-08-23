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

#include <gtest/gtest.h>
#include "Jit.hpp"
#include "JitTest.hpp"
#include "jitbuilder_compiler.hpp"

using IfcmpReductionParamType = std::tuple<std::string, std::string, std::string, std::string>;

class IfcmpReductionTest : public ::testing::TestWithParam<IfcmpReductionParamType>
   {
   public:

   static void SetUpTestCase()
      {
      // Disable global value propagation and local common subexpression elimination.
      // Otherwise, the trees are optimized into nothing.
      const char *options = "-Xjit:acceptHugeMethods,enableBasicBlockHoisting,omitFramePointer,"
         "useIlValidator,paranoidoptcheck,disableGlobalVP,disableLocalCSE";

      auto initSuccess = initializeJitWithOptions(const_cast<char*>(options));
      ASSERT_TRUE(initSuccess) << "Failed to initialize the JIT.";
      }

   static void TearDownTestCase()
      {
      shutdownJit();
      }
   };

/**
 * @brief Struct equivalent to the IfcmpReductionParamType tuple
 *
 * Used for easier unpacking of test argument.
 */
struct IfcmpReductionParamStruct
   {
   std::string type;
   std::string sign;
   std::string val1;
   std::string val2;
   };

/**
 * @brief Given an instance of IfcmpReductionParamType, returns an equivalent instance
 *    of IfcmpReductionParamStruct
 */
IfcmpReductionParamStruct to_struct(IfcmpReductionParamType param)
   {
   IfcmpReductionParamStruct s;
   s.type = std::get<0>(param);
   s.sign = std::get<1>(param);
   s.val1 = std::get<2>(param);
   s.val2 = std::get<3>(param);
   return s;
   }

/*
 * Test reductions of the ifcmpge opcode to ifcmpeq
 *
 * Currently, we cannot perform this test automatically, so we are resorted to
 * manually running the tests and examining the trace file.
 *
 * Ideally, the following should be performed:
 * 1) Construct the trees that are to be transformed
 * 2) Do the transformation
 * 3) Compare the resulting trees with the expected trees.
 *
 * However, the current implementation of Tril does not support step 3).
 *
 * To produce a trace log, run the following:
 * $ TR_Options=traceilgen,tracefull,log=trace.log ./comptest --gtest_filter="IfcmpInt32Reduction/IfcmpReductionTest.Reduction/IntegerSigned_1_42"
 *
 * Then, open the trace log and witness the transformation.
 */
TEST_P(IfcmpReductionTest, Reduction)
   {
   auto param = to_struct(GetParam());

   std::string type = param.type;
   std::string sign = param.sign;
   std::string val1 = param.val1;
   std::string val2 = param.val2;

   const std::size_t treeBufferSize = 600;

   char inputTrees[treeBufferSize];
   std::snprintf(inputTrees, treeBufferSize,
         "(method return=\"NoType\""
         "  (block name=\"b\" fallthrough=\"u\""
         "    (%sstore temp=\"val\""
         "       (%sconst %s) )"
         "      (if%s%scmpge target=\"t\""
         "          (%sand"
         "              (%sload temp=\"val\")"
         "              (%sconst %s))"
         "          (%sconst %s) ) )"
         "  (block name=\"u\" (return))"
         "  (block name=\"t\" (return))"
         "  ) ; method"
         "  )",
         type.c_str(),  // type of store
         type.c_str(),  // type of the store's const
         val1.c_str(),  // value of the variable
         type.c_str(),  // type of ifcmpge
         sign.c_str(),  // signedness of ifcmpge
         type.c_str(),  // type of and
         type.c_str(),  // type of and's load
         type.c_str(),  // type of and's const
         val2.c_str(),  // value of and's const
         type.c_str(),  // type of the ifcmpge's const
         val2.c_str()   // value of the of the ifcmpge's const
         );

   auto trees = parseString(inputTrees);

   ASSERT_NOTNULL(trees);

   Tril::JitBuilderCompiler compiler{trees};

   ASSERT_EQ(0, compiler.compile()) << "Compilation failed";
   }

/**
 * @brief Sanitize string representation of numbers so they can be used in test names.
 *
 * E.g., transform "-1" into "minus1" since the dash sign is illegal in a test name.
 */
std::string sanitizeStrNumber(const std::string &strNum)
   {
      if (!strNum.empty() && strNum[0] == '-')
         {
         return std::string("minus") + strNum.substr(1);
         }
      return strNum;
   }

/**
* @brief Create a name for each test instead of using numbers to denote a test.
*
* E.g., LongUnsigned_0_42 denotes a case, in which:
* - The type is long unsigned
* - The value of the variable is 0
* - The value of the constant is 42
*/
std::string reductionTestNameGenerator(::testing::TestParamInfo<IfcmpReductionParamType> paramInfo)
   {
   std::string name;

   auto param = to_struct(paramInfo.param);

   std::string type = param.type;
   std::string sign = param.sign;
   std::string val1 = param.val1;
   std::string val2 = param.val2;

   if (std::string("i") == type)
       {
       name += "Integer";
       }
   else if (std::string("l") == type)
       {
       name += "Long";
       }
   else if (std::string("b") == type)
       {
       name += "Byte";
       }
   else
       {
       name += "Short";
       }

   if (std::string("u") == sign)
       {
       name += "Unsigned";
       }
   else
       {
       name += "Signed";
       }

   name += "_";
   name += sanitizeStrNumber(val1);
   name += "_";
   name += sanitizeStrNumber(val2);

   return name;
   }

/**
 * @brief Wrapper around const_values returning unique test inputs of the specified type
 */
template <typename T>
std::vector<T> const_values_unique()
   {
   std::vector<T> cvalues = TRTest::const_values<T>();
   std::set<T> cvaluesSet(cvalues.begin(), cvalues.end());
   std::vector<T> res(cvaluesSet.begin(), cvaluesSet.end());
   return res;
   }

/**
 * @brief Wrapper around const_values_unique returning stringified test inputs of the specified type
 */
template <typename T>
std::vector<std::string> str_const_values()
   {
   std::vector<T> cvalues = const_values_unique<T>();

   std::vector<std::string> res;
   res.reserve(cvalues.size());
   for (auto it = cvalues.begin(); it != cvalues.end(); ++it)
      {
      res.push_back(std::to_string(*it));
      }
   return res;
   }

/* Tests are combinations of the following:
 * 1) Integer / long / address / byte / short
 * 2) Signed / unsigned (e.g., ificmp / ifiUcmp)
 * 3) Combinations of const values for the variable
 * 4) Combinations of const values for the constant
 */
INSTANTIATE_TEST_CASE_P(IfcmpInt8Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("b"),
            ::testing::Values(""),
            ::testing::ValuesIn(str_const_values<int8_t>()),
            ::testing::ValuesIn(str_const_values<int8_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpUint8Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("b"),
            ::testing::Values("u"),
            ::testing::ValuesIn(str_const_values<uint8_t>()),
            ::testing::ValuesIn(str_const_values<uint8_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpInt16Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("s"),
            ::testing::Values(""),
            ::testing::ValuesIn(str_const_values<int16_t>()),
            ::testing::ValuesIn(str_const_values<int16_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpUint16Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("s"),
            ::testing::Values("u"),
            ::testing::ValuesIn(str_const_values<uint16_t>()),
            ::testing::ValuesIn(str_const_values<uint16_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpInt32Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("i"),
            ::testing::Values(""),
            ::testing::ValuesIn(str_const_values<int32_t>()),
            ::testing::ValuesIn(str_const_values<int32_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpUnt32Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("i"),
            ::testing::Values("u"),
            ::testing::ValuesIn(str_const_values<uint32_t>()),
            ::testing::ValuesIn(str_const_values<uint32_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpInt64Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("l"),
            ::testing::Values(""),
            ::testing::ValuesIn(str_const_values<int64_t>()),
            ::testing::ValuesIn(str_const_values<int64_t>())),
        reductionTestNameGenerator);

INSTANTIATE_TEST_CASE_P(IfcmpUint64Reduction, IfcmpReductionTest,
        ::testing::Combine(
            ::testing::Values("l"),
            ::testing::Values("u"),
            ::testing::ValuesIn(str_const_values<uint64_t>()),
            ::testing::ValuesIn(str_const_values<uint64_t>())),
        reductionTestNameGenerator);

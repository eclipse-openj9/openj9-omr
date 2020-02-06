/*******************************************************************************
 * Copyright (c) 2019, 2020 IBM Corp. and others
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

#include "OpCodeTest.hpp"
#include "default_compiler.hpp"
#include "omrformatconsts.h"

// C++11 upgrade (Issue #1916).
template <typename CompareType, typename ValType>
class TernaryTest : public TRTest::JitTest, public ::testing::WithParamInterface<std::tuple<std::tuple<CompareType, CompareType>, std::tuple<ValType, ValType>, ValType (*)(CompareType, CompareType, ValType, ValType)>> {};

/** \brief
 *     Struct equivalent of the TernaryTestParamStruct tuple.
 *
 *  \tparam CompareType
 *     The type of the compare child of the ternary opcode.
 *
 *  \tparam ValType
 *     The type that the ternary opcode returns.
 */
template <typename CompareType, typename ValType>
struct TernaryTestParamStruct
   {
   CompareType c1;
   CompareType c2;
   ValType v1;
   ValType v2;
   ValType (*oracle)(CompareType, CompareType, ValType, ValType);
   };

/** \brief
 *     Given an instance of TernaryTestParamStruct, returns an equivalent instance of TernaryTestParamStruct.
 */
template <typename CompareType, typename ValType>
TernaryTestParamStruct<CompareType, ValType> to_struct(std::tuple<std::tuple<CompareType, CompareType>, std::tuple<ValType, ValType>, ValType (*)(CompareType, CompareType, ValType, ValType)> param)
   {
   TernaryTestParamStruct<CompareType, ValType> s;

   s.c1 = std::get<0>(std::get<0>(param));
   s.c2 = std::get<1>(std::get<0>(param));
   s.v1 = std::get<0>(std::get<1>(param));
   s.v2 = std::get<1>(std::get<1>(param));
   s.oracle = std::get<2>(param);
   return s;
   }

/** \brief
 *     The oracle function which computes the expected result of the ternary test.
 *
 *  \tparam CompareType
 *     The type of the values compared by oracle function.
 *  \tparam ValType
 *     The return type of the oracle function.
 */
template <typename CompareType, typename ValType>
static ValType xternaryOracle(CompareType c1, CompareType c2, ValType v1, ValType v2)
   {
   return ((c1 < c2) ? v1 : v2);
   }

/** \brief
 *     The function computes the input vector of pairs for the compare node.
 */
template <typename T>
static std::vector<std::tuple<T, T>> compareInputs()
   {
   std::tuple<T, T> inputArray[] =
      {
      std::make_pair(std::numeric_limits<T>::min(), std::numeric_limits<T>::max()),
      std::make_pair(std::numeric_limits<T>::max(), std::numeric_limits<T>::min()),
      std::make_pair(std::numeric_limits<T>::min(), std::numeric_limits<T>::min()),
      std::make_pair(0, std::numeric_limits<T>::max()),
      std::make_pair(0, std::numeric_limits<T>::min()),
      std::make_pair(std::numeric_limits<T>::min(), 0),
      std::make_pair(std::numeric_limits<T>::max(), 0),
      };
   return std::vector<std::tuple<T, T>>(inputArray, inputArray + sizeof(inputArray)/sizeof(std::tuple<T, T>));
   }

/** \brief
 *     The function computes the true/false result vector of pairs for the ternary node.
 */
template <typename T>
static std::vector<std::tuple<T, T>> resultInputs()
   {
   std::tuple<T, T> inputArray[] =
      {
      std::make_pair(1, 0),
      std::make_pair(0, 1),
      };
   return std::vector<std::tuple<T, T>>(inputArray, inputArray + sizeof(inputArray)/sizeof(std::tuple<T, T>));
   }

class Int32TernaryInt32CompareTest : public TernaryTest<int32_t, int32_t> {};

TEST_P(Int32TernaryInt32CompareTest, UsingLoadParam) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int32 args=[Int32, Int32, Int32, Int32]"
        "  (block"
        "    (ireturn"
        "      (iternary"
        "        (icmplt"
        "          (iload parm=0)"
        "          (iload parm=1))"
        "        (iload parm=2)"
        "        (iload parm=3))"
        ")))");
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int32_t, int32_t, int32_t, int32_t)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2, param.v1, param.v2));
}

TEST_P(Int32TernaryInt32CompareTest, UsingConst) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (iternary"
        "        (icmplt"
        "          (iconst %d)"
        "          (iconst %d))"
        "        (iconst %d)"
        "        (iconst %d))"
        ")))",
        param.c1,
        param.c2,
        param.v1,
        param.v2);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point());
}

INSTANTIATE_TEST_CASE_P(TernaryTest, Int32TernaryInt32CompareTest,
        ::testing::Combine(
            ::testing::ValuesIn(compareInputs<int32_t>()),
            ::testing::ValuesIn(resultInputs<int32_t>()),
            ::testing::Values(xternaryOracle<int32_t, int32_t>)));

class Int64TernaryInt64CompareTest : public TernaryTest<int64_t, int64_t> {};

TEST_P(Int64TernaryInt64CompareTest, UsingLoadParam) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int64 args=[Int64, Int64, Int64, Int64]"
        "  (block"
        "    (lreturn"
        "      (lternary"
        "        (lcmplt"
        "          (lload parm=0)"
        "          (lload parm=1))"
        "        (lload parm=2)"
        "        (lload parm=3))"
        ")))");
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(int64_t, int64_t, int64_t, int64_t)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2, param.v1, param.v2));
}

TEST_P(Int64TernaryInt64CompareTest, UsingConst) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (lternary"
        "        (lcmplt"
        "          (lconst %" OMR_PRId64 ")"
        "          (lconst %" OMR_PRId64 "))"
        "        (lconst %" OMR_PRId64 ")"
        "        (lconst %" OMR_PRId64 "))"
        ")))",
        param.c1,
        param.c2,
        param.v1,
        param.v2);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(void)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point());
}

INSTANTIATE_TEST_CASE_P(TernaryTest, Int64TernaryInt64CompareTest,
        ::testing::Combine(
            ::testing::ValuesIn(compareInputs<int64_t>()),
            ::testing::ValuesIn(resultInputs<int64_t>()),
            ::testing::Values(xternaryOracle<int64_t, int64_t>)));


class Int64TernaryDoubleCompareTest : public TernaryTest<double, int64_t> {};

TEST_P(Int64TernaryDoubleCompareTest, UsingLoadParam) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int64 args=[Double, Double, Int64, Int64]"
        "  (block"
        "    (lreturn"
        "      (lternary"
        "        (dcmplt"
        "          (dload parm=0)"
        "          (dload parm=1))"
        "        (lload parm=2)"
        "        (lload parm=3))"
        ")))");
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(double, double, int64_t, int64_t)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2, param.v1, param.v2));
}

TEST_P(Int64TernaryDoubleCompareTest, UsingConst) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int64 args=[Double, Double]"
        "  (block"
        "    (lreturn"
        "      (lternary"
        "        (dcmplt"
        "          (dload parm=0)"
        "          (dload parm=1))"
        "        (lconst %" OMR_PRId64 ")"
        "        (lconst %" OMR_PRId64 "))"
        ")))",
        param.v1,
        param.v2);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(double, double)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2));
}

INSTANTIATE_TEST_CASE_P(TernaryTest, Int64TernaryDoubleCompareTest,
        ::testing::Combine(
            ::testing::ValuesIn(compareInputs<double>()),
            ::testing::ValuesIn(resultInputs<int64_t>()),
            ::testing::Values(xternaryOracle<double, int64_t>)));

class Int32TernaryDoubleCompareTest : public TernaryTest<double, int32_t> {};

TEST_P(Int32TernaryDoubleCompareTest, UsingLoadParam) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int32 args=[Double, Double, Int32, Int32]"
        "  (block"
        "    (ireturn"
        "      (iternary"
        "        (dcmplt"
        "          (dload parm=0)"
        "          (dload parm=1))"
        "        (iload parm=2)"
        "        (iload parm=3))"
        ")))");
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(double, double, int32_t, int32_t)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2, param.v1, param.v2));
}

TEST_P(Int32TernaryDoubleCompareTest, UsingConst) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int32 args=[Double, Double]"
        "  (block"
        "    (ireturn"
        "      (iternary"
        "        (dcmplt"
        "          (dload parm=0)"
        "          (dload parm=1))"
        "        (iconst %d)"
        "        (iconst %d))"
        ")))",
        param.v1,
        param.v2);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(double, double)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2));
}

INSTANTIATE_TEST_CASE_P(TernaryTest, Int32TernaryDoubleCompareTest,
        ::testing::Combine(
            ::testing::ValuesIn(compareInputs<double>()),
            ::testing::ValuesIn(resultInputs<int32_t>()),
            ::testing::Values(xternaryOracle<double, int32_t>)));


class ShortTernaryDoubleCompareTest : public TernaryTest<double, int16_t> {};

TEST_P(ShortTernaryDoubleCompareTest, UsingLoadParam) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int16 args=[Double, Double, Int16, Int16]"
        "  (block"
        "    (ireturn"
        "      (s2i"
        "        (sternary"
        "          (dcmplt"
        "            (dload parm=0)"
        "            (dload parm=1))"
        "          (sload parm=2)"
        "          (sload parm=3))"
        "))))");
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int16_t (*)(double, double, int16_t, int16_t)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2, param.v1, param.v2));
}

TEST_P(ShortTernaryDoubleCompareTest, UsingConst) {
    auto param = to_struct(GetParam());

    char inputTrees[512] = {0};
    std::snprintf(inputTrees, sizeof(inputTrees),
        "(method return=Int16 args=[Double, Double]"
        "  (block"
        "    (ireturn"
        "      (s2i"
        "        (sternary"
        "          (dcmplt"
        "            (dload parm=0)"
        "            (dload parm=1))"
        "          (sconst %d)"
        "          (sconst %d))"
        "))))",
        param.v1,
        param.v2);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    int32_t compileResult = compiler.compile();
    ASSERT_EQ(0, compileResult) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int16_t (*)(double, double)>();
    ASSERT_EQ(param.oracle(param.c1, param.c2, param.v1, param.v2), entry_point(param.c1, param.c2));
}

INSTANTIATE_TEST_CASE_P(TernaryTest, ShortTernaryDoubleCompareTest,
        ::testing::Combine(
            ::testing::ValuesIn(compareInputs<double>()),
            ::testing::ValuesIn(resultInputs<int16_t>()),
            ::testing::Values((xternaryOracle<double, int16_t>))));

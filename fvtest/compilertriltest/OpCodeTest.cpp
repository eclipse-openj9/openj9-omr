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

#include "OpCodeTest.hpp"
#include "jitbuilder_compiler.hpp"

#include <limits>

#define INT32_POS 3
#define INT32_NEG -2
#define INT32_ZERO 0

class Int32Arithmetic : public BinaryOpTest<int32_t> {};

TEST_P(Int32Arithmetic, ConstArithmetic) {
    auto param = to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120, "(method \"Int32\" (block (ireturn (i%s (iconst %d) (iconst %d)) )))", param.opcode.c_str(), param.lhs, param.rhs);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::JitBuilderCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    ASSERT_EQ(param.oracle(param.lhs, param.rhs), entry_point());
}

TEST_P(Int32Arithmetic, LoadParamArithmetic) {
    auto param = to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120, "(method \"Int32\" \"Int32\" \"Int32\" (block (ireturn (i%s (iload parm=0) (iload parm=1)) )))", param.opcode.c_str());
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::JitBuilderCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int32_t, int32_t)>();
    ASSERT_EQ(param.oracle(param.lhs, param.rhs), entry_point(param.lhs, param.rhs));
}

INSTANTIATE_TEST_CASE_P(ArithmeticTest, Int32Arithmetic, ::testing::Combine(
    ::testing::ValuesIn(
        make_value_pairs<int32_t>(INT32_ZERO, INT32_POS, INT32_NEG, INT32_MIN, INT32_MAX)),
    ::testing::Values(
        std::make_tuple("add", [](int32_t l, int32_t r){return l+r;}),
        std::make_tuple("sub", [](int32_t l, int32_t r){return l-r;}),
        std::make_tuple("mul", [](int32_t l, int32_t r){return l*r;}) )));

INSTANTIATE_TEST_CASE_P(DivArithmeticTest, Int32Arithmetic, ::testing::Combine(
    ::testing::ValuesIn(
        filter(make_value_pairs<int32_t>(INT32_ZERO, INT32_POS, INT32_NEG, INT32_MIN, INT32_MAX),
               [](std::tuple<int32_t, int32_t> a){ return std::get<1>(a) == 0;} )),
    ::testing::Values(
        std::make_tuple("div", [](int32_t l, int32_t r){return l/r;}),
        std::make_tuple("rem", [](int32_t l, int32_t r){return l%r;}) )));

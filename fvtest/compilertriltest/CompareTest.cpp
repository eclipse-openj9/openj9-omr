/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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

int32_t icmpeq(int32_t l, int32_t r) {
    return (l == r) ? 1 : 0;
}

int32_t icmpne(int32_t l, int32_t r) {
    return (l != r) ? 1 : 0;
}

int32_t icmpgt(int32_t l, int32_t r) {
    return (l > r) ? 1 : 0;
}

int32_t icmpge(int32_t l, int32_t r) {
    return (l >= r) ? 1 : 0;
}

int32_t icmplt(int32_t l, int32_t r) {
    return (l < r) ? 1 : 0;
}

int32_t icmple(int32_t l, int32_t r) {
    return (l <= r) ? 1 : 0;
}

class Int32Compare : public TRTest::BinaryOpTest<int32_t> {};

TEST_P(Int32Compare, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120,
       "(method return=Int32 "
         "(block "
           "(ireturn "
             "(%s "
               "(iconst %d) "
               "(iconst %d)))))",
       param.opcode.c_str(), param.lhs, param.rhs);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int32Compare, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120,
       "(method return=Int32 args=[Int32, Int32] "
         "(block "
           "(ireturn "
             "(%s "
               "(iload parm=0) "
               "(iload parm=1)))))",
       param.opcode.c_str());
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int32_t, int32_t)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point(param.lhs, param.rhs);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(CompareTest, Int32Compare, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_value_pairs<int32_t, int32_t>()),
    ::testing::Values(
        std::make_tuple("icmpeq", icmpeq),
        std::make_tuple("icmpne", icmpne),
        std::make_tuple("icmpgt", icmpgt),
        std::make_tuple("icmpge", icmpge),
        std::make_tuple("icmplt", icmplt),
        std::make_tuple("icmple", icmple) )));

int32_t iucmpeq(uint32_t l, uint32_t r) {
    return (l == r) ? 1 : 0;
}

int32_t iucmpne(uint32_t l, uint32_t r) {
    return (l != r) ? 1 : 0;
}

int32_t iucmpgt(uint32_t l, uint32_t r) {
    return (l > r) ? 1 : 0;
}

int32_t iucmpge(uint32_t l, uint32_t r) {
    return (l >= r) ? 1 : 0;
}

int32_t iucmplt(uint32_t l, uint32_t r) {
    return (l < r) ? 1 : 0;
}

int32_t iucmple(uint32_t l, uint32_t r) {
    return (l <= r) ? 1 : 0;
}

class UInt32Compare : public TRTest::OpCodeTest<int32_t,uint32_t,uint32_t> {};

TEST_P(UInt32Compare, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120,
       "(method return=Int32 "
         "(block "
           "(ireturn "
             "(%s "
               "(iconst %d) "
               "(iconst %d)))))",
       param.opcode.c_str(), param.lhs, param.rhs);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt32Compare, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[120] = {0};
    std::snprintf(inputTrees, 120,
       "(method return=Int32 args=[Int32, Int32] "
         "(block "
           "(ireturn "
             "(%s "
               "(iload parm=0) "
               "(iload parm=1)))))",
       param.opcode.c_str());
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(uint32_t, uint32_t)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point(param.lhs, param.rhs);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(CompareTest, UInt32Compare, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_value_pairs<uint32_t, uint32_t>()),
    ::testing::Values(
        std::make_tuple("iucmpeq", iucmpeq),
        std::make_tuple("iucmpne", iucmpne),
        std::make_tuple("iucmpgt", iucmpgt),
        std::make_tuple("iucmpge", iucmpge),
        std::make_tuple("iucmplt", iucmplt),
        std::make_tuple("iucmple", iucmple) )));

int32_t lcmpeq(int64_t l, int64_t r) {
    return (l == r) ? 1 : 0;
}

int32_t lcmpne(int64_t l, int64_t r) {
    return (l != r) ? 1 : 0;
}

int32_t lcmpgt(int64_t l, int64_t r) {
    return (l > r) ? 1 : 0;
}

int32_t lcmpge(int64_t l, int64_t r) {
    return (l >= r) ? 1 : 0;
}

int32_t lcmplt(int64_t l, int64_t r) {
    return (l < r) ? 1 : 0;
}

int32_t lcmple(int64_t l, int64_t r) {
    return (l <= r) ? 1 : 0;
}

int32_t lcmp(int64_t l, int64_t r) {
    return (l < r) ? -1 : ((l > r) ? 1 : 0);
}

class Int64Compare : public TRTest::OpCodeTest<int32_t,int64_t,int64_t> {};

TEST_P(Int64Compare, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
       "(method return=Int32 "
         "(block "
           "(ireturn "
             "(%s "
               "(lconst %lld) "
               "(lconst %lld)))))",
       param.opcode.c_str(), param.lhs, param.rhs);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int64Compare, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
       "(method return=Int32 args=[Int64, Int64] "
         "(block "
           "(ireturn "
             "(%s "
               "(lload parm=0) "
               "(lload parm=1)))))",
       param.opcode.c_str());
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int64_t, int64_t)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point(param.lhs, param.rhs);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(CompareTest, Int64Compare, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_value_pairs<int64_t, int64_t>()),
    ::testing::Values(
        std::make_tuple("lcmpeq", lcmpeq),
        std::make_tuple("lcmpne", lcmpne),
        std::make_tuple("lcmpgt", lcmpgt),
        std::make_tuple("lcmpge", lcmpge),
        std::make_tuple("lcmplt", lcmplt),
        std::make_tuple("lcmple", lcmple),
        std::make_tuple("lcmp", lcmp)
 )));

int32_t lucmpeq(uint64_t l, uint64_t r) {
    return (l == r) ? 1 : 0;
}

int32_t lucmpne(uint64_t l, uint64_t r) {
    return (l != r) ? 1 : 0;
}

int32_t lucmpgt(uint64_t l, uint64_t r) {
    return (l > r) ? 1 : 0;
}

int32_t lucmpge(uint64_t l, uint64_t r) {
    return (l >= r) ? 1 : 0;
}

int32_t lucmplt(uint64_t l, uint64_t r) {
    return (l < r) ? 1 : 0;
}

int32_t lucmple(uint64_t l, uint64_t r) {
    return (l <= r) ? 1 : 0;
}

class UInt64Compare : public TRTest::OpCodeTest<int32_t,uint64_t,uint64_t> {};

TEST_P(UInt64Compare, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
       "(method return=Int32 "
         "(block "
           "(ireturn "
             "(%s "
               "(lconst %d) "
               "(lconst %d)))))",
       param.opcode.c_str(), param.lhs, param.rhs);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(void)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt64Compare, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
       "(method return=Int32 args=[Int64, Int64] "
         "(block "
           "(ireturn "
             "(%s "
               "(lload parm=0) "
               "(lload parm=1)))))",
       param.opcode.c_str());
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(uint64_t, uint64_t)>();
    volatile auto exp = param.oracle(param.lhs, param.rhs);
    volatile auto act = entry_point(param.lhs, param.rhs);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(CompareTest, UInt64Compare, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_value_pairs<uint64_t, uint64_t>()),
    ::testing::Values(
        std::make_tuple("lucmpeq", lucmpeq),
        std::make_tuple("lucmpne", lucmpne),
        std::make_tuple("lucmpgt", lucmpgt),
        std::make_tuple("lucmpge", lucmpge),
        std::make_tuple("lucmplt", lucmplt),
        std::make_tuple("lucmple", lucmple) )));

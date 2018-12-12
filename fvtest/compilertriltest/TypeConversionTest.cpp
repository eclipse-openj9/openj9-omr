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

int32_t b2i(int8_t x) {
    return static_cast<int32_t>(x);
}

int32_t bu2i(uint8_t x) {
    return static_cast<int32_t>(x);
}

int64_t b2l(int8_t x) {
    return static_cast<int64_t>(x);
}

int64_t bu2l(uint8_t x) {
    return static_cast<int64_t>(x);
}

int32_t s2i(int16_t x) {
    return static_cast<int32_t>(x);
}

int32_t su2i(uint16_t x) {
    return static_cast<int32_t>(x);
}

int64_t s2l(int16_t x) {
    return static_cast<int64_t>(x);
}

int64_t su2l(uint16_t x) {
    return static_cast<int64_t>(x);
}

int64_t i2l(int32_t x) {
    return static_cast<int64_t>(x);
}

int64_t iu2l(uint32_t x) {
    return static_cast<int64_t>(x);
}

class Int8ToInt32 : public TRTest::UnaryOpTest<int32_t,int8_t> {};

TEST_P(Int8ToInt32, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (b2i"
        "        (bconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int8ToInt32, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int32 args=[Int8]"
        "  (block"
        "    (ireturn"
        "      (b2i"
        "        (bload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int8_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int8ToInt32, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int8_t>()),
    ::testing::Values(
        std::make_tuple("b2i", b2i) )));

class UInt8ToInt32 : public TRTest::UnaryOpTest<int32_t,uint8_t> {};

TEST_P(UInt8ToInt32, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (bu2i"
        "        (bconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt8ToInt32, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int32 args=[Int8]"
        "  (block"
        "    (ireturn"
        "      (bu2i"
        "        (bload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(uint8_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, UInt8ToInt32, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<uint8_t>()),
    ::testing::Values(
        std::make_tuple("bu2i", bu2i) )));

class Int8ToInt64 : public TRTest::UnaryOpTest<int64_t,int8_t> {};

TEST_P(Int8ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (b2l"
        "        (bconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int8ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int8]"
        "  (block"
        "    (lreturn"
        "      (b2l"
        "        (bload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(int8_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int8ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int8_t>()),
    ::testing::Values(
        std::make_tuple("b2l", b2l) )));

class UInt8ToInt64 : public TRTest::UnaryOpTest<int64_t,uint8_t> {};

TEST_P(UInt8ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (bu2l"
        "        (bconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt8ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int8]"
        "  (block"
        "    (lreturn"
        "      (bu2l"
        "        (bload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(uint8_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, UInt8ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<uint8_t>()),
    ::testing::Values(
        std::make_tuple("bu2l", bu2l) )));

class Int16ToInt32 : public TRTest::UnaryOpTest<int32_t,int16_t> {};

TEST_P(Int16ToInt32, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (s2i"
        "        (sconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int16ToInt32, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int32 args=[Int16]"
        "  (block"
        "    (ireturn"
        "      (s2i"
        "        (sload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int16_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int16ToInt32, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int16_t>()),
    ::testing::Values(
        std::make_tuple("s2i", s2i) )));

class UInt16ToInt32 : public TRTest::UnaryOpTest<int32_t,uint16_t> {};

TEST_P(UInt16ToInt32, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (su2i"
        "        (sconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt16ToInt32, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int32 args=[Int16]"
        "  (block"
        "    (ireturn"
        "      (su2i"
        "        (sload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(uint16_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, UInt16ToInt32, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<uint16_t>()),
    ::testing::Values(
        std::make_tuple("su2i", su2i) )));

class Int16ToInt64 : public TRTest::UnaryOpTest<int64_t,int16_t> {};

TEST_P(Int16ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (s2l"
        "        (sconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int16ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int16]"
        "  (block"
        "    (lreturn"
        "      (s2l"
        "        (sload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(int16_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int16ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int16_t>()),
    ::testing::Values(
        std::make_tuple("s2l", s2l) )));

class UInt16ToInt64 : public TRTest::UnaryOpTest<int64_t,uint16_t> {};

TEST_P(UInt16ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (su2l"
        "        (sconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt16ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int16]"
        "  (block"
        "    (lreturn"
        "      (su2l"
        "        (sload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(uint16_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, UInt16ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<uint16_t>()),
    ::testing::Values(
        std::make_tuple("su2l", su2l) )));

class Int32ToInt64 : public TRTest::UnaryOpTest<int64_t,int32_t> {};

TEST_P(Int32ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (i2l"
        "        (iconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int32ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int32]"
        "  (block"
        "    (lreturn"
        "      (i2l"
        "        (iload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(int32_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int32ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int32_t>()),
    ::testing::Values(
        std::make_tuple("i2l", i2l) )));

class UInt32ToInt64 : public TRTest::UnaryOpTest<int64_t,uint32_t> {};

TEST_P(UInt32ToInt64, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int64"
        "  (block"
        "    (lreturn"
        "      (iu2l"
        "        (iconst %d) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(UInt32ToInt64, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int64 args=[Int32]"
        "  (block"
        "    (lreturn"
        "      (iu2l"
        "        (iload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int64_t (*)(uint32_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, UInt32ToInt64, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<uint32_t>()),
    ::testing::Values(
        std::make_tuple("iu2l", iu2l) )));

int32_t l2i(int64_t x) {
    return static_cast<int32_t>(x);
}

class Int64ToInt32 : public TRTest::UnaryOpTest<int32_t,int64_t> {};

TEST_P(Int64ToInt32, UsingConst) {
    auto param = TRTest::to_struct(GetParam());

    char inputTrees[160] = {0};
    std::snprintf(inputTrees, 160,
        "(method return=Int32"
        "  (block"
        "    (ireturn"
        "      (l2i"
        "        (lconst %lld) ) ) ) )",
        param.value);
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)()>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point();
    ASSERT_EQ(exp, act);
}

TEST_P(Int64ToInt32, UsingLoadParam) {
    auto param = TRTest::to_struct(GetParam());

    char *inputTrees =
        "(method return=Int32 args=[Int64]"
        "  (block"
        "    (ireturn"
        "      (l2i"
        "        (lload parm=0) ) ) ) )";
    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<int32_t (*)(int64_t)>();
    volatile auto exp = param.oracle(param.value);
    volatile auto act = entry_point(param.value);
    ASSERT_EQ(exp, act);
}

INSTANTIATE_TEST_CASE_P(TypeConversionTest, Int64ToInt32, ::testing::Combine(
    ::testing::ValuesIn(TRTest::const_values<int64_t>()),
    ::testing::Values(
        std::make_tuple("l2i", l2i) )));

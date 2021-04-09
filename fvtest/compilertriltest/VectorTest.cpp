/*******************************************************************************
 * Copyright (c) 2017, 2021 IBM Corp. and others
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

#include "JitTest.hpp"
#include "default_compiler.hpp"

class VectorTest : public TRTest::JitTest {};


TEST_F(VectorTest, VDoubleAdd) { 

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorDouble offset=0                        "
                     "         (aload parm=0)                                         "
                     "            (vadd                                               "
                     "                 (vloadi type=VectorDouble (aload parm=1))      "
                     "                 (vloadi type=VectorDouble (aload parm=2))))    "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved. 
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(double[],double[],double[])>();
    //TODO: What do we query to determine vector width?  
    // -- This test currently assumes 128bit SIMD  

    double output[] =  {0.0, 0.0};
    double inputA[] =  {1.0, 2.0};
    double inputB[] =  {1.0, 2.0};

    entry_point(output,inputA,inputB); 
    EXPECT_DOUBLE_EQ(inputA[0] + inputB[0], output[0]); // Epsilon = 4ULP -- is this necessary? 
    EXPECT_DOUBLE_EQ(inputA[1] + inputB[1], output[1]); // Epsilon = 4ULP -- is this necessary? 
}

TEST_F(VectorTest, VInt8Add) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt8 offset=0                          "
                     "         (aload parm=0)                                         "
                     "            (vadd                                               "
                     "                 (vloadi type=VectorInt8 (aload parm=1))        "
                     "                 (vloadi type=VectorInt8 (aload parm=2))))      "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int8_t[],int8_t[],int8_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int8_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t inputA[] =  {7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, 7};
    int8_t inputB[] =  {-14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, 14, 1};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] + inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VInt16Add) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt16 offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vadd                                               "
                     "                 (vloadi type=VectorInt16 (aload parm=1))       "
                     "                 (vloadi type=VectorInt16 (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int16_t[],int16_t[],int16_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int16_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t inputA[] =  {60, 45, 30, 0, -3, -2, -1, 2};
    int16_t inputB[] =  {-5, -10, -1, 13, 15, 10, 7, 5};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] + inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VFloatAdd) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorFloat offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vadd                                               "
                     "                 (vloadi type=VectorFloat (aload parm=1))       "
                     "                 (vloadi type=VectorFloat (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(float[],float[],float[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    float output[] =  {0.0f, 0.0f, 0.0f, 0.0f};
    float inputA[] =  {6.0f, 0.0f, -0.1f, 0.6f};
    float inputB[] =  {-0.5f, 3.5f, 3.0f, 0.7f};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_FLOAT_EQ(inputA[i] + inputB[i], output[i]); // Epsilon = 4ULP -- is this necessary? 
    }
}

TEST_F(VectorTest, VInt8Sub) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt8 offset=0                          "
                     "         (aload parm=0)                                         "
                     "            (vsub                                               "
                     "                 (vloadi type=VectorInt8 (aload parm=1))        "
                     "                 (vloadi type=VectorInt8 (aload parm=2))))      "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int8_t[],int8_t[],int8_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int8_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t inputA[] =  {7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, 9};
    int8_t inputB[] =  {14, 12, 10, 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -12, -14, 1};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] - inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VInt16Sub) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt16 offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vsub                                               "
                     "                 (vloadi type=VectorInt16 (aload parm=1))       "
                     "                 (vloadi type=VectorInt16 (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int16_t[],int16_t[],int16_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int16_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t inputA[] =  {60, 45, 30, 0, -3, -2, -1, 9};
    int16_t inputB[] =  {5, 10, 1, -13, -15, -10, -7, 2};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] - inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VFloatSub) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorFloat offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vsub                                               "
                     "                 (vloadi type=VectorFloat (aload parm=1))       "
                     "                 (vloadi type=VectorFloat (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(float[],float[],float[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    float output[] =  {0.0f, 0.0f, 0.0f, 0.0f};
    float inputA[] =  {6.0f, 0.0f, -0.1f, 2.0f};
    float inputB[] =  {0.5f, -3.5f, -3.0f, 0.7f};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_FLOAT_EQ(inputA[i] - inputB[i], output[i]); // Epsilon = 4ULP -- is this necessary?
    }
}

TEST_F(VectorTest, VDoubleSub) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorDouble offset=0                        "
                     "         (aload parm=0)                                         "
                     "            (vsub                                               "
                     "                 (vloadi type=VectorDouble (aload parm=1))      "
                     "                 (vloadi type=VectorDouble (aload parm=2))))    "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(double[],double[],double[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    double output[] =  {0.0, 0.0};
    double inputA[] =  {1.0, -1.5};
    double inputB[] =  {1.1, -3.0};

    entry_point(output,inputA,inputB);
    EXPECT_DOUBLE_EQ(inputA[0] - inputB[0], output[0]); // Epsilon = 4ULP -- is this necessary?
    EXPECT_DOUBLE_EQ(inputA[1] - inputB[1], output[1]); // Epsilon = 4ULP -- is this necessary?
}

TEST_F(VectorTest, VInt8Mul) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt8 offset=0                          "
                     "         (aload parm=0)                                         "
                     "            (vmul                                               "
                     "                 (vloadi type=VectorInt8 (aload parm=1))        "
                     "                 (vloadi type=VectorInt8 (aload parm=2))))      "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);
    SKIP_ON_X86(MissingImplementation);
    SKIP_ON_HAMMER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int8_t[],int8_t[],int8_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int8_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t inputA[] =  {7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, 7};
    int8_t inputB[] =  {-14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12, -14, 1};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] * inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VInt16Mul) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorInt16 offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vmul                                               "
                     "                 (vloadi type=VectorInt16 (aload parm=1))       "
                     "                 (vloadi type=VectorInt16 (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_POWER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int16_t[],int16_t[],int16_t[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    int16_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t inputA[] =  {60, 45, 30, 0, -3, -2, -1, 2};
    int16_t inputB[] =  {-5, -10, -1, 13, 15, 10, -7, 5};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] * inputB[i], output[i]);
    }
}

TEST_F(VectorTest, VFloatMul) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorFloat offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vmul                                               "
                     "                 (vloadi type=VectorFloat (aload parm=1))       "
                     "                 (vloadi type=VectorFloat (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(float[],float[],float[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    float output[] =  {0.0f, 0.0f, 0.0f, 0.0f};
    float inputA[] =  {6.0f, 0.0f, -0.1f, 0.6f};
    float inputB[] =  {-0.5f, 3.5f, -3.0f, 0.7f};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_FLOAT_EQ(inputA[i] * inputB[i], output[i]); // Epsilon = 4ULP -- is this necessary?
    }
}

TEST_F(VectorTest, VDoubleMul) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorDouble offset=0                        "
                     "         (aload parm=0)                                         "
                     "            (vmul                                               "
                     "                 (vloadi type=VectorDouble (aload parm=1))      "
                     "                 (vloadi type=VectorDouble (aload parm=2))))    "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(double[],double[],double[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    double output[] =  {0.0, 0.0};
    double inputA[] =  {1.0, -1.5};
    double inputB[] =  {-1.1, -3.0};

    entry_point(output,inputA,inputB);
    EXPECT_DOUBLE_EQ(inputA[0] * inputB[0], output[0]); // Epsilon = 4ULP -- is this necessary?
    EXPECT_DOUBLE_EQ(inputA[1] * inputB[1], output[1]); // Epsilon = 4ULP -- is this necessary?
}

TEST_F(VectorTest, VFloatDiv) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorFloat offset=0                         "
                     "         (aload parm=0)                                         "
                     "            (vdiv                                               "
                     "                 (vloadi type=VectorFloat (aload parm=1))       "
                     "                 (vloadi type=VectorFloat (aload parm=2))))     "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(float[],float[],float[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    float output[] =  {0.0f, 0.0f, 0.0f, 0.0f};
    float inputA[] =  {6.0f, 0.0f, -9.0f, 0.6f};
    float inputB[] =  {-0.5f, 3.5f, -3.0f, 0.7f};

    entry_point(output,inputA,inputB);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_FLOAT_EQ(inputA[i] / inputB[i], output[i]); // Epsilon = 4ULP -- is this necessary?
    }
}

TEST_F(VectorTest, VDoubleDiv) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address]           "
                     "  (block                                                        "
                     "     (vstorei type=VectorDouble offset=0                        "
                     "         (aload parm=0)                                         "
                     "            (vdiv                                               "
                     "                 (vloadi type=VectorDouble (aload parm=1))      "
                     "                 (vloadi type=VectorDouble (aload parm=2))))    "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    //
    // This test is currently disabled on Z platforms because not all Z platforms
    // have vector support. Determining whether a specific platform has the support
    // at runtime is currently not possible in tril. So the test is being disabled altogether
    // on Z for now.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(double[],double[],double[])>();
    //TODO: What do we query to determine vector width?
    // -- This test currently assumes 128bit SIMD

    double output[] =  {0.0, 0.0};
    double inputA[] =  {12.0, -1.5};
    double inputB[] =  {-4.0, -3.0};

    entry_point(output,inputA,inputB);
    EXPECT_DOUBLE_EQ(inputA[0] / inputB[0], output[0]); // Epsilon = 4ULP -- is this necessary?
    EXPECT_DOUBLE_EQ(inputA[1] / inputB[1], output[1]); // Epsilon = 4ULP -- is this necessary?
}

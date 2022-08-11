/*******************************************************************************
 * Copyright (c) 2017, 2022 IBM Corp. and others
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
#include "compilerunittest/CompilerUnitTest.hpp"

#define MAX(x, y) (x > y) ? x : y
#define ABS(x) (x < 0) ? -x : x

class VectorTest : public TRTest::JitTest {};

class ParameterizedBinaryVectorArithmeticTest : public VectorTest, public ::testing::WithParamInterface<std::tuple<TR::ILOpCode, TR::VectorLength>> {};

int vectorSize(TR::VectorLength vl) {
    switch (vl) {
        case TR::VectorLength64:
            return 8;
        case TR::VectorLength128:
            return 16;
        case TR::VectorLength256:
            return 32;
        case TR::VectorLength512:
            return 64;
        default:
            TR_ASSERT_FATAL(0, "Illegal vector length");
            return 0;
    }
}

int typeSize(TR::DataTypes dt) {
    switch (dt) {
        case TR::Int8:
            return 1;
        case TR::Int16:
            return 2;
        case TR::Int32:
            return 4;
        case TR::Int64:
            return 8;
        case TR::Float:
            return 4;
        case TR::Double:
            return 8;
        default:
            TR_ASSERT_FATAL(0, "Illegal data type");
            return 0;
    }
}

void compareResults(void *expected, void *actual, TR::DataTypes dt, TR::VectorLength vl) {
    int lengthBytes = vectorSize(vl);
    int elementBytes = typeSize(dt);

    for (int i = 0; i < lengthBytes; i += elementBytes) {
        switch (dt) {
            case TR::Int8:
                EXPECT_EQ(*((int8_t *) expected), *((int8_t *) actual));
                break;
            case TR::Int16:
                EXPECT_EQ(*((int16_t *) expected), *((int16_t *) actual));
                break;
            case TR::Int32:
                EXPECT_EQ(*((int32_t *) expected), *((int32_t *) actual));
                break;
            case TR::Int64:
                EXPECT_EQ(*((int64_t *) expected), *((int64_t *) actual));
                break;
            case TR::Float:
                EXPECT_FLOAT_EQ(*((float *) expected), *((float *) actual));
                break;
            case TR::Double:
                EXPECT_DOUBLE_EQ(*((double *) expected), *((double *) actual));
                break;
            default:
                TR_ASSERT_FATAL(0, "Illegal type to compare");
                break;
        }
        expected = static_cast<char *>(expected) + elementBytes;
        actual = static_cast<char *>(actual) + elementBytes;
    }
}

void generateByType(void *output, TR::DataType dt, bool nonZero) {
    switch (dt) {
        case TR::Int8:
            *((int8_t *) output) = -128 + static_cast<int8_t>(rand() % 255);
            if (nonZero && *((int8_t *) output) == 0) *((int8_t *) output) = 1;
            break;
        case TR::Int16:
            *((int16_t *) output) = -200 + static_cast<int16_t>(rand() % 400);
            if (nonZero && *((int16_t *) output) == 0) *((int16_t *) output) = 1;
            break;
        case TR::Int32:
            *((int32_t *) output) = -1000 + static_cast<int32_t>(rand() % 2000);
            if (nonZero && *((int32_t *) output) == 0) *((int32_t *) output) = 1;
            break;
        case TR::Int64:
            *((int64_t *) output) = -1000 + static_cast<int64_t>(rand() % 2000);
            if (nonZero && *((int64_t *) output) == 0) *((int64_t *) output) = 1;
            break;
        case TR::Float:
            *((float *) output) = static_cast<float>(rand() / 1000.0);
            break;
        case TR::Double:
            *((double *) output) = static_cast<double>(rand() / 1000.0);
            break;
    }
}

void generateIO(TR::ILOpCode scalarOpcode, TR::VectorLength vl, void *output, void *inputA, void *inputB) {
    TR::ILOpCode vectorOpcode = OMR::ILOpCode::convertScalarToVector(scalarOpcode.getOpCodeValue(), vl);
    TR::DataType elementType = vectorOpcode.getType().getVectorElementType();
    TR::ILOpCode storeOpcode = OMR::ILOpCode::indirectStoreOpCode(elementType);
    TR::ILOpCode loadOpcode = OMR::ILOpCode::indirectLoadOpCode(elementType);
    char inputTrees[1024];

    if (inputB) {
        std::snprintf(inputTrees, sizeof(inputTrees),
                      "(method return=NoType args=[Address, Address, Address]"
                      "  (block"
                      "    (%s"
                      "      (aload parm=0)"
                      "        (%s"
                      "          (%s (aload parm=1))"
                      "          (%s (aload parm=2))))"
                      "    (return)))",
                      storeOpcode.getName(),
                      scalarOpcode.getName(),
                      loadOpcode.getName(),
                      loadOpcode.getName()
        );
    } else {
        std::snprintf(inputTrees, sizeof(inputTrees),
                      "(method return=NoType args=[Address, Address]"
                      "  (block"
                      "    (%s"
                      "      (aload parm=0)"
                      "        (%s"
                      "          (%s (aload parm=1))))"
                      "    (return)))",
                      storeOpcode.getName(),
                      scalarOpcode.getName(),
                      loadOpcode.getName()
        );
    }

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    int numBytes = vectorSize(vl);
    int elementSize = typeSize(elementType.getDataType());

    int numElements = numBytes / elementSize;

    if (inputB) {
        auto entry_point = compiler.getEntryPoint < void(*)(void * , void *, void *) > ();
        void *aOff = inputA;
        void *bOff = inputB;
        void *outOff = output;

        for (int i = 0; i < numElements; i++) {
            generateByType(aOff, elementType, scalarOpcode.isDiv());
            generateByType(bOff, elementType, scalarOpcode.isDiv());

            entry_point(outOff, aOff, bOff);

            aOff = static_cast<char *>(aOff) + elementSize;
            bOff = static_cast<char *>(bOff) + elementSize;
            outOff = static_cast<char *>(outOff) + elementSize;
        }
    } else {
        auto entry_point = compiler.getEntryPoint < void(*)(void * , void *) > ();
        void *aOff = inputA;
        void *outOff = output;

        for (int i = 0; i < numElements; i++) {
            generateByType(aOff, elementType, scalarOpcode.isDiv());
            entry_point(outOff, aOff);

            aOff = static_cast<char *>(aOff) + elementSize;
            outOff = static_cast<char *>(outOff) + elementSize;
        }
    }
}

void generateAndExecuteVectorTest(TR::ILOpCode vectorOpcode, void *expected, void *inputA, void *inputB) {
    TR::VectorLength vl = vectorOpcode.getType().getVectorLength();
    TR::DataType elementType = vectorOpcode.getType().getVectorElementType();
    TR::DataType vt = TR::DataType::createVectorType(elementType.getDataType(), vl);
    TR::ILOpCode loadOp = TR::ILOpCode::createVectorOpCode(TR::vloadi, vt);
    TR::ILOpCode storeOp = TR::ILOpCode::createVectorOpCode(TR::vstorei, vt);

    char type[64];
    char inputTrees[1024];

    std::snprintf(type, sizeof(type), "Vector%i%s", vectorSize(vl) * 8, TR::DataType::getName(elementType));

    if (vectorOpcode.expectedChildCount() == 1) {
        std::snprintf(inputTrees, sizeof(inputTrees),
                      "(method return= NoType args=[Address,Address]                   "
                      "  (block                                                        "
                      "     (%s%s  offset=0                                            "
                      "         (aload parm=0)                                         "
                      "            (%s%s                                               "
                      "                 (%s%s (aload parm=1))))                        "
                      "     (return)))                                                 ",

                      storeOp.getName(),
                      type,
                      vectorOpcode.getName(),
                      type,
                      loadOp.getName(),
                      type
        );
    } else {
        std::snprintf(inputTrees, sizeof(inputTrees),
                      "(method return= NoType args=[Address,Address,Address]           "
                      "  (block                                                        "
                      "     (%s%s  offset=0                                            "
                      "         (aload parm=0)                                         "
                      "            (%s%s                                               "
                      "                 (%s%s (aload parm=1))                          "
                      "                 (%s%s (aload parm=2))))                        "
                      "     (return)))                                                 ",

                      storeOp.getName(),
                      type,
                      vectorOpcode.getName(),
                      type,
                      loadOp.getName(),
                      type,
                      loadOp.getName(),
                      type
        );
    }

    auto trees = parseString(inputTrees);
    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    uint8_t result[64];
    if (vectorOpcode.expectedChildCount() == 1) {
        auto entry_point = compiler.getEntryPoint < void(*)(void * , void *) > ();
        entry_point(result, inputA);
    } else {
        auto entry_point = compiler.getEntryPoint < void(*)(void * , void *, void *) > ();
        entry_point(result, inputA, inputB);
    }

    compareResults(expected, result, elementType.getDataType(), vl);
}

TEST_P(ParameterizedBinaryVectorArithmeticTest, VLoadStore) {
    TR::ILOpCode scalarOpcode = std::get<0>(GetParam());
    TR::VectorLength vl = std::get<1>(GetParam());
    TR::DataTypes et = scalarOpcode.getType().getDataType();

    SKIP_IF(vl > TR::NumVectorLengths, MissingImplementation) << "Vector length is not supported by the target platform";
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";

    TR::ILOpCode vectorOpcode = OMR::ILOpCode::convertScalarToVector(scalarOpcode.getOpCodeValue(), vl);
    ASSERT_NE(TR::BadILOp, vectorOpcode.getOpCodeValue());
    TR::DataType elementType = vectorOpcode.getType().getVectorElementType();
    TR::DataType vt = TR::DataType::createVectorType(et, vl);
    TR::ILOpCode loadOp = TR::ILOpCode::createVectorOpCode(TR::vloadi, vt);
    TR::ILOpCode storeOp = TR::ILOpCode::createVectorOpCode(TR::vstorei, vt);

    char type[64];
    std::snprintf(type, sizeof(type), "Vector%i%s", vectorSize(vl) * 8, TR::DataType::getName(elementType));

    TR::CPU cpu = TR::CPU::detect(privateOmrPortLibrary);
    bool platformSupport = TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, loadOp) &&
                           TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, storeOp) &&
                           TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, vectorOpcode);

    SKIP_IF(!platformSupport, MissingImplementation) << "Opcode " << vectorOpcode.getName() << type << " is not supported by the target platform";

    uint8_t expected[128];
    uint8_t a[128];
    uint8_t b[128];

    generateIO(scalarOpcode, vl, expected, a, vectorOpcode.expectedChildCount() == 1 ? NULL : b);
    generateAndExecuteVectorTest(vectorOpcode, expected, a, b);
}

#define ALL_VL(opcode) \
    std::make_tuple(opcode, TR::VectorLength128),\
    std::make_tuple(opcode, TR::VectorLength256),\
    std::make_tuple(opcode, TR::VectorLength512)

INSTANTIATE_TEST_CASE_P(VectorArithmetic, ParameterizedBinaryVectorArithmeticTest, ::testing::ValuesIn(*TRTest::MakeVector<std::tuple<TR::ILOpCode, TR::VectorLength>>(
    /* vadd */

    ALL_VL(TR::badd),
    ALL_VL(TR::sadd),
    ALL_VL(TR::iadd),
    ALL_VL(TR::ladd),
    ALL_VL(TR::dadd),
    ALL_VL(TR::fadd),

    /* vsub */

    ALL_VL(TR::bsub),
    ALL_VL(TR::ssub),
    ALL_VL(TR::isub),
    ALL_VL(TR::lsub),
    ALL_VL(TR::fsub),
    ALL_VL(TR::dsub),

    /* vmul */

    ALL_VL(TR::bmul),
    ALL_VL(TR::smul),
    ALL_VL(TR::imul),
    ALL_VL(TR::lmul),
    ALL_VL(TR::fmul),
    ALL_VL(TR::dmul),

    /* vdiv */

    ALL_VL(TR::bdiv),
    ALL_VL(TR::sdiv),
    ALL_VL(TR::idiv),
    ALL_VL(TR::ldiv),
    ALL_VL(TR::fdiv),
    ALL_VL(TR::ddiv),

    /* vand */

    ALL_VL(TR::band),
    ALL_VL(TR::sand),
    ALL_VL(TR::iand),
    ALL_VL(TR::land),

    /* vor */

    ALL_VL(TR::bor),
    ALL_VL(TR::sor),
    ALL_VL(TR::ior),
    ALL_VL(TR::lor),

    /* vxor */

    ALL_VL(TR::bxor),
    ALL_VL(TR::sxor),
    ALL_VL(TR::ixor),
    ALL_VL(TR::lxor),

    /* vmin */

    /* No opcode for bmin, smin */

    ALL_VL(TR::imin),
    ALL_VL(TR::lmin),

    /* fmin is not supported */
    /* dmin is not supported */

    /* vmax */

    /* No opcode for bmax, smax */

    ALL_VL(TR::imax),
    ALL_VL(TR::lmax),

    /* fmax is not supported */
    /* dmax is not supported */

    /* ABS */

    ALL_VL(TR::iabs),
    ALL_VL(TR::labs),
    ALL_VL(TR::fabs),
    ALL_VL(TR::dabs),

    /* vsqrt */

    /* No opcode for bsqrt, ssqrt, isqrt, lsqrt */
    ALL_VL(TR::fsqrt),
    ALL_VL(TR::dsqrt),

    /* vneg */

    ALL_VL(TR::bneg),
    ALL_VL(TR::sneg),
    ALL_VL(TR::ineg),
    ALL_VL(TR::lneg),
    ALL_VL(TR::fneg),
    ALL_VL(TR::dneg)
)));


class ParameterizedVectorTest : public VectorTest, public ::testing::WithParamInterface<std::tuple<TR::VectorLength, TR::DataTypes>> {};

TEST_P(ParameterizedVectorTest, VLoadStore) {
    TR::VectorLength vl = std::get<0>(GetParam());
    TR::DataTypes et = std::get<1>(GetParam());

    SKIP_IF(vl > TR::NumVectorLengths, MissingImplementation) << "Vector length is not supported by the target platform";
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";

    TR::DataType vt = TR::DataType::createVectorType(et, vl);

    TR::ILOpCode loadOp = TR::ILOpCode::createVectorOpCode(TR::vloadi, vt);
    TR::ILOpCode storeOp = TR::ILOpCode::createVectorOpCode(TR::vstorei, vt);
    TR::CPU cpu = TR::CPU::detect(privateOmrPortLibrary);
    bool platformSupport = TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, loadOp) && TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, storeOp);
    SKIP_IF(!platformSupport, MissingImplementation) << "Opcode is not supported by the target platform";

    char inputTrees[1024];
    char *formatStr = "(method return= NoType args=[Address,Address]   "
                      "  (block                                        "
                      "     (vstorei%s  offset=0                       "
                      "         (aload parm=0)                         "
                      "         (vloadi%s (aload parm=1)))             "
                      "     (return)))                                 ";

    sprintf(inputTrees, formatStr, vt.toString(), vt.toString());
    auto trees = parseString(inputTrees);
    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<void (*)(void *,void *)>();

    const uint8_t maxVectorLength = 64;
    char output[maxVectorLength] = {0};
    char input[maxVectorLength] = {0};
    char zero[maxVectorLength] = {0};

    for (int i = 0; i < maxVectorLength; i++) {
        input[i] = i;
    }

    entry_point(output, input);

    EXPECT_EQ(0, memcmp(input, output, TR::DataType::getSize(vt)));
    EXPECT_EQ(0, memcmp(output + TR::DataType::getSize(vt), zero, maxVectorLength - TR::DataType::getSize(vt)));
}

TEST_P(ParameterizedVectorTest, VSplats) {
    TR::VectorLength vl = std::get<0>(GetParam());
    TR::DataTypes et = std::get<1>(GetParam());

    SKIP_IF(vl > TR::NumVectorLengths, MissingImplementation) << "Vector length is not supported by the target platform";
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";

    TR::DataType vt = TR::DataType::createVectorType(et, vl);

    TR::ILOpCode loadOp = TR::ILOpCode::createVectorOpCode(TR::vloadi, vt);
    TR::ILOpCode storeOp = TR::ILOpCode::createVectorOpCode(TR::vstorei, vt);
    TR::ILOpCode splatsOp = TR::ILOpCode::createVectorOpCode(TR::vsplats, vt);
    TR::ILOpCode elementLoadOp = OMR::ILOpCode::indirectLoadOpCode(et);
    TR::CPU cpu = TR::CPU::detect(privateOmrPortLibrary);

    bool platformSupport = TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, loadOp) &&
            TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, storeOp) &&
            TR::CodeGenerator::getSupportsOpCodeForAutoSIMD(&cpu, splatsOp);

    SKIP_IF(!platformSupport, MissingImplementation) << "Opcode " << splatsOp.getName() << vt.toString() << " is not supported by the target platform";

    char inputTrees[1024];
    char *formatStr = "(method return= NoType args=[Address,Address]   "
                      "  (block                                        "
                      "     (vstorei%s  offset=0                       "
                      "         (aload parm=0)                         "
                      "         (vsplats%s                             "
                      "             (%s (aload parm=1))))              "
                      "     (return)))                                 ";

    sprintf(inputTrees, formatStr, vt.toString(), vt.toString(), elementLoadOp.getName());

    auto trees = parseString(inputTrees);
    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    auto entry_point = compiler.getEntryPoint<void (*)(void *,void *)>();

    const uint8_t maxVectorLength = 64;
    char output[maxVectorLength] = {0};
    char expected[maxVectorLength] = {0};
    char input[maxVectorLength] = {0};

    int etSize = typeSize(et);
    int vlSize = vectorSize(vl);
    generateByType(input, et, false);

    for (int i = 0; i < vlSize; i += etSize) {
        memcpy(expected + i, input, etSize);
    }

    entry_point(output, input);

    EXPECT_EQ(0, memcmp(expected, output, vlSize));
}

INSTANTIATE_TEST_CASE_P(VectorTypeParameters, ParameterizedVectorTest, ::testing::ValuesIn(*TRTest::MakeVector<std::tuple<TR::VectorLength, TR::DataTypes>>(
    std::make_tuple(TR::VectorLength128, TR::Int8),
    std::make_tuple(TR::VectorLength128, TR::Int16),
    std::make_tuple(TR::VectorLength128, TR::Int32),
    std::make_tuple(TR::VectorLength128, TR::Int64),
    std::make_tuple(TR::VectorLength128, TR::Float),
    std::make_tuple(TR::VectorLength128, TR::Double),
    std::make_tuple(TR::VectorLength256, TR::Int8),
    std::make_tuple(TR::VectorLength256, TR::Int16),
    std::make_tuple(TR::VectorLength256, TR::Int32),
    std::make_tuple(TR::VectorLength256, TR::Int64),
    std::make_tuple(TR::VectorLength256, TR::Float),
    std::make_tuple(TR::VectorLength256, TR::Double),
    std::make_tuple(TR::VectorLength512, TR::Int8),
    std::make_tuple(TR::VectorLength512, TR::Int16),
    std::make_tuple(TR::VectorLength512, TR::Int32),
    std::make_tuple(TR::VectorLength512, TR::Int64),
    std::make_tuple(TR::VectorLength512, TR::Float),
    std::make_tuple(TR::VectorLength512, TR::Double)
)));

TEST_F(VectorTest, VInt8Not) {

   auto inputTrees = "(method return= NoType args=[Address,Address]                   "
                     "  (block                                                        "
                     "     (vstoreiVector128Int8 offset=0                             "
                     "         (aload parm=0)                                         "
                     "            (vnotVector128Int8                                  "
                     "                 (vloadiVector128Int8 (aload parm=1))))         "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_X86(MissingImplementation);
    SKIP_ON_HAMMER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int8_t[],int8_t[])>();
    // This test currently assumes 128bit SIMD

    int8_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t inputA[] =  {7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, 7};

    entry_point(output,inputA);

    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(~inputA[i], output[i]);
    }
}

TEST_F(VectorTest, VInt8BitSelect) {

   auto inputTrees = "(method return= NoType args=[Address,Address,Address,Address]   "
                     "  (block                                                        "
                     "     (vstoreiVector128Int8 offset=0                             "
                     "         (aload parm=0)                                         "
                     "            (vbitselectVector128Int8                            "
                     "                 (vloadiVector128Int8 (aload parm=1))           "
                     "                 (vloadiVector128Int8 (aload parm=2))           "
                     "                 (vloadiVector128Int8 (aload parm=3))))         "
                     "     (return)))                                                 ";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);
    //TODO: Re-enable this test on S390 after issue #1843 is resolved.
    SKIP_ON_S390(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_S390X(KnownBug) << "This test is currently disabled on Z platforms because not all Z platforms have vector support (issue #1843)";
    SKIP_ON_RISCV(MissingImplementation);
    SKIP_ON_X86(MissingImplementation);
    SKIP_ON_HAMMER(MissingImplementation);

    Tril::DefaultCompiler compiler(trees);
    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;


    auto entry_point = compiler.getEntryPoint<void (*)(int8_t[],int8_t[],int8_t[],int8_t[])>();
    // This test currently assumes 128bit SIMD

    int8_t output[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t inputA[] =  {8, -3, 62, 56, -108, -13, 114, -100, 69, -80, 6, 104, 67, 78, 12, -72};
    int8_t inputB[] =  {55, 107, -12, 39, 77, 103, -3, 15, -17, -16, -62, -41, 71, 77, 111, -119};
    int8_t inputC[] =  {-121, 28, -85, 63, 59, 19, 21, 95, -14, -21, 8, -41, 8, 103, -100, -16};

    entry_point(output,inputA,inputB,inputC);

    // The expected result is a^((a^b)&c), which extracts bits from b if the corresponding bit of c is 1.
    for (int i = 0; i < (sizeof(output) / sizeof(*output)); i++) {
        EXPECT_EQ(inputA[i] ^ ((inputA[i] ^ inputB[i]) & inputC[i]), output[i]);
    }
}

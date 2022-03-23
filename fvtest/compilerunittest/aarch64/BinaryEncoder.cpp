/*******************************************************************************
 * Copyright (c) 2022, 2022 IBM Corp. and others
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

#include <gtest/gtest.h>
#include "../CodeGenTest.hpp"
#include "codegen/GenerateInstructions.hpp"

#define ARM64_INSTRUCTION_ALIGNMENT 32

class ARM64BinaryInstruction : public TRTest::BinaryInstruction {
public:

    ARM64BinaryInstruction() : TRTest::BinaryInstruction() {
    }
    ARM64BinaryInstruction(const char *instr) : TRTest::BinaryInstruction(instr) {
        // As the tests are encoded as big endian strings, we need to convert them to little endian
        for (int i = 0; i < _size / sizeof(uint32_t); i++) {
            std::swap(_buf[i * sizeof(uint32_t)], _buf[i * sizeof(uint32_t) + 3]);
            std::swap(_buf[i * sizeof(uint32_t) + 1], _buf[i * sizeof(uint32_t) + 2]);
        }
    }
};

std::ostream &operator<<(std::ostream &os, const ARM64BinaryInstruction &instr) {
    os << static_cast<TRTest::BinaryInstruction>(instr);
    return os;
}

class ARM64Trg1ImmEncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, uint32_t, ARM64BinaryInstruction>> {};

TEST_P(ARM64Trg1ImmEncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto imm = std::get<2>(GetParam());

    auto instr = generateTrg1ImmInstruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, imm);

    ASSERT_EQ(std::get<3>(GetParam()), encodeInstruction(instr));
}

class ARM64Trg1ImmShiftedEncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, uint32_t, uint32_t, ARM64BinaryInstruction>> {};

TEST_P(ARM64Trg1ImmShiftedEncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto imm = std::get<2>(GetParam());
    auto shiftAmount = std::get<3>(GetParam());

    auto instr = generateTrg1ImmShiftedInstruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, imm, shiftAmount);

    ASSERT_EQ(std::get<4>(GetParam()), encodeInstruction(instr));
}

class ARM64Trg1Src1EncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, TR::RealRegister::RegNum, ARM64BinaryInstruction>> {};

TEST_P(ARM64Trg1Src1EncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto src1Reg = cg()->machine()->getRealRegister(std::get<2>(GetParam()));

    auto instr = generateTrg1Src1Instruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, src1Reg);

    ASSERT_EQ(std::get<3>(GetParam()), encodeInstruction(instr));
}

class ARM64Trg1Src2EncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, TR::RealRegister::RegNum, TR::RealRegister::RegNum, ARM64BinaryInstruction>> {};

TEST_P(ARM64Trg1Src2EncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto src1Reg = cg()->machine()->getRealRegister(std::get<2>(GetParam()));
    auto src2Reg = cg()->machine()->getRealRegister(std::get<3>(GetParam()));

    auto instr = generateTrg1Src2Instruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, src1Reg, src2Reg);

    ASSERT_EQ(std::get<4>(GetParam()), encodeInstruction(instr));
}

class ARM64VectorShiftImmediateEncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, TR::RealRegister::RegNum, uint32_t, ARM64BinaryInstruction>> {};

TEST_P(ARM64VectorShiftImmediateEncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto src1Reg = cg()->machine()->getRealRegister(std::get<2>(GetParam()));

    auto instr = generateVectorShiftImmediateInstruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, src1Reg, std::get<3>(GetParam()));

    ASSERT_EQ(std::get<4>(GetParam()), encodeInstruction(instr));
}

INSTANTIATE_TEST_CASE_P(MOV, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x3, 0, "d2800003"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x3, 0xffff, "d29fffe3"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x3, 0x1ffff, "d2bfffe3"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x3, 0x2ffff, "d2dfffe3"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x3, 0x3ffff, "d2ffffe3"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x28, 0, "d280001c"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x28, 0xffff, "d29ffffc"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x28, 0x1ffff, "d2bffffc"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x28, 0x2ffff, "d2dffffc"),
    std::make_tuple(TR::InstOpCode::movzx,  TR::RealRegister::x28, 0x3ffff, "d2fffffc"),

    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x3, 0, "f2800003"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x3, 0xffff, "f29fffe3"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x3, 0x1ffff, "f2bfffe3"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x3, 0x2ffff, "f2dfffe3"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x3, 0x3ffff, "f2ffffe3"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x28, 0, "f280001c"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x28, 0xffff, "f29ffffc"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x28, 0x1ffff, "f2bffffc"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x28, 0x2ffff, "f2dffffc"),
    std::make_tuple(TR::InstOpCode::movkx,  TR::RealRegister::x28, 0x3ffff, "f2fffffc"),

    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x3, 0, "92800003"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x3, 0xffff, "929fffe3"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x3, 0x1ffff, "92bfffe3"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x3, 0x2ffff, "92dfffe3"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x3, 0x3ffff, "92ffffe3"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x28, 0, "9280001c"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x28, 0xffff, "929ffffc"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x28, 0x1ffff, "92bffffc"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x28, 0x2ffff, "92dffffc"),
    std::make_tuple(TR::InstOpCode::movnx,  TR::RealRegister::x28, 0x3ffff, "92fffffc"),

    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x3, 0, "52800003"),
    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x3, 0xffff, "529fffe3"),
    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x3, 0x1ffff, "52bfffe3"),
    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x28, 0, "5280001c"),
    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x28, 0xffff, "529ffffc"),
    std::make_tuple(TR::InstOpCode::movzw,  TR::RealRegister::x28, 0x1ffff, "52bffffc"),

    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x3, 0, "72800003"),
    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x3, 0xffff, "729fffe3"),
    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x3, 0x1ffff, "72bfffe3"),
    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x28, 0, "7280001c"),
    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x28, 0xffff, "729ffffc"),
    std::make_tuple(TR::InstOpCode::movkw,  TR::RealRegister::x28, 0x1ffff, "72bffffc"),

    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x3, 0, "12800003"),
    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x3, 0xffff, "129fffe3"),
    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x3, 0x1ffff, "12bfffe3"),
    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x28, 0, "1280001c"),
    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x28, 0xffff, "129ffffc"),
    std::make_tuple(TR::InstOpCode::movnw,  TR::RealRegister::x28, 0x1ffff, "12bffffc")
));

INSTANTIATE_TEST_CASE_P(FMOV, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::fmovimmd,  TR::RealRegister::v0, 0, "1e601000"),    // abcdefgh = 0, expanded to 2.0
    std::make_tuple(TR::InstOpCode::fmovimmd,  TR::RealRegister::v0, 0xff, "1e7ff000"), // abcdefgh = 0xff, expanded to -1.9375
    std::make_tuple(TR::InstOpCode::fmovimmd,  TR::RealRegister::v31, 0, "1e60101f"),
    std::make_tuple(TR::InstOpCode::fmovimmd,  TR::RealRegister::v31, 0xff, "1e7ff01f"),

    std::make_tuple(TR::InstOpCode::fmovimms,  TR::RealRegister::v0, 0, "1e201000"),
    std::make_tuple(TR::InstOpCode::fmovimms,  TR::RealRegister::v0, 0xff, "1e3ff000"),
    std::make_tuple(TR::InstOpCode::fmovimms,  TR::RealRegister::v31, 0, "1e20101f"),
    std::make_tuple(TR::InstOpCode::fmovimms,  TR::RealRegister::v31, 0xff, "1e3ff01f"),

    std::make_tuple(TR::InstOpCode::vfmov2d,  TR::RealRegister::v0, 0, "6f00f400"),
    std::make_tuple(TR::InstOpCode::vfmov2d,  TR::RealRegister::v0, 0xff, "6f07f7e0"),
    std::make_tuple(TR::InstOpCode::vfmov2d,  TR::RealRegister::v31, 0, "6f00f41f"),
    std::make_tuple(TR::InstOpCode::vfmov2d,  TR::RealRegister::v31, 0xff, "6f07f7ff"),

    std::make_tuple(TR::InstOpCode::vfmov4s,  TR::RealRegister::v0, 0, "4f00f400"),
    std::make_tuple(TR::InstOpCode::vfmov4s,  TR::RealRegister::v0, 0xff, "4f07f7e0"),
    std::make_tuple(TR::InstOpCode::vfmov4s,  TR::RealRegister::v31, 0, "4f00f41f"),
    std::make_tuple(TR::InstOpCode::vfmov4s,  TR::RealRegister::v31, 0xff, "4f07f7ff")
));

INSTANTIATE_TEST_CASE_P(MOVI, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v0, 0, "4f00e400"),
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v0, 0x55, "4f02e6a0"),
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v0, 0xff, "4f07e7e0"),
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v31, 0, "4f00e41f"),
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v31, 0x55, "4f02e6bf"),
    std::make_tuple(TR::InstOpCode::vmovi16b,  TR::RealRegister::v31, 0xff, "4f07e7ff"),

    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v0, 0, "4f008400"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v0, 0x55, "4f0286a0"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v0, 0xff, "4f0787e0"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v31, 0, "4f00841f"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v31, 0x55, "4f0286bf"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v31, 0xff, "4f0787ff"),

    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0, "4f000400"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0x55, "4f0206a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0xff, "4f0707e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0, "4f00041f"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0x55, "4f0206bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0xff, "4f0707ff"),

    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v0, 0, "6f00e400"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v0, 0xaa, "6f05e540"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v0, 0x55, "6f02e6a0"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v0, 0xff, "6f07e7e0"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v31, 0, "6f00e41f"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v31, 0xaa, "6f05e55f"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v31, 0x55, "6f02e6bf"),
    std::make_tuple(TR::InstOpCode::vmovi2d,  TR::RealRegister::v31, 0xff, "6f07e7ff"),

    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v0, 0, "2f00e400"),
    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v0, 0x55, "2f02e6a0"),
    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v0, 0xff, "2f07e7e0"),
    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v31, 0, "2f00e41f"),
    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v31, 0x55, "2f02e6bf"),
    std::make_tuple(TR::InstOpCode::movid,  TR::RealRegister::v31, 0xff, "2f07e7ff"),

    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v0, 0, "0f000400"),
    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v0, 0x55, "0f0206a0"),
    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v0, 0xff, "0f0707e0"),
    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v31, 0, "0f00041f"),
    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v31, 0x55, "0f0206bf"),
    std::make_tuple(TR::InstOpCode::vmovi2s,  TR::RealRegister::v31, 0xff, "0f0707ff")
));

INSTANTIATE_TEST_CASE_P(MVNI, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v0, 0, "6f008400"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v0, 0x55, "6f0286a0"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v0, 0xff, "6f0787e0"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v31, 0, "6f00841f"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v31, 0x55, "6f0286bf"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v31, 0xff, "6f0787ff"),

    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0, "6f000400"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0x55, "6f0206a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0xff, "6f0707e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0, "6f00041f"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0x55, "6f0206bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0xff, "6f0707ff")
));

INSTANTIATE_TEST_CASE_P(BIC, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v0, 0x0, "6f009400"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v0, 0x55, "6f0296a0"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v0, 0xff, "6f0797e0"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v31, 0x0, "6f00941f"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v31, 0x55, "6f0296bf"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v31, 0xff, "6f0797ff"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0x0, "6f001400"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0x55, "6f0216a0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0xff, "6f0717e0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0x0, "6f00141f"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0x55, "6f0216bf"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0xff, "6f0717ff")
));

INSTANTIATE_TEST_CASE_P(ORR, ARM64Trg1ImmEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v0, 0x0, "4f009400"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v0, 0x55, "4f0296a0"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v0, 0xff, "4f0797e0"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v31, 0x0, "4f00941f"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v31, 0x55, "4f0296bf"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v31, 0xff, "4f0797ff"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0x0, "4f001400"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0x55, "4f0216a0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0xff, "4f0717e0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0x0, "4f00141f"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0x55, "4f0216bf"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0xff, "4f0717ff")
));

INSTANTIATE_TEST_CASE_P(MOVI, ARM64Trg1ImmShiftedEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v0, 0x55, 8, "4f02a6a0"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v0, 0xff, 8, "4f07a7e0"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v31, 0x55, 8, "4f02a6bf"),
    std::make_tuple(TR::InstOpCode::vmovi8h,  TR::RealRegister::v31, 0xff, 8, "4f07a7ff"),

    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0x55, 8, "4f0226a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0xff, 8, "4f0727e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0x55, 16, "4f0246a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0xff, 16, "4f0747e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0x55, 24, "4f0266a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v0, 0xff, 24, "4f0767e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0x55, 8, "4f0226bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0xff, 8, "4f0727ff"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0x55, 16, "4f0246bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0xff, 16, "4f0747ff"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0x55, 24, "4f0266bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s,  TR::RealRegister::v31, 0xff, 24, "4f0767ff"),

    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v0, 0x55, 8, "4f02c6a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v0, 0xff, 8, "4f07c7e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v0, 0x55, 16, "4f02d6a0"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v0, 0xff, 16, "4f07d7e0"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v31, 0x55, 8, "4f02c6bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v31, 0xff, 8, "4f07c7ff"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v31, 0x55, 16, "4f02d6bf"),
    std::make_tuple(TR::InstOpCode::vmovi4s_one,  TR::RealRegister::v31, 0xff, 16, "4f07d7ff")
));

INSTANTIATE_TEST_CASE_P(MVNI, ARM64Trg1ImmShiftedEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v0, 0x55, 8, "6f02a6a0"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v0, 0xff, 8, "6f07a7e0"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v31, 0x55, 8, "6f02a6bf"),
    std::make_tuple(TR::InstOpCode::vmvni8h,  TR::RealRegister::v31, 0xff, 8, "6f07a7ff"),

    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0x55, 8, "6f0226a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0xff, 8, "6f0727e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0x55, 16, "6f0246a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0xff, 16, "6f0747e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0x55, 24, "6f0266a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v0, 0xff, 24, "6f0767e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0x55, 8, "6f0226bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0xff, 8, "6f0727ff"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0x55, 16, "6f0246bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0xff, 16, "6f0747ff"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0x55, 24, "6f0266bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s,  TR::RealRegister::v31, 0xff, 24, "6f0767ff"),

    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v0, 0x55, 8, "6f02c6a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v0, 0xff, 8, "6f07c7e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v0, 0x55, 16, "6f02d6a0"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v0, 0xff, 16, "6f07d7e0"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v31, 0x55, 8, "6f02c6bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v31, 0xff, 8, "6f07c7ff"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v31, 0x55, 16, "6f02d6bf"),
    std::make_tuple(TR::InstOpCode::vmvni4s_one,  TR::RealRegister::v31, 0xff, 16, "6f07d7ff")
));

INSTANTIATE_TEST_CASE_P(BIC, ARM64Trg1ImmShiftedEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v0, 0x55, 8, "6f02b6a0"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v0, 0xff, 8, "6f07b7e0"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v31, 0x55, 8, "6f02b6bf"),
    std::make_tuple(TR::InstOpCode::vbicimm8h, TR::RealRegister::v31, 0xff, 8, "6f07b7ff"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0x55, 8, "6f0236a0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0xff, 8, "6f0737e0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0x55, 16, "6f0256a0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0xff, 16, "6f0757e0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0x55, 24, "6f0276a0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v0, 0xff, 24, "6f0777e0"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0x55, 8, "6f0236bf"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0xff, 8, "6f0737ff"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0x55, 16, "6f0256bf"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0xff, 16, "6f0757ff"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0x55, 24, "6f0276bf"),
    std::make_tuple(TR::InstOpCode::vbicimm4s, TR::RealRegister::v31, 0xff, 24, "6f0777ff")
));

INSTANTIATE_TEST_CASE_P(ORR, ARM64Trg1ImmShiftedEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v0, 0x55, 8, "4f02b6a0"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v0, 0xff, 8, "4f07b7e0"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v31, 0x55, 8, "4f02b6bf"),
    std::make_tuple(TR::InstOpCode::vorrimm8h, TR::RealRegister::v31, 0xff, 8, "4f07b7ff"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0x55, 8, "4f0236a0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0xff, 8, "4f0737e0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0x55, 16, "4f0256a0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0xff, 16, "4f0757e0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0x55, 24, "4f0276a0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v0, 0xff, 24, "4f0777e0"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0x55, 8, "4f0236bf"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0xff, 8, "4f0737ff"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0x55, 16, "4f0256bf"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0xff, 16, "4f0757ff"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0x55, 24, "4f0276bf"),
    std::make_tuple(TR::InstOpCode::vorrimm4s, TR::RealRegister::v31, 0xff, 24, "4f0777ff")
));

INSTANTIATE_TEST_CASE_P(VectorSQRT, ARM64Trg1Src1EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vfsqrt4s,  TR::RealRegister::v15, TR::RealRegister::v0, "6ea1f80f"),
    std::make_tuple(TR::InstOpCode::vfsqrt4s,  TR::RealRegister::v31, TR::RealRegister::v0, "6ea1f81f"),
    std::make_tuple(TR::InstOpCode::vfsqrt4s,  TR::RealRegister::v0, TR::RealRegister::v15, "6ea1f9e0"),
    std::make_tuple(TR::InstOpCode::vfsqrt4s,  TR::RealRegister::v0, TR::RealRegister::v31, "6ea1fbe0"),

    std::make_tuple(TR::InstOpCode::vfsqrt2d,  TR::RealRegister::v15, TR::RealRegister::v0, "6ee1f80f"),
    std::make_tuple(TR::InstOpCode::vfsqrt2d,  TR::RealRegister::v31, TR::RealRegister::v0, "6ee1f81f"),
    std::make_tuple(TR::InstOpCode::vfsqrt2d,  TR::RealRegister::v0, TR::RealRegister::v15, "6ee1f9e0"),
    std::make_tuple(TR::InstOpCode::vfsqrt2d,  TR::RealRegister::v0, TR::RealRegister::v31, "6ee1fbe0")
));

INSTANTIATE_TEST_CASE_P(VectorMLA, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20cc0f"),
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20cc1f"),
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e20cde0"),
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e20cfe0"),
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2fcc00"),
    std::make_tuple(TR::InstOpCode::vfmla4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3fcc00"),

    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60cc0f"),
    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60cc1f"),
    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e60cde0"),
    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e60cfe0"),
    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6fcc00"),
    std::make_tuple(TR::InstOpCode::vfmla2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7fcc00"),

    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20940f"),
    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20941f"),
    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e2095e0"),
    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e2097e0"),
    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f9400"),
    std::make_tuple(TR::InstOpCode::vmla16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f9400"),

    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60940f"),
    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60941f"),
    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e6095e0"),
    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e6097e0"),
    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f9400"),
    std::make_tuple(TR::InstOpCode::vmla8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f9400"),

    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0940f"),
    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0941f"),
    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea095e0"),
    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea097e0"),
    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf9400"),
    std::make_tuple(TR::InstOpCode::vmla4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf9400")
));

INSTANTIATE_TEST_CASE_P(VectorMIN, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e206c0f"),
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e206c1f"),
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e206de0"),
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e206fe0"),
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f6c00"),
    std::make_tuple(TR::InstOpCode::vsmin16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f6c00"),

    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e606c0f"),
    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e606c1f"),
    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e606de0"),
    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e606fe0"),
    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f6c00"),
    std::make_tuple(TR::InstOpCode::vsmin8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f6c00"),

    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea06c0f"),
    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea06c1f"),
    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea06de0"),
    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea06fe0"),
    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf6c00"),
    std::make_tuple(TR::InstOpCode::vsmin4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf6c00"),

    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0f40f"),
    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0f41f"),
    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0f5e0"),
    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0f7e0"),
    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaff400"),
    std::make_tuple(TR::InstOpCode::vfmin4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebff400"),

    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ee0f40f"),
    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ee0f41f"),
    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0f5e0"),
    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0f7e0"),
    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eeff400"),
    std::make_tuple(TR::InstOpCode::vfmin2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4efff400")
));

INSTANTIATE_TEST_CASE_P(VectorMAX, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20640f"),
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20641f"),
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e2065e0"),
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e2067e0"),
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f6400"),
    std::make_tuple(TR::InstOpCode::vsmax16b,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f6400"),

    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60640f"),
    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60641f"),
    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e6065e0"),
    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e6067e0"),
    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f6400"),
    std::make_tuple(TR::InstOpCode::vsmax8h,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f6400"),

    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0640f"),
    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0641f"),
    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea065e0"),
    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea067e0"),
    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf6400"),
    std::make_tuple(TR::InstOpCode::vsmax4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf6400"),

    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20f40f"),
    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20f41f"),
    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e20f5e0"),
    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e20f7e0"),
    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2ff400"),
    std::make_tuple(TR::InstOpCode::vfmax4s,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3ff400"),

    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60f40f"),
    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60f41f"),
    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e60f5e0"),
    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e60f7e0"),
    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6ff400"),
    std::make_tuple(TR::InstOpCode::vfmax2d,  TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7ff400")
));

INSTANTIATE_TEST_CASE_P(VectorLogic, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e201c0f"),
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e201c1f"),
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e201de0"),
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e201fe0"),
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f1c00"),
    std::make_tuple(TR::InstOpCode::vand16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f1c00"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e601c0f"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e601c1f"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e601de0"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e601fe0"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f1c00"),
    std::make_tuple(TR::InstOpCode::vbic16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f1c00"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea01c0f"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea01c1f"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea01de0"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea01fe0"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf1c00"),
    std::make_tuple(TR::InstOpCode::vorr16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf1c00"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e201c0f"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e201c1f"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e201de0"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e201fe0"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2f1c00"),
    std::make_tuple(TR::InstOpCode::veor16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3f1c00")
));

INSTANTIATE_TEST_CASE_P(VectorABS, ARM64Trg1Src1EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vabs16b,  TR::RealRegister::v15, TR::RealRegister::v0, "4e20b80f"),
    std::make_tuple(TR::InstOpCode::vabs16b,  TR::RealRegister::v31, TR::RealRegister::v0, "4e20b81f"),
    std::make_tuple(TR::InstOpCode::vabs16b,  TR::RealRegister::v0, TR::RealRegister::v15, "4e20b9e0"),
    std::make_tuple(TR::InstOpCode::vabs16b,  TR::RealRegister::v0, TR::RealRegister::v31, "4e20bbe0"),

    std::make_tuple(TR::InstOpCode::vabs8h,  TR::RealRegister::v15, TR::RealRegister::v0, "4e60b80f"),
    std::make_tuple(TR::InstOpCode::vabs8h,  TR::RealRegister::v31, TR::RealRegister::v0, "4e60b81f"),
    std::make_tuple(TR::InstOpCode::vabs8h,  TR::RealRegister::v0, TR::RealRegister::v15, "4e60b9e0"),
    std::make_tuple(TR::InstOpCode::vabs8h,  TR::RealRegister::v0, TR::RealRegister::v31, "4e60bbe0"),

    std::make_tuple(TR::InstOpCode::vabs4s,  TR::RealRegister::v15, TR::RealRegister::v0, "4ea0b80f"),
    std::make_tuple(TR::InstOpCode::vabs4s,  TR::RealRegister::v31, TR::RealRegister::v0, "4ea0b81f"),
    std::make_tuple(TR::InstOpCode::vabs4s,  TR::RealRegister::v0, TR::RealRegister::v15, "4ea0b9e0"),
    std::make_tuple(TR::InstOpCode::vabs4s,  TR::RealRegister::v0, TR::RealRegister::v31, "4ea0bbe0"),

    std::make_tuple(TR::InstOpCode::vabs2d,  TR::RealRegister::v15, TR::RealRegister::v0, "4ee0b80f"),
    std::make_tuple(TR::InstOpCode::vabs2d,  TR::RealRegister::v31, TR::RealRegister::v0, "4ee0b81f"),
    std::make_tuple(TR::InstOpCode::vabs2d,  TR::RealRegister::v0, TR::RealRegister::v15, "4ee0b9e0"),
    std::make_tuple(TR::InstOpCode::vabs2d,  TR::RealRegister::v0, TR::RealRegister::v31, "4ee0bbe0"),

    std::make_tuple(TR::InstOpCode::vfabs4s,  TR::RealRegister::v15, TR::RealRegister::v0, "4ea0f80f"),
    std::make_tuple(TR::InstOpCode::vfabs4s,  TR::RealRegister::v31, TR::RealRegister::v0, "4ea0f81f"),
    std::make_tuple(TR::InstOpCode::vfabs4s,  TR::RealRegister::v0, TR::RealRegister::v15, "4ea0f9e0"),
    std::make_tuple(TR::InstOpCode::vfabs4s,  TR::RealRegister::v0, TR::RealRegister::v31, "4ea0fbe0"),

    std::make_tuple(TR::InstOpCode::vfabs2d,  TR::RealRegister::v15, TR::RealRegister::v0, "4ee0f80f"),
    std::make_tuple(TR::InstOpCode::vfabs2d,  TR::RealRegister::v31, TR::RealRegister::v0, "4ee0f81f"),
    std::make_tuple(TR::InstOpCode::vfabs2d,  TR::RealRegister::v0, TR::RealRegister::v15, "4ee0f9e0"),
    std::make_tuple(TR::InstOpCode::vfabs2d,  TR::RealRegister::v0, TR::RealRegister::v31, "4ee0fbe0")
));

INSTANTIATE_TEST_CASE_P(VectorCMP1, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e208c0f"),
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e208c1f"),
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e208de0"),
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e208fe0"),
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2f8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3f8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e608c0f"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e608c1f"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e608de0"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e608fe0"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e6f8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e7f8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ea08c0f"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ea08c1f"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ea08de0"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ea08fe0"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eaf8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6ebf8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ee08c0f"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ee08c1f"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ee08de0"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ee08fe0"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eef8c00"),
    std::make_tuple(TR::InstOpCode::vcmeq2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6eff8c00"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e203c0f"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e203c1f"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e203de0"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e203fe0"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2f3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3f3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e603c0f"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e603c1f"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e603de0"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e603fe0"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e6f3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e7f3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ea03c0f"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ea03c1f"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ea03de0"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ea03fe0"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eaf3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6ebf3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ee03c0f"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ee03c1f"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ee03de0"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ee03fe0"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eef3c00"),
    std::make_tuple(TR::InstOpCode::vcmhs2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6eff3c00")
));
INSTANTIATE_TEST_CASE_P(VectorCMP2, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e203c0f"),
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e203c1f"),
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e203de0"),
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e203fe0"),
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f3c00"),
    std::make_tuple(TR::InstOpCode::vcmge16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f3c00"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e603c0f"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e603c1f"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e603de0"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e603fe0"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f3c00"),
    std::make_tuple(TR::InstOpCode::vcmge8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f3c00"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea03c0f"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea03c1f"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea03de0"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea03fe0"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf3c00"),
    std::make_tuple(TR::InstOpCode::vcmge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf3c00"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ee03c0f"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ee03c1f"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ee03de0"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ee03fe0"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eef3c00"),
    std::make_tuple(TR::InstOpCode::vcmge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4eff3c00"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e20340f"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e20341f"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e2035e0"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e2037e0"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2f3400"),
    std::make_tuple(TR::InstOpCode::vcmhi16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3f3400"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e60340f"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e60341f"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e6035e0"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e6037e0"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e6f3400"),
    std::make_tuple(TR::InstOpCode::vcmhi8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e7f3400"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0340f"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0341f"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ea035e0"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ea037e0"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eaf3400"),
    std::make_tuple(TR::InstOpCode::vcmhi4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6ebf3400"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0340f"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0341f"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ee035e0"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ee037e0"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eef3400"),
    std::make_tuple(TR::InstOpCode::vcmhi2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6eff3400")
));
INSTANTIATE_TEST_CASE_P(VectorCMP3, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20340f"),
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20341f"),
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e2035e0"),
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e2037e0"),
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f3400"),
    std::make_tuple(TR::InstOpCode::vcmgt16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f3400"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60340f"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60341f"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e6035e0"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e6037e0"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f3400"),
    std::make_tuple(TR::InstOpCode::vcmgt8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f3400"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0340f"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea0341f"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea035e0"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea037e0"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf3400"),
    std::make_tuple(TR::InstOpCode::vcmgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf3400"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ee0340f"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ee0341f"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ee035e0"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ee037e0"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eef3400"),
    std::make_tuple(TR::InstOpCode::vcmgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4eff3400"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e208c0f"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e208c1f"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e208de0"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e208fe0"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2f8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst16b, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3f8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e608c0f"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e608c1f"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e608de0"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e608fe0"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6f8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst8h, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7f8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ea08c0f"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ea08c1f"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ea08de0"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ea08fe0"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eaf8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4ebf8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4ee08c0f"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4ee08c1f"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4ee08de0"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4ee08fe0"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4eef8c00"),
    std::make_tuple(TR::InstOpCode::vcmtst2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4eff8c00")
));

INSTANTIATE_TEST_CASE_P(VectorCMPZERO1, ARM64Trg1Src1EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vcmeq16b_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e20980f"),
    std::make_tuple(TR::InstOpCode::vcmeq16b_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e20981f"),
    std::make_tuple(TR::InstOpCode::vcmeq16b_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e2099e0"),
    std::make_tuple(TR::InstOpCode::vcmeq16b_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e209be0"),
    std::make_tuple(TR::InstOpCode::vcmeq8h_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e60980f"),
    std::make_tuple(TR::InstOpCode::vcmeq8h_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e60981f"),
    std::make_tuple(TR::InstOpCode::vcmeq8h_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e6099e0"),
    std::make_tuple(TR::InstOpCode::vcmeq8h_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e609be0"),
    std::make_tuple(TR::InstOpCode::vcmeq4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0980f"),
    std::make_tuple(TR::InstOpCode::vcmeq4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0981f"),
    std::make_tuple(TR::InstOpCode::vcmeq4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea099e0"),
    std::make_tuple(TR::InstOpCode::vcmeq4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea09be0"),
    std::make_tuple(TR::InstOpCode::vcmeq2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0980f"),
    std::make_tuple(TR::InstOpCode::vcmeq2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0981f"),
    std::make_tuple(TR::InstOpCode::vcmeq2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee099e0"),
    std::make_tuple(TR::InstOpCode::vcmeq2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee09be0"),
    std::make_tuple(TR::InstOpCode::vcmge16b_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6e20880f"),
    std::make_tuple(TR::InstOpCode::vcmge16b_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6e20881f"),
    std::make_tuple(TR::InstOpCode::vcmge16b_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6e2089e0"),
    std::make_tuple(TR::InstOpCode::vcmge16b_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6e208be0"),
    std::make_tuple(TR::InstOpCode::vcmge8h_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6e60880f"),
    std::make_tuple(TR::InstOpCode::vcmge8h_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6e60881f"),
    std::make_tuple(TR::InstOpCode::vcmge8h_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6e6089e0"),
    std::make_tuple(TR::InstOpCode::vcmge8h_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6e608be0"),
    std::make_tuple(TR::InstOpCode::vcmge4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0880f"),
    std::make_tuple(TR::InstOpCode::vcmge4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0881f"),
    std::make_tuple(TR::InstOpCode::vcmge4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ea089e0"),
    std::make_tuple(TR::InstOpCode::vcmge4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ea08be0"),
    std::make_tuple(TR::InstOpCode::vcmge2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0880f"),
    std::make_tuple(TR::InstOpCode::vcmge2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0881f"),
    std::make_tuple(TR::InstOpCode::vcmge2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ee089e0"),
    std::make_tuple(TR::InstOpCode::vcmge2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ee08be0"),
    std::make_tuple(TR::InstOpCode::vcmgt16b_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e20880f"),
    std::make_tuple(TR::InstOpCode::vcmgt16b_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e20881f"),
    std::make_tuple(TR::InstOpCode::vcmgt16b_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e2089e0"),
    std::make_tuple(TR::InstOpCode::vcmgt16b_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e208be0"),
    std::make_tuple(TR::InstOpCode::vcmgt8h_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e60880f"),
    std::make_tuple(TR::InstOpCode::vcmgt8h_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e60881f"),
    std::make_tuple(TR::InstOpCode::vcmgt8h_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e6089e0"),
    std::make_tuple(TR::InstOpCode::vcmgt8h_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e608be0"),
    std::make_tuple(TR::InstOpCode::vcmgt4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0880f"),
    std::make_tuple(TR::InstOpCode::vcmgt4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0881f"),
    std::make_tuple(TR::InstOpCode::vcmgt4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea089e0"),
    std::make_tuple(TR::InstOpCode::vcmgt4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea08be0"),
    std::make_tuple(TR::InstOpCode::vcmgt2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0880f"),
    std::make_tuple(TR::InstOpCode::vcmgt2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0881f"),
    std::make_tuple(TR::InstOpCode::vcmgt2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee089e0"),
    std::make_tuple(TR::InstOpCode::vcmgt2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee08be0")
));

INSTANTIATE_TEST_CASE_P(VectorCMPZERO2, ARM64Trg1Src1EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vcmle16b_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6e20980f"),
    std::make_tuple(TR::InstOpCode::vcmle16b_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6e20981f"),
    std::make_tuple(TR::InstOpCode::vcmle16b_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6e2099e0"),
    std::make_tuple(TR::InstOpCode::vcmle16b_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6e209be0"),
    std::make_tuple(TR::InstOpCode::vcmle8h_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6e60980f"),
    std::make_tuple(TR::InstOpCode::vcmle8h_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6e60981f"),
    std::make_tuple(TR::InstOpCode::vcmle8h_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6e6099e0"),
    std::make_tuple(TR::InstOpCode::vcmle8h_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6e609be0"),
    std::make_tuple(TR::InstOpCode::vcmle4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0980f"),
    std::make_tuple(TR::InstOpCode::vcmle4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0981f"),
    std::make_tuple(TR::InstOpCode::vcmle4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ea099e0"),
    std::make_tuple(TR::InstOpCode::vcmle4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ea09be0"),
    std::make_tuple(TR::InstOpCode::vcmle2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0980f"),
    std::make_tuple(TR::InstOpCode::vcmle2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0981f"),
    std::make_tuple(TR::InstOpCode::vcmle2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ee099e0"),
    std::make_tuple(TR::InstOpCode::vcmle2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ee09be0"),
    std::make_tuple(TR::InstOpCode::vcmlt16b_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e20a80f"),
    std::make_tuple(TR::InstOpCode::vcmlt16b_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e20a81f"),
    std::make_tuple(TR::InstOpCode::vcmlt16b_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e20a9e0"),
    std::make_tuple(TR::InstOpCode::vcmlt16b_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e20abe0"),
    std::make_tuple(TR::InstOpCode::vcmlt8h_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4e60a80f"),
    std::make_tuple(TR::InstOpCode::vcmlt8h_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4e60a81f"),
    std::make_tuple(TR::InstOpCode::vcmlt8h_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4e60a9e0"),
    std::make_tuple(TR::InstOpCode::vcmlt8h_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4e60abe0"),
    std::make_tuple(TR::InstOpCode::vcmlt4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0a80f"),
    std::make_tuple(TR::InstOpCode::vcmlt4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0a81f"),
    std::make_tuple(TR::InstOpCode::vcmlt4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea0a9e0"),
    std::make_tuple(TR::InstOpCode::vcmlt4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea0abe0"),
    std::make_tuple(TR::InstOpCode::vcmlt2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0a80f"),
    std::make_tuple(TR::InstOpCode::vcmlt2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0a81f"),
    std::make_tuple(TR::InstOpCode::vcmlt2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee0a9e0"),
    std::make_tuple(TR::InstOpCode::vcmlt2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee0abe0")
));

INSTANTIATE_TEST_CASE_P(VectorFCMP0, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e20e40f"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e20e41f"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e20e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e20e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e2fe400"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e3fe400"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "4e60e40f"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "4e60e41f"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "4e60e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "4e60e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "4e6fe400"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "4e7fe400"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e20e40f"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e20e41f"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e20e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e20e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2fe400"),
    std::make_tuple(TR::InstOpCode::vfcmge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3fe400"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e60e40f"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e60e41f"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e60e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e60e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e6fe400"),
    std::make_tuple(TR::InstOpCode::vfcmge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e7fe400"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0e40f"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0e41f"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eafe400"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6ebfe400"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0e40f"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0e41f"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0e5e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0e7e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eefe400"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6effe400")
));

INSTANTIATE_TEST_CASE_P(VectorFCMP1, ARM64Trg1Src2EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e20ec0f"),
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e20ec1f"),
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e20ede0"),
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e20efe0"),
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e2fec00"),
    std::make_tuple(TR::InstOpCode::vfacge4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e3fec00"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6e60ec0f"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6e60ec1f"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6e60ede0"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6e60efe0"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6e6fec00"),
    std::make_tuple(TR::InstOpCode::vfacge2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6e7fec00"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0ec0f"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ea0ec1f"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0ede0"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0efe0"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eafec00"),
    std::make_tuple(TR::InstOpCode::vfacgt4s, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6ebfec00"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v15, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0ec0f"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v31, TR::RealRegister::v0, TR::RealRegister::v0, "6ee0ec1f"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v0, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0ede0"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v0, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0efe0"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v15, "6eefec00"),
    std::make_tuple(TR::InstOpCode::vfacgt2d, TR::RealRegister::v0, TR::RealRegister::v0, TR::RealRegister::v31, "6effec00")
));

INSTANTIATE_TEST_CASE_P(VectorFCMPZERO0, ARM64Trg1Src1EncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vfcmeq4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0d80f"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0d81f"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea0d9e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea0dbe0"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0d80f"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0d81f"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee0d9e0"),
    std::make_tuple(TR::InstOpCode::vfcmeq2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee0dbe0"),
    std::make_tuple(TR::InstOpCode::vfcmge4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0c80f"),
    std::make_tuple(TR::InstOpCode::vfcmge4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0c81f"),
    std::make_tuple(TR::InstOpCode::vfcmge4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ea0c9e0"),
    std::make_tuple(TR::InstOpCode::vfcmge4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ea0cbe0"),
    std::make_tuple(TR::InstOpCode::vfcmge2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0c80f"),
    std::make_tuple(TR::InstOpCode::vfcmge2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0c81f"),
    std::make_tuple(TR::InstOpCode::vfcmge2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ee0c9e0"),
    std::make_tuple(TR::InstOpCode::vfcmge2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ee0cbe0"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0c80f"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0c81f"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea0c9e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea0cbe0"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0c80f"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0c81f"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee0c9e0"),
    std::make_tuple(TR::InstOpCode::vfcmgt2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee0cbe0"),
    std::make_tuple(TR::InstOpCode::vfcmle4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ea0d80f"),
    std::make_tuple(TR::InstOpCode::vfcmle4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ea0d81f"),
    std::make_tuple(TR::InstOpCode::vfcmle4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ea0d9e0"),
    std::make_tuple(TR::InstOpCode::vfcmle4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ea0dbe0"),
    std::make_tuple(TR::InstOpCode::vfcmle2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "6ee0d80f"),
    std::make_tuple(TR::InstOpCode::vfcmle2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "6ee0d81f"),
    std::make_tuple(TR::InstOpCode::vfcmle2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "6ee0d9e0"),
    std::make_tuple(TR::InstOpCode::vfcmle2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "6ee0dbe0"),
    std::make_tuple(TR::InstOpCode::vfcmlt4s_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ea0e80f"),
    std::make_tuple(TR::InstOpCode::vfcmlt4s_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ea0e81f"),
    std::make_tuple(TR::InstOpCode::vfcmlt4s_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ea0e9e0"),
    std::make_tuple(TR::InstOpCode::vfcmlt4s_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ea0ebe0"),
    std::make_tuple(TR::InstOpCode::vfcmlt2d_zero, TR::RealRegister::v15, TR::RealRegister::v0, "4ee0e80f"),
    std::make_tuple(TR::InstOpCode::vfcmlt2d_zero, TR::RealRegister::v31, TR::RealRegister::v0, "4ee0e81f"),
    std::make_tuple(TR::InstOpCode::vfcmlt2d_zero, TR::RealRegister::v0, TR::RealRegister::v15, "4ee0e9e0"),
    std::make_tuple(TR::InstOpCode::vfcmlt2d_zero, TR::RealRegister::v0, TR::RealRegister::v31, "4ee0ebe0")
));

INSTANTIATE_TEST_CASE_P(VectorShiftLeftImmediate, ARM64VectorShiftImmediateEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f09540f"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v15, TR::RealRegister::v0, 7, "4f0f540f"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f09541f"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v31, TR::RealRegister::v0, 7, "4f0f541f"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f0955e0"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v0, TR::RealRegister::v15, 7, "4f0f55e0"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f0957e0"),
    std::make_tuple(TR::InstOpCode::vshl16b, TR::RealRegister::v0, TR::RealRegister::v31, 7, "4f0f57e0"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f11540f"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v15, TR::RealRegister::v0, 15, "4f1f540f"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f11541f"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v31, TR::RealRegister::v0, 15, "4f1f541f"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f1155e0"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v0, TR::RealRegister::v15, 15, "4f1f55e0"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f1157e0"),
    std::make_tuple(TR::InstOpCode::vshl8h, TR::RealRegister::v0, TR::RealRegister::v31, 15, "4f1f57e0"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f21540f"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v15, TR::RealRegister::v0, 31, "4f3f540f"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f21541f"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v31, TR::RealRegister::v0, 31, "4f3f541f"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f2155e0"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v0, TR::RealRegister::v15, 31, "4f3f55e0"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f2157e0"),
    std::make_tuple(TR::InstOpCode::vshl4s, TR::RealRegister::v0, TR::RealRegister::v31, 31, "4f3f57e0"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f41540f"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v15, TR::RealRegister::v0, 63, "4f7f540f"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f41541f"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v31, TR::RealRegister::v0, 63, "4f7f541f"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f4155e0"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v0, TR::RealRegister::v15, 63, "4f7f55e0"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f4157e0"),
    std::make_tuple(TR::InstOpCode::vshl2d, TR::RealRegister::v0, TR::RealRegister::v31, 63, "4f7f57e0")
));

INSTANTIATE_TEST_CASE_P(VectorSignedShiftRightImmediate, ARM64VectorShiftImmediateEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f0f040f"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v15, TR::RealRegister::v0, 7, "4f09040f"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f0f041f"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v31, TR::RealRegister::v0, 7, "4f09041f"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f0f05e0"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v0, TR::RealRegister::v15, 7, "4f0905e0"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f0f07e0"),
    std::make_tuple(TR::InstOpCode::vsshr16b, TR::RealRegister::v0, TR::RealRegister::v31, 7, "4f0907e0"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f1f040f"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v15, TR::RealRegister::v0, 15, "4f11040f"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f1f041f"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v31, TR::RealRegister::v0, 15, "4f11041f"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f1f05e0"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v0, TR::RealRegister::v15, 15, "4f1105e0"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f1f07e0"),
    std::make_tuple(TR::InstOpCode::vsshr8h, TR::RealRegister::v0, TR::RealRegister::v31, 15, "4f1107e0"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f3f040f"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v15, TR::RealRegister::v0, 31, "4f21040f"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f3f041f"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v31, TR::RealRegister::v0, 31, "4f21041f"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f3f05e0"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v0, TR::RealRegister::v15, 31, "4f2105e0"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f3f07e0"),
    std::make_tuple(TR::InstOpCode::vsshr4s, TR::RealRegister::v0, TR::RealRegister::v31, 31, "4f2107e0"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v15, TR::RealRegister::v0, 1, "4f7f040f"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v15, TR::RealRegister::v0, 63, "4f41040f"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v31, TR::RealRegister::v0, 1, "4f7f041f"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v31, TR::RealRegister::v0, 63, "4f41041f"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v0, TR::RealRegister::v15, 1, "4f7f05e0"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v0, TR::RealRegister::v15, 63, "4f4105e0"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v0, TR::RealRegister::v31, 1, "4f7f07e0"),
    std::make_tuple(TR::InstOpCode::vsshr2d, TR::RealRegister::v0, TR::RealRegister::v31, 63, "4f4107e0")
));

INSTANTIATE_TEST_CASE_P(VectorUnsignedShiftRightImmediate, ARM64VectorShiftImmediateEncodingTest, ::testing::Values(
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v15, TR::RealRegister::v0, 1, "6f0f040f"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v15, TR::RealRegister::v0, 7, "6f09040f"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v31, TR::RealRegister::v0, 1, "6f0f041f"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v31, TR::RealRegister::v0, 7, "6f09041f"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v0, TR::RealRegister::v15, 1, "6f0f05e0"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v0, TR::RealRegister::v15, 7, "6f0905e0"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v0, TR::RealRegister::v31, 1, "6f0f07e0"),
    std::make_tuple(TR::InstOpCode::vushr16b, TR::RealRegister::v0, TR::RealRegister::v31, 7, "6f0907e0"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v15, TR::RealRegister::v0, 1, "6f1f040f"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v15, TR::RealRegister::v0, 15, "6f11040f"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v31, TR::RealRegister::v0, 1, "6f1f041f"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v31, TR::RealRegister::v0, 15, "6f11041f"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v0, TR::RealRegister::v15, 1, "6f1f05e0"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v0, TR::RealRegister::v15, 15, "6f1105e0"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v0, TR::RealRegister::v31, 1, "6f1f07e0"),
    std::make_tuple(TR::InstOpCode::vushr8h, TR::RealRegister::v0, TR::RealRegister::v31, 15, "6f1107e0"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v15, TR::RealRegister::v0, 1, "6f3f040f"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v15, TR::RealRegister::v0, 31, "6f21040f"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v31, TR::RealRegister::v0, 1, "6f3f041f"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v31, TR::RealRegister::v0, 31, "6f21041f"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v0, TR::RealRegister::v15, 1, "6f3f05e0"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v0, TR::RealRegister::v15, 31, "6f2105e0"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v0, TR::RealRegister::v31, 1, "6f3f07e0"),
    std::make_tuple(TR::InstOpCode::vushr4s, TR::RealRegister::v0, TR::RealRegister::v31, 31, "6f2107e0"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v15, TR::RealRegister::v0, 1, "6f7f040f"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v15, TR::RealRegister::v0, 63, "6f41040f"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v31, TR::RealRegister::v0, 1, "6f7f041f"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v31, TR::RealRegister::v0, 63, "6f41041f"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v0, TR::RealRegister::v15, 1, "6f7f05e0"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v0, TR::RealRegister::v15, 63, "6f4105e0"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v0, TR::RealRegister::v31, 1, "6f7f07e0"),
    std::make_tuple(TR::InstOpCode::vushr2d, TR::RealRegister::v0, TR::RealRegister::v31, 63, "6f4107e0")
));

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

class ARM64Trg1Src2EncodingTest : public TRTest::BinaryEncoderTest<ARM64_INSTRUCTION_ALIGNMENT>, public ::testing::WithParamInterface<std::tuple<TR::InstOpCode::Mnemonic, TR::RealRegister::RegNum, TR::RealRegister::RegNum, TR::RealRegister::RegNum, ARM64BinaryInstruction>> {};

TEST_P(ARM64Trg1Src2EncodingTest, encode) {
    auto trgReg = cg()->machine()->getRealRegister(std::get<1>(GetParam()));
    auto src1Reg = cg()->machine()->getRealRegister(std::get<2>(GetParam()));
    auto src2Reg = cg()->machine()->getRealRegister(std::get<3>(GetParam()));

    auto instr = generateTrg1Src2Instruction(cg(), std::get<0>(GetParam()), fakeNode, trgReg, src1Reg, src2Reg);

    ASSERT_EQ(std::get<4>(GetParam()), encodeInstruction(instr));
}

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

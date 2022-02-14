/*******************************************************************************
 * Copyright (c) 2020, 2022 IBM Corp. and others
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

#ifndef CODEGENTEST_HPP
#define CODEGENTEST_HPP

#include "CompilerUnitTest.hpp"
#include "codegen/Instruction.hpp"
#include "include_core/OMR/Bytes.hpp"

#define INSTRUCTION_MAX_LENGTH 32


namespace TRTest {

class CodeGenTest : public TRTest::CompilerUnitTest {
public:
    CodeGenTest() : CompilerUnitTest() {
        fakeNode = TR::Node::create(TR::treetop);
    }

    TR::CodeGenerator* cg() { return _comp.cg(); }
    TR::Node *fakeNode;
};

class BinaryInstruction {

public:

    uint8_t _buf[INSTRUCTION_MAX_LENGTH];
    size_t _size;

    BinaryInstruction() : _size(0) {}

    BinaryInstruction(uint8_t *instr, size_t size) : _size(size) {
        TR_ASSERT_FATAL(size <= INSTRUCTION_MAX_LENGTH, "buffer size should not exceed INSTRUCTION_MAX_LENGTH");
        memcpy(_buf, instr, size);
    }

    BinaryInstruction(const char *instr);

    TRTest::BinaryInstruction prepend(uint32_t data) const {
        TRTest::BinaryInstruction ret;

        memcpy(ret._buf, &data, sizeof(uint32_t));
        memcpy(ret._buf + sizeof(uint32_t), _buf, _size);
        ret._size += _size + sizeof(uint32_t);

        return ret;
    }

    BinaryInstruction operator^(const BinaryInstruction &other) const {
        if (_size != other._size)
            throw std::invalid_argument("Attempt to XOR instructions of different sizes");

        BinaryInstruction ret;

        for (size_t i = 0; i < _size; i++) {
            ret._buf[i] = _buf[i] ^ other._buf[i];
        }

        ret._size = _size;

        return ret;
    }

    bool operator==(const BinaryInstruction &other) const {
        if (_size != other._size)
            return false;
        return memcmp(_buf, other._buf, _size) == 0;
    }
};

template <size_t ALIGNMENT = 0x1>
class BinaryEncoderTest : public TRTest::CodeGenTest {
    public:
    uint8_t buf[ALIGNMENT + INSTRUCTION_MAX_LENGTH];

    BinaryEncoderTest() {
        cg()->setBinaryBufferStart(reinterpret_cast<uint8_t *>(&getAlignedBuf()[0]));
    }

    uint8_t *getAlignedBuf() {
        size_t sz = ALIGNMENT + INSTRUCTION_MAX_LENGTH;
        void *ptr = buf;

        uint8_t *alignedBuf = reinterpret_cast<uint8_t*>(OMR::align(ALIGNMENT, INSTRUCTION_MAX_LENGTH, ptr, sz));
        TR_ASSERT_FATAL(alignedBuf, "The desired alignment is not possible");

        return alignedBuf;
    }

    TRTest::BinaryInstruction encodeInstruction(TR::Instruction *instr, uint32_t off) {
        instr->estimateBinaryLength(0);
        uint8_t *begin = reinterpret_cast<uint8_t *>(getAlignedBuf()) + off;
        cg()->setBinaryBufferCursor(begin);

        instr->generateBinaryEncoding();
        return BinaryInstruction(begin, instr->getBinaryLength());
    }

    TRTest::BinaryInstruction encodeInstruction(TR::Instruction *instr) {
        return encodeInstruction(instr, 0);
    }
};

std::ostream& operator<<(std::ostream& os, const BinaryInstruction& instr);

}

#endif

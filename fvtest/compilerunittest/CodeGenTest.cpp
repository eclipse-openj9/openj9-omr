/*******************************************************************************
 * Copyright (c) 2021, 2022 IBM Corp. and others
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

#include "CodeGenTest.hpp"

namespace TRTest {

std::ostream &operator<<(std::ostream &os, const BinaryInstruction &instr) {
    os << "[ ";

    for (size_t i = 0; i < instr._size; i++) {
        os << std::hex << std::setw(2) << std::setfill('0') << (0xff & instr._buf[i]) << " ";
    }

    os << "]";

    return os;
}

uint8_t hexCharToInt(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;

    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;

    throw std::invalid_argument("Illegal hex char");
}

BinaryInstruction::BinaryInstruction(const char *instr) {
    size_t len = strlen(instr);

    if (len % 2 != 0)
        throw std::invalid_argument("Hex string must be multiple of 2 in length");

    _size = len / 2;
    for (int i = 0; i < len; i += 2) {
        _buf[i / 2] = hexCharToInt(instr[i]) << 4 | hexCharToInt(instr[i + 1]);
    }
}

}

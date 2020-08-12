/*******************************************************************************
 * Copyright (c) 2020, 2020 IBM Corp. and others
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

#include "../CodeGenTest.hpp"
#include "Util.hpp"

#include "codegen/GenerateInstructions.hpp"
#include "codegen/Peephole.hpp"

class PeepholeTest : public TRTest::CodeGenTest {};

TEST_F(PeepholeTest, testRegressIssue5418FlatLoad) {
    TR::RealRegister *gr1 = cg()->machine()->getRealRegister(TR::RealRegister::gr1);
    TR::RealRegister *gr2 = cg()->machine()->getRealRegister(TR::RealRegister::gr2);

    TR::Instruction *instr1 = generateMemSrc1Instruction(cg(), TR::InstOpCode::stw, fakeNode, TR::MemoryReference::createWithDisplacement(cg(), gr1, 0, 4), gr2);
    TR::Instruction *instr2 = generateTrg1Src1ImmInstruction(cg(), TR::InstOpCode::lwz, fakeNode, gr2, gr1, 0);

    TR::Peephole peephole(cg()->comp());
    peephole.perform();

    ASSERT_EQ(instr1, cg()->getFirstInstruction());
    ASSERT_EQ(instr2, instr1->getNext());
    ASSERT_FALSE(instr2->getNext());
}

TEST_F(PeepholeTest, testRegressIssue5418End) {
    TR::RealRegister *gr1 = cg()->machine()->getRealRegister(TR::RealRegister::gr1);
    TR::RealRegister *gr2 = cg()->machine()->getRealRegister(TR::RealRegister::gr2);

    TR::Instruction *instr = generateMemSrc1Instruction(cg(), TR::InstOpCode::stw, fakeNode, TR::MemoryReference::createWithDisplacement(cg(), gr1, 0, 4), gr2);

    TR::Peephole peephole(cg()->comp());
    peephole.perform();

    ASSERT_EQ(instr, cg()->getFirstInstruction());
    ASSERT_FALSE(instr->getNext());
}

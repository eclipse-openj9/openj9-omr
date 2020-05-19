/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#ifndef OMR_Z_PEEPHOLE_INCL
#define OMR_Z_PEEPHOLE_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_PEEPHOLE_CONNECTOR
#define OMR_PEEPHOLE_CONNECTOR
namespace OMR { namespace Z { class Peephole; } }
namespace OMR { typedef OMR::Z::Peephole PeepholeConnector; }
#else
#error OMR::Z::Peephole expected to be a primary connector, but an OMR connector is already defined
#endif

#include "compiler/codegen/OMRPeephole.hpp"

#include <stdint.h>
#include "codegen/InstOpCode.hpp"
#include "env/jittypes.h"
#include "infra/Flags.hpp"

namespace OMR
{

namespace Z
{

class OMR_EXTENSIBLE Peephole : public OMR::Peephole
   {
   public:

   Peephole(TR::Compilation* comp);

   virtual bool performOnInstruction(TR::Instruction* cursor);

   private:

   /** \brief
    *     Attempts to reduce L[' '|FH|G] R,MR1  ST[' '|FH|G] R,MR2 sequences to MVC MR2, MR1
    *     to save a register and instruction.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \param storeOpCode
    *     The store op code that matches the load.
    *
    *  \param size
    *     The number of bits being moved.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptLoadStoreReduction(TR::Instruction* cursor, TR::InstOpCode::Mnemonic storeOpCode, uint16_t size);

   /** \brief
    *     Attempts to reduce a 64-bit shift instruction to a 32-bit shift instruction based on the IL node associated
    *     with the 64-bit shift. For example if the node which generated the 64-bit shift operation is a \c TR::ishl
    *     then the 64-bit shfit:
    *
    *     <code>
    *     SLAK GPR15,GPR15,0(GPR3)
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     SLA GPR15,0(GPR3)
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduce64BitShiftTo32BitShift(TR::Instruction* cursor);

   /** \brief
    *     Attempts to reduce AGIs (Address Generation Interlock) which occur when an instruction requires a register
    *     its operand address calculation but the register is unavailable because it is written to by a preceeding
    *     instruction which has not been completed yet. This causes a pipeline stall which we would like to avoid. In
    *     some cases however the AGI can be avoided by register renaming. For example, the following:
    *
    *     <code>
    *     LTGR GPR3,GPR8
    *     AGRK GPR2,GPR11,GPR12
    *     LOCGR GPR11,16(GPR3),B'1000'
    *     </code>
    *
    *     can be register renamed to:
    *
    *     <code>
    *     LTGR GPR3,GPR8
    *     AGRK GPR2,GPR11,GPR12
    *     LOCGR GPR11,16(GPR8),B'1000'
    *     </code>
    *
    *     which avoids the AGI because \c GPR3 is no longer used within the memory reference.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceAGI(TR::Instruction* cursor);
   
   /** \brief
    *     Attempts to reduce a compare logical (\c CLR) insturction followed by a branch to a compare and branch
    *     instruction (\c CLRJ) For example:
    *
    *     <code>
    *     CLR GPR1,GPR2
    *     BRC B'1000',<LABEL>
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     CLRJ GPR1,GPR2,<LABEL>,B'1000'
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceCLRToCLRJ(TR::Instruction* cursor);
   
   /** \brief
    *     Attempts to reduce a simple branch conditional load of an immediate to a load immediate on condition branch-
    *     less sequence. For example:
    *
    *     <code>
    *     CRJ GPR2,GPR3,B'1010',<LABEL>
    *     LHI GPR4,55
    *     LABEL:
    *     ...
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     CR GPR2,GPR3
    *     LOCHI GPR4,55,B'1010'
    *     ...
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \param compareMnemonic
    *     The compare mnemonic which will replace the compare and branch instruction and feed the condition code into
    *     the conditional load.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceCRJLHIToLOCHI(TR::Instruction* cursor, TR::InstOpCode::Mnemonic compareMnemonic);
   
   /** \brief
    *     Attempts to reduce a load instruction (\c L) to an insert character under mask (\c ICM) instruction. This can
    *     be done if following the load we have a load and test or a compare against certain immediates. For example:
    *
    *     <code>
    *     L GPR2,8(GPR5)
    *     LTR GPR2,GPR2
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     ICM GPR2,8(GPR5),B'1111'
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLToICM(TR::Instruction* cursor);

   /** \brief
    *     Attempts to reduce a load instruction (\c L, \c LG, \c LLGF) followed by an \c NILL to its equivalent load
    *     and zero rightmost byte equivalent instruction. For example:
    *
    *     <code>
    *     LG GPR2,8(GPR5)
    *     NILL GPR2,0xFF00
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     LZRG GPR2,8(GPR5)
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \param loadAndZeroRightMostByteMnemonic
    *     The load and zero rightmost byte mnemonic to replace the original load with.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLToLZRF(TR::Instruction* cursor, TR::InstOpCode::Mnemonic loadAndZeroRightMostByteMnemonic);
   
   /** \brief
    *     Attempts to reduce a load register instruction (\c LGR or \c LTGR) followed by a sign extension to \c LGFR.
    *     For example:
    *
    *     <code>
    *     LGR GPR2,GPR3
    *     LGFR GPR2,GPR2
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     LGFR GPR2,GPR3
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLGRToLGFR(TR::Instruction* cursor);

   /** \brief
    *     Attempts to reduce a load halfword immedaite instruction (\c LHI) to an XOR \c XR.
    *     For example:
    *
    *     <code>
    *     LHI GPR2,0
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     XR GPR2,GPR2
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLHIToXR(TR::Instruction* cursor);
   
   /** \brief
    *     Attempts to reduce a load logical character instruction (\c LLC) followed by a zero extension to \c LLGC.
    *     For example:
    *
    *     <code>
    *     LLC GPR2,8(GPR3,GPR5)
    *     LGFR GPR2,GPR2
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     LLGC GPR2,8(GPR3,GPR5)
    *     </code>
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLLCToLLGC(TR::Instruction* cursor);
   
   /** \brief
    *     Attempts to reduce a load and test register instruction (\c LTR or \c LTGR) to a compare halfword immediate if
    *     the target register of the load is used in a future memory reference. This is an attempt to reduce the AGI
    *     incurred by using the target register. For example:
    *
    *     <code>
    *     LTR GPR2,GPR2
    *     BRC B'1000',<LABEL>
    *     AHI GPR3,6
    *     L GPR4,8(GPR2,GPR3)
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     CHI GPR2,0
    *     BRC B'1000',<LABEL>
    *     AHI GPR3,6
    *     L GPR4,8(GPR2,GPR3)
    *     </code>
    *
    *     which will avoid the AGI incurred by the memory refrence on the \c L instruction.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToReduceLTRToCHI(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove duplicate (\c LR, \c LGR, \c LDR, \c LER) instructions which have the same source and
    *     target registers.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveDuplicateLR(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove duplicate NILF instructions which target the same register and use the same immediate or
    *     redundant NILF instructions where the second NILF operation would not change the value of the target register.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveDuplicateNILF(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove duplicate NILH instructions which target the same register and use the same immediate.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveDuplicateNILH(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove redundant compare and trap instructions which can never be executed because a previous
    *     compare and trap instruction would have triggered a trap already.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveRedundantCompareAndTrap(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove redundant \c LA instructions whose evaluation would result in a NOP.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveRedundantLA(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove redundant shift instructions which can be fused into a single shift. For example:
    *
    *     <code>
    *     SRL GPR2,8(GPR2)
    *     SRL GPR2,4(GPR2)
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     SRL GPR2,12(GPR2)
    *     </code>

    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveRedundantShift(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove redundant \c LR or \c LGR instructions which are followed by a load and test instruction
    *     acting on the same source and target registers.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveRedundantLR(TR::Instruction* cursor);

   /** \brief
    *     Attempts to remove redundant \c LTR or \c LTGR instructions if we can reuse the condition code from a
    *     previous arithmetic operation. For example:
    *
    *     <code>
    *     SLR GPR2,GPR3
    *     LTR GPR2,GPR2
    *     BRC B'1000',<LABEL>
    *     </code>
    *
    *     can be reduced to:
    *
    *     <code>
    *     SLR GPR2,GPR3
    *     BRC B'1010',<LABEL>
    *     </code>
    *
    *     Note the modified mask value of the \c BRC instruction.
    *
    *  \param cursor
    *     The instruction cursor currently being processed.
    *
    *  \return
    *     true if the reduction was successful; false otherwise.
    */
   bool attemptToRemoveRedundantLTR(TR::Instruction* cursor);
   };

}

}

#endif

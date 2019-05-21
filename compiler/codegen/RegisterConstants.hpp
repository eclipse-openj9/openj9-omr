/*******************************************************************************
 * Copyright (c) 2000, 2019 IBM Corp. and others
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

#ifndef REGISTERKINDS_INCL
#define REGISTERKINDS_INCL

#include <stdint.h>
#include "infra/Flags.hpp"

typedef uint32_t TR_RegisterMask;

#define TO_KIND_MASK(x)       (1<<x)
#define REGISTER_KIND_NAME(x) (TR::Register::getRegisterKindName(x))

enum TR_RegisterKinds
   {
   TR_GPR           = 0,
   TR_FPR           = 1,
   TR_CCR           = 2,
   TR_X87           = 3,
   TR_VRF           = 4,
   TR_VSX_SCALAR    = 5,
   TR_VSX_VECTOR    = 6,
   TR_SSR           = 7,   // used for TR_PseudoRegisters for SS (storage to storage) instructions to return results
   LastRegisterKind = TR_SSR,
   NumRegisterKinds = LastRegisterKind+1,

   TR_NoRegister    = LastRegisterKind+1,

   TR_NoKind_Mask = 0,
   TR_GPR_Mask    = TO_KIND_MASK(TR_GPR),
   TR_FPR_Mask    = TO_KIND_MASK(TR_FPR),
   TR_CCR_Mask    = TO_KIND_MASK(TR_CCR),
   TR_X87_Mask    = TO_KIND_MASK(TR_X87),
   TR_VRF_Mask    = TO_KIND_MASK(TR_VRF),
   TR_VSX_SCALAR_Mask    = TO_KIND_MASK(TR_VSX_SCALAR),
   TR_VSX_VECTOR_Mask    = TO_KIND_MASK(TR_VSX_VECTOR),
   TR_SSR_Mask    = TO_KIND_MASK(TR_SSR)
   };

enum TR_RegisterSizes
   {
   TR_UnknownSizeReg = -1,
   TR_ByteReg        = 0,
   TR_HalfWordReg    = 1,
   TR_WordReg        = 2,
   TR_DoubleWordReg  = 3,
   TR_QuadWordReg    = 4,
   TR_FloatReg       = 5,
   TR_DoubleReg      = 6,
   TR_VectorReg      = 7
   };

enum TR_RematerializableTypes
   {
   // TODO:AMD64: We should just use TR::DataType for this and simplify everything
   TR_RematerializableLoadEffectiveAddress = 0,
   TR_RematerializableByte                 = 1,
   TR_RematerializableShort                = 2,
   TR_RematerializableChar                 = 3,
   TR_RematerializableInt                  = 4,
   TR_RematerializableAddress              = 5,
   TR_RematerializableLong                 = 6,
   TR_RematerializableFloat                = 7,
   TR_RematerializableDouble               = 8,
   TR_NumRematerializableTypes             = 9,
   };

enum TR_RegisterAssignmentFlagBits
   {
   TR_NormalAssignment        = 0x00000000,
   TR_RegisterSpilled         = 0x00000001,
   TR_RegisterReloaded        = 0x00000002,
   TR_PreDependencyCoercion   = 0x00000004,
   TR_PostDependencyCoercion  = 0x00000008,
   TR_IndirectCoercion        = 0x00000010,
   TR_ByAssociation           = 0x00000020,
   TR_HasBetterSpillPlacement = 0x00000040,
   TR_ByColouring             = 0x00000080,
   TR_ColouringCoercion       = 0x00000100,
   };

typedef flags32_t TR_RegisterAssignmentFlags;

typedef int16_t TR_GlobalRegisterNumber;

#endif

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

#ifndef OMR_OPCODEMACROS_INCL
#define OMR_OPCODEMACROS_INCL

#define FOR_EACH_OPCODE(MACRO) \
   MACRO(\
      TR::BadILOp, /* .opcode */ \
      "BadILOp", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      BadILOp = 0, /* illegal op hopefully help with uninitialized nodes */ \
   ) \
   MACRO(\
      TR::aconst, /* .opcode */ \
      "aconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aconst, /* load address constant (zero value means NULL) */ \
   ) \
   MACRO(\
      TR::iconst, /* .opcode */ \
      "iconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iconst, /* load integer constant (32-bit signed 2's complement) */ \
   ) \
   MACRO(\
      TR::lconst, /* .opcode */ \
      "lconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lconst, /* load long integer constant (64-bit signed 2's complement) */ \
   ) \
   MACRO(\
      TR::fconst, /* .opcode */ \
      "fconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fconst, /* load float constant (32-bit ieee fp) */ \
   ) \
   MACRO(\
      TR::dconst, /* .opcode */ \
      "dconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dconst, /* load double constant (64-bit ieee fp) */ \
   ) \
   MACRO(\
      TR::bconst, /* .opcode */ \
      "bconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bconst, /* load byte integer constant (8-bit signed 2's complement) */ \
   ) \
   MACRO(\
      TR::sconst, /* .opcode */ \
      "sconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sconst, /* load short integer constant (16-bit signed 2's complement) */ \
   ) \
   MACRO(\
      TR::iload, /* .opcode */ \
      "iload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iload, /* load integer */ \
   ) \
   MACRO(\
      TR::fload, /* .opcode */ \
      "fload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fload, /* load float */ \
   ) \
   MACRO(\
      TR::dload, /* .opcode */ \
      "dload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dload, /* load double */ \
   ) \
   MACRO(\
      TR::aload, /* .opcode */ \
      "aload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aload, /* load address */ \
   ) \
   MACRO(\
      TR::bload, /* .opcode */ \
      "bload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bload, /* load byte */ \
   ) \
   MACRO(\
      TR::sload, /* .opcode */ \
      "sload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sload, /* load short integer */ \
   ) \
   MACRO(\
      TR::lload, /* .opcode */ \
      "lload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lload, /* load long integer */ \
   ) \
   MACRO(\
      TR::irdbar, /* .opcode */ \
      "irdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      irdbar, /* read barrier for load integer */ \
   ) \
   MACRO(\
      TR::frdbar, /* .opcode */ \
      "frdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      frdbar, /* read barrier for load float */ \
   ) \
   MACRO(\
      TR::drdbar, /* .opcode */ \
      "drdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      drdbar, /* read barrier for load double */ \
   ) \
   MACRO(\
      TR::ardbar, /* .opcode */ \
      "ardbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ardbar, /* read barrier for load address */ \
   ) \
   MACRO(\
      TR::brdbar, /* .opcode */ \
      "brdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      brdbar, /* read barrier for load byte */ \
   ) \
   MACRO(\
      TR::srdbar, /* .opcode */ \
      "srdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      srdbar, /* load short integer */ \
   ) \
   MACRO(\
      TR::lrdbar, /* .opcode */ \
      "lrdbar", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lrdbar, /* load long integer */ \
   ) \
   MACRO(\
      TR::iloadi, /* .opcode */ \
      "iloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iloadi, /* load indirect integer */ \
   ) \
   MACRO(\
      TR::floadi, /* .opcode */ \
      "floadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      floadi, /* load indirect float */ \
   ) \
   MACRO(\
      TR::dloadi, /* .opcode */ \
      "dloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dloadi, /* load indirect double */ \
   ) \
   MACRO(\
      TR::aloadi, /* .opcode */ \
      "aloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aloadi, /* load indirect address */ \
   ) \
   MACRO(\
      TR::bloadi, /* .opcode */ \
      "bloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bloadi, /* load indirect byte */ \
   ) \
   MACRO(\
      TR::sloadi, /* .opcode */ \
      "sloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sloadi, /* load indirect short integer */ \
   ) \
   MACRO(\
      TR::lloadi, /* .opcode */ \
      "lloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lloadi, /* load indirect long integer */ \
   ) \
   MACRO(\
      TR::irdbari, /* .opcode */ \
      "irdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      irdbari, /* read barrier for load indirect integer */ \
   ) \
   MACRO(\
      TR::frdbari, /* .opcode */ \
      "frdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      frdbari, /* read barrier for load indirect float */ \
   ) \
   MACRO(\
      TR::drdbari, /* .opcode */ \
      "drdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      drdbari, /* read barrier for load indirect double */ \
   ) \
   MACRO(\
      TR::ardbari, /* .opcode */ \
      "ardbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ardbari, /* read barrier for load indirect address */ \
   ) \
   MACRO(\
      TR::brdbari, /* .opcode */ \
      "brdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      brdbari, /* read barrier for load indirect byte */ \
   ) \
   MACRO(\
      TR::srdbari, /* .opcode */ \
      "srdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      srdbari, /* read barrier for load indirect short integer */ \
   ) \
   MACRO(\
      TR::lrdbari, /* .opcode */ \
      "lrdbari", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lrdbari, /* read barrier for load indirect long integer */ \
   ) \
   MACRO(\
      TR::istore, /* .opcode */ \
      "istore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      istore, /* store integer */ \
   ) \
   MACRO(\
      TR::lstore, /* .opcode */ \
      "lstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lstore, /* store long integer */ \
   ) \
   MACRO(\
      TR::fstore, /* .opcode */ \
      "fstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fstore, /* store float */ \
   ) \
   MACRO(\
      TR::dstore, /* .opcode */ \
      "dstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dstore, /* store double */ \
   ) \
   MACRO(\
      TR::astore, /* .opcode */ \
      "astore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      astore, /* store address */ \
   ) \
   MACRO(\
      TR::bstore, /* .opcode */ \
      "bstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bstore, /* store byte */ \
   ) \
   MACRO(\
      TR::sstore, /* .opcode */ \
      "sstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sstore, /* store short integer */ \
   ) \
   MACRO(\
      TR::iwrtbar, /* .opcode */ \
      "iwrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int32, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iwrtbar, /* write barrier for store direct integer */ \
   ) \
   MACRO(\
      TR::lwrtbar, /* .opcode */ \
      "lwrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int64, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lwrtbar, /* write barrier for store direct long integer */ \
   ) \
   MACRO(\
      TR::fwrtbar, /* .opcode */ \
      "fwrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Float, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fwrtbar, /* write barrier for store direct float */ \
   ) \
   MACRO(\
      TR::dwrtbar, /* .opcode */ \
      "dwrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Double, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dwrtbar, /* write barrier for store direct double */ \
   ) \
   MACRO(\
      TR::awrtbar, /* .opcode */ \
      "awrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      awrtbar, /* write barrier for store direct address */ \
   ) \
   MACRO(\
      TR::bwrtbar, /* .opcode */ \
      "bwrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int8, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bwrtbar, /* write barrier for store direct byte */ \
   ) \
   MACRO(\
      TR::swrtbar, /* .opcode */ \
      "swrtbar", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int16, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      swrtbar, /* write barrier for store direct short integer */ \
   ) \
   MACRO(\
      TR::lstorei, /* .opcode */ \
      "lstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lstorei, /* store indirect long integer           (child1 a, child2 l) */ \
   ) \
   MACRO(\
      TR::fstorei, /* .opcode */ \
      "fstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fstorei, /* store indirect float                  (child1 a, child2 f) */ \
   ) \
   MACRO(\
      TR::dstorei, /* .opcode */ \
      "dstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dstorei, /* store indirect double                 (child1 a, child2 d) */ \
   ) \
   MACRO(\
      TR::astorei, /* .opcode */ \
      "astorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      astorei, /* store indirect address                (child1 a dest, child2 a value) */ \
   ) \
   MACRO(\
      TR::bstorei, /* .opcode */ \
      "bstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bstorei, /* store indirect byte                   (child1 a, child2 b) */ \
   ) \
   MACRO(\
      TR::sstorei, /* .opcode */ \
      "sstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sstorei, /* store indirect short integer          (child1 a, child2 s) */ \
   ) \
   MACRO(\
      TR::istorei, /* .opcode */ \
      "istorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      istorei, /* store indirect integer                (child1 a, child2 i) */ \
   ) \
   MACRO(\
      TR::lwrtbari, /* .opcode */ \
      "lwrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int64, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lwrtbari, /* write barrier for store indirect long integer */ \
   ) \
   MACRO(\
      TR::fwrtbari, /* .opcode */ \
      "fwrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Float, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fwrtbari, /* write barrier for store indirect float */ \
   ) \
   MACRO(\
      TR::dwrtbari, /* .opcode */ \
      "dwrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Double, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dwrtbari, /* write barrier for store indirect double */ \
   ) \
   MACRO(\
      TR::awrtbari, /* .opcode */ \
      "awrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Address, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      awrtbari, /* write barrier for store indirect address */ \
   ) \
   MACRO(\
      TR::bwrtbari, /* .opcode */ \
      "bwrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int8, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bwrtbari, /* write barrier for store indirect byte */ \
   ) \
   MACRO(\
      TR::swrtbari, /* .opcode */ \
      "swrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int16, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      swrtbari, /* write barrier for store indirect short integer */ \
   ) \
   MACRO(\
      TR::iwrtbari, /* .opcode */ \
      "iwrtbari", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int32, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iwrtbari, /* write barrier for store indirect integer */ \
   ) \
   MACRO(\
      TR::Goto, /* .opcode */ \
      "goto", /* .name */ \
      ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      Goto, /* goto label address */ \
   ) \
   MACRO(\
      TR::ireturn, /* .opcode */ \
      "ireturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ireturn, /* return an integer */ \
   ) \
   MACRO(\
      TR::lreturn, /* .opcode */ \
      "lreturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lreturn, /* return a long integer */ \
   ) \
   MACRO(\
      TR::freturn, /* .opcode */ \
      "freturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      freturn, /* return a float */ \
   ) \
   MACRO(\
      TR::dreturn, /* .opcode */ \
      "dreturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dreturn, /* return a double */ \
   ) \
   MACRO(\
      TR::areturn, /* .opcode */ \
      "areturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      areturn, /* return an address */ \
   ) \
   MACRO(\
      TR::Return, /* .opcode */ \
      "return", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      Return, /* void return */ \
   ) \
   MACRO(\
      TR::asynccheck, /* .opcode */ \
      "asynccheck", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      asynccheck, /* GC point */ \
   ) \
   MACRO(\
      TR::athrow, /* .opcode */ \
      "athrow", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MustBeLowered | ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      athrow, /* throw an exception */ \
   ) \
   MACRO(\
      TR::icall, /* .opcode */ \
      "icall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      icall, /* direct call returning integer */ \
   ) \
   MACRO(\
      TR::lcall, /* .opcode */ \
      "lcall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lcall, /* direct call returning long integer */ \
   ) \
   MACRO(\
      TR::fcall, /* .opcode */ \
      "fcall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fcall, /* direct call returning float */ \
   ) \
   MACRO(\
      TR::dcall, /* .opcode */ \
      "dcall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcall, /* direct call returning double */ \
   ) \
   MACRO(\
      TR::acall, /* .opcode */ \
      "acall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      acall, /* direct call returning reference */ \
   ) \
   MACRO(\
      TR::call, /* .opcode */ \
      "call", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      call, /* direct call returning void */ \
   ) \
   MACRO(\
      TR::iadd, /* .opcode */ \
      "iadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iadd, /* add 2 integers */ \
   ) \
   MACRO(\
      TR::ladd, /* .opcode */ \
      "ladd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::ladd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ladd, /* add 2 long integers */ \
   ) \
   MACRO(\
      TR::fadd, /* .opcode */ \
      "fadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fadd, /* add 2 floats */ \
   ) \
   MACRO(\
      TR::dadd, /* .opcode */ \
      "dadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dadd, /* add 2 doubles */ \
   ) \
   MACRO(\
      TR::badd, /* .opcode */ \
      "badd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::badd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      badd, /* add 2 bytes */ \
   ) \
   MACRO(\
      TR::sadd, /* .opcode */ \
      "sadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sadd, /* add 2 short integers */ \
   ) \
   MACRO(\
      TR::isub, /* .opcode */ \
      "isub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      isub, /* subtract 2 integers                (child1 - child2) */ \
   ) \
   MACRO(\
      TR::lsub, /* .opcode */ \
      "lsub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lsub, /* subtract 2 long integers           (child1 - child2) */ \
   ) \
   MACRO(\
      TR::fsub, /* .opcode */ \
      "fsub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fsub, /* subtract 2 floats                  (child1 - child2) */ \
   ) \
   MACRO(\
      TR::dsub, /* .opcode */ \
      "dsub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dsub, /* subtract 2 doubles                 (child1 - child2) */ \
   ) \
   MACRO(\
      TR::bsub, /* .opcode */ \
      "bsub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bsub, /* subtract 2 bytes                   (child1 - child2) */ \
   ) \
   MACRO(\
      TR::ssub, /* .opcode */ \
      "ssub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ssub, /* subtract 2 short integers          (child1 - child2) */ \
   ) \
   MACRO(\
      TR::asub, /* .opcode */ \
      "asub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      asub, /* subtract 2 addresses (child1 - child2) */ \
   ) \
   MACRO(\
      TR::imul, /* .opcode */ \
      "imul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::imul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      imul, /* multiply 2 integers */ \
   ) \
   MACRO(\
      TR::lmul, /* .opcode */ \
      "lmul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lmul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lmul, /* multiply 2 signed or unsigned long integers */ \
   ) \
   MACRO(\
      TR::fmul, /* .opcode */ \
      "fmul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fmul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fmul, /* multiply 2 floats */ \
   ) \
   MACRO(\
      TR::dmul, /* .opcode */ \
      "dmul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dmul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dmul, /* multiply 2 doubles */ \
   ) \
   MACRO(\
      TR::bmul, /* .opcode */ \
      "bmul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bmul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bmul, /* multiply 2 bytes */ \
   ) \
   MACRO(\
      TR::smul, /* .opcode */ \
      "smul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::smul, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      smul, /* multiply 2 short integers */ \
   ) \
   MACRO(\
      TR::idiv, /* .opcode */ \
      "idiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      idiv, /* divide 2 integers                (child1 / child2) */ \
   ) \
   MACRO(\
      TR::ldiv, /* .opcode */ \
      "ldiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ldiv, /* divide 2 long integers           (child1 / child2) */ \
   ) \
   MACRO(\
      TR::fdiv, /* .opcode */ \
      "fdiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fdiv, /* divide 2 floats                  (child1 / child2) */ \
   ) \
   MACRO(\
      TR::ddiv, /* .opcode */ \
      "ddiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ddiv, /* divide 2 doubles                 (child1 / child2) */ \
   ) \
   MACRO(\
      TR::bdiv, /* .opcode */ \
      "bdiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bdiv, /* divide 2 bytes                   (child1 / child2) */ \
   ) \
   MACRO(\
      TR::sdiv, /* .opcode */ \
      "sdiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sdiv, /* divide 2 short integers          (child1 / child2) */ \
   ) \
   MACRO(\
      TR::iudiv, /* .opcode */ \
      "iudiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iudiv, /* divide 2 unsigned integers       (child1 / child2) */ \
   ) \
   MACRO(\
      TR::ludiv, /* .opcode */ \
      "ludiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ludiv, /* divide 2 unsigned long integers  (child1 / child2) */ \
   ) \
   MACRO(\
      TR::irem, /* .opcode */ \
      "irem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      irem, /* remainder of 2 integers                (child1 % child2) */ \
   ) \
   MACRO(\
      TR::lrem, /* .opcode */ \
      "lrem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lrem, /* remainder of 2 long integers           (child1 % child2) */ \
   ) \
   MACRO(\
      TR::frem, /* .opcode */ \
      "frem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      frem, /* remainder of 2 floats                  (child1 % child2) */ \
   ) \
   MACRO(\
      TR::drem, /* .opcode */ \
      "drem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      drem, /* remainder of 2 doubles                 (child1 % child2) */ \
   ) \
   MACRO(\
      TR::brem, /* .opcode */ \
      "brem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      brem, /* remainder of 2 bytes                   (child1 % child2) */ \
   ) \
   MACRO(\
      TR::srem, /* .opcode */ \
      "srem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      srem, /* remainder of 2 short integers          (child1 % child2) */ \
   ) \
   MACRO(\
      TR::iurem, /* .opcode */ \
      "iurem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iurem, /* remainder of 2 unsigned integers       (child1 % child2) */ \
   ) \
   MACRO(\
      TR::ineg, /* .opcode */ \
      "ineg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ineg, /* negate an integer */ \
   ) \
   MACRO(\
      TR::lneg, /* .opcode */ \
      "lneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lneg, /* negate a long integer */ \
   ) \
   MACRO(\
      TR::fneg, /* .opcode */ \
      "fneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fneg, /* negate a float */ \
   ) \
   MACRO(\
      TR::dneg, /* .opcode */ \
      "dneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dneg, /* negate a double */ \
   ) \
   MACRO(\
      TR::bneg, /* .opcode */ \
      "bneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bneg, /* negate a bytes */ \
   ) \
   MACRO(\
      TR::sneg, /* .opcode */ \
      "sneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sneg, /* negate a short integer */ \
   ) \
   MACRO(\
      TR::iabs, /* .opcode */ \
      "iabs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::Abs, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iabs, /* absolute value of integer */ \
   ) \
   MACRO(\
      TR::labs, /* .opcode */ \
      "labs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::Abs, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      labs, /* absolute value of long */ \
   ) \
   MACRO(\
      TR::fabs, /* .opcode */ \
      "fabs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::Abs, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fabs, /* absolute value of float */ \
   ) \
   MACRO(\
      TR::dabs, /* .opcode */ \
      "dabs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::Abs, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dabs, /* absolute value of double */ \
   ) \
   MACRO(\
      TR::ishl, /* .opcode */ \
      "ishl", /* .name */ \
      ILProp1::LeftShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ishl, /* shift integer left                (child1 << child2) */ \
   ) \
   MACRO(\
      TR::lshl, /* .opcode */ \
      "lshl", /* .name */ \
      ILProp1::LeftShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int64, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lshl, /* shift long integer left           (child1 << child2) */ \
   ) \
   MACRO(\
      TR::bshl, /* .opcode */ \
      "bshl", /* .name */ \
      ILProp1::LeftShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int8, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bshl, /* shift byte left                   (child1 << child2) */ \
   ) \
   MACRO(\
      TR::sshl, /* .opcode */ \
      "sshl", /* .name */ \
      ILProp1::LeftShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int16, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sshl, /* shift short integer left          (child1 << child2) */ \
   ) \
   MACRO(\
      TR::ishr, /* .opcode */ \
      "ishr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ishr, /* shift integer right arithmetically               (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::lshr, /* .opcode */ \
      "lshr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int64, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lshr, /* shift long integer right arithmetically          (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::bshr, /* .opcode */ \
      "bshr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int8, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bshr, /* shift byte right arithmetically                  (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::sshr, /* .opcode */ \
      "sshr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int16, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sshr, /* shift short integer arithmetically               (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::iushr, /* .opcode */ \
      "iushr", /* .name */ \
      ILProp1::RightShift | ILProp1::ShiftLogical, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iushr, /* shift integer right logically                   (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::lushr, /* .opcode */ \
      "lushr", /* .name */ \
      ILProp1::RightShift | ILProp1::ShiftLogical, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int64, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lushr, /* shift long integer right logically              (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::bushr, /* .opcode */ \
      "bushr", /* .name */ \
      ILProp1::RightShift | ILProp1::ShiftLogical, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int8, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bushr, /* shift byte right logically                      (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::sushr, /* .opcode */ \
      "sushr", /* .name */ \
      ILProp1::RightShift | ILProp1::ShiftLogical, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int16, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sushr, /* shift short integer right logically             (child1 >> child2) */ \
   ) \
   MACRO(\
      TR::irol, /* .opcode */ \
      "irol", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::LeftRotate, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      irol, /* rotate integer left */ \
   ) \
   MACRO(\
      TR::lrol, /* .opcode */ \
      "lrol", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::LeftRotate, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Int64, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lrol, /* rotate long integer left */ \
   ) \
   MACRO(\
      TR::iand, /* .opcode */ \
      "iand", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::And, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iand, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iand, /* boolean and of 2 integers */ \
   ) \
   MACRO(\
      TR::land, /* .opcode */ \
      "land", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::And, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::land, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      land, /* boolean and of 2 long integers */ \
   ) \
   MACRO(\
      TR::band, /* .opcode */ \
      "band", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::And, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::band, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      band, /* boolean and of 2 bytes */ \
   ) \
   MACRO(\
      TR::sand, /* .opcode */ \
      "sand", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::And, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sand, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sand, /* boolean and of 2 short integers */ \
   ) \
   MACRO(\
      TR::ior, /* .opcode */ \
      "ior", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ior, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ior, /* boolean or of 2 integers */ \
   ) \
   MACRO(\
      TR::lor, /* .opcode */ \
      "lor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lor, /* boolean or of 2 long integers */ \
   ) \
   MACRO(\
      TR::bor, /* .opcode */ \
      "bor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bor, /* boolean or of 2 bytes */ \
   ) \
   MACRO(\
      TR::sor, /* .opcode */ \
      "sor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sor, /* boolean or of 2 short integers */ \
   ) \
   MACRO(\
      TR::ixor, /* .opcode */ \
      "ixor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ixor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ixor, /* boolean xor of 2 integers */ \
   ) \
   MACRO(\
      TR::lxor, /* .opcode */ \
      "lxor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lxor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lxor, /* boolean xor of 2 long integers */ \
   ) \
   MACRO(\
      TR::bxor, /* .opcode */ \
      "bxor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bxor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bxor, /* boolean xor of 2 bytes */ \
   ) \
   MACRO(\
      TR::sxor, /* .opcode */ \
      "sxor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sxor, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sxor, /* boolean xor of 2 short integers */ \
   ) \
   MACRO(\
      TR::i2l, /* .opcode */ \
      "i2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2l, /* convert integer to long integer with sign extension */ \
   ) \
   MACRO(\
      TR::i2f, /* .opcode */ \
      "i2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2f, /* convert integer to float */ \
   ) \
   MACRO(\
      TR::i2d, /* .opcode */ \
      "i2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2d, /* convert integer to double */ \
   ) \
   MACRO(\
      TR::i2b, /* .opcode */ \
      "i2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2b, /* convert integer to byte */ \
   ) \
   MACRO(\
      TR::i2s, /* .opcode */ \
      "i2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2s, /* convert integer to short integer */ \
   ) \
   MACRO(\
      TR::i2a, /* .opcode */ \
      "i2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      i2a, /* convert integer to address */ \
   ) \
   MACRO(\
      TR::iu2l, /* .opcode */ \
      "iu2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iu2l, /* convert unsigned integer to long integer with zero extension */ \
   ) \
   MACRO(\
      TR::iu2f, /* .opcode */ \
      "iu2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iu2f, /* convert unsigned integer to float */ \
   ) \
   MACRO(\
      TR::iu2d, /* .opcode */ \
      "iu2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iu2d, /* convert unsigned integer to double */ \
   ) \
   MACRO(\
      TR::iu2a, /* .opcode */ \
      "iu2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iu2a, /* convert unsigned integer to address */ \
   ) \
   MACRO(\
      TR::l2i, /* .opcode */ \
      "l2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2i, /* convert long integer to integer */ \
   ) \
   MACRO(\
      TR::l2f, /* .opcode */ \
      "l2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2f, /* convert long integer to float */ \
   ) \
   MACRO(\
      TR::l2d, /* .opcode */ \
      "l2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2d, /* convert long integer to double */ \
   ) \
   MACRO(\
      TR::l2b, /* .opcode */ \
      "l2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2b, /* convert long integer to byte */ \
   ) \
   MACRO(\
      TR::l2s, /* .opcode */ \
      "l2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2s, /* convert long integer to short integer */ \
   ) \
   MACRO(\
      TR::l2a, /* .opcode */ \
      "l2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      l2a, /* convert long integer to address */ \
   ) \
   MACRO(\
      TR::lu2f, /* .opcode */ \
      "lu2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lu2f, /* convert unsigned long integer to float */ \
   ) \
   MACRO(\
      TR::lu2d, /* .opcode */ \
      "lu2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lu2d, /* convert unsigned long integer to double */ \
   ) \
   MACRO(\
      TR::lu2a, /* .opcode */ \
      "lu2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lu2a, /* convert unsigned long integer to address */ \
   ) \
   MACRO(\
      TR::f2i, /* .opcode */ \
      "f2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2i, /* convert float to integer */ \
   ) \
   MACRO(\
      TR::f2l, /* .opcode */ \
      "f2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2l, /* convert float to long integer */ \
   ) \
   MACRO(\
      TR::f2d, /* .opcode */ \
      "f2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2d, /* convert float to double */ \
   ) \
   MACRO(\
      TR::f2b, /* .opcode */ \
      "f2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2b, /* convert float to byte */ \
   ) \
   MACRO(\
      TR::f2s, /* .opcode */ \
      "f2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2s, /* convert float to short integer */ \
   ) \
   MACRO(\
      TR::d2i, /* .opcode */ \
      "d2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2i, /* convert double to integer */ \
   ) \
   MACRO(\
      TR::d2l, /* .opcode */ \
      "d2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2l, /* convert double to long integer */ \
   ) \
   MACRO(\
      TR::d2f, /* .opcode */ \
      "d2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2f, /* convert double to float */ \
   ) \
   MACRO(\
      TR::d2b, /* .opcode */ \
      "d2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2b, /* convert double to byte */ \
   ) \
   MACRO(\
      TR::d2s, /* .opcode */ \
      "d2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2s, /* convert double to short integer */ \
   ) \
   MACRO(\
      TR::b2i, /* .opcode */ \
      "b2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2i, /* convert byte to integer with sign extension */ \
   ) \
   MACRO(\
      TR::b2l, /* .opcode */ \
      "b2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2l, /* convert byte to long integer with sign extension */ \
   ) \
   MACRO(\
      TR::b2f, /* .opcode */ \
      "b2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2f, /* convert byte to float */ \
   ) \
   MACRO(\
      TR::b2d, /* .opcode */ \
      "b2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2d, /* convert byte to double */ \
   ) \
   MACRO(\
      TR::b2s, /* .opcode */ \
      "b2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2s, /* convert byte to short integer with sign extension */ \
   ) \
   MACRO(\
      TR::b2a, /* .opcode */ \
      "b2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      b2a, /* convert byte to address */ \
   ) \
   MACRO(\
      TR::bu2i, /* .opcode */ \
      "bu2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2i, /* convert byte to integer with zero extension */ \
   ) \
   MACRO(\
      TR::bu2l, /* .opcode */ \
      "bu2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2l, /* convert byte to long integer with zero extension */ \
   ) \
   MACRO(\
      TR::bu2f, /* .opcode */ \
      "bu2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2f, /* convert unsigned byte to float */ \
   ) \
   MACRO(\
      TR::bu2d, /* .opcode */ \
      "bu2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2d, /* convert unsigned byte to double */ \
   ) \
   MACRO(\
      TR::bu2s, /* .opcode */ \
      "bu2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2s, /* convert byte to short integer with zero extension */ \
   ) \
   MACRO(\
      TR::bu2a, /* .opcode */ \
      "bu2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bu2a, /* convert unsigned byte to unsigned address */ \
   ) \
   MACRO(\
      TR::s2i, /* .opcode */ \
      "s2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2i, /* convert short integer to integer with sign extension */ \
   ) \
   MACRO(\
      TR::s2l, /* .opcode */ \
      "s2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2l, /* convert short integer to long integer with sign extension */ \
   ) \
   MACRO(\
      TR::s2f, /* .opcode */ \
      "s2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2f, /* convert short integer to float */ \
   ) \
   MACRO(\
      TR::s2d, /* .opcode */ \
      "s2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2d, /* convert short integer to double */ \
   ) \
   MACRO(\
      TR::s2b, /* .opcode */ \
      "s2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2b, /* convert short integer to byte */ \
   ) \
   MACRO(\
      TR::s2a, /* .opcode */ \
      "s2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      s2a, /* convert short integer to address */ \
   ) \
   MACRO(\
      TR::su2i, /* .opcode */ \
      "su2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      su2i, /* zero extend short to int */ \
   ) \
   MACRO(\
      TR::su2l, /* .opcode */ \
      "su2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      su2l, /* zero extend char to long */ \
   ) \
   MACRO(\
      TR::su2f, /* .opcode */ \
      "su2f", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      su2f, /* convert char to float */ \
   ) \
   MACRO(\
      TR::su2d, /* .opcode */ \
      "su2d", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      su2d, /* convert char to double */ \
   ) \
   MACRO(\
      TR::su2a, /* .opcode */ \
      "su2a", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      su2a, /* convert char to address */ \
   ) \
   MACRO(\
      TR::a2i, /* .opcode */ \
      "a2i", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      a2i, /* convert address to integer */ \
   ) \
   MACRO(\
      TR::a2l, /* .opcode */ \
      "a2l", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      a2l, /* convert address to long integer */ \
   ) \
   MACRO(\
      TR::a2b, /* .opcode */ \
      "a2b", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      a2b, /* convert address to byte */ \
   ) \
   MACRO(\
      TR::a2s, /* .opcode */ \
      "a2s", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      a2s, /* convert address to short */ \
   ) \
   MACRO(\
      TR::icmpeq, /* .opcode */ \
      "icmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmpeq, /* .swapChildrenOpCode */ \
      TR::icmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmpeq, /* .ifCompareOpCode */ \
      icmpeq, /* integer compare if equal */ \
   ) \
   MACRO(\
      TR::icmpne, /* .opcode */ \
      "icmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmpne, /* .swapChildrenOpCode */ \
      TR::icmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmpne, /* .ifCompareOpCode */ \
      icmpne, /* integer compare if not equal */ \
   ) \
   MACRO(\
      TR::icmplt, /* .opcode */ \
      "icmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmpgt, /* .swapChildrenOpCode */ \
      TR::icmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmplt, /* .ifCompareOpCode */ \
      icmplt, /* integer compare if less than */ \
   ) \
   MACRO(\
      TR::icmpge, /* .opcode */ \
      "icmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmple, /* .swapChildrenOpCode */ \
      TR::icmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmpge, /* .ifCompareOpCode */ \
      icmpge, /* integer compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::icmpgt, /* .opcode */ \
      "icmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmplt, /* .swapChildrenOpCode */ \
      TR::icmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmpgt, /* .ifCompareOpCode */ \
      icmpgt, /* integer compare if greater than */ \
   ) \
   MACRO(\
      TR::icmple, /* .opcode */ \
      "icmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::icmpge, /* .swapChildrenOpCode */ \
      TR::icmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmple, /* .ifCompareOpCode */ \
      icmple, /* integer compare if less than or equal */ \
   ) \
   MACRO(\
      TR::iucmpeq, /* .opcode */ \
      "iucmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmpeq, /* .swapChildrenOpCode */ \
      TR::iucmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmpeq, /* .ifCompareOpCode */ \
      iucmpeq, /* unsigned integer compare if equal */ \
   ) \
   MACRO(\
      TR::iucmpne, /* .opcode */ \
      "iucmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmpne, /* .swapChildrenOpCode */ \
      TR::iucmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmpne, /* .ifCompareOpCode */ \
      iucmpne, /* unsigned integer compare if not equal */ \
   ) \
   MACRO(\
      TR::iucmplt, /* .opcode */ \
      "iucmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmpgt, /* .swapChildrenOpCode */ \
      TR::iucmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmplt, /* .ifCompareOpCode */ \
      iucmplt, /* unsigned integer compare if less than */ \
   ) \
   MACRO(\
      TR::iucmpge, /* .opcode */ \
      "iucmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmple, /* .swapChildrenOpCode */ \
      TR::iucmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmpge, /* .ifCompareOpCode */ \
      iucmpge, /* unsigned integer compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::iucmpgt, /* .opcode */ \
      "iucmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmplt, /* .swapChildrenOpCode */ \
      TR::iucmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmpgt, /* .ifCompareOpCode */ \
      iucmpgt, /* unsigned integer compare if greater than */ \
   ) \
   MACRO(\
      TR::iucmple, /* .opcode */ \
      "iucmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iucmpge, /* .swapChildrenOpCode */ \
      TR::iucmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifiucmple, /* .ifCompareOpCode */ \
      iucmple, /* unsigned integer compare if less than or equal */ \
   ) \
   MACRO(\
      TR::lcmpeq, /* .opcode */ \
      "lcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmpeq, /* .swapChildrenOpCode */ \
      TR::lcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmpeq, /* .ifCompareOpCode */ \
      lcmpeq, /* long compare if equal */ \
   ) \
   MACRO(\
      TR::lcmpne, /* .opcode */ \
      "lcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmpne, /* .swapChildrenOpCode */ \
      TR::lcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmpne, /* .ifCompareOpCode */ \
      lcmpne, /* long compare if not equal */ \
   ) \
   MACRO(\
      TR::lcmplt, /* .opcode */ \
      "lcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmpgt, /* .swapChildrenOpCode */ \
      TR::lcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmplt, /* .ifCompareOpCode */ \
      lcmplt, /* long compare if less than */ \
   ) \
   MACRO(\
      TR::lcmpge, /* .opcode */ \
      "lcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmple, /* .swapChildrenOpCode */ \
      TR::lcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmpge, /* .ifCompareOpCode */ \
      lcmpge, /* long compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::lcmpgt, /* .opcode */ \
      "lcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmplt, /* .swapChildrenOpCode */ \
      TR::lcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmpgt, /* .ifCompareOpCode */ \
      lcmpgt, /* long compare if greater than */ \
   ) \
   MACRO(\
      TR::lcmple, /* .opcode */ \
      "lcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lcmpge, /* .swapChildrenOpCode */ \
      TR::lcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflcmple, /* .ifCompareOpCode */ \
      lcmple, /* long compare if less than or equal */ \
   ) \
   MACRO(\
      TR::lucmpeq, /* .opcode */ \
      "lucmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmpeq, /* .swapChildrenOpCode */ \
      TR::lucmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmpeq, /* .ifCompareOpCode */ \
      lucmpeq, /* unsigned long compare if equal */ \
   ) \
   MACRO(\
      TR::lucmpne, /* .opcode */ \
      "lucmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmpne, /* .swapChildrenOpCode */ \
      TR::lucmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmpne, /* .ifCompareOpCode */ \
      lucmpne, /* unsigned long compare if not equal */ \
   ) \
   MACRO(\
      TR::lucmplt, /* .opcode */ \
      "lucmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmpgt, /* .swapChildrenOpCode */ \
      TR::lucmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmplt, /* .ifCompareOpCode */ \
      lucmplt, /* unsigned long compare if less than */ \
   ) \
   MACRO(\
      TR::lucmpge, /* .opcode */ \
      "lucmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmple, /* .swapChildrenOpCode */ \
      TR::lucmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmpge, /* .ifCompareOpCode */ \
      lucmpge, /* unsigned long compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::lucmpgt, /* .opcode */ \
      "lucmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmplt, /* .swapChildrenOpCode */ \
      TR::lucmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmpgt, /* .ifCompareOpCode */ \
      lucmpgt, /* unsigned long compare if greater than */ \
   ) \
   MACRO(\
      TR::lucmple, /* .opcode */ \
      "lucmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lucmpge, /* .swapChildrenOpCode */ \
      TR::lucmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iflucmple, /* .ifCompareOpCode */ \
      lucmple, /* unsigned long compare if less than or equal */ \
   ) \
   MACRO(\
      TR::fcmpeq, /* .opcode */ \
      "fcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpeq, /* .swapChildrenOpCode */ \
      TR::fcmpneu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpeq, /* .ifCompareOpCode */ \
      fcmpeq, /* float compare if equal */ \
   ) \
   MACRO(\
      TR::fcmpne, /* .opcode */ \
      "fcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpne, /* .swapChildrenOpCode */ \
      TR::fcmpequ, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpne, /* .ifCompareOpCode */ \
      fcmpne, /* float compare if not equal */ \
   ) \
   MACRO(\
      TR::fcmplt, /* .opcode */ \
      "fcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpgt, /* .swapChildrenOpCode */ \
      TR::fcmpgeu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmplt, /* .ifCompareOpCode */ \
      fcmplt, /* float compare if less than */ \
   ) \
   MACRO(\
      TR::fcmpge, /* .opcode */ \
      "fcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmple, /* .swapChildrenOpCode */ \
      TR::fcmpltu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpge, /* .ifCompareOpCode */ \
      fcmpge, /* float compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::fcmpgt, /* .opcode */ \
      "fcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmplt, /* .swapChildrenOpCode */ \
      TR::fcmpleu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpgt, /* .ifCompareOpCode */ \
      fcmpgt, /* float compare if greater than */ \
   ) \
   MACRO(\
      TR::fcmple, /* .opcode */ \
      "fcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpge, /* .swapChildrenOpCode */ \
      TR::fcmpgtu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmple, /* .ifCompareOpCode */ \
      fcmple, /* float compare if less than or equal */ \
   ) \
   MACRO(\
      TR::fcmpequ, /* .opcode */ \
      "fcmpequ", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpequ, /* .swapChildrenOpCode */ \
      TR::fcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpequ, /* .ifCompareOpCode */ \
      fcmpequ, /* float compare if equal or unordered */ \
   ) \
   MACRO(\
      TR::fcmpneu, /* .opcode */ \
      "fcmpneu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpneu, /* .swapChildrenOpCode */ \
      TR::fcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpneu, /* .ifCompareOpCode */ \
      fcmpneu, /* float compare if not equal or unordered */ \
   ) \
   MACRO(\
      TR::fcmpltu, /* .opcode */ \
      "fcmpltu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpgtu, /* .swapChildrenOpCode */ \
      TR::fcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpltu, /* .ifCompareOpCode */ \
      fcmpltu, /* float compare if less than or unordered */ \
   ) \
   MACRO(\
      TR::fcmpgeu, /* .opcode */ \
      "fcmpgeu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpleu, /* .swapChildrenOpCode */ \
      TR::fcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpgeu, /* .ifCompareOpCode */ \
      fcmpgeu, /* float compare if greater than or equal or unordered */ \
   ) \
   MACRO(\
      TR::fcmpgtu, /* .opcode */ \
      "fcmpgtu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpltu, /* .swapChildrenOpCode */ \
      TR::fcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpgtu, /* .ifCompareOpCode */ \
      fcmpgtu, /* float compare if greater than or unordered */ \
   ) \
   MACRO(\
      TR::fcmpleu, /* .opcode */ \
      "fcmpleu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::fcmpgeu, /* .swapChildrenOpCode */ \
      TR::fcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::iffcmpleu, /* .ifCompareOpCode */ \
      fcmpleu, /* float compare if less than or equal or unordered */ \
   ) \
   MACRO(\
      TR::dcmpeq, /* .opcode */ \
      "dcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpeq, /* .swapChildrenOpCode */ \
      TR::dcmpneu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpeq, /* .ifCompareOpCode */ \
      dcmpeq, /* double compare if equal */ \
   ) \
   MACRO(\
      TR::dcmpne, /* .opcode */ \
      "dcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpne, /* .swapChildrenOpCode */ \
      TR::dcmpequ, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpne, /* .ifCompareOpCode */ \
      dcmpne, /* double compare if not equal */ \
   ) \
   MACRO(\
      TR::dcmplt, /* .opcode */ \
      "dcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpgt, /* .swapChildrenOpCode */ \
      TR::dcmpgeu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmplt, /* .ifCompareOpCode */ \
      dcmplt, /* double compare if less than */ \
   ) \
   MACRO(\
      TR::dcmpge, /* .opcode */ \
      "dcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmple, /* .swapChildrenOpCode */ \
      TR::dcmpltu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpge, /* .ifCompareOpCode */ \
      dcmpge, /* double compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::dcmpgt, /* .opcode */ \
      "dcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmplt, /* .swapChildrenOpCode */ \
      TR::dcmpleu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpgt, /* .ifCompareOpCode */ \
      dcmpgt, /* double compare if greater than */ \
   ) \
   MACRO(\
      TR::dcmple, /* .opcode */ \
      "dcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpge, /* .swapChildrenOpCode */ \
      TR::dcmpgtu, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmple, /* .ifCompareOpCode */ \
      dcmple, /* double compare if less than or equal */ \
   ) \
   MACRO(\
      TR::dcmpequ, /* .opcode */ \
      "dcmpequ", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpequ, /* .swapChildrenOpCode */ \
      TR::dcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpequ, /* .ifCompareOpCode */ \
      dcmpequ, /* double compare if equal or unordered */ \
   ) \
   MACRO(\
      TR::dcmpneu, /* .opcode */ \
      "dcmpneu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpneu, /* .swapChildrenOpCode */ \
      TR::dcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpneu, /* .ifCompareOpCode */ \
      dcmpneu, /* double compare if not equal or unordered */ \
   ) \
   MACRO(\
      TR::dcmpltu, /* .opcode */ \
      "dcmpltu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpgtu, /* .swapChildrenOpCode */ \
      TR::dcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpltu, /* .ifCompareOpCode */ \
      dcmpltu, /* double compare if less than or unordered */ \
   ) \
   MACRO(\
      TR::dcmpgeu, /* .opcode */ \
      "dcmpgeu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpleu, /* .swapChildrenOpCode */ \
      TR::dcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpgeu, /* .ifCompareOpCode */ \
      dcmpgeu, /* double compare if greater than or equal or unordered */ \
   ) \
   MACRO(\
      TR::dcmpgtu, /* .opcode */ \
      "dcmpgtu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpltu, /* .swapChildrenOpCode */ \
      TR::dcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpgtu, /* .ifCompareOpCode */ \
      dcmpgtu, /* double compare if greater than or unordered */ \
   ) \
   MACRO(\
      TR::dcmpleu, /* .opcode */ \
      "dcmpleu", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::dcmpgeu, /* .swapChildrenOpCode */ \
      TR::dcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifdcmpleu, /* .ifCompareOpCode */ \
      dcmpleu, /* double compare if less than or equal or unordered */ \
   ) \
   MACRO(\
      TR::acmpeq, /* .opcode */ \
      "acmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmpeq, /* .swapChildrenOpCode */ \
      TR::acmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmpeq, /* .ifCompareOpCode */ \
      acmpeq, /* address compare if equal */ \
   ) \
   MACRO(\
      TR::acmpne, /* .opcode */ \
      "acmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmpne, /* .swapChildrenOpCode */ \
      TR::acmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmpne, /* .ifCompareOpCode */ \
      acmpne, /* address compare if not equal */ \
   ) \
   MACRO(\
      TR::acmplt, /* .opcode */ \
      "acmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmpgt, /* .swapChildrenOpCode */ \
      TR::acmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmplt, /* .ifCompareOpCode */ \
      acmplt, /* address compare if less than */ \
   ) \
   MACRO(\
      TR::acmpge, /* .opcode */ \
      "acmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmple, /* .swapChildrenOpCode */ \
      TR::acmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmpge, /* .ifCompareOpCode */ \
      acmpge, /* address compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::acmpgt, /* .opcode */ \
      "acmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmplt, /* .swapChildrenOpCode */ \
      TR::acmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmpgt, /* .ifCompareOpCode */ \
      acmpgt, /* address compare if greater than */ \
   ) \
   MACRO(\
      TR::acmple, /* .opcode */ \
      "acmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::acmpge, /* .swapChildrenOpCode */ \
      TR::acmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifacmple, /* .ifCompareOpCode */ \
      acmple, /* address compare if less than or equal */ \
   ) \
   MACRO(\
      TR::bcmpeq, /* .opcode */ \
      "bcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmpeq, /* .swapChildrenOpCode */ \
      TR::bcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmpeq, /* .ifCompareOpCode */ \
      bcmpeq, /* byte compare if equal */ \
   ) \
   MACRO(\
      TR::bcmpne, /* .opcode */ \
      "bcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmpne, /* .swapChildrenOpCode */ \
      TR::bcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmpne, /* .ifCompareOpCode */ \
      bcmpne, /* byte compare if not equal */ \
   ) \
   MACRO(\
      TR::bcmplt, /* .opcode */ \
      "bcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmpgt, /* .swapChildrenOpCode */ \
      TR::bcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmplt, /* .ifCompareOpCode */ \
      bcmplt, /* byte compare if less than */ \
   ) \
   MACRO(\
      TR::bcmpge, /* .opcode */ \
      "bcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmple, /* .swapChildrenOpCode */ \
      TR::bcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmpge, /* .ifCompareOpCode */ \
      bcmpge, /* byte compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::bcmpgt, /* .opcode */ \
      "bcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmplt, /* .swapChildrenOpCode */ \
      TR::bcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmpgt, /* .ifCompareOpCode */ \
      bcmpgt, /* byte compare if greater than */ \
   ) \
   MACRO(\
      TR::bcmple, /* .opcode */ \
      "bcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bcmpge, /* .swapChildrenOpCode */ \
      TR::bcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbcmple, /* .ifCompareOpCode */ \
      bcmple, /* byte compare if less than or equal */ \
   ) \
   MACRO(\
      TR::bucmpeq, /* .opcode */ \
      "bucmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmpeq, /* .swapChildrenOpCode */ \
      TR::bucmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmpeq, /* .ifCompareOpCode */ \
      bucmpeq, /* unsigned byte compare if equal */ \
   ) \
   MACRO(\
      TR::bucmpne, /* .opcode */ \
      "bucmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmpne, /* .swapChildrenOpCode */ \
      TR::bucmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmpne, /* .ifCompareOpCode */ \
      bucmpne, /* unsigned byte compare if not equal */ \
   ) \
   MACRO(\
      TR::bucmplt, /* .opcode */ \
      "bucmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmpgt, /* .swapChildrenOpCode */ \
      TR::bucmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmplt, /* .ifCompareOpCode */ \
      bucmplt, /* unsigned byte compare if less than */ \
   ) \
   MACRO(\
      TR::bucmpge, /* .opcode */ \
      "bucmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmple, /* .swapChildrenOpCode */ \
      TR::bucmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmpge, /* .ifCompareOpCode */ \
      bucmpge, /* unsigned byte compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::bucmpgt, /* .opcode */ \
      "bucmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmplt, /* .swapChildrenOpCode */ \
      TR::bucmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmpgt, /* .ifCompareOpCode */ \
      bucmpgt, /* unsigned byte compare if greater than */ \
   ) \
   MACRO(\
      TR::bucmple, /* .opcode */ \
      "bucmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::bucmpge, /* .swapChildrenOpCode */ \
      TR::bucmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifbucmple, /* .ifCompareOpCode */ \
      bucmple, /* unsigned byte compare if less than or equal */ \
   ) \
   MACRO(\
      TR::scmpeq, /* .opcode */ \
      "scmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmpeq, /* .swapChildrenOpCode */ \
      TR::scmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmpeq, /* .ifCompareOpCode */ \
      scmpeq, /* short integer compare if equal */ \
   ) \
   MACRO(\
      TR::scmpne, /* .opcode */ \
      "scmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmpne, /* .swapChildrenOpCode */ \
      TR::scmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmpne, /* .ifCompareOpCode */ \
      scmpne, /* short integer compare if not equal */ \
   ) \
   MACRO(\
      TR::scmplt, /* .opcode */ \
      "scmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmpgt, /* .swapChildrenOpCode */ \
      TR::scmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmplt, /* .ifCompareOpCode */ \
      scmplt, /* short integer compare if less than */ \
   ) \
   MACRO(\
      TR::scmpge, /* .opcode */ \
      "scmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmple, /* .swapChildrenOpCode */ \
      TR::scmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmpge, /* .ifCompareOpCode */ \
      scmpge, /* short integer compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::scmpgt, /* .opcode */ \
      "scmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmplt, /* .swapChildrenOpCode */ \
      TR::scmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmpgt, /* .ifCompareOpCode */ \
      scmpgt, /* short integer compare if greater than */ \
   ) \
   MACRO(\
      TR::scmple, /* .opcode */ \
      "scmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::scmpge, /* .swapChildrenOpCode */ \
      TR::scmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifscmple, /* .ifCompareOpCode */ \
      scmple, /* short integer compare if less than or equal */ \
   ) \
   MACRO(\
      TR::sucmpeq, /* .opcode */ \
      "sucmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmpeq, /* .swapChildrenOpCode */ \
      TR::sucmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmpeq, /* .ifCompareOpCode */ \
      sucmpeq, /* char compare if equal */ \
   ) \
   MACRO(\
      TR::sucmpne, /* .opcode */ \
      "sucmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmpne, /* .swapChildrenOpCode */ \
      TR::sucmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmpne, /* .ifCompareOpCode */ \
      sucmpne, /* char compare if not equal */ \
   ) \
   MACRO(\
      TR::sucmplt, /* .opcode */ \
      "sucmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmpgt, /* .swapChildrenOpCode */ \
      TR::sucmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmplt, /* .ifCompareOpCode */ \
      sucmplt, /* char compare if less than */ \
   ) \
   MACRO(\
      TR::sucmpge, /* .opcode */ \
      "sucmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmple, /* .swapChildrenOpCode */ \
      TR::sucmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmpge, /* .ifCompareOpCode */ \
      sucmpge, /* char compare if greater than or equal */ \
   ) \
   MACRO(\
      TR::sucmpgt, /* .opcode */ \
      "sucmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmplt, /* .swapChildrenOpCode */ \
      TR::sucmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmpgt, /* .ifCompareOpCode */ \
      sucmpgt, /* char compare if greater than */ \
   ) \
   MACRO(\
      TR::sucmple, /* .opcode */ \
      "sucmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::sucmpge, /* .swapChildrenOpCode */ \
      TR::sucmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ifsucmple, /* .ifCompareOpCode */ \
      sucmple, /* char compare if less than or equal */ \
   ) \
   MACRO(\
      TR::lcmp, /* .opcode */ \
      "lcmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lcmp, /* long compare (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2) */ \
   ) \
   MACRO(\
      TR::fcmpl, /* .opcode */ \
      "fcmpl", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fcmpl, /* float compare l (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2 or unordered) */ \
   ) \
   MACRO(\
      TR::fcmpg, /* .opcode */ \
      "fcmpg", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fcmpg, /* float compare g (1 if child1 > child2 or unordered, 0 if child1 == child2, -1 if child1 < child2) */ \
   ) \
   MACRO(\
      TR::dcmpl, /* .opcode */ \
      "dcmpl", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcmpl, /* double compare l (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2 or unordered) */ \
   ) \
   MACRO(\
      TR::dcmpg, /* .opcode */ \
      "dcmpg", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcmpg, /* double compare g (1 if child1 > child2 or unordered, 0 if child1 == child2, -1 if child1 < child2) */ \
   ) \
   MACRO(\
      TR::ificmpeq, /* .opcode */ \
      "ificmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmpeq, /* .swapChildrenOpCode */ \
      TR::ificmpne, /* .reverseBranchOpCode */ \
      TR::icmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpeq, /* integer compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ificmpne, /* .opcode */ \
      "ificmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmpne, /* .swapChildrenOpCode */ \
      TR::ificmpeq, /* .reverseBranchOpCode */ \
      TR::icmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpne, /* integer compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ificmplt, /* .opcode */ \
      "ificmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmpgt, /* .swapChildrenOpCode */ \
      TR::ificmpge, /* .reverseBranchOpCode */ \
      TR::icmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmplt, /* integer compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ificmpge, /* .opcode */ \
      "ificmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmple, /* .swapChildrenOpCode */ \
      TR::ificmplt, /* .reverseBranchOpCode */ \
      TR::icmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpge, /* integer compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ificmpgt, /* .opcode */ \
      "ificmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmplt, /* .swapChildrenOpCode */ \
      TR::ificmple, /* .reverseBranchOpCode */ \
      TR::icmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpgt, /* integer compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ificmple, /* .opcode */ \
      "ificmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ificmpge, /* .swapChildrenOpCode */ \
      TR::ificmpgt, /* .reverseBranchOpCode */ \
      TR::icmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmple, /* integer compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifiucmpeq, /* .opcode */ \
      "ifiucmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmpeq, /* .swapChildrenOpCode */ \
      TR::ifiucmpne, /* .reverseBranchOpCode */ \
      TR::iucmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmpeq, /* unsigned integer compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifiucmpne, /* .opcode */ \
      "ifiucmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmpne, /* .swapChildrenOpCode */ \
      TR::ifiucmpeq, /* .reverseBranchOpCode */ \
      TR::iucmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmpne, /* unsigned integer compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifiucmplt, /* .opcode */ \
      "ifiucmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmpgt, /* .swapChildrenOpCode */ \
      TR::ifiucmpge, /* .reverseBranchOpCode */ \
      TR::iucmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmplt, /* unsigned integer compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifiucmpge, /* .opcode */ \
      "ifiucmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmple, /* .swapChildrenOpCode */ \
      TR::ifiucmplt, /* .reverseBranchOpCode */ \
      TR::iucmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmpge, /* unsigned integer compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifiucmpgt, /* .opcode */ \
      "ifiucmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmplt, /* .swapChildrenOpCode */ \
      TR::ifiucmple, /* .reverseBranchOpCode */ \
      TR::iucmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmpgt, /* unsigned integer compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifiucmple, /* .opcode */ \
      "ifiucmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::ifiucmpge, /* .swapChildrenOpCode */ \
      TR::ifiucmpgt, /* .reverseBranchOpCode */ \
      TR::iucmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifiucmple, /* unsigned integer compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::iflcmpeq, /* .opcode */ \
      "iflcmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmpeq, /* .swapChildrenOpCode */ \
      TR::iflcmpne, /* .reverseBranchOpCode */ \
      TR::lcmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpeq, /* long compare and branch if equal */ \
   ) \
   MACRO(\
      TR::iflcmpne, /* .opcode */ \
      "iflcmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmpne, /* .swapChildrenOpCode */ \
      TR::iflcmpeq, /* .reverseBranchOpCode */ \
      TR::lcmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpne, /* long compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::iflcmplt, /* .opcode */ \
      "iflcmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmpgt, /* .swapChildrenOpCode */ \
      TR::iflcmpge, /* .reverseBranchOpCode */ \
      TR::lcmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmplt, /* long compare and branch if less than */ \
   ) \
   MACRO(\
      TR::iflcmpge, /* .opcode */ \
      "iflcmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmple, /* .swapChildrenOpCode */ \
      TR::iflcmplt, /* .reverseBranchOpCode */ \
      TR::lcmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpge, /* long compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::iflcmpgt, /* .opcode */ \
      "iflcmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmplt, /* .swapChildrenOpCode */ \
      TR::iflcmple, /* .reverseBranchOpCode */ \
      TR::lcmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpgt, /* long compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::iflcmple, /* .opcode */ \
      "iflcmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflcmpge, /* .swapChildrenOpCode */ \
      TR::iflcmpgt, /* .reverseBranchOpCode */ \
      TR::lcmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmple, /* long compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::iflucmpeq, /* .opcode */ \
      "iflucmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmpeq, /* .swapChildrenOpCode */ \
      TR::iflucmpne, /* .reverseBranchOpCode */ \
      TR::lucmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmpeq, /* unsigned long compare and branch if equal */ \
   ) \
   MACRO(\
      TR::iflucmpne, /* .opcode */ \
      "iflucmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmpne, /* .swapChildrenOpCode */ \
      TR::iflucmpeq, /* .reverseBranchOpCode */ \
      TR::lucmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmpne, /* unsigned long compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::iflucmplt, /* .opcode */ \
      "iflucmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmpgt, /* .swapChildrenOpCode */ \
      TR::iflucmpge, /* .reverseBranchOpCode */ \
      TR::lucmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmplt, /* unsigned long compare and branch if less than */ \
   ) \
   MACRO(\
      TR::iflucmpge, /* .opcode */ \
      "iflucmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmple, /* .swapChildrenOpCode */ \
      TR::iflucmplt, /* .reverseBranchOpCode */ \
      TR::lucmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmpge, /* unsigned long compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::iflucmpgt, /* .opcode */ \
      "iflucmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmplt, /* .swapChildrenOpCode */ \
      TR::iflucmple, /* .reverseBranchOpCode */ \
      TR::lucmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmpgt, /* unsigned long compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::iflucmple, /* .opcode */ \
      "iflucmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::iflucmpge, /* .swapChildrenOpCode */ \
      TR::iflucmpgt, /* .reverseBranchOpCode */ \
      TR::lucmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflucmple, /* unsigned long compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::iffcmpeq, /* .opcode */ \
      "iffcmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpeq, /* .swapChildrenOpCode */ \
      TR::iffcmpneu, /* .reverseBranchOpCode */ \
      TR::fcmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpeq, /* float compare and branch if equal */ \
   ) \
   MACRO(\
      TR::iffcmpne, /* .opcode */ \
      "iffcmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpne, /* .swapChildrenOpCode */ \
      TR::iffcmpequ, /* .reverseBranchOpCode */ \
      TR::fcmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpne, /* float compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::iffcmplt, /* .opcode */ \
      "iffcmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpgt, /* .swapChildrenOpCode */ \
      TR::iffcmpgeu, /* .reverseBranchOpCode */ \
      TR::fcmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmplt, /* float compare and branch if less than */ \
   ) \
   MACRO(\
      TR::iffcmpge, /* .opcode */ \
      "iffcmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmple, /* .swapChildrenOpCode */ \
      TR::iffcmpltu, /* .reverseBranchOpCode */ \
      TR::fcmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpge, /* float compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::iffcmpgt, /* .opcode */ \
      "iffcmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmplt, /* .swapChildrenOpCode */ \
      TR::iffcmpleu, /* .reverseBranchOpCode */ \
      TR::fcmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpgt, /* float compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::iffcmple, /* .opcode */ \
      "iffcmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpge, /* .swapChildrenOpCode */ \
      TR::iffcmpgtu, /* .reverseBranchOpCode */ \
      TR::fcmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmple, /* float compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::iffcmpequ, /* .opcode */ \
      "iffcmpequ", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpequ, /* .swapChildrenOpCode */ \
      TR::iffcmpne, /* .reverseBranchOpCode */ \
      TR::fcmpequ, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpequ, /* float compare and branch if equal or unordered */ \
   ) \
   MACRO(\
      TR::iffcmpneu, /* .opcode */ \
      "iffcmpneu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpneu, /* .swapChildrenOpCode */ \
      TR::iffcmpeq, /* .reverseBranchOpCode */ \
      TR::fcmpneu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpneu, /* float compare and branch if not equal or unordered */ \
   ) \
   MACRO(\
      TR::iffcmpltu, /* .opcode */ \
      "iffcmpltu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpgtu, /* .swapChildrenOpCode */ \
      TR::iffcmpge, /* .reverseBranchOpCode */ \
      TR::fcmpltu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpltu, /* float compare and branch if less than or unordered */ \
   ) \
   MACRO(\
      TR::iffcmpgeu, /* .opcode */ \
      "iffcmpgeu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpleu, /* .swapChildrenOpCode */ \
      TR::iffcmplt, /* .reverseBranchOpCode */ \
      TR::fcmpgeu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpgeu, /* float compare and branch if greater than or equal or unordered */ \
   ) \
   MACRO(\
      TR::iffcmpgtu, /* .opcode */ \
      "iffcmpgtu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpltu, /* .swapChildrenOpCode */ \
      TR::iffcmple, /* .reverseBranchOpCode */ \
      TR::fcmpgtu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpgtu, /* float compare and branch if greater than or unordered */ \
   ) \
   MACRO(\
      TR::iffcmpleu, /* .opcode */ \
      "iffcmpleu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::iffcmpgeu, /* .swapChildrenOpCode */ \
      TR::iffcmpgt, /* .reverseBranchOpCode */ \
      TR::fcmpleu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iffcmpleu, /* float compare and branch if less than or equal or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpeq, /* .opcode */ \
      "ifdcmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpeq, /* .swapChildrenOpCode */ \
      TR::ifdcmpneu, /* .reverseBranchOpCode */ \
      TR::dcmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpeq, /* double compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifdcmpne, /* .opcode */ \
      "ifdcmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpne, /* .swapChildrenOpCode */ \
      TR::ifdcmpequ, /* .reverseBranchOpCode */ \
      TR::dcmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpne, /* double compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifdcmplt, /* .opcode */ \
      "ifdcmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpgt, /* .swapChildrenOpCode */ \
      TR::ifdcmpgeu, /* .reverseBranchOpCode */ \
      TR::dcmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmplt, /* double compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifdcmpge, /* .opcode */ \
      "ifdcmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmple, /* .swapChildrenOpCode */ \
      TR::ifdcmpltu, /* .reverseBranchOpCode */ \
      TR::dcmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpge, /* double compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifdcmpgt, /* .opcode */ \
      "ifdcmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmplt, /* .swapChildrenOpCode */ \
      TR::ifdcmpleu, /* .reverseBranchOpCode */ \
      TR::dcmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpgt, /* double compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifdcmple, /* .opcode */ \
      "ifdcmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpge, /* .swapChildrenOpCode */ \
      TR::ifdcmpgtu, /* .reverseBranchOpCode */ \
      TR::dcmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmple, /* double compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifdcmpequ, /* .opcode */ \
      "ifdcmpequ", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpequ, /* .swapChildrenOpCode */ \
      TR::ifdcmpne, /* .reverseBranchOpCode */ \
      TR::dcmpequ, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpequ, /* double compare and branch if equal or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpneu, /* .opcode */ \
      "ifdcmpneu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpneu, /* .swapChildrenOpCode */ \
      TR::ifdcmpeq, /* .reverseBranchOpCode */ \
      TR::dcmpneu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpneu, /* double compare and branch if not equal or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpltu, /* .opcode */ \
      "ifdcmpltu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpgtu, /* .swapChildrenOpCode */ \
      TR::ifdcmpge, /* .reverseBranchOpCode */ \
      TR::dcmpltu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpltu, /* double compare and branch if less than or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpgeu, /* .opcode */ \
      "ifdcmpgeu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpleu, /* .swapChildrenOpCode */ \
      TR::ifdcmplt, /* .reverseBranchOpCode */ \
      TR::dcmpgeu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpgeu, /* double compare and branch if greater than or equal or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpgtu, /* .opcode */ \
      "ifdcmpgtu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpltu, /* .swapChildrenOpCode */ \
      TR::ifdcmple, /* .reverseBranchOpCode */ \
      TR::dcmpgtu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpgtu, /* double compare and branch if greater than or unordered */ \
   ) \
   MACRO(\
      TR::ifdcmpleu, /* .opcode */ \
      "ifdcmpleu", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::ifdcmpgeu, /* .swapChildrenOpCode */ \
      TR::ifdcmpgt, /* .reverseBranchOpCode */ \
      TR::dcmpleu, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifdcmpleu, /* double compare and branch if less than or equal or unordered */ \
   ) \
   MACRO(\
      TR::ifacmpeq, /* .opcode */ \
      "ifacmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmpeq, /* .swapChildrenOpCode */ \
      TR::ifacmpne, /* .reverseBranchOpCode */ \
      TR::acmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmpeq, /* address compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifacmpne, /* .opcode */ \
      "ifacmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmpne, /* .swapChildrenOpCode */ \
      TR::ifacmpeq, /* .reverseBranchOpCode */ \
      TR::acmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmpne, /* address compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifacmplt, /* .opcode */ \
      "ifacmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmpgt, /* .swapChildrenOpCode */ \
      TR::ifacmpge, /* .reverseBranchOpCode */ \
      TR::acmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmplt, /* address compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifacmpge, /* .opcode */ \
      "ifacmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmple, /* .swapChildrenOpCode */ \
      TR::ifacmplt, /* .reverseBranchOpCode */ \
      TR::acmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmpge, /* address compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifacmpgt, /* .opcode */ \
      "ifacmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmplt, /* .swapChildrenOpCode */ \
      TR::ifacmple, /* .reverseBranchOpCode */ \
      TR::acmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmpgt, /* address compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifacmple, /* .opcode */ \
      "ifacmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::ifacmpge, /* .swapChildrenOpCode */ \
      TR::ifacmpgt, /* .reverseBranchOpCode */ \
      TR::acmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifacmple, /* address compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifbcmpeq, /* .opcode */ \
      "ifbcmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmpeq, /* .swapChildrenOpCode */ \
      TR::ifbcmpne, /* .reverseBranchOpCode */ \
      TR::bcmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmpeq, /* byte compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifbcmpne, /* .opcode */ \
      "ifbcmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmpne, /* .swapChildrenOpCode */ \
      TR::ifbcmpeq, /* .reverseBranchOpCode */ \
      TR::bcmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmpne, /* byte compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifbcmplt, /* .opcode */ \
      "ifbcmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmpgt, /* .swapChildrenOpCode */ \
      TR::ifbcmpge, /* .reverseBranchOpCode */ \
      TR::bcmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmplt, /* byte compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifbcmpge, /* .opcode */ \
      "ifbcmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmple, /* .swapChildrenOpCode */ \
      TR::ifbcmplt, /* .reverseBranchOpCode */ \
      TR::bcmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmpge, /* byte compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifbcmpgt, /* .opcode */ \
      "ifbcmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmplt, /* .swapChildrenOpCode */ \
      TR::ifbcmple, /* .reverseBranchOpCode */ \
      TR::bcmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmpgt, /* byte compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifbcmple, /* .opcode */ \
      "ifbcmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbcmpge, /* .swapChildrenOpCode */ \
      TR::ifbcmpgt, /* .reverseBranchOpCode */ \
      TR::bcmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbcmple, /* byte compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifbucmpeq, /* .opcode */ \
      "ifbucmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmpeq, /* .swapChildrenOpCode */ \
      TR::ifbucmpne, /* .reverseBranchOpCode */ \
      TR::bucmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmpeq, /* unsigned byte compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifbucmpne, /* .opcode */ \
      "ifbucmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmpne, /* .swapChildrenOpCode */ \
      TR::ifbucmpeq, /* .reverseBranchOpCode */ \
      TR::bucmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmpne, /* unsigned byte compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifbucmplt, /* .opcode */ \
      "ifbucmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmpgt, /* .swapChildrenOpCode */ \
      TR::ifbucmpge, /* .reverseBranchOpCode */ \
      TR::bucmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmplt, /* unsigned byte compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifbucmpge, /* .opcode */ \
      "ifbucmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmple, /* .swapChildrenOpCode */ \
      TR::ifbucmplt, /* .reverseBranchOpCode */ \
      TR::bucmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmpge, /* unsigned byte compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifbucmpgt, /* .opcode */ \
      "ifbucmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmplt, /* .swapChildrenOpCode */ \
      TR::ifbucmple, /* .reverseBranchOpCode */ \
      TR::bucmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmpgt, /* unsigned byte compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifbucmple, /* .opcode */ \
      "ifbucmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::ifbucmpge, /* .swapChildrenOpCode */ \
      TR::ifbucmpgt, /* .reverseBranchOpCode */ \
      TR::bucmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifbucmple, /* unsigned byte compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifscmpeq, /* .opcode */ \
      "ifscmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmpeq, /* .swapChildrenOpCode */ \
      TR::ifscmpne, /* .reverseBranchOpCode */ \
      TR::scmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmpeq, /* short integer compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifscmpne, /* .opcode */ \
      "ifscmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmpne, /* .swapChildrenOpCode */ \
      TR::ifscmpeq, /* .reverseBranchOpCode */ \
      TR::scmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmpne, /* short integer compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifscmplt, /* .opcode */ \
      "ifscmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmpgt, /* .swapChildrenOpCode */ \
      TR::ifscmpge, /* .reverseBranchOpCode */ \
      TR::scmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmplt, /* short integer compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifscmpge, /* .opcode */ \
      "ifscmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmple, /* .swapChildrenOpCode */ \
      TR::ifscmplt, /* .reverseBranchOpCode */ \
      TR::scmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmpge, /* short integer compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifscmpgt, /* .opcode */ \
      "ifscmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmplt, /* .swapChildrenOpCode */ \
      TR::ifscmple, /* .reverseBranchOpCode */ \
      TR::scmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmpgt, /* short integer compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifscmple, /* .opcode */ \
      "ifscmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifscmpge, /* .swapChildrenOpCode */ \
      TR::ifscmpgt, /* .reverseBranchOpCode */ \
      TR::scmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifscmple, /* short integer compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::ifsucmpeq, /* .opcode */ \
      "ifsucmpeq", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmpeq, /* .swapChildrenOpCode */ \
      TR::ifsucmpne, /* .reverseBranchOpCode */ \
      TR::sucmpeq, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmpeq, /* char compare and branch if equal */ \
   ) \
   MACRO(\
      TR::ifsucmpne, /* .opcode */ \
      "ifsucmpne", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmpne, /* .swapChildrenOpCode */ \
      TR::ifsucmpeq, /* .reverseBranchOpCode */ \
      TR::sucmpne, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmpne, /* char compare and branch if not equal */ \
   ) \
   MACRO(\
      TR::ifsucmplt, /* .opcode */ \
      "ifsucmplt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmpgt, /* .swapChildrenOpCode */ \
      TR::ifsucmpge, /* .reverseBranchOpCode */ \
      TR::sucmplt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmplt, /* char compare and branch if less than */ \
   ) \
   MACRO(\
      TR::ifsucmpge, /* .opcode */ \
      "ifsucmpge", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmple, /* .swapChildrenOpCode */ \
      TR::ifsucmplt, /* .reverseBranchOpCode */ \
      TR::sucmpge, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmpge, /* char compare and branch if greater than or equal */ \
   ) \
   MACRO(\
      TR::ifsucmpgt, /* .opcode */ \
      "ifsucmpgt", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmplt, /* .swapChildrenOpCode */ \
      TR::ifsucmple, /* .reverseBranchOpCode */ \
      TR::sucmpgt, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmpgt, /* char compare and branch if greater than */ \
   ) \
   MACRO(\
      TR::ifsucmple, /* .opcode */ \
      "ifsucmple", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::ifsucmpge, /* .swapChildrenOpCode */ \
      TR::ifsucmpgt, /* .reverseBranchOpCode */ \
      TR::sucmple, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ifsucmple, /* char compare and branch if less than or equal */ \
   ) \
   MACRO(\
      TR::loadaddr, /* .opcode */ \
      "loadaddr", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::LoadAddress, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      loadaddr, /* load address of non-heap storage item (Auto, Parm, Static or Method) */ \
   ) \
   MACRO(\
      TR::ZEROCHK, /* .opcode */ \
      "ZEROCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ZEROCHK, /* Zero-check an int.  Symref indicates call to perform when first child is zero.  Other children are arguments to the call. */ \
   ) \
   MACRO(\
      TR::callIf, /* .opcode */ \
      "callIf", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      CHILD_COUNT(ILChildProp::UnspecifiedChildCount) | FIRST_CHILD(TR::Int32) | SECOND_CHILD(ILChildProp::UnspecifiedChildType) | THIRD_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      callIf, /* Call symref if first child evaluates to true.  Other childrem are arguments to the call. */ \
   ) \
   MACRO(\
      TR::iRegLoad, /* .opcode */ \
      "iRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iRegLoad, /* Load integer global register */ \
   ) \
   MACRO(\
      TR::aRegLoad, /* .opcode */ \
      "aRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aRegLoad, /* Load address global register */ \
   ) \
   MACRO(\
      TR::lRegLoad, /* .opcode */ \
      "lRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lRegLoad, /* Load long integer global register */ \
   ) \
   MACRO(\
      TR::fRegLoad, /* .opcode */ \
      "fRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fRegLoad, /* Load float global register */ \
   ) \
   MACRO(\
      TR::dRegLoad, /* .opcode */ \
      "dRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dRegLoad, /* Load double global register */ \
   ) \
   MACRO(\
      TR::sRegLoad, /* .opcode */ \
      "sRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sRegLoad, /* Load short global register */ \
   ) \
   MACRO(\
      TR::bRegLoad, /* .opcode */ \
      "bRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bRegLoad, /* Load byte global register */ \
   ) \
   MACRO(\
      TR::iRegStore, /* .opcode */ \
      "iRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iRegStore, /* Store integer global register */ \
   ) \
   MACRO(\
      TR::aRegStore, /* .opcode */ \
      "aRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aRegStore, /* Store address global register */ \
   ) \
   MACRO(\
      TR::lRegStore, /* .opcode */ \
      "lRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lRegStore, /* Store long integer global register */ \
   ) \
   MACRO(\
      TR::fRegStore, /* .opcode */ \
      "fRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fRegStore, /* Store float global register */ \
   ) \
   MACRO(\
      TR::dRegStore, /* .opcode */ \
      "dRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dRegStore, /* Store double global register */ \
   ) \
   MACRO(\
      TR::sRegStore, /* .opcode */ \
      "sRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sRegStore, /* Store short global register */ \
   ) \
   MACRO(\
      TR::bRegStore, /* .opcode */ \
      "bRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bRegStore, /* Store byte global register */ \
   ) \
   MACRO(\
      TR::GlRegDeps, /* .opcode */ \
      "GlRegDeps", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      GlRegDeps, /* Global Register Dependency List */ \
   ) \
   MACRO(\
      TR::iselect, /* .opcode */ \
      "iselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Int32, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iselect, /* Select Operator:  Based on the result of the first child, take the value of the */ \
   ) \
   MACRO(\
      TR::lselect, /* .opcode */ \
      "lselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Int64, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lselect, /* second (first child evaluates to true) or third(first child evaluates to false) child */ \
   ) \
   MACRO(\
      TR::bselect, /* .opcode */ \
      "bselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Int8, TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bselect, \
   ) \
   MACRO(\
      TR::sselect, /* .opcode */ \
      "sselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Int16, TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sselect, \
   ) \
   MACRO(\
      TR::aselect, /* .opcode */ \
      "aselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Address, TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aselect, \
   ) \
   MACRO(\
      TR::fselect, /* .opcode */ \
      "fselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Float, TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fselect, \
   ) \
   MACRO(\
      TR::dselect, /* .opcode */ \
      "dselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Double, TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dselect, \
   ) \
   MACRO(\
      TR::treetop, /* .opcode */ \
      "treetop", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      treetop, /* tree top to anchor subtrees with side-effects */ \
   ) \
   MACRO(\
      TR::MethodEnterHook, /* .opcode */ \
      "MethodEnterHook", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      MethodEnterHook, /* called after a frame is built, temps initialized, and monitor acquired (if necessary) */ \
   ) \
   MACRO(\
      TR::MethodExitHook, /* .opcode */ \
      "MethodExitHook", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      MethodExitHook, /* called immediately before returning, frame not yet collapsed, monitor released (if necessary) */ \
   ) \
   MACRO(\
      TR::PassThrough, /* .opcode */ \
      "PassThrough", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      PassThrough, /* Dummy node that represents its single child. */ \
   ) \
   MACRO(\
      TR::compressedRefs, /* .opcode */ \
      "compressedRefs", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(ILChildProp::UnspecifiedChildType, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      compressedRefs, /* no-op anchor providing optimizable subexpressions used for compression/decompression.  First child is address load/store, second child is heap base displacement */ \
   ) \
   MACRO(\
      TR::BBStart, /* .opcode */ \
      "BBStart", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      BBStart, /* Start of Basic Block */ \
   ) \
   MACRO(\
      TR::BBEnd, /* .opcode */ \
      "BBEnd", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      BBEnd, /* End of Basic Block */ \
   ) \
   MACRO(\
      TR::virem, /* .opcode */ \
      "virem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      virem, /* vector integer remainder */ \
   ) \
   MACRO(\
      TR::vimin, /* .opcode */ \
      "vimin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vimin, /* vector integer minimum */ \
   ) \
   MACRO(\
      TR::vimax, /* .opcode */ \
      "vimax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vimax, /* vector integer maximum */ \
   ) \
   MACRO(\
      TR::vigetelem, /* .opcode */ \
      "vigetelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vigetelem, /* get vector int element */ \
   ) \
   MACRO(\
      TR::visetelem, /* .opcode */ \
      "visetelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      visetelem, /* set vector int element */ \
   ) \
   MACRO(\
      TR::vimergel, /* .opcode */ \
      "vimergel", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vimergel, /* vector int merge low */ \
   ) \
   MACRO(\
      TR::vimergeh, /* .opcode */ \
      "vimergeh", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vimergeh, /* vector int merge high */ \
   ) \
   MACRO(\
      TR::vicmpeq, /* .opcode */ \
      "vicmpeq", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpeq, /* vector integer compare equal  (return vector mask) */ \
   ) \
   MACRO(\
      TR::vicmpgt, /* .opcode */ \
      "vicmpgt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpgt, /* vector integer compare greater than */ \
   ) \
   MACRO(\
      TR::vicmpge, /* .opcode */ \
      "vicmpge", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpge, /* vector integer compare greater equal */ \
   ) \
   MACRO(\
      TR::vicmplt, /* .opcode */ \
      "vicmplt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmplt, /* vector integer compare less than */ \
   ) \
   MACRO(\
      TR::vicmple, /* .opcode */ \
      "vicmple", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmple, /* vector integer compare less equal */ \
   ) \
   MACRO(\
      TR::vicmpalleq, /* .opcode */ \
      "vicmpalleq", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpalleq, /* vector integer all equal (return boolean) */ \
   ) \
   MACRO(\
      TR::vicmpallne, /* .opcode */ \
      "vicmpallne", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpallne, /* vector integer all not equal */ \
   ) \
   MACRO(\
      TR::vicmpallgt, /* .opcode */ \
      "vicmpallgt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpallgt, /* vector integer all greater than */ \
   ) \
   MACRO(\
      TR::vicmpallge, /* .opcode */ \
      "vicmpallge", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpallge, /* vector integer all greater equal */ \
   ) \
   MACRO(\
      TR::vicmpalllt, /* .opcode */ \
      "vicmpalllt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpalllt, /* vector integer all less than */ \
   ) \
   MACRO(\
      TR::vicmpallle, /* .opcode */ \
      "vicmpallle", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpallle, /* vector integer all less equal */ \
   ) \
   MACRO(\
      TR::vicmpanyeq, /* .opcode */ \
      "vicmpanyeq", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanyeq, /* vector integer any equal */ \
   ) \
   MACRO(\
      TR::vicmpanyne, /* .opcode */ \
      "vicmpanyne", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanyne, /* vector integer any not equal */ \
   ) \
   MACRO(\
      TR::vicmpanygt, /* .opcode */ \
      "vicmpanygt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanygt, /* vector integer any greater than */ \
   ) \
   MACRO(\
      TR::vicmpanyge, /* .opcode */ \
      "vicmpanyge", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanyge, /* vector integer any greater equal */ \
   ) \
   MACRO(\
      TR::vicmpanylt, /* .opcode */ \
      "vicmpanylt", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanylt, /* vector integer any less than */ \
   ) \
   MACRO(\
      TR::vicmpanyle, /* .opcode */ \
      "vicmpanyle", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vicmpanyle, /* vector integer any less equal */ \
   ) \
   MACRO(\
      TR::vnot, /* .opcode */ \
      "vnot", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vnot, /* vector boolean not */ \
   ) \
   MACRO(\
      TR::vbitselect, /* .opcode */ \
      "vbitselect", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vbitselect, /* vector bit select */ \
   ) \
   MACRO(\
      TR::vperm, /* .opcode */ \
      "vperm", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vperm, /* vector permute */ \
   ) \
   MACRO(\
      TR::vsplats, /* .opcode */ \
      "vsplats", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vsplats, /* vector splats */ \
   ) \
   MACRO(\
      TR::vdmergel, /* .opcode */ \
      "vdmergel", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmergel, /* vector double merge low */ \
   ) \
   MACRO(\
      TR::vdmergeh, /* .opcode */ \
      "vdmergeh", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmergeh, /* vector double merge high */ \
   ) \
   MACRO(\
      TR::vdsetelem, /* .opcode */ \
      "vdsetelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdsetelem, /* set vector double element */ \
   ) \
   MACRO(\
      TR::vdgetelem, /* .opcode */ \
      "vdgetelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdgetelem, /* get vector double element */ \
   ) \
   MACRO(\
      TR::vdsel, /* .opcode */ \
      "vdsel", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdsel, /* get vector select double */ \
   ) \
   MACRO(\
      TR::vdrem, /* .opcode */ \
      "vdrem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdrem, /* vector double remainder */ \
   ) \
   MACRO(\
      TR::vdmadd, /* .opcode */ \
      "vdmadd", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmadd, /* vector double fused multiply add */ \
   ) \
   MACRO(\
      TR::vdnmsub, /* .opcode */ \
      "vdnmsub", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdnmsub, /* vector double fused negative multiply subtract */ \
   ) \
   MACRO(\
      TR::vdmsub, /* .opcode */ \
      "vdmsub", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmsub, /* vector double fused multiply subtract */ \
   ) \
   MACRO(\
      TR::vdmax, /* .opcode */ \
      "vdmax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmax, /* vector double maximum */ \
   ) \
   MACRO(\
      TR::vdmin, /* .opcode */ \
      "vdmin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdmin, /* vector double minimum */ \
   ) \
   MACRO(\
      TR::vdcmpeq, /* .opcode */ \
      "vdcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpeq, /* .swapChildrenOpCode */ \
      TR::vdcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpeq, /* vector double compare equal  (return vector mask) */ \
   ) \
   MACRO(\
      TR::vdcmpne, /* .opcode */ \
      "vdcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpne, /* .swapChildrenOpCode */ \
      TR::vdcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpne, /* vector double compare not equal  (return vector mask) */ \
   ) \
   MACRO(\
      TR::vdcmpgt, /* .opcode */ \
      "vdcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmplt, /* .swapChildrenOpCode */ \
      TR::vdcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpgt, /* vector double compare greater than */ \
   ) \
   MACRO(\
      TR::vdcmpge, /* .opcode */ \
      "vdcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmple, /* .swapChildrenOpCode */ \
      TR::vdcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpge, /* vector double compare greater equal */ \
   ) \
   MACRO(\
      TR::vdcmplt, /* .opcode */ \
      "vdcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpgt, /* .swapChildrenOpCode */ \
      TR::vdcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmplt, /* vector double compare less than */ \
   ) \
   MACRO(\
      TR::vdcmple, /* .opcode */ \
      "vdcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpge, /* .swapChildrenOpCode */ \
      TR::vdcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmple, /* vector double compare less equal */ \
   ) \
   MACRO(\
      TR::vdcmpalleq, /* .opcode */ \
      "vdcmpalleq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpalleq, /* .swapChildrenOpCode */ \
      TR::vdcmpallne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpalleq, /* vector double compare all equal  (return boolean) */ \
   ) \
   MACRO(\
      TR::vdcmpallne, /* .opcode */ \
      "vdcmpallne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpallne, /* .swapChildrenOpCode */ \
      TR::vdcmpalleq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpallne, /* vector double compare all not equal  (return boolean) */ \
   ) \
   MACRO(\
      TR::vdcmpallgt, /* .opcode */ \
      "vdcmpallgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpalllt, /* .swapChildrenOpCode */ \
      TR::vdcmpallle, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpallgt, /* vector double compare all greater than */ \
   ) \
   MACRO(\
      TR::vdcmpallge, /* .opcode */ \
      "vdcmpallge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpallle, /* .swapChildrenOpCode */ \
      TR::vdcmpalllt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpallge, /* vector double compare all greater equal */ \
   ) \
   MACRO(\
      TR::vdcmpalllt, /* .opcode */ \
      "vdcmpalllt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpallgt, /* .swapChildrenOpCode */ \
      TR::vdcmpallge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpalllt, /* vector double compare all less than */ \
   ) \
   MACRO(\
      TR::vdcmpallle, /* .opcode */ \
      "vdcmpallle", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpallge, /* .swapChildrenOpCode */ \
      TR::vdcmpallgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpallle, /* vector double compare all less equal */ \
   ) \
   MACRO(\
      TR::vdcmpanyeq, /* .opcode */ \
      "vdcmpanyeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanyeq, /* .swapChildrenOpCode */ \
      TR::vdcmpanyne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanyeq, /* vector double compare any equal  (return boolean) */ \
   ) \
   MACRO(\
      TR::vdcmpanyne, /* .opcode */ \
      "vdcmpanyne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanyne, /* .swapChildrenOpCode */ \
      TR::vdcmpanyeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanyne, /* vector double compare any not equal  (return boolean) */ \
   ) \
   MACRO(\
      TR::vdcmpanygt, /* .opcode */ \
      "vdcmpanygt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanylt, /* .swapChildrenOpCode */ \
      TR::vdcmpanyle, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanygt, /* vector double compare any greater than */ \
   ) \
   MACRO(\
      TR::vdcmpanyge, /* .opcode */ \
      "vdcmpanyge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanyle, /* .swapChildrenOpCode */ \
      TR::vdcmpanylt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanyge, /* vector double compare any greater equal */ \
   ) \
   MACRO(\
      TR::vdcmpanylt, /* .opcode */ \
      "vdcmpanylt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanygt, /* .swapChildrenOpCode */ \
      TR::vdcmpanyge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanylt, /* vector double compare any less than */ \
   ) \
   MACRO(\
      TR::vdcmpanyle, /* .opcode */ \
      "vdcmpanyle", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vdcmpanyge, /* .swapChildrenOpCode */ \
      TR::vdcmpanygt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdcmpanyle, /* vector double compare any less equal */ \
   ) \
   MACRO(\
      TR::vdsqrt, /* .opcode */ \
      "vdsqrt", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdsqrt, /* vector double square root */ \
   ) \
   MACRO(\
      TR::vdlog, /* .opcode */ \
      "vdlog", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdlog, /* vector double natural log */ \
   ) \
   MACRO(\
      TR::vinc, /* .opcode */ \
      "vinc", /* .name */ \
      ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vinc, /* vector increment */ \
   ) \
   MACRO(\
      TR::vdec, /* .opcode */ \
      "vdec", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdec, /* vector decrement */ \
   ) \
   MACRO(\
      TR::vneg, /* .opcode */ \
      "vneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vneg, /* vector negation */ \
   ) \
   MACRO(\
      TR::vcom, /* .opcode */ \
      "vcom", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcom, /* vector complement */ \
   ) \
   MACRO(\
      TR::vadd, /* .opcode */ \
      "vadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vadd, /* vector add */ \
   ) \
   MACRO(\
      TR::vsub, /* .opcode */ \
      "vsub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vsub, /* vector subtract */ \
   ) \
   MACRO(\
      TR::vmul, /* .opcode */ \
      "vmul", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vmul, /* vector multiply */ \
   ) \
   MACRO(\
      TR::vdiv, /* .opcode */ \
      "vdiv", /* .name */ \
      ILProp1::Div, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdiv, /* vector divide */ \
   ) \
   MACRO(\
      TR::vrem, /* .opcode */ \
      "vrem", /* .name */ \
      ILProp1::Rem, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vrem, /* vector remainder */ \
   ) \
   MACRO(\
      TR::vand, /* .opcode */ \
      "vand", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::And, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vand, /* vector logical AND */ \
   ) \
   MACRO(\
      TR::vor, /* .opcode */ \
      "vor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vor, /* vector logical OR */ \
   ) \
   MACRO(\
      TR::vxor, /* .opcode */ \
      "vxor", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vxor, /* vector exclusive OR integer */ \
   ) \
   MACRO(\
      TR::vshl, /* .opcode */ \
      "vshl", /* .name */ \
      ILProp1::LeftShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vshl, /* vector shift left */ \
   ) \
   MACRO(\
      TR::vushr, /* .opcode */ \
      "vushr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vushr, /* vector shift right logical */ \
   ) \
   MACRO(\
      TR::vshr, /* .opcode */ \
      "vshr", /* .name */ \
      ILProp1::RightShift, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vshr, /* vector shift right arithmetic */ \
   ) \
   MACRO(\
      TR::vcmpeq, /* .opcode */ \
      "vcmpeq", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpeq, /* .swapChildrenOpCode */ \
      TR::vcmpne, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::ificmpeq, /* .ifCompareOpCode */ \
      vcmpeq, /* vector compare equal */ \
   ) \
   MACRO(\
      TR::vcmpne, /* .opcode */ \
      "vcmpne", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpne, /* .swapChildrenOpCode */ \
      TR::vcmpeq, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcmpne, /* vector compare not equal */ \
   ) \
   MACRO(\
      TR::vcmplt, /* .opcode */ \
      "vcmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpgt, /* .swapChildrenOpCode */ \
      TR::vcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcmplt, /* vector compare less than */ \
   ) \
   MACRO(\
      TR::vucmplt, /* .opcode */ \
      "vucmplt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpgt, /* .swapChildrenOpCode */ \
      TR::vcmpge, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vucmplt, /* vector unsigned compare less than */ \
   ) \
   MACRO(\
      TR::vcmpgt, /* .opcode */ \
      "vcmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmplt, /* .swapChildrenOpCode */ \
      TR::vcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcmpgt, /* vector compare greater than */ \
   ) \
   MACRO(\
      TR::vucmpgt, /* .opcode */ \
      "vucmpgt", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmplt, /* .swapChildrenOpCode */ \
      TR::vcmple, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vucmpgt, /* vector unsigned compare greater than */ \
   ) \
   MACRO(\
      TR::vcmple, /* .opcode */ \
      "vcmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpge, /* .swapChildrenOpCode */ \
      TR::vcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcmple, /* vector compare less or equal */ \
   ) \
   MACRO(\
      TR::vucmple, /* .opcode */ \
      "vucmple", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmpge, /* .swapChildrenOpCode */ \
      TR::vcmpgt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vucmple, /* vector unsigned compare less or equal */ \
   ) \
   MACRO(\
      TR::vcmpge, /* .opcode */ \
      "vcmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmple, /* .swapChildrenOpCode */ \
      TR::vcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcmpge, /* vector compare greater or equal */ \
   ) \
   MACRO(\
      TR::vucmpge, /* .opcode */ \
      "vucmpge", /* .name */ \
      ILProp1::BooleanCompare, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, /* .properties2 */ \
      ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::vcmple, /* .swapChildrenOpCode */ \
      TR::vcmplt, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vucmpge, /* vector unsigned compare greater or equal */ \
   ) \
   MACRO(\
      TR::vload, /* .opcode */ \
      "vload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vload, /* load vector */ \
   ) \
   MACRO(\
      TR::vloadi, /* .opcode */ \
      "vloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vloadi, /* load indirect vector */ \
   ) \
   MACRO(\
      TR::vstore, /* .opcode */ \
      "vstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vstore, /* store vector */ \
   ) \
   MACRO(\
      TR::vstorei, /* .opcode */ \
      "vstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vstorei, /* store indirect vector */ \
   ) \
   MACRO(\
      TR::vrand, /* .opcode */ \
      "vrand", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::VectorReduction, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vrand, /* AND all elements into single value of element size */ \
   ) \
   MACRO(\
      TR::vreturn, /* .opcode */ \
      "vreturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vreturn, /* return a vector */ \
   ) \
   MACRO(\
      TR::vcall, /* .opcode */ \
      "vcall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcall, /* direct call returning a vector */ \
   ) \
   MACRO(\
      TR::vcalli, /* .opcode */ \
      "vcalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vcalli, /* indirect call returning a vector */ \
   ) \
   MACRO(\
      TR::vselect, /* .opcode */ \
      "vselect", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vselect, /* vector select operator */ \
   ) \
   MACRO(\
      TR::v2v, /* .opcode */ \
      "v2v", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      v2v, /* vector to vector conversion. preserves bit pattern (noop), only changes datatype */ \
   ) \
   MACRO(\
      TR::vl2vd, /* .opcode */ \
      "vl2vd", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vl2vd, /* vector to vector conversion. converts each long element to double */ \
   ) \
   MACRO(\
      TR::vconst, /* .opcode */ \
      "vconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vconst, /* vector constant */ \
   ) \
   MACRO(\
      TR::getvelem, /* .opcode */ \
      "getvelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::HasNoDataType, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      getvelem, /* get vector element, returns a scalar */ \
   ) \
   MACRO(\
      TR::vsetelem, /* .opcode */ \
      "vsetelem", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vsetelem, /* vector set element */ \
   ) \
   MACRO(\
      TR::vbRegLoad, /* .opcode */ \
      "vbRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt8, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vbRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::vsRegLoad, /* .opcode */ \
      "vsRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt16, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vsRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::viRegLoad, /* .opcode */ \
      "viRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      viRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::vlRegLoad, /* .opcode */ \
      "vlRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vlRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::vfRegLoad, /* .opcode */ \
      "vfRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorFloat, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vfRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::vdRegLoad, /* .opcode */ \
      "vdRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdRegLoad, /* Load vector global register */ \
   ) \
   MACRO(\
      TR::vbRegStore, /* .opcode */ \
      "vbRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt8, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vbRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::vsRegStore, /* .opcode */ \
      "vsRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt16, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vsRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::viRegStore, /* .opcode */ \
      "viRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt32, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      viRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::vlRegStore, /* .opcode */ \
      "vlRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorInt64, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vlRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::vfRegStore, /* .opcode */ \
      "vfRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorFloat, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vfRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::vdRegStore, /* .opcode */ \
      "vdRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::VectorDouble, /* .dataType */ \
      ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      vdRegStore, /* Store vector global register */ \
   ) \
   MACRO(\
      TR::iuconst, /* .opcode */ \
      "iuconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuconst, /* load unsigned integer constant (32-but unsigned) */ \
   ) \
   MACRO(\
      TR::luconst, /* .opcode */ \
      "luconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luconst, /* load unsigned long integer constant (64-bit unsigned) */ \
   ) \
   MACRO(\
      TR::buconst, /* .opcode */ \
      "buconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      buconst, /* load unsigned byte integer constant (8-bit unsigned) */ \
   ) \
   MACRO(\
      TR::iuload, /* .opcode */ \
      "iuload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuload, /* load unsigned integer */ \
   ) \
   MACRO(\
      TR::luload, /* .opcode */ \
      "luload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luload, /* load unsigned long integer */ \
   ) \
   MACRO(\
      TR::buload, /* .opcode */ \
      "buload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      buload, /* load unsigned byte */ \
   ) \
   MACRO(\
      TR::iuloadi, /* .opcode */ \
      "iuloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuloadi, /* load indirect unsigned integer */ \
   ) \
   MACRO(\
      TR::luloadi, /* .opcode */ \
      "luloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luloadi, /* load indirect unsigned long integer */ \
   ) \
   MACRO(\
      TR::buloadi, /* .opcode */ \
      "buloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      buloadi, /* load indirect unsigned byte */ \
   ) \
   MACRO(\
      TR::iustore, /* .opcode */ \
      "iustore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iustore, /* store unsigned integer */ \
   ) \
   MACRO(\
      TR::lustore, /* .opcode */ \
      "lustore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lustore, /* store unsigned long integer */ \
   ) \
   MACRO(\
      TR::bustore, /* .opcode */ \
      "bustore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bustore, /* store unsigned byte */ \
   ) \
   MACRO(\
      TR::iustorei, /* .opcode */ \
      "iustorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iustorei, /* store indirect unsigned integer       (child1 a, child2 i) */ \
   ) \
   MACRO(\
      TR::lustorei, /* .opcode */ \
      "lustorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lustorei, /* store indirect unsigned long integer  (child1 a, child2 l) */ \
   ) \
   MACRO(\
      TR::bustorei, /* .opcode */ \
      "bustorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bustorei, /* store indirect unsigned byte          (child1 a, child2 b) */ \
   ) \
   MACRO(\
      TR::iureturn, /* .opcode */ \
      "iureturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iureturn, /* return an unsigned integer */ \
   ) \
   MACRO(\
      TR::lureturn, /* .opcode */ \
      "lureturn", /* .name */ \
      ILProp1::Return | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lureturn, /* return a long unsigned integer */ \
   ) \
   MACRO(\
      TR::iucall, /* .opcode */ \
      "iucall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iucall, /* direct call returning unsigned integer */ \
   ) \
   MACRO(\
      TR::lucall, /* .opcode */ \
      "lucall", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lucall, /* direct call returning unsigned long integer */ \
   ) \
   MACRO(\
      TR::iuadd, /* .opcode */ \
      "iuadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iuadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuadd, /* add 2 unsigned integers */ \
   ) \
   MACRO(\
      TR::luadd, /* .opcode */ \
      "luadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::luadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luadd, /* add 2 unsigned long integers */ \
   ) \
   MACRO(\
      TR::buadd, /* .opcode */ \
      "buadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::buadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      buadd, /* add 2 unsigned bytes */ \
   ) \
   MACRO(\
      TR::iusub, /* .opcode */ \
      "iusub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iusub, /* subtract 2 unsigned integers       (child1 - child2) */ \
   ) \
   MACRO(\
      TR::lusub, /* .opcode */ \
      "lusub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lusub, /* subtract 2 unsigned long integers  (child1 - child2) */ \
   ) \
   MACRO(\
      TR::busub, /* .opcode */ \
      "busub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      busub, /* subtract 2 unsigned bytes          (child1 - child2) */ \
   ) \
   MACRO(\
      TR::iuneg, /* .opcode */ \
      "iuneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuneg, /* negate an unsigned integer */ \
   ) \
   MACRO(\
      TR::luneg, /* .opcode */ \
      "luneg", /* .name */ \
      ILProp1::Neg, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luneg, /* negate a unsigned long integer */ \
   ) \
   MACRO(\
      TR::f2iu, /* .opcode */ \
      "f2iu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2iu, /* convert float to unsigned integer */ \
   ) \
   MACRO(\
      TR::f2lu, /* .opcode */ \
      "f2lu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2lu, /* convert float to unsigned long integer */ \
   ) \
   MACRO(\
      TR::f2bu, /* .opcode */ \
      "f2bu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2bu, /* convert float to unsigned byte */ \
   ) \
   MACRO(\
      TR::f2c, /* .opcode */ \
      "f2c", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      f2c, /* convert float to char */ \
   ) \
   MACRO(\
      TR::d2iu, /* .opcode */ \
      "d2iu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2iu, /* convert double to unsigned integer */ \
   ) \
   MACRO(\
      TR::d2lu, /* .opcode */ \
      "d2lu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2lu, /* convert double to unsigned long integer */ \
   ) \
   MACRO(\
      TR::d2bu, /* .opcode */ \
      "d2bu", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2bu, /* convert double to unsigned byte */ \
   ) \
   MACRO(\
      TR::d2c, /* .opcode */ \
      "d2c", /* .name */ \
      ILProp1::Conversion, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      d2c, /* convert double to char */ \
   ) \
   MACRO(\
      TR::iuRegLoad, /* .opcode */ \
      "iuRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuRegLoad, /* Load unsigned integer global register */ \
   ) \
   MACRO(\
      TR::luRegLoad, /* .opcode */ \
      "luRegLoad", /* .name */ \
      ILProp1::LoadReg, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luRegLoad, /* Load unsigned long integer global register */ \
   ) \
   MACRO(\
      TR::iuRegStore, /* .opcode */ \
      "iuRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuRegStore, /* Store unsigned integer global register */ \
   ) \
   MACRO(\
      TR::luRegStore, /* .opcode */ \
      "luRegStore", /* .name */ \
      ILProp1::StoreReg | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luRegStore, /* Store long integer global register */ \
   ) \
   MACRO(\
      TR::cconst, /* .opcode */ \
      "cconst", /* .name */ \
      ILProp1::LoadConst, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cconst, /* load unicode constant (16-bit unsigned) */ \
   ) \
   MACRO(\
      TR::cload, /* .opcode */ \
      "cload", /* .name */ \
      ILProp1::LoadVar | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cload, /* load short unsigned integer */ \
   ) \
   MACRO(\
      TR::cloadi, /* .opcode */ \
      "cloadi", /* .name */ \
      ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cloadi, /* load indirect unsigned short integer */ \
   ) \
   MACRO(\
      TR::cstore, /* .opcode */ \
      "cstore", /* .name */ \
      ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ONE_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cstore, /* store unsigned short integer */ \
   ) \
   MACRO(\
      TR::cstorei, /* .opcode */ \
      "cstorei", /* .name */ \
      ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cstorei, /* store indirect unsigned short integer (child1 a, child2 c) */ \
   ) \
   MACRO(\
      TR::monent, /* .opcode */ \
      "monent", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      monent, /* acquire lock for synchronising method */ \
   ) \
   MACRO(\
      TR::monexit, /* .opcode */ \
      "monexit", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      monexit, /* release lock for synchronising method */ \
   ) \
   MACRO(\
      TR::monexitfence, /* .opcode */ \
      "monexitfence", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      monexitfence, /* denotes the end of a monitored region solely for live monitor meta data */ \
   ) \
   MACRO(\
      TR::tstart, /* .opcode */ \
      "tstart", /* .name */ \
      ILProp1::HasSymbolRef | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      ILProp3::HasBranchChild, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      THREE_SAME_CHILD(TR::NoType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      tstart, /* transaction begin */ \
   ) \
   MACRO(\
      TR::tfinish, /* .opcode */ \
      "tfinish", /* .name */ \
      ILProp1::HasSymbolRef | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      tfinish, /* transaction end */ \
   ) \
   MACRO(\
      TR::tabort, /* .opcode */ \
      "tabort", /* .name */ \
      ILProp1::HasSymbolRef | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      tabort, /* transaction abort */ \
   ) \
   MACRO(\
      TR::instanceof, /* .opcode */ \
      "instanceof", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      instanceof, /* instanceof - symref is the class object, cp index is in the */ \
   ) \
   MACRO(\
      TR::checkcast, /* .opcode */ \
      "checkcast", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::Check | ILProp2::CheckCast| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      checkcast, /* checkcast */ \
   ) \
   MACRO(\
      TR::checkcastAndNULLCHK, /* .opcode */ \
      "checkcastAndNULLCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::Check | ILProp2::CheckCast| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      checkcastAndNULLCHK, /* checkcast and NULL check the underlying object reference */ \
   ) \
   MACRO(\
      TR::New, /* .opcode */ \
      "new", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      New, /* new - child is class */ \
   ) \
   MACRO(\
      TR::newvalue, /* .opcode */ \
      "newvalue", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      newvalue, /* allocate and initialize - children provide field values */ \
   ) \
   MACRO(\
      TR::newarray, /* .opcode */ \
      "newarray", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      newarray, /* new array of primitives */ \
   ) \
   MACRO(\
      TR::anewarray, /* .opcode */ \
      "anewarray", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      anewarray, /* new array of objects */ \
   ) \
   MACRO(\
      TR::variableNew, /* .opcode */ \
      "variableNew", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      variableNew, /* new - child is class, type not known at compile time */ \
   ) \
   MACRO(\
      TR::variableNewArray, /* .opcode */ \
      "variableNewArray", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      variableNewArray, /* new array - type not known at compile time, type must be a j9class, do not use type enums */ \
   ) \
   MACRO(\
      TR::multianewarray, /* .opcode */ \
      "multianewarray", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MustBeLowered | ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      multianewarray, /* multi-dimensional new array of objects */ \
   ) \
   MACRO(\
      TR::arraylength, /* .opcode */ \
      "arraylength", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::ArrayLength, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraylength, /* number of elements in an array */ \
   ) \
   MACRO(\
      TR::contigarraylength, /* .opcode */ \
      "contigarraylength", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::ArrayLength, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      contigarraylength, /* number of elements in a contiguous array */ \
   ) \
   MACRO(\
      TR::discontigarraylength, /* .opcode */ \
      "discontigarraylength", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::ArrayLength, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      discontigarraylength, /* number of elements in a discontiguous array */ \
   ) \
   MACRO(\
      TR::icalli, /* .opcode */ \
      "icalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      icalli, /* indirect call returning integer (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::iucalli, /* .opcode */ \
      "iucalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iucalli, /* indirect call returning unsigned integer (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::lcalli, /* .opcode */ \
      "lcalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lcalli, /* indirect call returning long integer (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::lucalli, /* .opcode */ \
      "lucalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lucalli, /* indirect call returning unsigned long integer (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::fcalli, /* .opcode */ \
      "fcalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fcalli, /* indirect call returning float (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::dcalli, /* .opcode */ \
      "dcalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcalli, /* indirect call returning double (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::acalli, /* .opcode */ \
      "acalli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      acalli, /* indirect call returning reference */ \
   ) \
   MACRO(\
      TR::calli, /* .opcode */ \
      "calli", /* .name */ \
      ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::IndirectCallType, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      calli, /* indirect call returning void (child1 is addr of function) */ \
   ) \
   MACRO(\
      TR::fence, /* .opcode */ \
      "fence", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::Fence | ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fence, /* barrier to optimization */ \
   ) \
   MACRO(\
      TR::luaddh, /* .opcode */ \
      "luaddh", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::luaddh, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luaddh, /* add 2 unsigned long integers (the high parts of prior luadd) as high part of 128bit addition. */ \
   ) \
   MACRO(\
      TR::cadd, /* .opcode */ \
      "cadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::cadd, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cadd, /* add 2 unsigned short integers */ \
   ) \
   MACRO(\
      TR::aiadd, /* .opcode */ \
      "aiadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aiadd, /* add integer to address with address result (child1 a, child2 i) */ \
   ) \
   MACRO(\
      TR::aiuadd, /* .opcode */ \
      "aiuadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aiuadd, /* add unsigned integer to address with address result (child1 a, child2 i) */ \
   ) \
   MACRO(\
      TR::aladd, /* .opcode */ \
      "aladd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aladd, /* add long integer to address with address result (child1 a, child2 i) (64-bit only) */ \
   ) \
   MACRO(\
      TR::aluadd, /* .opcode */ \
      "aluadd", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, /* .properties1 */ \
      ILProp2::ValueNumberShare, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      aluadd, /* add unsigned long integer to address with address result (child1 a, child2 i) (64-bit only) */ \
   ) \
   MACRO(\
      TR::lusubh, /* .opcode */ \
      "lusubh", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lusubh, /* subtract 2 unsigned long integers (the high parts of prior lusub) as high part of 128bit subtraction. */ \
   ) \
   MACRO(\
      TR::csub, /* .opcode */ \
      "csub", /* .name */ \
      ILProp1::Sub, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      csub, /* subtract 2 unsigned short integers (child1 - child2) */ \
   ) \
   MACRO(\
      TR::imulh, /* .opcode */ \
      "imulh", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::imulh, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      imulh, /* multiply 2 integers, and return the high word of the product */ \
   ) \
   MACRO(\
      TR::iumulh, /* .opcode */ \
      "iumulh", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::iumulh, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iumulh, /* multiply 2 unsigned integers, and return the high word of the product */ \
   ) \
   MACRO(\
      TR::lmulh, /* .opcode */ \
      "lmulh", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lmulh, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lmulh, /* multiply 2 long integers, and return the high word of the product */ \
   ) \
   MACRO(\
      TR::lumulh, /* .opcode */ \
      "lumulh", /* .name */ \
      ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::lumulh, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lumulh, /* multiply 2 unsigned long integers, and return the high word of the product */ \
   ) \
   MACRO(\
      TR::ibits2f, /* .opcode */ \
      "ibits2f", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ibits2f, /* type-coerce int to float */ \
   ) \
   MACRO(\
      TR::fbits2i, /* .opcode */ \
      "fbits2i", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fbits2i, /* type-coerce float to int */ \
   ) \
   MACRO(\
      TR::lbits2d, /* .opcode */ \
      "lbits2d", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lbits2d, /* type-coerce long to double */ \
   ) \
   MACRO(\
      TR::dbits2l, /* .opcode */ \
      "dbits2l", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dbits2l, /* type-coerce double to long */ \
   ) \
   MACRO(\
      TR::lookup, /* .opcode */ \
      "lookup", /* .name */ \
      ILProp1::TreeTop | ILProp1::Switch, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      ILProp3::HasBranchChild, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lookup, /* lookupswitch (child1 is selector expression, child2 the default destination, subsequent children are case nodes */ \
   ) \
   MACRO(\
      TR::trtLookup, /* .opcode */ \
      "trtLookup", /* .name */ \
      ILProp1::TreeTop | ILProp1::Switch, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      ILProp3::HasBranchChild, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      trtLookup, /* special lookupswitch (child1 must be trt, child2 the default destination, subsequent children are case nodes) */ \
   ) \
   MACRO(\
      TR::Case, /* .opcode */ \
      "case", /* .name */ \
      ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      Case, /* case nodes that are children of TR_switch.  Uses the branchdestination and the int const field */ \
   ) \
   MACRO(\
      TR::table, /* .opcode */ \
      "table", /* .name */ \
      ILProp1::TreeTop | ILProp1::Switch, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      ILProp3::HasBranchChild, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      table, /* tableswitch (child1 is the selector, child2 the default destination, subsequent children are the branch targets */ \
   ) \
   MACRO(\
      TR::exceptionRangeFence, /* .opcode */ \
      "exceptionRangeFence", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::ExceptionRangeFence, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      exceptionRangeFence, /* (J9) SymbolReference is the aliasing effect, initializer is where the code address gets put when binary is generated */ \
   ) \
   MACRO(\
      TR::dbgFence, /* .opcode */ \
      "dbgFence", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::ExceptionRangeFence, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dbgFence, /* used to delimit code (stmts) for debug info.  Has no symbol reference. */ \
   ) \
   MACRO(\
      TR::NULLCHK, /* .opcode */ \
      "NULLCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::NullCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      NULLCHK, /* Null check a pointer.  child 1 is indirect reference. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::ResolveCHK, /* .opcode */ \
      "ResolveCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::ResolveCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ResolveCHK, /* Resolve check a static, field or method. child 1 is reference to be resolved. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::ResolveAndNULLCHK, /* .opcode */ \
      "ResolveAndNULLCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::NullCheck | ILProp2::ResolveCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ResolveAndNULLCHK, /* Resolve check a static, field or method and Null check the underlying pointer.  child 1 is reference to be resolved. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::DIVCHK, /* .opcode */ \
      "DIVCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      DIVCHK, /* Divide by zero check. child 1 is the divide. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::OverflowCHK, /* .opcode */ \
      "OverflowCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::Check| ILProp2::MayUseSystemStack | ILProp2::CanRaiseException, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      OverflowCHK, /* Overflow check. child 1 is the operation node(add, mul, sub). Child 2 and child 3 are the operands of the operation of the operation. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::UnsignedOverflowCHK, /* .opcode */ \
      "UnsignedOverflowCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::Check| ILProp2::MayUseSystemStack | ILProp2::CanRaiseException, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      UnsignedOverflowCHK, /* UnsignedOverflow check. child 1 is the operation node(add, mul, sub). Child 2 and child 3 are the operands of the operation of the operation. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::BNDCHK, /* .opcode */ \
      "BNDCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      BNDCHK, /* Array bounds check, checks that child 1 > child 2 >= 0 (child 1 is bound, 2 is index). Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::ArrayCopyBNDCHK, /* .opcode */ \
      "ArrayCopyBNDCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ArrayCopyBNDCHK, /* Array copy bounds check, checks that child 1 >= child 2. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::BNDCHKwithSpineCHK, /* .opcode */ \
      "BNDCHKwithSpineCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::SpineCheck, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      BNDCHKwithSpineCHK, /* Array bounds check and spine check */ \
   ) \
   MACRO(\
      TR::SpineCHK, /* .opcode */ \
      "SpineCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::Check, /* .properties2 */ \
      ILProp3::SpineCheck, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      SpineCHK, /* Check if the base array has a spine */ \
   ) \
   MACRO(\
      TR::ArrayStoreCHK, /* .opcode */ \
      "ArrayStoreCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ONE_CHILD(ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ArrayStoreCHK, /* Array store check. child 1 is object, 2 is array. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::ArrayCHK, /* .opcode */ \
      "ArrayCHK", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ArrayCHK, /* Array compatibility check. child 1 is object1, 2 is object2. Symbolref indicates failure action/destination */ \
   ) \
   MACRO(\
      TR::Ret, /* .opcode */ \
      "Ret", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      Ret, /* Used by ilGen only */ \
   ) \
   MACRO(\
      TR::arraycopy, /* .opcode */ \
      "arraycopy", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::MayUseSystemStack | ILProp2::CanRaiseException | 0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraycopy, /* Call to System.arraycopy that may be partially inlined */ \
   ) \
   MACRO(\
      TR::arrayset, /* .opcode */ \
      "arrayset", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arrayset, /* Inline code for memory initialization of part of an array */ \
   ) \
   MACRO(\
      TR::arraytranslate, /* .opcode */ \
      "arraytranslate", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraytranslate, /* Inline code for translation of part of an array to another form via lookup */ \
   ) \
   MACRO(\
      TR::arraytranslateAndTest, /* .opcode */ \
      "arraytranslateAndTest", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException | ILProp2::BndCheck, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraytranslateAndTest, /* Inline code for scanning of part of an array for a particular 8-bit character */ \
   ) \
   MACRO(\
      TR::long2String, /* .opcode */ \
      "long2String", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      long2String, /* Convert integer/long value to String */ \
   ) \
   MACRO(\
      TR::bitOpMem, /* .opcode */ \
      "bitOpMem", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bitOpMem, /* bit operations (AND, OR, XOR) for memory to memory */ \
   ) \
   MACRO(\
      TR::bitOpMemND, /* .opcode */ \
      "bitOpMemND", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bitOpMemND, /* 3 operand(source1,source2,target) version of bitOpMem */ \
   ) \
   MACRO(\
      TR::arraycmp, /* .opcode */ \
      "arraycmp", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraycmp, /* Inline code for memory comparison of part of an array */ \
   ) \
   MACRO(\
      TR::arraycmpWithPad, /* .opcode */ \
      "arraycmpWithPad", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      arraycmpWithPad, /* memory comparison when src1 length != src2 length and padding is needed */ \
   ) \
   MACRO(\
      TR::allocationFence, /* .opcode */ \
      "allocationFence", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      allocationFence, /* Internal fence guarding escape of newObject & final fields - eliminatable */ \
   ) \
   MACRO(\
      TR::loadFence, /* .opcode */ \
      "loadFence", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      loadFence, /* JEP171: prohibits loadLoad and loadStore reordering (on globals) */ \
   ) \
   MACRO(\
      TR::storeFence, /* .opcode */ \
      "storeFence", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      storeFence, /* JEP171: prohibits loadStore and storeStore reordering (on globals) */ \
   ) \
   MACRO(\
      TR::fullFence, /* .opcode */ \
      "fullFence", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fullFence, /* JEP171: prohibits loadLoad, loadStore, storeLoad, and storeStore reordering (on globals) */ \
   ) \
   MACRO(\
      TR::MergeNew, /* .opcode */ \
      "MergeNew", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      MergeNew, /* Parent for New etc. nodes that can all be allocated together */ \
   ) \
   MACRO(\
      TR::computeCC, /* .opcode */ \
      "computeCC", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      computeCC, /* compute Condition Codes */ \
   ) \
   MACRO(\
      TR::butest, /* .opcode */ \
      "butest", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      butest, /* zEmulator: mask unsigned byte (UInt8) and set condition codes */ \
   ) \
   MACRO(\
      TR::sutest, /* .opcode */ \
      "sutest", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sutest, /* zEmulator: mask unsigned short (UInt16) and set condition codes */ \
   ) \
   MACRO(\
      TR::bucmp, /* .opcode */ \
      "bucmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bucmp, /* Currently only valid for zEmulator. Based on the ordering of the two children set the return value: */ \
   ) \
   MACRO(\
      TR::bcmp, /* .opcode */ \
      "bcmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bcmp, /* 0 : child1 == child2 */ \
   ) \
   MACRO(\
      TR::sucmp, /* .opcode */ \
      "sucmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sucmp, /* 1 : child1 < child2 */ \
   ) \
   MACRO(\
      TR::scmp, /* .opcode */ \
      "scmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      scmp, /* 2 : child1 > child2 */ \
   ) \
   MACRO(\
      TR::iucmp, /* .opcode */ \
      "iucmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iucmp, \
   ) \
   MACRO(\
      TR::icmp, /* .opcode */ \
      "icmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      icmp, \
   ) \
   MACRO(\
      TR::lucmp, /* .opcode */ \
      "lucmp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, /* .properties2 */ \
      ILProp3::Signum, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lucmp, \
   ) \
   MACRO(\
      TR::ificmpo, /* .opcode */ \
      "ificmpo", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::ificmpno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpo, /* integer compare and branch if overflow */ \
   ) \
   MACRO(\
      TR::ificmpno, /* .opcode */ \
      "ificmpno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::ificmpo, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmpno, /* integer compare and branch if not overflow */ \
   ) \
   MACRO(\
      TR::iflcmpo, /* .opcode */ \
      "iflcmpo", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::iflcmpno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpo, /* long compare and branch if overflow */ \
   ) \
   MACRO(\
      TR::iflcmpno, /* .opcode */ \
      "iflcmpno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::iflcmpo, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmpno, /* long compare and branch if not overflow */ \
   ) \
   MACRO(\
      TR::ificmno, /* .opcode */ \
      "ificmno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::ificmnno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmno, /* integer compare negative and branch if overflow */ \
   ) \
   MACRO(\
      TR::ificmnno, /* .opcode */ \
      "ificmnno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::ificmno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ificmnno, /* integer compare negative and branch if not overflow */ \
   ) \
   MACRO(\
      TR::iflcmno, /* .opcode */ \
      "iflcmno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::iflcmnno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmno, /* long compare negative and branch if overflow */ \
   ) \
   MACRO(\
      TR::iflcmnno, /* .opcode */ \
      "iflcmnno", /* .name */ \
      ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::OverflowCompare, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::iflcmno, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iflcmnno, /* long compare negative and branch if not overflow */ \
   ) \
   MACRO(\
      TR::iuaddc, /* .opcode */ \
      "iuaddc", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectAdd, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuaddc, /* Currently only valid for zEmulator.  Add two unsigned ints with carry */ \
   ) \
   MACRO(\
      TR::luaddc, /* .opcode */ \
      "luaddc", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectAdd, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luaddc, /* Add two longs with carry */ \
   ) \
   MACRO(\
      TR::iusubb, /* .opcode */ \
      "iusubb", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectSub, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iusubb, /* Subtract two ints with borrow */ \
   ) \
   MACRO(\
      TR::lusubb, /* .opcode */ \
      "lusubb", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectSub, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lusubb, /* Subtract two longs with borrow */ \
   ) \
   MACRO(\
      TR::icmpset, /* .opcode */ \
      "icmpset", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int32, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      icmpset, /* icmpset(pointer,c,r): compare *pointer with c, if it matches, replace with r.  Returns 0 on match, 1 otherwise */ \
   ) \
   MACRO(\
      TR::lcmpset, /* .opcode */ \
      "lcmpset", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Address, TR::Int64, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lcmpset, /* the operation is done atomically - return type is int for both [il]cmpset */ \
   ) \
   MACRO(\
      TR::bztestnset, /* .opcode */ \
      "bztestnset", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, ILChildProp::UnspecifiedChildType), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bztestnset, /* bztestnset(pointer,c): atomically sets *pointer to c and returns the original value of *p (represents Test And Set on Z) */ \
   ) \
   MACRO(\
      TR::ibatomicor, /* .opcode */ \
      "ibatomicor", /* .name */ \
      ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ibatomicor, \
   ) \
   MACRO(\
      TR::isatomicor, /* .opcode */ \
      "isatomicor", /* .name */ \
      ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      isatomicor, \
   ) \
   MACRO(\
      TR::iiatomicor, /* .opcode */ \
      "iiatomicor", /* .name */ \
      ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iiatomicor, \
   ) \
   MACRO(\
      TR::ilatomicor, /* .opcode */ \
      "ilatomicor", /* .name */ \
      ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_CHILD(TR::Address, TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ilatomicor, \
   ) \
   MACRO(\
      TR::dexp, /* .opcode */ \
      "dexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dexp, /* double exponent */ \
   ) \
   MACRO(\
      TR::branch, /* .opcode */ \
      "branch", /* .name */ \
      ILProp1::Branch | ILProp1::CompBranchOnly | ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      branch, /* generic branch --> DEPRECATED use TR::case instead */ \
   ) \
   MACRO(\
      TR::igoto, /* .opcode */ \
      "igoto", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(TR::Address), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      igoto, /* indirect goto, branches to the address specified by a child */ \
   ) \
   MACRO(\
      TR::bexp, /* .opcode */ \
      "bexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bexp, /* signed byte exponent  (raise signed byte to power) */ \
   ) \
   MACRO(\
      TR::buexp, /* .opcode */ \
      "buexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int8), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      buexp, /* unsigned byte exponent */ \
   ) \
   MACRO(\
      TR::sexp, /* .opcode */ \
      "sexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sexp, /* short exponent */ \
   ) \
   MACRO(\
      TR::cexp, /* .opcode */ \
      "cexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int16), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      cexp, /* unsigned short exponent */ \
   ) \
   MACRO(\
      TR::iexp, /* .opcode */ \
      "iexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iexp, /* integer exponent */ \
   ) \
   MACRO(\
      TR::iuexp, /* .opcode */ \
      "iuexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iuexp, /* unsigned integer exponent */ \
   ) \
   MACRO(\
      TR::lexp, /* .opcode */ \
      "lexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lexp, /* long exponent */ \
   ) \
   MACRO(\
      TR::luexp, /* .opcode */ \
      "luexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      luexp, /* unsigned long exponent */ \
   ) \
   MACRO(\
      TR::fexp, /* .opcode */ \
      "fexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::SignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fexp, /* float exponent */ \
   ) \
   MACRO(\
      TR::fuexp, /* .opcode */ \
      "fuexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Float, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fuexp, /* float base to unsigned integral exponent */ \
   ) \
   MACRO(\
      TR::duexp, /* .opcode */ \
      "duexp", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::UnsignedExponentiation, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_CHILD(TR::Double, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      duexp, /* double base to unsigned integral exponent */ \
   ) \
   MACRO(\
      TR::ixfrs, /* .opcode */ \
      "ixfrs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ixfrs, /* transfer sign integer */ \
   ) \
   MACRO(\
      TR::lxfrs, /* .opcode */ \
      "lxfrs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lxfrs, /* transfer sign long */ \
   ) \
   MACRO(\
      TR::fxfrs, /* .opcode */ \
      "fxfrs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fxfrs, /* transfer sign float */ \
   ) \
   MACRO(\
      TR::dxfrs, /* .opcode */ \
      "dxfrs", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dxfrs, /* transfer sign double */ \
   ) \
   MACRO(\
      TR::fint, /* .opcode */ \
      "fint", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fint, /* truncate float to int */ \
   ) \
   MACRO(\
      TR::dint, /* .opcode */ \
      "dint", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dint, /* truncate double to int */ \
   ) \
   MACRO(\
      TR::fnint, /* .opcode */ \
      "fnint", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fnint, /* round float to nearest int */ \
   ) \
   MACRO(\
      TR::dnint, /* .opcode */ \
      "dnint", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dnint, /* round double to nearest int */ \
   ) \
   MACRO(\
      TR::fsqrt, /* .opcode */ \
      "fsqrt", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fsqrt, /* square root of float */ \
   ) \
   MACRO(\
      TR::dsqrt, /* .opcode */ \
      "dsqrt", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dsqrt, /* square root of double */ \
   ) \
   MACRO(\
      TR::getstack, /* .opcode */ \
      "getstack", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Address, /* .dataType */ \
      ILTypeProp::Reference, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      getstack, /* returns current value of SP */ \
   ) \
   MACRO(\
      TR::dealloca, /* .opcode */ \
      "dealloca", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dealloca, /* resets value of SP */ \
   ) \
   MACRO(\
      TR::idoz, /* .opcode */ \
      "idoz", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      idoz, /* difference or zero */ \
   ) \
   MACRO(\
      TR::dcos, /* .opcode */ \
      "dcos", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcos, /* cos of double, returning double */ \
   ) \
   MACRO(\
      TR::dsin, /* .opcode */ \
      "dsin", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dsin, /* sin of double, returning double */ \
   ) \
   MACRO(\
      TR::dtan, /* .opcode */ \
      "dtan", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dtan, /* tan of double, returning double */ \
   ) \
   MACRO(\
      TR::dcosh, /* .opcode */ \
      "dcosh", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dcosh, /* cos of double, returning double */ \
   ) \
   MACRO(\
      TR::dsinh, /* .opcode */ \
      "dsinh", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dsinh, /* sin of double, returning double */ \
   ) \
   MACRO(\
      TR::dtanh, /* .opcode */ \
      "dtanh", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dtanh, /* tan of double, returning double */ \
   ) \
   MACRO(\
      TR::dacos, /* .opcode */ \
      "dacos", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dacos, /* arccos of double, returning double */ \
   ) \
   MACRO(\
      TR::dasin, /* .opcode */ \
      "dasin", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dasin, /* arcsin of double, returning double */ \
   ) \
   MACRO(\
      TR::datan, /* .opcode */ \
      "datan", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      datan, /* arctan of double, returning double */ \
   ) \
   MACRO(\
      TR::datan2, /* .opcode */ \
      "datan2", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      datan2, /* arctan2 of double, returning double */ \
   ) \
   MACRO(\
      TR::dlog, /* .opcode */ \
      "dlog", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dlog, /* log of double, returning double */ \
   ) \
   MACRO(\
      TR::dfloor, /* .opcode */ \
      "dfloor", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dfloor, /* floor of double, returning double */ \
   ) \
   MACRO(\
      TR::ffloor, /* .opcode */ \
      "ffloor", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ffloor, /* floor of float, returning float */ \
   ) \
   MACRO(\
      TR::dceil, /* .opcode */ \
      "dceil", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dceil, /* ceil of double, returning double */ \
   ) \
   MACRO(\
      TR::fceil, /* .opcode */ \
      "fceil", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      ONE_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fceil, /* ceil of float, returning float */ \
   ) \
   MACRO(\
      TR::ibranch, /* .opcode */ \
      "ibranch", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ibranch, /* generic indirct branch --> first child is a constant indicating the mask */ \
   ) \
   MACRO(\
      TR::mbranch, /* .opcode */ \
      "mbranch", /* .name */ \
      ILProp1::Branch, /* .properties1 */ \
      ILProp2::JumpWithMultipleTargets, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      mbranch, /* generic branch to multiple potential targets */ \
   ) \
   MACRO(\
      TR::getpm, /* .opcode */ \
      "getpm", /* .name */ \
      0, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      getpm, /* get program mask */ \
   ) \
   MACRO(\
      TR::setpm, /* .opcode */ \
      "setpm", /* .name */ \
      ILProp1::TreeTop, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      setpm, /* set program mask */ \
   ) \
   MACRO(\
      TR::loadAutoOffset, /* .opcode */ \
      "loadAutoOffset", /* .name */ \
      ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      ILChildProp::NoChildren, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      loadAutoOffset, /* loads the offset (from the SP) of an auto */ \
   ) \
   MACRO(\
      TR::imax, /* .opcode */ \
      "imax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      imax, /* max of 2 or more integers */ \
   ) \
   MACRO(\
      TR::iumax, /* .opcode */ \
      "iumax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iumax, /* max of 2 or more unsigned integers */ \
   ) \
   MACRO(\
      TR::lmax, /* .opcode */ \
      "lmax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lmax, /* max of 2 or more longs */ \
   ) \
   MACRO(\
      TR::lumax, /* .opcode */ \
      "lumax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lumax, /* max of 2 or more unsigned longs */ \
   ) \
   MACRO(\
      TR::fmax, /* .opcode */ \
      "fmax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fmax, /* max of 2 or more floats */ \
   ) \
   MACRO(\
      TR::dmax, /* .opcode */ \
      "dmax", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dmax, /* max of 2 or more doubles */ \
   ) \
   MACRO(\
      TR::imin, /* .opcode */ \
      "imin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      imin, /* min of 2 or more integers */ \
   ) \
   MACRO(\
      TR::iumin, /* .opcode */ \
      "iumin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      iumin, /* min of 2 or more unsigned integers */ \
   ) \
   MACRO(\
      TR::lmin, /* .opcode */ \
      "lmin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lmin, /* min of 2 or more longs */ \
   ) \
   MACRO(\
      TR::lumin, /* .opcode */ \
      "lumin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Unsigned, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Int64), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lumin, /* min of 2 or more unsigned longs */ \
   ) \
   MACRO(\
      TR::fmin, /* .opcode */ \
      "fmin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Float, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Float), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      fmin, /* min of 2 or more floats */ \
   ) \
   MACRO(\
      TR::dmin, /* .opcode */ \
      "dmin", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Double, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Floating_Point, /* .typeProperties */ \
      TWO_SAME_CHILD(TR::Double), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      dmin, /* min of 2 or more doubles */ \
   ) \
   MACRO(\
      TR::trt, /* .opcode */ \
      "trt", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      trt, /* translate and test */ \
   ) \
   MACRO(\
      TR::trtSimple, /* .opcode */ \
      "trtSimple", /* .name */ \
      ILProp1::Call | ILProp1::HasSymbolRef, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse | ILProp3::LikeDef, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      trtSimple, /* same as TRT but ignoring the returned source byte address and table entry value */ \
   ) \
   MACRO(\
      TR::ihbit, /* .opcode */ \
      "ihbit", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ihbit, \
   ) \
   MACRO(\
      TR::ilbit, /* .opcode */ \
      "ilbit", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ilbit, \
   ) \
   MACRO(\
      TR::inolz, /* .opcode */ \
      "inolz", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      inolz, \
   ) \
   MACRO(\
      TR::inotz, /* .opcode */ \
      "inotz", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      inotz, \
   ) \
   MACRO(\
      TR::ipopcnt, /* .opcode */ \
      "ipopcnt", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ipopcnt, \
   ) \
   MACRO(\
      TR::lhbit, /* .opcode */ \
      "lhbit", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lhbit, \
   ) \
   MACRO(\
      TR::llbit, /* .opcode */ \
      "llbit", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      llbit, \
   ) \
   MACRO(\
      TR::lnolz, /* .opcode */ \
      "lnolz", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lnolz, \
   ) \
   MACRO(\
      TR::lnotz, /* .opcode */ \
      "lnotz", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lnotz, \
   ) \
   MACRO(\
      TR::lpopcnt, /* .opcode */ \
      "lpopcnt", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::SupportedForPRE, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lpopcnt, \
   ) \
   MACRO(\
      TR::ibyteswap, /* .opcode */ \
      "ibyteswap", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ByteSwap, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      ONE_CHILD(TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ibyteswap, /* swap bytes in an integer */ \
   ) \
   MACRO(\
      TR::bbitpermute, /* .opcode */ \
      "bbitpermute", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int8, /* .dataType */ \
      ILTypeProp::Size_1 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int8, TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      bbitpermute, \
   ) \
   MACRO(\
      TR::sbitpermute, /* .opcode */ \
      "sbitpermute", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int16, /* .dataType */ \
      ILTypeProp::Size_2 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int16, TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      sbitpermute, \
   ) \
   MACRO(\
      TR::ibitpermute, /* .opcode */ \
      "ibitpermute", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int32, /* .dataType */ \
      ILTypeProp::Size_4 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int32, TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      ibitpermute, \
   ) \
   MACRO(\
      TR::lbitpermute, /* .opcode */ \
      "lbitpermute", /* .name */ \
      0, /* .properties1 */ \
      ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, /* .properties2 */ \
      ILProp3::LikeUse, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::Int64, /* .dataType */ \
      ILTypeProp::Size_8 | ILTypeProp::Integer, /* .typeProperties */ \
      THREE_CHILD(TR::Int64, TR::Address, TR::Int32), /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      lbitpermute, \
   ) \
   MACRO(\
      TR::Prefetch, /* .opcode */ \
      "Prefetch", /* .name */ \
      ILProp1::TreeTop | ILProp1::HasSymbolRef, /* .properties1 */ \
      0, /* .properties2 */ \
      0, /* .properties3 */ \
      0, /* .properties4 */ \
      TR::NoType, /* .dataType */ \
      0, /* .typeProperties */ \
      ILChildProp::Unspecified, /* .childProperties */ \
      TR::BadILOp, /* .swapChildrenOpCode */ \
      TR::BadILOp, /* .reverseBranchOpCode */ \
      TR::BadILOp, /* .booleanCompareOpCode */ \
      TR::BadILOp, /* .ifCompareOpCode */ \
      Prefetch, /* Prefetch */ \
   ) 
#endif
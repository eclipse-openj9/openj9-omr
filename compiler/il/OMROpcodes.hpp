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
      /* .opcode               = */ TR::BadILOp, \
      /* .name                 = */ "BadILOp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ BadILOp = 0, /* illegal op hopefully help with uninitialized nodes */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aconst, \
      /* .name                 = */ "aconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aconst, /* load address constant (zero value means NULL) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainAConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iconst, \
      /* .name                 = */ "iconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iconst, /* load integer constant (32-bit signed 2's complement) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainIntConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lconst, \
      /* .name                 = */ "lconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lconst, /* load long integer constant (64-bit signed 2's complement) */ \
      /* .simplifierHandler    = */ lconstSimplifier, \
      /* .vpHandler            = */ constrainLongConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fconst, \
      /* .name                 = */ "fconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fconst, /* load float constant (32-bit ieee fp) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainFloatConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dconst, \
      /* .name                 = */ "dconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dconst, /* load double constant (64-bit ieee fp) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bconst, \
      /* .name                 = */ "bconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bconst, /* load byte integer constant (8-bit signed 2's complement) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainByteConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sconst, \
      /* .name                 = */ "sconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sconst, /* load short integer constant (16-bit signed 2's complement) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainShortConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iload, \
      /* .name                 = */ "iload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iload, /* load integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fload, \
      /* .name                 = */ "fload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fload, /* load float */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainFload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dload, \
      /* .name                 = */ "dload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dload, /* load double */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainDload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aload, \
      /* .name                 = */ "aload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aload, /* load address */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainAload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bload, \
      /* .name                 = */ "bload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bload, /* load byte */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sload, \
      /* .name                 = */ "sload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sload, /* load short integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainShortLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lload, \
      /* .name                 = */ "lload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lload, /* load long integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::irdbar, \
      /* .name                 = */ "irdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ irdbar, /* read barrier for load integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::frdbar, \
      /* .name                 = */ "frdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ frdbar, /* read barrier for load float */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainFload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::drdbar, \
      /* .name                 = */ "drdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ drdbar, /* read barrier for load double */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainDload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ardbar, \
      /* .name                 = */ "ardbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ardbar, /* read barrier for load address */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainAload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::brdbar, \
      /* .name                 = */ "brdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ brdbar, /* read barrier for load byte */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::srdbar, \
      /* .name                 = */ "srdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ srdbar, /* load short integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainShortLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lrdbar, \
      /* .name                 = */ "lrdbar", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lrdbar, /* load long integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iloadi, \
      /* .name                 = */ "iloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iloadi, /* load indirect integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIiload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::floadi, \
      /* .name                 = */ "floadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ floadi, /* load indirect float */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainFload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dloadi, \
      /* .name                 = */ "dloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dloadi, /* load indirect double */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainDload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aloadi, \
      /* .name                 = */ "aloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aloadi, /* load indirect address */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIaload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bloadi, \
      /* .name                 = */ "bloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bloadi, /* load indirect byte */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sloadi, \
      /* .name                 = */ "sloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sloadi, /* load indirect short integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainShortLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lloadi, \
      /* .name                 = */ "lloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lloadi, /* load indirect long integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::irdbari, \
      /* .name                 = */ "irdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ irdbari, /* read barrier for load indirect integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIiload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::frdbari, \
      /* .name                 = */ "frdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ frdbari, /* read barrier for load indirect float */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainFload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::drdbari, \
      /* .name                 = */ "drdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ drdbari, /* read barrier for load indirect double */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainDload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ardbari, \
      /* .name                 = */ "ardbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ardbari, /* read barrier for load indirect address */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIaload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::brdbari, \
      /* .name                 = */ "brdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ brdbari, /* read barrier for load indirect byte */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::srdbari, \
      /* .name                 = */ "srdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ srdbari, /* read barrier for load indirect short integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainShortLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lrdbari, \
      /* .name                 = */ "lrdbari", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::ReadBarrierLoad, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lrdbari, /* read barrier for load indirect long integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::istore, \
      /* .name                 = */ "istore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ istore, /* store integer */ \
      /* .simplifierHandler    = */ directStoreSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lstore, \
      /* .name                 = */ "lstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lstore, /* store long integer */ \
      /* .simplifierHandler    = */ lstoreSimplifier, \
      /* .vpHandler            = */ constrainLongStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fstore, \
      /* .name                 = */ "fstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fstore, /* store float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dstore, \
      /* .name                 = */ "dstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dstore, /* store double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::astore, \
      /* .name                 = */ "astore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ astore, /* store address */ \
      /* .simplifierHandler    = */ astoreSimplifier, \
      /* .vpHandler            = */ constrainAstore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bstore, \
      /* .name                 = */ "bstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bstore, /* store byte */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sstore, \
      /* .name                 = */ "sstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sstore, /* store short integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iwrtbar, \
      /* .name                 = */ "iwrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int32, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iwrtbar, /* write barrier for store direct integer */ \
      /* .simplifierHandler    = */ directStoreSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lwrtbar, \
      /* .name                 = */ "lwrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int64, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lwrtbar, /* write barrier for store direct long integer */ \
      /* .simplifierHandler    = */ lstoreSimplifier, \
      /* .vpHandler            = */ constrainLongStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fwrtbar, \
      /* .name                 = */ "fwrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Float, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fwrtbar, /* write barrier for store direct float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dwrtbar, \
      /* .name                 = */ "dwrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Double, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dwrtbar, /* write barrier for store direct double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::awrtbar, \
      /* .name                 = */ "awrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ awrtbar, /* write barrier for store direct address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainWrtBar, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bwrtbar, \
      /* .name                 = */ "bwrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int8, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bwrtbar, /* write barrier for store direct byte */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::swrtbar, \
      /* .name                 = */ "swrtbar", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int16, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ swrtbar, /* write barrier for store direct short integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lstorei, \
      /* .name                 = */ "lstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lstorei, /* store indirect long integer           (child1 a, child2 l) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fstorei, \
      /* .name                 = */ "fstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fstorei, /* store indirect float                  (child1 a, child2 f) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dstorei, \
      /* .name                 = */ "dstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dstorei, /* store indirect double                 (child1 a, child2 d) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::astorei, \
      /* .name                 = */ "astorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ astorei, /* store indirect address                (child1 a dest, child2 a value) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainAstore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bstorei, \
      /* .name                 = */ "bstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bstorei, /* store indirect byte                   (child1 a, child2 b) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sstorei, \
      /* .name                 = */ "sstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sstorei, /* store indirect short integer          (child1 a, child2 s) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::istorei, \
      /* .name                 = */ "istorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ istorei, /* store indirect integer                (child1 a, child2 i) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lwrtbari, \
      /* .name                 = */ "lwrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int64, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lwrtbari, /* write barrier for store indirect long integer */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fwrtbari, \
      /* .name                 = */ "fwrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Float, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fwrtbari, /* write barrier for store indirect float */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dwrtbari, \
      /* .name                 = */ "dwrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Double, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dwrtbari, /* write barrier for store indirect double */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::awrtbari, \
      /* .name                 = */ "awrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Address, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ awrtbari, /* write barrier for store indirect address */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainWrtBar, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bwrtbari, \
      /* .name                 = */ "bwrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int8, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bwrtbari, /* write barrier for store indirect byte */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::swrtbari, \
      /* .name                 = */ "swrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int16, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ swrtbari, /* write barrier for store indirect short integer */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iwrtbari, \
      /* .name                 = */ "iwrtbari", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::WriteBarrierStore| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int32, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iwrtbari, /* write barrier for store indirect integer */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::Goto, \
      /* .name                 = */ "goto", \
      /* .properties1          = */ ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ Goto, /* goto label address */ \
      /* .simplifierHandler    = */ gotoSimplifier, \
      /* .vpHandler            = */ constrainGoto, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ireturn, \
      /* .name                 = */ "ireturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ireturn, /* return an integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lreturn, \
      /* .name                 = */ "lreturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lreturn, /* return a long integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::freturn, \
      /* .name                 = */ "freturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ freturn, /* return a float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dreturn, \
      /* .name                 = */ "dreturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dreturn, /* return a double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::areturn, \
      /* .name                 = */ "areturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ areturn, /* return an address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::Return, \
      /* .name                 = */ "return", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ Return, /* void return */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::asynccheck, \
      /* .name                 = */ "asynccheck", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ asynccheck, /* GC point */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::athrow, \
      /* .name                 = */ "athrow", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MustBeLowered | ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ athrow, /* throw an exception */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainThrow, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icall, \
      /* .name                 = */ "icall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ icall, /* direct call returning integer */ \
      /* .simplifierHandler    = */ ifdCallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcall, \
      /* .name                 = */ "lcall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lcall, /* direct call returning long integer */ \
      /* .simplifierHandler    = */ lcallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcall, \
      /* .name                 = */ "fcall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fcall, /* direct call returning float */ \
      /* .simplifierHandler    = */ ifdCallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcall, \
      /* .name                 = */ "dcall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcall, /* direct call returning double */ \
      /* .simplifierHandler    = */ ifdCallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acall, \
      /* .name                 = */ "acall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ acall, /* direct call returning reference */ \
      /* .simplifierHandler    = */ acallSimplifier, \
      /* .vpHandler            = */ constrainAcall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::call, \
      /* .name                 = */ "call", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ call, /* direct call returning void */ \
      /* .simplifierHandler    = */ vcallSimplifier, \
      /* .vpHandler            = */ constrainVcall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iadd, \
      /* .name                 = */ "iadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iadd, /* add 2 integers */ \
      /* .simplifierHandler    = */ iaddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ladd, \
      /* .name                 = */ "ladd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::ladd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ladd, /* add 2 long integers */ \
      /* .simplifierHandler    = */ laddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fadd, \
      /* .name                 = */ "fadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fadd, /* add 2 floats */ \
      /* .simplifierHandler    = */ faddSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dadd, \
      /* .name                 = */ "dadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dadd, /* add 2 doubles */ \
      /* .simplifierHandler    = */ daddSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::badd, \
      /* .name                 = */ "badd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::badd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ badd, /* add 2 bytes */ \
      /* .simplifierHandler    = */ baddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sadd, \
      /* .name                 = */ "sadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sadd, /* add 2 short integers */ \
      /* .simplifierHandler    = */ saddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::isub, \
      /* .name                 = */ "isub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ isub, /* subtract 2 integers                (child1 - child2) */ \
      /* .simplifierHandler    = */ isubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lsub, \
      /* .name                 = */ "lsub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lsub, /* subtract 2 long integers           (child1 - child2) */ \
      /* .simplifierHandler    = */ lsubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fsub, \
      /* .name                 = */ "fsub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fsub, /* subtract 2 floats                  (child1 - child2) */ \
      /* .simplifierHandler    = */ fsubSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dsub, \
      /* .name                 = */ "dsub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dsub, /* subtract 2 doubles                 (child1 - child2) */ \
      /* .simplifierHandler    = */ dsubSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bsub, \
      /* .name                 = */ "bsub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bsub, /* subtract 2 bytes                   (child1 - child2) */ \
      /* .simplifierHandler    = */ bsubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ssub, \
      /* .name                 = */ "ssub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ssub, /* subtract 2 short integers          (child1 - child2) */ \
      /* .simplifierHandler    = */ ssubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::asub, \
      /* .name                 = */ "asub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ asub, /* subtract 2 addresses (child1 - child2) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::imul, \
      /* .name                 = */ "imul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::imul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ imul, /* multiply 2 integers */ \
      /* .simplifierHandler    = */ imulSimplifier, \
      /* .vpHandler            = */ constrainImul, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lmul, \
      /* .name                 = */ "lmul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lmul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lmul, /* multiply 2 signed or unsigned long integers */ \
      /* .simplifierHandler    = */ lmulSimplifier, \
      /* .vpHandler            = */ constrainLmul, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fmul, \
      /* .name                 = */ "fmul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fmul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fmul, /* multiply 2 floats */ \
      /* .simplifierHandler    = */ fmulSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dmul, \
      /* .name                 = */ "dmul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dmul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dmul, /* multiply 2 doubles */ \
      /* .simplifierHandler    = */ dmulSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bmul, \
      /* .name                 = */ "bmul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bmul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bmul, /* multiply 2 bytes */ \
      /* .simplifierHandler    = */ bmulSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::smul, \
      /* .name                 = */ "smul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::smul, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ smul, /* multiply 2 short integers */ \
      /* .simplifierHandler    = */ smulSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::idiv, \
      /* .name                 = */ "idiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ idiv, /* divide 2 integers                (child1 / child2) */ \
      /* .simplifierHandler    = */ idivSimplifier, \
      /* .vpHandler            = */ constrainIdiv, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ldiv, \
      /* .name                 = */ "ldiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ldiv, /* divide 2 long integers           (child1 / child2) */ \
      /* .simplifierHandler    = */ ldivSimplifier, \
      /* .vpHandler            = */ constrainLdiv, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fdiv, \
      /* .name                 = */ "fdiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fdiv, /* divide 2 floats                  (child1 / child2) */ \
      /* .simplifierHandler    = */ fdivSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ddiv, \
      /* .name                 = */ "ddiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ddiv, /* divide 2 doubles                 (child1 / child2) */ \
      /* .simplifierHandler    = */ ddivSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bdiv, \
      /* .name                 = */ "bdiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bdiv, /* divide 2 bytes                   (child1 / child2) */ \
      /* .simplifierHandler    = */ bdivSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sdiv, \
      /* .name                 = */ "sdiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sdiv, /* divide 2 short integers          (child1 / child2) */ \
      /* .simplifierHandler    = */ sdivSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iudiv, \
      /* .name                 = */ "iudiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iudiv, /* divide 2 unsigned integers       (child1 / child2) */ \
      /* .simplifierHandler    = */ idivSimplifier, \
      /* .vpHandler            = */ constrainIdiv, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ludiv, \
      /* .name                 = */ "ludiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ludiv, /* divide 2 unsigned long integers  (child1 / child2) */ \
      /* .simplifierHandler    = */ ldivSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::irem, \
      /* .name                 = */ "irem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ irem, /* remainder of 2 integers                (child1 % child2) */ \
      /* .simplifierHandler    = */ iremSimplifier, \
      /* .vpHandler            = */ constrainIrem, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lrem, \
      /* .name                 = */ "lrem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lrem, /* remainder of 2 long integers           (child1 % child2) */ \
      /* .simplifierHandler    = */ lremSimplifier, \
      /* .vpHandler            = */ constrainLrem, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::frem, \
      /* .name                 = */ "frem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ frem, /* remainder of 2 floats                  (child1 % child2) */ \
      /* .simplifierHandler    = */ fremSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::drem, \
      /* .name                 = */ "drem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ drem, /* remainder of 2 doubles                 (child1 % child2) */ \
      /* .simplifierHandler    = */ dremSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::brem, \
      /* .name                 = */ "brem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ brem, /* remainder of 2 bytes                   (child1 % child2) */ \
      /* .simplifierHandler    = */ bremSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::srem, \
      /* .name                 = */ "srem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ srem, /* remainder of 2 short integers          (child1 % child2) */ \
      /* .simplifierHandler    = */ sremSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iurem, \
      /* .name                 = */ "iurem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iurem, /* remainder of 2 unsigned integers       (child1 % child2) */ \
      /* .simplifierHandler    = */ iremSimplifier, \
      /* .vpHandler            = */ constrainIrem, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ineg, \
      /* .name                 = */ "ineg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ineg, /* negate an integer */ \
      /* .simplifierHandler    = */ inegSimplifier, \
      /* .vpHandler            = */ constrainIneg, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lneg, \
      /* .name                 = */ "lneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lneg, /* negate a long integer */ \
      /* .simplifierHandler    = */ lnegSimplifier, \
      /* .vpHandler            = */ constrainLneg, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fneg, \
      /* .name                 = */ "fneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fneg, /* negate a float */ \
      /* .simplifierHandler    = */ fnegSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dneg, \
      /* .name                 = */ "dneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dneg, /* negate a double */ \
      /* .simplifierHandler    = */ dnegSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bneg, \
      /* .name                 = */ "bneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bneg, /* negate a bytes */ \
      /* .simplifierHandler    = */ bnegSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sneg, \
      /* .name                 = */ "sneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sneg, /* negate a short integer */ \
      /* .simplifierHandler    = */ snegSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iabs, \
      /* .name                 = */ "iabs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::Abs, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iabs, /* absolute value of integer */ \
      /* .simplifierHandler    = */ ilfdabsSimplifier, \
      /* .vpHandler            = */ constrainIabs, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::labs, \
      /* .name                 = */ "labs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::Abs, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ labs, /* absolute value of long */ \
      /* .simplifierHandler    = */ ilfdabsSimplifier, \
      /* .vpHandler            = */ constrainLabs, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fabs, \
      /* .name                 = */ "fabs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::Abs, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fabs, /* absolute value of float */ \
      /* .simplifierHandler    = */ ilfdabsSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dabs, \
      /* .name                 = */ "dabs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::Abs, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dabs, /* absolute value of double */ \
      /* .simplifierHandler    = */ ilfdabsSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ishl, \
      /* .name                 = */ "ishl", \
      /* .properties1          = */ ILProp1::LeftShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ishl, /* shift integer left                (child1 << child2) */ \
      /* .simplifierHandler    = */ ishlSimplifier, \
      /* .vpHandler            = */ constrainIshl, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lshl, \
      /* .name                 = */ "lshl", \
      /* .properties1          = */ ILProp1::LeftShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int64, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lshl, /* shift long integer left           (child1 << child2) */ \
      /* .simplifierHandler    = */ lshlSimplifier, \
      /* .vpHandler            = */ constrainLshl, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bshl, \
      /* .name                 = */ "bshl", \
      /* .properties1          = */ ILProp1::LeftShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int8, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bshl, /* shift byte left                   (child1 << child2) */ \
      /* .simplifierHandler    = */ bshlSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sshl, \
      /* .name                 = */ "sshl", \
      /* .properties1          = */ ILProp1::LeftShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int16, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sshl, /* shift short integer left          (child1 << child2) */ \
      /* .simplifierHandler    = */ sshlSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ishr, \
      /* .name                 = */ "ishr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ishr, /* shift integer right arithmetically               (child1 >> child2) */ \
      /* .simplifierHandler    = */ ishrSimplifier, \
      /* .vpHandler            = */ constrainIshr, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lshr, \
      /* .name                 = */ "lshr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int64, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lshr, /* shift long integer right arithmetically          (child1 >> child2) */ \
      /* .simplifierHandler    = */ lshrSimplifier, \
      /* .vpHandler            = */ constrainLshr, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bshr, \
      /* .name                 = */ "bshr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int8, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bshr, /* shift byte right arithmetically                  (child1 >> child2) */ \
      /* .simplifierHandler    = */ bshrSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sshr, \
      /* .name                 = */ "sshr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int16, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sshr, /* shift short integer arithmetically               (child1 >> child2) */ \
      /* .simplifierHandler    = */ sshrSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iushr, \
      /* .name                 = */ "iushr", \
      /* .properties1          = */ ILProp1::RightShift | ILProp1::ShiftLogical, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iushr, /* shift integer right logically                   (child1 >> child2) */ \
      /* .simplifierHandler    = */ iushrSimplifier, \
      /* .vpHandler            = */ constrainIushr, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lushr, \
      /* .name                 = */ "lushr", \
      /* .properties1          = */ ILProp1::RightShift | ILProp1::ShiftLogical, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int64, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lushr, /* shift long integer right logically              (child1 >> child2) */ \
      /* .simplifierHandler    = */ lushrSimplifier, \
      /* .vpHandler            = */ constrainLushr, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bushr, \
      /* .name                 = */ "bushr", \
      /* .properties1          = */ ILProp1::RightShift | ILProp1::ShiftLogical, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int8, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bushr, /* shift byte right logically                      (child1 >> child2) */ \
      /* .simplifierHandler    = */ bushrSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sushr, \
      /* .name                 = */ "sushr", \
      /* .properties1          = */ ILProp1::RightShift | ILProp1::ShiftLogical, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int16, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sushr, /* shift short integer right logically             (child1 >> child2) */ \
      /* .simplifierHandler    = */ sushrSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::irol, \
      /* .name                 = */ "irol", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::LeftRotate, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ irol, /* rotate integer left */ \
      /* .simplifierHandler    = */ irolSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lrol, \
      /* .name                 = */ "lrol", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::LeftRotate, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Int64, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lrol, /* rotate long integer left */ \
      /* .simplifierHandler    = */ lrolSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iand, \
      /* .name                 = */ "iand", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::And, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iand, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iand, /* boolean and of 2 integers */ \
      /* .simplifierHandler    = */ iandSimplifier, \
      /* .vpHandler            = */ constrainIand, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::land, \
      /* .name                 = */ "land", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::And, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::land, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ land, /* boolean and of 2 long integers */ \
      /* .simplifierHandler    = */ landSimplifier, \
      /* .vpHandler            = */ constrainLand, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::band, \
      /* .name                 = */ "band", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::And, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::band, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ band, /* boolean and of 2 bytes */ \
      /* .simplifierHandler    = */ bandSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sand, \
      /* .name                 = */ "sand", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::And, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sand, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sand, /* boolean and of 2 short integers */ \
      /* .simplifierHandler    = */ sandSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ior, \
      /* .name                 = */ "ior", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ior, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ior, /* boolean or of 2 integers */ \
      /* .simplifierHandler    = */ iorSimplifier, \
      /* .vpHandler            = */ constrainIor, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lor, \
      /* .name                 = */ "lor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lor, /* boolean or of 2 long integers */ \
      /* .simplifierHandler    = */ lorSimplifier, \
      /* .vpHandler            = */ constrainLor, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bor, \
      /* .name                 = */ "bor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bor, /* boolean or of 2 bytes */ \
      /* .simplifierHandler    = */ borSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sor, \
      /* .name                 = */ "sor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sor, /* boolean or of 2 short integers */ \
      /* .simplifierHandler    = */ sorSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ixor, \
      /* .name                 = */ "ixor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ixor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ixor, /* boolean xor of 2 integers */ \
      /* .simplifierHandler    = */ ixorSimplifier, \
      /* .vpHandler            = */ constrainIxor, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lxor, \
      /* .name                 = */ "lxor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lxor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lxor, /* boolean xor of 2 long integers */ \
      /* .simplifierHandler    = */ lxorSimplifier, \
      /* .vpHandler            = */ constrainLxor, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bxor, \
      /* .name                 = */ "bxor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bxor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bxor, /* boolean xor of 2 bytes */ \
      /* .simplifierHandler    = */ bxorSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sxor, \
      /* .name                 = */ "sxor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sxor, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sxor, /* boolean xor of 2 short integers */ \
      /* .simplifierHandler    = */ sxorSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2l, \
      /* .name                 = */ "i2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2l, /* convert integer to long integer with sign extension */ \
      /* .simplifierHandler    = */ i2lSimplifier, \
      /* .vpHandler            = */ constrainI2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2f, \
      /* .name                 = */ "i2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2f, /* convert integer to float */ \
      /* .simplifierHandler    = */ i2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2d, \
      /* .name                 = */ "i2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2d, /* convert integer to double */ \
      /* .simplifierHandler    = */ i2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2b, \
      /* .name                 = */ "i2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2b, /* convert integer to byte */ \
      /* .simplifierHandler    = */ i2bSimplifier, \
      /* .vpHandler            = */ constrainNarrowToByte, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2s, \
      /* .name                 = */ "i2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2s, /* convert integer to short integer */ \
      /* .simplifierHandler    = */ i2sSimplifier, \
      /* .vpHandler            = */ constrainNarrowToShort, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::i2a, \
      /* .name                 = */ "i2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ i2a, /* convert integer to address */ \
      /* .simplifierHandler    = */ i2aSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iu2l, \
      /* .name                 = */ "iu2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iu2l, /* convert unsigned integer to long integer with zero extension */ \
      /* .simplifierHandler    = */ iu2lSimplifier, \
      /* .vpHandler            = */ constrainIu2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iu2f, \
      /* .name                 = */ "iu2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iu2f, /* convert unsigned integer to float */ \
      /* .simplifierHandler    = */ iu2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iu2d, \
      /* .name                 = */ "iu2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iu2d, /* convert unsigned integer to double */ \
      /* .simplifierHandler    = */ iu2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iu2a, \
      /* .name                 = */ "iu2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iu2a, /* convert unsigned integer to address */ \
      /* .simplifierHandler    = */ i2aSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2i, \
      /* .name                 = */ "l2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2i, /* convert long integer to integer */ \
      /* .simplifierHandler    = */ l2iSimplifier, \
      /* .vpHandler            = */ constrainNarrowToInt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2f, \
      /* .name                 = */ "l2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2f, /* convert long integer to float */ \
      /* .simplifierHandler    = */ l2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2d, \
      /* .name                 = */ "l2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2d, /* convert long integer to double */ \
      /* .simplifierHandler    = */ l2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2b, \
      /* .name                 = */ "l2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2b, /* convert long integer to byte */ \
      /* .simplifierHandler    = */ l2bSimplifier, \
      /* .vpHandler            = */ constrainNarrowToByte, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2s, \
      /* .name                 = */ "l2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2s, /* convert long integer to short integer */ \
      /* .simplifierHandler    = */ l2sSimplifier, \
      /* .vpHandler            = */ constrainNarrowToShort, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::l2a, \
      /* .name                 = */ "l2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ l2a, /* convert long integer to address */ \
      /* .simplifierHandler    = */ l2aSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lu2f, \
      /* .name                 = */ "lu2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lu2f, /* convert unsigned long integer to float */ \
      /* .simplifierHandler    = */ lu2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lu2d, \
      /* .name                 = */ "lu2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lu2d, /* convert unsigned long integer to double */ \
      /* .simplifierHandler    = */ lu2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lu2a, \
      /* .name                 = */ "lu2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lu2a, /* convert unsigned long integer to address */ \
      /* .simplifierHandler    = */ l2aSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2i, \
      /* .name                 = */ "f2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2i, /* convert float to integer */ \
      /* .simplifierHandler    = */ f2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2l, \
      /* .name                 = */ "f2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2l, /* convert float to long integer */ \
      /* .simplifierHandler    = */ f2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2d, \
      /* .name                 = */ "f2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2d, /* convert float to double */ \
      /* .simplifierHandler    = */ f2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2b, \
      /* .name                 = */ "f2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2b, /* convert float to byte */ \
      /* .simplifierHandler    = */ f2bSimplifier, \
      /* .vpHandler            = */ constrainNarrowToByte, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2s, \
      /* .name                 = */ "f2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2s, /* convert float to short integer */ \
      /* .simplifierHandler    = */ f2sSimplifier, \
      /* .vpHandler            = */ constrainNarrowToShort, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2i, \
      /* .name                 = */ "d2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2i, /* convert double to integer */ \
      /* .simplifierHandler    = */ d2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2l, \
      /* .name                 = */ "d2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2l, /* convert double to long integer */ \
      /* .simplifierHandler    = */ d2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2f, \
      /* .name                 = */ "d2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2f, /* convert double to float */ \
      /* .simplifierHandler    = */ d2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2b, \
      /* .name                 = */ "d2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2b, /* convert double to byte */ \
      /* .simplifierHandler    = */ d2bSimplifier, \
      /* .vpHandler            = */ constrainNarrowToByte, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2s, \
      /* .name                 = */ "d2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2s, /* convert double to short integer */ \
      /* .simplifierHandler    = */ d2sSimplifier, \
      /* .vpHandler            = */ constrainNarrowToShort, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2i, \
      /* .name                 = */ "b2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2i, /* convert byte to integer with sign extension */ \
      /* .simplifierHandler    = */ b2iSimplifier, \
      /* .vpHandler            = */ constrainB2i, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2l, \
      /* .name                 = */ "b2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2l, /* convert byte to long integer with sign extension */ \
      /* .simplifierHandler    = */ b2lSimplifier, \
      /* .vpHandler            = */ constrainB2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2f, \
      /* .name                 = */ "b2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2f, /* convert byte to float */ \
      /* .simplifierHandler    = */ b2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2d, \
      /* .name                 = */ "b2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2d, /* convert byte to double */ \
      /* .simplifierHandler    = */ b2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2s, \
      /* .name                 = */ "b2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2s, /* convert byte to short integer with sign extension */ \
      /* .simplifierHandler    = */ b2sSimplifier, \
      /* .vpHandler            = */ constrainB2s, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::b2a, \
      /* .name                 = */ "b2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ b2a, /* convert byte to address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2i, \
      /* .name                 = */ "bu2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2i, /* convert byte to integer with zero extension */ \
      /* .simplifierHandler    = */ bu2iSimplifier, \
      /* .vpHandler            = */ constrainBu2i, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2l, \
      /* .name                 = */ "bu2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2l, /* convert byte to long integer with zero extension */ \
      /* .simplifierHandler    = */ bu2lSimplifier, \
      /* .vpHandler            = */ constrainBu2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2f, \
      /* .name                 = */ "bu2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2f, /* convert unsigned byte to float */ \
      /* .simplifierHandler    = */ bu2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2d, \
      /* .name                 = */ "bu2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2d, /* convert unsigned byte to double */ \
      /* .simplifierHandler    = */ bu2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2s, \
      /* .name                 = */ "bu2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2s, /* convert byte to short integer with zero extension */ \
      /* .simplifierHandler    = */ bu2sSimplifier, \
      /* .vpHandler            = */ constrainBu2s, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bu2a, \
      /* .name                 = */ "bu2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bu2a, /* convert unsigned byte to unsigned address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2i, \
      /* .name                 = */ "s2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2i, /* convert short integer to integer with sign extension */ \
      /* .simplifierHandler    = */ s2iSimplifier, \
      /* .vpHandler            = */ constrainS2i, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2l, \
      /* .name                 = */ "s2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SignExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2l, /* convert short integer to long integer with sign extension */ \
      /* .simplifierHandler    = */ s2lSimplifier, \
      /* .vpHandler            = */ constrainS2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2f, \
      /* .name                 = */ "s2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2f, /* convert short integer to float */ \
      /* .simplifierHandler    = */ s2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2d, \
      /* .name                 = */ "s2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2d, /* convert short integer to double */ \
      /* .simplifierHandler    = */ s2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2b, \
      /* .name                 = */ "s2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2b, /* convert short integer to byte */ \
      /* .simplifierHandler    = */ s2bSimplifier, \
      /* .vpHandler            = */ constrainNarrowToByte, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::s2a, \
      /* .name                 = */ "s2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ s2a, /* convert short integer to address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::su2i, \
      /* .name                 = */ "su2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ su2i, /* zero extend short to int */ \
      /* .simplifierHandler    = */ su2iSimplifier, \
      /* .vpHandler            = */ constrainSu2i, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::su2l, \
      /* .name                 = */ "su2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ZeroExtension, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ su2l, /* zero extend char to long */ \
      /* .simplifierHandler    = */ su2lSimplifier, \
      /* .vpHandler            = */ constrainSu2l, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::su2f, \
      /* .name                 = */ "su2f", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ su2f, /* convert char to float */ \
      /* .simplifierHandler    = */ su2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::su2d, \
      /* .name                 = */ "su2d", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ su2d, /* convert char to double */ \
      /* .simplifierHandler    = */ su2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::su2a, \
      /* .name                 = */ "su2a", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ su2a, /* convert char to address */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::a2i, \
      /* .name                 = */ "a2i", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ a2i, /* convert address to integer */ \
      /* .simplifierHandler    = */ a2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::a2l, \
      /* .name                 = */ "a2l", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ a2l, /* convert address to long integer */ \
      /* .simplifierHandler    = */ a2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::a2b, \
      /* .name                 = */ "a2b", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ a2b, /* convert address to byte */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::a2s, \
      /* .name                 = */ "a2s", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ a2s, /* convert address to short */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmpeq, \
      /* .name                 = */ "icmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmpeq, \
      /* .reverseBranchOpCode  = */ TR::icmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmpeq, \
      /* .description          = */ icmpeq, /* integer compare if equal */ \
      /* .simplifierHandler    = */ icmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmpne, \
      /* .name                 = */ "icmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmpne, \
      /* .reverseBranchOpCode  = */ TR::icmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmpne, \
      /* .description          = */ icmpne, /* integer compare if not equal */ \
      /* .simplifierHandler    = */ icmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmplt, \
      /* .name                 = */ "icmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmpgt, \
      /* .reverseBranchOpCode  = */ TR::icmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmplt, \
      /* .description          = */ icmplt, /* integer compare if less than */ \
      /* .simplifierHandler    = */ icmpltSimplifier, \
      /* .vpHandler            = */ constrainCmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmpge, \
      /* .name                 = */ "icmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmple, \
      /* .reverseBranchOpCode  = */ TR::icmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmpge, \
      /* .description          = */ icmpge, /* integer compare if greater than or equal */ \
      /* .simplifierHandler    = */ icmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmpgt, \
      /* .name                 = */ "icmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmplt, \
      /* .reverseBranchOpCode  = */ TR::icmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmpgt, \
      /* .description          = */ icmpgt, /* integer compare if greater than */ \
      /* .simplifierHandler    = */ icmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmple, \
      /* .name                 = */ "icmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::icmpge, \
      /* .reverseBranchOpCode  = */ TR::icmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmple, \
      /* .description          = */ icmple, /* integer compare if less than or equal */ \
      /* .simplifierHandler    = */ icmpleSimplifier, \
      /* .vpHandler            = */ constrainCmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmpeq, \
      /* .name                 = */ "iucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmpeq, \
      /* .reverseBranchOpCode  = */ TR::iucmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmpeq, \
      /* .description          = */ iucmpeq, /* unsigned integer compare if equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmpne, \
      /* .name                 = */ "iucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmpne, \
      /* .reverseBranchOpCode  = */ TR::iucmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmpne, \
      /* .description          = */ iucmpne, /* unsigned integer compare if not equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmplt, \
      /* .name                 = */ "iucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmpgt, \
      /* .reverseBranchOpCode  = */ TR::iucmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmplt, \
      /* .description          = */ iucmplt, /* unsigned integer compare if less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmpge, \
      /* .name                 = */ "iucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmple, \
      /* .reverseBranchOpCode  = */ TR::iucmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmpge, \
      /* .description          = */ iucmpge, /* unsigned integer compare if greater than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmpgt, \
      /* .name                 = */ "iucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmplt, \
      /* .reverseBranchOpCode  = */ TR::iucmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmpgt, \
      /* .description          = */ iucmpgt, /* unsigned integer compare if greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmple, \
      /* .name                 = */ "iucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iucmpge, \
      /* .reverseBranchOpCode  = */ TR::iucmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifiucmple, \
      /* .description          = */ iucmple, /* unsigned integer compare if less than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmpeq, \
      /* .name                 = */ "lcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmpeq, \
      /* .reverseBranchOpCode  = */ TR::lcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmpeq, \
      /* .description          = */ lcmpeq, /* long compare if equal */ \
      /* .simplifierHandler    = */ lcmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmpne, \
      /* .name                 = */ "lcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmpne, \
      /* .reverseBranchOpCode  = */ TR::lcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmpne, \
      /* .description          = */ lcmpne, /* long compare if not equal */ \
      /* .simplifierHandler    = */ lcmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmplt, \
      /* .name                 = */ "lcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmpgt, \
      /* .reverseBranchOpCode  = */ TR::lcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmplt, \
      /* .description          = */ lcmplt, /* long compare if less than */ \
      /* .simplifierHandler    = */ lcmpltSimplifier, \
      /* .vpHandler            = */ constrainCmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmpge, \
      /* .name                 = */ "lcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmple, \
      /* .reverseBranchOpCode  = */ TR::lcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmpge, \
      /* .description          = */ lcmpge, /* long compare if greater than or equal */ \
      /* .simplifierHandler    = */ lcmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmpgt, \
      /* .name                 = */ "lcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmplt, \
      /* .reverseBranchOpCode  = */ TR::lcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmpgt, \
      /* .description          = */ lcmpgt, /* long compare if greater than */ \
      /* .simplifierHandler    = */ lcmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmple, \
      /* .name                 = */ "lcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lcmpge, \
      /* .reverseBranchOpCode  = */ TR::lcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflcmple, \
      /* .description          = */ lcmple, /* long compare if less than or equal */ \
      /* .simplifierHandler    = */ lcmpleSimplifier, \
      /* .vpHandler            = */ constrainCmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmpeq, \
      /* .name                 = */ "lucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmpeq, \
      /* .reverseBranchOpCode  = */ TR::lucmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmpeq, \
      /* .description          = */ lucmpeq, /* unsigned long compare if equal */ \
      /* .simplifierHandler    = */ lcmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmpne, \
      /* .name                 = */ "lucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmpne, \
      /* .reverseBranchOpCode  = */ TR::lucmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmpne, \
      /* .description          = */ lucmpne, /* unsigned long compare if not equal */ \
      /* .simplifierHandler    = */ lcmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmplt, \
      /* .name                 = */ "lucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmpgt, \
      /* .reverseBranchOpCode  = */ TR::lucmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmplt, \
      /* .description          = */ lucmplt, /* unsigned long compare if less than */ \
      /* .simplifierHandler    = */ lucmpltSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmpge, \
      /* .name                 = */ "lucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmple, \
      /* .reverseBranchOpCode  = */ TR::lucmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmpge, \
      /* .description          = */ lucmpge, /* unsigned long compare if greater than or equal */ \
      /* .simplifierHandler    = */ lucmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmpgt, \
      /* .name                 = */ "lucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmplt, \
      /* .reverseBranchOpCode  = */ TR::lucmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmpgt, \
      /* .description          = */ lucmpgt, /* unsigned long compare if greater than */ \
      /* .simplifierHandler    = */ lucmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmple, \
      /* .name                 = */ "lucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lucmpge, \
      /* .reverseBranchOpCode  = */ TR::lucmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iflucmple, \
      /* .description          = */ lucmple, /* unsigned long compare if less than or equal */ \
      /* .simplifierHandler    = */ lucmpleSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpeq, \
      /* .name                 = */ "fcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpeq, \
      /* .reverseBranchOpCode  = */ TR::fcmpneu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpeq, \
      /* .description          = */ fcmpeq, /* float compare if equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpne, \
      /* .name                 = */ "fcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpne, \
      /* .reverseBranchOpCode  = */ TR::fcmpequ, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpne, \
      /* .description          = */ fcmpne, /* float compare if not equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmplt, \
      /* .name                 = */ "fcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpgt, \
      /* .reverseBranchOpCode  = */ TR::fcmpgeu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmplt, \
      /* .description          = */ fcmplt, /* float compare if less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpge, \
      /* .name                 = */ "fcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmple, \
      /* .reverseBranchOpCode  = */ TR::fcmpltu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpge, \
      /* .description          = */ fcmpge, /* float compare if greater than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpgt, \
      /* .name                 = */ "fcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmplt, \
      /* .reverseBranchOpCode  = */ TR::fcmpleu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpgt, \
      /* .description          = */ fcmpgt, /* float compare if greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmple, \
      /* .name                 = */ "fcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpge, \
      /* .reverseBranchOpCode  = */ TR::fcmpgtu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmple, \
      /* .description          = */ fcmple, /* float compare if less than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpequ, \
      /* .name                 = */ "fcmpequ", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpequ, \
      /* .reverseBranchOpCode  = */ TR::fcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpequ, \
      /* .description          = */ fcmpequ, /* float compare if equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpneu, \
      /* .name                 = */ "fcmpneu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpneu, \
      /* .reverseBranchOpCode  = */ TR::fcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpneu, \
      /* .description          = */ fcmpneu, /* float compare if not equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpltu, \
      /* .name                 = */ "fcmpltu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpgtu, \
      /* .reverseBranchOpCode  = */ TR::fcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpltu, \
      /* .description          = */ fcmpltu, /* float compare if less than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpgeu, \
      /* .name                 = */ "fcmpgeu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpleu, \
      /* .reverseBranchOpCode  = */ TR::fcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpgeu, \
      /* .description          = */ fcmpgeu, /* float compare if greater than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpgtu, \
      /* .name                 = */ "fcmpgtu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpltu, \
      /* .reverseBranchOpCode  = */ TR::fcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpgtu, \
      /* .description          = */ fcmpgtu, /* float compare if greater than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpleu, \
      /* .name                 = */ "fcmpleu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::fcmpgeu, \
      /* .reverseBranchOpCode  = */ TR::fcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::iffcmpleu, \
      /* .description          = */ fcmpleu, /* float compare if less than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpeq, \
      /* .name                 = */ "dcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpeq, \
      /* .reverseBranchOpCode  = */ TR::dcmpneu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpeq, \
      /* .description          = */ dcmpeq, /* double compare if equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpne, \
      /* .name                 = */ "dcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpne, \
      /* .reverseBranchOpCode  = */ TR::dcmpequ, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpne, \
      /* .description          = */ dcmpne, /* double compare if not equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmplt, \
      /* .name                 = */ "dcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpgt, \
      /* .reverseBranchOpCode  = */ TR::dcmpgeu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmplt, \
      /* .description          = */ dcmplt, /* double compare if less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpge, \
      /* .name                 = */ "dcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmple, \
      /* .reverseBranchOpCode  = */ TR::dcmpltu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpge, \
      /* .description          = */ dcmpge, /* double compare if greater than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpgt, \
      /* .name                 = */ "dcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmplt, \
      /* .reverseBranchOpCode  = */ TR::dcmpleu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpgt, \
      /* .description          = */ dcmpgt, /* double compare if greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmple, \
      /* .name                 = */ "dcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpge, \
      /* .reverseBranchOpCode  = */ TR::dcmpgtu, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmple, \
      /* .description          = */ dcmple, /* double compare if less than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpequ, \
      /* .name                 = */ "dcmpequ", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpequ, \
      /* .reverseBranchOpCode  = */ TR::dcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpequ, \
      /* .description          = */ dcmpequ, /* double compare if equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpneu, \
      /* .name                 = */ "dcmpneu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpneu, \
      /* .reverseBranchOpCode  = */ TR::dcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpneu, \
      /* .description          = */ dcmpneu, /* double compare if not equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpltu, \
      /* .name                 = */ "dcmpltu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpgtu, \
      /* .reverseBranchOpCode  = */ TR::dcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpltu, \
      /* .description          = */ dcmpltu, /* double compare if less than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpgeu, \
      /* .name                 = */ "dcmpgeu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpleu, \
      /* .reverseBranchOpCode  = */ TR::dcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpgeu, \
      /* .description          = */ dcmpgeu, /* double compare if greater than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpgtu, \
      /* .name                 = */ "dcmpgtu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpltu, \
      /* .reverseBranchOpCode  = */ TR::dcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpgtu, \
      /* .description          = */ dcmpgtu, /* double compare if greater than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpleu, \
      /* .name                 = */ "dcmpleu", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::dcmpgeu, \
      /* .reverseBranchOpCode  = */ TR::dcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifdcmpleu, \
      /* .description          = */ dcmpleu, /* double compare if less than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmpeq, \
      /* .name                 = */ "acmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmpeq, \
      /* .reverseBranchOpCode  = */ TR::acmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmpeq, \
      /* .description          = */ acmpeq, /* address compare if equal */ \
      /* .simplifierHandler    = */ acmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmpne, \
      /* .name                 = */ "acmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmpne, \
      /* .reverseBranchOpCode  = */ TR::acmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmpne, \
      /* .description          = */ acmpne, /* address compare if not equal */ \
      /* .simplifierHandler    = */ acmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmplt, \
      /* .name                 = */ "acmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmpgt, \
      /* .reverseBranchOpCode  = */ TR::acmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmplt, \
      /* .description          = */ acmplt, /* address compare if less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmpge, \
      /* .name                 = */ "acmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmple, \
      /* .reverseBranchOpCode  = */ TR::acmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmpge, \
      /* .description          = */ acmpge, /* address compare if greater than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmpgt, \
      /* .name                 = */ "acmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmplt, \
      /* .reverseBranchOpCode  = */ TR::acmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmpgt, \
      /* .description          = */ acmpgt, /* address compare if greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acmple, \
      /* .name                 = */ "acmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::UnsignedCompare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::acmpge, \
      /* .reverseBranchOpCode  = */ TR::acmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifacmple, \
      /* .description          = */ acmple, /* address compare if less than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmpeq, \
      /* .name                 = */ "bcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmpeq, \
      /* .reverseBranchOpCode  = */ TR::bcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmpeq, \
      /* .description          = */ bcmpeq, /* byte compare if equal */ \
      /* .simplifierHandler    = */ bcmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmpne, \
      /* .name                 = */ "bcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmpne, \
      /* .reverseBranchOpCode  = */ TR::bcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmpne, \
      /* .description          = */ bcmpne, /* byte compare if not equal */ \
      /* .simplifierHandler    = */ bcmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmplt, \
      /* .name                 = */ "bcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmpgt, \
      /* .reverseBranchOpCode  = */ TR::bcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmplt, \
      /* .description          = */ bcmplt, /* byte compare if less than */ \
      /* .simplifierHandler    = */ bcmpltSimplifier, \
      /* .vpHandler            = */ constrainCmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmpge, \
      /* .name                 = */ "bcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmple, \
      /* .reverseBranchOpCode  = */ TR::bcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmpge, \
      /* .description          = */ bcmpge, /* byte compare if greater than or equal */ \
      /* .simplifierHandler    = */ bcmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmpgt, \
      /* .name                 = */ "bcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmplt, \
      /* .reverseBranchOpCode  = */ TR::bcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmpgt, \
      /* .description          = */ bcmpgt, /* byte compare if greater than */ \
      /* .simplifierHandler    = */ bcmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmple, \
      /* .name                 = */ "bcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bcmpge, \
      /* .reverseBranchOpCode  = */ TR::bcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbcmple, \
      /* .description          = */ bcmple, /* byte compare if less than or equal */ \
      /* .simplifierHandler    = */ bcmpleSimplifier, \
      /* .vpHandler            = */ constrainCmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmpeq, \
      /* .name                 = */ "bucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmpeq, \
      /* .reverseBranchOpCode  = */ TR::bucmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmpeq, \
      /* .description          = */ bucmpeq, /* unsigned byte compare if equal */ \
      /* .simplifierHandler    = */ bcmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmpne, \
      /* .name                 = */ "bucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmpne, \
      /* .reverseBranchOpCode  = */ TR::bucmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmpne, \
      /* .description          = */ bucmpne, /* unsigned byte compare if not equal */ \
      /* .simplifierHandler    = */ bcmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmplt, \
      /* .name                 = */ "bucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmpgt, \
      /* .reverseBranchOpCode  = */ TR::bucmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmplt, \
      /* .description          = */ bucmplt, /* unsigned byte compare if less than */ \
      /* .simplifierHandler    = */ bcmpltSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmpge, \
      /* .name                 = */ "bucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmple, \
      /* .reverseBranchOpCode  = */ TR::bucmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmpge, \
      /* .description          = */ bucmpge, /* unsigned byte compare if greater than or equal */ \
      /* .simplifierHandler    = */ bcmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmpgt, \
      /* .name                 = */ "bucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmplt, \
      /* .reverseBranchOpCode  = */ TR::bucmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmpgt, \
      /* .description          = */ bucmpgt, /* unsigned byte compare if greater than */ \
      /* .simplifierHandler    = */ bcmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmple, \
      /* .name                 = */ "bucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::bucmpge, \
      /* .reverseBranchOpCode  = */ TR::bucmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifbucmple, \
      /* .description          = */ bucmple, /* unsigned byte compare if less than or equal */ \
      /* .simplifierHandler    = */ bcmpleSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmpeq, \
      /* .name                 = */ "scmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmpeq, \
      /* .reverseBranchOpCode  = */ TR::scmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmpeq, \
      /* .description          = */ scmpeq, /* short integer compare if equal */ \
      /* .simplifierHandler    = */ scmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmpne, \
      /* .name                 = */ "scmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmpne, \
      /* .reverseBranchOpCode  = */ TR::scmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmpne, \
      /* .description          = */ scmpne, /* short integer compare if not equal */ \
      /* .simplifierHandler    = */ scmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmplt, \
      /* .name                 = */ "scmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmpgt, \
      /* .reverseBranchOpCode  = */ TR::scmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmplt, \
      /* .description          = */ scmplt, /* short integer compare if less than */ \
      /* .simplifierHandler    = */ scmpltSimplifier, \
      /* .vpHandler            = */ constrainCmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmpge, \
      /* .name                 = */ "scmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmple, \
      /* .reverseBranchOpCode  = */ TR::scmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmpge, \
      /* .description          = */ scmpge, /* short integer compare if greater than or equal */ \
      /* .simplifierHandler    = */ scmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmpgt, \
      /* .name                 = */ "scmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmplt, \
      /* .reverseBranchOpCode  = */ TR::scmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmpgt, \
      /* .description          = */ scmpgt, /* short integer compare if greater than */ \
      /* .simplifierHandler    = */ scmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmple, \
      /* .name                 = */ "scmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::scmpge, \
      /* .reverseBranchOpCode  = */ TR::scmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifscmple, \
      /* .description          = */ scmple, /* short integer compare if less than or equal */ \
      /* .simplifierHandler    = */ scmpleSimplifier, \
      /* .vpHandler            = */ constrainCmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmpeq, \
      /* .name                 = */ "sucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmpeq, \
      /* .reverseBranchOpCode  = */ TR::sucmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmpeq, \
      /* .description          = */ sucmpeq, /* char compare if equal */ \
      /* .simplifierHandler    = */ sucmpeqSimplifier, \
      /* .vpHandler            = */ constrainCmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmpne, \
      /* .name                 = */ "sucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmpne, \
      /* .reverseBranchOpCode  = */ TR::sucmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmpne, \
      /* .description          = */ sucmpne, /* char compare if not equal */ \
      /* .simplifierHandler    = */ sucmpneSimplifier, \
      /* .vpHandler            = */ constrainCmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmplt, \
      /* .name                 = */ "sucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmpgt, \
      /* .reverseBranchOpCode  = */ TR::sucmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmplt, \
      /* .description          = */ sucmplt, /* char compare if less than */ \
      /* .simplifierHandler    = */ sucmpltSimplifier, \
      /* .vpHandler            = */ constrainCmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmpge, \
      /* .name                 = */ "sucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmple, \
      /* .reverseBranchOpCode  = */ TR::sucmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmpge, \
      /* .description          = */ sucmpge, /* char compare if greater than or equal */ \
      /* .simplifierHandler    = */ sucmpgeSimplifier, \
      /* .vpHandler            = */ constrainCmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmpgt, \
      /* .name                 = */ "sucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmplt, \
      /* .reverseBranchOpCode  = */ TR::sucmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmpgt, \
      /* .description          = */ sucmpgt, /* char compare if greater than */ \
      /* .simplifierHandler    = */ sucmpgtSimplifier, \
      /* .vpHandler            = */ constrainCmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmple, \
      /* .name                 = */ "sucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::sucmpge, \
      /* .reverseBranchOpCode  = */ TR::sucmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ifsucmple, \
      /* .description          = */ sucmple, /* char compare if less than or equal */ \
      /* .simplifierHandler    = */ sucmpleSimplifier, \
      /* .vpHandler            = */ constrainCmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmp, \
      /* .name                 = */ "lcmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lcmp, /* long compare (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2) */ \
      /* .simplifierHandler    = */ lcmpSimplifier, \
      /* .vpHandler            = */ constrainFloatCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpl, \
      /* .name                 = */ "fcmpl", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fcmpl, /* float compare l (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2 or unordered) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainFloatCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcmpg, \
      /* .name                 = */ "fcmpg", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fcmpg, /* float compare g (1 if child1 > child2 or unordered, 0 if child1 == child2, -1 if child1 < child2) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainFloatCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpl, \
      /* .name                 = */ "dcmpl", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcmpl, /* double compare l (1 if child1 > child2, 0 if child1 == child2, -1 if child1 < child2 or unordered) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainFloatCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcmpg, \
      /* .name                 = */ "dcmpg", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcmpg, /* double compare g (1 if child1 > child2 or unordered, 0 if child1 == child2, -1 if child1 < child2) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainFloatCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpeq, \
      /* .name                 = */ "ificmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmpeq, \
      /* .reverseBranchOpCode  = */ TR::ificmpne, \
      /* .booleanCompareOpCode = */ TR::icmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpeq, /* integer compare and branch if equal */ \
      /* .simplifierHandler    = */ ificmpeqSimplifier, \
      /* .vpHandler            = */ constrainIfcmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpne, \
      /* .name                 = */ "ificmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmpne, \
      /* .reverseBranchOpCode  = */ TR::ificmpeq, \
      /* .booleanCompareOpCode = */ TR::icmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpne, /* integer compare and branch if not equal */ \
      /* .simplifierHandler    = */ ificmpneSimplifier, \
      /* .vpHandler            = */ constrainIfcmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmplt, \
      /* .name                 = */ "ificmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmpgt, \
      /* .reverseBranchOpCode  = */ TR::ificmpge, \
      /* .booleanCompareOpCode = */ TR::icmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmplt, /* integer compare and branch if less than */ \
      /* .simplifierHandler    = */ ificmpltSimplifier, \
      /* .vpHandler            = */ constrainIfcmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpge, \
      /* .name                 = */ "ificmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmple, \
      /* .reverseBranchOpCode  = */ TR::ificmplt, \
      /* .booleanCompareOpCode = */ TR::icmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpge, /* integer compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ificmpgeSimplifier, \
      /* .vpHandler            = */ constrainIfcmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpgt, \
      /* .name                 = */ "ificmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmplt, \
      /* .reverseBranchOpCode  = */ TR::ificmple, \
      /* .booleanCompareOpCode = */ TR::icmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpgt, /* integer compare and branch if greater than */ \
      /* .simplifierHandler    = */ ificmpgtSimplifier, \
      /* .vpHandler            = */ constrainIfcmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmple, \
      /* .name                 = */ "ificmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ificmpge, \
      /* .reverseBranchOpCode  = */ TR::ificmpgt, \
      /* .booleanCompareOpCode = */ TR::icmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmple, /* integer compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ificmpleSimplifier, \
      /* .vpHandler            = */ constrainIfcmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmpeq, \
      /* .name                 = */ "ifiucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifiucmpne, \
      /* .booleanCompareOpCode = */ TR::iucmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmpeq, /* unsigned integer compare and branch if equal */ \
      /* .simplifierHandler    = */ ificmpeqSimplifier, \
      /* .vpHandler            = */ constrainIfcmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmpne, \
      /* .name                 = */ "ifiucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmpne, \
      /* .reverseBranchOpCode  = */ TR::ifiucmpeq, \
      /* .booleanCompareOpCode = */ TR::iucmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmpne, /* unsigned integer compare and branch if not equal */ \
      /* .simplifierHandler    = */ ificmpneSimplifier, \
      /* .vpHandler            = */ constrainIfcmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmplt, \
      /* .name                 = */ "ifiucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifiucmpge, \
      /* .booleanCompareOpCode = */ TR::iucmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmplt, /* unsigned integer compare and branch if less than */ \
      /* .simplifierHandler    = */ ificmpltSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmpge, \
      /* .name                 = */ "ifiucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmple, \
      /* .reverseBranchOpCode  = */ TR::ifiucmplt, \
      /* .booleanCompareOpCode = */ TR::iucmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmpge, /* unsigned integer compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ificmpgeSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmpgt, \
      /* .name                 = */ "ifiucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmplt, \
      /* .reverseBranchOpCode  = */ TR::ifiucmple, \
      /* .booleanCompareOpCode = */ TR::iucmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmpgt, /* unsigned integer compare and branch if greater than */ \
      /* .simplifierHandler    = */ ificmpgtSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifiucmple, \
      /* .name                 = */ "ifiucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::ifiucmpge, \
      /* .reverseBranchOpCode  = */ TR::ifiucmpgt, \
      /* .booleanCompareOpCode = */ TR::iucmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifiucmple, /* unsigned integer compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ificmpleSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpeq, \
      /* .name                 = */ "iflcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmpeq, \
      /* .reverseBranchOpCode  = */ TR::iflcmpne, \
      /* .booleanCompareOpCode = */ TR::lcmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpeq, /* long compare and branch if equal */ \
      /* .simplifierHandler    = */ iflcmpeqSimplifier, \
      /* .vpHandler            = */ constrainIfcmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpne, \
      /* .name                 = */ "iflcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmpne, \
      /* .reverseBranchOpCode  = */ TR::iflcmpeq, \
      /* .booleanCompareOpCode = */ TR::lcmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpne, /* long compare and branch if not equal */ \
      /* .simplifierHandler    = */ iflcmpneSimplifier, \
      /* .vpHandler            = */ constrainIfcmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmplt, \
      /* .name                 = */ "iflcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmpgt, \
      /* .reverseBranchOpCode  = */ TR::iflcmpge, \
      /* .booleanCompareOpCode = */ TR::lcmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmplt, /* long compare and branch if less than */ \
      /* .simplifierHandler    = */ iflcmpltSimplifier, \
      /* .vpHandler            = */ constrainIfcmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpge, \
      /* .name                 = */ "iflcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmple, \
      /* .reverseBranchOpCode  = */ TR::iflcmplt, \
      /* .booleanCompareOpCode = */ TR::lcmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpge, /* long compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ iflcmpgeSimplifier, \
      /* .vpHandler            = */ constrainIfcmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpgt, \
      /* .name                 = */ "iflcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmplt, \
      /* .reverseBranchOpCode  = */ TR::iflcmple, \
      /* .booleanCompareOpCode = */ TR::lcmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpgt, /* long compare and branch if greater than */ \
      /* .simplifierHandler    = */ iflcmpgtSimplifier, \
      /* .vpHandler            = */ constrainIfcmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmple, \
      /* .name                 = */ "iflcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflcmpge, \
      /* .reverseBranchOpCode  = */ TR::iflcmpgt, \
      /* .booleanCompareOpCode = */ TR::lcmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmple, /* long compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ iflcmpleSimplifier, \
      /* .vpHandler            = */ constrainIfcmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmpeq, \
      /* .name                 = */ "iflucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmpeq, \
      /* .reverseBranchOpCode  = */ TR::iflucmpne, \
      /* .booleanCompareOpCode = */ TR::lucmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmpeq, /* unsigned long compare and branch if equal */ \
      /* .simplifierHandler    = */ iflcmpeqSimplifier, \
      /* .vpHandler            = */ constrainIfcmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmpne, \
      /* .name                 = */ "iflucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmpne, \
      /* .reverseBranchOpCode  = */ TR::iflucmpeq, \
      /* .booleanCompareOpCode = */ TR::lucmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmpne, /* unsigned long compare and branch if not equal */ \
      /* .simplifierHandler    = */ iflcmpneSimplifier, \
      /* .vpHandler            = */ constrainIfcmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmplt, \
      /* .name                 = */ "iflucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmpgt, \
      /* .reverseBranchOpCode  = */ TR::iflucmpge, \
      /* .booleanCompareOpCode = */ TR::lucmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmplt, /* unsigned long compare and branch if less than */ \
      /* .simplifierHandler    = */ iflcmpltSimplifier, \
      /* .vpHandler            = */ constrainIfcmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmpge, \
      /* .name                 = */ "iflucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmple, \
      /* .reverseBranchOpCode  = */ TR::iflucmplt, \
      /* .booleanCompareOpCode = */ TR::lucmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmpge, /* unsigned long compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ iflcmpgeSimplifier, \
      /* .vpHandler            = */ constrainIfcmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmpgt, \
      /* .name                 = */ "iflucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmplt, \
      /* .reverseBranchOpCode  = */ TR::iflucmple, \
      /* .booleanCompareOpCode = */ TR::lucmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmpgt, /* unsigned long compare and branch if greater than */ \
      /* .simplifierHandler    = */ iflcmpgtSimplifier, \
      /* .vpHandler            = */ constrainIfcmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflucmple, \
      /* .name                 = */ "iflucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::iflucmpge, \
      /* .reverseBranchOpCode  = */ TR::iflucmpgt, \
      /* .booleanCompareOpCode = */ TR::lucmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflucmple, /* unsigned long compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ iflcmpleSimplifier, \
      /* .vpHandler            = */ constrainIfcmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpeq, \
      /* .name                 = */ "iffcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpeq, \
      /* .reverseBranchOpCode  = */ TR::iffcmpneu, \
      /* .booleanCompareOpCode = */ TR::fcmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpeq, /* float compare and branch if equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpne, \
      /* .name                 = */ "iffcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpne, \
      /* .reverseBranchOpCode  = */ TR::iffcmpequ, \
      /* .booleanCompareOpCode = */ TR::fcmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpne, /* float compare and branch if not equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmplt, \
      /* .name                 = */ "iffcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpgt, \
      /* .reverseBranchOpCode  = */ TR::iffcmpgeu, \
      /* .booleanCompareOpCode = */ TR::fcmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmplt, /* float compare and branch if less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpge, \
      /* .name                 = */ "iffcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmple, \
      /* .reverseBranchOpCode  = */ TR::iffcmpltu, \
      /* .booleanCompareOpCode = */ TR::fcmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpge, /* float compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpgt, \
      /* .name                 = */ "iffcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmplt, \
      /* .reverseBranchOpCode  = */ TR::iffcmpleu, \
      /* .booleanCompareOpCode = */ TR::fcmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpgt, /* float compare and branch if greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmple, \
      /* .name                 = */ "iffcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpge, \
      /* .reverseBranchOpCode  = */ TR::iffcmpgtu, \
      /* .booleanCompareOpCode = */ TR::fcmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmple, /* float compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpequ, \
      /* .name                 = */ "iffcmpequ", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpequ, \
      /* .reverseBranchOpCode  = */ TR::iffcmpne, \
      /* .booleanCompareOpCode = */ TR::fcmpequ, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpequ, /* float compare and branch if equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpneu, \
      /* .name                 = */ "iffcmpneu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpneu, \
      /* .reverseBranchOpCode  = */ TR::iffcmpeq, \
      /* .booleanCompareOpCode = */ TR::fcmpneu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpneu, /* float compare and branch if not equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpltu, \
      /* .name                 = */ "iffcmpltu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpgtu, \
      /* .reverseBranchOpCode  = */ TR::iffcmpge, \
      /* .booleanCompareOpCode = */ TR::fcmpltu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpltu, /* float compare and branch if less than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpgeu, \
      /* .name                 = */ "iffcmpgeu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpleu, \
      /* .reverseBranchOpCode  = */ TR::iffcmplt, \
      /* .booleanCompareOpCode = */ TR::fcmpgeu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpgeu, /* float compare and branch if greater than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpgtu, \
      /* .name                 = */ "iffcmpgtu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpltu, \
      /* .reverseBranchOpCode  = */ TR::iffcmple, \
      /* .booleanCompareOpCode = */ TR::fcmpgtu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpgtu, /* float compare and branch if greater than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iffcmpleu, \
      /* .name                 = */ "iffcmpleu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::iffcmpgeu, \
      /* .reverseBranchOpCode  = */ TR::iffcmpgt, \
      /* .booleanCompareOpCode = */ TR::fcmpleu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iffcmpleu, /* float compare and branch if less than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpeq, \
      /* .name                 = */ "ifdcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpneu, \
      /* .booleanCompareOpCode = */ TR::dcmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpeq, /* double compare and branch if equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpne, \
      /* .name                 = */ "ifdcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpne, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpequ, \
      /* .booleanCompareOpCode = */ TR::dcmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpne, /* double compare and branch if not equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmplt, \
      /* .name                 = */ "ifdcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpgeu, \
      /* .booleanCompareOpCode = */ TR::dcmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmplt, /* double compare and branch if less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpge, \
      /* .name                 = */ "ifdcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmple, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpltu, \
      /* .booleanCompareOpCode = */ TR::dcmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpge, /* double compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpgt, \
      /* .name                 = */ "ifdcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmplt, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpleu, \
      /* .booleanCompareOpCode = */ TR::dcmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpgt, /* double compare and branch if greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmple, \
      /* .name                 = */ "ifdcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpge, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpgtu, \
      /* .booleanCompareOpCode = */ TR::dcmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmple, /* double compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpequ, \
      /* .name                 = */ "ifdcmpequ", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpequ, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpne, \
      /* .booleanCompareOpCode = */ TR::dcmpequ, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpequ, /* double compare and branch if equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpneu, \
      /* .name                 = */ "ifdcmpneu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpneu, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpeq, \
      /* .booleanCompareOpCode = */ TR::dcmpneu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpneu, /* double compare and branch if not equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpltu, \
      /* .name                 = */ "ifdcmpltu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpgtu, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpge, \
      /* .booleanCompareOpCode = */ TR::dcmpltu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpltu, /* double compare and branch if less than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpgeu, \
      /* .name                 = */ "ifdcmpgeu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpleu, \
      /* .reverseBranchOpCode  = */ TR::ifdcmplt, \
      /* .booleanCompareOpCode = */ TR::dcmpgeu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpgeu, /* double compare and branch if greater than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpgtu, \
      /* .name                 = */ "ifdcmpgtu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpltu, \
      /* .reverseBranchOpCode  = */ TR::ifdcmple, \
      /* .booleanCompareOpCode = */ TR::dcmpgtu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpgtu, /* double compare and branch if greater than or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifdcmpleu, \
      /* .name                 = */ "ifdcmpleu", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual | ILProp3::CompareTrueIfUnordered, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::ifdcmpgeu, \
      /* .reverseBranchOpCode  = */ TR::ifdcmpgt, \
      /* .booleanCompareOpCode = */ TR::dcmpleu, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifdcmpleu, /* double compare and branch if less than or equal or unordered */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmpeq, \
      /* .name                 = */ "ifacmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifacmpne, \
      /* .booleanCompareOpCode = */ TR::acmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmpeq, /* address compare and branch if equal */ \
      /* .simplifierHandler    = */ ifacmpeqSimplifier, \
      /* .vpHandler            = */ constrainIfcmpeq, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmpne, \
      /* .name                 = */ "ifacmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmpne, \
      /* .reverseBranchOpCode  = */ TR::ifacmpeq, \
      /* .booleanCompareOpCode = */ TR::acmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmpne, /* address compare and branch if not equal */ \
      /* .simplifierHandler    = */ ifacmpneSimplifier, \
      /* .vpHandler            = */ constrainIfcmpne, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmplt, \
      /* .name                 = */ "ifacmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifacmpge, \
      /* .booleanCompareOpCode = */ TR::acmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmplt, /* address compare and branch if less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIfcmplt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmpge, \
      /* .name                 = */ "ifacmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmple, \
      /* .reverseBranchOpCode  = */ TR::ifacmplt, \
      /* .booleanCompareOpCode = */ TR::acmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmpge, /* address compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIfcmpge, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmpgt, \
      /* .name                 = */ "ifacmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmplt, \
      /* .reverseBranchOpCode  = */ TR::ifacmple, \
      /* .booleanCompareOpCode = */ TR::acmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmpgt, /* address compare and branch if greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIfcmpgt, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifacmple, \
      /* .name                 = */ "ifacmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::ifacmpge, \
      /* .reverseBranchOpCode  = */ TR::ifacmpgt, \
      /* .booleanCompareOpCode = */ TR::acmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifacmple, /* address compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIfcmple, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmpeq, \
      /* .name                 = */ "ifbcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifbcmpne, \
      /* .booleanCompareOpCode = */ TR::bcmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmpeq, /* byte compare and branch if equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmpne, \
      /* .name                 = */ "ifbcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmpne, \
      /* .reverseBranchOpCode  = */ TR::ifbcmpeq, \
      /* .booleanCompareOpCode = */ TR::bcmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmpne, /* byte compare and branch if not equal */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmplt, \
      /* .name                 = */ "ifbcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifbcmpge, \
      /* .booleanCompareOpCode = */ TR::bcmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmplt, /* byte compare and branch if less than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmpge, \
      /* .name                 = */ "ifbcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmple, \
      /* .reverseBranchOpCode  = */ TR::ifbcmplt, \
      /* .booleanCompareOpCode = */ TR::bcmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmpge, /* byte compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmpgt, \
      /* .name                 = */ "ifbcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmplt, \
      /* .reverseBranchOpCode  = */ TR::ifbcmple, \
      /* .booleanCompareOpCode = */ TR::bcmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmpgt, /* byte compare and branch if greater than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbcmple, \
      /* .name                 = */ "ifbcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbcmpge, \
      /* .reverseBranchOpCode  = */ TR::ifbcmpgt, \
      /* .booleanCompareOpCode = */ TR::bcmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbcmple, /* byte compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmpeq, \
      /* .name                 = */ "ifbucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifbucmpne, \
      /* .booleanCompareOpCode = */ TR::bucmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmpeq, /* unsigned byte compare and branch if equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmpne, \
      /* .name                 = */ "ifbucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmpne, \
      /* .reverseBranchOpCode  = */ TR::ifbucmpeq, \
      /* .booleanCompareOpCode = */ TR::bucmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmpne, /* unsigned byte compare and branch if not equal */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmplt, \
      /* .name                 = */ "ifbucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifbucmpge, \
      /* .booleanCompareOpCode = */ TR::bucmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmplt, /* unsigned byte compare and branch if less than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmpge, \
      /* .name                 = */ "ifbucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmple, \
      /* .reverseBranchOpCode  = */ TR::ifbucmplt, \
      /* .booleanCompareOpCode = */ TR::bucmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmpge, /* unsigned byte compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmpgt, \
      /* .name                 = */ "ifbucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmplt, \
      /* .reverseBranchOpCode  = */ TR::ifbucmple, \
      /* .booleanCompareOpCode = */ TR::bucmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmpgt, /* unsigned byte compare and branch if greater than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifbucmple, \
      /* .name                 = */ "ifbucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::ifbucmpge, \
      /* .reverseBranchOpCode  = */ TR::ifbucmpgt, \
      /* .booleanCompareOpCode = */ TR::bucmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifbucmple, /* unsigned byte compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmpeq, \
      /* .name                 = */ "ifscmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifscmpne, \
      /* .booleanCompareOpCode = */ TR::scmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmpeq, /* short integer compare and branch if equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmpne, \
      /* .name                 = */ "ifscmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmpne, \
      /* .reverseBranchOpCode  = */ TR::ifscmpeq, \
      /* .booleanCompareOpCode = */ TR::scmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmpne, /* short integer compare and branch if not equal */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmplt, \
      /* .name                 = */ "ifscmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifscmpge, \
      /* .booleanCompareOpCode = */ TR::scmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmplt, /* short integer compare and branch if less than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmpge, \
      /* .name                 = */ "ifscmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmple, \
      /* .reverseBranchOpCode  = */ TR::ifscmplt, \
      /* .booleanCompareOpCode = */ TR::scmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmpge, /* short integer compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmpgt, \
      /* .name                 = */ "ifscmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmplt, \
      /* .reverseBranchOpCode  = */ TR::ifscmple, \
      /* .booleanCompareOpCode = */ TR::scmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmpgt, /* short integer compare and branch if greater than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifscmple, \
      /* .name                 = */ "ifscmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifscmpge, \
      /* .reverseBranchOpCode  = */ TR::ifscmpgt, \
      /* .booleanCompareOpCode = */ TR::scmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifscmple, /* short integer compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmpeq, \
      /* .name                 = */ "ifsucmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmpeq, \
      /* .reverseBranchOpCode  = */ TR::ifsucmpne, \
      /* .booleanCompareOpCode = */ TR::sucmpeq, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmpeq, /* char compare and branch if equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmpne, \
      /* .name                 = */ "ifsucmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmpne, \
      /* .reverseBranchOpCode  = */ TR::ifsucmpeq, \
      /* .booleanCompareOpCode = */ TR::sucmpne, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmpne, /* char compare and branch if not equal */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmplt, \
      /* .name                 = */ "ifsucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmpgt, \
      /* .reverseBranchOpCode  = */ TR::ifsucmpge, \
      /* .booleanCompareOpCode = */ TR::sucmplt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmplt, /* char compare and branch if less than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmpge, \
      /* .name                 = */ "ifsucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmple, \
      /* .reverseBranchOpCode  = */ TR::ifsucmplt, \
      /* .booleanCompareOpCode = */ TR::sucmpge, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmpge, /* char compare and branch if greater than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmpgt, \
      /* .name                 = */ "ifsucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmplt, \
      /* .reverseBranchOpCode  = */ TR::ifsucmple, \
      /* .booleanCompareOpCode = */ TR::sucmpgt, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmpgt, /* char compare and branch if greater than */ \
      /* .simplifierHandler    = */ ifCmpWithoutEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ifsucmple, \
      /* .name                 = */ "ifsucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::ifsucmpge, \
      /* .reverseBranchOpCode  = */ TR::ifsucmpgt, \
      /* .booleanCompareOpCode = */ TR::sucmple, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ifsucmple, /* char compare and branch if less than or equal */ \
      /* .simplifierHandler    = */ ifCmpWithEqualitySimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::loadaddr, \
      /* .name                 = */ "loadaddr", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack | ILProp2::LoadAddress, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ loadaddr, /* load address of non-heap storage item (Auto, Parm, Static or Method) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLoadaddr, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ZEROCHK, \
      /* .name                 = */ "ZEROCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ZEROCHK, /* Zero-check an int.  Symref indicates call to perform when first child is zero.  Other children are arguments to the call. */ \
      /* .simplifierHandler    = */ lowerTreeSimplifier, \
      /* .vpHandler            = */ constrainZeroChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::callIf, \
      /* .name                 = */ "callIf", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ CHILD_COUNT(ILChildProp::UnspecifiedChildCount) | FIRST_CHILD(TR::Int32) | SECOND_CHILD(ILChildProp::UnspecifiedChildType) | THIRD_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ callIf, /* Call symref if first child evaluates to true.  Other childrem are arguments to the call. */ \
      /* .simplifierHandler    = */ lowerTreeSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iRegLoad, \
      /* .name                 = */ "iRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iRegLoad, /* Load integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aRegLoad, \
      /* .name                 = */ "aRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aRegLoad, /* Load address global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lRegLoad, \
      /* .name                 = */ "lRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lRegLoad, /* Load long integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fRegLoad, \
      /* .name                 = */ "fRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fRegLoad, /* Load float global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dRegLoad, \
      /* .name                 = */ "dRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dRegLoad, /* Load double global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sRegLoad, \
      /* .name                 = */ "sRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sRegLoad, /* Load short global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bRegLoad, \
      /* .name                 = */ "bRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bRegLoad, /* Load byte global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iRegStore, \
      /* .name                 = */ "iRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iRegStore, /* Store integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aRegStore, \
      /* .name                 = */ "aRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aRegStore, /* Store address global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lRegStore, \
      /* .name                 = */ "lRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lRegStore, /* Store long integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fRegStore, \
      /* .name                 = */ "fRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fRegStore, /* Store float global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dRegStore, \
      /* .name                 = */ "dRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dRegStore, /* Store double global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sRegStore, \
      /* .name                 = */ "sRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sRegStore, /* Store short global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bRegStore, \
      /* .name                 = */ "bRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bRegStore, /* Store byte global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::GlRegDeps, \
      /* .name                 = */ "GlRegDeps", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ GlRegDeps, /* Global Register Dependency List */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iselect, \
      /* .name                 = */ "iselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Int32, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iselect, /* Select Operator:  Based on the result of the first child, take the value of the */ \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lselect, \
      /* .name                 = */ "lselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Int64, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lselect, /* second (first child evaluates to true) or third(first child evaluates to false) child */ \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bselect, \
      /* .name                 = */ "bselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Int8, TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bselect, \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sselect, \
      /* .name                 = */ "sselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Int16, TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sselect, \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aselect, \
      /* .name                 = */ "aselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Address, TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aselect, \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fselect, \
      /* .name                 = */ "fselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Float, TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fselect, \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dselect, \
      /* .name                 = */ "dselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Double, TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dselect, \
      /* .simplifierHandler    = */ selectSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::treetop, \
      /* .name                 = */ "treetop", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ treetop, /* tree top to anchor subtrees with side-effects */ \
      /* .simplifierHandler    = */ treetopSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::MethodEnterHook, \
      /* .name                 = */ "MethodEnterHook", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ MethodEnterHook, /* called after a frame is built, temps initialized, and monitor acquired (if necessary) */ \
      /* .simplifierHandler    = */ lowerTreeSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::MethodExitHook, \
      /* .name                 = */ "MethodExitHook", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MustBeLowered| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ MethodExitHook, /* called immediately before returning, frame not yet collapsed, monitor released (if necessary) */ \
      /* .simplifierHandler    = */ lowerTreeSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::PassThrough, \
      /* .name                 = */ "PassThrough", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ PassThrough, /* Dummy node that represents its single child. */ \
      /* .simplifierHandler    = */ passThroughSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::compressedRefs, \
      /* .name                 = */ "compressedRefs", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(ILChildProp::UnspecifiedChildType, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ compressedRefs, /* no-op anchor providing optimizable subexpressions used for compression/decompression.  First child is address load/store, second child is heap base displacement */ \
      /* .simplifierHandler    = */ anchorSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::BBStart, \
      /* .name                 = */ "BBStart", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ BBStart, /* Start of Basic Block */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::BBEnd, \
      /* .name                 = */ "BBEnd", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ BBEnd, /* End of Basic Block */ \
      /* .simplifierHandler    = */ endBlockSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::virem, \
      /* .name                 = */ "virem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ virem, /* vector integer remainder */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vimin, \
      /* .name                 = */ "vimin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vimin, /* vector integer minimum */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vimax, \
      /* .name                 = */ "vimax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vimax, /* vector integer maximum */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vigetelem, \
      /* .name                 = */ "vigetelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vigetelem, /* get vector int element */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::visetelem, \
      /* .name                 = */ "visetelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ visetelem, /* set vector int element */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vimergel, \
      /* .name                 = */ "vimergel", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vimergel, /* vector int merge low */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vimergeh, \
      /* .name                 = */ "vimergeh", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vimergeh, /* vector int merge high */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpeq, \
      /* .name                 = */ "vicmpeq", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpeq, /* vector integer compare equal  (return vector mask) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpgt, \
      /* .name                 = */ "vicmpgt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpgt, /* vector integer compare greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpge, \
      /* .name                 = */ "vicmpge", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpge, /* vector integer compare greater equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmplt, \
      /* .name                 = */ "vicmplt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmplt, /* vector integer compare less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmple, \
      /* .name                 = */ "vicmple", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmple, /* vector integer compare less equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpalleq, \
      /* .name                 = */ "vicmpalleq", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpalleq, /* vector integer all equal (return boolean) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpallne, \
      /* .name                 = */ "vicmpallne", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpallne, /* vector integer all not equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpallgt, \
      /* .name                 = */ "vicmpallgt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpallgt, /* vector integer all greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpallge, \
      /* .name                 = */ "vicmpallge", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpallge, /* vector integer all greater equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpalllt, \
      /* .name                 = */ "vicmpalllt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpalllt, /* vector integer all less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpallle, \
      /* .name                 = */ "vicmpallle", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpallle, /* vector integer all less equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanyeq, \
      /* .name                 = */ "vicmpanyeq", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanyeq, /* vector integer any equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanyne, \
      /* .name                 = */ "vicmpanyne", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanyne, /* vector integer any not equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanygt, \
      /* .name                 = */ "vicmpanygt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanygt, /* vector integer any greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanyge, \
      /* .name                 = */ "vicmpanyge", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanyge, /* vector integer any greater equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanylt, \
      /* .name                 = */ "vicmpanylt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanylt, /* vector integer any less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vicmpanyle, \
      /* .name                 = */ "vicmpanyle", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vicmpanyle, /* vector integer any less equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vnot, \
      /* .name                 = */ "vnot", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vnot, /* vector boolean not */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vbitselect, \
      /* .name                 = */ "vbitselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vbitselect, /* vector bit select */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vperm, \
      /* .name                 = */ "vperm", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vperm, /* vector permute */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vsplats, \
      /* .name                 = */ "vsplats", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vsplats, /* vector splats */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmergel, \
      /* .name                 = */ "vdmergel", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmergel, /* vector double merge low */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmergeh, \
      /* .name                 = */ "vdmergeh", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmergeh, /* vector double merge high */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdsetelem, \
      /* .name                 = */ "vdsetelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdsetelem, /* set vector double element */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdgetelem, \
      /* .name                 = */ "vdgetelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdgetelem, /* get vector double element */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdsel, \
      /* .name                 = */ "vdsel", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdsel, /* get vector select double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdrem, \
      /* .name                 = */ "vdrem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdrem, /* vector double remainder */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmadd, \
      /* .name                 = */ "vdmadd", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmadd, /* vector double fused multiply add */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdnmsub, \
      /* .name                 = */ "vdnmsub", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdnmsub, /* vector double fused negative multiply subtract */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmsub, \
      /* .name                 = */ "vdmsub", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmsub, /* vector double fused multiply subtract */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmax, \
      /* .name                 = */ "vdmax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmax, /* vector double maximum */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdmin, \
      /* .name                 = */ "vdmin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdmin, /* vector double minimum */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpeq, \
      /* .name                 = */ "vdcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpeq, \
      /* .reverseBranchOpCode  = */ TR::vdcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpeq, /* vector double compare equal  (return vector mask) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpne, \
      /* .name                 = */ "vdcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpne, \
      /* .reverseBranchOpCode  = */ TR::vdcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpne, /* vector double compare not equal  (return vector mask) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpgt, \
      /* .name                 = */ "vdcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmplt, \
      /* .reverseBranchOpCode  = */ TR::vdcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpgt, /* vector double compare greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpge, \
      /* .name                 = */ "vdcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmple, \
      /* .reverseBranchOpCode  = */ TR::vdcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpge, /* vector double compare greater equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmplt, \
      /* .name                 = */ "vdcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpgt, \
      /* .reverseBranchOpCode  = */ TR::vdcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmplt, /* vector double compare less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmple, \
      /* .name                 = */ "vdcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpge, \
      /* .reverseBranchOpCode  = */ TR::vdcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmple, /* vector double compare less equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpalleq, \
      /* .name                 = */ "vdcmpalleq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpalleq, \
      /* .reverseBranchOpCode  = */ TR::vdcmpallne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpalleq, /* vector double compare all equal  (return boolean) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpallne, \
      /* .name                 = */ "vdcmpallne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpallne, \
      /* .reverseBranchOpCode  = */ TR::vdcmpalleq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpallne, /* vector double compare all not equal  (return boolean) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpallgt, \
      /* .name                 = */ "vdcmpallgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpalllt, \
      /* .reverseBranchOpCode  = */ TR::vdcmpallle, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpallgt, /* vector double compare all greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpallge, \
      /* .name                 = */ "vdcmpallge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpallle, \
      /* .reverseBranchOpCode  = */ TR::vdcmpalllt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpallge, /* vector double compare all greater equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpalllt, \
      /* .name                 = */ "vdcmpalllt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpallgt, \
      /* .reverseBranchOpCode  = */ TR::vdcmpallge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpalllt, /* vector double compare all less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpallle, \
      /* .name                 = */ "vdcmpallle", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpallge, \
      /* .reverseBranchOpCode  = */ TR::vdcmpallgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpallle, /* vector double compare all less equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanyeq, \
      /* .name                 = */ "vdcmpanyeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanyeq, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanyne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanyeq, /* vector double compare any equal  (return boolean) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanyne, \
      /* .name                 = */ "vdcmpanyne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanyne, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanyeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanyne, /* vector double compare any not equal  (return boolean) */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanygt, \
      /* .name                 = */ "vdcmpanygt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanylt, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanyle, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanygt, /* vector double compare any greater than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanyge, \
      /* .name                 = */ "vdcmpanyge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanyle, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanylt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanyge, /* vector double compare any greater equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanylt, \
      /* .name                 = */ "vdcmpanylt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanygt, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanyge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanylt, /* vector double compare any less than */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdcmpanyle, \
      /* .name                 = */ "vdcmpanyle", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vdcmpanyge, \
      /* .reverseBranchOpCode  = */ TR::vdcmpanygt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdcmpanyle, /* vector double compare any less equal */ \
      /* .simplifierHandler    = */ normalizeCmpSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdsqrt, \
      /* .name                 = */ "vdsqrt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdsqrt, /* vector double square root */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdlog, \
      /* .name                 = */ "vdlog", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Floating_Point | ILTypeProp::Vector, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdlog, /* vector double natural log */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vinc, \
      /* .name                 = */ "vinc", \
      /* .properties1          = */ ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vinc, /* vector increment */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdec, \
      /* .name                 = */ "vdec", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdec, /* vector decrement */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vneg, \
      /* .name                 = */ "vneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vneg, /* vector negation */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcom, \
      /* .name                 = */ "vcom", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcom, /* vector complement */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vadd, \
      /* .name                 = */ "vadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vadd, /* vector add */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vsub, \
      /* .name                 = */ "vsub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vsub, /* vector subtract */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vmul, \
      /* .name                 = */ "vmul", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vmul, /* vector multiply */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdiv, \
      /* .name                 = */ "vdiv", \
      /* .properties1          = */ ILProp1::Div, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdiv, /* vector divide */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vrem, \
      /* .name                 = */ "vrem", \
      /* .properties1          = */ ILProp1::Rem, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vrem, /* vector remainder */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vand, \
      /* .name                 = */ "vand", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::And, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vand, /* vector logical AND */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vor, \
      /* .name                 = */ "vor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Or, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vor, /* vector logical OR */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vxor, \
      /* .name                 = */ "vxor", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Xor, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vxor, /* vector exclusive OR integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vshl, \
      /* .name                 = */ "vshl", \
      /* .properties1          = */ ILProp1::LeftShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vshl, /* vector shift left */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vushr, \
      /* .name                 = */ "vushr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vushr, /* vector shift right logical */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vshr, \
      /* .name                 = */ "vshr", \
      /* .properties1          = */ ILProp1::RightShift, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vshr, /* vector shift right arithmetic */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmpeq, \
      /* .name                 = */ "vcmpeq", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpeq, \
      /* .reverseBranchOpCode  = */ TR::vcmpne, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::ificmpeq, \
      /* .description          = */ vcmpeq, /* vector compare equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmpne, \
      /* .name                 = */ "vcmpne", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpne, \
      /* .reverseBranchOpCode  = */ TR::vcmpeq, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcmpne, /* vector compare not equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmplt, \
      /* .name                 = */ "vcmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpgt, \
      /* .reverseBranchOpCode  = */ TR::vcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcmplt, /* vector compare less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vucmplt, \
      /* .name                 = */ "vucmplt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpgt, \
      /* .reverseBranchOpCode  = */ TR::vcmpge, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vucmplt, /* vector unsigned compare less than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmpgt, \
      /* .name                 = */ "vcmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmplt, \
      /* .reverseBranchOpCode  = */ TR::vcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcmpgt, /* vector compare greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vucmpgt, \
      /* .name                 = */ "vucmpgt", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmplt, \
      /* .reverseBranchOpCode  = */ TR::vcmple, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vucmpgt, /* vector unsigned compare greater than */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmple, \
      /* .name                 = */ "vcmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpge, \
      /* .reverseBranchOpCode  = */ TR::vcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcmple, /* vector compare less or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vucmple, \
      /* .name                 = */ "vucmple", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfLess | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmpge, \
      /* .reverseBranchOpCode  = */ TR::vcmpgt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vucmple, /* vector unsigned compare less or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcmpge, \
      /* .name                 = */ "vcmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmple, \
      /* .reverseBranchOpCode  = */ TR::vcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcmpge, /* vector compare greater or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vucmpge, \
      /* .name                 = */ "vucmpge", \
      /* .properties1          = */ ILProp1::BooleanCompare, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare, \
      /* .properties3          = */ ILProp3::CompareTrueIfGreater | ILProp3::CompareTrueIfEqual, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::Unsigned | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::vcmple, \
      /* .reverseBranchOpCode  = */ TR::vcmplt, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vucmpge, /* vector unsigned compare greater or equal */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCmp, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vload, \
      /* .name                 = */ "vload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vload, /* load vector */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vloadi, \
      /* .name                 = */ "vloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vloadi, /* load indirect vector */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vstore, \
      /* .name                 = */ "vstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vstore, /* store vector */ \
      /* .simplifierHandler    = */ directStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vstorei, \
      /* .name                 = */ "vstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vstorei, /* store indirect vector */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vrand, \
      /* .name                 = */ "vrand", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::VectorReduction, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vrand, /* AND all elements into single value of element size */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vreturn, \
      /* .name                 = */ "vreturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vreturn, /* return a vector */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcall, \
      /* .name                 = */ "vcall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcall, /* direct call returning a vector */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vcalli, \
      /* .name                 = */ "vcalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vcalli, /* indirect call returning a vector */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vselect, \
      /* .name                 = */ "vselect", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Select, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vselect, /* vector select operator */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildrenFirstToLast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::v2v, \
      /* .name                 = */ "v2v", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ v2v, /* vector to vector conversion. preserves bit pattern (noop), only changes datatype */ \
      /* .simplifierHandler    = */ v2vSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vl2vd, \
      /* .name                 = */ "vl2vd", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vl2vd, /* vector to vector conversion. converts each long element to double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vconst, \
      /* .name                 = */ "vconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vconst, /* vector constant */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::getvelem, \
      /* .name                 = */ "getvelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ getvelem, /* get vector element, returns a scalar */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vsetelem, \
      /* .name                 = */ "vsetelem", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Vector | ILTypeProp::HasNoDataType, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vsetelem, /* vector set element */ \
      /* .simplifierHandler    = */ vsetelemSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vbRegLoad, \
      /* .name                 = */ "vbRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt8, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vbRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vsRegLoad, \
      /* .name                 = */ "vsRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt16, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vsRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::viRegLoad, \
      /* .name                 = */ "viRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ viRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vlRegLoad, \
      /* .name                 = */ "vlRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vlRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vfRegLoad, \
      /* .name                 = */ "vfRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorFloat, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vfRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdRegLoad, \
      /* .name                 = */ "vdRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdRegLoad, /* Load vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vbRegStore, \
      /* .name                 = */ "vbRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt8, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vbRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vsRegStore, \
      /* .name                 = */ "vsRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt16, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vsRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::viRegStore, \
      /* .name                 = */ "viRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt32, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ viRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vlRegStore, \
      /* .name                 = */ "vlRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorInt64, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vlRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vfRegStore, \
      /* .name                 = */ "vfRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorFloat, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vfRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::vdRegStore, \
      /* .name                 = */ "vdRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::VectorDouble, \
      /* .typeProperties       = */ ILTypeProp::Size_16 | ILTypeProp::Integer | ILTypeProp::Vector, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ vdRegStore, /* Store vector global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuconst, \
      /* .name                 = */ "iuconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuconst, /* load unsigned integer constant (32-but unsigned) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainIntConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luconst, \
      /* .name                 = */ "luconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luconst, /* load unsigned long integer constant (64-bit unsigned) */ \
      /* .simplifierHandler    = */ lconstSimplifier, \
      /* .vpHandler            = */ constrainLongConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::buconst, \
      /* .name                 = */ "buconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ buconst, /* load unsigned byte integer constant (8-bit unsigned) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainByteConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuload, \
      /* .name                 = */ "iuload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuload, /* load unsigned integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luload, \
      /* .name                 = */ "luload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luload, /* load unsigned long integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::buload, \
      /* .name                 = */ "buload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ buload, /* load unsigned byte */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuloadi, \
      /* .name                 = */ "iuloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuloadi, /* load indirect unsigned integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIiload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luloadi, \
      /* .name                 = */ "luloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luloadi, /* load indirect unsigned long integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainLload, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::buloadi, \
      /* .name                 = */ "buloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ buloadi, /* load indirect unsigned byte */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iustore, \
      /* .name                 = */ "iustore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iustore, /* store unsigned integer */ \
      /* .simplifierHandler    = */ directStoreSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lustore, \
      /* .name                 = */ "lustore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lustore, /* store unsigned long integer */ \
      /* .simplifierHandler    = */ lstoreSimplifier, \
      /* .vpHandler            = */ constrainLongStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bustore, \
      /* .name                 = */ "bustore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bustore, /* store unsigned byte */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iustorei, \
      /* .name                 = */ "iustorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iustorei, /* store indirect unsigned integer       (child1 a, child2 i) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lustorei, \
      /* .name                 = */ "lustorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lustorei, /* store indirect unsigned long integer  (child1 a, child2 l) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bustorei, \
      /* .name                 = */ "bustorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bustorei, /* store indirect unsigned byte          (child1 a, child2 b) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iureturn, \
      /* .name                 = */ "iureturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iureturn, /* return an unsigned integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lureturn, \
      /* .name                 = */ "lureturn", \
      /* .properties1          = */ ILProp1::Return | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lureturn, /* return a long unsigned integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainReturn, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucall, \
      /* .name                 = */ "iucall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iucall, /* direct call returning unsigned integer */ \
      /* .simplifierHandler    = */ ifdCallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucall, \
      /* .name                 = */ "lucall", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lucall, /* direct call returning unsigned long integer */ \
      /* .simplifierHandler    = */ lcallSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuadd, \
      /* .name                 = */ "iuadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iuadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuadd, /* add 2 unsigned integers */ \
      /* .simplifierHandler    = */ iaddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luadd, \
      /* .name                 = */ "luadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::luadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luadd, /* add 2 unsigned long integers */ \
      /* .simplifierHandler    = */ laddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::buadd, \
      /* .name                 = */ "buadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::buadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ buadd, /* add 2 unsigned bytes */ \
      /* .simplifierHandler    = */ baddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iusub, \
      /* .name                 = */ "iusub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iusub, /* subtract 2 unsigned integers       (child1 - child2) */ \
      /* .simplifierHandler    = */ isubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lusub, \
      /* .name                 = */ "lusub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lusub, /* subtract 2 unsigned long integers  (child1 - child2) */ \
      /* .simplifierHandler    = */ lsubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::busub, \
      /* .name                 = */ "busub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ busub, /* subtract 2 unsigned bytes          (child1 - child2) */ \
      /* .simplifierHandler    = */ bsubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuneg, \
      /* .name                 = */ "iuneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuneg, /* negate an unsigned integer */ \
      /* .simplifierHandler    = */ inegSimplifier, \
      /* .vpHandler            = */ constrainIneg, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luneg, \
      /* .name                 = */ "luneg", \
      /* .properties1          = */ ILProp1::Neg, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luneg, /* negate a unsigned long integer */ \
      /* .simplifierHandler    = */ lnegSimplifier, \
      /* .vpHandler            = */ constrainLneg, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2iu, \
      /* .name                 = */ "f2iu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2iu, /* convert float to unsigned integer */ \
      /* .simplifierHandler    = */ f2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2lu, \
      /* .name                 = */ "f2lu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2lu, /* convert float to unsigned long integer */ \
      /* .simplifierHandler    = */ f2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2bu, \
      /* .name                 = */ "f2bu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2bu, /* convert float to unsigned byte */ \
      /* .simplifierHandler    = */ f2bSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::f2c, \
      /* .name                 = */ "f2c", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ f2c, /* convert float to char */ \
      /* .simplifierHandler    = */ f2cSimplifier, \
      /* .vpHandler            = */ constrainNarrowToChar, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2iu, \
      /* .name                 = */ "d2iu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2iu, /* convert double to unsigned integer */ \
      /* .simplifierHandler    = */ d2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2lu, \
      /* .name                 = */ "d2lu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2lu, /* convert double to unsigned long integer */ \
      /* .simplifierHandler    = */ d2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2bu, \
      /* .name                 = */ "d2bu", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2bu, /* convert double to unsigned byte */ \
      /* .simplifierHandler    = */ d2bSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::d2c, \
      /* .name                 = */ "d2c", \
      /* .properties1          = */ ILProp1::Conversion, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ d2c, /* convert double to char */ \
      /* .simplifierHandler    = */ d2cSimplifier, \
      /* .vpHandler            = */ constrainNarrowToChar, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuRegLoad, \
      /* .name                 = */ "iuRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuRegLoad, /* Load unsigned integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luRegLoad, \
      /* .name                 = */ "luRegLoad", \
      /* .properties1          = */ ILProp1::LoadReg, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luRegLoad, /* Load unsigned long integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuRegStore, \
      /* .name                 = */ "iuRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuRegStore, /* Store unsigned integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luRegStore, \
      /* .name                 = */ "luRegStore", \
      /* .properties1          = */ ILProp1::StoreReg | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luRegStore, /* Store long integer global register */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cconst, \
      /* .name                 = */ "cconst", \
      /* .properties1          = */ ILProp1::LoadConst, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cconst, /* load unicode constant (16-bit unsigned) */ \
      /* .simplifierHandler    = */ constSimplifier, \
      /* .vpHandler            = */ constrainShortConst, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cload, \
      /* .name                 = */ "cload", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cload, /* load short unsigned integer */ \
      /* .simplifierHandler    = */ directLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cloadi, \
      /* .name                 = */ "cloadi", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Indirect | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cloadi, /* load indirect unsigned short integer */ \
      /* .simplifierHandler    = */ indirectLoadSimplifier, \
      /* .vpHandler            = */ constrainIntLoad, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cstore, \
      /* .name                 = */ "cstore", \
      /* .properties1          = */ ILProp1::Store | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ONE_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cstore, /* store unsigned short integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cstorei, \
      /* .name                 = */ "cstorei", \
      /* .properties1          = */ ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cstorei, /* store indirect unsigned short integer (child1 a, child2 c) */ \
      /* .simplifierHandler    = */ indirectStoreSimplifier, \
      /* .vpHandler            = */ constrainStore, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::monent, \
      /* .name                 = */ "monent", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ monent, /* acquire lock for synchronising method */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainMonent, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::monexit, \
      /* .name                 = */ "monexit", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ monexit, /* release lock for synchronising method */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainMonexit, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::monexitfence, \
      /* .name                 = */ "monexitfence", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ monexitfence, /* denotes the end of a monitored region solely for live monitor meta data */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainMonexitfence, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::tstart, \
      /* .name                 = */ "tstart", \
      /* .properties1          = */ ILProp1::HasSymbolRef | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ ILProp3::HasBranchChild, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ THREE_SAME_CHILD(TR::NoType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ tstart, /* transaction begin */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainTstart, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::tfinish, \
      /* .name                 = */ "tfinish", \
      /* .properties1          = */ ILProp1::HasSymbolRef | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ tfinish, /* transaction end */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainTfinish, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::tabort, \
      /* .name                 = */ "tabort", \
      /* .properties1          = */ ILProp1::HasSymbolRef | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ tabort, /* transaction abort */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainTabort, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::instanceof, \
      /* .name                 = */ "instanceof", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ instanceof, /* instanceof - symref is the class object, cp index is in the */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainInstanceOf, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::checkcast, \
      /* .name                 = */ "checkcast", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::Check | ILProp2::CheckCast| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ checkcast, /* checkcast */ \
      /* .simplifierHandler    = */ checkcastSimplifier, \
      /* .vpHandler            = */ constrainCheckcast, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::checkcastAndNULLCHK, \
      /* .name                 = */ "checkcastAndNULLCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::Check | ILProp2::CheckCast| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ checkcastAndNULLCHK, /* checkcast and NULL check the underlying object reference */ \
      /* .simplifierHandler    = */ checkcastAndNULLCHKSimplifier, \
      /* .vpHandler            = */ constrainCheckcastNullChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::New, \
      /* .name                 = */ "new", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ New, /* new - child is class */ \
      /* .simplifierHandler    = */ NewSimplifier, \
      /* .vpHandler            = */ constrainNew, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::newvalue, \
      /* .name                 = */ "newvalue", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ newvalue, /* allocate and initialize - children provide field values */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::newarray, \
      /* .name                 = */ "newarray", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ newarray, /* new array of primitives */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainNewArray, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::anewarray, \
      /* .name                 = */ "anewarray", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ anewarray, /* new array of objects */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainANewArray, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::variableNew, \
      /* .name                 = */ "variableNew", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ variableNew, /* new - child is class, type not known at compile time */ \
      /* .simplifierHandler    = */ variableNewSimplifier, \
      /* .vpHandler            = */ constrainVariableNew, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::variableNewArray, \
      /* .name                 = */ "variableNewArray", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ variableNewArray, /* new array - type not known at compile time, type must be a j9class, do not use type enums */ \
      /* .simplifierHandler    = */ variableNewSimplifier, \
      /* .vpHandler            = */ constrainVariableNewArray, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::multianewarray, \
      /* .name                 = */ "multianewarray", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MustBeLowered | ILProp2::CanRaiseException| ILProp2::MayUseSystemStack | ILProp2::New, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ multianewarray, /* multi-dimensional new array of objects */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainMultiANewArray, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraylength, \
      /* .name                 = */ "arraylength", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::ArrayLength, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraylength, /* number of elements in an array */ \
      /* .simplifierHandler    = */ arrayLengthSimplifier, \
      /* .vpHandler            = */ constrainArraylength, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::contigarraylength, \
      /* .name                 = */ "contigarraylength", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::ArrayLength, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ contigarraylength, /* number of elements in a contiguous array */ \
      /* .simplifierHandler    = */ arrayLengthSimplifier, \
      /* .vpHandler            = */ constrainArraylength, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::discontigarraylength, \
      /* .name                 = */ "discontigarraylength", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::MustBeLowered | ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::ArrayLength, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ discontigarraylength, /* number of elements in a discontiguous array */ \
      /* .simplifierHandler    = */ arrayLengthSimplifier, \
      /* .vpHandler            = */ constrainArraylength, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icalli, \
      /* .name                 = */ "icalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ icalli, /* indirect call returning integer (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucalli, \
      /* .name                 = */ "iucalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iucalli, /* indirect call returning unsigned integer (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcalli, \
      /* .name                 = */ "lcalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lcalli, /* indirect call returning long integer (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucalli, \
      /* .name                 = */ "lucalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lucalli, /* indirect call returning unsigned long integer (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fcalli, \
      /* .name                 = */ "fcalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fcalli, /* indirect call returning float (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcalli, \
      /* .name                 = */ "dcalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcalli, /* indirect call returning double (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::acalli, \
      /* .name                 = */ "acalli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ acalli, /* indirect call returning reference */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainAcall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::calli, \
      /* .name                 = */ "calli", \
      /* .properties1          = */ ILProp1::Indirect | ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::SupportedForPRE| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::IndirectCallType, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ calli, /* indirect call returning void (child1 is addr of function) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCall, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fence, \
      /* .name                 = */ "fence", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::Fence | ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fence, /* barrier to optimization */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luaddh, \
      /* .name                 = */ "luaddh", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::luaddh, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luaddh, /* add 2 unsigned long integers (the high parts of prior luadd) as high part of 128bit addition. */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cadd, \
      /* .name                 = */ "cadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::cadd, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cadd, /* add 2 unsigned short integers */ \
      /* .simplifierHandler    = */ caddSimplifier, \
      /* .vpHandler            = */ constrainAdd, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aiadd, \
      /* .name                 = */ "aiadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aiadd, /* add integer to address with address result (child1 a, child2 i) */ \
      /* .simplifierHandler    = */ iaddSimplifier, \
      /* .vpHandler            = */ constrainAddressRef, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aiuadd, \
      /* .name                 = */ "aiuadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aiuadd, /* add unsigned integer to address with address result (child1 a, child2 i) */ \
      /* .simplifierHandler    = */ iaddSimplifier, \
      /* .vpHandler            = */ constrainAddressRef, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aladd, \
      /* .name                 = */ "aladd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aladd, /* add long integer to address with address result (child1 a, child2 i) (64-bit only) */ \
      /* .simplifierHandler    = */ laddSimplifier, \
      /* .vpHandler            = */ constrainAddressRef, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::aluadd, \
      /* .name                 = */ "aluadd", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Add, \
      /* .properties2          = */ ILProp2::ValueNumberShare, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ aluadd, /* add unsigned long integer to address with address result (child1 a, child2 i) (64-bit only) */ \
      /* .simplifierHandler    = */ laddSimplifier, \
      /* .vpHandler            = */ constrainAddressRef, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lusubh, \
      /* .name                 = */ "lusubh", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lusubh, /* subtract 2 unsigned long integers (the high parts of prior lusub) as high part of 128bit subtraction. */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::csub, \
      /* .name                 = */ "csub", \
      /* .properties1          = */ ILProp1::Sub, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ csub, /* subtract 2 unsigned short integers (child1 - child2) */ \
      /* .simplifierHandler    = */ csubSimplifier, \
      /* .vpHandler            = */ constrainSubtract, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::imulh, \
      /* .name                 = */ "imulh", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::imulh, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ imulh, /* multiply 2 integers, and return the high word of the product */ \
      /* .simplifierHandler    = */ imulhSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iumulh, \
      /* .name                 = */ "iumulh", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::iumulh, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iumulh, /* multiply 2 unsigned integers, and return the high word of the product */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lmulh, \
      /* .name                 = */ "lmulh", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lmulh, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lmulh, /* multiply 2 long integers, and return the high word of the product */ \
      /* .simplifierHandler    = */ lmulhSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lumulh, \
      /* .name                 = */ "lumulh", \
      /* .properties1          = */ ILProp1::Commutative | ILProp1::Associative | ILProp1::Mul, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::lumulh, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lumulh, /* multiply 2 unsigned long integers, and return the high word of the product */ \
      /* .simplifierHandler    = */ lmulhSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ibits2f, \
      /* .name                 = */ "ibits2f", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ibits2f, /* type-coerce int to float */ \
      /* .simplifierHandler    = */ ibits2fSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fbits2i, \
      /* .name                 = */ "fbits2i", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fbits2i, /* type-coerce float to int */ \
      /* .simplifierHandler    = */ fbits2iSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lbits2d, \
      /* .name                 = */ "lbits2d", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lbits2d, /* type-coerce long to double */ \
      /* .simplifierHandler    = */ lbits2dSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dbits2l, \
      /* .name                 = */ "dbits2l", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dbits2l, /* type-coerce double to long */ \
      /* .simplifierHandler    = */ dbits2lSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lookup, \
      /* .name                 = */ "lookup", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::Switch, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ ILProp3::HasBranchChild, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lookup, /* lookupswitch (child1 is selector expression, child2 the default destination, subsequent children are case nodes */ \
      /* .simplifierHandler    = */ lookupSwitchSimplifier, \
      /* .vpHandler            = */ constrainSwitch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::trtLookup, \
      /* .name                 = */ "trtLookup", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::Switch, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ ILProp3::HasBranchChild, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ trtLookup, /* special lookupswitch (child1 must be trt, child2 the default destination, subsequent children are case nodes) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::Case, \
      /* .name                 = */ "case", \
      /* .properties1          = */ ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ Case, /* case nodes that are children of TR_switch.  Uses the branchdestination and the int const field */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCase, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::table, \
      /* .name                 = */ "table", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::Switch, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ ILProp3::HasBranchChild, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ table, /* tableswitch (child1 is the selector, child2 the default destination, subsequent children are the branch targets */ \
      /* .simplifierHandler    = */ tableSwitchSimplifier, \
      /* .vpHandler            = */ constrainSwitch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::exceptionRangeFence, \
      /* .name                 = */ "exceptionRangeFence", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::ExceptionRangeFence, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ exceptionRangeFence, /* (J9) SymbolReference is the aliasing effect, initializer is where the code address gets put when binary is generated */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dbgFence, \
      /* .name                 = */ "dbgFence", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::ExceptionRangeFence, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dbgFence, /* used to delimit code (stmts) for debug info.  Has no symbol reference. */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::NULLCHK, \
      /* .name                 = */ "NULLCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::NullCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ NULLCHK, /* Null check a pointer.  child 1 is indirect reference. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ nullchkSimplifier, \
      /* .vpHandler            = */ constrainNullChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ResolveCHK, \
      /* .name                 = */ "ResolveCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::ResolveCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ResolveCHK, /* Resolve check a static, field or method. child 1 is reference to be resolved. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainResolveChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ResolveAndNULLCHK, \
      /* .name                 = */ "ResolveAndNULLCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::NullCheck | ILProp2::ResolveCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ResolveAndNULLCHK, /* Resolve check a static, field or method and Null check the underlying pointer.  child 1 is reference to be resolved. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainResolveNullChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::DIVCHK, \
      /* .name                 = */ "DIVCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ DIVCHK, /* Divide by zero check. child 1 is the divide. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ divchkSimplifier, \
      /* .vpHandler            = */ constrainDivChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::OverflowCHK, \
      /* .name                 = */ "OverflowCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::Check| ILProp2::MayUseSystemStack | ILProp2::CanRaiseException, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ OverflowCHK, /* Overflow check. child 1 is the operation node(add, mul, sub). Child 2 and child 3 are the operands of the operation of the operation. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainOverflowChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::UnsignedOverflowCHK, \
      /* .name                 = */ "UnsignedOverflowCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::Check| ILProp2::MayUseSystemStack | ILProp2::CanRaiseException, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ THREE_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ UnsignedOverflowCHK, /* UnsignedOverflow check. child 1 is the operation node(add, mul, sub). Child 2 and child 3 are the operands of the operation of the operation. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainUnsignedOverflowChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::BNDCHK, \
      /* .name                 = */ "BNDCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ BNDCHK, /* Array bounds check, checks that child 1 > child 2 >= 0 (child 1 is bound, 2 is index). Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ bndchkSimplifier, \
      /* .vpHandler            = */ constrainBndChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ArrayCopyBNDCHK, \
      /* .name                 = */ "ArrayCopyBNDCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ArrayCopyBNDCHK, /* Array copy bounds check, checks that child 1 >= child 2. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ arraycopybndchkSimplifier, \
      /* .vpHandler            = */ constrainArrayCopyBndChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::BNDCHKwithSpineCHK, \
      /* .name                 = */ "BNDCHKwithSpineCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check | ILProp2::BndCheck| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::SpineCheck, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ BNDCHKwithSpineCHK, /* Array bounds check and spine check */ \
      /* .simplifierHandler    = */ bndchkwithspinechkSimplifier, \
      /* .vpHandler            = */ constrainBndChkWithSpineChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::SpineCHK, \
      /* .name                 = */ "SpineCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::Check, \
      /* .properties3          = */ ILProp3::SpineCheck, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ SpineCHK, /* Check if the base array has a spine */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ArrayStoreCHK, \
      /* .name                 = */ "ArrayStoreCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ONE_CHILD(ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ArrayStoreCHK, /* Array store check. child 1 is object, 2 is array. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainArrayStoreChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ArrayCHK, \
      /* .name                 = */ "ArrayCHK", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::CanRaiseException | ILProp2::Check| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ArrayCHK, /* Array compatibility check. child 1 is object1, 2 is object2. Symbolref indicates failure action/destination */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainArrayChk, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::Ret, \
      /* .name                 = */ "Ret", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ Ret, /* Used by ilGen only */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraycopy, \
      /* .name                 = */ "arraycopy", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::MayUseSystemStack | ILProp2::CanRaiseException | 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraycopy, /* Call to System.arraycopy that may be partially inlined */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainArraycopy, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arrayset, \
      /* .name                 = */ "arrayset", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arrayset, /* Inline code for memory initialization of part of an array */ \
      /* .simplifierHandler    = */ arraysetSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraytranslate, \
      /* .name                 = */ "arraytranslate", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraytranslate, /* Inline code for translation of part of an array to another form via lookup */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraytranslateAndTest, \
      /* .name                 = */ "arraytranslateAndTest", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException | ILProp2::BndCheck, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraytranslateAndTest, /* Inline code for scanning of part of an array for a particular 8-bit character */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainTRT, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::long2String, \
      /* .name                 = */ "long2String", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ long2String, /* Convert integer/long value to String */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bitOpMem, \
      /* .name                 = */ "bitOpMem", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bitOpMem, /* bit operations (AND, OR, XOR) for memory to memory */ \
      /* .simplifierHandler    = */ bitOpMemSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bitOpMemND, \
      /* .name                 = */ "bitOpMemND", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bitOpMemND, /* 3 operand(source1,source2,target) version of bitOpMem */ \
      /* .simplifierHandler    = */ bitOpMemNDSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraycmp, \
      /* .name                 = */ "arraycmp", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraycmp, /* Inline code for memory comparison of part of an array */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::arraycmpWithPad, \
      /* .name                 = */ "arraycmpWithPad", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef | ILProp3::SkipDynamicLitPoolOnInts, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ arraycmpWithPad, /* memory comparison when src1 length != src2 length and padding is needed */ \
      /* .simplifierHandler    = */ arrayCmpWithPadSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::allocationFence, \
      /* .name                 = */ "allocationFence", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ allocationFence, /* Internal fence guarding escape of newObject & final fields - eliminatable */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::loadFence, \
      /* .name                 = */ "loadFence", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ loadFence, /* JEP171: prohibits loadLoad and loadStore reordering (on globals) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::storeFence, \
      /* .name                 = */ "storeFence", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ storeFence, /* JEP171: prohibits loadStore and storeStore reordering (on globals) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fullFence, \
      /* .name                 = */ "fullFence", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fullFence, /* JEP171: prohibits loadLoad, loadStore, storeLoad, and storeStore reordering (on globals) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::MergeNew, \
      /* .name                 = */ "MergeNew", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::CanRaiseException| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ MergeNew, /* Parent for New etc. nodes that can all be allocated together */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::computeCC, \
      /* .name                 = */ "computeCC", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ computeCC, /* compute Condition Codes */ \
      /* .simplifierHandler    = */ computeCCSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::butest, \
      /* .name                 = */ "butest", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ butest, /* zEmulator: mask unsigned byte (UInt8) and set condition codes */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sutest, \
      /* .name                 = */ "sutest", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sutest, /* zEmulator: mask unsigned short (UInt16) and set condition codes */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bucmp, \
      /* .name                 = */ "bucmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bucmp, /* Currently only valid for zEmulator. Based on the ordering of the two children set the return value: */ \
      /* .simplifierHandler    = */ bucmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bcmp, \
      /* .name                 = */ "bcmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bcmp, /* 0 : child1 == child2 */ \
      /* .simplifierHandler    = */ bcmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sucmp, \
      /* .name                 = */ "sucmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sucmp, /* 1 : child1 < child2 */ \
      /* .simplifierHandler    = */ sucmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::scmp, \
      /* .name                 = */ "scmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ scmp, /* 2 : child1 > child2 */ \
      /* .simplifierHandler    = */ scmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iucmp, \
      /* .name                 = */ "iucmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iucmp, \
      /* .simplifierHandler    = */ iucmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmp, \
      /* .name                 = */ "icmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ icmp, \
      /* .simplifierHandler    = */ icmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lucmp, \
      /* .name                 = */ "lucmp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::UnsignedCompare | ILProp2::CondCodeComputation, \
      /* .properties3          = */ ILProp3::Signum, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lucmp, \
      /* .simplifierHandler    = */ lucmpSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpo, \
      /* .name                 = */ "ificmpo", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::ificmpno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpo, /* integer compare and branch if overflow */ \
      /* .simplifierHandler    = */ ifxcmpoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmpno, \
      /* .name                 = */ "ificmpno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::ificmpo, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmpno, /* integer compare and branch if not overflow */ \
      /* .simplifierHandler    = */ ifxcmpoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpo, \
      /* .name                 = */ "iflcmpo", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::iflcmpno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpo, /* long compare and branch if overflow */ \
      /* .simplifierHandler    = */ ifxcmpoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmpno, \
      /* .name                 = */ "iflcmpno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::iflcmpo, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmpno, /* long compare and branch if not overflow */ \
      /* .simplifierHandler    = */ ifxcmpoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmno, \
      /* .name                 = */ "ificmno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::ificmnno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmno, /* integer compare negative and branch if overflow */ \
      /* .simplifierHandler    = */ ifxcmnoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ificmnno, \
      /* .name                 = */ "ificmnno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::ificmno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ificmnno, /* integer compare negative and branch if not overflow */ \
      /* .simplifierHandler    = */ ifxcmnoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmno, \
      /* .name                 = */ "iflcmno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::iflcmnno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmno, /* long compare negative and branch if overflow */ \
      /* .simplifierHandler    = */ ifxcmnoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iflcmnno, \
      /* .name                 = */ "iflcmnno", \
      /* .properties1          = */ ILProp1::BooleanCompare | ILProp1::Branch | ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::OverflowCompare, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::iflcmno, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iflcmnno, /* long compare negative and branch if not overflow */ \
      /* .simplifierHandler    = */ ifxcmnoSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuaddc, \
      /* .name                 = */ "iuaddc", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectAdd, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuaddc, /* Currently only valid for zEmulator.  Add two unsigned ints with carry */ \
      /* .simplifierHandler    = */ iaddSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luaddc, \
      /* .name                 = */ "luaddc", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectAdd, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luaddc, /* Add two longs with carry */ \
      /* .simplifierHandler    = */ laddSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iusubb, \
      /* .name                 = */ "iusubb", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectSub, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iusubb, /* Subtract two ints with borrow */ \
      /* .simplifierHandler    = */ isubSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lusubb, \
      /* .name                 = */ "lusubb", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::SelectSub, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lusubb, /* Subtract two longs with borrow */ \
      /* .simplifierHandler    = */ lsubSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::icmpset, \
      /* .name                 = */ "icmpset", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int32, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ icmpset, /* icmpset(pointer,c,r): compare *pointer with c, if it matches, replace with r.  Returns 0 on match, 1 otherwise */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lcmpset, \
      /* .name                 = */ "lcmpset", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Address, TR::Int64, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lcmpset, /* the operation is done atomically - return type is int for both [il]cmpset */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bztestnset, \
      /* .name                 = */ "bztestnset", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, ILChildProp::UnspecifiedChildType), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bztestnset, /* bztestnset(pointer,c): atomically sets *pointer to c and returns the original value of *p (represents Test And Set on Z) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ibatomicor, \
      /* .name                 = */ "ibatomicor", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ibatomicor, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::isatomicor, \
      /* .name                 = */ "isatomicor", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ isatomicor, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iiatomicor, \
      /* .name                 = */ "iiatomicor", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iiatomicor, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ilatomicor, \
      /* .name                 = */ "ilatomicor", \
      /* .properties1          = */ ILProp1::LoadVar | ILProp1::Store | ILProp1::Indirect | ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::ValueNumberShare| ILProp2::MayUseSystemStack, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_CHILD(TR::Address, TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ilatomicor, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dexp, \
      /* .name                 = */ "dexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dexp, /* double exponent */ \
      /* .simplifierHandler    = */ expSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::branch, \
      /* .name                 = */ "branch", \
      /* .properties1          = */ ILProp1::Branch | ILProp1::CompBranchOnly | ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ branch, /* generic branch --> DEPRECATED use TR::case instead */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainCondBranch, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::igoto, \
      /* .name                 = */ "igoto", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(TR::Address), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ igoto, /* indirect goto, branches to the address specified by a child */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIgoto, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bexp, \
      /* .name                 = */ "bexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bexp, /* signed byte exponent  (raise signed byte to power) */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::buexp, \
      /* .name                 = */ "buexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int8), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ buexp, /* unsigned byte exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sexp, \
      /* .name                 = */ "sexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sexp, /* short exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::cexp, \
      /* .name                 = */ "cexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int16), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ cexp, /* unsigned short exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iexp, \
      /* .name                 = */ "iexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iexp, /* integer exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iuexp, \
      /* .name                 = */ "iuexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iuexp, /* unsigned integer exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lexp, \
      /* .name                 = */ "lexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lexp, /* long exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::luexp, \
      /* .name                 = */ "luexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ luexp, /* unsigned long exponent */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fexp, \
      /* .name                 = */ "fexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::SignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fexp, /* float exponent */ \
      /* .simplifierHandler    = */ expSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fuexp, \
      /* .name                 = */ "fuexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Float, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fuexp, /* float base to unsigned integral exponent */ \
      /* .simplifierHandler    = */ expSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::duexp, \
      /* .name                 = */ "duexp", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::UnsignedExponentiation, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_CHILD(TR::Double, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ duexp, /* double base to unsigned integral exponent */ \
      /* .simplifierHandler    = */ expSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ixfrs, \
      /* .name                 = */ "ixfrs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ixfrs, /* transfer sign integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lxfrs, \
      /* .name                 = */ "lxfrs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lxfrs, /* transfer sign long */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fxfrs, \
      /* .name                 = */ "fxfrs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fxfrs, /* transfer sign float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dxfrs, \
      /* .name                 = */ "dxfrs", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dxfrs, /* transfer sign double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fint, \
      /* .name                 = */ "fint", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fint, /* truncate float to int */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dint, \
      /* .name                 = */ "dint", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dint, /* truncate double to int */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fnint, \
      /* .name                 = */ "fnint", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fnint, /* round float to nearest int */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dnint, \
      /* .name                 = */ "dnint", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dnint, /* round double to nearest int */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fsqrt, \
      /* .name                 = */ "fsqrt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fsqrt, /* square root of float */ \
      /* .simplifierHandler    = */ fsqrtSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dsqrt, \
      /* .name                 = */ "dsqrt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dsqrt, /* square root of double */ \
      /* .simplifierHandler    = */ dsqrtSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::getstack, \
      /* .name                 = */ "getstack", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Address, \
      /* .typeProperties       = */ ILTypeProp::Reference, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ getstack, /* returns current value of SP */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dealloca, \
      /* .name                 = */ "dealloca", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dealloca, /* resets value of SP */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::idoz, \
      /* .name                 = */ "idoz", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ idoz, /* difference or zero */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcos, \
      /* .name                 = */ "dcos", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcos, /* cos of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dsin, \
      /* .name                 = */ "dsin", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dsin, /* sin of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dtan, \
      /* .name                 = */ "dtan", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dtan, /* tan of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dcosh, \
      /* .name                 = */ "dcosh", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dcosh, /* cos of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dsinh, \
      /* .name                 = */ "dsinh", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dsinh, /* sin of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dtanh, \
      /* .name                 = */ "dtanh", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dtanh, /* tan of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dacos, \
      /* .name                 = */ "dacos", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dacos, /* arccos of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dasin, \
      /* .name                 = */ "dasin", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dasin, /* arcsin of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::datan, \
      /* .name                 = */ "datan", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ datan, /* arctan of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::datan2, \
      /* .name                 = */ "datan2", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ datan2, /* arctan2 of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dlog, \
      /* .name                 = */ "dlog", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dlog, /* log of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dfloor, \
      /* .name                 = */ "dfloor", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dfloor, /* floor of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ffloor, \
      /* .name                 = */ "ffloor", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ffloor, /* floor of float, returning float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dceil, \
      /* .name                 = */ "dceil", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dceil, /* ceil of double, returning double */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fceil, \
      /* .name                 = */ "fceil", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ ONE_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fceil, /* ceil of float, returning float */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ibranch, \
      /* .name                 = */ "ibranch", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ibranch, /* generic indirct branch --> first child is a constant indicating the mask */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIgoto, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::mbranch, \
      /* .name                 = */ "mbranch", \
      /* .properties1          = */ ILProp1::Branch, \
      /* .properties2          = */ ILProp2::JumpWithMultipleTargets, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ mbranch, /* generic branch to multiple potential targets */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIgoto, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::getpm, \
      /* .name                 = */ "getpm", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ getpm, /* get program mask */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::setpm, \
      /* .name                 = */ "setpm", \
      /* .properties1          = */ ILProp1::TreeTop, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ setpm, /* set program mask */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::loadAutoOffset, \
      /* .name                 = */ "loadAutoOffset", \
      /* .properties1          = */ ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ ILChildProp::NoChildren, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ loadAutoOffset, /* loads the offset (from the SP) of an auto */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::imax, \
      /* .name                 = */ "imax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ imax, /* max of 2 or more integers */ \
      /* .simplifierHandler    = */ imaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iumax, \
      /* .name                 = */ "iumax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iumax, /* max of 2 or more unsigned integers */ \
      /* .simplifierHandler    = */ imaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lmax, \
      /* .name                 = */ "lmax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lmax, /* max of 2 or more longs */ \
      /* .simplifierHandler    = */ lmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lumax, \
      /* .name                 = */ "lumax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lumax, /* max of 2 or more unsigned longs */ \
      /* .simplifierHandler    = */ lmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fmax, \
      /* .name                 = */ "fmax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fmax, /* max of 2 or more floats */ \
      /* .simplifierHandler    = */ fmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dmax, \
      /* .name                 = */ "dmax", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Max, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dmax, /* max of 2 or more doubles */ \
      /* .simplifierHandler    = */ dmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::imin, \
      /* .name                 = */ "imin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ imin, /* min of 2 or more integers */ \
      /* .simplifierHandler    = */ imaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::iumin, \
      /* .name                 = */ "iumin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ iumin, /* min of 2 or more unsigned integers */ \
      /* .simplifierHandler    = */ imaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lmin, \
      /* .name                 = */ "lmin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lmin, /* min of 2 or more longs */ \
      /* .simplifierHandler    = */ lmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lumin, \
      /* .name                 = */ "lumin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Unsigned, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Int64), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lumin, /* min of 2 or more unsigned longs */ \
      /* .simplifierHandler    = */ lmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::fmin, \
      /* .name                 = */ "fmin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Float, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Float), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ fmin, /* min of 2 or more floats */ \
      /* .simplifierHandler    = */ fmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::dmin, \
      /* .name                 = */ "dmin", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::Min, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Double, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Floating_Point, \
      /* .childProperties      = */ TWO_SAME_CHILD(TR::Double), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ dmin, /* min of 2 or more doubles */ \
      /* .simplifierHandler    = */ dmaxminSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::trt, \
      /* .name                 = */ "trt", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ trt, /* translate and test */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::trtSimple, \
      /* .name                 = */ "trtSimple", \
      /* .properties1          = */ ILProp1::Call | ILProp1::HasSymbolRef, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse | ILProp3::LikeDef, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ trtSimple, /* same as TRT but ignoring the returned source byte address and table entry value */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ihbit, \
      /* .name                 = */ "ihbit", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ihbit, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntegerHighestOneBit, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ilbit, \
      /* .name                 = */ "ilbit", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ilbit, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntegerLowestOneBit, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::inolz, \
      /* .name                 = */ "inolz", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ inolz, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntegerNumberOfLeadingZeros, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::inotz, \
      /* .name                 = */ "inotz", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ inotz, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntegerNumberOfTrailingZeros, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ipopcnt, \
      /* .name                 = */ "ipopcnt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ipopcnt, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainIntegerBitCount, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lhbit, \
      /* .name                 = */ "lhbit", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lhbit, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongHighestOneBit, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::llbit, \
      /* .name                 = */ "llbit", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ llbit, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongLowestOneBit, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lnolz, \
      /* .name                 = */ "lnolz", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lnolz, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongNumberOfLeadingZeros, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lnotz, \
      /* .name                 = */ "lnotz", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lnotz, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongNumberOfTrailingZeros, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lpopcnt, \
      /* .name                 = */ "lpopcnt", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::SupportedForPRE, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lpopcnt, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainLongBitCount, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ibyteswap, \
      /* .name                 = */ "ibyteswap", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE | ILProp2::ByteSwap, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ ONE_CHILD(TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ibyteswap, /* swap bytes in an integer */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::bbitpermute, \
      /* .name                 = */ "bbitpermute", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int8, \
      /* .typeProperties       = */ ILTypeProp::Size_1 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int8, TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ bbitpermute, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::sbitpermute, \
      /* .name                 = */ "sbitpermute", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int16, \
      /* .typeProperties       = */ ILTypeProp::Size_2 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int16, TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ sbitpermute, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::ibitpermute, \
      /* .name                 = */ "ibitpermute", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int32, \
      /* .typeProperties       = */ ILTypeProp::Size_4 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int32, TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ ibitpermute, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::lbitpermute, \
      /* .name                 = */ "lbitpermute", \
      /* .properties1          = */ 0, \
      /* .properties2          = */ ILProp2::ValueNumberShare | ILProp2::SupportedForPRE, \
      /* .properties3          = */ ILProp3::LikeUse, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::Int64, \
      /* .typeProperties       = */ ILTypeProp::Size_8 | ILTypeProp::Integer, \
      /* .childProperties      = */ THREE_CHILD(TR::Int64, TR::Address, TR::Int32), \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ lbitpermute, \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) \
   MACRO(\
      /* .opcode               = */ TR::Prefetch, \
      /* .name                 = */ "Prefetch", \
      /* .properties1          = */ ILProp1::TreeTop | ILProp1::HasSymbolRef, \
      /* .properties2          = */ 0, \
      /* .properties3          = */ 0, \
      /* .properties4          = */ 0, \
      /* .dataType             = */ TR::NoType, \
      /* .typeProperties       = */ 0, \
      /* .childProperties      = */ ILChildProp::Unspecified, \
      /* .swapChildrenOpCode   = */ TR::BadILOp, \
      /* .reverseBranchOpCode  = */ TR::BadILOp, \
      /* .booleanCompareOpCode = */ TR::BadILOp, \
      /* .ifCompareOpCode      = */ TR::BadILOp, \
      /* .description          = */ Prefetch, /* Prefetch */ \
      /* .simplifierHandler    = */ dftSimplifier, \
      /* .vpHandler            = */ constrainChildren, \
   ) 
#endif
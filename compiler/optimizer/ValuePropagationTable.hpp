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

#ifndef VALUEPROPAGATIONTABLE_INCL
#define VALUEPROPAGATIONTABLE_INCL

TR::Node *constrainAcall(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAdd(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAddressRef(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainANewArray(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainVariableNew(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainVariableNewArray(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAnyIntLoad(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainArrayChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainArraycopy(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainArrayCopyBndChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainArraylength(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainArrayStoreChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAstore(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainB2i(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainB2s(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainB2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBndChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBndChkWithSpineChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBu2i(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBu2s(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBu2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainByteConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCall(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCase(OMR::ValuePropagation *vp, TR::Node *node);
//TR::Node *constrainCharConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCheckcast(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCheckcastNullChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmp(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmpeq(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmpge(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmpgt(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmple(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmplt(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCmpne(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainCondBranch(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainDivChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainOverflowChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainUnsignedOverflowChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainDload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainFload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainFloatCmp(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainGoto(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainI2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIaload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIand(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIdiv(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmpeq(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmpge(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmpgt(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmple(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmplt(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIfcmpne(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIiload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainImul(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIneg(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIabs(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainInstanceOf(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIntAndFloatConstHelper(OMR::ValuePropagation *vp, TR::Node *node, int32_t value);
TR::Node *constrainIntConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainFloatConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIntLoad(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIntStore(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIor(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIrem(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIshl(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIshr(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIu2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIushr(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIxor(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLand(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLdiv(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLload(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLmul(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLneg(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLabs(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLoadaddr(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLongConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLongStore(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLor(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLrem(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLshl(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLshr(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLushr(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainLxor(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainMonent(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainMonexit(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainMonexitfence(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainTstart(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainTfinish(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainTabort(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainMultiANewArray(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNarrowToByte(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNarrowToChar(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNarrowToInt(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNarrowToShort(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNew(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNewArray(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainNullChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainZeroChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainAsm(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainIgoto(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainResolveChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainResolveNullChk(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainReturn(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainS2i(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainS2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainShortConst(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainShortLoad(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainStore(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainSu2i(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainSu2l(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainSubtract(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainSwitch(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainThrow(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainTRT(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainWrtBar(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBCDCHK(OMR::ValuePropagation *vp, TR::Node *node);

TR::Node *constrainBCDAggrLoad(OMR::ValuePropagation *vp, TR::Node *node);

TR::Node *constrainIntegralToBCD(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node *constrainBCDToIntegral(OMR::ValuePropagation *vp, TR::Node *node);


TR::Node * constrainIntegerHighestOneBit(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainIntegerLowestOneBit(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainIntegerNumberOfLeadingZeros(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainIntegerNumberOfTrailingZeros(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainIntegerBitCount(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainLongHighestOneBit(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainLongLowestOneBit(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainLongNumberOfLeadingZeros(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainLongNumberOfTrailingZeros(OMR::ValuePropagation *vp, TR::Node *node);
TR::Node * constrainLongBitCount(OMR::ValuePropagation *vp, TR::Node *node);

const ValuePropagationPtr constraintHandlers[] =
   {
#define GET_VP_HANDLER(\
   opcode, \
   name, \
   prop1, \
   prop2, \
   prop3, \
   prop4, \
   dataType, \
   typeProps, \
   childProps, \
   swapChildrenOpcode, \
   reverseBranchOpcode, \
   boolCompareOpcode, \
   ifCompareOpcode, \
   enumValue, \
   simplifier, \
   vpHandler, \
   ...) vpHandler,

   FOR_EACH_OPCODE(GET_VP_HANDLER)

#undef GET_VP_HANDLER

#ifdef J9_PROJECT_SPECIFIC
   constrainChildren,           // TR::dfconst
   constrainChildren,           // TR::ddconst
   constrainChildren,           // TR::deconst
   constrainChildren,           // TR::dfload
   constrainChildren,           // TR::ddload
   constrainChildren,           // TR::deload
   constrainChildren,           // TR::dfloadi
   constrainChildren,           // TR::ddloadi
   constrainChildren,           // TR::deloadi
   constrainChildren,           // TR::dfstore
   constrainChildren,           // TR::ddstore
   constrainChildren,           // TR::destore
   constrainChildren,           // TR::dfstorei
   constrainChildren,           // TR::ddstorei
   constrainChildren,           // TR::destorei
   constrainReturn,             // TR::dfreturn
   constrainReturn,             // TR::ddreturn
   constrainReturn,             // TR::dereturn
   constrainCall,               // TR::dfcall
   constrainCall,               // TR::ddcall
   constrainCall,               // TR::decall
   constrainCall,               // TR::idfcall
   constrainCall,               // TR::iddcall
   constrainCall,               // TR::idecall
   constrainChildren,           // TR::dfadd
   constrainChildren,           // TR::ddadd
   constrainChildren,           // TR::deadd
   constrainChildren,           // TR::dfsub
   constrainChildren,           // TR::ddsub
   constrainChildren,           // TR::desub
   constrainChildren,           // TR::dfmul
   constrainChildren,           // TR::ddmul
   constrainChildren,           // TR::demul
   constrainChildren,           // TR::dfdiv
   constrainChildren,           // TR::dddiv
   constrainChildren,           // TR::dediv
   constrainChildren,           // TR::dfrem
   constrainChildren,           // TR::ddrem
   constrainChildren,           // TR::derem
   constrainChildren,           // TR::dfneg
   constrainChildren,           // TR::ddneg
   constrainChildren,           // TR::deneg
   constrainChildren,           // TR::dfabs
   constrainChildren,           // TR::ddabs
   constrainChildren,           // TR::deabs
   constrainChildren,           // TR::dfshl
   constrainChildren,           // TR::dfshr
   constrainChildren,           // TR::ddshl
   constrainChildren,           // TR::ddshr
   constrainChildren,           // TR::deshl
   constrainChildren,           // TR::deshr
   constrainChildren,           // TR::dfshrRounded
   constrainChildren,           // TR::ddshrRounded
   constrainChildren,           // TR::deshrRounded
   constrainChildren,           // TR::dfSetNegative
   constrainChildren,           // TR::ddSetNegative
   constrainChildren,           // TR::deSetNegative
   constrainChildren,           // TR::dfModifyPrecision
   constrainChildren,           // TR::ddModifyPrecision
   constrainChildren,           // TR::deModifyPrecision

   constrainChildren,           // TR::i2df
   constrainChildren,           // TR::iu2df
   constrainChildren,           // TR::l2df
   constrainChildren,           // TR::lu2df
   constrainChildren,           // TR::f2df
   constrainChildren,           // TR::d2df
   constrainChildren,           // TR::dd2df
   constrainChildren,           // TR::de2df
   constrainChildren,           // TR::b2df
   constrainChildren,           // TR::bu2df
   constrainChildren,           // TR::s2df
   constrainChildren,           // TR::su2df

   constrainChildren,           // TR::df2i
   constrainChildren,           // TR::df2iu
   constrainChildren,           // TR::df2l
   constrainChildren,           // TR::df2lu
   constrainChildren,           // TR::df2f
   constrainChildren,           // TR::df2d
   constrainChildren,           // TR::df2dd
   constrainChildren,           // TR::df2de
   constrainChildren,           // TR::df2b
   constrainChildren,           // TR::df2bu
   constrainChildren,           // TR::df2s
   constrainChildren,           // TR::df2c

   constrainChildren,           // TR::i2dd
   constrainChildren,           // TR::iu2dd
   constrainChildren,           // TR::l2dd
   constrainChildren,           // TR::lu2dd
   constrainChildren,           // TR::f2dd
   constrainChildren,           // TR::d2dd
   constrainChildren,           // TR::de2dd
   constrainChildren,           // TR::b2dd
   constrainChildren,           // TR::bu2dd
   constrainChildren,           // TR::s2dd
   constrainChildren,           // TR::su2dd

   constrainChildren,           // TR::dd2i
   constrainChildren,           // TR::dd2iu
   constrainChildren,           // TR::dd2l
   constrainChildren,           // TR::dd2lu
   constrainChildren,           // TR::dd2f
   constrainChildren,           // TR::dd2d
   constrainChildren,           // TR::dd2de
   constrainChildren,           // TR::dd2b
   constrainChildren,           // TR::dd2bu
   constrainChildren,           // TR::dd2s
   constrainChildren,           // TR::dd2c

   constrainChildren,           // TR::i2de
   constrainChildren,           // TR::iu2de
   constrainChildren,           // TR::l2de
   constrainChildren,           // TR::lu2de
   constrainChildren,           // TR::f2de
   constrainChildren,           // TR::d2de
   constrainChildren,           // TR::b2de
   constrainChildren,           // TR::bu2de
   constrainChildren,           // TR::s2de
   constrainChildren,           // TR::su2de

   constrainChildren,           // TR::de2i
   constrainChildren,           // TR::de2iu
   constrainChildren,           // TR::de2l
   constrainChildren,           // TR::de2lu
   constrainChildren,           // TR::de2f
   constrainChildren,           // TR::de2d
   constrainChildren,           // TR::de2b
   constrainChildren,           // TR::de2bu
   constrainChildren,           // TR::de2s
   constrainChildren,           // TR::de2c

   constrainCondBranch,         // TR::ifdfcmpeq
   constrainCondBranch,         // TR::ifdfcmpne
   constrainCondBranch,         // TR::ifdfcmplt
   constrainCondBranch,         // TR::ifdfcmpge
   constrainCondBranch,         // TR::ifdfcmpgt
   constrainCondBranch,         // TR::ifdfcmple
   constrainCondBranch,         // TR::ifdfcmpequ
   constrainCondBranch,         // TR::ifdfcmpneu
   constrainCondBranch,         // TR::ifdfcmpltu
   constrainCondBranch,         // TR::ifdfcmpgeu
   constrainCondBranch,         // TR::ifdfcmpgtu
   constrainCondBranch,         // TR::ifdfcmpleu

   constrainChildren,           // TR::dfcmpeq
   constrainChildren,           // TR::dfcmpne
   constrainChildren,           // TR::dfcmplt
   constrainChildren,           // TR::dfcmpge
   constrainChildren,           // TR::dfcmpgt
   constrainChildren,           // TR::dfcmple
   constrainChildren,           // TR::dfcmpequ
   constrainChildren,           // TR::dfcmpneu
   constrainChildren,           // TR::dfcmpltu
   constrainChildren,           // TR::dfcmpgeu
   constrainChildren,           // TR::dfcmpgtu
   constrainChildren,           // TR::dfcmpleu

   constrainCondBranch,         // TR::ifddcmpeq
   constrainCondBranch,         // TR::ifddcmpne
   constrainCondBranch,         // TR::ifddcmplt
   constrainCondBranch,         // TR::ifddcmpge
   constrainCondBranch,         // TR::ifddcmpgt
   constrainCondBranch,         // TR::ifddcmple
   constrainCondBranch,         // TR::ifddcmpequ
   constrainCondBranch,         // TR::ifddcmpneu
   constrainCondBranch,         // TR::ifddcmpltu
   constrainCondBranch,         // TR::ifddcmpgeu
   constrainCondBranch,         // TR::ifddcmpgtu
   constrainCondBranch,         // TR::ifddcmpleu

   constrainChildren,           // TR::ddcmpeq
   constrainChildren,           // TR::ddcmpne
   constrainChildren,           // TR::ddcmplt
   constrainChildren,           // TR::ddcmpge
   constrainChildren,           // TR::ddcmpgt
   constrainChildren,           // TR::ddcmple
   constrainChildren,           // TR::ddcmpequ
   constrainChildren,           // TR::ddcmpneu
   constrainChildren,           // TR::ddcmpltu
   constrainChildren,           // TR::ddcmpgeu
   constrainChildren,           // TR::ddcmpgtu
   constrainChildren,           // TR::ddcmpleu

   constrainCondBranch,         // TR::ifdecmpeq
   constrainCondBranch,         // TR::ifdecmpne
   constrainCondBranch,         // TR::ifdecmplt
   constrainCondBranch,         // TR::ifdecmpge
   constrainCondBranch,         // TR::ifdecmpgt
   constrainCondBranch,         // TR::ifdecmple
   constrainCondBranch,         // TR::ifdecmpequ
   constrainCondBranch,         // TR::ifdecmpneu
   constrainCondBranch,         // TR::ifdecmpltu
   constrainCondBranch,         // TR::ifdecmpgeu
   constrainCondBranch,         // TR::ifdecmpgtu
   constrainCondBranch,         // TR::ifdecmpleu

   constrainChildren,           // TR::decmpeq
   constrainChildren,           // TR::decmpne
   constrainChildren,           // TR::decmplt
   constrainChildren,           // TR::decmpge
   constrainChildren,           // TR::decmpgt
   constrainChildren,           // TR::decmple
   constrainChildren,           // TR::decmpequ
   constrainChildren,           // TR::decmpneu
   constrainChildren,           // TR::decmpltu
   constrainChildren,           // TR::decmpgeu
   constrainChildren,           // TR::decmpgtu
   constrainChildren,           // TR::decmpleu

   constrainChildren,           // TR::dfRegLoad
   constrainChildren,           // TR::ddRegLoad
   constrainChildren,           // TR::deRegLoad
   constrainChildren,           // TR::dfRegStore
   constrainChildren,           // TR::ddRegStore
   constrainChildren,           // TR::deRegStore

   constrainChildrenFirstToLast,        // TR::dfselect
   constrainChildrenFirstToLast,        // TR::ddselect
   constrainChildrenFirstToLast,        // TR::deselect

   constrainChildren,           // TR::dfexp
   constrainChildren,           // TR::ddexp
   constrainChildren,           // TR::deexp
   constrainChildren,           // TR::dfnint
   constrainChildren,           // TR::ddnint
   constrainChildren,           // TR::denint
   constrainChildren,           // TR::dfsqrt
   constrainChildren,           // TR::ddsqrt
   constrainChildren,           // TR::desqrt

   constrainChildren,           // TR::dfcos
   constrainChildren,           // TR::ddcos
   constrainChildren,           // TR::decos
   constrainChildren,           // TR::dfsin
   constrainChildren,           // TR::ddsin
   constrainChildren,           // TR::desin
   constrainChildren,           // TR::dftan
   constrainChildren,           // TR::ddtan
   constrainChildren,           // TR::detan

   constrainChildren,           // TR::dfcosh
   constrainChildren,           // TR::ddcosh
   constrainChildren,           // TR::decosh
   constrainChildren,           // TR::dfsinh
   constrainChildren,           // TR::ddsinh
   constrainChildren,           // TR::desinh
   constrainChildren,           // TR::dftanh
   constrainChildren,           // TR::ddtanh
   constrainChildren,           // TR::detanh

   constrainChildren,           // TR::dfacos
   constrainChildren,           // TR::ddacos
   constrainChildren,           // TR::deacos
   constrainChildren,           // TR::dfasin
   constrainChildren,           // TR::ddasin
   constrainChildren,           // TR::deasin
   constrainChildren,           // TR::dfatan
   constrainChildren,           // TR::ddatan
   constrainChildren,           // TR::deatan

   constrainChildren,           // TR::dfatan2
   constrainChildren,           // TR::ddatan2
   constrainChildren,           // TR::deatan2
   constrainChildren,           // TR::dflog
   constrainChildren,           // TR::ddlog
   constrainChildren,           // TR::delog
   constrainChildren,           // TR::dffloor
   constrainChildren,           // TR::ddfloor
   constrainChildren,           // TR::defloor
   constrainChildren,           // TR::dfceil
   constrainChildren,           // TR::ddceil
   constrainChildren,           // TR::deceil
   constrainChildren,           // TR::dfmax
   constrainChildren,           // TR::ddmax
   constrainChildren,           // TR::demax
   constrainChildren,           // TR::dfmin
   constrainChildren,           // TR::ddmin
   constrainChildren,           // TR::demin

   constrainChildren,           // TR::dfInsExp
   constrainChildren,           // TR::ddInsExp
   constrainChildren,           // TR::deInsExp

   constrainChildren,           // TR::ddclean
   constrainChildren,           // TR::declean

   constrainBCDAggrLoad,        // TR::zdload
   constrainBCDAggrLoad,        // TR::zdloadi
   constrainStore,              // TR::zdstore
   constrainStore,              // TR::zdstorei

   constrainChildren,           // TR::pd2zd
   constrainChildren,           // TR::zd2pd

   constrainBCDAggrLoad,        // TR::zdsleLoad
   constrainBCDAggrLoad,        // TR::zdslsLoad
   constrainBCDAggrLoad,        // TR::zdstsLoad

   constrainBCDAggrLoad,        // TR::zdsleLoadi
   constrainBCDAggrLoad,        // TR::zdslsLoadi
   constrainBCDAggrLoad,        // TR::zdstsLoadi

   constrainStore,              // TR::zdsleStore
   constrainStore,              // TR::zdslsStore
   constrainStore,              // TR::zdstsStore

   constrainStore,              // TR::zdsleStorei
   constrainStore,              // TR::zdslsStorei
   constrainStore,              // TR::zdstsStorei

   constrainChildren,           // TR::zd2zdsle
   constrainChildren,           // TR::zd2zdsls
   constrainChildren,           // TR::zd2zdsts

   constrainChildren,           // TR::zdsle2pd
   constrainChildren,           // TR::zdsls2pd
   constrainChildren,           // TR::zdsts2pd

   constrainChildren,           // TR::zdsle2zd
   constrainChildren,           // TR::zdsls2zd
   constrainChildren,           // TR::zdsts2zd

   constrainChildren,           // TR::pd2zdsls
   constrainChildren,           // TR::pd2zdslsSetSign
   constrainChildren,           // TR::pd2zdsts
   constrainChildren,           // TR::pd2zdstsSetSign

   constrainChildren,           // TR::zd2df
   constrainChildren,           // TR::df2zd
   constrainChildren,           // TR::zd2dd
   constrainChildren,           // TR::dd2zd
   constrainChildren,           // TR::zd2de
   constrainChildren,           // TR::de2zd

   constrainChildren,           // TR::zd2dfAbs
   constrainChildren,           // TR::zd2ddAbs
   constrainChildren,           // TR::zd2deAbs

   constrainChildren,           // TR::df2zdSetSign
   constrainChildren,           // TR::dd2zdSetSign
   constrainChildren,           // TR::de2zdSetSign

   constrainChildren,           // TR::df2zdClean
   constrainChildren,           // TR::dd2zdClean
   constrainChildren,           // TR::de2zdClean

   constrainBCDAggrLoad,        // TR::udLoad
   constrainBCDAggrLoad,        // TR::udslLoad
   constrainBCDAggrLoad,        // TR::udstLoad

   constrainBCDAggrLoad,        // TR::udLoadi
   constrainBCDAggrLoad,        // TR::udslLoadi
   constrainBCDAggrLoad,        // TR::udstLoadi

   constrainStore,              // TR::udStore
   constrainStore,              // TR::udslStore
   constrainStore,              // TR::udstStore

   constrainStore,              // TR::udStorei
   constrainStore,              // TR::udslStorei
   constrainStore,              // TR::udstStorei

   constrainChildren,           // TR::pd2ud
   constrainChildren,           // TR::pd2udsl
   constrainChildren,           // TR::pd2udst

   constrainChildren,           // TR::udsl2ud
   constrainChildren,           // TR::udst2ud

   constrainChildren,           // TR::ud2pd
   constrainChildren,           // TR::udsl2pd
   constrainChildren,           // TR::udst2pd

   constrainBCDAggrLoad,        // TR::pdload
   constrainBCDAggrLoad,        // TR::pdloadi
   constrainStore,              // TR::pdstore
   constrainStore,              // TR::pdstorei
   constrainChildren,           // TR::pdadd
   constrainChildren,           // TR::pdsub
   constrainChildren,           // TR::pdmul
   constrainChildren,           // TR::pddiv
   constrainChildren,           // TR::pdrem
   constrainChildren,           // TR::pdneg
   constrainChildren,           // TR::pdabs
   constrainChildren,           // TR::pdshr
   constrainChildren,           // TR::pdshl

   constrainChildren,           // TR::pdshrSetSign
   constrainChildren,           // TR::pdshlSetSign
   constrainChildren,           // TR::pdshlOverflow
   constrainChildren,           // TR::pdchk
   constrainBCDToIntegral,      // TR::pd2i
   constrainChildren,           // TR::pd2iOverflow
   constrainBCDToIntegral,      // TR::pd2iu
   constrainIntegralToBCD,      // TR::i2pd
   constrainIntegralToBCD,      // TR::iu2pd
   constrainBCDToIntegral,      // TR::pd2l
   constrainChildren,           // TR::pd2lOverflow
   constrainBCDToIntegral,      // TR::pd2lu
   constrainIntegralToBCD,      // TR::l2pd
   constrainIntegralToBCD,      // TR::lu2pd
   constrainChildren,           // TR::pd2f
   constrainChildren,           // TR::pd2d
   constrainChildren,           // TR::f2pd
   constrainChildren,           // TR::d2pd

   constrainChildren,           // TR::pdcmpeq
   constrainChildren,           // TR::pdcmpne
   constrainChildren,           // TR::pdcmplt
   constrainChildren,           // TR::pdcmpge
   constrainChildren,           // TR::pdcmpgt
   constrainChildren,           // TR::pdcmple

   constrainChildren,           // TR::pdclean
   constrainChildren,           // TR::pdclear
   constrainChildren,           // TR::pdclearSetSign

   constrainChildren,           // TR::pdSetSign

   constrainChildren,           // TR::pdModifyPrecision

   constrainChildren,           // TR::countDigits

   constrainChildren,           // TR::pd2df
   constrainChildren,           // TR::pd2dfAbs
   constrainChildren,           // TR::df2pd
   constrainChildren,           // TR::df2pdSetSign
   constrainChildren,           // TR::df2pdClean
   constrainChildren,           // TR::pd2dd
   constrainChildren,           // TR::pd2ddAbs
   constrainChildren,           // TR::dd2pd
   constrainChildren,           // TR::dd2pdSetSign
   constrainChildren,           // TR::dd2pdClean
   constrainChildren,           // TR::pd2de
   constrainChildren,           // TR::pd2deAbs
   constrainChildren,           // TR::de2pd
   constrainChildren,           // TR::de2pdSetSign
   constrainChildren,           // TR::de2pdClean
   constrainBCDCHK,          // TR::BCDCHK
#endif

   };

#endif

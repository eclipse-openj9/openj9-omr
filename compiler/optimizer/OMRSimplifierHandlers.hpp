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

#ifndef OMR_SIMPLIFIERHANDLERS_INCL
#define OMR_SIMPLIFIERHANDLERS_INCL

TR::Node * dftSimplifier(TR::Node *node, TR::Block *block, TR::Simplifier *s);
TR::Node * lstoreSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * directLoadSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * indirectLoadSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * indirectStoreSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * directStoreSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * astoreSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * gotoSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifdCallSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcallSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * acallSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * vcallSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * treetopSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * anchorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iaddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * laddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * faddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * daddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * baddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * saddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * isubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lsubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fsubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dsubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bsubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ssubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * imulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lmulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fmulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dmulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bmulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * smulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * idivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ldivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fdivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ddivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bdivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sdivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * inegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lnegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fnegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dnegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bnegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * snegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * constSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lconstSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ilfdabsSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ishlSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lshlSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bshlSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sshlSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ishrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lshrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bshrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sshrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iushrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lushrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bushrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sushrSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * irolSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lrolSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iandSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * landSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bandSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sandSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * borSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ixorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lxorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bxorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sxorSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fsqrtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dsqrtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2bSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2aSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iu2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iu2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iu2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2bSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2aSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lu2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lu2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2bSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2bSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * b2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * b2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * b2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * b2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * b2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bu2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bu2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bu2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bu2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bu2sSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * s2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * s2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * s2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * s2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * s2bSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * su2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * su2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * su2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * su2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ificmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iflcmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * normalizeCmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifacmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifacmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifCmpWithEqualitySimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifCmpWithoutEqualitySimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lucmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lucmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lucmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lucmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * acmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * acmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpeqSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpneSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sucmpltSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sucmpgeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sucmpgtSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sucmpleSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lcmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * passThroughSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * endBlockSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * selectSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * a2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * a2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * v2vSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * vsetelemSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * checkcastSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * checkcastAndNULLCHKSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * variableNewSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * caddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * csubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * imulhSimplifier(TR::Node * node, TR::Block *block, TR::Simplifier * s);
TR::Node * lmulhSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2cSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2cSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * expSimplifier(TR::Node *node,TR::Block *block,TR::Simplifier *s);
TR::Node * ibits2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lbits2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fbits2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dbits2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifxcmpoSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * ifxcmnoSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lookupSwitchSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * tableSwitchSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * nullchkSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * divchkSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bndchkSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * arraycopybndchkSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bndchkwithspinechkSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bcmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bucmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * scmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * sucmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * icmpSimplifier(TR::Node *node, TR::Block * block, TR::Simplifier * s);
TR::Node * iucmpSimplifier(TR::Node *node, TR::Block * block, TR::Simplifier * s);
TR::Node * lucmpSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * imaxminSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lmaxminSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * fmaxminSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * dmaxminSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * emaxminSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * byteswapSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * computeCCSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * arraysetSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bitOpMemSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * bitOpMemNDSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * arrayCmpWithPadSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * eaddSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * esubSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * enegSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * emulSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * eremSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * edivSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * i2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * iu2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * l2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * f2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * d2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * c2eSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * e2iSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * e2lSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * e2fSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * e2dSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * NewSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * lowerTreeSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);
TR::Node * arrayLengthSimplifier(TR::Node * node, TR::Block * block, TR::Simplifier * s);


TR::Node * removeArithmeticsUnderIntegralCompare(TR::Node* node,TR::Simplifier * s);


/*
 * One-to-one mapping between opcodes and their simplifier handlers.
 */

#define BadILOpSimplifierHandler dftSimplifier
#define aconstSimplifierHandler dftSimplifier
#define iconstSimplifierHandler constSimplifier
#define lconstSimplifierHandler lconstSimplifier
#define fconstSimplifierHandler dftSimplifier
#define dconstSimplifierHandler dftSimplifier
#define bconstSimplifierHandler constSimplifier
#define sconstSimplifierHandler constSimplifier
#define iloadSimplifierHandler directLoadSimplifier
#define floadSimplifierHandler directLoadSimplifier
#define dloadSimplifierHandler directLoadSimplifier
#define aloadSimplifierHandler directLoadSimplifier
#define bloadSimplifierHandler directLoadSimplifier
#define sloadSimplifierHandler directLoadSimplifier
#define lloadSimplifierHandler directLoadSimplifier
#define irdbarSimplifierHandler directLoadSimplifier
#define frdbarSimplifierHandler directLoadSimplifier
#define drdbarSimplifierHandler directLoadSimplifier
#define ardbarSimplifierHandler directLoadSimplifier
#define brdbarSimplifierHandler directLoadSimplifier
#define srdbarSimplifierHandler directLoadSimplifier
#define lrdbarSimplifierHandler directLoadSimplifier
#define iloadiSimplifierHandler indirectLoadSimplifier
#define floadiSimplifierHandler indirectLoadSimplifier
#define dloadiSimplifierHandler indirectLoadSimplifier
#define aloadiSimplifierHandler indirectLoadSimplifier
#define bloadiSimplifierHandler indirectLoadSimplifier
#define sloadiSimplifierHandler indirectLoadSimplifier
#define lloadiSimplifierHandler indirectLoadSimplifier
#define irdbariSimplifierHandler indirectLoadSimplifier
#define frdbariSimplifierHandler indirectLoadSimplifier
#define drdbariSimplifierHandler indirectLoadSimplifier
#define ardbariSimplifierHandler indirectLoadSimplifier
#define brdbariSimplifierHandler indirectLoadSimplifier
#define srdbariSimplifierHandler indirectLoadSimplifier
#define lrdbariSimplifierHandler indirectLoadSimplifier
#define istoreSimplifierHandler directStoreSimplifier
#define lstoreSimplifierHandler lstoreSimplifier
#define fstoreSimplifierHandler dftSimplifier
#define dstoreSimplifierHandler dftSimplifier
#define astoreSimplifierHandler astoreSimplifier
#define bstoreSimplifierHandler dftSimplifier
#define sstoreSimplifierHandler dftSimplifier
#define iwrtbarSimplifierHandler directStoreSimplifier
#define lwrtbarSimplifierHandler lstoreSimplifier
#define fwrtbarSimplifierHandler dftSimplifier
#define dwrtbarSimplifierHandler dftSimplifier
#define awrtbarSimplifierHandler dftSimplifier
#define bwrtbarSimplifierHandler dftSimplifier
#define swrtbarSimplifierHandler dftSimplifier
#define lstoreiSimplifierHandler indirectStoreSimplifier
#define fstoreiSimplifierHandler indirectStoreSimplifier
#define dstoreiSimplifierHandler indirectStoreSimplifier
#define astoreiSimplifierHandler indirectStoreSimplifier
#define bstoreiSimplifierHandler indirectStoreSimplifier
#define sstoreiSimplifierHandler indirectStoreSimplifier
#define istoreiSimplifierHandler indirectStoreSimplifier
#define lwrtbariSimplifierHandler indirectStoreSimplifier
#define fwrtbariSimplifierHandler indirectStoreSimplifier
#define dwrtbariSimplifierHandler indirectStoreSimplifier
#define awrtbariSimplifierHandler indirectStoreSimplifier
#define bwrtbariSimplifierHandler indirectStoreSimplifier
#define swrtbariSimplifierHandler indirectStoreSimplifier
#define iwrtbariSimplifierHandler indirectStoreSimplifier
#define GotoSimplifierHandler gotoSimplifier
#define ireturnSimplifierHandler dftSimplifier
#define lreturnSimplifierHandler dftSimplifier
#define freturnSimplifierHandler dftSimplifier
#define dreturnSimplifierHandler dftSimplifier
#define areturnSimplifierHandler dftSimplifier
#define ReturnSimplifierHandler dftSimplifier
#define asynccheckSimplifierHandler dftSimplifier
#define athrowSimplifierHandler dftSimplifier
#define icallSimplifierHandler ifdCallSimplifier
#define lcallSimplifierHandler lcallSimplifier
#define fcallSimplifierHandler ifdCallSimplifier
#define dcallSimplifierHandler ifdCallSimplifier
#define acallSimplifierHandler acallSimplifier
#define callSimplifierHandler vcallSimplifier
#define iaddSimplifierHandler iaddSimplifier
#define laddSimplifierHandler laddSimplifier
#define faddSimplifierHandler faddSimplifier
#define daddSimplifierHandler daddSimplifier
#define baddSimplifierHandler baddSimplifier
#define saddSimplifierHandler saddSimplifier
#define isubSimplifierHandler isubSimplifier
#define lsubSimplifierHandler lsubSimplifier
#define fsubSimplifierHandler fsubSimplifier
#define dsubSimplifierHandler dsubSimplifier
#define bsubSimplifierHandler bsubSimplifier
#define ssubSimplifierHandler ssubSimplifier
#define asubSimplifierHandler dftSimplifier
#define imulSimplifierHandler imulSimplifier
#define lmulSimplifierHandler lmulSimplifier
#define fmulSimplifierHandler fmulSimplifier
#define dmulSimplifierHandler dmulSimplifier
#define bmulSimplifierHandler bmulSimplifier
#define smulSimplifierHandler smulSimplifier
#define idivSimplifierHandler idivSimplifier
#define ldivSimplifierHandler ldivSimplifier
#define fdivSimplifierHandler fdivSimplifier
#define ddivSimplifierHandler ddivSimplifier
#define bdivSimplifierHandler bdivSimplifier
#define sdivSimplifierHandler sdivSimplifier
#define iudivSimplifierHandler idivSimplifier
#define ludivSimplifierHandler ldivSimplifier
#define iremSimplifierHandler iremSimplifier
#define lremSimplifierHandler lremSimplifier
#define fremSimplifierHandler fremSimplifier
#define dremSimplifierHandler dremSimplifier
#define bremSimplifierHandler bremSimplifier
#define sremSimplifierHandler sremSimplifier
#define iuremSimplifierHandler iremSimplifier
#define inegSimplifierHandler inegSimplifier
#define lnegSimplifierHandler lnegSimplifier
#define fnegSimplifierHandler fnegSimplifier
#define dnegSimplifierHandler dnegSimplifier
#define bnegSimplifierHandler bnegSimplifier
#define snegSimplifierHandler snegSimplifier
#define iabsSimplifierHandler ilfdabsSimplifier
#define labsSimplifierHandler ilfdabsSimplifier
#define fabsSimplifierHandler ilfdabsSimplifier
#define dabsSimplifierHandler ilfdabsSimplifier
#define ishlSimplifierHandler ishlSimplifier
#define lshlSimplifierHandler lshlSimplifier
#define bshlSimplifierHandler bshlSimplifier
#define sshlSimplifierHandler sshlSimplifier
#define ishrSimplifierHandler ishrSimplifier
#define lshrSimplifierHandler lshrSimplifier
#define bshrSimplifierHandler bshrSimplifier
#define sshrSimplifierHandler sshrSimplifier
#define iushrSimplifierHandler iushrSimplifier
#define lushrSimplifierHandler lushrSimplifier
#define bushrSimplifierHandler bushrSimplifier
#define sushrSimplifierHandler sushrSimplifier
#define irolSimplifierHandler irolSimplifier
#define lrolSimplifierHandler lrolSimplifier
#define iandSimplifierHandler iandSimplifier
#define landSimplifierHandler landSimplifier
#define bandSimplifierHandler bandSimplifier
#define sandSimplifierHandler sandSimplifier
#define iorSimplifierHandler iorSimplifier
#define lorSimplifierHandler lorSimplifier
#define borSimplifierHandler borSimplifier
#define sorSimplifierHandler sorSimplifier
#define ixorSimplifierHandler ixorSimplifier
#define lxorSimplifierHandler lxorSimplifier
#define bxorSimplifierHandler bxorSimplifier
#define sxorSimplifierHandler sxorSimplifier
#define i2lSimplifierHandler i2lSimplifier
#define i2fSimplifierHandler i2fSimplifier
#define i2dSimplifierHandler i2dSimplifier
#define i2bSimplifierHandler i2bSimplifier
#define i2sSimplifierHandler i2sSimplifier
#define i2aSimplifierHandler i2aSimplifier
#define iu2lSimplifierHandler iu2lSimplifier
#define iu2fSimplifierHandler iu2fSimplifier
#define iu2dSimplifierHandler iu2dSimplifier
#define iu2aSimplifierHandler i2aSimplifier
#define l2iSimplifierHandler l2iSimplifier
#define l2fSimplifierHandler l2fSimplifier
#define l2dSimplifierHandler l2dSimplifier
#define l2bSimplifierHandler l2bSimplifier
#define l2sSimplifierHandler l2sSimplifier
#define l2aSimplifierHandler l2aSimplifier
#define lu2fSimplifierHandler lu2fSimplifier
#define lu2dSimplifierHandler lu2dSimplifier
#define lu2aSimplifierHandler l2aSimplifier
#define f2iSimplifierHandler f2iSimplifier
#define f2lSimplifierHandler f2lSimplifier
#define f2dSimplifierHandler f2dSimplifier
#define f2bSimplifierHandler f2bSimplifier
#define f2sSimplifierHandler f2sSimplifier
#define d2iSimplifierHandler d2iSimplifier
#define d2lSimplifierHandler d2lSimplifier
#define d2fSimplifierHandler d2fSimplifier
#define d2bSimplifierHandler d2bSimplifier
#define d2sSimplifierHandler d2sSimplifier
#define b2iSimplifierHandler b2iSimplifier
#define b2lSimplifierHandler b2lSimplifier
#define b2fSimplifierHandler b2fSimplifier
#define b2dSimplifierHandler b2dSimplifier
#define b2sSimplifierHandler b2sSimplifier
#define b2aSimplifierHandler dftSimplifier
#define bu2iSimplifierHandler bu2iSimplifier
#define bu2lSimplifierHandler bu2lSimplifier
#define bu2fSimplifierHandler bu2fSimplifier
#define bu2dSimplifierHandler bu2dSimplifier
#define bu2sSimplifierHandler bu2sSimplifier
#define bu2aSimplifierHandler dftSimplifier
#define s2iSimplifierHandler s2iSimplifier
#define s2lSimplifierHandler s2lSimplifier
#define s2fSimplifierHandler s2fSimplifier
#define s2dSimplifierHandler s2dSimplifier
#define s2bSimplifierHandler s2bSimplifier
#define s2aSimplifierHandler dftSimplifier
#define su2iSimplifierHandler su2iSimplifier
#define su2lSimplifierHandler su2lSimplifier
#define su2fSimplifierHandler su2fSimplifier
#define su2dSimplifierHandler su2dSimplifier
#define su2aSimplifierHandler dftSimplifier
#define a2iSimplifierHandler a2iSimplifier
#define a2lSimplifierHandler a2lSimplifier
#define a2bSimplifierHandler dftSimplifier
#define a2sSimplifierHandler dftSimplifier
#define icmpeqSimplifierHandler icmpeqSimplifier
#define icmpneSimplifierHandler icmpneSimplifier
#define icmpltSimplifierHandler icmpltSimplifier
#define icmpgeSimplifierHandler icmpgeSimplifier
#define icmpgtSimplifierHandler icmpgtSimplifier
#define icmpleSimplifierHandler icmpleSimplifier
#define iucmpltSimplifierHandler dftSimplifier
#define iucmpgeSimplifierHandler dftSimplifier
#define iucmpgtSimplifierHandler dftSimplifier
#define iucmpleSimplifierHandler dftSimplifier
#define lcmpeqSimplifierHandler lcmpeqSimplifier
#define lcmpneSimplifierHandler lcmpneSimplifier
#define lcmpltSimplifierHandler lcmpltSimplifier
#define lcmpgeSimplifierHandler lcmpgeSimplifier
#define lcmpgtSimplifierHandler lcmpgtSimplifier
#define lcmpleSimplifierHandler lcmpleSimplifier
#define lucmpltSimplifierHandler lucmpltSimplifier
#define lucmpgeSimplifierHandler lucmpgeSimplifier
#define lucmpgtSimplifierHandler lucmpgtSimplifier
#define lucmpleSimplifierHandler lucmpleSimplifier
#define fcmpeqSimplifierHandler normalizeCmpSimplifier
#define fcmpneSimplifierHandler normalizeCmpSimplifier
#define fcmpltSimplifierHandler normalizeCmpSimplifier
#define fcmpgeSimplifierHandler normalizeCmpSimplifier
#define fcmpgtSimplifierHandler normalizeCmpSimplifier
#define fcmpleSimplifierHandler normalizeCmpSimplifier
#define fcmpequSimplifierHandler normalizeCmpSimplifier
#define fcmpneuSimplifierHandler normalizeCmpSimplifier
#define fcmpltuSimplifierHandler normalizeCmpSimplifier
#define fcmpgeuSimplifierHandler normalizeCmpSimplifier
#define fcmpgtuSimplifierHandler normalizeCmpSimplifier
#define fcmpleuSimplifierHandler normalizeCmpSimplifier
#define dcmpeqSimplifierHandler normalizeCmpSimplifier
#define dcmpneSimplifierHandler normalizeCmpSimplifier
#define dcmpltSimplifierHandler normalizeCmpSimplifier
#define dcmpgeSimplifierHandler normalizeCmpSimplifier
#define dcmpgtSimplifierHandler normalizeCmpSimplifier
#define dcmpleSimplifierHandler normalizeCmpSimplifier
#define dcmpequSimplifierHandler normalizeCmpSimplifier
#define dcmpneuSimplifierHandler normalizeCmpSimplifier
#define dcmpltuSimplifierHandler normalizeCmpSimplifier
#define dcmpgeuSimplifierHandler normalizeCmpSimplifier
#define dcmpgtuSimplifierHandler normalizeCmpSimplifier
#define dcmpleuSimplifierHandler normalizeCmpSimplifier
#define acmpeqSimplifierHandler acmpeqSimplifier
#define acmpneSimplifierHandler acmpneSimplifier
#define acmpltSimplifierHandler dftSimplifier
#define acmpgeSimplifierHandler dftSimplifier
#define acmpgtSimplifierHandler dftSimplifier
#define acmpleSimplifierHandler dftSimplifier
#define bcmpeqSimplifierHandler bcmpeqSimplifier
#define bcmpneSimplifierHandler bcmpneSimplifier
#define bcmpltSimplifierHandler bcmpltSimplifier
#define bcmpgeSimplifierHandler bcmpgeSimplifier
#define bcmpgtSimplifierHandler bcmpgtSimplifier
#define bcmpleSimplifierHandler bcmpleSimplifier
#define bucmpltSimplifierHandler bcmpltSimplifier
#define bucmpgeSimplifierHandler bcmpgeSimplifier
#define bucmpgtSimplifierHandler bcmpgtSimplifier
#define bucmpleSimplifierHandler bcmpleSimplifier
#define scmpeqSimplifierHandler scmpeqSimplifier
#define scmpneSimplifierHandler scmpneSimplifier
#define scmpltSimplifierHandler scmpltSimplifier
#define scmpgeSimplifierHandler scmpgeSimplifier
#define scmpgtSimplifierHandler scmpgtSimplifier
#define scmpleSimplifierHandler scmpleSimplifier
#define sucmpltSimplifierHandler sucmpltSimplifier
#define sucmpgeSimplifierHandler sucmpgeSimplifier
#define sucmpgtSimplifierHandler sucmpgtSimplifier
#define sucmpleSimplifierHandler sucmpleSimplifier
#define lcmpSimplifierHandler lcmpSimplifier
#define fcmplSimplifierHandler dftSimplifier
#define fcmpgSimplifierHandler dftSimplifier
#define dcmplSimplifierHandler dftSimplifier
#define dcmpgSimplifierHandler dftSimplifier
#define ificmpeqSimplifierHandler ificmpeqSimplifier
#define ificmpneSimplifierHandler ificmpneSimplifier
#define ificmpltSimplifierHandler ificmpltSimplifier
#define ificmpgeSimplifierHandler ificmpgeSimplifier
#define ificmpgtSimplifierHandler ificmpgtSimplifier
#define ificmpleSimplifierHandler ificmpleSimplifier
#define ifiucmpltSimplifierHandler ificmpltSimplifier
#define ifiucmpgeSimplifierHandler ificmpgeSimplifier
#define ifiucmpgtSimplifierHandler ificmpgtSimplifier
#define ifiucmpleSimplifierHandler ificmpleSimplifier
#define iflcmpeqSimplifierHandler iflcmpeqSimplifier
#define iflcmpneSimplifierHandler iflcmpneSimplifier
#define iflcmpltSimplifierHandler iflcmpltSimplifier
#define iflcmpgeSimplifierHandler iflcmpgeSimplifier
#define iflcmpgtSimplifierHandler iflcmpgtSimplifier
#define iflcmpleSimplifierHandler iflcmpleSimplifier
#define iflucmpltSimplifierHandler iflcmpltSimplifier
#define iflucmpgeSimplifierHandler iflcmpgeSimplifier
#define iflucmpgtSimplifierHandler iflcmpgtSimplifier
#define iflucmpleSimplifierHandler iflcmpleSimplifier
#define iffcmpeqSimplifierHandler normalizeCmpSimplifier
#define iffcmpneSimplifierHandler normalizeCmpSimplifier
#define iffcmpltSimplifierHandler normalizeCmpSimplifier
#define iffcmpgeSimplifierHandler normalizeCmpSimplifier
#define iffcmpgtSimplifierHandler normalizeCmpSimplifier
#define iffcmpleSimplifierHandler normalizeCmpSimplifier
#define iffcmpequSimplifierHandler normalizeCmpSimplifier
#define iffcmpneuSimplifierHandler normalizeCmpSimplifier
#define iffcmpltuSimplifierHandler normalizeCmpSimplifier
#define iffcmpgeuSimplifierHandler normalizeCmpSimplifier
#define iffcmpgtuSimplifierHandler normalizeCmpSimplifier
#define iffcmpleuSimplifierHandler normalizeCmpSimplifier
#define ifdcmpeqSimplifierHandler normalizeCmpSimplifier
#define ifdcmpneSimplifierHandler normalizeCmpSimplifier
#define ifdcmpltSimplifierHandler normalizeCmpSimplifier
#define ifdcmpgeSimplifierHandler normalizeCmpSimplifier
#define ifdcmpgtSimplifierHandler normalizeCmpSimplifier
#define ifdcmpleSimplifierHandler normalizeCmpSimplifier
#define ifdcmpequSimplifierHandler normalizeCmpSimplifier
#define ifdcmpneuSimplifierHandler normalizeCmpSimplifier
#define ifdcmpltuSimplifierHandler normalizeCmpSimplifier
#define ifdcmpgeuSimplifierHandler normalizeCmpSimplifier
#define ifdcmpgtuSimplifierHandler normalizeCmpSimplifier
#define ifdcmpleuSimplifierHandler normalizeCmpSimplifier
#define ifacmpeqSimplifierHandler ifacmpeqSimplifier
#define ifacmpneSimplifierHandler ifacmpneSimplifier
#define ifacmpltSimplifierHandler dftSimplifier
#define ifacmpgeSimplifierHandler dftSimplifier
#define ifacmpgtSimplifierHandler dftSimplifier
#define ifacmpleSimplifierHandler dftSimplifier
#define ifbcmpeqSimplifierHandler ifCmpWithEqualitySimplifier
#define ifbcmpneSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifbcmpltSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifbcmpgeSimplifierHandler ifCmpWithEqualitySimplifier
#define ifbcmpgtSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifbcmpleSimplifierHandler ifCmpWithEqualitySimplifier
#define ifbucmpltSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifbucmpgeSimplifierHandler ifCmpWithEqualitySimplifier
#define ifbucmpgtSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifbucmpleSimplifierHandler ifCmpWithEqualitySimplifier
#define ifscmpeqSimplifierHandler ifCmpWithEqualitySimplifier
#define ifscmpneSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifscmpltSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifscmpgeSimplifierHandler ifCmpWithEqualitySimplifier
#define ifscmpgtSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifscmpleSimplifierHandler ifCmpWithEqualitySimplifier
#define ifsucmpltSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifsucmpgeSimplifierHandler ifCmpWithEqualitySimplifier
#define ifsucmpgtSimplifierHandler ifCmpWithoutEqualitySimplifier
#define ifsucmpleSimplifierHandler ifCmpWithEqualitySimplifier
#define loadaddrSimplifierHandler dftSimplifier
#define ZEROCHKSimplifierHandler lowerTreeSimplifier
#define callIfSimplifierHandler lowerTreeSimplifier
#define iRegLoadSimplifierHandler dftSimplifier
#define aRegLoadSimplifierHandler dftSimplifier
#define lRegLoadSimplifierHandler dftSimplifier
#define fRegLoadSimplifierHandler dftSimplifier
#define dRegLoadSimplifierHandler dftSimplifier
#define sRegLoadSimplifierHandler dftSimplifier
#define bRegLoadSimplifierHandler dftSimplifier
#define iRegStoreSimplifierHandler dftSimplifier
#define aRegStoreSimplifierHandler dftSimplifier
#define lRegStoreSimplifierHandler dftSimplifier
#define fRegStoreSimplifierHandler dftSimplifier
#define dRegStoreSimplifierHandler dftSimplifier
#define sRegStoreSimplifierHandler dftSimplifier
#define bRegStoreSimplifierHandler dftSimplifier
#define GlRegDepsSimplifierHandler dftSimplifier
#define iselectSimplifierHandler selectSimplifier
#define lselectSimplifierHandler selectSimplifier
#define bselectSimplifierHandler selectSimplifier
#define sselectSimplifierHandler selectSimplifier
#define aselectSimplifierHandler selectSimplifier
#define fselectSimplifierHandler selectSimplifier
#define dselectSimplifierHandler selectSimplifier
#define treetopSimplifierHandler treetopSimplifier
#define MethodEnterHookSimplifierHandler lowerTreeSimplifier
#define MethodExitHookSimplifierHandler lowerTreeSimplifier
#define PassThroughSimplifierHandler passThroughSimplifier
#define compressedRefsSimplifierHandler anchorSimplifier
#define BBStartSimplifierHandler dftSimplifier
#define BBEndSimplifierHandler endBlockSimplifier
#define viremSimplifierHandler dftSimplifier
#define viminSimplifierHandler dftSimplifier
#define vimaxSimplifierHandler dftSimplifier
#define vigetelemSimplifierHandler dftSimplifier
#define visetelemSimplifierHandler dftSimplifier
#define vimergelSimplifierHandler dftSimplifier
#define vimergehSimplifierHandler dftSimplifier
#define vicmpeqSimplifierHandler dftSimplifier
#define vicmpgtSimplifierHandler dftSimplifier
#define vicmpgeSimplifierHandler dftSimplifier
#define vicmpltSimplifierHandler dftSimplifier
#define vicmpleSimplifierHandler dftSimplifier
#define vicmpalleqSimplifierHandler dftSimplifier
#define vicmpallneSimplifierHandler dftSimplifier
#define vicmpallgtSimplifierHandler dftSimplifier
#define vicmpallgeSimplifierHandler dftSimplifier
#define vicmpallltSimplifierHandler dftSimplifier
#define vicmpallleSimplifierHandler dftSimplifier
#define vicmpanyeqSimplifierHandler dftSimplifier
#define vicmpanyneSimplifierHandler dftSimplifier
#define vicmpanygtSimplifierHandler dftSimplifier
#define vicmpanygeSimplifierHandler dftSimplifier
#define vicmpanyltSimplifierHandler dftSimplifier
#define vicmpanyleSimplifierHandler dftSimplifier
#define vnotSimplifierHandler dftSimplifier
#define vbitselectSimplifierHandler dftSimplifier
#define vpermSimplifierHandler dftSimplifier
#define vsplatsSimplifierHandler dftSimplifier
#define vdmergelSimplifierHandler dftSimplifier
#define vdmergehSimplifierHandler dftSimplifier
#define vdsetelemSimplifierHandler dftSimplifier
#define vdgetelemSimplifierHandler dftSimplifier
#define vdselSimplifierHandler dftSimplifier
#define vdremSimplifierHandler dftSimplifier
#define vdmaddSimplifierHandler dftSimplifier
#define vdnmsubSimplifierHandler dftSimplifier
#define vdmsubSimplifierHandler dftSimplifier
#define vdmaxSimplifierHandler dftSimplifier
#define vdminSimplifierHandler dftSimplifier
#define vdcmpeqSimplifierHandler normalizeCmpSimplifier
#define vdcmpneSimplifierHandler normalizeCmpSimplifier
#define vdcmpgtSimplifierHandler normalizeCmpSimplifier
#define vdcmpgeSimplifierHandler normalizeCmpSimplifier
#define vdcmpltSimplifierHandler normalizeCmpSimplifier
#define vdcmpleSimplifierHandler normalizeCmpSimplifier
#define vdcmpalleqSimplifierHandler normalizeCmpSimplifier
#define vdcmpallneSimplifierHandler normalizeCmpSimplifier
#define vdcmpallgtSimplifierHandler normalizeCmpSimplifier
#define vdcmpallgeSimplifierHandler normalizeCmpSimplifier
#define vdcmpallltSimplifierHandler normalizeCmpSimplifier
#define vdcmpallleSimplifierHandler normalizeCmpSimplifier
#define vdcmpanyeqSimplifierHandler normalizeCmpSimplifier
#define vdcmpanyneSimplifierHandler normalizeCmpSimplifier
#define vdcmpanygtSimplifierHandler normalizeCmpSimplifier
#define vdcmpanygeSimplifierHandler normalizeCmpSimplifier
#define vdcmpanyltSimplifierHandler normalizeCmpSimplifier
#define vdcmpanyleSimplifierHandler normalizeCmpSimplifier
#define vdsqrtSimplifierHandler dftSimplifier
#define vdlogSimplifierHandler dftSimplifier
#define vincSimplifierHandler dftSimplifier
#define vdecSimplifierHandler dftSimplifier
#define vnegSimplifierHandler dftSimplifier
#define vcomSimplifierHandler dftSimplifier
#define vaddSimplifierHandler dftSimplifier
#define vsubSimplifierHandler dftSimplifier
#define vmulSimplifierHandler dftSimplifier
#define vdivSimplifierHandler dftSimplifier
#define vremSimplifierHandler dftSimplifier
#define vandSimplifierHandler dftSimplifier
#define vorSimplifierHandler dftSimplifier
#define vxorSimplifierHandler dftSimplifier
#define vshlSimplifierHandler dftSimplifier
#define vushrSimplifierHandler dftSimplifier
#define vshrSimplifierHandler dftSimplifier
#define vcmpeqSimplifierHandler dftSimplifier
#define vcmpneSimplifierHandler dftSimplifier
#define vcmpltSimplifierHandler dftSimplifier
#define vucmpltSimplifierHandler dftSimplifier
#define vcmpgtSimplifierHandler dftSimplifier
#define vucmpgtSimplifierHandler dftSimplifier
#define vcmpleSimplifierHandler dftSimplifier
#define vucmpleSimplifierHandler dftSimplifier
#define vcmpgeSimplifierHandler dftSimplifier
#define vucmpgeSimplifierHandler dftSimplifier
#define vloadSimplifierHandler directLoadSimplifier
#define vloadiSimplifierHandler indirectLoadSimplifier
#define vstoreSimplifierHandler directStoreSimplifier
#define vstoreiSimplifierHandler indirectStoreSimplifier
#define vrandSimplifierHandler dftSimplifier
#define vreturnSimplifierHandler dftSimplifier
#define vcallSimplifierHandler dftSimplifier
#define vcalliSimplifierHandler dftSimplifier
#define vselectSimplifierHandler dftSimplifier
#define v2vSimplifierHandler v2vSimplifier
#define vl2vdSimplifierHandler dftSimplifier
#define vconstSimplifierHandler dftSimplifier
#define getvelemSimplifierHandler dftSimplifier
#define vsetelemSimplifierHandler vsetelemSimplifier
#define vbRegLoadSimplifierHandler dftSimplifier
#define vsRegLoadSimplifierHandler dftSimplifier
#define viRegLoadSimplifierHandler dftSimplifier
#define vlRegLoadSimplifierHandler dftSimplifier
#define vfRegLoadSimplifierHandler dftSimplifier
#define vdRegLoadSimplifierHandler dftSimplifier
#define vbRegStoreSimplifierHandler dftSimplifier
#define vsRegStoreSimplifierHandler dftSimplifier
#define viRegStoreSimplifierHandler dftSimplifier
#define vlRegStoreSimplifierHandler dftSimplifier
#define vfRegStoreSimplifierHandler dftSimplifier
#define vdRegStoreSimplifierHandler dftSimplifier
#define iuconstSimplifierHandler constSimplifier
#define luconstSimplifierHandler lconstSimplifier
#define buconstSimplifierHandler constSimplifier
#define iuloadSimplifierHandler directLoadSimplifier
#define luloadSimplifierHandler directLoadSimplifier
#define buloadSimplifierHandler directLoadSimplifier
#define iuloadiSimplifierHandler indirectLoadSimplifier
#define luloadiSimplifierHandler indirectLoadSimplifier
#define buloadiSimplifierHandler indirectLoadSimplifier
#define iustoreSimplifierHandler directStoreSimplifier
#define lustoreSimplifierHandler lstoreSimplifier
#define bustoreSimplifierHandler dftSimplifier
#define iustoreiSimplifierHandler indirectStoreSimplifier
#define lustoreiSimplifierHandler indirectStoreSimplifier
#define bustoreiSimplifierHandler indirectStoreSimplifier
#define iureturnSimplifierHandler dftSimplifier
#define lureturnSimplifierHandler dftSimplifier
#define lucallSimplifierHandler lcallSimplifier
#define iuaddSimplifierHandler iaddSimplifier
#define luaddSimplifierHandler laddSimplifier
#define buaddSimplifierHandler baddSimplifier
#define iusubSimplifierHandler isubSimplifier
#define lusubSimplifierHandler lsubSimplifier
#define busubSimplifierHandler bsubSimplifier
#define iunegSimplifierHandler inegSimplifier
#define lunegSimplifierHandler lnegSimplifier
#define f2iuSimplifierHandler f2iSimplifier
#define f2luSimplifierHandler f2lSimplifier
#define f2buSimplifierHandler f2bSimplifier
#define f2cSimplifierHandler f2cSimplifier
#define d2iuSimplifierHandler d2iSimplifier
#define d2luSimplifierHandler d2lSimplifier
#define d2buSimplifierHandler d2bSimplifier
#define d2cSimplifierHandler d2cSimplifier
#define iuRegLoadSimplifierHandler dftSimplifier
#define luRegLoadSimplifierHandler dftSimplifier
#define iuRegStoreSimplifierHandler dftSimplifier
#define luRegStoreSimplifierHandler dftSimplifier
#define cconstSimplifierHandler constSimplifier
#define cloadSimplifierHandler directLoadSimplifier
#define cloadiSimplifierHandler indirectLoadSimplifier
#define cstoreSimplifierHandler dftSimplifier
#define cstoreiSimplifierHandler indirectStoreSimplifier
#define monentSimplifierHandler dftSimplifier
#define monexitSimplifierHandler dftSimplifier
#define monexitfenceSimplifierHandler dftSimplifier
#define tstartSimplifierHandler dftSimplifier
#define tfinishSimplifierHandler dftSimplifier
#define tabortSimplifierHandler dftSimplifier
#define instanceofSimplifierHandler dftSimplifier
#define checkcastSimplifierHandler checkcastSimplifier
#define checkcastAndNULLCHKSimplifierHandler checkcastAndNULLCHKSimplifier
#define NewSimplifierHandler NewSimplifier
#define newvalueSimplifierHandler dftSimplifier
#define newarraySimplifierHandler dftSimplifier
#define anewarraySimplifierHandler dftSimplifier
#define variableNewSimplifierHandler variableNewSimplifier
#define variableNewArraySimplifierHandler variableNewSimplifier
#define multianewarraySimplifierHandler dftSimplifier
#define arraylengthSimplifierHandler arrayLengthSimplifier
#define contigarraylengthSimplifierHandler arrayLengthSimplifier
#define discontigarraylengthSimplifierHandler arrayLengthSimplifier
#define icalliSimplifierHandler dftSimplifier
#define lcalliSimplifierHandler dftSimplifier
#define lucalliSimplifierHandler dftSimplifier
#define fcalliSimplifierHandler dftSimplifier
#define dcalliSimplifierHandler dftSimplifier
#define acalliSimplifierHandler dftSimplifier
#define calliSimplifierHandler dftSimplifier
#define fenceSimplifierHandler dftSimplifier
#define luaddhSimplifierHandler dftSimplifier
#define caddSimplifierHandler caddSimplifier
#define aiaddSimplifierHandler iaddSimplifier
#define aiuaddSimplifierHandler iaddSimplifier
#define aladdSimplifierHandler laddSimplifier
#define aluaddSimplifierHandler laddSimplifier
#define lusubhSimplifierHandler dftSimplifier
#define csubSimplifierHandler csubSimplifier
#define imulhSimplifierHandler imulhSimplifier
#define iumulhSimplifierHandler dftSimplifier
#define lmulhSimplifierHandler lmulhSimplifier
#define lumulhSimplifierHandler lmulhSimplifier
#define ibits2fSimplifierHandler ibits2fSimplifier
#define fbits2iSimplifierHandler fbits2iSimplifier
#define lbits2dSimplifierHandler lbits2dSimplifier
#define dbits2lSimplifierHandler dbits2lSimplifier
#define lookupSimplifierHandler lookupSwitchSimplifier
#define trtLookupSimplifierHandler dftSimplifier
#define CaseSimplifierHandler dftSimplifier
#define tableSimplifierHandler tableSwitchSimplifier
#define exceptionRangeFenceSimplifierHandler dftSimplifier
#define dbgFenceSimplifierHandler dftSimplifier
#define NULLCHKSimplifierHandler nullchkSimplifier
#define ResolveCHKSimplifierHandler dftSimplifier
#define ResolveAndNULLCHKSimplifierHandler dftSimplifier
#define DIVCHKSimplifierHandler divchkSimplifier
#define OverflowCHKSimplifierHandler dftSimplifier
#define UnsignedOverflowCHKSimplifierHandler dftSimplifier
#define BNDCHKSimplifierHandler bndchkSimplifier
#define ArrayCopyBNDCHKSimplifierHandler arraycopybndchkSimplifier
#define BNDCHKwithSpineCHKSimplifierHandler bndchkwithspinechkSimplifier
#define SpineCHKSimplifierHandler dftSimplifier
#define ArrayStoreCHKSimplifierHandler dftSimplifier
#define ArrayCHKSimplifierHandler dftSimplifier
#define RetSimplifierHandler dftSimplifier
#define arraycopySimplifierHandler dftSimplifier
#define arraysetSimplifierHandler arraysetSimplifier
#define arraytranslateSimplifierHandler dftSimplifier
#define arraytranslateAndTestSimplifierHandler dftSimplifier
#define long2StringSimplifierHandler dftSimplifier
#define bitOpMemSimplifierHandler bitOpMemSimplifier
#define bitOpMemNDSimplifierHandler bitOpMemNDSimplifier
#define arraycmpSimplifierHandler dftSimplifier
#define arraycmpWithPadSimplifierHandler arrayCmpWithPadSimplifier
#define allocationFenceSimplifierHandler dftSimplifier
#define loadFenceSimplifierHandler dftSimplifier
#define storeFenceSimplifierHandler dftSimplifier
#define fullFenceSimplifierHandler dftSimplifier
#define MergeNewSimplifierHandler dftSimplifier
#define computeCCSimplifierHandler computeCCSimplifier
#define butestSimplifierHandler dftSimplifier
#define sutestSimplifierHandler dftSimplifier
#define bucmpSimplifierHandler bucmpSimplifier
#define bcmpSimplifierHandler bcmpSimplifier
#define sucmpSimplifierHandler sucmpSimplifier
#define scmpSimplifierHandler scmpSimplifier
#define iucmpSimplifierHandler iucmpSimplifier
#define icmpSimplifierHandler icmpSimplifier
#define lucmpSimplifierHandler lucmpSimplifier
#define ificmpoSimplifierHandler ifxcmpoSimplifier
#define ificmpnoSimplifierHandler ifxcmpoSimplifier
#define iflcmpoSimplifierHandler ifxcmpoSimplifier
#define iflcmpnoSimplifierHandler ifxcmpoSimplifier
#define ificmnoSimplifierHandler ifxcmnoSimplifier
#define ificmnnoSimplifierHandler ifxcmnoSimplifier
#define iflcmnoSimplifierHandler ifxcmnoSimplifier
#define iflcmnnoSimplifierHandler ifxcmnoSimplifier
#define iuaddcSimplifierHandler iaddSimplifier
#define luaddcSimplifierHandler laddSimplifier
#define iusubbSimplifierHandler isubSimplifier
#define lusubbSimplifierHandler lsubSimplifier
#define icmpsetSimplifierHandler dftSimplifier
#define lcmpsetSimplifierHandler dftSimplifier
#define bztestnsetSimplifierHandler dftSimplifier
#define ibatomicorSimplifierHandler dftSimplifier
#define isatomicorSimplifierHandler dftSimplifier
#define iiatomicorSimplifierHandler dftSimplifier
#define ilatomicorSimplifierHandler dftSimplifier
#define dexpSimplifierHandler expSimplifier
#define branchSimplifierHandler dftSimplifier
#define igotoSimplifierHandler dftSimplifier
#define bexpSimplifierHandler dftSimplifier
#define buexpSimplifierHandler dftSimplifier
#define sexpSimplifierHandler dftSimplifier
#define cexpSimplifierHandler dftSimplifier
#define iexpSimplifierHandler dftSimplifier
#define iuexpSimplifierHandler dftSimplifier
#define lexpSimplifierHandler dftSimplifier
#define luexpSimplifierHandler dftSimplifier
#define fexpSimplifierHandler expSimplifier
#define fuexpSimplifierHandler expSimplifier
#define duexpSimplifierHandler expSimplifier
#define ixfrsSimplifierHandler dftSimplifier
#define lxfrsSimplifierHandler dftSimplifier
#define fxfrsSimplifierHandler dftSimplifier
#define dxfrsSimplifierHandler dftSimplifier
#define fintSimplifierHandler dftSimplifier
#define dintSimplifierHandler dftSimplifier
#define fnintSimplifierHandler dftSimplifier
#define dnintSimplifierHandler dftSimplifier
#define fsqrtSimplifierHandler fsqrtSimplifier
#define dsqrtSimplifierHandler dsqrtSimplifier
#define getstackSimplifierHandler dftSimplifier
#define deallocaSimplifierHandler dftSimplifier
#define idozSimplifierHandler dftSimplifier
#define dcosSimplifierHandler dftSimplifier
#define dsinSimplifierHandler dftSimplifier
#define dtanSimplifierHandler dftSimplifier
#define dcoshSimplifierHandler dftSimplifier
#define dsinhSimplifierHandler dftSimplifier
#define dtanhSimplifierHandler dftSimplifier
#define dacosSimplifierHandler dftSimplifier
#define dasinSimplifierHandler dftSimplifier
#define datanSimplifierHandler dftSimplifier
#define datan2SimplifierHandler dftSimplifier
#define dlogSimplifierHandler dftSimplifier
#define dfloorSimplifierHandler dftSimplifier
#define ffloorSimplifierHandler dftSimplifier
#define dceilSimplifierHandler dftSimplifier
#define fceilSimplifierHandler dftSimplifier
#define ibranchSimplifierHandler dftSimplifier
#define mbranchSimplifierHandler dftSimplifier
#define getpmSimplifierHandler dftSimplifier
#define setpmSimplifierHandler dftSimplifier
#define loadAutoOffsetSimplifierHandler dftSimplifier
#define imaxSimplifierHandler imaxminSimplifier
#define iumaxSimplifierHandler imaxminSimplifier
#define lmaxSimplifierHandler lmaxminSimplifier
#define lumaxSimplifierHandler lmaxminSimplifier
#define fmaxSimplifierHandler fmaxminSimplifier
#define dmaxSimplifierHandler dmaxminSimplifier
#define iminSimplifierHandler imaxminSimplifier
#define iuminSimplifierHandler imaxminSimplifier
#define lminSimplifierHandler lmaxminSimplifier
#define luminSimplifierHandler lmaxminSimplifier
#define fminSimplifierHandler fmaxminSimplifier
#define dminSimplifierHandler dmaxminSimplifier
#define trtSimplifierHandler dftSimplifier
#define trtSimpleSimplifierHandler dftSimplifier
#define ihbitSimplifierHandler dftSimplifier
#define ilbitSimplifierHandler dftSimplifier
#define inolzSimplifierHandler dftSimplifier
#define inotzSimplifierHandler dftSimplifier
#define ipopcntSimplifierHandler dftSimplifier
#define lhbitSimplifierHandler dftSimplifier
#define llbitSimplifierHandler dftSimplifier
#define lnolzSimplifierHandler dftSimplifier
#define lnotzSimplifierHandler dftSimplifier
#define lpopcntSimplifierHandler dftSimplifier
#define ibyteswapSimplifierHandler dftSimplifier
#define bbitpermuteSimplifierHandler dftSimplifier
#define sbitpermuteSimplifierHandler dftSimplifier
#define ibitpermuteSimplifierHandler dftSimplifier
#define lbitpermuteSimplifierHandler dftSimplifier
#define PrefetchSimplifierHandler dftSimplifier



#endif

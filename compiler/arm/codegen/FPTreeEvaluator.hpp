/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef IA32FPTREEEVALUATOR_INCL
#define IA32FPTREEEVALUATOR_INCL

// FPCW values
//
#define SINGLE_PRECISION_ROUND_TO_ZERO      0x0c7f
#define DOUBLE_PRECISION_ROUND_TO_ZERO      0x0e7f
#define SINGLE_PRECISION_ROUND_TO_NEAREST   0x007f
#define DOUBLE_PRECISION_ROUND_TO_NEAREST   0x027f

// Binary representation of double 1.0
//
#ifndef _LONG_LONG
#define IEEE_DOUBLE_1_0 0x3ff0000000000000L
#else
#define IEEE_DOUBLE_1_0 0x3ff0000000000000LL
#endif

extern void insertPrecisionAdjustment(TR_Register *reg,
									  TR::Node     *node);
#endif


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

#ifndef OMR_ILOPCODES_ENUM_INCL
#define OMR_ILOPCODES_ENUM_INCL

//  NOTE: IF you add opcodes or change the order then you must fix the following
//        files (at least): ./ILOpCodeProperties.hpp
//                          compiler/ras/Tree.cpp (2 tables)
//                          compiler/optimizer/SimplifierTable.hpp
//                          compiler/optimizer/ValuePropagationTable.hpp
//                          compiler/x/amd64/codegen/TreeEvaluatorTable.cpp
//                          compiler/x/i386/codegen/TreeEvaluatorTable.cpp
//                          compiler/p/codegen/TreeEvaluatorTable.cpp
//                          compiler/z/codegen/TreeEvaluatorTable.cpp
//                          compiler/aarch64/codegen/TreeEvaluatorTable.cpp
//                          compiler/arm/codegen/TreeEvaluatorTable.cpp
//                          compiler/il/OMRILOpCodesEnum.hpp
//                          compiler/il/ILOpCodes.hpp
// Also check tables in ../codegen/ILOps.hpp


#include "il/OMROpcodes.hpp"

#define GET_ENUM_VAL(\
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
   ...) enumValue,

   FirstOMROp,
   BadILOp = 0,
   FOR_EACH_OPCODE(GET_ENUM_VAL)
   LastOMROp = Prefetch,

#undef GET_ENUM_VAL
#endif

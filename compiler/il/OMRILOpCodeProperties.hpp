/*******************************************************************************
 * Copyright (c) 2000, 2022 IBM Corp. and others
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

#ifndef OMR_OPCODEPROPERTIES_INCL
#define OMR_OPCODEPROPERTIES_INCL

#define OPCODE_MACRO(\
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
   ifCompareOpcode, ...) \
   \
   { \
   TR::opcode, \
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
   },

   {
     /* .opcode               = */ TR::BadILOp,
     /* .name                 = */ "BadILOp",
     /* .properties1          = */ 0,
     /* .properties2          = */ 0,
     /* .properties3          = */ 0,
     /* .properties4          = */ 0,
     /* .dataType             = */ TR::NoType,
     /* .typeProperties       = */ 0,
     /* .childProperties      = */ ILChildProp::NoChildren,
     /* .swapChildrenOpCode   = */ TR::BadILOp,
     /* .reverseBranchOpCode  = */ TR::BadILOp,
     /* .booleanCompareOpCode = */ TR::BadILOp,
     /* .ifCompareOpCode      = */ TR::BadILOp,
   },

#include "il/Opcodes.enum"
#undef OPCODE_MACRO


#define VECTOR_OPERATION_MACRO(\
   operation, \
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
   ifCompareOpcode, ...) \
   \
   { \
   TR::BadILOp, \
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
   },

#include "il/VectorOperations.enum"
#undef VECTOR_OPERATION_MACRO

#endif

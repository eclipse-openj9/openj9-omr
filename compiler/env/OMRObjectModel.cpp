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
 ******************************************************************************/

#include "env/ObjectModel.hpp"

#include <limits.h>                   // for LONG_MAX
#include <stddef.h>                   // for NULL
#include <stdint.h>                   // for int32_t, int64_t, uint32_t
#include "codegen/CodeGenerator.hpp"  // for CodeGenerator
#include "compile/Compilation.hpp"    // for Compilation
#include "env/CompilerEnv.hpp"
#include "env/jittypes.h"             // for uintptrj_t, intptrj_t
#include "infra/Assert.hpp"           // for TR_ASSERT

namespace TR { class Node; }

#define notImplemented(A) TR_ASSERT(0, "OMR::ObjectModel::%s is undefined", (A) )

OMR::ObjectModel::ObjectModel()
   {
   }

int32_t
OMR::ObjectModel::sizeofReferenceField()
   {
   notImplemented("sizeofReferenceField");
   return 0;
   }

intptrj_t
OMR::ObjectModel::sizeofReferenceAddress()
   {
   return TR::Compiler->target.is64Bit() ? 8 : 4;
   }

uintptrj_t
OMR::ObjectModel::elementSizeOfBooleanArray()
   {
   notImplemented("elementSizeOfBooleanArray");
   return 0;
   }


uint32_t
OMR::ObjectModel::getSizeOfArrayElement(TR::Node * node)
   {
   notImplemented("getSizeOfArrayElement");
   return 0;
   }

int64_t
OMR::ObjectModel::maxArraySizeInElementsForAllocation(TR::Node *newArray, TR::Compilation *comp)
   {
   return LONG_MAX;
   }

int64_t
OMR::ObjectModel::maxArraySizeInElements(int32_t knownMinElementSize, TR::Compilation *comp)
   {
   return LONG_MAX;
   }

int32_t
OMR::ObjectModel::compressedReferenceShiftOffset()
   {
   return 0;
   }

int32_t
OMR::ObjectModel::compressedReferenceShift()
   {
   return 0;
   }
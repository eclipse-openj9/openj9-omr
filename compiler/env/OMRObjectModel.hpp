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

#ifndef OMR_OBJECT_MODEL_INCL
#define OMR_OBJECT_MODEL_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef OMR_OBJECT_MODEL_CONNECTOR
#define OMR_OBJECT_MODEL_CONNECTOR
namespace OMR { class ObjectModel; }
namespace OMR { typedef OMR::ObjectModel ObjectModelConnector; }
#endif

#include <stdint.h>        // for int32_t, int64_t, uint32_t
#include "env/jittypes.h"  // for uintptrj_t, intptrj_t

class TR_OpaqueClassBlock;
namespace OMR { class ObjectModel; }
namespace TR { class Compilation; }
namespace TR { class Node; }

namespace OMR
{

class ObjectModel
   {

   public:

   ObjectModel();

   void initialize() { }

   bool mayRequireSpineChecks() { return false; }

   bool generateCompressedObjectHeaders() { return false; }

   // This query answers whether or not this VM is capable of generating arraylet
   // trees.  This does not imply that it will for this compilation unit: you must ask
   // Compilation::generateArraylets() to answer that.
   //
   bool canGenerateArraylets() { return false; }

   bool useHybridArraylets() { return false; }
   bool usesDiscontiguousArraylets() { return false; }

   int32_t maxContiguousArraySizeInBytes() { return 0; }

   uintptrj_t contiguousArrayHeaderSizeInBytes() { return 0; }

   uintptrj_t discontiguousArrayHeaderSizeInBytes() { return 0; }

   bool isDiscontiguousArray(int32_t sizeInBytes) { return false; }
   bool isDiscontiguousArray(int32_t sizeInElements, int32_t elementSize) { return false; }


   int32_t compressedReferenceShiftOffset();

   int32_t compressedReferenceShift();

   uintptrj_t offsetOfObjectVftField() { return 0; }

   // --------------------------------------------------------------------------
   // Object shape
   //
   int32_t sizeofReferenceField();
   intptrj_t sizeofReferenceAddress();
   uintptrj_t elementSizeOfBooleanArray();
   uint32_t getSizeOfArrayElement(TR::Node * node);
   int64_t maxArraySizeInElementsForAllocation(TR::Node *newArray, TR::Compilation *comp);
   int64_t maxArraySizeInElements(int32_t knownMinElementSize, TR::Compilation *comp);
   bool nativeAddressesCanChangeSize() { return false; }
   };
}

#endif

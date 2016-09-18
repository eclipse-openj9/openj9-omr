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

#ifndef OMR_METHOD_METADATAPOD_INCL
#define OMR_METHOD_METADATAPOD_INCL

/*
 * The following #define(s) and typedef(s) must appear before any #includes in this file
 */
#ifndef OMR_METHOD_METADATAPOD_CONNECTOR
#define OMR_METHOD_METADATAPOD_CONNECTOR
namespace OMR { struct MethodMetaDataPOD; }
namespace OMR { typedef OMR::MethodMetaDataPOD MethodMetaDataPODConnector; }
#endif

#include <stdint.h>               // for uintptr_t
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

/*
 * This structure describes the shape of the method meta data information.
 * It must be a C++ POD object and follow all the rules behind POD formation.
 * Because its fields may be extracted by a runtime system, its exact layout
 * shape MUST be preserved.
 *
 */

namespace OMR
{

struct OMR_EXTENSIBLE MethodMetaDataPOD
   {
   uintptr_t startPC;
   uintptr_t endPC;


#if 0
#if defined(RAS_EYECATCHERS)
    struct J9UTF8* className;
    struct J9UTF8* methodName;
    struct J9UTF8* methodSignature;
#endif
    struct J9ConstantPool* constantPool;
    struct J9Method* ramMethod;
    UDATA startPC;
    UDATA endWarmPC;
    UDATA startColdPC;
    UDATA endPC;
    UDATA totalFrameSize;
    I_16 slots;
    I_16 scalarTempSlots;
    I_16 objectTempSlots;
    U_16 prologuePushes;
    I_16 tempOffset;
    U_16 numExcptionRanges;
    I_32 size;
    UDATA flags;
    UDATA assumptionFlags;
    UDATA registerSaveDescription;
    void* relocationData;
    void* gcStackAtlas;
    void* inlinedCalls;
    void* bodyInfo;
    struct J9JITExceptionTable* nextMethod;
    UDATA compileMethodCodeStartPC;
    UDATA compileMethodCodeSize;
    UDATA compileMethodDataStartPC;
    UDATA compileMethodDataSize;
    UDATA compileFirstClassLocation;
    void* osrInfo;
    void* runtimeAssumptionList;
    I_32 hotness;
    UDATA codeCacheAlloc;
    void* gpuCode;
#endif
   };

}

#endif

/*******************************************************************************
 * Copyright (c) 2017, 2017 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

/*
 * Wrapper for communication between valgrind and GC.
*/

#include "omrcfg.h"
#if defined(OMR_VALGRIND_MEMCHECK)

#include <valgrind/memcheck.h>

#include "MemcheckWrapper.hpp"
#include "GCExtensionsBase.hpp"
#include "ForwardedHeader.hpp"

void valgrindCreateMempool(MM_GCExtensionsBase *extensions,uintptr_t poolAddr)
{
    //1 lets valgrind know that objects will be defined when allocated
    VALGRIND_CREATE_MEMPOOL(poolAddr, 0, 1);
    extensions->valgrindMempoolAddr = poolAddr;
}

void valgrindDestroyMempool(MM_GCExtensionsBase *extensions)
{
    if(extensions->valgrindMempoolAddr != 0)
	{
        //All objects should have been freed by now!
        Assert_MM_true(extensions->_allocatedObjects.empty());
        VALGRIND_DESTROY_MEMPOOL(extensions->valgrindMempoolAddr);
        extensions->valgrindMempoolAddr = 0;
    }
}

void valgrindMempoolAlloc(MM_GCExtensionsBase *extensions, uintptr_t baseAddress, uintptr_t size)
{
#if defined(VALGRIND_REQUEST_LOGS)
    VALGRIND_PRINTF_BACKTRACE("Allocating object at 0x%lx of size %lu\n", baseAddress, size);
#endif /* defined(VALGRIND_REQUEST_LOGS) */
    /* Allocate object in Valgrind memory pool. */ 		
    VALGRIND_MEMPOOL_ALLOC(extensions->valgrindMempoolAddr, baseAddress, size);
    extensions->_allocatedObjects.insert(baseAddress);
}

void valgrindMakeMemDefined(uintptr_t address, uintptr_t size)
{
#if defined(VALGRIND_REQUEST_LOGS)
    VALGRIND_PRINTF_BACKTRACE("Marking area defined at 0x%lx of size %lu\n", address, size);
#endif /* defined(VALGRIND_REQUEST_LOGS) */
    VALGRIND_MAKE_MEM_DEFINED(address, size);
}

void valgrindMakeMemNoaccess(uintptr_t address, uintptr_t size)
{
#if defined(VALGRIND_REQUEST_LOGS)
    VALGRIND_PRINTF_BACKTRACE("Marking area noaccess at 0x%lx of size %lu\n", address, size);
#endif /* defined(VALGRIND_REQUEST_LOGS) */
    VALGRIND_MAKE_MEM_NOACCESS(address, size);
}

void valgrindClearRange(MM_GCExtensionsBase *extensions, uintptr_t baseAddress, uintptr_t size)
{
    if(size == 0)
        return;

    uintptr_t topInclusiveAddr = baseAddress + size - 1;
#if defined(VALGRIND_REQUEST_LOGS)	
    VALGRIND_PRINTF_BACKTRACE("Clearing objects in range b/w 0x%lx and  0x%lx\n", baseAddress,topInclusiveAddr);
#endif /* defined(VALGRIND_REQUEST_LOGS) */

    if(!extensions->_allocatedObjects.empty())
    {	
        std::set<uintptr_t>::iterator it;
        std::set<uintptr_t>::iterator setEnd = extensions->_allocatedObjects.end();
        std::set<uintptr_t>::iterator lBound = extensions->_allocatedObjects.lower_bound(baseAddress);
        std::set<uintptr_t>::iterator uBound = --extensions->_allocatedObjects.end();	

        while(uBound !=  extensions->_allocatedObjects.begin() && *uBound > topInclusiveAddr)
            uBound--;

#if defined(VALGRIND_REQUEST_LOGS)	
        VALGRIND_PRINTF("lBound = %lx, uBound = %lx\n",*lBound,*uBound);			
#endif /* defined(VALGRIND_REQUEST_LOGS) */			
        
        if(*uBound >topInclusiveAddr)
            return; //all elements left in set are greater than our range

        for(it = lBound;*it <= *uBound && it != setEnd;it++)
        {
            int objSize;
            if(MM_ForwardedHeader((omrobjectptr_t) *it).isForwardedPointer())
                objSize = sizeof(MM_ForwardedHeader);
            else
                objSize = (int) ((GC_ObjectModel)extensions->objectModel).getConsumedSizeInBytesWithHeader((omrobjectptr_t) *it);
#if defined(VALGRIND_REQUEST_LOGS)
            VALGRIND_PRINTF("Clearing object at 0x%lx of size %d\n", *it,objSize);
#endif /* defined(VALGRIND_REQUEST_LOGS) */
            VALGRIND_CHECK_MEM_IS_DEFINED(*it,objSize);
            VALGRIND_MEMPOOL_FREE(extensions->valgrindMempoolAddr,*it);
        }
        if(*uBound >= *lBound && uBound != setEnd)
        {
#if defined(VALGRIND_REQUEST_LOGS)				
            VALGRIND_PRINTF("Erasing set from lBound = %lx to uBound = %lx\n",*lBound,*uBound);		
#endif /* defined(VALGRIND_REQUEST_LOGS) */			
            extensions->_allocatedObjects.erase(lBound,++uBound);//uBound is exclusive
        }
    }
    /* Valgrind automatically marks free objects as noaccess.
    We still mark the entire region for left out areas */
    valgrindMakeMemNoaccess(baseAddress,size);
}

void valgrindFreeObject(MM_GCExtensionsBase *extensions, uintptr_t baseAddress)
{
    int objSize;
    if(MM_ForwardedHeader((omrobjectptr_t) baseAddress).isForwardedPointer())
        objSize = sizeof(MM_ForwardedHeader);
    else
        objSize = (int) ((GC_ObjectModel)extensions->objectModel).getConsumedSizeInBytesWithHeader((omrobjectptr_t) baseAddress);
#if defined(VALGRIND_REQUEST_LOGS)				    
    VALGRIND_PRINTF_BACKTRACE("Clearing object at 0x%lx of size %d\n",baseAddress,objSize);
#endif /* defined(VALGRIND_REQUEST_LOGS) */
    VALGRIND_CHECK_MEM_IS_DEFINED(baseAddress,objSize);
    VALGRIND_MEMPOOL_FREE(extensions->valgrindMempoolAddr,baseAddress);
    extensions->_allocatedObjects.erase(baseAddress);
}

bool valgrindCheckObjectInPool(MM_GCExtensionsBase *extensions, uintptr_t baseAddress)
{
    return (extensions->_allocatedObjects.find(baseAddress) != extensions->_allocatedObjects.end());
}

void valgrindResizeObject(MM_GCExtensionsBase *extensions, uintptr_t baseAddress, uintptr_t oldSize, uintptr_t newSize)
{

#if defined(VALGRIND_REQUEST_LOGS)
    VALGRIND_PRINTF_BACKTRACE("Resizing object at 0x%lx from size %d to %d\n", baseAddress, (int) oldSize, (int) newSize);
#endif /* defined(VALGRIND_REQUEST_LOGS) */

    /* We could have used VALGRIND_MEMPOOL_CHANGE request to let Valgrind know of moved object
    but it is very slow without an internal hack. (https://bugs.kde.org/show_bug.cgi?id=366817)*/
    VALGRIND_CHECK_MEM_IS_DEFINED(baseAddress, oldSize);

    /* Valgrind already knows former size of object allocated at baseAddress. So it will 
    mark the area from baseAddress to oldSize-1 noaccess on a free request as desired*/
    VALGRIND_MEMPOOL_FREE(extensions->valgrindMempoolAddr, baseAddress);

    /* And we don't need to remove and add same address in extensions->_allocatedObjects */
    VALGRIND_MEMPOOL_ALLOC(extensions->valgrindMempoolAddr, baseAddress, newSize);
}

#endif /* defined(OMR_VALGRIND_MEMCHECK) */

/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1991, 2017
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
 *    Multiple authors (IBM Corp.) - z/TPF platform initial port to OMR environment
 *******************************************************************************/


/**
 * @file
 * @ingroup GC_Base_Core
 */

#if !defined(NONSHAREDVIRTUALMEMORY_HPP_)
#define NONSHAREDVIRTUALMEMORY_HPP_

#include "omrcfg.h"
#include "omrcomp.h"
#include "omrport.h"
#include "modronopt.h"

#include "BaseVirtual.hpp"
#include "VirtualMemory.hpp"

class MM_EnvironmentBase;

/**
 * Provides facility for management of Virtual memory not associated with the heap.
 * Some methods are implemented for AIX platforms to allow non-heap structures to
 * be malloced.
 * @ingroup GC_Base_Core
 */
class MM_NonVirtualMemory : public MM_VirtualMemory {
/*
 * Data members
 */
private:
protected:
public:
/*
 * Function members
 */
private:
protected:
	static MM_NonVirtualMemory* newInstance(MM_EnvironmentBase* env, uintptr_t heapAlignment, uintptr_t size, uint32_t memoryCategory);

	MM_NonVirtualMemory(MM_EnvironmentBase* env, uintptr_t heapAlignment)
		: MM_VirtualMemory(env, heapAlignment, 1, OMRPORT_VMEM_PAGE_FLAG_NOT_USED, 0, (OMRPORT_VMEM_MEMORY_MODE_READ | OMRPORT_VMEM_MEMORY_MODE_WRITE))
	{
		_typeId = __FUNCTION__;
	};
#if (defined(AIXPPC) && (!defined(PPC64) || defined(OMR_GC_REALTIME))) || defined(J9ZOS39064) || defined(OMRZTPF)
	virtual void tearDown(MM_EnvironmentBase* env);
	virtual void* reserveMemory(J9PortVmemParams* params);
#endif /* (defined(AIXPPC) && (!defined(PPC64) || defined(OMR_GC_REALTIME))) || defined(J9ZOS39064) || defined(OMRZTPF) */
public:
#if (defined(AIXPPC) && (!defined(PPC64) || defined(OMR_GC_REALTIME))) || defined(J9ZOS39064) || defined(OMRZTPF)
	virtual bool commitMemory(void* address, uintptr_t size);
	virtual bool decommitMemory(void* address, uintptr_t size, void* lowValidAddress, void* highValidAddress);
	virtual bool setNumaAffinity(uintptr_t numaNode, void* address, uintptr_t byteAmount);
#endif /* (defined(AIXPPC) && (!defined(PPC64) || defined(OMR_GC_REALTIME))) || defined(J9ZOS39064) || defined(OMRZTPF) */

public:
/*
 * friends
 */
	friend class MM_MemoryManager;
};

#endif /* NONSHAREDVIRTUALMEMORY_HPP_ */

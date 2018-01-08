/*******************************************************************************
 * Copyright (c) 1991, 2016 IBM Corp. and others
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

#include "Forge.hpp"

#include "omrcomp.h"
#include "EnvironmentBase.hpp"

namespace OMR {
namespace GC {

struct MemoryHeader {
	uintptr_t allocatedBytes;
	OMR::GC::AllocationCategory::Enum category;
};

union AlignedMemoryHeader {
	double forAlignment;
	MemoryHeader header;
};

bool
Forge::initialize(OMRPortLibrary* port)
{
	_portLibrary = port;

	if (0 != omrthread_monitor_init_with_name(&_mutex, 0, "OMR::GC::Forge")) {
		return false;
	}	

	for (uintptr_t i = 0; i < OMR::GC::AllocationCategory::CATEGORY_COUNT; i++) {
		_statistics[i].category = (OMR::GC::AllocationCategory::Enum) i;
		_statistics[i].allocated = 0;
		_statistics[i].highwater = 0;
	}
	
	return true;
}

void 
Forge::tearDown()
{
	_portLibrary = NULL;
	
	if (NULL != _mutex) {
		omrthread_monitor_destroy(_mutex);
		_mutex = NULL;
	}
}

/**
 * Allocates the amount of memory requested in bytesRequested.  Returns a pointer to the allocated memory, or NULL if the request could
 * not be performed.  This function is a wrapper of omrmem_allocate_memory.
 *
 * @param[in] byesRequested - the number of bytes to allocate
 * @param[in] category - the memory usage category for the allocated memory
 * @param[in] callsite - the origin of the memory request (e.g. filename.c:5), which should be found using the OMR_GET_CALLSITE() macro
 * @return a pointer to the allocated memory, or NULL if the request could not be performed
 */
void* 
Forge::allocate(std::size_t bytesRequested, OMR::GC::AllocationCategory::Enum category, const char* callsite)
{
	AlignedMemoryHeader* memoryPointer;

	memoryPointer = (AlignedMemoryHeader *) _portLibrary->mem_allocate_memory(_portLibrary, bytesRequested + sizeof(AlignedMemoryHeader), callsite, OMRMEM_CATEGORY_MM);
	if (NULL != memoryPointer) {
		memoryPointer->header.allocatedBytes = bytesRequested;
		memoryPointer->header.category = category;

		omrthread_monitor_enter(_mutex);

		_statistics[category].allocated += bytesRequested;
		if (_statistics[category].allocated > _statistics[category].highwater) {
			_statistics[category].highwater = _statistics[category].allocated; 
		}

		omrthread_monitor_exit(_mutex);
		memoryPointer += 1;
	}
	
	return memoryPointer;
}

/**
 * Deallocate memory that has been allocated by the garbage collector.  This function should not be called to deallocate memory that has
 * not been allocated by either the allocate or reallocate functions.  This function is a wrapper of omrmem_free_memory.
 *
 * @param[in] memoryPointer - a pointer to the memory that will be freed
 */
void 
Forge::free(void* memoryPointer)
{	
	if (NULL == memoryPointer) {
		return;
	}

	AlignedMemoryHeader* alignedHeader = (AlignedMemoryHeader *) memoryPointer;
	alignedHeader -= 1;


	omrthread_monitor_enter(_mutex);
	_statistics[alignedHeader->header.category].allocated -= alignedHeader->header.allocatedBytes; 
	omrthread_monitor_exit(_mutex);
	
	OMRPORT_ACCESS_FROM_OMRPORT(_portLibrary);
	omrmem_free_memory(alignedHeader);
}

/**
 * Returns the current memory usage statistics for the garbage collector.  Each entry in the array corresponds to a memory usage category type.
 * To locate memory usage statistics for a particular category, use the enumeration value as the array index (e.g. stats[REFERENCES]).
 *
 * @return an array of memory usage statistics indexed using the OMR::GC::AllocationCategory enumeration
 */
OMR_GC_MemoryStatistics*
Forge::getCurrentStatistics()
{
	return _statistics;
}

} // namespace GC
} // namespace OMR

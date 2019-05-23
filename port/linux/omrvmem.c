/*******************************************************************************
 * Copyright (c) 1991, 2019 IBM Corp. and others
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

/**
 * @file
 * @ingroup Port
 * @brief Virtual memory
 */

/* for syscall */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "omrport.h"
#include "omrportpriv.h"
#include "omrportpg.h"
#include "ut_omrport.h"
#include "omrportasserts.h"
#include "omrvmem.h"

#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#include <sys/mman.h>
/* MADV_HUGEPAGE is not defined in <sys/mman.h> in RHEL 6 & CentOS 6 */
#if !defined(MADV_HUGEPAGE)
#define MADV_HUGEPAGE 14
#endif /* MADV_HUGEPAGE */

#if defined(OMR_PORT_NUMA_SUPPORT)
#include <numaif.h>
#endif /* OMR_PORT_NUMA_SUPPORT */

#if !defined(MPOL_F_MEMS_ALLOWED)
#define MPOL_F_MEMS_ALLOWED 4
#endif

#include <sys/shm.h>

#if !defined(MAP_FAILED)
#define MAP_FAILED -1
#endif

#define INVALID_KEY -1
#define FILE_NAME_SIZE 64

#if 0
#define OMRVMEM_DEBUG
#endif

#define VMEM_MEMINFO_SIZE_MAX	2048
#define VMEM_PROC_MEMINFO_FNAME	"/proc/meminfo"
#define VMEM_PROC_MAPS_FNAME	"/proc/self/maps"

#define VMEM_TRANSPARENT_HUGEPAGE_FNAME "/sys/kernel/mm/transparent_hugepage/enabled"
#define VMEM_TRANSPARENT_HUGEPAGE_MADVISE "always [madvise] never"
#define VMEM_TRANSPARENT_HUGEPAGE_MADVISE_LENGTH 22

typedef struct vmem_hugepage_info_t {
	uintptr_t	enabled;		/*!< boolean enabling j9 large page support */
	uintptr_t	pages_total;	/*!< total number of pages maintained by the kernel */
	uintptr_t	pages_free;		/*!< number of free pages that may be allocated by us */
	uintptr_t	page_size;		/*!< page size in bytes */
} vmem_hugepage_info_t;

typedef void *ADDRESS;

typedef struct AddressRange {
	ADDRESS start;
	ADDRESS end;
} AddressRange;

static void addressRange_Init(AddressRange *range, ADDRESS start, ADDRESS end);
static BOOLEAN addressRange_Intersect(AddressRange *a, AddressRange *b, AddressRange *result);
static BOOLEAN addressRange_IsValid(AddressRange *range);
static uintptr_t addressRange_Width(AddressRange *range);
static ADDRESS findAvailableMemoryBlockNoMalloc(struct OMRPortLibrary *portLibrary, ADDRESS start, ADDRESS end, uintptr_t byteAmount, BOOLEAN reverse, BOOLEAN strictRange);

/*
 * This structure captures the state of an iterator of addresses between minimum
 * and maximum (inclusive). Each address returned will be a multiple of alignment
 * which must be a power of two.
 */
typedef struct AddressIterator {
	/* the minimum address (NULL will not be returned even if this is NULL) */
	ADDRESS minimum;
	/* the maximum address to return */
	ADDRESS maximum;
	/* all returned addresses will be a multiple of this */
	uintptr_t alignment;
	/* addresses are returned in increasing order if this is positive, decreasing order otherwise */
	intptr_t direction;
	/* the next address to be returned or NULL if the iterator has been exhausted */
	ADDRESS next;
} AddressIterator;

static void addressIterator_init(AddressIterator *iterator, ADDRESS minimum, ADDRESS maximum, uintptr_t alignment, intptr_t direction);
static BOOLEAN addressIterator_next(AddressIterator *iterator, ADDRESS *address);

static void *getMemoryInRangeForLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, key_t addressKey, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t pageSize, uintptr_t mode);
static void *getMemoryInRangeForDefaultPages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t mode);
static void *allocateMemoryForLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, void *currentAddress, key_t addressKey, OMRMemCategory *category, uintptr_t byteAmount, uintptr_t pageSize, uintptr_t mode);
static BOOLEAN isStrictAndOutOfRange(void *memoryPointer, void *startAddress, void *endAddress, uintptr_t vmemOptions);
static BOOLEAN rangeIsValid(struct J9PortVmemIdentifier *identifier, void *address, uintptr_t byteAmount);
static void *reserveLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t pageSize, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t mode);
static uintptr_t adviseHugepage(struct OMRPortLibrary *portLibrary, void* address, uintptr_t byteAmount);

static void *default_pageSize_reserve_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier, uintptr_t mode, uintptr_t pageSize, OMRMemCategory *category);
#if defined(OMR_PORT_NUMA_SUPPORT)
static void port_numa_interleave_memory(struct OMRPortLibrary *portLibrary, void *start, uintptr_t size);
#endif /* OMR_PORT_NUMA_SUPPORT */
static void update_vmemIdentifier(J9PortVmemIdentifier *identifier, void *address, void *handle, uintptr_t byteAmount, uintptr_t mode, uintptr_t pageSize, uintptr_t pageFlags, uintptr_t allocator, OMRMemCategory *category, int fd);
static uintptr_t get_hugepages_info(struct OMRPortLibrary *portLibrary, vmem_hugepage_info_t *page_info);
static uintptr_t get_transparent_hugepage_info(struct OMRPortLibrary *portLibrary);
static int get_protectionBits(uintptr_t mode);

#if defined(OMR_PORT_NUMA_SUPPORT)
/*
 * glibc doesn't include mbind yet. It is in libnuma, but we can't rely on that being available on all systems.
 * We define our own helper to make the syscall so that we can use it without relying on external libraries.
 */
static long
do_mbind(void *start, unsigned long len, int policy, const unsigned long *nodemask, unsigned long maxnode, unsigned flags)
{
	return (long)syscall(SYS_mbind, start, len, policy, nodemask, maxnode, flags);
}

/**
 * glibc doesn't include get_mempolicy. It is in libnuma, but we can't rely on that being available on all systems.
 * We define our own helper to make the syscall so that we can use it without relying on external libraries.
 *
 * For details, see the man page for get_mempolicy(2).
 * Below is an incomplete summary.
 *
 * do_get_mempolicy()
 *
 *     gets the NUMA policy of the calling process or virtual memory address
 *
 * @param int           *policy  [out] returns the policy, or next interleave node
 * @param unsigned long *nmask   [out] the node bit mask associated with the policy
 * @param unsigned long  maxnode [in]  length of nmask array
 * @param unsigned long  addr    [in]  virtual memory address of requested policy info
 * @param unsigned long  flags   [in]  controls get_mempolicy behaviour
 *
 * @return 0 on success, -1 on error returned via errno.
 * errno==EFAULT: Part of all of memory specified by nmask inaccessible
 * errno==EINVAL: Invalid arguments (many invalid combinations).
 */
static long
do_get_mempolicy(int *policy, unsigned long *nmask, unsigned long maxnode, unsigned long addr, unsigned long flags)
{
	return (long)syscall(SYS_get_mempolicy, policy, nmask, maxnode, addr, flags);
}

/*
 * Read the /sys/devices/system/node/ directory, if it exists, to find out the indices of
 * all NUMA nodes on this system. Store them into a bitmask in the port platform globals.
 */
static intptr_t
initializeNumaGlobals(struct OMRPortLibrary *portLibrary)
{
	uintptr_t result = 0;
	uintptr_t maxNodes = sizeof(J9PortNodeMask) * 8;
	uintptr_t nodeCount = 0;
	int schedReturnCode = 0;
	int mempolicyReturnCode = 0;
	int useAllNodes = 0;
	PPG_numaSyscallNotAllowed = FALSE;

	memset(&PPG_numa_available_node_mask, 0, sizeof(J9PortNodeMask));
	PPG_numa_max_node_bits = 0;

	/* populate the processAffinity with the process's incoming affinity - this will allow us to honour options like "numactl --cpubind=" */
	CPU_ZERO(&PPG_process_affinity);
	schedReturnCode = sched_getaffinity(0, sizeof(PPG_process_affinity), &PPG_process_affinity);

	/* look-up the process's default NUMA policy as applied to the set of nodes (the policy is usually "0" and applies to no nodes) */
	PPG_numa_policy_mode = -1;
	memset(&PPG_numa_mempolicy_node_mask, 0, sizeof(J9PortNodeMask));
	mempolicyReturnCode = do_get_mempolicy(&PPG_numa_policy_mode, PPG_numa_mempolicy_node_mask.mask, maxNodes, (unsigned long)NULL, 0);
	if ((0 != mempolicyReturnCode) && (EPERM == errno)) {
		PPG_numaSyscallNotAllowed = TRUE;
	}
	if (0 == mempolicyReturnCode) {

		long anySet = 0;
		int arrayLen = sizeof(PPG_numa_mempolicy_node_mask.mask) / sizeof(PPG_numa_mempolicy_node_mask.mask[0]);
		int i = 0;

		/* Check if any bits are set in PPG_numa_mempolicy_node_mask. If no nodes are specified in either the system default or process default
		 * policies then the mask will be all 0's which (if used later) would indicate no nodes are available for virtual memory allocation. */
		for (i = 0; ((0 == anySet) && (i < arrayLen)); i++) {
			anySet |= PPG_numa_mempolicy_node_mask.mask[i];
		}
		if (0 == anySet) {
			/* If an empty set is returned, e.g. if numactl is not used, ask again with MPOL_F_MEMS_ALLOWED to get the correct node mask. */
			mempolicyReturnCode = do_get_mempolicy((int *) NULL, PPG_numa_mempolicy_node_mask.mask, maxNodes, (unsigned long)NULL, MPOL_F_MEMS_ALLOWED);

			/* MPOL_F_MEMS_ALLOWED is unavailable on older systems. If it fails, we use PPG_numa_available_node_mask instead. */
			if (0 != mempolicyReturnCode) {
				Trc_PRT_vmem_omrvmem_initializeNumaGlobals_get_allowed_mems_failure(errno);
				mempolicyReturnCode = 0;
				useAllNodes = 1;
			}
		}
	} else {
		Trc_PRT_vmem_omrvmem_initializeNumaGlobals_get_mempolicy_failure(errno);
	}

	/* Proceed if we could successfully look up the default affinity.
	 * We proceed to get node count even if `getmempolicy` fails due to security restrictions.
	 */
	if ((0 == schedReturnCode) && ((0 == mempolicyReturnCode) || (PPG_numaSyscallNotAllowed))) {
		DIR *nodes = opendir("/sys/devices/system/node/");
		if (NULL != nodes) {
			struct dirent *node = readdir(nodes);
			while (NULL != node) {
				unsigned long nodeIndex = 0;
				if (1 == sscanf(node->d_name, "node%lu", &nodeIndex)) {
					if (nodeIndex < maxNodes) {
						unsigned long wordIndex = nodeIndex / sizeof(PPG_numa_available_node_mask.mask[0]);
						unsigned long bit = 1 << (nodeIndex % sizeof(PPG_numa_available_node_mask.mask[0]));
						PPG_numa_available_node_mask.mask[wordIndex] |= bit;
						/**
						 * PPG_numa_max_node_bits represents the maximum number of nodes.
						 * Node indexes start from 0, therefore PPG_numa_max_node_bits is max node index number plus 1.
						 * For instance, if there are following nodes:node0, node5, node8,
						 * then PPG_numa_max_node_bits is equal to 8 + 1 = 9
						 */
						if (nodeIndex >= PPG_numa_max_node_bits) {
							PPG_numa_max_node_bits = nodeIndex + 1;
						}

						nodeCount += 1;
					}
				}
				node = readdir(nodes);
			}
			closedir(nodes);
		}
	}

	/* if there aren't at least two nodes then there's nothing we can do with NUMA */
	if (nodeCount < 2) {
		result = -1;
	} else if (1 == useAllNodes) {
		/* Failed to get memory policy allowed nodes, use all available nodes instead. */
		memcpy(PPG_numa_mempolicy_node_mask.mask, PPG_numa_available_node_mask.mask, sizeof(PPG_numa_available_node_mask.mask));
	}

	return result;
}
#endif /* defined(OMR_PORT_NUMA_SUPPORT) */

/*
 * Init the range with values
 *
 * @param AddressRange* range	[out]	Returns the range object
 * @param void* 		start	[in] 	The start address of the range
 * @param void*  		end		[in]	The end address of the range
 */
static void
addressRange_Init(AddressRange *range, ADDRESS start, ADDRESS end)
{
	range->start = start;
	range->end = end;
}

/*
 * Calculate out the intersection of 2 ranges
 *
 * @param AddressRange* a		[in] 	a range object
 * @param AddressRange* b		[in] 	another range object
 * @param AddressRange* result 	[out]	the intersection of the above 2 ranges
 *
 * Returns TRUE if they have intersection.
 */
static BOOLEAN
addressRange_Intersect(AddressRange *a, AddressRange *b, AddressRange *result)
{
	result->start = a->start > b->start ? a->start : b->start;
	result->end = a->end < b->end ? a->end : b->end;

	return addressRange_IsValid(result);
}

/*
 * Calculate if a range is valid.
 * A valid range should have start < its end
 *
 * @param AddressRange* range	[in] 	a range object
 *
 * Returns TRUE if range's start < end.
 */
static BOOLEAN
addressRange_IsValid(AddressRange *range)
{
	return (range->end > range->start) ? TRUE : FALSE;
}

/*
 * Calculate out the with of a range
 *
 * @param AddressRange* range 	[in]	a range object
 *
 * Returns the width of the range.
 * Caller should make sure the input parameter 'range' is valid,
 * otherwise, unexpected value may be returned.
 */
static uintptr_t
addressRange_Width(AddressRange *range)
{
	Assert_PRT_true(addressRange_IsValid(range));
	return range->end - range->start;
}

/*
 * Find a memory block using maps information from file /proc/self/maps
 * In order to avoid file context corruption, this method implemented without memory operation such as malloc/free
 *
 * @param OMRPortLibrary *portLibrary	[in] The portLibrary object
 * @param ADDRESS 		start			[in] The start address allowed, see also @param end
 * @param ADDRESS 		end				[in] The end address allowed, see also @param start.
 * 											 The returned memory address should be within the range defined by the @param start and the @param end.
 * @param uintptr_t 	byteAmount		[in] The block size required.
 * @param BOOLEAN 		reverse			[in] Returns the first available memory block when this param equals FALSE, returns the last available memory block when this param equals TRUE
 * @param BOOLEAN       strictRange     [in] Must the returned address within the range defined by @param start and @param end?
 *
 * returns the address available.
 */
static ADDRESS
findAvailableMemoryBlockNoMalloc(struct OMRPortLibrary *portLibrary, ADDRESS start, ADDRESS end, uintptr_t byteAmount, BOOLEAN reverse, BOOLEAN strictRange)
{
	BOOLEAN dataCorrupt = FALSE;
	BOOLEAN matchFound = FALSE;
	BOOLEAN strictMatchFound = FALSE;

	/*
	 * The caller provides start and end addresses that constrain a non-null
	 * address that can be returned by this function. Internally, however,
	 * this function operates on ranges which are inclusive of the space
	 * requested by the caller: the allowed range must be initialized taking
	 * this difference into account by adding the requested block size to the
	 * end address.
	 */
	AddressRange allowedRange;
	addressRange_Init(&allowedRange, start, end + byteAmount);

	AddressRange lastAvailableRange;
	addressRange_Init(&lastAvailableRange, NULL, NULL);

	int fd = -1;
	if ((fd = omrfile_open(portLibrary, VMEM_PROC_MAPS_FNAME, EsOpenRead, 0)) != -1) {
		char readBuf[1024];
		intptr_t bytesRead = 0;
		/*
		 * Please refer to the file format of /proc/self/maps.
		 * 'lineBuf' represent for a line in the file.
		 *
		 * Don't change the length of 'lineBuf' easily.
		 * The minimal length should be 34 on 64bit machine,
		 * as 34 is the minimal safe length to represent for a 64bit address range.
		 * For example: "1234567890120000-1234567890123000" (NOTE: there is a '\0' in the end)
		 */
		char lineBuf[100];
		intptr_t lineCursor = 0;
		AddressRange lastMmapRange;
		addressRange_Init(&lastMmapRange, NULL, (ADDRESS)PPG_vmem_pageSize[0]);

		while (TRUE) { /* read-file-loop */
			BOOLEAN lineEnd = FALSE;
			BOOLEAN gotEOF = FALSE;
			bytesRead = omrfile_read(portLibrary, fd, readBuf, sizeof(readBuf));
			if (-1 == bytesRead) {
				break;
			}

			intptr_t readCursor = 0;
			do { /* proc-chars-loop */
				char ch = '\0';
				/*
				 * Scan out a line from 'readBuf', and save the line data to 'lineBuf'.
				 * 1. End the line with '\0'.
				 * 2. Set lineEnd = TRUE when '\n' found.
				 */
				if (0 == bytesRead) {
					gotEOF = TRUE;
					ch = '\n';
				} else {
					ch = readBuf[readCursor];
					readCursor++;
				}
				/*
				 * We only save the beginning chars for each line, because:
				 * 1. we only need the memory range data, it always at the beginning of the lines.
				 * 2. the size of the 'lineBuf' is fixed, we skip the extra chars in order to avoid out-of-bound-writting
				 */
				if (lineCursor < sizeof(lineBuf)) {
					lineBuf[lineCursor] = ch;
					lineCursor++;
				}

				if ('\n' == ch) {
					lineEnd = TRUE;
					lineBuf[lineCursor - 1] = '\0';
				}

				if (lineEnd) { /* proc-line-data */
					AddressRange currentMmapRange;
					addressRange_Init(&currentMmapRange, NULL, NULL);
					Assert_PRT_true(lineBuf[lineCursor - 1] == '\0');

					if (gotEOF) {
						/* We reach the end of the file. */
						addressRange_Init(&currentMmapRange, (ADDRESS)(uintptr_t)(-1), (ADDRESS)(uintptr_t)(-1));
					} else {
						char *next = NULL;
						uintptr_t start = 0;
						uintptr_t end = 0;
						start = (uintptr_t)strtoull(lineBuf, &next, 16);
						if ((ULLONG_MAX == start) && (ERANGE == errno)) {
							dataCorrupt = TRUE;
							break;
						}
						/* skip the '-' */
						next++;
						if (next >= (lineBuf + lineCursor - 1)) {
							dataCorrupt = TRUE;
							break;
						}
						end = (uintptr_t)strtoull(next, &next, 16);
						if ((ULLONG_MAX == end) && (ERANGE == errno)) {
							dataCorrupt = TRUE;
							break;
						}

						currentMmapRange.start = (ADDRESS)start;
						currentMmapRange.end = (ADDRESS)end;
						lineCursor = 0;

						if ((currentMmapRange.start >= currentMmapRange.end) ||
							(currentMmapRange.start < lastMmapRange.end)) {
							dataCorrupt = TRUE;
							break;
						}
					}

					if (currentMmapRange.start == lastMmapRange.end) {
						lastMmapRange.end = currentMmapRange.end;
					} else {
						/* free block found */
						AddressRange freeRange;
						AddressRange intersectAvailable;
						BOOLEAN haveIntersect = FALSE;

						addressRange_Init(&freeRange, lastMmapRange.end, currentMmapRange.start);
						memcpy(&lastMmapRange, &currentMmapRange, sizeof(AddressRange));

#if defined(OMRVMEM_DEBUG)
						printf("block free %p-%p\n", freeRange.start, freeRange.end);
						fflush(stdout);
#endif

						/* check if the free block has intersection with the allowed range */
						haveIntersect = addressRange_Intersect(&allowedRange, &freeRange, &intersectAvailable);
						if (haveIntersect && addressRange_Width(&intersectAvailable) >= byteAmount) {
#if defined(OMRVMEM_DEBUG)
							printf("intersection %p-%p\n", intersectAvailable.start, intersectAvailable.end);
							fflush(stdout);
#endif
							memcpy(&lastAvailableRange, &intersectAvailable, sizeof(AddressRange));
							matchFound = TRUE;
							strictMatchFound = TRUE;
							if (FALSE == reverse) {
								break;
							}
						} else if (!strictRange && (addressRange_Width(&freeRange) >= byteAmount)) {
							/* This range is large enough and the caller isn't being strict.
							 * Remember the first such range when low addresses are preferred,
							 * or the last otherwise.
							 */
							if (reverse || !matchFound) {
#if defined(OMRVMEM_DEBUG)
								printf("considering %p-%p\n", freeRange.start, freeRange.end);
								fflush(stdout);
#endif
								memcpy(&lastAvailableRange, &freeRange, sizeof(AddressRange));
								matchFound = TRUE;
							}
						}
					}
					lineEnd = FALSE;
				} /* end proc-line-data */
			} while (readCursor < bytesRead); /* end proc-chars-loop */

			if ((FALSE == reverse) && strictMatchFound) {
				break;
			}

			if (gotEOF || dataCorrupt) {
				break;
			}
		} /* end read-file-loop */
		omrfile_close(portLibrary, fd);
	} else {
		dataCorrupt = TRUE;
	}

	if (dataCorrupt) {
		return NULL;
	}

	if (matchFound) {
		if (FALSE == reverse) {
			return lastAvailableRange.start;
		} else {
			return lastAvailableRange.end - byteAmount;
		}
	} else {
		return NULL;
	}
}

void
omrvmem_shutdown(struct OMRPortLibrary *portLibrary)
{
#if defined(OMR_PORT_NUMA_SUPPORT)
	if (NULL != portLibrary->portGlobals) {
		PPG_numa_platform_supports_numa = 0;
	}
#endif
}

int32_t
omrvmem_startup(struct OMRPortLibrary *portLibrary)
{
	vmem_hugepage_info_t vmem_page_info;

	/* clear page info data, this has the effect of starting off in a standard state */
	memset(&vmem_page_info, 0x00, sizeof(vmem_hugepage_info_t));
	get_hugepages_info(portLibrary, &vmem_page_info);

	/* 0 terminate the table */
	memset(PPG_vmem_pageSize, 0, OMRPORT_VMEM_PAGESIZE_COUNT * sizeof(uintptr_t));
	memset(PPG_vmem_pageFlags, 0, OMRPORT_VMEM_PAGESIZE_COUNT * sizeof(uintptr_t));

	/* First the default page size */
	PPG_vmem_pageSize[0] = (uintptr_t)sysconf(_SC_PAGESIZE);
	PPG_vmem_pageFlags[0] = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;

	/* Now the large pages */
	if (vmem_page_info.enabled) {
		PPG_vmem_pageSize[1] = vmem_page_info.page_size;
		PPG_vmem_pageFlags[1] = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;
	}

#if defined(OMR_PORT_NUMA_SUPPORT)
	if (0 == initializeNumaGlobals(portLibrary)) {
		PPG_numa_platform_supports_numa = 1;
	} else {
		PPG_numa_platform_supports_numa = 0;
		omrthread_numa_set_enabled(FALSE); /* Disabling thread NUMA as well */
	}
#endif

	/* set default value to advise OS about vmem that is no longer needed */
	portLibrary->portGlobals->vmemAdviseOSonFree = 1;

	/* set value to advise OS about vmem to consider for Transparent HugePage (Only for Linux) */
	portLibrary->portGlobals->vmemEnableMadvise = get_transparent_hugepage_info(portLibrary);

	return 0;
}

void *
omrvmem_commit_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier)
{
	void *rc = NULL;
	Trc_PRT_vmem_omrvmem_commit_memory_Entry(address, byteAmount);

	if (rangeIsValid(identifier, address, byteAmount)) {
		ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(address, identifier->pageSize);
		ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(byteAmount, identifier->pageSize);

		/* Default page size */
		if (PPG_vmem_pageSize[0] == identifier->pageSize ||
			0 != (identifier->mode & OMRPORT_VMEM_MEMORY_MODE_EXECUTE)
		) {
			if (0 == mprotect(address, byteAmount, get_protectionBits(identifier->mode))) {
#if defined(OMRVMEM_DEBUG)
				printf("\t\tomrvmem_commit_memory called mprotect, returning %p\n", address);
				fflush(stdout);
#endif
				rc = address;
			} else {
				Trc_PRT_vmem_omrvmem_commit_memory_mprotect_failure(errno);
				portLibrary->error_set_last_error(portLibrary,  errno, OMRPORT_ERROR_VMEM_OPFAILED);
			}
		} else if (PPG_vmem_pageSize[1] == identifier->pageSize) {
			rc = address;
		}
	} else {
		Trc_PRT_vmem_omrvmem_commit_memory_invalidRange(identifier->address, identifier->size, address, byteAmount);
		portLibrary->error_set_last_error(portLibrary,  -1, OMRPORT_ERROR_VMEM_INVALID_PARAMS);
	}

#if defined(OMRVMEM_DEBUG)
	printf("\t\tomrvmem_commit_memory returning %p\n", rc);
	fflush(stdout);
#endif
	Trc_PRT_vmem_omrvmem_commit_memory_Exit(rc);
	return rc;
}

intptr_t
omrvmem_decommit_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier)
{
	intptr_t result = -1;

	Trc_PRT_vmem_omrvmem_decommit_memory_Entry(address, byteAmount);

	if (portLibrary->portGlobals->vmemAdviseOSonFree == 1) {
		if (rangeIsValid(identifier, address, byteAmount)) {
			ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(address, identifier->pageSize);
			ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(byteAmount, identifier->pageSize);

			if (byteAmount > 0) {
				if (identifier->allocator == OMRPORT_VMEM_RESERVE_USED_MMAP) {
					result  = (intptr_t)madvise((void *)address, (size_t) byteAmount, MADV_DONTNEED);
				} else {
					/* need to determine what to use in the case of shmat/shmget, till then return success */
					result = 0;
				}

				if (0 != result) {
					Trc_PRT_vmem_omrvmem_decommit_memory_failure(errno, address, byteAmount);
				}
			} else {
				/* nothing to de-commit. */
				Trc_PRT_vmem_decommit_memory_zero_pages();
				result = 0;
			}
		} else {
			result = -1;
			Trc_PRT_vmem_omrvmem_decommit_memory_invalidRange(identifier->address, identifier->size, address, byteAmount);
			portLibrary->error_set_last_error(portLibrary,  result, OMRPORT_ERROR_VMEM_INVALID_PARAMS);
		}
	} else {
		if (!rangeIsValid(identifier, address, byteAmount)) {
			result = -1;
			Trc_PRT_vmem_omrvmem_decommit_memory_invalidRange(identifier->address, identifier->size, address, byteAmount);
			portLibrary->error_set_last_error(portLibrary,  result, OMRPORT_ERROR_VMEM_INVALID_PARAMS);
		} else {
			ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(address, identifier->pageSize);
			ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(byteAmount, identifier->pageSize);

			/* JVM is not allowed to decommit, just return success */
			Trc_PRT_vmem_decommit_memory_not_allowed(portLibrary->portGlobals->vmemAdviseOSonFree);
			result = 0;
		}
	}
	Trc_PRT_vmem_omrvmem_decommit_memory_Exit(result);
	return result;
}

int32_t
omrvmem_free_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier)
{
	int32_t ret = -1;
	OMRMemCategory *category = identifier->category;
	/* CMVC 180372 - Some users store the identifier in the allocated memory.
	 * We therefore store the allocator in a temp, clear the identifier then
	 * free the memory
	 */
	uintptr_t allocator = identifier->allocator;
	int fd = identifier->fd;
	uintptr_t mode = identifier->mode;
	Trc_PRT_vmem_omrvmem_free_memory_Entry(address, byteAmount);

	/* CMVC 180372 - Identifier must be cleared before memory is freed, see comment above */
	update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);

	/* Default page Size */
	if (OMRPORT_VMEM_RESERVE_USED_SHM != allocator) {
		ret = (int32_t)munmap(address, (size_t)byteAmount);
	} else {
		ret = (int32_t)shmdt(address);
	}

	if((mode & OMRPORT_VMEM_MEMORY_MODE_SHARE_FILE_OPEN) && fd != -1) {
		close(fd);
	}

	omrmem_categories_decrement_counters(category, byteAmount);

	Trc_PRT_vmem_omrvmem_free_memory_Exit(ret);
	return ret;
}

int32_t
omrvmem_vmem_params_init(struct OMRPortLibrary *portLibrary, struct J9PortVmemParams *params)
{
	memset(params, 0, sizeof(struct J9PortVmemParams));
	params->startAddress = NULL;
	params->endAddress = OMRPORT_VMEM_MAX_ADDRESS;
	params->byteAmount = 0;
	params->pageSize = PPG_vmem_pageSize[0];
	params->pageFlags = PPG_vmem_pageFlags[0];
	params->mode = OMRPORT_VMEM_MEMORY_MODE_READ | OMRPORT_VMEM_MEMORY_MODE_WRITE;
	params->options = 0;
	params->category = OMRMEM_CATEGORY_UNKNOWN;
	params->alignmentInBytes = 0;
	return 0;
}

void *
omrvmem_reserve_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier, uintptr_t mode, uintptr_t pageSize, uint32_t category)
{
	struct J9PortVmemParams params;
	omrvmem_vmem_params_init(portLibrary, &params);
	if (NULL != address) {
		params.startAddress = address;
		params.endAddress = address;
	}
	params.byteAmount = byteAmount;
	params.mode = mode;
	params.pageSize = pageSize;
	params.pageFlags = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;
	params.options = 0;
	params.category = category;
	return portLibrary->vmem_reserve_memory_ex(portLibrary, identifier, &params);
}

void *
omrvmem_reserve_memory_ex(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, struct J9PortVmemParams *params)
{
	void *memoryPointer = NULL;
	OMRMemCategory *category = omrmem_get_category(portLibrary, params->category);

	Trc_PRT_vmem_omrvmem_reserve_memory_Entry_replacement(params->startAddress, params->byteAmount, params->pageSize);

	Assert_PRT_true(params->startAddress <= params->endAddress);
	ASSERT_VALUE_IS_PAGE_SIZE_ALIGNED(params->byteAmount, params->pageSize);

	/* Invalid input */
	if (0 == params->pageSize) {
		update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
		Trc_PRT_vmem_omrvmem_reserve_memory_invalid_input();
	} else if (PPG_vmem_pageSize[0] == params->pageSize) {
		uintptr_t alignmentInBytes = OMR_MAX(params->pageSize, params->alignmentInBytes);
		uintptr_t minimumGranule = OMR_MIN(params->pageSize, params->alignmentInBytes);

		/* Make sure that the alignment is a multiple of both requested alignment and page size (enforces that arguments are powers of two and, thus, their max is their lowest common multiple) */
		if ((0 == minimumGranule) || (0 == (alignmentInBytes % minimumGranule))) {
			memoryPointer = getMemoryInRangeForDefaultPages(portLibrary, identifier, category, params->byteAmount, params->startAddress, params->endAddress, alignmentInBytes, params->options, params->mode);
		}
	} else if (PPG_vmem_pageSize[1] == params->pageSize) {
		uintptr_t largePageAlignmentInBytes = OMR_MAX(params->pageSize, params->alignmentInBytes);
		uintptr_t largePageMinimumGranule = OMR_MIN(params->pageSize, params->alignmentInBytes);

		/* Make sure that the alignment is a multiple of both requested alignment and page size (enforces that arguments are powers of two and, thus, their max is their lowest common multiple) */
		if ((0 == largePageMinimumGranule) || (0 == (largePageAlignmentInBytes % largePageMinimumGranule))) {
			memoryPointer = reserveLargePages(portLibrary, identifier, category, params->byteAmount, params->startAddress, params->endAddress, params->pageSize, largePageAlignmentInBytes, params->options, params->mode);
		}
		if (NULL == memoryPointer) {
			/* If strict page size flag is not set try again with default page size */
			if (OMR_ARE_NO_BITS_SET(params->options, OMRPORT_VMEM_STRICT_PAGE_SIZE)) {
#if defined(OMRVMEM_DEBUG)
				printf("\t\t\tNULL == memoryPointer, reverting to default pages\n");
				fflush(stdout);
#endif
				uintptr_t defaultPageSize = PPG_vmem_pageSize[0];
				uintptr_t alignmentInBytes = OMR_MAX(defaultPageSize, params->alignmentInBytes);
				uintptr_t minimumGranule = OMR_MIN(defaultPageSize, params->alignmentInBytes);

				/* Make sure that the alignment is a multiple of both requested alignment and page size (enforces that arguments are powers of two and, thus, their max is their lowest common multiple) */
				if ((0 == minimumGranule) || (0 == (alignmentInBytes % minimumGranule))) {
					memoryPointer = getMemoryInRangeForDefaultPages(portLibrary, identifier, category, params->byteAmount, params->startAddress, params->endAddress, alignmentInBytes, params->options, params->mode);
				}
			} else {
				update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
			}
		}
	} else {
		/* If the pageSize is not one of the supported page sizes, error */
		update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
		Trc_PRT_vmem_omrvmem_reserve_memory_unsupported_page_size(params->pageSize);
	}
#if defined(OMR_PORT_NUMA_SUPPORT)
	if (NULL != memoryPointer) {
		port_numa_interleave_memory(portLibrary, memoryPointer, params->byteAmount);
	}
#endif

#if defined(OMRVMEM_DEBUG)
	printf("\tomrvmem_reserve_memory_ex(start=%p,end=%p,size=0x%zx,page=0x%zx,options=0x%zx) returning %p\n",
			params->startAddress, params->endAddress, params->byteAmount, params->pageSize, (size_t)params->options, memoryPointer);
	fflush(stdout);
#endif

	Trc_PRT_vmem_omrvmem_reserve_memory_Exit_replacement(memoryPointer, params->startAddress);
	return memoryPointer;
}

static void *
reserveLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t pageSize, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t mode)
{
	/* The address should usually be passed in as NULL in order to let the kernel find and assign a
	 * large enough window of virtual memory
	 *
	 * Requesting HUGETLB pages via shmget has the effect of "reserving" them. They have to be attached to become allocated for use
	 * The execute flags are ignored by shmget
	 */
	key_t addressKey;
	int shmgetFlags = SHM_HUGETLB | IPC_CREAT;
	void *memoryPointer = NULL;

	if (0 != (OMRPORT_VMEM_MEMORY_MODE_READ & mode)) {
		shmgetFlags |= SHM_R;
	}
	if (0 != (OMRPORT_VMEM_MEMORY_MODE_WRITE & mode)) {
		shmgetFlags |= SHM_W;
	}

	addressKey = shmget(IPC_PRIVATE, (size_t) byteAmount, shmgetFlags);
	if (-1 == addressKey) {
		Trc_PRT_vmem_omrvmem_reserve_memory_shmget_failed(byteAmount, shmgetFlags);
	} else {
		memoryPointer = getMemoryInRangeForLargePages(portLibrary, identifier, addressKey, category, byteAmount, startAddress, endAddress, alignmentInBytes, vmemOptions, pageSize, mode);

		/* release when complete, protect from ^C or crash */
		if (0 != shmctl(addressKey, IPC_RMID, NULL)) {
			/* if releasing addressKey fails detach memory to avoid leaks and fail */
			if (NULL != memoryPointer) {
				if (0 != shmdt(memoryPointer)) {
					Trc_PRT_vmem_omrvmem_reserve_memory_shmdt_failed(memoryPointer);
				}
			}
			Trc_PRT_vmem_omrvmem_reserve_memory_failure();
			memoryPointer = NULL;
		}

		if (NULL != memoryPointer) {
			/* Commit memory if required, else return reserved memory */
			if (0 != (OMRPORT_VMEM_MEMORY_MODE_COMMIT & mode)) {
				if (NULL == omrvmem_commit_memory(portLibrary, memoryPointer, byteAmount, identifier)) {
					/* If the commit fails free the memory */
					omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier);
					memoryPointer = NULL;
				}
			}
		}
	}

	if (NULL == memoryPointer) {
		update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
	}

#if defined(OMRVMEM_DEBUG)
	printf("\treserveLargePages returning %p\n", memoryPointer);
	fflush(stdout);
#endif
	return memoryPointer;
}

/**
 * Advise memory to enable use of Transparent HugePages (THP) (Linux Only)
 *
 * Notify kernel that the virtual memory region specified by address and byteAmount should be labelled
 * with MADV_HUGEPAGE, where the khugepage process could promote to THP when possible.
 *
 * @param[in] portLibrary The port library.
 * @param[in] address The starting virtual address.
 * @param[in] byteAmount The amount of bytes after address to map to hugepage.
 *
 * @return 0 on success, OMRPORT_ERROR_VMEM_OPFAILED if an error occurred, or OMRPORT_ERROR_VMEM_NOT_SUPPORTED.
 */
static uintptr_t
adviseHugepage(struct OMRPortLibrary *portLibrary, void* address, uintptr_t byteAmount)
{
#if defined(MAP_ANON) || defined(MAP_ANONYMOUS)
	if (portLibrary->portGlobals->vmemEnableMadvise) {
		uintptr_t start = (uintptr_t)address;
		uintptr_t end = (uintptr_t)address + byteAmount;

		/* Align start and end to be page-size aligned */
		start = start + ((start % PPG_vmem_pageSize[0]) ? (PPG_vmem_pageSize[0] - (start % PPG_vmem_pageSize[0])) : 0);
		end = end - (end % PPG_vmem_pageSize[0]);
		if (start < end) {
			if (0 != madvise((void *)start, end - start, MADV_HUGEPAGE)) {
				return OMRPORT_ERROR_VMEM_OPFAILED;
			}
		}
	}
	return 0;
#else /* defined(MAP_ANON) || defined(MAP_ANONYMOUS) */
	return OMRPORT_ERROR_VMEM_NOT_SUPPORTED;
#endif /* defined(MAP_ANON) || defined(MAP_ANONYMOUS) */
}

uintptr_t
omrvmem_get_page_size(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier)
{
	return identifier->pageSize;
}

uintptr_t
omrvmem_get_page_flags(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier)
{
	return identifier->pageFlags;
}

uintptr_t *
omrvmem_supported_page_sizes(struct OMRPortLibrary *portLibrary)
{
	return PPG_vmem_pageSize;
}

uintptr_t *
omrvmem_supported_page_flags(struct OMRPortLibrary *portLibrary)
{
	return PPG_vmem_pageFlags;
}

/* Get the state of Transparent HugePage (THP) from OS
 *
 * return 0 if THP is set to never/always
 * 		- (always will handled by OS automatically, treat same as never)
 * retrun 1 if THP is set to madvise
 */
static uintptr_t
get_transparent_hugepage_info(struct OMRPortLibrary *portLibrary)
{
	int fd;
	int bytes_read;
	char read_buf[VMEM_MEMINFO_SIZE_MAX];

	fd = omrfile_open(portLibrary, VMEM_TRANSPARENT_HUGEPAGE_FNAME, EsOpenRead, 0);
	if (fd < 0) {
		return 0;
	}

	bytes_read = omrfile_read(portLibrary, fd, read_buf, VMEM_MEMINFO_SIZE_MAX - 1);

	omrfile_close(portLibrary, fd);

	if (bytes_read <= 0) {
		return 0;
	}

	/* make sure its null terminated */
	read_buf[bytes_read] = 0;

	if (!strncmp(read_buf, VMEM_TRANSPARENT_HUGEPAGE_MADVISE, VMEM_TRANSPARENT_HUGEPAGE_MADVISE_LENGTH)) {
		return 1;
	}

	return 0;
}

static uintptr_t
get_hugepages_info(struct OMRPortLibrary *portLibrary, vmem_hugepage_info_t *page_info)
{
	int 	fd;
	int	bytes_read;
	char	*line_ptr, read_buf[VMEM_MEMINFO_SIZE_MAX];

	fd = omrfile_open(portLibrary, VMEM_PROC_MEMINFO_FNAME, EsOpenRead, 0);
	if (fd < 0) {
		return 0;
	}

	bytes_read = omrfile_read(portLibrary, fd, read_buf, VMEM_MEMINFO_SIZE_MAX - 1);
	if (bytes_read <= 0) {
		omrfile_close(portLibrary, fd);
		return 0;
	}

	/* make sure its null terminated */
	read_buf[bytes_read] = 0;

	/** Why this is data is not available via a well defined system call remains a mystery. Meanwhile
	 *  we have to parse /proc/meminfo to figure out how many pages are available/free as well as
	 *  their size
	 */
	line_ptr = read_buf;
	while (line_ptr && *line_ptr) {
		char token_name[128];
		uintptr_t token_value = 0;
		int tokens_assigned = sscanf(line_ptr, "%127s %" SCNuPTR " %*s", token_name, &token_value);

#ifdef LPDEBUG
		portLibrary->tty_printf(portLibrary, VMEM_PROC_MEMINFO_FNAME " => %s [%" PRIuPTR "] %d\n", token_name, token_value, tokens_assigned);
#endif

		if (2 == tokens_assigned) {
			if (!strcmp(token_name, "HugePages_Total:")) {
				page_info->pages_total = token_value;
			} else if (!strcmp(token_name, "HugePages_Free:")) {
				page_info->pages_free = token_value;
			} else if (!strcmp(token_name, "Hugepagesize:")) {
				page_info->page_size = token_value * 1024;	/* value is in kB, convert to bytes */
			}
		}

		line_ptr = strpbrk(line_ptr, "\n");
		if (line_ptr && *line_ptr) {
			line_ptr++;	/* skip the \n if we are not done yet */
		}
	}

	omrfile_close(portLibrary, fd);

	/* "Enable" large page support if the system has been found to be initialized */
	if (page_info->pages_total) {
		page_info->enabled = 1;
	}

	return 1;
}

static void *
default_pageSize_reserve_memory(struct OMRPortLibrary *portLibrary, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier, uintptr_t mode, uintptr_t pageSize, OMRMemCategory *category)
{
	/* This function is cloned in J9SourceUnixJ9VMem (omrvmem_reserve_memory).
	 * Any changes made here may need to be reflected in that version.
	 */
	int fd = -1;
	int ft = -1;
	int flags = 0;
	void *result = NULL;
	int protectionFlags = PROT_NONE;
	BOOLEAN useBackingSharedFile = FALSE;
	BOOLEAN useBackingFile = FALSE;

	Trc_PRT_vmem_default_reserve_entry(address, byteAmount);

	if(mode & OMRPORT_VMEM_MEMORY_MODE_SHARE_FILE_OPEN) {
		flags |= MAP_SHARED;
		useBackingSharedFile = TRUE;
	} else {
#if defined(MAP_ANONYMOUS)
		flags |= MAP_ANONYMOUS | MAP_PRIVATE;
		useBackingSharedFile = FALSE;
#elif defined(MAP_ANON)
		flags |= MAP_ANON | MAP_PRIVATE;
		useBackingSharedFile = FALSE;
#else
		useBackingFile = TRUE;
		flags |= MAP_PRIVATE;
#endif
	}

	if (useBackingSharedFile) {
		char filename[FILE_NAME_SIZE + 1];
		sprintf(filename, "omrvmem_temp_%zu_%09d_%zu",  (size_t)omrtime_current_time_millis(portLibrary), 
								getpid(), 
								(size_t)pthread_self()); 
		fd = shm_open(filename, O_RDWR | O_CREAT, 0600);
		shm_unlink(filename);
		ft = ftruncate(fd, byteAmount);

		if (fd == -1 || ft == -1) {
			Trc_PRT_vmem_default_reserve_failed(address, byteAmount);
			update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
			Trc_PRT_vmem_default_reserve_exit(result, address, byteAmount);
			return result;
		}
	} else if(useBackingFile) {
		fd = portLibrary->file_open(portLibrary, "/dev/zero", EsOpenRead | EsOpenWrite, 0);
	}

	if((fd == -1 && (!useBackingSharedFile && !useBackingFile)) || 
	   (fd != -1 && (useBackingSharedFile || useBackingFile))) {
	
		if (0 != (OMRPORT_VMEM_MEMORY_MODE_COMMIT & mode)) {
			protectionFlags = get_protectionBits(mode);
		} else {
			flags |= MAP_NORESERVE;
		}

		/* do NOT use the MAP_FIXED flag on Linux. With this flag, Linux may return
		 * an address that has already been reserved.
		 */
		result = mmap(address, (size_t)byteAmount, protectionFlags, flags, fd, 0);

		if(useBackingFile) {
			portLibrary->file_close(portLibrary, fd); 
		}
	
		if (MAP_FAILED == result) {
			if(useBackingSharedFile && fd != -1)
				close(fd);
			result = NULL;
		} else {
			/* Update identifier and commit memory if required, else return reserved memory */
			update_vmemIdentifier(identifier, result, result, byteAmount, mode, pageSize, OMRPORT_VMEM_PAGE_FLAG_NOT_USED, OMRPORT_VMEM_RESERVE_USED_MMAP, category, fd);
			omrmem_categories_increment_counters(category, byteAmount);
			if (0 != (OMRPORT_VMEM_MEMORY_MODE_COMMIT & mode)) {
				if (NULL == omrvmem_commit_memory(portLibrary, result, byteAmount, identifier)) {
					/* If the commit fails free the memory */
					omrvmem_free_memory(portLibrary, result, byteAmount, identifier);
					result = NULL;
				}
			}
		}
	}

	if (NULL == result) {
		Trc_PRT_vmem_default_reserve_failed(address, byteAmount);
		update_vmemIdentifier(identifier, NULL, NULL, 0, 0, 0, 0, 0, NULL, -1);
	}

	Trc_PRT_vmem_default_reserve_exit(result, address, byteAmount);
	return result;
}

/**
 *  Maps a contiguous region of memory to double map addresses[] passed in.
 *
 *  @param OMRPortLibrary       *portLibrary                            [in] The portLibrary object
 *  @param void*                addressesOffeset[]                              [in] Addresses to be double mapped
 *  @param uintptr_t            byteAmount                                      [in] Total size to allocate for contiguous block of memory
 *  @param struct J9PortVmemIdentifier *oldIdentifier   [in]  old Identifier containing file descriptor
 *  @param struct J9PortVmemIdentifier *newIdentifier   [out] new Identifier for new block of memory. The structure to be updated
 *  @param uintptr_t            mode,           [in] Access Mode
 *  @paramuintptr_t             pageSize,       [in] onstant describing pageSize
 *  @param OMRMemCategory       *category       [in] Memory allocation category
 */

void *
omrvmem_get_contiguous_region_memory(struct OMRPortLibrary *portLibrary, void* addresses[], uintptr_t addressesCount, uintptr_t addressSize, uintptr_t byteAmount, struct J9PortVmemIdentifier *oldIdentifier, struct J9PortVmemIdentifier *newIdentifier, uintptr_t mode, uintptr_t pageSize, OMRMemCategory *category)
{
	int protectionFlags = PROT_READ | PROT_WRITE;
	int flags = MAP_PRIVATE | MAP_ANON;
	int fd = -1;
	void* contiguousMap = NULL;
	byteAmount = addressesCount * addressSize;
	BOOLEAN successfulContiguousMap = FALSE;
	BOOLEAN shouldUnmapAddr = FALSE;

	if (0 != (OMRPORT_VMEM_MEMORY_MODE_COMMIT & mode)) {
		protectionFlags = get_protectionBits(mode);
	}

	contiguousMap = mmap(
			NULL,
			byteAmount,
			protectionFlags,
			flags,
			fd,
			0);

	if (contiguousMap == MAP_FAILED) {
		contiguousMap = NULL;
		portLibrary->error_set_last_error_with_message(portLibrary, OMRPORT_ERROR_VMEM_OPFAILED, "Failed to map contiguous block of memory");
	} else {
		shouldUnmapAddr = TRUE;
		successfulContiguousMap = TRUE;
		/* Update identifier and commit memory if required, else return reserved memory */
		update_vmemIdentifier(newIdentifier, contiguousMap, contiguousMap, byteAmount, mode, pageSize, OMRPORT_VMEM_PAGE_FLAG_NOT_USED, OMRPORT_VMEM_RESERVE_USED_MMAP, category, -1);
		omrmem_categories_increment_counters(category, byteAmount); 
		if (0 != (OMRPORT_VMEM_MEMORY_MODE_COMMIT & mode)) {
			if (NULL == omrvmem_commit_memory(portLibrary, contiguousMap, byteAmount, newIdentifier)) {
				/* If the commit fails free the memory  */
#if defined(OMRVMEM_DEBUG)
				printf("omrvmem_commit_memory failed at contiguous_region_reserve_memory.\n");
				fflush(stdout);
#endif
				successfulContiguousMap = FALSE;
			}
		}
	}

	/* Perform double mapping  */
	if(contiguousMap != NULL) {
		flags = MAP_SHARED | MAP_FIXED; // Must be shared, SIGSEGV otherwise
		fd = oldIdentifier->fd;
#if defined(OMRVMEM_DEBUG)
		printf("Found %zu arraylet leaves.\n", addressesCount);
		int j = 0;
		for(j = 0; j < addressesCount; j++) {
			printf("addresses[%d] = %p\n", j, addresses[j]);
		}
		printf("Contiguous address is: %p\n", contiguousMap);
		printf("About to double map arraylets. File handle got from old identifier: %d\n", fd);
		fflush(stdout);
#endif
		size_t i;
		for (i = 0; i < addressesCount; i++) {
			void *nextAddress = (void *)(contiguousMap + i * addressSize);
			uintptr_t addressOffset = (uintptr_t)(addresses[i] - (uintptr_t)oldIdentifier->address);
			void *address = mmap(
					nextAddress,
					addressSize,
					protectionFlags,
					flags,
					fd,
					addressOffset);

			if (address == MAP_FAILED) {
				successfulContiguousMap = FALSE;
                                portLibrary->error_set_last_error_with_message(portLibrary, OMRPORT_ERROR_VMEM_OPFAILED, "Failed to double map.");
#if defined(OMRVMEM_DEBUG)
				printf("***************************** errno: %d\n", errno);
				printf("Failed to mmap address[%zu] at mmapContiguous()\n", i);
				fflush(stdout);
#endif
				break;
			} else if (nextAddress != address) {
				successfulContiguousMap = FALSE;
                                portLibrary->error_set_last_error_with_message(portLibrary, OMRPORT_ERROR_VMEM_OPFAILED, "Double Map failed to provide the correct address");
#if defined(OMRVMEM_DEBUG)
				printf("Map failed to provide the correct address. nextAddress %p != %p\n", nextAddress, address);
				fflush(stdout);
#endif
				break;
			}
		}
	}

	if (!successfulContiguousMap) {
		if (shouldUnmapAddr) {
			munmap(contiguousMap, byteAmount);
		}
		contiguousMap = NULL;
	}

	return contiguousMap;
}


/**
 * @internal
 * Update J9PortVmIdentifier structure
 *
 * @param[in] identifier The structure to be updated
 * @param[in] address Base address
 * @param[in] handle Platform specific handle for reserved memory
 * @param[in] byteAmount Size of allocated area
 * @param[in] mode Access Mode
 * @param[in] pageSize Constant describing pageSize
 * @param[in] pageFlags flags for describing page type
 * @param[in] allocator Constant describing how the virtual memory was allocated.
 * @param[in] category Memory allocation category
 */
static void
update_vmemIdentifier(J9PortVmemIdentifier *identifier, void *address, void *handle, uintptr_t byteAmount, uintptr_t mode, uintptr_t pageSize, uintptr_t pageFlags, uintptr_t allocator, OMRMemCategory *category, int fd)
{
	identifier->address = address;
	identifier->handle = handle;
	identifier->size = byteAmount;
	identifier->pageSize = pageSize;
	identifier->pageFlags = pageFlags;
	identifier->mode = mode;
	identifier->allocator = allocator;
	identifier->category = category;
	identifier->fd = fd;
}

static int
get_protectionBits(uintptr_t mode)
{
	int protectionFlags = 0;

	if (0 != (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode)) {
		protectionFlags |= PROT_EXEC;
	}
	if (0 != (OMRPORT_VMEM_MEMORY_MODE_READ & mode)) {
		protectionFlags |= PROT_READ;
	}
	if (0 != (OMRPORT_VMEM_MEMORY_MODE_WRITE & mode)) {
		protectionFlags |= PROT_WRITE;
	}
	if (0 == protectionFlags) {
		protectionFlags = PROT_NONE;
	}

	return protectionFlags;
}

#if defined(OMR_PORT_NUMA_SUPPORT)
void
port_numa_interleave_memory(struct OMRPortLibrary *portLibrary, void  *start, uintptr_t  size)
{
	Trc_PRT_vmem_port_numa_interleave_memory_enter();

	if (1 == PPG_numa_platform_interleave_memory) {
		/* There seem to be variations in different kernel levels regarding how they treat the maxnode argument.
		 * Depending on the system we're running on, it may fail if we specify a mask that is greater than the number of nodes.
		 *
		 * To support both configs, we first try to pass in the count of all bits in the mask, regardless of
		 * how many we're using (the unused ones are zero and will simply be ignored). If that fails, we try
		 * again, this time passing in the actual number of nodes.
		 */
		unsigned long maxnode = sizeof(J9PortNodeMask) * 8;

		if (0 != do_mbind(start, size, MPOL_INTERLEAVE, PPG_numa_mempolicy_node_mask.mask, maxnode, 0)) {
			if (0 != do_mbind(start, size, MPOL_INTERLEAVE, PPG_numa_mempolicy_node_mask.mask, PPG_numa_max_node_bits, 0)) {
				/* record the error in the trace buffer and assert */
				Trc_PRT_vmem_port_numa_interleave_memory_mbind_failed(errno, strerror(errno));

				/* There are cases where mbind() can fail that we are currently not handling. Instead of
				 * asserting, and thereby bringing down the VM in such cases, simply record the above
				 * tracepoint and continue execution, since this isn't a fatal error.
				 *
				 * In one instance on RHEL6, it was observed that the /sys/devices/system/node/ filesystem
				 * had entries for node0 and node4, but setting node0 in the mask would cause the mbind()
				 * call to fail. On this sytem, there was a file /sys/devices/system/node/has_normal_memory
				 * which only listed node 4. It might make sense to parse that file, if it exists, and to
				 * only set the bits for nodes that are listed in that file. Alternatively we could test
				 * each node using mbind() with only the node's bit set, to determine if we should set
				 * that bit in PPG_numa_available_node_mask.mask.
				 */
#if 0
				Assert_PRT_true(FALSE);
#endif
			}
		}
	} else {
		/* numa could not be configured at startup */
		Trc_PRT_vmem_port_numa_numa_support_could_not_be_configured_at_startup();
	}

	Trc_PRT_vmem_port_numa_interleave_memory_exit();
}
#endif /* defined(OMR_PORT_NUMA_SUPPORT) */

void
omrvmem_default_large_page_size_ex(struct OMRPortLibrary *portLibrary, uintptr_t mode, uintptr_t *pageSize, uintptr_t *pageFlags)
{
#if defined(LINUXPPC)
	if (OMRPORT_VMEM_MEMORY_MODE_EXECUTE != (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode))
#endif /* defined(LINUXPPC) */
	{
		/* Note that the PPG_vmem_pageSize is a null-terminated list of page sizes.
		 * There will always be the 0 element (default page size),
		 * so the 1 element will be zero or the default large size where only 2 are supported
		 * (platforms with more complicated logic will use their own special process to determine the best response
		 */
		if (NULL != pageSize) {
			*pageSize = PPG_vmem_pageSize[1];
		}
		if (NULL != pageFlags) {
			*pageFlags = PPG_vmem_pageFlags[1];
		}
	}
#if defined(LINUXPPC)
	else {
		if (NULL != pageSize) {
			/* Return the same page size as used by JIT code cache */
			uintptr_t defaultPageSize = (uintptr_t)sysconf(_SC_PAGESIZE);
			*pageSize = (FOUR_K == defaultPageSize) ? 0 : defaultPageSize;
			if (NULL != pageFlags) {
				*pageFlags = (0 == *pageSize) ? 0 : OMRPORT_VMEM_PAGE_FLAG_NOT_USED;
			}
		} else {
			if (NULL != pageFlags) {
				*pageFlags = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;
			}
		}
	}
#endif /* defined(LINUXPPC) */

	return;
}

intptr_t
omrvmem_find_valid_page_size(struct OMRPortLibrary *portLibrary, uintptr_t mode, uintptr_t *pageSize, uintptr_t *pageFlags, BOOLEAN *isSizeSupported)
{
	uintptr_t validPageSize = *pageSize;
	uintptr_t validPageFlags = *pageFlags;
	uintptr_t defaultLargePageSize = 0;
	uintptr_t defaultLargePageFlags = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;

	Assert_PRT_true_wrapper(OMRPORT_VMEM_PAGE_FLAG_NOT_USED == validPageFlags);

	if (0 != validPageSize) {
#if defined(LINUXPPC)
		/* On Linux PPC, for executable pages, search through the list of supported page sizes only if
		 * - request is for 16M pages, and
		 * - 64 bit system OR (32 bit system AND CodeCacheConsolidation is enabled)
		 */
#if defined(OMR_ENV_DATA64)
		if ((OMRPORT_VMEM_MEMORY_MODE_EXECUTE != (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode)) ||
			(SIXTEEN_M == validPageSize))
#else
		/* Check if the TR_ppcCodeCacheConsolidationEnabled env variable is set.
		 * TR_ppcCodeCacheConsolidationEnabled is a manually set env variable used to indicate
		 * that Code Cache Consolidation is enabled in the JIT.
		 */
		BOOLEAN codeCacheConsolidationEnabled = FALSE;

		if (OMRPORT_VMEM_MEMORY_MODE_EXECUTE == (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode)) {
			if (portLibrary->sysinfo_get_env(portLibrary, "TR_ppcCodeCacheConsolidationEnabled", NULL, 0) != -1) {
				codeCacheConsolidationEnabled = TRUE;
			}
		}
		if ((OMRPORT_VMEM_MEMORY_MODE_EXECUTE != (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode)) ||
			(codeCacheConsolidationEnabled && (SIXTEEN_M == validPageSize)))
#endif /* defined(OMR_ENV_DATA64) */
#endif /* defined(LINUXPPC) */
		{
			uintptr_t pageIndex = 0;
			uintptr_t *supportedPageSizes = portLibrary->vmem_supported_page_sizes(portLibrary);
			uintptr_t *supportedPageFlags = portLibrary->vmem_supported_page_flags(portLibrary);

			for (pageIndex = 0; 0 != supportedPageSizes[pageIndex]; pageIndex++) {
				if ((supportedPageSizes[pageIndex] == validPageSize) &&
					(supportedPageFlags[pageIndex] == validPageFlags)
				) {
					goto _end;
				}
			}
		}
	}

	portLibrary->vmem_default_large_page_size_ex(portLibrary, mode, &defaultLargePageSize, &defaultLargePageFlags);
	if (0 != defaultLargePageSize) {
		validPageSize = defaultLargePageSize;
		validPageFlags = defaultLargePageFlags;
	} else {
#if defined(LINUXPPC)
		if (OMRPORT_VMEM_MEMORY_MODE_EXECUTE == (OMRPORT_VMEM_MEMORY_MODE_EXECUTE & mode)) {
			validPageSize = (uintptr_t)sysconf(_SC_PAGESIZE);
			validPageFlags = OMRPORT_VMEM_PAGE_FLAG_NOT_USED;
		} else
#endif /* defined(LINUXPPC) */
		{
			validPageSize = PPG_vmem_pageSize[0];
			validPageFlags = PPG_vmem_pageFlags[0];
		}
	}

_end:
	/* Since page type is not used, ignore pageFlags when setting isSizeSupported. */
	*isSizeSupported = (*pageSize == validPageSize);
	*pageSize = validPageSize;
	*pageFlags = validPageFlags;

	return 0;
}

/**
 * Allocates memory in specified range using a best effort approach
 * (unless OMRPORT_VMEM_STRICT_ADDRESS flag is used) and returns a pointer
 * to the newly allocated memory. Returns NULL on failure.
 */
static void *
getMemoryInRangeForDefaultPages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t mode)
{
	intptr_t direction = 1;
	void *memoryPointer = NULL;

	/* check allocation direction */
	if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ALLOC_DIR_TOP_DOWN)) {
		direction = -1;
	} else if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ALLOC_DIR_BOTTOM_UP)) {
		direction = 1;
	} else if ((NULL == startAddress) && (OMRPORT_VMEM_MAX_ADDRESS == endAddress)) {
		/* if caller specified the entire address range and does not care about the direction
		 * save time by letting OS choose where to allocate the memory
		 */
		goto allocAnywhere;
	}

	/* check if we should use quick search for fast performance */
	if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ALLOC_QUICK)) {
		void *smartAddress = findAvailableMemoryBlockNoMalloc(portLibrary,
				startAddress, endAddress, byteAmount,
				(1 == direction) ? FALSE : TRUE,
	 			OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_STRICT_ADDRESS) ? TRUE : FALSE);

		if (NULL != smartAddress) {
			/* smartAddress is not NULL: try to get memory there */
			memoryPointer = default_pageSize_reserve_memory(portLibrary, smartAddress, byteAmount, identifier, mode, PPG_vmem_pageSize[0], category);
			if (NULL != memoryPointer) {
				if (OMR_ARE_NO_BITS_SET(vmemOptions, OMRPORT_VMEM_STRICT_ADDRESS)) {
					/* OMRPORT_VMEM_STRICT_ADDRESS is not set: accept whatever we get */
				} else if ((startAddress <= memoryPointer) && (memoryPointer <= endAddress)) {
					/* the address is in the requested range */
				} else {
					/* the address is outside of the range, free it */
					if (0 != omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier)) {
						/* free failed: we fail too */
						return NULL;
					}
					/* try a linear search below */
					memoryPointer = NULL;
				}
			}
		}
		/*
		 * memoryPointer != NULL means that available address was found.
		 * Otherwise, the below logic will continue trying.
		 */
	}

	if (NULL == memoryPointer) {
		AddressIterator iterator;
		void *currentAddress = NULL;
		/* return after first attempt when OMRPORT_VMEM_ADDRESS_HINT is set */
		if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ADDRESS_HINT)) {
			return default_pageSize_reserve_memory(portLibrary, startAddress, byteAmount, identifier, mode, PPG_vmem_pageSize[0], category);
		}
		/* try all addresses within range */
		addressIterator_init(&iterator, startAddress, endAddress, alignmentInBytes, direction);
		while (addressIterator_next(&iterator, &currentAddress)) {
			memoryPointer = default_pageSize_reserve_memory(portLibrary, currentAddress, byteAmount, identifier, mode, PPG_vmem_pageSize[0], category);

			if (NULL != memoryPointer) {
				/* stop if returned pointer is within range */
				if ((startAddress <= memoryPointer) && (memoryPointer <= endAddress)) {
					break;
				}
				if (0 != omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier)) {
					return NULL;
				}
				memoryPointer = NULL;
			}
		}
	}

	/* if strict flag is not set and we did not get any memory, attempt to get memory at any address */
	if (OMR_ARE_NO_BITS_SET(vmemOptions, OMRPORT_VMEM_STRICT_ADDRESS) && (NULL == memoryPointer)) {
allocAnywhere:
		memoryPointer = default_pageSize_reserve_memory(portLibrary, NULL, byteAmount, identifier, mode, PPG_vmem_pageSize[0], category);
	}

	if (NULL == memoryPointer) {
		memoryPointer = NULL;
	} else if (isStrictAndOutOfRange(memoryPointer, startAddress, endAddress, vmemOptions)) {
		/* if strict flag is set and returned pointer is not within range then fail */
		omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier);

		Trc_PRT_vmem_omrvmem_reserve_memory_ex_UnableToAllocateWithinSpecifiedRange(byteAmount, startAddress, endAddress);

		memoryPointer = NULL;
	} else {
		adviseHugepage(portLibrary, memoryPointer, byteAmount);
	}

	return memoryPointer;
}

/**
 * Allocates memory in specified range using a best effort approach
 * (unless OMRPORT_VMEM_STRICT_ADDRESS flag is used) and returns a pointer
 * to the newly allocated memory. Returns NULL on failure.
 */
static void *
getMemoryInRangeForLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, key_t addressKey, OMRMemCategory *category, uintptr_t byteAmount, void *startAddress, void *endAddress, uintptr_t alignmentInBytes, uintptr_t vmemOptions, uintptr_t pageSize, uintptr_t mode)
{
	AddressIterator iterator;
	intptr_t direction = 1;
	void *currentAddress = NULL;
	void *memoryPointer = NULL;

	/* check allocation direction */
	if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ALLOC_DIR_TOP_DOWN)) {
		direction = -1;
	} else if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_ALLOC_DIR_BOTTOM_UP)) {
		direction = 1;
	} else if ((NULL == startAddress) && (OMRPORT_VMEM_MAX_ADDRESS == endAddress)) {
		/* if caller specified the entire address range and does not care about the direction
		 * save time by letting OS choose where to allocate the memory
		 */
		goto allocAnywhere;
	}

	/* prevent while loop from attempting to free memory on first entry */
	memoryPointer = MAP_FAILED;

	/* try all addresses within range */
	addressIterator_init(&iterator, startAddress, endAddress, alignmentInBytes, direction);
	while (addressIterator_next(&iterator, &currentAddress)) {
		/* free previously attached memory and attempt to get new memory */
		if (memoryPointer != MAP_FAILED) {
			if (0 != omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier)) {
				return NULL;
			}
		}

		memoryPointer = allocateMemoryForLargePages(portLibrary, identifier, currentAddress, addressKey, category, byteAmount, pageSize, mode);

		/* stop if returned pointer is within range */
		if ((MAP_FAILED != memoryPointer) && (startAddress <= memoryPointer) && (memoryPointer <= endAddress)) {
			break;
		}
	}

	/* if strict flag is not set and we did not get any memory, attempt to get memory at any address */
	if (OMR_ARE_NO_BITS_SET(vmemOptions, OMRPORT_VMEM_STRICT_ADDRESS) && (MAP_FAILED == memoryPointer)) {
allocAnywhere:
		memoryPointer = allocateMemoryForLargePages(portLibrary, identifier, NULL, addressKey, category, byteAmount, pageSize, mode);
	}

	if (MAP_FAILED == memoryPointer) {
		Trc_PRT_vmem_omrvmem_reserve_memory_shmat_failed2(byteAmount, startAddress, endAddress);
		memoryPointer = NULL;
	} else if (isStrictAndOutOfRange(memoryPointer, startAddress, endAddress, vmemOptions)) {
		/* if strict flag is set and returned pointer is not within range then fail */
		omrvmem_free_memory(portLibrary, memoryPointer, byteAmount, identifier);

		Trc_PRT_vmem_omrvmem_reserve_memory_ex_UnableToAllocateWithinSpecifiedRange(byteAmount, startAddress, endAddress);

		memoryPointer = NULL;
	}

	return memoryPointer;
}

/**
 * Attempts to allocate memory at currentAddress by delegating to the appropriate
 * function depending on whether or not the default page size is being used.
 * Returns pointer to newly allocated memory upon success and if default page size
 * is being used NULL upon failure else -1 upon failure.
 */
static void *
allocateMemoryForLargePages(struct OMRPortLibrary *portLibrary, struct J9PortVmemIdentifier *identifier, void *currentAddress, key_t addressKey, OMRMemCategory *category, uintptr_t byteAmount, uintptr_t pageSize, uintptr_t mode)
{
	void *memoryPointer = shmat(addressKey, currentAddress, 0);

	if (MAP_FAILED != memoryPointer) {
		update_vmemIdentifier(identifier, memoryPointer, (void *)(uintptr_t)addressKey, byteAmount, mode, pageSize, OMRPORT_VMEM_PAGE_FLAG_NOT_USED, OMRPORT_VMEM_RESERVE_USED_SHM, category, -1);
		omrmem_categories_increment_counters(category, byteAmount);
	}

	return memoryPointer;
}

/**
 * Returns TRUE if OMRPORT_VMEM_STRICT_ADDRESS flag is set and memoryPointer
 * is outside of range, else returns FALSE
 */
static BOOLEAN
isStrictAndOutOfRange(void *memoryPointer, void *startAddress, void *endAddress, uintptr_t vmemOptions)
{
	if (OMR_ARE_ANY_BITS_SET(vmemOptions, OMRPORT_VMEM_STRICT_ADDRESS) &&
		((memoryPointer > endAddress) || (memoryPointer < startAddress))) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * Ensures that the requested address and (address + byteAmount) fall
 * within the reserved identifier->address and (identifier->address + identifier->size)
 * @returns TRUE if the range is valid, FALSE otherwise
 */
static BOOLEAN
rangeIsValid(struct J9PortVmemIdentifier *identifier, void *address, uintptr_t byteAmount)
{
	BOOLEAN isValid = FALSE;
	uintptr_t requestedUpperLimit = (uintptr_t)address + byteAmount - 1;

	if (requestedUpperLimit + 1 >= byteAmount) {
		/* Requested range does not wrap around */
		uintptr_t realUpperLimit = (uintptr_t)identifier->address + identifier->size - 1;

		if (((uintptr_t)address >= (uintptr_t)identifier->address) &&
			(requestedUpperLimit <= realUpperLimit)
		) {
			isValid = TRUE;
		}
	}

	return isValid;
}

intptr_t
omrvmem_numa_set_affinity(struct OMRPortLibrary *portLibrary, uintptr_t numaNode, void *address, uintptr_t byteAmount, struct J9PortVmemIdentifier *identifier)
{
	intptr_t result = OMRPORT_ERROR_VMEM_OPFAILED;

	Trc_PRT_vmem_port_numa_set_affinity_enter(numaNode, address, byteAmount);

#if defined(OMR_PORT_NUMA_SUPPORT)
	if ((1 == PPG_numa_platform_supports_numa) && (OMR_ARE_NO_BITS_SET(identifier->mode, OMRPORT_VMEM_NO_AFFINITY))) {
		unsigned long maxnode = sizeof(J9PortNodeMask) * 8;

		if ((numaNode > 0) && (numaNode <= PPG_numa_max_node_bits) && (numaNode <= maxnode)) {
			J9PortNodeMask nodeMask;
			/* The Linux NUMA node indices start at 0 while ours start at 1 */
			unsigned long nodeIndex = numaNode - 1;
			unsigned long wordIndex = nodeIndex / sizeof(nodeMask.mask[0]);
			unsigned long bit = 1 << (nodeIndex % sizeof(nodeMask.mask[0]));

			/* Check if this node actually exists. (The PPG_numa_available_node_mask has all valid nodes set.) */
			if (bit == (PPG_numa_available_node_mask.mask[wordIndex] & bit)) {
				memset(&nodeMask, 0, sizeof(nodeMask));
				nodeMask.mask[wordIndex] |= bit;

				/**
				 *	mbind returns 0 in the case of success, otherwise -1.
				 *
				 */
				if (0 != do_mbind(address, byteAmount, MPOL_PREFERRED, nodeMask.mask, PPG_numa_max_node_bits, 0)) {
					/* record the error in the trace buffer and assert */
					Trc_PRT_vmem_numa_set_affinity_mbind_failed(errno, strerror(errno));
					/**
					 * In the case of an error, assert the failure and exit the VM.
					 * This might not be a fatal error and we could ignore it and continue running just like in port_numa_interleave_memory.
					 * This assert is useful to determine any problems calling mbind and improve the performance by succeeding mbind call.
					 *
					 */
					Assert_PRT_true(FALSE);
				} else {
					result = 0;
				}
			} else {
				result = OMRPORT_ERROR_VMEM_OPFAILED;
				Trc_PRT_vmem_port_numa_set_affinity_node_not_present(numaNode);
			}
		} else {
			result = OMRPORT_ERROR_VMEM_OPFAILED;
			Trc_PRT_vmem_port_numa_set_affinity_node_out_of_range(numaNode);
		}
	} else {
		result = OMRPORT_ERROR_VMEM_OPFAILED;
		Trc_PRT_vmem_port_numa_set_affinity_no_NUMA();
	}
#endif /* OMR_PORT_NUMA_SUPPORT */

	Trc_PRT_vmem_port_numa_set_affinity_exit(result);

	return result;
}

intptr_t
omrvmem_numa_get_node_details(struct OMRPortLibrary *portLibrary, J9MemoryNodeDetail *numaNodes, uintptr_t *nodeCount)
{
	intptr_t result = OMRPORT_ERROR_VMEM_OPFAILED;

#if defined(OMR_PORT_NUMA_SUPPORT)
	/* only try to look-up node details if NUMA is supported */
	if (PPG_numa_platform_supports_numa) {
		DIR *nodes = opendir("/sys/devices/system/node/");
		if (NULL == nodes) {
			/* couldn't find the directory - this would be inconsistent since PPG_numa_platform_supports_numa requires seeing this directory, in the current implementation */
			result = OMRPORT_ERROR_VMEM_OPFAILED;
		} else {
			uintptr_t arraySize = *nodeCount;
			uintptr_t populatedNodeCount = 0;
			struct dirent *node = NULL;

			/* by default, set the SET and CLEAR states to the same value since "default" NUMA mode has all nodes cleared */
			J9MemoryState nodeSetState = J9NUMA_PREFERRED;
			J9MemoryState nodeClearState = nodeSetState;
			switch (PPG_numa_policy_mode) {
			case MPOL_BIND:
				/* BIND is not supposed to permit allocation elsewhere */
				nodeSetState = J9NUMA_ALLOWED;
				nodeClearState = J9NUMA_DENIED;
				break;
			case MPOL_INTERLEAVE:
				/* interleave is allowed to fall back to other nodes */
			case MPOL_PREFERRED:
				/* preferred merely states the intent to favour the given node(s) */
				nodeSetState = J9NUMA_PREFERRED;
				nodeClearState = J9NUMA_ALLOWED;
				break;
			default:
				/* do nothing */
				break;
			}
			if (PPG_numaSyscallNotAllowed) {
				nodeSetState = J9NUMA_DENIED;
				nodeClearState = J9NUMA_DENIED;
			}

			/* walk through the /sys/devices/system/node/ directory to find each individual node */

			/*
			from readdir man page:
			If the end of the directory stream is reached,
			NULL is returned and errno is not changed. If an error occurs,
			NULL is returned and errno is set appropriately.
			*/
			errno = 0;
			while (NULL != (node = readdir(nodes))) {
				unsigned long nodeIndex = 0;
				if (1 == sscanf(node->d_name, "node%lu", &nodeIndex)) {
					if (nodeIndex < PPG_numa_max_node_bits) {
						DIR *oneNode = NULL;
						char nodeName[sizeof("/sys/devices/system/node/") + NAME_MAX + 1] = "/sys/devices/system/node/";

						strncat(nodeName, node->d_name, NAME_MAX);
						/* walk the directory for this specific node to find its CPUs */
						oneNode = opendir(nodeName);
						if (NULL != oneNode) {
							/* as long as we haven't yet filled the user-provided buffer, populate the entry with the node details */
							if (populatedNodeCount < arraySize) {
								J9MemoryState memoryState = nodeClearState;

								/* find each CPU and check to see if it is part of our global mask */
								uintptr_t allowedCPUCount = 0;
								struct dirent *fileEntry = NULL;
								uint64_t memoryOnNodeInKibiBytes = 0;
								while (NULL != (fileEntry = readdir(oneNode))) {
									unsigned long cpuIndex = 0;
									const char *fileName = fileEntry->d_name;
									if ((1 == sscanf(fileName, "cpu%lu", &cpuIndex)) && (CPU_ISSET(cpuIndex, &PPG_process_affinity))) {
										/* the CPU exists and is in our process-wide affinity so we know that we can use it */
										allowedCPUCount += 1;
									} else if (0 == strcmp(fileName, "meminfo")) {
										/* parse out the memory quantity of this node since a 0-byte node will need to be reported as "J9NUMA_DENIED" to ensure that the mbind won't fail with EINVAL */
										char memInfoFileName[sizeof(nodeName) + NAME_MAX + 1];
										FILE *memInfoStream = 0;

										memset(memInfoFileName, 0x0, sizeof(memInfoFileName));
										strncpy(memInfoFileName, nodeName, sizeof(memInfoFileName));
										strncat(memInfoFileName, "/meminfo", NAME_MAX);
										memInfoStream = fopen(memInfoFileName, "r");
										if (NULL != memInfoStream) {
											if (1 != fscanf(memInfoStream, " Node %*u MemTotal: %" SCNu64 " kB", &memoryOnNodeInKibiBytes)) {
												/* failed to find string */
												memoryOnNodeInKibiBytes = 0;
											}
											fclose(memInfoStream);
										}
									}
								}
								closedir(oneNode);

								/* check to see if this node was described in the default policy for the process */
								if (0 != (PPG_numa_mempolicy_node_mask.mask[nodeIndex / 8] & (1 << nodeIndex % 8))) {
									memoryState = nodeSetState;
								}
								/* if we didn't find any memory on this node, don't let us use it */
								if (0 == memoryOnNodeInKibiBytes) {
									memoryState = J9NUMA_DENIED;
								}
								numaNodes[populatedNodeCount].j9NodeNumber = nodeIndex + 1;
								numaNodes[populatedNodeCount].memoryPolicy = memoryState;
								numaNodes[populatedNodeCount].computationalResourcesAvailable = allowedCPUCount;
							}
							populatedNodeCount += 1;
						}
					}
				}
			}
			//when error occurs, node is null and errno is changed (!=0)
			if(node == NULL && errno != 0)
			{
				result = OMRPORT_ERROR_VMEM_OPFAILED;
			}
			else
			{
				/* save back how many entries we saw (the caller knows that the number of entries we populated is the minimum of how many we were given and how many we found) */
				*nodeCount = populatedNodeCount;
				result = 0;
				closedir(nodes);
			}
		}
	}
#endif /* OMR_PORT_NUMA_SUPPORT */

	return result;
}

int32_t
omrvmem_get_available_physical_memory(struct OMRPortLibrary *portLibrary, uint64_t *freePhysicalMemorySize)
{
	int64_t pageSize = sysconf(_SC_PAGESIZE);
	int64_t availablePages = 0;
	uint64_t result = 0;
	if (pageSize < 0)  {
		intptr_t sysconfError = (intptr_t)errno;
		Trc_PRT_vmem_get_available_physical_memory_failed("pageSize", sysconfError);
		return OMRPORT_ERROR_VMEM_OPFAILED;
	}
	availablePages = sysconf(_SC_AVPHYS_PAGES);
	if (availablePages < 0)  {
		intptr_t sysconfError = (intptr_t)errno;
		Trc_PRT_vmem_get_available_physical_memory_failed("availablePages", sysconfError);
		return OMRPORT_ERROR_VMEM_OPFAILED;
	}
	result =  pageSize * availablePages;
	*freePhysicalMemorySize = result;
	Trc_PRT_vmem_get_available_physical_memory_result(result);
	return 0;
}

int32_t
omrvmem_get_process_memory_size(struct OMRPortLibrary *portLibrary, J9VMemMemoryQuery queryType, uint64_t *memorySize)
{
	int32_t result = OMRPORT_ERROR_VMEM_OPFAILED;
	int64_t pageSize = -1;
	Trc_PRT_vmem_get_process_memory_enter((int32_t)queryType);
	pageSize = sysconf(_SC_PAGESIZE);
	if (pageSize <= 0)  {
		intptr_t sysconfError = (intptr_t)errno;
		Trc_PRT_vmem_get_process_memory_failed("pageSize", sysconfError);
		result = OMRPORT_ERROR_VMEM_OPFAILED;
	} else {
		char *statFilename = "/proc/self/statm";
		FILE *statmStream = fopen(statFilename, "r");
		if (NULL != statmStream) {
			unsigned long programSize = 0;
			unsigned long residentSize = 0;
			unsigned long sharedSize = 0;
			int parsedFields = fscanf(statmStream, "%lu %lu %lu", &programSize, &residentSize, &sharedSize);
			if (3 == parsedFields) {
				result = 0;
				switch (queryType) {
				case OMRPORT_VMEM_PROCESS_PHYSICAL:
					*memorySize = (uint64_t)(residentSize * pageSize);
					break;
				case OMRPORT_VMEM_PROCESS_PRIVATE:
					*memorySize = (uint64_t)((programSize - sharedSize) * pageSize);
					break;
				case OMRPORT_VMEM_PROCESS_VIRTUAL:
					*memorySize = (uint64_t)(programSize * pageSize);
					break;
				default:
					Trc_PRT_vmem_get_process_memory_failed("invalid query", 0);
					result = OMRPORT_ERROR_VMEM_OPFAILED;
					break;
				}
			} else {
				Trc_PRT_vmem_get_process_memory_failed("format error in statm", 0);
				result = OMRPORT_ERROR_VMEM_OPFAILED;
			}
			fclose(statmStream);
		} else {
			Trc_PRT_vmem_get_process_memory_failed(statFilename, 0);
			result = OMRPORT_ERROR_VMEM_OPFAILED;
		}
	}
	Trc_PRT_vmem_get_process_memory_exit(result, *memorySize);
	return result;
}

static void
addressIterator_init(AddressIterator *iterator, ADDRESS minimum, ADDRESS maximum, uintptr_t alignment, intptr_t direction)
{
	uintptr_t multiple = 0;
	ADDRESS first = NULL;

	Assert_PRT_true(minimum <= maximum);
	/* alignment must be a power of two */
	Assert_PRT_true((0 != alignment) && (0 == (alignment & (alignment - 1))));

#if defined(OMRVMEM_DEBUG)
	printf("addressIterator_init(%p,%p,0x%zx,%ld)\n", minimum, maximum, (size_t)alignment, direction);
	fflush(stdout);
#endif

	if (direction > 0) {
		if ((uintptr_t)minimum < alignment) {
			multiple = 1;
		} else {
			multiple = ((uintptr_t)minimum + alignment - 1) / alignment;
		}
		first = (ADDRESS)(multiple * alignment);
		if (first > maximum) {
			first = NULL;
		}
	} else {
		multiple = ((uintptr_t)maximum) / alignment;
		first = (ADDRESS)(multiple * alignment);
		if (first < minimum) {
			first = NULL;
		}
	}

	iterator->minimum = minimum;
	iterator->maximum = maximum;
	iterator->alignment = alignment;
	iterator->direction = direction;
	iterator->next = first;
}

static BOOLEAN
addressIterator_next(AddressIterator *iterator, ADDRESS *address)
{
	BOOLEAN hasNext = FALSE;

	if (NULL != iterator->next) {
		uintptr_t step = iterator->alignment;

		*address = iterator->next;
		hasNext = TRUE;

		if (iterator->direction > 0) {
			if (iterator->maximum - iterator->next >= step) {
				iterator->next += step;
			} else {
				iterator->next = NULL;
			}
		} else {
			if (iterator->next - iterator->minimum >= step) {
				iterator->next -= step;
			} else {
				iterator->next = NULL;
			}
		}
	}

#if defined(OMRVMEM_DEBUG)
	if (hasNext) {
		printf("addressIterator_next() -> %p\n", *address);
	} else {
		printf("addressIterator_next() -> done\n");
	}
	fflush(stdout);
#endif

	return hasNext;
}

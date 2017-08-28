/*
 * Wrapper for communication between valgrind and GC.
 * 
 * Contributor:
 *    Varun Garg - initial implementation and documentation
*/

#ifndef _MEMCHECK_WRAPPER_H_
#define _MEMCHECK_WRAPPER_H_

#include <set>
#include "stdint.h"

#if 0
#define VALGRIND_REQUEST_LOGS
#endif

class MM_GCExtensionsBase;

/**
 * Create memory pool and store its address.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 * @param[in] poolAddr address to refer to memory pool.
 *
*/
void valgrindCreateMempool(MM_GCExtensionsBase *extensions,uintptr_t poolAddr);

/**
 * Destroy memory pool.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 *
*/
void valgrindDestroyMempool(MM_GCExtensionsBase *extensions);

/**
 * Allocate Object in memory pool.
 * Object Will be automatically accessable after using this request.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 * @param[in] baseAddress starting address of the object.
 * @param[in] size size of the object.
 *
*/
void valgrindMempoolAlloc(MM_GCExtensionsBase *extensions, uintptr_t baseAddress, uintptr_t size);

/**
 * Mark a address range as defined (accessable).
 *
 * @param[in] starting address of the range.
 * @param[in] size size of the range.
 *
*/
void valgrindMakeMemDefined(uintptr_t address, uintptr_t size);

/**
 * Mark a address range as noaccess (unaccessable).
 *
 * @param[in] starting address of the range.
 * @param[in] size size of the range.
 *
*/
void valgrindMakeMemNoaccess(uintptr_t address, uintptr_t size);

/**
 * Free objects in given range from memory pool
 * Objects will become unaccessable after this request.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 * @param[in] baseAddress starting address of the range.
 * @param[in] size size of the range.
 *
*/
void valgrindClearRange(MM_GCExtensionsBase *extensions, uintptr_t baseAddress, uintptr_t size);

/**
 * Free a object from memory pool
 * Object will become unaccessable after this request.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 * @param[in] baseAddress starting address of the range.
 *
*/
void valgrindFreeObject(MM_GCExtensionsBase *extensions, uintptr_t baseAddress);

/**
 * Check if object exists in memory pool.
 *
 * @param[in] extensions pointer to MM_GCExtensionsBase.
 * @param[in] baseAddress starting address of the range.
 *
 * @return boolean: true if it exists.
 *
*/
bool valgrindCheckObjectInPool(MM_GCExtensionsBase *extensions, uintptr_t baseAddress);

#endif /* _MEMCHECK_WRAPPER_H_ */
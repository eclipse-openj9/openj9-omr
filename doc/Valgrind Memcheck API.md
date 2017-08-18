# Valgrind Memchek API

## About Memcheck
Memcheck is a memory error detector. It can be used to detect memory leaks in objects used in languages built with OMR.

## Need for using API
Valgrind monitors system calls for creation deletion of objects and also memory operations using its own synthetic CPU to detect memory errors. 

For general purpose we don't need it's API but in OMR's Garbage collector we are using custom memory heaps and allocate objects inside it. Valgrind will see the entire heaps as single objects, ignoring the individuality of objects inside it requested by user. This prevents valgrind to show errors when objects leak outside their size or even write over others as long as they are accessing area inside heap.

## API requests used
We have used following valgrind API requests:

1.  `VALGRIND_MAKE_MEM_NOACCESS(lowAddress, size)` - This marks address ranges as completely inaccessible. This is mainly used when a new range added to the heap in `MM_HeapVirtualMemory::heapAddRange`.

2. `VALGRIND_MAKE_MEM_DEFINED(lowAddress, size)` - This marks range as accessable, mainly used when we need to access some area of heap that is unallocated (NOACCESS) and then mark it back with `VALGRIND_MAKE_MEM_NOACCESS`

3. `VALGRIND_MAKE_MEM_UNDEFINED(lowAddress, size)` - This marks address ranges as accessible but containing undefined data. This request is used in `MM_HeapVirtualMemory::heapRemoveRange` to exit the heap as it would have been earlier.

4. `VALGRIND_CREATE_MEMPOOLL(pool, rzB, is_zeroed)` - pool provides the anchor address for our memory pool. We have set it to  `handle->getMemoryBase(),` in `MM_MemoryManager::newInstance` and saved it to `valgrindMempoolAddr` in extensions base to identify the mempool whenever we add or remove objects.

5. `VALGRIND_MEMPOOL_ALLOC(pool, addr, size)` This request is used to inform valgrind that object of `size` has been allocated at `addr` inside `pool`.  This also marks the object as `DEFINED`. The request is added to `allocateAndInitializeObject(OMR_VMThread *omrVMThread)`.

6. `VALGRIND_MEMPOOL_FREE(pool, addr)`. This request is used to inform valgrind that object at `addr` has been freed from the `pool`.  This also marks the object as `NOACCESS`. The request is to be used in `MM_SweepPoolManagerAddressOrderedListBase::addFreeMemory(env, *sweepChunk, *address, size)`

## Different Ways of freeing objects in Valgrind.

API request `VALGRIND_MEMPOOL_FREE(pool, addr)` requires us to have base address of object that we are attempting to free. GC keeps record of heap areas that are available or free. It is informed by `MM_MarkingDelegate::scanRoots` in the target language implementation of the root objects that are alive, but it does not track objects that are even allocated so even if we have a range of non-marked objects, it is not possible to calculate the ones that are dead (to be freed).

There are 3 workarounds for this issue.

1. Using `MM_MarkingDelegate::masterCleanupAfterGC`. This is simplest workaround. Target language has entire object table, and this method is called after GC cycle has been completed with a record of marked objects. While it is freeing the dead objects from object table we are also requesting API. **Disadvantage:** request should have been made inside the GC and not by the user. This is currently in pull request
 
 2. Using a list to store allocated objects. This workaround allows us to free objects from GC directly. **Disadvantage** It works 99.99& (a few bugs are left) but we were not doing storing this list till now, it feels a bit wrong to use it just for freeing a range that we already know.

 3. Adding new valgrind API: Valgrind already has the list of allocated objects, so it should be possible for it to find the objects in a range and also free them. Andrew Young has written a patch for this and it is in their mailing lists for review. https://sourceforge.net/p/valgrind/mailman/message/35996446/ . This works from the GC and also does not require to have specific addresses. This is best solution, but until it is merged in their code and a build is out, it will be a bit difficult to use.

 ## Usage

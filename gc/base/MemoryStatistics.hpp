#if !defined(OMR_GC_MEMORYSTATISTICS_HPP_)
#define OMR_GC_MEMORYSTATISTICS_HPP_

#include "omrcfg.h"
#include "AllocationCategory.hpp"
#include <stdint.h>

struct OMR_GC_MemoryStatistics {
	OMR::GC::AllocationCategory::Enum category;
	uintptr_t allocated;
	uintptr_t highwater;
};

typedef OMR_GC_MemoryStatistics MM_MemoryStatistics;

#endif /* OMR_GC_MEMORYSTATISTICS_HPP_ */

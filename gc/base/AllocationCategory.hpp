#if !defined(OMR_GC_ALLOCATIONCATEGORY_HPP_)
#define OMR_GC_ALLOCATIONCATEGORY_HPP_

namespace OMR {
namespace GC {

/**
 * Forge allocation categories.
 */
struct AllocationCategory {
	enum Enum {
		FIXED = 0,           /** Memory that is a fixed cost of running the garbage collector (e.g. memory for GCExtensions) */
		WORK_PACKETS,        /** Memory that is used for work packets  */
		REFERENCES,          /** Memory that is used to track soft, weak, and phantom references */
		FINALIZE,            /** Memory that is used to track and finalize objects */
		DIAGNOSTIC,          /** Memory that is used to track gc behaviour (e.g. gc check, verbose gc) */
		REMEMBERED_SET,      /** Memory that is used to track the remembered set */
		GC_HEAP,             /** Memory that is used for the GC's heap */
		JAVA_HEAP = GC_HEAP, /** Java alias for GC_HEAP */
		OTHER,               /** Memory that does not fall into any of the above categories */

		/* Must be last, do not use this category! */
		CATEGORY_COUNT
	};
};

} /* namespace GC */
} /* namespace OMR */

typedef OMR::GC::AllocationCategory MM_AllocationCategory;

#endif /* OMR_GC_ALLOCATIONCATEGORY_HPP_ */

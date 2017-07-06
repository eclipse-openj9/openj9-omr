#if !defined(DDR_UNORDERED_MAP)
#define DDR_UNORDERED_MAP

#include "ddr/config.hpp"

#if defined(OMR_HAVE_CXX11)
#include <unordered_map>

#elif defined(OMR_HAVE_TR1)
#include <unordered_map>
namespace std {
using std::tr1::hash;
using std::tr1::unordered_map;
} /* namespace std */

#else
#error "Need std::unordered_map defined in TR1 or C++11."
#endif /* OMR_HAVE_TR1 */

#endif /* DDR_UNORDERED_MAP */

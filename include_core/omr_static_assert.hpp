#if !defined(OMR_STATIC_ASSERT_HPP_)
#define OMR_STATIC_ASSERT_HPP_

#include "omrcfg.h"

/**
 * OMR_STATIC_ASSERT(assert)
 * OMR_STATIC_ASSERT_MSG(assert, msg)
 *
 * At compile time, assert that the condition `assert` evaluates to true. If
 * `assert` evaluates to false or zero, emit a compile time error. Use
 * `OMR_STATIC_ASSERT` to test for properties about the code that must always be
 * true. Especially useful for ensuring correctness of the glue.
 *
 * `assert` must be a constexpr evaluating to bool.
 */
#if defined(OMR_HAVE_CXX11_STATIC_ASSERT)
#  define OMR_STATIC_ASSERT(assert) static_assert(assert, #assert)
#  define OMR_STATIC_ASSERT_MSG(assert, msg) static_assert(assert, msg)
#else
/* disable */
#define OMR_STATIC_ASSERT(assert)
#define OMR_STATIC_ASSERT_MSG(assert, msg)
#endif

#endif /* defined(OMR_STATIC_ASSERT_HPP_) */

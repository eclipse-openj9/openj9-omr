#if !defined(OMR_STATIC_ASSERT_HPP_)
#define OMR_STATIC_ASSERT_HPP_

#include "omrcfg.h"

/**
 * OMR_TOKEN_PASTE(token, token)
 *
 * Expand both tokens and paste them together, forming a new C token.
 */
#define OMR_TOKEN_PASTE_(a, b) a ## b
#define OMR_TOKEN_PASTE(a, b) OMR_TOKEN_PASTE_(a, b)

/**
 * OMR_UNIQUE
 *
 * Expands to a unique value. May be unsafe to use multiple times on a single
 * line. Use with OMR_TOKEN_PASTE to ensure correct pasting.
 */
#if defined(__COUNTER__)
#  define OMR_UNIQUE __COUNTER__
#else
#  define OMR_UNIQUE __LINE__
#endif

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
#  define OMR_STATIC_ASSERT(assert) enum { OMR_TOKEN_PASTE(OMR_ASSERT_FAIL, OMR_UNIQUE) = 1/(int)(!!(assert)) }
#  define OMR_STATIC_ASSERT_MSG(assert, msg) OMR_STATIC_ASSERT(assert)
#endif

#endif /* defined(OMR_STATIC_ASSERT_HPP_) */

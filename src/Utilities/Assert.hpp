#pragma once

// NOLINTBEGIN cppcoreguidelines-macro-usage
#ifdef NDEBUG
#define POKESIM_ASSERT(condition, message) (void(0))
#define POKESIM_ASSERT_NM(condition) (void(0))
#define POKESIM_ASSERT_FAIL(message) (void(0))
#else
#include <cassert>
#include <cstdint>

namespace pokesim::debug {
inline uint64_t ASSERT_COUNT = 0;  // NOLINT cppcoreguidelines-avoid-non-const-global-variables
}

#define POKESIM_ASSERT(condition, message) \
  pokesim::debug::ASSERT_COUNT++;          \
  assert((void(message), condition))
#define POKESIM_ASSERT_NM(condition) POKESIM_ASSERT(condition, 0)
#define POKESIM_ASSERT_FAIL(message) POKESIM_ASSERT(false, message)
#endif
// NOLINTEND cppcoreguidelines-macro-usage
#pragma once

// NOLINTBEGIN cppcoreguidelines-macro-usage
#ifdef NDEBUG
namespace pokesim::debug {
inline char ASSERT_COUNT = 0;  // NOLINT cppcoreguidelines-avoid-non-const-global-variables
}

#define POKESIM_ASSERT(condition, message)
#else
#include <cassert>
#include <cstdint>

namespace pokesim::debug {
inline uint64_t ASSERT_COUNT = 0;  // NOLINT cppcoreguidelines-avoid-non-const-global-variables
}

#define POKESIM_ASSERT(condition, message) \
  pokesim::debug::ASSERT_COUNT++;          \
  assert((void(message), condition))
#endif

// An assert with no message. For use in debug checks only.
#define POKESIM_ASSERT_NM(condition) POKESIM_ASSERT(condition, 0)
#define POKESIM_ASSERT_FAIL(message) POKESIM_ASSERT(false, message)
// NOLINTEND cppcoreguidelines-macro-usage
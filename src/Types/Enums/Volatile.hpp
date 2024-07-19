#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class Volatile : std::uint8_t {
  // clang-format off
  NO_VOLATILE = 0,
  CHOICE_LOCK,
  // clang-format on
};
}  // namespace pokesim::dex

#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class Volatile : std::uint8_t {
  // clang-format off
  NO_VOLATILE = 0,
  CHOICE_LOCK,
  VOLATILE_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_VOLATILE_COUNT = (std::uint8_t)Volatile::VOLATILE_TOTAL - 1U;
}  // namespace pokesim::dex

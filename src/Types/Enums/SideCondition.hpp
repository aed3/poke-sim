#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class SideCondition : std::uint8_t {
  // clang-format off
  NO_SIDE_CONDITION = 0U, SIDE_CONDITION_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_SIDE_CONDITION_COUNT = (std::uint8_t)SideCondition::SIDE_CONDITION_TOTAL - 1U;
}  // namespace pokesim::dex

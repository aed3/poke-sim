#pragma once

#include <cstdint>

namespace pokesim::dex {
// Pokemon nature name
enum class Nature : std::uint8_t {
  // clang-format off
  NO_NATURE = 0U, ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, NATURE_TOTAL
  // clang-format on
};

static constexpr std::uint8_t TOTAL_NATURE_COUNT = (std::uint8_t)Nature::NATURE_TOTAL - 1U;
}  // namespace pokesim::dex

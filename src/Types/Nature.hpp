#pragma once
#include <cstdint>

namespace pokesim::dex {
/** @brief Pokemon nature name */
enum Nature : std::uint8_t {
  // clang-format off
  NO_NATURE = 0, ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, NATURE_TOTAL
  // clang-format on
};
}  // namespace pokesim::dex

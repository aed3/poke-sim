#pragma once
#include <cstdint>

namespace pokesim {
namespace dex {
// clang-format off

/** @brief Pokemon nature name */
enum Nature : std::uint8_t {
  NO_NATURE = 0, ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, NATURE_TOTAL
};

// clang-format on
}  // namespace dex

struct NatureName {
  dex::Nature name = dex::NO_NATURE;
};
}  // namespace pokesim
#pragma once

#include <Types/Move.hpp>

namespace pokesim {
/**
 * @brief Chance of an effect occurring.
 *
 * Examples: Air Slash has a 60% chance to flinch, Harvest has a 50% chance to restore a used berry.
 */
struct Chance {
  types::baseEffectChance chance = 100;
};
}  // namespace pokesim
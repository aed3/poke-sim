#pragma once

#include <Types/Constants.hpp>
#include <Types/Random.hpp>

namespace pokesim {
/**
 * @brief Chance of an effect occurring.
 *
 * Examples: Air Slash has a 60% chance to flinch, Harvest has a 50% chance to restore a used berry.
 */
struct BaseEffectChance {
  types::percentChance val = Constants::MoveBaseEffectChance::DEFAULT;
};
}  // namespace pokesim

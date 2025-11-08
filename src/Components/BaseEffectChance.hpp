#pragma once

#include <Types/MechanicConstants.hpp>
#include <Types/Random.hpp>

namespace pokesim {
/**
 * @brief Chance of an effect occurring.
 *
 * Examples: Air Slash has a 60% chance to flinch, Harvest has a 50% chance to restore a used berry.
 */
struct BaseEffectChance {
  types::percentChance val = MechanicConstants::MoveBaseEffectChance::MAX;
};
}  // namespace pokesim
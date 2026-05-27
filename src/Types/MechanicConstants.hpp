#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Event.hpp>
#include <Types/Random.hpp>
#include <array>

// NOLINTBEGIN(readability-magic-numbers)
namespace pokesim {
struct MechanicConstants {
  static constexpr std::array<types::percentChance, 4U> CRIT_CHANCE_DIVISORS(GameMechanics) {
    return {24U, 8U, 2U, 1U};
  }

  static constexpr types::effectMultiplier CRIT_MULTIPLIER(GameMechanics) { return 1.5F; }
};
}  // namespace pokesim
// NOLINTEND(readability-magic-numbers)

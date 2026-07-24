#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct Competitive {
  static constexpr Ability name(GameMechanics) { return dex::Ability::COMPETITIVE; }

  static constexpr types::boost onAfterEachBoostSpaBoost(GameMechanics) { return 2; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Competitive"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "competitive"; }
  };

  static void onAfterEachBoost(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

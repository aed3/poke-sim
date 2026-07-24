#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct Defiant {
  static constexpr Ability name(GameMechanics) { return dex::Ability::DEFIANT; }

  static constexpr types::boost onAfterEachBoostAtkBoost(GameMechanics) { return 2; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Defiant"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "defiant"; }
  };

  static void onAfterEachBoost(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

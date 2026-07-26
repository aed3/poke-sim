#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct Scrappy {
  static constexpr Ability name(GameMechanics) { return dex::Ability::SCRAPPY; }

  struct Strings {
    static constexpr std::string_view name() { return "Scrappy"; }
    static constexpr std::string_view smogonId() { return "scrappy"; }
  };

  static void onModifyMove(Simulation& simulation);
  static void onTryBoost(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

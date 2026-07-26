#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Analytic {
  static constexpr Ability name(GameMechanics) { return dex::Ability::ANALYTIC; }

  static constexpr types::eventModifier onBasePowerNumerator(GameMechanics) { return 5324U; }
  static constexpr types::eventModifier onBasePowerDenominator(GameMechanics) { return 4096U; }

  struct Strings {
    static constexpr std::string_view name() { return "Analytic"; }
    static constexpr std::string_view smogonId() { return "analytic"; }
  };

  static void onBasePower(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

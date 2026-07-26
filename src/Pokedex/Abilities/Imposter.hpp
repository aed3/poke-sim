#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/AbilityProperty.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Imposter {
  static constexpr Ability name(GameMechanics) { return dex::Ability::IMPOSTER; }

  static constexpr AbilityProperty properties(GameMechanics) {
    return AbilityProperty::CANNOT_BREAK | AbilityProperty::NO_ROLE_PLAY | AbilityProperty::NO_RECEIVER |
           AbilityProperty::NO_ENTRAINMENT | AbilityProperty::NO_TRACE;
  }

  struct Strings {
    static constexpr std::string_view name() { return "Imposter"; }
    static constexpr std::string_view smogonId() { return "imposter"; }
  };

  static void onSwitchIn(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

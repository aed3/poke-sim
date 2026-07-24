#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/AbilityProperty.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct StanceChange {
  static constexpr Ability name(GameMechanics) { return dex::Ability::STANCE_CHANGE; }

  static constexpr AbilityProperty properties(GameMechanics) {
    return AbilityProperty::CANNOT_BREAK | AbilityProperty::CANNOT_SUPPRESS | AbilityProperty::NO_ROLE_PLAY |
           AbilityProperty::NO_RECEIVER | AbilityProperty::NO_ENTRAINMENT | AbilityProperty::NO_SKILL_SWAP |
           AbilityProperty::NO_TRACE;
  }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Stance Change"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "stancechange"; }
  };

  static void onModifyMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

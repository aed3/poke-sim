#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/AbilityProperty.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <array>
#include <string_view>

namespace pokesim {
class Simulation;
}

namespace pokesim::dex {
struct StanceChange {
  static constexpr Ability name(GameMechanics = {}) { return dex::Ability::STANCE_CHANGE; }

  static constexpr AbilityProperty properties(GameMechanics) {
    return AbilityProperty::CANNOT_BREAK | AbilityProperty::CANNOT_SUPPRESS | AbilityProperty::NO_ROLE_PLAY |
           AbilityProperty::NO_RECEIVER | AbilityProperty::NO_ENTRAINMENT | AbilityProperty::NO_SKILL_SWAP |
           AbilityProperty::NO_TRACE;
  }

  struct associatedForms {
    static constexpr Species base(GameMechanics) { return Species::SHIELD_AEGISLASH; }
    static constexpr std::array<Species, 2U> all(GameMechanics) {
      return {Species::SHIELD_AEGISLASH, Species::BLADE_AEGISLASH};
    }
  };

  struct Strings {
    static constexpr std::string_view name() { return "Stance Change"; }
    static constexpr std::string_view smogonId() { return "stancechange"; }
  };

  static void onModifyMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SWORD_SHIELD; }
};
}  // namespace pokesim::dex

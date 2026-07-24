#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Braviary {
  static constexpr Species name(GameMechanics) { return Species::BRAVIARY; }
  static constexpr types::baseStat hp(GameMechanics) { return 100U; }
  static constexpr types::baseStat atk(GameMechanics) { return 123U; }
  static constexpr types::baseStat def(GameMechanics) { return 75U; }
  static constexpr types::baseStat spa(GameMechanics) { return 57U; }
  static constexpr types::baseStat spd(GameMechanics) { return 75U; }
  static constexpr types::baseStat spe(GameMechanics) { return 80U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::NORMAL, Type::FLYING}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::KEEN_EYE; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::SHEER_FORCE; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::DEFIANT; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Braviary"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Braviary"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "braviary"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

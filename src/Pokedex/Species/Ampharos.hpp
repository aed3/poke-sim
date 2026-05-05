#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ampharos {
  static constexpr Species name(GameMechanics) { return Species::AMPHAROS; }
  static constexpr types::baseStat hp(GameMechanics) { return 90U; }
  static constexpr types::baseStat atk(GameMechanics) { return 75U; }
  static constexpr types::baseStat def(GameMechanics) { return 85U; }
  static constexpr types::baseStat spa(GameMechanics) { return 115U; }
  static constexpr types::baseStat spd(GameMechanics) { return 90U; }
  static constexpr types::baseStat spe(GameMechanics) { return 55U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::ELECTRIC}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::STATIC; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::PLUS; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Ampharos"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Ampharos"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "ampharos"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

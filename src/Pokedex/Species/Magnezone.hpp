#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Magnezone {
  static constexpr Species name(GameMechanics) { return Species::MAGNEZONE; }
  static constexpr types::baseStat hp(GameMechanics) { return 70U; }
  static constexpr types::baseStat atk(GameMechanics) { return 70U; }
  static constexpr types::baseStat def(GameMechanics) { return 115U; }
  static constexpr types::baseStat spa(GameMechanics) { return 130U; }
  static constexpr types::baseStat spd(GameMechanics) { return 90U; }
  static constexpr types::baseStat spe(GameMechanics) { return 60U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::ELECTRIC, Type::STEEL}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::MAGNET_PULL; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::STURDY; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::ANALYTIC; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Magnezone"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Magnezone"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "magnezone"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

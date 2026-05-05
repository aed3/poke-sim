#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ribombee {
  static constexpr Species name(GameMechanics) { return Species::RIBOMBEE; }
  static constexpr types::baseStat hp(GameMechanics) { return 60U; }
  static constexpr types::baseStat atk(GameMechanics) { return 55U; }
  static constexpr types::baseStat def(GameMechanics) { return 60U; }
  static constexpr types::baseStat spa(GameMechanics) { return 95U; }
  static constexpr types::baseStat spd(GameMechanics) { return 70U; }
  static constexpr types::baseStat spe(GameMechanics) { return 124U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::BUG, Type::FAIRY}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::HONEY_GATHER; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::SHIELD_DUST; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::SWEET_VEIL; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Ribombee"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Ribombee"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "Ribombee"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Dragapult {
  static constexpr Species name(GameMechanics) { return Species::DRAGAPULT; }
  static constexpr types::baseStat hp(GameMechanics) { return 88U; }
  static constexpr types::baseStat atk(GameMechanics) { return 120U; }
  static constexpr types::baseStat def(GameMechanics) { return 75U; }
  static constexpr types::baseStat spa(GameMechanics) { return 100U; }
  static constexpr types::baseStat spd(GameMechanics) { return 75U; }
  static constexpr types::baseStat spe(GameMechanics) { return 142U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::DRAGON, Type::GHOST}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::CLEAR_BODY; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::INFILTRATOR; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::CURSED_BODY; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Dragapult"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Dragapult"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "dragapult"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

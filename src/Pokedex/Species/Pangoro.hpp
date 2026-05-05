#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Pangoro {
  static constexpr Species name(GameMechanics) { return Species::PANGORO; }
  static constexpr types::baseStat hp(GameMechanics) { return 95U; }
  static constexpr types::baseStat atk(GameMechanics) { return 124U; }
  static constexpr types::baseStat def(GameMechanics) { return 78U; }
  static constexpr types::baseStat spa(GameMechanics) { return 69U; }
  static constexpr types::baseStat spd(GameMechanics) { return 71U; }
  static constexpr types::baseStat spe(GameMechanics) { return 58U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::FIGHTING, Type::DARK}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::IRON_FIST; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::MOLD_BREAKER; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::SCRAPPY; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Pangoro"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Pangoro"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "pangoro"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SWORD_SHIELD; }
};
}  // namespace pokesim::dex

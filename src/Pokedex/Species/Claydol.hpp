#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Claydol {
  static constexpr Species name(GameMechanics) { return Species::CLAYDOL; }
  static constexpr types::baseStat hp(GameMechanics) { return 60U; }
  static constexpr types::baseStat atk(GameMechanics) { return 70U; }
  static constexpr types::baseStat def(GameMechanics) { return 105U; }
  static constexpr types::baseStat spa(GameMechanics) { return 70U; }
  static constexpr types::baseStat spd(GameMechanics) { return 120U; }
  static constexpr types::baseStat spe(GameMechanics) { return 75U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::GROUND, Type::PSYCHIC}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::LEVITATE; }

  struct Strings {
    static constexpr std::string_view name() { return "Claydol"; }
    static constexpr std::string_view smogonName() { return "Claydol"; }
    static constexpr std::string_view smogonId() { return "claydol"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL; }
};
}  // namespace pokesim::dex

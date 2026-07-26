#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Ditto {
  static constexpr Species name(GameMechanics) { return Species::DITTO; }
  static constexpr types::baseStat hp(GameMechanics) { return 48U; }
  static constexpr types::baseStat atk(GameMechanics) { return 48U; }
  static constexpr types::baseStat def(GameMechanics) { return 48U; }
  static constexpr types::baseStat spa(GameMechanics) { return 48U; }
  static constexpr types::baseStat spd(GameMechanics) { return 48U; }
  static constexpr types::baseStat spe(GameMechanics) { return 48U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::NORMAL}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::LIMBER; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::IMPOSTER; }

  struct Strings {
    static constexpr std::string_view name() { return "Ditto"; }
    static constexpr std::string_view smogonName() { return "Ditto"; }
    static constexpr std::string_view smogonId() { return "ditto"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

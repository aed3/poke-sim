#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Gardevoir {
  static constexpr Species name(GameMechanics) { return Species::GARDEVOIR; }
  static constexpr types::baseStat hp(GameMechanics) { return 68U; }
  static constexpr types::baseStat atk(GameMechanics) { return 65U; }
  static constexpr types::baseStat def(GameMechanics) { return 65U; }
  static constexpr types::baseStat spa(GameMechanics) { return 125U; }
  static constexpr types::baseStat spd(GameMechanics) { return 115U; }
  static constexpr types::baseStat spe(GameMechanics) { return 80U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::PSYCHIC, Type::FAIRY}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::SYNCHRONIZE; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::TRACE; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::TELEPATHY; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Gardevoir"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Gardevoir"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "gardevoir"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

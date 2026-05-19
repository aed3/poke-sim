#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Empoleon {
  static constexpr Species name(GameMechanics) { return dex::Species::EMPOLEON; }
  static constexpr types::baseStat hp(GameMechanics) { return 84U; }
  static constexpr types::baseStat atk(GameMechanics) { return 86U; }
  static constexpr types::baseStat def(GameMechanics) { return 88U; }
  static constexpr types::baseStat spa(GameMechanics) { return 111U; }
  static constexpr types::baseStat spd(GameMechanics) { return 101U; }
  static constexpr types::baseStat spe(GameMechanics) { return 60U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::WATER, Type::STEEL}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::TORRENT; }
  static constexpr Ability hiddenAbility(GameMechanics gameMechanic) {
    switch (gameMechanic) {
      case GameMechanics::SWORD_SHIELD:
      case GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL: return Ability::DEFIANT;
      case GameMechanics::SCARLET_VIOLET:
      default:                                             return Ability::COMPETITIVE;
    }
  }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Empoleon"; }
    static constexpr std::string_view smogonName(GameMechanics) { return "Empoleon"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "empoleon"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

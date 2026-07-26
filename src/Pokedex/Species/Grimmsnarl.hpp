#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
struct Grimmsnarl {
  static constexpr Species name(GameMechanics) { return Species::GRIMMSNARL; }
  static constexpr types::baseStat hp(GameMechanics) { return 95U; }
  static constexpr types::baseStat atk(GameMechanics) { return 120U; }
  static constexpr types::baseStat def(GameMechanics) { return 65U; }
  static constexpr types::baseStat spa(GameMechanics) { return 95U; }
  static constexpr types::baseStat spd(GameMechanics) { return 75U; }
  static constexpr types::baseStat spe(GameMechanics) { return 60U; }

  static constexpr SpeciesTypes type(GameMechanics) { return {Type::DARK, Type::FAIRY}; }

  static constexpr Ability primaryAbility(GameMechanics) { return Ability::PRANKSTER; }
  static constexpr Ability secondaryAbility(GameMechanics) { return Ability::FRISK; }
  static constexpr Ability hiddenAbility(GameMechanics) { return Ability::PICKPOCKET; }

  struct Strings {
    static constexpr std::string_view name() { return "Grimmsnarl"; }
    static constexpr std::string_view smogonName() { return "Grimmsnarl"; }
    static constexpr std::string_view smogonId() { return "grimmsnarl"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

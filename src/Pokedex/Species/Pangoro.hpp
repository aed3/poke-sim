#pragma once

#include <Components/Pokedex/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Pangoro {
  static constexpr Species name = Species::PANGORO;
  static constexpr types::baseStat hp = 95, atk = 124, def = 78, spa = 69, spd = 71, spe = 58;

  static constexpr SpeciesTypes type = {Type::FIGHTING_TYPE, Type::DARK_TYPE};

  static constexpr Ability primaryAbility = Ability::IRON_FIST;
  static constexpr Ability secondaryAbility = Ability::MOLD_BREAKER;
  static constexpr Ability hiddenAbility = Ability::SCRAPPY;

  struct Strings {
    static constexpr std::string_view name = "Pangoro";
    static constexpr std::string_view smogonName = "Pangoro";
    static constexpr std::string_view smogonId = "pangoro";
  };
};

namespace latest {
using Pangoro = dex::Pangoro<GameMechanics::SWORD_SHIELD>;
}
}  // namespace pokesim::dex
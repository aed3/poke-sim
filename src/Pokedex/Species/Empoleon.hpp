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
struct Empoleon {
  static constexpr Species name = Species::EMPOLEON;
  static constexpr types::baseStat hp = 84, atk = 86, def = 88, spa = 111, spd = 101, spe = 60;

  static constexpr SpeciesTypes type = {Type::WATER_TYPE, Type::STEEL_TYPE};

  static constexpr Ability primaryAbility = Ability::TORRENT;
  static constexpr Ability hiddenAbility = Ability::DEFIANT;

  struct Strings {
    static constexpr std::string_view name = "Empoleon";
    static constexpr std::string_view smogonName = "Empoleon";
    static constexpr std::string_view smogonId = "empoleon";
  };
};

template <>
struct Empoleon<GameMechanics::SCARLET_VIOLET> : Empoleon<GameMechanics::NONE> {
  static constexpr Ability hiddenAbility = Ability::COMPETITIVE;
};

namespace latest {
using Empoleon = dex::Empoleon<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
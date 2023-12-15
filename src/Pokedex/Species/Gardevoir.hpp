#pragma once

#include <Components/DexData/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Gardevoir {
  static constexpr Species name = Species::GARDEVOIR;
  static constexpr types::BaseStat hp = 68, atk = 65, def = 65, spa = 125, spd = 115, spe = 80;

  static constexpr SpeciesTypes type = {Type::PSYCHIC_TYPE, Type::FAIRY_TYPE};

  static constexpr Ability primaryAbility = Ability::SYNCHRONIZE;
  static constexpr Ability secondaryAbility = Ability::TRACE;
  static constexpr Ability hiddenAbility = Ability::TELEPATHY;

  struct Strings {
    static constexpr std::string_view name = "Gardevoir";
    static constexpr std::string_view smogonName = "Gardevoir";
    static constexpr std::string_view smogonId = "gardevoir";
  };
};

namespace latest {
using Gardevoir = dex::Gardevoir<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
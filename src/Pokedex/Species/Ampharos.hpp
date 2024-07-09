#pragma once

#include <Components/SpeciesTypes.hpp>
#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Species.hpp>
#include <Types/Enums/Type.hpp>
#include <Types/Stats.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct Ampharos {
  static constexpr Species name = Species::AMPHAROS;
  static constexpr types::baseStat hp = 90, atk = 75, def = 85, spa = 115, spd = 90, spe = 55;

  static constexpr SpeciesTypes type = {Type::ELECTRIC_TYPE};

  static constexpr Ability primaryAbility = Ability::STATIC;
  static constexpr Ability hiddenAbility = Ability::PLUS;

  struct Strings {
    static constexpr std::string_view name = "Ampharos";
    static constexpr std::string_view smogonName = "Ampharos";
    static constexpr std::string_view smogonId = "ampharos";
  };
};

namespace latest {
using Ampharos = dex::Ampharos<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
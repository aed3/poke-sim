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
  static constexpr types::baseStat hp = 90U, atk = 75U, def = 85U, spa = 115U, spd = 90U, spe = 55U;

  static constexpr SpeciesTypes type = {Type::ELECTRIC};

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
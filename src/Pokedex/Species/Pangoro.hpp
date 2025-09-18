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
struct Pangoro {
  static constexpr Species name = Species::PANGORO;
  static constexpr types::baseStat hp = 95U, atk = 124U, def = 78U, spa = 69U, spd = 71U, spe = 58U;

  static constexpr SpeciesTypes type = {Type::FIGHTING, Type::DARK};

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
using Pangoro = dex::Pangoro<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
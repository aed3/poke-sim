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
struct Ribombee {
  static constexpr Species name = Species::RIBOMBEE;
  static constexpr types::baseStat hp = 60U, atk = 55U, def = 60U, spa = 95U, spd = 70U, spe = 124U;

  static constexpr SpeciesTypes type = {Type::BUG, Type::FAIRY};

  static constexpr Ability primaryAbility = Ability::HONEY_GATHER;
  static constexpr Ability secondaryAbility = Ability::SHIELD_DUST;
  static constexpr Ability hiddenAbility = Ability::SWEET_VEIL;

  struct Strings {
    static constexpr std::string_view name = "Ribombee";
    static constexpr std::string_view smogonName = "Ribombee";
    static constexpr std::string_view smogonId = "Ribombee";
  };
};

namespace latest {
using Ribombee = dex::Ribombee<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
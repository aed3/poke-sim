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
struct Dragapult {
  static constexpr Species name = Species::DRAGAPULT;
  static constexpr types::baseStat hp = 88U, atk = 120U, def = 75U, spa = 100U, spd = 75U, spe = 142U;

  static constexpr SpeciesTypes type = {Type::DRAGON, Type::GHOST};

  static constexpr Ability primaryAbility = Ability::CLEAR_BODY;
  static constexpr Ability secondaryAbility = Ability::INFILTRATOR;
  static constexpr Ability hiddenAbility = Ability::CURSED_BODY;

  struct Strings {
    static constexpr std::string_view name = "Dragapult";
    static constexpr std::string_view smogonName = "Dragapult";
    static constexpr std::string_view smogonId = "dragapult";
  };
};

namespace latest {
using Dragapult = dex::Dragapult<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex
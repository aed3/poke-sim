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
struct Empoleon {
  static constexpr Species name = Species::EMPOLEON;
  static constexpr types::baseStat hp = 84U, atk = 86U, def = 88U, spa = 111U, spd = 101U, spe = 60U;

  static constexpr SpeciesTypes type = {Type::WATER, Type::STEEL};

  static constexpr Ability primaryAbility = Ability::TORRENT;
  static constexpr Ability hiddenAbility = Ability::COMPETITIVE;

  struct Strings {
    static constexpr std::string_view name = "Empoleon";
    static constexpr std::string_view smogonName = "Empoleon";
    static constexpr std::string_view smogonId = "empoleon";
  };
};

template <>
struct Empoleon<GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL> : Empoleon<GameMechanics::NONE> {
  static constexpr Ability hiddenAbility = Ability::DEFIANT;
};

template <>
struct Empoleon<GameMechanics::SWORD_SHIELD> : Empoleon<GameMechanics::BRILLIANT_DIAMOND_SHINING_PEARL> {};

namespace latest {
using Empoleon = dex::Empoleon<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex
#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Static {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::STATIC; }

  static constexpr types::percentChance onDamagingHitChance(GameMechanics) { return 30U; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Static"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "static"; }
  };

  static void onDamagingHit(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

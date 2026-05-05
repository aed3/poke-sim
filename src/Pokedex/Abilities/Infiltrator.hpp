#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct Infiltrator {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::INFILTRATOR; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Infiltrator"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "infiltrator"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

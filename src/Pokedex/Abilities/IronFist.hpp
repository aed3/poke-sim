#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct IronFist {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::IRON_FIST; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Iron Fist"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "ironfist"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

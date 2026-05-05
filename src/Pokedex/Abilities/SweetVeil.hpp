#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct SweetVeil {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::SWEET_VEIL; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Sweet Veil"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "sweetveil"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

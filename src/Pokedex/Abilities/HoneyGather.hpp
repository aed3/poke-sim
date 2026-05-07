#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct HoneyGather {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::HONEY_GATHER; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Honey Gather"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "honeygather"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

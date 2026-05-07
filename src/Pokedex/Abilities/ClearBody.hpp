#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct ClearBody {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::CLEAR_BODY; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Clear Body"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "clearbody"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Defiant {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::DEFIANT; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Defiant"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "defiant"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

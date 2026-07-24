#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Synchronize {
  static constexpr dex::Ability name(GameMechanics) { return dex::Ability::SYNCHRONIZE; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Synchronize"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "synchronize"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

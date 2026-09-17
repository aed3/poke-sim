#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Overgrow {
  static constexpr Ability name(GameMechanics = {}) { return dex::Ability::OVERGROW; }

  struct Strings {
    static constexpr std::string_view name() { return "Overgrow"; }
    static constexpr std::string_view smogonId() { return "overgrow"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

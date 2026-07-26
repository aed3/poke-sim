#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Trace {
  static constexpr Ability name(GameMechanics) { return dex::Ability::TRACE; }

  struct Strings {
    static constexpr std::string_view name() { return "Trace"; }
    static constexpr std::string_view smogonId() { return "trace"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

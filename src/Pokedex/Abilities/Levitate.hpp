#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Levitate {
  static constexpr Ability name(GameMechanics) { return dex::Ability::LEVITATE; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Levitate"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "levitate"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

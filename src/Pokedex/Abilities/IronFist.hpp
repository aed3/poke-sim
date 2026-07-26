#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct IronFist {
  static constexpr Ability name(GameMechanics) { return dex::Ability::IRON_FIST; }

  struct Strings {
    static constexpr std::string_view name() { return "Iron Fist"; }
    static constexpr std::string_view smogonId() { return "ironfist"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

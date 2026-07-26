#pragma once

#include <Types/Enums/Ability.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <string_view>

namespace pokesim::dex {
struct Torrent {
  static constexpr Ability name(GameMechanics) { return dex::Ability::TORRENT; }

  struct Strings {
    static constexpr std::string_view name() { return "Torrent"; }
    static constexpr std::string_view smogonId() { return "torrent"; }
  };

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

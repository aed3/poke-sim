#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct KingsRock {
  static constexpr Item name(GameMechanics) { return dex::Item::KINGS_ROCK; }

  struct Strings {
    static constexpr std::string_view name() { return "King's Rock"; }
    static constexpr std::string_view smogonId() { return "kingsrock"; }
  };

  static void onModifyMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

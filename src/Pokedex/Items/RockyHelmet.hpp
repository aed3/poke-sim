#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct RockyHelmet {
  static constexpr Item name(GameMechanics) { return dex::Item::ROCKY_HELMET; }

  struct Strings {
    static constexpr std::string_view name() { return "Rocky Helmet"; }
    static constexpr std::string_view smogonId() { return "rockyhelmet"; }
  };

  static void onDamagingHit(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

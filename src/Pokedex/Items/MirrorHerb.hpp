#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct MirrorHerb {
  static constexpr Item name(GameMechanics) { return dex::Item::MIRROR_HERB; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Mirror Herb"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "mirrorherb"; }
  };

  static void onFoeAfterBoost(Simulation& simulation);
  static void onAnySwitchIn(Simulation& simulation);
  static void onAnyAfterMega(Simulation& simulation);
  static void onAnyAfterTerastallize(Simulation& simulation);
  static void onAnyAfterMove(Simulation& simulation);
  static void onResidual(Simulation& simulation);
  static void onUse(Simulation& simulation);
  static void onEnd(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

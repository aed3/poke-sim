#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Random.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct QuickClaw {
  static constexpr Item name(GameMechanics) { return dex::Item::QUICK_CLAW; }

  static constexpr types::percentChance onFractionalPriorityChance(GameMechanics) { return 20U; }
  struct Strings {
    static constexpr std::string_view name() { return "Quick Claw"; }
    static constexpr std::string_view smogonId() { return "quickclaw"; }
  };

  static void onFractionalPriority(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

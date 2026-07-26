#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct BrightPowder {
  static constexpr Item name(GameMechanics) { return dex::Item::BRIGHT_POWDER; }

  static constexpr types::eventModifier onModifyAccuracyNumerator(GameMechanics) { return 3686U; }
  static constexpr types::eventModifier onModifyAccuracyDenominator(GameMechanics) { return 4096U; }

  struct Strings {
    static constexpr std::string_view name() { return "Bright Powder"; }
    static constexpr std::string_view smogonId() { return "brightpowder"; }
  };

  static void onModifyAccuracy(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct QuickPowder {
  static constexpr Item name(GameMechanics = {}) { return dex::Item::QUICK_POWDER; }

  static constexpr types::effectMultiplier onModifySpeModifier(GameMechanics) { return 2.0F; }

  struct Strings {
    static constexpr std::string_view name() { return "Quick Powder"; }
    static constexpr std::string_view smogonId() { return "quickpowder"; }
  };

  static void onModifySpe(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

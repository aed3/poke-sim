#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Volatile.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct ChoiceLock {
  static constexpr dex::Volatile name(GameMechanics) { return dex::Volatile::CHOICE_LOCK; }

  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Choice Lock"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "choicelock"; }
  };

  static void onBeforeMove(Simulation& simulation);
  static void onDisableMove(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

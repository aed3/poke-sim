#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct BrightPowder {
  static void onModifyAccuracy(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct BrightPowder {
  static constexpr dex::Item name = dex::Item::BRIGHT_POWDER;

  static constexpr types::eventModifier onModifyAccuracyNumerator = 3686U;
  static constexpr types::eventModifier onModifyAccuracyDenominator = 4096U;
  struct Strings {
    static constexpr std::string_view name = "Bright Powder";
    static constexpr std::string_view smogonId = "brightpowder";
  };
};

namespace latest {
using BrightPowder = dex::BrightPowder<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex
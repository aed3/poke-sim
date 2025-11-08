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
struct ChoiceSpecs {
  static void onModifySpa(Simulation& simulation);
  static void onSourceModifyMove(Simulation& simulation);
  static void onEnd(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct ChoiceSpecs : events::ChoiceSpecs {
  static constexpr dex::Item name = dex::Item::CHOICE_SPECS;

  static constexpr types::effectMultiplier onModifySpaModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Choice Specs";
    static constexpr std::string_view smogonId = "choicespecs";
  };
};

namespace latest {
using ChoiceSpecs = dex::ChoiceSpecs<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex
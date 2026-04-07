#pragma once

#include <Components/Tags/ItemPropertyTags.hpp>
#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <Utilities/Tags.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
namespace events {
struct ChoiceScarf {
  static void onModifySpe(Simulation& simulation);
  static void onSourceModifyMove(Simulation& simulation);
  static void onEnd(Simulation& simulation);
};
}  // namespace events

template <GameMechanics>
struct ChoiceScarf : events::ChoiceScarf {
  static constexpr dex::Item name = dex::Item::CHOICE_SCARF;

  static constexpr types::effectMultiplier onModifySpeModifier = 1.5F;

  static constexpr Tags<item::tags::Choice> itemTags{};
  struct Strings {
    static constexpr std::string_view name = "Choice Scarf";
    static constexpr std::string_view smogonId = "choicescarf";
  };
};

namespace latest {
using ChoiceScarf = dex::ChoiceScarf<GameMechanics::LATEST>;
}
}  // namespace pokesim::dex

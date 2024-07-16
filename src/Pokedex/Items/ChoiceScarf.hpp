#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Registry.hpp>
#include <string_view>

namespace pokesim {
struct EventModifier;
struct Battle;
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct ChoiceScarfEvents {
  static void onModifySpe(EventModifier& eventModifier);
  static void onSourceModifyMove(types::handle pokemonHandle, const Battle& battle);
};
}  // namespace internal

template <GameMechanics>
struct ChoiceScarf : internal::ChoiceScarfEvents {
  static constexpr dex::Item name = dex::Item::CHOICE_SCARF;

  static constexpr float onModifySpeModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Choice Scarf";
    static constexpr std::string_view smogonId = "choicescarf";
  };
};

namespace latest {
using ChoiceScarf = dex::ChoiceScarf<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Event.hpp>
#include <Types/Registry.hpp>
#include <string_view>

namespace pokesim {
struct EventModifier;
struct Battle;
}  // namespace pokesim

namespace pokesim::dex {
namespace internal {
struct ChoiceSpecsEvents {
  static void onModifySpa(EventModifier& eventModifier);
  static void onSourceModifyMove(types::handle pokemonHandle, const Battle& battle);
};
}  // namespace internal

template <GameMechanics>
struct ChoiceSpecs : internal::ChoiceSpecsEvents {
  static constexpr dex::Item name = dex::Item::CHOICE_SPECS;

  static constexpr types::effectMultiplier onModifySpaModifier = 1.5F;
  struct Strings {
    static constexpr std::string_view name = "Choice Specs";
    static constexpr std::string_view smogonId = "choicespecs";
  };
};

namespace latest {
using ChoiceSpecs = dex::ChoiceSpecs<GameMechanics::SCARLET_VIOLET>;
}
}  // namespace pokesim::dex
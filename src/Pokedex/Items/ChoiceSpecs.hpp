#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <Types/Enums/ItemProperty.hpp>
#include <Types/Event.hpp>
#include <string_view>

namespace pokesim {
class Simulation;
}  // namespace pokesim

namespace pokesim::dex {
struct ChoiceSpecs {
  static constexpr dex::Item name(GameMechanics) { return dex::Item::CHOICE_SPECS; }

  static constexpr types::effectMultiplier onModifySpaModifier(GameMechanics) { return 1.5F; }

  static constexpr ItemProperty itemProperties(GameMechanics) { return ItemProperty::CHOICE; }
  struct Strings {
    static constexpr std::string_view name(GameMechanics) { return "Choice Specs"; }
    static constexpr std::string_view smogonId(GameMechanics) { return "choicespecs"; }
  };

  static void onModifySpa(Simulation& simulation);
  static void onSourceModifyMove(Simulation& simulation);
  static void onEnd(Simulation& simulation);

  static constexpr GameMechanics latest() { return GameMechanics::SCARLET_VIOLET; }
};
}  // namespace pokesim::dex

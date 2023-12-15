#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct ChoiceSpecs {
  static constexpr dex::Item name = dex::Item::CHOICE_SPECS;

  struct Strings {
    static constexpr std::string_view name = "Choice Specs";
    static constexpr std::string_view smogonId = "choicespecs";
  };
};
}  // namespace pokesim::dex
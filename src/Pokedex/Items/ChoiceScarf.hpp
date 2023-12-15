#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct ChoiceScarf {
  static constexpr dex::Item name = dex::Item::CHOICE_SCARF;

  struct Strings {
    static constexpr std::string_view name = "Choice Scarf";
    static constexpr std::string_view smogonId = "choicescarf";
  };
};
}  // namespace pokesim::dex
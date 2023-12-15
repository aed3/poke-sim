#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct BrightPowder {
  static constexpr dex::Item name = dex::Item::BRIGHT_POWDER;

  struct Strings {
    static constexpr std::string_view name = "Bright Powder";
    static constexpr std::string_view smogonId = "brightpowder";
  };
};
}  // namespace pokesim::dex
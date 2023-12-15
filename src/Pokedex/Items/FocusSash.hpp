#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct FocusSash {
  static constexpr dex::Item name = dex::Item::FOCUS_SASH;

  struct Strings {
    static constexpr std::string_view name = "Focus Sash";
    static constexpr std::string_view smogonId = "focussash";
  };
};
}  // namespace pokesim::dex
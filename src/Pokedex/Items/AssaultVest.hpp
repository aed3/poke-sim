#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct AssaultVest {
  static constexpr dex::Item name = dex::Item::ASSAULT_VEST;

  struct Strings {
    static constexpr std::string_view name = "Assault Vest";
    static constexpr std::string_view smogonId = "assaultvest";
  };
};
}  // namespace pokesim::dex
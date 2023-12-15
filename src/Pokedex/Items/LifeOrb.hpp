#pragma once

#include <Types/Enums/GameMechanics.hpp>
#include <Types/Enums/Item.hpp>
#include <string_view>

namespace pokesim::dex {
template <GameMechanics>
struct LifeOrb {
  static constexpr dex::Item name = dex::Item::LIFE_ORB;

  struct Strings {
    static constexpr std::string_view name = "Life Orb";
    static constexpr std::string_view smogonId = "lifeorb";
  };
};
}  // namespace pokesim::dex
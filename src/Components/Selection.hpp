#pragma once

#include <cstdint>

namespace pokesim {
struct SelectedForViewBattle {
  std::uint8_t count = 0;
};
struct SelectedForViewSide {
  std::uint8_t count = 0;
};
struct SelectedForViewPokemon {
  std::uint8_t count = 0;
};
struct SelectedForViewMove {
  std::uint8_t count = 0;
};
}  // namespace pokesim

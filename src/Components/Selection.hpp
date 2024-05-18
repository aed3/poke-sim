#pragma once

#include <cstdint>

// TODO (aed3): THIS WILL NOT BE OK WHEN RUNNING MULTIPLE SIMULATION OBJECTS ON DIFFERENT THREADS!
namespace pokesim {
struct SelectedForViewBattle {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewSide {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewPokemon {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
struct SelectedForViewMove {
  static inline std::uint8_t depth = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
};
}  // namespace pokesim

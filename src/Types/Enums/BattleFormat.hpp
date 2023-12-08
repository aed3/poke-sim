#pragma once

#include <cstdint>

namespace pokesim {
// Denotes if the simulated battle is a single or double battle
enum class BattleFormat : std::uint8_t {
  SINGLES_BATTLE_FORMAT = 1,
  DOUBLES_BATTLE_FORMAT = 2,
};
}  // namespace pokesim
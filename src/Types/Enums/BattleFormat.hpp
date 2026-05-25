#pragma once

#include <array>
#include <cstdint>

namespace pokesim {
// Denotes if the simulated battle is a single or double battle
enum class BattleFormat : std::uint8_t {
  SINGLES = 1U,
  DOUBLES = 2U,
};

static constexpr inline std::array<BattleFormat, 2U> VALID_BATTLE_FORMATS = {
  BattleFormat::SINGLES,
  BattleFormat::DOUBLES,
};
}  // namespace pokesim

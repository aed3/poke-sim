#pragma once
#include <cstdint>

namespace pokesim {
namespace dex {
/** @brief Pokemon stat abbreviated name */
enum Stat : std::uint8_t {
  HP = 0b000001,
  ATK = 0b000010,
  DEF = 0b000100,
  SPA = 0b001000,
  SPD = 0b010000,
  SPE = 0b100000,
  STAT_TOTAL = 6
  // SPC = SPA | SPD,
};
}  // namespace dex

struct StatName {
  dex::Stat name;
};
}  // namespace pokesim
#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class MoveProperty : std::uint64_t {
  ACCURACY_DEPENDENT_HIT_COUNT = 1UL << 0U,
  BITE = 1UL << 1U,
  BULLET = 1UL << 2U,
  BYPASS_PROTECT = 1UL << 3U,
  BYPASS_SUBSTITUTE = 1UL << 4U,
  CAN_REFLECT = 1UL << 5U,
  CAN_SNATCH = 1UL << 6U,
  CANNOT_USE_TWICE = 1UL << 7U,
  CHARGE = 1UL << 8U,
  CONTACT = 1UL << 9U,
  DANCE = 1UL << 10U,
  DEFROST = 1UL << 11U,
  FAIL_WITH_GRAVITY = 1UL << 12U,
  FUTURE_MOVE = 1UL << 13U,
  HEAL = 1UL << 14U,
  MUST_PRESSURE = 1UL << 15U,
  NO_ASSIST = 1UL << 16U,
  NO_COPYCAT = 1UL << 17U,
  NO_ENCORE = 1UL << 18U,
  NO_INSTRUCT = 1UL << 19U,
  NO_ME_FIRST = 1UL << 20U,
  NO_METRONOME = 1UL << 21U,
  NO_MIMIC = 1UL << 22U,
  NO_MIRROR_MOVE = 1UL << 23U,
  NO_PARENTAL_BOND = 1UL << 24U,
  NO_SKETCH = 1UL << 25U,
  NO_SLEEP_TALK = 1UL << 26U,
  PLEDGE = 1UL << 27U,
  POWDER = 1UL << 28U,
  PULSE = 1UL << 29U,
  PUNCH = 1UL << 30U,
  RECHARGE = 1UL << 31U,
  SLICING = 1UL << 32U,
  SOUND = 1UL << 33U,
  STRENGTHEN_ON_MINIMIZED = 1UL << 34U,
  VARIABLE_HIT_COUNT = 1UL << 35U,
  WIND = 1UL << 36U,
};

constexpr MoveProperty operator|(MoveProperty kindA, MoveProperty kindB) {
  return static_cast<MoveProperty>(static_cast<std::uint64_t>(kindA) | static_cast<std::uint64_t>(kindB));
}

constexpr bool operator&(MoveProperty kindA, MoveProperty kindB) {
  return (static_cast<std::uint64_t>(kindA) & static_cast<std::uint64_t>(kindB)) != 0U;
}
}  // namespace pokesim::dex

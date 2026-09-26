#pragma once

#include <cstdint>

namespace pokesim::dex {
enum class MoveProperty : std::uint64_t {
  ACCURACY_DEPENDENT_HIT_COUNT = 1ULL << 0U,
  BITE = 1ULL << 1U,
  BULLET = 1ULL << 2U,
  BYPASS_PROTECT = 1ULL << 3U,
  BYPASS_SUBSTITUTE = 1ULL << 4U,
  CAN_REFLECT = 1ULL << 5U,
  CAN_SNATCH = 1ULL << 6U,
  CANNOT_USE_TWICE = 1ULL << 7U,
  CHARGE = 1ULL << 8U,
  CONTACT = 1ULL << 9U,
  DANCE = 1ULL << 10U,
  DEFROST = 1ULL << 11U,
  FAIL_WITH_GRAVITY = 1ULL << 12U,
  FUTURE_MOVE = 1ULL << 13U,
  HEAL = 1ULL << 14U,
  IGNORE_IMMUNITIES = 1ULL << 15U,
  MUST_PRESSURE = 1ULL << 16U,
  NO_ASSIST = 1ULL << 17U,
  NO_COPYCAT = 1ULL << 18U,
  NO_ENCORE = 1ULL << 19U,
  NO_INSTRUCT = 1ULL << 20U,
  NO_ME_FIRST = 1ULL << 21U,
  NO_METRONOME = 1ULL << 22U,
  NO_MIMIC = 1ULL << 23U,
  NO_MIRROR_MOVE = 1ULL << 24U,
  NO_PARENTAL_BOND = 1ULL << 25U,
  NO_SKETCH = 1ULL << 26U,
  NO_SLEEP_TALK = 1ULL << 27U,
  PLEDGE = 1ULL << 28U,
  POWDER = 1ULL << 29U,
  PULSE = 1ULL << 30U,
  PUNCH = 1ULL << 31U,
  RECHARGE = 1ULL << 32U,
  SELF_SWITCH = 1ULL << 33U,
  SLICING = 1ULL << 34U,
  SOUND = 1ULL << 35U,
  STATUS_USES_IMMUNITIES = 1ULL << 36U,
  STRENGTHEN_ON_MINIMIZED = 1ULL << 37U,
  VARIABLE_HIT_COUNT = 1ULL << 38U,
  WIND = 1ULL << 39U,
};

constexpr MoveProperty operator|(MoveProperty propertyA, MoveProperty propertyB) {
  return static_cast<MoveProperty>(static_cast<std::uint64_t>(propertyA) | static_cast<std::uint64_t>(propertyB));
}

constexpr bool operator&(MoveProperty propertyA, MoveProperty propertyB) {
  return (static_cast<std::uint64_t>(propertyA) & static_cast<std::uint64_t>(propertyB)) != 0U;
}
}  // namespace pokesim::dex

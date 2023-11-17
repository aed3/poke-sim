#pragma once

#include <cstdint>

namespace pokesim {
/**
 * @brief Represents the target slot for a move in a Pokemon battle.
 *
 * In a single battle, only P1A and P1B are valid targets. In a double battle, all four are valid.
 */
struct TargetSlot {
  enum : std::uint8_t {
    P1A,
    P1B,
    P2A,
    P2B,
  } targetSlot = P1A;
};
}  // namespace pokesim
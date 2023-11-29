#pragma once

#include <Types/Enums/TargetSlot.hpp>

namespace pokesim {
/**
 * @brief Represents the target slot for a move in a Pokemon battle.
 *
 * In a single battle, only P1A and P1B are valid targets. In a double battle, all four are valid.
 */
struct TargetSlotName {
  TargetSlot targetSlot = TargetSlot::P1A;
};
}  // namespace pokesim
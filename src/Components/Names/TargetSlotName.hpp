#pragma once

#include <Types/Enums/Slot.hpp>

namespace pokesim {
/**
 * @brief Represents the target slot for a move in a Pokemon battle.
 *
 * In a single battle, only P1A and P2A are valid targets. In a double battle, P1B and P2B are also valid.
 */
struct TargetSlotName {
  Slot name = Slot::NONE;
};
}  // namespace pokesim
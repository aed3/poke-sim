#pragma once

#include <Types/Enums/Slot.hpp>

namespace pokesim {
/**
 * @brief Represents the source slot for a move in a Pokemon battle.
 */
struct SourceSlotName {
  Slot name = Slot::NONE;
};
}  // namespace pokesim
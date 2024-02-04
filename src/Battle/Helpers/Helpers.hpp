#pragma once

#include <Types/Entity.hpp>
#include <Types/Enums/Slot.hpp>

namespace pokesim {
struct Sides;

types::entity slotToEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot);
types::entity slotToEntity(const types::registry& registry, const Sides& sides, Slot targetSlot);
}  // namespace pokesim
#pragma once
#include <Types/Entity.hpp>
#include <Types/Enums/TargetSlot.hpp>

namespace pokesim {
struct Sides;

/*_inline_*/ types::entity targetSlotEntity(const types::registry& registry, const Sides& sides, TargetSlot targetSlot);
}  // namespace pokesim
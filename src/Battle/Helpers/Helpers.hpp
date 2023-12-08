#pragma once
#include <Types/Enums/TargetSlot.hpp>
#include <entt/entity/fwd.hpp>

namespace pokesim {
struct Sides;

/*_inline_*/ entt::entity targetSlotEntity(const entt::registry& registry, const Sides& sides, TargetSlot targetSlot);
}  // namespace pokesim
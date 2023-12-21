#include "Helpers.hpp"

#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/TargetSlot.hpp>
#include <Types/State.hpp>
#include <cstdint>
#include <entt/entity/registry.hpp>

namespace pokesim {
types::entity targetSlotEntity(const types::registry& registry, const Sides& sides, TargetSlot targetSlot) {
  ENTT_ASSERT(targetSlot != TargetSlot::NONE, "Can only get entity from valid target slot");
  types::entity sideEntity = (std::uint8_t)targetSlot % 2 ? sides.p1 : sides.p2;
  types::TeamPositionIndex index = ((std::uint8_t)targetSlot - 1) / 2;

  const Team& team = registry.get<Team>(sideEntity);
  ENTT_ASSERT(team.team.size() >= index, "Choosing a target slot for team member that does not exist");
  return team.team[index];
}
}  // namespace pokesim

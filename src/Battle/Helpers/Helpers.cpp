#include "Helpers.hpp"

#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <cstdint>
#include <entt/entity/registry.hpp>

namespace pokesim {

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  types::entity sideEntity = (std::uint8_t)targetSlot % 2 ? sides.p1 : sides.p2;
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  types::teamPositionIndex index = ((std::uint8_t)targetSlot - 1) / 2;

  const Team& team = registry.get<Team>(sideEntity);
  ENTT_ASSERT(team.team.size() > index, "Choosing a target slot for team member that does not exist");
  return team.team[index];
}

types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  return slotToPokemonEntity(registry, slotToSideEntity(sides, targetSlot), targetSlot);
}

types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  Slot allySlot = Slot::NONE;
  types::teamPositionIndex index = 0;

  switch (targetSlot) {
    case Slot::P1A: {
      allySlot = Slot::P1B;
      index = 1;
      break;
    }
    case Slot::P1B:
    case Slot::P1C:
    case Slot::P1D:
    case Slot::P1E:
    case Slot::P1F: {
      allySlot = Slot::P1A;
      break;
    }
    case Slot::P2A: {
      allySlot = Slot::P2B;
      index = 1;
      break;
    }
    case Slot::P2B:
    case Slot::P2C:
    case Slot::P2D:
    case Slot::P2E:
    case Slot::P2F: {
      allySlot = Slot::P2A;
      break;
    }
    default: break;
  }

  types::entity sideEntity = slotToSideEntity(sides, allySlot);
  const Team& team = registry.get<Team>(sideEntity);
  if (team.team.size() <= index) {
    return entt::null;
  }

  types::entity allyEntity = team.team[index];
  if (registry.any_of<tags::Fainted>(allyEntity)) {
    return entt::null;
  }

  return allyEntity;
}

types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move) {
  for (types::entity moveSlot : moveSlots.moveSlots) {
    if (registry.get<MoveName>(moveSlot).name == move) {
      return moveSlot;
    }
  }

  ENTT_FAIL("No move of entity found");
  return entt::null;
}
}  // namespace pokesim

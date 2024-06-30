#include "Helpers.hpp"

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/State.hpp>
#include <cstdint>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  ENTT_ASSERT(targetSlot != Slot::NONE, "Can only get entity from valid target slot");
  types::entity sideEntity = sides.val[((std::uint8_t)targetSlot - 1) % 2];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  types::teamPositionIndex index = ((std::uint8_t)targetSlot - 1) / 2;

  const Team& team = registry.get<Team>(sideEntity);
  ENTT_ASSERT(team.val.size() > index, "Choosing a target slot for team member that does not exist");
  return team.val[index];
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
  if (team.val.size() <= index) {
    return entt::null;
  }

  types::entity allyEntity = team.val[index];
  if (registry.any_of<tags::Fainted>(allyEntity)) {
    return entt::null;
  }

  return allyEntity;
}

types::entity moveToEntity(const types::registry& registry, const MoveSlots& moveSlots, dex::Move move) {
  for (types::entity moveSlot : moveSlots.val) {
    if (registry.get<MoveName>(moveSlot).name == move) {
      return moveSlot;
    }
  }

  ENTT_FAIL("No move of entity found");
  return entt::null;
}

types::entity createActionMoveForTarget(
  types::handle targetHandle, types::entity battleEntity, types::entity sourceEntity, dex::Move move,
  const Pokedex& pokedex) {
  types::registry& registry = *targetHandle.registry();
  types::entity moveEntity = pokedex.buildActionMove(move, registry);

  registry.emplace<tags::CurrentActionMove>(moveEntity);
  registry.emplace<Battle>(moveEntity, battleEntity);
  registry.emplace<CurrentActionSource>(moveEntity, sourceEntity);
  registry.emplace<CurrentActionTargets>(moveEntity).val.push_back(targetHandle.entity());

  targetHandle.emplace<CurrentActionMove>(moveEntity);

  return moveEntity;
}
}  // namespace pokesim

#include "Helpers.hpp"

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <cstdint>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {

types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  types::entity sideEntity = sides.val[((types::teamPositionIndex)targetSlot - 1U) % 2U];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  types::teamPositionIndex index = ((types::teamPositionIndex)targetSlot - 1U) / 2U;

  const Team& team = registry.get<Team>(sideEntity);
  POKESIM_REQUIRE(team.val.size() > index, "Choosing a target slot for team member that does not exist.");
  return team.val[index];
}

types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  return slotToPokemonEntity(registry, slotToSideEntity(sides, targetSlot), targetSlot);
}

types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  Slot allySlot = Slot::NONE;
  types::teamPositionIndex index = 0U;

  switch (targetSlot) {
    case Slot::P1A: {
      allySlot = Slot::P1B;
      index = 1U;
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
      index = 1U;
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
  if (index >= team.val.size()) {
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

  POKESIM_REQUIRE_FAIL("No move of entity found.");
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

  targetHandle.get_or_emplace<CurrentActionMoves>().val.push_back(moveEntity);
  registry.get_or_emplace<CurrentActionMoves>(sourceEntity).val.push_back(moveEntity);

  return moveEntity;
}
}  // namespace pokesim

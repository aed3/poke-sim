#include "Helpers.hpp"

#include <Battle/Setup/EmplaceTagFromEnum.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/MoveSlots.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/Selection.hpp>
#include <Components/Tags/SimulationTags.hpp>
#include <Config/Require.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
types::entity slotToSideEntity(const Sides& sides, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
  types::entity sideEntity = sides.val[((types::teamPositionIndex)targetSlot - 1U) % 2U];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot targetSlot) {
  POKESIM_REQUIRE(targetSlot != Slot::NONE, "Can only get entity from valid target slot.");
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

types::moveSlotIndex moveToMoveSlot(const MoveSlots& moveSlots, dex::Move move) {
  for (types::moveSlotIndex i = 0; i < moveSlots.val.size(); i++) {
    if (moveSlots.val[i].move == move) {
      return i;
    }
  }

  POKESIM_REQUIRE_FAIL("No move found.");
  return 0U;
}

void internal::setupActionMoveBuild(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  types::entity actionMoveEntity, pokesim::dex::Move move) {
  types::handle actionMoveHandle{registry, actionMoveEntity};

  move::tags::emplaceTagFromEnum(move, actionMoveHandle);
  actionMoveHandle.emplace<Battle>(battleEntity);
  actionMoveHandle.emplace<CurrentActionSource>(sourceEntity);
  actionMoveHandle.emplace<CurrentActionTarget>(targetEntity);
  actionMoveHandle.emplace<pokesim::internal::tags::BuildActionMove>();
  actionMoveHandle.emplace<pokesim::tags::CurrentActionMove>();

  registry.get_or_emplace<CurrentActionMovesAsTarget>(targetEntity).val.push_back(actionMoveEntity);
  registry.get_or_emplace<CurrentActionMovesAsSource>(sourceEntity).val.push_back(actionMoveEntity);
}
}  // namespace pokesim

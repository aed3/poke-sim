#include "Helpers.hpp"

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
#include <Pokedex/EnumToTag/MoveEnumToTag.hpp>
#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/Move.hpp>
#include <Types/Enums/Slot.hpp>
#include <Types/Indexes.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

#include "InternalHelpers.hpp"

namespace pokesim {
namespace {
void checkSlot(Slot slot) {
  POKESIM_REQUIRE(
    std::find(internal::VALID_SLOTS.begin(), internal::VALID_SLOTS.end(), slot) != internal::VALID_SLOTS.end(),
    "Invalid slot found.");
}

types::teamPositionIndex slotToIndex(Slot slot) {
  checkSlot(slot);
  return (types::teamPositionIndex)slot & internal::SLOT_LETTER_MASK;
}
}  // namespace

Slot sideIdAndPositionToSlot(PlayerSideId sideId, types::teamPositionIndex position) {
  Slot slot = (Slot)(((types::teamPositionIndex)sideId << 4U) + position);
  checkSlot(slot);
  return slot;
}

PlayerSideId slotToSideId(Slot slot) {
  checkSlot(slot);
  return (types::teamPositionIndex)slot >= (types::teamPositionIndex)Slot::P2A ? PlayerSideId::P2 : PlayerSideId::P1;
}

types::entity slotToSideEntity(const Sides& sides, Slot slot) {
  types::entity sideEntity = sides.val[slotToSideId(slot) == PlayerSideId::P1 ? 0U : 1U];
  return sideEntity;
}

types::entity slotToPokemonEntity(const types::registry& registry, types::entity sideEntity, Slot slot) {
  types::teamPositionIndex index = slotToIndex(slot);

  const Team& team = registry.get<Team>(sideEntity);
  POKESIM_REQUIRE(team.val.size() > index, "Choosing a slot for team member that does not exist.");
  return team.val[index];
}

types::entity slotToPokemonEntity(const types::registry& registry, const Sides& sides, Slot slot) {
  return slotToPokemonEntity(registry, slotToSideEntity(sides, slot), slot);
}

void swapEntitySlots(types::registry& registry, types::entity sideEntity, Slot slot1, Slot slot2) {
  POKESIM_REQUIRE(slotToSideId(slot1) == slotToSideId(slot2), "Swapped slots must be from the same side");
  types::teamPositionIndex index1 = slotToIndex(slot1);
  types::teamPositionIndex index2 = slotToIndex(slot2);

  Team& team = registry.get<Team>(sideEntity);
  POKESIM_REQUIRE(team.val.size() > index1, "Choosing a slot for team member that does not exist.");
  POKESIM_REQUIRE(team.val.size() > index2, "Choosing a slot for team member that does not exist.");

  std::swap(team.val[index1], team.val[index2]);
}

void swapEntitySlots(types::registry& registry, const Sides& sides, Slot slot1, Slot slot2) {
  swapEntitySlots(registry, slotToSideEntity(sides, slot1), slot1, slot2);
}

types::entity slotToAllyPokemonEntity(const types::registry& registry, const Sides& sides, Slot slot) {
  checkSlot(slot);
  Slot allySlot = Slot::NONE;
  types::teamPositionIndex index = 0U;

  switch (slot) {
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

namespace internal {
void setupActionMoveBuild(
  types::registry& registry, types::entity battleEntity, types::entity sourceEntity, types::entity targetEntity,
  types::entity actionMoveEntity, pokesim::dex::Move move, bool useExtended) {
  types::handle actionMoveHandle{registry, actionMoveEntity};

  dex::emplaceTagFromEnum(move, actionMoveHandle);
  actionMoveHandle.emplace<Battle>(battleEntity);
  actionMoveHandle.emplace<CurrentActionSource>(sourceEntity);
  actionMoveHandle.emplace<CurrentActionTarget>(targetEntity);
  actionMoveHandle.emplace<pokesim::internal::tags::BuildActionMove>();
  actionMoveHandle.emplace<pokesim::tags::CurrentActionMove>();

  bool addBaseTargetMoves = !useExtended;
  bool addBaseSourceMoves = !useExtended;
  if (useExtended) {
    if (registry.all_of<CurrentActionMovesAsTarget>(targetEntity)) {
      registry.get_or_emplace<CurrentActionMovesAsTargetExtended>(targetEntity).val.push_back(actionMoveEntity);
    }
    else {
      addBaseTargetMoves = true;
    }

    auto& currentMoves = registry.get_or_emplace<CurrentActionMovesAsSource>(sourceEntity);
    if (currentMoves.val.size() == currentMoves.val.max_size()) {
      registry.get_or_emplace<CurrentActionMovesAsSourceExtended>(sourceEntity).val.push_back(actionMoveEntity);
    }
    else {
      currentMoves.val.push_back(actionMoveEntity);
    }
  }

  if (addBaseTargetMoves) {
    registry.emplace<CurrentActionMovesAsTarget>(targetEntity, actionMoveEntity);
  }
  if (addBaseSourceMoves) {
    registry.get_or_emplace<CurrentActionMovesAsSource>(sourceEntity).val.push_back(actionMoveEntity);
  }
}
}  // namespace internal
}  // namespace pokesim

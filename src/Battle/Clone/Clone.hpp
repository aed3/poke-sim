#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>

namespace pokesim {
/*_inline_*/ void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);

/*_inline_*/ void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);
/*_inline_*/ void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);
/*_inline_*/ void cloneActionQueue(
  types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);
/*_inline_*/ void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);
/*_inline_*/ void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount);

/*_inline_*/ void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap);
template <typename Component, typename GetEntity>
/*_inline_*/ void remapEntityMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntity getEntity);
template <typename Component, typename GetEntityList>
/*_inline_*/ void remapEntityListMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntityList getEntityList);

/*_inline_*/ void remapActionQueueEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapFoeSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapMoveEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapMoveEffectEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapMoveSlotsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapPokemonEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapSidesEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
/*_inline_*/ void remapTeamEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
}  // namespace pokesim
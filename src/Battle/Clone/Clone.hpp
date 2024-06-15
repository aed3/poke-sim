#pragma once

#include <Types/Entity.hpp>
#include <Types/State.hpp>
#include <optional>

namespace pokesim {
struct CloneTo;

types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount);

namespace internal {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void cloneActionQueue(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap);
template <typename Component, typename GetEntity>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntity getEntity);
template <typename Component, typename GetEntityList>
void remapEntityListMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntityList getEntityList);

void remapActionQueueEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapFoeSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapMoveEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapMoveEffectEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapMoveSlotsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapPokemonEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapSidesEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapTeamEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
void remapActionTargetsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
}  // namespace internal
}  // namespace pokesim
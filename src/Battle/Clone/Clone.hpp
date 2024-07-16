#pragma once

#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
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
void cloneAction(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void cloneCurrentActionMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);
void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount);

template <typename Component>
void remapComponentEntities(types::registry& registry, const types::ClonedEntityMap& entityMap);

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap);
template <typename Component>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
template <typename Component>
void remapEntityListMembers(types::registry& registry, const types::ClonedEntityMap& entityMap);
}  // namespace internal
}  // namespace pokesim
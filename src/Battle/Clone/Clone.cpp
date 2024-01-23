#include "Clone.hpp"

#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount) {
  auto& list = entityMap[src] = std::vector<types::entity>{duplicateCount};
  for (types::cloneIndex i = 0; i < duplicateCount; i++) {
    types::entity dest = list[i] = registry.create();
    registry.emplace<CloneTo>(dest, i);
    for (auto [id, storage] : registry.storage()) {
      if (storage.contains(src)) {
        storage.push(dest, storage.value(src));
      }
    }
  }
}

void cloneBattle(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex duplicateCount) {
  for (const auto [entity, sides, actionQueue] : registry.view<tags::CloneFrom, Sides, ActionQueue>().each()) {
    registry.emplace<tags::CloneFrom>(sides.p1);
    registry.emplace<tags::CloneFrom>(sides.p2);
    for (auto queueItem : actionQueue.actionQueue) {
      registry.emplace<tags::CloneFrom>(queueItem);
    }

    cloneEntity(entity, registry, entityMap, duplicateCount);
  }
}

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap) {
  ENTT_ASSERT(entityMap.contains(entity), "Source node was not loaded into the map");
  ENTT_ASSERT(entityMap.at(entity).size() > cloneTo.index, "More entities are trying to be copied to than were copied");
  entity = entityMap.at(entity)[cloneTo.index];
}

template <typename Component, typename GetEntity>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntity getEntity) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    types::entity& entity = getEntity(component);
    remapEntity(entity, cloneTo, entityMap);
  }
}

template <typename Component, typename GetEntityList>
void remapEntityListMembers(
  types::registry& registry, const types::ClonedEntityMap& entityMap, GetEntityList getEntityList) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    auto& entities = getEntityList(component);
    for (types::entity& entity : entities) {
      remapEntity(entity, cloneTo, entityMap);
    }
  }
}

void remapActionQueueEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  remapEntityListMembers<ActionQueue>(
    registry,
    entityMap,
    [](ActionQueue & queue) -> auto& { return queue.actionQueue; });
}

void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  remapEntityMembers<Battle>(
    registry,
    entityMap,
    [](Battle & battle) -> auto& { return battle.battle; });
}
}  // namespace pokesim
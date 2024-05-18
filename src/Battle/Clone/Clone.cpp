#include "Clone.hpp"

#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/SpeedSort.hpp>
#include <Types/Entity.hpp>
#include <Types/State.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/core/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount) {
  types::cloneIndex count = cloneCount.value_or(1);
  types::ClonedEntityMap entityMap;
  entt::dense_map<entt::id_type, std::vector<types::entity>> srcEntityStorages;

  internal::cloneBattle(registry, entityMap, srcEntityStorages, count);
  internal::cloneSide(registry, entityMap, srcEntityStorages, count);
  internal::cloneActionQueue(registry, entityMap, srcEntityStorages, count);
  internal::clonePokemon(registry, entityMap, srcEntityStorages, count);
  internal::cloneMove(registry, entityMap, srcEntityStorages, count);

  for (auto [id, storage] : registry.storage()) {
    if (srcEntityStorages.contains(id)) {
      const auto& sources = srcEntityStorages.at(id);
      storage.reserve(storage.size() + sources.size() * count);
      for (types::entity src : sources) {
        auto* value = storage.value(src);
        for (types::cloneIndex i = 0; i < count; i++) {
          storage.push(entityMap[src][i], value);
        }
      }
    }
  }

  auto& cloneToStorage = registry.storage<CloneTo>();
  cloneToStorage.reserve(entityMap.size() * count);

  for (const auto& [src, destinations] : entityMap) {
    cloneToStorage.insert(destinations.begin(), destinations.end());
  }

  for (const auto& [src, destinations] : entityMap) {
    for (types::cloneIndex i = 0; i < count; i++) {
      cloneToStorage.get(destinations[i]).index = i;
    }
  }

  internal::remapActionQueueEntityMembers(registry, entityMap);
  internal::remapBattleEntityMembers(registry, entityMap);
  internal::remapFoeSideEntityMembers(registry, entityMap);
  internal::remapMoveEntityMembers(registry, entityMap);
  internal::remapMoveEffectEntityMembers(registry, entityMap);
  internal::remapMoveSlotsEntityMembers(registry, entityMap);
  internal::remapPokemonEntityMembers(registry, entityMap);
  internal::remapSideEntityMembers(registry, entityMap);
  internal::remapSidesEntityMembers(registry, entityMap);
  internal::remapTeamEntityMembers(registry, entityMap);

  registry.clear<CloneTo, tags::CloneFrom>();

  return entityMap;
}

namespace internal {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = std::vector<types::entity>{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, sides, actionQueue] : registry.view<tags::CloneFrom, Sides, ActionQueue>().each()) {
    registry.emplace<tags::CloneFrom>(sides.p1);
    registry.emplace<tags::CloneFrom>(sides.p2);
    for (auto queueItem : actionQueue.actionQueue) {
      registry.emplace<tags::CloneFrom>(queueItem);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, team] : registry.view<tags::CloneFrom, Team>().each()) {
    for (auto pokemon : team.team) {
      registry.emplace<tags::CloneFrom>(pokemon);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneActionQueue(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, SpeedSort>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, species, moveSlots] : registry.view<tags::CloneFrom, SpeciesName, MoveSlots>().each()) {
    for (auto move : moveSlots.moveSlots) {
      registry.emplace<tags::CloneFrom>(move);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, MoveName>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
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
  auto getEntityList = [](ActionQueue & queue) -> auto& {
    return queue.actionQueue;
  };

  remapEntityListMembers<ActionQueue>(registry, entityMap, getEntityList);
}

void remapBattleEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Battle & battle) -> auto& {
    return battle.battle;
  };

  remapEntityMembers<Battle>(registry, entityMap, getEntity);
}

void remapFoeSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](FoeSide & foeSide) -> auto& {
    return foeSide.foeSide;
  };

  remapEntityMembers<FoeSide>(registry, entityMap, getEntity);
}

void remapMoveEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](LastUsedMove & lastUsedMove) -> auto& {
    return lastUsedMove.lastUsedMove;
  };

  remapEntityMembers<LastUsedMove>(registry, entityMap, getEntity);
}

void remapMoveEffectEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](MoveEffect & moveEffect) -> auto& {
    return moveEffect.moveEffect;
  };

  remapEntityMembers<MoveEffect>(registry, entityMap, getEntity);
}

void remapMoveSlotsEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](MoveSlots & moveSlots) -> auto& {
    return moveSlots.moveSlots;
  };

  remapEntityListMembers<MoveSlots>(registry, entityMap, getEntityList);
}

void remapPokemonEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Pokemon & pokemon) -> auto& {
    return pokemon.pokemon;
  };

  remapEntityMembers<Pokemon>(registry, entityMap, getEntity);
}

void remapSideEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntity = [](Side & side) -> auto& {
    return side.side;
  };

  remapEntityMembers<Side>(registry, entityMap, getEntity);
}

void remapSidesEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, sides] : registry.view<CloneTo, Sides>().each()) {
    remapEntity(sides.p1, cloneTo, entityMap);
    remapEntity(sides.p2, cloneTo, entityMap);
  }
}

void remapTeamEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  auto getEntityList = [](Team & team) -> auto& {
    return team.team;
  };

  remapEntityListMembers<Team>(registry, entityMap, getEntityList);
}
}  // namespace internal
}  // namespace pokesim
#include "Clone.hpp"

#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/SpeedSort.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount) {
  types::cloneIndex count = cloneCount.value_or(1);
  types::ClonedEntityMap entityMap;

  internal::cloneBattle(registry, entityMap, count);
  internal::cloneSide(registry, entityMap, count);
  internal::cloneActionQueue(registry, entityMap, count);
  internal::clonePokemon(registry, entityMap, count);
  internal::cloneMove(registry, entityMap, count);

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
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  auto& list = entityMap[src] = std::vector<types::entity>{cloneCount};

  for (types::cloneIndex i = 0; i < cloneCount; i++) {
    types::entity dest = list[i] = registry.create();
    registry.emplace<CloneTo>(dest, i);
    for (auto [id, storage] : registry.storage()) {
      if (storage.contains(src)) {
        storage.push(dest, storage.value(src));
      }
    }
  }
}

void cloneBattle(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  for (const auto [entity, sides, actionQueue] : registry.view<tags::CloneFrom, Sides, ActionQueue>().each()) {
    registry.emplace<tags::CloneFrom>(sides.p1);
    registry.emplace<tags::CloneFrom>(sides.p2);
    for (auto queueItem : actionQueue.actionQueue) {
      registry.emplace<tags::CloneFrom>(queueItem);
    }

    cloneEntity(entity, registry, entityMap, cloneCount);
  }
}

void cloneSide(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  for (const auto [entity, team] : registry.view<tags::CloneFrom, Team>().each()) {
    for (auto pokemon : team.team) {
      registry.emplace<tags::CloneFrom>(pokemon);
    }

    cloneEntity(entity, registry, entityMap, cloneCount);
  }
}

void cloneActionQueue(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, SpeedSort>()) {
    cloneEntity(entity, registry, entityMap, cloneCount);
  }
}

void clonePokemon(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  for (const auto [entity, species, moveSlots] : registry.view<tags::CloneFrom, SpeciesName, MoveSlots>().each()) {
    for (auto move : moveSlots.moveSlots) {
      registry.emplace<tags::CloneFrom>(move);
    }

    cloneEntity(entity, registry, entityMap, cloneCount);
  }
}

void cloneMove(types::registry& registry, types::ClonedEntityMap& entityMap, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, MoveName>()) {
    cloneEntity(entity, registry, entityMap, cloneCount);
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
  auto getEntity = [](Move & move) -> auto& {
    return move.move;
  };

  remapEntityMembers<Move>(registry, entityMap, getEntity);
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
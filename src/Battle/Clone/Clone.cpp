#include "Clone.hpp"

#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/PP.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Config/Config.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/core/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
namespace {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      POKESIM_REQUIRE(
        std::find(srcEntityStorages[id].begin(), srcEntityStorages[id].end(), src) == std::end(srcEntityStorages[id]),
        "Adding an entity twice here means an entity will be duplicated more than it should.");
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = std::vector<types::entity>{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

template <typename VisitEntity = void*>
void traverseBattle(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, sides, actionQueue] : registry.view<Tag, tags::Battle, Sides, ActionQueue>().each()) {
    for (auto side : sides.val) {
      registry.emplace<Tag>(side);
    }
    for (auto queueItem : actionQueue.val) {
      registry.emplace<Tag>(queueItem);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }
  for (const auto [entity, currentAction] : registry.view<Tag, tags::Battle, CurrentAction>().each()) {
    registry.emplace<Tag>(currentAction.val);
  }
}

template <typename VisitEntity = void*>
void traverseSide(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, team] : registry.view<Tag, tags::Side, Team>().each()) {
    for (auto pokemon : team.val) {
      registry.emplace<Tag>(pokemon);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traverseAction(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, SpeedSort>()) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traverseCurrentActionMove(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, tags::CurrentActionMove>()) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traversePokemon(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, moveSlots] : registry.view<Tag, tags::Pokemon, MoveSlots>().each()) {
    for (auto move : moveSlots.val) {
      registry.emplace<Tag>(move);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }

  for (const auto [entity, moves] : registry.view<Tag, CurrentActionMoves>().each()) {
    for (types::entity move : moves.val) {
      registry.emplace_or_replace<Tag>(move);
    }
  }
}

template <typename VisitEntity = void*>
void traverseMove(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, MoveName, Pp>()) {
      visitEntity(entity);
    }
  }
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traverseBattle(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traverseSide(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneAction(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traverseAction(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneCurrentActionMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traverseCurrentActionMove(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traversePokemon(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  traverseMove(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void deleteBattle(types::registry& registry) {
  traverseBattle(registry);
}

void deleteSide(types::registry& registry) {
  traverseSide(registry);
}

void deleteAction(types::registry& registry) {
  traverseAction(registry);
}

void deleteCurrentActionMove(types::registry& registry) {
  traverseCurrentActionMove(registry);
}

void deletePokemon(types::registry& registry) {
  traversePokemon(registry);
}

void deleteMove(types::registry& registry) {
  traverseMove(registry);
}

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap) {
  POKESIM_REQUIRE(entityMap.contains(entity), "Source node was not loaded into the map.");
  POKESIM_REQUIRE(
    entityMap.at(entity).size() > cloneTo.val,
    "More entities are trying to be copied to than were copied.");
  entity = entityMap.at(entity)[cloneTo.val];
}

template <typename Component>
void remapEntityMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    remapEntity(component.val, cloneTo, entityMap);
  }
}

template <typename Component>
void remapEntityListMembers(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, component] : registry.view<CloneTo, Component>().each()) {
    for (types::entity& entity : component.val) {
      remapEntity(entity, cloneTo, entityMap);
    }
  }
}

template <typename Component>
void remapComponentEntities(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  if constexpr (std::is_same_v<decltype(Component::val), types::entity>) {
    remapEntityMembers<Component>(registry, entityMap);
  }
  else {
    remapEntityListMembers<Component>(registry, entityMap);
  }
}
}  // namespace

types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount) {
  types::cloneIndex count = cloneCount.value_or(1);
  types::ClonedEntityMap entityMap, battleMap;
  if (count == 0) {
    return entityMap;
  }

  entt::dense_map<entt::id_type, std::vector<types::entity>> srcEntityStorages;

  cloneBattle(registry, entityMap, srcEntityStorages, count);
  battleMap = entityMap;
  cloneSide(registry, entityMap, srcEntityStorages, count);
  cloneAction(registry, entityMap, srcEntityStorages, count);
  clonePokemon(registry, entityMap, srcEntityStorages, count);
  cloneCurrentActionMove(registry, entityMap, srcEntityStorages, count);
  cloneMove(registry, entityMap, srcEntityStorages, count);

  for (auto [id, storage] : registry.storage()) {
    if (srcEntityStorages.contains(id)) {
      const auto& sources = srcEntityStorages.at(id);
      storage.reserve(storage.size() + (sources.size() * count));
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
      cloneToStorage.get(destinations[i]).val = i;
    }
  }

  // Not simplified further to a, for example, packed template type list, to make debugging what type went wrong easier
  remapComponentEntities<ActionQueue>(registry, entityMap);
  remapComponentEntities<Battle>(registry, entityMap);
  remapComponentEntities<ChoiceLock>(registry, entityMap);
  remapComponentEntities<CurrentAction>(registry, entityMap);
  remapComponentEntities<CurrentActionMoves>(registry, entityMap);
  remapComponentEntities<CurrentActionMoveSlot>(registry, entityMap);
  remapComponentEntities<CurrentActionSource>(registry, entityMap);
  remapComponentEntities<CurrentActionTargets>(registry, entityMap);
  remapComponentEntities<FoeSide>(registry, entityMap);
  remapComponentEntities<LastUsedMove>(registry, entityMap);
  remapComponentEntities<MoveEffect>(registry, entityMap);
  remapComponentEntities<MoveSlots>(registry, entityMap);
  remapComponentEntities<NextAction>(registry, entityMap);
  remapComponentEntities<Pokemon>(registry, entityMap);
  remapComponentEntities<Side>(registry, entityMap);
  remapComponentEntities<Sides>(registry, entityMap);
  remapComponentEntities<Team>(registry, entityMap);

  registry.clear<CloneTo, tags::CloneFrom>();

  for (const auto& [originalBattle, clonedBattles] : battleMap) {
    registry.remove<ParentBattle>(clonedBattles.begin(), clonedBattles.end());
    registry.insert<ParentBattle>(clonedBattles.begin(), clonedBattles.end(), {originalBattle});
  }

#ifdef POKESIM_DEBUG_CHECK_UTILITIES
  for (const auto& [src, destinations] : entityMap) {
    registry.remove<ParentEntity>(destinations.begin(), destinations.end());
    registry.insert<ParentEntity>(destinations.begin(), destinations.end(), {src});
  }
#endif

  return entityMap;
}

void deleteClones(types::registry& registry) {
  deleteBattle(registry);
  deleteSide(registry);
  deleteAction(registry);
  deletePokemon(registry);
  deleteCurrentActionMove(registry);
  deleteMove(registry);
  auto remove = registry.view<tags::CloneToRemove>();
  registry.destroy(remove.begin(), remove.end());
}
}  // namespace pokesim
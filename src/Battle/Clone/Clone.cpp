#include "Clone.hpp"

#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Pokedex/PP.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Components/Tags/RecycledEntities.hpp>
#include <Config/Config.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/core/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
namespace {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      POKESIM_REQUIRE(
        std::find(srcEntityStorages[id].begin(), srcEntityStorages[id].end(), src) == std::end(srcEntityStorages[id]),
        "Adding an entity twice here means an entity will be duplicated more than it should.");
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = types::entityVector{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

template <typename VisitEntity = void*>
void traverseBattle(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  for (const auto [entity, sides] : registry.view<Tag, tags::Battle, Sides>().each()) {
    for (auto side : sides.val) {
      registry.emplace<Tag>(side);
    }

    if constexpr (ForCloning) {
      visitEntity(entity);
    }
  }

  for (const auto [entity, recycledAction, recycledActionMove] :
       registry.view<Tag, tags::Battle, RecycledAction, RecycledActionMove>().each()) {
    registry.emplace<Tag>(recycledAction.val);
    registry.emplace<Tag>(recycledActionMove.val);
  }

  for (const auto [entity, addedRecycledActionMove1, addedRecycledActionMove2] :
       registry.view<Tag, tags::Battle, AddedRecycledActionMove1, AddedRecycledActionMove2>().each()) {
    registry.emplace<Tag>(addedRecycledActionMove1.val);
    registry.emplace<Tag>(addedRecycledActionMove2.val);
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
    for (types::entity entity : registry.view<Tag, tags::RecycledAction>()) {
      visitEntity(entity);
    }

    for (types::entity entity : registry.view<Tag, tags::RecycledActionMove>()) {
      visitEntity(entity);
    }

    for (types::entity entity : registry.view<Tag, tags::AddedRecycledActionMove1>()) {
      visitEntity(entity);
    }

    for (types::entity entity : registry.view<Tag, tags::AddedRecycledActionMove2>()) {
      visitEntity(entity);
    }
  }
}

template <typename VisitEntity = void*>
void traversePokemon(types::registry& registry, VisitEntity visitEntity = nullptr) {
  const static bool ForCloning = !std::is_same_v<void*, VisitEntity>;
  using Tag = std::conditional_t<ForCloning, tags::CloneFrom, tags::CloneToRemove>;

  if constexpr (ForCloning) {
    for (types::entity entity : registry.view<Tag, tags::Pokemon>()) {
      visitEntity(entity);
    }
  }

  for (const auto [entity, moves] : registry.view<Tag, CurrentActionMovesAsSource>().each()) {
    for (types::entity move : moves.val) {
      registry.emplace_or_replace<Tag>(move);
    }
  }

  for (const auto [entity, moves] : registry.view<Tag, CurrentActionMovesAsTarget>().each()) {
    for (types::entity move : moves.val) {
      registry.emplace_or_replace<Tag>(move);
    }
  }
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseBattle(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseSide(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void cloneAction(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traverseAction(registry, [&](types::entity entity) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  });
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, types::entityVector>& srcEntityStorages, types::entityIndex cloneCount) {
  traversePokemon(registry, [&](types::entity entity) {
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

void deletePokemon(types::registry& registry) {
  traversePokemon(registry);
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

void remapCurrentAction(types::registry& registry, const types::ClonedEntityMap& entityMap) {
  for (auto [clonedEntity, cloneTo, currentAction] : registry.view<CloneTo, CurrentAction>().each()) {
    remapEntity(currentAction.action, cloneTo, entityMap);
    remapEntity(currentAction.source, cloneTo, entityMap);
    for (types::entity& target : currentAction.targets) {
      remapEntity(target, cloneTo, entityMap);
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

types::ClonedEntityMap clone(types::registry& registry, std::optional<types::entityIndex> cloneCount) {
  types::entityIndex count = cloneCount.value_or(1U);
  types::ClonedEntityMap entityMap, battleMap;
  if (count == 0U) {
    return entityMap;
  }

  entt::dense_map<entt::id_type, types::entityVector> srcEntityStorages;

  cloneBattle(registry, entityMap, srcEntityStorages, count);
  battleMap = entityMap;
  cloneSide(registry, entityMap, srcEntityStorages, count);
  cloneAction(registry, entityMap, srcEntityStorages, count);
  clonePokemon(registry, entityMap, srcEntityStorages, count);

  for (auto [id, storage] : registry.storage()) {
    if (srcEntityStorages.contains(id)) {
      const auto& sources = srcEntityStorages.at(id);
      storage.reserve(storage.size() + (sources.size() * count));
      for (types::entity src : sources) {
        auto* value = storage.value(src);
        for (types::entityIndex i = 0U; i < count; i++) {
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
    for (types::entityIndex i = 0U; i < count; i++) {
      cloneToStorage.get(destinations[i]).val = i;
    }
  }

  // Not simplified further to a, for example, packed template type list, to make debugging what type went wrong easier
  remapComponentEntities<AddedRecycledActionMove1>(registry, entityMap);
  remapComponentEntities<AddedRecycledActionMove2>(registry, entityMap);
  remapComponentEntities<Battle>(registry, entityMap);
  remapComponentEntities<CurrentActionMovesAsSource>(registry, entityMap);
  remapComponentEntities<CurrentActionMovesAsTarget>(registry, entityMap);
  remapComponentEntities<CurrentActionSource>(registry, entityMap);
  remapComponentEntities<CurrentActionTarget>(registry, entityMap);
  remapComponentEntities<CurrentEffectSource>(registry, entityMap);
  remapComponentEntities<CurrentEffectTarget>(registry, entityMap);
  remapComponentEntities<CurrentEffectsAsSource>(registry, entityMap);
  remapComponentEntities<CurrentEffectsAsTarget>(registry, entityMap);
  remapComponentEntities<FailedCurrentActionSource>(registry, entityMap);
  remapComponentEntities<FailedCurrentActionTarget>(registry, entityMap);
  remapComponentEntities<FaintQueue>(registry, entityMap);
  remapComponentEntities<FoeSide>(registry, entityMap);
  remapComponentEntities<RecycledAction>(registry, entityMap);
  remapComponentEntities<RecycledActionMove>(registry, entityMap);
  remapComponentEntities<Side>(registry, entityMap);
  remapComponentEntities<Sides>(registry, entityMap);
  remapComponentEntities<Team>(registry, entityMap);
  remapCurrentAction(registry, entityMap);

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
  auto remove = registry.view<tags::CloneToRemove>();
  registry.destroy(remove.begin(), remove.end());
}
}  // namespace pokesim

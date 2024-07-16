#include "Clone.hpp"

#include <Components/CalcDamage/Aliases.hpp>
#include <Components/CloneFromCloneTo.hpp>
#include <Components/EntityHolders/headers.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/PP.hpp>
#include <Components/SimulateTurn/ActionTags.hpp>
#include <Components/SpeedSort.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/Current.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/core/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <vector>

namespace pokesim {
types::ClonedEntityMap clone(types::registry& registry, std::optional<types::cloneIndex> cloneCount) {
  types::cloneIndex count = cloneCount.value_or(1);
  types::ClonedEntityMap entityMap, battleMap;
  entt::dense_map<entt::id_type, std::vector<types::entity>> srcEntityStorages;

  internal::cloneBattle(registry, entityMap, srcEntityStorages, count);
  battleMap = entityMap;
  internal::cloneSide(registry, entityMap, srcEntityStorages, count);
  internal::cloneAction(registry, entityMap, srcEntityStorages, count);
  internal::clonePokemon(registry, entityMap, srcEntityStorages, count);
  internal::cloneCurrentActionMove(registry, entityMap, srcEntityStorages, count);
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
      cloneToStorage.get(destinations[i]).val = i;
    }
  }

  // Not simplified further to a, for example, packed template type list, to make debugging what type went wrong easier
  internal::remapComponentEntities<ActionQueue>(registry, entityMap);
  internal::remapComponentEntities<Battle>(registry, entityMap);
  internal::remapComponentEntities<ChoiceLocked>(registry, entityMap);
  internal::remapComponentEntities<CurrentAction>(registry, entityMap);
  internal::remapComponentEntities<CurrentActionMove>(registry, entityMap);
  internal::remapComponentEntities<CurrentActionMoveSlot>(registry, entityMap);
  internal::remapComponentEntities<CurrentActionSource>(registry, entityMap);
  internal::remapComponentEntities<CurrentActionTargets>(registry, entityMap);
  internal::remapComponentEntities<FoeSide>(registry, entityMap);
  internal::remapComponentEntities<LastUsedMove>(registry, entityMap);
  internal::remapComponentEntities<MoveEffect>(registry, entityMap);
  internal::remapComponentEntities<MoveSlots>(registry, entityMap);
  internal::remapComponentEntities<NextAction>(registry, entityMap);
  internal::remapComponentEntities<Pokemon>(registry, entityMap);
  internal::remapComponentEntities<Side>(registry, entityMap);
  internal::remapComponentEntities<Sides>(registry, entityMap);
  internal::remapComponentEntities<Team>(registry, entityMap);

  registry.clear<CloneTo, tags::CloneFrom>();

  for (const auto& [originalBattle, clonedBattles] : battleMap) {
    registry.remove<ParentBattle>(clonedBattles.begin(), clonedBattles.end());
    registry.insert<ParentBattle>(clonedBattles.begin(), clonedBattles.end(), {originalBattle});
  }

  return entityMap;
}

namespace internal {
void cloneEntity(
  types::entity src, types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (auto [id, storage] : registry.storage()) {
    if (storage.contains(src)) {
      ENTT_ASSERT(
        std::find(srcEntityStorages[id].begin(), srcEntityStorages[id].end(), src) == std::end(srcEntityStorages[id]),
        "Adding an entity twice here means an entity will be duplicated more than it should.");
      srcEntityStorages[id].push_back(src);
    }
  }

  auto& destinations = entityMap[src] = std::vector<types::entity>{cloneCount};
  registry.create(destinations.begin(), destinations.end());
}

void cloneBattle(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, sides, actionQueue] :
       registry.view<tags::CloneFrom, tags::Battle, Sides, ActionQueue>().each()) {
    for (auto side : sides.val) {
      registry.emplace<tags::CloneFrom>(side);
    }
    for (auto queueItem : actionQueue.val) {
      registry.emplace<tags::CloneFrom>(queueItem);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
  for (const auto [entity, currentAction] : registry.view<tags::CloneFrom, tags::Battle, CurrentAction>().each()) {
    registry.emplace<tags::CloneFrom>(currentAction.val);
  }
}

void cloneSide(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, team] : registry.view<tags::CloneFrom, tags::Side, Team>().each()) {
    for (auto pokemon : team.val) {
      registry.emplace<tags::CloneFrom>(pokemon);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneAction(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, SpeedSort>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void cloneCurrentActionMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, tags::CurrentActionMove>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void clonePokemon(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (const auto [entity, moveSlots] : registry.view<tags::CloneFrom, tags::Pokemon, MoveSlots>().each()) {
    for (auto move : moveSlots.val) {
      registry.emplace<tags::CloneFrom>(move);
    }

    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }

  for (const auto [entity, move] : registry.view<tags::CloneFrom, CurrentActionMove>().each()) {
    registry.emplace<tags::CloneFrom>(move.val);
  }
}

void cloneMove(
  types::registry& registry, types::ClonedEntityMap& entityMap,
  entt::dense_map<entt::id_type, std::vector<types::entity>>& srcEntityStorages, types::cloneIndex cloneCount) {
  for (types::entity entity : registry.view<tags::CloneFrom, MoveName, Pp>()) {
    cloneEntity(entity, registry, entityMap, srcEntityStorages, cloneCount);
  }
}

void remapEntity(types::entity& entity, const CloneTo& cloneTo, const types::ClonedEntityMap& entityMap) {
  ENTT_ASSERT(entityMap.contains(entity), "Source node was not loaded into the map");
  ENTT_ASSERT(entityMap.at(entity).size() > cloneTo.val, "More entities are trying to be copied to than were copied");
  entity = entityMap.at(entity)[cloneTo.val];
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
}  // namespace internal
}  // namespace pokesim
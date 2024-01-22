#include <PokeSim.hpp>
#include <Types/Entity.hpp>

namespace pokesim {
struct CopyFrom {};
struct CopyTo {
  std::uint16_t index;
};

void copy(
  types::entity src, types::registry& registry, entt::dense_map<types::entity, std::vector<types::entity>>& entityMap,
  std::uint16_t duplicates) {
  auto& list = entityMap[src] = std::vector<types::entity>{duplicates};
  for (std::uint16_t i = 0; i < duplicates; i++) {
    types::entity dest = list[i] = registry.create();
    registry.emplace<CopyTo>(dest, i);
    for (auto [id, storage] : registry.storage()) {
      if (storage.contains(src)) {
        storage.push(dest, storage.value(src));
      }
    }
  }
}

void cloneBattle(
  types::registry& registry, entt::dense_map<types::entity, std::vector<types::entity>>& entityMap,
  std::uint16_t duplicates) {
  for (const auto [entity, sides, actionQueue] : registry.view<CopyFrom, Sides, ActionQueue>().each()) {
    registry.emplace<CopyFrom>(sides.p1);
    registry.emplace<CopyFrom>(sides.p2);
    for (auto queueItem : actionQueue.actionQueue) registry.emplace<CopyFrom>(queueItem);

    copy(entity, registry, entityMap, duplicates);
  }
}

void updateSideEntity(
  types::registry& registry, const entt::dense_map<types::entity, std::vector<types::entity>>& entityMap) {
  for (auto [entity, copyTo, side] : registry.view<CopyTo, Side>().each()) {
    ENTT_ASSERT(entityMap.contains(side.side), "Source node was not loaded into the map");
    ENTT_ASSERT(
      entityMap.at(side.side).size() > copyTo.index,
      "More entities are trying to be copied to than were copied");
    side.side = entityMap.at(side.side)[copyTo.index];
  }
}
}  // namespace pokesim
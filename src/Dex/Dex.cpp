#include "Dex.hpp"

#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim {
entt::handle Dex::createEntry() {
  return {registry, registry.create()};
}

template <typename GetBuild, typename T>
void Dex::load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, GetBuild getBuild) {
  map.reserve(list.size());
  for (T listItem : list) {
    ENTT_ASSERT(!map.contains(listItem), "Shouldn't build data entries twice");
    map[listItem] = getBuild(listItem)(*this);
  }
}

void Dex::loadSpecies(const entt::dense_set<dex::Species>& speciesList) {
  load(speciesMap, speciesList, getSpeciesBuild);
}

void Dex::loadItems(const entt::dense_set<dex::Item>& itemsList) {
  load(itemsMap, itemsList, getItemBuild);
}

void Dex::loadMoves(const entt::dense_set<dex::Move>& moveList) {
  load(movesMap, moveList, getMoveBuild);
}

}  // namespace pokesim
#include "Pokedex.hpp"

#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim {
entt::handle Pokedex::createEntry() {
  return {registry, registry.create()};
}

template <typename GetBuild, typename T>
void Pokedex::load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, GetBuild getBuild) {
  map.reserve(list.size());
  for (T listItem : list) {
    ENTT_ASSERT(!map.contains(listItem), "Shouldn't build data entries twice");
    map[listItem] = getBuild(listItem)(*this);
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesList) {
  load(speciesMap, speciesList, getSpeciesBuild);
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemList) {
  load(itemsMap, itemList, getItemBuild);
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveList) {
  load(movesMap, moveList, getMoveBuild);
}

}  // namespace pokesim
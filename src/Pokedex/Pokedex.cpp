#include "Pokedex.hpp"

#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
entt::handle Pokedex::createEntry() {
  return {registry, registry.create()};
}

template <typename GetBuild, typename T>
void Pokedex::load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, GetBuild getBuild) {
  map.reserve(map.size() + list.size());
  for (T listItem : list) {
    ENTT_ASSERT(!map.contains(listItem), "Shouldn't build data entries twice");
    map[listItem] = getBuild(listItem)(*this);
  }
}

void Pokedex::loadSpecies(const entt::dense_set<dex::Species>& speciesSet) {
  load(speciesMap, speciesSet, getSpeciesBuild);
}

void Pokedex::loadItems(const entt::dense_set<dex::Item>& itemSet) {
  load(itemsMap, itemSet, getItemBuild);
}

void Pokedex::loadMoves(const entt::dense_set<dex::Move>& moveSet) {
  load(movesMap, moveSet, getMoveBuild);
}

}  // namespace pokesim
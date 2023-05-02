#include "Dex.hpp"

#include <entt/container/dense_set.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim {
entt::handle Dex::createEntry() {
  return {registry, registry.create()};
}

void Dex::loadSpecies(const entt::dense_set<dex::Species>& speciesList) {
  speciesMap.reserve(speciesList.size());
  for (dex::Species species : speciesList) {
    ENTT_ASSERT(!speciesMap.contains(species), "Shouldn't build data entries twice");
    speciesMap[species] = getSpeciesBuild(species)(*this);
  }
}
}  // namespace pokesim
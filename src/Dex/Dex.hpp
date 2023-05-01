#pragma once

#include <Types/headers.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Dex {
 private:
  entt::registry registry{};

  entt::dense_map<dex::Species, entt::entity> speciesMap{};
  entt::dense_map<dex::Item, entt::entity> itemsMap{};
  entt::dense_map<dex::Move, entt::entity> movesMap{};

  /*__inline__*/ static entt::entity (*getSpeciesBuild(dex::Species species))(Dex&);
  /*__inline__*/ static entt::entity (*getMoveBuild(dex::Move move))(Dex&);
  /*__inline__*/ static entt::entity (*getItemBuild(dex::Item item))(Dex&);

 public:
  const GameMechanic mechanics;

  Dex(GameMechanic mechanics_ = SCARLET_VIOLET) : mechanics(mechanics_) {}

  /*__inline__*/ entt::handle createEntry();

  /*__inline__*/ void loadSpecies(const entt::dense_set<dex::Species>& speciesList);

  template <typename... T>
  auto getSpeciesData(dex::Species species) const {
    return registry.get<T...>(speciesMap.at(species));
  }
};
}  // namespace pokesim
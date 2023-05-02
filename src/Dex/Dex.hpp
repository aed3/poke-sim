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

  template <typename GetBuild, typename T>
  /*__inline__*/ void load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, GetBuild getBuild);

  /*__inline__*/ static entt::entity (*getSpeciesBuild(dex::Species species))(Dex&);
  /*__inline__*/ static entt::entity (*getMoveBuild(dex::Move move))(Dex&);
  /*__inline__*/ static entt::entity (*getItemBuild(dex::Item item))(Dex&);

 public:
  const GameMechanic mechanics;

  Dex(GameMechanic mechanics_ = SCARLET_VIOLET) : mechanics(mechanics_) {}

  /*__inline__*/ entt::handle createEntry();

  /*__inline__*/ void loadSpecies(const entt::dense_set<dex::Species>& speciesList);
  /*__inline__*/ void loadItems(const entt::dense_set<dex::Item>& itemList);
  /*__inline__*/ void loadMoves(const entt::dense_set<dex::Move>& moveList);

  template <typename... T>
  auto getSpeciesData(dex::Species species) const {
    return registry.get<T...>(speciesMap.at(species));
  }

  template <typename... T>
  auto getItemData(dex::Item item) const {
    return registry.get<T...>(itemsMap.at(item));
  }

  template <typename... T>
  auto getMoveData(dex::Move move) const {
    return registry.get<T...>(movesMap.at(move));
  }
};
}  // namespace pokesim
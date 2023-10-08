#pragma once

#include <Types/headers.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Pokedex {
 private:
  entt::registry registry{};

  entt::dense_map<dex::Species, entt::entity> speciesMap{};
  entt::dense_map<dex::Item, entt::entity> itemsMap{};
  entt::dense_map<dex::Move, entt::entity> movesMap{};

  template <typename GetBuild, typename T>
  /*_inline_*/ void load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, GetBuild getBuild);

  /*_inline_*/ static entt::entity (*getSpeciesBuild(dex::Species species))(Pokedex&);
  /*_inline_*/ static entt::entity (*getMoveBuild(dex::Move move))(Pokedex&);
  /*_inline_*/ static entt::entity (*getItemBuild(dex::Item item))(Pokedex&);

 public:
  /**
   * @brief The data for the Pokedex will be based the this game's data.
   * For example, if this is set to DIAMOND_PEARL_GAME_MECHANICS, Clefable's data will list it as a Normal type, but if
   * it's set to BRILLIANT_DIAMOND_SHINING_PEARL_GAME_MECHANICS, Clefable will be listed as a Fairy type.
   */
  const GameMechanics mechanics;

  Pokedex(GameMechanics mechanics_) : mechanics(mechanics_) {}

  // Creates an entity to store a new Pokedex entry of any species, item, or move.
  /*_inline_*/ entt::handle createEntry();

  /**
   * @brief Calls the load functions for a set of species to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's species.
   *
   * @note Only call this once per species per Pokedex instance.
   */
  /*_inline_*/ void loadSpecies(const entt::dense_set<dex::Species>& speciesSet);

  /**
   * @brief Calls the load functions for a set of items to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's items.
   *
   * @note Only call this once per item per Pokedex instance.
   */
  /*_inline_*/ void loadItems(const entt::dense_set<dex::Item>& itemSet);

  /**
   * @brief Calls the load functions for a set of moves to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if one turn of a single battle is being simulated and both opposing Pokemon have 4
   * moves, then this function should only be called with a set of those 8 moves.
   *
   * @note Only call this once per move per Pokedex instance.
   */
  /*_inline_*/ void loadMoves(const entt::dense_set<dex::Move>& moveSet);

  /**
   * @brief Returns references to the given dex data components for a species
   *
   * @warning
   * Attempting to get a component from a species that doesn't own it or an unloaded species results in undefined
   * behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getSpeciesData(dex::Species species) const {
    return registry.get<T...>(speciesMap.at(species));
  }

  /**
   * @brief Returns references to the given dex data components for an item
   *
   * @warning
   * Attempting to get a component from an item that doesn't own it or an unloaded item results in undefined behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getItemData(dex::Item item) const {
    return registry.get<T...>(itemsMap.at(item));
  }

  /**
   * @brief Returns references to the given dex data components for a move
   *
   * @warning
   * Attempting to get a component from a move that doesn't own it or an unloaded item results in undefined behavior.
   *
   * @tparam T Type of data to retrieve. Valid types: TODO(aed3): Add in list
   */
  template <typename... T>
  auto getMoveData(dex::Move move) const {
    return registry.get<T...>(movesMap.at(move));
  }
};
}  // namespace pokesim
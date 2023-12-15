#pragma once

#include <Components/EntityHolders/MoveEffect.hpp>
#include <Types/Enums/headers.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/container/fwd.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {

/**
 * @brief Holds the data of each species, item, and move in a simulation.
 *
 * @details It is designed so only one instance of this class is needed while simulating a particular battle as the
 * entirety of all the move, item, and species, information that might be needed for a battle is known when it starts.
 * In theory, all the information within the game mechanics chosen or information for multiple battles could be loaded
 * into one Pokedex instance and used repeatedly, but that is not recommended as this class is optimized for instances
 * holding a small amount of data.
 */
class Pokedex {
 private:
  entt::registry registry{};

  entt::dense_map<dex::Species, entt::entity> speciesMap{};
  entt::dense_map<dex::Item, entt::entity> itemsMap{};
  entt::dense_map<dex::Move, entt::entity> movesMap{};

  template <typename Build, typename T>
  /*_inline_*/ void load(entt::dense_map<T, entt::entity>& map, const entt::dense_set<T>& list, Build build);

  /*_inline_*/ static entt::entity (*getSpeciesBuild(dex::Species species))(Pokedex&);
  /*_inline_*/ static entt::entity (*getMoveBuild(dex::Move move))(Pokedex&);
  /*_inline_*/ static entt::entity (*getItemBuild(dex::Item item))(Pokedex&);

  /*_inline_*/ entt::entity buildSpecies(dex::Species species);
  /*_inline_*/ entt::entity buildMove(dex::Move move);
  /*_inline_*/ entt::entity buildItem(dex::Item item);

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
   * instance is assigned to, so if one turn of a single battle is being simulated and the opposing Pokemon have 4
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
    // ENTT_ASSERT(registry.all_of<T...>(speciesMap.at(species)), "Species does not contain at least one of the
    // component types");
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

  template <typename... T>
  auto getEffectData(MoveEffect effect) const {
    return registry.get<T...>(effect.moveEffect);
  }

  template <typename... T>
  bool speciesHas(dex::Species species) const {
    return registry.all_of<T...>(speciesMap.at(species));
  }

  template <typename... T>
  bool itemHas(dex::Item item) const {
    return registry.all_of<T...>(itemsMap.at(item));
  }

  template <typename... T>
  bool moveHas(dex::Move move) const {
    return registry.all_of<T...>(movesMap.at(move));
  }

  template <typename... T>
  bool effectHas(MoveEffect effect) const {
    return registry.all_of<T...>(effect.moveEffect);
  }
};
}  // namespace pokesim
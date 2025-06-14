#pragma once

#include <Components/EntityHolders/MoveEffect.hpp>
#include <Config/Config.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Enums/headers.hpp>
#include <Types/Registry.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/container/fwd.hpp>
#include <entt/entity/registry.hpp>

#include "TypeChart.hpp"

namespace pokesim {
class Simulation;
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
  friend class Simulation;

 private:
  types::registry dexRegistry{};

  entt::dense_map<dex::Species, types::entity> speciesMap{};
  entt::dense_map<dex::Item, types::entity> itemsMap{};
  entt::dense_map<dex::Move, types::entity> movesMap{};
  entt::dense_map<dex::Ability, types::entity> abilitiesMap{};

  template <typename Build, typename T>
  void load(entt::dense_map<T, types::entity>& map, const entt::dense_set<T>& list, Build build);

  types::entity buildSpecies(dex::Species species, types::registry& registry, bool forActiveMove) const;
  types::entity buildMove(dex::Move move, types::registry& registry, bool forActiveMove) const;
  types::entity buildItem(dex::Item item, types::registry& registry, bool forActiveMove) const;
  types::entity buildAbility(dex::Ability ability, types::registry& registry, bool forActiveMove) const;

  /**
   * @brief The data for the Pokedex will be based the this game's data.
   * For example, if this is set to DIAMOND_PEARL_GAME_MECHANICS, Clefable's data will list it as a Normal type, but if
   * it's set to BRILLIANT_DIAMOND_SHINING_PEARL_GAME_MECHANICS, Clefable will be listed as a Fairy type.
   */
  GameMechanics mechanics;
  TypeChart constantTypeChart;

 private:
#ifdef POKESIM_DEBUG_CHECK_UTILITIES
  static inline entt::dense_map<const Pokedex*, entt::dense_set<const Simulation*>> attachedSimulations;
  static void attachSimulation(const Pokedex* pokedex, const Simulation* simulation) {
    for (const auto& [existingPokedex, simulations] : attachedSimulations) {
      POKESIM_REQUIRE(!simulations.contains(simulation), "Simulation was already assigned to Pokedex.");
    }
    attachedSimulations[pokedex].insert(simulation);
  }

  static void detachSimulation(const Pokedex* pokedex, const Simulation* simulation) {
    POKESIM_REQUIRE(attachedSimulations.contains(pokedex), "Detaching Simulation from a Pokedex that was never added.");
    auto& simulations = attachedSimulations[pokedex];
    POKESIM_REQUIRE(simulations.contains(simulation), "Detaching Simulation from a Pokedex it was never attached to.");
    simulations.erase(simulation);
    if (simulations.empty()) {
      attachedSimulations.erase(pokedex);
    }
  }

  static void checkIfDetached(const Pokedex* pokedex) {
    if (attachedSimulations.contains(pokedex)) {
      std::size_t simulationCount = attachedSimulations[pokedex].size();
      if (simulationCount) {
        POKESIM_REQUIRE_FAIL(std::to_string(simulationCount) + " simulations are still using this Pokedex.");
      }
      else {
        POKESIM_REQUIRE_FAIL("Pokedex still exist on attachment list despite having no simulations attached.");
      }
    }
  }

  static bool isPokedexAttachedToSimulation(const Pokedex* pokedex, const Simulation* simulation) {
    if (attachedSimulations.contains(pokedex)) {
      return attachedSimulations[pokedex].contains(simulation);
    }
    return false;
  }
#else
  static void attachSimulation(const Pokedex*, const Simulation*) {}
  static void detachSimulation(const Pokedex*, const Simulation*) {}
  static void checkIfDetached(const Pokedex*) {}
  static bool isPokedexAttachedToSimulation(const Pokedex*, const Simulation*) { return true; }
#endif

 public:
  Pokedex(GameMechanics mechanics_) : mechanics(mechanics_), constantTypeChart(mechanics_) {}
  ~Pokedex() { Pokedex::checkIfDetached(this); }

  /**
   * @brief Calls the load functions for a set of species to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's species.
   *
   * @note Only call this once per species per Pokedex instance.
   */
  void loadSpecies(const entt::dense_set<dex::Species>& speciesSet);

  /**
   * @brief Calls the load functions for a set of items to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's items.
   *
   * @note Only call this once per item per Pokedex instance.
   */
  void loadItems(const entt::dense_set<dex::Item>& itemSet);

  /**
   * @brief Calls the load functions for a set of moves to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if one turn of a single battle is being simulated and the opposing Pokemon have 4
   * moves, then this function should only be called with a set of those 8 moves.
   *
   * @note Only call this once per move per Pokedex instance.
   */
  void loadMoves(const entt::dense_set<dex::Move>& moveSet);

  /**
   * @brief Calls the load functions for a set of abilities to add their data to a Pokedex's storage.
   *
   * @details The Pokedex class is designed to store the minimum amount of data needed to run the simulation a Pokedex
   * instance is assigned to, so if a battle is being simulated where both sides have 6 Pokemon each, then this function
   * should only be called with a set of those 12 Pokemon's abilities.
   *
   * @note Only call this once per ability per Pokedex instance.
   */
  void loadAbilities(const entt::dense_set<dex::Ability>& abilitySet);

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
    // POKESIM_REQUIRE(registry.all_of<T...>(speciesMap.at(species)), "Species does not contain at least one of the
    // component types.");
    return dexRegistry.get<T...>(speciesMap.at(species));
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
    return dexRegistry.get<T...>(itemsMap.at(item));
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
    return dexRegistry.get<T...>(movesMap.at(move));
  }

  template <typename... T>
  auto getEffectData(MoveEffect effect) const {
    return dexRegistry.get<T...>(effect.val);
  }

  template <typename... T>
  bool speciesHas(dex::Species species) const {
    return dexRegistry.all_of<T...>(speciesMap.at(species));
  }

  template <typename... T>
  bool itemHas(dex::Item item) const {
    return dexRegistry.all_of<T...>(itemsMap.at(item));
  }

  template <typename... T>
  bool moveHas(dex::Move move) const {
    return dexRegistry.all_of<T...>(movesMap.at(move));
  }

  template <typename... T>
  bool effectHas(MoveEffect effect) const {
    return dexRegistry.all_of<T...>(effect.val);
  }

  constexpr const TypeChart& typeChart() const { return constantTypeChart; }

  types::entity buildActionMove(dex::Move move, types::registry& registry) const;
};
}  // namespace pokesim
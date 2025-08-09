#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EVsIVs.hpp>
#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/Current.hpp>
#include <Components/EntityHolders/FoeSide.hpp>
#include <Components/EntityHolders/MoveSlots.hpp>
#include <Components/EntityHolders/Pokemon.hpp>
#include <Components/EntityHolders/Side.hpp>
#include <Components/EntityHolders/Sides.hpp>
#include <Components/EntityHolders/Team.hpp>
#include <Components/ID.hpp>
#include <Components/Level.hpp>
#include <Components/Names/AbilityNames.hpp>
#include <Components/Names/MoveNames.hpp>
#include <Components/Names/SpeciesNames.hpp>
#include <Components/PP.hpp>
#include <Components/PlayerSide.hpp>
#include <Components/Probability.hpp>
#include <Components/RNGSeed.hpp>
#include <Components/SpeciesTypes.hpp>
#include <Components/Stats.hpp>
#include <Components/Tags/BattleTags.hpp>
#include <Components/Tags/PokemonTags.hpp>
#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/AssertComponentsEqual.hpp>
#include <cstddef>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/core/type_info.hpp>

#include "ArgumentChecks.hpp"

namespace pokesim::debug {
struct Checks {
  Checks(const Simulation& _simulation) : simulation(&_simulation), registry(&_simulation.registry) {}

 protected:
  const Simulation* simulation;
  const types::registry* registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;
  types::entityIndex initialEntityCount = 0U;

  template <typename T>
  bool has(types::entity entity) const {
    return registry->all_of<T>(entity);
  }

  void copyRemainingEntities() {
    for (types::entity entity : registry->view<types::entity>()) {
      if (!registry->orphan(entity)) {
        initialEntityCount++;
        if (originalToCopy.contains(entity)) {
          specificallyChecked.emplace(entity);
        }
        else {
          originalToCopy[entity] = createEntityCopy(entity, *registry, registryOnInput);
        }
      }
    }
  }

  void checkRemainingOutputs() const {
    for (auto [original, copy] : originalToCopy) {
      if (!specificallyChecked.contains(original)) {
        areEntitiesEqual(*registry, original, registryOnInput, copy);
      }
    }
  }

  types::entityIndex getFinalEntityCount() const {
    types::entityIndex finalEntityCount = 0U;
    for (types::entity entity : registry->view<types::entity>()) {
      if (!registry->orphan(entity)) {
        finalEntityCount++;
      }
    }
    return finalEntityCount;
  }

  void checkMoveSlot(types::entity moveEntity) const { pokesim::debug::checkMoveSlot(moveEntity, *registry); }
  void checkPokemon(types::entity pokemonEntity) const { pokesim::debug::checkPokemon(pokemonEntity, *registry); }
  void checkSide(types::entity sideEntity) const { pokesim::debug::checkSide(sideEntity, *registry); }
  void checkBattle(types::entity battleEntity) const { pokesim::debug::checkBattle(battleEntity, *registry); }
};
}  // namespace pokesim::debug

#else
namespace pokesim::debug {
struct Checks {};
}  // namespace pokesim::debug
#endif
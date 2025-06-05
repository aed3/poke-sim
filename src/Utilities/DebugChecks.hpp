#pragma once

#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Simulation/Simulation.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/AssertComponentsEqual.hpp>
#include <cstddef>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/core/type_info.hpp>

namespace pokesim::debug {
struct Checks {
  Checks(const Simulation& _simulation) : simulation(&_simulation), registry(&_simulation.registry) {}

 protected:
  const Simulation* simulation;
  const types::registry* registry;
  types::registry registryOnInput;
  entt::dense_map<types::entity, types::entity> originalToCopy;
  entt::dense_set<types::entity> specificallyChecked;
  types::cloneIndex initialEntityCount = 0;

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

  types::cloneIndex getFinalEntityCount() const {
    types::cloneIndex finalEntityCount = 0;
    for (types::entity entity : registry->view<types::entity>()) {
      if (!registry->orphan(entity)) {
        finalEntityCount++;
      }
    }
    return finalEntityCount;
  }
};
}  // namespace pokesim::debug

#else
namespace pokesim::debug {
struct Checks {};
}  // namespace pokesim::debug
#endif
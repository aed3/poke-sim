#ifndef NDEBUG

#include "DebugChecks.hpp"

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/Assert.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>
#include <entt/meta/resolve.hpp>
#include <vector>

namespace pokesim::debug {
types::entity createEntityCopy(types::entity entity, const types::registry& src, types::registry& dst) {
  types::entity dstEntity = dst.create();
  for (auto [id, storage] : src.storage()) {
    if (storage.contains(entity)) {
      entt::resolve(storage.type())
        .invoke(types::registry::MetaFunctions::COPY_TO_OTHER_REGISTRY, {}, &src, entity, &dst, dstEntity);
    }
  }
  return dstEntity;
}

void hasSameComponents(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore) {
  for (auto [id, currStorage] : currReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (currStorage.contains(currEntity)) {
      const auto* const initStorage = initReg.storage(id);
      POKESIM_ASSERT(initStorage != nullptr, "The inital registry never contained this component.");
      POKESIM_ASSERT(initStorage->contains(initEntity), "The inital doesn't contain the current's component.");
    }
  }

  for (auto [id, initStorage] : initReg.storage()) {
    if (initStorage.contains(initEntity)) {
      const auto* const currStorage = currReg.storage(id);
      POKESIM_ASSERT(currStorage != nullptr, "The current registry never contained this component.");
      POKESIM_ASSERT(currStorage->contains(currEntity), "The current doesn't contain the inital's component.");
    }
  }
}

void areEntitiesEqual(
  const types::registry& currReg, types::entity currEntity, const types::registry& initReg, types::entity initEntity,
  const TypesToIgnore& typesToIgnore) {
  hasSameComponents(currReg, currEntity, initReg, initEntity, typesToIgnore);
  for (auto [id, storage] : currReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (storage.contains(currEntity)) {
      entt::resolve(storage.type())
        .invoke(
          types::registry::MetaFunctions::ENTITY_COMPONENTS_EQUAL,
          {},
          &currReg,
          currEntity,
          &initReg,
          initEntity);
    }
  }
}

types::entity findCopyParent(
  const entt::dense_map<types::entity, types::entity>& initialEntities, const types::registry& registry,
  types::entity entity) {
  if (initialEntities.contains(entity)) {
    return entity;
  }

  const ParentEntity* parentEntity = registry.try_get<ParentEntity>(entity);
  for (std::size_t i = 0; parentEntity != nullptr; i++) {
    if (i >= registry.storage<types::registry::entity_type>()->size()) {
      POKESIM_ASSERT_FAIL("A loop in the battle tree caused an infinite loop.");
    }

    for (auto [original, _] : initialEntities) {
      if (original == parentEntity->val) {
        return original;
      }
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  POKESIM_ASSERT_FAIL("Could not find original entity of a clone.");
}

bool checkIfCopyParent(types::entity current, types::entity initial, const types::registry& registry) {
  const ParentEntity* parentEntity = registry.try_get<ParentEntity>(current);
  for (std::size_t i = 0; parentEntity != nullptr; i++) {
    if (i >= registry.storage<types::registry::entity_type>()->size()) {
      POKESIM_ASSERT_FAIL("A loop in the battle tree caused an infinite loop.");
    }

    if (parentEntity->val == initial) {
      return true;
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  return false;
}
}  // namespace pokesim::debug

#endif
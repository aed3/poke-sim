#include <Config/Config.hpp>

#ifdef POKESIM_DEBUG_CHECK_UTILITIES

#include <Components/EntityHolders/Battle.hpp>
#include <Components/EntityHolders/BattleTree.hpp>
#include <Config/Require.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <cstddef>
#include <entt/container/dense_map.hpp>
#include <entt/container/dense_set.hpp>
#include <entt/entity/registry.hpp>
#include <entt/meta/resolve.hpp>
#include <vector>

#include "AssertComponentsEqual.hpp"

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
      std::string typeName{currStorage.type().name()};
      const auto* const initStorage = initReg.storage(id);
      POKESIM_REQUIRE(initStorage != nullptr, "The inital registry never contained this component: " + typeName);
      POKESIM_REQUIRE(
        initStorage->contains(initEntity),
        "The inital registry doesn't contain the current's component: " + typeName);
    }
  }

  for (auto [id, initStorage] : initReg.storage()) {
    if (typesToIgnore.contains(id)) continue;

    if (initStorage.contains(initEntity)) {
      std::string typeName{initStorage.type().name()};
      const auto* const currStorage = currReg.storage(id);
      POKESIM_REQUIRE(currStorage != nullptr, "The current registry never contained this component: " + typeName);
      POKESIM_REQUIRE(
        currStorage->contains(currEntity),
        "The current registry doesn't contain the inital's component: " + typeName);
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
      POKESIM_REQUIRE_FAIL("A loop in the battle tree caused an infinite loop.");
    }

    for (auto [original, _] : initialEntities) {
      if (original == parentEntity->val) {
        return original;
      }
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  POKESIM_REQUIRE_FAIL("Could not find original entity of a clone.");
}

bool checkIfCopyParent(types::entity potentialChild, types::entity potentialParent, const types::registry& registry) {
  const ParentEntity* parentEntity = registry.try_get<ParentEntity>(potentialChild);
  for (std::size_t i = 0; parentEntity != nullptr; i++) {
    if (i >= registry.storage<types::registry::entity_type>()->size()) {
      POKESIM_REQUIRE_FAIL("A loop in the battle tree caused an infinite loop.");
    }

    if (parentEntity->val == potentialParent) {
      return true;
    }
    parentEntity = registry.try_get<ParentEntity>(parentEntity->val);
  }

  return false;
}
}  // namespace pokesim::debug

#endif
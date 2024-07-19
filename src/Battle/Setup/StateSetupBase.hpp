#pragma once

#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
/**
 * @brief Class that holds the shared methods used between the various battle state setup tools.
 *
 * It is intended to be extended by classes that set up the state of a battle, such as StateSetupSide.
 */
struct StateSetupBase {
 protected:
  types::handle handle;

 public:
  StateSetupBase(types::registry& registry, types::entity entity) : handle(registry, entity) {}

  /**
   * @brief Sets a property on the entity.
   *
   * @tparam Tag The tag type of the property to set.
   */
  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  types::entity entity() const { return handle.entity(); }
};

}  // namespace pokesim::internal
#pragma once

#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim::internal {
struct StateSetupBase {
 protected:
  entt::handle handle;

 public:
  StateSetupBase(entt::registry& registry, entt::entity entity) : handle(registry, entity) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  entt::entity entity() { return handle; }
};

}  // namespace pokesim::internal
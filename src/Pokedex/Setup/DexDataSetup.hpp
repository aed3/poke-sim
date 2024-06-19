#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Utilities/Tags.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  types::handle handle;

 public:
  DexDataSetup(types::registry& registry) : handle(registry, registry.create()) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  template <typename... T>
  void setProperties(Tags<T...>) {
    (handle.emplace<T>(), ...);
  }

  types::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::dex::internal
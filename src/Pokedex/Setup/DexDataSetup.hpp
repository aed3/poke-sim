#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Utilities/Tags.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::internal::dex {
struct DexDataSetup {
 protected:
  types::handle handle;

 public:
  DexDataSetup(types::registry& registry) : handle(registry, registry.create()) {}
  DexDataSetup(types::registry& registry, types::entity entity) : handle(registry, entity) {}

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
}  // namespace pokesim::internal::dex

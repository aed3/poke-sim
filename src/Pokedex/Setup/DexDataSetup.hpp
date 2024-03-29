#pragma once

#include <Pokedex/Pokedex.hpp>
#include <Types/Entity.hpp>
#include <entt/entity/handle.hpp>

#include "DexDataTags.hpp"

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  types::handle handle;

 public:
  DexDataSetup(Pokedex* pokedex) : handle(pokedex->createEntry()) {}

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
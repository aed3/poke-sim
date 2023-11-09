#pragma once

#include <Pokedex/Pokedex.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/handle.hpp>

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  entt::handle handle;

 public:
  DexDataSetup(Pokedex& pokedex) : handle(pokedex.createEntry()) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }

  entt::entity entity() { return handle.entity(); }
};
}  // namespace pokesim::dex::internal
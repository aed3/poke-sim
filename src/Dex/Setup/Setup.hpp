#pragma once

#include <Dex/Dex.hpp>

namespace pokesim::dex::internal {
struct DexDataSetup {
 protected:
  entt::handle handle;

 public:
  DexDataSetup(Dex& pokedex) : handle(pokedex.createEntry()) {}

  template <typename Tag>
  void setProperty() {
    handle.emplace<Tag>();
  }
};
}  // namespace pokesim::dex::internal
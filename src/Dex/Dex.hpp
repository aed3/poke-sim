#pragma once

#include <Types/headers.hpp>
#include <entt/container/dense_map.hpp>
#include <entt/entity/handle.hpp>
#include <entt/entity/registry.hpp>

namespace pokesim {
class Dex {
 private:
  entt::registry registry{};

 public:
  entt::dense_map<dex::Move, entt::entity> moves{};

  entt::handle createEntry() { return {registry, registry.create()}; }
};
}  // namespace pokesim
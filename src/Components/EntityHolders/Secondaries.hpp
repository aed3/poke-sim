#pragma once

#include <entt/entity/fwd.hpp>
#include <vector>

namespace pokesim {
struct Secondaries {
  std::vector<entt::entity> secondaries{};
};
}  // namespace pokesim
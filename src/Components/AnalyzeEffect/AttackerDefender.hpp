#pragma once

#include <entt/entity/entity.hpp>

namespace pokesim::analyze_effect {
struct Attacker {
  entt::entity attacker;
};

struct Defender {
  entt::entity defender;
};
}  // namespace pokesim::analyze_effect
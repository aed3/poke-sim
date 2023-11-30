#pragma once

#include <entt/entity/entity.hpp>

namespace pokesim::damage_calc {
struct Attacker {
  entt::entity attacker;
};

struct Defender {
  entt::entity defender;
};
}  // namespace pokesim::damage_calc
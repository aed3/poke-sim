#pragma once

#include <entt/entity/entity.hpp>

namespace pokesim::calc_damage {
struct Attacker {
  entt::entity attacker;
};

struct Defender {
  entt::entity defender;
};
}  // namespace pokesim::calc_damage
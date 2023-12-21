#pragma once

#include <Types/Entity.hpp>

namespace pokesim::calc_damage {
struct Attacker {
  types::entity attacker;
};

struct Defender {
  types::entity defender;
};
}  // namespace pokesim::calc_damage
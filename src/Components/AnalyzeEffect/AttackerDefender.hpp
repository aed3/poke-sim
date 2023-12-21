#pragma once

#include <Types/Entity.hpp>

namespace pokesim::analyze_effect {
struct Attacker {
  types::entity attacker;
};

struct Defender {
  types::entity defender;
};
}  // namespace pokesim::analyze_effect
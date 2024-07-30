#pragma once

#include <Components/Damage.hpp>
#include <Types/Damage.hpp>
#include <Types/Entity.hpp>
#include <Types/Move.hpp>
#include <Types/Stats.hpp>
#include <vector>

namespace pokesim {
class Simulation;

namespace simulate_turn {
struct TurnOutcomeBattles {
  std::vector<types::entity> val;
};
}  // namespace simulate_turn

namespace calc_damage {
struct MaxDamage {
  types::damage val = 0;
};

struct MinUsesUntilKo {
  types::damage val = 0;
};

struct AttackerHpRecovered {
  types::stat val = 0;
};

struct AttackerHpLost {
  types::stat val = 0;
};
}  // namespace calc_damage

namespace analyze_effect {
struct EffectMultiplier {
  float val = 1.0F;
};

struct MultipliedMaxDamage {
  types::damage val;
};

struct MultipliedDamageRolls : DamageRolls {};

struct MultipliedKoChance {
  float val = 1.0F;
};
}  // namespace analyze_effect
}  // namespace pokesim
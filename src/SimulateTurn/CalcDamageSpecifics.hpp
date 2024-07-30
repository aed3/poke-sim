#pragma once

#include <Types/Enums/DamageRollKind.hpp>

namespace pokesim {
class Simulation;
namespace simulate_turn {
void cloneFromDamageRolls(Simulation& simulation, DamageRollKind damageRollKind);
void setIfMoveCrits(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim
#pragma once

#include <Types/Enums/DamageRollKind.hpp>

namespace pokesim {
class Simulation;
namespace internal::simulate_turn {
void cloneFromDamageRolls(Simulation& simulation, DamageRollKind damageRollKind);
void setIfMoveCrits(Simulation& simulation);
}  // namespace internal::simulate_turn
}  // namespace pokesim

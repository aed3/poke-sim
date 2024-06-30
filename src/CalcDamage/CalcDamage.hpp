#pragma once

#include <Components/CalcDamage/Aliases.hpp>
#include <Types/Entity.hpp>

namespace pokesim {
class Simulation;

namespace calc_damage {
struct CritBoost;

namespace internal {
void assignCritChanceDivisor(types::handle moveHandle, CritBoost critBoost);
}  // namespace internal

void run(Simulation& simulation);
void clearRunVariables(Simulation& simulation);

void modifyDamageWithTypes(Simulation& simulation);
void getDamageRole(Simulation& simulation);

void setIfMoveCrits(Simulation& simulation);
void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim
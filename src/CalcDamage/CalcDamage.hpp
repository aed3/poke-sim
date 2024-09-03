#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/PlayerSideId.hpp>

namespace pokesim {
class Simulation;
struct Damage;

namespace calc_damage {
void run(Simulation& simulation);

void applyDamageRoll(Damage& damage, types::damageRoll damageRoll);
void applyAverageDamageRoll(Damage& damage);
void applyMinDamageRoll(Damage& damage);

void setDamageRollModifiers(Simulation& simulation);
template <typename SimulationTag>
void applyDamageRollsAndModifiers(Simulation& simulation);

void setIfMoveCrits(Simulation& simulation);
void getDamage(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim
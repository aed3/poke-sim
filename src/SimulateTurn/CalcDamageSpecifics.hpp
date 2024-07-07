#pragma once

#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>

namespace pokesim {
class Simulation;
namespace simulate_turn {
void getDamageRole(Simulation& simulation, PlayerSideId sideForDamageRolls);
void setIfMoveCrits(Simulation& simulation);
}  // namespace simulate_turn
}  // namespace pokesim
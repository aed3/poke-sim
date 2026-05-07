#pragma once

#include <Types/Damage.hpp>
#include <Types/Enums/DamageRollKind.hpp>
#include <Types/Enums/PlayerSideId.hpp>
#include <Types/Stats.hpp>

namespace pokesim {
class Simulation;
namespace calc_damage {
void run(Simulation& simulation);
}  // namespace calc_damage
}  // namespace pokesim

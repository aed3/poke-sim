#pragma once

#include <Components/RandomEventInputs.hpp>
#include <Types/Enums/BattleFormat.hpp>
#include <Types/Move.hpp>
#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;

void runMoveHitChecks(Simulation& simulation);
}  // namespace pokesim
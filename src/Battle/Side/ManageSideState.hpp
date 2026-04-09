#pragma once

#include <Types/Entity.hpp>
#include <Types/Registry.hpp>
#include <Types/State.hpp>

namespace pokesim {
class Simulation;

types::teamPositionIndex sidePokemonLeft(const types::registry& registry, types::entity sideEntity);
types::teamPositionIndex foeSidePokemonLeft(const types::registry& registry, types::entity sideEntity);
}  // namespace pokesim

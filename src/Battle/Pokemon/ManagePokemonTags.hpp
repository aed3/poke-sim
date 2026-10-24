#pragma once

#include <Types/Registry.hpp>

namespace pokesim {
class Simulation;

namespace internal {
void clearVolatileTags(Simulation& simulation);
bool hasUpdateEvent(types::handle pokemonHandle);
}  // namespace internal
}  // namespace pokesim

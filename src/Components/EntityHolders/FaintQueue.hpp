#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Types/FixedMemoryVector.hpp>

namespace pokesim {
// Contains the list of pokemon that will faint at the end of the current action.
struct FaintQueue {
  types::fixedMemoryVector<types::entity, Constants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim

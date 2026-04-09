#pragma once

#include <Types/Entity.hpp>
#include <Types/MechanicConstants.hpp>
#include <Utilities/MaxSizedVector.hpp>

namespace pokesim {
// Contains the list of pokemon that will faint at the end of the current action.
struct FaintQueue {
  internal::maxSizedVector<types::entity, MechanicConstants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim

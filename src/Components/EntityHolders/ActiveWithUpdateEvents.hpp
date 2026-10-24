#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Types/FixedMemoryVector.hpp>

namespace pokesim {
struct ActiveWithUpdateEvents {
  types::fixedMemoryVector<types::entity, Constants::ActivePokemon::MAX> val{};
};
}  // namespace pokesim

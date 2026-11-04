#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct ActiveWithUpdateEvents {
  types::activePokemon<types::entity> val{};
};
}  // namespace pokesim

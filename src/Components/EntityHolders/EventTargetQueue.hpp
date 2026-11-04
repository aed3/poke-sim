#pragma once

#include <Types/Constants.hpp>
#include <Types/Entity.hpp>
#include <Types/FixedMemoryVector.hpp>
#include <Types/State.hpp>

namespace pokesim {
struct EventTargetQueue {
  types::activePokemon<types::entity> val{};
};
}  // namespace pokesim
